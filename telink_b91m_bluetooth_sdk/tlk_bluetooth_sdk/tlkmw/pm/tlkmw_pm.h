/********************************************************************************************************
 * @file    tlkmw_pm.h
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
#ifndef TLKMW_PM_H
#define TLKMW_PM_H

/**
 * @brief      Initialize power management module
 * @param      none
 * @return     none
 */
void tlkmdi_pm_init(void);

/**
 * @brief      Process power management tasks
 * @param      none
 * @return     none
 */
void tlkmdi_pm_process(void);

/**
 * @brief      Enable or disable controller core sleep check
 * @param[in]  en - Enable flag. Non-zero to disable sleep check, zero to enable sleep check
 * @return     none
 */
void tlkmw_pm_enableControllerCoreSleepCheck(uint8_t en);

#endif // #ifndef TLKMW_USER_CTRL_H
