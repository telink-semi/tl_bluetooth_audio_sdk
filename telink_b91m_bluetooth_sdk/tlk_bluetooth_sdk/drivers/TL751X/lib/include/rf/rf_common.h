/********************************************************************************************************
 * @file    rf_common.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
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
#ifndef RF_COMMON_H
#define RF_COMMON_H

#include "lib/include/sys.h"
#include "lib/include/rf/rf_dma.h"
#include "lib/include/rf/rf_ble.h"
#include "lib/include/rf/rf_private.h"
#include "lib/include/rf/rf_zigbee.h"
#include "gpio.h"
/**********************************************************************************************************************
 *                                         RF  global macro                                                           *
 *********************************************************************************************************************/
/**
 *  @brief This define serve to calculate the DMA length of packet.
 */
#define rf_tx_packet_dma_len(rf_data_len) (((rf_data_len) + 3) / 4) | (((rf_data_len) % 4) << 22)

/**
 *  @brief This define serve to compensate for RF RSSI offset values.
 */
#define RF_RSSI_OFFSET        3
/**
 *  @brief This macro provides an alternative name for the rf_get_latched_rssi() function to be compatible with older versions of code
 */
#define rf_get_rssi rf_get_latched_rssi
/**********************************************************************************************************************
 *                                       RF global data type                                                          *
 *********************************************************************************************************************/

typedef struct
{
    unsigned char txPower_index;
    unsigned char txPower_level; /*!< added to be compatible with driver api */

} rf_power_t;

extern rf_power_t rf_power;

/**
 *  @brief  select status of rf.
 */
typedef enum
{
    RF_MODE_TX   = 0, /**<  Tx mode */
    RF_MODE_RX   = 1, /**<  Rx mode */
    RF_MODE_AUTO = 2, /**<  Auto mode */
    RF_MODE_OFF  = 3  /**<  TXRX OFF mode */
} rf_status_e;

/**
 *  @brief  Define power list of RF.
 *  @note   Attention:
 *          (1)The TX power values in the table are based on tests conducted with VDDRF1 at 1.8V and VDDRF2 at 1.04V under BGA package testing.
 *          (2)The power values in the table are for reference only, and the specific power values are subject to actual testing
 *          (3)At present, the configuration method of tx power is not the final version and will be updated in the future
 *          (4)TX Power levels above 13dbm are internal test versions and are not open to users.
 *
 */
typedef enum
{
    /*HP*/
    #ifdef GREATER_TX_POWER_EN
    RF_POWER_P14p00dBm = 0x2e, /**<  14.0 dbm */
    RF_POWER_P13p50dBm = 0x2d, /**<  13.5 dbm */
    #endif
    RF_POWER_P13p00dBm = 0x2c, /**<  13.0 dbm */
    RF_POWER_P12p50dBm = 0x2b, /**< 12.5 dbm */
    RF_POWER_P12p00dBm = 0x2a, /**< 12.0 dbm */
    RF_POWER_P11p50dBm = 0x29, /**< 11.5 dbm */
    RF_POWER_P11p00dBm = 0x28, /**< 11.0 dbm */
    RF_POWER_P10p50dBm = 0x27, /**< 10.5 dbm */
    RF_POWER_P10p00dBm = 0x26, /**< 10.0 dbm */
    RF_POWER_P9p50dBm  = 0x25, /**<  9.5 dbm */
    RF_POWER_P9p00dBm  = 0x24, /**<  9.0 dbm */
    RF_POWER_P8p50dBm  = 0x23, /**<  8.5 dbm */
    RF_POWER_P8p00dBm  = 0x22, /**<  8.0 dbm */
    RF_POWER_P7p75dBm  = 0x21, /**<  7.75 dbm */
    RF_POWER_P7p50dBm  = 0x20, /**<  7.5 dbm */
    RF_POWER_P7p00dBm  = 0x1f, /**<  7.0 dbm */
    RF_POWER_P6p50dBm  = 0x1e, /**<  6.5 dbm */
    RF_POWER_P6p00dBm  = 0x1d, /**<  6.0 dbm */
    RF_POWER_P5p50dBm  = 0x1c, /**<  5.5 dbm */
    RF_POWER_P5p00dBm  = 0x1b, /**<  5.0 dbm */
    RF_POWER_P4p50dBm  = 0x1a, /**<  4.5 dbm */
    RF_POWER_P4p00dBm  = 0x19, /**<  4.0 dbm */
    RF_POWER_P3p50dBm  = 0x18, /**<  3.5 dbm */
    RF_POWER_P3p00dBm  = 0x17, /**<  3.0 dbm */
    RF_POWER_P2p50dBm  = 0x16, /**<  2.5 dbm */
    /*LP*/
    RF_POWER_P1p00dBm  = 0x14, /**<  1.0 dbm */
    RF_POWER_P0p50dBm  = 0x13, /**<  0.50 dbm */
    RF_POWER_P0p00dBm  = 0x12, /**<  0.00 dbm */
    RF_POWER_N0p25dBm  = 0x11, /**<  -0.25 dbm */
    RF_POWER_N0p75dBm  = 0x10, /**<  -0.75 dbm */
    RF_POWER_N1p00dBm  = 0x0f, /**<  -1.00 dbm */
    RF_POWER_N1p75dBm  = 0x0e, /**<  -1.75 dbm */
    RF_POWER_N2p50dBm  = 0x0d, /**<  -2.50 dbm */
    RF_POWER_N3p00dBm  = 0x0c, /**<  -3.00 dbm */
    RF_POWER_N3p50dBm  = 0x0b, /**<  -3.50 dbm */
    RF_POWER_N4p00dBm  = 0x0a, /**<  -4.00 dbm */
    RF_POWER_N4p50dBm  = 0x09, /**<  -4.50 dbm */
    RF_POWER_N5p00dBm  = 0x08, /**<  -5.00 dbm */
    RF_POWER_N7p00dBm  = 0x07, /**<  -7.00 dbm */
    RF_POWER_N9p50dBm  = 0x06, /**<  -9.50 dbm */
    RF_POWER_N12p00dBm  = 0x05, /**<  -12.00 dbm */
    RF_POWER_N15p00dBm  = 0x04, /**<  -15.00 dbm */
    RF_POWER_N17p00dBm  = 0x03, /**<  -17.00 dbm */
    RF_POWER_N19p00dBm  = 0x02, /**<  -19.00 dbm */
    RF_POWER_N22p00dBm  = 0x01, /**<  -22.00 dbm */
    RF_POWER_N24p00dBm  = 0x00, /**<  -24.00 dbm */
} rf_power_level_e;

/**
 *  @brief  Define power index list of RF.
 *  @note   Attention:
 *          (1)The TX power values in the table are based on tests conducted with VDDRF1 at 1.8V and VDDRF2 at 1.04V under BGA package testing.
 *          (2)The power values in the table are for reference only, and the specific power values are subject to actual testing
 *          (3)At present, the configuration method of tx power is not the final version and will be updated in the future
 *          (4)TX Power levels above 13dbm are internal test versions and are not open to users.
 *
 */
typedef enum
{

    /*HP*/
    #ifdef GREATER_TX_POWER_EN
    RF_POWER_INDEX_P14p00dBm, /**<  14.0 dbm */
    RF_POWER_INDEX_P13p50dBm, /**<  13.5 dbm */
    #endif
    RF_POWER_INDEX_P13p00dBm, /**<  13.0 dbm */
    RF_POWER_INDEX_P12p50dBm, /**< 12.5 dbm */
    RF_POWER_INDEX_P12p00dBm, /**< 12.0 dbm */
    RF_POWER_INDEX_P11p50dBm, /**< 11.5 dbm */
    RF_POWER_INDEX_P11p00dBm, /**< 11.0 dbm */
    RF_POWER_INDEX_P10p50dBm, /**< 10.5 dbm */
    RF_POWER_INDEX_P10p00dBm, /**< 10.0 dbm */
    RF_POWER_INDEX_P9p50dBm, /**<  9.5 dbm */
    RF_POWER_INDEX_P9p00dBm, /**<  9.0 dbm */
    RF_POWER_INDEX_P8p50dBm, /**<  8.5 dbm */
    RF_POWER_INDEX_P8p00dBm, /**<  8.0 dbm */
    RF_POWER_INDEX_P7p75dBm, /**<  7.75 dbm */
    RF_POWER_INDEX_P7p50dBm, /**<  7.5 dbm */
    RF_POWER_INDEX_P7p00dBm, /**<  7.0 dbm */
    RF_POWER_INDEX_P6p50dBm, /**<  6.5 dbm */
    RF_POWER_INDEX_P6p00dBm, /**<  6.0 dbm */
    RF_POWER_INDEX_P5p50dBm, /**<  5.5 dbm */
    RF_POWER_INDEX_P5p00dBm, /**<  5.0 dbm */
    RF_POWER_INDEX_P4p50dBm, /**<  4.5 dbm */
    RF_POWER_INDEX_P4p00dBm, /**<  4.0 dbm */
    RF_POWER_INDEX_P3p50dBm, /**<  3.5 dbm */
    RF_POWER_INDEX_P3p00dBm, /**<  3.0 dbm */
    RF_POWER_INDEX_P2p50dBm, /**<  2.5 dbm */
    /*LP*/
    RF_POWER_INDEX_P1p00dBm, /**<  1.0 dbm */
    RF_POWER_INDEX_P0p50dBm, /**<  0.50 dbm */
    RF_POWER_INDEX_P0p00dBm, /**<  0.00 dbm */
    RF_POWER_INDEX_N0p25dBm, /**<  -0.25 dbm */
    RF_POWER_INDEX_N0p75dBm, /**<  -0.75 dbm */
    RF_POWER_INDEX_N1p00dBm, /**<  -1.00 dbm */
    RF_POWER_INDEX_N1p75dBm, /**<  -1.75 dbm */
    RF_POWER_INDEX_N2p50dBm, /**<  -2.50 dbm */
    RF_POWER_INDEX_N3p00dBm, /**<  -3.00 dbm */
    RF_POWER_INDEX_N3p50dBm, /**<  -3.50 dbm */
    RF_POWER_INDEX_N4p00dBm, /**<  -4.00 dbm */
    RF_POWER_INDEX_N4p50dBm, /**<  -4.50 dbm */
    RF_POWER_INDEX_N5p00dBm, /**<  -5.00 dbm */
    RF_POWER_INDEX_N7p00dBm, /**<  -7.00 dbm */
    RF_POWER_INDEX_N9p50dBm, /**<  -9.50 dbm */
    RF_POWER_INDEX_N12p00dBm, /**<  -12.00 dbm */
    RF_POWER_INDEX_N15p00dBm, /**<  -15.00 dbm */
    RF_POWER_INDEX_N17p00dBm, /**<  -17.00 dbm */
    RF_POWER_INDEX_N19p00dBm, /**<  -19.00 dbm */
    RF_POWER_INDEX_N22p00dBm, /**<  -22.00 dbm */
    RF_POWER_INDEX_N24p00dBm, /**<  -24.00 dbm */

} rf_power_level_index_e;

/**
 *  @brief  Define RF mode.
 */
typedef enum
{
    RF_MODE_BLE_2M       = BIT(0), /**< ble 2m mode */
    RF_MODE_BLE_1M       = BIT(1), /**< ble 1M mode */
    RF_MODE_BLE_1M_NO_PN = BIT(2), /**< ble 1M close pn mode */
    RF_MODE_LR_S2_500K   = BIT(4), /**< ble 500K mode */
    RF_MODE_LR_S8_125K   = BIT(5), /**< ble 125K mode */
    RF_MODE_ZIGBEE_250K  = BIT(3), /**< zigbee 250K mode */
    RF_MODE_PRIVATE_1M   = BIT(6), /**< private 1M mode */
    RF_MODE_PRIVATE_2M   = BIT(7), /**< private 2M mode */
    RF_MODE_BLE_2M_NO_PN = BIT(8), /**< ble 2M close pn mode */
} rf_mode_e;


/**
 *  @brief  Define RF channel.
 */
typedef enum
{
    RF_CHANNEL_0    = BIT(0), /**< RF channel 0 */
    RF_CHANNEL_1    = BIT(1), /**< RF channel 1 */
    RF_CHANNEL_2    = BIT(2), /**< RF channel 2 */
    RF_CHANNEL_3    = BIT(3), /**< RF channel 3 */
    RF_CHANNEL_4    = BIT(4), /**< RF channel 4 */
    RF_CHANNEL_5    = BIT(5), /**< RF channel 5 */
    RF_CHANNEL_NONE = 0x00,   /**< none RF channel*/
    RF_CHANNEL_ALL  = 0x3f,   /**< all RF channel */
} rf_channel_e;

/**
 *  @brief  RF module state machine definition.
 */
typedef enum {
    RF_IDLE_STATE, /**< RF idle state */
    RF_TX_STATE,   /**< RF tx state */
    RF_RX_STATE,   /**< RF rx state */
    RF_TX_WAIT,    /**< RF wait for tx state */
    RF_RX_WAIT,    /**< RF wait for rx state */
}rf_state_e;
/**********************************************************************************************************************
 *                                         RF global constants                                                        *
 *********************************************************************************************************************/
extern volatile rf_power_level_e rf_power_Level_list[60];
extern rf_mode_e              g_rfmode;

/**********************************************************************************************************************
 *                                         RF function declaration                                                    *
 *********************************************************************************************************************/


/**
 * @brief       This function serves to judge the statue of  RF receive.
 * @return      -#0:idle
 *              -#1:rx_busy
 */
static inline unsigned char rf_receiving_flag(void)
{
    //if the value of [2:0] of the reg_0x170040 isn't 0 , it means that the RF is in the receiving packet phase.(confirmed by junwen).
    return ((read_reg8(0xd4170040) & 0x07) > 1);
}

/**
 * @brief       This function serves to set the which irq enable.
 * @param[in]   mask    - Options that need to be enabled.
 * @return      Yes: 1, NO: 0.
 */
static inline void rf_set_irq_mask(rf_irq_e mask)
{
    BM_SET(reg_rf_irq_mask, mask);
    BM_SET(reg_rf_ll_irq_mask_h, (mask >> 16) & 0x3f);
}

/**
 * @brief       This function serves to clear the TX/RX irq mask.
 * @param[in]   mask    - RX/TX irq value.
 * @return      none.
 */
static inline void rf_clr_irq_mask(rf_irq_e mask)
{
    BM_CLR(reg_rf_irq_mask, mask);
    BM_CLR(reg_rf_ll_irq_mask_h, (mask >> 16));
}

/**
 * @brief       This function serves to judge whether it is in a certain state.
 * @param[in]   mask      - RX/TX irq status.
 * @retval      non-zero      - the interrupt occurred.
 * @retval      zero  - the interrupt did not occur.
 */
static inline unsigned int rf_get_irq_status(rf_irq_e status)
{
    return ((unsigned int)(BM_IS_SET(reg_rf_irq_status, status) | (BM_IS_SET((reg_rf_ll_irq_list_h << 16), status))));
}

/**
 *@brief    This function serves to clear the Tx/Rx finish flag bit.
 *          After all packet data are sent, corresponding Tx finish flag bit
 *          will be set as 1.By reading this flag bit, it can check whether
 *          packet transmission is finished. After the check, it is needed to
 *          manually clear this flag bit so as to avoid misjudgment.
 *@return   none.
 */
static inline void rf_clr_irq_status(rf_irq_e status)
{
    reg_rf_irq_status    = status;
    reg_rf_ll_irq_list_h = (status >> 16) & 0x3f;
}

/**
 * @brief       This function serves to set RF access code.
 * @param[in]   acc   - the value of access code.
 * @return      none.
 */
static inline void rf_access_code_comm(unsigned int acc)
{
    reg_rf_access_code = acc;
}

/**
 * @brief     This function serves to reset RF Tx/Rx mode.
 * @return    none.
 */
static inline void rf_set_tx_rx_off(void)
{
    write_reg8(0xd4170028, 0x80);                              // rx disable
    write_reg8(0xd4170202, 0x45);                              // reset tx/rx state machine
    write_reg8(0xd417022b, read_reg8(0xd417022b) & (~BIT(7))); //dis continue mode.
}

/**
 * @brief    This function serves to turn off RF auto mode.
 * @return   none.
 * @note     Attention: When forcibly stopping the state machine through this interface, it must be ensured
 *           that rx is not in the process of receiving packets.Otherwise, an error may be caused.To determine
 *           whether the packet is being received, you can use the function rf_receiving_flag.
 */
static inline void rf_set_tx_rx_off_auto_mode(void)
{
    write_reg8(0xd4170200, 0x80);
}

/**
 * @brief    This function serves to set CRC advantage.
 * @return   none.
 */
static inline void rf_set_ble_crc_adv(void)
{
    write_reg32(0xd4170024, 0x555555);
}

/**
 * @brief       This function serves to set CRC value for RF.
 * @param[in]   crc  - CRC value.
 * @return      none.
 */
static inline void rf_set_ble_crc_value(unsigned int crc)
{
    write_reg32(0xd4170024, crc);
}

/**
 * @brief      This function serves to set the max length of rx packet.Use byte_len to limit what DMA
 *             moves out will not exceed the buffer size we define.And old chip do this through dma size.
 * @param[in]  byte_len  - The longest of rx packet.
 * @return     none.
 */
static inline void rf_set_rx_maxlen(unsigned int byte_len)
{
    reg_rf_rxtmaxlen0 = byte_len & 0xff;
    reg_rf_rxtmaxlen1 = (byte_len >> 8) & 0xff;
}

/**
 * @brief       This function serve to rx dma fifo size.
 * @param[in]   fifo_byte_size - the size of each fifo.
 * @return      none
 */
static inline void rf_set_rx_dma_fifo_size(unsigned short fifo_byte_size)
{
    reg_rf_bb_rx_size   = fifo_byte_size & 0xff; //rx_idx_addr = {rx_wptr*bb_rx_size,4'h0}// in this setting the max data in one dma buffer is 0x20<<4.
    reg_rf_bb_rx_size_h = fifo_byte_size >> 8;
}

/**
 * @brief       This function serve to set rx dma wptr.
 * @param[in]   wptr    -rx_wptr_real=rx_wptr & mask:After receiving 4 packets,the address returns to original address.mask value must in (0x01,0x03,0x07,0x0f).
 * @return      none
 */
static inline void rf_set_rx_dma_fifo_num(unsigned char fifo_num)
{
    reg_rf_rx_wptr_mask = fifo_num; //rx_wptr_real=rx_wptr & mask:After receiving 4 packets,the address returns to original address.mask value must in (0x01,0x03,0x07,0x0f).
}

/**
 * @brief       This function serves to DMA rxFIFO address
 *              The function apply to the configuration of one rxFiFO when receiving packets,
 *              In this case,the rxFiFo address can be changed every time a packet is received
 *              Before setting, call the function "rf_set_rx_dma" to clear DMA fifo mask value(set 0)
 * @param[in]   rx_addr   - The address store receive packet.
 * @return      none
 */
static inline void rf_set_rx_buffer(unsigned char *rx_addr)
{
    rx_addr += 4;
    rf_dma_set_dst_address(RF_RX_DMA, (unsigned int)(rx_addr));
}

/**
 * @brief       This function serve to set the number of tx dma fifo.
 * @param[in]   fifo_dep - the number of dma fifo is 2 to the power of fifo_dep.
 * @return      none
 */
static inline void rf_set_tx_dma_fifo_num(unsigned char fifo_num)
{
    reg_rf_bb_tx_chn_dep = fifo_num; //tx_chn_dep = 2^2 =4 (have 4 fifo)
}

/**
 * @brief       This function serve to set the number of tx dma fifo.
 * @param[in]   fifo_byte_size - the size of each dma fifo.
 * @return      none
 */
static inline void rf_set_tx_dma_fifo_size(unsigned short fifo_byte_size)
{
    reg_rf_bb_tx_size   = fifo_byte_size & 0xff; //tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}// in this setting the max data in one dma buffer is 0x20<<4.And the The product of fifo_dep and bytesize cannot exceed 0xfff.
    reg_rf_bb_tx_size_h = fifo_byte_size >> 8;
}

/**
 * @brief   This function serves to set RF tx and rx settle time.
 * @param[in] tx_rx_stl_us - tx and rx settle time,the unit is us.This parameter can be configured from 43~200us
 * @return    none.
 * @note      Attention:(1)The default settle time for tx and rx are 43us.
 *                      (2)The settle time for tx and rx can only be set to the same.
 *            TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_tx_rx_settle_time(unsigned short tx_rx_stl_us)
{
    unsigned short tx_rx_stl_us_add = 0;
    unsigned char  settle_1         = 0x82;
    unsigned char  settle_2         = 0x8f;
    tx_rx_stl_us &= 0x0fff;
    write_reg16(0xd4170204, (read_reg16(0xd4170204) & 0xf000) | (tx_rx_stl_us - 1));
    write_reg16(0xd417020c, (read_reg16(0xd417020c) & 0xf000) | (tx_rx_stl_us - 1));

    if (tx_rx_stl_us > 53) {
        if (tx_rx_stl_us > 178) {
            tx_rx_stl_us_add = tx_rx_stl_us - 53;
            settle_1 += 125;
            settle_2 += (tx_rx_stl_us_add - 125);
        } else {
            tx_rx_stl_us_add = tx_rx_stl_us - 53; //tx settle time- 53
            settle_1 += tx_rx_stl_us_add;
        }
    } else if (tx_rx_stl_us < 53) {
        tx_rx_stl_us_add = 53 - tx_rx_stl_us;
        settle_2 -= tx_rx_stl_us_add;
    }

    write_reg8(CSEMDIGADDR + 0x9C0 + 0x5e, settle_1); //default value:0x82(bit7 non-data bit),target value:0x02+tx_stl_us_add
    write_reg8(CSEMDIGADDR + 0x9C0 + 0x5a, settle_2);
}

/**
 * @brief   This function serves to set RF tx settle time.
 * @param[in] tx_stl_us - tx settle time,the unit is us.This parameter can be configured from 43~200us;
 * @return    none.
 * @note      Attention:(1)The default tx settle time is 43us.
 *                      (2)The settle time for tx and rx can only be set to the same.
 *                      (3)It is not necessary to call this function to adjust the settling time in the normal sending state.
 *            TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_tx_settle_time(unsigned short tx_stl_us)
{
    rf_set_tx_rx_settle_time(tx_stl_us);
}

/**
 * @brief   This function serves to set RF tx settle time and rx settle time.
 * @param[in] rx_stl_us - rx settle time,the unit is us.This parameter can be configured from 43~200us;
 * @return    none.
 * @note      Attention:(1)The default rx settle time is 43us.
 *                      (2)The settle time for tx and rx can only be set to the same.
 *                      (3)It is not necessary to call this function to adjust the settling time in the normal sending state.
 *            TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_rx_settle_time(unsigned short rx_stl_us)
{
    rf_set_tx_rx_settle_time(rx_stl_us);
}

/**
 * @brief       This function serves to settle adjust for RF Tx.This function for adjust the differ time
 *              when rx_dly enable.
 * @param[in]   txstl_us   - adjust TX settle time.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_tx_settle_us(unsigned short txstl_us)
{
    rf_set_tx_rx_settle_time(txstl_us);
}

/**
 * @brief      This function serves to set the tx wait time during the rx2tx process
 * @param[in]  tx_wait_us  tx wait time,the unit is us.The max value of this param is 0xfff; The default wait time value is 10us.
 * @return     none.
 * @note       Attention:It is not necessary to call this function to adjust the wait time in the rx2tx process.
 */
static inline void rf_set_tx_wait_time(unsigned short tx_wait_us)
{
    if (tx_wait_us > 0x0fff) {
        tx_wait_us = 0x0fff;
    }
    reg_rf_ll_txwait = (reg_rf_ll_txwait & 0xf000) | (tx_wait_us - 1);
}

/**
 * @brief      This function serves to set the rx wait time during the tx2rx process
 * @param[in]  rx_wait_us  rx wait time,the unit is us.The max value of this param is 0xfff; The default wait time value is 10us.
 * @return     none.
 * @note       Attention:It is not necessary to call this function to adjust the wait time in the tx2rx process.
 */
static inline void rf_set_rx_wait_time(unsigned short rx_wait_us)
{
    if (rx_wait_us > 0x0fff) {
        rx_wait_us = 0x0fff;
    }
    reg_rf_ll_rxwait = (reg_rf_ll_rxwait & 0xf000) | (rx_wait_us - 1);
}

/**
 * @brief   This function serve to get ptx wptr.
 * @param[in]   pipe_id -   The number of tx fifo.0<= pipe_id <=5.
 * @return      The write pointer of the tx.
 */
static inline unsigned char rf_get_tx_wptr(unsigned char pipe_id)
{
    return reg_rf_dma_tx_wptr(pipe_id);
}

/**
 * @brief   This function serve to update the wptr of tx terminal.
 * @param[in]   pipe_id -   The number of pipe which need to update wptr.
 * @param[in]   wptr    -   The pointer of write in tx terminal.
 * @return      none
 */
static inline void rf_set_tx_wptr(unsigned char pipe_id, unsigned char wptr)
{
    reg_rf_dma_tx_wptr(pipe_id) = wptr;
}

/**
 * @brief   This function serve to clear the writer pointer of tx terminal.
 * @param[in]   pipe_id -   The number of tx DMA.0<= pipe_id <=5.
 * @return  none.
 */
static inline void rf_clr_tx_wptr(unsigned char pipe_id)
{
    reg_rf_dma_tx_wptr(pipe_id) = 0;
}

/**
 * @brief   This function serve to get ptx rptr.
 * @param[in]   pipe_id -The number of tx pipe.0<= pipe_id <=5.
 * @return      The read pointer of the tx.
 */
static inline unsigned char rf_get_tx_rptr(unsigned char pipe_id)
{
    return reg_rf_dma_tx_rptr(pipe_id);
}

/**
 * @brief   This function serve to clear read pointer of tx terminal.
 * @param[in]   pipe_id -   The number of tx DMA.0<= pipe_id <=5.
 * @return  none.
 */
static inline void rf_clr_tx_rptr(unsigned char pipe_id)
{
    reg_rf_dma_tx_rptr(pipe_id) = 0x80;
}

/**
 * @brief   This function serve to get the pointer of read in rx terminal.
 * @return  wptr    -   The pointer of rx_rptr.
 */
static inline unsigned char rf_get_rx_rptr(void)
{
    return reg_rf_dma_rx_rptr;
}

/**
 * @brief   This function serve to clear read pointer of rx terminal.
 * @return  none.
 */
static inline void rf_clr_rx_rptr(void)
{
    write_reg8(0xd41708f5, 0x80); //clear rptr
}

/**
 * @brief   This function serve to get the pointer of write in rx terminal.
 * @return  wptr    -   The pointer of rx_wptr.
 */
static inline unsigned char rf_get_rx_wptr(void)
{
    return reg_rf_dma_rx_wptr;
}

/**
 * @brief    This function is used to initialize the baseband related digital modules for n22
 * @return   none.
 * @note     Attention:
 *           (1)This interface should be called before rf_mode_init.
 *           (2)This interface should be called after sys_n22_init.
 */
void rf_n22_dig_init(void);

/**
 * @brief      This function serves to initiate information of RF.
 * @return     none.
 */
void rf_mode_init(void);

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] none
 * @return    none.
 */
void rf_set_tx_dma_config(void);

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] fifo_depth        - tx chn deep.
 * @param[in] fifo_byte_size    - tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}.
 * @return    none.
 */
void rf_set_tx_dma(unsigned char fifo_depth, unsigned short fifo_byte_size);


/**
 * @brief     This function serves to rx dma setting.
 * @param[in] buff            - The buffer that store received packet.
 * @param[in] wptr_mask       - DMA fifo mask value (0~fif0_num-1).
 * @param[in] fifo_byte_size  - The length of one dma fifo.
 * @return    none.
 */
void rf_set_rx_dma(unsigned char *buff, unsigned char wptr_mask, unsigned short fifo_byte_size);

/**
 * @brief       This function serve to rx dma config
 * @param[in]   none
 * @return      none
 */
void rf_set_rx_dma_config(void);

/**
 * @brief     This function serves to trigger srx on.
 * @param[in] tick  - Trigger rx receive packet after tick delay.
 * @return    none.
 */
void rf_start_srx(unsigned int tick);


/**
 * @brief       This function serves to get latched rssi.
 * @return      rssi value.
 * @note        1.The RSSI values above -20dBm tend to saturate, resulting in unreliable measurements.
 */
signed char rf_get_latched_rssi(void);

/**
 * @brief       This function serves to get the real time rssi.
 * @return      rssi value.
 * @note        1.The RSSI values above -20dBm tend to saturate, resulting in unreliable measurements.
 *              2.The RSSI values detected by different chips for the same signal strength may vary.
 */
signed char rf_get_real_time_rssi(void);

/**
 * @brief       This function is used to convert the RSSI value of the HD information in the Packet.
 * @param[in]   pkt_rssi  -  the RSSI value of the HD information in the received packets
 * @return      rssi value.
 * @note        1.The RSSI values above -20dBm tend to saturate, resulting in unreliable measurements.
 */
signed char rf_decode_pkt_rssi(unsigned char pkt_rssi);

/**
 * @brief       This function serves to get the max rssi.
 * @return      rssi value.
 * @note        1.The RSSI values above -20dBm tend to saturate, resulting in unreliable measurements.
 *              2.The RSSI values detected by different chips for the same signal strength may vary.
 */
signed char rf_get_max_rssi(void);
/**
 * @brief       This function serves to set RF Tx mode.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 *                   (TX manual mode has a bug)
 */
void rf_set_txmode(void);

/**
 * @brief       This function serves to set RF Tx packet address to DMA src_addr.
 * @param[in]   addr   - The packet address which to send.
 * @return      none.
 */
_attribute_ram_code_sec_ void rf_tx_pkt(void *addr);

/**
 * @brief       This function serves to judge RF Tx/Rx state.
 * @param[in]   rf_status   - Tx/Rx status.
 * @param[in]   rf_channel  - This param serve to set frequency channel(2400+rf_channel);
 *                            (Parameter range:2~80,Frequency range:2402~2480 MHz)
 * @return      Whether the setting is successful(-1:failed;else success).
 */
int rf_set_trx_state(rf_status_e rf_status, unsigned char rf_channel);

/**
 * @brief       This function serves to set rf channel for all mode.The actual channel set by this function is 2400+chn.
 * @param[in]   chn   - That you want to set the channel as 2400+chn;
 *                      (Parameter range:2~80,Frequency range:2402~2480 MHz)
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_set_chn(unsigned char chn);

/**
 * @brief       This function serves to disable pn of ble mode.
 * @return      none.
 */
void rf_pn_disable(void);

/**
 * @brief       This function serves to get the right fifo packet.
 * @param[in]   fifo_num   - The number of fifo set in dma.
 * @param[in]   fifo_dep   - depth of each fifo set in dma.
 * @param[in]   addr       - address of rx packet.
 * @return      the next rx_packet address.
 */
unsigned char *rf_get_rx_packet_addr(int fifo_num, int fifo_dep, void *addr);

/**
 * @brief       This function serves to set RF power level.
 * @param[in]   level    - The power level to set.
 * @return      none.
 */
void rf_set_power_level(rf_power_level_e level);

/**
 * @brief       This function serves to set RF power through select the level index.
 * @param[in]   idx      - The index of power level which you want to set.
 * @return      none.
 */
void rf_set_power_level_index(rf_power_level_index_e idx);

/**
 * @brief       This function serves to update the value of internal cap.
 * @param[in]   value   - The value of internal cap which you want to set.(0 < value < 0xff)
 * @return      none.
 * @note        Attention:
 *             (1)Adjusting the capacitance value may cause abnormal operation of the crystal oscillator!!!
 *             (2)This function call must be made after rf_mode_init
 */
void rf_update_internal_cap(unsigned char value);

/**
 * @brief       This function serves to close internal cap;
 * @return      none.
 * @note        Attention: This function call must be made after rf_mode_init
 *
 */
void rf_turn_off_internal_cap(void);

/**
 * @brief   This function serve to change the length of preamble.
 * @param[in]   len     -The value of preamble length.Set the register bit<0>~bit<4>.
 * @return      none
 */
void rf_set_preamble_len(unsigned char len);

/**
 * @brief   This function serve to set the length of access code.
 * @param[in]   byte_len    -   The value of access code length.
 * @return      none
 */
void rf_set_access_code_len(unsigned char byte_len);

/**
 * @brief   This function serves to set RF rx timeout.
 * @param[in]   timeout_us  -   rx_timeout after timeout_us us,The maximum of this param is 0xfff.
 * @return  none.
 */
static inline void rf_set_rx_timeout(unsigned short timeout_us)
{
    reg_rf_rx_timeout = timeout_us - 1;
}

/**
 * @brief       This function serves to RF trigger stx
 * @param[in]   addr    - DMA tx buffer.
 * @param[in]   tick    - Send after tick delay.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_start_stx(void *addr, unsigned int tick);

/**
 * @brief       This function serves to RF trigger stx2rx
 * @param[in]   addr    - DMA tx buffer.
 * @param[in]   tick    - Send after tick delay.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_start_stx2rx(void *addr, unsigned int tick);

/**
 * @brief       This function serves to set RF Rx manual on.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_set_rxmode(void);

/**
 * @brief       This function serves to RF trigger srx2tx.
 * @param[in]   addr  - DMA tx buffer.
 * @param[in]   tick  - Trigger rx receive packet after tick delay.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_start_srx2tx(void *addr, unsigned int tick);


/**
 * @brief       This function serves to get RF status.
 * @return      RF Rx/Tx status.
 */
rf_status_e rf_get_trx_state(void);

/**
 * @brief       This function is used to judge whether there is a CRC error in the received packet through hardware.
 *              For the same packet, the value of this bit is consistent with the CRC flag bit in the packet.
 * @param[in]   none.
 * @return      none.
 */
unsigned char rf_get_crc_err(void);

/**
 * @brief      This function serves to reset RF digital logic states.
 * @return     none
 * @note       (1)The rf_dma_reset interface needs to be called before this interface is called.
 *             (2)This function requires setting reset zb, rstl_bb, and reset modem.
 *                It is used to clear RF related state machines, IRQ states, and digital internal logic states.
 *             (3)This interface must be called after the RF seq has finished,
 *                and you can get the TX and RX seq status via rf_get_tx_sequence_done and rf_get_tx_sequence_done respectively
 *
 */
_attribute_ram_code_sec_noinline_ void rf_clr_dig_logic_state(void);

/**
 * @brief       This interface is used to obtain whether sequence is completed during the TX process.
 * @param[in]   none.
 * @return     1:Tx sequence completed. 0:Tx sequence not completed
 * @note       This interface is only valid when the RF state is TX_STATE.
 */
static inline bool rf_get_tx_sequence_done(void)
{
    return ((reg_rf_ana_trx_rf_pa_dtc0&0x02) == 0x02);
}

/**
 * @brief       This interface is used to obtain whether sequence is completed during the RX process.
 * @param[in]   none.
 * @return     1:Rx sequence completed. 0:Rx sequence not completed
 * @note       This interface is only valid when the RF state is RF_RX_STATE.
 */
static inline bool rf_get_rx_sequence_done(void)
{
    return ((reg_rf_ana_trx_rf_block_ena&0x01) == 0x01);
}

/**
 * @brief This function checks the RF state register to determine if the RF is in IDLE, TX, RX, or waiting states.
 * @return rf_state_e Current RF operational state (RF_IDLE_STATE/RF_TX_STATE/RF_RX_STATE/RF_TX_WAIT/RF_RX_WAIT)
 *
 * @note Register 0x24[2:0] encoding:
 *   0: Idle      | 1: Active (unused) | 2: TX Start-up
 *   3: TX State  | 4: RX Wait         | 5: RX State
 *   6: TX Wait   | 7: Reserved
 */
static inline rf_state_e rf_get_state(void)
{
    unsigned char rf_state = reg_rf_ll_2d_sclk & 0x07;
    
    /* State decoding */
    switch(rf_state) {
        case FLD_RF_STATE_MACHINE_TX_SETTLE:  // TX_STL: Transmitter settle (merge to TX_STATE)
        case FLD_RF_STATE_MACHINE_TX:  // TX_ACT: Active transmission
            return RF_TX_STATE;
            
        case FLD_RF_STATE_MACHINE_RX:  // RX_ACT: Active reception
            return RF_RX_STATE;
            
        case FLD_RF_STATE_MACHINE_RX_WAIT:  // RX_WAIT: Waiting for receiver activation
            return RF_RX_WAIT;
            
        case FLD_RF_STATE_MACHINE_TX_WAIT:  // TX_WAIT: Waiting for transmitter activation
            return RF_TX_WAIT;
            
        default: // IDLE/Reserved/Undefined states
            return RF_IDLE_STATE;
    }
}

/**
 * @brief       This function is used to reliably reset the RF digital logic state, preventing abnormal RF conditions due to reset sequence issues.
 * @param[in]   none.
 * @return      none
 * @note        The core flags rf_get_rx_sequence_done() and rf_get_tx_sequence_done() are only valid during LL RX STATE and LL TX STATE. These flags are unreliable while in the IDLE state.
 */
static inline void rf_reset_baseband(void)
{
    rf_state_e state = rf_get_state();
    if(RF_RX_STATE == state)
    {
        while (!rf_get_rx_sequence_done())
        {
            if(rf_get_state() == RF_TX_STATE)
            {
                 while (!rf_get_tx_sequence_done());
                 break;
            }
            else{
                break;
            }
        }
    }
    else if (RF_TX_STATE == state)
    {
        while (!rf_get_tx_sequence_done())
        {
            if(rf_get_state() == RF_RX_STATE)
            {
                while (!rf_get_rx_sequence_done());
                break;
            }
            else{
                break;
            }
        }
    }
    rf_clr_dig_logic_state();
    rf_dma_reset();
    rf_clr_dig_logic_state();
}

#endif
