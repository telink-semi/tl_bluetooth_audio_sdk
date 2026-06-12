/********************************************************************************************************
 * @file    tlkmdi_tinySql.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"

#if (TLK_MW_TINYSQL_ENABLE)

/******************************************************************************
                           private code begin
******************************************************************************/
#ifndef TLKMDI_TINYSQL_SAVE_DELAY_TIME
#define TLKMDI_TINYSQL_SAVE_DELAY_TIME (5 * 1000 * 1000)
#endif

typedef struct
{
    uint8_t needSave;   // Flag indicating which disks need to be saved
    uint8_t saveEnable; // Flag to enable/disable saving
} tlkmdi_tinysql_t;

static tlkmdi_tinysql_t sTlkmdiTinysql          = {0};
static TlkApiTimer_t    sTlkmdiTinySqlSaveTimer = {0};

/**
 * @brief   Calculates the actual save address in flash.
 * @param[in] offset - The offset from the base address.
 * @return  The actual flash address for saving data.
 * @note    This function calculates the address based on the flash size.
 */
inline unsigned int tlkmdi_tinySql_getSaveAddr(unsigned int offset)
{
    return offset + tlkhal_flash_get_size() - 0x100000;
}

/**
 * @brief   Requests a save operation for a specific disk.
 * @param[in] whichDisk - The index of the disk that needs to be saved.
 * @note    This function sets the save flag and triggers the system task event.
 */
inline void tlkmdi_tinySql_requestSave(uint8_t whichDisk)
{
    sTlkmdiTinysql.needSave |= 1 << whichDisk;
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
}

/**
 * @brief   Checks if a save request exists for a specific disk.
 * @param[in] whichDisk - The index of the disk to check.
 * @return  true if a save request exists, false otherwise.
 */
static bool tlkmdi_tinySql_checkSaveRequest(uint8_t whichDisk)
{
    if (sTlkmdiTinysql.needSave & (1 << whichDisk)) {
        return true;
    }
    return false;
}

// Array of disk interfaces
const tinySqlDisk_t *const tlkmdi_tinySql_disks[tinySql_maxSaveIndex] = {
    [tinySql_userSettingsSaveIndex] = &tinySql_userSetting_disk,
#if TLK_STK_BT_ENABLE
    [tinySql_pairingDevicesSaveIndex] = &tinySql_pairingDevice_disk,
#endif
#if TLKBTP_CFG_PBAP_ENABLE
    [tinySql_pbapSaveIndex] = &tinySql_pbap_disk,
#endif
#if TLK_STK_BLE_ENABLE
    [tinySql_leSaveIndex] = &tinySql_le_disk,
#endif
};

/**
 * @brief       Timer callback function to save flash data.
 * @param[in]   pTimer    - Pointer to the timer handle.
 * @param[in]   userArg   - User-defined argument.
 * @return      none.
 */
static void tlkmdi_tinySql_saveTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    if (tlkmdi_tinySql_isRequestSave()) {
        tlkmdi_tinySql_save();
    }
}

/**
 * @brief       Handles flash saving requests.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_tinySql_flashHandler(void)
{
    if (tlkmdi_tinySql_isRequestSave() == false) {
        return;
    }
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &sTlkmdiTinySqlSaveTimer);
}

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief   Operates the mutex for TinySQL module.
 * @param[in] isLocked - true to lock the mutex, false to unlock.
 * @note    This function is used to ensure thread safety when accessing TinySQL data.
 */
void tlkmdi_tinySql_mutex_operate(uint8_t isLocked)
{
    if (isLocked) {
        tlksys_mutex_lock(TLKSYS_MUTEX_TINYSQL);
    } else {
        tlksys_mutex_unlock(TLKSYS_MUTEX_TINYSQL);
    }
}

/**
 * @brief   Initializes the TinySQL module.
 * @note    This function initializes all registered disk modules.
 */
void tlkmdi_tinySql_init(void)
{
    sTlkmdiTinysql.saveEnable = 1;
    for (int i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_disks[i] == NULL) {
            continue;
        }
        tlkmdi_tinySql_disks[i]->init();
    }
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkmdiTinySqlSaveTimer, TLKMDI_TINYSQL_SAVE_DELAY_TIME, false, tlkmdi_tinySql_saveTimer, NULL);
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE, tlkmdi_tinySql_flashHandler);
}

/**
 * @brief   Saves all pending data to flash.
 * @return  TLK_ENONE if successful, error code otherwise.
 * @note    This function saves data from all disk modules that have pending changes.
 */
int tlkmdi_tinySql_save(void)
{
    if (sTlkmdiTinysql.saveEnable == 0) {
        return -TLK_ENOREADY;
    }
    if (sTlkmdiTinysql.needSave == 0) {
        return TLK_ENONE;
    }
    for (int i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_checkSaveRequest(i) == false) {
            continue;
        }
        tlksys_enter_critical();
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS, 1);
        if (tlkmdi_tinySql_disks[i] != NULL) {
            tlkmdi_tinySql_disks[i]->save();
        }
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS, 0);
        sTlkmdiTinysql.needSave &= ~(1 << i);
        tlksys_leave_critical();
        tlkapi_printf(TLKMDI_TINYSQL_LOG_ENABLE, "[SQL]<TRACE> disk:%d saved", i);
    }
    return TLK_ENONE;
}

/**
 * @brief   Restores all disk modules to factory settings.
 * @note    This function resets all data to default values.
 */
void tlkmdi_tinySql_restoreFactorySettings(void)
{
    for (int i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_disks[i] == NULL) {
            continue;
        }
        tlkmdi_tinySql_disks[i]->restoreFactory();
    }
}

/**
 * @brief   Checks if there is any pending save request.
 * @return  true if there is a pending save request, false otherwise.
 */
bool tlkmdi_tinySql_isRequestSave(void)
{
    if (sTlkmdiTinysql.needSave == 0) {
        return false;
    }
    return true;
}

/**
 * @brief   Enables or disables the saving functionality.
 * @param[in] en - 1 to enable saving, 0 to disable.
 * @note    When disabled, save requests will be ignored.
 */
void tlkmdi_tinySql_setSaveEnable(uint8_t en)
{
    sTlkmdiTinysql.saveEnable = en;
}

#endif // TLK_MW_TINYSQL_ENABLE