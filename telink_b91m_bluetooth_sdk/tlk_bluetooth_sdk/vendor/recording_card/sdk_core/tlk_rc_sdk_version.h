/********************************************************************************************************
 * @file    tlk_rc_sdk_version.h
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
 * @brief       Get the base SDK version string (Bluetooth audio sdk).
 * @param       none.
 * @return      Pointer to the base SDK version string.
 */
const char *tlk_rc_get_bluetooth_audio_sdk_version(void);

/**
 * @brief       Get the recording card SDK version string.
 * @param       none.
 * @return      Pointer to the recording card SDK version string.
 */
const char *tlk_rc_get_sdk_version(void);