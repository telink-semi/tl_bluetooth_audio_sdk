/********************************************************************************************************
 * @file    rwip_int.h
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

#ifndef _RWIP_INT_H_
#define _RWIP_INT_H_

/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @brief Entry points of the RW IP stacks/modules
 *
 * This module contains the primitives that allow an application accessing and running the
 * RW IP protocol stacks / modules.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h" // stack configuration

#include <stdint.h>  // standard integer definitions
#include <stdbool.h> // standard boolean definitions

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

#if (CHIP_TYPE == CHIP_TYPE_TL752X)
struct rwip_env_tag
{
#if (BLE_EMB_PRESENT && BLE_ISO_PRESENT)
    rwip_iso_timer_t iso_timer;
#endif // (BLE_EMB_PRESENT && BLE_ISO_PRESENT)

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Arbiter target timer  (integer part, in half slots)
    uint32_t timer_arb_target;
    /// Alarm target timer (integer part, in half slots)
    uint32_t timer_alarm_target;
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Common target timer (in half slots)
    uint32_t timer_co_target;
    /// Last Sampled time (used for time conversion)
    rwip_time_t last_samp_time;

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Contains sleep duration accumulated timing error (32kHz: 1/2 half us | 32.768kHz: 1/256 half-us)
    uint32_t sleep_acc_error;
    /// Power_up delay (in LP clock cycle unit, depends on Low power clock frequency)
    uint32_t lp_cycle_wakeup_delay;
    /// Duration of sleep and wake-up algorithm (depends on CPU speed) expressed in half us.
    uint16_t sleep_algo_dur;
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Prevent sleep bit field
    uint16_t prevent_sleep;
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// External wake-up support
    bool ext_wakeup_enable;
#if (!BLE_EMB_PRESENT)
    /// BTS sampling clock half microseconds residual (0 or 1)
    uint8_t samp_hus_residual;
#endif // (!BLE_EMB_PRESENT)
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
};
#else
/// RWIP Environment structure
struct rwip_env_tag
{
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Half slot target timer (in half slots)
    uint32_t timer_hs_target;
    /// Half us target timer (in half us)
    uint32_t timer_hus_target;
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// 10 ms target timer (in half slots)
    uint32_t timer_10ms_target;
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Contains sleep duration accumulated timing error (32kHz: 1/2 half us | 32.768kHz: 1/256 half-us)
    uint32_t sleep_acc_error;
    /// Power_up delay (in LP clock cycle unit, depends on Low power clock frequency)
    uint32_t lp_cycle_wakeup_delay;
    /// Duration of sleep and wake-up algorithm (depends on CPU speed) expressed in half us.
    uint16_t sleep_algo_dur;
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Prevent sleep bit field
    uint16_t prevent_sleep;
    uint8_t  resv0[2];
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// External wake-up support
    bool ext_wakeup_enable;

    uint8_t resv1;
#endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
};

#endif
/*
 * GLOBAL DEFINITIONS
 ****************************************************************************************
 */

/// RW SW environment
extern struct rwip_env_tag tlk_bt_ctrl_g_rwip_env;


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 * Initialization of the RW IP Common core driver
 *
 * @param reset true if reset requested, false for a boot
 */
void rwip_driver_init(bool reset);


///@} ROOT

#endif // _RWIP_INT_H_
