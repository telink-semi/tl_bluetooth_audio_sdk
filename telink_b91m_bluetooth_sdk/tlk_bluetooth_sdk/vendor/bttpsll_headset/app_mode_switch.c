/********************************************************************************************************
 * @file    app_mode_switch.c
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
#if TLK_STK_BT_ENABLE
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "tlkmw/ble/le_audio/inc/lea_us.h"
#include "tlkapp/tlkapp.h"
#define APP_SWITCH_LOG_EN 1

typedef struct
{
    uint32_t taskID;
    void (*func)(void);
} app_switch_func_t;

typedef enum
{
    APP_MODE_BT_TPH,
    APP_MODE_LEA,
    APP_MODE_UAC,
    APP_MODE_MAX,
} app_mode_e;

typedef struct
{
    uint8_t busy            : 1;
    uint8_t need_timer      : 1;
    uint8_t out_finish      : 1;
    uint8_t in_finish       : 1;
    uint8_t bt_idle_finish  : 1;
    uint8_t tph_idle_finish : 1;
    uint8_t resv            : 2;
    uint8_t mode;
} app_mode_switch_t;

static TlkApiTimer_t     app_mode_switch_timer_handle = {0};
static app_mode_switch_t app_mode_switch_ctrl         = {0};

//all thread safe

/**
 * @brief      Wake up the mode switching task
 * @param[in]  none
 * @return     none
 */
static void app_switch_mode_task_wake_up(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_APP_0);
}

/**
 * @brief      Timer callback for mode switching operations
 * @param[in]  pTimer  - pointer to timer handle
 * @param[in]  userArg - user argument (unused)
 * @return     none
 */
static void app_mode_switch_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    if (tlkmdi_tone_is_busy()) {
        tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, pTimer);
        return;
    }
    app_mode_switch_ctrl.out_finish = 1;
    app_switch_mode_task_wake_up();
}

/**
 * @brief      Callback function when host stop is finished
 * @param[in]  none
 * @return     none
 */
static void app_host_stop_finished_cb(void)
{
    app_mode_switch_ctrl.need_timer = 1;
    app_switch_mode_task_wake_up();
}

/**
 * @brief      Callback function when BT host stop is finished
 * @param[in]  none
 * @return     none
 */
static void app_bt_host_stop_finished_cb(void)
{
    if (app_mode_switch_ctrl.tph_idle_finish) {
        app_mode_switch_ctrl.tph_idle_finish = 0;
        app_host_stop_finished_cb();
        return;
    }
    app_mode_switch_ctrl.bt_idle_finish = 1;
}

/**
 * @brief      Callback function when TPH host stop is finished
 * @param[in]  none
 * @return     none
 */
static void app_tph_host_stop_finished_cb(void)
{
    if (app_mode_switch_ctrl.bt_idle_finish) {
        app_mode_switch_ctrl.bt_idle_finish = 0;
        app_host_stop_finished_cb();
        return;
    }
    app_mode_switch_ctrl.tph_idle_finish = 1;
}

/**
 * @brief      Callback function when LEA host stop is finished
 * @param[in]  none
 * @return     none
 */
static void app_lea_host_stop_finished_cb(void)
{
    app_host_stop_finished_cb();
}

/**
 * @brief      Switch to BT/TPH mode
 * @param[in]  none
 * @return     none
 */
static void app_bt_tph_mode_switch_in(void)
{
    tlkapp_host_bt_taskStartHook();
    tlkmdi_bt_tph_restart();
    app_mode_switch_ctrl.in_finish = 1;
    app_switch_mode_task_wake_up();
}

/**
 * @brief      Switch out of BT/TPH mode
 * @param[in]  none
 * @return     none
 */
static void app_bt_tph_mode_switch_out(void)
{
    tlkmdi_bt_forceToIdle(app_bt_host_stop_finished_cb);
    tlkmdi_bt_tph_forceToIdle(app_tph_host_stop_finished_cb);
}

/**
 * @brief      Switch to LEA mode
 * @param[in]  none
 * @return     none
 */
static void app_lea_mode_switch_in(void)
{
    lea_unicast_server_start_advertising();
    app_mode_switch_ctrl.in_finish = 1;
    app_switch_mode_task_wake_up();
}

/**
 * @brief      Switch out of LEA mode
 * @param[in]  none
 * @return     none
 */
static void app_lea_mode_switch_out(void)
{
    lea_unicast_server_stop_task(app_lea_host_stop_finished_cb);
}

/**
 * @brief      Switch to UAC mode
 * @param[in]  none
 * @return     none
 */
static void app_uac_mode_switch_in(void)
{
    tlkapp_audioScheduler_init(TLKAPP_AUDIO_SCHEDULER_CFG_SCH_UAC);
    tlkapp_audioUac_init();
    app_mode_switch_ctrl.in_finish = 1;
    app_switch_mode_task_wake_up();
}

/**
 * @brief      Switch out of UAC mode
 * @param[in]  none
 * @return     none
 */
static void app_uac_mode_switch_out(void)
{
    tlkapp_audioUac_deinit();
    tlkapp_audioScheduler_init(TLKAPP_AUDIO_SCHEDULER_CFG_SCH_HEADSET_DEFAULT);
    app_mode_switch_ctrl.out_finish = 1;
    app_switch_mode_task_wake_up();
}

static const app_switch_func_t app_switch_in_funcs[APP_MODE_MAX] = {
    [APP_MODE_BT_TPH] = {.func = app_bt_tph_mode_switch_in, .taskID = TLKSYS_TASKID_HOST},
    [APP_MODE_LEA]    = {.func = app_lea_mode_switch_in, .taskID = TLKSYS_TASKID_HOST},
    [APP_MODE_UAC]    = {.func = app_uac_mode_switch_in, .taskID = TLKSYS_TASKID_AUDIO},
};
static const app_switch_func_t app_switch_out_funcs[APP_MODE_MAX] = {
    [APP_MODE_BT_TPH] = {.func = app_bt_tph_mode_switch_out, .taskID = TLKSYS_TASKID_HOST},
    [APP_MODE_LEA]    = {.func = app_lea_mode_switch_out, .taskID = TLKSYS_TASKID_HOST},
    [APP_MODE_UAC]    = {.func = app_uac_mode_switch_out, .taskID = TLKSYS_TASKID_AUDIO},
};

/**
 * @brief      Execute UI for mode switching
 * @param[in]  none
 * @return     none
 */
static void app_switch_mode_get_exe_ui(void)
{
    if (app_mode_switch_ctrl.busy) {
        tlkapi_printf(APP_SWITCH_LOG_EN, "***APP mode switch busy now");
        return;
    }
    tlkapi_printf(APP_SWITCH_LOG_EN, "***APP mode switch begin now");
    app_mode_switch_ctrl.busy      = 1;
    const app_switch_func_t *pFunc = &app_switch_out_funcs[app_mode_switch_ctrl.mode];
    tlksys_runFuncInTask(pFunc->taskID, pFunc->func);
}

/**
 * @brief      Task handler for mode switching
 * @param[in]  none
 * @return     none
 */
static void app_switch_mode_task_handler(void)
{
    if (app_mode_switch_ctrl.busy == 0) {
        return;
    }
    if (app_mode_switch_ctrl.need_timer == 1) {
        app_mode_switch_ctrl.need_timer = 0;
        tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &app_mode_switch_timer_handle);
    }
    //1. wait switch out ok
    if (app_mode_switch_ctrl.out_finish) {
        app_mode_switch_ctrl.out_finish = 0;
        tlkapi_printf(APP_SWITCH_LOG_EN, "***APP mode switch out ok");
        uint8_t                  next_mode = (app_mode_switch_ctrl.mode + 1) % APP_MODE_MAX;
        const app_switch_func_t *pFunc     = &app_switch_in_funcs[next_mode];
        tlksys_runFuncInTask(pFunc->taskID, pFunc->func);
        return;
    }
    //2. wait switch in ok
    if (app_mode_switch_ctrl.in_finish == 0) {
        return;
    }
    app_mode_switch_ctrl.in_finish = 0;
    app_mode_switch_ctrl.mode      = (app_mode_switch_ctrl.mode + 1) % APP_MODE_MAX;
    app_mode_switch_ctrl.busy      = 0;
    tlkapi_printf(APP_SWITCH_LOG_EN, "***APP mode now switch to %d", app_mode_switch_ctrl.mode);
}

/**
 * @brief      Hook function called when system task initialization is completed
 * @param[in]  none
 * @return     none
 */
void tlkapp_sys_taskInitCompletedHook(void)
{
    (void)app_switch_mode_get_exe_ui;
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_APP_0, app_switch_mode_task_handler);
#if TLK_DEV_KEY_ENABLE
    tlkdrv_key_registerVendorConfig1Callback(app_switch_mode_get_exe_ui);
#endif
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &app_mode_switch_timer_handle, 200 * 1000, false, app_mode_switch_timer, NULL);

    tlkmdi_tinySql_setSaveEnable(0);
}

#endif