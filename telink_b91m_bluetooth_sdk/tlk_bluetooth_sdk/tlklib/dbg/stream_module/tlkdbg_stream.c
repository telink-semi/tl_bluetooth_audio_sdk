/********************************************************************************************************
 * @file    tlkdbg_stream.c
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
#if (TLK_DEBUG_ENABLE)
#include "async/tlkdbg_async_stream.h"
#include "sync/tlkdbg_sync_stream.h"
#include "tlkdbg_stream.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "../tlkdbg_cfg.h"

/**
 * @brief       Copy N22 log data
 * @param[in]   pData    - Pointer to data
 * @param[in]   dataLen  - Data length
 * @return      none.
 */
static void tlkdbg_n22log_copy(uint8_t *pData, uint32_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)
    if (dataLen > 250 - TLK_DEBUG_LOG_CACHE_RESERVE_LEN) {
        return;
    }
    uint8_t buffer[250];
    tmemcpy(buffer + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE, pData, dataLen);
    tlksys_mutex_lock(TLKSYS_MUTEX_LOG);
    tlkdbg_stream_write(0, buffer, dataLen + TLK_DEBUG_LOG_CACHE_RESERVE_LEN);
    tlksys_mutex_unlock(TLKSYS_MUTEX_LOG);
#endif
}

/**
 * @brief       Initialize dual-core debug functionality
 * @return      none.
 */
static void tlkdbg_dualcore_init(void)
{
    (void)tlkdbg_n22log_copy;
#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)
    tlk_d25f_register_log_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_LOG, tlkdbg_n22log_copy);
#endif
}

/**
 * @brief       Process dual-core debug events
 * @return      none.
 */
static void tlkdbg_dualcore_handler(void)
{
#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)
    tlk_share_memory_service_log_handler();
#endif
}

/**
 * @brief       Initialize debug stream modules
 * @return      none.
 * @note        Initializes both async and sync stream modules
 */
void tlkdbg_stream_init(void)
{
    tlkdbg_dualcore_init();
    tlkdbg_async_stream_init();
    tlkdbg_sync_stream_init();
}

/**
 * @brief       Reset debug stream modules
 * @return      none.
 * @note        Resets async stream modules
 */
void tlkdbg_stream_reset(void)
{
    tlkdbg_async_stream_reset();
}

/**
 * @brief       Write data to debug streams
 * @param[in]   type    - Stream type
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Writes to both async and sync streams
 */
void tlkdbg_stream_write(uint8_t type, void *pData, uint16_t dataLen)
{
    tlkdbg_async_stream_write(type, pData, dataLen);
    tlkdbg_sync_stream_write(type, pData, dataLen);
}

/**
 * @brief       Process debug stream data
 * @return      none.
 * @note        Handles both dual-core and async stream processing
 */
void tlkdbg_stream_handler(void)
{
    tlkdbg_dualcore_handler();
    tlkdbg_async_stream_handler();
}

/**
 * @brief       Check if debug stream is busy
 * @return      true if busy, false otherwise
 * @note        Checks if async stream is busy
 */
bool tlkdbg_stream_is_busy(void)
{
    return tlkdbg_async_stream_is_busy();
}

/**
 * @brief       Write status data to debug stream
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Writes status data to async stream with type 1
 */
_attribute_ram_code_sec_ void tlkdbg_stream_status_write(void *pData, uint16_t dataLen)
{
    tlkdbg_async_stream_write(1, pData, dataLen);
}

/**
 * @brief       Handle debug stream download
 * @return      none.
 * @note        Delegates to async stream download handler
 */
_attribute_ram_code_sec_ void tlkdbg_stream_download_handler(void)
{
    tlkdbg_async_stream_download_handler();
}

#endif