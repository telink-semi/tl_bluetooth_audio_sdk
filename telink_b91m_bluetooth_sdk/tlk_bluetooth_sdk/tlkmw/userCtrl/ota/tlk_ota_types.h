/********************************************************************************************************
 * @file    tlk_ota_types.h
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
#ifndef TLK_OTA_TYPES_H
#define TLK_OTA_TYPES_H
#include "tl_common.h"

#include "tlk_ota_interface_port.h"
#include "tlk_ota_timer_port.h"
#include "tlk_ota_timer_porting_example.h"
#include "general_protocol/tlk_ota_general_protocol.h"

#define OTA_PRINTF(format, args...) tlk_printf(format, ##args)

#define OTA_MEMSET                  tmemset
#define OTA_MEMCPY                  tmemcpy

typedef enum
{
    TLK_FIRMWARE_OTA_IDLE    = 0xFF,
    TLK_FIRMWARE_OTA_ING     = 0x01,
    TLK_FIRMWARE_OTA_FAIL    = 0x02,
    TLK_FIRMWARE_OTA_SUCCESS = 0x03,
} TLK_FIRMWARE_OTA_STATUS_ENUM;

#define OTA_NONE        0x00 //"no error"
#define OTA_INITERR     0x01 //"init error"
#define OTA_DEFINEDERR  0x02 //"define error or not defined"
#define OTA_PARAMERR    0x03 //"parameter error"
#define OTA_READERR     0x04 //"read error"
#define OTA_WRITEERR    0x05 //"write error"
#define OTA_LOADERR     0x06 //"write error"
#define OTA_SAVEERR     0x07 //"write error"
#define OTA_PARSEERR    0x08 //"write error"
#define OTA_STATUSERR   0x09 //"write error"
#define OTA_CHANNELERR  0x0A //"write error"
#define OTA_MALLOCERR   0x0B //"write error"
#define OTA_MEMORYERR   0x0C //"write error"
#define OTA_REGISTERERR 0x0D //"write error"


#endif // #ifndef TLK_OTA_TYPES_H
