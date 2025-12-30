/********************************************************************************************************
 * @file    app_ui.c
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
#include "drivers.h"
#include "stack/btble.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"

#include "app_ble_bis_sync.h"
#include "app_ble_bis_sink.h"

#include "app_ui.h"
#include "app_bt.h"
#include "app_ble_bis.h"


#if (APP_UI_ENABLE)

enum app_ui_mode
{
    APP_UI_MODE_A2DP_TO_BIS,
    APP_UI_MODE_BIS_SINK_WITH_ASSISTANT,
    APP_UI_MODE_BIS_SINK,
};

struct app_ui_info
{
    bool    op_busy;
    uint8_t mode;
};

static struct app_ui_info s_ui_info = {
    .op_busy = false,
    .mode    = APP_UI_MODE_A2DP_TO_BIS,
};

/**
 * @brief       Callback function for BIS source open operation
 * @param       none
 * @return      none
 */
static void app_ui_bis_source_open_callback(void)
{
    tlk_printf("%s", __func__);
    s_ui_info.op_busy = false;
    s_ui_info.mode    = APP_UI_MODE_A2DP_TO_BIS;
}

/**
 * @brief       Change UI mode to A2DP to BIS
 * @param       none
 * @return      none
 */
static void app_ui_mode_change_to_a2dp_to_bis(void)
{
    s_ui_info.op_busy = true;
    app_btmgr_init();
    app_ble_bis_source_init();
    app_ble_bis_source_open(app_ui_bis_source_open_callback);
}

/**
 * @brief       Change UI mode to BIS sink with assistant
 * @param       none
 * @return      none
 */
static void app_ui_mode_change_to_bis_sink_with_assistant(void)
{
    s_ui_info.op_busy = true;
    app_ble_bis_sync_start();
    s_ui_info.op_busy = false;
    s_ui_info.mode    = APP_UI_MODE_BIS_SINK_WITH_ASSISTANT;
}

/**
 * @brief       Callback function for BIS source close operation
 * @param       none
 * @return      none
 */
static void app_ui_bis_source_close_callback(void)
{
    s_ui_info.op_busy = false;
    app_ui_mode_change_to_bis_sink_with_assistant();
}

/**
 * @brief       Close BIS source mode
 * @param       none
 * @return      none
 */
static void app_ui_mode_close_bis_source(void)
{
    app_ble_bis_source_deinit();
    app_ble_bis_source_close(app_ui_bis_source_close_callback);
}

/**
 * @brief       Callback function for BT deinitialization
 * @param[in]   nowState - Current deinitialization state
 * @return      none
 */
static void app_bt_deinit_callback(APP_BTMGR_DEINIT_STATE_ENUM nowState)
{
    tlk_printf("%s, state:%d", __func__, nowState);
    if (nowState == APP_BTMGR_DEINIT_STATE_FINISHED) {
        app_ui_mode_close_bis_source();
    }
}

/**
 * @brief       Close A2DP to BIS mode
 * @param       none
 * @return      none
 */
static void app_ui_mode_close_a2dp_to_bis(void)
{
    app_btmgr_deinit();
}

/**
 * @brief       Change UI mode to BIS sink
 * @param       none
 * @return      none
 */
static void app_ui_mode_change_to_bis_sink(void)
{
    s_ui_info.op_busy = true;
    app_ble_bis_sink_start();
    s_ui_info.op_busy = false;
    s_ui_info.mode    = APP_UI_MODE_BIS_SINK;
}

/**
 * @brief       Close BIS sink with assistant mode
 * @param       none
 * @return      none
 */
static void app_ui_mode_close_bis_sink_with_assistant(void)
{
    app_ble_bis_sync_stop();
    s_ui_info.op_busy = false;
    app_ui_mode_change_to_bis_sink();
}

/**
 * @brief       Finish closing BIS sink mode
 * @param       none
 * @return      none
 */
static void app_ui_mode_close_bis_sink_finish(void)
{
    app_ui_mode_change_to_a2dp_to_bis();
}

/**
 * @brief       Close BIS sink mode
 * @param       none
 * @return      none
 */
static void app_ui_mode_close_bis_sink(void)
{
    app_ble_bis_sink_stop(app_ui_mode_close_bis_sink_finish);
}

/**
 * @brief       Handle mode change button press
 * @param       none
 * @return      none
 */
static void app_ui_mode_change_press(void)
{
    if (s_ui_info.op_busy == true) {
        tlkapi_printf(APP_UI_LOG_ENABLE, "mode change is busy, please wait");
        return;
    }

    tlkapi_printf(APP_UI_LOG_ENABLE, "mode change to %d", s_ui_info.mode);
    s_ui_info.op_busy = true;
    if (s_ui_info.mode == APP_UI_MODE_A2DP_TO_BIS) {
        app_ui_mode_close_a2dp_to_bis();
    } else if (s_ui_info.mode == APP_UI_MODE_BIS_SINK_WITH_ASSISTANT) {
        app_ui_mode_close_bis_sink_with_assistant();
    } else if (s_ui_info.mode == APP_UI_MODE_BIS_SINK) {
        app_ui_mode_close_bis_sink();
    }
}

/**
 * @brief       Handle state change button press
 * @param       none
 * @return      none
 */
static void app_ui_change_state_press(void)
{
    tlk_printf("%s", __func__);
    if (s_ui_info.op_busy == true) {
        tlkapi_printf(APP_UI_LOG_ENABLE, "mode change is busy, please wait");
        return;
    }
    if (s_ui_info.mode == APP_UI_MODE_BIS_SINK) {
        app_ble_bis_sink_stop_stream_open_adv();
    }
}

/**
 * @brief       Initialize the application UI
 * @param       none
 * @return      none
 */
void app_ui_init(void)
{
    app_btmgr_regDeinitCB(app_bt_deinit_callback);
    app_ui_mode_change_to_a2dp_to_bis();
    tlkdrv_key_registerVendorConfig1Callback(app_ui_mode_change_press);
    tlkdrv_key_registerVendorConfig2Callback(app_ui_change_state_press);
}

#endif //APP_UI_ENABLE