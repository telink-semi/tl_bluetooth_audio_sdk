/********************************************************************************************************
 * @file    tlksys_hal_platform_TL751X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL751X
/**
 * @brief  Initialize HAL platform
 * @param  None.
 * @returns  None.
 */
void tlksys_hal_platform_init(void)
{
    const tlksys_hal_platform_init_cfg_t *pCfg = tlksys_hal_port_getPlatformInitCfg();
    if (pCfg == NULL) {
        return;
    }
    extern void tlkapp_flash_prot_init(unsigned char flash_protect_en);
    tlkapp_flash_prot_init(pCfg->flashProtectEn);

    extern void tlkapp_flash_enable_4line(unsigned char en);
    switch (pCfg->flashLineCfg) {
    case TLKSYS_HAL_INIT_FLASH_LINE_CFG_4LINE_DIS:
        tlkapp_flash_enable_4line(0);
        break;
    default:
        tlkapp_flash_enable_4line(1);
        break;
    }
    switch (pCfg->powerCfg) {
    case TLKSYS_HAL_INIT_POWER_CFG_LDO:
        sys_init(LDO_AVDD_LDO_DVDD, VBAT_MAX_VALUE_GREATER_THAN_3V6);
        break;
    default:
        sys_init(DCDC_AVDD_DCDC_DVDD_PEAK_CUR_800MA, VBAT_MAX_VALUE_GREATER_THAN_3V6);
        break;
    }

    switch (pCfg->gpioCfg) {
    case TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN:
        gpio_shutdown(GPIO_ALL);
        break;
    default:
        gpio_init(0);
        break;
    }

    gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
    pm_update_status_info(1);

    tlkhal_clock_setLevel(pCfg->clockLevel, NULL);

    wd_32k_stop();
    wd_stop();
    switch (pCfg->calibrationCfg) {
    case TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS:
        break;
    default:
        extern void calibration_func(void);
        calibration_func();
        break;
    }

#if (1) //current driver add this, need check latter TODO:
//The A1 Chip consistency is poor, one is that some chips do not meet the DVDD2>=DVDD1>=0.8 this requirement,
//two is that some chips AVDD1/AVDD2 does not meet the theoretical design value (ldo power supply mode reference value: 1.04v/1.8v):
//When the performance test, the actual measurement of the default voltage, whether to adjust the corresponding voltage block to meet the above conditions.
#if defined(MCU_CORE_TL751X)
    if (g_chip_version == CHIP_VERSION_A1) {
        if (pCfg->powerCfg == TLKSYS_HAL_INIT_POWER_CFG_LDO) {
            pm_set_avdd1(PM_AVDD1_VOLTAGE_1V075);                     //target 1.04
            pm_set_avdd2(PM_AVDD2_REF_0V740, PM_AVDD2_VOLTAGE_1V890); //target 1.8-1.98
            pm_set_dvdd1(PM_DVDD1_VOLTAGE_0V825);                     //target 0.8
            pm_set_dvdd2(PM_DVDD2_VOLTAGE_0V850);                     //target 0.8
        } else {
            pm_set_bk1(PM_BK1_TRIM_VOLTAGE_1V949, PM_BK1_ADJ_VOLTAGE_1V850); //target 1.8-1.98
            pm_set_bk2(PM_BK2_3_4_VOLTAGE_1V04);                             //target 1.04
            pm_set_bk3(PM_BK2_3_4_VOLTAGE_0V93);                             //target 0.8
        }
    }
#endif

#if (defined(MCU_CORE_TL751X) && !defined(MCU_CORE_TL751X_N22) && DCDC_WORKAROUND_MODE)
    if (pm_get_dcdc_power() != 0x00) {
        lpc_set_input_chn(LPC_INPUT_PF6);
        lpc_set_input_ref(LPC_NORMAL, LPC_REF_820MV);
        lpc_set_scaling_coeff(LPC_SCALING_PER100);
        lpc_power_on();
        pm_set_wakeup_src(PM_WAKEUP_COMPARATOR);
        pm_clr_irq_status(FLD_WAKEUP_STATUS_ALL);
        plic_interrupt_enable(IRQ_PM_LVL);
        core_interrupt_enable();
    }
#endif
#endif

    switch (pCfg->lpTmrCfg) {
    case TLKSYS_HAL_INIT_LP_TMR_CFG_32kXTAL:
        blc_pm_select_external_32k_crystal(); //NOT supported, error_code
        break;
    default:
        blc_pm_select_internal_32k_crystal();
        break;
    }

    uint8_t flashPlicThreshold = pCfg->flashPlicThreshold;
    if (flashPlicThreshold < 1) {
        flashPlicThreshold = 1;
    }
    flash_plic_preempt_config(1, flashPlicThreshold);
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);
    core_interrupt_enable();
}


#endif
