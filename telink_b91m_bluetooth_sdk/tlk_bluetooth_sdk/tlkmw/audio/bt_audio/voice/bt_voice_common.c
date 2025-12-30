/********************************************************************************************************
 * @file    bt_voice_common.c
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
#include "tlkmw/audio/tlkmw_audio.h"

#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/a2dp/btp_a2dp.h"
#include "stack/bt/host/btp/avrcp/btp_avrcp.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"

#if AUDIO_TWS_MODE
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"
#endif

#if (TLKBTP_CFG_HFP_ENABLE)
#define TLKMDI_BT_VOICE_DBG_FLAG      ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_CC_BT_AUDIO << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BT_VOICE_DBG_SIGN      "[MDI BT_V]"

#define TLKMDI_BT_VOICE_ENC_BUFF_SIZE SPK_ENC_BUFF_SIZE *SPK_ENC_BUFF_NUM + MIC_ENC_BUFF_SIZE *MIC_ENC_BUFF_NUM

static uint8_t *s_alg_voice_dec_buff = NULL;
static uint8_t *s_alg_voice_enc_buff = NULL;
#if TLKALG_AGC_ENABLE
uint8_t *g_alg_voice_agc_buff = NULL;
#endif
#if TLKALG_ANS_ENABLE
uint8_t *g_alg_voice_ans_buff = NULL;
#endif
#if TLK_ALG_HYBRID_ENABLE
static uint8_t *s_alg_voice_hybrid_buff = NULL;
#endif

#if (TLKALG_ASRC_16TO48_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
uint8_t *s_alg_asrc_16to48_buff = NULL;
#endif

#if (TLKALG_ASRC_48TO16_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
static uint8_t *s_alg_asrc_48to16_16bit_buff = NULL;
#endif

#if (TLKALG_ASRC_48TO16_24BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
static uint8_t *s_alg_asrc_48to16_24bit_buff = NULL;
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
static uint8_t *s_alg_lc3_24bit_dec_buffer = NULL;
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
static uint8_t *s_alg_lc3_plus_dec_buffer         = NULL;
static uint8_t *s_alg_lc3_plus_dec_scratch_buffer = NULL;
#endif

#if DONGLE_VOICE_MIC_EN && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
#if TLKALG_LC3_24BIT_ENC_ENABLE
static uint8_t *s_alg_lc3_24bit_enc_buffer = NULL;
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
static uint8_t *s_alg_lc3_plus_enc_buffer         = NULL;
static uint8_t *s_alg_lc3_plus_enc_scratch_buffer = NULL;
#endif
#endif

extern bt_voice_cfg_t bt_voice_cfg;

#if SCO_ENC_QUEUE_ENABLE
extern void btif_sco_enc_buff_init(uint8_t *p_sco_buff);
#endif

/**
 * @brief   Initialize BT voice buffer and algorithm
 * @param[in]  None
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_buff_and_alg_init(void)
{
    bool                   ret                = true;
    uint8_t               *p_bt_voice_enc_buf = NULL;
    audio_alg_interface_t *p_enc_alg_if;
    audio_alg_interface_t *p_dec_alg_if;
#if (TLK_ALG_HYBRID_ENABLE || TLKALG_AGC_ENABLE || TLKALG_ANS_ENABLE)
    audio_alg_interface_t *p_voice_alg_if;
#endif
    void *p_enc_func;
    void *p_dec_func;

    if (btif_get_hfp_codec() == HFP_CODEC_ID_CVSD) {
        p_enc_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_ENC);
        p_dec_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_DEC);

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@enter CVSD");
    } else {
        p_enc_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_ENC);
        p_dec_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_DEC);

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@enter MSBC");
    }

    p_bt_voice_enc_buf = (uint8_t *)tlkmdi_audmem_calloc(TLKMDI_BT_VOICE_ENC_BUFF_SIZE);
    if (p_bt_voice_enc_buf == NULL) {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "malloc voice enc buff failed");
        ret = false;
    } else {
        bt_voice_set_buffer(p_bt_voice_enc_buf);
    }

    /* register bt voice encode callback and decode callback */
    p_enc_func = p_enc_alg_if->audio_alg_process;
    p_dec_func = p_dec_alg_if->audio_alg_process;
    bt_voice_register_codec_callback(p_enc_func, p_dec_func);

    /* encode alg init */
    if (s_alg_voice_enc_buff == NULL) {
        s_alg_voice_enc_buff = (uint8_t *)tlkalg_malloc_func(p_enc_alg_if->audio_alg_get_size(0));
        p_enc_alg_if->audio_alg_init(s_alg_voice_enc_buff, 0);
    }

    /* decode alg init  */
    if (s_alg_voice_dec_buff == NULL) {
        s_alg_voice_dec_buff = (uint8_t *)tlkalg_malloc_func(p_dec_alg_if->audio_alg_get_size(0));
        p_dec_alg_if->audio_alg_init(s_alg_voice_dec_buff, 0);
    }

#if TLKMW_INTERPHONE_EN
    if (!tlkmdi_interphone_is_busy())
#endif
    {
#if TLKALG_AGC_ENABLE
        p_voice_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
        if (g_alg_voice_agc_buff == NULL) {
            g_alg_voice_agc_buff = (uint8_t *)tlkalg_malloc_func(p_voice_alg_if->audio_alg_get_size(0));
            p_voice_alg_if->audio_alg_init(g_alg_voice_agc_buff, 0);
        }
#endif

#if TLKALG_ANS_ENABLE
        p_voice_alg_if = audio_alg_get_interface_by_type(ALG_ANS);
        if (g_alg_voice_ans_buff == NULL) {
            g_alg_voice_ans_buff = (uint8_t *)tlkalg_malloc_func(p_voice_alg_if->audio_alg_get_size(0));
            p_voice_alg_if->audio_alg_init(g_alg_voice_ans_buff, 0);
        }
#endif
    }

#if TLK_ALG_HYBRID_ENABLE
    p_voice_alg_if = audio_alg_get_interface_by_type(ALG_HYBRID);
    if (s_alg_voice_hybrid_buff == NULL) {
        s_alg_voice_hybrid_buff = (uint8_t *)tlkalg_malloc_func(p_voice_alg_if->audio_alg_get_size(0));
        p_voice_alg_if->audio_alg_init(s_alg_voice_hybrid_buff, 0);
    }
#endif

#if (TLKALG_ASRC_16TO48_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    audio_alg_interface_t *p_16to48_16bit   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    uint16_t               asrc_16to48_size = p_16to48_16bit->audio_alg_get_size(ALG_CHANNEL_LEFT);
    s_alg_asrc_16to48_buff                  = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
    if (s_alg_asrc_16to48_buff == NULL) {
        tlkapi_printf(1, "s_alg_voice_asrc_buff malloc failed");
    }
    p_16to48_16bit->audio_alg_init(s_alg_asrc_16to48_buff, ALG_CHANNEL_LEFT);
#endif

#if (TLKALG_ASRC_48TO16_24BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    audio_alg_interface_t *p_48to16_24bit = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)  //0: only nn_ns running in dsp and no other voice processing alg running
    uint16_t asrc_48to16_24bit_size = p_48to16_24bit->audio_alg_get_size(ALG_CHANNEL_STEREO);
#else
    uint16_t asrc_48to16_24bit_size = p_48to16_24bit->audio_alg_get_size(ALG_CHANNEL_LEFT);
#endif
    tlkapi_printf(APP_AUDIO_LOG_EN, "ALG_ASRC_48TO16_24bit_mem_size: %d", asrc_48to16_24bit_size);

    s_alg_asrc_48to16_24bit_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_24bit_size);
    if (s_alg_asrc_48to16_24bit_buff == NULL) {
        tlkapi_printf(APP_AUDIO_LOG_EN, "ASRC 48TO16 24bit buffer malloc failed");
    }
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)  //0: only nn_ns running in dsp and no other voice processing alg running
    p_48to16_24bit->audio_alg_init(s_alg_asrc_48to16_24bit_buff, ALG_CHANNEL_STEREO);
#else
    p_48to16_24bit->audio_alg_init(s_alg_asrc_48to16_24bit_buff, ALG_CHANNEL_LEFT);
#endif
#endif

#if (TLKALG_ASRC_48TO16_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    audio_alg_interface_t *p_asrc_48to16_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)  //0: only nn_ns running in dsp and no other voice processing alg running 
    uint16_t               asrc_48to16_16bit_size = p_asrc_48to16_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
#else
    uint16_t asrc_48to16_16bit_size = p_asrc_48to16_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
#endif
    s_alg_asrc_48to16_16bit_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_16bit_size);
    if (s_alg_asrc_48to16_16bit_buff == NULL) {
        tlkapi_printf(1, "spTlkMdiU2hAsrc48to16Buff malloc failed");
    }
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)  //0: only nn_ns running in dsp and no other voice processing alg running
    p_asrc_48to16_if->audio_alg_init(s_alg_asrc_48to16_16bit_buff, ALG_CHANNEL_STEREO);
#else
    p_asrc_48to16_if->audio_alg_init(s_alg_asrc_48to16_16bit_buff, ALG_CHANNEL_LEFT);
#endif
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    audio_alg_interface_t *p_lc3_dec_24bit_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);

    if (s_alg_lc3_24bit_dec_buffer == NULL) {
        uint16_t lc3_24bit_dec_mem_size = p_lc3_dec_24bit_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_dec_mem_size);
        if (s_alg_lc3_24bit_dec_buffer == NULL) {
            tlkapi_printf(1, "s_alg_lc3_24bit_dec_buffer malloc failed");
        }
        p_lc3_dec_24bit_if->audio_alg_init(s_alg_lc3_24bit_dec_buffer, ALG_CHANNEL_STEREO);

        tlkapi_printf(1, "lc3_24bit_dec_mem_size: %d", lc3_24bit_dec_mem_size);
    }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    audio_alg_interface_t *p_lc3_plus_dec_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer == NULL) {
        p_lc3_plus_dec_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_NORMAL, NULL);

        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_dec_mem_size = p_lc3_plus_dec_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec buffer alloc failed");
        }
        p_lc3_plus_dec_if->audio_alg_init(s_alg_lc3_plus_dec_buffer, size_param);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_dec_mem_size             = p_lc3_plus_dec_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec scratch buffer alloc failed");
        }
        p_lc3_plus_dec_if->audio_alg_init(s_alg_lc3_plus_dec_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);
    }
#endif

#if DONGLE_VOICE_MIC_EN && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
#if TLKALG_LC3_24BIT_ENC_ENABLE
    audio_alg_interface_t *p_lc3_24bit = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
    if (s_alg_lc3_24bit_enc_buffer == NULL) {
        uint16_t lc3_24bit_enc_mem_size = p_lc3_24bit->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_enc_mem_size);
        if (s_alg_lc3_24bit_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 4bit enc buffer alloc failed");
        }
        p_lc3_24bit->audio_alg_init(s_alg_lc3_24bit_enc_buffer, ALG_CHANNEL_LEFT);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_enc_mem_size: %d", lc3_24bit_enc_mem_size);
    }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
    audio_alg_interface_t *p_lc3_plus_enc_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer == NULL) {
        p_lc3_plus_enc_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_NORMAL, NULL);

        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_enc_mem_size = p_lc3_plus_enc_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus enc buffer alloc failed");
        }
        p_lc3_plus_enc_if->audio_alg_init(s_alg_lc3_plus_enc_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_enc_mem_size             = p_lc3_plus_enc_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc scratch_buffer alloc failed");
        }
        p_lc3_plus_enc_if->audio_alg_init(s_alg_lc3_plus_enc_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);
    }
#endif

#endif

#if AUDIO_TWS_MODE
    app_tws_spk_ppm_init(1);
#endif

    tlkmdi_audmem_print();

    return ret;
}

/**
 * @brief   Deinitialize BT voice buffer and algorithm
 * @param[in]  None
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_buff_and_alg_deinit(void)
{
    bool                   ret                = true;
    uint8_t               *p_bt_voice_enc_buf = NULL;
    audio_alg_interface_t *p_enc_alg_if;
    audio_alg_interface_t *p_dec_alg_if;
#if (TLKALG_AGC_ENABLE || TLK_ALG_HYBRID_ENABLE || TLKALG_ANS_ENABLE)
    audio_alg_interface_t *p_voice_alg_if;
#endif
    if (btif_get_hfp_codec() == HFP_CODEC_ID_CVSD) {
        p_enc_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_ENC);
        p_dec_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_DEC);
    } else {
        p_enc_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_ENC);
        p_dec_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_DEC);
    }

    p_bt_voice_enc_buf = bt_voice_get_buffer();
    if (p_bt_voice_enc_buf != NULL) {
        tlkmdi_audmem_free(p_bt_voice_enc_buf);
        bt_voice_set_buffer(NULL);
    }

    bt_voice_register_codec_callback(NULL, NULL);

    /* encode alg deinit */
    if (s_alg_voice_enc_buff != NULL) {
        tlkalg_free_func(s_alg_voice_enc_buff);
        p_enc_alg_if->audio_alg_deinit();
        s_alg_voice_enc_buff = NULL;
    }

    /* decode alg deinit */
    if (s_alg_voice_dec_buff != NULL) {
        tlkalg_free_func(s_alg_voice_dec_buff);
        p_dec_alg_if->audio_alg_deinit();
        s_alg_voice_dec_buff = NULL;
    }
#if TLKMW_INTERPHONE_EN
    if (!tlkmdi_interphone_is_busy())
#endif
    {
#if TLKALG_AGC_ENABLE
        if (g_alg_voice_agc_buff != NULL) {
            tlkalg_free_func(g_alg_voice_agc_buff);
            g_alg_voice_agc_buff = NULL;

            p_voice_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
            p_voice_alg_if->audio_alg_deinit();
        }
#endif

#if TLKALG_ANS_ENABLE
        if (g_alg_voice_ans_buff != NULL) {
            tlkalg_free_func(g_alg_voice_ans_buff);
            g_alg_voice_ans_buff = NULL;

            p_voice_alg_if = audio_alg_get_interface_by_type(ALG_ANS);
            p_voice_alg_if->audio_alg_deinit();
        }
#endif
    }
#if TLK_ALG_HYBRID_ENABLE
    if (s_alg_voice_hybrid_buff != NULL) {
        tlkalg_free_func(s_alg_voice_hybrid_buff);
        s_alg_voice_hybrid_buff = NULL;

        p_voice_alg_if = audio_alg_get_interface_by_type(ALG_HYBRID);
        p_voice_alg_if->audio_alg_deinit();
    }
#endif

#if (TLKALG_ASRC_16TO48_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    // audio_alg_interface_t *p_alg_16to48_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_alg_16to48_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    if (s_alg_asrc_16to48_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_16to48_buff);
        p_alg_16to48_if->audio_alg_deinit();
        s_alg_asrc_16to48_buff = NULL;
    }
#endif

#if (TLKALG_ASRC_48TO16_16BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    // audio_alg_interface_t *p_alg_48to16_16bit_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_alg_48to16_16bit_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    if (s_alg_asrc_48to16_16bit_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_48to16_16bit_buff);
        p_alg_48to16_16bit_if->audio_alg_deinit();
        s_alg_asrc_48to16_16bit_buff = NULL;
    }
#endif

#if (TLKALG_ASRC_48TO16_24BIT_ENABLE && (TLKSTK_BT_TPS_ENABLE) && (MCU_CORE_TYPE == MCU_CORE_TL751X))
    // audio_alg_interface_t *p_alg_48to16_24bit_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_alg_48to16_24bit_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
    if (s_alg_asrc_48to16_24bit_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_48to16_24bit_buff);
        p_alg_48to16_24bit_if->audio_alg_deinit();
        s_alg_asrc_48to16_24bit_buff = NULL;
    }
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    audio_alg_interface_t *p_lc3_24bit_dec_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
    if (s_alg_lc3_24bit_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_24bit_dec_buffer);
        p_lc3_24bit_dec_if->audio_alg_deinit();
        s_alg_lc3_24bit_dec_buffer = NULL;
    }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    audio_alg_interface_t *p_lc3_plus_dec_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_dec_buffer);
        tlkalg_free_func(s_alg_lc3_plus_dec_scratch_buffer);
        p_lc3_plus_dec_if->audio_alg_deinit();
        s_alg_lc3_plus_dec_buffer         = NULL;
        s_alg_lc3_plus_dec_scratch_buffer = NULL;
    }
#endif

#if DONGLE_VOICE_MIC_EN && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
#if TLKALG_LC3_24BIT_ENC_ENABLE
    audio_alg_interface_t *p_lc3_24bit_enc_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
    if (s_alg_lc3_24bit_enc_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_24bit_enc_buffer);
        p_lc3_24bit_enc_if->audio_alg_deinit();
        s_alg_lc3_24bit_enc_buffer = NULL;
    }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
    audio_alg_interface_t *p_lc3_plus_enc_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_enc_buffer);
        tlkalg_free_func(s_alg_lc3_plus_enc_scratch_buffer);
        p_lc3_plus_enc_if->audio_alg_deinit();
        s_alg_lc3_plus_enc_buffer         = NULL;
        s_alg_lc3_plus_enc_scratch_buffer = NULL;
    }
#endif

#endif

#if AUDIO_TWS_MODE
    app_tws_spk_ppm_deinit();
#endif

#if ((TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE) && MIX_MODE_PPM_ENABLE)
    ll_audio_spk_ppm_deinit();
#endif

    return ret;
}

/**
 * @brief   Initialize mic enc buffer and cvsd in bt voice mode
 * @param[in]  none
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_audio_path_init(void)
{
    bool ret = true;

    bt_voice_cfg.spk_enc_rptr = 0;
    bt_voice_cfg.spk_enc_wptr = 0;

    bt_voice_cfg.mic_enc_rptr = 0;
    bt_voice_cfg.mic_enc_wptr = 0;

    bt_voice_cfg.tick      = 0;
    bt_voice_cfg.status    = BT_VOICE_ST_IDLE;
    bt_voice_cfg.init      = 0;
    bt_voice_cfg.sync_init = 0;

    bt_voice_cfg.dec_func  = NULL;
    bt_voice_cfg.enc_func  = NULL;
    bt_voice_cfg.plc_func  = NULL;
    bt_voice_cfg.mic_state = 0;

    ret = bt_voice_buff_and_alg_init();
    if (!ret) {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "bt_voice_buff_and_alg_init fail: %d", ret);
        return false;
    }

    bt_voice_init_mic_enc_buff();

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@ bt_voice_audio_path_init");

#if AUDIO_TWS_MODE
    bt_voice_sync_init();
#endif

#if SCO_ENC_QUEUE_ENABLE
    btif_sco_enc_buff_init(g_sco_data_buff);
#endif

#if (TLKSTK_BT_TPS_ENABLE)
#if (LE_AUDIO_BT_MUSIC_MIX_ENABLE || LE_AUDIO_BT_VOICE_MIX_ENABLE)
    ll_audio_bt_music_mix_init();
#endif
#endif

    return true;
}

/**
 * @brief   Deinitialize BT voice audio path
 * @param[in]  None
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_audio_path_deinit(void)
{
    bt_voice_cfg_t *p_bt_voice_env = get_bt_voice_env_ptr();
    bool            ret            = true;

    bt_voice_reset();
    p_bt_voice_env->sco_data_ready = 0;
#if AUDIO_TWS_MODE
    p_bt_voice_env->voice_play_sync = 0;
#endif

    ret = bt_voice_buff_and_alg_deinit();
    if (!ret) {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "bt_voice_buff_and_alg_deinit fail: %d", ret);
        return false;
    }

    return true;
}

/**
 * @brief   Open codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_open_codec(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_B92)
    g_codec_cfg.sample_rate = AUDIO_SAMPLE_RATE_16K;
#elif (TLKSTK_BT_TPS_ENABLE)
    g_codec_cfg.sample_rate       = AUDIO_SAMPLE_RATE_48K;
    g_codec_cfg.codec_sample_rate = AUDIO_SAMPLE_RATE_48K;
#else
    g_codec_cfg.sample_rate       = AUDIO_SAMPLE_RATE_16K;
    g_codec_cfg.codec_sample_rate = AUDIO_SAMPLE_RATE_48K;
#endif
#if TLK_CFG_HRA_ENABLE
    g_codec_cfg.codec_sample_rate = AUDIO_SAMPLE_RATE_16K;
#endif

#if AUDIO_PATH_24BITS_EN
#if 0 //AUDIO_HD_HAC_EN
    st_audio_sys_clk = 36864000;
    tlkdrv_hac_init(&(gp_hac_cfg_st[0]), st_audio_sys_clk, 16000, 48000,120);
#endif
    // tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, AUDIO_SAMPLE_RATE_48K, 4096);

#if CODEC_DAC_MONO_MODE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate, 4096);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate, 4096);
#endif

#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
        //mic stereo for future enc alg
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, AUDIO_SAMPLE_RATE_16K, 4096);
        bt_voice_cfg.mic_state = 1;
    }
#else
    //mic stereo for future enc alg
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, AUDIO_SAMPLE_RATE_16K, 4096);
#endif

#else
#if TLK_CFG_HRA_ENABLE && TLKALG_MIC_SPK_MONO_ENABLE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, g_codec_cfg.sample_rate, 0);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, g_codec_cfg.sample_rate, 0);

    tlkdrv_codec_sync_play_samples(240 * 3);
#endif

#if TLK_CFG_HRA_ENABLE
    tlkdrv_codec_sync_speaker_samples(g_codec_cfg.sample_rate / 1000);     //sample rate*ms*chnl
    tlkdrv_codec_sync_mic_samples(g_codec_cfg.sample_rate / 1000 * 2 + 4); //sample rate*ms*chnl
#endif

#endif // AUDIO_PATH_24BITS_EN
}

/**
 * @brief   Close codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_close_codec(void)
{
    tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
    tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
    bt_voice_cfg.mic_state = 0;
}

/**
 * @brief   Close ANC codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_anc_close_codec(void)
{
    tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
    bt_voice_cfg.mic_state = 0;
}

/**
 * @brief   Switch in handler for BT voice
 * @param[in]  handle - connection handle
 * @returns    None
 */
void tlkmdi_btvoice_switch_in(uint16_t handle)
{
#if AUDIO_TWS_MODE
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_SYNC_DATA_TWS_AUD, tlkmdi_mailbox_audio_sync_callback);
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_TWS_SLAVE_AUDIO_SYNC_DATA, tlkmdi_mailbox_tws_slave_audio_sync_callback);
#endif
    bool ret = bt_voice_audio_path_init();
    if (!ret) {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "bt_voice_audio_path_init fail: %d", ret);
        return;
    }

#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle())
#endif
    {
#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
        tlkmw_dsp_resume();
        bt_voice_clear_dsp_ret_buff_status();
        ipc_msg_register_data_process_done_cb(bt_voice_dsp_msg_process_callback, BT_VOICE, BT_VOICE_ID);
#if DSP_NN_NS_BUFF_DISENABLE
        dsp_voice_nn_comm_reset_status();
#endif
#endif
    }

#if TLKMW_INTERPHONE_EN
    if (bt_voice_master_is_interphone()) {
        tlkmdi_mute_i2s_tx_buff();
        audio_tx_dma_en(INTERPHONE_I2S_TX_DMA);
    }
#endif

    tlkmw_audio_btif_inform_host_audio_en(handle, false);
    bt_audio_set_voice_vol_percent_by_handle(handle);
    bt_audio_register_get_pcm_data_callback(bt_voice_get_playback_data);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_audio_main);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, bt_audio_main_loop);
    bt_audio_task_register_run_cb(NULL, 1);

#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, ll_mix_fifo0_irq_handler_func);
#endif

#if TLK_CFG_HRA_ENABLE
    tlkmdi_hra_voice_init();
#endif
}

/**
 * @brief   Switch out handler for BT voice
 * @param[in]  handle - connection handle
 * @returns    None
 */
void tlkmdi_btvoice_switch_out(uint16_t handle)
{
#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
    ll_mix_mic_deinit();
#endif

#if TLKMW_INTERPHONE_EN
    if (bt_voice_master_is_interphone()) {
        audio_tx_dma_dis(INTERPHONE_I2S_TX_DMA);
    }
    tlkdrv_codec_muteSpkBuff();
#endif


    bt_audio_task_register_run_cb(NULL, 0);
    bt_audio_register_get_pcm_data_callback(NULL);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);

    bool ret = bt_voice_audio_path_deinit();
    if (!ret) {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "bt_voice_audio_path_deinit fail: %d", ret);
    }
    tlkmw_audio_btif_inform_host_audio_dis(handle);
}

#endif
