/********************************************************************************************************
 * @file    ext_rf.h
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
#ifndef DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_
#define DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_

#include "../../lib/include/rf.h"
#include "common/types.h"

/******************************* ext_rf start ******************************************************************/
#ifndef FAST_SETTLE_NORMAL_MODE
#define FAST_SETTLE_NORMAL_MODE 1
#endif
#ifndef FAST_SETTLE
#define FAST_SETTLE 0
#endif

#define TX_FAST_SETTLE_LEVEL TX_SETTLE_TIME_50US
#define RX_FAST_SETTLE_LEVEL RX_SETTLE_TIME_45US

#define TX_FAST_SETTLE_TIME  50
#define RX_FAST_SETTLE_TIME  45

#ifndef BLE_S2_S8_NEW_PATH
#define BLE_S2_S8_NEW_PATH 0
#endif

#if !RF_RX_SEC_FLT_CONFIG
extern unsigned char  s_dcoc_software_cal_en;
extern unsigned short g_rf_dcoc_iq_code;
extern void           rf_rx_dcoc_cali_by_sw(void);                   //use extern to avoid modifying Driver rf.h
extern void           rf_set_dcoc_iq_code(unsigned short iq_code);   //use extern to avoid modifying Driver rf.h
extern void           rf_set_dcoc_iq_offset(signed short iq_offset); //use extern to avoid modifying Driver rf.h
#endif

/* tx power iq reg, for bt scale */
#define reg_modem_tx_scale  REG_ADDR8(0x170491)
#define FLAG_BT_TX_SCALE_EN BIT(5)
/* ble 1M 2M settings share some regs with bt mode, todo by mingqian 20240828. */
extern u8 blt_rf_mode_recover_from_bt;

#if 1 //Remain this till B91 driver update.
//#define reg_dma_rx_wptr           REG_ADDR8(0x801004f4)
#define reg_dma_tx_wptr REG_ADDR8(0x80100500) //rf_get_tx_wptr(0)

enum
{
    FLD_DMA_WPTR_MASK = BIT_RNG(0, 4),
};

#define reg_dma_rx_rptr REG_ADDR8(0x801004f5)
#define reg_dma_tx_rptr REG_ADDR8(0x80100501)

enum
{
    FLD_DMA_RPTR_MASK = BIT_RNG(0, 4),
    FLD_DMA_RPTR_SET  = BIT(5),
    FLD_DMA_RPTR_NEXT = BIT(6),
    FLD_DMA_RPTR_CLR  = BIT(7),
};
#endif

enum
{
    //BLE mode
    FLD_RF_BRX_SN_INIT   = BIT(4),
    FLD_RF_BRX_NESN_INIT = BIT(5),
    FLD_RF_BTX_SN_INIT   = BIT(6),
    FLD_RF_BTX_NESN_INIT = BIT(7),
};

//*********************Note: CRYPT*****************************/
enum
{
    FLD_TLK_CRYPT_ENABLE = BIT(1),
    FLD_TLK_MST_SLV      = BIT(2),
};

#define reg_rf_tlk_sk0  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa4)
#define reg_rf_tlk_sk1  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa8)
#define reg_rf_tlk_sk2  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xac)
#define reg_rf_tlk_sk3  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb0)

#define reg_tlk_sk(v)   REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa4 + (v * 4))


#define reg_rf_tlk_iv0  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb4)
#define reg_rf_tlk_iv1  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb8)

#define reg_rf_tlk_aad  REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xbc)

#define reg_ccm_control REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x3f)

enum
{
    FLD_R_TXLEN_FLAG = BIT(0),
};

//39 bits
#define reg_rf_tx_ccm_pkt_cnt0_31  REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x40)
#define reg_rf_tx_ccm_pkt_cnt32_38 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x44)

#define reg_rf_rx_ccm_pkt_cnt0_31  REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x48)
#define reg_rf_rx_ccm_pkt_cnt32_38 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x4c)

//*********************Note: must write and read one word*****************************/
#define REG_CV_LLBT_BASE_ADDR  (0x160000)

#define reg_cv_intcntl         REG_ADDR32(REG_CV_LLBT_BASE_ADDR + 0x0c)

#define reg_cv_llbt_rpase_cntl REG_ADDR32(REG_CV_LLBT_BASE_ADDR + 0x288)

enum
{
    FLD_CV_PRAND        = BIT_RNG(0, 23),
    FLD_CV_IRK_NUM      = BIT_RNG(24, 27),
    FLD_CV_GEN_RES      = BIT(29),
    FLD_CV_RPASE_START  = BIT(30),
    FLD_CV_PRASE_ENABLE = BIT(31),
};

#define reg_cv_llbt_hash_status REG_ADDR32(REG_CV_LLBT_BASE_ADDR + 0x28c)

enum
{
    FLD_CV_hash             = BIT_RNG(0, 23),
    FLD_CV_IRK_CNT          = BIT_RNG(24, 27),
    FLD_CV_RPASE_STATUS_CLR = BIT(28),
    FLD_CV_RPASE_STATUS     = BIT_RNG(29, 30),
    FLD_CV_HASH_MATCH       = BIT(31),
};

#define reg_cv_llbt_irk_ptr                       REG_ADDR32(REG_CV_LLBT_BASE_ADDR + 0x290)


#define DMA_RFRX_LEN_HW_INFO                      0 // 826x: 8
#define DMA_RFRX_OFFSET_HEADER                    4 // 826x: 12
#define DMA_RFRX_OFFSET_RFLEN                     5 // 826x: 13
#define DMA_RFRX_OFFSET_DATA                      6 // 826x: 14

#define RF_TX_PACKET_DMA_LEN(rf_data_len)         (((rf_data_len) + 3) / 4) | (((rf_data_len) % 4) << 22)
#define DMA_RFRX_OFFSET_CRC24(p)                  (p[DMA_RFRX_OFFSET_RFLEN] + 6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)             (p[DMA_RFRX_OFFSET_RFLEN] + 9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)            (p[DMA_RFRX_OFFSET_RFLEN] + 13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)                   (p[DMA_RFRX_OFFSET_RFLEN] + 15) //data len:1, signed
#define DMA_RFRX_OFFSET_STATUS(p)                 (p[DMA_RFRX_OFFSET_RFLEN] + 16)

#define RF_BLE_RF_PAYLOAD_LENGTH_OK(p)            (*((unsigned int *)p) == (unsigned int)(p[5] + 13)) //dma_len must 4 byte aligned
#define RF_BLE_RF_PACKET_CRC_OK(p)                ((p[(p[5] + 5 + 11)] & 0x01) == 0x0)
#define RF_BLE_PACKET_VALIDITY_CHECK(p)           (RF_BLE_RF_PAYLOAD_LENGTH_OK(p) && RF_BLE_RF_PACKET_CRC_OK(p))

#define RF_BLE_RF_PACKET_CRC_OKT_CRC_OK_HW_ECC(p) ((p[p[5] + 5 + 11 - 4] & 0x01) == 0x0)

#define rf_set_tx_packet_address(addr)            (dma_set_src_address(DMA0, convert_ram_addr_cpu2bus(addr)))


#ifndef RF_ACCESS_CODE_DEFAULT_THRESHOLD
#define RF_ACCESS_CODE_DEFAULT_THRESHOLD (31) //0x1e  . BQB may use 32. Coded PHY may use 0xF0
#endif


//RF BLE Minimum TX Power LVL (unit: 1dBm)
extern const char ble_rf_min_tx_pwr;
//RF BLE Maximum TX Power LVL (unit: 1dBm)
extern const char ble_rf_max_tx_pwr;
//RF BLE Current TX Path Compensation
extern signed short ble_rf_tx_path_comp;
//RF BLE Current RX Path Compensation
extern signed short ble_rf_rx_path_comp;
//Current RF RX DMA buffer point for BLE
extern unsigned char *ble_curr_rx_dma_buff;

typedef enum
{
    RF_ACC_CODE_TRIGGER_AUTO = BIT(0), /**< auto trigger */
    RF_ACC_CODE_TRIGGER_MANU = BIT(1), /**< manual trigger */
} rf_acc_trigger_mode;

extern signed char ble_txPowerLevel;

_attribute_ram_code_ void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16);

_attribute_ram_code_ void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char size_div_16);

_attribute_ram_code_ void ble_rx_dma_config(void);

/**
 * @brief   This function serves to settle adjust for RF Tx.This function for adjust the differ time
 *          when rx_dly enable.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
__INLINE void rf_ble_set_tx_settle(unsigned short txstl_us)
{
    REG_ADDR16(0x80170204) = txstl_us;
}

__INLINE unsigned short rf_read_tx_settle(void)
{
    return REG_ADDR16(0x80170204);
}

/* attention that not bigger than 4095 */
__INLINE void rf_ble_set_rx_settle(unsigned short rx_stl_us)
{
    write_reg16(0x17020c, rx_stl_us);
}

__INLINE void rf_ble_set_rx_wait(unsigned short rx_wait_us)
{
    write_reg8(0x170206, rx_wait_us);
}

__INLINE void rf_ble_set_tx_wait(unsigned short tx_wait_us)
{
    tx_wait_us = (tx_wait_us > 0x0fff ? 0x0fff : tx_wait_us);
    write_reg16(0x17020e, tx_wait_us);
}

/**
 * @brief       This function serves to update the value of internal cap.
 * @param[in]   value   - The value of internal cap which you want to set.
 * @return      none.
 */
__INLINE void rf_update_internal_capacitance(unsigned char value)
{
    /*
     * afe1v_reg10<5:0>     reg_xo_cdac_ana<5:0>        CDAC value (lowest cap to highest cap)
     * afe1v_reg10<6>       reg_xo_mode_ana             mode control - 0 : AMP_OFF, 1 : AMP_ON.
     *                                                  0 is to support dc coupling and 1 is to support ac coupling
     * afe1v_reg10<7>       reg_xo_cap_off_ana          control of X1 and X2 capacitance values
                                                        0 : cap follows CDAC, 1 : cap OFF
     */
    analog_write_reg8(0x8A, (analog_read_reg8(0x8A) & 0x40) | (value & 0x3f));
}

__INLINE void rf_ble_set_access_code_threshold(u8 threshold)
{
    write_reg8(0x17044e, threshold);
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
__INLINE void rf_set_ble_access_code_value(unsigned int ac)
{
    write_reg32(0x80170008, ac);
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
__INLINE void rf_set_ble_access_code(unsigned char *p)
{
    write_reg32(0x80170008, p[3] | (p[2] << 8) | (p[1] << 16) | (p[0] << 24));
}

/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
__INLINE void rf_set_ble_access_code_adv(void)
{
    write_reg32(0x80170008, 0xd6be898e);
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 *******************need driver change
 */
__INLINE void reset_sn_nesn(void)
{
    REG_ADDR8(0x80170201) = 0x01;
}

/**
 * @brief   This function serves to trigger accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
__INLINE void rf_trigger_codedPhy_accesscode(void)
{
    write_reg8(0x170425, read_reg8(0x170425) | 0x01);
}

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
__INLINE void rf_ble_tx_on(void)
{
    write_reg8(0x80170202, 0x45 | BIT(4)); // TX enable
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
__INLINE void rf_ble_tx_done(void)
{
    write_reg8(0x80170202, 0x45);
}

/**
 * @brief   This function serves to set RX first timeout value.
 * @param[in]   tm - timeout, unit: uS.
 * @return  none.
 */
__INLINE void rf_set_1st_rx_timeout(unsigned int tm)
{
    reg_rf_ll_rx_fst_timeout = tm;
}

/**
 * @brief   This function serves to set RX timeout value.
 * @param[in]   tm - timeout, unit: uS, range: 0 ~ 0xfff
 * @return  none.
 */
__INLINE void rf_ble_set_rx_timeout(u16 tm)
{
    reg_rf_rx_timeout = tm;
}

/**
 * @brief   This function serve to set the length of preamble for BLE packet.
 * @param[in]   len     -The value of preamble length.Set the register bit<0>~bit<4>.
 * @return      none
 */
__INLINE void rf_ble_set_preamble_len(u8 len)
{
    write_reg8(0x170002, (read_reg8(0x170002) & 0xe0) | (len & 0x1f));
}

__INLINE int rf_ble_get_preamble_len(void)
{
    return (read_reg8(0x170002) & 0x1f); //[4:0]: BLE preamble length
}

__INLINE void rf_set_dma_tx_addr(unsigned int src_addr) //Todo:need check by sunwei
{
    reg_dma_src_addr(DMA0) = convert_ram_addr_cpu2bus(src_addr);
}

typedef enum
{
    FSM_BTX   = 0x81,
    FSM_BRX   = 0x82,
    FSM_PTX   = 0x83,
    FSM_PRX   = 0x84,
    FSM_STX   = 0x85,
    FSM_SRX   = 0x86,
    FSM_TX2RX = 0x87,
    FSM_RX2TX = 0x88,
} fsm_mode_e;

/**
 * @brief       This function serves to RF trigger RF state machine.
 * @param[in]   mode  - FSM mode.
 * @param[in]   tx_addr  - DMA TX buffer, if not TX, must be "NULL"
 * @param[in]   tick  - FAM Trigger tick.
 * @return      none.
 */
void rf_start_fsm(fsm_mode_e mode, void *tx_addr, unsigned int tick);

/**
 * @brief      This function serves to reset baseband for BLE SDK
 * @return     none
 */
void ble_rf_reset_baseband(void);

/**
 * @brief       This function serves to set RF baseband channel.This function is suitable for ble open PN mode.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return      none.
 */
void rf_set_ble_channel(signed char chn_num);

/**
 * @brief     This function performs to switch PHY test mode.
 * @param[in] mode - PHY mode
 * @return    none.
 */
void rf_switchPhyTestMode(rf_mode_e mode);


/*
 * brief:If already know the DMA length value,this API can calculate the real RF length value that is easier for humans to understand.
 * param: dma_len -- the value calculated by this macro "rf_tx_packet_dma_len"
 * return: 0xFFFFFFFE --- error;
 *         other value--- success;
 */
u32 rf_cal_rfLenFromDmaLen(u32 dma_len);

//TODO: merge into driver
enum
{
    FLD_RF_SN = BIT(0),
};

/**
 * @brief    This function serves to enable zb_rt interrupt source.
 * @return  none
 */
__INLINE void zb_rt_irq_enable(void)
{
    plic_interrupt_enable(IRQ_ZB_RT);
}

#define RF_RX_WAIT_MIN_VALUE (0)
#define RF_TX_WAIT_MIN_VALUE (0)


/*
 * SiHui & QingHua & SunWei sync with Xuqiang.Zhang & Zhiwei.Wang & Kaixin.Chen & Shujuan.chu
 * B91/B92
 * TX settle recommend value by digital team: 108.5uS without fast settle;   108.5-58.5=50 with fast settle
 * we BLE use 110 without fast settle; 110-57=53 with fast settle, here 53 = real settle 45uS + extra 1 preamble 8uS(1M for example)
 *
 * RX settle recommend value by digital team: 108.5uS without fast settle;   85-40=45 with fast settle
 */

#if (FAST_SETTLE || FAST_SETTLE_NORMAL_MODE)
#define RX_SETTLE_US RX_FAST_SETTLE_TIME
#else
#define RX_SETTLE_US 85
#endif

#define RXSET_OPTM_ANTI_INTRF 100 //RX settle value for optimize anti-interference


#define PRMBL_LENGTH_1M       2  //preamble length for 1M PHY
#define PRMBL_LENGTH_2M       3  //preamble length for 2M PHY
#define PRMBL_LENGTH_Coded    10 //preamble length for Coded PHY, never change this value !!!

#define PRMBL_EXTRA_1M        (PRMBL_LENGTH_1M - 1) // 1 byte for 1M PHY
#define PRMBL_EXTRA_2M        (PRMBL_LENGTH_2M - 2) // 2 byte for 2M
#define PRMBL_EXTRA_Coded     0                     // 10byte for Coded, 80uS, no extra byte


#if RF_RX_SHORT_MODE_EN //open rx dly

#if (FAST_SETTLE || FAST_SETTLE_NORMAL_MODE)
#define TX_STL_LEGADV_SCANRSP_REAL TX_FAST_SETTLE_TIME //can change, consider TX packet quality
#else
#define TX_STL_LEGADV_SCANRSP_REAL 110 //can change, consider TX packet quality
#endif
#define TX_STL_LEGADV_SCANRSP_SET (TX_STL_LEGADV_SCANRSP_REAL - PRMBL_EXTRA_1M * 8) //can not change !!!


#if (FAST_SETTLE || FAST_SETTLE_NORMAL_MODE)
#define TX_STL_TIFS_REAL_COMMON TX_FAST_SETTLE_TIME //can change, consider TX packet quality
#else
#define TX_STL_TIFS_REAL_COMMON 110 //can change, consider TX packet quality
#endif

#define TX_STL_TIFS_REAL_1M    TX_STL_TIFS_REAL_COMMON                    //can not change !!!
#define TX_STL_TIFS_SET_1M     (TX_STL_TIFS_REAL_1M - PRMBL_EXTRA_1M * 8) //can not change !!!

#define TX_STL_TIFS_REAL_2M    TX_STL_TIFS_REAL_COMMON                    //can not change !!!
#define TX_STL_TIFS_SET_2M     (TX_STL_TIFS_REAL_2M - PRMBL_EXTRA_2M * 4) //can not change !!!

#define TX_STL_TIFS_REAL_CODED TX_STL_TIFS_REAL_COMMON //can not change !!!
#define TX_STL_TIFS_SET_CODED  TX_STL_TIFS_REAL_CODED  //can not change !!!


#if (FAST_SETTLE || FAST_SETTLE_NORMAL_MODE)
#define TX_STL_ADV_REAL_COMMON TX_FAST_SETTLE_TIME //can change, consider TX packet quality
#else
#define TX_STL_ADV_REAL_COMMON 110 //can change, consider TX packet quality
#endif
#define TX_STL_ADV_REAL_1M    TX_STL_ADV_REAL_COMMON
#define TX_STL_ADV_SET_1M     (TX_STL_ADV_REAL_1M - PRMBL_EXTRA_1M * 8) //can not change !!!

#define TX_STL_ADV_REAL_2M    TX_STL_ADV_REAL_COMMON
#define TX_STL_ADV_SET_2M     (TX_STL_ADV_REAL_2M - PRMBL_EXTRA_2M * 4) //can not change !!!

#define TX_STL_ADV_REAL_CODED TX_STL_ADV_REAL_COMMON
#define TX_STL_ADV_SET_CODED  TX_STL_ADV_REAL_CODED //can not change !!!

#if !RF_RX_SEC_FLT_CONFIG

//after enable DCOC, ACL T_IFS is about 152.5us, minus 2 us settle time by lihaojie at 2024-05-07
#define TX_STL_AUTO_MODE_1M (125 - PRMBL_EXTRA_1M * 8)
#define TX_STL_AUTO_MODE_2M (133 - PRMBL_EXTRA_2M * 4)
//after enable DCOC, ACL T_IFS is about 151.5us, minus 1 us settle time by lihaojie at 2024-05-07
#define TX_STL_AUTO_MODE_CODED 124

#else

#define TX_STL_AUTO_MODE_1M    (127 - PRMBL_EXTRA_1M * 8)
#define TX_STL_AUTO_MODE_2M    (133 - PRMBL_EXTRA_2M * 4)
#define TX_STL_AUTO_MODE_CODED 125

#endif


#if (FAST_SETTLE || FAST_SETTLE_NORMAL_MODE)
#define TX_STL_BTX_1ST_PKT_REAL TX_FAST_SETTLE_TIME //110 - 57 = 53
#else
/* normal mode(no fast settle): for ACL Central, tx settle real 110uS or 107uS or even 105uS, not big difference,
         * but for CIS Central, timing is very urgent considering T_MSS between two sub_event, so SiHui use 107, we keep this set
         * fast settle mode:  */
#define TX_STL_BTX_1ST_PKT_REAL (110 - 3) //3 is total switch delay time
#endif

#define TX_STL_BTX_1ST_PKT_SET_1M    (TX_STL_BTX_1ST_PKT_REAL - PRMBL_EXTRA_1M * 8)
#define TX_STL_BTX_1ST_PKT_SET_2M    (TX_STL_BTX_1ST_PKT_REAL - PRMBL_EXTRA_2M * 4)
#define TX_STL_BTX_1ST_PKT_SET_CODED TX_STL_BTX_1ST_PKT_REAL
#else
#error "add code here, TX settle time"
#endif


#if (1) //(LL_FEATURE_ENABLE_FRAME_SPACE_UPDATE)
#define RX_PATH_DLY_EXTRA_PREAMBLE_1M (150 - TX_STL_AUTO_MODE_1M)
#define RX_PATH_DLY_EXTRA_PREAMBLE_2M (150 - TX_STL_AUTO_MODE_2M)

#if (BLE_S2_S8_NEW_PATH)
#define RX_PATH_DLY_EXTRA_PREAMBLE_S2 (150 - TX_STL_AUTO_MODE_CODED_S2)
#define RX_PATH_DLY_EXTRA_PREAMBLE_S8 (150 - TX_STL_AUTO_MODE_CODED_S8)
#else
#define RX_PATH_DLY_EXTRA_PREAMBLE_CODED (150 - TX_STL_AUTO_MODE_CODED)
#endif
#endif

/* AD convert delay : timing cost on RF analog to digital convert signal process:
 *                  Eagle   1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 *                  Jaguar  1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 */
#define AD_CONVERT_DLY_1M    19 //before:20. Jaguar T_IFS need 32M + AD_Convert=19, tested by kai.jia at 2022-11-17
#define AD_CONVERT_DLY_2M    13
#define AD_CONVERT_DLY_CODED 16

#if !RF_RX_SEC_FLT_CONFIG
#define OTHER_SWITCH_DELAY_1M    2 //after enable DCOC, ADV T_IFS is about 152.5us, add 2 us delay by lihaojie at 2024-05-07
#define OTHER_SWITCH_DELAY_2M    0
#define OTHER_SWITCH_DELAY_CODED 2 //after enable DCOC, ADV T_IFS is about 154.5us, add 4 us delay by lihaojie at 2024-05-07

#else

#define OTHER_SWITCH_DELAY_1M    0
#define OTHER_SWITCH_DELAY_2M    0
#define OTHER_SWITCH_DELAY_CODED 0

#endif

#define HW_DELAY_1M    (AD_CONVERT_DLY_1M + OTHER_SWITCH_DELAY_1M)
#define HW_DELAY_2M    (AD_CONVERT_DLY_2M + OTHER_SWITCH_DELAY_2M)
#define HW_DELAY_CODED (AD_CONVERT_DLY_CODED + OTHER_SWITCH_DELAY_CODED)

/* Based on Driver API "rf_mode_init":
 * 1. Move to ram_code. __INLINE function called in ram_code function.
 * 2. Combine write_reg8 to write_reg16/32 to save instructions.
 * 3. Add Preamble length judgement. */
__INLINE void rf_mode_optimize_init(void)
{
    //To modify DCOC parameters
#if !RF_RX_SEC_FLT_CONFIG //todo: It is not good to put the whole block here in a staic inline function. But we put here currently. -- kai.jia 20240507
    extern void rf_sw_dcoc_cal(void);
    rf_sw_dcoc_cal();
#endif
    write_reg16(0x1706d2, 0x199b); //default:0x15bb;DCOC_SFIIP DCOC_SFQQP ,DCOC_SFQQ

    //Setting for blanking
#if RF_RX_SHORT_MODE_EN
    write_reg8(0x17047b, 0x0e); //default :0xf6;BLANK_WINDOW
    write_reg8(0x170479, 0x38); //BIT[3] RX_DIS_PDET_BLANK.BIT_RNG[4,5]SHORT MODE all mode open pdet blank to fix
                                //per floor issue.modified by zhiwei,confirmed by qiangkai and xuqiang.20221205
#else
    write_reg8(0x17047b, 0xfe);
    write_reg8(0x170479, 0x08); //RX_DIS_PDET_BLANK.BIT_RNG[4,5]SHORT MODE all mode open pdet blank to fix per floor
                                //issue.modified by zhiwei,confirmed by qiangkai and xuqiang.20221205
#endif

    //To set AGC thresholds
    write_reg16(0x17064a, 0x090e);     //default:0x0689;POW_000_001,POW_001_010_H
                                       //  write_reg16(0x17064e,0x0f09);       //default:0x0f09;POW_100_101 ,POW_101_100_L,POW_101_100_H;
    write_reg32(0x170654, 0x080c090e); //default:0x078c0689,POW_001_010_L,POW_001_010_H,POW_011_100_L,POW_011_100_H
                                       //  write_reg16(0x170658,0x0f09);       //default: 0x0f09;POW_101_100_L,POW_101_100_H
    //For optimum preamble detection
    write_reg16(0x170476, 0x7350); //default:0x7357;FREQ_CORR_CFG2_0,FREQ_CORR_CFG2_1
#if RF_RX_SHORT_MODE_EN
    write_reg16(0x17003a, 0x6586); //default:0x2d4e;rx_ant_offset  rx_dly(0x17047b,0x170479,0x17003a,0x17003b),samp_offset
#endif
    analog_write_reg8(0x8b, 0x04); //default:0x06;FREQ_CORR_CFG2_1

    //Note:Delete the modification here for 0x17074e, 0x17074c, 0x1706e4, 0x1706e5, and refer to the 8278
    //processing method for the 24M sensitivity problem.modified by zhiwei,confirmed by zhiwei.20230106

    write_reg8(0x170774, 0x97); //Change this setting from 0x96 to 0x97 to improve the tx power about 1dbm(A1,A2);
                                //modified by zhiwei,confirmed by wenfeng,20230106
}

/* Based on Driver API "rf_set_ble_1M_mode":
 * 1. Move to ram_code. __INLINE function called in ram_code function.
 * 2. Combine write_reg8 to write_reg16/32 to save instructions.
 * 3. Delete repeated settings from rf_module_init. */
__INLINE void rf_ble_set_1m_phy(void)
{
    write_reg8(0x17063d, 0x61);                             //default:0x61;ble:bw_code
    write_reg32(0x170620, 0x23200a16);                      //default:0x23200a16;sc_code,if_freq,IF = 1Mhz,BW = 1Mhz,HPMC_EXP_DIFF_COUNT_L,HPMC_EXP_DIFF_COUNT_H
                                                            //  write_reg8(0x17063f,0x00);          //default:0x00;250k modulation index:telink add rx for 250k/500k
                                                            //  write_reg8(0x17043f,0x00);          //default:0x00;LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k
    write_reg8(0x170420, 0x8c);                             // script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
                                                            //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
                                                            //by qiangkai and xuqiang.20221205
    write_reg8(0x170422, 0x00);                             //default:0x00;modem:BLE_MODE_TX,2MBPS
    write_reg8(0x17044d, 0x01);                             //default:0x01;r_rxchn_en_i:To modem
    write_reg8(0x17044e, RF_ACCESS_CODE_DEFAULT_THRESHOLD); //default:0x1e;ble sync threshold:To modem
                                                            //  write_reg8(0x170421,0x00);          //default:0x00;modem:ZIGBEE_MODE:01
                                                            //  write_reg8(0x170423,0x00);          //default:0x00;modem:ZIGBEE_MODE_TX
                                                            //  write_reg8(0x170426,0x00);          //default:0x00;modem:sync rst sel,for zigbee access code sync.
                                                            //  write_reg8(0x17042a,0x10);          //default:0x10;modem:disable MSK
                                                            //  write_reg8(0x17043d,0x00);          //default:0x00;modem:zb_sfd_frm_ll
                                                            //  write_reg8(0x17042c,0x38);          //default:0x38;modem:zb_dis_rst_pdet_isfd
    write_reg16(0x170436, 0x0eb7);                          //default:0x0eb7;LR_NUM_GEAR_L,LR_NUM_GEAR_H,
    write_reg16(0x170438, 0x71c4);                          //default:0x71c4;LR_TIM_EDGE_DEV,LR_TIM_REC_CFG_1
    write_reg8(0x170473, 0x01);                             //default:0x01;TOT_DEV_RST

    write_reg8(0x17049a, 0x00);        //default:0x08;tx_tp_align
    write_reg16(0x1704c2, 0x4b3a);     //default:0x4836;grx_0,grx_1
    write_reg32(0x1704c4, 0x7a6e6356); //default:0x796e6254;grx_2,grx_3,grx_4,grx_5
    write_reg8(0x1704c8, 0x39);        //default:0x00;bit[0:5]grx_fix

    write_reg32(0x170000, 0x4440081f | PRMBL_LENGTH_1M << 16); //default:0x4442001f;tx_mode,zb_pn_en,preamble set 2 for BLE,bit<0:1>private mode control. bit<2:3> tx mode

    write_reg16(0x170004, 0x04f5); //default:0x04f5; bit<4>mode:1->1m;bit<0:3>:ble head.ble whiting;lr mode bit<4:5> 0:off,3:125k,2:500k

    //  write_reg8(0x170021,0xa1);          //default:0xa1;rx packet len 0 enable
    //  write_reg8(0x170022,0x00);          //default:0x00;rxchn_man_en
    //  write_reg8(0x17044c,0x4c);          //default:0x4c;RX:acc_len modem

#if !RF_RX_SEC_FLT_CONFIG
        //All modes turn on the secondary filter to improve sensitivity performance.
    //But this will lead to a narrowing of the RX packet receiving bandwidth and a decrease in frequency offset performance.(Modified by chenxi,confirmed by xuqiang and yuya at 20231128)
    write_reg8(0x1704bb, 0x70); //rx ctrl1  0x50->0x70
                                //<5>:rxc_chf_sel_ble   default 0,->1 Turn on the secondary filter
#else
    write_reg8(0x1704bb, 0x50); //BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
                                //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
#endif
}

__INLINE void rf_ble_set_2m_phy(void)
{
    write_reg8(0x17063d, 0x41);
    write_reg32(0x170620, 0x26432a06);
    write_reg8(0x170420, 0x8c); // script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
                                //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
                                //by qiangkai and xuqiang.20221205
    write_reg8(0x170422, 0x01);

    //The initialization is already set, so it will not be set here. (Multi-mode todo)
    //ronglu.20230905
    //write_reg8(0x17044d,0x01);
    write_reg8(0x17044e, RF_ACCESS_CODE_DEFAULT_THRESHOLD);
    write_reg16(0x170436, 0x0eb7);
    write_reg16(0x170438, 0x71c4);
    write_reg8(0x170473, 0x01);

    //The initialization is already set, so it will not be set here. (Multi-mode todo)
    //ronglu.20230905
    //write_reg8(0x17049a,0x00);//tx_tp_align.

    write_reg16(0x1704c2, 0x4c3b);
    write_reg32(0x1704c4, 0x7b726359);
//The initialization is already set, so it will not be set here. (Multi-mode todo)
//ronglu.20230905
//write_reg8(0x1704c8,0x39);//bit[0:5]grx_fix
#if 1
    write_reg32(0x170000, 0x4440081f | PRMBL_LENGTH_2M << 16);
#else
    write_reg32(0x170000, 0x4446081f);
#endif

    write_reg16(0x170004, 0x04e5);

#if !RF_RX_SEC_FLT_CONFIG
    //All modes turn on the secondary filter to improve sensitivity performance.
    //But this will lead to a narrowing of the RX packet receiving bandwidth and a decrease in frequency offset performance.(Modified by chenxi,confirmed by xuqiang and yuya at 20231128)
    write_reg8(0x1704bb, 0x70); //BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
#else
    write_reg8(0x1704bb, 0x70); //BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
                                //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
#endif
}

__INLINE void rf_ble_set_coded_phy_common(void)
{
    write_reg8(0x17063d, 0x61);
    write_reg32(0x170620, 0x23200a16);
    write_reg8(0x170420, 0xcd); // script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
                                //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
                                //by qiangkai and xuqiang.20221205

    write_reg8(0x170422, 0x00);
    //The initialization is already set, so it will not be set here. (Multi-mode todo)
    //ronglu.20230905
    //write_reg8(0x17044d,0x01);
    write_reg8(0x17044e, 0xf0);
    write_reg16(0x170438, 0x7dc8);
    write_reg8(0x170473, 0xa1);

    //The initialization is already set, so it will not be set here. (Multi-mode todo)
    //ronglu.20230905
    //write_reg8(0x17049a,0x00);//tx_tp_align.

    write_reg16(0x1704c2, 0x4b3a);
    write_reg32(0x1704c4, 0x7a6e6356);
//The initialization is already set, so it will not be set here. (Multi-mode todo)
//ronglu.20230905
//write_reg8(0x1704c8,0x39);//bit[0:5]grx_fix
#if 1
    write_reg32(0x170000, 0x4440081f | PRMBL_LENGTH_Coded << 16);
#else
    write_reg32(0x170000, 0x444a081f);
#endif

#if !RF_RX_SEC_FLT_CONFIG
    //All modes turn on the secondary filter to improve sensitivity performance.
    //But this will lead to a narrowing of the RX packet receiving bandwidth and a decrease in frequency offset performance.(Modified by chenxi,confirmed by xuqiang and yuya at 20231128)
    write_reg8(0x1704bb, 0x70); //rx ctrl1  0x50->0x70
                                //<5>:rxc_chf_sel_ble   default 0,->1 Turn on the secondary filter
#else
    write_reg8(0x1704bb, 0x50); //BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
                                //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
#endif
}

__INLINE void rf_ble_set_coded_phy_s2(void)
{
    write_reg16(0x170436, 0x0cee);
    write_reg16(0x170004, 0xa4f5);
}

__INLINE void rf_ble_set_coded_phy_s8(void)
{
    write_reg16(0x170436, 0x0cf6);
    write_reg16(0x170004, 0xb4f5);
}

//This is to be compatible in older versions. If you don't use them, you can delete them.
#define rf_trigle_codedPhy_accesscode rf_trigger_codedPhy_accesscode


#if FAST_SETTLE
typedef struct
{
    unsigned short cal_tbl[81];
    rf_ldo_trim_t  ldo_trim;
    rf_rccal_cal_t rccal_cal;
    unsigned char  tx_fast_en;
    unsigned char  rx_fast_en;
} Fast_Settle;

extern Fast_Settle fast_settle_1M;
extern Fast_Settle fast_settle_2M;
extern Fast_Settle fast_settle_S2;
extern Fast_Settle fast_settle_S8;
#endif

/******************************* ext_rf end ********************************************************************/
/**********************************************************************************************************************
 * RF TX MODE SETTING BEGIN:
 *********************************************************************************************************************/
#define reg_modem_rx_ctrl1       REG_ADDR8(0x1704bb)
#define reg_rf_mode_cfg_tx1rx1   REG_ADDR32(0x170620)
#define reg_pdzb_em_base_address REG_ADDR32(0x170304)

typedef enum
{
    RADIO_TX_MODE_BLE = 1,
    RADIO_TX_MODE_IQ  = 2,
    RADIO_TX_MODE_TP  = 3
} Radio_TxModeDef;

extern unsigned char rf_mode_curr;

static inline void rf_tx_mode_reset(void)
{
    rf_mode_curr = 0xff;
}

_attribute_retention_code_ void rf_btble_change_radio_mode(Radio_TxModeDef mode);

_attribute_retention_code_ Radio_TxModeDef rf_get_BT_radio_mode(bool isIQ);

static inline void rf_set_em_base_address(uint32_t addr)
{
    reg_pdzb_em_base_address = addr;
}

__INLINE u8 rf_ble_get_tx_pwr_idx(s8 rfTxPower)
{
    rf_power_level_index_e rfPwrLvlIdx;

    /*VBAT*/
    if (rfTxPower >= 9) {
        rfPwrLvlIdx = RF_POWER_INDEX_P9p15dBm;
    } else if (rfTxPower >= 8) {
        rfPwrLvlIdx = RF_POWER_INDEX_P8p25dBm;
    } else if (rfTxPower >= 7) {
        rfPwrLvlIdx = RF_POWER_INDEX_P7p00dBm;
    } else if (rfTxPower >= 6) {
        rfPwrLvlIdx = RF_POWER_INDEX_P6p32dBm;
    }
    /*VANT*/
    else if (rfTxPower >= 5) {
        rfPwrLvlIdx = RF_POWER_INDEX_P5p21dBm;
    } else if (rfTxPower >= 4) {
        rfPwrLvlIdx = RF_POWER_INDEX_P4p02dBm;
    } else if (rfTxPower >= 3) {
        rfPwrLvlIdx = RF_POWER_INDEX_P3p00dBm;
    } else if (rfTxPower >= 2) {
        rfPwrLvlIdx = RF_POWER_INDEX_P2p01dBm;
    } else if (rfTxPower >= 1) {
        rfPwrLvlIdx = RF_POWER_INDEX_P1p03dBm;
    } else if (rfTxPower >= 0) {
        rfPwrLvlIdx = RF_POWER_INDEX_P0p01dBm;
    } else if (rfTxPower >= -4) {
        rfPwrLvlIdx = RF_POWER_INDEX_N3p95dBm;
    } else if (rfTxPower >= -6) {
        rfPwrLvlIdx = RF_POWER_INDEX_N5p94dBm;
    } else if (rfTxPower >= -10) {
        rfPwrLvlIdx = RF_POWER_INDEX_N9p03dBm;
    } else if (rfTxPower >= -14) {
        rfPwrLvlIdx = RF_POWER_INDEX_N13p42dBm;
    } else {
        rfPwrLvlIdx = RF_POWER_INDEX_N22p53dBm;
    }

    return rfPwrLvlIdx;
}

__INLINE s8 rf_ble_get_tx_pwr_level(rf_power_level_index_e rfPwrLvlIdx)
{
    s8 rfTxPower;

    /*VBAT*/
    if (rfPwrLvlIdx <= RF_POWER_INDEX_P9p15dBm) {
        rfTxPower = 9;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P8p25dBm) {
        rfTxPower = 8;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P7p00dBm) {
        rfTxPower = 7;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P6p32dBm) {
        rfTxPower = 6;
    }
    /*VANT*/
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P5p21dBm) {
        rfTxPower = 5;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P4p02dBm) {
        rfTxPower = 4;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P3p00dBm) {
        rfTxPower = 3;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P2p01dBm) {
        rfTxPower = 2;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P1p03dBm) {
        rfTxPower = 1;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P0p01dBm) {
        rfTxPower = 0;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N3p95dBm) {
        rfTxPower = -4;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N5p94dBm) {
        rfTxPower = -6;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N9p03dBm) {
        rfTxPower = -10;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N13p42dBm) {
        rfTxPower = -14;
    } else {
        rfTxPower = -23;
    }

    return rfTxPower;
}

signed char rf_ble_get_rf_actual_tx_pwr(signed char txPwr, bool needComp);

signed char rf_ble_get_inc_rf_tx_pwr(signed char reqPwr, signed char delta);

#endif /* DRIVERS_B92_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_ */
