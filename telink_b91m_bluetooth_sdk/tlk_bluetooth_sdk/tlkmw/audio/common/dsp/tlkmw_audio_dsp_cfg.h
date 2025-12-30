/********************************************************************************************************
 * @file    tlkmw_audio_dsp_cfg.h
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

#ifndef DSP_USE_RAM_BOOT
#define DSP_USE_RAM_BOOT (1 && !MCU_CORE_TL752X_TEMP)
#endif

#ifndef DSP_FW_DOWNLOAD_FLASH_ADDR
#define DSP_FW_DOWNLOAD_FLASH_ADDR		0x20200000
#endif

#if DSP_USE_RAM_BOOT
#undef DSP_FW_DOWNLOAD_FLASH_ADDR
#define DSP_FW_DOWNLOAD_FLASH_ADDR		0x2100660
#endif

#ifndef DSP_SUPPORT_SUSPEND
#define DSP_SUPPORT_SUSPEND	  (TLK_CFG_SUSPEND_ENABLE)
#endif

#ifndef DSP_DBG_LOG_ENABLE
#define DSP_DBG_LOG_ENABLE 1
#endif
