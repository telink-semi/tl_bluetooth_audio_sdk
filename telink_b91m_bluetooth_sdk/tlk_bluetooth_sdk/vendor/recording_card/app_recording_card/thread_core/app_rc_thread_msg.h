/********************************************************************************************************
 * @file    app_rc_thread_msg.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once

typedef enum
{
    APP_RC_THREAD_MSG_START_RECORD = 0,
    APP_RC_THREAD_MSG_STOP_RECORD,
    APP_RC_THREAD_MSG_REPORT_OPUS,
    APP_RC_THREAD_MSG_REPORT_WAV,
    APP_RC_THREAD_MSG_SEND_OPUS,
    APP_RC_THREAD_MSG_SEND_WIFI_FILE,
    APP_RC_THREAD_MSG_DEL_FILE,
    APP_RC_THREAD_MSG_RENAME_FILE,
    APP_RC_THREAD_MSG_MSC_SWITCH,
    APP_RC_THREAD_MSG_WIFI_POWER_ON,
    APP_RC_THREAD_MSG_WIFI_POWER_OFF,
    APP_RC_THREAD_MSG_WIFI_POWER_SWITCH,
} app_rc_thread_msg_e;

int app_recording_card_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen);