/********************************************************************************************************
 * @file    tlkos_define.h
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

#define TLKOS_WAIT_FOREVER (0XFFFFFFFFU)

typedef void *TlkOsTaskHandle_t;
typedef void *TlkOsTimerHandle_t;
typedef void *TlkOsSemphrHandle_t;
typedef void *TlkOsMutexHandle_t;
typedef void *TlkOsMsgQHandle_t;
typedef void *TlkOsWorkQHandle_t;
typedef void *TlkOsEventTabHandle_t;

typedef void *TlkOsFileCtrlHandle_t;
typedef void *TlkOsFsIter_t;

typedef void (*TlkOsTaskEnterCB)(void *pUsrArg);
typedef void (*TlkOsTimerEnterCB)(TlkOsTimerHandle_t timerHandle, void *pUsrArg);
typedef void (*TlkOsEventDealCB)(void);
typedef void (*TlkOsInitFunc_t)(void);

typedef enum
{
    TLKOS_KERNEL_STATE_UNKNOWN = 0,
    TLKOS_KERNEL_STATE_NOT_START,
    TLKOS_KERNEL_STATE_RUNNING,
    TLKOS_KERNEL_STATE_SUSPEND,
    TLKOS_KERNEL_STATE_CRASH,
} TLKOS_KERNEL_STATE_ENUM;

typedef enum
{
    TLKOS_IRQ_STATE_UNKNOWN = 0,
    TLKOS_IRQ_STATE_NOT_IN_IRQ,
    TLKOS_IRQ_STATE_IN_IRQ,
} TLKOS_IRQ_STATE_ENUM;

typedef enum
{
    TLKOS_CRASH_REASON_NONE = 0,
    TLKOS_CRASH_REASON_TRAP_EXCEPTION,
    TLKOS_CRASH_REASON_CORE_MALLOC_FAIL,
    TLKOS_CRASH_REASON_TASK_STACK_OVERFLOW,
} TLKOS_CRASH_REASON_ENUM;

typedef struct
{
    uint32_t reason; //refer to TLKOS_CRASH_REASON_ENUM
    char    *detailInfo;
} TlkOsCrashInfo_t;
