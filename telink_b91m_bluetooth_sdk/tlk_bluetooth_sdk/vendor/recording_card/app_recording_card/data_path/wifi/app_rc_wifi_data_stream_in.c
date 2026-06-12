/********************************************************************************************************
 * @file    app_rc_wifi_data_stream_in.c
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
#include "tlkapi/tlkapi.h"
#include "../app_rc_stream.h"
#include "app_rc_wifi_api.h"
#include "../../../app_recording_card/app_recording_card_api.h"

typedef void (*rc_wifi_data_stream_in_open_cb_t)(void);

typedef struct
{
    uint8_t                          isOpen;
    uint8_t                          isClosing;
    uint8_t                          resv[2];
    rc_wifi_spi_send_finished_cb_t   sendCb;
    rc_wifi_data_stream_in_open_cb_t openCb;
} wifiDataInCtrl_t;

static wifiDataInCtrl_t app_rc_wifi_data_stream_ctrl = {0};

static void app_rc_wifi_data_stream_close_spi_cmd_cb(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen)
{
    (void)isSuccess;
    (void)pAckData;
    (void)dataLen;
    if (app_rc_wifi_data_stream_ctrl.isClosing == 0) {
        return;
    }
    app_rc_wifi_data_stream_ctrl.isClosing = 0;
    if (app_rc_wifi_data_stream_ctrl.isOpen && app_rc_wifi_data_stream_ctrl.openCb) {
        app_rc_wifi_data_stream_ctrl.openCb();
    }
}

static void app_rc_wifi_data_stream_in_init(void)
{
    app_rc_wifi_spi_init();
}

static int app_rc_wifi_data_stream_in_open(void *cfg)
{
    if (app_rc_wifi_data_stream_ctrl.isOpen) {
        return -TLK_EREPEAT;
    }
    app_rc_wifi_data_stream_ctrl.isOpen = 1;
    app_rc_stream_cfg_t *pFunc          = (app_rc_stream_cfg_t *)cfg;
    app_rc_wifi_data_stream_ctrl.openCb = (rc_wifi_data_stream_in_open_cb_t)pFunc->arg0;
    app_rc_wifi_data_stream_ctrl.sendCb = (rc_wifi_spi_send_finished_cb_t)pFunc->arg1;
    if (app_rc_wifi_data_stream_ctrl.isClosing == 0 && app_rc_wifi_data_stream_ctrl.openCb) {
        app_rc_wifi_data_stream_ctrl.openCb();
    }
    return TLK_ENONE;
}

static int app_rc_wifi_data_stream_in_write(void *pData, uint32_t len)
{
    if (!app_rc_wifi_data_stream_ctrl.isOpen || app_rc_wifi_data_stream_ctrl.isClosing) {
        return -TLK_EFAIL;
    }
    return app_rc_wifi_spi_send_data((uint8_t *)pData, len, app_rc_wifi_data_stream_ctrl.sendCb);
}

static void app_rc_wifi_data_stream_in_close(void)
{
    if (!app_rc_wifi_data_stream_ctrl.isOpen) {
        return;
    }
    app_rc_wifi_data_stream_ctrl.isOpen    = 0;
    app_rc_wifi_data_stream_ctrl.isClosing = 1;
    app_rc_wifi_spi_cleanFrameNum();
    app_rc_wifi_data_stream_ctrl.sendCb = NULL;
    app_rc_wifi_data_stream_ctrl.openCb = NULL;
    app_rc_wifi_spi_send_cmd(WIFI_SPI_CMD_STOP_SEND_DATA, NULL, 0, app_rc_wifi_data_stream_close_spi_cmd_cb);
}

const app_rc_stream_t app_rc_wifi_data_stream_in = {
    .init  = app_rc_wifi_data_stream_in_init,
    .open  = app_rc_wifi_data_stream_in_open,
    .close = app_rc_wifi_data_stream_in_close,
    .write = app_rc_wifi_data_stream_in_write,
};
