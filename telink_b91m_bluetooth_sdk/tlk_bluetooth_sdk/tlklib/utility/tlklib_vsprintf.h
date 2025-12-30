/********************************************************************************************************
 * @file    tlklib_vsprintf.h
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
#include "vendor/common/user_config.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int tlklib_vsprintf(char *buf, const char *fmt, va_list ap);

int tlklib_sprintf(char *buf, const char *fmt, ...);

int tlklib_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

int tlklib_snprintf(char *buf, size_t size, const char *fmt, ...);

//STDLIB USE TOO MUCH STACK SIZE
#ifndef TLK_CFG_VSPRINTF_USE_STD_LIB
#define TLK_CFG_VSPRINTF_USE_STD_LIB 0
#endif

#if TLK_CFG_VSPRINTF_USE_STD_LIB
    #define tlk_vsnprintf(buf, size, fmt, ap)  vsnprintf((buf), (size), (fmt), (ap))
    #define tlk_snprintf(buf, size, fmt, ...)  snprintf((buf), (size), (fmt), ##__VA_ARGS__)
    #define tlk_vsprintf(buf, fmt, ap)         vsprintf((buf), (fmt), (ap))
    #define tlk_sprintf(buf, size, fmt, ...)   sprintf((buf), (fmt), ##__VA_ARGS__)
#else
    #define tlk_vsnprintf(buf, size, fmt, ap)  tlklib_vsnprintf((buf), (size), (fmt), (ap))
    #define tlk_snprintf(buf, size, fmt, ...)  tlklib_snprintf((buf), (size), (fmt), ##__VA_ARGS__)
    #define tlk_vsprintf(buf, fmt, ap)         tlklib_vsprintf((buf), (fmt), (ap))
    #define tlk_sprintf(buf, size, fmt, ...)   tlklib_sprintf((buf), (fmt), ##__VA_ARGS__)
#endif

