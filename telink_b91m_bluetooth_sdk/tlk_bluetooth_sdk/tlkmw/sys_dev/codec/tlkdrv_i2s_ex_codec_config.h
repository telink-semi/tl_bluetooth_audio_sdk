/********************************************************************************************************
 * @file    tlkdrv_i2s_ex_codec_config.h
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


#define I2S_CODEC_CHIP_NULL		0
#define I2S_CODEC_CHIP_NAU88L21 2

#define I2S_CODEC_SEL			I2S_CODEC_CHIP_NAU88L21

#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_I2S_MASTER_ENABLE)


#if (I2S_CODEC_SEL ==I2S_CODEC_CHIP_NAU88L21)
	#include "ex_nau88l21/tlkdrv_nau88l21_interface.h"
	#include "ex_nau88l21/tlkdrv_nau88l21.h"

	#define hal_i2s_pin_config 		hal_nau8821_i2s_pin_config
	#define hal_i2s_config 			hal_nau8821_i2s_config
	#define hal_i2s_input			hal_nau8821_i2s_input
	#define hal_i2s_output 			hal_nau8821_i2s_output

	#define hal_i2c_init 			hal_nau8821_i2c_init
	#define hal_i2c_write 			hal_nau8821_i2c_write
	#define hal_i2c_read 			hal_nau8821_i2c_read
	#define hal_i2c_update_bits 	hal_nau8821_i2c_update_bits
	#define hal_i2s_init 			hal_nau8821_i2s_init

	#define regmap_write               hal_nau8821_i2c_write
	#define regmap_read                hal_nau8821_i2c_read
	#define regmap_update_bits         hal_nau8821_i2c_update_bits
	#define ussleep(ms)                delay_ms(ms) //vTaskDelay(pdMS_TO_TICKS(ms));

#endif

#endif
