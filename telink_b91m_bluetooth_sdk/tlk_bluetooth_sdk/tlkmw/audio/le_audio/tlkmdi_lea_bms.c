/********************************************************************************************************
 * @file    tlkmdi_lea_bms.c
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
#include "stack/ble/ble.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tlkapi/tlkapi.h"

#include "le_audio_common.h"
#include "le_audio_main.h"
#include "tlkmdi_lea_bms.h"

#include "codec/lea_codec.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#if (TLK_MW_LEA_BMS_ENABLE)
#define TLKMDI_LEA_BMS_DBG_FLAG 0xFFFFFFFF
#define TLKMDI_LEA_BMS_DBG_SIGN "[BMS]"



struct lea_bms_audio_state
{
    bool                        busy;
    struct lea_bms_stream_param stream;
};

static struct lea_bms_audio_state s_lea_bms_state = {
        .busy = false,
};
uint32_t cig_start_time = 0;
static _attribute_ram_code_ void tlkmdi_le_cig_start_time(void)
{
    cig_start_time = clock_time();
}
/**
 * @brief       Initialize BMS state, callbacks, and open 48 kHz codec.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_bms_init(void)
{
    tlkapi_trace(TLKMDI_LEA_BMS_DBG_FLAG, TLKMDI_LEA_BMS_DBG_SIGN, __func__);
    lea_input_config_initial();
    blt_ll_cis_central_event_start_cb(tlkmdi_le_cig_start_time);
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000,0);
    return TLK_ENONE;
}

/**
 * @brief       Check whether BMS audio pipeline is currently active.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_bms_is_busy(void)
{
    return s_lea_bms_state.busy;
}

/**
 * @brief       Handle audio open/close requests for BMS pipeline.
 * @param[in]   handle  - audio manager handle (unused).
 * @param[in]   status  - TLK_STATE_* transition command.
 * @return      true if transition accepted, false otherwise.
 */
bool tlkmdi_lea_bms_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(TLKMDI_LEA_BMS_DBG_FLAG, TLKMDI_LEA_BMS_DBG_SIGN, "tlkmdi_lea_bms_switch%s, status=%d", __func__, status);

    if (status == TLK_STATE_OPENED) {
        if (s_lea_bms_state.busy) {
            return false;
        }
        s_lea_bms_state.busy = true;
    } else {
        if (!s_lea_bms_state.busy) {
            return false;
        }
        s_lea_bms_state.busy = false;
    }

    if (status == TLK_STATE_OPENED) {
    } else {
    }
    return true;
}

/**
 * @brief       Configure codec/input path and add ASE configs for broadcast source.
 * @param[in]   p_config    - LC3 configuration from controller.
 * @return      none.
 */
void tlkmdi_lea_bms_start(struct tlkmdi_lea_bms_config *p_config)
{
    tlk_printf("tlkmdi_lea_bms_start");
    s_lea_bms_state.stream.frequency = p_config->samplingFrequency;
    s_lea_bms_state.stream.location = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT;
    s_lea_bms_state.stream.num = 2;
    s_lea_bms_state.stream.bms_config[0].samplingFrequency = p_config->samplingFrequency;
    s_lea_bms_state.stream.bms_config[0].frameDuration = p_config->frameDuration;
    s_lea_bms_state.stream.bms_config[0].frameOctets = p_config->frameOctets;
    s_lea_bms_state.stream.bms_config[0].iso_handle = p_config->iso_handle[0];

    s_lea_bms_state.stream.bms_config[1].samplingFrequency = p_config->samplingFrequency;
    s_lea_bms_state.stream.bms_config[1].frameDuration = p_config->frameDuration;
    s_lea_bms_state.stream.bms_config[1].frameOctets = p_config->frameOctets;
    s_lea_bms_state.stream.bms_config[1].iso_handle = p_config->iso_handle[1];

    struct lea_codec_config codec_config = {
            .is_input_stream_init  = true,
            .input_sample_rate     = p_config->samplingFrequency,
            .input_location       = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
            .is_output_stream_init = false,
    };
    lea_codec_stream_init(&codec_config);

    lea_input_config_initial();
    lea_open_input(clock_time());
    lea_set_input_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
    lea_set_input_sample_config_bap(p_config->samplingFrequency, p_config->frameDuration);
    struct lea_config s_lea_config = {
            .blocks = 1,
            // .location = LEA_LOCATION_FRONT_LEFT,
            .samplingFrequency = p_config->samplingFrequency,
            .frameDuration     = p_config->frameDuration,
            .frameOctets       = p_config->frameOctets,
            // .iso_handle = 0x10,
            // .presentationDelay = 40000,
    };
    s_lea_config.location   = LEA_LOCATION_FRONT_LEFT;
    s_lea_config.iso_handle = p_config->iso_handle[0];
    lea_set_input_config(&s_lea_config);
    s_lea_config.location   = LEA_LOCATION_FRONT_RIGHT;
    s_lea_config.iso_handle = p_config->iso_handle[1];
    lea_set_input_config(&s_lea_config);
#if (MCU_CORE_TYPE == MCU_CORE_TL721X||MCU_CORE_TYPE == MCU_CORE_TL322X)
        timer_set_irq_mask(FLD_TMR0_MODE_IRQ);
#endif
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);

    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN,le_audio_main_loop);
}
/**
 * @brief       Cache stream parameters and set ISO data path host->controller.
 * @param[in]   param   - stream parameter descriptor.
 * @return      none.
 */
void tlkmdi_lea_bms_start_stream(struct lea_bms_stream_param *param)
{
    s_lea_bms_state.stream = *param;
    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_LEA_BMS, 0);
    for (int i = 0; i < param->num; i++) {
        struct ble_hci_le_setup_iso_data_path_full_cp p_cig_params = {
        .conn_handle = param->bms_config[i].iso_handle,
        .data_path_dir = Data_Dir_Output,
         .data_path_id = Data_Path_HCI,
    };

    struct ble_hci_le_setup_iso_data_path_rp  p_cig_params_rp;
    ble_host_hci_le_setup_iso_data_path(&p_cig_params,&p_cig_params_rp);
    }
}

/**
 * @brief       Notify audio manager to close BMS stream.
 * @return      none.
 */
void tlkmdi_lea_bms_stop_stream(void)
{
    tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_LEA_BMS, 0);
}
#endif
