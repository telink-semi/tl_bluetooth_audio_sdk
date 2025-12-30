/********************************************************************************************************
 * @file    app_key.h
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
#pragma once

/**
 * @brief  Register callback function for a specific key.
 *          Register corresponding vendor configuration callback function according to key ID.
 *          Key ID to vendor configuration mapping:
 *          - key_id 0-3: Corresponds to KEY1-KEY4 click events (vendor config 1-4)
 *          - key_id 4-7: Corresponds to KEY1-KEY4 double click events (vendor config 5-8)
 *
 * @param[in] key_id     Key ID, range 0-7
 *                       - 0: KEY1 click event callback (vendor config 1)
 *                       - 1: KEY2 click event callback (vendor config 2)
 *                       - 2: KEY3 click event callback (vendor config 3)
 *                       - 3: KEY4 click event callback (vendor config 4)
 *                       - 4: KEY1 double click event callback (vendor config 5)
 *                       - 5: KEY2 double click event callback (vendor config 6)
 *                       - 6: KEY3 double click event callback (vendor config 7)
 *                       - 7: KEY4 double click event callback (vendor config 8)
 * @param[in] callback   Callback function pointer, called when corresponding key event is triggered.
 *                       Pass NULL to clear the callback function.
 *
 * @note    Usage example:
 *          @code
 *          void my_key1_click_handler(void) {
 *              // Handle KEY1 click event
 *          }
 *          app_key_register_callback(0, my_key1_click_handler);
 *          @endcode
 */
void app_key_register_callback(uint8_t key_id, void (*callback)(void));
