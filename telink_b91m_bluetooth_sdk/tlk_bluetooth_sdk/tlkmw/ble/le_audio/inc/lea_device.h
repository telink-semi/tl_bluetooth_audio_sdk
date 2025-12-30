/********************************************************************************************************
 * @file    lea_device.h
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

struct lea_device_param
{
    const char *device_name; /** < Advertising/display name. */
    uint16_t    interval;    /** < Extended advertising interval in milliseconds. */
    uint8_t     volume;      /** < Initial render volume (0~255). */
};

typedef void (*lea_device_operate_finish_callback)(void);

/**
 * @brief       Initialize generic LEA device stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_init(const struct lea_device_param *param);

/**
 * @brief       Start advertising if device is currently idle.
 * @return      none.
 */
void lea_device_start(void);

/**
 * @brief       Stop advertising or disconnect then invoke completion callback.
 * @param[in]   callback    - invoked when stop operation completes.
 * @return      none.
 */
void lea_device_stop(lea_device_operate_finish_callback callback);
