/********************************************************************************************************
 * @file    tlkmdi_lea_uc.c
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
#include "tlkapi/tlkapi.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "drivers.h"

#include "le_audio_main.h"
#include "le_audio_common.h"
#include "tlkmdi_lea_uc.h"

#include "stack/ble/ble.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "codec/lea_codec.h"
#include "stack/ble/host_v1/profile/le_audio/trans_coord/cap/inc/cap_sm.h"
#if (TLK_MW_LEA_UC_ENABLE)
#if (TLK_USB_UAC_ENABLE)
extern uint8_t gLeaPpmStatus;
#endif
#define TLKMDI_LEA_UC_DBG_FLAG        ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_LEA_UC << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define LE_AUDIO_UNICAST_CLIENT_MUSIC 0x01
#define LE_AUDIO_UNICAST_CLIENT_VOICE 0x02
#define MDI_LEA_UC_PRINTF(fmt, ...)   tlk_printf("[MDI_LEA_UC] " fmt, ##__VA_ARGS__)

// #define MDI_LEA_UC_PRINTF(...)
enum
{
    TLKMW_LE_AUDIO_STOP = 0,
    TLKMW_LE_AUDIO_CREATE_MUSIC,
    TLKMW_LE_AUDIO_IN_MUSIC,
    TLKMW_LE_AUDIO_CREATE_VOICE,
    TLKMW_LE_AUDIO_IN_VOICE,
    TLKMW_LE_AUDIO_STOP_MUSIC,
    TLKMW_LE_AUDIO_STOP_VOICE,
};

struct tlkmdi_lea_uc_ctrl
{
    uint32_t state;
    uint16_t virtual_handle;
    uint32_t cig_start_time;
};

static struct tlkmdi_lea_uc_ctrl s_tlkmdi_lea_uc_ctrl = {
    .state = TLKMW_LE_AUDIO_STOP,
};


static struct prefer_codec_cfg *s_prefer_codec_cfg;
// #if TLK_SIMPLE_PPM_EN
uint8_t g_ppm_in_flag = 0;
// #endif

static TlkApiTimerHandle_t s_lea_uc_oper_timer;

static void tlkmdi_lea_uc_oper_timer_callback(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)handle;
    (void)userArg;
    uint8_t                            task_handle  = *(uint8_t *)userArg;
    const struct ble_ase_codec_config *music_config = NULL;
    const struct ble_ase_codec_config *voice_config = NULL;
    MDI_LEA_UC_PRINTF("codec reject timer callback %d", s_tlkmdi_lea_uc_ctrl.state);
    if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_VOICE) {
        if (s_prefer_codec_cfg && s_prefer_codec_cfg->voice_codec_select + 1 < s_prefer_codec_cfg->voice_codec_number) {
            s_prefer_codec_cfg->voice_codec_select++;
            music_config = s_prefer_codec_cfg->voice_codec_cfg[s_prefer_codec_cfg->voice_codec_select].music_config;
            voice_config = s_prefer_codec_cfg->voice_codec_cfg[s_prefer_codec_cfg->voice_codec_select].voice_config;
            ble_capuc_start_voice_streaming(task_handle, music_config, voice_config);
        } else {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP;
            MDI_LEA_UC_PRINTF("no available config to start voice");
        }
    } else if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_MUSIC) {
        if (s_prefer_codec_cfg && s_prefer_codec_cfg->music_codec_select + 1 < s_prefer_codec_cfg->music_codec_number) {
            s_prefer_codec_cfg->music_codec_select++;
            music_config = s_prefer_codec_cfg->music_codec_cfg[s_prefer_codec_cfg->music_codec_select].music_config;
            ble_capuc_start_music_streaming(task_handle, music_config);
        } else {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP;
            MDI_LEA_UC_PRINTF("no available config to start music");
        }
    } else {
        MDI_LEA_UC_PRINTF("state is not right,cant start stream");
    }
}

static void tlkmdi_le_audio_unicast_client_state_callback(int state, const void *param)
{
    MDI_LEA_UC_PRINTF("cap state callback state:%d, param:%s", state, param ? hex_to_str(param, 8) : "None");
    if (state == CAP_BOTH_INPUT_INIT_PARAM) {
        lea_open_input(s_tlkmdi_lea_uc_ctrl.cig_start_time);
    } else if (state == CAP_BOTH_INPUT_DEINIT_PARAM) {
        lea_close_input();
    } else if (state == CAP_INPUT_INIT_PARAM) {
        lea_set_input_config(param);
    } else if (state == CAP_INPUT_DEINIT_PARAM) {
        const uint16_t *p_iso_handle = (const uint16_t *)param;
        lea_release_input_config(*p_iso_handle);
    } else if (state == CAP_BOTH_OUTPUT_INIT_PARAM) {
        lea_open_output();
    } else if (state == CAP_OUTPUT_INIT_PARAM) {
        lea_set_output_config(param);
    } else if (state == CAP_BOTH_OUTPUT_DEINIT_PARAM) {
        lea_close_output();
    } else if (state == CAP_OUTPUT_DEINIT_PARAM) {
        const uint16_t *p_iso_handle = (const uint16_t *)param;
        lea_release_output_config(*p_iso_handle);
    } else if (state == CAP_STREAMING_SUCCESS) {
        if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_MUSIC) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_IN_MUSIC;
#if (TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE)
            if (sTlkUsbUacEvt2StatusCB != NULL) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_START_RSP);
            }
#endif
        } else if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_VOICE) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_IN_VOICE;

#if (TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE)
            if (sTlkUsbUacEvt2StatusCB != NULL) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_START_RSP);
            }
#endif
        }
        // tlksys_timer_start(TLKSYS_TASKID_AUDIO, s_lea_uc_oper_timer);

    } else if (state == CAP_ENTER_IDLE) {
#if (TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE)
        if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_MUSIC || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_MUSIC ||
            s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_MUSIC) {
            if (sTlkUsbUacEvt2StatusCB != NULL) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_STOP_RSP);
            }
        } else if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_VOICE || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_VOICE ||
                   s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_VOICE) {
            if (sTlkUsbUacEvt2StatusCB != NULL) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_STOP_RSP);
            }
        }
#endif
        if (s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_MUSIC || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_VOICE) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP;
        }

        lea_close_input();
        lea_close_output();
        lea_codec_stream_deinit();
    } else if (state == CAP_CODEC_REJECT) {
        static uint8_t task_handle = 0;
        task_handle                = *(uint8_t *)param;
        int ret                    = tlksys_timer_create(TLKSYS_TASKID_AUDIO, &s_lea_uc_oper_timer, 100 * 1000, false, tlkmdi_lea_uc_oper_timer_callback, &task_handle);
        tlksys_timer_start(TLKSYS_TASKID_AUDIO, s_lea_uc_oper_timer);
        MDI_LEA_UC_PRINTF("cap uc codec reject create timer %d", ret);
    }
}

static _attribute_ram_code_ void tlkmdi_le_cig_start_time(void)
{
    s_tlkmdi_lea_uc_ctrl.cig_start_time = clock_time();
}

/**
 * @brief       Cache preferred codec list used when creating streams.
 * @param[in]   cfg - pointer to preference structure.
 * @return      TLK_ENONE.
 */
int tlkmdi_lea_uc_set_prefer_codec_cfg(struct prefer_codec_cfg *cfg)
{
    if (cfg) {
        s_prefer_codec_cfg = cfg;
    }
    return TLK_ENONE;
}

/*
 * @brief  Initial the LE audio block, register the data callback.
 *
 * @param[in] None
 *
 * @returns TLK_ENONE is success,other value is false.
 */
/**
 * @brief       Initialize LE Audio unicast client control and callbacks.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_init(void)
{
    MDI_LEA_UC_PRINTF("%s", __FUNCTION__);
    lea_input_config_initial();
    lea_output_config_initial();

    blt_ll_cis_central_event_start_cb(tlkmdi_le_cig_start_time);
    cap_uc_register_state_change_callback(tlkmdi_le_audio_unicast_client_state_callback);

    return TLK_ENONE;
}

/**
 * @brief       Send music play command to peer/USB stack.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   param   - optional parameter (unused).
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_music_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;
    MDI_LEA_UC_PRINTF("%s", __FUNCTION__);
#if (TLK_USB_UAC_ENABLE)
    tlklib_uacctrl_play_pause();
#endif
    return TLK_ENONE;
}

/**
 * @brief       Send music pause command to peer/USB stack.
 * @param[in]   handle  - LE Audio virtual handle.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_music_close(uint16_t handle)
{
    (void)handle;
    MDI_LEA_UC_PRINTF("%s", __FUNCTION__);
#if (TLK_USB_UAC_ENABLE)
    tlklib_uacctrl_play_pause();
#endif
    return TLK_ENONE;
}

/**
 * @brief       Request next track via USB controller.
 * @return      true always.
 */
bool tlkmdi_lea_uc_music_next(void)
{
#if (TLK_USB_UAC_ENABLE)
    tlklib_uacctrl_play_next();
#endif
    return true;
}

/**
 * @brief       Request previous track via USB controller.
 * @return      true always.
 */
bool tlkmdi_lea_uc_music_previous(void)
{
#if (TLK_USB_UAC_ENABLE)
    tlklib_uacctrl_play_prev();
#endif
    return true;
}

static bool tlkmdi_lea_uc_operate(uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    bool ret = true;

    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
#if (TLK_USB_UAC_ENABLE)
        tlkusb_uacctrl_volume_up();
#endif
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
#if (TLK_USB_UAC_ENABLE)
        tlkusb_uacctrl_volume_down();
#endif
    } break;
    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
#if (TLK_USB_UAC_ENABLE)
        tlklib_uacctrl_callAccept();
#endif
    } break;

    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
#if (TLK_USB_UAC_ENABLE)
        tlklib_uacctrl_callHungUp();
#endif
    } break;
    default:
    {
        ret = false;
    } break;
    }
    return ret;
}

/**
 * @brief       Execute operation for music session.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   opcode  - TLKAUD opcode (volume, call, etc.).
 * @param[in]   pdata   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_uc_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)handle;
    return tlkmdi_lea_uc_operate(opcode, pdata, dataLen);
}

static bool tlkmdi_lea_uc_switch(uint16_t handle, uint8_t status, uint8_t type)
{
    bool ret = true;
    MDI_LEA_UC_PRINTF("%s", __FUNCTION__);
    MDI_LEA_UC_PRINTF("receive switch %s, handle:0x%03x", status == TLK_STATE_OPENED ? "open" : "close or paused", handle);
    MDI_LEA_UC_PRINTF("status is %d, state is %d, handle :%d", status, s_tlkmdi_lea_uc_ctrl.state, handle);
    if (status == TLK_STATE_OPENED) {
        // if le audio unicast client is already in use, return false.
        if (s_tlkmdi_lea_uc_ctrl.state != TLKMW_LE_AUDIO_STOP) {
            ret = false;
        }
    } else {
        if (type == LE_AUDIO_UNICAST_CLIENT_MUSIC) {
            ret = s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_MUSIC || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_MUSIC;
        } else if (type == LE_AUDIO_UNICAST_CLIENT_VOICE) {
            ret = s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_VOICE || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_VOICE;
        }
    }

    if (ret == false) {
        return false;
    }

    s_tlkmdi_lea_uc_ctrl.virtual_handle = handle;

    MDI_LEA_UC_PRINTF("check status success");

    if (status == TLK_STATE_OPENED) {
#if (TLK_USB_UAC_ENABLE)
        gLeaPpmStatus = 0x01;
#endif
        struct lea_codec_config codec_config = {
            .is_input_stream_init  = true,
            .input_sample_rate     = LEA_SELECT_SAMPLING_FREQ_48000_HZ,
            .input_location        = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
            .is_output_stream_init = false,
        };

        if (type == LE_AUDIO_UNICAST_CLIENT_MUSIC) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_CREATE_MUSIC;
            lea_set_input_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
            lea_set_input_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_10);
            MDI_LEA_UC_PRINTF("start tlkmdi_lea_uc_switch create music stream %d", handle);
            if (s_prefer_codec_cfg) {
                ble_capuc_start_music_streaming(handle, s_prefer_codec_cfg->music_codec_cfg[s_prefer_codec_cfg->music_codec_select].music_config);
            } else {
                ble_capuc_start_music_streaming(handle, LEA_CAPUC_LC3_48_4_STEREO_BALANCED);
            }

            lea_codec_stream_init(&codec_config);
        }

        if (type == LE_AUDIO_UNICAST_CLIENT_VOICE) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_CREATE_VOICE;
            lea_set_input_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
            lea_set_input_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10);
            lea_set_output_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
            lea_set_output_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10);
            MDI_LEA_UC_PRINTF("start tlkmdi_lea_uc_switch create voice stream %d", handle);
            if (s_prefer_codec_cfg) {
                ble_capuc_start_voice_streaming(handle, s_prefer_codec_cfg->voice_codec_cfg[s_prefer_codec_cfg->voice_codec_select].music_config,
                                                s_prefer_codec_cfg->voice_codec_cfg[s_prefer_codec_cfg->voice_codec_select].voice_config);
            } else {
                ble_capuc_start_voice_streaming(handle, LEA_CAPUC_LC3_48_4_STEREO_BALANCED, LEA_CAPUC_LC3_16_2_STEREO_BALANCED);
            }

            codec_config.input_sample_rate     = LEA_SELECT_SAMPLING_FREQ_32000_HZ;
            codec_config.is_output_stream_init = true;
            codec_config.output_sample_rate    = LEA_SELECT_SAMPLING_FREQ_32000_HZ;
            codec_config.output_location       = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT;

            lea_codec_stream_init(&codec_config);
        }


#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X)
        timer_set_irq_mask(FLD_TMR0_MODE_IRQ);
#endif
        /* can not delete this code because other project is different with this project in async init */
#if (MCU_CORE_TL752X_TEMP)
        plic_interrupt_enable(IRQ_TIMER0_0);

        plic_set_priority(IRQ_TIMER0_0, 1);
#else
        plic_interrupt_enable(IRQ_TIMER0);
        plic_set_priority(IRQ_TIMER0, 1);
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, le_audio_timer_irq);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, le_audio_main_loop);

    } else {
        if (type == LE_AUDIO_UNICAST_CLIENT_MUSIC) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP_MUSIC;
        } else if (type == LE_AUDIO_UNICAST_CLIENT_VOICE) {
            s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP_VOICE;
        }
#if (TLK_USB_UAC_ENABLE)
        if (gLeaPpmStatus) {
            if (g_ppm_in_flag == 1) {
                g_ppm_in_flag = 0;
            }
            gLeaPpmStatus = 0x00;
        }
#endif
        lea_release_input_location();
        lea_release_output_location();
        MDI_LEA_UC_PRINTF("stop tlkmdi_lea_uc_switch stop stream %d", handle);
        if (ble_capuc_stop_music_streaming(handle) != BLE_HOST_ERR_SUCC) {
            // s_tlkmdi_lea_uc_ctrl.state = TLKMW_LE_AUDIO_STOP;
        }

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        lea_codec_stream_deinit();
    }

    return true;
}

/**
 * @brief       Open/close music path depending on TLK_STATE command.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   status  - TLK_STATE_* value.
 * @return      true if state transition accepted.
 */
bool tlkmdi_lea_uc_music_switch(uint16_t handle, uint8_t status)
{
    return tlkmdi_lea_uc_switch(handle, status, LE_AUDIO_UNICAST_CLIENT_MUSIC);
}

/**
 * @brief       Report whether music pipeline is active or pending stop.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_uc_music_is_busy(void)
{
    return s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_MUSIC || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_MUSIC ||
           s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_MUSIC;
}

/**
 * @brief       Execute operation for voice session.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pdata   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_uc_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)handle;
    return tlkmdi_lea_uc_operate(opcode, pdata, dataLen);
}

/**
 * @brief       Open/close voice path depending on TLK_STATE command.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   status  - TLK_STATE_* value.
 * @return      true if state transition accepted.
 */
bool tlkmdi_lea_uc_voice_switch(uint16_t handle, uint8_t status)
{
    return tlkmdi_lea_uc_switch(handle, status, LE_AUDIO_UNICAST_CLIENT_VOICE);
}

/**
 * @brief       Report whether voice pipeline is active or pending stop.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_uc_voice_is_busy(void)
{
    return s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_CREATE_VOICE || s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_IN_VOICE ||
           s_tlkmdi_lea_uc_ctrl.state == TLKMW_LE_AUDIO_STOP_VOICE;
}

#endif
