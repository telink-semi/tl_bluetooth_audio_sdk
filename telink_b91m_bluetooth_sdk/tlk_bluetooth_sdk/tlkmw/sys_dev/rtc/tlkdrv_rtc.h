/********************************************************************************************************
 * @file    tlkdrv_rtc.h
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

#if TLK_DEV_RTC_ENABLE	
#include <time.h>

void tlkdrv_rtc_init(void);

const struct tm *tlkdrv_rtc_getTime(int32_t timeZone);

uint32_t tlkdrv_rtc_getTimeStamp(int32_t timeZone);

void tlkdrv_rtc_setTimeStamp(uint32_t time);

void tlkdrv_rtc_setTime(struct tm *time);

void tlkdrv_rtc_powerOffSave(void);
#endif

