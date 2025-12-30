/********************************************************************************************************
 * @file    tlkmw_host.h
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
 * @brief       This function initializes the host module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_init(void);

/**
 * @brief       This function resets the controller.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_resetController(void);

/**
 * @brief       This function closes the controller.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_closeController(void);

/**
 * @brief       This function checks if C2H is pending.
 * @param[in]   none.
 * @return      Returns true if C2H is pending, otherwise false.
 */
bool tlkmw_host_isC2hPending(void);