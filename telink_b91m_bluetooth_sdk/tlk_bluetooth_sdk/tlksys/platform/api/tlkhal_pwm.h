/********************************************************************************************************
 * @file    tlkhal_pwm.h
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
    uint8_t  futureUse;
    uint16_t pin;
}tlkhal_pwm_chn_cfg_t;

/**
 * @brief  Initialize PWM channel
 * @param[in] pCfg : Pointer to PWM channel configuration structure
 * @returns  None.
 */
void tlkhal_pwm_chnInit(const tlkhal_pwm_chn_cfg_t * const pCfg);

/**
 * @brief  Start PWM channel
 * @param[in] chn : PWM channel number to start
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_pwm_chnStart(uint8_t chn, void *futureUse);

/**
 * @brief  Stop PWM channel
 * @param[in] chn : PWM channel number to stop
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_pwm_chnStop(uint8_t chn, void *futureUse);
