/********************************************************************************************************
 * @file    app.c
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

#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "tlkmw/tlkmw.h"

/**
 * @brief      Force save SQL information
 * @param[in]  none
 * @return     none
 * @note       Enables saving, saves data, then disables saving again
 */
static void app_sql_info_force_save(void)
{
    tlkmdi_tinySql_setSaveEnable(1);
    tlkmdi_tinySql_save();
    tlkmdi_tinySql_setSaveEnable(0);
}

/**
 * @brief       This function processes USB debug shell data.
 * @param[in]   pData   - pointer to the data received from USB.
 * @param[in]   dataLen - length of the data received.
 * @return      none.
 * @note
 */
__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_array(0xffffffff, "<test>", "tlkusb_debug_shell_hook :", pData, dataLen);

    if (pData[0] != 0x11 && pData[1] != 0x03) {
        return;
    }

    switch (pData[2]) {
    case 0x01:
        tlkapi_printf(1, "Compile Time %s", __TIME__);
        break;
    case 0x02:
    {
        tlkapi_printf(1, "tlkmdi_bt_tph_pair_start: Urtal-low-latency");
        tlkmdi_bt_tph_pair_start(TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY);
    } break;
    case 0x03:
    {
        tlkapi_printf(1, "tlkmdi_bt_tph_exitLowLatencyMode: Exit Urtal-low-latency");
        tlkmdi_bt_tph_exitLowLatencyMode();
    } break;

    case 0x10:
    {
        app_sql_info_force_save();
    } break;

    case 0x30: // TPSLL HCI TEST
    {
        if (pData[3] == TPSLL_HCI_CONNECT_CMD_OPCODE) {
            tlkapi_send_string_data(APP_LOG_EN, "tpsll_hci_sendCreateConnectCmd", &pData[3], 1);
            tpsll_hci_sendCreateConnectCmd(TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
        } else if (pData[3] == TPSLL_HCI_WRITE_AC_CHNID_CMD_OPCODE) {
            tlkapi_send_string_u32s(APP_LOG_EN, "tpsll_hci_sendWriteAccessCodeAndChnIDCmd", TPH_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPH_HOST_DONGLE_SETUP_COMMON_CHN, 0, 0);
            tpsll_hci_sendWriteAccessCodeAndChnIDCmd(TPH_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPH_HOST_DONGLE_SETUP_COMMON_CHN);
        } else if (pData[3] == TPSLL_HCI_SET_BT_MAC_CMD_OPCODE) {
            u8 local_mac[6] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};
            tlkapi_send_string_data(APP_LOG_EN, "tpsll_hci_sendSetBtAddrCmd", &local_mac[0], 6);
            tpsll_hci_sendSetBtAddrCmd(0 /*istws*/, &local_mac[0]);
        } else if (pData[3] == TPSLL_HCI_SCO_MIC_DATA_OPCODE) {
            u8 mic_data[/*TPSLL_HOST_10MS_MIC_LC3_DATA_LEN*/ 30] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                                                    0x88, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
            tlkapi_send_string_data(APP_LOG_EN, "tpsll_hci_sendMicDataCmd", &mic_data[0], 30);

            tpsll_hci_sendMicDataCmd(/*TPSLL_HOST_AUDIO_FORMAT_LC3A*/ 1, 0, 30, &mic_data[0]);
        }
    } break;

    case 0x31: //open bt music low latency mode
    {
        bt_music_set_low_latency_mode(pData[3]); //attention : not thread safe
    } break;

    case 0x32: //choose tone mode
    {
        tone_play(pData[3]); //attention : not thread safe
    } break;

#if DONGLE_VOICE_MIC_EN
    case 0x33: //choose mix mic mode: 00:bt mic  01:tpsll mic
    {
        ll_mix_voice_set_mic_mode(pData[3]); //attention : not thread safe
    } break;
#endif

        break;
    case 0x60:
    {
        if (pData[3] == 0x01) {
            tlkdrv_codec_test_mode_en(1);
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Open");
        } else if (pData[3] == 0x00) {
            tlkdrv_codec_test_mode_en(0);
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Close");
        }
    } break;
    case 0x70:
    {
#if (TLKBTP_CFG_CUSTP_ENABLE)
        extern int btp_custp_connect(uint16_t aclHandle, uint8_t usrID);
        btp_custp_connect(0x08, 0);
#endif
    } break;

#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
    case 0x80:
    {
        audio_alg_set_loopback_enable(pData[3]);
    } break;
#endif
    case 0x55:
    {
        //extern void bt_audio_print_task_stack_info(void);
        //bt_audio_print_task_stack_info();
    } break;

    default:
        break;
    }
}

void tlkapp_audio_taskInitCompletedHook(void)
{
#if (TLK_USB_UAC_ENABLE)
    extern int tlkapp_audioUac_init(void);
    tlkapp_audioUac_init();
#endif
}
