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
#ifndef DRIVERS_TL751X_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_
#define DRIVERS_TL751X_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_

#include "types.h"
#include "lib/include/clic.h"
#include "lib/include/plic.h"


/* ble 1M 2M settings share some regs with bt mode, todo by mingqian 20240828. */
extern u8 blt_rf_mode_recover_from_bt;

/******************************* ext_rf start ******************************************************************/

#ifndef FAST_SETTLE
#define FAST_SETTLE 0
#endif


enum
{
    //BLE mode
    FLD_RF_BRX_SN_INIT   = BIT(4),
    FLD_RF_BRX_NESN_INIT = BIT(5),
    FLD_RF_BTX_SN_INIT   = BIT(6),
    FLD_RF_BTX_NESN_INIT = BIT(7),
};

#define DMA_RFRX_LEN_HW_INFO              0 // 826x: 8
#define DMA_RFRX_OFFSET_HEADER            4 // 826x: 12
#define DMA_RFRX_OFFSET_RFLEN             5 // 826x: 13
#define DMA_RFRX_OFFSET_DATA              6 // 826x: 14

#define RF_TX_PACKET_DMA_LEN(rf_data_len) (((rf_data_len) + 3) / 4) | (((rf_data_len) % 4) << 22)
#define DMA_RFRX_OFFSET_CRC24(p)          (p[DMA_RFRX_OFFSET_RFLEN] + 6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)     (p[DMA_RFRX_OFFSET_RFLEN] + 9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)    (p[DMA_RFRX_OFFSET_RFLEN] + 13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)           (p[DMA_RFRX_OFFSET_RFLEN] + 15) //data len:1, signed
#define DMA_RFRX_OFFSET_STATUS(p)         (p[DMA_RFRX_OFFSET_RFLEN] + 16)

#define RF_BLE_RF_PAYLOAD_LENGTH_OK(p)    (*((unsigned int *)p) == (unsigned int)(p[5] + 13)) //dma_len must 4 byte aligned
#define RF_BLE_RF_PACKET_CRC_OK(p)        (rf_ble_packet_crc_ok(p) && !(rf_get_crc_err()))
#define RF_BLE_PACKET_VALIDITY_CHECK(p)   (RF_BLE_RF_PAYLOAD_LENGTH_OK(p) && RF_BLE_RF_PACKET_CRC_OK(p))

#define RF_BLE_RF_PACKET_CRC_OK_HW_ECC(p) ((p[p[5] + 5 + 11 - 4] & 0x01) == 0x0)

#define rf_set_tx_packet_address(addr)    rf_dma_set_src_address(RF_TX_DMA, (unsigned int)(addr))

#define rf_set_rxpara()
#define rf_ldot_ldo_rxtxlf_bypass_en()
#define rf_ldot_ldo_rxtxlf_bypass_dis()

#ifndef RF_ACCESS_CODE_DEFAULT_THRESHOLD
#define RF_ACCESS_CODE_DEFAULT_THRESHOLD (31) //0x1e	. BQB may use 32. Coded PHY may use 0xF0
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

typedef enum
{
    RADIO_TX_MODE_BLE = 1,
    RADIO_TX_MODE_BT  = 2,
} Radio_TxModeDef;

extern signed char ble_txPowerLevel;

_attribute_ram_code_ void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16);

_attribute_ram_code_ void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char size_div_16);

_attribute_ram_code_ void ble_rx_dma_config(void);

void rf_ble_csem_set_tx_rx_settle(int adjust_tx_in_fsm, unsigned char tx_stl_us, unsigned char rx_stl_us);

extern unsigned char rf_mode_curr;

_attribute_retention_code_ void rf_btble_change_radio_mode(Radio_TxModeDef mode);

_attribute_retention_code_ Radio_TxModeDef rf_get_BT_radio_mode(bool isIQ);

/**
 * @brief   This function serves to settle adjust for RF Tx.This function for adjust the differ time
 * 			when rx_dly enable.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */


/**
 * @brief	  	This function serves to update the value of internal cap.
 * @param[in]  	value   - The value of internal cap which you want to set.
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

/**
 * @brief     This function performs to set RF Access Code Threshold.// use for BQB
 * @param[in] threshold   cans be 0-32bits
 * @return    none.
 */
__INLINE void rf_ble_set_access_code_threshold(u8 threshold)
{
    reg_rf_modem_sync_thres_ble = threshold;
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
__INLINE void rf_set_ble_access_code_value(unsigned int ac)
{
    reg_rf_access_code = ac;
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
__INLINE void rf_set_ble_access_code(unsigned char *p)
{
    reg_rf_access_code = p[3] | (p[2] << 8) | (p[1] << 16) | (p[0] << 24);
}

/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
__INLINE void rf_set_ble_access_code_adv(void)
{
    reg_rf_access_code = 0xd6be898e;
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 *******************need driver change
 */
__INLINE void reset_sn_nesn(void)
{
    reg_rf_ll_rest_pid = FLD_RF_R_RESET_PID_0;
}

/**
 * @brief   This function serves to trigger accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
__INLINE void rf_trigger_codedPhy_accesscode(void)
{
    // TL751X not need?
}

/**
 * @brief     This function performs to disable RF Rx continue mode.
 * @param[in] none.
 * @return    none.
 */
__INLINE void rf_ble_csem_close_rx_continue_mode(void)
{
    write_reg8(0xd4170028, 0x80);                              // rx disable
    write_reg8(0xd4170202, 0x45);                              // reset tx/rx state machine
    write_reg8(0xd417022b, read_reg8(0xd417022b) & (~BIT(7))); //close RX continue mode.
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
__INLINE void rf_ble_tx_done(void)
{
    reg_rf_ll_ctrl0 = 0x45; // reset tx/rx state machine
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
    reg_rf_preamble_trail = (reg_rf_preamble_trail & 0xe0) | (len & 0x1f); //rf preamble len
}

__INLINE int rf_ble_get_preamble_len(void)
{
    return reg_rf_preamble_trail & 0x1f; //[4:0]: BLE preamble length
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

/**
 * @brief     This function calilbrate dc
 * @param[in] none
 * @return    none.
 */
void rf_calib_dc(void);

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
#ifdef MCU_CORE_N22
    clic_interrupt_enable(IRQ_ZB_RT);
#else
    plic_interrupt_enable(IRQ_ZB_RT);
#endif
}

#define RF_RX_WAIT_MIN_VALUE       (8)
#define RF_TX_WAIT_MIN_VALUE       (8)


#define ONCA_CHIP_TX_RAMP_UP_DELAY 3
#define ONCA_CHIP_TX_PATH_DELAY    10
#define ONCA_CHIP_TX_DELAY_TOTAL   (ONCA_CHIP_TX_RAMP_UP_DELAY + ONCA_CHIP_TX_PATH_DELAY) //13


/* minimum value for register setting */
#define TX_SETTLE_REG_MIN 44
#define RX_SETTLE_REG_MIN 44


/* for some special case, 1M & 2M & Coded PHY first anchor point must be aligned
 * TX settle minimum value align with 1M PHY
 * 1M PHY: 43us, 2M PHY: 47uS, Coded PHY 51uS, stack process OK */
#define TX_SETTLE_MIN_ALIGN_1MPHY 57 //43 + 8(1 preamble 1M PHY)

/* RX settle */
#define RX_SETTLE_US          RX_SETTLE_REG_MIN
#define RXSET_OPTM_ANTI_INTRF 100 //RX settle value for optimize anti-interference


#define PRMBL_LENGTH_1M       2  //preamble length for 1M PHY
#define PRMBL_LENGTH_2M       3  //preamble length for 2M PHY
#define PRMBL_LENGTH_Coded    10 //preamble length for Coded PHY, never change this value !!!

#define PRMBL_EXTRA_1M        (PRMBL_LENGTH_1M - 1) // 1 byte for 1M PHY
#define PRMBL_EXTRA_2M        (PRMBL_LENGTH_2M - 2) // 2 byte for 2M
#define PRMBL_EXTRA_Coded     0                     // 10byte for Coded, 80uS, no extra byte


/* TX settle set to minimum value */
#define TX_STL_LEGADV_SCANRSP_SET  TX_SETTLE_REG_MIN                                                   //43
#define TX_STL_LEGADV_SCANRSP_REAL (TX_SETTLE_REG_MIN + ONCA_CHIP_TX_DELAY_TOTAL + PRMBL_EXTRA_1M * 8) //43+13+8=64


#define TX_STL_TIFS_REAL_1M        (TX_SETTLE_REG_MIN + ONCA_CHIP_TX_DELAY_TOTAL + PRMBL_EXTRA_1M * 8) //43+13+8=64
#define TX_STL_TIFS_SET_1M         TX_SETTLE_REG_MIN                                                   //43

#define TX_STL_TIFS_REAL_2M        (TX_SETTLE_REG_MIN + ONCA_CHIP_TX_DELAY_TOTAL + PRMBL_EXTRA_2M * 4) //43+13+4=60
#define TX_STL_TIFS_SET_2M         TX_SETTLE_REG_MIN                                                   //43

#define TX_STL_TIFS_REAL_CODED     (TX_SETTLE_REG_MIN + ONCA_CHIP_TX_DELAY_TOTAL) //43+13=56
#define TX_STL_TIFS_SET_CODED      TX_SETTLE_REG_MIN                              //43


/* TX_STL_ADV_REAL_COMMON is used in stack to calculate packet length, so we should make all PHYs same value */
#define TX_STL_ADV_REAL_COMMON (TX_SETTLE_MIN_ALIGN_1MPHY + ONCA_CHIP_TX_DELAY_TOTAL) //64

#define TX_STL_ADV_REAL_1M     TX_STL_ADV_REAL_COMMON                           //64
#define TX_STL_ADV_SET_1M      (TX_SETTLE_MIN_ALIGN_1MPHY - PRMBL_EXTRA_1M * 8) //51 - 8 = 43

#define TX_STL_ADV_REAL_2M     TX_STL_ADV_REAL_COMMON                           //64
#define TX_STL_ADV_SET_2M      (TX_SETTLE_MIN_ALIGN_1MPHY - PRMBL_EXTRA_2M * 4) //51 - 4 = 47

#define TX_STL_ADV_REAL_CODED  TX_STL_ADV_REAL_COMMON    //64
#define TX_STL_ADV_SET_CODED   TX_SETTLE_MIN_ALIGN_1MPHY //51


/* for TIFW_150uS align */
#define TX_STL_AUTO_MODE_1M    (127 - PRMBL_EXTRA_1M * 8 - ONCA_CHIP_TX_DELAY_TOTAL)
#define TX_STL_AUTO_MODE_2M    (132 - PRMBL_EXTRA_2M * 4 - ONCA_CHIP_TX_DELAY_TOTAL)
#define TX_STL_AUTO_MODE_CODED (125 - 0 - -ONCA_CHIP_TX_DELAY_TOTAL)


/* 1M & 2M & Coded PHY first anchor point must be aligned */
#define TX_STL_BTX_1ST_PKT_REAL      (TX_SETTLE_MIN_ALIGN_1MPHY + ONCA_CHIP_TX_DELAY_TOTAL) //64

#define TX_STL_BTX_1ST_PKT_SET_1M    (TX_SETTLE_MIN_ALIGN_1MPHY - PRMBL_EXTRA_1M * 8) //51 - 8 = 43
#define TX_STL_BTX_1ST_PKT_SET_2M    (TX_SETTLE_MIN_ALIGN_1MPHY - PRMBL_EXTRA_2M * 4) //51 - 4 = 47
#define TX_STL_BTX_1ST_PKT_SET_CODED TX_SETTLE_MIN_ALIGN_1MPHY                        //51


/* AD convert delay : timing cost on RF analog to digital convert signal process:
 *                  Eagle   1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 *                  Jaguar  1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 */
#define AD_CONVERT_DLY_1M        9
#define AD_CONVERT_DLY_2M        4
#define AD_CONVERT_DLY_CODED     14

#define OTHER_SWITCH_DELAY_1M    0
#define OTHER_SWITCH_DELAY_2M    0
#define OTHER_SWITCH_DELAY_CODED 0


#define HW_DELAY_1M              (AD_CONVERT_DLY_1M + OTHER_SWITCH_DELAY_1M)
#define HW_DELAY_2M              (AD_CONVERT_DLY_2M + OTHER_SWITCH_DELAY_2M)
#define HW_DELAY_CODED           (AD_CONVERT_DLY_CODED + OTHER_SWITCH_DELAY_CODED)

__INLINE void rf_ble_set_1m_phy(void)
{
    rf_set_ble_1M_mode();
}

__INLINE void rf_ble_set_2m_phy(void)
{
    rf_set_ble_2M_mode();
}

__INLINE void rf_ble_set_coded_phy_common(void) {}

__INLINE void rf_ble_set_coded_phy_s2(void)
{
    rf_set_ble_500K_mode();
}

__INLINE void rf_ble_set_coded_phy_s8(void)
{
    rf_set_ble_125K_mode();
}

//This is to be compatible in older versions. If you don't use them, you can delete them.
#define rf_trigle_codedPhy_accesscode rf_trigger_codedPhy_accesscode

__INLINE unsigned char rf_ble_get_tx_pwr_idx(signed char rfTxPower)
{
    rf_power_level_index_e rfPwrLvlIdx;

    /*VBAT*/
    if (rfTxPower >= 9) {
        rfPwrLvlIdx = RF_POWER_INDEX_P9p00dBm;
    } else if (rfTxPower >= 8) {
        rfPwrLvlIdx = RF_POWER_INDEX_P8p00dBm;
    } else if (rfTxPower >= 7) {
        rfPwrLvlIdx = RF_POWER_INDEX_P7p00dBm;
    } else if (rfTxPower >= 6) {
        rfPwrLvlIdx = RF_POWER_INDEX_P6p00dBm;
    }
    /*VANT*/
    else if (rfTxPower >= 5) {
        rfPwrLvlIdx = RF_POWER_INDEX_P5p00dBm;
    } else if (rfTxPower >= 4) {
        rfPwrLvlIdx = RF_POWER_INDEX_P4p00dBm;
    } else if (rfTxPower >= 3) {
        rfPwrLvlIdx = RF_POWER_INDEX_P3p00dBm;
    } else if (rfTxPower >= 2) {
        rfPwrLvlIdx = RF_POWER_INDEX_P2p50dBm;
    } else if (rfTxPower >= 1) {
        rfPwrLvlIdx = RF_POWER_INDEX_P1p00dBm;
    } else if (rfTxPower >= 0) {
        rfPwrLvlIdx = RF_POWER_INDEX_P0p00dBm;
    } else if (rfTxPower >= -4) {
        rfPwrLvlIdx = RF_POWER_INDEX_N4p00dBm;
    } else if (rfTxPower >= -9) {
        rfPwrLvlIdx = RF_POWER_INDEX_N9p50dBm;
    } else if (rfTxPower >= -12) {
        rfPwrLvlIdx = RF_POWER_INDEX_N12p00dBm;
    } else if (rfTxPower >= -17) {
        rfPwrLvlIdx = RF_POWER_INDEX_N17p00dBm;
    } else { //rfTxPower >= -24
        rfPwrLvlIdx = RF_POWER_INDEX_N24p00dBm;
    }

    return rfPwrLvlIdx;
}

__INLINE signed char rf_ble_get_tx_pwr_level(rf_power_level_index_e rfPwrLvlIdx)
{
    signed char rfTxPower;

    /*VBAT*/
    if (rfPwrLvlIdx <= RF_POWER_INDEX_P9p00dBm) {
        rfTxPower = 9;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P8p00dBm) {
        rfTxPower = 8;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P7p00dBm) {
        rfTxPower = 7;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P6p00dBm) {
        rfTxPower = 6;
    }
    /*VANT*/
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P5p00dBm) {
        rfTxPower = 5;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P4p00dBm) {
        rfTxPower = 4;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P3p00dBm) {
        rfTxPower = 3;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P2p50dBm) {
        rfTxPower = 2;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P1p00dBm) {
        rfTxPower = 1;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_P0p00dBm) {
        rfTxPower = 0;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N4p00dBm) {
        rfTxPower = -4;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N9p50dBm) {
        rfTxPower = -9;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N12p00dBm) {
        rfTxPower = -12;
    } else if (rfPwrLvlIdx <= RF_POWER_INDEX_N17p00dBm) {
        rfTxPower = -17;
    } else { //rfPwrLvlIdx <= RF_POWER_INDEX_N24p00dBm
        rfTxPower = -24;
    }

    return rfTxPower;
}

signed char rf_ble_get_rf_actual_tx_pwr(signed char txPwr, bool needComp);

signed char rf_ble_get_inc_rf_tx_pwr(signed char reqPwr, signed char delta);

//751X don't need to reset tx mode,just for compile pass.
static inline void rf_tx_mode_reset(void) {}

void rf_ble_set_tx_rx_settle_time(unsigned char tx_rx_stl_us);

/******************************* ext_rf end ********************************************************************/
/* must be empty, due to TX RX settle share same setting */
#define rf_ble_set_tx_settle(x)
#define rf_ble_set_rx_settle(x)
#define rf_ble_set_rx_wait(x)
#define rf_ble_set_tx_wait(x)
#define rf_ble_tx_on()
#endif /* DRIVERS_B91_EXT_DRIVER_DRIVER_LIB_EXT_RF_H_ */
