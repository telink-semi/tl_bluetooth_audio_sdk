/********************************************************************************************************
 * @file    keyscan_ana.h
 *
 * @brief   This is the header file for tl322x
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
#ifndef KEYSCAN_ANA_H
#define KEYSCAN_ANA_H

#include "adc.h"
#include "dma.h"
#include "gpio.h"

#define KS_FIXED_COLS               8
#define KS_FIXED_ROWS               16

#define ANA_SWITCH_CHANNEL_NUMBER   4 //for analog switch 16 mux 1
#define HALL_ROWS_POWER_NUMBER      8 //for keyboard rows power

#define KS_FIXED_SCAN_TOTAL_KEYS    (KS_FIXED_COLS * KS_FIXED_ROWS)

/**
 * @brief analog keyscan key-value map
 */
typedef struct
{
    unsigned char key;
    unsigned char value[10];
} ks_ana_data_t;

/**
 * @brief analog keyscan hall threshold
 */
typedef struct
{
    int release_threshold;
    int press_threshold;
} ks_ana_threshold_t;

/**
 * @brief analog keyscan key status
 */
typedef struct
{
    unsigned int last_key_bits[KS_FIXED_ROWS];
    unsigned int now_key_bits[KS_FIXED_ROWS];
} ks_ana_value_t;

/**
 * @brief analog keyscan gpio
 * @note  please refer to the gpio_lookuptable, the channel pin and the power pin must in the same group
 */
typedef struct
{
    /* the pin group id */
    unsigned char pin_group_id;

    /* the enable pin for all the analog switches*/
    gpio_pin_e    ana_switch_enable_pin;

    /* the channel pin for the analog switch*/
    gpio_pin_e    ana_switch_channel_pin[ANA_SWITCH_CHANNEL_NUMBER];

    /* the power pin the halls*/
    gpio_pin_e    hall_rows_power_pin[HALL_ROWS_POWER_NUMBER];
} ks_ana_gpio_pin_t;

/**
 * @brief analog keyscan mode
 */
typedef enum {
    KEYSCAN_1XADC_8K_ONCE_MODE = 0,//For internal testing only. Not available.
    KEYSCAN_2XADC_8K_ONCE_MODE,
    KEYSCAN_2XADC_8K_TWICE_M1_MODE,//For internal testing only. Not available.
    KEYSCAN_2XADC_8K_TWICE_M2_MODE,//For internal testing only. Not available. (note: this mode will trigger twice interrupt)
    KEYSCAN_MODE_MAX,
} ks_ana_mode_e;

/**
 * @brief analog keyscan configuration
 */
typedef struct
{
    /* 
     * set adc simple rate
     * simple rate = (adc_simple_rate + 1) * (1 / clk) 
     */
    unsigned char adc_sample_rate;

    /* 
     * ks_a_setl0: set one analog switch channel scan during time
     * time = (one_channel_scan_time + 1) * (1 / clk)
     */
    unsigned char one_channel_scan_time;

    /* 
     * ks_a_setl1-2: set time from start channel switch to start adc simple (adc0-adc1)
     * time = ((2 * channel_switch_to_adc_simple_time) + 1) * (1 / clk)
     */
    unsigned char channel_switch_to_adc_sample_time[2];

    /* 
     * ks_a_setl3: set time from keyscan enable to finish once scan
     * time = (16 * ks_en_to_scan_once_time) * (1 / clk)
     */
    unsigned char ks_en_to_scan_once_time;

    /*
    * ks_a_setl5: set time from keyscan enable to start second scan
    * time = (16 * ks_en_to_second_scan_time) * (1 / clk)
    */
    unsigned char ks_en_to_second_scan_time;

    /*
    * set time from keyscan enable to power on rows (row0-row7)
    * time = (16 * ks_en_to_row_power_on_time) * (1 / clk)
    */
    unsigned char ks_en_to_row_power_on_time[8];

    /*
    * set time from keyscan enable to power down rows (row0-row7)
    * time = (16 * ks_en_to_row_power_dn_time) * (1 / clk)
    */
    unsigned char ks_en_to_row_power_dn_time[8];
} ks_ana_config_t;

/**
 * @brief analog keyscan clock source 
 */
typedef enum
{
    KS_SRC_32K       = 0x01,
    KS_8K_SRC_RC24M  = 0x23,
    KS_8K_SRC_XTL24M = 0x43,
} ks_clock_mode_e;

/**
 * @brief the data length of a key with sar_adc selection
 */
typedef enum
{
    KS_ADC0_SCAN_ONCE_DATA_LEN    = 1,
    KS_ADC0_1_SCAN_ONCE_DATA_LEN  = 1,
    KS_ADC0_1_SCAN_TWICE_DATA_LEN = 1,
} ks_saradc_data_len_e;

/**
 * @brief the data size of a scan round with sar_adc selection (half word)
 */
typedef enum
{
    KS_ADC0_SCAN_ONCE_DATA_SIZE    = 128,
    KS_ADC0_1_SCAN_ONCE_DATA_SIZE  = 128,
    KS_ADC0_1_SCAN_TWICE_DATA_SIZE = 256,
} ks_saradc_data_size_e;

/**
 * @brief the channel and power pin map
 * 
 */
typedef enum
{
    KS_PIN_GROUP_ID_0 = 0,
    KS_PIN_GROUP_ID_1,
    KS_PIN_GROUP_ID_2,
    KS_PIN_GROUP_ID_3,
    KS_PIN_GROUP_ID_TOTAL,
}ks_pin_group_e;

/**
 * @brief    This function servers to clear the keyscan irq status.
 * @return   none.
 */
static inline void ks_ana_clr_irq_status(unsigned char irq)
{
    reg_ks_a_status |= irq;
}

/**
 * @brief    This function servers to get the keyscan irq status.
 * @return   the keyscan irq status.
 */
static inline unsigned char ks_ana_get_irq_status(void)
{
    return reg_ks_a_status & 0x03;
}

/**
 * @brief      This function serves to set keyscan hall threshold.
 * @param[in]  ks_ana_threshold          - the analog keyscan hall threshold
*/
void ks_ana_set_hall_threshold(ks_ana_threshold_t ks_ana_threshold);

/**
 * @brief      This function serves to init keyscan analog module.
 * @param[in]  ks_ana_gpio_pin          - the analog keyscan gpio pins 
 * @param[in]  ks_ana_mode              - the analog keyscan mode
 * @param[in]  hall_init_time    -set time from keyscan enable to start first scan, time = (16 * hall_init_time) * (1 / clk)
 * @param[in]  ks_ana_threshold         - the analog keyscan hall threshold
 * @return     none
 */
void ks_ana_init(ks_ana_gpio_pin_t ks_ana_gpio_pin, ks_ana_mode_e ks_ana_mode, unsigned char hall_init_time, ks_ana_threshold_t ks_ana_threshold);

void ks_ana_sar_adc_set_dma_chain_llp(adc_num_e adc_chn, dma_chn_e dma_chn, unsigned short *dst_addr, unsigned int size_byte, dma_chain_config_t *head_of_list);

void ks_ana_rx_dma_add_list_element(adc_num_e adc_chn, dma_chn_e chn,  dma_chain_config_t *node, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int size_byte);

/**
 * @brief     This function serves to configure adc_dma_chn channel.
 * @param[in] adc_chn          - the ADC channel
 * @param[in] dma_chn          - the DMA channel
 * @param[in] data_buffer  - the DMA data_buffer address
 * @param[in] size_byte    - the DMA buffer size
 * @return    none
 */
void ks_ana_receive_dma(adc_num_e adc_chn, dma_chn_e dma_chn, unsigned short *data_buffer, unsigned int size_byte);

void ks_ana_set_rx_dma_config(adc_num_e adc_chn, dma_chn_e dma_chn);

void ks_ana_rx_dma_chain_init(adc_num_e adc_chn, dma_chn_e adc0_dma_chn, unsigned short *dst_addr, unsigned int size_byte);

/**
 * @brief      This function serves to disable keyscan analog module.
 * @return     none
 * @note       the disable order: ks_ana_disable-> dma_chn_dis
 */
void ks_ana_disable(void);

/**
 * @brief      This function serves to enable keyscan analog module.
 * @return     none
 * @note       the enable order: ks_ana_enable-> ks_ana_rx_dma_chain_init
 */
void ks_ana_enable(void);
#endif
