/********************************************************************************************************
 * @file    tlkdrv_codec_adapt.h
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

#include "tl_common.h"

typedef enum
{
    TLKDRV_CODEC_ADAPT_TIMER_ADC_DELAY = 0,
    TLKDRV_CODEC_ADAPT_TIMER_DAC_DELAY,
    TLKDRV_CODEC_ADAPT_TIMER_NUM,
} TLKDRV_CODEC_ADAPT_TIMER_ENUM;

/**
 * @brief Start a codec timer for once execution
 * @param[in] type - Timer type
 * @param[in] periodMs - Timer period in milliseconds
 * @param[in] CBEnter - Timer callback function
 * @param[in] userArg - User argument for callback
 * @return Operation result status
 */
int tlkdrv_codec_startTimerOnce(TLKDRV_CODEC_ADAPT_TIMER_ENUM type, uint32_t periodMs, TlkOsTimerEnterCB CBEnter, void *userArg);

/**
 * @brief Destroy a codec timer
 * @param[in] type - Timer type to destroy
 * @return Operation result status
 */
int tlkdrv_codec_destroyTimer(TLKDRV_CODEC_ADAPT_TIMER_ENUM type);

/**
 * @brief Convert timer handle to timer type
 * @param[in] timerHandle - Timer handle to convert
 * @return Corresponding timer type
 */
TLKDRV_CODEC_ADAPT_TIMER_ENUM tlkdrv_codec_timerHandle2type(TlkOsTimerHandle_t timerHandle);