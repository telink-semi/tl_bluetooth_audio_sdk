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
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/tph/tph_hcicmd.h"
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
 * @brief      Hook function called when system task initialization is completed
 * @param[in]  none
 * @return     none
 * @note       Disables SQL auto-save and registers key callback for force saving
 */
void tlkapp_sys_taskInitCompletedHook(void)
{
    tlkmdi_tinySql_setSaveEnable(0);
#if TLK_DEV_KEY_ENABLE
    tlkdrv_key_registerVendorConfig1Callback(app_sql_info_force_save);
#endif

#if TLK_CFG_SUSPEND_ENABLE
    tlksdk_pm_setDeepsleepRetentionEarlyWakeupTiming(2850);
#endif
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
    app_sql_info_force_save(); //any 11 03 xx usb shell will save
    switch (pData[2]) {
    case 0x01:
    {
        extern void tlkapp_audioScheduler_debug(void); // not thread safe
        tlkapp_audioScheduler_debug();
    } break;
    case 0x02:
    {
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(TPT_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPT_HOST_DONGLE_SETUP_COMMON_CHN);
        tpsll_hci_sendCreateConnectCmd(TPT_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
    } break;

    case 0x03:
    {
        tpsll_hci_sendExitDongleConnSetupCmd();
    } break;

    case 0x04:
        tpsll_hci_sendDisconnCmd(TPT_HOST_DISCONNECT_REASON_DONGLE_SETUP);
        break;
    case 0x05:
    {
        tlkapi_printf(1, "Master slave link viture...");
        tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_3S, 500 * 1000);

    } break;
    case 0x06:
    {
        tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_3S, 500 * 1000);
    } break;
    case 0x07:
    {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_START_HANDOVER, NULL, 0);
    } break;
    case 0x08:
    {
        tlkapi_printf(1, "start anc audio");
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_ANC, 0xffff);
    } break;
    case 0x09:
    {
        tlkapi_printf(1, "stop anc audio");
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_ANC, 0xffff);
    } break;
#if TLK_MW_DSP_COMM_ENABLE && TLKALG_ANC_ENABLE
    case 0x0a:
    {
        tlkapi_printf(1, "[test]open anc adaptive anc");
        d25f_switch_anc_adaptive_mode(1);
    } break;
    case 0x0b:
    {
        tlkapi_printf(1, "[test]close anc adaptive anc");
        d25f_switch_anc_adaptive_mode(0);
    } break;
#endif
    case 0x0c:
    {
#if TLK_CFG_TONE_ENABLE
        tlkapi_printf(1, "tlkmdi_tone_is_busy %d", tlkmdi_tone_is_busy());
#endif
    } break;
    case 0x0d:
    {
        tlkmdi_bt_tpt_requestToneSync(0);
    } break;
    case 0x0e:
    {
        tlkmdi_bt_tpt_requestToneSync(0);
        tlkmdi_bt_tpt_requestToneSync(6);
        tlkmdi_bt_tpt_requestToneSync(1);
    } break;
#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
    case 0x10:
    {
        audio_alg_set_loopback_enable(pData[3]);
    } break;
#endif
    case 0x17:
    {
        tlkmdi_btSet_scan(3, 120);
    } break;
    case 0x30: // TPSLL HCI TEST
    {
        if (pData[3] == TPSLL_HCI_START_HEADSET_CONNECTION_SETUP_OPCODE) {
            tlkapi_send_string_data(APP_LOG_EN, "tpsll_hci_sendCreateConnectCmd", &pData[2], 1);
            tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(TPH_HOST_HEADSET_SETUP_COMMON_ACCESSCODE, TPH_HOST_HEADSET_SETUP_COMMON_CHN);
            tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_3S, 500 * 1000);
        } else if (pData[3] == TPSLL_HCI_DISCONN_CMD_OPCODE) {
            tpsll_hci_sendDisconnCmd(0 /*TPT_HOST_DISCONNECT_REASON_NONE*/);
        } else if (pData[3] == TPSLL_HCI_TWS_SET_SINGLE_ROLE_OPCODE) {
            tlkapi_send_string_u32s(APP_LOG_EN, "tpsll_hci_sendSetHeadsetIsSingleCmd", 30, 0, 0, 0);
            tpsll_hci_sendSetHeadsetIsSingleCmd();
        } else if (pData[3] == TPSLL_HCI_TX_POWER_CONTROL_OPCODE) {
            tlkapi_send_string_u32s(APP_LOG_EN, "tpsll_hci_setPower_Control", 0, 0, 0, 0);
            tpsll_hci_setPower_Control(TPT_HOST_LOCAL_POWER_CONTROL, TPT_HOST_DECR_POWER_REQ);
        } else if (pData[3] == TPSLL_HCI_SNIFF_REQUEST_OPCODE) {
            tlkapi_send_string_u32s(APP_LOG_EN, "tpsll_hci_sniff request", pData[4], 0, 0, 0);
            tpsll_hci_sniff_Request(pData[4]);
        } else if (pData[3] == 0xF1) {
            tlkapi_send_string_u32s(APP_LOG_EN, "tlkmdi_btSet_scan", 30, 0, 0, 0);
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 30); //not thread safe in rtos !!!!!! attention
        }
    } break;

    case 0x31: // Get TWS cur role
    {
        extern void tlkmdi_bt_tpt_debug_printf_addr_info(void);
        tlk_printf("##TWS cur_role:[%d], is master:[%d], is slave:[%d]", tlkmdi_bt_tpt_getRole(), tlkmdi_bt_tpt_isMaster(), tlkmdi_bt_tpt_isSlave());
        tlkapi_printf(1, "##TWS cur_mode:[0x%x]", app_tph_headset_get_mode());
        tlkmdi_bt_tpt_debug_printf_addr_info();
    } break;

    case 0x32:
    {
        tpsll_hci_sendReadProfileSyncInfoCmd();
    } break;

#if DONGLE_VOICE_MIC_EN
    case 0x33: //choose mix mic mode: 00:bt mic  01:tpsll mic
    {
        ll_mix_voice_set_mic_mode(pData[3]); //attention : not thread safe
    } break;
#endif

    case 0x34: // Shutdwon
    {
        tlk_printf("tws cur_role:[%d], is master:[%d], is slave:[%d]", tlkmdi_bt_tpt_getRole(), tlkmdi_bt_tpt_isMaster(), tlkmdi_bt_tpt_isSlave());
        tpsll_hci_sendDisconnCmd(TPT_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN);
    } break;
    case 0x35:
    {
#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
        extern void bt_music_set_latency_mode(uint8_t mode);
        bt_music_set_latency_mode(pData[3]);
#endif
    } break;

    case 0x37:
    {
        extern void tlkmdi_bt_tpt_pair_start_req(uint8_t isRefactory, uint8_t * peerMac);
        tlkmdi_bt_tpt_pair_start_req(7, NULL);
    } break;
    case 0x38:
    {
        extern void tlkmdi_bt_tpt_headset_exitLowLatencyMode(void);
        tlkmdi_bt_tpt_headset_exitLowLatencyMode();
    } break;
    case 0x40:
    {
        extern void bt_music_set_ppm_debug(int16_t ppm_debug);
        int16_t     ppm = pData[3] | pData[4] << 8;

        if (pData[5] == 1) {
            ppm = -ppm;
        }

        bt_music_set_ppm_debug(ppm);
    } break;

#if AUDIO_HD_HAC_EN
    case 0x50:
        if (audio_hd_anc_enable) {
            audio_hd_anc_enable = 0;
        } else {
            audio_hd_anc_enable = 1;
        }
        tlkapi_send_string_data(APP_LOG_EN, "audio MCLK switch:184.32M = %d ", &audio_hd_anc_enable, 4);
        break;
#endif
    case 0x55:
    {
        //extern void bt_audio_print_task_stack_info(void);
        //bt_audio_print_task_stack_info();
    } break;
    case 0x60:
    {
        if (pData[3] == 0x01) {
            g_sys_work_mode = 0x80; //attention : not thread safe
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Open");
        } else if (pData[3] == 0x00) {
            g_sys_work_mode = 0x00; //attention : not thread safe
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Close");
        }
    } break;

    default:
        break;
    }
}