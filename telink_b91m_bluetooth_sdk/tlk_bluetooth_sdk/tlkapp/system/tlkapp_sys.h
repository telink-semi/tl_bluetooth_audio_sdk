/********************************************************************************************************
 * @file    tlkapp_sys.h
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
 * @brief       Gets the configuration for the system task.
 * @param[in]   none.
 * @return      Returns a pointer to the system task configuration.
 */
const tlksys_task_cfg_t * tlkapp_system_getTaskCfg(void);

/**
 * @brief       Provides a hook function when the system task init is completed.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_taskInitCompletedHook(void);

