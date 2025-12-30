/********************************************************************************************************
 * @file    ll_audio_common.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"

#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/a2dp/btp_a2dp.h"
#include "stack/bt/host/btp/avrcp/btp_avrcp.h"
#include "stack/tpsll/tpsll.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"
#include "ll_audio_main.h"
#include "ll_audio.h"

#if (TLKSTK_BT_TPS_ENABLE)

#if TLKALG_LC3_24BIT_ENC_ENABLE && TLKALG_LC3_24BIT_DEC_ENABLE
static uint8_t *s_alg_lc3_24bit_dec_buffer = NULL;
static uint8_t *s_alg_lc3_24bit_enc_buffer = NULL;
#endif

    #if TLKALG_ASRC_48TO16_24BIT_ENABLE
static uint8_t *s_alg_asrc_48to16_buff = NULL;
    #endif

#if TLKALG_LC3_PLUS_ENC_ENABLE && TLKALG_LC3_PLUS_DEC_ENABLE
static uint8_t *s_alg_lc3_plus_dec_buffer = NULL;
static uint8_t *s_alg_lc3_plus_enc_buffer = NULL;
static uint8_t *s_alg_lc3_plus_dec_scratch_buffer = NULL;
static uint8_t *s_alg_lc3_plus_enc_scratch_buffer = NULL;
#endif

/**
 * @brief      Initialize audio algorithms.
 * @param      None.
 * @return     None.
 */
void ll_audio_alg_init(void)
{
    audio_alg_interface_t *p_audio_alg_if;
    (void) p_audio_alg_if;
#if TLKALG_LC3_24BIT_DEC_ENABLE && TLKALG_LC3_24BIT_ENC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
    if (s_alg_lc3_24bit_dec_buffer == NULL) {
        uint16_t lc3_24bit_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_dec_mem_size);
        if (s_alg_lc3_24bit_dec_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 4bit dec buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_dec_buffer, ALG_CHANNEL_STEREO);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_dec_mem_size: %d", lc3_24bit_dec_mem_size);
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
    if (s_alg_lc3_24bit_enc_buffer == NULL) {
        uint16_t lc3_24bit_enc_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_enc_mem_size);
        if (s_alg_lc3_24bit_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 4bit enc buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_enc_buffer, ALG_CHANNEL_LEFT);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_enc_mem_size: %d", lc3_24bit_enc_mem_size);
    }
#endif

#if TLKALG_ASRC_48TO16_24BIT_ENABLE
    p_audio_alg_if            = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
    if (s_alg_asrc_48to16_buff == NULL) {
        #if 0//(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
        uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        #else
        uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        #endif
        tlkapi_printf(APP_AUDIO_LOG_EN, "ALG_ASRC_48TO16_mem_size: %d", asrc_48to16_size);

        s_alg_asrc_48to16_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
        if (s_alg_asrc_48to16_buff == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "ASRC 48TO16 buffer malloc failed");
        }
        #if 0//(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
        p_audio_alg_if->audio_alg_init(s_alg_asrc_48to16_buff, ALG_CHANNEL_STEREO);
        #else
        p_audio_alg_if->audio_alg_init(s_alg_asrc_48to16_buff, ALG_CHANNEL_LEFT);
        #endif
    }
#endif
    
#if TLKALG_LC3_PLUS_ENC_ENABLE && TLKALG_LC3_PLUS_DEC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer == NULL) {
        if (ll_audio_get_ultra_low_latency_flag()) {
            p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_ULTRA_LOW_LATENCY, NULL);
        } else {
            p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_NORMAL, NULL);
        }

        //ENCORD BUFF
        uint8_t size_param = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_dec_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_buffer, size_param);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);

        //SCRATCH BUFF
        size_param = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_dec_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_scratch_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec scratch buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer == NULL) {
        if (ll_audio_get_ultra_low_latency_flag()) {
            p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY, NULL);
        } else {
            p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_NORMAL, NULL);
        }

        //ENCORD BUFF
        uint8_t size_param = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_enc_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus enc buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);

        //SCRATCH BUFF
        size_param = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_enc_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_scratch_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc scratch_buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);
    }

#endif
}

/**
 * @brief      Deinitialize audio algorithms.
 * @param      None.
 * @return     None.
 */
void ll_audio_alg_deinit(void)
{
    audio_alg_interface_t *p_audio_alg_if;
    (void) p_audio_alg_if;
#if TLKALG_LC3_24BIT_DEC_ENABLE && TLKALG_LC3_24BIT_ENC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
    if (s_alg_lc3_24bit_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_24bit_dec_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_24bit_dec_buffer = NULL;
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
    if (s_alg_lc3_24bit_enc_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_24bit_enc_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_24bit_enc_buffer = NULL;
    }
#endif

#if TLKALG_ASRC_48TO16_24BIT_ENABLE
    audio_alg_interface_t *p_alg_48to16_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
    if (s_alg_asrc_48to16_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_48to16_buff);
        p_alg_48to16_if->audio_alg_deinit();
        s_alg_asrc_48to16_buff = NULL;
    }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE && TLKALG_LC3_PLUS_DEC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_dec_buffer);
        tlkalg_free_func(s_alg_lc3_plus_dec_scratch_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_plus_dec_buffer = NULL;
        s_alg_lc3_plus_dec_scratch_buffer = NULL;
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_enc_buffer);
        tlkalg_free_func(s_alg_lc3_plus_enc_scratch_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_plus_enc_buffer = NULL;
        s_alg_lc3_plus_enc_scratch_buffer = NULL;
    }
#endif
}

/**
 * @brief      Get current audio mode.
 * @param[in]  pData - Data pointer for mode retrieval.
 * @return     Current audio mode.
 */
uint16_t ll_audio_get_mode(uint8_t *pData)
{
    ll_audio_set_mode(pData[0]);

#if AUDIO_TWS_MODE
    uint8_t ultra_ll_mode = pData[1];
    tlkapi_printf(APP_AUDIO_LOG_EN, "=== ultra_ll_mode: %d", ultra_ll_mode);
    ll_audio_set_ultra_low_latency_flag(ultra_ll_mode);
#endif

    return pData[0];
}

/**
 * @brief      Switch input source for audio processing.
 * @param      None.
 * @return     None.
 */
void ll_audio_switch_in(void)
{
    ll_audio_alg_init();
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, ll_audio_main);
    tlkdrv_codec_muteSpkBuff();
    ll_audio_enter_audio_mode();
}

/**
 * @brief      Switch output destination for audio processing.
 * @param      None.
 * @return     None.
 */
void ll_audio_switch_out(void)
{
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
    tlkmdi_audio_stop_timer();
    ll_audio_exit_audio_mode();
    ll_audio_alg_deinit();
}

#endif //#if (TLKSTK_BT_TPS_ENABLE)
