/********************************************************************************************************
 * @file    app_flash.h
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

#include "vendor/common/user_config.h"

/******************************************************************************
 * Macro: Flash offset
 * Descr: The default capacity of Flash is 2M. The following macros
 * define the relative offset of Flash. For details about flash
 * capacity configuration/address configuration, see the function
 * tlkcfg_getFLashCap/tlkcfg_getFlashAddr.
 *******************************************************************************/
#ifndef TLK_CFG_FLASH_MAC_ADDR
#define TLK_CFG_FLASH_MAC_ADDR (0xFF000) // BLE &BT MAC ADDRESS ,and can not change. 4K 0XFF000-0XFFFFF
#endif

#ifndef TLK_CFG_FLASH_USER_SETTING_ADDR
#define TLK_CFG_FLASH_USER_SETTING_ADDR (0xF0000) // 8K 0XF0000-0XF1FFF
#endif

#ifndef TLK_CFG_FLASH_BT_DEVICES_ADDR
#define TLK_CFG_FLASH_BT_DEVICES_ADDR (0xF2000) // 8k 0XF2000-0XF3FFF
#endif

#ifndef TLK_CFG_FLASH_LE_ADDR
#define TLK_CFG_FLASH_LE_ADDR (0xF4000) // 8k 0XF4000-0Xf5FFF
#endif

#ifndef CONFIG_TLK_FLASH_KVDB_ADDR
#define CONFIG_TLK_FLASH_KVDB_ADDR (0xF0000)
#endif

#ifndef CONFIG_TLK_FLASH_KVDB_SIZE
#define CONFIG_TLK_FLASH_KVDB_SIZE (32 * 1024)
#endif

#ifndef TLK_CFG_FLASH_USBID_ADDR
#define TLK_CFG_FLASH_USBID_ADDR (0xF8000)
#endif

#ifndef TLK_CFG_FLASH_PBAP_LIST_ADDR
#define TLK_CFG_FLASH_PBAP_LIST_ADDR (0xE0000) // 64k 0XE0000-0XEFFFF
#endif
