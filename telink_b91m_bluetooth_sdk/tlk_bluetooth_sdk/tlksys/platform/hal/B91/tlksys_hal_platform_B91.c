/********************************************************************************************************
 * @file    tlksys_hal_platform_B91.c
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
#include "tl_common.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_B91

#if TLK_LOW_POWER_ENABLE
typedef enum
{
    CORE_0V8  = 0x10,
    CORE_0V85 = 0x15,
    CORE_0V9  = 0x19,
    CORE_0V95 = 0x1C,
    CORE_1V   = BIT(5) | 0x00,
    CORE_1V05 = BIT(5) | 0x05,
    CORE_1V1  = BIT(5) | 0x09,
    CORE_1V15 = BIT(5) | 0x0D,
    CORE_1V2  = BIT(5) | 0x10,
} pm_digital_core_voltage_e;

static inline void pm_set_digital_core_voltage(pm_digital_core_voltage_e dcore_voltage)
{
    /*    The supported frequencies for cclk, hclk, pclk, mspi and clkzbmst under different digital core voltages are as follows:
 *
 *          digital_core      cclk      hclk      pclk      mspi      clkzbmst(BT)
 *              1.2V          96M       48M       48M       64M          32M
 *              1.0V          48M       24M       24M       24M          24M
 *
 *    1.The highest frequency for BT group to use clkzbmst is 8M,
 *    and currently they are not concerned about the impact of digital core voltage on clkzbmst.(confirmed by junwei, 20230516)
 *
 *    2.The built-in flash can support the maximum operating frequency of the mspi in the table above.
 *    (Because when selecting flash chips, the TCLQV indicators of Flash will be controlled
 *    according to the chip design requirements to ensure compliance with the requirements)(confirmed by junwen/shenyan, 20230516)
 *
 *    3.The maximum operating frequency of the mspi that can be supported by the external Flash is
 *    related to the actual development board and the TCLQV indicator of the Flash.(confirmed by junwen/shenyan, 20230516)
 *
 */
    analog_write_reg8(0x06, analog_read_reg8(0x06) & ~BIT(4)); //<4>:pd_ldo_dcore,  default:1,->0 Power up of digital core ldo.
    analog_write_reg8(0x09, analog_read_reg8(0x09) | BIT(6));  //<6>:pd_sw_dcore,  default:0,->1 power down the main dig ldo to dcore.
    //0x1d <5>:pd_sw_dcore:level shift of dcore, default:1->dcore supply trim: 0.999V~1.362V(HIGHER_RANGE),0->dcore supply trim: 0.658V~0.997V(LOWER_RANGE)
    //0x1d <4:0>:Correspond to dcore voltage value
    analog_write_reg8(0x1d, (analog_read_reg8(0x1d) & 0xc0) | dcore_voltage);
}
#endif
/**
 * @brief  Initialize HAL platform
 * @param  None.
 * @returns  None.
 */
void tlksys_hal_platform_init(void)
{
    sys_init(DCDC_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
    gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
    wd_stop();
    CCLK_96M_HCLK_48M_PCLK_24M;
    tlkhal_flash_init(0);
    core_interrupt_enable();
    gpio_init(0);

    blc_pm_select_internal_32k_crystal();
}


#endif
