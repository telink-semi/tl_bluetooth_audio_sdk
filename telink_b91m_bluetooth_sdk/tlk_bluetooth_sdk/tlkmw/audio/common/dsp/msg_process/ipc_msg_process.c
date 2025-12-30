/********************************************************************************************************
 * @file    ipc_msg_process.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#if (TLK_MW_DSP_COMM_ENABLE)

#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
#include "hal_mailbox.h"
#else
#include "mailbox.h"
#endif
#include "tlkmw/audio/tlkmw_audio.h"
#include "../drv/tlkdrv_dsp.h"

/** ipc msg queue */
static uint32_t s_msg_queue_buffer[MSG_QUEUE_NUMBER * MSG_SIZE];
static Queue_t  s_msg_queue;

/** DSP log queue */
static uint32_t s_dsp_log_queue_buffer[DSP_LOG_QUEUE_NUMBER * DSP_LOG_SIZE];
static Queue_t  s_dsp_log_queue;

ipc_msg_data_process_done_cb_t ipc_msg_data_process_done_cb[IPC_DATA_PATH_MAX] = {NULL, NULL};
uint8_t                        s_ipc_process_type[IPC_DATA_PATH_MAX];

#if JTAG_DEBUG_ENABLE
extern tlkmdi_hra_env_t s_hra_env_t;
#endif
static uint32_t gDspVersion = 0x00;

/**
 * @brief       Get DSP version
 * @return      DSP version
 */
uint32_t tlk_GetDspVersion(void)
{
    return gDspVersion;
}

/**
 * @brief       Register callback for data process done
 * @param[in]   p_callback - Callback function pointer
 * @param[in]   type - Process type
 * @param[in]   id - IPC data path ID
 */
void ipc_msg_register_data_process_done_cb(ipc_msg_data_process_done_cb_t p_callback, uint8_t type, uint8_t id)
{
    ipc_msg_data_process_done_cb[id] = p_callback;
    s_ipc_process_type[id]           = type;
}

/**
 * @brief       Initialize IPC message queue
 */
audio_ram_code void ipc_message_queue_init(void)
{
    /* Initialize the msg queue */
    queue_init(&s_msg_queue, s_msg_queue_buffer, MSG_QUEUE_NUMBER);

    queue_init(&s_dsp_log_queue, s_dsp_log_queue_buffer, DSP_LOG_QUEUE_NUMBER);
}

/**
 * @brief       Handle IPC message IRQ
 */
audio_ram_code void d25f_ipc_message_irq_handler(void)
{
    ipc_message_t msg;

    if (mailbox_get_irq_status() & FLD_MAILBOX_DSP_TO_D25F_IRQ) {
        d25f_get_ipc_message(msg.ipc_msg);
        tlksys_task_setEvtFromIsr(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_DSP);
#if JTAG_DEBUG_ENABLE
        if (s_hra_env_t.overtime_flag) {
            return;
        }
#endif
        if (MSG_SIZE < queue_get_free_size(&s_msg_queue)) {
            queue_put_buffer(&s_msg_queue, msg.ipc_msg, MSG_SIZE);
        } else {
            //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "Queue is Full");
        }

        mailbox_clr_irq_status(FLD_MAILBOX_DSP_TO_D25F_IRQ);
    }
}

/**
 * @brief       Process DSP log messages
 */
audio_ram_code void d25f_process_dsp_log_msg(void)
{
    unsigned int debug_info[2];
    //    uint8_t dsp_log[8];

    while (queue_get_buffer(&s_dsp_log_queue, debug_info, DSP_LOG_SIZE)) {
        //        dsp_log[0] = debug_info[0] & 0xff;
        //        dsp_log[1] = (debug_info[0] >> 8) & 0xff;
        //        dsp_log[2] = (debug_info[0] >> 16) & 0xff;
        //        dsp_log[3] = (debug_info[0] >> 24) & 0xff;

        //        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "dsp_log: %x %x %x %x\n",
        //                     dsp_log[0], dsp_log[1], dsp_log[2], dsp_log[3]);
    }
}

extern uint8_t dsp_ipc_flag;

/**
 * @brief       Process IPC messages
 */
audio_ram_code void d25f_process_ipc_msg(void)
{
    ipc_message_t           msg;
    uint8_t                 id = IPC_DATA_PATH_0;
    audio_buffer_context_t *p_audio_buf_ctx;

    while (queue_get_buffer(&s_msg_queue, msg.ipc_msg, MSG_SIZE)) {
        // if (msg.header.type != IPC_DSP2D25F_DATA_PROCESS_DONE) {
        //     tlkapi_printf(APP_LOG_EN, "msg.header.type: %d, 0x%x 0x%x\n", msg.header.type, msg.ipc_msg[0], msg.ipc_msg[1]);
        // }

        if (msg.header.magic != IPC_MSG_MAGIC) {
            return;
        }

        switch (msg.header.type) {
        case IPC_DSP2D25F_HANDSHAKE_DONE:
        {
            tlkdrv_dsp_bootOkCB();
            tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_DSP_RDY);
            handshake_done_t handshake_done;
            tmemcpy(&handshake_done, msg.payload, IPC_MSG_PAYLOAD_LEN);
            gDspVersion = handshake_done.dsp_fw_version;
            tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "dsp_fw_version: 0x%x\n", handshake_done.dsp_fw_version);
            break;
        }
        case IPC_DSP2D25F_SET_PARAM_DONE:
        {
            audio_buffer_context_t *set_param_buf_ctx;
            set_param_done_t        set_param_done;

            memcpy(&set_param_done, msg.payload, IPC_MSG_PAYLOAD_LEN);
            set_param_buf_ctx                = get_audio_buff_context_ptr(IPC_SET_PARAM_PATH_2);
            set_param_buf_ctx->pcm_buff_wptr = set_param_done.buff_wptr;
            int8_t index                     = set_param_buf_ctx->pcm_buff_wptr - 1;
            if (index < 0) {
                index &= (set_param_buf_ctx->pcm_buff_num - 1);
            }
            set_param_buf_ctx->pcm_data_len[index] = set_param_done.data_len;
            set_param_buf_ctx->pcm_buff_wptr       = index;
            d25f_update_enc_buff_rptr(IPC_SET_PARAM_PATH_2);
            if (set_param_done.status == 1) {
#if TLKALG_ANC_ENABLE
                uint16_t param_data_len = 0;
                if (set_param_done.para_type == ANC_PARA) {
                    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&param_data_len, IPC_SET_PARAM_PATH_2);
                    int     *pdata    = (int *)pcm_data;
                    if (ALG_PARA == pcm_data[0]) {
                        tlkalg_anc_load_para((uint8_t *)(pdata + 1));
                    } else if (MODE_PARA == pcm_data[0]) {
                        tlkalg_anc_proc_mode_para(pdata + 1);
                    }
                }
#endif
            }
            break;
        }
        case IPC_DSP2D25F_DATA_PROCESS_DONE:
        {
            data_process_done_t data_process_done;
            int8_t              index;

            dsp_ipc_flag = 0;

            //log_tick(SL_DSP_EN, SLET_dsp_data_process_done);

            tmemcpy(&data_process_done, msg.payload, IPC_MSG_PAYLOAD_LEN);

            //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "alg_type: %d\n", data_process_done.alg_type);

            if (data_process_done.alg_type == BT_VOICE_CVSD_ENC || data_process_done.alg_type == BT_VOICE_MSBC_ENC || data_process_done.alg_type == LL_AUDIO_ENC ||
                data_process_done.alg_type == HRA_ALG || data_process_done.alg_type == HRA_MUSIC || data_process_done.alg_type == BT_VOICE ||
                data_process_done.alg_type == LL_VOICE || data_process_done.alg_type == VAD_NN_NS || data_process_done.alg_type == DSP_BYPASS) {
                id = IPC_DATA_PATH_0;
            } else if (data_process_done.alg_type == BT_VOICE_CVSD_DEC || data_process_done.alg_type == BT_VOICE_MSBC_DEC || data_process_done.alg_type == LL_AUDIO_DEC ||
                       data_process_done.alg_type == BT_MUSIC_AAC_DEC || data_process_done.alg_type == BT_MUSIC_SBC_DEC) {
                id = IPC_DATA_PATH_1;
            } else if (data_process_done.alg_type == MIX_MODE_LL_AUDIO_DEC) {
                id = MIX_MODE_LL_AUDIO_DEC_ID;
            }

            p_audio_buf_ctx = get_audio_buff_context_ptr(id);

            //seq num to fix issue that dsp enter and exit jtag mode, fill mute and irq may happen at the same time.
            //cancel seq num to solve bt-7.5ms frame, nn_ns-20ms frame, which cannot keep same
            // if (p_audio_buf_ctx->enc_seq == data_process_done.seq) {
#if JTAG_DEBUG_ENABLE
            s_hra_env_t.dsp2d25f_irq_flag = true;
#endif

            p_audio_buf_ctx->pcm_buff_wptr = data_process_done.pcm_buff_wptr;

            index = p_audio_buf_ctx->pcm_buff_wptr - 1;
            if (index < 0) {
                index &= (p_audio_buf_ctx->pcm_buff_num - 1);
            }
            p_audio_buf_ctx->pcm_data_len[index] = data_process_done.pcm_data_len;
            p_audio_buf_ctx->pcm_buff_wptr       = index;

            d25f_update_enc_buff_rptr(id);

            if (ipc_msg_data_process_done_cb[id] && s_ipc_process_type[id] == data_process_done.alg_type) {
                ipc_msg_data_process_done_cb[id](data_process_done.enc_buff_wptr, s_ipc_process_type[id]);
            }

            break;
        }
        case IPC_DSP2D25F_DATA_PROCESS_ERROR:
        {
            break;
        }
        case IPC_DSP2D25F_MODE_CHANGE_DONE:
        {
            mode_change_done_t mode_change_done;

            g_app_dsp_ctx[0].dsp_is_ready = 1;

            tmemcpy(&mode_change_done, msg.payload, IPC_MSG_PAYLOAD_LEN);
            //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "new mode: %d, DSP is ready\n", mode_change_done.new_mode);
            extern void ipc_update_audio_buff_config(uint8_t mode);
            ipc_update_audio_buff_config(mode_change_done.new_mode);
            break;
        }
        case IPC_DSP2D25F_DEBUG_INFO:
        {
            //          debug_info_t debug_info;
            //
            //          tmemcpy(&debug_info, msg.payload, IPC_MSG_PAYLOAD_LEN);
            //          printf("DEBUG INFO: %x %x %x\n", debug_info.dsp_debug_info[0],
            //                                           debug_info.dsp_debug_info[1], debug_info.dsp_debug_info[2]);

            unsigned int debug_info[2];

            tmemcpy(&debug_info, msg.payload, IPC_MSG_PAYLOAD_LEN);

            if (DSP_LOG_SIZE < queue_get_free_size(&s_dsp_log_queue)) {
                queue_put_buffer(&s_dsp_log_queue, debug_info, DSP_LOG_SIZE);
            } else {
                //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "Log Queue is Full");
            }

            break;
        }
        default:
            break;
        }
    }
}
#endif
