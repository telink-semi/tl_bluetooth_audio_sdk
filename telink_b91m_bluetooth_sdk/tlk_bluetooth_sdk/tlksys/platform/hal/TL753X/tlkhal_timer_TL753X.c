/********************************************************************************************************
 * @file    tlkhal_timer_TL753X.c
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
#include "tl_common.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_TL753X
/**
 * @brief  Start timer
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
__attribute__((always_inline)) inline void tlkhal_timer_start(const tlkhal_timer_cfg_t *cfg)
{
    TLKHAL_ASSERT(cfg->chn <= TIMER1);

    switch (cfg->chn) {
    case TIMER0:
        reg_tmr_ctrl0 |= FLD_TMR0_EN;
        break;
    case TIMER1:
        reg_tmr_ctrl0 |= FLD_TMR1_EN;
        break;
    default:
        break;
    }
}

/**
 * @brief  Stop timer
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
__attribute__((always_inline)) inline void tlkhal_timer_stop(const tlkhal_timer_cfg_t *cfg)
{
    TLKHAL_ASSERT(cfg->chn <= TIMER1);

    switch (cfg->chn) {
    case TIMER0:
        reg_tmr_ctrl0 &= (~FLD_TMR0_EN);
        break;
    case TIMER1:
        reg_tmr_ctrl0 &= (~FLD_TMR1_EN);
        break;
    default:
        break;
    }
}

/**
 * @brief  Set timer mode
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
__attribute__((always_inline)) inline void tlkhal_timer_set_mode(const tlkhal_timer_cfg_t *cfg)
{
    TLKHAL_ASSERT(cfg->chn <= TIMER1);
    TLKHAL_ASSERT(cfg->mode <= TIMER_MODE_TICK);

    switch (cfg->chn) {
    case TIMER0:
        reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR0_MODE);
        reg_tmr_ctrl0 |= cfg->mode;
        break;
    case TIMER1:
        reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR1_MODE);
        reg_tmr_ctrl0 |= (cfg->mode << 4);
        break;
    default:
        break;
    }
}

#endif
