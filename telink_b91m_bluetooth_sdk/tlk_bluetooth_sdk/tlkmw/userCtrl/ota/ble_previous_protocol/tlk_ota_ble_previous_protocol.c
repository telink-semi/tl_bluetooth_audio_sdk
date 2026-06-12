/********************************************************************************************************
 * @file    tlk_ota_ble_previous_protocol.c
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
#include "drivers.h"
#include "tlk_ota_ble_previous_protocol.h"
#include "../tlk_ota_protocol_common.h"
#include "../tlk_ota_types.h"

/**
 * @brief      Receive and process OTA data in BLE previous protocol mode
 * @param[in]  taskID - The ID of the task that sent the data
 * @param[in]  pData - Pointer to the received data buffer
 * @param[in]  dataLen - Length of the received data
 * @param[in]  UserArg - User-defined argument pointer
 * @return     int - Status code, 0 for success
 */
int tlk_ota_ble_previous_protocol_recv_data(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)taskID;
    (void)pData;
    (void)dataLen;
    (void)UserArg;

    return 0;
}

/**
 * @brief      Initialize the BLE previous protocol OTA module
 * @param[in]  pInterface - Pointer to the NVDS OTA interface structure (not used in this implementation)
 * @return     int - Status code, OTA_NONE indicating success
 */
int tlk_ota_ble_previous_protocol_init(nvds_ota_Interface_t *pInterface)
{
    (void)pInterface;

    return OTA_NONE;
}
