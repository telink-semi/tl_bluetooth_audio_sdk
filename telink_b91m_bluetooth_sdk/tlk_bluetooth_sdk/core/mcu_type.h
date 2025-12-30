/********************************************************************************************************
 * @file    mcu_type.h
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
#define CHIP_TYPE_B91    1
#define CHIP_TYPE_B92    2
#define CHIP_TYPE_TL7518 3
#define CHIP_TYPE_TL721X 4
#define CHIP_TYPE_TL751X 5
#define CHIP_TYPE_TL753X 6
#define CHIP_TYPE_TL322X 7
#define CHIP_TYPE_TL752X 8
#ifndef CHIP_TYPE
    #define CHIP_TYPE CHIP_TYPE_B92
#endif

#define MCU_CORE_B91    1
#define MCU_CORE_B92    2
#define MCU_CORE_TL7518 3
#define MCU_CORE_TL721X 4
#define MCU_CORE_TL751X 5
#define MCU_CORE_TL753X 6
#define MCU_CORE_TL322X 7
#define MCU_CORE_TL752X 8

#if (CHIP_TYPE == CHIP_TYPE_B91)
    #define MCU_CORE_TYPE MCU_CORE_B91
#elif (CHIP_TYPE == CHIP_TYPE_B92)
    #define MCU_CORE_TYPE MCU_CORE_B92
#elif (CHIP_TYPE == CHIP_TYPE_TL7518)
    #define MCU_CORE_TYPE MCU_CORE_TL7518
#elif (CHIP_TYPE == CHIP_TYPE_TL721X)
    #define MCU_CORE_TYPE MCU_CORE_TL721X
#elif (CHIP_TYPE == CHIP_TYPE_TL751X)
    #define MCU_CORE_TYPE MCU_CORE_TL751X
#elif (CHIP_TYPE == CHIP_TYPE_TL753X)
    #define MCU_CORE_TYPE MCU_CORE_TL753X
#elif (CHIP_TYPE == CHIP_TYPE_TL322X)
    #define MCU_CORE_TYPE MCU_CORE_TL322X
#elif (CHIP_TYPE == CHIP_TYPE_TL752X)
    #define MCU_CORE_TYPE MCU_CORE_TL752X

    #define MCU_CORE_TL752X_TEMP    1
#endif

#ifndef MCU_CORE_TL752X_TEMP
    #define MCU_CORE_TL752X_TEMP    0
#endif

#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X) || (MCU_CORE_TYPE == CHIP_TYPE_TL753X) ||(MCU_CORE_TYPE == CHIP_TYPE_TL752X)
    #define MCU_DUAL_CORE_ENABLE 1
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
    #if (PROJ_BLE_EXAMPLE)
        #define MCU_DUAL_CORE_ENABLE    0
    #else
        #define MCU_DUAL_CORE_ENABLE    1
    #endif
#else
    #define MCU_DUAL_CORE_ENABLE 0
#endif

#if (MCU_CORE_TL751X_N22 || MCU_CORE_TL752X_N22 || MCU_CORE_TL753X_N22|| MCU_CORE_TL322X_N22)
    #define MCU_CORE_N22 1
#endif
