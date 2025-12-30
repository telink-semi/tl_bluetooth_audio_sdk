/********************************************************************************************************
 * @file    tlkmdi_debug.h
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
#ifndef TLKMDI_DEBUG_H
#define TLKMDI_DEBUG_H

/**
 * @brief       This function initializes the debug module.
 * @param[in]   none.
 * @return      Returns TLK_ENONE on success, otherwise failed.
 */
int tlkmdi_debug_init(void);

/**
 * @brief       This function handles the debug module processing.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_debug_handler(void);

#endif // TLKMDI_DEBUG_H
