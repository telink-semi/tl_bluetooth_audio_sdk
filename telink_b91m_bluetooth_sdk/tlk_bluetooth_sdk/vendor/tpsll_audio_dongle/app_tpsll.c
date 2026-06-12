/********************************************************************************************************
 * @file    app_tpsll.c
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
#include "stack/tpsll/tpsll.h"


#include "app_config.h"
#include "tlkapp/system/tlkapp_sysLed.h"
/*for debug, will remove later.*/
#include "tlkmw/tpsll/tlkmdi_tpsll_audio_dongle.h"

/**
 * @brief     This function servers to process usb input data.
 * @param[in] p   - pointer to the input data
 * @param[in] len - length of the input data
 * @returns   None.
 */
__attribute__((noinline)) void tlkusb_debug_shell_hook(unsigned char *p, uint16_t len)
{
    tlkapi_send_string_data(APP_LOG_EN, "[APP] ##USB DEBUG TPSLL##", p, len);
    if (p[0] != 0x11 && p[1] != 0x03) {
        return;
    }

    switch (p[2]) {
    case 0x02:
    {
        tlk_tpsll_tpd_host_dongle_set_setup_ac_chn(TPD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPD_HOST_DONGLE_SETUP_COMMON_CHN);
        uint8_t state = tlk_tpsll_tpd_host_dongle_start_connection_scan();
        tlkapi_send_string_data(APP_LOG_EN, "[APP] tlk_tpsll_tpd_host_dongle_start_connection_scan", &state, 1);
    } break;
    case 0x20:
    {
        u8 dongle_state = 0;
        tlkapi_send_string_data(APP_LOG_EN, "app tlk_tpsll_tph_dongle_start_connection_setup ", &dongle_state, 1);
    } break;

    case 0x21:
    {
        u8 dongle_state = 0;
        tlkapi_send_string_data(APP_LOG_EN, "tlk_tpsll_tph_dongle_exit_connection_setup ", &dongle_state, 1);
    } break;

    case 0x22:
    {
        if (tlk_tpsll_tpd_dongle_is_connected()) {
            u8 state = tlk_tpsll_tpd_host_start_dongle_sco_setup(p[2]);
            if (p[3] == TPD_HOST_DG_SCO_AUDIO_MODE_SPEAKER) {
                tlkapi_sendData(APP_LOG_EN, "tpd_dongle_start_sco_setup: TPD_DG_SCO_AUDIO_MODE_SPEAKER", &state, 1);
            } else if (p[3] == TPD_HOST_DG_SCO_AUDIO_MODE_MIC) {
                tlkapi_sendData(APP_LOG_EN, "tpd_dongle_start_sco_setup: TPD_DG_SCO_AUDIO_MODE_MIC", &state, 1);
            }
        }
        break;
    }
    case 0x30:
    {
        extern bool tlkmdi_ll_dongle_music_switch(uint16_t handle, uint8_t status);
        static u8   music_status = 0;
        music_status ^= 1;
        tlkmdi_ll_dongle_music_switch(0xffff, music_status);
    } break;

    case 0x31:
    {
        extern bool tlkmdi_ll_dongle_music_switch(uint16_t handle, uint8_t status);
        static u8   voice_status = 0;
        voice_status ^= 1;
        tlkmdi_ll_dongle_music_switch(0xfffe, voice_status);
    } break;

    case 0x32:
    {
        if (p[3] == 0x01) {
            tlkapi_sendData(APP_LOG_EN, "tpd_dongle improve tx power ", &p[3], 1);
        } else if (p[3] == 0x02) {
            tlkapi_sendData(APP_LOG_EN, "tpd_dongle reduce tx power ", &p[3], 1);
        } else if (p[3] == 0x03) {
            //            rf_set_power_level_index(p[4]);/*TPSLL_TX_POWER_INDEX*/
            tlk_tpsll_tpd_host_set_power_index(p[4]);
            tlkapi_sendData(APP_LOG_EN, "tpd_dongle set tx power index", &p[4], 1);
        }
    } break;
    case 0x33:
    {
        if (p[3] == 0x01) {
            tlk_tpsll_tpd_host_sniff_request();
            tlkapi_sendData(APP_LOG_EN, "tpd sniff request", &p[3], 1);
        } else if (p[3] == 0x00) {
            tlk_tpsll_tpd_host_unsniff_request();
            tlkapi_sendData(APP_LOG_EN, "tpd unsniff request", &p[3], 1);
        }
    } break;

    case 0x34:
    {
        tlk_tpsll_tpd_dongle_start_disconnection(TPD_DISCONNECT_REASON_USB_ENTER_SUSPEND);
        tlkapi_sendData(APP_LOG_EN, "tpd start disconnection because usb spd", 0, 0);
    } break;

    case 0x35:
    {
        tlkmdi_tpsll_audio_dongle_ReconHeadset_fromSuspend();
        tlkapi_sendData(APP_LOG_EN, "tpd start ReconHeadset from suspend", 0, 0);
    } break;

    case 0x36:
    {
        u8            acl_mtu        = tlk_tpsll_tpd_host_get_acl_mtu();
        unsigned char s_acl_pdu[180] = {0};
        for (unsigned char i = 0; i < 180; i++) {
            s_acl_pdu[i] = i;
        }
        tlk_tpsll_tpd_host_send_acl_data(TPD_HOST_MSG_PDU_ACL_CMD_APP, &s_acl_pdu[0], 180, NULL);
        tlkapi_sendData(APP_LOG_EN, "tpd send acl data to headset, to test acl mtu", &acl_mtu, 1);
    } break;

    case 0x37:
    {
        tlk_tpsll_tpd_host_per_statistics_ctrl(p[3]);
        tlkapi_sendData(APP_LOG_EN, "tpd set per state", &p[3], 1);
    } break;

    case 0x38:
    {
        tlkapi_sendData(APP_LOG_EN, "tpd GIP Certification", &p[3], 1);
        if (p[3]) {
            tlk_tpsll_tpd_host_start_gip_mode();
        } else {
            tlk_tpsll_tpd_host_exit_gip_mode();
        }
    } break;

    default:
        break;
    }
    return;
}
