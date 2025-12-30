/********************************************************************************************************
 * @file    checks.h
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
#ifndef RTC_BASE_CHECKS_H_
#define RTC_BASE_CHECKS_H_

#include <stdio.h>
#include <stdbool.h>
// If you for some reason need to know if DCHECKs are on, test the value of
// RTC_DCHECK_IS_ON. (Test its value, not if it's defined; it'll always be
// defined, to either a true or a false value.)
#if !defined(NDEBUG) || defined(DCHECK_ALWAYS_ON)
#define RTC_DCHECK_IS_ON 1
#else
#define RTC_DCHECK_IS_ON 0
#endif

// Annotate a function that will not return control flow to the caller.
#if defined(_MSC_VER)
#define RTC_NORETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define RTC_NORETURN __attribute__((__noreturn__))
#else
#define RTC_NORETURN
#endif

#ifdef RTC_DISABLE_CHECK_MSG
#define RTC_CHECK_MSG_ENABLED 0
#else
#define RTC_CHECK_MSG_ENABLED 1
#endif

#if RTC_CHECK_MSG_ENABLED
#define RTC_CHECK_EVAL_MESSAGE(message) message
#else
#define RTC_CHECK_EVAL_MESSAGE(message) ""
#endif

// C version. Lacks many features compared to the C++ version, but usage
// guidelines are the same.
#define rtc_FatalMessage(file, line, msg) printf("[%s:%d]%s \n", file, line, msg)
#define RTC_CHECK(condition)                                                                           \
    do {                                                                                               \
        if (!(condition)) {                                                                            \
            rtc_FatalMessage(__FILE__, __LINE__, RTC_CHECK_EVAL_MESSAGE("CHECK failed: " #condition)); \
        }                                                                                              \
    } while (0)

#define RTC_CHECK_EQ(a, b) RTC_CHECK((a) == (b))
#define RTC_CHECK_NE(a, b) RTC_CHECK((a) != (b))
#define RTC_CHECK_LE(a, b) RTC_CHECK((a) <= (b))
#define RTC_CHECK_LT(a, b) RTC_CHECK((a) < (b))
#define RTC_CHECK_GE(a, b) RTC_CHECK((a) >= (b))
#define RTC_CHECK_GT(a, b) RTC_CHECK((a) > (b))

#ifdef __riscv
#define RTC_DCHECK(condition)
#else
#define RTC_DCHECK(condition)                                                                           \
    do {                                                                                                \
        if (RTC_DCHECK_IS_ON && !(condition)) {                                                         \
            rtc_FatalMessage(__FILE__, __LINE__, RTC_CHECK_EVAL_MESSAGE("DCHECK failed: " #condition)); \
        }                                                                                               \
    } while (0)
#endif

#define RTC_DCHECK_EQ(a, b)      RTC_DCHECK((a) == (b))
#define RTC_DCHECK_NE(a, b)      RTC_DCHECK((a) != (b))
#define RTC_DCHECK_LE(a, b)      RTC_DCHECK((a) <= (b))
#define RTC_DCHECK_LT(a, b)      RTC_DCHECK((a) < (b))
#define RTC_DCHECK_GE(a, b)      RTC_DCHECK((a) >= (b))
#define RTC_DCHECK_GT(a, b)      RTC_DCHECK((a) > (b))

#define RTC_UNREACHABLE_CODE_HIT false
#define RTC_NOTREACHED()         RTC_DCHECK(RTC_UNREACHABLE_CODE_HIT)
#endif // RTC_BASE_CHECKS_H_
