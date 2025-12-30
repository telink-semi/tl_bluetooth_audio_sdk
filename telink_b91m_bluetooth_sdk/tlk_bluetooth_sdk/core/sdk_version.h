/********************************************************************************************************
 * @file    sdk_version.h
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
#include "core/mcu_type.h"
/*
 * It is only used by SDK, and the customer is not allowed to modify it
 */
/*
 * Release Tool need to change this macro to match the release version,
 * the replace rules is: "$$$B92m_driver_sdk_"#sdk_version_num"$$$", The "#sdk_version_num"
 * will replace with this macro value.
 */
//#define B92_SDK_VERSION_NUM           V5.1.0.0
//#define SDK_VERSION_NUM               B92_SDK_VERSION_NUM

//#define SDK_VERSION1(sdk_version_num) "$$$b92_btll_cc_general_sdk_" #sdk_version_num "$$$"
//#define SDK_VERSION(sdk_version_num)  SDK_VERSION1(sdk_version_num)

//#define B92_SDK_VERSION_NUM           "V5.1.0.0"
//#define SDK_VERSION_NUM               B92_SDK_VERSION_NUM


//#define SDK_VERSION1(sdk_version_num) "$$$b92_btll_cc_general_sdk_" sdk_version_num "$$$"
//#define SDK_VERSION(sdk_version_num)  SDK_VERSION1(sdk_version_num)


/*
 * It is only used by SDK, and the customer is not allowed to modify it
 */

// Version : CERTIFICATION_MARK.SOFT_STRUCTURE.Major.Minor Patch
#define CERTIFICATION_MARK   6
#define SOFT_STRUCTURE       0
#define MAJOR_VERSION        0
#define MINOR_VERSION        0
#define PATCH_NUM            0

#define CUSTOM_MAJOR_VERSION 0
#define CUSTOM_MINOR_VERSION 0

/*
 * Release Tool need to change this macro to match the release version,
 * the replace rules is: "$$$B91m_driver_sdk_"#sdk_version_num"$$$", The "#sdk_version_num"
 * will replace with this macro value.
 */
#if (MCU_CORE_TYPE == MCU_CORE_B91)
    #define B91_SDK_VERSION_NUM           CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               B91_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$B91_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
    #define B92_SDK_VERSION_NUM           CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               B92_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$B92_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
    #define TL751X_SDK_VERSION_NUM        CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               TL751X_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$TL751X_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#elif (MCU_CORE_TYPE == MCU_CORE_TL753X)
    #define TL753X_SDK_VERSION_NUM        CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               TL753X_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$TL753X_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    #define TL721X_SDK_VERSION_NUM        CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               TL721X_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$TL721X_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"

#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
    #define TL322X_SDK_VERSION_NUM        CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               TL322X_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$TL322X_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#else
    #define CHIP_SDK_VERSION_NUM        CERTIFICATION_MARK.SOFT_STRUCTURE.MAJOR_VERSION.MINOR_VERSION
    #define SDK_VERSION_NUM               CHIP_SDK_VERSION_NUM

    #define SDK_VERSION1(sdk_version_num) "$$$CHIP_tlk_bluetooth_audio_sdk_V" #sdk_version_num "$$$"
#endif


#define SDK_VERSION(sdk_version_num)         SDK_VERSION1(sdk_version_num)

#define PATCH_VERSION1(patch_num)            "$$$patch_" #patch_num "$$$"
#define PATCH_VERSION(patch_num)             PATCH_VERSION1(patch_num)

#define CUSTOM_VERSION_NUM                   CUSTOM_MAJOR_VERSION.CUSTOM_MINOR_VERSION

#define CUSTOM_VERSION1(cunstom_version_num) "$$$custom_" #cunstom_version_num "$$$"
#define CUSTOM_VERSION(cunstom_version_num)  CUSTOM_VERSION1(cunstom_version_num)
