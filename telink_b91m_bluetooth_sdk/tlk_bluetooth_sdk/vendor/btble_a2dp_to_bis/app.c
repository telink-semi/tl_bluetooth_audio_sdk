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
#include "tlklib/usb/tlkusb.h"
#include "tlklib/usb/udb/tlkusb_udb.h"

#include "stack/btble.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkapp/tlkapp.h"
#include "tlkmw/tlkmw.h"
#include "app_bt.h"
#include "app_ble.h"
#include "stack/controller.h"
#include "app_ui.h"
#include "app_ble_bis.h"

/**
 * @brief       USB debug shell hook function
 * @param[in]   pData - Pointer to received data
 * @param[in]   dataLen - Length of received data
 * @return      none
 * @note        This function handles USB debug commands, particularly for switching
 *              between A2DP to BIS and BT music audio scenes
 */
__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_send_string_data(APP_LOG_EN, "tlkusb_debug_shell_hook: received", pData, dataLen);
    if (pData[0] != 0x11) {
        return;
    }
    if (pData[1] == 0x01) //BT
    {
        switch (pData[2]) {
        case 0x09:
            tlkapp_audioScheduler_debug();
            break;
        case 0x10:
            tlkapi_printf(APP_LOG_EN, "tlkapp_audioScheduler_roundRobin");
            tlkapp_audioScheduler_roundRobin();
            if (tlkmdi_a2dp_to_bis_get_audio_scene() == TLKMDI_AUDIO_PATH_A2DP2BIS) {
                tlkapi_printf(APP_LOG_EN, "switch out a2dp to bt_music");
                tlkmdi_a2dp_to_bis_set_audio_scene(TLKMDI_AUDIO_PATH_BTMUSIC);
                app_ble_bis_source_close(NULL);
                //add open 2.4G
            } else if (tlkmdi_a2dp_to_bis_get_audio_scene() == TLKMDI_AUDIO_PATH_BTMUSIC) {
                tlkapi_printf(APP_LOG_EN, "switch out bt_music to a2dp");
                tlkmdi_a2dp_to_bis_set_audio_scene(TLKMDI_AUDIO_PATH_A2DP2BIS);
                app_ble_bis_source_open(NULL);
                //add close 2.4G
            }
            break;
        default:
            break;
        }
    }
}