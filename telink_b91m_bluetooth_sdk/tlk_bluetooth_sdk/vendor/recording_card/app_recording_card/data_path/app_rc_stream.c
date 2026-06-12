/********************************************************************************************************
 * @file    app_rc_stream.c
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
#include "app_rc_stream.h"

typedef const app_rc_stream_t *p_rc_stream_ctrl_t;

extern const app_rc_stream_t app_rc_file_wav_stream_in;
extern const app_rc_stream_t app_rc_file_opus_stream_in;
extern const app_rc_stream_t app_rc_mic_pcm_stream_out;
extern const app_rc_stream_t app_rc_mic_opus_stream_out;
extern const app_rc_stream_t app_rc_ble_opus_stream_in;
extern const app_rc_stream_t app_rc_wifi_data_stream_in;
extern const app_rc_stream_t app_rc_file_opus_stream_out;

static const p_rc_stream_ctrl_t sp_app_rc_stream_tab[RC_STREAM_TYPE_NUM] = {
#if TLK_CFG_RC_WAV_SAVE
    [RC_STREAM_TYPE_FILE_WAV_IN] = &app_rc_file_wav_stream_in,
#endif
    [RC_STREAM_TYPE_FILE_OPUS_IN] = &app_rc_file_opus_stream_in,
#if TLK_CFG_RC_WAV_SAVE
    [RC_STREAM_TYPE_MIC_PCM_OUT] = &app_rc_mic_pcm_stream_out,
#endif
    [RC_STREAM_TYPE_MIC_OPUS_OUT] = &app_rc_mic_opus_stream_out,   [RC_STREAM_TYPE_BLE_OPUS_IN] = &app_rc_ble_opus_stream_in,
#if TLK_CFG_WIFI_LOGIC_ENABLE
    [RC_STREAM_TYPE_WIFI_DATA_IN] = &app_rc_wifi_data_stream_in,
#endif
    [RC_STREAM_TYPE_FILE_OPUS_OUT] = &app_rc_file_opus_stream_out,
};

void app_rc_stream_initAll(void)
{
    for (uint32_t i = 0; i < RC_STREAM_TYPE_NUM; i++) {
        if (sp_app_rc_stream_tab[i] == NULL || sp_app_rc_stream_tab[i]->init == NULL) {
            continue;
        }
        sp_app_rc_stream_tab[i]->init();
    }
}

int app_rc_stream_openEx(uint32_t streamType, void *cfg)
{
    if (streamType >= RC_STREAM_TYPE_NUM || sp_app_rc_stream_tab[streamType] == NULL) {
        return -TLK_EFAIL;
    }
    if (sp_app_rc_stream_tab[streamType]->open == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sp_app_rc_stream_tab[streamType]->open(cfg);
}

int app_rc_stream_open(uint32_t streamType)
{
    return app_rc_stream_openEx(streamType, NULL);
}

int app_rc_stream_write(uint32_t streamType, void *pData, uint32_t len)
{
    if (streamType >= RC_STREAM_TYPE_NUM || sp_app_rc_stream_tab[streamType] == NULL) {
        return -TLK_EFAIL;
    }
    if (sp_app_rc_stream_tab[streamType]->write == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sp_app_rc_stream_tab[streamType]->write(pData, len);
}

int app_rc_stream_read(uint32_t streamType, void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen)
{
    if (streamType >= RC_STREAM_TYPE_NUM || sp_app_rc_stream_tab[streamType] == NULL) {
        return -TLK_EFAIL;
    }
    if (sp_app_rc_stream_tab[streamType]->read == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sp_app_rc_stream_tab[streamType]->read(pBuffer, reqReadLen, relReadLen);
}

void app_rc_stream_close(uint32_t streamType)
{
    if (streamType >= RC_STREAM_TYPE_NUM || sp_app_rc_stream_tab[streamType] == NULL) {
        return;
    }
    if (sp_app_rc_stream_tab[streamType]->close == NULL) {
        return;
    }
    return sp_app_rc_stream_tab[streamType]->close();
}
