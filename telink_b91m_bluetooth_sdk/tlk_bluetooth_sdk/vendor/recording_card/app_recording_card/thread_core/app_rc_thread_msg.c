/********************************************************************************************************
 * @file    app_rc_thread_msg.c
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
#include "app_rc_thread_msg.h"
#include "../logic/app_rc_logic.h"

static void app_rc_api_rename_file_deal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint32_t len = strlen((char *)pData);
    app_rc_rename_file((char *)pData, (char *)pData + 1 + len);
}

int app_recording_card_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    switch (msgID) {
    case APP_RC_THREAD_MSG_START_RECORD:
        app_rc_start_record();
        break;
    case APP_RC_THREAD_MSG_STOP_RECORD:
        app_rc_stop_record();
        break;
    case APP_RC_THREAD_MSG_REPORT_OPUS:
        app_rc_report_opus_table();
        break;
    case APP_RC_THREAD_MSG_REPORT_WAV:
        app_rc_report_wav_table();
        break;
    case APP_RC_THREAD_MSG_SEND_OPUS:
    {
        uint32_t offset = (uint32_t)pData[0] | ((uint32_t)pData[1] << 8) | ((uint32_t)pData[2] << 16) | ((uint32_t)pData[3] << 24);
        app_rc_send_ops_file((char *)(pData + 4), offset);
    } break;
    case APP_RC_THREAD_MSG_SEND_WIFI_FILE:
    {
        uint32_t offset = (uint32_t)pData[0] | ((uint32_t)pData[1] << 8) | ((uint32_t)pData[2] << 16) | ((uint32_t)pData[3] << 24);
        app_rc_send_file_by_Wifi((char *)(pData + 4), offset);
    } break;
    case APP_RC_THREAD_MSG_DEL_FILE:
        app_rc_delete_file((char *)pData);
        break;
    case APP_RC_THREAD_MSG_RENAME_FILE:
        app_rc_api_rename_file_deal(pData, dataLen);
        break;
    case APP_RC_THREAD_MSG_MSC_SWITCH:
        app_rc_msc_switch();
        break;
    case APP_RC_THREAD_MSG_WIFI_POWER_ON:
        uint16_t delayMs = pData[0] | (((uint16_t)pData[1]) << 8);
        app_rc_wifi_power_on(delayMs);
        break;
    case APP_RC_THREAD_MSG_WIFI_POWER_OFF:
        app_rc_wifi_power_off();
        break;
    case APP_RC_THREAD_MSG_WIFI_POWER_SWITCH:
        app_rc_wifi_power_switch();
        break;
    default:
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}
