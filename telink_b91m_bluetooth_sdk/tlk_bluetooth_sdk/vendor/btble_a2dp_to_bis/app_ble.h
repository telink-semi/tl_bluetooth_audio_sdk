/********************************************************************************************************
 * @file    app_ble.h
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
#ifndef VENDOR_APP_BLE_H_
#define VENDOR_APP_BLE_H_


/**
 * @brief       Initialize user BLE functionality
 * @param       none
 * @return      none
 */
void user_ble_init(void);

/**
 * @brief       Main loop for user BLE functionality
 * @param       none
 * @return      none
 */
void user_ble_main_loop(void);

/**
 * @brief       Enable extended advertising
 * @param       none
 * @return      0 if successful, error code otherwise
 */
int app_ble_enable_ext_adv(void);

/**
 * @brief       Disable extended advertising
 * @param       none
 * @return      0 if successful, error code otherwise
 */
int app_ble_disable_ext_adv(void);

#endif /* VENDOR_APP_BLE_H_ */