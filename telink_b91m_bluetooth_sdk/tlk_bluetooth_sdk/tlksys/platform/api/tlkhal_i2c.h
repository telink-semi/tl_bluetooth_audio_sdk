/********************************************************************************************************
 * @file    tlkhal_i2c.h
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

typedef struct
{
    uint8_t  chn;
    uint8_t  resv[3];
    uint16_t sda_pin;
    uint16_t scl_pin;
} tlkhal_i2c_cfg_t;

/**
 * @brief  Set I2C pin configuration
 * @param[in] pCfg : Pointer to I2C configuration structure
 * @returns  None.
 */
void tlkhal_i2c_set_pin(const tlkhal_i2c_cfg_t *const pCfg);