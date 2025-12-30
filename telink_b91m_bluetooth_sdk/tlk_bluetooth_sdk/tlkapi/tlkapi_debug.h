/********************************************************************************************************
 * @file    tlkapi_debug.h
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
#include "core/mcu_type.h"
#include "common/types.h"
#include "core/tlk_debug.h"
#include "tlklib/dbg/tlkdbg.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X)
#define USB_CNT 1
#else
#define USB_CNT 0
#endif

#define TLKAPI_DBG_WARN_FLAG   0x02
#define TLKAPI_DBG_INFO_FLAG   0x04
#define TLKAPI_DBG_TRACE_FLAG  0x08
#define TLKAPI_DBG_ERROR_FLAG  0x10
#define TLKAPI_DBG_FATAL_FLAG  0x20
#define TLKAPI_DBG_ARRAY_FLAG  0x40
#define TLKAPI_DBG_ASSERT_FLAG 0x80
#define TLKAPI_DBG_FLAG_ALL    0xFE

#define TLKAPI_WARN_HEAD       "<WARN>"
#define TLKAPI_INFO_HEAD       "<INFO>"
#define TLKAPI_TRACE_HEAD      "<TRACE>"
#define TLKAPI_FATAL_HEAD      "<FATAL>"
#define TLKAPI_ERROR_HEAD      "<ERROR>"
#define TLKAPI_ARRAY_HEAD      "<ARRAY>"

typedef enum
{
    TLKAPI_DEBUG_LEVEL_LEVEL1 =
        TLKAPI_DBG_ASSERT_FLAG | TLKAPI_DBG_FATAL_FLAG | TLKAPI_DBG_ERROR_FLAG | TLKAPI_DBG_WARN_FLAG | TLKAPI_DBG_INFO_FLAG | TLKAPI_DBG_TRACE_FLAG | TLKAPI_DBG_ARRAY_FLAG,
    TLKAPI_DEBUG_LEVEL_LEVEL2 = TLKAPI_DBG_ASSERT_FLAG | TLKAPI_DBG_FATAL_FLAG | TLKAPI_DBG_ERROR_FLAG | TLKAPI_DBG_WARN_FLAG | TLKAPI_DBG_INFO_FLAG,
    TLKAPI_DEBUG_LEVEL_LEVEL3 = TLKAPI_DBG_ASSERT_FLAG | TLKAPI_DBG_FATAL_FLAG | TLKAPI_DBG_ERROR_FLAG | TLKAPI_DBG_WARN_FLAG,
    TLKAPI_DEBUG_LEVEL_LEVEL4 = TLKAPI_DBG_ASSERT_FLAG | TLKAPI_DBG_FATAL_FLAG | TLKAPI_DBG_ERROR_FLAG,
    TLKAPI_DEBUG_LEVEL_LEVEL5 = TLKAPI_DBG_ASSERT_FLAG | TLKAPI_DBG_FATAL_FLAG,
} TLKAPI_DEBUG_LEVEL_ENUM;

#define tlkapi_warn(flags, pSign, format, args...)         tlkdbg_message(flags, pSign, TLKAPI_DBG_WARN_FLAG, TLKAPI_WARN_HEAD, format, ##args)
#define tlkapi_info(flags, pSign, format, args...)         tlkdbg_message(flags, pSign, TLKAPI_DBG_INFO_FLAG, TLKAPI_INFO_HEAD, format, ##args)
#define tlkapi_trace(flags, pSign, format, args...)        tlkdbg_message(flags, pSign, TLKAPI_DBG_TRACE_FLAG, TLKAPI_TRACE_HEAD, format, ##args)
#define tlkapi_fatal(flags, pSign, format, args...)        tlkdbg_message(flags, pSign, TLKAPI_DBG_FATAL_FLAG, TLKAPI_FATAL_HEAD, format, ##args)
#define tlkapi_error(flags, pSign, format, args...)        tlkdbg_message(flags, pSign, TLKAPI_DBG_ERROR_FLAG, TLKAPI_ERROR_HEAD, format, ##args)
#define tlkapi_array(flags, pSign, format, pData, dataLen) tlkdbg_array(flags, pSign, format, (uint8_t *)pData, dataLen)

#define tlkapi_sprintf                                     tlkdbg_sprintf

#define tlkapi_sendStr(en, pStr)                           tlkapi_send_string_data(en, pStr, 0, 0)
#define tlkapi_sendData(en, pStr, pData, dataLen)          tlkapi_send_string_data(en, pStr, (uint8_t *)pData, dataLen)
#define tlkapi_sendU08s(en, pStr, val0, val1, val2, val3)  tlkapi_send_string_u8s(en, pStr, (uint8_t)(val0), (uint8_t)(val1), (uint8_t)(val2), (uint8_t)(val3))
#define tlkapi_sendU32s(en, pStr, val0, val1, val2, val3)  tlkapi_send_string_u32s(en, pStr, (uint32_t)(val0), (uint32_t)(val1), (uint32_t)(val2), (uint32_t)(val3))


/////////////////////////////// Add by BLE, send string data with high efficiency //////////////////////////
///////////////////////////////                     Begin                         //////////////////////////

/**
 * @brief	user do not need to pay attention to three APIs below, and do not use them in application.
 */


/**
 * @brief   	Send debug log to log FIFO, character string and data mixed mode.
 *				attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	en - send log enable, 1: enable;  0: disable
 * @param[in]	str - character string
 * @param[in]	pData - pointer of data
 * @param[in]	len - length of data
 * @return		1: send to FIFO success; 0: send to FIFO fail
 */
#define tlkapi_send_string_data(en, str, pData, len)        \
    if (en) {                                               \
        tlkdbg_send_str_data(str, (uint8_t *)(pData), len); \
    }


/**
 * @brief   	Send debug log to log FIFO, character string and data mixed mode, with variable length data, data in "unsigned int" format
 *				attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	en - send log enable, 1: enable;  0: disable
 * @param[in]	str - character string
 * @param[in]	... - variable length data, maximum length is 8
 * @param[in]	data_len - length of data
 * @return		1: send to FIFO success; 0: send to FIFO fail
 */
#define tlkapi_send_string_u32s(en, str, ...)                              \
    if (en) {                                                              \
        tlkdbg_send_str_u32s(str, COUNT_ARGS(__VA_ARGS__), ##__VA_ARGS__); \
    }


/**
 * @brief   	Send debug log to log FIFO, character string and data mixed mode, with variable length data, data in "unsigned char" format
 *				attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	en - send log enable, 1: enable;  0: disable
 * @param[in]	str - character string
 * @param[in]	... - variable length data, maximum length is 16
 * @param[in]	data_len - length of data
 * @return		1: send to FIFO success; 0: send to FIFO fail
 */
#define tlkapi_send_string_u8s(en, str, ...)                              \
    if (en) {                                                             \
        tlkdbg_send_str_u8s(str, COUNT_ARGS(__VA_ARGS__), ##__VA_ARGS__); \
    }

/**
 * @brief   	Send debug log to log FIFO, string only mode
 *				attention: here just send log to FIFO, can not output immediately, wait for "tlkapi debug_handler" to output log.
 * @param[in]	en - send log enable, 1: enable;  0: disable
 * @param[in]	fmt - please refer to standard C function "printf"
 * @return		none
 */
#define tlkapi_printf(en, fmt, ...)     \
    if (en) {                           \
        tlk_printf(fmt, ##__VA_ARGS__); \
    }
