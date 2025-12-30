/********************************************************************************************************
 * @file    app_lea_uc_prf.h
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

typedef void (*found_connectable_us_callback)(uint8_t addr_type, const uint8_t addr[6], const uint8_t *device_name, int8_t rssi);

struct discovery_us_init_param
{
    found_connectable_us_callback found_callback;
};

/**
 * @brief  Initialize LE Audio UC profile.
 *          Initialize CAP UC, register PACS/ASCS/CSIS control clients and initialize SMP.
 */
void app_uc_prf_initial(void);

/**
 * @brief  Start discovery of connectable UC devices.
 *          Register GAP event handler and set found callback.
 *
 * @param[in] param  Discovery initialization parameters.
 */
void app_uc_prf_discovery_us(const struct discovery_us_init_param *param);

/**
 * @brief  Set discovery filter by SIRK.
 *          Switch to filter mode and store SIRK for RSI validation.
 *
 * @param[in] srik  Set Identity Resolving Key (SIRK), 16 bytes.
 */
void app_uc_prf_set_discovery_filter(uint8_t srik[16]);

/**
 * @brief  Clear discovery filter.
 *          Switch back to connectable mode and clear SIRK.
 */
void app_uc_prf_clear_discovery_filter(void);
