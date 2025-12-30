/********************************************************************************************************
 * @file    tlksys_hal_platform_B92.c
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
#if MCU_CORE_TYPE == MCU_CORE_B92

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
    const tlksys_hal_platform_init_cfg_t * pCfg = tlksys_hal_port_getPlatformInitCfg();
    if(pCfg == NULL){
        return;
    }
    extern void tlkapp_flash_prot_init(unsigned char flash_protect_en);
    tlkapp_flash_prot_init(pCfg->flashProtectEn);

    extern void tlkapp_flash_enable_4line(unsigned char en);
    switch (pCfg->flashLineCfg){
        case TLKSYS_HAL_INIT_FLASH_LINE_CFG_4LINE_DIS:
            tlkapp_flash_enable_4line(0);
            break;  
        default:
            tlkapp_flash_enable_4line(1);
            break;
    }

    /* For B92, must before sys_init, sys_init use ext_driver(from BLE SDK legacy design).
     * DRV pm_sleep_apis, RTOS 32k source, sys_pm module need this */
    switch (pCfg->lpTmrCfg) {
        case TLKSYS_HAL_INIT_LP_TMR_CFG_32kXTAL:
            blc_pm_select_external_32k_crystal(); //NOT supported, error_code
            break;
        default:
            blc_pm_select_internal_32k_crystal();
            break;
    }

    switch (pCfg->powerCfg){
        case TLKSYS_HAL_INIT_POWER_CFG_LDO:
            sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3, INTERNAL_CAP_XTAL24M);
            break;  
        default:
            sys_init(DCDC_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3, INTERNAL_CAP_XTAL24M);
            break;
    }

    switch (pCfg->gpioCfg){
        case TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN:
            gpio_shutdown(GPIO_ALL);
            break;  
        default:
            gpio_init(0);
            break;
    }

    wd_32k_stop();
    switch (pCfg->calibrationCfg){
        case TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS:
            break;  
        default:
            extern void calibration_func(void);
            calibration_func();
            break;
    }
#if TLK_LOW_POWER_ENABLE
    clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV3_TO_CCLK, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);//CCLK_64M_HCLK_32M_PCLK_16M
    pm_set_digital_core_voltage(CORE_1V2);
    analog_write_reg8(0x05, analog_read_reg8(0x05) | BIT(2)); //Power down of 24MHz RC oscillator
#else
    CCLK_96M_HCLK_48M_PCLK_24M;
#endif

    core_interrupt_enable();
}


#endif
