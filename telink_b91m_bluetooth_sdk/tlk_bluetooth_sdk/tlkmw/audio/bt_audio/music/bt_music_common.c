/********************************************************************************************************
 * @file    bt_music_common.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "drivers.h"
#include "bt_music.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"

#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
#define TLKMDI_BT_MUSIC_ENC_FIFO_SIZE BT_MUSIC_ENC_FIFO_SIZE

static uint8_t *s_alg_sbc_dec_buffer = NULL;
#if TLKALG_PPM_SPK_ENABLE && !AUDIO_HD_HAC_EN
static uint8_t *s_alg_ppm_stereo_buffer = NULL;
#endif

#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
static uint8_t *s_alg_eq_buff = NULL;
#endif
#if TLKALG_ASRC_441TO48_16BIT_ENABLE
static uint8_t *s_alg_asrc_441to48_buff = NULL;
#endif
#if TLKALG_ASRC_441TO16_16BIT_ENABLE
static uint8_t *s_alg_asrc_441to16_buff = NULL;
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

#if TLKALG_ASRC_48TO16_24BIT_ENABLE
static uint8_t *s_alg_asrc_48to16_buff = NULL;
#endif
#endif

#if AAC_CODEC_ENABLE
static uint8_t *s_alg_aac_dec_buffer = NULL;
#endif

static uint8_t bt_codec_type = 0;

/**
 * @brief       Set BT codec type
 * @param[in]   type - Codec type to set
 * @return      None
 */
void bt_music_set_codec_type(uint8_t type)
{
    bt_codec_type = type;
}

#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
/**
 * @brief       Initialize BT music EQ algorithm
 * @param[in]   samplerate - Sample rate
 * @param[in]   chnl - Channel index
 * @param[in]   eq_type - EQ type
 * @return      None
 */
void bt_music_alg_eq_init(uint32_t samplerate, ALG_CHANNEL_IDX chnl, e_eq_type_e eq_type)
{
    tlkapi_printf(APP_LOG_EN, "bt_music_alg_eq_init");
    if (s_alg_eq_buff == NULL) {
        tlkapi_printf(APP_LOG_EN, "EQ set para and init");
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
        s_alg_eq_buff                         = (uint8_t *)tlkalg_malloc_func(4);
        eq_para_t btmusic_eq_para             = {
                        .samplerate = samplerate,
                        .channel    = chnl,
                        .eq_type    = eq_type,
            // .width = ALG_WIDTH_16,
        };
        p_audio_alg_if->audio_alg_param_set(0, (void *)&btmusic_eq_para);
        p_audio_alg_if->audio_alg_init(s_alg_eq_buff, ALG_CHANNEL_STEREO);
    }
}
#endif

/**
 * @brief       Initialize BT music buffers and algorithms
 * @param[in]   None
 * @return      Operation result, TRUE on success, FALSE on failure
 */
bool bt_music_buff_and_alg_init(void)
{
    uint8_t               *p_bt_music_enc_buf = NULL;
    audio_alg_interface_t *p_audio_alg_if     = NULL;

    p_bt_music_enc_buf = (uint8_t *)tlkmdi_audmem_calloc(TLKMDI_BT_MUSIC_ENC_FIFO_SIZE);
    if (p_bt_music_enc_buf == NULL) {
        tlkapi_printf(APP_LOG_EN, "malloc music enc buff failed");
        return false;
    } else {
        bt_music_set_enc_buffer(p_bt_music_enc_buf);
    }

    if (bt_codec_type == SEPID_SBC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_DEC);

        if (s_alg_sbc_dec_buffer == NULL) {
            uint16_t sbc_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_alg_sbc_dec_buffer      = (uint8_t *)tlkalg_malloc_func(sbc_dec_mem_size);
            if (s_alg_sbc_dec_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "sbc dec buff alloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(s_alg_sbc_dec_buffer, ALG_CHANNEL_STEREO);

            tlkapi_printf(APP_LOG_EN, "sbc_dec_mem_size: %d", sbc_dec_mem_size);
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer == NULL) {
            int hd_hac_buff      = tlkdrv_hac_get_size(44100, 48000, 128, 24);
            g_hd_hac_asrc_buffer = (uint8_t *)tlkalg_malloc_func(hd_hac_buff);
            if (g_hd_hac_asrc_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "ASRC buff alloc failed");
                return false;
            }
            tlkapi_printf(APP_LOG_EN, "SBC ASRC _size: %d addr%d", hd_hac_buff, g_hd_hac_asrc_buffer);
        }
#endif
    } else if (bt_codec_type == SEPID_AAC) {
#if AAC_CODEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AAC_DEC);

        if (s_alg_aac_dec_buffer == NULL) {
            uint16_t aac_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_alg_aac_dec_buffer      = (uint8_t *)tlkalg_malloc_func(aac_dec_mem_size);
            if (s_alg_aac_dec_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "aac dec buff alloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(s_alg_aac_dec_buffer, ALG_CHANNEL_STEREO);

            tlkapi_printf(APP_LOG_EN, "aac_dec_mem_size: %d", aac_dec_mem_size);
        }
#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer == NULL) {
            int hd_hac_buff      = tlkdrv_hac_get_size(44100, 48000, 1024, 24);
            g_hd_hac_asrc_buffer = (uint8_t *)tlkalg_malloc_func(hd_hac_buff);
            if (g_hd_hac_asrc_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "ASRC buff alloc failed");
                return false;
            }
            tlkapi_printf(APP_LOG_EN, "AAC ASRC _size: %d addr%d", hd_hac_buff, g_hd_hac_asrc_buffer);
        }
#endif
#endif
    }

#if TLKALG_PPM_SPK_ENABLE && !AUDIO_HD_HAC_EN
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
    if (s_alg_ppm_stereo_buffer == NULL) {
        uint16_t ppm_stereo_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_ppm_stereo_buffer      = (uint8_t *)tlkalg_malloc_func(ppm_stereo_mem_size);
        if (s_alg_ppm_stereo_buffer == NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm_spk alloc failed");
            return false;
        }
        p_audio_alg_if->audio_alg_init(s_alg_ppm_stereo_buffer, ALG_CHANNEL_STEREO);

        tlkapi_printf(APP_LOG_EN, "ppm_stereo_mem_size: %d", ppm_stereo_mem_size);
    }
#endif

#if DONGLE_VOICE_MIC_EN && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
#if TLKALG_LC3_24BIT_ENC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
    if (s_alg_lc3_24bit_enc_buffer == NULL) {
        uint16_t lc3_24bit_enc_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_enc_mem_size);
        if (s_alg_lc3_24bit_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 24bit enc buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_enc_buffer, ALG_CHANNEL_LEFT);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_enc_mem_size: %d", lc3_24bit_enc_mem_size);
    }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer == NULL) {
        p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_NORMAL, NULL); //TODO:WQ

        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_enc_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus enc buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_enc_mem_size             = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc scratch_buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);
    }
#endif

#if TLKALG_ASRC_48TO16_24BIT_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
    if (s_alg_asrc_48to16_buff == NULL) {
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
        uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
#else
        uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
#endif
        tlkapi_printf(APP_AUDIO_LOG_EN, "ALG_ASRC_48TO16_mem_size: %d", asrc_48to16_size);

        s_alg_asrc_48to16_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
        if (s_alg_asrc_48to16_buff == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "ASRC 48TO16 buffer malloc failed");
        }
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
        p_audio_alg_if->audio_alg_init(s_alg_asrc_48to16_buff, ALG_CHANNEL_STEREO);
#else
        p_audio_alg_if->audio_alg_init(s_alg_asrc_48to16_buff, ALG_CHANNEL_LEFT);
#endif
    }
#endif
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET) //TODO: xiaogang :Optimization of macros management.
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);

    if (s_alg_lc3_24bit_dec_buffer == NULL) {
        uint16_t lc3_24bit_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_lc3_24bit_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_dec_mem_size);
        if (s_alg_lc3_24bit_dec_buffer == NULL) {
            tlkapi_printf(1, "s_alg_lc3_24bit_dec_buffer alloc failed");
            return false;
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_dec_buffer, ALG_CHANNEL_STEREO);

        tlkapi_printf(1, "lc3_24bit_dec_mem_size: %d", lc3_24bit_dec_mem_size);
    }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer == NULL) {
        p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_NORMAL, NULL); //TODO:WQ

        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_dec_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_buffer, size_param);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_dec_mem_size             = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec scratch buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);
    }
#endif

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE && !TLKMW_INTERPHONE_EN && !TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE)
    // p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_ASRC_441TO48);
    p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    uint16_t asrc_441to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    s_alg_asrc_441to48_buff    = (uint8_t *)tlkalg_malloc_func(asrc_441to48_size);
    if (s_alg_asrc_441to48_buff == NULL) {
        tlkapi_printf(APP_LOG_EN, "asrc 441to48 alloc failed");
        return false;
    }
    p_audio_alg_if->audio_alg_init(s_alg_asrc_441to48_buff, ALG_CHANNEL_STEREO);
#endif

#if AUDIO_TWS_MODE
    app_tws_spk_ppm_init(1);
#endif

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
    // p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_ASRC_441TO16);
    p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
    uint16_t asrc_441to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    s_alg_asrc_441to16_buff    = (uint8_t *)tlkalg_malloc_func(asrc_441to16_size);
    if (s_alg_asrc_441to16_buff == NULL) {
        tlkapi_printf(APP_LOG_EN, "asrc 441to16 alloc failed");
        return false;
    }
    p_audio_alg_if->audio_alg_init(s_alg_asrc_441to16_buff, ALG_CHANNEL_STEREO);
#endif

    tlkmdi_audmem_print();

    return true;
}

#if TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE
/**
 * @brief       Initialize 44.1kHz to 48kHz ASRC conversion
 * @param[in]   None
 * @return      None
 */
void bt_music_441to48_init(void)
{
#if (TLKALG_ASRC_441TO48_16BIT_ENABLE && !TLKMW_INTERPHONE_EN)
    audio_alg_interface_t *p_audio_alg_if    = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    uint16_t               asrc_441to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    s_alg_asrc_441to48_buff                  = (uint8_t *)tlkalg_malloc_func(asrc_441to48_size);
    if (s_alg_asrc_441to48_buff == NULL) {
        tlkapi_printf(APP_LOG_EN, "asrc 441to48 alloc failed");
        return;
    }
    p_audio_alg_if->audio_alg_init(s_alg_asrc_441to48_buff, ALG_CHANNEL_STEREO);
#endif
}

/**
 * @brief       Deinitialize 44.1kHz to 48kHz ASRC conversion
 * @param[in]   None
 * @return      None
 */
void bt_music_441to48_deinit(void)
{
#if TLKALG_ASRC_441TO48_16BIT_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    if (s_alg_asrc_441to48_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_441to48_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_asrc_441to48_buff = NULL;
    }
#endif
}
#endif

/**
 * @brief       Deinitialize BT music buffers and algorithms
 * @param[in]   None
 * @return      Operation result, TRUE on success, FALSE on failure
 */
bool bt_music_buff_and_alg_deinit(void)
{
    bool                   ret                = true;
    uint8_t               *p_bt_music_enc_buf = NULL;
    audio_alg_interface_t *p_audio_alg_if     = NULL;

    p_bt_music_enc_buf = bt_music_get_enc_buffer();
    if (p_bt_music_enc_buf != NULL) {
        tlkmdi_audmem_free(p_bt_music_enc_buf);
        bt_music_set_enc_buffer(NULL);
    }

    if (bt_codec_type == SEPID_SBC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_DEC);

        if (s_alg_sbc_dec_buffer != NULL) {
            tlkalg_free_func(s_alg_sbc_dec_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_sbc_dec_buffer = NULL;
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer != NULL) {
            tlkalg_free_func(g_hd_hac_asrc_buffer);
            tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
            g_hd_hac_asrc_buffer = NULL;
        }
#endif
    } else if (bt_codec_type == SEPID_AAC) {
#if AAC_CODEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AAC_DEC);

        if (s_alg_aac_dec_buffer != NULL) {
            tlkalg_free_func(s_alg_aac_dec_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_aac_dec_buffer = NULL;
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer != NULL) {
            tlkalg_free_func(g_hd_hac_asrc_buffer);
            tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
            g_hd_hac_asrc_buffer = NULL;
        }
#endif

#endif
    }

#if TLKALG_PPM_SPK_ENABLE && !AUDIO_HD_HAC_EN
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
    if (s_alg_ppm_stereo_buffer != NULL) {
        tlkalg_free_func(s_alg_ppm_stereo_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_ppm_stereo_buffer = NULL;
    }
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
    if (s_alg_lc3_24bit_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_24bit_dec_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_24bit_dec_buffer = NULL;
    }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE && (PROJ_BTTPSLL_TWS || PROJ_BTTPSLL_HEADSET)
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_dec_buffer);
        tlkalg_free_func(s_alg_lc3_plus_dec_scratch_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_plus_dec_buffer         = NULL;
        s_alg_lc3_plus_dec_scratch_buffer = NULL;
    }
#endif

#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
    if (s_alg_eq_buff != NULL) {
        tlkalg_free_func(s_alg_eq_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_eq_buff = NULL;
    }
#endif

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE && !TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE)
    // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48);
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    if (s_alg_asrc_441to48_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_441to48_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_asrc_441to48_buff = NULL;
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
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer != NULL) {
        tlkalg_free_func(s_alg_lc3_plus_enc_buffer);
        tlkalg_free_func(s_alg_lc3_plus_enc_scratch_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_lc3_plus_enc_buffer         = NULL;
        s_alg_lc3_plus_enc_scratch_buffer = NULL;
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
#endif

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
    // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16);
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
    if (s_alg_asrc_441to16_buff != NULL) {
        tlkalg_free_func(s_alg_asrc_441to16_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_asrc_441to16_buff = NULL;
    }
#endif

#if AUDIO_TWS_MODE
    app_tws_spk_ppm_deinit();
#endif

#if ((TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE) && MIX_MODE_PPM_ENABLE)
    ll_audio_spk_ppm_deinit();
#endif

    return ret;
}

#if AUDIO_TWS_MODE
/**
 * @brief       Callback function for mailbox audio synchronization
 * @param[in]   p_data - Pointer to data
 * @param[in]   len - Data length
 * @return      None
 */
_attribute_ram_code_ void tlkmdi_mailbox_audio_sync_callback(void *p_data, uint16_t len)
{
    (void)len;
    uint8_t *p_sync_info     = (uint8_t *)p_data;
    uint8_t *p_data_tmp      = (uint8_t *)p_data;
    uint32_t task_tick       = p_data_tmp[0] | p_data_tmp[1] << 8 | p_data_tmp[2] << 16 | p_data_tmp[3] << 24;
    uint32_t stimer_irq_tick = p_data_tmp[4] | p_data_tmp[5] << 8 | p_data_tmp[6] << 16 | p_data_tmp[7] << 24;

    uint16_t fno           = p_data_tmp[8] | p_data_tmp[9] << 8;
    uint8_t  sco_id        = p_data_tmp[10];
    uint32_t task_start_ts = task_tick; //data[6] | data[7] << 8 | data[8] << 16 | data[9] << 24;

    g_bt_audio_task_env.stimer_irq_tick = stimer_irq_tick;
    g_bt_audio_task_env.task_start_ts   = task_start_ts;
    g_bt_audio_task_env.sco_id          = sco_id;

    //    extern void gpio_toggle_test(uint8_t times);
    //    gpio_toggle_test(sco_id);

    bt_audio_sync_update(task_tick, p_sync_info, fno);
}

extern uint8_t bt_tpt_handover_success;
extern bool    tlkmdi_bt_tpt_isSlave();
extern void    bt_music_tws_slave_init_param(void);

uint8_t g_sync_info_rcv_cnt = 0;

/**
 * @brief       Callback function for TWS slave audio synchronization
 * @param[in]   p_data - Pointer to data
 * @param[in]   len - Data length
 * @return      None
 */
_attribute_ram_code_ void tlkmdi_mailbox_tws_slave_audio_sync_callback(void *p_data, uint16_t len)
{
    (void)len;
    uint8_t  type = AUDIO_TYPE_MUSIC;
    uint8_t  master_info[TWS_SYNC_INFO_LEN];
    uint8_t  slave_info[TWS_SYNC_INFO_LEN];
    uint8_t *p_data_tmp      = (uint8_t *)p_data;
    uint32_t s_capture_tick  = 0; //p_data_tmp[s_tick_offset] | p_data_tmp[s_tick_offset+1] << 8 | p_data_tmp[s_tick_offset+2] << 16 | p_data_tmp[s_tick_offset+3] << 24;
    uint32_t m_schedule_tick = 0; //p_data_tmp[m_tick_offset] | p_data_tmp[m_tick_offset+1] << 8 | p_data_tmp[m_tick_offset+2] << 16 | p_data_tmp[m_tick_offset+3] << 24;

    if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btmusic_is_busy()
#endif
    ) {
        if (bt_music_cfg.rcv_sync_info == 1) {
            return;
        }
        bt_music_cfg.rcv_sync_info = 1;
    }

    if (tlkmdi_bt_voice_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btvoice_is_busy()
#endif
    ) {
        if (bt_voice_cfg.rcv_sync_info == 1) {
            return;
        }
        bt_voice_cfg.rcv_sync_info = 1;
    }

    if ((tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
         || tlkmdi_anc_btmusic_is_busy()
#endif
             ) &&
        (app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO)) {
        g_sync_info_rcv_cnt++;
        if (g_sync_info_rcv_cnt == 1) {
            return;
        }
    }

    if (bt_tpt_handover_success) {
        bt_tpt_handover_success = 0;
        if (tlkmdi_bt_tpt_isSlave()) {
            bt_music_tws_slave_init_param();
        }
    }

    tmemcpy(slave_info, p_data_tmp, TWS_SYNC_INFO_LEN);
    tmemcpy(master_info, p_data_tmp + 16, TWS_SYNC_INFO_LEN);
    tmemcpy(&s_capture_tick, p_data_tmp + 32, 4);
    tmemcpy(&m_schedule_tick, p_data_tmp + 36, 4);

    g_bt_audio_task_env.m_schedule_tick = m_schedule_tick;
    g_bt_audio_task_env.s_capture_tick  = s_capture_tick;

    if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btmusic_is_busy()
#endif
    ) {
        type = AUDIO_TYPE_MUSIC;
    } else if (tlkmdi_bt_voice_is_busy()
#if TLKALG_ANC_ENABLE
               || tlkmdi_anc_btvoice_is_busy()
#endif
    ) {
        type = AUDIO_TYPE_VOICE;
    }

    bt_audio_check_sync_cfg(master_info, slave_info, type);
    g_sync_info_rcv_cnt = 0;
}

/**
 * @brief       Callback function for TWS slave sync tick processing
 * @param[in]   p_data - Pointer to data
 * @param[in]   len - Data length
 * @return      None
 */
_attribute_ram_code_ void tlkmdi_mailbox_tws_slave_sync_tick_proc_callback(void *p_data, uint16_t len)
{
    (void)len;
    uint8_t *p_data_tmp      = (uint8_t *)p_data;
    uint32_t s_capture_tick  = p_data_tmp[0] | p_data_tmp[1] << 8 | p_data_tmp[2] << 16 | p_data_tmp[3] << 24;
    uint32_t m_schedule_tick = p_data_tmp[4] | p_data_tmp[5] << 8 | p_data_tmp[6] << 16 | p_data_tmp[7] << 24;

    g_bt_audio_task_env.m_schedule_tick = m_schedule_tick;
    g_bt_audio_task_env.s_capture_tick  = s_capture_tick;
}
#endif

/**
 * @brief       Switch in BT music mode
 * @param[in]   handle - Connection handle
 * @return      None
 */
void tlkmdi_btmusic_switch_in(uint16_t handle)
{
    bt_audio_set_music_vol_percent_by_handle(handle);

    uint8_t type = btp_a2dpsnk_getCurrCodec(handle);
    bt_music_set_codec_type(type);
    //	tlkapi_printf(APP_LOG_EN, "current codec type %d", type);

    bool ret = bt_music_buff_and_alg_init();
    if (!ret) {
        tlkapi_printf(APP_LOG_EN, "bt_music_buff_and_alg_init fail: %d", ret);
        return;
    }

    bt_music_audio_path_init();

    bt_audio_register_get_pcm_data_callback(bt_music_get_playback_data);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_audio_main);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, bt_audio_main_loop);
    bt_audio_task_register_run_cb(NULL, 1);
    tlkmw_audio_btif_inform_host_audio_en(handle, true);
    audio_codec_flag_set(CODEC_FLAG_MUSIC, 1);
#if AUDIO_TWS_MODE
    bt_music_sync_init();
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_SYNC_DATA_TWS_AUD, tlkmdi_mailbox_audio_sync_callback);
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_TWS_SLAVE_AUDIO_SYNC_DATA, tlkmdi_mailbox_tws_slave_audio_sync_callback);
    //tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_TWS_SLAVE_SEND_SYNC_TICK, tlkmdi_mailbox_tws_slave_sync_tick_proc_callback);
    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
        bt_music_calibrate_enable(1);
    } else {
        bt_music_calibrate_enable(0);
    }
#else
    bt_music_calibrate_enable(1);
#endif

#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, ll_mix_fifo0_irq_handler_func);
#endif
}

/**
 * @brief       Switch out BT music mode
 * @param[in]   handle - Connection handle
 * @return      None
 */
void tlkmdi_btmusic_switch_out(uint16_t handle)
{
#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
    ll_mix_bt_music_mic_deinit();
#endif

#if TLKMDI_HRA_MUSIC_MIC_EN
    tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
#endif

    bt_audio_task_register_run_cb(NULL, 0);
    bt_audio_register_get_pcm_data_callback(NULL);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);

    bt_music_audio_path_deinit();

    bool ret = bt_music_buff_and_alg_deinit();
    if (!ret) {
        tlkapi_printf(APP_LOG_EN, "bt_music_buff_and_alg_deinit fail: %d", ret);
        return;
    }

#if AUDIO_TWS_MODE
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_SYNC_DATA_TWS_AUD, NULL);
    tlksys_regHandleSyncDataCb(TLKSYS_REQ_HANDLE_TWS_SLAVE_AUDIO_SYNC_DATA, NULL);
#endif

    tlkmw_audio_btif_inform_host_audio_dis(handle);
}

#endif //#if (TLKBTP_CFG_A2DPSNK_ENABLE)
