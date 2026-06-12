/********************************************************************************************************
 * @file    ipc_msg.c
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
#include "common/tstring.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmw/audio/audio_mw_manager.h"

uint8_t dsp_ipc_flag;

#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
uint32_t ipcm_d25_tx_addr = 0xb204b000;
uint32_t ipcm_dsp_tx_addr = 0xb204b040;
#endif

/**
 * @brief       Send IPC message from D25F to DSP
 * @param[in]   msg_words - Message words to send
 * @return    none
 */
audio_ram_code void d25f_send_ipc_message(unsigned int *msg_words)
{
#ifdef SLET_dsp_msg_tx
    log_tick_irq(SL_DSP_EN, SLET_dsp_msg_tx);
#endif
#ifdef SL01_dsp_process
    log_task_begin_irq(SL_DSP_EN, SL01_dsp_process);
#endif

#if 0
    gpio_write(GPIO_PF5, 0);
    gpio_write(GPIO_PF5, 1);
    gpio_write(GPIO_PF5, 0);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    memcpy((void *)ipcm_d25_tx_addr, (const void *)msg_words, sizeof(uint32_t) * 2);
    msg_words[0] = ipcm_d25_tx_addr;
    mailbox_d25f_set_dsp_msg((unsigned int *)msg_words);
#else
    mailbox_d25f_set_dsp_msg(msg_words);
#endif
}

/**
 * @brief       Get IPC message from DSP to D25F
 * @param[out]  msg_words - Buffer to store received message words
 * @return    none
 */
audio_ram_code void d25f_get_ipc_message(unsigned int *msg_words)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    unsigned int msg_word_temp[2] = {0};
    mailbox_d25f_get_dsp_msg((unsigned int *)msg_word_temp);
    if (msg_word_temp[0] == ipcm_dsp_tx_addr) {
        memcpy((void *)msg_words, (const void *)ipcm_dsp_tx_addr, sizeof(uint32_t) * 2);
    } else {
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "get dsp share data address error: %d\n", msg_word_temp[0]);
    }
#else
    mailbox_d25f_get_dsp_msg(msg_words);
#endif
}

/**
 * @brief       Send request to DSP
 * @param[in]   type - Message type
 * @param[in]   buffer - Data buffer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_request_to_dsp(ipc_message_type_e type, uint8_t *buffer, uint8_t len)
{
    ipc_message_t request;

    if (len > IPC_MSG_PAYLOAD_LEN) {
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "data invalid: %d\n", type);
        /* invalid length */
        return 0;
    }

    request.header.type         = type;
    request.header.payload_size = len;
    request.header.magic        = IPC_MSG_MAGIC;

    for (uint8_t i = 0; i < len; i++) {
        request.payload[i] = buffer[i];
    }

    //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "d25f_send_request_to_dsp: %d\n", type);
    //if (type == IPC_D25F2DSP_DATA_PROCESS)
    //    log_tick(SL_DSP_EN, SLET_dsp_data_process_req);

    d25f_send_ipc_message(request.ipc_msg);

    return 1;
}

/**
 * @brief       Send handshake message to DSP
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_handshake_msg(void)
{
    uint8_t     ret;
    handshake_t handshake;

    handshake.d25f_fw_version = D25F_FW_VERSION;

    ret = d25f_send_request_to_dsp(IPC_D25F2DSP_HANDSHAKE, (uint8_t *)(&handshake), IPC_MSG_PAYLOAD_LEN);
    return ret;
}

audio_ram_code uint8_t d25f_response_handshake_msg(void)
{
    uint8_t              ret;
    handshake_response_t handshake;

    handshake.d25f_alg_type = 0xc000;
#if (TLKALG_BONE_CODUCTION_EN)
    handshake.bbf_type = TLKALG_BBF_ENABLE - 1;
#else
    handshake.bbf_type = TLKALG_BBF_ENABLE;
#endif
    handshake.alg_dis = 0x80;

    ret = d25f_send_request_to_dsp(IPC_D25F2DSP_HANDSHAKE, (uint8_t *)(&handshake), IPC_MSG_PAYLOAD_LEN);
    return ret;
}

/**
 * @brief       Send sleep message to DSP
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_sleep_msg(void)
{
    uint8_t     ret;
    handshake_t handshake;

    handshake.d25f_fw_version = D25F_FW_VERSION;

    ret = d25f_send_request_to_dsp(IPC_D25F2DSP_SLEEP, (uint8_t *)(&handshake), IPC_MSG_PAYLOAD_LEN);

    return ret;
}

/**
 * @brief       Send mode change message to DSP
 * @param[in]   mode - New mode
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_mode_change_msg(uint8_t mode)
{
    uint8_t       ret;
    mode_change_t mode_change;

    mode_change.old_mode = DSP_PROCESS_TEST_MODE;
    mode_change.new_mode = mode;

    ret = d25f_send_request_to_dsp(IPC_D25F2DSP_MODE_CHANGE, (uint8_t *)(&mode_change), IPC_MSG_PAYLOAD_LEN);

    return ret;
}

/**
 * @brief       Send data process message to DSP
 * @param[in]   p_audio_buf_ctx - Audio buffer context
 * @param[in]   id - IPC data path ID
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_data_process_msg(audio_buffer_context_t *p_audio_buf_ctx, uint8_t id)
{
    uint8_t        ret;
    data_process_t data_process;

    data_process.seq                = p_audio_buf_ctx->enc_seq;
    data_process.enc_buff_wptr      = p_audio_buf_ctx->enc_audio_buff_wptr;
    data_process.enc_audio_data_len = p_audio_buf_ctx->enc_audio_data_len;
    data_process.mode               = p_audio_buf_ctx->current_mode;
    data_process.alg_type           = p_audio_buf_ctx->alg_type;

    //    tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "d25f_send_data_process_msg mode: %d, type: %d",
    //                 data_process.mode, data_process.alg_type);

    if (d25f_get_number_of_enc_buff_free(id)) {
        d25f_update_enc_buff_wptr(id);
        dsp_ipc_flag = 1;

        ret = d25f_send_request_to_dsp(IPC_D25F2DSP_DATA_PROCESS, (uint8_t *)&data_process, IPC_MSG_PAYLOAD_LEN);
    } else {
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "enc buff is empty");
        ret = 0;
    }

    return ret;
}
#endif
