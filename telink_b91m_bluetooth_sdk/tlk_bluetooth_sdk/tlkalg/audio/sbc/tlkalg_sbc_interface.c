/********************************************************************************************************
 * @file    tlkalg_sbc_interface.c
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

#include "tlkalg/audio/sbc/tlkalg_sbc_interface.h"
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
//#include "stack/debug/debug_internal.h"

#if (TLKALG_SBC_DEC_ENABLE || TLKALG_SBC_ENC_ENABLE)
#define BT_VOICE_FLAG_PACKET_LOSS 1
#define SBC_ENC_CORE_FRAME_HEADER 4

SBC_CFG_Param g_sbc_param = {
    .sbc_blocks     = 16,
    .sbc_bitpool    = 36,
    .sbc_allocation = 0,
    .sbc_samplerate = 48000, //44100,
    .sbc_channel    = SBC_MODE_JOINT_STEREO,
    .msbc           = 0,
    .sbc_subbands   = 8,
};

SBC_CFG_Param g_msbc_param = {
    .sbc_blocks     = 15,
    .sbc_bitpool    = 26,
    .sbc_allocation = 0,
    .sbc_samplerate = 16000,
    .sbc_channel    = 1,
    .msbc           = 1,
    .sbc_subbands   = 8,
};

HFP_PLC_CFG_PARAM plc_paras = {
    .samplerate = 16000,
    .frame_size = 120,
};

uint8_t *g_sbc_dec_buf_ptr    = NULL;
uint8_t *g_sbc_enc_buf_ptr    = NULL;
uint8_t *g_msbc_dec_buf_ptr   = NULL;
uint8_t *g_msbc_enc_buf_ptr   = NULL;
uint8_t *g_hfp_plc_buf_ptr    = NULL;
void    *sbc_enc_scratch      = NULL;
void    *sbc_dec_scratch      = NULL;
int      sbc_enc_scratch_size = 0;
int      sbc_dec_scratch_size = 0;

void *msbc_enc_scratch      = NULL;
void *msbc_dec_scratch      = NULL;
int   msbc_enc_scratch_size = 0;
int   msbc_dec_scratch_size = 0;

#if TLKALG_MSBC_ENABLE_CH2
uint8_t *g_msbc_dec_ch2buf_ptr = NULL;
uint8_t *g_msbc_enc_ch2buf_ptr = NULL;
uint8_t *g_hfp_plc_ch2buf_ptr  = NULL;
#endif

#if BT_VOICE_SPP_TEST
uint8_t *g_msbc_dec_micbuf_ptr = NULL;
uint8_t *g_msbc_dec_spkbuf_ptr = NULL;
uint8_t *g_msbc_enc_micbuf_ptr = NULL;
uint8_t *g_msbc_enc_spkbuf_ptr = NULL;
#endif

uint8_t msbc_silence_pkt[60] = {0x01, 0x08, 0xad, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d,
                                0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd,
                                0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6c, 0x00};

/*
 * @brief       This function updates the SBC encoder bitpool.
 * @param[in]   bitpool - the new bitpool value.
 * @return      None.
 */
void tlkalg_sbc_enc_update_bitpool(uint8_t bitpool)
{
    tlk_printf("tlkalg_sbc_enc_update_bitpool bitpool %d", bitpool);
    g_sbc_param.sbc_bitpool = bitpool;
}

/**
 * @brief       This function retrieves the size required for the SBC decoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the SBC decoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_sbc_dec_get_size(uint8_t channel)
{
    (void)channel;
#if (TLKALG_SBC_DEC_ENABLE)
    int size = tlka_sbc_dec_get_size();
    size     = (size + 3) / 4 * 4; //4 Byte align
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "sbc dec size %d", size);

    sbc_dec_scratch_size = tlka_sbc_dec_get_scratch_size();
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "sbc dec scratch size :%d", sbc_dec_scratch_size);

    return (uint16_t)size;
#else
    return 0;
#endif
}

/**
 * @brief       This function initializes the SBC decoder.
 * @param[in]   p_buff  - the buffer to be used for the decoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_sbc_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
#if (TLKALG_SBC_DEC_ENABLE)
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_init error");
        return 0;
    }
    g_sbc_dec_buf_ptr = p_buff;

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_sbc_dec_buf_ptr, &g_sbc_param);
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_init ret %d %x", ret, g_sbc_dec_buf_ptr);

    return ret;
#else
    (void)p_buff;
    return 0;
#endif
}

/**
 * @brief       This function deinitializes the SBC decoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_sbc_dec_deinit(void)
{
#if (TLKALG_SBC_DEC_ENABLE)
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_deinit");
    g_sbc_dec_buf_ptr = NULL;
    return 0;
#else
    return 0;
#endif
}

/**
 * @brief       This function processes an SBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the encoded audio data.
 * @param[out]  pd      - the output buffer for the decoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the decoded audio data.
 */
int tlkalg_sbc_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
#if (TLKALG_SBC_DEC_ENABLE)
    (void)width;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "sbc dec data buffer null");
        return ret;
    }

    if (g_sbc_dec_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "sbc dec struct pointer null");
        return ret;
    }

    sbc_dec_scratch = (void *)tlkalg_malloc_func(sbc_dec_scratch_size);
    if (sbc_dec_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[SBC]", "sbc decoder scratch NULL");
        return 0;
    }

    ret = tlka_sbc_dec_process((sbc_dec_para_t *)g_sbc_dec_buf_ptr, (const uint8_t *)ps, (uint32_t)len, (uint16_t *)pd, &dlen, 0x00, channel, sbc_dec_scratch);

    if (sbc_dec_scratch != NULL) {
        tlkalg_free_func(sbc_dec_scratch);
        sbc_dec_scratch = NULL;
    }

    if (ret && dlen) {
        ret = 1;
    } else {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "sbc dec error");
    }

    if (0x03 == channel) {
        dlen *= 2;
    }

    return dlen;
#else
    (void)ps;
    (void)pd;
    (void)len;
    (void)width;
    (void)channel;
    return 0;
#endif
}

/**
 * @brief       This function retrieves the size required for the SBC encoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the SBC encoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_sbc_enc_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_sbc_enc_get_size();
    size     = (size + 3) / 4 * 4; //4 Byte align
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "sbc enc size %d", size);

    sbc_enc_scratch_size = tlka_sbc_enc_get_scratch_size();
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "sbc enc scratch size: %d", sbc_enc_scratch_size);

    return (uint16_t)(size);
}

/**
 * @brief       This function initializes the SBC encoder.
 * @param[in]   p_buff  - the buffer to be used for the encoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_sbc_enc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_enc_init error");
        return 0;
    }
    g_sbc_enc_buf_ptr = p_buff;

    int ret = tlka_sbc_enc_init((sbc_enc_para_t *)g_sbc_enc_buf_ptr, &g_sbc_param);

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_enc_init ret %d %x", ret, g_sbc_enc_buf_ptr);

    return ret;
}

/**
 * @brief       This function sets the encoder parameters for SBC.
 * @param[in]   type  - the type of parameter to set.
 * @param[in]   param - the parameter value to set.
 * @return      The result of setting the parameter.
 */
uint8_t tlkalg_sbc_enc_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val                   = (int *)param;
    g_sbc_param.sbc_samplerate = (uint16_t)(*val);
    tlka_sbc_enc_init((sbc_enc_para_t *)g_sbc_enc_buf_ptr, &g_sbc_param);

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm set samplerate %d", g_sbc_param.sbc_samplerate);
    return 0;
}

/**
 * @brief       This function deinitializes the SBC encoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_sbc_enc_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_enc_deinit");
    g_sbc_enc_buf_ptr = NULL;
    return 0;
}

uint16_t tlkalg_sbc_enc_get_data_len(void)
{
    uint8_t joint_flag = 0;
    uint8_t channel    = 1;
    if (g_sbc_param.sbc_channel == SBC_MODE_JOINT_STEREO) {
        joint_flag = 1;
        channel    = 2;
    } else if (g_sbc_param.sbc_channel == SBC_MODE_JOINT_STEREO) {
        channel = 2;
    }
    uint16_t len = SBC_ENC_CORE_FRAME_HEADER + joint_flag + (4 * channel * g_sbc_param.sbc_subbands) / 8 + (g_sbc_param.sbc_blocks * g_sbc_param.sbc_bitpool) / 8;
    return len;
}

/**
 * @brief       This function processes an SBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the audio data to be encoded.
 * @param[out]  pd      - the output buffer for the encoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the encoded audio data.
 */
int tlkalg_sbc_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "SBC enc input buff point null");
        return ret;
    }

    if (g_sbc_enc_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "SBC enc struct point null");
        return ret;
    }


    sbc_enc_scratch = (void *)tlkalg_malloc_func(sbc_enc_scratch_size);
    if (sbc_enc_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[SBC]", "sbc encoder scratch NULL");
        return 0;
    }

    tlka_sbc_enc_process((sbc_enc_para_t *)g_sbc_enc_buf_ptr, (int16_t *)ps, len, pd, &dlen, 0x00, sbc_enc_scratch);

    if (sbc_enc_scratch != NULL) {
        tlkalg_free_func(sbc_enc_scratch);
        sbc_enc_scratch = NULL;
    }

    return dlen;
}

/**
 * @brief       This function retrieves the size required for the mSBC decoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the mSBC decoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_msbc_dec_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_sbc_dec_get_size() + tlka_hfp_plc_get_size();
    size     = (size + 3) / 4 * 4; //4 Byte align
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "msbc buffer size %d", size);

    msbc_dec_scratch_size = tlka_sbc_dec_get_scratch_size();
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "msbc dec scratch size :%d", msbc_dec_scratch_size);

    return size;
}

/**
 * @brief       This function initializes the mSBC decoder.
 * @param[in]   p_buff  - the buffer to be used for the decoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_msbc_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_init error");
        return 0;
    }
    g_msbc_dec_buf_ptr = p_buff;
    g_hfp_plc_buf_ptr  = p_buff + tlka_sbc_dec_get_size();

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_msbc_dec_buf_ptr, &g_msbc_param);
    tlka_hfp_plc_init((plc_state_t *)g_hfp_plc_buf_ptr, plc_paras);

    return ret;
}

/**
 * @brief       This function deinitializes the mSBC decoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_msbc_dec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_deinit");
    g_msbc_dec_buf_ptr = NULL;
    g_hfp_plc_buf_ptr  = NULL;
    return 0;
}

/**
 * @brief       This function processes an mSBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the encoded audio data.
 * @param[out]  pd      - the output buffer for the decoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the decoded audio data.
 */
int tlkalg_msbc_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec data buffer null");
        return ret;
    }

    if (g_msbc_dec_buf_ptr == NULL || g_hfp_plc_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec struct pointer null");
        return ret;
    }

    msbc_dec_scratch = (void *)tlkalg_malloc_func(msbc_dec_scratch_size);
    if (msbc_dec_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc decoder scratch NULL");
        return 0;
    }

    if (BT_VOICE_FLAG_PACKET_LOSS != ps[0]) {
        if (MSBC_SYNCWORD == ps[4]) {
            ret = tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);

            if (ret && dlen) {
                ret = 1;
            } else {
                //decode error
                tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec error");
            }
        } else {
            tmemset(pd, 0, 240);
            dlen = 120;
            tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC dec syncword err");
        }
    }

    if (ret == 1) {
        tlka_hfp_plc_good_frame((plc_state_t *)g_hfp_plc_buf_ptr, (int16_t *)pd, (int16_t *)pd);
    } else {
        tmemcpy(ps + 2, msbc_silence_pkt, 60);
        tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);
        ret = tlka_hfp_plc_bad_frame((plc_state_t *)g_hfp_plc_buf_ptr, (int16_t *)pd, (int16_t *)pd);

#if (SLET_sco_plc)
        log_tick_irq(1, SLET_sco_plc);
#endif

        //        tlkapi_trace(0xFFFFFFFF, "[PLC]", "mSBC plc ret %d", ret);
        // DBG_COMMON_CHN8_HIGH;
        // DBG_COMMON_CHN8_LOW;
    }

    if (msbc_dec_scratch != NULL) {
        tlkalg_free_func(msbc_dec_scratch);
        msbc_dec_scratch = NULL;
    }

    return dlen;
}

/**
 * @brief       This function retrieves the size required for the mSBC encoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the mSBC encoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_msbc_enc_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_sbc_enc_get_size();
    size     = (size + 3) / 4 * 4; //4 Byte align
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "msbc enc buffer size %d", size);

    msbc_enc_scratch_size = tlka_sbc_enc_get_scratch_size();
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "msbc enc scratch size : %d", msbc_enc_scratch_size);

    return size;
}

/**
 * @brief       This function initializes the mSBC encoder.
 * @param[in]   p_buff  - the buffer to be used for the encoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_msbc_enc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_init error");
        return 0;
    }
    g_msbc_enc_buf_ptr = p_buff;

    int ret = tlka_sbc_enc_init((sbc_enc_para_t *)g_msbc_enc_buf_ptr, &g_msbc_param);

    return ret;
}

/**
 * @brief       This function deinitializes the mSBC encoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_msbc_enc_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_deinit");
    g_msbc_enc_buf_ptr = NULL;
    return 0;
}

/**
 * @brief       This function processes an mSBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the audio data to be encoded.
 * @param[out]  pd      - the output buffer for the encoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the encoded audio data.
 */
int tlkalg_msbc_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    static uint8_t sn                    = 0;
    unsigned char  h2_header_sn_table[4] = {0x08, 0x38, 0xc8, 0xf8};
    uint32_t       dlen                  = 0;
    uint32_t       ret                   = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc input buff point null");
        return ret;
    }

    if (g_msbc_enc_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc struct point null");
        return ret;
    }

    msbc_enc_scratch = (void *)tlkalg_malloc_func(msbc_enc_scratch_size);
    if (msbc_enc_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc encoder scratch NULL");
        return 0;
    }

    pd[0] = 0x01;
    pd[1] = h2_header_sn_table[sn++ & 3];
    tlka_sbc_enc_process((sbc_enc_para_t *)g_msbc_enc_buf_ptr, (int16_t *)ps, len, pd + 2, &dlen, 0x01, msbc_enc_scratch);

    if (msbc_enc_scratch != NULL) {
        tlkalg_free_func(msbc_enc_scratch);
        msbc_enc_scratch = NULL;
    }

    return dlen;
}

#if TLKALG_MSBC_ENABLE_CH2
int8_t tlkalg_msbc_dec_ch2_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_ch2_init error");
        return 0;
    }
    g_msbc_dec_ch2buf_ptr = p_buff;
    g_hfp_plc_ch2buf_ptr  = p_buff + tlka_sbc_dec_get_size();

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_msbc_dec_ch2buf_ptr, &g_msbc_param);
    tlka_hfp_plc_init((plc_state_t *)g_hfp_plc_ch2buf_ptr, plc_paras);

    return ret;
}

int8_t tlkalg_msbc_dec_ch2_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_ch2_deinit");
    g_msbc_dec_ch2buf_ptr = NULL;
    g_hfp_plc_ch2buf_ptr  = NULL;
    return 0;
}

int tlkalg_msbc_dec_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec ch2 buffer data null");
        return ret;
    }

    if (g_msbc_dec_ch2buf_ptr == NULL || g_hfp_plc_ch2buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec ch2 struct pointer null");
        return ret;
    }

    msbc_dec_scratch = (void *)tlkalg_malloc_func(msbc_dec_scratch_size);
    if (msbc_dec_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc decoder scratch NULL");
        return 0;
    }

    if (BT_VOICE_FLAG_PACKET_LOSS != ps[0]) {
        if (MSBC_SYNCWORD == ps[4]) {
            ret = tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_ch2buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);

            if (ret && dlen) {
                ret = 1;
            } else {
                //decode error
                tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec ch2 error");
            }
        } else {
            tmemset(pd, 0, 240);
            dlen = 120;
            tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC dec ch2 syncword err");
        }
    }

    if (ret == 1) {
        tlka_hfp_plc_good_frame((plc_state_t *)g_hfp_plc_ch2buf_ptr, (int16_t *)pd, (int16_t *)pd);
    } else {
        tmemcpy(ps + 2, msbc_silence_pkt, 60);
        tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_ch2buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);
        ret = tlka_hfp_plc_bad_frame((plc_state_t *)g_hfp_plc_ch2buf_ptr, (int16_t *)pd, (int16_t *)pd);

#if (SLET_sco_plc)
        log_tick_irq(1, SLET_sco_plc);
#endif
    }

    if (msbc_dec_scratch != NULL) {
        tlkalg_free_func(msbc_dec_scratch);
        msbc_dec_scratch = NULL;
    }

    return dlen;
}

int8_t tlkalg_msbc_enc_ch2_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_ch2_init error");
        return 0;
    }
    g_msbc_enc_ch2buf_ptr = p_buff;

    int ret = tlka_sbc_enc_init((sbc_enc_para_t *)g_msbc_enc_ch2buf_ptr, &g_msbc_param);

    return ret;
}

int8_t tlkalg_msbc_enc_ch2_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_ch2_deinit");
    g_msbc_enc_ch2buf_ptr = NULL;
    return 0;
}

int tlkalg_msbc_enc_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    static uint8_t sn                    = 0;
    unsigned char  h2_header_sn_table[4] = {0x08, 0x38, 0xc8, 0xf8};
    uint32_t       dlen                  = 0;
    uint32_t       ret                   = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc ch2 input buff point null");
        return ret;
    }

    if (g_msbc_enc_ch2buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc ch2 struct point null");
        return ret;
    }

    msbc_enc_scratch = (void *)tlkalg_malloc_func(msbc_enc_scratch_size);
    if (msbc_enc_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc encoder scratch NULL");
        return 0;
    }

    pd[0] = 0x01;
    pd[1] = h2_header_sn_table[sn++ & 3];
    tlka_sbc_enc_process((sbc_enc_para_t *)g_msbc_enc_ch2buf_ptr, (int16_t *)ps, len, pd + 2, &dlen, 0x01, msbc_enc_scratch);

    if (msbc_enc_scratch != NULL) {
        tlkalg_free_func(msbc_enc_scratch);
        msbc_enc_scratch = NULL;
    }

    return dlen;
}
#endif


#if BT_VOICE_SPP_TEST
int8_t tlkalg_msbc_dec_micbuf_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_micbuf_init error");
        return 0;
    }
    g_msbc_dec_micbuf_ptr = p_buff;

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_msbc_dec_micbuf_ptr, &g_msbc_param);

    return ret;
}

int8_t tlkalg_msbc_dec_spkbuf_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_spkbuf_init error");
        return 0;
    }
    g_msbc_dec_spkbuf_ptr = p_buff;

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_msbc_dec_spkbuf_ptr, &g_msbc_param);

    return ret;
}

int8_t tlkalg_msbc_dec_micbuf_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_micbuf_deinit");
    g_msbc_dec_micbuf_ptr = NULL;
    return 0;
}

int8_t tlkalg_msbc_dec_spkbuf_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_dec_spkbuf_deinit");
    g_msbc_dec_spkbuf_ptr = NULL;
    return 0;
}

int tlkalg_msbc_dec_micbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec micbuf data null");
        return ret;
    }

    if (g_msbc_dec_micbuf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec micbuf struct pointer null");
        return ret;
    }

    msbc_dec_scratch = (void *)tlkalg_malloc_func(msbc_dec_scratch_size);
    if (msbc_dec_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc decoder scratch NULL");
        return 0;
    }

    tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_micbuf_ptr, (const uint8_t *)ps + 6, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);

    if (msbc_dec_scratch != NULL) {
        tlkalg_free_func(msbc_dec_scratch);
        msbc_dec_scratch = NULL;
    }

    return dlen;
}

int tlkalg_msbc_dec_spkbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t dlen = 0;
    uint32_t ret  = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec spkbuf data null");
        return ret;
    }

    if (g_msbc_dec_spkbuf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "msbc dec spkbuf struct pointer null");
        return ret;
    }

    msbc_dec_scratch = (void *)tlkalg_malloc_func(msbc_dec_scratch_size);
    if (msbc_dec_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc decoder scratch NULL");
        return 0;
    }

    tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_spkbuf_ptr, (const uint8_t *)ps + 6, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01, msbc_dec_scratch);

    if (msbc_dec_scratch != NULL) {
        tlkalg_free_func(msbc_dec_scratch);
        msbc_dec_scratch = NULL;
    }

    return dlen;
}

int8_t tlkalg_msbc_enc_micbuf_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_micbuf_init error");
        return 0;
    }
    g_msbc_enc_micbuf_ptr = p_buff;

    int ret = tlka_sbc_enc_init((sbc_enc_para_t *)g_msbc_enc_micbuf_ptr, &g_msbc_param);

    return ret;
}

int8_t tlkalg_msbc_enc_spkbuf_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_spkbuf_init error");
        return 0;
    }
    g_msbc_enc_spkbuf_ptr = p_buff;

    int ret = tlka_sbc_enc_init((sbc_enc_para_t *)g_msbc_enc_spkbuf_ptr, &g_msbc_param);

    return ret;
}

int8_t tlkalg_msbc_enc_micbuf_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_micbuf_deinit");
    g_msbc_enc_micbuf_ptr = NULL;
    return 0;
}

int8_t tlkalg_msbc_enc_spkbuf_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_msbc_enc_spkbuf_deinit");
    g_msbc_enc_spkbuf_ptr = NULL;
    return 0;
}

int tlkalg_msbc_enc_micbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    uint32_t        dlen         = 0;
    uint32_t        ret          = 0;
    static uint16_t hfp_miccount = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc input micbuff point null");
        return ret;
    }

    if (g_msbc_enc_micbuf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc struct micbuff point null");
        return ret;
    }

    msbc_enc_scratch = (void *)tlkalg_malloc_func(msbc_enc_scratch_size);
    if (msbc_enc_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc encoder scratch NULL");
        return 0;
    }

    pd[0] = 0x5a;
    pd[1] = 0x5a;
    pd[2] = 0x5a;
    pd[3] = hfp_miccount & 0xff;
    pd[4] = (hfp_miccount >> 8) & 0xff;
    pd[5] = 0x02;
    hfp_miccount++;
    tlka_sbc_enc_process((sbc_enc_para_t *)g_msbc_enc_micbuf_ptr, (int16_t *)ps, len, pd + 6, &dlen, 0x01, msbc_enc_scratch);

    if (msbc_enc_scratch != NULL) {
        tlkalg_free_func(msbc_enc_scratch);
        msbc_enc_scratch = NULL;
    }

    return dlen;
}

int tlkalg_msbc_enc_spkbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    uint32_t        dlen         = 0;
    uint32_t        ret          = 0;
    static uint16_t hfp_spkcount = 0;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc input spkbuff point null");
        return ret;
    }

    if (g_msbc_enc_spkbuf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "mSBC enc struct spkbuff point null");
        return ret;
    }

    msbc_enc_scratch = (void *)tlkalg_malloc_func(msbc_enc_scratch_size);
    if (msbc_enc_scratch == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[MSBC]", "msbc encoder scratch NULL");
        return 0;
    }

    pd[0] = 0x5a;
    pd[1] = 0x5a;
    pd[2] = 0x5a;
    pd[3] = hfp_spkcount & 0xff;
    pd[4] = (hfp_spkcount >> 8) & 0xff;
    pd[5] = 0x01;
    hfp_spkcount++;
    tlka_sbc_enc_process((sbc_enc_para_t *)g_msbc_enc_spkbuf_ptr, (int16_t *)ps, len, pd + 6, &dlen, 0x01, msbc_enc_scratch);

    if (msbc_enc_scratch != NULL) {
        tlkalg_free_func(msbc_enc_scratch);
        msbc_enc_scratch = NULL;
    }

    return dlen;
}
#endif

#endif
