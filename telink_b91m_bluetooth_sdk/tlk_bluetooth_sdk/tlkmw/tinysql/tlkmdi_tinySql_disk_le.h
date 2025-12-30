/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_le.h
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
 * @brief       Get the volume level for LE disk.
 * @return      Current volume level.
 */
uint8_t tlkmdi_tinySql_leDiskGetVolume(void);

/**
 * @brief       Set the volume level for LE disk.
 * @param[in]   vol - Volume level to set.
 * @return      none.
 */
void tlkmdi_tinySql_leDiskSetVolume(uint8_t vol);