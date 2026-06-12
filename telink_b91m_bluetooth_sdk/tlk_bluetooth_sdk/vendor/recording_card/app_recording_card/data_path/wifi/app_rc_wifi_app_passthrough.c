/********************************************************************************************************
 * @file    app_rc_wifi_app_passthrough.c
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
#include "../../app_recording_card_api.h"
#include "app_rc_wifi_api.h"

typedef enum
{
    APP_RC_CMD_DELETE_FILE          = 0x24,
    APP_RC_CMD_RENAME_FILE          = 0x25,
    APP_RC_CMD_READ_FILE_BY_WIFI_EX = 0x41,
} app_rc_uart_cmd;

int app_tlk_record_report_event_by_other_way(uint8_t *value, uint16_t value_len)
{
    return app_rc_wifi_uart_sendData(value, value_len);
}

void app_rc_wifi_uart_delete_file(uint8_t *value, uint8_t value_len)
{
    (void)value_len;
    char file_name[256] = {0};
    memcpy(file_name, value, value_len);
    tlk_printf("delete file:%s", file_name);

    app_rc_api_delete_file(file_name);
}

/**
 * @brief       This function renames a file from old_file_name to new_file_name.
 * @param[in]   old_file_name - the old file name.
 * @param[in]   old_file_name_len - the length of the old file name.
 * @param[in]   new_file_name - the new file name.
 * @param[in]   new_file_name_len - the length of the new file name.
 * @return      none.
 * @note    
 */
void app_rc_wifi_uart_rename_file(uint8_t *old_file_name, uint8_t old_file_name_len, uint8_t *new_file_name, uint8_t new_file_name_len)
{
    char file_name_old[256] = {0};
    memcpy(file_name_old, old_file_name, old_file_name_len);
    char file_name_new[256] = {0};
    memcpy(file_name_new, new_file_name, new_file_name_len);

    tlk_printf("rename file:%s to %s", file_name_old, file_name_new);
    app_rc_api_rename_file(file_name_old, file_name_new);
}

/**
 * @brief       This function reads a file and sends its content over WiFi.
 * @param[in]   value   - the file name.
 * @param[in]   value_len - the length of the file name.
 * @return      none.
 * @note    
 */
void app_rc_wifi_uart_read_file_wifi(uint8_t *value, uint8_t value_len)
{
    uint32_t offset         = (uint32_t)value[0] | ((uint32_t)value[1] << 8) | ((uint32_t)value[2] << 16) | ((uint32_t)value[3] << 24);
    char     file_name[256] = {0};
    memcpy(file_name, value + 4, value_len - 4);
    tlk_printf("app_rc_wifi_uart_read_file_wifi value = %s, value_len = %d\n", file_name, value_len);
    app_rc_api_send_file_by_Wifi((char *)file_name, offset);
}

void app_rc_wifi_uart_getDataDeal(uint8_t *value, uint16_t value_len)
{
    tlkapi_array(0xFFFFFFFF, "[WIFI]", "app_rc_wifi_uart_getDataDeal:", value, value_len);
    if (value[0] != 0x11) {
        return;
    }
    switch (value[1]) {
    case APP_RC_CMD_DELETE_FILE:
        app_rc_wifi_uart_delete_file(value + 2, value_len - 2);
        break;
    case APP_RC_CMD_RENAME_FILE:
        uint8_t  old_file_len = value[2];
        uint8_t *old_file     = value + 3;
        uint8_t  new_file_len = value[3 + old_file_len];
        uint8_t *new_file     = value + 4 + old_file_len;
        app_rc_wifi_uart_rename_file(old_file, old_file_len, new_file, new_file_len);
        break;
    case APP_RC_CMD_READ_FILE_BY_WIFI_EX:
        app_rc_wifi_uart_read_file_wifi(value + 2, value_len - 2);
        break;
    default:
        break;
    }
}
