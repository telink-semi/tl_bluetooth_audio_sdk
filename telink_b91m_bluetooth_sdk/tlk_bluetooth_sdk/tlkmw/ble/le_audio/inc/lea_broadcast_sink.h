/********************************************************************************************************
 * @file    lea_broadcast_sink.h
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
struct lea_broadcast_sink_param
{
    const char *device_name; /** < Advertising/display name. */
    uint8_t     volume;      /** < Initial render volume (0~255). */
};

typedef void (*lea_broadcast_sink_operate_callback)(void);

/**
 * @brief       Initialize broadcast sink stack, services, and event callbacks.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_broadcast_sink_init(const struct lea_broadcast_sink_param *param);

/**
 * @brief       Enable advertising so broadcasters can initiate connections.
 * @return      none.
 */
void lea_broadcast_sink_start(void);

/**
 * @brief       Stop BIS streaming or disconnect, then invoke completion callback.
 * @param[in]   callback    - called when resources are released.
 * @return      none.
 */
void lea_broadcast_sink_stop(lea_broadcast_sink_operate_callback callback);

/**
 * @brief       Stop current BIS stream and immediately reopen advertising when idle.
 * @return      none.
 */
void lea_broadcast_sink_stop_stream_open_adv(void);
