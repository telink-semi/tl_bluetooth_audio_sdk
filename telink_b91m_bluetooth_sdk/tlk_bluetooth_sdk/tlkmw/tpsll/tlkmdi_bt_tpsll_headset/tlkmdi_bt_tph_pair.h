/********************************************************************************************************
 * @file    tlkmdi_bt_tph_pair.h
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
#ifndef TLKMDI_BT_TPSLL_PAIR_H
#define TLKMDI_BT_TPSLL_PAIR_H

#define TLK_MDI_TPSLL_TPH_PAIRING_TIMEOUT 30     /* default 30 s*/
#define TLK_MDI_TPSLL_TIMEOUT             100000 // 100ms

/**
 * @brief       Initializes the Bluetooth pairing process.
 * @return      TLK_ENONE on success.
 * @note    
 */
int tlkmdi_bt_tph_pair_init(void);

/**
 * @brief       Handles the Bluetooth pairing process.
 * @return      TLK_ENONE on success.
 * @note    
 */
int tlkmdi_bt_tph_pairing_handler(void);

/**
 * @brief       Sync version to dongle.
 * @return      None
 */
void tlkmdi_tph_version_sync(void);
#endif
