/********************************************************************************************************
 * @file    watchdog.c
 *
 * @brief   This is the source file for tl751x
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
#include "watchdog.h"

/**
 * @brief     start 32k watchdog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void)
{
    analog_write_reg8(0x79, analog_read_reg8(0x79) | 0x01);
}

/**
 * @brief     stop 32k watchdog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void)
{
    analog_write_reg8(0x79, analog_read_reg8(0x79) & 0xfe);
}

/**
 * @brief     get 32k watchdog overflow status.
 * @return    watchdog overflow status.
 */
_attribute_ram_code_sec_noinline_ unsigned char wd_32k_get_status(void)
{
    return (analog_read_reg8(areg_aon_0x69) & FLD_32K_WD_OVERFLOW_STATUS);
}

/**
 * @brief     Clear the reset state caused by the 32k watchdog overflow.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_clear_status(void)
{
    analog_write_reg8(areg_aon_0x69, FLD_32K_WD_OVERFLOW_STATUS);
}

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is millisecond, the lower 8bit of the wd timer register is invalid, so ranges from 8~134,217,720ms.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{
    unsigned int tmp_period_ms = 0;

    tmp_period_ms = clock_get_32k_tick() + 32 * period_ms;

    analog_write_reg8(0x7c, tmp_period_ms >> 24);
    analog_write_reg8(0x7b, tmp_period_ms >> 16);
    analog_write_reg8(0x7a, tmp_period_ms >> 8);
}
