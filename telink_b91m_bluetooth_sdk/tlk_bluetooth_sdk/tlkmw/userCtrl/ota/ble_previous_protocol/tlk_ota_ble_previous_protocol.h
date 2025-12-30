/********************************************************************************************************
 * @file    tlk_ota_ble_previous_protocol.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLK_OTA_BLE_PREVIOUS_PROTOCOL_H
#define TLK_OTA_BLE_PREVIOUS_PROTOCOL_H

#include "../tlk_ota_protocol_common.h"

/**
 * @brief      Initialize the BLE previous protocol OTA
 * @param[in]  pInterface - Pointer to the NVDS OTA interface
 * @param[out] none
 * @return     int - OTA_NONE
 */
int tlk_ota_ble_previous_protocol_init(nvds_ota_Interface_t *pInterface);

#endif // #ifndef TLK_OTA_BLE_PREVIOUS_PROTOCOL_H
