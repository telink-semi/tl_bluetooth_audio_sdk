/********************************************************************************************************
 * @file    ipc_msg_process.h
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
#ifndef __IPC_MSG_PROCESS_H__
#define __IPC_MSG_PROCESS_H__

#if (TLK_MW_DSP_COMM_ENABLE)

/**
 * @brief   Callback function type for data process done
 */
typedef void (*ipc_msg_data_process_done_cb_t)(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief       Initialize IPC message queue
 */
void ipc_message_queue_init(void);

/**
 * @brief       Handle IPC message IRQ
 */
void d25f_ipc_message_irq_handler(void);

/**
 * @brief       Process IPC messages
 */
void d25f_process_ipc_msg(void);

/**
 * @brief       Process DSP log messages
 */
void d25f_process_dsp_log_msg(void);

/**
 * @brief       Register callback for data process done
 * @param[in]   p_callback - Callback function pointer
 * @param[in]   type - Process type
 * @param[in]   id - IPC data path ID
 */
void ipc_msg_register_data_process_done_cb(ipc_msg_data_process_done_cb_t p_callback, uint8_t type, uint8_t id);

/**
 * @brief       Get DSP version
 * @return      DSP version
 */
uint32_t tlk_GetDspVersion(void);

#endif

#endif
