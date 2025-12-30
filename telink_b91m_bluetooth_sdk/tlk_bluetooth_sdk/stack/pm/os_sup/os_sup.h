/********************************************************************************************************
 * @file    os_sup.h
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
#ifndef OS_SUP_H_
#define OS_SUP_H_

#include "tl_common.h"

/**
 *  @brief  Define the prototypes that the os_give_sem_t must conform to.
 */
typedef void (*os_give_sem_t)(void);

/**
 *  @brief  Define the prototypes that the take_mutex_sem&give_mutex_sem must conform to.
 */
typedef void (*os_mutex_sem_t)(void);

/**
 * @brief       This feature is used to enable support for OS mode
 * @param[in]   true - Enable, false - disable
 * @return      none
 */
void tlksdk_os_setOsSupEnable(bool en);

/**
 * @brief       This function is to check if OS is supported
 * @param[in]   none
 * @return      true - Enable, false - disable
 */
bool tlksdk_os_isOsSupEnable(void);

/**
 * @brief      Register an input for the Bluetooth OS semaphore
 * @param[in]  give_sem_from_isr -Interrupt  use
 * @param[in]  give_sem - Non-interruptible  use
 * @return
 */
void tlksdk_os_registerGiveSemCb(os_give_sem_t give_sem_from_isr, os_give_sem_t give_sem);

/**
 * @brief      Register a Bluetooth send data mutex.
 * Handling multitasking while sending packets results in an exception.
 * @param[in]  take_mutex_sem    -  lock
 * @param[in]  give_mutex_sem    -  unlock
 * @return
 */
void tlksdk_os_registerMutexSemCb(os_mutex_sem_t take_mutex_sem, os_mutex_sem_t give_mutex_sem);

#endif /* OS_SUP_H_ */
