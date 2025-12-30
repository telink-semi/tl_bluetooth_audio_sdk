/********************************************************************************************************
 * @file    lin_reg.h
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
#ifndef LIN_REG_H
#define LIN_REG_H
#include "soc.h"

/******************************* lin0 registers: 0x140500 *******************************/
/******************************* lin1 registers: 0x240140 *******************************/

#define lin_base_addr(i)             (((i) == 0) ? (0x140500) : (0x240140)) /* lin0, lin1 */

#define reg_lin_data_buf(i, j)       REG_ADDR8(lin_base_addr(i) + (j))      /* lin[0-1] buf[0-3] */
#define reg_lin_data_hword_buf(i, j) REG_ADDR16(lin_base_addr(i) + (j) * 2) /* lin[0-1] buf[0-1] */
#define reg_lin_data_word_buf(i)     REG_ADDR32(lin_base_addr(i))           /* lin[0-1] */

#define reg_lin_uart_clk_div(i)      REG_ADDR16(lin_base_addr(i) + 0x04)

enum
{
    FLD_LIN_UART_CLK_DIV    = BIT_RNG(0, 14),
    FLD_LIN_UART_CLK_DIV_EN = BIT(15)
};

#define reg_lin_uart_ctrl0(i) REG_ADDR8(lin_base_addr(i) + 0x06)

enum
{
    FLD_LIN_UART_BPWC_O           = BIT_RNG(0, 3),
    FLD_LIN_UART_RX_CLR_EN        = BIT(4),
    FLD_LIN_UART_NDMA_RXDONE_EN   = BIT(5), /**< just for ndma mode,dma mode must close. */
    FLD_LIN_UART_RXTIMEOUT_RTS_EN = BIT(6),
    FLD_LIN_UART_S7816_EN         = BIT(7),
};

#define reg_lin_uart_ctrl1(i) REG_ADDR8(lin_base_addr(i) + 0x07)

enum
{
    FLD_LIN_UART_TX_CTS_POLARITY = BIT(0),
    FLD_LIN_UART_TX_CTS_ENABLE   = BIT(1),
    FLD_LIN_UART_PARITY_ENABLE   = BIT(2),
    FLD_LIN_UART_PARITY_POLARITY = BIT(3),        /**< 1:odd parity   0:even parity. */
    FLD_LIN_UART_STOP_SEL        = BIT_RNG(4, 5), /**< 0:1bit  1:1.5bit  other:2bits */
    FLD_LIN_UART_TTL_ENABLE      = BIT(6),
    FLD_LIN_UART_LOOPBACK_O      = BIT(7),
};

#define reg_lin_uart_ctrl2(i) REG_ADDR8(lin_base_addr(i) + 0x08)

enum
{
    FLD_LIN_UART_RTS_TRIQ_LEV = BIT_RNG(0, 3),
    FLD_LIN_UART_RTS_POLARITY = BIT(4),
    FLD_LIN_UART_RTS_MANUAL_V = BIT(5),
    FLD_LIN_UART_RTS_MANUAL_M = BIT(6),
    FLD_LIN_UART_RTS_EN       = BIT(7),
};

#define reg_lin_uart_ctrl3(i) REG_ADDR8(lin_base_addr(i) + 0x09)

enum
{
    FLD_LIN_UART_RX_IRQ_TRIQ_LEV = BIT_RNG(0, 3),
    FLD_LIN_UART_TX_IRQ_TRIQ_LEV = BIT_RNG(4, 7),
};

#define reg_lin_uart_rx_timeout0(i) REG_ADDR8(lin_base_addr(i) + 0x0a)

enum
{
    FLD_LIN_UART_TIMEOUT_BW = BIT_RNG(0, 7),
};

#define reg_lin_uart_rx_timeout1(i) REG_ADDR8(lin_base_addr(i) + 0x0b)
#define reg_lin_uart_mask(i)        REG_ADDR8(lin_base_addr(i) + 0x0b)

enum
{
    FLD_LIN_UART_TIMEOUT_MUL  = BIT_RNG(0, 1),
    FLD_LIN_UART_MASK_RX_IRQ  = BIT(2),
    FLD_LIN_UART_MASK_TX_IRQ  = BIT(3),
    FLD_LIN_UART_MASK_RXDONE  = BIT(4),
    FLD_LIN_UART_MASK_TXDONE  = BIT(5),
    FLD_LIN_UART_MASK_ERR_IRQ = BIT(6),
    FLD_LIN_UART_MASK_ALL     = BIT_RNG(2, 6)
};

#define reg_lin_uart_buf_cnt(i) REG_ADDR8(lin_base_addr(i) + 0x0c)

enum
{
    FLD_LIN_UART_RX_BUF_CNT = BIT_RNG(0, 3),
    FLD_LIN_UART_TX_BUF_CNT = BIT_RNG(4, 7),
};

#define reg_lin_uart_status(i) REG_ADDR8(lin_base_addr(i) + 0x0d)

enum
{
    FLD_LIN_UART_RBCNT  = BIT_RNG(0, 2),
    FLD_LIN_UART_IRQ_O  = BIT(3),
    FLD_LIN_UART_WBCNT  = BIT_RNG(4, 6),
    FLD_LIN_UART_RXDONE = BIT(7),
};

#define reg_lin_uart_irq(i) REG_ADDR8(lin_base_addr(i) + 0x0e)

enum
{
    FLD_LIN_UART_RX_MEM_CNT = BIT_RNG(0, 1),
    FLD_LIN_UART_RX_BUF_IRQ = BIT(2),
    FLD_LIN_UART_TX_BUF_IRQ = BIT(3),
    FLD_LIN_UART_RXDONE_IRQ = BIT(4),
    FLD_LIN_UART_TXDONE_IRQ = BIT(5),
    FLD_LIN_UART_RX_ERR_IRQ = BIT(6),
    FLD_LIN_UART_TIMEOUT    = BIT(7),
};

/**
 * @brief state machine use for IC debug.
 */
#define reg_lin_uart_state(i) REG_ADDR8(lin_base_addr(i) + 0x0f)

enum
{
    FLD_LIN_UART_TSTATE_i = BIT_RNG(0, 2), /**< only for dma default 1.*/
    FLD_LIN_UART_RX_FULL  = BIT(3),
    FLD_LIN_UART_RSTATE_i = BIT_RNG(4, 7),
};

#define reg_lin_uart_ctrl4(i) REG_ADDR8(lin_base_addr(i) + 0x10)

enum
{
    FLD_LIN_UART_RXDONE_RTS_EN         = BIT(0),
    FLD_LIN_UART_TIMEOUT_EN            = BIT(1),
    FLD_LIN_UART_RX_TIMEOUT_RELOAD_SEL = BIT(2),
    FLD_LIN_UART_RTS_STOP_TIMEOUT_EN   = BIT(3),
    FLD_LIN_UART_PEM_EVENT_EN          = BIT(4),
    FLD_LIN_UART_EN                    = BIT(6),
};

#define reg_lin_uart_rxtimeout_o_exp(i) REG_ADDR8(lin_base_addr(i) + 0x11)

#define reg_lin_uart_pem_task_en(i)     REG_ADDR8(lin_base_addr(i) + 0x12)

enum
{
    FLD_LIN_UART_PEM_TASK_EN = BIT_RNG(0, 4),
};

#define reg_lin_ctrl0(i) REG_ADDR8(lin_base_addr(i) + 0x13)

enum
{
    FLD_LIN_COMM_EN_RW           = BIT(0),
    FLD_LIN_RESP_EN_RW           = BIT(1),
    FLD_LIN_RATE_AUTO_RW         = BIT(2),
    FLD_LIN_PID_REG_EN_RW        = BIT(3), /* 1:using reg pid, 0:using txbuf pid */
    FLD_LIN_TXBK_CHECK_EN_RW     = BIT(4), /* 1:lin node check tx data by rx lin-bus */
    FLD_LIN_PID_PUT_FIFO_RW      = BIT(5), /* 1:lin rx-pid put-into rx-fifo */
    FLD_LIN_CHECKSUM_PUT_FIFO_RW = BIT(6), /* 1:lin rx checksum put-into rx-fifo */
    FLD_TX_FIFO_BUF_AUTO_CLR_RW  = BIT(7)
};

#define reg_lin_ctrl1(i) REG_ADDR8(lin_base_addr(i) + 0x14)

enum
{
    FLD_LIN_BREAK_LEN_RW             = BIT_RNG(0, 1), /* 0:13 1:14 2:15 3:16 */
    FLD_LIN_BREAK_1_LEN_RW           = BIT_RNG(2, 3), /* Hold high level after break, 0:1 1:2 2:3 */
    FLD_LIN_T_BASE_SEL_RW            = BIT(4),        /* 1:10ms  0:5ms */
    FLD_LIN_COMM_TX_LOOP_RW          = BIT(5),        /* 1:comm tx loop, 0:comm tx once */
    FLD_LIN_SCHEDULE_FLT_PID_EN_RW   = BIT(6),        /* using schedule_cnt buf mode */
    FLD_LIN_SCHEDULE_DLY_TABLE_EN_RW = BIT(7)         /* using schedule_cnt buf mode */
};

#define reg_lin_cmd(i) REG_ADDR8(lin_base_addr(i) + 0x15)

enum
{
    FLD_LIN_ST_CMD_RW          = BIT_RNG(0, 1), /* 0:init, 1:run , 2:sleep */
    FLD_LIN_COMM_HEADER_ACT_W  = BIT(2),        /* 0:comm no act ,1:comm tx header */
    FLD_LIN_T_BASE_MAN_CLR_W   = BIT(3),        /* t_base_timer clear , re-counting from 0ms */
    FLD_LIN_COMM_FORCE_RUN_W   = BIT(4),        /* 1:abort current action, re-run now */
    FLD_LIN_RESPONSE_DISCARD_W = BIT(5),        /*1:discard response, not keep in buf */
    FLD_LIN_TX_WAKE_UP_SIG_W   = BIT(6)         /*1:tx wake_up signal*/
};

#define reg_lin_resp1(i) REG_ADDR8(lin_base_addr(i) + 0x16)

enum
{
    FLD_LIN_TXST_RX_BUF_AUTO_CLR_EN_RW  = BIT(0),
    FLD_LIN_NONFLT_PID_DISCARD_EN_RW    = BIT(1),       /* no-matched flt_pid received, frame discard */
    FLD_LIN_DIAG_FRAME_CS_LEN_AUTO_RW   = BIT(2),       /*auto set frame_checksum classic, frame_len 8 */
    FLD_LIN_UART_RX_SMP_POSITION_ADJ_RW = BIT_RNG(3, 5) /*default:2(center position)*/
};

#define reg_lin_node_table(i) REG_ADDR8(lin_base_addr(i) + 0x17)

enum
{
    FLD_LIN_FLT_PID_MATCH_IDX_R = BIT_RNG(0, 3), /* read-back match filtered_pid num*/
    FLD_LIN_SCHEDULE_CUR_IDX_R  = BIT_RNG(4, 7)  /* read-back for current comm tx schedule cnt */
};

#define reg_lin_frame0(i) REG_ADDR8(lin_base_addr(i) + 0x18)

enum
{
    FLD_LIN_PID_VOL = BIT_RNG(0, 5), /* comm_mode can set pid, resp mode can read pid */
};

#define reg_lin_frame1(i) REG_ADDR8(lin_base_addr(i) + 0x19)

enum
{
    FLD_LIN_CHECKSUM_TYPE_RW    = BIT(0),        /* 1:enhanced  0:classic */
    FLD_LIN_CHECKSUM_SW_RW      = BIT(1),        /* 1:checksum by software */
    FLD_LIN_FRAME_DIRECTION_VOL = BIT(2),        /* 1:tx   0:rx */
    FLD_LIN_FRAME_LEN_RW        = BIT_RNG(4, 6), /* 0~7 map 1~8 */
    FLD_LIN_FRAME_LEN_TRIG_W    = BIT(7),        /* len update trig */
};

/* If lin_checksum_sw == 1 :checksum write/read validation by software;
   If lin_checksum_sw == 0 :checksum calculated by hardware, can be read
*/
#define reg_lin_frame2(i) REG_ADDR8(lin_base_addr(i) + 0x1a)

enum
{
    FLD_LIN_CHECKSUM_VOL = BIT_RNG(0, 7)
};

#define reg_comm0(i) REG_ADDR8(lin_base_addr(i) + 0x1b)

enum
{
    FLD_LIN_COMM_FRAME_CNT_RW  = BIT_RNG(0, 5), /* frame slot t_base count, set 0 in adjacent mode */
    FLD_LIN_FRMAE_SLOT_AUTO_RW = BIT(6),        /* 1:frame slot auto by nearest t_base, 0:by slot_cnt or table */
};

#define reg_lin_ctrl2(i) REG_ADDR8(lin_base_addr(i) + 0x1c)

enum
{
    FLD_LIN_T_BASE_CNT_L_RW = BIT_RNG(0, 7), /* 5ms t_base_cnt = 12e4 / uart_clkdiv */
};

#define reg_lin_ctrl3(i) REG_ADDR8(lin_base_addr(i) + 0x1d)

enum
{
    FLD_LIN_T_BASE_CNT_H_RW  = BIT_RNG(0, 2), /* 5ms t_base_cnt = 12e4 / uart_clkdiv */
    FLD_LIN_TIMEOUT_CNT_H_RW = BIT_RNG(3, 7)  /* timeout for lin bus 1 period */
};

#define reg_lin_ctrl4(i) REG_ADDR8(lin_base_addr(i) + 0x1e)

enum
{
    FLD_LIN_TIMEOUT_CNT_L_RW = BIT_RNG(0, 7), /* timeout for lin bus 1 period, counter @ clk_div */
};

#define reg_lin_node_fsm(i) REG_ADDR8(lin_base_addr(i) + 0x1f)

enum
{
    FLD_LIN_RESP_FSM_R = BIT_RNG(0, 3), /* lin_resp FSM rd */
    FLD_LIN_COMM_FSM_R = BIT_RNG(4, 6)  /* lin_comm FSM rd */
};

/* Lin irq status, W1C */
#define reg_lin_status(i) REG_ADDR16(lin_base_addr(i) + 0x20)

typedef enum
{
    FLD_LIN_REC_HIT_NON_FLT_PID_W1C   = BIT(0),
    FLD_LIN_REC_HIT_PID_ERR_W1C       = BIT(1),
    FLD_LIN_RESP_TX_DONE_W1C          = BIT(2),
    FLD_LIN_RESP_RX_DONE_W1C          = BIT(3),
    FLD_LIN_RESP_CHECKSUM_ERR_W1C     = BIT(4),
    FLD_LIN_FRAME_ERR_W1C             = BIT(5),
    FLD_LIN_TX_RETURN_BYTE_ERR_W1C    = BIT(6),
    FLD_LIN_HIT_FLT_PID_TX_W1C        = BIT(7),
    FLD_LIN_REC_HIT_PID_W1C           = BIT(8),
    FLD_LIN_COMM_HIT_PID_W1C          = BIT(9),  /* Tx header done */
    FLD_LIN_RESP_TIMEOUT_W1C          = BIT(10), /* Subscriber-node(comm) receive resp timeout */
    FLD_LIN_HEADER_TIMEOUT_W1C        = BIT(11), /* Resp-node match break, wait header timeout */
    FLD_LIN_COMM_FRAME_SLOT_DONE_W1C  = BIT(12), /* Hit frame slot */
    FLD_LIN_COMM_HIT_SCHEDULE_END_W1C = BIT(13), /* Comm tx frame num hit schedule set */
    FLD_LIN_REC_NEW_BREAK_W1C         = BIT(14), /* resp_task detect another break_field */
    FLD_LIN_BUS_INACTIVE_W1C          = BIT(15)  /* 4~10s lin-bus inactive, sleep indicator */
} lin_irq_status_e;

/* Lin irq mask */
#define reg_lin_irq(i) REG_ADDR16(lin_base_addr(i) + 0x22)

enum
{
    FLD_LIN_MASK_REC_NON_FLT_PID_RW      = BIT(0),  /* Hit_non_flt_pid, no err */
    FLD_LIN_MASK_REC_HEADER_ERR_RW       = BIT(1),  /* Pid parity check error */
    FLD_LIN_MASK_RESP_TX_DONE_RW         = BIT(2),  /* Resp tx data done */
    FLD_LIN_MASK_RESP_RX_DONE_RW         = BIT(3),  /* Resp rx data done */
    FLD_LIN_MASK_RESP_CHECKSUM_ERR_RW    = BIT(4),  /* Resp rx checksum err */
    FLD_LIN_MASK_FRAME_ERR_RW            = BIT(5),  /* Uart frame err, check stop bit */
    FLD_LIN_MASK_TX_RETURN_BYTE_ERR_RW   = BIT(6),  /* Tx data bit check from rx lin-bus */
    FLD_LIN_MASK_FLT_PID_TX_RW           = BIT(7),  /* Hit_flt_pid, the pid set tx */
    FLD_LIN_MASK_REC_HEADER_RW           = BIT(8),  /* Hit_pid, no err */
    FLD_LIN_MASK_COMM_HEADER_DONE_RW     = BIT(9),  /* Comm tx header done */
    FLD_LIN_MASK_RESP_TIMEOUT_RW         = BIT(10), /* Resp_timeout: no resp or no checksum */
    FLD_LIN_MASK_HEADER_TIMEOUT_RW       = BIT(11), /* Resp-node match break, wait header timeout */
    FLD_LIN_MASK_COMM_FRAME_SLOT_DONE_RW = BIT(12), /* Hit frame slot */
    FLD_LIN_MASK_COMM_SCHEDULE_END_RW    = BIT(13),
    FLD_LIN_MASK_REC_NEW_BREAK_RW        = BIT(14),
    FLD_LIN_MASK_BUS_INACTIVE_RW         = BIT(15), /* 4~10s lin-bus inactive, sleep indicator */
    FLD_LIN_MASK_ALL_RW                  = BIT_RNG(0, 15)
};

/* Selected by FIT_PID_CTRL[0:1], 16 filter pid = 4 pid_filter * 4 group_sel */
#define reg_lin_pid_filter(i, j) REG_ADDR8(lin_base_addr(i) + 0x24 + (j))
#define reg_lin_pid_filter0(i)   REG_ADDR8(lin_base_addr(i) + 0x24)
#define reg_lin_pid_filter1(i)   REG_ADDR8(lin_base_addr(i) + 0x25)
#define reg_lin_pid_filter2(i)   REG_ADDR8(lin_base_addr(i) + 0x26)
#define reg_lin_pid_filter3(i)   REG_ADDR8(lin_base_addr(i) + 0x27)

enum
{
    FLD_LIN_FILTERED_FRAME_ID_RW = BIT_RNG(0, 5),
    FLD_LIN_FILTERED_PID_TX_RW   = BIT(6) /* 1:hit_flt_pid response tx */
};

#define reg_lin_pid_fil_en(i) REG_ADDR16(lin_base_addr(i) + 0x28)

typedef enum
{
    FLD_LIN_FLT_PID_EN0_RW  = BIT(0),
    FLD_LIN_FLT_PID_EN1_RW  = BIT(1),
    FLD_LIN_FLT_PID_EN2_RW  = BIT(2),
    FLD_LIN_FLT_PID_EN3_RW  = BIT(3),
    FLD_LIN_FLT_PID_EN4_RW  = BIT(4),
    FLD_LIN_FLT_PID_EN5_RW  = BIT(5),
    FLD_LIN_FLT_PID_EN6_RW  = BIT(6),
    FLD_LIN_FLT_PID_EN7_RW  = BIT(7),
    FLD_LIN_FLT_PID_EN8_RW  = BIT(8),
    FLD_LIN_FLT_PID_EN9_RW  = BIT(9),
    FLD_LIN_FLT_PID_EN10_RW = BIT(10),
    FLD_LIN_FLT_PID_EN11_RW = BIT(11),
    FLD_LIN_FLT_PID_EN12_RW = BIT(12),
    FLD_LIN_FLT_PID_EN13_RW = BIT(13),
    FLD_LIN_FLT_PID_EN14_RW = BIT(14),
    FLD_LIN_FLT_PID_EN15_RW = BIT(15)
} lin_flt_pid_en_e;

#define reg_lin_fit_pid_len(i) REG_ADDR16(lin_base_addr(i) + 0x2a)

enum
{
    FLD_LIN_FLT_PID0_LEN_RW = BIT_RNG(0, 2),
    FLD_LIN_FLT_PID1_LEN_RW = BIT_RNG(4, 6),
    FLD_LIN_FLT_PID2_LEN_RW = BIT_RNG(8, 10),
    FLD_LIN_FLT_PID3_LEN_RW = BIT_RNG(12, 14)
};

/* Selected by SCHEDULE_GROUP_SEL[0:1], 16 schedules = 4 schedule_reg * 4 group_sel */
#define reg_lin_schedule(i, j) REG_ADDR8(lin_base_addr(i) + 0x2c + (j))
#define reg_lin_schedule0(i)   REG_ADDR8(lin_base_addr(i) + 0x2c)
#define reg_lin_schedule1(i)   REG_ADDR8(lin_base_addr(i) + 0x2d)
#define reg_lin_schedule2(i)   REG_ADDR8(lin_base_addr(i) + 0x2e)
#define reg_lin_schedule3(i)   REG_ADDR8(lin_base_addr(i) + 0x2f)

enum
{
    FLD_LIN_COMM_SCHEDULE_DLY_BUF_RW = BIT_RNG(0, 4)
};

#define reg_lin_flt_pid_ctrl(i) REG_ADDR8(lin_base_addr(i) + 0x30)

enum
{
    FLD_LIN_FLT_PID_BUF_GRP_SEL_RW   = BIT_RNG(0, 1), /* 0~3, sel filtered_pid_group, total 16 pid_buf */
    FLD_LIN_FLT_PID_BUF_WR_EN_RW     = BIT(2),
    FLD_LIN_FLT_PID_BUF_CLR_W        = BIT(3),        /* clear filter_pid_buf, set 1 */
    FLD_LIN_COMM_SCHEDULE_GRP_SEL_RW = BIT_RNG(4, 5), /* 0~3,total 16 schedule buf */
    FLD_LIN_SCHEDULE_CLR_W           = BIT(6),        /* clear all schedule buf cnt */
    FLD_LIN_COMM_SCHEDULE_IDX_RST_W  = BIT(7)         /* schedule buf curr_index reset */
};

#define reg_lin_comm1(i) REG_ADDR8(lin_base_addr(i) + 0x31)

enum
{
    FLD_LIN_SCHEDULE_LOOP_START_IDX_RW = BIT_RNG(0, 3), /* Schedule dly cnt lookup-table config start-index */
    FLD_LIN_SCHEDULE_LOOP_END_IDX_RW   = BIT_RNG(4, 7)  /* Schedule dly cnt lookup-table config end-index */
};

#define reg_lin_bus0(i) REG_ADDR8(lin_base_addr(i) + 0x32)

enum
{
    FLD_LIN_TX_WAKE_UP_TIME_RW = BIT_RNG(0, 7)
};

#define reg_lin_bus1(i) REG_ADDR8(lin_base_addr(i) + 0x33)

enum
{
    FLD_LIN_BUS_INACTIVE_TIME_CNT_L_RW = BIT_RNG(0, 7)
};

#define reg_lin_bus2(i) REG_ADDR8(lin_base_addr(i) + 0x34)

enum
{
    FLD_LIN_BUS_INACTIVE_TIME_CNT_M_RW = BIT_RNG(0, 7)
};

#define reg_lin_bus3(i) REG_ADDR8(lin_base_addr(i) + 0x35)

enum
{
    FLD_LIN_BUS_INACTIVE_TIME_CNT_H_RW = BIT_RNG(0, 1),
    FLD_LIN_COMM_TX_WAIT_CNT_RW        = BIT_RNG(2, 6)
};


#endif
