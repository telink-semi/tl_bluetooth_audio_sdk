/********************************************************************************************************
 * @file    app_rc_file_opus_stream_in.c
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

typedef struct
{
    TlkOsFileCtrlHandle_t file;
    app_rc_file_cache_t  *cache;
} opusInCtrl_t;

#if TLK_CFG_FS_ENABLE
static opusInCtrl_t sOpusInCtrl = {0};

static int app_rc_file_opus_stream_in_write_core(void *pData, uint32_t len)
{
    return tlkmw_fs_write(sOpusInCtrl.file, pData, len);
}
#endif

static int app_rc_file_opus_stream_in_open(void *cfg)
{
    (void)cfg;
#if TLK_CFG_FS_ENABLE
    if (sOpusInCtrl.file) {
        return -TLK_EREPEAT;
    }
    tlkmw_fs_allow_sleep(0);
    sOpusInCtrl.file = tlkmw_fs_mallocFileCtrl();
    char path[256];
    app_rc_file_name_make_by_rtc(path, sizeof(path), "ops");
    int ret = tlkmw_fs_createNewFile(sOpusInCtrl.file, path);
    if (ret != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sOpusInCtrl.file);
        sOpusInCtrl.file = NULL;
    } else {
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X)
        sOpusInCtrl.cache = app_rc_file_cache_create(20 * 1024, app_rc_file_opus_stream_in_write_core, NULL);
#else // TL721X RAM NOT ENOUGH
        sOpusInCtrl.cache = app_rc_file_cache_create(5 * 1024, app_rc_file_opus_stream_in_write_core, NULL);
#endif
    }

    return ret;
#else
    return -1;
#endif
}

static int app_rc_file_opus_stream_in_write(void *pData, uint32_t len)
{
#if TLK_CFG_FS_ENABLE
    if (sOpusInCtrl.file == NULL) {
        return -TLK_EFAIL;
    }
    if (sOpusInCtrl.cache) {
        return app_rc_file_cache_write(sOpusInCtrl.cache, pData, len);
    }
    return app_rc_file_opus_stream_in_write_core(pData, len);
#else
    (void)pData;
    (void)len;
    return -1;
#endif
}

static void app_rc_file_opus_stream_in_close(void)
{
#if TLK_CFG_FS_ENABLE
    if (sOpusInCtrl.file == NULL) {
        return;
    }
    tlkmw_fs_allow_sleep(1);
    app_rc_file_cache_destroy(sOpusInCtrl.cache);
    sOpusInCtrl.cache = NULL;
    tlkmw_fs_close(sOpusInCtrl.file);
    tlkmw_fs_freeFileCtrl(sOpusInCtrl.file);
    sOpusInCtrl.file = NULL;
#endif
}

const app_rc_stream_t app_rc_file_opus_stream_in = {
    .open  = app_rc_file_opus_stream_in_open,
    .close = app_rc_file_opus_stream_in_close,
    .write = app_rc_file_opus_stream_in_write,
};
