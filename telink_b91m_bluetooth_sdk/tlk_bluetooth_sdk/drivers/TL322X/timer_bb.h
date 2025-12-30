/********************************************************************************************************
 * @file    timer_bb.h
 *
 * @brief   This is the header file for tl322x
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
/** @page TIMER_BB
 *
 *  Introduction
 *  ===============
 *  -# Timer_bb is for N22 core only, not for D25F core, D25F please use Timer.
 *
 *  -# Difference between N22 Timer_bb and D25F Timer:
 *    -# N22 Timer_bb: There are three Timer_bb, respectively Timer_bb0/1/2, they only support mode0 (System Clock Mode) and mode3 (Tick Mode).
 *    -# D25F Timer: There are only two Timers, Timer0/1, both support mode0/1/2/3.
 *
 *  API Reference
 *  ===============
 *  Header File: timer_bb.h
 */
#ifndef TIMER_BB_H_
#define TIMER_BB_H_

#include "lib/include/analog.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief   Type of Timer_BB
 */
typedef enum
{
    TIMER_BB0 = 0,
    TIMER_BB1 = 1,
    TIMER_BB2 = 2,
} timer_bb_type_e;

/**
 * @brief   Mode of Timer_BB
 * @note    TIMER_BB does not support modes GPIO_TRIGGER_MODE and GPIO_WIDTH_MODE.
 */
typedef enum
{
    TIMER_BB_MODE_SYSCLK = 0,
    TIMER_BB_MODE_TICK   = 3,
} timer_bb_mode_e;

typedef enum
{
    TIMER_BB0_IRQ = BIT(0),
    TIMER_BB1_IRQ = BIT(1),
    TIMER_BB2_IRQ = BIT(2),
} timer_bb_irq_e;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief      This function refer to get timer_bb irq status.
 * @param[in]  status    - variable of enum to select the timer_bb interrupt source.
 * @retval     non-zero      -  the interrupt occurred.
 * @retval     zero  -  the interrupt did not occur.
 */
static inline unsigned char timer_bb_get_irq_status(timer_bb_irq_e status)
{
    return reg_tmr_bb_int_sts & status;
}

/**
 * @brief     This function refer to clr timer_bb irq status.
 * @param[in] status - variable of enum to select the timer_bb interrupt source.
 * @return    none
 */
static inline void timer_bb_clr_irq_status(timer_bb_irq_e status)
{
    reg_tmr_bb_int_sts = status;
}

/**
 * @brief   This function refer to set timer_bb tick.
 * @param[in] type - timer_bb0/timer_bb1/timer_bb2.
 * @param[in] tick - the tick of timer_bb
 * @return  none
 */
static inline void timer_bb_set_tick(timer_bb_type_e type, unsigned int tick)
{
    reg_tmr_bb_tick(type) = tick;
}

/**
 * @brief   This function refer to get timer_bb tick.
 * @param[in] type - timer_bb0/timer_bb1/timer_bb2.
 * @return  none
 */
static inline unsigned int timer_bb_get_tick(timer_bb_type_e type)
{
    return reg_tmr_bb_tick(type);
}

/**
 * @brief     This function set to initial tick for timer_bb.
 * @param[in] type - timer_bb0/timer_bb1/timer_bb2.
 * @param[in] init_tick - initial tick value.
 * @return    none
 */
static inline void timer_bb_set_init_tick(timer_bb_type_e type, unsigned int init_tick)
{
    reg_tmr_bb_tick(type) = init_tick;
}

/**
 * @brief     This function set to capture tick for timer_bb0/timer_bb1/timer_bb2.
 * @param[in] type - timer_bb0/timer_bb1/timer_bb2.
 * @param[in] cap_tick - initial tick value.
 * @return    none
 */
static inline void timer_bb_set_cap_tick(timer_bb_type_e type, unsigned int cap_tick)
{
    reg_tmr_bb_capt(type) = cap_tick;
}

/**
 * @brief This function servers to set timer_bb irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static inline void timer_bb_set_irq_mask(timer_bb_irq_e mask)
{
    reg_tmr_bb_int_mask |= mask;
}

/**
 * @brief     the specified timer_bb start working.
 * @param[in] type - select the timer_bb to start.
 * @return    none
 */
void timer_bb_start(timer_bb_type_e type);

/**
 * @brief     set mode of timer_bb.
 * @param[in] type - select the timer_bb to start.
 * @param[in] mode - select mode for timer_bb.
 * @return    none
 */
void timer_bb_set_mode(timer_bb_type_e type, timer_bb_mode_e mode);

/**
 * @brief     the specified timer_bb stop working.
 * @param[in] type - select the timer_bb to stop.
 * @return    none
 */
void timer_bb_stop(timer_bb_type_e type);


#endif /* TIMER_BB_H_ */
