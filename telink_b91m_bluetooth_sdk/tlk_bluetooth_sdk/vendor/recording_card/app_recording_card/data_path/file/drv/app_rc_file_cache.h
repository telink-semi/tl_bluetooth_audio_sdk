/********************************************************************************************************
 * @file    app_rc_file_cache.h
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

typedef int (*cache_write_api)(void *pData, uint32_t len);

typedef int (*cache_read_api)(void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen);

typedef struct
{
    uint32_t        ptr;
    uint32_t        size;
    uint32_t        remain;
    cache_write_api write;
    cache_read_api  read;
    uint8_t         cache[0];
} app_rc_file_cache_t;

app_rc_file_cache_t *app_rc_file_cache_create(uint32_t cacheSize, cache_write_api writeApi, cache_read_api readApi);

void app_rc_file_cache_destroy(app_rc_file_cache_t *cache);

int app_rc_file_cache_write(app_rc_file_cache_t *cache, void *pData, uint32_t len);

int app_rc_file_cache_read(app_rc_file_cache_t *cache, void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen);