/********************************************************************************************************
 * @file    sch_alarm.h
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


#ifndef _SCH_ALARM_H_
#define _SCH_ALARM_H_

/**
 ****************************************************************************************
 * @addtogroup SCH
 * @brief Entry points of the Scheduling alaram module
 *
 * @{
 ****************************************************************************************
 */

#include "stack/bt/common/rwip_config.h" // stack configuration

#include <stdint.h>  // integer
#include <stdbool.h> // boolean

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * DEFINITIONS
 ****************************************************************************************
 */

/// SCH_ALARM error codes
enum sch_alarm_error
{
    /// SCH_ALARM request succeed
    SCH_ALARM_ERROR_OK = 0,
    /// SCH_ALARM request rejected error
    SCH_ALARM_ERROR_REJECTED,
    /// SCH_ALARM element not found error
    SCH_ALARM_ERROR_NOT_FOUND,
    /// SCH_ALARM request rejected due to bandwidth full error
    SCH_ALARM_ERROR_BW_FULL
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Alarm element structure
struct sch_alarm_tag
{
    /// List element for chaining in the Even Arbiter lists
    struct co_list_hdr hdr;

    /// Timestamp of alarm expiry (in BT half-slots)
    uint32_t timestamp;

    /// Call back function invoked upon alarm expiry
    void (*cb_alarm)(struct sch_alarm_tag *);
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the Scheduling alarm.
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_alarm_init(bool reset);

/**
 ****************************************************************************************
 * @brief Alarm interrupt handler
 *
 * This function is called under interrupt when an alarm is triggered.
 *
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_alarm_timer_isr(void);

/**
 ****************************************************************************************
 * @brief Set an alarm
 *
 * If the alarm specify an instant in the past, the alarm is scheduled for earliest
 * possible time.
 *
 * @param[in] elt      Pointer to the alarm element to be programmed
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_alarm_set(struct sch_alarm_tag *elt);

/**
 ****************************************************************************************
 * @brief Clear an alarm
 *
 * @param[in] elt      Pointer to the alarm element to be cleared
 *
 * @return 0 - no error / 1:255 - error
 ****************************************************************************************
 */
uint8_t tlk_bt_ctrl_sch_alarm_clear(struct sch_alarm_tag *elt);

///@} SCH_ALARM

#endif // _SCH_ALARM_H_
