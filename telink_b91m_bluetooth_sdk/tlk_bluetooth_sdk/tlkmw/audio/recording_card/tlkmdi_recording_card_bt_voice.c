/********************************************************************************************************
 * @file    tlkmdi_recording_card_bt_voice.c
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
#include "tlkmw/tlkmw.h"

#if (TLKMW_RECORDING_CARD_EN && TLKBTP_CFG_HFPAG_ENABLE)

adc_int g_record_mic_buff[120];

static signed short s_sin_16k_stereo[32] = {
    0, 0, 12539,  12539,  23169,  23169,  30272,  30272,  32767,  32767,  30272,  30272,  23169,  23169,  12539,  12539,
    4, 4, -12539, -12539, -23169, -23169, -30272, -30272, -32767, -32767, -30272, -30272, -23169, -23169, -12539, -12539,
}; //16K

static int s_record_count = 0;

bool tlkmdi_record_mix_mic_and_spk_data(int16_t *pMic, int16_t *pSpk, int16_t *pMix, uint16_t samples)
{
    if (pMix == NULL || samples == 0) {
        return false;
    }

    if (pMic == NULL && pSpk == NULL) {
        tmemset(pMix, 0, samples * sizeof(int16_t));
    } else if (pMic == NULL) {
        tmemcpy(pMix, pSpk, samples * sizeof(int16_t));
    } else if (pSpk == NULL) {
        tmemcpy(pMix, pMic, samples * sizeof(int16_t));
    } else {
        for (int i = 0; i < samples; i++) {
            int32_t mix_val = (int32_t)pMic[i] + (int32_t)pSpk[i];
            pMix[i]         = (int16_t)(mix_val / 2);
        }
    }

    return true;
}

bool tlkmdi_record_get_mic_data_from_uac(int16_t *pData, uint16_t samples)
{
    int16_t *p_mic = (int16_t *)g_record_mic_buff;

    for (int i = 0; i < samples; i++) {
        pData[i] = p_mic[i];
    }

    return true;
}

bool tlkmdi_record_fill_mic_data_from_uac(uint16_t samples)
{
    //16k sin now, replace with get data from uac rx buff.
    (void)samples;
    (void)s_sin_16k_stereo;
    (void)s_record_count;
    // int16_t *p_mic = (int16_t *)g_record_mic_buff;
    // for (int i = 0; i < samples; i++) {
    //     p_mic[i] = s_sin_16k_stereo[s_record_count++];
    //     s_record_count %= 32;
    // }

    return true;
}

bool tlkmdi_record_fill_spk_data_to_uac(int16_t *pData, uint16_t samples)
{
    (void)pData;
    (void)samples;

    if (samples > 120 * 2) {
        samples = 120 * 2;
    }

    // int16_t *p_mic = (int16_t *)g_record_mic_buff;
    // for (int i = 0; i < samples; i++) {
    //     p_mic[i] = pData[i];
    // }

    // int16_t *p_mic = (int16_t *)g_record_mic_buff;
    // for (int i = 0; i < samples; i++) {
    //     p_mic[i] = s_sin_16k_stereo[s_record_count++];
    //     s_record_count %= 32;
    // }

    adc_int  p_temp[120];
    int16_t *p_mic = (int16_t *)p_temp;
    for (int i = 0; i < samples; i++) {
        p_mic[i] = s_sin_16k_stereo[s_record_count++];
        s_record_count %= 32;
    }

    tlkmdi_record_mix_mic_and_spk_data(p_mic, pData, (int16_t *)g_record_mic_buff, samples);

    return true;
}


#endif
