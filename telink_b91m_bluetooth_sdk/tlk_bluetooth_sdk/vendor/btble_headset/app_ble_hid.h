/********************************************************************************************************
 * @file    app_ble_hid.h
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
#ifndef _APP_BLE_HID_H_
#define _APP_BLE_HID_H_

/**
 * @brief       Initialize the BLE HID service
 * @param       none
 * @return      none
 * @note        This function registers various BLE services including Battery Service (BAS),
 *              Device Information Service (DIS), Scan Parameters Service (ScPS) and HID service.
 *              It also sets up connection callbacks and key reporting functionality.
 */
void app_ble_hid_init(void);


#endif //_APP_BLE_HID_H_