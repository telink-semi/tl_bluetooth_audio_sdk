/********************************************************************************************************
 * @file    app_ble_command.c
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
#include "stack/ble/ble.h"

#include "app_ble_server.h"

static char *file_table[] = {
    "0:/record_01.wav -15678 B -350 s",   "0:/record_02.wav -89234 B -1200 s",  "0:/record_03.wav -3456 B -80 s",     "0:/record_04.wav -120987 B -1800 s",
    "0:/record_05.wav -56789 B -750 s",   "0:/record_06.wav -19876 B -280 s",   "0:/record_07.wav -98765 B -1400 s",  "0:/record_08.wav -2345 B -30 s",
    "0:/record_09.wav -102345 B -1500 s", "0:/record_10.wav -67890 B -900 s",   "0:/record_11.wav -12345 B -180 s",   "0:/record_12.wav -87654 B -1250 s",
    "0:/record_13.wav -4567 B -60 s",     "0:/record_14.wav -111234 B -1600 s", "0:/record_15.wav -54321 B -700 s",   "0:/record_16.wav -22345 B -320 s",
    "0:/record_17.wav -76543 B -1100 s",  "0:/record_18.wav -34567 B -480 s",   "0:/record_19.wav -134567 B -2000 s", "0:/record_20.wav -65432 B -950 s",
    "0:/record_21.wav -15432 B -220 s",   "0:/record_22.wav -90123 B -1300 s",  "0:/record_23.wav -5678 B -75 s",     "0:/record_24.wav -123456 B -1700 s",
    "0:/record_25.wav -67890 B -920 s",   "0:/record_26.wav -23456 B -330 s",   "0:/record_27.wav -87654 B -1280 s",  "0:/record_28.wav -45678 B -650 s",
    "0:/record_29.wav -145678 B -2100 s", "0:/record_30.wav -76543 B -1100 s",  "0:/record_31.wav -16543 B -240 s",   "0:/record_32.wav -98765 B -1450 s",
    "0:/record_33.wav -6789 B -90 s",     "0:/record_34.wav -134567 B -1900 s", "0:/record_35.wav -87654 B -1250 s",  "0:/record_36.wav -24567 B -350 s",
    "0:/record_37.wav -109876 B -1600 s", "0:/record_38.wav -56789 B -800 s",   "0:/record_39.wav -17654 B -250 s",   "0:/record_40.wav -112345 B -1650 s",
    "0:/record_41.wav -7654 B -100 s",    "0:/record_42.wav -123456 B -1800 s", "0:/record_43.wav -65432 B -930 s",   "0:/record_44.wav -25678 B -360 s",
    "0:/record_45.wav -98765 B -1400 s",  "0:/record_46.wav -54321 B -750 s",   "0:/record_47.wav -18765 B -270 s",   "0:/record_48.wav -134567 B -1950 s",
    "0:/record_49.wav -87654 B -1260 s",  "0:/record_50.wav -34567 B -490 s",   "0:/record_51.wav -19876 B -280 s",   "0:/record_52.wav -109876 B -1550 s",
    "0:/record_53.wav -76543 B -1100 s",  "0:/record_54.wav -45678 B -630 s",   "0:/record_55.wav -20987 B -290 s",   "0:/record_56.wav -112345 B -1600 s",
    "0:/record_57.wav -87654 B -1250 s",  "0:/record_58.wav -34567 B -480 s",   "0:/record_59.wav -21345 B -300 s",   "0:/record_60.wav -134567 B -2000 s",
    "0:/record_61.wav -98765 B -1420 s",  "0:/record_62.wav -56789 B -800 s",   "0:/record_63.wav -22456 B -310 s",   "0:/record_64.wav -123456 B -1750 s",
    "0:/record_65.wav -67890 B -950 s",   "0:/record_66.wav -45678 B -650 s",   "0:/record_67.wav -156789 B -2200 s", "0:/record_68.wav -76543 B -1100 s",
    "0:/record_69.wav -23456 B -330 s",   "0:/record_70.wav -109876 B -1580 s", "0:/record_71.wav -87654 B -1280 s",  "0:/record_72.wav -56789 B -820 s",
    "0:/record_73.wav -24567 B -340 s",   "0:/record_74.wav -145678 B -2100 s", "0:/record_75.wav -98765 B -1450 s",  "0:/record_76.wav -67890 B -980 s",
    "0:/record_77.wav -34567 B -490 s",   "0:/record_78.wav -123456 B -1700 s", "0:/record_79.wav -76543 B -1120 s",  "0:/record_80.wav -25678 B -350 s",
    "0:/record_81.wav -134567 B -1900 s", "0:/record_82.wav -87654 B -1250 s",  "0:/record_83.wav -56789 B -800 s",   "0:/record_84.wav -26789 B -370 s",
    "0:/record_85.wav -112345 B -1600 s", "0:/record_86.wav -98765 B -1400 s",  "0:/record_87.wav -67890 B -950 s",   "0:/record_88.wav -34567 B -480 s",
    "0:/record_89.wav -145678 B -2100 s", "0:/record_90.wav -76543 B -1100 s",  "0:/record_91.wav -27890 B -380 s",   "0:/record_92.wav -123456 B -1700 s",
    "0:/record_93.wav -87654 B -1250 s",  "0:/record_94.wav -56789 B -820 s",   "0:/record_95.wav -28901 B -390 s",   "0:/record_96.wav -156789 B -2200 s",
    "0:/record_97.wav -98765 B -1450 s",  "0:/record_98.wav -67890 B -980 s",   "0:/record_99.wav -34567 B -490 s",   "0:/record_100.wav -134567 B -1900 s"};

static uint8_t file_table_idx = 0;

static void app_ble_report_file_info_callback(uint16_t conn_handle, bool success)
{
    (void)conn_handle;
    if (success) {
        file_table_idx++;
        if (file_table_idx < ARRAY_SIZE(file_table)) {
            app_ble_send_file_info(file_table[file_table_idx], strlen(file_table[file_table_idx]), app_ble_report_file_info_callback);
        } else {
            uint8_t storage_size_ok[] = {0xFF, 0x55, 0xAA, 0x00, 0x2f};
            app_ble_notify_value(storage_size_ok, sizeof(storage_size_ok));
        }
    }
}

void app_ble_search_file_table(uint8_t *value, uint8_t value_len)
{
    file_table_idx = 0;
    if (value_len == 0) {
        tlk_printf("search files tables");
    } else if (memcmp(value, "0:/ .wav", 8) == 0) {
        tlk_printf("find file wav");
    } else if (memcmp(value, "1:/ .pcm", 8) == 0) {
        tlk_printf("find file pcm");
    }

    app_ble_send_file_info(file_table[file_table_idx], strlen(file_table[file_table_idx]), app_ble_report_file_info_callback);
}

int app_ble_delete_file(uint8_t *value, uint8_t value_len)
{
    tlk_printf("delete file[Len:%d], %s", value_len, value);
    return 0;
}

uint32_t app_ble_send_file(uint8_t *value, uint8_t value_len)
{
    tlk_printf("send file[Len:%d], %s", value_len, value);
    return 20484;
}

void app_ble_start_send_file(void)
{
    tlk_printf("start send file");
    app_ble_ack_start_send_file_ok();
}
