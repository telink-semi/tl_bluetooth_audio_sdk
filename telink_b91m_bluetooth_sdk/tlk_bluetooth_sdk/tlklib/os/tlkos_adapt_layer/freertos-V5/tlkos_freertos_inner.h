/********************************************************************************************************
 * @file    tlkos_freertos_inner.h
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

#include "common/types.h"
#include "common/compiler.h"
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/os/tlkos_config.h"

/**
 * @brief Convert milliseconds to OS ticks
 * @param[in] ms - Time in milliseconds to convert
 * @return OS tick count corresponding to the given milliseconds
 *         If ms is TLKOS_WAIT_FOREVER, returns TLKOS_WAIT_FOREVER
 *         Otherwise, returns the calculated tick count based on OS tick frequency
 */
static __attribute__((__always_inline__)) inline uint32_t tlkos_freertos_msToTick(uint32_t ms)
{
    if (ms == TLKOS_WAIT_FOREVER) {
        return TLKOS_WAIT_FOREVER;
    }
    return ms * 1000 / TLKOS_CFG_OS_TICK_HZ;
}
