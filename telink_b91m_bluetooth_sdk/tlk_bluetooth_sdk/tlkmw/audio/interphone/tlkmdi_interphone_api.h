/********************************************************************************************************
 * @file    tlkmdi_interphone_api.h
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
#pragma once

/**
 * @brief       Enable interphone mesh
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_api_mesh_enable(void);

/**
 * @brief       Disable interphone mesh
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_api_mesh_disable(void);

/**
 * @brief       Check if interphone mesh is enabled
 * @param       None
 * @return      1 if mesh is enabled, 0 otherwise
 */
uint8_t tlkmdi_interphone_api_is_mesh_enable(void);