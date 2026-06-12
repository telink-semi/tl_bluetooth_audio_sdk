/********************************************************************************************************
 * @file    app_rc_file_nameMaker.c
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
#include <time.h>
#include <stdio.h>

static const struct tm *app_rc_file_name_get_rtc(void)
{
    return tlkdrv_rtc_getTime(+8);
}

int app_rc_file_name_make_by_rtc(char *pathBuf, uint32_t bufSize, char *suffix)
{
    if (bufSize < 128) {
        return -TLK_ENOMEM;
    }
    const struct tm *time  = app_rc_file_name_get_rtc();
    int              index = 0;
    while (1) {
        int ret = 0;
        if (index == 0) {
            ret = snprintf(pathBuf, bufSize, "0:/rev_Y%04dM%02dD%02d_%02d_%02d_%02d.%s", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min,
                           time->tm_sec, suffix);
        } else {
            ret = snprintf(pathBuf, bufSize, "0:/rev_Y%04dM%02dD%02d_%02d_%02d_%02d(%d).%s", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min,
                           time->tm_sec, index, suffix);
        }
        if (ret < 0) {
            return -TLK_EFAIL;
        }
        bool isExisted = tlkmw_fs_isFileExisted(pathBuf);
        if (!isExisted) {
            break;
        }
        index++;
    }
    return TLK_ENONE;
}
