/********************************************************************************************************
 * @file    tlkmw_sysdev.h
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


#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkmw/sys_dev/codec/tlkdrv_codec_adapt.h"
#if (CHIP_TYPE == CHIP_TYPE_TL721X)
#include "tlkmw/sys_dev/codec/tlkdrv_i2s_master_tl721x.h"
#include "tlkmw/sys_dev/codec/tlkdrv_icodec_tl721x.h"
#elif (CHIP_TYPE == CHIP_TYPE_TL751X)
#include "tlkmw/sys_dev/codec/tlkdrv_icodec_tl751x.h"
#elif (CHIP_TYPE == CHIP_TYPE_TL752X)
#include "tlkmw/sys_dev/codec/tlkdrv_icodec_tl752x.h"
#endif

#include "tlkmw/sys_dev/key/tlkdrv_key.h"
#include "tlkmw/sys_dev/led/tlkdrv_led.h"
#include "tlkmw/sys_dev/serial/tlkdrv_serial.h"
#include "tlkmw/sys_dev/sy5500/tlkdrv_sy5500.h"
#include "tlkmw/sys_dev/rtc/tlkdrv_rtc.h"
