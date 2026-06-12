/********************************************************************************************************
 * @file    lea_codec.c
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
#include "common/types.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"

#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "drivers.h"

#include "lea_codec.h"

#include "stack/ble/ble.h"

#if (TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)

static const int16_t s_audio_zero_buffer[480] = {0}; // support 48kHz 10ms

/**
 * @brief       Get mute buffer pointer.
 * @return      pointer to the mute buffer.
 */
int16_t *lea_codec_get_mute_buffer(void)
{
    return (int16_t *)(size_t)s_audio_zero_buffer;
}

struct lea_codec_state
{
    uint8_t  input_stream_init;
    uint8_t  output_stream_init;
    uint8_t  input_location;       // 0x01 left only, 0x02 right only, 0x03 stereo
    uint8_t  output_location;      // 0x01 left only, 0x02 right only, 0x03 stereo
    uint32_t input_sampling_rate;  // 8kHz 8000, 16kHz 16000, 32kHz 32000, 48kHz 48000
    uint32_t output_sampling_rate; // 8kHz 8000, 16kHz 16000, 32kHz 32000, 48kHz 48000

    uint8_t output_volume;
};

static struct lea_codec_state s_le_codec_state = {
    .input_stream_init  = 0,
    .output_stream_init = 0,
    .output_volume      = 200,
};


#if MCU_CORE_TL752X_TEMP
__attribute__((weak)) void lea_codec_in_output_stream_init(void) {}

__attribute__((weak)) void lea_codec_in_output_stream_deinit(void) {}

__attribute__((weak)) void lea_codec_input_stream_init(void) {}

__attribute__((weak)) void lea_codec_input_stream_deinit(void) {}

__attribute__((weak)) void lea_codec_output_stream_init(void) {}

__attribute__((weak)) void lea_codec_output_stream_deinit(void) {}
#endif

/**
 * @brief       Calculate sample rate value from sample rate code.
 * @param[in]   sample_rate    - sample rate code.
 * @return      sample rate value in Hz.
 */
static uint32_t lea_codec_calc_sample_rate_value(uint8_t sample_rate)
{
    uint32_t sample_rate_codec = 0;
    if (sample_rate == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
        sample_rate_codec = 8000;
    } else if (sample_rate == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
        sample_rate_codec = 16000;
    } else if (sample_rate == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
        sample_rate_codec = 24000;
    } else if (sample_rate == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
        sample_rate_codec = 32000;
    } else if (sample_rate == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
        sample_rate_codec = 48000;
    }

    return sample_rate_codec;
}

/**
 * @brief       VCS event handler callback function.
 * @param[in]   conn_handle    - connection handle.
 * @param[in]   event_id       - VCS event ID.
 * @param[in]   event_data     - pointer to event data.
 * @return      none.
 */
static void lea_codec_vcs_event_handler(uint16_t conn_handle, enum ble_vcss_event_id event_id, void *event_data)
{
    tlkapi_printf(0, "received vcs event, Acl:0x%03x, Event:0x%02x", conn_handle, event_id);

    if (event_id == VCSS_EVT_ID_VOLUME_STATE_CHANGED) {
        struct vcss_volume_state_changed_event *p_event = (struct vcss_volume_state_changed_event *)event_data;
        tlkapi_printf(0, "received vcs event, Volume:0x%02x", p_event->volume_setting);
        lea_codec_set_output_volume(p_event->volume_setting);
    }
}

/**
 * @brief       Set output volume.
 * @param[in]   volume    - volume value to set.
 * @return      none.
 */
void lea_codec_set_output_volume(uint8_t volume)
{
    s_le_codec_state.output_volume = volume;
}

/**
 * @brief       Get output volume.
 * @return      current output volume value.
 */
uint8_t lea_codec_get_output_volume(void)
{
    return s_le_codec_state.output_volume;
}

/**
 * @brief       Initialize LE Audio codec stream.
 * @param[in]   config    - pointer to the codec configuration structure.
 * @return      none.
 */
void lea_codec_stream_init(struct lea_codec_config *config)
{
    ble_lea_register_VCS_event_callback(lea_codec_vcs_event_handler);

    s_le_codec_state.input_stream_init    = config->is_input_stream_init;
    s_le_codec_state.output_stream_init   = config->is_output_stream_init;
    s_le_codec_state.input_sampling_rate  = lea_codec_calc_sample_rate_value(config->input_sample_rate);
    s_le_codec_state.input_location       = config->input_location;
    s_le_codec_state.output_sampling_rate = lea_codec_calc_sample_rate_value(config->output_sample_rate);
    s_le_codec_state.output_location      = config->output_location;

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_OUTPUT_TYPE)
    if (config->is_input_stream_init && config->is_output_stream_init) {
        lea_codec_in_output_stream_init();
        return; // if both input and output are initialized, no need to initialize again.
    }
#endif
#if (LE_AUDIO_CODEC_INPUT_TYPE != LE_AUDIO_CODEC_TYPE_NONE)
    if (config->is_input_stream_init) {
        lea_codec_input_stream_init();
    }
#endif
#if (LE_AUDIO_CODEC_OUTPUT_TYPE != LE_AUDIO_CODEC_TYPE_NONE)
    if (config->is_output_stream_init) {
        lea_codec_output_stream_init();
    }
#endif
}

/**
 * @brief       Deinitialize LE Audio codec stream.
 * @return      none.
 */
void lea_codec_stream_deinit(void)
{
#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_OUTPUT_TYPE)
    if (s_le_codec_state.input_stream_init && s_le_codec_state.output_stream_init) {
        s_le_codec_state.input_stream_init  = 0;
        s_le_codec_state.output_stream_init = 0;
        lea_codec_in_output_stream_deinit();
        return; // if both input and output are initialized, no need to initialize again.
    }
#endif

#if (LE_AUDIO_CODEC_INPUT_TYPE != LE_AUDIO_CODEC_TYPE_NONE)
    if (s_le_codec_state.input_stream_init) {
        s_le_codec_state.input_stream_init = 0;
        lea_codec_input_stream_deinit();
    }
#endif
#if (LE_AUDIO_CODEC_OUTPUT_TYPE != LE_AUDIO_CODEC_TYPE_NONE)
    if (s_le_codec_state.output_stream_init) {
        s_le_codec_state.output_stream_init = 0;
        lea_codec_output_stream_deinit();
    }
#endif
}

/**
 * @brief       Get input sampling rate.
 * @return      input sampling rate in Hz, 0 if input stream is not initialized.
 */
uint32_t lea_codec_get_input_sampling_rate(void)
{
    if (s_le_codec_state.input_stream_init == 0) {
        return 0;
    }
    return s_le_codec_state.input_sampling_rate;
}

/**
 * @brief       Get output sampling rate.
 * @return      output sampling rate in Hz, 0 if output stream is not initialized.
 */
uint32_t lea_codec_get_output_sampling_rate(void)
{
    if (s_le_codec_state.output_stream_init == 0) {
        return 0;
    }
    return s_le_codec_state.output_sampling_rate;
}

/**
 * @brief       Get input location.
 * @return      input location value, 0 if input stream is not initialized.
 */
uint32_t lea_codec_get_input_location(void)
{
    if (s_le_codec_state.input_stream_init == 0) {
        return 0;
    }
    return s_le_codec_state.input_location;
}

/**
 * @brief       Get output location.
 * @return      output location value, 0 if output stream is not initialized.
 */
uint32_t lea_codec_get_output_location(void)
{
    if (s_le_codec_state.output_stream_init == 0) {
        return 0;
    }
    return s_le_codec_state.output_location;
}

__attribute__((weak)) void lea_codec_output_set_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    (void)left_data;
    (void)right_data;
    (void)sample_num;
}

__attribute__((weak)) bool lea_codec_input_get_audio_data(adc_mono_int *left_data, adc_mono_int *right_data, uint16_t sample_num)
{
    (void)left_data;
    (void)right_data;
    (void)sample_num;
    return false;
}

__attribute__((weak)) void lea_codec_input_clean_buffer(void) {}

#endif //#if (TLK_MW_LE_AUDIO_ENABLE)
