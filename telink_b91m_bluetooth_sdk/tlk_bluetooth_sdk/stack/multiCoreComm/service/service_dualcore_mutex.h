/********************************************************************************************************
 * @file    service_dualcore_mutex.h
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
#pragma once

#ifndef TLKIPC_MUTEX_LOCK_ENABLE
#define TLKIPC_MUTEX_LOCK_ENABLE 0
#endif

#if TLKIPC_MUTEX_LOCK_ENABLE
#define TLKIPC_MUTEX_LOCK   tlkipc_mutex_lock(0)
#define TLKIPC_MUTEX_UNLOCK tlkipc_mutex_unlock(0)
#else
#define TLKIPC_MUTEX_LOCK
#define TLKIPC_MUTEX_UNLOCK
#endif

typedef enum
{
    TLKIPC_MUTEX_VCD = 0,
    TLKIPC_MUTEX_NUM
} tlkipc_mutex_type_e;

/**
 * @brief     Locks a mutex.
 * @param[in] mutex_id refer to tlkipc_mutex_type_e.
 */
void tlkipc_mutex_lock(uint32_t mutex_id);

/**
 * @brief     Unlocks a mutex.
 * @param[in] mutex_id refer to tlkipc_mutex_type_e.
 */
void tlkipc_mutex_unlock(uint32_t mutex_id);

uint32_t tlkipc_mutex_getCtrlBlockAddr(void);

void tlkipc_mutex_syncCtrlBlockAddrHandler(uint32_t addr);
