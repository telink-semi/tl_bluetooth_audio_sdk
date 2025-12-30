/********************************************************************************************************
 * @file    ke_env.h
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


#ifndef _KE_ENV_H_
#define _KE_ENV_H_

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
#include "stack/common/co_list.h"        // kernel queue definition
#include "ke_event.h"                    // kernel event


// forward declaration
struct mblock_free;

/// Kernel environment definition
struct ke_env_tag
{
    /// Queue of sent messages but not yet delivered to receiver
    struct co_list queue_sent;
    /// Queue of messages delivered but not consumed by receiver
    struct co_list queue_saved;
    /// Queue of timers
    struct co_list queue_timer;

    /// Root pointer = pointer to first element of heap linked lists
    struct mblock_free *heap[KE_MEM_BLOCK_MAX];
    /// Size of heaps
    uint16_t heap_size[KE_MEM_BLOCK_MAX];

#if (KE_PROFILING)
    /// Size of heap used
    uint16_t heap_used[KE_MEM_BLOCK_MAX];
    /// Maximum heap memory used
    uint32_t max_heap_used;
#elif (!BLE_HOST_PRESENT)
    /// Memory allocated for Attribute database
    uint16_t resv;
#endif //KE_PROFILING
};

/// Kernel environment
extern struct ke_env_tag ke_env;

/// @} ENV

#endif // _KE_ENV_H_
