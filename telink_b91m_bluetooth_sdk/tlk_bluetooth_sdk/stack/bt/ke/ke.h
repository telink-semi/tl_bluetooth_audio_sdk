/********************************************************************************************************
 * @file    ke.h
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


#ifndef _KE_H_
#define _KE_H_

#define LC_HCI_LINK_TEST 1
#define LC_LMP_LINK_TEST 1
/**
 ****************************************************************************************
 * @addtogroup ENV Environment
 * @ingroup KERNEL
 * @brief Kernel Environment
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "stack/bt/common/rwip_config.h" // stack configuration
#include "ke_env.h"                      // kernel environment
#include "ke_mem.h"                      // kernel memory
#include "ke_msg.h"                      // kernel message
#include "ke_event.h"                    // kernel event
#include "ke_queue.h"                    // kernel queue
#include "ke_task.h"                     // kernel task
#include "ke_timer.h"                    // kernel timer
#include <stdbool.h>                     // standard boolean definitions
#include <stdint.h>                      // standard integer definitions

/*
 * ENUMERATION
 ****************************************************************************************
 */

/// Kernel Error Status
enum KE_STATUS
{
    KE_SUCCESS = 0,
    KE_FAIL
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief This function performs all the initializations of the kernel.
 *
 * It initializes first the heap, then the message queues and the events. Then if required
 * it initializes the trace.
 *
 ****************************************************************************************
 */
void ke_init(void);

/**
 ****************************************************************************************
 * @brief This function flushes all messages, timers and events currently pending in the
 * kernel.
 *
 ****************************************************************************************
 */
void ke_flush(void);

/**
 ****************************************************************************************
 * @brief This function checks if sleep is possible or kernel is processing
 *
 * @return      True if sleep is allowed, false otherwise
 ****************************************************************************************
 */
bool ke_sleep_check(void);

#if (KE_PROFILING)
/**
 ****************************************************************************************
 * @brief This function gets the statistics of the kernel usage.
 *
 * @param[out]   max_msg_sent      Max message sent
 * @param[out]   max_msg_saved     Max message saved
 * @param[out]   max_timer_used    Max timer used
 * @param[out]   max_heap_used     Max heap used
 ****************************************************************************************
 */
enum KE_STATUS ke_stats_get(uint8_t *max_msg_sent, uint8_t *max_msg_saved, uint8_t *max_timer_used, uint16_t *max_heap_used);
#endif //KE_PROFILING

/// @} KE

#endif // _KE_H_
