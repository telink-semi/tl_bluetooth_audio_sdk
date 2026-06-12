/********************************************************************************************************
 * @file    tlkhal_clock_TL322X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL322X
/**
 * @brief  Set the system clock level
 * @param[in] level : Clock level to set
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_clock_setLevel(uint8_t level, void *futureUse)
{
    (void)level;
    (void)futureUse;
}

/**
 * @brief  Get the current system clock level
 * @param[in] futureUse : Reserved for future use
 * @returns  Current clock level.
 */
uint8_t tlkhal_clock_getCurrentLevel(void *futureUse)
{
    (void)futureUse;
    return 0;
}
#endif
