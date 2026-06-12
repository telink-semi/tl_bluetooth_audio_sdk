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
    switch (pCfg->lpTmrCfg) {
    default:
        blc_pm_select_internal_32k_crystal();
        break;
    }

    switch (pCfg->powerCfg) {
    case TLKSYS_HAL_INIT_POWER_CFG_LDO:
        sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3, INTERNAL_CAP_XTAL24M);
        break;
    default:
        sys_init(DCDC_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3, INTERNAL_CAP_XTAL24M);
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

    wd_32k_stop();
    switch (pCfg->calibrationCfg) {
    case TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS:
        break;
    default:
    {
        extern void calibration_func(void);
        calibration_func();
        break;
    }
    }
    CCLK_96M_HCLK_48M_PCLK_24M;
    core_interrupt_enable();
}

#endif
