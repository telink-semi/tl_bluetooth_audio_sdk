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
#include "tlkmdi_tinySql.h"
#include "tlkmdi_tinySql_inner.h"

#if (TLK_MW_TINYSQL_V2_ENABLE)

/******************************************************************************
                           private code begin
******************************************************************************/
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

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief   Initializes the TinySQL module.
 * @note    This function initializes all registered disk modules.
 */
void tlkmdi_tinySql_init(void)
{
    tlkmdi_tinySql_core_init();
    for (size_t i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_disks[i] == NULL || tlkmdi_tinySql_disks[i]->init == NULL) {
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
    tlkmdi_tinySql_cache_sync();
    return TLK_ENONE;
}

/**
 * @brief   Restores all disk modules to factory settings.
 * @note    This function resets all data to default values.
 */
void tlkmdi_tinySql_restoreFactorySettings(void)
{
    for (int i = 0; i < tinySql_maxSaveIndex; i++) {
        if (tlkmdi_tinySql_disks[i] == NULL || tlkmdi_tinySql_disks[i]->restoreFactory == NULL) {
            continue;
        }
        tlkmdi_tinySql_disks[i]->restoreFactory();
    }
}

/**
 * @brief   Enables or disables the saving functionality.
 * @param[in] en - 1 to enable saving, 0 to disable.
 * @note    When disabled, save requests will be ignored.
 */
void tlkmdi_tinySql_setSaveEnable(uint8_t en)
{
    tlkmdi_tinySql_core_save_en(en);
}

#endif // TLK_MW_TINYSQL_ENABLE