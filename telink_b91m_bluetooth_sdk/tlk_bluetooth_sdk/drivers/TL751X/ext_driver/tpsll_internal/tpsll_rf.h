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
#ifndef DRIVERS_TL751X_DRIVER_EXT_TPSLL_RF_H_
#define DRIVERS_TL751X_DRIVER_EXT_TPSLL_RF_H_

#include "tl_common.h"
#include "lib/include/rf/rf_common.h"
#include "lib/include/rf/rf_dma.h"


#define reg_pdzb_ntl_cv_l                    REG_ADDR8(0xd4170300)
#define reg_pdzb_ntl_cv_h                    REG_ADDR8(0xd4170301)

#define TPSLL_RF_BLE_RF_PAYLOAD_LENGTH_OK(p) (p[5] <= (reg_rf_rxtmaxlen0 + (reg_rf_rxtmaxlen1 & FLD_RF_MAX_LEN) * 256))
#define TPSLL_RF_BLE_RF_PACKET_CRC_OK(p)     ((p[p[5] + 5 + 11] & 0x01) == 0x0)

#define RF_RX_SETTLE_TIME_US                 45 //Unit:us
#define RF_TX_SETTLE_TIME_US                 45 //Unit:us
#define RF_RX_FASTSETTLE_TIME_US             45 //Unit:us
#define RF_TX_FASTSETTLE_TIME_US             45 //Unit:us
#define RF_TX_RX_FASTSETTLE_TIME_US          45 //Unit:us
#define TPSLL_RF_TX_PATH_DELAY_US            10 //Unit:us
#define TPSLL_RF_RX_PATH_DELAY_US            5

#define RF_MAX_CHN_TOTAL_NUM                 40
#define NORMAL_FASTSETTLE_EN                 (0) // in onca chip, must be disable, onca doesn't have "fastsettle" concept.
#define NORMAL_REGS_VALUE_RECOVER_EN         (0)
#define TPSLL_LOW_POWER_EN                   (0) // todo
#define TPSLL_BBTIMER_EN                     (1)
#define TPSLL_TX_POWER_INDEX                 (RF_POWER_INDEX_P10p00dBm)
#define TPSLL_MAX_TX_POWER_INDEX             (RF_POWER_INDEX_P13p00dBm)
#define TPSLL_MIN_TX_POWER_INDEX             (RF_POWER_INDEX_N24p00dBm)

#define reg_rf_ll_txwait                     REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x0e)
#define reg_rf_ll_rxwait                     REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x06)
#define TPSLL_RF_RSSI_OFFSET                 (3)

#ifndef TPSLL_BASEBAND_DEBUG_PORT_EN
#define TPSLL_BASEBAND_DEBUG_PORT_EN         (0)
#endif

extern u8  *rf_get_chn_num_from_selected_map(void);
extern void rf_fastsettle_init(u8 tx_fastsettle_en, u8 rx_fastsettle_en);
extern void rf_bb_dbg_setting(u8 en);
extern signed char rf_tpsll_get_rssi(unsigned char raw_rssi);

__INLINE void rf_tpsll_set_rx_wait(u8 rx_wait_us)
{
    reg_rf_ll_rxwait_l = rx_wait_us;
}

__INLINE void rf_tpsll_set_tx_wait(u8 tx_wait_us)
{
    reg_rf_ll_txwait_l = tx_wait_us;
}

#endif /* DRIVERS_TL751X_DRIVER_EXT_TPSLL_RF_H_ */
