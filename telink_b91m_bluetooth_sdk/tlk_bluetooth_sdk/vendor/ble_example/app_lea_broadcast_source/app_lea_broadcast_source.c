/********************************************************************************************************
 * @file    app_lea_broadcast_source.c
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
#include <stdio.h>
#include "stack/ble/ble.h"
#include "driver.h"
#include "../app_example.h"

#include "tlkmw/ble/le_audio/inc/lea_broadcast_source.h"
#include "tlkmw/sys_dev/key/tlkdrv_key.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"

static const uint16_t lea_broadcast_source_key_cfg[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {

            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_2,
        },
#endif
};
static void lea_broadcast_source_opened(void)
{
    tlkapi_printf(APP_LOG_EN, "[APP][INI] lea broadcast source opened");
}

static void lea_broadcast_source_closed(void)
{
    tlkapi_printf(APP_LOG_EN, "[APP][INI] lea broadcast source closed");
}

static void lea_broadcast_source_press_open(void)
{
    struct lea_broadcast_source_param param = {
        .device_name = "lea_broadcast_source",
    };
    lea_broadcast_source_open(&param, lea_broadcast_source_opened);
}

static void lea_broadcast_source_press_close(void)
{
    lea_broadcast_source_close(lea_broadcast_source_closed);
}

int INIT(APP_BLE_LEA_BROADCAST_SOURCE)(void)
{
    struct lea_broadcast_source_param param = {
        .device_name = "tlk_lea_broadcast_source",
    };
    lea_broadcast_source_open(&param, lea_broadcast_source_opened);
    tlkdrv_key_registerVendorConfig1Callback(lea_broadcast_source_press_open);
    tlkdrv_key_registerVendorConfig2Callback(lea_broadcast_source_press_close);
    tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)lea_broadcast_source_key_cfg);
    return 0;
}

void START(APP_BLE_LEA_BROADCAST_SOURCE)(void) {}
