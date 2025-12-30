/********************************************************************************************************
 * @file    charger.c
 *
 * @brief   This is the source file for B92
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "charger.h"

/**
 * @brief   This function serves the initialization of charger module charging function.
 * @return  none
 */
void charger_init(void)
{
    analog_write_reg8(0x22, analog_read_reg8(0x22) & 0xf8); //<2>BLAS //<1>TX   //<0>RX
}
