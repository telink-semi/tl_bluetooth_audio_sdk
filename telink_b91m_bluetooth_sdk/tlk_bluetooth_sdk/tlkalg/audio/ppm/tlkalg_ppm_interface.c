/********************************************************************************************************
 * @file    tlkalg_ppm_interface.c
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

#include "tlkalg/audio/ppm/tlkalg_ppm_interface.h"
#include "tlkapi/tlkapi.h"

#if (TLKALG_PPM_SPK_ENABLE | TLKALG_PPM_MIC_ENABLE)
uint8_t *g_tlkalg_ppm_spk_buff = NULL;
uint8_t *g_tlkalg_ppm_mic_buff = NULL;

uint8_t g_tlkalg_ppm_spk_chn = 0;
uint8_t g_tlkalg_ppm_mic_chn = 0;

/**
 * @brief       Get the size required for the speaker PPM buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_ppm_asrc_16_bit_get_size();
    size     = (size + 3) / 4 * 4;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk size %d", size);
    return size;
}

/**
 * @brief       Set the PPM parameter for the speaker
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val = (int *)param;

    int tlkalg_ppm_spk_value = *val;
    if (g_tlkalg_ppm_spk_buff != NULL) {
        tlka_ppm_asrc_16_bit_init((void *)g_tlkalg_ppm_spk_buff, g_tlkalg_ppm_spk_chn, tlkalg_ppm_spk_value);
    }
    
    //    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk val %d", tlkalg_ppm_spk_value);
    return 0;
}

/**
 * @brief       Initialize the speaker PPM module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_init(uint8_t *p_buff, uint8_t channel)
{
    if (p_buff == NULL) {
        return 0;
    }

    TLKA_PPM_ASRC_CHANNEL chn;
    if (channel == 1 || channel == 2) {
        chn = TLKA_PPM_ASRC_SINGLE;
    } else if (channel == 3) {
        chn = TLKA_PPM_ASRC_STEREO;
    } else {
        return 0;
    }
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk init");
    g_tlkalg_ppm_spk_buff = p_buff;
    g_tlkalg_ppm_spk_chn  = chn;

    tlka_ppm_asrc_16_bit_init((void *)g_tlkalg_ppm_spk_buff, g_tlkalg_ppm_spk_chn, 0);

    return 0;
}

/**
 * @brief       Deinitialize the speaker PPM module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_deinit(void)
{
    g_tlkalg_ppm_spk_buff = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk deinit");
    return 0;
}

/**
 * @brief       Process the speaker PPM data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ps pd error");
        return 0;
    }
    if (g_tlkalg_ppm_spk_buff == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "g_tlkalg_ppm_spk_buff error");
        return 0;
    }
    #if 1
    short *psrc    = (short *)ps;
    short *pdes    = (short *)pd;
    int    out_num = 0;
    while (len) {
        uint16_t ni = len > PPM_DATA_LEN_MAX ? PPM_DATA_LEN_MAX : len;

        len -= ni;
        int out_len = tlka_ppm_asrc_16_bit_frame_process((void *)g_tlkalg_ppm_spk_buff, psrc, ni, pdes);
        psrc += ni * 2;      //stereo
        pdes += out_len * 2; //stereo
        out_num += out_len;
    }

    return out_num;
    #else
    int out_num = tlka_ppm_asrc_16_bit_frame_process((void *)g_tlkalg_ppm_spk_buff, (short *)ps, len, (short *)pd);
    return out_num;
    #endif
}

/**
 * @brief       Get the size required for the microphone PPM buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_mic_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_ppm_asrc_16_bit_get_size();
    size     = (size + 3) / 4 * 4;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic size %d", size);
    return size;
}

/**
 * @brief       Set the PPM parameter for the microphone
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_mic_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val                  = (int *)param;
    int  tlkalg_ppm_mic_value = *val;
    tlka_ppm_asrc_16_bit_init((void *)g_tlkalg_ppm_mic_buff, g_tlkalg_ppm_mic_chn, tlkalg_ppm_mic_value);

    //    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic val %d", tlkalg_ppm_mic_value);
    return 0;
}

/**
 * @brief       Initialize the microphone PPM module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_init(uint8_t *p_buff, uint8_t channel)
{
    if (p_buff == NULL) {
        return 0;
    }

    TLKA_PPM_ASRC_CHANNEL chn;
    if (channel == 1 || channel == 2) {
        chn = TLKA_PPM_ASRC_SINGLE;
    } else if (channel == 3) {
        chn = TLKA_PPM_ASRC_STEREO;
    } else {
        return 0;
    }

    g_tlkalg_ppm_mic_buff = p_buff;
    g_tlkalg_ppm_mic_chn  = chn;

    tlka_ppm_asrc_16_bit_init((void *)g_tlkalg_ppm_mic_buff, g_tlkalg_ppm_mic_chn, 0);

    return 0;
}

/**
 * @brief       Deinitialize the microphone PPM module
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_deinit(void)
{
    g_tlkalg_ppm_mic_buff = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic deinit");
    return 0;
}

/**
 * @brief       Process the microphone PPM data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_mic_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if (ps == NULL || pd == NULL) {
        return 0;
    }
    if (g_tlkalg_ppm_mic_buff == NULL) {
        return 0;
    }

    return tlka_ppm_asrc_16_bit_frame_process((void *)g_tlkalg_ppm_mic_buff, (short *)ps, len, (short *)pd);
}

#endif