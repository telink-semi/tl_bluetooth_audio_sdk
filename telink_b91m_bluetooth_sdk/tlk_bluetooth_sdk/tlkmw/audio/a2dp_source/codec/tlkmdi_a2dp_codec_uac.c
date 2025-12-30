/********************************************************************************************************
 * @file    tlkmdi_a2dp_codec_uac.c
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

#if (BT_A2DP_OUT_INPUT_TYPE == TLKMDI_A2DP_OUT_TYPE_USB_AUDIO)
#if (TLK_USB_UAC_ENABLE)
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"

/**
 * @brief      This function is used to get the sample rate of the audio output.
 * @return     The sample rate of the audio output.
*/
uint32_t tlkmdi_a2dp_codec_hal_get_sampleRate(void)
{
    return tlkusb_uac_get_iso_out_SampleRate();
}

/**
 * @brief      This function is used to get the number of channels of the audio output.
 * @return     The number of channels of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_channels(void)
{
    return tlkusb_uac_get_iso_out_Channels();
}

/**
 * @brief      This function is used to get the bit depth of the audio output.
 * @return     The bit depth of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_bitDepth(void)
{
    return tlkusb_uac_get_iso_out_BitDepth();
}

/**
 * @brief      This function is used to get the number of used samples in the audio output.
 * @return     The number of used samples in the audio output.
*/
uint16_t tlkmdi_a2dp_out_get_used_samples(void)
{
    return tlkusb_uac_get_ppm_out_used_samples();
}

/**
 * @brief      This function is used to read samples from the audio output.
 * @param[out] pData  The buffer to store the samples.
 * @param      dataLen The length of the buffer.
 * @return     The number of read samples.
 */
uint16_t tlkmdi_a2dp_out_read_samples(int16_t *pData, uint16_t dataLen)
{
    return tlkusb_uac_read_ppm_out_samples(pData, dataLen);
}

/**
 * @brief      This function is used to play or stop the audio output.
*/
void tlkmdi_a2dp_out_play_pause(void)
{
    tlklib_uacctrl_play_pause();
}

/**
 * @brief      This function is used to play the next track in the audio output.
*/
void tlkmdi_a2dp_out_play_next(void)
{
    tlklib_uacctrl_play_next();
}

/**
 * @brief      This function is used to play the previous track in the audio output.
*/
void tlkmdi_a2dp_out_play_prev(void)
{
    tlklib_uacctrl_play_prev();
}

/**
 * @brief      This function is used to increase the volume of the audio output.
*/
void tlkmdi_a2dp_out_volume_up(void)
{
    tlkusb_uacctrl_volume_up();
}

/**
 * @brief      This function is used to decrease the volume of the audio output.
*/
void tlkmdi_a2dp_out_volume_down(void)
{
    tlkusb_uacctrl_volume_down();
}

/**
 * @brief      This function is used to adjust the volume of the audio output.
 * @param[in]  pData  The buffer to store the samples.
 * @param[in]  dataLen The length of the buffer.
*/
void tlkmdi_a2dp_out_volume_adjust(int16_t *pData, uint16_t dataLen)
{
    extern uint tlkusb_uacspk_getVolume(void);
    int16_t     audio_volume = tlkusb_uacspk_getVolume();

    for (uint index = 0; index < dataLen; index++) {
        pData[index] = (pData[index] * audio_volume + 0x2000) >> 14;
    }
}

/**
 * @brief      This function is used to handle the interrupt of the audio output.
 * @param[in]  userArg  User argument for the interrupt handler.
*/
void tlkmdi_a2dp_out_user_irq_handler(void *userArg)
{
    uint32_t tick = *(uint32_t *)userArg;
    tlkusb_ppm_process(tick);
}

/**
 * @brief      This function is used to initialize the audio output algorithm.
 * @param[in]  enable  True to enable the algorithm, False to disable the algorithm.
*/
void tlkmdi_a2dp_out_user_alg_init(bool enable)
{
    if (enable) {
        tlkalg_u2s_ppm_clear_status();
        tlkalg_u2s_ppm_set_val(0);
        tlkusb_uac_set_state(TLKUSB_UAC_MUSIC);
    } else {
#if (TLK_ALG_PPM_ENABLE && TLKALG_PPM_SPK_ENABLE)
        tlkusb_uacspk_ppm_stop();
        tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
#endif
    }
}

/**
 * @brief      This function is used to notify the audio output status.
 * @param[in]  enable  True is notify audio start, False is notify audio stop.
*/
void tlkmdi_a2dp_out_user_notify(bool enable)
{
    if (enable) {
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_START_RSP);
        }
    } else {
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_STOP_RSP);
        }
    }
}

/**
 * @brief      This function is used to handle user logic when audio task switch.
 * @param[in]  enable  True is notify audio start, False is notify audio stop.
*/
void tlkmdi_a2dp_out_user_switch(bool enable)
{
    (void)enable;
}

#else

/**
 * @brief      This function is used to get the sample rate of the audio output.
 * @return     The sample rate of the audio output.
*/
uint32_t tlkmdi_a2dp_codec_hal_get_sampleRate(void)
{
    return 0;
}

/**
 * @brief      This function is used to get the number of channels of the audio output.
 * @return     The number of channels of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_channels(void)
{
    return 0;
}

/**
 * @brief      This function is used to get the bit depth of the audio output.
 * @return     The bit depth of the audio output.
*/
uint8_t tlkmdi_a2dp_codec_hal_get_bitDepth(void)
{
    return 0;
}

/**
 * @brief      This function is used to get the number of used samples in the audio output.
 * @return     The number of used samples in the audio output.
*/
uint16_t tlkmdi_a2dp_out_get_used_samples(void)
{
    return 0;
}

/**
 * @brief      This function is used to read samples from the audio output.
 * @param[out] pData  The buffer to store the samples.
 * @param      dataLen The length of the buffer.
 * @return     The number of read samples.
 */
uint16_t tlkmdi_a2dp_out_read_samples(int16_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return 0;
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
    (void)enable;
}

#endif // TLK_USB_UAC_ENABLE

#endif // TLKMDI_A2DP_OUT_INPUT_TYPE == TLKMDI_A2DP_OUT_TYPE_USB_AUDIO
