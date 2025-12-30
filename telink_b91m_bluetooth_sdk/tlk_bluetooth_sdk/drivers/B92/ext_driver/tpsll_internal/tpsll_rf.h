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
#include "lib/include/rf.h"

/**********************************************************************************************************************
 * RF FLAG BEGIN:
 *********************************************************************************************************************/
#define RF_FLG_ALL_MSK                0xFFFFFFFFUL

#define RF_FLG_IQ_ID_FASTSETTLE_EN    (0x01UL << 0) // IQ fast settle en.
#define RF_FLG_IQ_NONID_FASTSETTLE_EN (0x01UL << 1)
#define RF_FLG_TP_ID_FASTSETTLE_EN    (0x01UL << 2) // TP fast settle en.
#define RF_FLG_TP_NONID_FASTSETTLE_EN (0x01UL << 3)
#define RF_FLG_IQ_TP_SWITCH_EN        (0x01UL << 4)
#define RF_FLG_IQ_ONLY_EN             (0x01UL << 5)
#define RF_FLG_IQ_PWR_MANUAL_EN       (0x01UL << 6)
#define RF_FLG_PA_MANUAL_EN           (0x01UL << 7)
#define RF_FLG_PA_AUTO_EN             (0x01UL << 8)
#define RF_FLG_TXPWR_ADJUST_EN        (0x01UL << 9)
#define RF_FLG_TXPWR_ADJUST_APP_EN    (0x01UL << 10)
#define RF_FLG_BTC_TWS_EN             (0x01UL << 11)

#define FLASH_SIZE_1M_CAL_ADD         0xdd000
#define FLASH_SIZE_2M_CAL_ADD         0x1dd000

#define FLASH_SIZE_1M_CAL_ADD_NM      0xFE000
#define FLASH_SIZE_2M_CAL_ADD_NM      0x1FE000

#define ZB_AHBADDR                    0x170000
#define BBADDR                        ZB_AHBADDR         //BB	0x80170000
#define BBLLADDR                      ZB_AHBADDR + 0x200 //BB_LL 0x80170200
#define MODEMADDR                     ZB_AHBADDR + 0x400 //TL_MODEM	0x80170400
#define RADIOADDR                     ZB_AHBADDR + 0x600 //TL_RADIO	0x80170600

extern u32  rf_flag;
extern void rf_flag_set(unsigned int flag, unsigned int en);

static inline u32 rf_flag_get(u32 flag)
{
    return (rf_flag & (flag));
}

#define reg_modem_sync_threshold_bt REG_ADDR8(0x17044f)
#define reg_modem_pdet_threshold    REG_ADDR8(0x1704a0)

enum
{
    FLD_MODEM_PDET_THRESHOLD = BIT_RNG(0, 5), //0x1c
};

/**********************************************************************************************************************
 * RF RADIO/MODEM SETTING BEGIN:
 *********************************************************************************************************************/

void btc_radio_modem_init(void);

void rf_tp_freq_offset_cal(void);

//#define DMA_RFRX_LEN_HW_INFO                 0                               // 826x: 8
//#define DMA_RFRX_OFFSET_HEADER               4                               // 826x: 12
//#define DMA_RFRX_OFFSET_RFLEN                5                               // 826x: 13
//#define DMA_RFRX_OFFSET_DATA                 6                               // 826x: 14
//
//#define DMA_RFRX_OFFSET_CRC24(p)             (p[DMA_RFRX_OFFSET_RFLEN] + 6)  //data len:3
//#define DMA_RFRX_OFFSET_TIME_STAMP(p)        (p[DMA_RFRX_OFFSET_RFLEN] + 9)  //data len:4
//#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)       (p[DMA_RFRX_OFFSET_RFLEN] + 13) //data len:2
//#define DMA_RFRX_OFFSET_RSSI(p)              (p[DMA_RFRX_OFFSET_RFLEN] + 15) //data len:1, signed

#define FIX_RF_DMA_REWRITE                   0                               /*B91 needs to 1*/

#define TPSLL_RF_BLE_RF_PAYLOAD_LENGTH_OK(p) (p[5] <= (reg_rf_rxtmaxlen0 + (reg_rf_rxtmaxlen1 & FLD_RF_MAX_LEN) * 256))
#define TPSLL_RF_BLE_RF_PACKET_CRC_OK(p)     ((p[p[5] + 5 + 11] & 0x01) == 0x0)

#define reg_rf_ll_txwait                     REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x0e)
#define reg_rf_ll_rxwait                     REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x06)

extern u8  *rf_get_chn_from_map(void);
extern u8  *rf_get_chn_num_from_selected_map(void);
extern void rf_set_tpsll_chn(u8 chn);
extern signed char rf_tpsll_get_rssi(unsigned char raw_rssi);


/* fast settle in manual mode(including normal mode and id mode)*/
#define RF_RX_SETTLE_TIME_US         80  //Unit:us
#define RF_TX_SETTLE_TIME_US         100 //Unit:us
#define RF_RX_FASTSETTLE_TIME_US     45  //Unit:us
#define RF_TX_FASTSETTLE_TIME_US     50  //Unit:us
#define TPSLL_RF_TX_PATH_DELAY_US    2   //Unit:us
#define TPSLL_RF_RX_PATH_DELAY_US    6   //Unit:us, error history value: 14us.

#define RF_TX_RX_FASTSETTLE_TIME_US  0
#define RF_MAX_CHN_TOTAL_NUM         40
#define NORMAL_FASTSETTLE_EN         (1)
#define NORMAL_REGS_VALUE_RECOVER_EN (1)

#define TPSLL_BASEBAND_DEBUG_PORT_EN (0)
#define TPSLL_BBTIMER_EN             (0)
#define TPSLL_LOW_POWER_EN           (0)
#define TPSLL_TX_POWER_INDEX         (RF_POWER_INDEX_P9p15dBm)
#define TPSLL_MAX_TX_POWER_INDEX     (RF_POWER_INDEX_P9p90dBm)
#define TPSLL_MIN_TX_POWER_INDEX     (RF_POWER_INDEX_N50dBm)

typedef struct
{
    u8 LDO_CAL_TRIM;    //0xea[5:0]
    u8 LDO_RXTXHF_TRIM; //0xee[5:0]
    u8 LDO_RXTXLF_TRIM; //0xee[7:6]  0xed[3:0]
    u8 LDO_PLL_TRIM;    //0xee[5:0]
    u8 LDO_VCO_TRIM;    //0xee[7:6]  0xef[3:0]
    u8 rsvd;
} rf_trx_ldo_trim_t;

typedef struct
{
    u16           hpmc_cal_tbl[RF_MAX_CHN_TOTAL_NUM];
    rf_ldo_trim_t ldo_trim;
#if RF_RX_SEC_FLT_CONFIG
    rf_dcoc_cal_t dcoc_cal;
#endif
    u8            tx_en;
    u8            rx_en;
} rf_fastsettle_t;

extern rf_fastsettle_t rf_fastsettle;

extern void rf_bb_dbg_setting(u8 en);

/* for compatibility */
#define rf_tpsll_set_rx_wait   rf_ble_set_rx_wait
#define rf_tpsll_set_tx_wait   rf_ble_set_tx_wait
#define rf_dma_set_src_address dma_set_src_address
#define rf_ble_set_tx_rx_settle_time(x) \
    do {                                \
    } while (0)
#endif /* EXT_RF_H_ */
