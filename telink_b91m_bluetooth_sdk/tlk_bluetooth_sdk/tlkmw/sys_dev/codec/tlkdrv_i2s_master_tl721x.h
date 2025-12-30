/********************************************************************************************************
 * @file    tlkdrv_i2s_master_tl721x.h
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
#ifndef TLKDRV_IIS_MASTER_H
#define TLKDRV_IIS_MASTER_H

#include "core/mcu_type.h"
#include "tl_common.h"
#include "drivers.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_I2S_MASTER_ENABLE)
	#include "tlkdrv_i2s_ex_codec_config.h"
	typedef struct
	{
		uint8_t isInit;
		uint8_t isOpen;
		uint8_t isMute;
		uint8_t micVol;
		uint8_t micIsEn;
		uint8_t spkIsEn;
		uint8_t channel;
		uint8_t bitDepth;
		uint32_t sampleRate;
	} tlkdrv_iismst_t;

#endif

#endif     // TLKDRV_IIS_MASTER_H
