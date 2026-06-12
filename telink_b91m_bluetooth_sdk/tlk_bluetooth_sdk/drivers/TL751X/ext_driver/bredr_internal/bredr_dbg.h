/********************************************************************************************************
 * @file    bredr_dbg.h
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
#ifndef BTC_DBG_H
#define BTC_DBG_H

#ifndef BT_DIAG_DEBUG_VCD
#if TLK_LOW_POWER_ENABLE || SDK_RELEASE_CHECK_EN
#define BT_DIAG_DEBUG_VCD 0
#else
#define BT_DIAG_DEBUG_VCD 0
#endif
#endif

//LOOPBACK or not
#define BT_BASEBAND_SIGNAL_LOOPBACK_VCD (1 && BT_DIAG_DEBUG_VCD)

#define DEBUG_PORT_TX_EN_PIN            GPIO_PA0
#define DEBUG_PORT_RX_EN_PIN            GPIO_PA1

//#define DEBUG_PORT_RX_SYNC_PIN          GPIO_PA3


/**
 * @brief		to enable some BT base_band RF signal output to GPIO
 * @param[in]	none
 * @return      none
 */
void btc_bb_dbg_init(void);


#endif /* BTC_DBG_H */
