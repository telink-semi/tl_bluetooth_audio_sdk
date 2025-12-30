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

/**
 * @brief   Calculates the actual save address in flash.
 * @param[in] offset - The offset from the base address.
 * @return  The actual flash address for saving data.
 * @note    This function calculates the address based on the flash size.
 */
inline unsigned int tlkmdi_tinySql_getSaveAddr(unsigned int offset)
{
    extern unsigned int flash_full_size;
    return offset + flash_full_size - 0x100000;
}

static uint8_t sSqlIsNeedSaveToFlash = 0;  // Flag indicating which disks need to be saved
static uint8_t sSqlIsSaveEnable = 1;       // Flag to enable/disable saving
static uint8_t sSqlCritical = 0;           // Critical section counter

/**
 * @brief   Requests a save operation for a specific disk.
 * @param[in] whichDisk - The index of the disk that needs to be saved.
 * @note    This function sets the save flag and triggers the system task event.
 */
inline void tlkmdi_tinySql_requestSave(uint8_t whichDisk)
{
    sSqlIsNeedSaveToFlash |= 1 << whichDisk;
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
}

/**
 * @brief   Checks if a save request exists for a specific disk.
 * @param[in] whichDisk - The index of the disk to check.
 * @return  true if a save request exists, false otherwise.
 */
static bool tlkmdi_tinySql_checkSaveRequest(uint8_t whichDisk)
{
    if (sSqlIsNeedSaveToFlash & (1 << whichDisk)) {
        return true;
    }
    return false;
}

// Array of disk interfaces
const tinySqlDisk_t *const tlkmdi_tinySql_disks[tinySql_maxSaveIndex] = {
    [tinySql_macSaveIndex] = &tinySql_mac_disk,
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
    #if TLKALG_EQ_ENABLE
    [tinySql_audioSaveIndex] = &tinySql_audio_disk,
    #endif
};

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
    for (int i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_disks[i] == NULL) {
            continue;
        }
        tlkmdi_tinySql_disks[i]->init();
    }
}

/**
 * @brief   Saves all pending data to flash.
 * @return  TLK_ENONE if successful, error code otherwise.
 * @note    This function saves data from all disk modules that have pending changes.
 */
int tlkmdi_tinySql_save(void)
{
    if (sSqlIsSaveEnable == 0 || sSqlCritical) {
        return -TLK_ENOREADY;
    }
    if (sSqlIsNeedSaveToFlash == 0) {
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
        sSqlIsNeedSaveToFlash &= ~(1 << i);
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
    if (sSqlIsNeedSaveToFlash == 0) {
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
    sSqlIsSaveEnable = en;
}

/**
 * @brief   Suspends saving functionality.
 * @note    This function increments the critical section counter to prevent saving.
 */
void tlkmdi_tinySql_suspendSave(void)
{
    tlksys_enter_critical();
    sSqlCritical++;
    tlksys_leave_critical();
}

/**
 * @brief   Resumes saving functionality.
 * @note    This function decrements the critical section counter and may trigger a save if needed.
 */
void tlkmdi_tinySql_resumeSave(void)
{
    tlksys_enter_critical();
    sSqlCritical--;
    if (sSqlCritical == 0 && sSqlIsNeedSaveToFlash) {
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
    }
    tlksys_leave_critical();
}
#endif // TLK_MW_TINYSQL_ENABLE