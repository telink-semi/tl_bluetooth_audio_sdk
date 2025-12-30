/********************************************************************************************************
 * @file    lea_broadcast_source.h
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
struct lea_broadcast_source_param
{
    const char *device_name; /** < Advertising/display name. */
};

typedef void (*lea_broadcast_source_operate_callback)(void);

/**
 * @brief       Start extended/periodic advertising and BIG creation for broadcast source.
 * @param[in]   param       - configuration structure with optional device name.
 * @param[in]   callback    - invoked once creation completes.
 * @return      true if request accepted, false otherwise.
 */
bool lea_broadcast_source_open(const struct lea_broadcast_source_param *param, lea_broadcast_source_operate_callback callback);

/**
 * @brief       Stop BIG, periodic advertising, and extended advertising.
 * @param[in]   callback    - invoked when closure is finished.
 * @return      true if request accepted, false otherwise.
 */
bool lea_broadcast_source_close(lea_broadcast_source_operate_callback callback);