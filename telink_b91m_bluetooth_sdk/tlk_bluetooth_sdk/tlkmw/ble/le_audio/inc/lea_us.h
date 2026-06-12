/********************************************************************************************************
 * @file    lea_us.h
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
struct tlk_mw_lea_cap_headset_param
{
    const char *device_name; /** < Advertising/display name. */
    uint16_t    interval;    /** < Extended advertising interval in milliseconds. */
    uint8_t     volume;      /** < Initial render volume (0~255). */
};

struct lea_us_tws_param
{
    const char *device_name; /** < Display name in advertising packet. */
    uint16_t    interval;    /** < Advertising interval in milliseconds. */
    uint8_t     volume;      /** < Initial render volume (0~255). */
    uint8_t     ear_type;    /** < TWS_LEFT_EAR_BUD or TWS_RIGHT_EAR_BUD. */
    uint8_t     sirk[16];    /** < CSIP Set Identity Resolving Key. */
};

enum
{
    TWS_LEFT_EAR_BUD = 0x20,
    TWS_RIGHT_EAR_BUD,
};

typedef void (*lea_unicast_server_operate_finish_callback)(void);

/**
 * @brief       Initialize headset LEA stack, services, and advertising.
 * @param[in]   param   - configuration for services and advertising.
 * @return      none.
 */
void lea_unicast_server_headset_initial(const struct tlk_mw_lea_cap_headset_param *param);

/**
 * @brief       Initialize speaker LE Audio stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_speaker_initial(const struct tlk_mw_lea_cap_headset_param *param);

/**
 * @brief       Initialize generic LEA device stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_init(const struct tlk_mw_lea_cap_headset_param *param);

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_tws_initial(const struct lea_us_tws_param *param);

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_tws_initial(const struct lea_us_tws_param *param);

/**
 * @brief       Start advertising if headset is idle.
 * @return      none.
 */
void lea_unicast_server_start_advertising(void);

/**
 * @brief       Stop advertising or disconnect existing ACL, then call callback.
 * @param[in]   callback    - called when stop operation is finished.
 * @return      none.
 */
void lea_unicast_server_stop_task(lea_unicast_server_operate_finish_callback callback);

/**
 * @brief       Start reconnect all paired devices.
 * @param[in]   timeout     - timeout in seconds.
 * @param[in]   callback    - called when stop operation is finished.
 * @return      true if operation is started, false if no paired devices.
 */
bool lea_unicast_server_reconnect_all_paired_devices(uint32_t timeout, lea_unicast_server_operate_finish_callback callback);
