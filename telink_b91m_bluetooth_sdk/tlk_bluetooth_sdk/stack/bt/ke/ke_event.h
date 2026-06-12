/********************************************************************************************************
 * @file    ke_event.h
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


#ifndef _KE_EVENT_H_
#define _KE_EVENT_H_

/**
 ****************************************************************************************
 * @addtogroup EVT Events and Schedule
 * @ingroup KERNEL
 * @brief Event scheduling module.
 *
 * The KE_EVT module implements event scheduling functions. It can be used to
 * implement deferred actions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

//#include "rwip_config.h"          // stack configuration

#include <stdint.h> // standard integer definition

/*
 * CONSTANTS
 ****************************************************************************************
 */


/// Status of ke_task API functions
enum KE_EVENT_STATUS
{
    KE_EVENT_OK = 0,
    KE_EVENT_FAIL,
    KE_EVENT_UNKNOWN,
    KE_EVENT_CAPA_EXCEEDED,
    KE_EVENT_ALREADY_EXISTS,
};

/*
 * TYPE DEFINITION
 ****************************************************************************************
 */


/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Initialize Kernel event module.
 ****************************************************************************************
 */
void tlk_bt_ctrl_ke_event_init(void);

/**
 ****************************************************************************************
 * @brief Register an event callback.
 *
 * @param[in]  event_type       Event type.
 * @param[in]  p_callback       Pointer to callback function.
 *
 * @return                      Status
 ****************************************************************************************
 */
uint8_t tlk_bt_ctrl_ke_event_callback_set(uint8_t event_type, void (*p_callback)(void));

/**
 ****************************************************************************************
 * @brief Set an event
 *
 * This primitive sets one event. It will trigger the call to the corresponding event
 * handler in the next scheduling call.
 *
 * @param[in]  event_type      Event to be set.
 ****************************************************************************************
 */
void tlk_bt_ctrl_ke_event_set(uint8_t event_type);

/**
 ****************************************************************************************
 * @brief Clear an event
 *
 * @param[in]  event_type      Event to be cleared.
 ****************************************************************************************
 */
void tlk_bt_ctrl_ke_event_clear(uint8_t event_type);

/**
 ****************************************************************************************
 * @brief Get the status of an event
 *
 * @param[in]  event_type      Event to get.
 *
 * @return                     Event status (0: not set / 1: set)
 ****************************************************************************************
 */
uint8_t tlk_bt_ctrl_ke_event_get(uint8_t event_type);

/**
 ****************************************************************************************
 * @brief Get all event status
 *
 * @return                     Events bit field
 ****************************************************************************************
 */
uint32_t tlk_bt_ctrl_ke_event_get_all(void);

/**
 ****************************************************************************************
 * @brief Flush all pending events.
 ****************************************************************************************
 */
void tlk_bt_ctrl_ke_event_flush(void);

/**
 ****************************************************************************************
 * @brief Event scheduler entry point.
 *
 * This primitive is the entry point of Kernel event scheduling.
 ****************************************************************************************
 */
void tlk_bt_ctrl_ke_event_schedule(void);


/// @} EVT

#endif //_KE_EVENT_H_
