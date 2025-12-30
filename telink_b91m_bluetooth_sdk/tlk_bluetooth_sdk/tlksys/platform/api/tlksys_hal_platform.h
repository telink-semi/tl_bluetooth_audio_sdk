/********************************************************************************************************
 * @file    tlksys_hal_platform.h
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

typedef enum
{
    TLKSYS_HAL_INIT_POWER_CFG_DEFAULT = 0,
    TLKSYS_HAL_INIT_POWER_CFG_DCDC,
    TLKSYS_HAL_INIT_POWER_CFG_LDO,
} TLKSYS_HAL_INIT_POWER_CFG_ENUM;

typedef enum
{
    TLKSYS_HAL_INIT_FLASH_LINE_CFG_DEFAULT = 0,
    TLKSYS_HAL_INIT_FLASH_LINE_CFG_4LINE_EN,
    TLKSYS_HAL_INIT_FLASH_LINE_CFG_4LINE_DIS,
} TLKSYS_HAL_INIT_FLASH_LINE_CFG_ENUM;

typedef enum
{
    TLKSYS_HAL_INIT_GPIO_CFG_DEFAULT = 0,
    TLKSYS_HAL_INIT_GPIO_CFG_NORMAL,
    TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN,
} TLKSYS_HAL_INIT_GPIO_CFG_ENUM;

typedef enum
{
    TLKSYS_HAL_INIT_LP_TMR_CFG_DEFAULT = 0,
    TLKSYS_HAL_INIT_LP_TMR_CFG_32kRC,
    TLKSYS_HAL_INIT_LP_TMR_CFG_32kXTAL,
} TLKSYS_HAL_INIT_LP_TMR_CFG_ENUM;

typedef enum
{
    TLKSYS_HAL_INIT_CALIBRATION_CFG_DEFAULT = 0,
    TLKSYS_HAL_INIT_CALIBRATION_CFG_EN,
    TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS,
} TLKSYS_HAL_INIT_CALIBRATION_CFG_ENUM;

typedef struct
{
    uint8_t flashPlicThreshold;
    uint8_t flashProtectEn; //0 or 1
    uint8_t flashLineCfg;   //refer to TLKSYS_HAL_INIT_FLASH_LINE_CFG_ENUM
    uint8_t powerCfg;       //refer to TLKSYS_HAL_INIT_POWER_CFG_ENUM
    uint8_t clockLevel;
    uint8_t gpioCfg;        //refer to TLKSYS_HAL_INIT_GPIO_CFG_ENUM
    uint8_t lpTmrCfg;       //refer to TLKSYS_HAL_INIT_LP_TMR_CFG_ENUM
    uint8_t calibrationCfg; //refer to TLKSYS_HAL_INIT_CALIBRATION_CFG_ENUM

} tlksys_hal_platform_init_cfg_t;

/**
 * @brief  Get platform initialization configuration
 * @param  None.
 * @returns  Pointer to platform initialization configuration structure.
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void);

/**
 * @brief  Initialize HAL platform
 * @param  None.
 * @returns  None.
 */
void tlksys_hal_platform_init(void);