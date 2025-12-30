/********************************************************************************************************
 * @file    tlkfs_diskio.h
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
#pragma once

typedef struct 
{
    uint32_t sector_size;
    uint32_t sector_num;
}tlkfs_diskio_info_t;


const struct tm *tlkfs_diskio_getTime(void);

int tlkfs_diskio_write(uint8_t *buff, uint32_t sector, uint32_t cnt);

int tlkfs_diskio_read(uint8_t *buff, uint32_t sector, uint32_t cnt);

tlkfs_diskio_info_t tlkfs_diskio_getDiskInfo(void);
