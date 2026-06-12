/********************************************************************************************************
 * @file    tpsll_osal.c
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2026
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
#include "tpsll_osal.h"

/*======================================================
 * Helper: non-NULL dummy handle for bare-metal stubs
 *======================================================*/
static int g_tpsll_osal_dummy_handle;
#define TPSLL_OSAL_DUMMY_HANDLE ((void *)&g_tpsll_osal_dummy_handle)

/*======================================================
 * Critical Section
 *======================================================*/

__attribute__((weak)) uint32_t tlk_tpsll_osal_enter_critical(void)
{
    return 0;
}

__attribute__((weak)) void tlk_tpsll_osal_exit_critical(uint32_t lock_val)
{
    (void)lock_val;
}

/*======================================================
 * Delay / Yield
 *======================================================*/

__attribute__((weak)) void tlk_tpsll_osal_delay_ms(uint32_t ms)
{
    (void)ms;
    /* Bare-metal no-op: caller is expected to poll or use hw timer */
}

__attribute__((weak)) void tlk_tpsll_osal_delay_us(uint32_t us)
{
    (void)us;
    /* Bare-metal no-op */
}

__attribute__((weak)) void tlk_tpsll_osal_yield(void)
{
    /* Bare-metal: no task scheduler to yield to */
}

/*======================================================
 * Semaphore
 *======================================================*/

__attribute__((weak)) void *tlk_tpsll_osal_sem_create(void)
{
    return TPSLL_OSAL_DUMMY_HANDLE;
}

__attribute__((weak)) void tlk_tpsll_osal_sem_delete(void *sem)
{
    (void)sem;
}

__attribute__((weak)) int tlk_tpsll_osal_sem_take(void *sem, uint32_t timeout)
{
    (void)sem;
    (void)timeout;
    /* Always succeed in bare-metal */
    return 0;
}

__attribute__((weak)) int tlk_tpsll_osal_sem_give(void *sem)
{
    (void)sem;
    return 0;
}

/*======================================================
 * Mutex
 *======================================================*/

__attribute__((weak)) void *tlk_tpsll_osal_mutex_create(void)
{
    return TPSLL_OSAL_DUMMY_HANDLE;
}

__attribute__((weak)) void tlk_tpsll_osal_mutex_delete(void *mutex)
{
    (void)mutex;
}

__attribute__((weak)) int tlk_tpsll_osal_mutex_lock(void *mutex, uint32_t timeout)
{
    (void)mutex;
    (void)timeout;
    return 0;
}

__attribute__((weak)) int tlk_tpsll_osal_mutex_unlock(void *mutex)
{
    (void)mutex;
    return 0;
}

/*======================================================
 * Memory
 *======================================================*/

__attribute__((weak)) void *tlk_tpsll_osal_malloc(uint32_t size)
{
    (void)size;
    return 0;
}

__attribute__((weak)) void tlk_tpsll_osal_free(void *ptr)
{
    (void)ptr;
}

/*======================================================
 * Software Timer
 *======================================================*/

__attribute__((weak)) void *tlk_tpsll_osal_timer_create(tlk_tpsll_osal_timer_cb_t cb, void *arg)
{
    (void)cb;
    (void)arg;
    return TPSLL_OSAL_DUMMY_HANDLE;
}

__attribute__((weak)) void tlk_tpsll_osal_timer_delete(void *timer)
{
    (void)timer;
}

__attribute__((weak)) int tlk_tpsll_osal_timer_start(void *timer, uint32_t ms)
{
    (void)timer;
    (void)ms;
    return 0;
}

__attribute__((weak)) int tlk_tpsll_osal_timer_stop(void *timer)
{
    (void)timer;
    return 0;
}