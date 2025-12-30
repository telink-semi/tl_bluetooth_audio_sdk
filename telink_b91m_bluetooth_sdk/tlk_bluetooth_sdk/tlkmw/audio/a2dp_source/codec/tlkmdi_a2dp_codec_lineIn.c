/********************************************************************************************************
 * @file    tlkmdi_a2dp_codec_lineIn.c
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
#include "tlkmdi_a2dp_codec_hal.h"
#if (BT_A2DP_OUT_INPUT_TYPE == TLKMDI_A2DP_OUT_TYPE_LINEIN)
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"

/**
 * @brief      This function is used to get the sample rate of the audio output.
 * @return     The sample rate of the audio output.
*/
uint32_t tlkmdi_a2dp_codec_hal_get_sampleRate(void)
{
    return 48000;
}

/**
 * @brief      This function is used to get the number of channels of the audio output.
 * @return     The number of channels of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_channels(void)
{
    return 2;
}

/**
 * @brief      This function is used to get the bit depth of the audio output.
 * @return     The bit depth of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_bitDepth(void)
{
    return 16;
}

/**
 * @brief      This function is used to get the number of used samples in the audio output.
 * @return     The number of used samples in the audio output.
*/
uint16_t tlkmdi_a2dp_out_get_used_samples(void)
{
    return tlkdrv_codec_getMicDataLen() / sizeof(codec_int);
}

/**
 * @brief      This function is used to read samples from the audio output.
 * @param[out] pData  The buffer to store the samples.
 * @param      dataLen The length of the buffer.
 * @return     The number of read samples.
 */
uint16_t tlkmdi_a2dp_out_read_samples(int16_t *pData, uint16_t dataLen)
{
    if (tlkdrv_codec_readMicData((uint8_t *)pData, dataLen * 2, 0)) {
        return dataLen;
    } else {
        return 0;
    }
}

/**
 * @brief      This function is used to play or stop the audio output.
*/
void tlkmdi_a2dp_out_play_pause(void) {}

/**
 * @brief      This function is used to play the next track in the audio output.
*/
void tlkmdi_a2dp_out_play_next(void) {}

/**
 * @brief      This function is used to play the previous track in the audio output.
*/
void tlkmdi_a2dp_out_play_prev(void) {}

/**
 * @brief      This function is used to increase the volume of the audio output.
*/
void tlkmdi_a2dp_out_volume_up(void) {}

/**
 * @brief      This function is used to decrease the volume of the audio output.
*/
void tlkmdi_a2dp_out_volume_down(void) {}

/**
 * @brief      This function is used to adjust the volume of the audio output.
 * @param[in]  pData  The buffer to store the samples.
 * @param[in]  dataLen The length of the buffer.
*/
void tlkmdi_a2dp_out_volume_adjust(int16_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
}

/**
 * @brief      This function is used to handle the interrupt of the audio output.
 * @param[in]  userArg  User argument for the interrupt handler.
*/
void tlkmdi_a2dp_out_user_irq_handler(void *userArg)
{
    (void)userArg;
}

/**
 * @brief      This function is used to initialize the audio output algorithm.
 * @param[in]  enable  True to enable the algorithm, False to disable the algorithm.
*/
void tlkmdi_a2dp_out_user_alg_init(bool enable)
{
    (void)enable;
}

/**
 * @brief      This function is used to notify the audio output status.
 * @param[in]  enable  True is notify audio start, False is notify audio stop.
*/
void tlkmdi_a2dp_out_user_notify(bool enable)
{
    (void)enable;
}

/**
 * @brief      This function is used to handle user logic when audio task switch.
 * @param[in]  enable  True is notify audio start, False is notify audio stop.
*/
void tlkmdi_a2dp_out_user_switch(bool enable)
{
    tlk_printf("tlkmdi_a2dp_out_user_switch enable[%d]", enable);

    if (enable) {
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
    } else {
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
    }
}

#endif
