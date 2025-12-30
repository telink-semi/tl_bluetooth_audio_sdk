/********************************************************************************************************
 * @file    lin.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page LIN
 *
 *  Introduction
 *  ===============
 *  tl322x contain two LIN module.
 *
 *  API Reference
 *  ===============
 *  Header File: lin.h
 */

#ifndef DRIVERS_LIN_H_
#define DRIVERS_LIN_H_

#include "gpio.h"
#include "reg_include/lin_reg.h"

typedef enum
{
    LIN0 = 0,
    LIN1,
    LIN_NUM,
} lin_num_e;

typedef enum
{
    LIN_MASTER_NODE = 0,
    LIN_SLAVE_NODE,
} lin_role_e;

typedef enum
{
    LIN_TIME_BASE_5MS = 0,
    LIN_TIME_BASE_10MS
} lin_time_base_e;

typedef enum
{
    LIN_BREAK13 = 0,
    LIN_BREAK14,
    LIN_BREAK15,
    LIN_BREAK16,
} lin_break_len_e;

typedef enum
{
    LIN_BREAK1_1 = 0,
    LIN_BREAK1_2,
    LIN_BREAK1_3
} lin_break1_len_e;

typedef enum
{
    LIN_STATE_INIT = 0,
    LIN_STATE_RUN,
    LIN_STATE_SLEEP,
} lin_state_e;

typedef enum
{
    SCHEDULE_GROUP1   = 0,
    FILTER_PID_GROUP1 = 0,
    SCHEDULE_GROUP2   = 1,
    FILTER_PID_GROUP2 = 1,
    SCHEDULE_GROUP3   = 2,
    FILTER_PID_GROUP3 = 2,
    SCHEDULE_GROUP4   = 3,
    FILTER_PID_GROUP4 = 3,
} lin_group_sel_e;

/* For schedule table and filter table */
typedef enum
{
    LIN_FRAME1 = 0,
    LIN_FRAME2,
    LIN_FRAME3,
    LIN_FRAME4,
    LIN_FRAME5,
    LIN_FRAME6,
    LIN_FRAME7,
    LIN_FRAME8,
    LIN_FRAME9,
    LIN_FRAME10,
    LIN_FRAME11,
    LIN_FRAME12,
    LIN_FRAME13,
    LIN_FRAME14,
    LIN_FRAME15,
    LIN_FRAME16
} lin_frame_index;

typedef enum
{
    LIN_CHECKSUM_TYPE_CLASSIC = 0,
    LIN_CHECKSUM_TYPE_ENHANCED
} lin_checksum_type_e;

typedef enum
{
    LIN_FRAME_LEN1 = 0,
    LIN_FRAME_LEN2 = 1,
    LIN_FRAME_LEN3 = 2,
    LIN_FRAME_LEN4 = 3,
    LIN_FRAME_LEN5 = 4,
    LIN_FRAME_LEN6 = 5,
    LIN_FRAME_LEN7 = 6,
    LIN_FRAME_LEN8 = 7
} lin_frame_len_e;

typedef enum
{
    LIN_RX_SMP_POS_CENTER_LEFT2  = 0,
    LIN_RX_SMP_POS_CENTER_LEFT1  = 1,
    LIN_RX_SMP_POS_CENTER        = 2,
    LIN_RX_SMP_POS_CENTER_RIGHT1 = 3,
    LIN_RX_SMP_POS_CENTER_RIGHT2 = 4,
} lin_rx_smp_pos_e;

typedef struct
{
    lin_break_len_e  break_len;
    lin_break1_len_e break1_len;
    lin_time_base_e  time_base;
    lin_rx_smp_pos_e rx_smp_pos;
    unsigned char    jitter_us;
    unsigned short   inactive_timeout_ms;
} lin_hw_cfg_t;

typedef struct
{
    bool      dma_mode_enable;
    dma_chn_e tx_dma_chn;
    dma_chn_e rx_dma_chn;
} lin_dma_cfg_t;

typedef struct
{
    unsigned char pid;
    unsigned char len;
    unsigned char checksum;
} lin_get_response_result_t, *lin_get_response_result_t_ptr;

/*
 * @Brief       Function definition of checksum used by LIN-Bus.
 * @Parameter   Para0: PID, Para1: data, Para2: data length, Para3: type of checksum.
 */
typedef unsigned char (*LIN_SUM_FUNC)(unsigned char, unsigned char *, unsigned char, lin_checksum_type_e);

typedef struct
{
    bool         checksum_sw_enable;
    LIN_SUM_FUNC sum_func;
} lin_checksum_cfg_t;

typedef struct
{
    lin_num_e          lin_num;
    lin_role_e         role;
    gpio_func_pin_e    en_pin;
    gpio_func_pin_e    tx_pin;
    gpio_func_pin_e    rx_pin;
    unsigned int       baudrate;
    unsigned int       pclk;
    lin_hw_cfg_t       hw_cfg;
    lin_dma_cfg_t      dma_cfg;
    lin_checksum_cfg_t checksum_cfg;
} lin_hw_init_para_t;

typedef struct
{
    unsigned char id        : 6;
    unsigned char tx_enable : 1;
    unsigned char           : 1;
    unsigned char cnt;
} lin_table_item_t;

typedef struct
{
    unsigned int baudrate;

    unsigned int uart_clk;

    unsigned int pclk;

    unsigned char  bwpc;
    unsigned char  header_bits;
    unsigned short div;

    unsigned int init_flag          : 1;
    unsigned int dma_mode_enable    : 1;
    unsigned int checksum_sw_enable : 1;
    unsigned int role               : 1;
    unsigned int check_sum_type     : 2;
    unsigned int lin_num            : 2;
    unsigned int                    : 16;

    unsigned int table_size  : 8;
    unsigned int table_index : 8;
    unsigned int tx_dma_chn  : 8;
    unsigned int rx_dma_chn  : 8;

    unsigned int en_pin : 16;
    unsigned int tx_pin : 16;
    unsigned int rx_pin : 16;

    LIN_SUM_FUNC     sum_func;
    lin_table_item_t table[16];
    unsigned int     tx_dma_buffer[3];
    unsigned int     rx_dma_buffer[3];

} lin_handle_t, *lin_handle_t_ptr;


typedef enum
{
    LIN_ACTION_TX   = 0,
    LIN_ACTION_RX   = 1,
    LIN_ACTION_NONE = 2
} lin_action_type_e;

struct lin_frame_item_t;
typedef struct lin_frame_item_t lin_frame_item_t, *lin_frame_item_t_ptr;

struct lin_schedule_t;
typedef struct lin_schedule_t lin_schedule_t, *lin_schedule_t_ptr;

struct lin_schedule_t
{
    lin_schedule_t_ptr   next_proc_schedule;
    lin_frame_item_t_ptr frame_list;
    unsigned char             item_cnt;
    unsigned char             item_index;
};

typedef void (*LIN_CALLBACK_FUNC)(void *para);

struct lin_frame_item_t
{
    lin_action_type_e  action;
    unsigned char           id;
    unsigned char           len;
    unsigned char           data[10];
    unsigned char           checksum_type;
    unsigned char           slot_cnt;
    lin_schedule_t_ptr conflict_proc_schedule;
    LIN_CALLBACK_FUNC       callback_func;
};

#define LIN_GET_ID_BIT(x,y) (((x) >> (y)) & 0x01U)

/**
 * @brief      This function serves to set state of LIN controller.
 * @param[in]  lin_num - LIN0/LIN1.
 * @param[in]  state - state of LIN controller, LIN_STATE_INIT/LIN_STATE_RUN.
 * @note       The sleep state is invalid.
 * @return     none
 */
static inline void lin_set_state(lin_num_e lin_num, lin_state_e state)
{
    reg_lin_cmd(lin_num) = (reg_lin_cmd(lin_num) & ~FLD_LIN_ST_CMD_RW) | MASK_VAL(FLD_LIN_ST_CMD_RW, state);
}

/**
 * @brief      This function serves to set the IRQ mask of LIN controller.
 * @param[in]  handle - operation handle.
 * @param[in]  mask - the mask value.
 * @return     none.
 */
static inline void lin_set_irq_mask(lin_handle_t_ptr handle, unsigned short mask)
{
    if (handle->init_flag) {
        reg_lin_irq(handle->lin_num) |= mask;
    }
}

/**
 * @brief      This function serves to clear the IRQ mask of LIN controller.
 * @param[in]  handle - operation handle.
 * @param[in]  mask - the mask value.
 * @return     none.
 */
static inline void lin_clr_irq_mask(lin_handle_t_ptr handle, unsigned short mask)
{
    if (handle->init_flag) {
        reg_lin_irq(handle->lin_num) &= ~(mask);
    }
}

/**
 * @brief      This function serves to get the irq status of LIN controller.
 * @param[in]  handle - operation handle.
 * @return     irq status.
 */
static inline unsigned short lin_get_irq_status(lin_handle_t_ptr handle)
{
    if (handle->init_flag) {
        return reg_lin_status(handle->lin_num);
    }
    return  0;
}

/**
 * @brief      This function serves to clear the status of LIN controller.
 * @param[in]  handle - operation handle.
 * @param[in]  mask - the mask value.
 * @return     none.
 */
static inline void lin_clr_irq_status(lin_handle_t_ptr handle, lin_irq_status_e mask)
{
    if (handle->init_flag) {
        reg_lin_status(handle->lin_num) = mask;
    }
}

/**
 * @brief      This function serves to trigger the action of discarding frame response.
 * @param[in]  lin_num - LIN0/LIN1.
 * @return     none
 */
static inline void lin_set_response_discard(lin_num_e lin_num)
{
    reg_lin_cmd(lin_num) |= FLD_LIN_RESPONSE_DISCARD_W;
}

/**
 * @brief      This function serves to set the frame duration.
 * @param[in]  handle - operation handle.
 * @param[in]  cnt - the count of t-base.
 * @return     none.
 */
static inline void lin_set_slot(lin_handle_t_ptr handle, unsigned char cnt)
{
    if (handle->init_flag) {
        if (cnt == 0) {
            reg_comm0(handle->lin_num) = (reg_comm0(handle->lin_num) & ~(FLD_LIN_FRMAE_SLOT_AUTO_RW)) |
                                         (MASK_VAL(FLD_LIN_FRMAE_SLOT_AUTO_RW, 1));
        } else {
            reg_comm0(handle->lin_num) = (reg_comm0(handle->lin_num) & ~(FLD_LIN_COMM_FRAME_CNT_RW | FLD_LIN_FRMAE_SLOT_AUTO_RW)) |
                                         (MASK_VAL(FLD_LIN_COMM_FRAME_CNT_RW, cnt - 1) | MASK_VAL(FLD_LIN_FRMAE_SLOT_AUTO_RW, 0));
        }
    }
}

/**
 * @brief      This function serves to get the current index of schedule table.
 * @param[in]  handle - operation handle.
 * @return     none.
 */
static inline unsigned char lin_master_get_schedule_table_cur_idx(lin_handle_t_ptr handle)
{
    return ((reg_lin_node_table(handle->lin_num) & FLD_LIN_SCHEDULE_CUR_IDX_R) >> 4);
}

/**
 * @brief      This function serves to get the current index of schedule table.
 * @param[in]  handle - operation handle.
 * @return     none.
 */
static inline unsigned char lin_slave_get_filter_table_match_idx(lin_handle_t_ptr handle)
{
    return (reg_lin_node_table(handle->lin_num) & FLD_LIN_FLT_PID_MATCH_IDX_R);
}

/**
 * @brief      This function serves to set EN PIN of LIN controller.
 * @param[in]  lin_num - LIN0/LIN1.
 * @param[in]  val - 1:enable LIN PHY; 0:disable LIN PHY
 * @return     none
 */
void lin_set_hw_en(lin_num_e lin_num, bool val);

/**
 * @brief      This function serves to generate checksum by software.
 * @param[in]  id - ID value of LIN frame.
 * @param[in]  data - data of LIN frame.
 * @param[in]  len - data length of LIN frame.
 * @param[in]  type - checksum type of LIN frame.
 * @return     none
 */
unsigned char lin_software_checksum(unsigned char id, unsigned char *data, unsigned char len, lin_checksum_type_e type);

/**
 * @brief      This function serves to get the id of frame.
 * @param[in]  handle - operation handle.
 * @return     PID value.
 */
unsigned char lin_get_id(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to send header when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @param[in]  id - the ID of frame.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_send_header(lin_handle_t_ptr handle, unsigned char id);

/**
 * @brief      This function serves to clear the schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_clr_schedule_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to initialize the schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_init_schedule_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to add item to schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @param[in]  id - the ID value of frame.
 * @param[in]  slot_cnt - the period of each frame, count of t-base.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_add_2_schedule_table(lin_handle_t_ptr handle, unsigned char id, unsigned char slot_cnt);

/**
 * @brief      This function serves to add item to schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @param[in]  id - the ID value of frame.
 * @param[in]  slot_cnt - the period of each frame, count of t-base.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_refresh_schedule_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to enable schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_enable_schedule_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to stop schedule table when acting as a LIN-master node.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_stop_schedule_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to discard response.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_discard_response(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to send response.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_send_response(lin_handle_t_ptr handle, unsigned char *data, unsigned char data_len, lin_checksum_type_e type);

/**
 * @brief      This function serves to start the action to receive response.
 * @param[in]  handle - operation handle.
 * @param[in]  data_len - data length of frame.
 * @param[in]  type - type of checksum, LIN_CHECKSUM_TYPE_CLASSIC/LIN_CHECKSUM_TYPE_ENHANCED
 * @return     The operation result, true:success, false:failed.
 */
bool lin_start_recv_response(lin_handle_t_ptr handle, unsigned char data_len, lin_checksum_type_e type);

/**
 * @brief      This function serves to start the action to receive response.
 * @param[in]  handle - operation handle.
 * @param[out] data - the data received.
 * @param[in]  data_len - the data length of frame.
 * @param[out] result - the structure to save the result data (PID,length,checksum), can be null.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_get_response_data(lin_handle_t_ptr handle, unsigned char *data, unsigned char data_len, lin_checksum_type_e type, lin_get_response_result_t_ptr result);

/**
 * @brief      This function serves to clear PID-filter table.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_slave_clr_pid_filter_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to add item to PID-filter table.
 * @param[in]  handle - operation handle.
 * @param[in]  id - the ID value of frame.
 * @param[in]  data_len - the data length of frame.
 * @param[in]  tx_enable - the enable state of TX.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_slave_add_2_pid_filter_table(lin_handle_t_ptr handle, unsigned char id, unsigned char data_len, bool tx_enable);

/**
 * @brief      This function serves to enable the PID-filter table.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_slave_enable_pid_filter_table(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to send wake-up signal.
 * @param[in]  handle - operation handle.
 * @param[in]  time_us - the signal duration.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_send_wakeup_signal(lin_handle_t_ptr handle, unsigned short time_us);

/**
 * @brief      This function serves to reset t-base counter.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_rst_timebase(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to force re-send frame header.
 * @param[in]  handle - operation handle.
 * @return     The operation result, true:success, false:failed.
 */
bool lin_master_force_resend_header(lin_handle_t_ptr handle);

/**
 * @brief      This function serves to initialize the hardware of LIN controller.
 * @param[in]  handle - operation handle.
 * @param[in]  para - the initialize parameter.
 * @return     The operation handle.
 */
lin_handle_t_ptr lin_hw_init(lin_hw_init_para_t *para);

#endif /* DRIVERS_LIN_H_ */
