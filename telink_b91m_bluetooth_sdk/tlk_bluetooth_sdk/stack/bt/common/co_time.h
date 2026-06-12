/********************************************************************************************************
 * @file    co_time.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef _CO_TIME_H_
#define _CO_TIME_H_
#if (CHIP_TYPE == CHIP_TYPE_TL752X)
/**
 ****************************************************************************************
 * @defgroup CO_TIME Utilities
 * @ingroup COMMON
 * @brief  Time utilities
 *
 * This module contains the Common time utilities functions and macros.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h> // standard definitions
#include <stddef.h> // standard definitions

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/*
 * ENUMERATIONS DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Function to called once timer expires
 *
 * @param[in] p_env   Pointer to environment that will be used as callback parameter.
 ****************************************************************************************
 */
typedef void (*co_time_timer_cb)(void *p_env);

/// Timer structure
typedef struct co_time_timer
{
    /// Pointer to next timer in timer list
    struct co_time_timer *p_next;
    /// Pointer to environment that will be used as callback parameter.
    void *p_env;
    /// Callback to execute in background context upon timer expiration
    co_time_timer_cb cb;
    /// Expiration time [0-31] part (in milliseconds)
    uint32_t exp_time_ms_lsb;
    /// Timer bit field (@see enum co_time_timer_bf)
    uint32_t timer_bf;
} co_time_timer_t;

/// Time Structure
typedef struct co_time
{
    /// Current time [0-31] part (in milliseconds)
    uint32_t ms_lsb;
    /// Current time [32-39] part (in milliseconds)
    uint8_t ms_msb;
} co_time_t;

/*
 * CONSTANT DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Common time module.
 *
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_init(uint8_t init_type);

/*
 ****************************************************************************************
 * Time and timer functions
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve current time in milliseconds.
 *
 * Time value can be either Up-time of the device or Time since the device has been
 * started for the first time.
 *
 * This depends if system is able to compensate power-off time to update internal time value
 *
 * @return current time in milliseconds.
 *
 ****************************************************************************************
 */
co_time_t tlk_bt_ctrl_co_time_get(void);

/**
 ****************************************************************************************
 * @brief Retrieve current time in milliseconds.
 *
 * Compensate device time. This compensation should be done after power-up of the device.
 * The time compensation is automatically performed by device when wake-up from sleep mode.
 * This function shall be called when no timer are programmed at device start-up.
 *
 * @param[in] delta_time_ms_lsb Delta time [0-31] part (in milliseconds)
 * @param[in] delta_time_ms_msb Delta time [32-39] part (in milliseconds)
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_compensate(uint32_t delta_time_ms_lsb, uint8_t delta_time_ms_msb);

/**
 ****************************************************************************************
 * @brief Initialize timer structure.
 *
 * @param[in] p_timer    Pointer to the timer structure.
 * @param[in] cb         Function to be called upon timer expiration.
 * @param[in] p_env      Pointer to be passed to the callback
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_timer_init(co_time_timer_t *p_timer, co_time_timer_cb cb, void *p_env);

/**
 ****************************************************************************************
 * @brief Program a timer to be scheduled in the future.
 *        If timer is already programmed, it is restarted.
 *        If delay is less than 10ms, delay is set to 10ms.
 *
 * @param[in] p_timer    Pointer to the timer structure.
 * @param[in] delay_ms   Duration before expiration of the timer (in milliseconds).
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_timer_set(co_time_timer_t *p_timer, uint32_t delay_ms);

/**
 ****************************************************************************************
 * @brief Program a timer to be scheduled in the future with duration greater than 49 days.
 *        If timer is already programmed, it is restarted.
 *        If delay is less than 10ms, delay is set to 10ms.
 *
 * @param[in] p_timer       Pointer to the timer structure.
 * @param[in] delay_ms_lsb  Duration before expiration of the timer [0-31] part (in milliseconds)
 * @param[in] delay_ms_msb  Duration before expiration of the timer [32-39] part (in milliseconds)
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_timer_long_set(co_time_timer_t *p_timer, uint32_t delay_ms_lsb, uint8_t delay_ms_msb);

/**
 ****************************************************************************************
 * @brief Program a timer to be scheduled periodically. If timer is already programmed,
 *        it is restarted.
 *        If period exceed maximum value, timer is programmed using maximum period.
 *        If period less than 10ms, period is set to 10ms.
 *
 * @param[in] p_timer    Pointer to the timer structure.
 * @param[in] period_ms  Periodic duration (in milliseconds). Range [10, 48388607] max ~2 hours
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_timer_periodic_set(co_time_timer_t *p_timer, uint32_t period_ms);

/**
 ****************************************************************************************
 * @brief Stop a programmed timer.
 *
 * @param[in] p_timer    Pointer to the timer structure.
 ****************************************************************************************
 */
void tlk_bt_ctrl_co_time_timer_stop(co_time_timer_t *p_timer);

/// @} CO_TIME
#endif
#endif // _CO_TIME_H_
