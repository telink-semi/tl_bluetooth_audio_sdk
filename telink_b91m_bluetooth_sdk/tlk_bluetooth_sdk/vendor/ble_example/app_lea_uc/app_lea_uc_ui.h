/********************************************************************************************************
 * @file    app_lea_uc_ui.h
 *
 * @brief   This is the header file for TLSR/TL
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
struct ble_app_sdp_flags
{
    uint16_t conn_handle;

    struct
    {
        // LE Audio Client flags check.
        uint16_t csis_client_flags : 1;
        uint16_t ascs_client_flags : 1;
        uint16_t pacs_client_flags : 1;

        uint16_t hid_client_flags : 1;
    };
};

typedef void (*tlk_app_sdp_finish_callback)(uint16_t conn_handle, struct ble_app_sdp_flags *flags);

/**
 * @brief  Initialize SDP flags management.
 *          Initialize all SDP flags entries and register event handler.
 *
 * @param[in] callback  Callback function to be called when SDP discovery is finished.
 */
void app_sdp_flags_initial(tlk_app_sdp_finish_callback callback);

/**
 * @brief  Initialize LE Audio UC UI module.
 *          Initialize device filter, extended scan control, discovery and ACL connection callbacks.
 */
void app_lea_uc_ui_init(void);

/**
 * @brief  Start LE Audio UC UI module.
 *          Configure timer, audio codec and start audio streams based on selected function mode.
 */
void app_lea_uc_ui_start(void);
