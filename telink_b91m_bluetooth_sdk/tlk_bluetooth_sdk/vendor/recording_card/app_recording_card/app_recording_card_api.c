/********************************************************************************************************
 * @file    app_recording_card_api.c
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
#include "tlkmw/tlkmw.h"
#include "logic/app_rc_logic.h"
#include "app_recording_card_api.h"
#include "thread_core/app_rc_thread_msg.h"
#include "power/inc/app_rc_ble_switch.h"
#include <stdio.h>

void app_rc_api_msc_switch(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_MSC_SWITCH, NULL, 0);
}

void app_rc_api_start_record(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_START_RECORD, NULL, 0);
}

void app_rc_api_stop_record(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_STOP_RECORD, NULL, 0);
}

void app_rc_api_enable_alg(uint8_t en)
{
    tlkmdi_recording_card_alg_en(en);
}

void app_rc_api_enable_wav_save(uint8_t en)
{
    app_rc_enable_wav_save(en);
}

void app_rc_api_enable_ble(uint8_t en)
{
    if (en) {
        app_rc_ble_open();
    } else {
        app_rc_ble_close();
    }
}

uint8_t app_rc_api_is_recording(void)
{
    return app_rc_is_recording();
}

uint8_t app_rc_api_is_busy(void)
{
    return app_rc_is_busy();
}

uint8_t app_rc_api_ble_is_enable(void)
{
    return app_rc_is_ble_open();
}

void app_rc_api_report_opus_table(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_REPORT_OPUS, NULL, 0);
}

void app_rc_api_report_wav_table(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_REPORT_WAV, NULL, 0);
}

void app_rc_api_send_opus_file(char *path, uint32_t offset)
{
    uint16_t len = strlen(path);
    uint8_t  buffer[600];
    buffer[0] = offset;
    buffer[1] = offset >> 8;
    buffer[2] = offset >> 16;
    buffer[3] = offset >> 24;
    memcpy(buffer + 4, path, len + 1);
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_SEND_OPUS, buffer, len + 5);
}

void app_rc_api_send_file_by_Wifi(char *path, uint32_t offset)
{
    uint16_t len = strlen(path);
    uint8_t  buffer[600];
    buffer[0] = offset;
    buffer[1] = offset >> 8;
    buffer[2] = offset >> 16;
    buffer[3] = offset >> 24;
    memcpy(buffer + 4, path, len + 1);
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_SEND_WIFI_FILE, buffer, len + 5);
}

void app_rc_api_rename_file(const char *oldPath, const char *newPath)
{
    uint32_t len0      = strlen(oldPath);
    uint32_t len1      = strlen(newPath);
    uint32_t totoalLen = len0 + len1 + 2;
    if (totoalLen > 1024) {
        return;
    }
    uint8_t buffer[1024];
    memcpy(buffer, oldPath, len0);
    buffer[len0] = 0;
    memcpy(buffer + len0 + 1, newPath, len1);
    buffer[totoalLen - 1] = 0;
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_RENAME_FILE, buffer, totoalLen);
}

void app_rc_api_delete_file(const char *path)
{
    uint16_t len = strlen(path);
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_DEL_FILE, (void *)path, len + 1);
}

void app_rc_api_wifi_power_on(uint16_t delayMs)
{
    uint8_t data[2] = {
        [0] = delayMs,
        [1] = delayMs >> 8,
    };
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_WIFI_POWER_ON, data, sizeof(data));
}

void app_rc_api_wifi_power_off(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_WIFI_POWER_OFF, NULL, 0);
}

void app_rc_api_wifi_power_switch(void)
{
    tlksys_sendMsg(APP_RC_THREAD_ID, APP_RC_THREAD_MSG_WIFI_POWER_SWITCH, NULL, 0);
}

void app_rc_debug_api_fake_file(uint32_t size)
{
    TlkOsFileCtrlHandle_t file = tlkmw_fs_mallocFileCtrl();
    if (!file) {
        return;
    }
    char    name[64] = {0};
    uint8_t cache[256];
    for (uint32_t i = 0; i < 256; i++) {
        cache[i] = i;
    }
    snprintf(name, sizeof(name), "fake_size%lu.wav", size);
    if (tlkmw_fs_createNewFile(file, name) == TLK_ENONE) {
        for (uint32_t i = 0; i < size / 256; i++) {
            tlkmw_fs_write(file, cache, 256);
        }
        tlkmw_fs_write(file, cache, size % 256);
    }
    tlkmw_fs_close(file);
    tlkmw_fs_freeFileCtrl(file);
}
