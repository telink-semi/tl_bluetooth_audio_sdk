/********************************************************************************************************
 * @file    tlkalg_audio_common.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkalg_audio_common.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"

tlkalg_type_e tlkalg_type = 0;

uint8_t *gtlkalg_ppm0_buff_ptr = NULL;
uint8_t *gtlkalg_ppm1_buff_ptr = NULL;

#if (TLKALG_EQ_ENABLE)
void tlkalg_eq_para_init(uint32_t samplerate, ALG_CHANNEL_IDX chnl, e_eq_type_e eq_type)
{
    tlkapi_printf(APP_LOG_EN, "tlkalg_eq_para_init");
    audio_alg_interface_t *tlkalg_eq_if   = audio_alg_get_interface_by_type(ALG_EQ);
    eq_para_t              tlkalg_eq_para = {
                     .samplerate = samplerate,
                     .channel    = chnl,
                     .eq_type    = eq_type,
        // .width = ALG_WIDTH_16,
    };
    tlkalg_eq_if->audio_alg_param_set(0, (void *)&tlkalg_eq_para);
    tlkalg_eq_if->audio_alg_init(NULL, ALG_CHANNEL_STEREO);
}
#endif

void tlkalg_init_common(tlkalg_type_e alg_type)
{
    (void)alg_type;
#if TLKALG_EQ_ENABLE
    if ((!(tlkalg_type & TLKALG_EQ)) && (alg_type & TLKALG_EQ)) {
        tlkalg_eq_para_init(48000, ALG_CHANNEL_STEREO, EQ_TYPE_MUSIC);
        tlkalg_type |= TLKALG_EQ;
    }
#endif

#if TLKALG_UAC_PPM_SPK_ENABLE
    if ((!(tlkalg_type & TLKALG_PPM_CHN0)) && (alg_type & TLKALG_PPM_CHN0)) {
        if (gtlkalg_ppm0_buff_ptr == NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm0 init");
#if TLKUAC_PPM_SPK_24BIT_ENABLE
            audio_alg_interface_t *tlkalg_ppm0_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
#else
            audio_alg_interface_t *tlkalg_ppm0_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
#endif
            uint16_t ppm_spk_size = tlkalg_ppm0_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            gtlkalg_ppm0_buff_ptr = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
            if (gtlkalg_ppm0_buff_ptr == NULL) {
                tlkapi_printf(APP_LOG_EN, "ppm0 buffer alloc failed");
                return;
            }
            tlkalg_ppm0_if->audio_alg_init(gtlkalg_ppm0_buff_ptr, ALG_CHANNEL_STEREO);
            tlkalg_type |= TLKALG_PPM_CHN0;
        }
    }
#endif

#if TLKALG_UAC_PPM_MIC_ENABLE
    if ((!(tlkalg_type & TLKALG_PPM_CHN1)) && (alg_type & TLKALG_PPM_CHN1)) {
        if (gtlkalg_ppm1_buff_ptr == NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm1 init");
            audio_alg_interface_t *tlkalg_ppm1_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
            uint16_t               ppm_mic_size   = tlkalg_ppm1_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
            gtlkalg_ppm1_buff_ptr                 = (uint8_t *)tlkalg_malloc_func(ppm_mic_size);
            if (gtlkalg_ppm1_buff_ptr == NULL) {
                tlkapi_printf(APP_LOG_EN, "ppm1 buffer alloc failed");
                return;
            }
            tlkalg_ppm1_if->audio_alg_init(gtlkalg_ppm1_buff_ptr, ALG_CHANNEL_LEFT);
            tlkalg_type |= TLKALG_PPM_CHN1;
        }
    }
#endif

#if TLKALG_PPM_USB_CALC_ENABLE
    if ((!(tlkalg_type & TLKALG_PPM_USB_CALC)) && (alg_type & TLKALG_PPM_USB_CALC)) {
        tlkapi_printf(APP_LOG_EN, "ppm usb cacl init");
        tlkalg_ppm_calc_init();
        tlkalg_type |= TLKALG_PPM_USB_CALC;
    }
#endif
}

void tlkalg_deinit_common(tlkalg_type_e alg_type)
{
#if TLKALG_EQ_ENABLE
    if ((tlkalg_type & TLKALG_EQ) && (alg_type & TLKALG_EQ)) {
        audio_alg_interface_t *tlkalg_eq_if = audio_alg_get_interface_by_type(ALG_EQ);
        tlkalg_eq_if->audio_alg_deinit();
    }
#endif

#if TLKALG_UAC_PPM_SPK_ENABLE
    if (((tlkalg_type & TLKALG_PPM_CHN0)) && (alg_type & TLKALG_PPM_CHN0)) {
        if (gtlkalg_ppm0_buff_ptr != NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm0 deinit");
#if TLKUAC_PPM_SPK_24BIT_ENABLE
            audio_alg_interface_t *tlkalg_ppm0_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
#else
            audio_alg_interface_t *tlkalg_ppm0_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
#endif
            tlkalg_ppm0_if->audio_alg_deinit();
            tlkalg_free_func(gtlkalg_ppm0_buff_ptr);
            gtlkalg_ppm0_buff_ptr = NULL;
        }
    }
#endif

#if TLKALG_UAC_PPM_MIC_ENABLE
    if (((tlkalg_type & TLKALG_PPM_CHN1)) && (alg_type & TLKALG_PPM_CHN1)) {
        if (gtlkalg_ppm1_buff_ptr != NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm1 deinit");
            audio_alg_interface_t *tlkalg_ppm1_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
            tlkalg_ppm1_if->audio_alg_deinit();
            tlkalg_free_func(gtlkalg_ppm1_buff_ptr);
            gtlkalg_ppm1_buff_ptr = NULL;
        }
    }
#endif

#if TLKALG_PPM_USB_CALC_ENABLE
    if (((tlkalg_type & TLKALG_PPM_USB_CALC)) && (alg_type & TLKALG_PPM_USB_CALC)) {
        tlkapi_printf(APP_LOG_EN, "tlkalg_ppm_calc_deint");
        tlkalg_ppm_calc_deint();
    }
#endif

    tlkalg_type &= (~alg_type);
}
