/********************************************************************************************************
 * @file    tlkos_debug.h
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

#include "common/types.h"
#include "tlklib/os/tlkos_api/tlkos_define.h"

typedef enum
{
    TLKOS_DEBUG_IO_MTI = 0,
    TLKOS_DEBUG_IO_MSI,
    TLKOS_DEBUG_IO_EXCEPT,
    TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS,
    TLKOS_DEBUG_IO_SYS_THREAD,
    TLKOS_DEBUG_IO_HOST_THREAD,
    TLKOS_DEBUG_IO_AUD_M_THREAD,
    TLKOS_DEBUG_IO_AUD_IRQ_THREAD,
    TLKOS_DEBUG_IO_GET_MAILBOX,
    TLKOS_DEBUG_IO_SWITCH_TASK,
    TLKOS_DEBUG_IO_TIMER_THREAD,
    TLKOS_DEBUG_IO_AUD_HIGHEST_THREAD,
    TLKOS_DEBUG_IO_LE_CONTROLLER_THREAD,
    TLKOS_DEBUG_IO_NUM,
} TLKOS_DEBUG_IO_ENUM;

typedef enum
{
    TLKOS_DEBUG_IO_EX_FEATURE_NONE = 0,
    TLKOS_DEBUG_IO_EX_FEATURE_AUD_IRQ,
    TLKOS_DEBUG_IO_EX_FEATURE_NUM,
} TLKOS_DEBUG_IO_EX_FEATURE;

/**
 * @brief       This function initializes the debug IOs.
 * @param[in]   none.
 * @return      none.
 */
void tlkos_debug_ioInit(void);

/**
 * @brief       This function controls the specified debug IO channel to set its level.
 * @param[in]   chn      - the debug IO channel, refer to TLKOS_DEBUG_IO_ENUM for details.
 * @param[in]   ioLevel  - the level to set, 0 for low level and 1 for high level.
 * @return      none.
 */
void tlkos_debug_ioCtrl(uint16_t chn, uint8_t ioLevel);

/**
 * @brief       This function controls the specified debug IO channel with extended features.
 * @param[in]   chn            - the debug IO channel, refer to TLKOS_DEBUG_IO_ENUM for details.
 * @param[in]   ioLevel        - the level to set, 0 for low level and 1 for high level.
 * @param[in]   exFeature      - extended feature selection, refer to TLKOS_DEBUG_IO_EX_FEATURE for details.
 * @param[in]   exFeatureArg   - argument for the extended feature.
 * @return      none.
 */
void tlkos_debug_ioCtrlEx(uint16_t chn, uint8_t ioLevel, uint32_t exFeature, void *exFeatureArg);

/**
 * @brief       This function handles system crash events.
 * @param[in]   info   - pointer to the crash information structure.
 * @return      none.
 */
void tlkos_crash(const TlkOsCrashInfo_t *info);

/**
 * @brief       This function gets the CPU usage.
 * @param[in]   none.
 * @return      CPU usage in percentage.
 */
uint32_t tlkos_debug_getCpuUsage(void);