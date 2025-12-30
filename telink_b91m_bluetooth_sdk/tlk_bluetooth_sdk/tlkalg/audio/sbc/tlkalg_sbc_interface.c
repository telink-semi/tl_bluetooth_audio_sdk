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

SBC_CFG_Param g_sbc_param = {
    .sbc_blocks     = 16,
    .sbc_bitpool    = 36,
    .sbc_allocation = 0,
    .sbc_samplerate = 48000, //44100,
    .sbc_channel    = SBC_MODE_STEREO,
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

uint8_t *g_sbc_dec_buf_ptr  = NULL;
uint8_t *g_sbc_enc_buf_ptr  = NULL;
uint8_t *g_msbc_dec_buf_ptr = NULL;
uint8_t *g_msbc_enc_buf_ptr = NULL;
uint8_t *g_hfp_plc_buf_ptr  = NULL;

uint8_t msbc_silence_pkt[60] = {0x01, 0x08, 0xad, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d,
                                0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd,
                                0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6c, 0x00};

/**
 * @brief       This function retrieves the size required for the SBC decoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the SBC decoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_sbc_dec_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_sbc_dec_get_size();
    size     = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "sbc dec size %d", size);
    return (uint16_t)size;
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
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_init error");
        return 0;
    }
    g_sbc_dec_buf_ptr = p_buff;

    int ret = tlka_sbc_dec_init((sbc_dec_para_t *)g_sbc_dec_buf_ptr, &g_sbc_param);
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_init ret %d %x", ret, g_sbc_dec_buf_ptr);

    return ret;
}

/**
 * @brief       This function deinitializes the SBC decoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_sbc_dec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_sbc_dec_deinit");
    g_sbc_dec_buf_ptr = NULL;
    return 0;
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

    ret = tlka_sbc_dec_process((sbc_dec_para_t *)g_sbc_dec_buf_ptr, (const uint8_t *)ps, (uint32_t)len, (uint16_t *)pd, &dlen, 0x00, channel);

    if (ret && dlen) {
        ret = 1;
    } else {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "sbc dec error");
    }

    if (0x03 == channel) {
        dlen *= 2;
    }

    return dlen;
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
    tlka_sbc_enc_process((sbc_enc_para_t *)g_sbc_enc_buf_ptr, (int16_t *)ps, len, pd, &dlen, 0x00);
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

    if (BT_VOICE_FLAG_PACKET_LOSS != ps[0]) {
        if (MSBC_SYNCWORD == ps[4]) {
            ret = tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01);

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
        tlka_sbc_dec_process((sbc_dec_para_t *)g_msbc_dec_buf_ptr, (const uint8_t *)ps + 4, (uint32_t)len, (uint16_t *)pd, &dlen, 0x01, 0x01);
        ret = tlka_hfp_plc_bad_frame((plc_state_t *)g_hfp_plc_buf_ptr, (int16_t *)pd, (int16_t *)pd);

#if (SLET_sco_plc)
        log_tick_irq(1, SLET_sco_plc);
#endif

        tlkapi_trace(0xFFFFFFFF, "[PLC]", "mSBC plc ret %d", ret);
        // DBG_COMMON_CHN8_HIGH;
        // DBG_COMMON_CHN8_LOW;
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

    pd[0] = 0x01;
    pd[1] = h2_header_sn_table[sn++ & 3];
    tlka_sbc_enc_process((sbc_enc_para_t *)g_msbc_enc_buf_ptr, (int16_t *)ps, len, pd + 2, &dlen, 0x01);
    return dlen;
}

#endif
