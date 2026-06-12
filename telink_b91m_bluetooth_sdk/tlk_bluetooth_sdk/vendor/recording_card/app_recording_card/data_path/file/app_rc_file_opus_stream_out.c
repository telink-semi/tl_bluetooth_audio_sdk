/********************************************************************************************************
 * @file    app_rc_file_opus_stream_out.c
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
#include "tlkmw/tlkmw.h"
#include "../app_rc_stream.h"
#include "drv/app_rc_file_cache.h"

typedef struct
{
    TlkOsFileCtrlHandle_t file;
    app_rc_file_cache_t  *cache;
} opusOutCtrl_t;

static opusOutCtrl_t sOpusOutCtrl = {0};

static int app_rc_file_opus_stream_out_read_core(void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen)
{
    return tlkmw_fs_read(sOpusOutCtrl.file, pBuffer, reqReadLen, relReadLen);
}

static int app_rc_file_opus_stream_out_open(void *cfg)
{
#if TLK_CFG_FS_ENABLE
    if (sOpusOutCtrl.file) {
        return -TLK_EREPEAT;
    }
    void    *path              = ((app_rc_stream_cfg_t *)cfg)->arg0;
    uint32_t isUseCache        = (uint32_t)((app_rc_stream_cfg_t *)cfg)->arg1;
    uint32_t file_offset_value = (uint32_t)((app_rc_stream_cfg_t *)cfg)->arg2;
    if (path == NULL) {
        return -TLK_EPARAM;
    }
    sOpusOutCtrl.file = tlkmw_fs_mallocFileCtrl();
    int ret           = tlkmw_fs_openForRead(sOpusOutCtrl.file, (char *)path);
    if (ret != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sOpusOutCtrl.file);
        sOpusOutCtrl.file = NULL;
        return ret;
    }
    ret = tlkmw_fs_seek(sOpusOutCtrl.file, file_offset_value);
    if (ret != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sOpusOutCtrl.file);
        sOpusOutCtrl.file = NULL;
        return ret;
    }
    if (isUseCache) {
        sOpusOutCtrl.cache = app_rc_file_cache_create(100 * 1024, NULL, app_rc_file_opus_stream_out_read_core);
    }

    return ret;
#else
    (void)cfg;
    return -TLK_ENOSUPPORT;
#endif
}

static int app_rc_file_opus_stream_out_read(void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen)
{
    if (sOpusOutCtrl.file == NULL) {
        return -TLK_EFAIL;
    }
    if (sOpusOutCtrl.cache) {
        return app_rc_file_cache_read(sOpusOutCtrl.cache, pBuffer, reqReadLen, relReadLen);
    }
    return app_rc_file_opus_stream_out_read_core(pBuffer, reqReadLen, relReadLen);
}

static void app_rc_file_opus_stream_out_close(void)
{
#if TLK_CFG_FS_ENABLE
    if (sOpusOutCtrl.file == NULL) {
        return;
    }
    app_rc_file_cache_destroy(sOpusOutCtrl.cache);
    sOpusOutCtrl.cache = NULL;
    tlkmw_fs_close(sOpusOutCtrl.file);
    tlkmw_fs_freeFileCtrl(sOpusOutCtrl.file);
    sOpusOutCtrl.file = NULL;
#endif
}

const app_rc_stream_t app_rc_file_opus_stream_out = {
    .open  = app_rc_file_opus_stream_out_open,
    .close = app_rc_file_opus_stream_out_close,
    .read  = app_rc_file_opus_stream_out_read,
};
