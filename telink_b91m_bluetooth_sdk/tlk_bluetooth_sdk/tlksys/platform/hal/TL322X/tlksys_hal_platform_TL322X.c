/********************************************************************************************************
 * @file    tlksys_hal_platform_TL322X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL322X
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
    tlkhal_flash_init(!pCfg->flashProtectClose);
    tlkhal_flash_4line_enable();

    /* RTOS 32k source select need this */
    switch (pCfg->lpTmrCfg) {
    default:
        blc_pm_select_internal_32k_crystal();
        break;
    }

    switch (pCfg->powerCfg) {
    case TLKSYS_HAL_INIT_POWER_CFG_LDO:
        sys_init(LDO_1P25_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
        break;
    default:
        sys_init(LDO_1P25_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
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
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN, PM_POWER_UP);
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

    pm_set_dig_ldo(DIG_VOL_1V1_MODE, 1000);
    PLL_192M_D25F_96M_HCLK_N22_48M_PCLK_48M_MSPI_48M;

    flash_plic_preempt_config(1, 1);
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);
    core_interrupt_enable();
}


#endif
