/********************************************************************************************************
 * @file    tlkapp_host_le_adapt.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "stack/ble/host_v1/inc/ble_host_sal.h"
#if TLK_STK_BLE_ENABLE

/**
 * @brief       Enters the critical section for BLE host SAL.
 * @param[in]   none.
 * @return      none.
 */
void ble_host_sal_os_enter_critical_section(void)
{
    tlksys_enter_critical();
}

/**
 * @brief       Exits the critical section for BLE host SAL.
 * @param[in]   none.
 * @return      none.
 */
void ble_host_sal_os_exit_critical_section(void)
{
    tlksys_leave_critical();
}

/**
 * @brief       Wakes up the BLE host SAL.
 * @param[in]   none.
 * @return      none.
 */
void ble_host_sal_os_wakeup(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_LE_EVT);
}

/**
 * @brief       Puts the BLE host SAL to sleep.
 * @param[in]   none.
 * @return      none.
 */
void ble_host_sal_os_sleep(void)
{
    //do nothing,host thread will auto sleep(block)
}

/**
 * @brief       Creates a timer for BLE host SAL.
 * @param[in]   cb          - Callback function for the timer.
 * @param[in]   arg         - Argument to pass to the callback function.
 * @param[in]   timeout_ms  - Timeout in milliseconds.
 * @param[out]  timer_hdl   - Handle to the created timer.
 * @return      Returns true on success, false otherwise.
 */
bool ble_host_sal_timer_create(ble_host_sal_timer_callback cb, void *arg, uint32_t timeout_ms, ble_host_sal_timer_handle_t *timer_hdl)
{
    int res = tlksys_timer_create(TLKSYS_TASKID_HOST, timer_hdl, timeout_ms * 1000, false, cb, arg);
    return res == TLK_ENONE;
}

/**
 * @brief       Updates the timeout of a BLE host SAL timer.
 * @param[in]   timer_hdl   - Handle to the timer.
 * @param[in]   timeout_ms  - New timeout in milliseconds.
 * @return      Returns true on success, false otherwise.
 */
bool ble_host_sal_timer_update_timeout(ble_host_sal_timer_handle_t timer_hdl, uint32_t timeout_ms)
{
    int res = tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, timer_hdl, timeout_ms * 1000);
    return res == TLK_ENONE;
}

/**
 * @brief       Starts a BLE host SAL timer.
 * @param[in]   timer_hdl   - Handle to the timer.
 * @return      Returns true on success, false otherwise.
 */
bool ble_host_sal_timer_start(ble_host_sal_timer_handle_t timer_hdl)
{
    int res = tlksys_timer_start(TLKSYS_TASKID_HOST, timer_hdl);
    return res == TLK_ENONE;
}

/**
 * @brief       Stops a BLE host SAL timer.
 * @param[in]   timer_hdl   - Handle to the timer.
 * @return      Returns true on success, false otherwise.
 */
bool ble_host_sal_timer_stop(ble_host_sal_timer_handle_t timer_hdl)
{
    int res = tlksys_timer_stop(TLKSYS_TASKID_HOST, timer_hdl);
    return res == TLK_ENONE;
}

/**
 * @brief       Deletes a BLE host SAL timer.
 * @param[in]   timer_hdl   - Handle to the timer.
 * @return      Returns true on success, false otherwise.
 */
bool ble_host_sal_timer_delete(ble_host_sal_timer_handle_t timer_hdl)
{
    int res = tlksys_timer_destroy(TLKSYS_TASKID_HOST, timer_hdl);
    return res == TLK_ENONE;
}

#endif
