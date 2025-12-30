/********************************************************************************************************
 * @file    wt.c
 *
 * @brief   This is the source file for tl751x
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
#include "wt.h"

/**********************************************************************************************************************
 *                                                 WT interface                                                       *
 *********************************************************************************************************************/
/*!
 * @name WT interface
 * @{
 */

/**
 * @brief      This function configures wt analog pin detecting signal.
 * @param[in]  ana_p - analog positive pin
 * @param[in]  ana_n - analog negative pin
 * @return     none
 */
void wt_set_ana_pin(wt_pin_e ana_p, wt_pin_e ana_n)
{
    gpio_input_en((gpio_pin_e)ana_p);
    gpio_function_dis((gpio_pin_e)ana_p);

    gpio_input_en((gpio_pin_e)ana_n);
    gpio_function_dis((gpio_pin_e)ana_n);
}

/**
 * @brief      This function serves to read wt analog register.
 * @param[in]  addr - wt analog addr
 * @return     none
  */
_attribute_ram_code_sec_noinline_ unsigned char wt_read_reg(unsigned int addr)
{
    analog_read_reg8(addr);
    while (analog_read_reg8(areg_0x127) != 0x55);
    return analog_read_reg8(addr);
}

/**
 * @brief      This function serves to write wt analog register.
 * @return     none
  */
void wt_init(void)
{
    pm_set_dig_module_power_switch(FLD_PD_WT_EN, PM_POWER_UP);
    BM_SET(reg_clk_en3, FLD_CLK3_WT);
    analog_write_reg8(areg_aon_0x54, (analog_read_reg8(areg_aon_0x54) | BIT(7)));
    delay_us(200);
    analog_write_reg8(areg_0x120, MASK_VAL(FLD_WT_PWR, WT_PWR_16dB)
                      | MASK_VAL(FLD_WT_NTRACK, WT_NTRACK_4) | MASK_VAL(FLD_WT_VTRACK, WT_VTRACK_1));
    analog_write_reg8(areg_0x124, MASK_VAL(FLD_WT_IRQ_MODE, 1));
    analog_write_reg8(areg_0x123,  MASK_VAL(FLD_WT_MCLK_DIV, 0) | MASK_VAL(FLD_WT_SLEEP, 0) | MASK_VAL(FLD_WT_SB, 0));

    wt_set_ana_pin(WT_P_IN, WT_N_IN);
}

/**
 * @}
 */
