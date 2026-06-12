/********************************************************************************************************
 * @file    tlk_rc_sdk_version.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "core/sdk_version.h"

const char *sc_rc_based_sdk_version = SDK_VERSION(SDK_VERSION_NUM);

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
const char *sc_rc_sdk_version = "v1.1.0";
#elif (MCU_CORE_TYPE == CHIP_TYPE_TL751X)
const char *sc_rc_sdk_version = "v1.1.1";
#else
const char *sc_rc_sdk_version = "Not_support";
#endif

/**
 * @brief       Get the base SDK version string (Bluetooth audio sdk).
 * @param       none.
 * @return      Pointer to the base SDK version string.
 */
const char *tlk_rc_get_bluetooth_audio_sdk_version(void)
{
    return sc_rc_based_sdk_version;
}

/**
 * @brief       Get the recording card SDK version string.
 * @param       none.
 * @return      Pointer to the recording card SDK version string.
 */
const char *tlk_rc_get_sdk_version(void)
{
    return sc_rc_sdk_version;
}
