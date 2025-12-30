/********************************************************************************************************
 * @file    ext_rf_fastsettle.h
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
#ifndef DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_FASTSETTLE_H_
#define DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_FASTSETTLE_H_
#include "tl_common.h"
#include "lib/include/rf.h"

/**********************************************************************************************************************
* RF NORMAL FASTSETTLE CODE BEGIN:
*********************************************************************************************************************/
/* Normal Fastsettle instructions: need to pay attention in the scene of "Low-power-mode" or "reset_baseband"
 * 1. Low-Power-Mode: B91 and B92 will lost RF Module Registers after low-power-mode.
 * 2. Reset_Baseband:
    - B91: if you do reset_baseband in b91, because RF Module registers will lost after "reset_baseband".
    - B92: reset_baseband will not lost RF Module Registers.

 * To consider the worst case scenario and reduce maintenance complexity,
 * it is necessary to do the following again after "low-power-mode" or "reset-baseband" in the beginning of the task:
 * - recover fast stl sequence: rf_normal_tx_fast_settle_init(); / rf_normal_tx_fast_settle_init();
 * - recover fast stl enable(or disable): rf_normal_tx_fast_settle_en(); / rf_normal_rx_fast_settle_en();
 * - recover (ldot/dcoc/hpmc) caliabration:
 *      - rf_set_normal_ldo_trim_val();
 *      - rf_set_normal_dcoc_cal_val();
 *      - rf_set_hpmc_cal_val();
 */
typedef struct
{
    u8  tx_fast_en;
    u8  rx_fast_en;
    u16 rsvd;
    u16 hpmc_cal_tbl[40];
#if RF_RX_SEC_FLT_CONFIG
    rf_dcoc_cal_t dcoc_cal;
#endif
    rf_ldo_trim_t ldo_trim;
} __attribute__((aligned(4))) rf_normal_fastsettle_t;

typedef enum
{
    NORMAL_RX_SETTLE_TIME_45US =
        0, /**<  disable rx_ldo_trim and rx_dcoc calibration,reduce 44.5us of rx settle time.Receive for a period of time and then do a normal calibration. */
    NORMAL_RX_SETTLE_TIME_80US         = 1, /**<  disable rx_ldo_trim calibration,reduce 4.5us of rx settle time. Do a normal calibration at the beginning.*/
    NORMAL_DEFAULT_RX_SETTLE_TIME_85US = 2  /* restore default value for bt. */
} rf_normal_rx_fast_settle_time_e;

typedef enum
{
    NORMAL_TX_SETTLE_TIME_50US  = 0, /**<  disable tx_ldo_trim function and tx_hpmc,reduce 58us of tx settle time.After frequency hopping, a normal calibration must be done.*/
    NORMAL_TX_SETTLE_TIME_104US = 1, /**<  disable tx_ldo_trim function,reduce 4.5us of tx settle time. Do a normal calibration at the beginning.*/
    NORMAL_DEFAULT_TX_SETTLE_TIME_108US = 2,
} rf_normal_tx_fast_settle_time_e;

extern rf_normal_fastsettle_t rf_normal_fast_stl_ble_1m_phy;
extern rf_normal_fastsettle_t rf_normal_fast_stl_ble_2m_phy;

extern _attribute_data_retention_ u8 temp_rx_buff[];

void rf_normal_fastsettle_init(rf_mode_e ble_phy_mode, u8 tx_fast_en, u8 rx_fast_en);
void rf_normal_rx_fast_settle_init(rf_normal_rx_fast_settle_time_e rx_settle_us);
void rf_normal_tx_fast_settle_init(rf_normal_tx_fast_settle_time_e tx_settle_us);
void rf_normal_tx_fast_settle_en(void);
void rf_normal_tx_fast_settle_dis(void);
void rf_normal_rx_fast_settle_en(void);
void rf_normal_rx_fast_settle_dis(void);

void rf_set_normal_ldo_trim_val(rf_ldo_trim_t ldo_trim);

#if RF_RX_SEC_FLT_CONFIG
void rf_set_normal_dcoc_cal_val(rf_dcoc_cal_t dcoc_cal);
#else
void rf_set_normal_dcoc_cal_val(void);
#endif
/**
 * @brief       This function serves to set the rf hpmc value when rf fast settle open..
 * @param[in]   phyType  - 1:BLE RF 1M.
 *                         2:BLE RF 2M.
 * @param[in]   chn      - ble rf channel number.
 * @return      none.
 */
void rf_fast_settle_set_hpmc_by_phy(unsigned char phyType, unsigned char chn);

/**
 * @brief       This function serves to set the rf ldo and dcoc value when rf fast settle open..
 * @param[in]   phyType  - 1:BLE RF 1M.
 *                         2:BLE RF 2M.
 * @return      none.
 */
void rf_fast_settle_set_ldo_dcoc_by_phy(unsigned char phyType);

#endif //DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_FASTSETTLE_H_
