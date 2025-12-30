/********************************************************************************************************
 * @file    tlkhal_clock_TL752X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_TL752X

static uint8_t sTlksysHalClockLevel = 0XFF;

/**
 * @brief  Set the system clock level
 * @param[in] level : Clock level to set
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_clock_setLevel(uint8_t level, void *futureUse)
{
    (void)futureUse;
    if (sTlksysHalClockLevel == level) {
        return;
    }
    sTlksysHalClockLevel = level;
    switch (level) {
    case 0:
    {
        // PLL_192M_D25F_DSP_48M_HCLK_48M_PCLK_48M_MSPI_48M_WT_12M
    } break;
    case 1:
    {
        // clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV3, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, PLL_DIV2_TO_MSPI_CLK, CLK_DIV2);
    } break;
    case 2:
    {
        // PLL_192M_D25F_DSP_96M_HCLK_48M_PCLK_48M_MSPI_48M_WT_12M
    } break;
    case 3:
    {
        // PLL_264M_D25F_DSP_132M_HCLK_66M_PCLK_66M_MSPI_44M_WT_11M
    } break;
    }
}

/**
 * @brief  Get the current system clock level
 * @param[in] futureUse : Reserved for future use
 * @returns  Current clock level.
 */
uint8_t tlkhal_clock_getCurrentLevel(void *futureUse)
{
    (void)futureUse;
    return sTlksysHalClockLevel;
}

#endif