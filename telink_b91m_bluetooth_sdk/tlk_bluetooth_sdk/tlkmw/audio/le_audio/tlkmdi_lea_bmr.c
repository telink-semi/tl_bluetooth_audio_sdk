/********************************************************************************************************
 * @file    tlkmdi_lea_bmr.c
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
#include "core/mcu_type.h"
#include "tl_common.h"
#include "stack/ble/ble.h"

#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkapi/tlkapi.h"

#include "le_audio_common.h"
#include "le_audio_main.h"
#include "tlkmdi_lea_bmr.h"

#include "codec/lea_codec.h"

#if (TLK_MW_LEA_BMR_ENABLE)

#define TLKMDI_LEA_BMR_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_LEA_BMR << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_LEA_BMR_DBG_SIGN "[BMR]"

struct lea_bmr_audio_state
{
    bool                        busy;
    struct lea_bmr_stream_param stream;
};

static struct lea_bmr_audio_state s_lea_bmr_state = {
    .busy = false,
};

/**
 * @brief       Initialize BMR state and LC3 decoder configuration storage.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_bmr_init(void)
{
    tlkapi_trace(TLKMDI_LEA_BMR_DBG_FLAG, TLKMDI_LEA_BMR_DBG_SIGN, __func__);
    lea_output_config_initial();
    return TLK_ENONE;
}

/**
 * @brief       Indicate whether BMR audio path is already active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_bmr_is_busy(void)
{
    return s_lea_bmr_state.busy;
}

/**
 * @brief       Handle audio open/close requests from the audio manager.
 * @param[in]   handle  - audio handle from manager (unused).
 * @param[in]   status  - TLK_STATE_* command describing transition.
 * @return      true when the transition is accepted, false otherwise.
 */
bool tlkmdi_lea_bmr_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(TLKMDI_LEA_BMR_DBG_FLAG, TLKMDI_LEA_BMR_DBG_SIGN, "%s, status=%d", __func__, status);

    if (status == TLK_STATE_OPENED) {
        if (tlkmdi_lea_bmr_is_busy()) {
            return false;
        }
    } else {
        if (!tlkmdi_lea_bmr_is_busy()) {
            return false;
        }
    }
    tlkapi_trace(TLKMDI_LEA_BMR_DBG_FLAG, TLKMDI_LEA_BMR_DBG_SIGN, "%s:%d, status=%d", __func__, __LINE__, status);
    struct lea_bmr_stream_param *stream = &s_lea_bmr_state.stream;

    if (status == TLK_STATE_OPENED) {
        s_lea_bmr_state.busy = true;
        lea_set_output_all_location(stream->all_location);
        struct lea_codec_config codec_config = {
            .is_output_stream_init = true,
            .output_sample_rate    = stream->frequency,
            .output_location       = stream->all_location,
        };

        lea_codec_stream_init(&codec_config);
        lea_open_output();
        lea_set_output_sample_config_bap(stream->frequency, stream->frame_duration);

        struct lea_config lea = {
            .blocks            = stream->bmr_config[0].blocks,
            .location          = stream->bmr_config[0].location,
            .samplingFrequency = stream->frequency,
            .frameDuration     = stream->frame_duration,
            .frameOctets       = stream->frame_octets,
            .presentationDelay = stream->presentation_delay,
            .iso_handle        = stream->bmr_config[0].iso_handle,
        };

        lea_set_output_config(&lea);

        if (stream->num > 1) {
            lea.blocks     = stream->bmr_config[1].blocks;
            lea.location   = stream->bmr_config[1].location;
            lea.iso_handle = stream->bmr_config[1].iso_handle;
            lea_set_output_config(&lea);
        }
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X)
        timer_set_irq_mask(FLD_TMR0_MODE_IRQ);
#endif

#if (MCU_CORE_TL752X_TEMP)
        plic_interrupt_enable(IRQ_TIMER0_0);
        plic_set_priority(IRQ_TIMER0_0, 1);
#else
        plic_interrupt_enable(IRQ_TIMER0);
        plic_set_priority(IRQ_TIMER0, 1);
#endif

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, le_audio_timer_irq);
    } else {
        lea_close_output();
        lea_codec_stream_deinit();
        lea_release_output_location();

        lea_release_output_config(stream->bmr_config[0].iso_handle);

        if (stream->num > 1) {
            lea_release_output_config(stream->bmr_config[1].iso_handle);
        }

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        if (status == TLK_STATE_CLOSED) {
            s_lea_bmr_state.busy = false;
        }
    }
    return true;
}

/**
 * @brief       Save stream parameters and setup ISO controller data paths.
 * @param[in]   param   - pointer to stream parameters.
 * @return      none.
 */
void tlkmdi_lea_bmr_set_stream_param(struct lea_bmr_stream_param *param)
{
    if (tlkmdi_lea_bmr_is_busy()) {
        return;
    }

    s_lea_bmr_state.stream = *param;

    for (int i = 0; i < param->num; i++) {
        ble_host_gap_iso_set_data_path_c2h(param->bmr_config[i].iso_handle);
    }
}

/**
 * @brief       Tear down ISO controller data paths if stream is active.
 * @return      none.
 */
void tlkmdi_lea_bmr_release_stream_param(void)
{
    if (!tlkmdi_lea_bmr_is_busy()) {
        return;
    }

    for (int i = 0; i < s_lea_bmr_state.stream.num; i++) {
        ble_host_gap_iso_remove_data_path_c2h(s_lea_bmr_state.stream.bmr_config[i].iso_handle);
    }
}

/**
 * @brief       Request audio subsystem to start BMR playback.
 * @return      none.
 */
void tlkmdi_lea_bmr_start_stream(void)
{
    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_LEA_BMR, 0);
}

/**
 * @brief       Request audio subsystem to stop BMR playback.
 * @return      none.
 */
void tlkmdi_lea_bmr_stop_stream(void)
{
    tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_LEA_BMR, 0);
}

#endif
