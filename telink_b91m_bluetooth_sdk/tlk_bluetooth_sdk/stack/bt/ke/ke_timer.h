/********************************************************************************************************
 * @file    ke_timer.h
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


#ifndef _KE_TIMER_H_
#define _KE_TIMER_H_

/**
 ****************************************************************************************
 * @defgroup TIMER BT Time
 * @ingroup KERNEL
 * @brief Timer management module.
 *
 * This module implements the functions used for managing kernel timers.
 *
 ****************************************************************************************
 */
#include "stack/bt/common/rwip_config.h" // stack configuration
#include "ke_msg.h"                      // messaging definition

#if (CHIP_TYPE == CHIP_TYPE_TL752X)
/*
 * DEFINITIONS
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief This function flushes all timers pending in the kernel.
 *
 ****************************************************************************************
 */
void ke_timer_flush(void);

/**
 ****************************************************************************************
 * @brief Set a timer.
 *
 * The function first cancel the timer if it is already existing, then
 * it creates a new one. The timer can be one-shot or periodic, i.e. it
 * will be automatically set again after each trigger.
 *
 * When the timer expires, a message is sent to the task provided as
 * argument, with the timer id as message id.
 *
 *
 * @param[in] timer_id      Timer identifier (message identifier type).
 * @param[in] task_id       Task identifier which will be notified
 * @param[in] delay         Delay in time milliseconds.
 ****************************************************************************************
 */
void ke_timer_set(ke_msg_id_t const timer_id, ke_task_id_t const task, uint32_t delay_ms);

/**
 ****************************************************************************************
 * @brief Remove an registered timer.
 *
 * This function search for the timer identified by its id and its task id.
 * If found it is stopped and freed, otherwise an error message is returned.
 *
 * @param[in] timer_id  Timer identifier.
 * @param[in] task      Task identifier.
 ****************************************************************************************
 */
void ke_timer_clear(ke_msg_id_t const timerid, ke_task_id_t const task);

/**
 ****************************************************************************************
 * @brief Checks if a requested timer is active.
 *
 * This function pops the first timer from the timer queue and notifies the appropriate
 * task by sending a kernel message. If the timer is periodic, it is set again;
 * if it is one-shot, the timer is freed. The function checks also the next timers
 * and process them if they have expired or are about to expire.
 ****************************************************************************************
 */
bool ke_timer_active(ke_msg_id_t const timer_id, ke_task_id_t const task_id);



/**
 ****************************************************************************************
 * @brief Initialize Kernel timer module.
 ****************************************************************************************
 */
void ke_timer_init(void);

#else
/*
 * DEFINITIONS
 ****************************************************************************************
 */

///  Convert timer from second to timer accuracy (10ms)
#define KE_TIME_IN_SEC(_time) (_time * 100)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Timer Object
struct ke_timer
{
    /// next ke timer
    struct ke_timer *next;
    /// message identifier
    ke_msg_id_t id;
    /// task identifier
    ke_task_id_t task;
    /// time value
    uint32_t time;
};

typedef enum
{
    TEST_TO,

} TLK_KE_TIMER_TYPE_ENUM;

/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Initialize Kernel timer module.
 ****************************************************************************************
 */
void ke_timer_init(void);

/**
 ****************************************************************************************
 * @brief Retrieve kernel time.
 *
 * @return time value (in Kernel time (10 ms))
 ****************************************************************************************
 */
uint32_t ke_time_get(void);

/**
 ****************************************************************************************
 * @brief Set a timer.
 *
 * The function first cancel the timer if it is already existing, then
 * it creates a new one. The timer can be one-shot or periodic, i.e. it
 * will be automatically set again after each trigger.
 *
 * When the timer expires, a message is sent to the task provided as
 * argument, with the timer id as message id.
 *
 * The timer is programmed in time units (TU is 10ms).
 *
 * @param[in] timer_id      Timer identifier (message identifier type).
 * @param[in] task_id       Task identifier which will be notified
 * @param[in] delay         Delay in time units in 10ms.
 ****************************************************************************************
 */
void ke_timer_set(ke_msg_id_t const timer_id, ke_task_id_t const task, uint32_t delay);

/**
 ****************************************************************************************
 * @brief Remove an registered timer.
 *
 * This function search for the timer identified by its id and its task id.
 * If found it is stopped and freed, otherwise an error message is returned.
 *
 * @param[in] timer_id  Timer identifier.
 * @param[in] task      Task identifier.
 ****************************************************************************************
 */
void ke_timer_clear(ke_msg_id_t const timerid, ke_task_id_t const task);

/**
 ****************************************************************************************
 * @brief Checks if a requested timer is active.
 *
 * This function pops the first timer from the timer queue and notifies the appropriate
 * task by sending a kernel message. If the timer is periodic, it is set again;
 * if it is one-shot, the timer is freed. The function checks also the next timers
 * and process them if they have expired or are about to expire.
 ****************************************************************************************
 */
bool ke_timer_active(ke_msg_id_t const timer_id, ke_task_id_t const task_id);

/**
 ****************************************************************************************
 * @brief Adjust all kernel timers by specified adjustment delay.
 *
 * This function updates all timers to align to a new SCLK after a system clock adjust.
 ****************************************************************************************
 */
void ke_timer_adjust_all(uint32_t delay);

/**
 ****************************************************************************************
 * @brief Compute the authorized sleep time until next kernel timer event
 * 
 * The function takes as argument the current time to compute the sleep time.
 * 
 * if some timer are programmed it return the sleep time between current time and 
 * first timer expiration.
 * 
 * If there is no timer programmed, function return false meaning that platform should not 
 * consider the kernel timer to decide the sleep time duration.
 * 
 * @param[in]     current_time   Current system time
 * @param[out]    sleep_time     Computed sleep duration
 *
 * @return true sleep time has been computed ; false if there is no programed timer, 
           meaning that sleep_time is not relevant and can be ignored.
 ****************************************************************************************
 */
bool ke_sleep_time_get(uint32_t current_time, int32_t *sleep_time);

/// @} TIMER

#endif
#endif // _KE_TIMER_H_
