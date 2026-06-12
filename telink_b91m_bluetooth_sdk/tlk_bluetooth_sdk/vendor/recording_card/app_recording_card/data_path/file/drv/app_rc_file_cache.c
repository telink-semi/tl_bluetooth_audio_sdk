/********************************************************************************************************
 * @file    app_rc_file_cache.c
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
#include "tlkapi/tlkapi_common.h"
#include "tlklib/mem/tlkmem1.h"
#include "app_rc_file_cache.h"

#define IS_ALLOW_FILE_CACHE       1

#define IS_USE_INDEPENDENT_POOL   (MCU_CORE_TYPE == CHIP_TYPE_TL751X)

#define APP_RC_FILE_MEM_POOL_SIZE (120 * 1024)

#if IS_USE_INDEPENDENT_POOL
__attribute__((aligned(4))) static uint8_t sAppRcFileMemPool[APP_RC_FILE_MEM_POOL_SIZE] = {0};
static uint8_t                             sAppRcFileMemPoolIsInit                      = 0;
#endif

static void *app_rc_file_cache_mem_malloc(uint32_t size)
{
#if IS_USE_INDEPENDENT_POOL
    if (sAppRcFileMemPoolIsInit == 0) {
        tlkmem1_init(sAppRcFileMemPool, APP_RC_FILE_MEM_POOL_SIZE);
        sAppRcFileMemPoolIsInit = 1;
    }
    void *ptr = tlkmem1_malloc(sAppRcFileMemPool, size);
    if (ptr != NULL) {
        return ptr;
    }
    return NULL;
#else
    if (size > 6 * 1024) {
        return NULL;
    }
    return tlkos_malloc(size);
#endif
}

static void app_rc_file_cache_mem_free(void *ptr)
{
#if IS_USE_INDEPENDENT_POOL
    tlkmem1_free(sAppRcFileMemPool, ptr);
#else
    tlkos_free(ptr);
#endif
}

app_rc_file_cache_t *app_rc_file_cache_create(uint32_t cacheSize, cache_write_api writeApi, cache_read_api readApi)
{
    (void)cacheSize;
    (void)writeApi;
    (void)readApi;
    (void)app_rc_file_cache_mem_malloc;
#if IS_ALLOW_FILE_CACHE
    if (writeApi == NULL && readApi == NULL) {
        return NULL;
    }
    if (writeApi != NULL && readApi != NULL) {
        return NULL;
    }
    uint32_t             totalSize = sizeof(app_rc_file_cache_t) + cacheSize;
    app_rc_file_cache_t *cache     = (app_rc_file_cache_t *)app_rc_file_cache_mem_malloc(totalSize);
    if (cache == NULL) {
        return NULL;
    }
    cache->size   = cacheSize;
    cache->write  = writeApi;
    cache->read   = readApi;
    cache->ptr    = 0;
    cache->remain = 0;
    return cache;
#else
    return NULL;
#endif
}

void app_rc_file_cache_destroy(app_rc_file_cache_t *cache)
{
    (void)cache;
    (void)app_rc_file_cache_mem_free;
#if IS_ALLOW_FILE_CACHE
    if (cache == NULL) {
        return;
    }
    if (cache->write && cache->ptr) {
        cache->write(cache->cache, cache->ptr);
    }
    app_rc_file_cache_mem_free(cache);
#endif
}

int app_rc_file_cache_write(app_rc_file_cache_t *cache, void *pData, uint32_t len)
{
    (void)cache;
    (void)pData;
    (void)len;
#if IS_ALLOW_FILE_CACHE
    if (cache == NULL || cache->write == NULL) {
        return -TLK_EPARAM;
    }
    if (len + cache->ptr < cache->size) {
        memcpy(cache->cache + cache->ptr, pData, len);
        cache->ptr += len;
        return TLK_ENONE;
    }
    cache->write(cache->cache, cache->ptr);
    cache->ptr = 0;
    if (len >= cache->size) {
        cache->write(pData, len);
    } else {
        memcpy(cache->cache, pData, len);
        cache->ptr = len;
    }
#endif
    return TLK_ENONE;
}

int app_rc_file_cache_read(app_rc_file_cache_t *cache, void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen)
{
    (void)cache;
    (void)pBuffer;
    (void)reqReadLen;
    (void)relReadLen;
#if IS_ALLOW_FILE_CACHE
    if (cache == NULL || cache->read == NULL || pBuffer == NULL) {
        return -TLK_EPARAM;
    }
    uint32_t readLen = 0;
    while (cache->remain < reqReadLen) {
        memcpy(pBuffer, cache->cache + cache->ptr, cache->remain);
        reqReadLen -= cache->remain;
        readLen += cache->remain;
        pBuffer += cache->remain;
        cache->ptr = 0;
        int ret    = cache->read(cache->cache, cache->size, &cache->remain);
        if (ret != TLK_ENONE || cache->remain == 0) {
            if (relReadLen) {
                *relReadLen = readLen;
            }
            return ret;
        }
    }
    memcpy(pBuffer, cache->cache + cache->ptr, reqReadLen);
    cache->ptr += reqReadLen;
    cache->remain -= reqReadLen;
    readLen += reqReadLen;
    if (relReadLen) {
        *relReadLen = readLen;
    }
#endif
    return TLK_ENONE;
}