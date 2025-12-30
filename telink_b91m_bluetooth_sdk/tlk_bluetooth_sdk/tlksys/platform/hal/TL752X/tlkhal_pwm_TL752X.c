/********************************************************************************************************
 * @file    tlkhal_pwm_TL752X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == CHIP_TYPE_TL752X
/**
 * @brief  Initialize PWM channel
 * @param[in] pCfg : Pointer to PWM channel configuration structure
 * @returns  None.
 */
void tlkhal_pwm_chnInit(const tlkhal_pwm_chn_cfg_t * const pCfg)
{
    (void)pCfg;
    // TLKHAL_ASSERT(pCfg->chn <= PWM5_ID);
    // pwm_set_pin(pCfg->pin,PWM0 + pCfg->chn);
}
/**
 * @brief  Start PWM channel
 * @param[in] chn : PWM channel number to start
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_pwm_chnStart(uint8_t chn, void *futureUse)
{
    (void) futureUse;
    (void) chn;
    // TLKHAL_ASSERT(chn <= PWM5_ID);
    // if (chn == 0) {
    //     pwm_start(FLD_PWM0_EN);
    // } else {
    //     pwm_start(BIT(chn));
    // }
}
/**
 * @brief  Stop PWM channel
 * @param[in] chn : PWM channel number to stop
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_pwm_chnStop(uint8_t chn, void *futureUse)
{
    (void) futureUse;
    (void)chn;
    // TLKHAL_ASSERT(chn <= PWM5_ID);
    // if (chn == 0) {
    //     pwm_stop(FLD_PWM0_EN);
    // } else {
    //     pwm_stop(BIT(chn));
    // }
}

#endif