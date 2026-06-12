/********************************************************************************************************
 * @file    app_rc_ble_opus_stream_in.c
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
#include "../app_rc_stream.h"

typedef struct
{
    uint8_t isOpen;
} bleOpusInCtrl_t;

static bleOpusInCtrl_t sBleOpusInCtrl = {0};

static int app_rc_ble_opus_stream_in_open(void *cfg)
{
    (void)cfg;
    if (sBleOpusInCtrl.isOpen) {
        return -TLK_EREPEAT;
    }
    sBleOpusInCtrl.isOpen = 1;
    return TLK_ENONE;
}

static int app_rc_ble_opus_stream_in_write(void *pData, uint32_t len)
{
    extern int app_tlk_record_report_data(uint8_t * value, uint16_t value_len);
    int        ret = app_tlk_record_report_data((uint8_t *)pData, len);
    if (ret != 0) {
        tlkapi_error(0xFFFFFFFF, "[BLE]", "app_tlk_record_report_data fail %d", ret);
    }
    return ret == 0 ? TLK_ENONE : -TLK_EFAIL;
}

static void app_rc_ble_opus_stream_in_close(void)
{
    if (sBleOpusInCtrl.isOpen == 0) {
        return;
    }
    sBleOpusInCtrl.isOpen = 0;
}

const app_rc_stream_t app_rc_ble_opus_stream_in = {
    .open  = app_rc_ble_opus_stream_in_open,
    .close = app_rc_ble_opus_stream_in_close,
    .write = app_rc_ble_opus_stream_in_write,
};
