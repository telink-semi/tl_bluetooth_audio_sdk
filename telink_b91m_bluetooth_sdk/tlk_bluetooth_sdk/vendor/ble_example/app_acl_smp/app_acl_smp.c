/********************************************************************************************************
 * @file    app_acl_smp.c
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
#include "stack/ble/ble.h"

#include "../app_example.h"
#include "../app_key.h"

#define APP_ACL_SMP_MODE_INPUT_CALLBACK    0x10
#define APP_ACL_SMP_MODE_OUTPUT_CALLBACK   0x20
#define APP_ACL_SMP_MODE_KEYBOARD_CALLBACK (APP_ACL_SMP_MODE_INPUT_CALLBACK | APP_ACL_SMP_MODE_OUTPUT_CALLBACK)

/** < BLE_HOST_SECURITY_MODE_1_LEVEL_2, legacy JustWorks. */
#define APP_ACL_SMP_MODE_LEGACY_JUST_WORKS 1
/** < BLE_HOST_SECURITY_MODE_1_LEVEL_3, legacy Out of Band. */
#define APP_ACL_SMP_MODE_LEGACY_OOB 2
/** < BLE_HOST_SECURITY_MODE_1_LEVEL_3 Legacy Passkey Entry. */
#define APP_ACL_SMP_MODE_LEGACY_INPUT_ONLY       (APP_ACL_SMP_MODE_INPUT_CALLBACK | 3)
#define APP_ACL_SMP_MODE_LEGACY_OUTPUT_ONLY      (APP_ACL_SMP_MODE_OUTPUT_CALLBACK | 4)
#define APP_ACL_SMP_MODE_LEGACY_PASSKEY          (APP_ACL_SMP_MODE_KEYBOARD_CALLBACK | 5)
#define APP_ACL_SMP_MODE_LEGACY_PASSKEY_KEYBOARD (APP_ACL_SMP_MODE_KEYBOARD_CALLBACK | 6)

/** < BLE_HOST_SECURITY_MODE_1_LEVEL_2, Secure Connections JustWorks. */
#define APP_ACL_SMP_MODE_SECURE_CONNECTION_JUST_WORKS 7
/** < BLE_HOST_SECURITY_MODE_1_LEVEL_4 Secure Connections Passkey Entry. */
#define APP_ACL_SMP_MODE_SECURE_CONNECTION_INPUT_ONLY  (APP_ACL_SMP_MODE_INPUT_CALLBACK | 8)
#define APP_ACL_SMP_MODE_SECURE_CONNECTION_OUTPUT_ONLY (APP_ACL_SMP_MODE_OUTPUT_CALLBACK | 9)
/** < BLE_HOST_SECURITY_MODE_1_LEVEL_4, Secure Connections Numeric Comparison. */
#define APP_ACL_SMP_MODE_SECURE_CONNECTION_PASSKEY          (APP_ACL_SMP_MODE_KEYBOARD_CALLBACK | 10)
#define APP_ACL_SMP_MODE_SECURE_CONNECTION_PASSKEY_KEYBOARD (APP_ACL_SMP_MODE_KEYBOARD_CALLBACK | 11)

#define APP_ACL_SMP_SELECT_MODE                             APP_ACL_SMP_MODE_LEGACY_JUST_WORKS

static uint16_t s_acl_smp_conn_handle = 0xFFFF;

static void app_acl_smp_pairing_started_callback(uint16_t conn_handle, const struct ble_host_smp_pairing_params *params)
{
    tlk_printf("[APP] smp start, conn_handle: 0x%03x, sc: %d, method: 0x%x, level: %d", conn_handle, params->secure_connection, params->method, params->level);
}

static void app_acl_smp_pairing_finish(uint16_t conn_handle, uint8_t reason)
{
    tlk_printf("[APP] smp finish, conn_handle: 0x%03x, reason: 0x%x", conn_handle, reason);
    s_acl_smp_conn_handle = 0xFFFF;
}

#if (APP_ACL_SMP_SELECT_MODE & APP_ACL_SMP_MODE_INPUT_CALLBACK)
static void app_acl_smp_pairing_input_callback(uint16_t conn_handle)
{
    s_acl_smp_conn_handle = conn_handle;
    tlk_printf("[APP] smp input, conn_handle: 0x%03x", conn_handle);
}
#endif

#if (APP_ACL_SMP_SELECT_MODE & APP_ACL_SMP_MODE_OUTPUT_CALLBACK)
static void app_acl_smp_pairing_output_callback(uint16_t conn_handle, uint32_t tk, bool numeric)
{
    tlk_printf("[APP] smp output, conn_handle: 0x%03x, tk: %d, numeric: %d", conn_handle, tk, numeric);
}
#endif

#if APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_OOB
static void app_acl_smp_pairing_oob_callback(uint16_t conn_handle, bool secure_connections)
{
    if (secure_connections) {
        // secure connection, use OOB
    } else {
#if APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_OOB
        // legacy pairing, use OOB
        uint8_t legacy_oob_value[16] = {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
        ble_host_smp_set_oob_value(conn_handle, legacy_oob_value);
#endif
    }
}
#endif


static void app_acl_smp_init_module(void)
{
#if APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_JUST_WORKS
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_JUST_WORKS(app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_INPUT_ONLY
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_PASSKEY_INIT_INPUT(true, app_acl_smp_pairing_input_callback, NULL, app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_OUTPUT_ONLY
    ble_host_smp_initial(
        BLE_HOST_SMP_LEGACY_PASSKEY_INIT_INPUT(false, NULL, app_acl_smp_pairing_output_callback, app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_PASSKEY
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_PASSKEY_INIT_INPUT(false, app_acl_smp_pairing_input_callback, app_acl_smp_pairing_output_callback,
                                                                app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_PASSKEY_KEYBOARD
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_PASSKEY_INIT_INPUT(true, app_acl_smp_pairing_input_callback, app_acl_smp_pairing_output_callback, app_acl_smp_pairing_started_callback,
                                                                app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_LEGACY_OOB
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_OOB_INIT(app_acl_smp_pairing_oob_callback, app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_SECURE_CONNECTION_JUST_WORKS
    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS(app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_SECURE_CONNECTION_INPUT_ONLY
    ble_host_smp_initial(BLE_HOST_SMP_SC_PASSKEY_INIT_INPUT(true, app_acl_smp_pairing_input_callback, NULL, app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_SECURE_CONNECTION_OUTPUT_ONLY
    ble_host_smp_initial(BLE_HOST_SMP_SC_PASSKEY_INIT_INPUT(false, NULL, app_acl_smp_pairing_output_callback, app_acl_smp_pairing_started_callback, app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_SECURE_CONNECTION_PASSKEY
    ble_host_smp_initial(BLE_HOST_SMP_SC_PASSKEY_INIT_INPUT(false, app_acl_smp_pairing_input_callback, app_acl_smp_pairing_output_callback, app_acl_smp_pairing_started_callback,
                                                            app_acl_smp_pairing_finish));
#elif APP_ACL_SMP_SELECT_MODE == APP_ACL_SMP_MODE_SECURE_CONNECTION_PASSKEY_KEYBOARD
    ble_host_smp_initial(BLE_HOST_SMP_SC_PASSKEY_INIT_INPUT(true, app_acl_smp_pairing_input_callback, app_acl_smp_pairing_output_callback, app_acl_smp_pairing_started_callback,
                                                            app_acl_smp_pairing_finish));
#endif
    // ble_host_smp_store_init(4, 4);
}

int  INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

int INIT(APP_BLE_ACL_SMP)(void)
{
    INIT(APP_BLE_ACL)();
    app_acl_smp_init_module();
    return 0;
}

void START(APP_BLE_ACL_SMP)(void)
{
    START(APP_BLE_ACL)();
}

#if IS_DEMO_SELECTED(APP_BLE_ACL_SMP)
static void app_acl_smp_input_value_callback(uint32_t value)
{
    tlk_printf("[APP] smp input value: %d", value);
    if (s_acl_smp_conn_handle != 0xFFFF) {
        int ret = ble_host_smp_set_passkey(s_acl_smp_conn_handle, value);
        tlk_printf("[APP] smp set passkey, ret: %d", ret);
    }
}

__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlk_printf("[SHELL] %s", hex_to_str(pData, dataLen));

    uint32_t value = 0;

    value += (pData[1] / 16) * 10 + (pData[1] % 16);
    value *= 100;
    value += (pData[2] / 16) * 10 + (pData[2] % 16);
    value *= 100;
    value += (pData[3] / 16) * 10 + (pData[3] % 16);

    app_acl_smp_input_value_callback(value);
}
#endif
