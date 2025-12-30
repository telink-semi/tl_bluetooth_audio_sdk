/********************************************************************************************************
 * @file    bredr_emi.h
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
#ifndef TLKDRV_B92_EXT_DRIVER_EMI_BT_EMI_H_
#define TLKDRV_B92_EXT_DRIVER_EMI_BT_EMI_H_

/**
 * @brief   The emi test mode.
 */
typedef enum
{
    BR_DH1   = 0,
    BR_DH3   = 1,
    BR_DH5   = 2,
    EDR2_DH1 = 3,
    EDR2_DH3 = 4,
    EDR2_DH5 = 5,
    EDR3_DH1 = 6,
    EDR3_DH3 = 7,
    EDR3_DH5 = 8,
} emi_RF_MODE_BT_e;

typedef enum
{
    BT_MODE_BR   = 0,
    BT_MODE_EDR2 = 2,
    BT_MODE_EDR3 = 3
} BT_MODE_e;

typedef enum
{
    BT_MODE_BR_DH1   = 0 | (1 << 2),
    BT_MODE_BR_DH3   = 0 | (3 << 2),
    BT_MODE_BR_DH5   = 0 | (5 << 2),
    BT_MODE_EDR2_DH1 = 2 | (1 << 2),
    BT_MODE_EDR2_DH3 = 2 | (3 << 2),
    BT_MODE_EDR2_DH5 = 2 | (5 << 2),
    BT_MODE_EDR3_DH1 = 3 | (1 << 2),
    BT_MODE_EDR3_DH3 = 3 | (3 << 2),
    BT_MODE_EDR3_DH5 = 3 | (5 << 2),
} RF_MODE_BT_e;

typedef enum
{
    BT_PKT_Data_Prbs9 = 0,
    BT_PKT_Data_0x55  = 1,
    BT_PKT_Data_0x0f  = 2,
    BT_PKT_Data_0x00  = 3,
    BT_PKT_Data_0xff
} BT_PKT_Data_type_e;

/**
 * @brief   The emi test command and function
 */
typedef struct
{
    unsigned char cmd_id;                                   /**< emi command id */
    void (*func)(RF_MODE_BT_e, unsigned char, signed char); /**< emi function */
} test_list_t;

#define BT_RXCHASS_ADDR        (RD0_ADDR + 0x08)
#define BT_RXDESC_SIZE         14

#define BT_PWR_LIST_MAX_LENGTH 50

extern void bt_emi_tx_continue_mode_run(RF_MODE_BT_e mode, signed short chn, unsigned char _scale, unsigned char _slice, BT_PKT_Data_type_e type);

extern void bt_emi_tx_burst_mode_init(RF_MODE_BT_e mode, signed short chn, unsigned char _scale, unsigned char _slice, BT_PKT_Data_type_e type);

extern void bt_emi_tx_burst_mode_loop(unsigned int *finish_times);

extern void bt_emi_tx_burst_mode_loop_hoop(unsigned int *finish_times);

extern void bt_emi_rx_mode_init(RF_MODE_BT_e mode, signed short chn);

extern void bt_emi_rx_mode_loop(unsigned int *finish_times);

extern void bt_emi_stop();

extern void bt_emi_init();

extern void bt_emi_reset_cv();

extern signed char bt_get_rssi(unsigned char index);

extern void bt_emi_single_tone(signed char rf_chn, unsigned char pwr);

void rd_emi_usr_definition(unsigned char _s);
#endif /* TLKDRV_B92_EXT_DRIVER_EMI_BT_EMI_H_ */
