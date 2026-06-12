/********************************************************************************************************
 * @file    tlkmdi_tinySql_config.h
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
#include "tlkmw/sys_dev/key/tlkdrv_key.h"
#include "vendor/common/app_flash.h"

#define TLKMDI_TINYSQL_SAVE_SIGN        0x3E
#define TLKMDI_TINYSQL_VER              0x42

#define TINYSQL_BT_NAME_LENS            32
#define TINYSQL_PARING_DEVICE_NAME_LENS 16
#define TINYSQL_PARING_DEVICE_NUMB      3

#define TLKMDI_TINYSQL_LOG_ENABLE       0XFFFFFFFF

typedef struct
{
    uint16_t EvtModes[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX];
} keyConfigs_t;

typedef bool (*tlkMdiTinySqlSearchFunc)(const void *arg);
