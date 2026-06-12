/********************************************************************************************************
 * @file    app_recording_card_api.h
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

#define APP_RC_THREAD_ID   TLKSYS_TASKID_USER
#define APP_RC_PM_CHN      TLKSYS_PM_CHN_APP0
#define APP_RC_WIFI_PM_CHN TLKSYS_PM_CHN_APP1

#define APP_RC_CLOCK_CNH   TLKSYS_CLOCK_CHN_APP0

typedef enum
{
    APP_RC_THREAD_EVT_CORE = 0,
    APP_RC_THREAD_EVT_GET_MIC_PCM,
    APP_RC_THREAD_EVT_GET_MIC_OPUS,
    APP_RC_THREAD_EVT_LOGIC_WORK,
    APP_RC_THREAD_EVT_SPI_IO_INTERRUPT,
    APP_RC_THREAD_EVT_WIFI_OTA,
    APP_RC_THREAD_EVT_WIFI_UART,
} app_rc_thread_evt_type_e;

void app_rc_api_msc_switch(void);

void app_rc_api_start_record(void);

void app_rc_api_stop_record(void);

void app_rc_api_enable_wav_save(uint8_t en);

void app_rc_api_enable_alg(uint8_t en);

void app_rc_api_enable_ble(uint8_t en);

uint8_t app_rc_api_is_recording(void);

uint8_t app_rc_api_is_busy(void);

uint8_t app_rc_api_ble_is_enable(void);

void app_rc_api_report_opus_table(void);

void app_rc_api_report_wav_table(void);

void app_rc_api_send_opus_file(char *path, uint32_t offset);

void app_rc_api_send_file_by_Wifi(char *path, uint32_t offset);

void app_rc_api_rename_file(const char *oldPath, const char *newPath);

void app_rc_api_delete_file(const char *file);

void app_rc_api_wifi_power_on(uint16_t delayMs);

void app_rc_api_wifi_power_off(void);

void app_rc_api_wifi_power_switch(void);