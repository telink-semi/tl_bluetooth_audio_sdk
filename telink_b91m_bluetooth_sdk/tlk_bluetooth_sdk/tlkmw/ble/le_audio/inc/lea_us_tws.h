/********************************************************************************************************
 * @file    lea_us_tws.h
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

typedef void (*lea_us_tws_operate_finish_callback)(void);

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_tws_initial(const struct lea_us_tws_param *param);

/**
 * @brief       Resume advertising when no ACL connection exists.
 * @return      none.
 */
void lea_unicast_server_tws_start(void);

/**
 * @brief       Stop advertising or disconnect and notify caller when finished.
 * @param[in]   callback    - invoked after resources are released.
 * @return      none.
 */
void lea_unicast_server_tws_stop(lea_us_tws_operate_finish_callback callback);
