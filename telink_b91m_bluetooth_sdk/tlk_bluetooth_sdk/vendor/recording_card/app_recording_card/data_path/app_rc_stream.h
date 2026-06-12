/********************************************************************************************************
 * @file    app_rc_stream.h
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

typedef enum
{
    RC_STREAM_TYPE_FILE_WAV_IN = 0,
    RC_STREAM_TYPE_FILE_OPUS_IN,
    RC_STREAM_TYPE_MIC_PCM_OUT,
    RC_STREAM_TYPE_MIC_OPUS_OUT,
    RC_STREAM_TYPE_BLE_OPUS_IN,
    RC_STREAM_TYPE_WIFI_DATA_IN,
    RC_STREAM_TYPE_FILE_OPUS_OUT,
    RC_STREAM_TYPE_NUM
} rc_stream_type_e;

typedef void (*rc_stream_data_rdy_cb_t)(void);

typedef struct
{
    void *arg0;
    void *arg1;
    void *arg2;
} app_rc_stream_cfg_t;

typedef struct
{
    void (*init)(void);
    int (*open)(void *cfg);
    int (*write)(void *pData, uint32_t len);
    int (*read)(void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen);
    void (*close)(void);
} app_rc_stream_t;

void app_rc_stream_initAll(void);

int app_rc_stream_open(uint32_t streamType);

int app_rc_stream_openEx(uint32_t streamType, void *cfg);

int app_rc_stream_write(uint32_t streamType, void *pData, uint32_t len);

int app_rc_stream_read(uint32_t streamType, void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen);

void app_rc_stream_close(uint32_t streamType);