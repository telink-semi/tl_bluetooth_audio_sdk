/********************************************************************************************************
 * @file    tlkmw_audio_dsp.c
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
#include "tlkmw/audio/audio_mw_manager.h"
#include "drivers.h"

#include "drv/tlkdrv_dsp.h"

#if TLKALG_ANC_ENABLE
__attribute__((section(".dsp_share_mem_anc_mic"))) int anc_mic_buff[ANC_RECORD_BUFF_SIZE];
__attribute__((section(".dsp_share_mem_anc_spk"))) int anc_spk_buff[ANC_PLAY_BUFF_SIZE];
#define _attribute_dsp_share_mem_sec_ __attribute__((section(".dsp_share_mem_d25f1")))
#else
#define _attribute_dsp_share_mem_sec_ __attribute__((section(".dsp_share_mem")))
#endif
_attribute_dsp_share_mem_sec_ uint8_t ipc_data_buffer[IPC_BUFF_LEN];

app_dsp_context_t g_app_dsp_ctx[IPC_DATA_PATH_MAX];

/**
 * @brief       Get DSP application context
 * @param[in]   id - IPC data path ID
 * @return      Pointer to application DSP context
 */
_attribute_ram_code_sec_noinline_ app_dsp_context_t *d25f_get_dsp_app_ctx(uint8_t id)
{
    return &g_app_dsp_ctx[id];
}

/**
 * @brief       Initialize IPC buffer
 */
void d25f_init_ipc_buffer(void)
{
    audio_buff_init(ipc_data_buffer);
    ipc_message_queue_init();
    // #if TLKALG_ANC_ENABLE
    // audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(IPC_ANC_PATH);
    // anc_mic_buff = (int *)p_audio_buf_ctx->p_enc_audio_buff;
    // anc_spk_buff = (int *)p_audio_buf_ctx->p_pcm_buff;
    // #endif
}

/**
 * @brief       Set algorithm type
 * @param[in]   alg_type - Algorithm type
 * @param[in]   id - IPC data path ID
 */
void d25f_set_alg_type(uint8_t alg_type, uint8_t id)
{
    g_app_dsp_ctx[id].alg_type = alg_type;
}

/**
 * @brief       Initialize DSP environment
 */
void d25f_init_dsp_env(void)
{
    //    uint8_t mode = DSP_PROCESS_BYPASS_MODE;

    //d25f_set_alg_type(ALG_SBC_DEC, IPC_DATA_PATH_0);
    //d25f_set_alg_type(ALG_SBC_DEC, IPC_DATA_PATH_1);
    d25f_send_handshake_msg();
    delay_ms(20);

    //    d25f_send_request_to_dsp(IPC_D25F2DSP_CLEANUP, NULL, 0);
    //    delay_ms(20);
    //
    //    d25f_send_request_to_dsp(IPC_D25F2DSP_SET_PARAM, NULL, 0);
    //    delay_ms(20);

    //    d25f_send_mode_change_msg(mode);
    //    g_app_dsp_ctx[IPC_DATA_PATH_0].current_mode = mode;
    //    g_app_dsp_ctx[IPC_DATA_PATH_1].current_mode = mode;
    //
    //    delay_ms(100);
}

/**
 * @brief       Dump DSP registers
 */
void app_dsp_dunp_reg(void)
{
#if (!MCU_CORE_TL752X_TEMP)
    uint8_t  reg_rst4_value             = reg_rst4;
    uint8_t  reg_clk_en4_value          = reg_clk_en4;
    uint8_t  reg_dsp_rst0_value         = reg_dsp_rst0;
    uint8_t  reg_dsp_clken0_value       = reg_dsp_clken0;
    uint32_t reg_dsp_reset_vector_value = reg_dsp_reset_vector;
    uint16_t reg_dsp_ctrl0_value        = reg_dsp_ctrl0;
    uint32_t reg_dsp_ctrl_value         = reg_dsp_ctrl;
    uint16_t reg_dsp_ctrl1_value        = reg_dsp_ctrl1;
    tlkapi_printf(APP_LOG_EN, "dsp reg: %x %x %x %x %x %x %x %x", reg_rst4_value, reg_clk_en4_value, reg_dsp_rst0_value, reg_dsp_clken0_value, reg_dsp_reset_vector_value,
                  reg_dsp_ctrl0_value, reg_dsp_ctrl_value, reg_dsp_ctrl1_value);
#endif
}

/**
 * @brief       Initialize TLK middleware DSP
 */
void tlkmw_dsp_init(void)
{
    d25f_init_ipc_buffer();
    tlkdrv_dsp_init();
}

/**
 * @brief       Pause TLK middleware DSP
 */
void tlkmw_dsp_pause(void)
{
    tlkdrv_dsp_pause();
}

/**
 * @brief       Resume TLK middleware DSP
 */
void tlkmw_dsp_resume(void)
{
    tlkdrv_dsp_resume();
}

/**
 * @brief       Check if TLK middleware DSP is working
 * @return      True if DSP is working, false otherwise
 */
bool tlkmw_dsp_isWorking(void)
{
    return tlkdrv_dsp_isWorking();
}

/**
 * @brief       Get PCM data from DSP
 * @param[out]  data_len - Pointer to store data length
 * @param[in]   id - IPC data path ID
 * @return      Pointer to PCM data
 */
audio_ram_code uint8_t *d25f_get_pcm_data_from_dsp(uint16_t *data_len, uint8_t id)
{
    uint16_t pcm_data_len;
    uint8_t *pcm_data = NULL;
    //audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    /* get PCM data from share memory */
    pcm_data  = d25f_get_pcm_buff(&pcm_data_len, id);
    *data_len = pcm_data_len;
    if (pcm_data == NULL) {
        //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "pcm data is NULL");
        /*
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "pcm data is NULL, r/w:%d %d",
                     p_audio_buf_ctx->pcm_buff_rptr,
                     p_audio_buf_ctx->pcm_buff_wptr);
        */

        return NULL;
    }

    return pcm_data;
}

//uint8_t lc3_data[90 * 10];
//uint8_t lc3_data_rcv;
//uint8_t lc3_data_rcv_cnt = 10;
//uint16_t g_pkt_cnt;
//uint16_t lc3_offset;
extern uint8_t dsp_ipc_flag;

/**
 * @brief       Send audio data to DSP
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   len - Data length
 * @param[in]   id - IPC data path ID
 * @return      1 if successful, 0 otherwise
 */
audio_ram_code uint8_t d25f_send_audio_data_to_dsp(uint8_t *p_data, uint16_t len, uint8_t id)
{
    if (!tlkdrv_dsp_isWorking()) {
        return 0;
    }

    uint8_t                 ret             = 1;
    uint8_t                *p_enc_buff      = d25f_get_enc_buff_ptr(id);
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    if (len > p_audio_buf_ctx->enc_audio_buff_frame_size) {
        len = p_audio_buf_ctx->enc_audio_buff_frame_size;
    }
    tmemcpy(p_enc_buff, p_data, len);

    //    uint16_t pcm_buff_avail = d25f_get_number_of_pcm_buff_available(id);
    //uint16_t enc_buff_free = d25f_get_number_of_enc_buff_free(id);
    d25f_get_number_of_enc_buff_free(id);
    app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(id);
    p_audio_buf_ctx->enc_seq++;
    p_audio_buf_ctx->alg_type        = p_dsp_app_ctx->alg_type;
    p_audio_buf_ctx->current_mode    = p_dsp_app_ctx->current_mode;
    p_dsp_app_ctx->cur_send_enc_wptr = p_audio_buf_ctx->enc_audio_buff_wptr;

    if (dsp_ipc_flag) {
        //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "dsp_ipc_flag: %d\n", dsp_ipc_flag);
        //return 0;
    }
    //log_b16(SL_APP_AUDIO_EN, SL16_dsp_pcm_wptr, p_audio_buf_ctx->pcm_buff_wptr | p_audio_buf_ctx->pcm_buff_rptr << 8);

    /*
    if (lc3_data_rcv == 0) {
        g_pkt_cnt++;
        if (g_pkt_cnt == 200) {
            lc3_data_rcv = 1;
        }
    }

    if (lc3_data_rcv && lc3_data_rcv_cnt > 0) {
        tmemcpy(lc3_data + lc3_offset, p_data, 90);
        lc3_offset += 90;
        lc3_data_rcv_cnt--;
        
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "store lc3 data");
    }
*/

#if (0)
    tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "$ datapath id:%d, enc_r:%d, enc_w:%d, enc_free:%d, pcm_avail:%d\n", id, p_audio_buf_ctx->enc_audio_buff_rptr,
                 p_audio_buf_ctx->enc_audio_buff_wptr, enc_buff_free, pcm_buff_avail);
#endif

    if (id == IPC_DATA_PATH_0) {
        // if (pcm_buff_avail > IPC_DATA_PATH0_PCM_BUFFER_NUM - 2) {
        //     tlkapi_printf(APP_LOG_EN, "[DSP] send failed, pcm_buff_avail: %d, path id: %d", pcm_buff_avail, id);
        //     ret = 0;
        //     return ret;
        // }
    } else if (id == IPC_DATA_PATH_1) {
        // if (pcm_buff_avail > IPC_DATA_PATH1_PCM_BUFFER_NUM - 2) {
        //     tlkapi_printf(APP_LOG_EN, "[DSP] send failed, pcm_buff_avail: %d, path id: %d", pcm_buff_avail, id);
        //     ret = 0;
        //     return ret;
        // }
    } else if (id == IPC_SET_PARAM_PATH_2) {
        // if (pcm_buff_avail > IPC_DATA_PATH2_PCM_BUFFER_NUM - 2) {
        //     tlkapi_printf(APP_LOG_EN, "[DSP] send failed, pcm_buff_avail: %d, path id: %d", pcm_buff_avail, id);
        //     ret = 0;
        //     return ret;
        // }
    }

    p_audio_buf_ctx->enc_audio_data_len = len;

    //bt_audio_gpio_toggle_audio_tx();
    //tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "d25f_send_audio_data_to_dsp 1111");

    d25f_send_data_process_msg(p_audio_buf_ctx, id);

    return ret;
}

#if DSP_NN_NS_BUFF_DISENABLE
#define VOICE_NN_FRAME_LEN (16 * 20)
#define VOICE_MIC_DATA_LEN (120 * 4) //7.5ms*16k*4frame

static uint16_t voice_len  = 0;
static uint16_t voice_wptr = 0;
static uint16_t voice_rptr = 0;

int16_t bt_voice_mic_data[VOICE_MIC_DATA_LEN];

/**
 * @brief       Get BT voice available NN buffer length
 * @param[in]   ps - Source pointer
 * @param[out]  pd - Destination pointer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_get_bt_voice_available_nn_buff_len(int16_t *ps, int16_t *pd, uint16_t len)
{
    voice_len += len;

    for (uint16_t i = 0; i < len; i++) {
        bt_voice_mic_data[voice_wptr++] = *ps++;
        voice_wptr %= VOICE_MIC_DATA_LEN;
    }

    if (voice_len >= VOICE_NN_FRAME_LEN) {
        voice_len -= VOICE_NN_FRAME_LEN;

        for (int j = 0; j < VOICE_NN_FRAME_LEN; j++) {
            *pd++ = bt_voice_mic_data[voice_rptr++];
            voice_rptr %= VOICE_MIC_DATA_LEN;
        }
        gpio_set_high_level(GPIO_PA0);
        gpio_set_low_level(GPIO_PA0);
        return 1;
    } else {
        return 0;
    }
}

int ll_voice_mic_data[VOICE_MIC_DATA_LEN];

/**
 * @brief       Get LL voice available NN buffer length
 * @param[in]   ps - Source pointer
 * @param[out]  pd - Destination pointer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_get_ll_voice_available_nn_buff_len(int *ps, int *pd, uint16_t len)
{
    voice_len += len;

    for (uint16_t i = 0; i < len; i++) {
        ll_voice_mic_data[voice_wptr++] = *ps++;
        voice_wptr %= VOICE_MIC_DATA_LEN;
    }

    if (voice_len >= VOICE_NN_FRAME_LEN) {
        voice_len -= VOICE_NN_FRAME_LEN;

        for (int j = 0; j < VOICE_NN_FRAME_LEN; j++) {
            *pd++ = ll_voice_mic_data[voice_rptr++];
            voice_rptr %= VOICE_MIC_DATA_LEN;
        }

        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief       Reset DSP voice NN communication status
 */
void dsp_voice_nn_comm_reset_status(void)
{
    voice_len  = 0;
    voice_wptr = 0;
    voice_rptr = 0;
}

#endif
#endif
