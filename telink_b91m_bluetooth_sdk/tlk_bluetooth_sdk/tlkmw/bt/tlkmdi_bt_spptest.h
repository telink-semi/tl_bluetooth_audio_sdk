/********************************************************************************************************
 * @file    tlkmdi_bt_spptest.h
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
#ifndef TLKMDI_BT_SPPTEST_H
#define TLKMDI_BT_SPPTEST_H

/**
 * @brief       This function initializes the SPP test module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_sppTestInit(void);

/**
 * @brief       This function starts the SPP sniff mode test.
 * @param[in]   none.
 * @return      0 on success, negative value on failure.
 */
int tlkmdi_bt_sppSniffTestStart(void);
#endif // TLKMDI_BT_SPPTEST_H
