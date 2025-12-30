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

//note: if change the storage format, the version number must be changed as well.
//example 0x10->0x11
#define TLKMDI_TINYSQL_VER              0x41


/*****************do not mondify!!!!!!!! start************************/
#define TLKMDI_TINYSQL_SAVE_SIGN_OTA        0x3E
#define TLKMDI_TINYSQL_VER_OTA              0x1B
/*****************do not mondify!!!!!!!! end************************/

#define TLKMDI_TINYSQL_DISK0_ADDR       TLK_CFG_FLASH_USER_SETTING_ADDR
#define TLKMDI_TINYSQL_DISK1_ADDR       TLK_CFG_FLASH_BT_DEVICES_ADDR
#define TLKMDI_TINYSQL_DISK2_ADDR       TLK_CFG_FLASH_MAC_ADDR
#define TLKMDI_TINYSQL_DISK3_ADDR       TLK_CFG_FLASH_PBAP_LIST_ADDR
#define TLKMDI_TINYSQL_DISK4_ADDR       TLK_CFG_FLASH_LE_ADDR
#define TLKMDI_TINYSQL_DISK5_ADDR       TLK_CFG_FLASH_AUDIO_ADDR

#define TINYSQL_BT_NAME_LENS            32
#define TINYSQL_PARING_DEVICE_NAME_LENS 16
#define TINYSQL_PARING_DEVICE_NUMB      3

#define TLKMDI_TINYSQL_LOG_ENABLE       0XFFFFFFFF

typedef struct
{
    uint16_t EvtModes[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX];
} keyConfigs_t; 

typedef bool (*tlkMdiTinySqlSearchFunc)(const void * arg);
