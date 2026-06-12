/********************************************************************************************************
 * @file    app_rc_logic.h
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

void app_rc_logic_init(void);

void app_rc_msc_switch(void);

void app_rc_start_record(void);

void app_rc_stop_record(void);

void app_rc_enable_wav_save(uint8_t en);

uint8_t app_rc_is_recording(void);

uint8_t app_rc_is_busy(void);

void app_rc_report_opus_table(void);

void app_rc_report_wav_table(void);

void app_rc_send_ops_file(char *path, uint32_t offset);

void app_rc_send_file_by_Wifi(char *path, uint32_t offset);

void app_rc_rename_file(const char *oldPath, const char *newPath);

void app_rc_delete_file(const char *file);

void app_rc_wifi_power_on(uint16_t delayMs);

void app_rc_wifi_power_off(void);

void app_rc_wifi_power_switch(void);