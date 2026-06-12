/********************************************************************************************************
 * @file    app_rc_file_wav_stream_in.c
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

#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "../app_rc_stream.h"
#include "drv/app_rc_file_cache.h"
#include "drv/app_rc_file_nameMaker.h"

#ifndef APP_RC_FILE_STREAM_WAV_CACHE_SIZE
#define APP_RC_FILE_STREAM_WAV_CACHE_SIZE (95 * 1024)
#endif

#define STREAM_UINT32H_TO_ARRAY(value, array, offset)   \
    (array)[(offset) + 0] = ((value) & 0xFF);           \
    (array)[(offset) + 1] = ((value) & 0xFF00) >> 8;    \
    (array)[(offset) + 2] = ((value) & 0xFF0000) >> 16; \
    (array)[(offset) + 3] = ((value) & 0xFF000000) >> 24;

typedef struct
{
    TlkOsFileCtrlHandle_t file;
    app_rc_file_cache_t  *cache;
    uint32_t              revTotalLen;
} wavInCtrl_t;

/*
----------------------------------------------------------------
| 偏移量 | 大小（字节） | 字段名称       | 描述                           |
----------------------------------------------------------------
| 0      | 4           | ChunkID       | 固定为"RIFF" (ASCII: 0x52494646) |
| 4      | 4           | ChunkSize     | 文件总大小 - 8 (字节)            |
| 8      | 4           | Format        | 固定为"WAVE" (ASCII: 0x57415645) |
----------------------------------------------------------------
| 12     | 4           | Subchunk1ID   | 固定为"fmt " (ASCII: 0x666D7420) |
| 16     | 4           | Subchunk1Size | 通常为16 (PCM格式)               |
| 20     | 2           | AudioFormat   | 音频格式，1表示PCM               |
| 22     | 2           | NumChannels   | 声道数：1=单声道，2=立体声        |
| 24     | 4           | SampleRate    | 采样率，如44100Hz                |
| 28     | 4           | ByteRate      | 每秒数据字节数 = SampleRate * NumChannels * BitsPerSample/8 |
| 32     | 2           | BlockAlign    | 块对齐 = NumChannels * BitsPerSample/8 |
| 34     | 2           | BitsPerSample | 每个样本的位数，如8、16、24、32   |
----------------------------------------------------------------
| 36     | 4           | Subchunk2ID   | 固定为"data" (ASCII: 0x64617461) |
| 40     | 4           | Subchunk2Size | 音频数据大小 (字节)               |
| 44     | ...         | 音频数据      | 原始PCM数据                      |
----------------------------------------------------------------
*/
uint8_t sWavHeadFormat[44] = {
    // RIFF
    0x52, 0x49, 0x46, 0x46,
    // Size
    0x00, 0x00, 0x00, 0x00,
    // WAVE
    0x57, 0x41, 0x56, 0x45,
    // fmt
    0x66, 0x6D, 0x74, 0x20,
    // PCM:16
    0x10, 0x00, 0x00, 0x00,
    // 音频格式：PCM
    0x01, 0x00,
    // 声道数：1
    0x01, 0x00,
    // 采样率：16000
    0x80, 0x3E, 0x00, 0x00,
    // 每秒数据字节数 = SampleRate * NumChannels * BitsPerSample/8
    0x00, 0x7D, 0x00, 0x00,
    // BlockAlign = NumChannels * BitsPerSample/8
    0x02, 0x00,
    // 每个样本的位数，如8、16、24、32
    0x10, 0x00,
    // data
    0x64, 0x61, 0x74, 0x61,
    // Size
    0x00, 0x00, 0x00, 0x00};
#if TLK_CFG_FS_ENABLE
static wavInCtrl_t sWavInCtrl = {0};

static int app_rc_file_wav_stream_in_write_core(void *pData, uint32_t len)
{
    int ret = tlkmw_fs_write(sWavInCtrl.file, pData, len);
    if (ret == TLK_ENONE) {
        sWavInCtrl.revTotalLen += len;
    }
    return ret;
}
#endif

static int app_rc_file_wav_stream_in_open(void *cfg)
{
    (void)cfg;
#if TLK_CFG_FS_ENABLE
    if (sWavInCtrl.file) {
        return -TLK_EREPEAT;
    }
    sWavInCtrl.file = tlkmw_fs_mallocFileCtrl();
    char path[256];
    app_rc_file_name_make_by_rtc(path, sizeof(path), "wav");
    int ret = tlkmw_fs_createNewFile(sWavInCtrl.file, path);
    if (ret != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sWavInCtrl.file);
        sWavInCtrl.file = NULL;
    } else {
        sWavInCtrl.cache = app_rc_file_cache_create(APP_RC_FILE_STREAM_WAV_CACHE_SIZE, app_rc_file_wav_stream_in_write_core, NULL);
    }
    return ret;
#else
    return -TLK_EREPEAT;
#endif
}

static int app_rc_file_wav_stream_in_write(void *pData, uint32_t len)
{
#if TLK_CFG_FS_ENABLE
    if (sWavInCtrl.file == NULL) {
        return -TLK_EFAIL;
    }
    if (sWavInCtrl.cache) {
        return app_rc_file_cache_write(sWavInCtrl.cache, pData, len);
    }
    return app_rc_file_wav_stream_in_write_core(pData, len);
#else
    (void)pData;
    (void)len;
    return -TLK_EREPEAT;
#endif
}

static void app_rc_file_wav_stream_in_close(void)
{
#if TLK_CFG_FS_ENABLE
    if (sWavInCtrl.file == NULL) {
        return;
    }
    app_rc_file_cache_destroy(sWavInCtrl.cache);
    sWavInCtrl.cache = NULL;
    int ret          = tlkmw_fs_seek(sWavInCtrl.file, 0);
    if (ret != TLK_ENONE) {
        tlkmw_fs_close(sWavInCtrl.file);
        tlkmw_fs_freeFileCtrl(sWavInCtrl.file);
        sWavInCtrl.file = NULL;
        return;
    }
    STREAM_UINT32H_TO_ARRAY(sWavInCtrl.revTotalLen + 36, sWavHeadFormat, 4);
    STREAM_UINT32H_TO_ARRAY(sWavInCtrl.revTotalLen, sWavHeadFormat, 40);

    tlkmw_fs_write(sWavInCtrl.file, sWavHeadFormat, 44); //TODO
    tlkmw_fs_close(sWavInCtrl.file);
    tlkmw_fs_freeFileCtrl(sWavInCtrl.file);
    sWavInCtrl.file = NULL;
#endif
}

const app_rc_stream_t app_rc_file_wav_stream_in = {
    .open  = app_rc_file_wav_stream_in_open,
    .close = app_rc_file_wav_stream_in_close,
    .write = app_rc_file_wav_stream_in_write,
};
