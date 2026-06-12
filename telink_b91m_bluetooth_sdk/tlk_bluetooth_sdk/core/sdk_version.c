/********************************************************************************************************
 * @file    sdk_version.c
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
#include "sdk_version.h"
#include "vendor/common/user_config.h"

#ifndef TLK_CFG_CUSTOMER_SDK_VERSION
volatile __attribute__((section(".sdk_version"))) unsigned char sdk_version[] = {SDK_VERSION(SDK_VERSION_NUM)};
#else
volatile __attribute__((section(".sdk_version"))) unsigned char sdk_version[] = {TLK_CFG_CUSTOMER_SDK_VERSION};
#endif