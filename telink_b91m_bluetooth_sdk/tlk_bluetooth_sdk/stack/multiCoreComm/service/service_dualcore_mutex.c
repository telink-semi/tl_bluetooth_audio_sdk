/********************************************************************************************************
 * @file    service_dualcore_mutex.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "driver.h"
#include "service_dualcore_mutex.h"

enum
{
    TLKIPC_MUTEX_OWNER_NONE = 0,
    TLKIPC_MUTEX_OWNER_MAIN_CORE,
    TLKIPC_MUTEX_OWNER_CONTROLLER_CORE,
};

typedef struct
{
    volatile uint32_t owner;
    volatile uint32_t ticket;
} tlkipc_mutex_t;

#if MCU_DUAL_CORE_ENABLE

#if !defined(MCU_CORE_N22)
__attribute__((aligned(4))) static tlkipc_mutex_t s_tlkipc_mutexes[TLKIPC_MUTEX_NUM];

static inline void tlkipc_cpu_wait(void)
{
    unsigned long long start = rdmcycle();
    while (rdmcycle() - start < 60);
    //maincore too fast
}
#else
static tlkipc_mutex_t *volatile sp_tlkipc_mutexes = NULL;

static inline void tlkipc_cpu_wait(void) {}
#endif

_attribute_ram_code_ void tlkipc_mutex_lock(uint32_t mutex_id)
{
#if !defined(MCU_CORE_N22)
    tlkipc_mutex_t *mutex = &s_tlkipc_mutexes[mutex_id];
    uint32_t        my_id = TLKIPC_MUTEX_OWNER_MAIN_CORE;
#else
    tlkipc_mutex_t *mutex = &sp_tlkipc_mutexes[mutex_id];
    uint32_t        my_id = TLKIPC_MUTEX_OWNER_CONTROLLER_CORE;
#endif
    uint32_t ticket;
    do {
        while (mutex->owner != TLKIPC_MUTEX_OWNER_NONE);
        mutex->owner = my_id;
        ticket       = mutex->ticket;
        tlkipc_cpu_wait();
    } while (mutex->owner != my_id || mutex->ticket != ticket);
    mutex->ticket++;
}

_attribute_ram_code_ void tlkipc_mutex_unlock(uint32_t mutex_id)
{
#if !defined(MCU_CORE_N22)
    tlkipc_mutex_t *mutex = &s_tlkipc_mutexes[mutex_id];
#else
    tlkipc_mutex_t *mutex = &sp_tlkipc_mutexes[mutex_id];
#endif
    mutex->owner = TLKIPC_MUTEX_OWNER_NONE;
}


#else // #if MCU_DUAL_CORE_ENABLE

void tlkipc_mutex_lock(uint32_t mutex_id)
{
    (void)mutex_id;
}

void tlkipc_mutex_unlock(uint32_t mutex_id)
{
    (void)mutex_id;
}
#endif

#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
uint32_t tlkipc_mutex_getCtrlBlockAddr(void)
{
    return (uint32_t)s_tlkipc_mutexes;
}
#endif

#if (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
void tlkipc_mutex_syncCtrlBlockAddrHandler(uint32_t addr)
{
    sp_tlkipc_mutexes = (tlkipc_mutex_t *)addr;
}
#endif
