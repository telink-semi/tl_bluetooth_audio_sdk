/********************************************************************************************************
 * @file    app_ble_headset.h
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
#ifndef _APP_BLE_HEADSET_H_
#define _APP_BLE_HEADSET_H_

#define APP_BLE_HEADSET_LOG_EN 1

/**
 *  @brief  app ble headset event callback parameter
 */
struct app_ble_headset_event
{
    int event_id;

    void (*event_callback)(u16 connHandle, const void *pEvent);
};

/**
 * @brief       Initialize the BLE headset application
 * @param[in]   mac_public - Public MAC address of the device
 * @return      none
 */
void app_ble_headset_init(uint8_t mac_public[6]);

/**
 * @brief       Start BLE advertising for the headset
 * @param       none
 * @return      none
 */
void app_ble_headset_advertise_start(void);

/**
 * @brief       Stop BLE advertising for the headset
 * @param       none
 * @return      none
 */
void app_ble_headset_advertise_stop(void);


#endif