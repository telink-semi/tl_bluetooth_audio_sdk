/********************************************************************************************************
 * @file    tlkdrv_codec_adapt.c
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
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec_adapt.h"

#if (TLK_DEV_CODEC_ENABLE)

static TlkOsTimerHandle_t sTlkDrvCodecTimers[TLKDRV_CODEC_ADAPT_TIMER_NUM] = {0};

/**
 * @brief Start a codec timer for once execution
 * @param[in] type - Timer type
 * @param[in] periodMs - Timer period in milliseconds
 * @param[in] CBEnter - Timer callback function
 * @param[in] userArg - User argument for callback
 * @return Operation result status
 */
int tlkdrv_codec_startTimerOnce(TLKDRV_CODEC_ADAPT_TIMER_ENUM type, uint32_t periodMs, TlkOsTimerEnterCB CBEnter, void * userArg)
{
    if(sTlkDrvCodecTimers[type] == 0){
        tlkos_timer_create(NULL,periodMs,false,CBEnter,userArg,&sTlkDrvCodecTimers[type]);
    }
    tlkos_timer_setPeriod(sTlkDrvCodecTimers[type],periodMs);
    tlkos_timer_start(sTlkDrvCodecTimers[type]);
    return TLK_ENONE;
}

/**
 * @brief Destroy a codec timer
 * @param[in] type - Timer type to destroy
 * @return Operation result status
 */
int tlkdrv_codec_destroyTimer(TLKDRV_CODEC_ADAPT_TIMER_ENUM type)
{
    tlkos_timer_destroy(sTlkDrvCodecTimers[type]);
    sTlkDrvCodecTimers[type] = 0;
    return TLK_ENONE;
}

/**
 * @brief Convert timer handle to timer type
 * @param[in] timerHandle - Timer handle to convert
 * @return Corresponding timer type
 */
TLKDRV_CODEC_ADAPT_TIMER_ENUM tlkdrv_codec_timerHandle2type(TlkOsTimerHandle_t timerHandle)
{
    for(int i = 0;i < TLKDRV_CODEC_ADAPT_TIMER_NUM;i++){
        if(sTlkDrvCodecTimers[i] == timerHandle){
            return i;
        }
    }
    return TLKDRV_CODEC_ADAPT_TIMER_NUM;
}

#endif
