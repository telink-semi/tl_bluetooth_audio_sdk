/********************************************************************************************************
 * @file    analog_afe1v_reg.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef ANALOG_AFE1V_REG_H
#define ANALOG_AFE1V_REG_H
#include "soc.h"
#define areg_0x104 0x104
#define areg_0x105 0x105

enum
{
    FLD_LOCK_DET_SIG_ENABLE = BIT(7), //lock detect enable signal
};

#define areg_0x106 0x106

enum
{
    FLD_LOCK_DET_SIG_RESET = BIT(7), //lock detect reset signal
};

#define areg_0x107 0x107

#define areg_0x108 0x108

enum
{
    FLD_BBPLL_LOCK_DETECTOR = BIT(5),
};

#define areg_0x10d 0x10d
#define areg_0x120 0x120
enum
{
    FLD_WT_PWR          = BIT_RNG(0, 2), /**< Power Level Sensitivity */
    FLD_WT_NTRACK       = BIT_RNG(3, 5), /**< Background Noise Tracking parameter */
    FLD_WT_VTRACK       = BIT_RNG(6, 7), /**< Voice Tracking parameter */
};

#define areg_0x121 0x121
enum
{
    FLD_WT_MINEVENT     = BIT_RNG(0, 2), /**< Minimum Event Duration */
    FLD_WT_MINDELAY     = BIT_RNG(3, 4), /**< Minimum Delay */
};

#define areg_0x122 0x122
enum
{
    FLD_WT_NFI_DET      = BIT_RNG(0, 5), /**< NFI Detection */
};

#define areg_0x123 0x123
enum
{
    FLD_WT_MCLK_DIV      = BIT_RNG(3, 5), /**< Clock selection */
    FLD_WT_SLEEP         = BIT(6),        /**< Sleep mode */
    FLD_WT_SB            = BIT(7),        /**< Stand-by mode */
};

#define areg_0x124 0x124
enum
{
    FLD_WT_IRQ_FLAG      = BIT(1), /**< Interrupt ReQuest flag */
    FLD_WT_IRQ_MODE      = BIT(2), /**< IRQ mode selection */
};

#define areg_0x125 0x125 /**< NFI value read back */
#define areg_0x126 0x126 /**< NFI_SYNC value read back */
#define areg_0x127 0x127 /**< read status: 0x55  successful, 0x00 failed */

#define areg_0x147 0x147

enum
{
    FLD_CAL_24M_RC_DISABLE = 0x0e,
    FLD_CAL_24M_RC_ENABLE  = 0x0f,
};

#define areg_0x146 0x146

enum
{
    FLD_CAL_32K_EN       = BIT(0),
    FLD_CAL_32K_LEN      = BIT(1),
    FLD_CAL_32K_NEW      = BIT(2),
    FLD_CAL_32K_TEST     = BIT(3),
    FLD_CAL_32K_WAIT_LEN = BIT_RNG(4, 6),
    FLD_CAL_32K_FINE     = BIT(7),
};

enum
{
    FLD_CAL_32K_RC_DISABLE = 0xf6,
    FLD_CAL_32K_RC_ENABLE  = 0xf7,
};

#define areg_0x148 0x148
#define areg_0x149 0x149
#define areg_0x14a 0x14a
#define areg_0x14b 0x14b
#define areg_0x14f 0x14f

enum
{
    FLD_CAL_32K_DONE = BIT(6),
    FLD_CAL_24M_DONE = BIT(7),
};

#define areg_0x151 0x151

enum
{
    FLD_BBPLL_LOCK_EN2X     = BIT(1),
    FLD_BBPLL_LOCK_RSTN     = BIT(2),
    FLD_AUDIO_BBPLL_LOCKDET = BIT(4),
};

#define areg_0x158 0x158
#define areg_0x15a 0x15a
#define areg_0x15b 0x15b
#define areg_0x15c 0x15c
#define areg_0x15d 0x15d

#define areg_0x162 0x162
#define areg_0x163 0x163
#endif
