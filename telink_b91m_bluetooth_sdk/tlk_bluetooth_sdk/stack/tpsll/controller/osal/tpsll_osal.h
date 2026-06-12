/********************************************************************************************************
 * @file    tpsll_osal.h
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
#ifndef TLKSTK_TPSLL_CONTROLLER_OSAL_TPSLL_OSAL_H_
#define TLKSTK_TPSLL_CONTROLLER_OSAL_TPSLL_OSAL_H_

/*======================================================
 * Critical Section
 *======================================================*/

/**
 * @brief Enter a critical section (disable interrupts / schedule).
 * @return The previous state / lock value, which should be passed to exit_critical.
 */
uint32_t tlk_tpsll_osal_enter_critical(void);

/**
 * @brief Exit a critical section (restore interrupts / schedule).
 * @param[in] lock_val - The value returned by enter_critical.
 */
void tlk_tpsll_osal_exit_critical(uint32_t lock_val);

/*======================================================
 * Delay / Yield
 *======================================================*/

/**
 * @brief Delay for a specified number of milliseconds.
 *        In bare-metal, this may be a busy-wait or a no-op.
 * @param[in] ms - Milliseconds to delay.
 */
void tlk_tpsll_osal_delay_ms(uint32_t ms);

/**
 * @brief Delay for a specified number of microseconds.
 *        In bare-metal, this may be a busy-wait or a no-op.
 * @param[in] us - Microseconds to delay.
 */
void tlk_tpsll_osal_delay_us(uint32_t us);

/**
 * @brief Yield the current context (task / thread).
 *        In bare-metal, this is typically a no-op.
 */
void tlk_tpsll_osal_yield(void);

/*======================================================
 * Semaphore
 *======================================================*/

/**
 * @brief Create a binary semaphore.
 * @return Handle to the semaphore, or NULL on failure.
 */
void *tlk_tpsll_osal_sem_create(void);

/**
 * @brief Delete a semaphore.
 * @param[in] sem - Semaphore handle.
 */
void tlk_tpsll_osal_sem_delete(void *sem);

/**
 * @brief Take (wait) a semaphore.
 * @param[in] sem     - Semaphore handle.
 * @param[in] timeout - Timeout in milliseconds, 0 for non-blocking, 0xFFFFFFFF for forever.
 * @return 0 on success, non-zero on timeout / error.
 */
int tlk_tpsll_osal_sem_take(void *sem, uint32_t timeout);

/**
 * @brief Give (signal) a semaphore.
 * @param[in] sem - Semaphore handle.
 * @return 0 on success, non-zero on error.
 */
int tlk_tpsll_osal_sem_give(void *sem);

/*======================================================
 * Mutex
 *======================================================*/

/**
 * @brief Create a mutex.
 * @return Handle to the mutex, or NULL on failure.
 */
void *tlk_tpsll_osal_mutex_create(void);

/**
 * @brief Delete a mutex.
 * @param[in] mutex - Mutex handle.
 */
void tlk_tpsll_osal_mutex_delete(void *mutex);

/**
 * @brief Lock a mutex.
 * @param[in] mutex   - Mutex handle.
 * @param[in] timeout - Timeout in milliseconds, 0 for non-blocking, 0xFFFFFFFF for forever.
 * @return 0 on success, non-zero on timeout / error.
 */
int tlk_tpsll_osal_mutex_lock(void *mutex, uint32_t timeout);

/**
 * @brief Unlock a mutex.
 * @param[in] mutex - Mutex handle.
 * @return 0 on success, non-zero on error.
 */
int tlk_tpsll_osal_mutex_unlock(void *mutex);

/*======================================================
 * Memory
 *======================================================*/

/**
 * @brief Allocate memory.
 * @param[in] size - Number of bytes to allocate.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
void *tlk_tpsll_osal_malloc(uint32_t size);

/**
 * @brief Free allocated memory.
 * @param[in] ptr - Pointer to the memory to free.
 */
void tlk_tpsll_osal_free(void *ptr);

/*======================================================
 * Software Timer
 *======================================================*/

/**
 * @brief Timer callback function prototype.
 */
typedef void (*tlk_tpsll_osal_timer_cb_t)(void *arg);

/**
 * @brief Create a software timer.
 * @param[in] cb  - Callback function to be invoked when timer expires.
 * @param[in] arg - User argument passed to the callback.
 * @return Handle to the timer, or NULL on failure.
 */
void *tlk_tpsll_osal_timer_create(tlk_tpsll_osal_timer_cb_t cb, void *arg);

/**
 * @brief Delete a software timer.
 * @param[in] timer - Timer handle.
 */
void tlk_tpsll_osal_timer_delete(void *timer);

/**
 * @brief Start / restart a software timer.
 * @param[in] timer - Timer handle.
 * @param[in] ms    - Timeout in milliseconds.
 * @return 0 on success, non-zero on error.
 */
int tlk_tpsll_osal_timer_start(void *timer, uint32_t ms);

/**
 * @brief Stop a software timer.
 * @param[in] timer - Timer handle.
 * @return 0 on success, non-zero on error.
 */
int tlk_tpsll_osal_timer_stop(void *timer);

#endif /* TLKSTK_TPSLL_CONTROLLER_OSAL_TPSLL_OSAL_H_ */
