/********************************************************************************************************
 * @file    bredr_dbgport.h
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
#ifndef _BREDR_DBGPORT_H_
#define _BREDR_DBGPORT_H_


#include "driver.h"

/**
 * Debug Port Usage:
 * 1. set DIAGCNTL-DIAGEN0/1/2/3 as 1 to enable DIAG0/1/2/3
 * 2. set DIAGCNTL-DIAG0/1/2/3<6:0> as target value ,refer to Table 2-46 in RW-BT-CORE-FS/2.19 Validation and test
 *    DIAG0/1/2/3STAT<0:7>  <->  DiagOut<0:7>  <-> bt_dbg0/1/2/3<0:7>
 * 3. set dbg_sel_bt0/1/2/3 <-> bt_dbg0/1/2/3<0:7> <-> DBG_BB<0:31> <-> PA<0:4> ,PB<0:7>,PC<0:7>,PD<0:7>,PE<0:2>
 *	  set dbg_sel_bt0(GPIO_BASE+0x55)<12> -> DiagOut<0:7> <-> bt_dbg0<0:7> <-> DBG_BB<0:7>
 *	  set dbg_sel_bt1(GPIO_BASE+0x55)<13> -> DiagOut<0:7> <-> bt_dbg1<0:7> <-> DBG_BB<8:15>
 *	  set dbg_sel_bt2(GPIO_BASE+0x55)<14> -> DiagOut<0:7> <-> bt_dbg2<0:7> <-> DBG_BB<16:23>
 *	  set dbg_sel_bt3(GPIO_BASE+0x55)<15> -> DiagOut<0:7> <-> bt_dbg3<0:7> <-> DBG_BB<24:31>
 * 4. set gpio mux function as DBG_BBx
 *
 * Take "radcntl_txen <-> GPIO_PA0" signal for example:
 * 1. DIAGCNTL-DIAGEN0=1 (enable DIAG0)
 * 2. DIAG0<6:0> = 0x03  (OUTPUT "radcntl_txen" signal)
 * 3. (GPIO_BASE+0x55)<12> =1 (DiagOut<0:7> <-> bt_dbg0<0:7> <-> DBG_BB<0:7> <-> PA<0:4> ,PB<0:2>)
 *    So, radcntl_txen <-> DiagOut<0> <-> bt_dbg0<0> <-> DBG_BB<0> <-> PA<0>
 * 4. 0x140300<1:0>=3 to enable GPIO MUX function as DBG_BB0
 * */


typedef enum
{
    BT_DBG0_BB0_A0 = GPIO_PA0,
    BT_DBG0_BB1_A1 = GPIO_PA1,
    BT_DBG0_BB2_A2 = GPIO_PA2,
    BT_DBG0_BB3_A3 = GPIO_PA3,
    BT_DBG0_BB4_A4 = GPIO_PA4,
    BT_DBG0_BB5_B0 = GPIO_PB0,
    BT_DBG0_BB6_B1 = GPIO_PB1,
    BT_DBG0_BB7_B2 = GPIO_PB2,

    BT_DBG1_BB8_B3  = GPIO_PB3,
    BT_DBG1_BB9_B4  = GPIO_PB4,
    BT_DBG1_BB10_B5 = GPIO_PB5,
    BT_DBG1_BB11_B6 = GPIO_PB6,
    BT_DBG1_BB12_B7 = GPIO_PB7,
    BT_DBG1_BB13_C0 = GPIO_PC0,
    BT_DBG1_BB14_C1 = GPIO_PC1,
    BT_DBG1_BB15_C2 = GPIO_PC2,

    BT_DBG2_BB16_C3 = GPIO_PC3,
    BT_DBG2_BB17_C4 = GPIO_PC4,
    BT_DBG2_BB18_C5 = GPIO_PC5,
    BT_DBG2_BB19_C6 = GPIO_PC6,
    BT_DBG2_BB20_C7 = GPIO_PC7,
    BT_DBG2_BB21_D0 = GPIO_PD0,
    BT_DBG2_BB22_D1 = GPIO_PD1,
    BT_DBG2_BB23_D2 = GPIO_PD2,

    BT_DBG3_BB24_D3 = GPIO_PD3,
    BT_DBG3_BB25_D4 = GPIO_PD4,
    BT_DBG3_BB26_D5 = GPIO_PD5,
    BT_DBG3_BB27_D6 = GPIO_PD6,
    BT_DBG3_BB28_D7 = GPIO_PD7,
    BT_DBG3_BB29_E0 = GPIO_PE0,
    BT_DBG3_BB30_E1 = GPIO_PE1,
    BT_DBG3_BB31_E2 = GPIO_PE2,
} btdbg_pin_e;

typedef enum
{
    PKTCNTL_MAIN_FSM_STS_IDLE        = 0,
    PKTCNTL_MAIN_FSM_STS_BEFORE_TX   = 1, //TX begin
    PKTCNTL_MAIN_FSM_STS_TX_AC       = 2,
    PKTCNTL_MAIN_FSM_STS_TX_HDR      = 3,
    PKTCNTL_MAIN_FSM_STS_TX_PHDR_BR  = 4,
    PKTCNTL_MAIN_FSM_STS_TX_PLD_BR   = 5,
    PKTCNTL_MAIN_FSM_STS_TX_CRC_BR   = 6,
    PKTCNTL_MAIN_FSM_STS_TX_PAD_BR   = 7,
    PKTCNTL_MAIN_FSM_STS_TX_GUARD    = 8,
    PKTCNTL_MAIN_FSM_STS_TX_SSEQ     = 9,
    PKTCNTL_MAIN_FSM_STS_TX_PHDR_EDR = 10,
    PKTCNTL_MAIN_FSM_STS_TX_PLD_EDR  = 11,
    PKTCNTL_MAIN_FSM_STS_TX_CRC_EDR  = 12,
    PKTCNTL_MAIN_FSM_STS_TX_PLD_PAD  = 13,
    PKTCNTL_MAIN_FSM_STS_TX_STATUS   = 14,
    PKTCNTL_MAIN_FSM_STS_BEFORE_RX   = 15, //RX begin
    PKTCNTL_MAIN_FSM_STS_RX_SYNC     = 16,
    PKTCNTL_MAIN_FSM_STS_RX_HDR      = 17,
    PKTCNTL_MAIN_FSM_STS_RX_PHDR_BR  = 18,
    PKTCNTL_MAIN_FSM_STS_RX_PLD_BR   = 19,
    PKTCNTL_MAIN_FSM_STS_RX_CRC_BR   = 20,
    PKTCNTL_MAIN_FSM_STS_RX_GUARD    = 21,
    PKTCNTL_MAIN_FSM_STS_RX_PHDR_EDR = 22,
    PKTCNTL_MAIN_FSM_STS_RX_PLD_EDR  = 23,
    PKTCNTL_MAIN_FSM_STS_RX_CRC_EDR  = 24,
    PKTCNTL_MAIN_FSM_STS_RX_RSSI     = 25,
} bt_pktcntl_main_fsm_sts_t;

typedef enum
{
    BT_DBG_SEL_EN   = BIT(8),
    BT_DBG_SEL_DBG0 = BIT(12),
    BT_DBG_SEL_DBG1 = BIT(13),
    BT_DBG_SEL_DBG2 = BIT(14),
    BT_DBG_SEL_DBG3 = BIT(15),
} bt_signal_group_e;

void bt_dbg_set_pin(btdbg_pin_e pin);
void bt_dbg_set_signal(bt_signal_group_e signal_group, u8 signal_byte);

void    bt_dbg_port_en(bt_signal_group_e signal_group, u8 signal_byte);
uint8_t bt_dbg_read_status(bt_signal_group_e signal_group);

void                      bt_bb_set_diag_code(uint8_t dbg_code);
bt_pktcntl_main_fsm_sts_t bt_bb_diag_code_02_main_fsm_get(void);
uint8_t                   bt_bb_diag_code_1f_get_frame_in_process(void);

#endif /* _BREDR_DBGPORT_H_ */
