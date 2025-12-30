/********************************************************************************************************
 * @file    timer_bb.c
 *
 * @brief   This is the source file for tl322x
 *
 * @author  Driver Group
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
#include "timer_bb.h"

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief     the specified timer_bb start working.
 * @param[in] type - select the timer_bb to start.
 * @return    none
 */
void timer_bb_start(timer_bb_type_e type)
{
    switch (type) {
    case TIMER_BB0:
        reg_tmr_bb_ctrl0 |= FLD_TMR_BB0_EN;
        break;
    case TIMER_BB1:
        reg_tmr_bb_ctrl0 |= FLD_TMR_BB1_EN;
        break;
    case TIMER_BB2:
        reg_tmr_bb_ctrl1 |= FLD_TMR_BB2_EN;
        break;
    default:
        break;
    }
}

/**
 * @brief     the specified timer_bb stop working.
 * @param[in] type - select the timer_bb to stop.
 * @return    none
 */
void timer_bb_stop(timer_bb_type_e type)
{
    switch (type) {
    case TIMER_BB0:
        reg_tmr_bb_ctrl0 &= (~FLD_TMR_BB0_EN);
        break;
    case TIMER_BB1:
        reg_tmr_bb_ctrl0 &= (~FLD_TMR_BB1_EN);
        break;
    case TIMER_BB2:
        reg_tmr_bb_ctrl1 &= (~FLD_TMR_BB2_EN);
        break;
    default:
        break;
    }
}

/**
 * @brief     set mode of timer_bb.
 * @param[in] type - select the timer_bb to start.
 * @param[in] mode - select mode for timer_bb.
 * @return    none
 */
void timer_bb_set_mode(timer_bb_type_e type, timer_bb_mode_e mode)
{
    switch (type) {
    case TIMER_BB0:
        reg_tmr_bb_int_sts = FLD_TMR_BB0_INT_STATUS; //clear irq status
        reg_tmr_bb_ctrl0   = (reg_tmr_bb_ctrl0 & (~FLD_TMR_BB0_MODE)) | mode;
        break;
    case TIMER_BB1:
        reg_tmr_bb_int_sts = FLD_TMR_BB1_INT_STATUS; //clear irq status
        reg_tmr_bb_ctrl0   = (reg_tmr_bb_ctrl0 & (~FLD_TMR_BB1_MODE)) | (mode << 4);
        break;
    case TIMER_BB2:
        reg_tmr_bb_int_sts = FLD_TMR_BB2_INT_STATUS; //clear irq status
        reg_tmr_bb_ctrl1   = (reg_tmr_bb_ctrl1 & (~FLD_TMR_BB2_MODE)) | mode;
        break;
    default:
        break;
    }
}
