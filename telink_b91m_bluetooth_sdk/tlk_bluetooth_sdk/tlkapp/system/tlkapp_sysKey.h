/********************************************************************************************************
 * @file    tlkapp_sysKey.h
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
 * @brief       Initializes the system key module.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sysKey_init(void);


/**
 * @brief       Handles key event actions.
 * @param[in]   evtID - The ID of the key event.
 * @return      none.
 */
void tlkapp_sysKey_evtAction(uint16_t evtID);

/**
 * @brief       Provides a hook function when the sys key init is completed.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sysKey_initCompletedHook(void);
