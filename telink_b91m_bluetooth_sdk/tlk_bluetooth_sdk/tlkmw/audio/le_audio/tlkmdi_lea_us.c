/********************************************************************************************************
 * @file    tlkmdi_lea_us.c
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
#include "tlkmw/audio/tlkmw_audio.h"

#include "tlkapi/tlkapi.h"

#include "drivers.h"

#include "le_audio_common.h"
#include "le_audio_main.h"
#include "codec/lea_codec.h"
#include "tlkmdi_lea_us.h"

#include "stack/ble/ble.h"
#if (TLK_CFG_TPSLL_HCI_ENABLE)
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/controller/tpdt/tpdt_host_interface.h"
#endif

#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif

// tlkdrv_codec_get_sin_data(pcm, samples); test sin data.
#if (TLK_MW_LE_AUDIO_ENABLE && TLK_MW_LEA_US_ENABLE)
#define TLKMDI_LEA_US_DBG_FLAG     ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_LEA_US << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_LEA_US_DBG_SIGN     "[US]"

#define LEA_US_INVALID_CONN_HANDLE 0xFFFF

#if TLKAUD_LEA_PLAY_FLOW_CTRL_EN
extern uint32_t lea_tmr_ideal_tick;
extern bool     render_sync_flag;
#endif

enum
{
    LEA_US_ACTIVE_STATE_IDLE,
    LEA_US_ACTIVE_STATE_MUSIC,
    LEA_US_ACTIVE_STATE_VOICE,
};

struct lea_us_audio_common_parameter
{
    uint16_t active_state; // music or voice
    uint16_t active_conn_handle;
    uint32_t cig_start_time;
};

static struct lea_us_audio_state
{
    uint16_t conn_handle;

#if (TLK_MW_LEA_US_VOICE_ENABLE)
    // le audio input enable/disable, frequency, location.
    uint8_t           input_enable;
    uint8_t           input_frequency;
    uint32_t          input_location;
    uint8_t           input_audio_index;
    struct lea_config input_audio_init[2];
#endif

    // le audio output enable/disable, frequency, location.
    uint8_t           output_enable;
    uint8_t           output_frequency;
    uint32_t          output_location;
    uint8_t           output_audio_index;
    struct lea_config output_audio_init[2];
} s_lea_us_audio_state;

static struct lea_us_audio_common_parameter s_lea_us_audio_param;

#if TLKADU_MIDBUF_ENABLE
extern void tlkaud_mainloop_thread(void);
extern void tlkaud_timer_thread(void);
extern void tlkaud_high_priority_thread(void);
extern void tlkaud_fifo_irq_handler(void);
extern void tlkaud_fifo_config(uint16_t byte_num);
extern void tlkaud_fifo_irq_disable(void);
extern void tlkaud_codec_task_init(void);
#endif

static void tlkmdi_lea_us_init_audio_state(void)
{
    s_lea_us_audio_param.active_state       = LEA_US_ACTIVE_STATE_IDLE;
    s_lea_us_audio_param.active_conn_handle = LEA_US_INVALID_CONN_HANDLE;

    memset(&s_lea_us_audio_state, 0, sizeof(struct lea_us_audio_state));
}

static struct lea_us_audio_state *tlkmdi_lea_us_get_audio_state(uint16_t conn_handle)
{
    (void)conn_handle;
    return &s_lea_us_audio_state;
}

static struct lea_us_audio_state *tlkmdi_lea_us_create_audio_state_by_conn_handle(uint16_t conn_handle)
{
    (void)conn_handle;
    return &s_lea_us_audio_state;
}

static void tlkmdi_lea_us_set_audio_state(uint16_t conn_handle, const struct ble_bap_us_event_codec_state *p_config_codec)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_create_audio_state_by_conn_handle(conn_handle);
    p_lea_audio_state->conn_handle               = conn_handle;

#if (TLK_MW_LEA_US_VOICE_ENABLE)
    p_lea_audio_state->input_enable    = p_config_codec->input_enable;
    p_lea_audio_state->input_frequency = p_config_codec->input_frequency;
    p_lea_audio_state->input_location  = p_config_codec->input_location;
#endif
    p_lea_audio_state->output_enable    = p_config_codec->output_enable;
    p_lea_audio_state->output_frequency = p_config_codec->output_frequency;
    p_lea_audio_state->output_location  = p_config_codec->output_location;
}

static void tlkmdi_lea_us_clear_audio_state(uint16_t conn_handle)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);
    memset(p_lea_audio_state, 0, sizeof(struct lea_us_audio_state));
}

#if (TLK_MW_LEA_US_VOICE_ENABLE)
static struct lea_config *tlkmdi_lea_us_set_input_codec_config(uint16_t conn_handle, const struct ble_bap_us_event_open_codec *p_config_codec)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);

    if (p_lea_audio_state->input_audio_index >= 2) {
        return NULL;
    }
    struct lea_config *p_lea_audio_init = &p_lea_audio_state->input_audio_init[p_lea_audio_state->input_audio_index];

    p_lea_audio_state->input_audio_index++;
    p_lea_audio_init->blocks            = p_config_codec->blocks;
    p_lea_audio_init->samplingFrequency = p_config_codec->sampling_frequency;
    p_lea_audio_init->frameDuration     = p_config_codec->frame_duration;
    p_lea_audio_init->frameOctets       = p_config_codec->frame_octets;
    p_lea_audio_init->location          = p_config_codec->location;
    p_lea_audio_init->iso_handle        = p_config_codec->cis_handle;
    p_lea_audio_init->presentationDelay = p_config_codec->presentation_delay;

    return p_lea_audio_init;
}

static void tlkmdi_lea_us_release_input_codec_config(uint16_t conn_handle, uint16_t iso_handle)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);

    if (p_lea_audio_state->input_audio_index == 1) {
        p_lea_audio_state->input_audio_index = 0;
    } else if (p_lea_audio_state->input_audio_index == 2) {
        p_lea_audio_state->input_audio_index = 1;
        if (p_lea_audio_state->input_audio_init[0].iso_handle == iso_handle) {
            memcpy(&p_lea_audio_state->input_audio_init[0], &p_lea_audio_state->input_audio_init[1], sizeof(struct lea_config));
        }
    }
}

#endif

static struct lea_config *tlkmdi_lea_us_set_output_codec_config(uint16_t conn_handle, const struct ble_bap_us_event_open_codec *p_config_codec)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);

    if (p_lea_audio_state->output_audio_index >= 2) {
        return NULL;
    }
    struct lea_config *p_lea_audio_init = &p_lea_audio_state->output_audio_init[p_lea_audio_state->output_audio_index];

    p_lea_audio_state->output_audio_index++;
    p_lea_audio_init->blocks            = p_config_codec->blocks;
    p_lea_audio_init->samplingFrequency = p_config_codec->sampling_frequency;
    p_lea_audio_init->frameDuration     = p_config_codec->frame_duration;
    p_lea_audio_init->frameOctets       = p_config_codec->frame_octets;
    p_lea_audio_init->location          = p_config_codec->location;
    p_lea_audio_init->iso_handle        = p_config_codec->cis_handle;
    p_lea_audio_init->presentationDelay = p_config_codec->presentation_delay;

    return p_lea_audio_init;
}

static void tlkmdi_lea_us_release_output_codec_config(uint16_t conn_handle, uint16_t iso_handle)
{
    struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);

    if (p_lea_audio_state->output_audio_index == 1) {
        p_lea_audio_state->output_audio_index = 0;
    } else if (p_lea_audio_state->output_audio_index == 2) {
        p_lea_audio_state->output_audio_index = 1;
        if (p_lea_audio_state->output_audio_init[0].iso_handle == iso_handle) {
            memcpy(&p_lea_audio_state->output_audio_init[0], &p_lea_audio_state->output_audio_init[1], sizeof(struct lea_config));
        }
    }
}

/**
 * @brief       Get the BLE connection handle currently driving playback.
 * @return      Active connection handle or 0xFFFF if idle.
 */
uint16_t tlkmdi_lea_us_get_active_conn_handle(void)
{
    return s_lea_us_audio_param.active_conn_handle;
}

/**
 * @brief       Check if the given connection handle is the active connection.
 * @param[in]   conn_handle - connection handle to check.
 * @return      true if active, false otherwise.
 */
static bool tlkmdi_lea_us_check_active_conn_handle(uint16_t conn_handle)
{
    return s_lea_us_audio_param.active_conn_handle == conn_handle;
}

/**
 * @brief       Set the active connection handle.
 * @param[in]   conn_handle - connection handle to set.
 * @return      none.
 */
static void tlkmdi_lea_us_set_active_conn_handle(uint16_t conn_handle)
{
    s_lea_us_audio_param.active_conn_handle = conn_handle;
}

/**
 * @brief       Clear the active connection handle.
 * @return      none.
 */
static void tlkmdi_lea_us_clear_active_conn_handle(void)
{
    s_lea_us_audio_param.active_state       = LEA_US_ACTIVE_STATE_IDLE;
    s_lea_us_audio_param.active_conn_handle = LEA_US_INVALID_CONN_HANDLE;
}

/**
 * @brief       Handle BAP unicast event callback.
 * @param[in]   conn_handle - connection handle.
 * @param[in]   event - event ID.
 * @param[in]   event_msg - event message.
 * @return      none.
 */
static void tlkmdi_bap_uc_event_callback(uint16_t conn_handle, enum ble_bap_us_event_id event, const void *event_msg)
{
    switch (event) {
    case BAP_US_EVENT_ID_CODEC_STATE:
    {
        tlkmdi_lea_us_set_audio_state(conn_handle, event_msg);
#if (TLK_MW_LEA_US_VOICE_ENABLE)
        const struct ble_bap_us_event_codec_state *p_event = event_msg;
        if (p_event->input_enable) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_LEA_US_VOICE, conn_handle);
        } else
#endif
        {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_LEA_US_MUSIC, conn_handle);
        }
    } break;
    case BAP_US_EVENT_ID_OPEN_OUTPUT:
    {
        struct lea_config *p_output_audio = tlkmdi_lea_us_set_output_codec_config(conn_handle, event_msg);
        if (tlkmdi_lea_us_check_active_conn_handle(conn_handle)) {
            lea_open_output();
            lea_set_output_config(p_output_audio);
            lea_set_output_sample_config_bap(p_output_audio->samplingFrequency, p_output_audio->frameDuration);
        }
    } break;
    case BAP_US_EVENT_ID_CLOSE_OUTPUT:
    {
        const struct ble_bap_us_event_close_codec *p_event    = event_msg;
        uint16_t                                   iso_handle = p_event->cis_handle;

        tlkmdi_lea_us_release_output_codec_config(conn_handle, iso_handle);
        if (tlkmdi_lea_us_check_active_conn_handle(conn_handle)) {
            lea_release_output_config(iso_handle);
        }
    } break;
    case BAP_US_EVENT_ID_DEINIT_CODEC_STATE:
    {
        if (tlkmdi_lea_us_check_active_conn_handle(conn_handle)) {
            lea_close_output();
            if (tone_is_playing()) {
                tlkmdi_audio_stop_timer();
                tlkmdi_audio_task_set_next_irq(100);
                tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, lea_us_tone_continue);
            }
        }
#if (TLK_MW_LEA_US_VOICE_ENABLE)
        struct lea_us_audio_state *p_lea_audio_state = tlkmdi_lea_us_get_audio_state(conn_handle);
        if (p_lea_audio_state->input_enable) {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_LEA_US_VOICE, conn_handle);
        } else
#endif
        {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_LEA_US_MUSIC, conn_handle); // must do it, it remove the task from audio task list.
        }
        if (!tlkmdi_lea_us_check_active_conn_handle(conn_handle)) { // if this audio task is not active, clear the audio state.
            tlkmdi_lea_us_clear_audio_state(conn_handle);
        }
    } break;
#if (TLK_MW_LEA_US_VOICE_ENABLE)
    case BAP_US_EVENT_ID_OPEN_INPUT:
    {
        struct lea_config *p_input_param = tlkmdi_lea_us_set_input_codec_config(conn_handle, event_msg);
        if (tlkmdi_lea_us_check_active_conn_handle(conn_handle)) {
            // lea_open_input(s_lea_us_audio_param.cig_start_time);
            lea_open_input(clock_time());
            lea_set_input_config(p_input_param);
            lea_set_input_sample_config_bap(p_input_param->samplingFrequency, p_input_param->frameDuration);
        }
    } break;
    case BAP_US_EVENT_ID_CLOSE_INPUT:
    {
        const struct ble_bap_us_event_close_codec *p_event    = event_msg;
        uint16_t                                   iso_handle = p_event->cis_handle;
        tlkmdi_lea_us_release_input_codec_config(conn_handle, iso_handle);
        if (tlkmdi_lea_us_check_active_conn_handle(conn_handle)) {
            lea_close_input();
            lea_release_input_config(iso_handle);
        }
    } break;
#endif
    default:
    {
        // do nothing
    } break;
    }
}

/**
 * @brief       Initialize LE Audio unicast server states and callbacks.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_us_init(void)
{
    tlkapi_trace(TLKMDI_LEA_US_DBG_FLAG, TLKMDI_LEA_US_DBG_SIGN, __func__);
#if (TLK_MW_LEA_US_VOICE_ENABLE)
    lea_input_config_initial();
#endif
    lea_output_config_initial();
    tlkmdi_lea_us_init_audio_state();

    ble_lea_initial_bap_unicast_server_role(tlkmdi_bap_uc_event_callback);

    return TLK_ENONE;
}

/**
 * @brief       Issue music play command to remote server.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   param   - optional (unused).
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_us_music_start(uint16_t handle, uint32_t param)
{
    (void)param;
    tlkapi_trace(TLKMDI_LEA_US_DBG_FLAG, TLKMDI_LEA_US_DBG_SIGN, __func__);
    ble_gmcsc_write_play(handle);
    return TLK_ENONE;
}

/**
 * @brief       Issue music pause command to remote server.
 * @param[in]   handle  - BLE ACL handle.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_us_music_close(uint16_t handle)
{
    tlkapi_trace(TLKMDI_LEA_US_DBG_FLAG, TLKMDI_LEA_US_DBG_SIGN, __func__);
    ble_gmcsc_write_pause(handle);
    return TLK_ENONE;
}

/**
 * @brief       Send next-track command through GMCS.
 * @return      true on success.
 */
bool tlkmdi_lea_us_music_next(void)
{
    ble_gmcsc_write_next_track(tlkmdi_lea_us_get_active_conn_handle());
    return true;
}

/**
 * @brief       Send previous-track command through GMCS.
 * @return      true on success.
 */
bool tlkmdi_lea_us_music_previous(void)
{
    ble_gmcsc_write_previous_track(tlkmdi_lea_us_get_active_conn_handle());
    return true;
}

static bool tlkmdi_lea_us_operate(uint8_t opcode, uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    bool ret = true;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        ble_vcss_update_volume_setting_change(tlkmdi_lea_us_get_active_conn_handle(), 20);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        ble_vcss_update_volume_setting_change(tlkmdi_lea_us_get_active_conn_handle(), -20);
    } break;
    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
        uint16_t handle = tlkmdi_lea_us_get_active_conn_handle();
        if (ble_gtbsc_check_incoming_state(handle)) {
            ble_gtbsc_accept_incoming_call(handle);
        }
    } break;
    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
        uint16_t handle = tlkmdi_lea_us_get_active_conn_handle();
        if (ble_gtbsc_check_telephone_state(handle)) {
            ble_gtbsc_terminate_new_call(handle);
        }
    } break;
    case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
    {
        return true;
    }
    default:
    {
        ret = false;
    } break;
    }
    return ret;
}

static bool tlkmdi_lea_us_switch(uint16_t handle, uint8_t status)
{
    if (status == TLK_STATE_OPENED) {
        tlkmdi_lea_us_set_active_conn_handle(handle);
        struct lea_us_audio_state *p_lea_us_audio_state = tlkmdi_lea_us_get_audio_state(handle);

#if (TLK_MW_LEA_US_VOICE_ENABLE)
        if (p_lea_us_audio_state->input_enable == 1) {
            lea_set_input_all_location(p_lea_us_audio_state->input_location);
        }
#endif

        if (p_lea_us_audio_state->output_enable == 1) {
            lea_set_output_all_location(p_lea_us_audio_state->output_location);
        }

        struct lea_codec_config codec_config = {
#if (TLK_MW_LEA_US_VOICE_ENABLE)
            .is_input_stream_init = (p_lea_us_audio_state->input_enable == 1),
            .input_sample_rate    = p_lea_us_audio_state->input_frequency,
            .input_location       = p_lea_us_audio_state->input_location,
#endif
            .is_output_stream_init = (p_lea_us_audio_state->output_enable == 1),
            .output_sample_rate    = p_lea_us_audio_state->output_frequency,
            .output_location       = p_lea_us_audio_state->output_location,
        };
        lea_codec_stream_init(&codec_config);

#if (TLK_MW_LEA_US_VOICE_ENABLE)
        if (p_lea_us_audio_state->input_audio_index > 0) {
            const struct lea_config *input_audio_init = &p_lea_us_audio_state->input_audio_init[0];
            // lea_open_input(s_lea_us_audio_param.cig_start_time);
            lea_open_input(clock_time());
            lea_set_input_config(input_audio_init);
            lea_set_input_sample_config_bap(input_audio_init->samplingFrequency, input_audio_init->frameDuration);
            if (p_lea_us_audio_state->input_audio_index > 1) {
                lea_set_input_config(&p_lea_us_audio_state->input_audio_init[1]);
            }
        }
#endif

        if (p_lea_us_audio_state->output_audio_index > 0) {
            const struct lea_config *output_audio_init = &p_lea_us_audio_state->output_audio_init[0];
            lea_open_output();
            lea_set_output_config(output_audio_init);
            lea_set_output_sample_config_bap(output_audio_init->samplingFrequency, output_audio_init->frameDuration);
            if (p_lea_us_audio_state->output_audio_index > 1) {
                lea_set_output_config(&p_lea_us_audio_state->output_audio_init[1]);
            }
        }

        /* can not delete this code because other project is different with this project in async init */
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
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, le_audio_timer_irq);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, le_audio_main_loop);
#if TLKAUD_LEA_PLAY_FLOW_CTRL_EN
        lea_tmr_ideal_tick += clock_time() + LEA_TRANS_PLAY_DATA_INTVAL * SYSTEM_TIMER_TICK_1US;
        tlkmdi_audio_task_set_next_irq(LEA_TRANS_PLAY_DATA_INTVAL);
        tlkmdi_midbuf_sync_spk(TLKADU_MIDBUF_SPK_LEN / 2);
        tlkaud_midbuf_mute_spk();
#endif
    } else {
        struct lea_us_audio_state *p_lea_us_audio_state = tlkmdi_lea_us_get_audio_state(handle);

#if (TLK_MW_LEA_US_VOICE_ENABLE)
        if (p_lea_us_audio_state->input_enable == 1) {
            lea_release_input_location();
        }
#endif
        if (p_lea_us_audio_state->output_enable == 1) {
            lea_release_output_location();
        }

        lea_codec_stream_deinit();
#if (TLK_MW_LEA_US_VOICE_ENABLE)
        if (p_lea_us_audio_state->input_audio_index > 0) {
            const struct lea_config *input_audio_init = &p_lea_us_audio_state->input_audio_init[0];
            lea_close_input();
            lea_release_input_config(input_audio_init->iso_handle);
            if (p_lea_us_audio_state->input_audio_index > 1) {
                lea_release_input_config(p_lea_us_audio_state->input_audio_init[1].iso_handle);
            }
        }
#endif
        if (p_lea_us_audio_state->output_audio_index > 0) {
            const struct lea_config *output_audio_init = &p_lea_us_audio_state->output_audio_init[0];
            lea_release_output_config(output_audio_init->iso_handle);
            if (p_lea_us_audio_state->output_audio_index > 1) {
                lea_release_output_config(p_lea_us_audio_state->output_audio_init[1].iso_handle);
            }
        }

#if TLKADU_MIDBUF_ENABLE
        tlkaud_codec_task_deinit();
        tlkaud_codec_task_clear_mode(PLAYER_MODE_LEA);
#endif
#if TLKAUD_LEA_PLAY_FLOW_CTRL_EN
        render_sync_flag = false;
        tlkmdi_audio_stop_timer();
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        if (status == TLK_STATE_CLOSED) {
            tlkmdi_lea_us_clear_active_conn_handle();
            tlkmdi_lea_us_clear_audio_state(handle);
        }
        s_lea_us_audio_param.active_state = LEA_US_ACTIVE_STATE_IDLE;
    }
    return true;
}

/**
 * @brief       Query whether music pipeline is currently active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_music_is_busy(void)
{
    return tlkmdi_lea_us_is_busy() && (s_lea_us_audio_param.active_state == LEA_US_ACTIVE_STATE_MUSIC);
}

/**
 * @brief       Execute opcode for music session (volume, call control).
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pData   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_us_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pData, uint16_t dataLen)
{
    (void)handle;
    return tlkmdi_lea_us_operate(opcode, pData, dataLen);
}

/**
 * @brief       Start/stop music audio path for given handle.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   status  - TLK_STATE_* command.
 * @return      true if transition accepted.
 */
bool tlkmdi_lea_us_music_switch(uint16_t handle, uint8_t status)
{
    s_lea_us_audio_param.active_state = LEA_US_ACTIVE_STATE_MUSIC;
    return tlkmdi_lea_us_switch(handle, status);
}

#if (TLK_MW_LEA_US_VOICE_ENABLE)

/**
 * @brief       Query whether voice pipeline is currently active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_voice_is_busy(void)
{
    return (tlkmdi_lea_us_is_busy() && s_lea_us_audio_param.active_state == LEA_US_ACTIVE_STATE_VOICE);
}

/**
 * @brief       Execute opcode for voice session.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pData   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_us_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pData, uint16_t dataLen)
{
    (void)handle;
    // maybe voice voice value maybe be different with music value.
    return tlkmdi_lea_us_operate(opcode, pData, dataLen);
}

/**
 * @brief       Start/stop voice audio path for given handle.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   status  - TLK_STATE_* command.
 * @return      true if transition accepted.
 */
bool tlkmdi_lea_us_voice_switch(uint16_t handle, uint8_t status)
{
    s_lea_us_audio_param.active_state = LEA_US_ACTIVE_STATE_VOICE;
    return tlkmdi_lea_us_switch(handle, status);
}

#endif

/**
 * @brief       Check if any LE Audio unicast server session is active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_is_busy(void)
{
    return (s_lea_us_audio_param.active_conn_handle != LEA_US_INVALID_CONN_HANDLE);
}

#endif
