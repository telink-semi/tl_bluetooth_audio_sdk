/********************************************************************************************************
 * @file    tlkmdi_ll_dongle_music.c
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

#include "ll_dongle_main.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"
#if (TLK_MW_LL_DONGLE_MUSIC_ENABLE)
#include "stack/tpsll/controller/tpd/tpd_host_interface.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
#include "stack/tpsll/controller/tpmd/tpmd_host_interface.h"
#include "stack/tpsll/host/tpsll_hci.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#endif
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"

typedef struct
{
    uint8_t  enable;
    uint8_t  startflag;
    uint16_t handle;
    uint16_t readFrame;
    uint16_t acl_handle;
    bool     micEn;
    bool     spkEn;
    uint32_t tick_rem;
    uint32_t last_tick;
} tlkmdi_ll_dongle_audio_context_t;

static tlkmdi_ll_dongle_audio_context_t s_tlk_mdi_ll_dongle_audio_ctx = {0};

#if TLKALG_LC3_24BIT_ENC_ENABLE && TLKALG_LC3_24BIT_DEC_ENABLE
static uint8_t *s_alg_lc3_24bit_dec_buffer = NULL;
static uint8_t *s_alg_lc3_24bit_enc_buffer = NULL;
#endif

#if TLKALG_ASRC_16TO48_24BIT_ENABLE
static uint8_t *s_alg_asrc_16to48_buff = NULL;
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
static uint8_t *s_alg_lc3_plus_enc_buffer = NULL;
#endif
#if TLKALG_LC3_PLUS_DEC_ENABLE
static uint8_t *s_alg_lc3_plus_dec_buffer = NULL;
#endif

/**
 * @brief     Initializes local audio algorithms based on microphone and speaker enable flags.
 * @param[in] isMicEn  Indicates whether the microphone function is enabled.
 * @param[in] isSpkEn  Indicates whether the speaker function is enabled.
 * @return    Returns true if all required audio algorithm interfaces are successfully initialized; otherwise false.
 */
static bool tlkmdi_ll_dongle_local_init_alg(bool isMicEn, bool isSpkEn)
{
#ifndef PROJ_MESH_AUDIO_DONGLE
    audio_alg_interface_t *p_audio_alg_if;
#endif

    if (isSpkEn) {
#if TLKALG_LC3_24BIT_ENC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
        if (s_alg_lc3_24bit_enc_buffer == NULL) {
            uint16_t lc3_24bit_enc_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_alg_lc3_24bit_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_enc_mem_size);
            if (s_alg_lc3_24bit_enc_buffer == NULL) {
                tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 4bit enc buffer alloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_enc_buffer, ALG_CHANNEL_STEREO);

            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_enc_mem_size: %d", lc3_24bit_enc_mem_size);
        }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
        if (s_alg_lc3_plus_enc_buffer == NULL) {
            if (tlk_tpsll_tpd_host_is_ultra_latency_mode()) {
                p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY, NULL);
            } else {
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
                p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY, NULL);
#else
                p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_NORMAL, NULL);
#endif
            }

            uint16_t lc3_plus_enc_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_alg_lc3_plus_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

            if (s_alg_lc3_plus_enc_buffer == NULL) {
                tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus enc buffer alloc failed");
            } else {
                p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_buffer, ALG_CHANNEL_STEREO);
                tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);
            }
        }
#endif
    } else {
#if TLKALG_LC3_24BIT_ENC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
        if (s_alg_lc3_24bit_enc_buffer != NULL) {
            tlkalg_free_func(s_alg_lc3_24bit_enc_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_lc3_24bit_enc_buffer = NULL;
        }
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
        if (s_alg_lc3_plus_enc_buffer != NULL) {
            tlkalg_free_func(s_alg_lc3_plus_enc_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_lc3_plus_enc_buffer = NULL;
        }
#endif
    }

    if (isMicEn) {
#if TLKALG_LC3_24BIT_DEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
        if (s_alg_lc3_24bit_dec_buffer == NULL) {
            uint16_t lc3_24bit_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_alg_lc3_24bit_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_24bit_dec_mem_size);
            if (s_alg_lc3_24bit_dec_buffer == NULL) {
                tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 4bit dec buffer alloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(s_alg_lc3_24bit_dec_buffer, ALG_CHANNEL_LEFT);

            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_24bit_dec_mem_size: %d", lc3_24bit_dec_mem_size);
        }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
        if (s_alg_lc3_plus_dec_buffer == NULL) {
            if (tlk_tpsll_tpd_host_is_ultra_latency_mode()) {
                p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_ULTRA_LOW_LATENCY, NULL);
            } else {
                p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_NORMAL, NULL);
            }

            uint16_t lc3_plus_dec_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
            s_alg_lc3_plus_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
            if (s_alg_lc3_plus_dec_buffer == NULL) {
                tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec buffer alloc failed");
            }
            p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_buffer, ALG_CHANNEL_LEFT);

            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);
        }
#endif

#if TLKALG_ASRC_16TO48_24BIT_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_24BIT);
        if (tlkusb_uac_get_iso_in_SampleRate() == 48000) {
            if (s_alg_asrc_16to48_buff == NULL) {
                uint16_t asrc_16to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
                tlkapi_printf(APP_AUDIO_LOG_EN, "ALG_ASRC_16TO48_mem_size: %d", asrc_16to48_size);

                s_alg_asrc_16to48_buff = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
                if (s_alg_asrc_16to48_buff == NULL) {
                    tlkapi_printf(APP_AUDIO_LOG_EN, "ASRC 16TO48 buffer malloc failed");
                    return false;
                }

                p_audio_alg_if->audio_alg_init(s_alg_asrc_16to48_buff, ALG_CHANNEL_LEFT);
            }
        }
#endif
    } else {
#if TLKALG_LC3_24BIT_DEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
        if (s_alg_lc3_24bit_dec_buffer != NULL) {
            tlkalg_free_func(s_alg_lc3_24bit_dec_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_lc3_24bit_dec_buffer = NULL;
        }
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
        if (s_alg_lc3_plus_dec_buffer != NULL) {
            tlkalg_free_func(s_alg_lc3_plus_dec_buffer);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_lc3_plus_dec_buffer = NULL;
        }
#endif


#if TLKALG_ASRC_16TO48_24BIT_ENABLE
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_24BIT);
        if (s_alg_asrc_16to48_buff != NULL) {
            tlkalg_free_func(s_alg_asrc_16to48_buff);
            p_audio_alg_if->audio_alg_deinit();
            s_alg_asrc_16to48_buff = NULL;
        }
#endif
    }

    return true;
}

/**
 * @brief     Handles HID command callbacks for music operations.
 * @param[in] handle The connection handle.
 * @param[in] cmd    The HID command to process.
 * @return    None
 */
static void tlkmdi_ll_dongle_get_hid_cmd_cb(uint16_t handle, uint8_t cmd)
{
    switch (cmd) {
    case TLKMDI_TPD_HID_CMD_VOL_UP:
        tlkmdi_ll_dongle_music_operate(handle, TLKAUD_OPCODE_VOLUME_INC, NULL, 0);
        break;
    case TLKMDI_TPD_HID_CMD_VOL_DOWN:
        tlkmdi_ll_dongle_music_operate(handle, TLKAUD_OPCODE_VOLUME_DEC, NULL, 0);
        break;
    case TLKMDI_TPD_HID_CMD_PLAY_PAUSE:
    {
        if (tlkmdi_ll_dongle_music_is_busy()) {
            tlkmdi_ll_dongle_music_close(handle);
        } else {
            tlkmdi_ll_dongle_music_start(handle, 0);
        }
        break;
    }
    case TLKMDI_TPD_HID_CMD_PREV:
        tlkmdi_ll_dongle_music_previous();
        break;
    case TLKMDI_TPD_HID_CMD_NEXT:
        tlkmdi_ll_dongle_music_next();
        break;
    }
}

/**
 * @brief   Retrieves the SCO audio mode for the dongle.
 * @param   None
 * @return  uint8_t - The current SCO audio mode.
 */
uint8_t tlkmdi_ll_dongle_get_sco_audio_mode(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
    uint8_t audio_mode = TPMD_HOST_DG_SCO_AUDIO_MODE_IDLE;
    if (s_tlk_mdi_ll_dongle_audio_ctx.micEn) {
        audio_mode |= TPMD_HOST_DG_SCO_AUDIO_MODE_MIC;

    } else {
        audio_mode &= ~TPMD_HOST_DG_SCO_AUDIO_MODE_MIC;
    }

    if (s_tlk_mdi_ll_dongle_audio_ctx.spkEn) {
        audio_mode |= TPMD_HOST_DG_SCO_AUDIO_MODE_SPEAKER;
    } else {
        audio_mode &= ~TPMD_HOST_DG_SCO_AUDIO_MODE_SPEAKER;
    }
#else
    uint8_t audio_mode = TPD_HOST_DG_SCO_AUDIO_MODE_IDLE;
    if (s_tlk_mdi_ll_dongle_audio_ctx.micEn) {
        audio_mode |= TPD_HOST_DG_SCO_AUDIO_MODE_MIC;

    } else {
        audio_mode &= ~TPD_HOST_DG_SCO_AUDIO_MODE_MIC;
    }

    if (s_tlk_mdi_ll_dongle_audio_ctx.spkEn) {
        audio_mode |= TPD_HOST_DG_SCO_AUDIO_MODE_SPEAKER;
    } else {
        audio_mode &= ~TPD_HOST_DG_SCO_AUDIO_MODE_SPEAKER;
    }
#endif
    tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_ll_dongle_get_sco_audio_mode, mode: %d", audio_mode);
    return audio_mode;
}

/**
 * @brief       Switch between different audio modes.
 * @param       None
 * @return      None
 */
void tlkmdi_ll_dongle_stack_switch(void)
{
    uint8_t audio_mode = tlkmdi_ll_dongle_get_sco_audio_mode();
    if (audio_mode != 0) {
        // DBG_MINGQIAN_CHN10_HIGH;
#if (TLK_STK_TPD_ENABLE)
        tlk_tpsll_tpd_host_switch_dongle_sco_audio_mode(audio_mode);
#endif
        // DBG_MINGQIAN_CHN10_LOW;
    } else {
#if (TLK_STK_TPD_ENABLE)
        tlk_tpsll_tpd_host_exit_dongle_sco();
#endif
    }
}

/**
 * @brief       Initialize the music module.
 * @param       None
 * @return      int     - Returns 0 if initialization is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_init(void)
{
    tmemset(&s_tlk_mdi_ll_dongle_audio_ctx, 0, sizeof(s_tlk_mdi_ll_dongle_audio_ctx));
    tlkmdi_tpd_regGetHidCmdCB(tlkmdi_ll_dongle_get_hid_cmd_cb);

#if (TLK_STK_TPD_ENABLE)
    tlk_tpsll_tpd_host_audio_path_cb_register(ll_dongle_audio_path_callback);
    tlk_tpsll_tpd_controller_traffic_notify_cb_register(ll_dongle_audio_path_traffic_notify);
#endif

#if DONGLE_AUDIO_PATH_GPIO_DEBUG
    gpio_function_en(GPIO_PF1);
    gpio_output_en(GPIO_PF1);

    gpio_function_en(GPIO_PF2);
    gpio_output_en(GPIO_PF2);

    gpio_function_en(GPIO_PF3);
    gpio_output_en(GPIO_PF3);

    gpio_function_en(GPIO_PF0);
    gpio_output_en(GPIO_PF0);
#endif

    return TLK_ENONE;
}
#if TLKALG_PPM_CALC_BY_SAMPLE
static uint8_t *spTlkMdiTPSLLSpkPpmBuff = NULL;
static uint8_t *spTlkMdiTPSLLMicPpmBuff = NULL;
#endif
/*
 * @brief   Initialize the PPM audio algorithm.
 * @param   None
 * @return  None
 */
void ll_dongle_audio_ppm_init(uint8_t spk_chn, uint8_t mic_chn)
{
#if TLKALG_PPM_CALC_BY_SAMPLE
    if (spTlkMdiTPSLLSpkPpmBuff == NULL) {
#if TLKUAC_PPM_SPK_24BIT_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
#else
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
#endif
        uint16_t ppm_spk_size   = p_audio_alg_if->audio_alg_get_size(spk_chn);
        spTlkMdiTPSLLSpkPpmBuff = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
        p_audio_alg_if->audio_alg_init(spTlkMdiTPSLLSpkPpmBuff, spk_chn);
        tlkapi_printf(APP_AUDIO_LOG_EN, "spk ppm need size %x", ppm_spk_size);
    } else {
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLSpkPpmBuff haved malloc");
    }

    if (spTlkMdiTPSLLMicPpmBuff == NULL) {
#if TLKUAC_PPM_MIC_24BIT_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC_24BIT);
#else
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
#endif
        uint16_t ppm_mic_size   = p_audio_alg_if->audio_alg_get_size(mic_chn);
        spTlkMdiTPSLLMicPpmBuff = (uint8_t *)tlkalg_malloc_func(ppm_mic_size);
        if (spTlkMdiTPSLLMicPpmBuff) {
            p_audio_alg_if->audio_alg_init(spTlkMdiTPSLLMicPpmBuff, mic_chn);
            tlkapi_printf(APP_AUDIO_LOG_EN, "mic ppm need size %x", ppm_mic_size);
        } else {
            tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLMicPpmBuff malloc fail");
        }
    } else {
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLMicPpmBuff haved malloc");
    }
#else
    (void)spk_chn;
    (void)mic_chn;
#endif
}

/*
 * @brief   Deinitializes the PPM audio algorithm and frees associated resources.
 * @param   None
 * @return  None
 */
void ll_dongle_audio_ppm_deinit(void)
{
#if TLKALG_PPM_CALC_BY_SAMPLE
    if (spTlkMdiTPSLLSpkPpmBuff) {
#if TLKUAC_PPM_SPK_24BIT_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
#else
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
#endif
        tlkalg_free_func(spTlkMdiTPSLLSpkPpmBuff);
        p_audio_alg_if->audio_alg_deinit();
        spTlkMdiTPSLLSpkPpmBuff = NULL;
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLSpkPpmBuff deinit");
    } else {
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLSpkPpmBuff haved deinit");
    }

    if (spTlkMdiTPSLLMicPpmBuff) {
#if TLKUAC_PPM_MIC_24BIT_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC_24BIT);
#else
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
#endif
        tlkalg_free_func(spTlkMdiTPSLLMicPpmBuff);
        p_audio_alg_if->audio_alg_deinit();
        spTlkMdiTPSLLMicPpmBuff = NULL;
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLMicPpmBuff deinit");
    } else {
        tlkapi_printf(APP_AUDIO_LOG_EN, "spTlkMdiTPSLLMicPpmBuff haved deinit");
    }
#endif
}

/**
 * @brief       Process audio PPM data from source to destination.
 * @param[in]   ps      - Pointer to the source buffer.
 * @param[out]  pd      - Pointer to the destination buffer.
 * @param[in]   len     - Length of the data to process.
 * @param[in]   channel - Audio channel information.
 * @return      uint16_t- Number of processed bytes.
 */
uint16_t ll_dongle_audio_ppm_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel)
{
    uint16_t        ret   = 0;
    static uint32_t count = 0;
    static int16_t  diff  = 0;

    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
    ret                                   = p_audio_alg_if->audio_alg_process(ps, pd, len, 0, channel);

    if (ret != len) {
        count++;
        diff += (len - ret);
        if (!(count % 0x7ff)) {
            diff = 0;
        }
    }
    return ret;
}

/**
 * @brief       Switch the music status for a given handle.
 * @param[in]   handle  - The connection handle.
 * @param[in]   status  - The target status to switch to.
 * @return      bool    - Returns true if the switch is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_switch(uint16_t handle, uint8_t status)
{
    tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_ll_dongle_music_switch, status: %d", status);

    if (status == TLK_STATE_OPENED) {
        // gpio_set_high_level(GPIO_PE4);
        // gpio_set_low_level(GPIO_PE4);
        s_tlk_mdi_ll_dongle_audio_ctx.enable     = true;
        s_tlk_mdi_ll_dongle_audio_ctx.acl_handle = handle;

#if (UDB_DEBUG_ENABLE)
        if (handle == 0xffff) { /*TPSLL UDB Music*/
            s_tlk_mdi_ll_dongle_audio_ctx.spkEn = 1;
            s_tlk_mdi_ll_dongle_audio_ctx.micEn = 0;
        } else if (handle == 0xfffe) { /*TPSLL UDB Voice*/
            s_tlk_mdi_ll_dongle_audio_ctx.spkEn = 1;
            s_tlk_mdi_ll_dongle_audio_ctx.micEn = 1;
        }
#else
        s_tlk_mdi_ll_dongle_audio_ctx.spkEn = tlkusb_uac_get_iso_out_enable();
        s_tlk_mdi_ll_dongle_audio_ctx.micEn = tlkusb_uac_get_iso_in_enable();
#endif
#if TLKLIB_UAC_PPM_SPK_ENABLE || TLKLIB_UAC_PPM_MIC_ENABLE
        tlkalg_ppm_calc_init();
        tlkalg_ppm_setting_init(APP_USB_ISO_OUT_BUFF_IDX_MASK);
#endif
        // DBG_MINGQIAN_CHN8_HIGH;
        // DBG_MINGQIAN_CHN8_LOW;
        ll_dongle_audio_ppm_init(3, 1);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
        u8 audio_mode = tlkmdi_ll_dongle_get_sco_audio_mode();
        tpsll_hci_send_start_dongle_sco_setup(audio_mode);
// tlkmdi_ll_dongle_local_init_alg(s_tlk_mdi_ll_dongle_audio_ctx.micEn, s_tlk_mdi_ll_dongle_audio_ctx.spkEn);
// tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER,ll_dongle_audio_main);
// tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN,tlkmdi_ll_dongle_mainloop);
// tlkdrv_codec_muteSpkBuff();
// ll_dongle_audio_enter_audio_mode();
#else

#if (TLK_STK_TPD_ENABLE)
        tlk_tpsll_tpd_host_start_dongle_sco_setup(tlkmdi_ll_dongle_get_sco_audio_mode());
#endif
        tlkmdi_ll_dongle_local_init_alg(s_tlk_mdi_ll_dongle_audio_ctx.micEn, s_tlk_mdi_ll_dongle_audio_ctx.spkEn);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, ll_dongle_audio_main);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, tlkmdi_ll_dongle_mainloop);
        tlkdrv_codec_muteSpkBuff();
        ll_dongle_audio_enter_audio_mode();
#endif
    } else {
        // gpio_set_high_level(GPIO_PE5);
        // gpio_set_low_level(GPIO_PE5);
        s_tlk_mdi_ll_dongle_audio_ctx.enable = false;
        s_tlk_mdi_ll_dongle_audio_ctx.micEn  = false;
        s_tlk_mdi_ll_dongle_audio_ctx.spkEn  = false;
        tlkmdi_ll_dongle_local_init_alg(s_tlk_mdi_ll_dongle_audio_ctx.micEn, s_tlk_mdi_ll_dongle_audio_ctx.spkEn);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
        // tlk_tpsll_tpmd_host_exit_dongle_sco();
        tpsll_hci_send_exit_dongle_sco();
#else
#if (TLK_STK_TPD_ENABLE)
        tlk_tpsll_tpd_host_exit_dongle_sco();
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        tlkmdi_audio_stop_timer();
        ll_dongle_clear_status();
#endif
        ll_dongle_audio_ppm_deinit();
        // DBG_MINGQIAN_CHN9_HIGH;
        // DBG_MINGQIAN_CHN9_LOW;
    }

    return true;
}

/**
 * @brief       Main loop processing for the music module.
 * @param       None
 * @return      None
 */
void tlkmdi_ll_dongle_mainloop(void) {}

/**
 * @brief       Check if the music module is busy.
 * @param       None
 * @return      bool    - Returns true if the music module is busy, false otherwise.
 */
_attribute_ram_code_sec_ bool tlkmdi_ll_dongle_music_is_busy(void)
{
    return s_tlk_mdi_ll_dongle_audio_ctx.enable;
}

/**
 * @brief       Start the music playback for a given handle with specified parameters.
 * @param[in]   handle  - The connection handle.
 * @param[in]   param   - Additional parameters for music playback.
 * @return      int     - Returns 0 if start is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;
    if (s_tlk_mdi_ll_dongle_audio_ctx.enable) {
        return -TLK_EREPEAT;
    }
    tlklib_uacctrl_play_pause();
    return TLK_ENONE;
}

/**
 * @brief       Close the music playback for a given handle.
 * @param[in]   handle  - The connection handle.
 * @return      int     - Returns 0 if close is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_close(uint16_t handle)
{
    (void)handle;
    if (!s_tlk_mdi_ll_dongle_audio_ctx.enable) {
        return -TLK_EREPEAT;
    }

    /* Handle value is used to represent UDB-music or UDB-voice, todo by mingqian 20250414.*/
    // if (handle != s_tlk_mdi_ll_dongle_audio_ctx.acl_handle) {
    //     return -TLK_EHANDLE;
    // }
    //Close the audio task using usb interrupt state.
    tlklib_uacctrl_play_pause();
    return TLK_ENONE;
}

/**
 * @brief       Skip to the next music track.
 * @param       None
 * @return      bool    - Returns true if skip is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_next(void)
{
    tlklib_uacctrl_play_next();
    return true;
}

/**
 * @brief       Skip to the previous music track.
 * @param       None
 * @return      bool    - Returns true if skip is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_previous(void)
{
    tlklib_uacctrl_play_prev();
    return true;
}

/**
 * @brief       Operate on the music module with an opcode and data.
 * @param[in]   handle  - The connection handle.
 * @param[in]   opcode  - Operation code.
 * @param[in]   pdata   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data.
 * @return      bool    - Returns true if operation is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)handle;
    (void)pdata;
    (void)dataLen;

    switch (opcode) {
    case TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE:
        bool micen = tlkusb_uac_get_iso_in_enable();
        bool spken = tlkusb_uac_get_iso_out_enable();

        if (micen == s_tlk_mdi_ll_dongle_audio_ctx.micEn && spken == s_tlk_mdi_ll_dongle_audio_ctx.spkEn) {
            return false;
        }

        s_tlk_mdi_ll_dongle_audio_ctx.micEn = micen;
        s_tlk_mdi_ll_dongle_audio_ctx.spkEn = spken;

        tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_ll_dongle_music_operate, mic: %d, spk: %d", s_tlk_mdi_ll_dongle_audio_ctx.micEn, s_tlk_mdi_ll_dongle_audio_ctx.spkEn);

        tlkmdi_ll_dongle_local_init_alg(s_tlk_mdi_ll_dongle_audio_ctx.micEn, s_tlk_mdi_ll_dongle_audio_ctx.spkEn);
        tlkmdi_ll_dongle_stack_switch();

        break;
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkusb_uacctrl_volume_up();
    } break;

    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkusb_uacctrl_volume_down();
    } break;

    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
        tlklib_uacctrl_callAccept();
    } break;

    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
        tlklib_uacctrl_callHungUp();
    } break;
    default:
        return false;
    }


    return true;
}

/**
 * @brief       Get the current state of the microphone.
 * @param       None
 * @return      bool    - Returns true if the microphone is active, false otherwise.
 */
bool tlkmdi_ll_dongle_get_mic_state(void)
{
    return s_tlk_mdi_ll_dongle_audio_ctx.micEn;
}

/**
 * @brief       Get the current state of the speaker.
 * @param       None
 * @return      bool    - Returns true if the speaker is active, false otherwise.
 */
bool tlkmdi_ll_dongle_get_spk_state(void)
{
    return s_tlk_mdi_ll_dongle_audio_ctx.spkEn;
}

#endif //TLK_MW_LL_DONGLE_MUSIC_ENABLE
