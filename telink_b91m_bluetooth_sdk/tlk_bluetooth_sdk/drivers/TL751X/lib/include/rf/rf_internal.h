/********************************************************************************************************
 * @file    rf_internal.h
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
#ifndef RF_INTERNAL_H
#define RF_INTERNAL_H

#include "lib/include/rf/rf_dma.h"
#include "lib/include/rf/rf_common.h"
#include "gpio.h"

#if (0)
/**********************************************************************************************************************
 *                                       RF global data type                                                          *
 *********************************************************************************************************************/

/**
 * @brief   select baseband transmission unit
 */
typedef enum
{
    RF_WORLD_WIDTH  = 0,
    RF_DWORLD_WIDTH = 1,
    RF_QWORLD_WIDTH = 2,
} rf_trans_unit_e;

typedef enum
{
    RF_NORMAL_LENGTH_PACKET     = 0,
    RF_HYPER_LENGTH_PACKET      = 1,
    RF_ADV1_HYPER_LENGTH_PACKET = 2,
    RF_ADV2_HYPER_LENGTH_PACKET = 3
} rf_pkt_len_mode_e;

typedef enum
{
    RF_RX_ACL_AOA_AOD_EN = BIT(0),
    RF_RX_ADV_AOA_AOD_EN = BIT(1),
    RF_TX_ACL_AOA_AOD_EN = BIT(2),
    RF_TX_ADV_AOA_AOD_EN = BIT(3),
    RF_AOA_AOD_OFF       = 0
} rf_aoa_aod_mode_e;

/*
 * @brief  Data length type of AOA/AOD sampling.
 * @note   Attention:When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the sampling data to overflow.
 */
typedef enum
{
    IQ_8_BIT_MODE       = 0,
    IQ_16_BIT_MODE      = 1,
    IQ_16_BIT_LOW_MODE  = 2,
    IQ_16_BIT_HIGH_MODE = 3,
    IQ_20_BIT_MODE      = 4
} rf_aoa_aod_iq_data_mode_e;

/*
 * @brief  AOA/AOD sample interval time type enumeration.
 * @note   Attention:When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling data to overflow.
 *         In normal mode, the sampling interval of AOA is 4us, and AOD will judge whether the sampling interval is 4us or 2us according to
 *         CTE info.
 */
typedef enum
{
    SAMPLE_NORMAL_INTERVAL = 0, //In this case sample interval of aoa is 4us, and aod will judge sample interval is 4us or 2us according to CTE info.
    SAMPLE_2US_INTERVAL    = 3,
    SAMPLE_1US_INTERVAL    = 4,
    SAMPLE_0P5US_INTERVAL  = 5,
    SAMPLE_0P25US_INTERVAL = 6
} rf_aoa_aod_sample_interval_time_e;

/**
 *  @brief  set the modulation index.
 */
typedef enum
{
    RF_MI_P0p00 = 0,   /**< MI = 0 */
    RF_MI_P0p32 = 32,  /**< MI = 0.32 */
    RF_MI_P0p50 = 50,  /**< MI = 0.5 */
    RF_MI_P0p60 = 60,  /**< MI = 0.6 */
    RF_MI_P0p70 = 70,  /**< MI = 0.7 */
    RF_MI_P0p80 = 80,  /**< MI = 0.8 */
    RF_MI_P0p90 = 90,  /**< MI = 0.9 */
    RF_MI_P1p20 = 120, /**< MI = 1.2 */
    RF_MI_P1p30 = 130, /**< MI = 1.3 */
    RF_MI_P1p40 = 140, /**< MI = 1.4 */
} rf_mi_value_e;

/**
 *  @brief  select RX_CYC2LNA and TX_CYC2PA pin;
 */

typedef enum
{
    RF_RFFE_RX_PB1 = GPIO_PB1, /**<  pb1 as rffe rx pin */
    RF_RFFE_RX_PD6 = GPIO_PD6, /**<  pd6 as rffe rx pin */
    RF_RFFE_RX_PE4 = GPIO_PE4  /**<  pe4 as rffe rx pin */
} rf_lna_rx_pin_e;

typedef enum
{
    RF_RFFE_TX_PB0 = GPIO_PB0, /**<  pb0 as rffe tx pin */
    RF_RFFE_TX_PB6 = GPIO_PB6, /**<  pb6 as rffe tx pin */
    RF_RFFE_TX_PD7 = GPIO_PD7, /**<  pd7 as rffe tx pin */
    RF_RFFE_TX_PE5 = GPIO_PE5  /**<  pe5 as rffe tx pin */
} rf_pa_tx_pin_e;

/**********************************************************************************************************************
 *                                         RF function declaration                                                    *
 *********************************************************************************************************************/

/**
 * @brief   This function serve to judge whether it is in a certain state.
 * @param[in]   status  - Option of rf state machine status.
 * @return      -#0:Not in parameter setting state
 *              -#1:In parameter setting state
 */
static inline unsigned short rf_get_state_machine_status(state_machine_status_e status)
{
    return status == read_reg8(0xd4170224);
}

/**
 * @brief       this function is to enable/disable each access_code channel for
 *              RF Rx terminal.
 * @param[in]   pipe    - Bit0~bit5 correspond to channel 0~5, respectively.
 *                      - #0:Disable.
 *                      - #1:Enable.
 *                        If "enable" is set as 0x3f (i.e. 00111111),
 *                        all access_code channels (0~5) are enabled.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 * @return      none
 */
static inline void rf_rx_acc_code_pipe_en(rf_channel_e pipe)
{
    write_reg8(0xd417044d, (read_reg8(0xd417044d) & 0xc0) | pipe); //rx_access_code_chn_en
}

/**
 * @brief       this function is to select access_code channel for RF tx terminal.
 * @param[in]   pipe    - Bit0~bit2 the value correspond to channel 0~5, respectively.
 *                        if value > 5 enable channel 5.And only 1 channel can be selected every time.
 *                      - #0:Disable.
 *                      - #1:Enable.
 *                        If "enable" is set as 0x7 (i.e. 0111),
 *                        the access_code channel (5) is enabled.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 * @return      none
 */
static inline void rf_tx_acc_code_pipe_en(rf_channel_e pipe)
{
    write_reg8(0xd4170215, (read_reg8(0xd4170215) & 0xf8) | pipe); //Tx_Channel_man[2:0]
}

/**
 * @brief   This function serve to get ptx initial pid value.
 * @return  The  value of ptx pid before update.
 */
static inline unsigned char rf_get_ptx_pid(void)
{
    return ((reg_rf_ll_ctrl_1 & 0xc0) >> 6);
}

/**
 * @brief   This function serve to set the new ptx pid value.
 * @param[in]   pipe_id -The number of pipe.0<= pipe_id <=5.
 * @return  none
 */
static inline void rf_set_ptx_pid(unsigned char pipe_pid)
{
    reg_rf_ll_ctrl_1 |= (pipe_pid << 6);
}


/**
 * @brief       This function serves to set pin for RFFE of RF.
 * @param[in]   tx_pin   - select pin as rffe to send.
 * @param[in]   rx_pin   - select pin as rffe to receive.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_rffe_pin(rf_pa_tx_pin_e tx_pin, rf_lna_rx_pin_e rx_pin);


/**
 * @brief   This function serve to set the private ack enable,mainly used in prx/ptx.
 * @param[in]   rf_mode     -   Must be one of the private mode.
 * @return      none
 */
void rf_set_pri_tx_ack_en(rf_mode_e rf_mode);

/**
 * @brief   This function serve to set access code.This function will first get the length of access code from register 0x140805
 *          and then set access code in addr.
 * @param[in]   pipe_id -The number of pipe.0<= pipe_id <=5.
 * @param[in]   acc -The value access code
 * @note        For compatibility with previous versions the access code should be bit transformed by bit_swap();
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_pipe_access_code(unsigned int pipe_id, unsigned char *addr);

/**
 * @brief   This function serve to initial the ptx setting.
 * @return  none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_ptx_config(void);

/**
 * @brief   This function serve to initial the prx setting.
 * @return  none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_prx_config(void);

/**
 * @brief   This function serves to set RF ptx trigger.
 * @param[in]   addr    -   The address of tx_packet.
 * @param[in]   tick    -   Trigger ptx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_start_ptx(void *addr, unsigned int tick);

/**
 * @brief   This function serves to set RF prx trigger.
 * @param[in]   tick    -   Trigger prx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_start_prx(unsigned int tick);


/**
 * @brief   This function to set retransmit and retransmit delay.
 * @param[in]   retry_times - Number of retransmit, 0: retransmit OFF
 * @param[in]   retry_delay - Retransmit delay time.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_ptx_retry(unsigned char retry_times, unsigned short retry_delay);

/**
 * @brief   This function serves to judge whether the FIFO is empty.
 * @param pipe_id specify the pipe.
 * @return TX FIFO empty bit.
 *          -#0 TX FIFO NOT empty.
 *          -#1 TX FIFO empty.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
unsigned char rf_is_rx_fifo_empty(unsigned char pipe_id);

/**
 * @brief       This function is used to set how many words as the transmission unit of baseband and dma.
 *              You don't need to call this function for normal use. By default, the unit is 1 world!
 *              After configuring the DMA, call this function to adjust the DMA rate.
 * @param[in]   rf_trans_unit_e size    - the unit of burst size .Identify how many bytes of data are
 *                                        handled by DMA each time
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_baseband_trans_unit(rf_trans_unit_e size);

/**
 * @brief       This function is used to  set the modulation index of the receiver.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]   mi_value- the value of modulation_index*100.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_rx_modulation_index(rf_mi_value_e mi_value);


/**
 * @brief       This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]   mi_value- the value of modulation_index*100.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_tx_modulation_index(rf_mi_value_e mi_value);

/****************************************************************************************************************************************
 *                                         RF User-defined package related functions                                                    *
 ****************************************************************************************************************************************/

/**
 * @brief       This function serves to enable user-defined packet sending function.
 *              In this mode, users can define the meaning of the contents of the package according to their own needs.
 *              Only ble1M_NO_PN mode supports user-defined packet.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:Only ble1M_NO_PN mode supports user-defined packet.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_ble1m_no_pn_tx_customer_mode_en(void)
{
    BM_SET(reg_rf_tx_mode2, FLD_RF_R_CUSTOM_MADE);
}

/**
 * @brief       This function serves to disable user-defined packet sending function.
 *              By default, the user-defined packet sending function is turned off, and normal protocol packets are sent;
 *              the function rf_ble1M_no_pn_tx_customer_mode_en needs to be called when the user-defined packet sending
 *              function is needed.
 *              Only ble1M_NO_PN mode supports user-defined packet.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:Only ble1M_NO_PN mode supports user-defined packet.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_ble1m_no_pn_tx_customer_mode_dis(void)
{
    BM_CLR(reg_rf_tx_mode2, FLD_RF_R_CUSTOM_MADE);
}

/**
 * @brief       This function serves to disable user-defined packet sending function.
 *              After enabling the hardware to calculate the CRC, the hardware will automatically
 *              add the CRC information to the header information when sending and receiving packets.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:The hardware CRC is enabled by default. If you need to turn off this function,
 *              call the rf_tx_hw_crc_dis function.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_tx_hw_crc_en(void)
{
    BM_SET(reg_rf_tx_mode1, FLD_RF_CRC_EN);
}

/**
 * @brief       This function is used to disable the hardware to calculate the value of crc.
 *              After disabling the hardware to calculate the CRC, if you want to add the CRC
 *              information to the package, you need to calculate it by the user software, and
 *              then add it to the package information.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:In this case, the receiver can only use software to check whether
 *              the CRC is wrong, and the hardware CRC flag is no longer accurate.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_tx_hw_crc_dis(void)
{
    BM_CLR(reg_rf_tx_mode1, FLD_RF_CRC_EN);
}

/****************************************************************************************************************************************
 *                                         RF : functions used by hyper length                                                          *
 ****************************************************************************************************************************************/

/**
 * @brief       This function is used to set tx  package of ordinary length format or a certain hyper length format package.
 *              After configuring rf initialization, call this function to set the packet length type, the default is the
 *              normal length.If it is powered on to send a normal-length packet, this function does not need to be called.
 * @param[in]   mode - Packet length mode.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_tx_pkt_len_mode(rf_pkt_len_mode_e mode)
{
    reg_rf_tx_hlen_mode = mode;
}

/**
 * @brief       This function is used to set rx  package of ordinary length format or a certain hyper length format package.
 *              After configuring the RF initialization, call this function to configure the length mode of the receiving
 *              packet before entering the packet receiving mode. The default is the normal length. If it is powered on to
 *              receive a normal length packet, this function does not need to be called.
 * @param[in]   mode - Packet length mode.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_rx_pkt_len_mode(rf_pkt_len_mode_e mode)
{
    reg_rf_burst_size = ((reg_rf_burst_size & (~FLD_RF_RX_HLEN_MODE)) | (mode << 2));
}

/**
 * @brief       This function is mainly used to enable the sending and receiving of BIS/CIS format packets.After initializing the RF,
 *              you can choose whether you need to call this function according to whether you need to send or receive a BIS/CIS format
 *              packet. Power-on initialization RF defaults to receive and send packets in common format.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:These two package formats do not support AOA and AOD functions temporarily.
*               TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_bis_cis_en(void)
{
    BM_SET(reg_rf_rxtmaxlen1, FLD_RF_RX_ISO_PDU);
}

/**
 * @brief       This function disables the new BIS and CIS packet formats in core5.2.
 *              After enabling the BIS/CIS function, if you want to resume sending packets in the normal format, you need to call this
 *              function to turn off the BIS/CIS function.
 * @param[in]   none.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_bis_cis_dis(void)
{
    BM_CLR(reg_rf_rxtmaxlen1, FLD_RF_RX_ISO_PDU);
}

/****************************************************************************************************************************************
 *                                         RF : AOA/AOD related functions                                                               *
 ****************************************************************************************************************************************/

/**
 * @brief       This function enables the sending and receiving functions of AOA/AOD in ordinary format packets or ADV format packets.
 *              After configuring the RF function, if you want to send or receive a packet with AOA/AOD information, you can call this
 *              function to make the chip enter the corresponding mode to receive or send the packet. The default state is a normal
 *              package without AOA/AOD information.
 * @param[in]   mode - AOA/AOD broadcast package or normal package trx mode.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_aoa_aod_set_trx_mode(rf_aoa_aod_mode_e mode)
{
    reg_rf_rxsupp = ((reg_rf_rxsupp & 0xf0) | mode);
}

/**
 * @brief       This function is used to calibrate AOA, AOD sampling frequency offset.This function is mainly used to set the position
 *              of iq data sampling point in sample slot to optimize the sampling data result. By default, sampling is performed at the
 *              middle position of iq sample slot, and the sampling point is 0.125us ahead of time for each decrease of 1 code.
 *              Each additional code will move the sampling point back by 0.125us
 * @param[in]   samp_locate:Compare the parameter with the default value, reduce one code to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_aoa_aod_sample_point_adjust(unsigned char samp_locate)
{
    reg_rf_samp_offset = samp_locate;
}

/**
 * @brief       This function is used to set the position of the first antenna switch after the reference.The default is in the middle of the
 *              first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code.
 *              Each additional code will move the switch point back by 0.125us
 * @param[in]   swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_aoa_rx_ant_switch_point_adjust(unsigned short swt_offset);


/**
 * @brief       This function is used to set the position of the first antenna switch after the AOD transmitter reference.The default is in the middle of the
 *              first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code. Each additional code will move
 *              the switch point back by 0.125us
 * @param[in]   swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_aod_tx_ant_switch_point_adjust(unsigned short swt_offset);


/**
 * @brief       This function is mainly used to set the IQ data sample interval time. In normal mode, the sampling interval of AOA is 4us, and AOD will judge whether
 *              the sampling interval is 4us or 2us according to CTE info.The 4us/2us sampling interval corresponds to the 2us/1us slot mode stipulated in the protocol.
 *              Since the current hardware only supports the antenna switching interval of 4us/2us, setting the sampling interval to 1us or less will cause multiple
 *              sampling at the interval of one antenna switching. Therefore, the sampling data needs to be processed by the upper layer according to the needs, and
 *              currently it is mostly used Used in the debug process.
 *              After configuring RF, you can call this function to configure slot time.
 * @param[in]   time_us - AOA or AOD slot time mode.
 * @return      none.
 * @note        Attention:(1)When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling data to overflow.
 *                        (2)Since only the antenna switching interval of 4us/2us is supported, the sampling interval of 1us and shorter time intervals
 *                            will be sampled multiple times in one antenna switching interval. Suggestions can be used according to specific needs.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_aoa_aod_sample_interval_time(rf_aoa_aod_sample_interval_time_e time_us);

/**
 * @brief       This function is mainly used to set the type of AOA/AODiq data. The default data type is 8bit. This configuration can be done before starting to receive
 *              the package.
 * @param[in]   mode    - The length of each I or Q data.
 * @return      none.
 * @note        Attention :When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the sampling data to overflow.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_aoa_aod_iq_data_mode(rf_aoa_aod_iq_data_mode_e mode)
{
    reg_rf_sof_offset = ((reg_rf_sof_offset & (~FLD_RF_SUPP_MODE)) | (mode << 4));
}

static inline void rf_bb_dbg_setting(void)
{
    REG_ADDR8(0x140C00 + 0x100) |= (BIT(1) | BIT(2));
    gpio_function_dis(GPIO_PA0);                 // PA[0]: TX_EN
    gpio_function_dis(GPIO_PA1);                 // PA[1]: TX_ON
    gpio_function_dis(GPIO_PA2);                 // PA[2]: RX_EN
    gpio_function_dis(GPIO_PA3);                 // PA[3]: bb clk
    gpio_function_dis(GPIO_PA4);                 // PA[4]: RX_HIT_SYNC
    gpio_function_dis(GPIO_PA6);                 // PA[6]: TX_DATA
    gpio_function_dis(GPIO_PB0);                 // PB[0]: RX_vld
    gpio_function_dis(GPIO_PB1);                 // PB[1]: RX_DATA
    gpio_function_dis(GPIO_PB5);                 // PB5[5]: linklayer_tx_ss [0]
    gpio_function_dis(GPIO_PB6);                 // PB6[6]: linklayer_tx_ss [1]
    gpio_function_dis(GPIO_PB7);                 // PB7[7]: linklayer_tx_ss [2]
    gpio_function_dis(GPIO_PD2);                 // PD2[2]: linklayer_rx_ss [0]
    gpio_function_dis(GPIO_PD3);                 // PD3[3]: linklayer_rx_ss [1]
    gpio_function_dis(GPIO_PD4);                 // PD4[4]: linklayer_rx_ss [2]
    gpio_function_dis(GPIO_PD7);                 // PD7[7]: DMA err

    gpio_set_mux_function(GPIO_FC_PA0, DBG_BB0); // PA[0]: TX_EN
    gpio_set_mux_function(GPIO_FC_PA1, DBG_BB0); // PA[1]: TX_ON
    gpio_set_mux_function(GPIO_FC_PA2, DBG_BB0); // PA[2]: RX_EN
    gpio_set_mux_function(GPIO_FC_PA3, DBG_BB0); // PA[3]: bb clk
    gpio_set_mux_function(GPIO_FC_PA4, DBG_BB0); // PA[4]: RX_HIT_SYNC
    gpio_set_mux_function(GPIO_FC_PA6, DBG_BB0); // PA[6]: TX_DATA
    gpio_set_mux_function(GPIO_FC_PB0, DBG_BB0); // PB[0]: RX_vld
    gpio_set_mux_function(GPIO_FC_PB1, DBG_BB0); // PB[1]: RX_DATA
    gpio_set_mux_function(GPIO_FC_PB5, DBG_BB0); // PB5[5]: linklayer_tx_ss [0]
    gpio_set_mux_function(GPIO_FC_PB6, DBG_BB0); // PB6[6]: linklayer_tx_ss [1]
    gpio_set_mux_function(GPIO_FC_PB7, DBG_BB0); // PB7[7]: linklayer_tx_ss [2]
    gpio_set_mux_function(GPIO_FC_PD2, DBG_BB0); // PD2[2]: linklayer_rx_ss [0]
    gpio_set_mux_function(GPIO_FC_PD3, DBG_BB0); // PD3[3]: linklayer_rx_ss [1]
    gpio_set_mux_function(GPIO_FC_PD4, DBG_BB0); // PD4[4]: linklayer_rx_ss [2]
    gpio_set_mux_function(GPIO_FC_PD7, DBG_BB0); // PD7[7]: DMA err
}
#endif


#endif
