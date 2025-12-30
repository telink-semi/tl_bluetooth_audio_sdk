/********************************************************************************************************
 * @file    lea_unicast_client.h
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
#ifndef TLK_BLUETOOTH_SRC_B92_LEA_UNICAST_CLIENT_H
#define TLK_BLUETOOTH_SRC_B92_LEA_UNICAST_CLIENT_H

/**
 * @brief       Entry point to bring up LEA unicast client profile and start scanning.
 * @return      none.
 */
void lea_unicast_client_start(void);
#endif //TLK_BLUETOOTH_SRC_B92_LEA_UNICAST_CLIENT_H