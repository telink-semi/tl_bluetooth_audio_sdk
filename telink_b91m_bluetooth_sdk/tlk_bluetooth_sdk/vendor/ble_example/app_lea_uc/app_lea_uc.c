/********************************************************************************************************
 * @file    app_lea_uc.c
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
#include <stdio.h>
#include "stack/ble/ble.h"
#include "driver.h"
#include "../app_example.h"

#include "app_lea_uc_prf.h"
#include "app_lea_uc_ui.h"


#ifndef APP_SDP_FLAGS_MAX_NUM
#define APP_SDP_FLAGS_MAX_NUM 4
#endif

#define APP_SDP_INVALID_CONN_HANDLE 0xFFFF


static struct ble_app_sdp_flags    s_app_sdp_flags[APP_SDP_FLAGS_MAX_NUM];
static tlk_app_sdp_finish_callback s_app_sdp_finish_callback = NULL;

static void app_sdp_flags_event_handler(uint16_t conn_handle, enum prf_common_event_id event_id, const void *event_msg);

/**
 * @brief  Find SDP flags structure by connection handle.
 *
 * @param[in] conn_handle  Connection handle.
 *
 * @return  Pointer to SDP flags structure if found, NULL otherwise.
 */
struct ble_app_sdp_flags *app_sdp_flags_find_conn_handle(uint16_t conn_handle);

/**
 * @brief  Initialize SDP flags management.
 *          Initialize all SDP flags entries and register event handler.
 *
 * @param[in] callback  Callback function to be called when SDP discovery is finished.
 */
void app_sdp_flags_initial(tlk_app_sdp_finish_callback callback)
{
    for (size_t i = 0; i < APP_SDP_FLAGS_MAX_NUM; i++) {
        s_app_sdp_flags[i].conn_handle = APP_SDP_INVALID_CONN_HANDLE;
    }

    s_app_sdp_finish_callback = callback;
    ble_prf_register_event_handler(app_sdp_flags_event_handler);
}

/**
 * @brief  Allocate a new SDP flags entry for the given connection handle.
 *
 * @param[in] conn_handle  Connection handle.
 */
static void app_sdp_flags_malloc_new_conn_handle(uint16_t conn_handle)
{
    for (size_t i = 0; i < APP_SDP_FLAGS_MAX_NUM; i++) {
        if (s_app_sdp_flags[i].conn_handle == APP_SDP_INVALID_CONN_HANDLE) {
            memset(&s_app_sdp_flags[i], 0, sizeof(s_app_sdp_flags[i]));
            s_app_sdp_flags[i].conn_handle = conn_handle;
            break;
        }
    }
}

/**
 * @brief  Free SDP flags entry for the given connection handle.
 *
 * @param[in] conn_handle  Connection handle.
 */
static void app_sdp_flags_free_conn_handle(uint16_t conn_handle)
{
    for (size_t i = 0; i < APP_SDP_FLAGS_MAX_NUM; i++) {
        if (s_app_sdp_flags[i].conn_handle == conn_handle) {
            memset(&s_app_sdp_flags[i], 0, sizeof(s_app_sdp_flags[i]));
            s_app_sdp_flags[i].conn_handle = APP_SDP_INVALID_CONN_HANDLE;
            break;
        }
    }
}

/**
 * @brief  Find SDP flags structure by connection handle.
 *
 * @param[in] conn_handle  Connection handle.
 *
 * @return  Pointer to SDP flags structure if found, NULL otherwise.
 */
struct ble_app_sdp_flags *app_sdp_flags_find_conn_handle(uint16_t conn_handle)
{
    for (size_t i = 0; i < APP_SDP_FLAGS_MAX_NUM; i++) {
        if (s_app_sdp_flags[i].conn_handle == conn_handle) {
            return &s_app_sdp_flags[i];
        }
    }
    return NULL;
}

static void app_sdp_flags_event_handler(uint16_t conn_handle, enum prf_common_event_id event_id, const void *event_msg)
{
    switch (event_id) {
    case PRF_EVT_ID_ACL_CONNECT:
    {
        app_sdp_flags_malloc_new_conn_handle(conn_handle);
    } break;
    case PRF_EVT_ID_ACL_DISCONNECT:
    {
        app_sdp_flags_free_conn_handle(conn_handle);
    } break;
    case PRF_EVT_ID_SDP_FOUND:
    {
        struct ble_prf_sdp_info  *p_sdp_info  = (struct ble_prf_sdp_info *)event_msg;
        struct ble_app_sdp_flags *p_sdp_flags = app_sdp_flags_find_conn_handle(conn_handle);
        if (p_sdp_flags != NULL) {
            if (p_sdp_info->service_id == SERVICE_ID_CSIS) {
                p_sdp_flags->csis_client_flags = 1;
            } else if (p_sdp_info->service_id == SERVICE_ID_ASCS) {
                p_sdp_flags->ascs_client_flags = 1;
            } else if (p_sdp_info->service_id == SERVICE_ID_PACS) {
                p_sdp_flags->pacs_client_flags = 1;
            } else if (p_sdp_info->service_id == SERVICE_ID_HID) {
                p_sdp_flags->hid_client_flags = 1;
            }
        }
    } break;
    case PRF_EVT_ID_SDP_FINISH:
    {
        struct ble_app_sdp_flags *p_sdp_flags = app_sdp_flags_find_conn_handle(conn_handle);
        if (p_sdp_flags != NULL && s_app_sdp_finish_callback != NULL) {
            s_app_sdp_finish_callback(conn_handle, p_sdp_flags);
        }
    } break;
    default:
    {
    } break;
    }
}

/**
 * @brief  Initialize LE Audio UC application module.
 *          Initialize CIS central, profile and UI components.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_LEA_UC)(void)
{
    ble_host_gap_cis_central_init();
    app_uc_prf_initial();
    app_lea_uc_ui_init();
    return 0;
}

/**
 * @brief  Start LE Audio UC application module.
 *          Start UI components.
 */
void START(APP_BLE_LEA_UC)(void)
{
    app_lea_uc_ui_start();
}
