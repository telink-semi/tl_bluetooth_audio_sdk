/********************************************************************************************************
 * @file    clock_internal.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page CLOCK_INTERNAL
 *
 *  Introduction
 *  ===============
 *  TL322x clock setting.
 *
 *  API Reference
 *  ===============
 *  Header File: clock_internal.h
 */

#ifndef _CLOCK_INTERNAL_H_
#define _CLOCK_INTERNAL_H_

#include "compiler.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/*
 * ALG_MODULE_MAX_CLK is configured to 24:
 * - The alg clock source is pclk, with a default division of 1/2.
 *   - At 1.0v, the maximum pclk is 39m, that is, the maximum alg is 19.5m. Just follow the default frequency division.
 *   - At 1.1v, the maximum pclk is 96m, and the maximum alg is 24m. It is necessary to modify the crossover according to the current size of the pclk.
 */
/**
 * The following chart lists the maximum operating frequency that each clock source can operate at different voltage levels.
 * (But if it is related to peripherals, this frequency will be related to the whole system, such as MSPI-->
 *  If it is built-in flash, the maximum speed of mspi is 64M,If it is an external flash, the maximum speed
 *  of mspi needs to be based on the board test.)
 * :-------------------------------- Voltage versus frequency(default is 0.8v)  table clock_h_1 --------------------------------------------------------------------------
 * |voltage|  pll  | cclk  | hclk | pclk | mspi | lspi(m/s) | gspi(m/s) | i2s   | alg   |   pm_top  |  rram  |  itag  |  adc  |  i3c | i3c_bus_pp | i3c_bus_od |
 * |  1.1V |  192  |  192  |  96  |  96  |  64  |  48/24    |  48/24    |  13   |  24   |     24    |   48   |   20   |  48   |  48  |     12     |     4      |
 * |  1.0V |  192  |  78   |  39  |  39  |  64  |  48/24    |  48/24    |  13   |  19.5 |     19.5  |   19.5 |   20   |  48   |  24  |     6      |     2      |
 */
//core 1.1V
#define PLL_192M_D25F_192M_HCLK_N22_96M_PCLK_96M_MSPI_48M clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV4)
#define PLL_180M_D25F_180M_HCLK_N22_90M_PCLK_90M_MSPI_45M clock_init(CLK_BASEBAND_PLL_180M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV4)
#define PLL_120M_D25F_120M_HCLK_N22_60M_PCLK_60M_MSPI_30M clock_init(CLK_BASEBAND_PLL_120M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV4)
/**
 * If it is built-in flash, the maximum speed of mspi is 64M.
 * If it is an external flash, the maximum speed of mspi needs to be based on the board test.
 * Because the maximum speed is related to the wiring of the board, and is also affected by temperature and GPIO voltage,
 * the maximum speed needs to be tested at the highest and lowest voltage of the board,
 *and the high and low temperature long-term stability test speed is no problem.
 */
//core 1.1V
#define PLL_192M_D25F_192M_HCLK_N22_96M_PCLK_96M_MSPI_64M clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV3)
#define PLL_120M_D25F_120M_HCLK_N22_60M_PCLK_60M_MSPI_40M clock_init(CLK_BASEBAND_PLL_120M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV3)
#define PLL_180M_D25F_180M_HCLK_N22_90M_PCLK_90M_MSPI_60M clock_init(CLK_BASEBAND_PLL_180M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV3)

#endif
