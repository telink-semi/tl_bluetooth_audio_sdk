/********************************************************************************************************
 * @file    tlkdbg_n22core.c
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
#include "drivers.h"
#include "../../../tlkdbg_cfg.h"
#include "../private/tlkdbg_sync_stream_inner.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#if (TLK_SM_LOG_ENABLE && defined(MCU_CORE_N22))

/**
 * @brief       Send log message from N22 core to shared memory
 * @param[in]   type    - log type
 * @param[in]   pData   - pointer to the data to send
 * @param[in]   dataLen - length of the data to send
 * @return      none.
 */
void tlkdbg_n22core_write(uint8_t type, void *pData, uint16_t dataLen)
{
    (void) type;
    uint8_t *headBuffer = pData + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE - 5;
    headBuffer[0] = '[';
    headBuffer[1] = 'N';
    headBuffer[2] = '2';
    headBuffer[3] = '2';
    headBuffer[4] = ']';
    tlk_n22_log_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_LOG, headBuffer, dataLen + 5 - TLK_DEBUG_LOG_CACHE_RESERVE_LEN);
}

#endif 