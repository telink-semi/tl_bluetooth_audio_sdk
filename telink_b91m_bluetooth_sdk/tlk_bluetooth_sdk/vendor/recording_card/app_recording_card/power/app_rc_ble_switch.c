/********************************************************************************************************
 * @file    app_rc_ble_switch.c
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
#include "tlkmw/tlkmw.h"
#include "../app_recording_card_api.h"
#include "../../ble/app_ble.h"

#define APP_RC_BLE_SWITCH_LOG_EN 0

enum
{
    APP_RC_BLE_CLOSED = 0,
    APP_RC_BLE_CLOSING,
    APP_RC_BLE_OPENED,
    APP_RC_BLE_OPENING,
};

static uint8_t sAppRcBleState = APP_RC_BLE_OPENED;

static void app_rc_ble_close_core_finished(void)
{
    //new version no need close n22
    // tlkmw_host_closeController();
    tlksys_enter_critical();
    sAppRcBleState = APP_RC_BLE_CLOSED;
    tlksys_leave_critical();
    tlkapi_printf(APP_RC_BLE_SWITCH_LOG_EN, "**ble closed");
}

static void app_rc_ble_close_core(void)
{
    app_ble_close_host(app_rc_ble_close_core_finished);
}

static void app_rc_ble_open_core(void)
{
    //new version no need close n22
    // tlkmw_host_resetController();
    // if(tlkos_task_delayMs(2) == -TLK_ENOSUPPORT){
    //     delay_ms(2);
    // }
    app_ble_open_host();
    tlksys_enter_critical();
    sAppRcBleState = APP_RC_BLE_OPENED;
    tlksys_leave_critical();
    tlkapi_printf(APP_RC_BLE_SWITCH_LOG_EN, "**ble opened");
}

void app_rc_ble_close(void)
{
    tlksys_enter_critical();
    if (sAppRcBleState != APP_RC_BLE_OPENED) {
        tlksys_leave_critical();
        return;
    }
    sAppRcBleState = APP_RC_BLE_CLOSING;
    tlksys_leave_critical();
    tlksys_runFuncInTask(TLKSYS_TASKID_HOST, app_rc_ble_close_core);
}

void app_rc_ble_open(void)
{
    tlksys_enter_critical();
    if (sAppRcBleState != APP_RC_BLE_CLOSED) {
        tlksys_leave_critical();
        return;
    }
    sAppRcBleState = APP_RC_BLE_OPENING;
    tlksys_leave_critical();
    tlksys_runFuncInTask(TLKSYS_TASKID_HOST, app_rc_ble_open_core);
}

bool app_rc_is_ble_open(void)
{
    return sAppRcBleState == APP_RC_BLE_OPENED || APP_RC_BLE_OPENED == APP_RC_BLE_OPENING;
}