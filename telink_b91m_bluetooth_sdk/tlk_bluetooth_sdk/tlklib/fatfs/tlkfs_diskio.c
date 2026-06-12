/********************************************************************************************************
 * @file    tlkfs_diskio.c
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
#include <stdint.h>
#include <time.h>
#include "ff/source/ff.h"
#include "ff/source/diskio.h"
#include "tlkfs_diskio.h"

__attribute__((weak)) const struct tm *tlkfs_diskio_getTime(void)
{
    static const struct tm time = {
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon  = 7,
        .tm_year = 2025 - 1900,
    };
    return &time;
}

__attribute__((weak)) int tlkfs_diskio_write(uint8_t *buff, uint32_t sector, uint32_t cnt)
{
    (void)buff;
    (void)sector;
    (void)cnt;
    return RES_NOTRDY;
}

__attribute__((weak)) int tlkfs_diskio_read(uint8_t *buff, uint32_t sector, uint32_t cnt)
{
    (void)buff;
    (void)sector;
    (void)cnt;
    return RES_NOTRDY;
}

__attribute__((weak)) tlkfs_diskio_info_t tlkfs_diskio_getDiskInfo(void)
{
    tlkfs_diskio_info_t ret = {0};
    return ret;
}
