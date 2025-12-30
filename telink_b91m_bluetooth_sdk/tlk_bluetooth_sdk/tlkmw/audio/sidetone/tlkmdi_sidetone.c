/********************************************************************************************************
 * @file    tlkmdi_sidetone.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmdi_sidetone.h"
#if TLKMW_SIDETONE_EN
#define SIDETONE_SAMPLERATE     16000
bool sidetone_busy_flag = false;

/**
 * @brief     Sets the microphone channel for sidetone processing.
 * @param[in] channel The audio channel to set, either left or right.
 * @returns   None
 */
void tlkmdi_sidetone_set_mic_chanl(uint8_t channel)
{
    if (channel == TLKDRV_CODEC_CHANNEL_LEFT) {
        audio_matrix_set_side_tone_route(SIDE_TONE_CHN6, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_LEFT_32_BIT);
        audio_matrix_set_side_tone_route(SIDE_TONE_CHN7, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_LEFT_32_BIT);
    } else if (channel == TLKDRV_CODEC_CHANNEL_RIGHT) {
        audio_matrix_set_side_tone_route(SIDE_TONE_CHN6, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_RIGHT_32_BIT);
        audio_matrix_set_side_tone_route(SIDE_TONE_CHN7, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_RIGHT_32_BIT);        
    } else {
        tlkapi_trace(0xffffffff, "<test>", "error channel para");
    }
}

/**
 * @brief  Initializes the sidetone module.
 * @param  None
 * @return Returns TLK_ENONE on success.
 */
int tlkmdi_sidetone_init(void)
{
    tlkapi_trace(0xffffffff, "<test>", "tlkmdi_sidetone_init");
    tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_RIGHT);

    extern int tlkdrv_tl751x_dac_sidetone_init(int samplerate);
    if (!tlkdrv_tl751x_dac_sidetone_init(SIDETONE_SAMPLERATE)) {
        tlkapi_trace(0xffffffff, "<test>", "sidetone init error");
    }

    return TLK_ENONE;
}

/**
 * @brief     Switches the sidetone state based on the provided status.
 * @param[in] handle The handle associated with the sidetone operation.
 * @param[in] status The desired state of the sidetone (open or closed).
 * @return    Returns true if the operation is successful.
 */
bool tlkmdi_sidetone_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(0xffffffff, "<test>", "tlkmdi_sidetone_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        tlkapi_trace(0xffffffff, "<test>", "open sidetone internal");
        tlkdrv_codec_muteSpkBuff();
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, SIDETONE_SAMPLERATE,0);

        audio_side_tone_dma_en(SIDE_TONE_CHN67);
        audio_side_tone_ch_en(SIDE_TONE_CHN6);
        audio_side_tone_ch_en(SIDE_TONE_CHN7);
        sidetone_busy_flag = true;
    } else {
        tlkapi_trace(0xffffffff, "<test>", "close sidetone internal");
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
        audio_side_tone_ch_dis(SIDE_TONE_CHN6);
        audio_side_tone_ch_dis(SIDE_TONE_CHN7);
        sidetone_busy_flag = false;
    }

    return true;
}

/**
 * @brief  Checks if the sidetone module is busy.
 * @param  None
 * @return Returns true if the sidetone module is busy; otherwise, returns false.
 */
bool tlkmdi_sidetone_is_busy(void)
{
    return sidetone_busy_flag;
}
#endif
