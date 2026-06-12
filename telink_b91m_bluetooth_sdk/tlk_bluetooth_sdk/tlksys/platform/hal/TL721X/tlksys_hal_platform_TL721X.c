/********************************************************************************************************
 * @file    tlksys_hal_platform_TL721X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL721X
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

    /* For TL721X, must before sys_init, sys_init use ext_driver(from BLE SDK legacy design).
     * DRV pm_sleep_apis, RTOS 32k source, sys_pm module need this */
    switch (pCfg->lpTmrCfg) {
    case TLKSYS_HAL_INIT_LP_TMR_CFG_32kXTAL:
        blc_pm_select_external_32k_crystal();
        break;
    default:
        blc_pm_select_internal_32k_crystal();
        break;
    }
    tlkhal_flash_init(!pCfg->flashProtectClose);
    tlkhal_flash_4line_enable();

    switch (pCfg->powerCfg) {
    default:
        sys_init(DCDC_0P94_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
        break;
    }

    gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
    wd_32k_stop();
    wd_stop();
#if (PROJ_TPSLL_AUDIO_DONGLE)
    pm_set_dvdd(CORE_0P9V_SRAM_0P9V_BB_0P9V, DMA1, 1000);
#endif
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) & 0x0f) | (12 << 4));
    analog_write_reg8(0x0e, (analog_read_reg8(0x0e) & 0x8f) | (5 << 4));

    tlkhal_clock_setLevel(pCfg->clockLevel, NULL);

    pm_update_status_info(1);

    switch (pCfg->gpioCfg) {
    case TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN:
        gpio_shutdown(GPIO_ALL);
        break;
    default:
        gpio_init(0);
        break;
    }
    switch (pCfg->calibrationCfg) {
    case TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS:
        break;
    default:
        extern void calibration_func(void);
        calibration_func();
        break;
    }

    flash_plic_preempt_config(1, 1);
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);
    core_interrupt_enable();
}

#endif
