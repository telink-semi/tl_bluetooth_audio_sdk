/********************************************************************************************************
 * @file    ext_bredr_reg.h
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


#ifndef EXT_BREDR_REG_H
#define EXT_BREDR_REG_H
#include "reg_include/soc.h"

//use dlm exchange memory,repalce boot link file,qipeng
#define EM_DLM_EN 1

#if EM_DLM_EN
    #define EM_BASE_DRAM_OFFSET 0x00000
    #define DRAM_ADDR           0x00080000
#else
    #define EM_BASE_IRAM_OFFSET 0x00200
    #define IRAM_ADDR           0x00000000
#endif

#define reg_zb_mst_mod               REG_ADDR8(0x1401d2)
#define reg_tx_midx_bt_l             REG_ADDR8(0x17049c)
#define reg_tx_midx_bt_h             REG_ADDR8(0x17049d)

#define EFUSE_ADDR                   (0x1401c8)
#define EFUSE_B0                     (0x1401cc)
#define RST0                         (0x1401e0)
#define RSVD0                        (0x1401f4)

#define VCD_GPIO_ADDR                (0x140300)

#define BTCORE_BASE_ADDR             0x80160000
#define EDRCNTL0                     (0x160028)
#define RADIOCNTL00                  (0x160070)
#define RADIOCNTL10                  (0x160074)
#define RADIOCNTL20                  (0x160078)
#define RADIOCNTL30                  (0x16007c)
#define TIMGENCNTL0                  (0x1600e0)

#define TXMICVAL0      (0x801600c8)
#define RXMICVAL0      (0x801600cc)

#define TL_MODEM                     (0x80170400)
#define MODEM_TX_CTRL_0              (0x170480)
#define GUARD_ENABLE_0               (0x170484)
#define GUARD_ENABLE_1               (0x170485)
#define MODEM_TX_SCALE               (0x170491)
#define TL_RADIO                     (0x80170600)
#define MODE_CFG_TX2_0               (0x170624)
#define TXRX_CB_CAL_CTRL             (0x170684)
#define IDLE_TXFSK_SS1_SS2_STRT_CB_0 (0x170696)
#define IDLE_TXFSK_SS3_SS4_STRT_CB0  (0x170698)
#define IDLE_RX_SS1_SS2_STRT_CB_0    (0x17069c)
#define IDLE_RX_SS5_SS6_STRT_CB_0    (0x1706a0)
#define LDOT_DBG2_0                  (0x1706e4)
#define LDOT_DBG2_1                  (0x1706e5)
#define SFT_RST                      (0x17073a)
#define LNM_PA_0                     (0x17074c)
#define CBPF_ADC_0                   (0x17074e)
#define CBPF_ADC_1                   (0x17074f)
#define RF_NEW_CTRL0                 (0x170772)
#define RF_NEW_CTRL1                 (0x170773)
#define RF_NEW_CTRL2                 (0x170774)

#define MODEM_MODE_CFG_RX1_0         (0x170420)
#define BB_PDZB                      (0x80170300)
#define MODE_CFG_TXRX_0              (0x170626)
#define MODE_CFG_RX2_0               (0x17043e)
////////////used in btc_rf.c/////////////////

/* default value 0x00
 * for BLE: 0x00, for BT: 0xff */
#define reg_pdzb_ntl_cv REG_ADDR8(0x170300)

/* 0x170420 default value 0xc4, BIT(2) is 1, BIT(4) is 0, same as tbl_rf_1m
 * for BLE: 0xc4, for BT: 0xd0
 * core_0x170421 default value 0x00 */
#define reg_modem_mode_rx_mz REG_ADDR16(0x170420)

/* default value 0x0026
 * for BLE: 0x0126, for BT: 0x0026 */
#define reg_burst_cfg_txrx_mz REG_ADDR16(0x80170628)

/* default value 0x80
 * for BLE: 0x80, for BT: 0x00 */
#define reg_ct_dint_care_mz REG_ADDR8(0x80170679)

/* default value 0x80 */
#define reg_ntl_cv_mask          REG_ADDR32(0x80170300)

#define reg_modem_tx_tl_tctrl    REG_ADDR8(0x17049a)
#define reg_radio_tx_pa          REG_ADDR16(0x170626)
#define reg_radio_cfg_txrx       REG_ADDR32(0x17063c)
#define reg_radio_tx_pa_mode     REG_ADDR8(0x17063d)
#define reg_radio_modulation_idx REG_ADDR8(0x17063e)

/////////////used in btc_dbgport.h//////////////////////
#define reg_bt_dbg_sel REG_ADDR16(0x140350)

////////////analog register////////////////
#define afe1v_aon_reg09 (0x09)
#define afe1v_aon_reg11 (0x8b)


#define ZB_AHBADDR      0x170000
#define BBADDR          ZB_AHBADDR         //Baseband	0x80170000
#define RADIOADDR       ZB_AHBADDR + 0x600 //TL_RADIO	0x80170600
#define MODEMADDR       ZB_AHBADDR + 0x400 //TL_MODEM	0x80170400


/*******************************      MODEM registers: 0x80140c20      ******************************/
#define reg_modem_mode_rx REG_ADDR16(0x170420)

enum
{
    FLD_MODEM_LR_MODE            = BIT(0),
    FLD_MODEM_BLE_BT_AUTO_SEL_EN = BIT(1),
    FLD_MODEM_BLE_BT_SEL         = BIT(2), //0-BT,1-BLE
    FLD_MODEM_CONTINUOUS_MODE    = BIT(3),
    FLD_MODEM_NTL_CV             = BIT(4),
    FLD_MODEM_RX_DATA_CLK_DBG    = BIT(5),
    FLD_MODEM_LR_TRIGGER_MODE    = BIT(6),
    FLD_MODEM_FDC_DBG_SEL        = BIT(7),
};

/*******************************    Baseband PDZB registers:  0x80140c20 ******************************/
#define reg_pdzb_cv              REG_ADDR32(0x170300)
#define reg_pdzb_ntl_cv          REG_ADDR8(0x170300)
#define reg_pdzb_em_base_address REG_ADDR32(0x170304)

#define reg_modem_sync_thres_bt  REG_ADDR8(0x17044f)
#define reg_modem_pdet_thres     REG_ADDR8(0x1704a0)

enum
{
    FLD_MODEM_PDET_THRES = BIT_RNG(0, 5), //0x1c
};

enum
{
    RF_MODEM_BT  = 0,
    RF_MODEM_BLE = 1,
};

/*******************************    TL Radio registers:  0x80140e00 ******************************/
#define reg_burst_cfg_txrx REG_ADDR16(0x80170628)

enum
{
    FLD_BURST_CFG_CHANNEL_NUM        = BIT_RNG(0, 7),
    FLD_BURST_CFG_CHANNEL_NUM_LL_SEL = BIT(8),
    FLD_BURST_CFG_TX_EN_PIF          = BIT(9),
    FLD_BURST_CFG_RX_EN_PIF          = BIT(10),
    FLD_BURST_CFG_RX_TESQ            = BIT(11),
    FLD_BURST_CFG_TX_TESQ            = BIT(12),
    FLD_BURST_CFG_FE_CTRIM           = BIT_RNG(13, 15),
};

#define reg_chnl_freq_direct REG_ADDR8(0x80170644)

enum
{
    FLD_TXRX_DBG3_0_CHNL_FREQ_DIRECT = BIT(0),
};

#define reg_ct_dint_care REG_ADDR8(0x80170679)

enum
{
    FLD_CT_FE_CTRIM_SEL = BIT(7),
};

#define reg_mode_cfg_tx3 REG_ADDR16(0x8017063c)

enum
{
    FLD_MODE_CFG_TX3_TXC_PWR_SRL = BIT(12),
};

#define reg_modem_mode_trx1 REG_ADDR8(0x80170427)

enum
{
    FLD_TX_TAIL_DELAY_BLE = BIT_RNG(0, 3),
    FLD_TX_TAIL_DELAY_BT  = BIT_RNG(4, 7),
};

#define reg_radio_frequency REG_ADDR32(0x1706c0)

enum
{
    FLD_RADIO_FREQUENCY    = BIT_RNG(2, 9),
    FLD_RADIO_FREQ_READ_EN = BIT_RNG(26, 29),
};

#define reg_modem_tx_tl_tctrl    REG_ADDR8(0x17049a)
#define reg_radio_tx_pa          REG_ADDR16(0x170626)
#define reg_radio_cfg_txrx       REG_ADDR32(0x17063c)
#define reg_radio_tx_pa_mode     REG_ADDR8(0x17063d)
#define reg_radio_modulation_idx REG_ADDR8(0x17063e)


#endif /* EXT_BREDR_REG_H */
