/********************************************************************************************************
 * @file    tpsll_rf.h
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
#ifndef _EXT_RF_H_
#define _EXT_RF_H_
#include "tl_common.h"
/* fast settle in manual mode(including normal mode and id mode)*/
#define RF_RX_SETTLE_TIME_US         80  //Unit:us
#define RF_TX_SETTLE_TIME_US         100 //Unit:us
#define RF_RX_FASTSETTLE_TIME_US     40  //Unit:us
#define RF_TX_FASTSETTLE_TIME_US     58  //Unit:us
#define RF_MAX_CHN_TOTAL_NUM         40

#define TPSLL_BBTIMER_EN             (0)
#define TPSLL_BASEBAND_DEBUG_PORT_EN (0)

// Tercel A2
#define TPSLL_TX_POWER_INDEX     (RF_POWER_INDEX_P11p00dBm) //(RF_POWER_INDEX_P10p00dBm)
#define TPSLL_MAX_TX_POWER_INDEX (RF_POWER_INDEX_P11p00dBm)
#define TPSLL_MIN_TX_POWER_INDEX (RF_POWER_INDEX_N49p00dBm)

extern u8         *rf_get_chn_num_from_selected_map(void);
extern void        rf_set_tpsll_chn(u8 chn);
extern void        rf_set_ble_sync_threshold(u8 thres_value);
extern void        rf_bb_dbg_setting(void);
extern void        rf_fastsettle_init(void);
extern signed char rf_tpsll_get_rssi(unsigned char raw_rssi);
extern void        rf_set_ble_2m_phy(void);
extern void        tlk_rf_init(void);
extern void        tlk_rf_init_recover(void);

/* for compatibility */
#define rf_tpsll_set_rx_wait rf_ble_set_rx_wait
#define rf_tpsll_set_tx_wait rf_ble_set_tx_wait

#endif /* EXT_RF_H_ */
