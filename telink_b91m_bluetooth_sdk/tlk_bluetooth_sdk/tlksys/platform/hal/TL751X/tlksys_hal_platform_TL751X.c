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

static inline void tlkhal_core_pmp_enable(void)
{
#define PMPCFG_LAXWR(l, a, x, w, r) (((l) << 7) | ((a) << 3) | ((x) << 2) | ((w) << 1) | ((r) << 0))
    extern uint32_t _RAMCODE_VMA_START;
    extern uint32_t _RAMCODE_VMA_END;

    uint32_t pmpcfg = 0;

    //step0: lock [0, 0x20000) ram to avoid use NULL addr.
    write_csr(NDS_PMPADDR0, ((0x20000UL) >> 2));
    pmpcfg = PMPCFG_LAXWR(1, 1, 0, 0, 0);
    write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~((0xFF) << ((0 % 4) << 3)))) | (((long)pmpcfg) << ((0 % 4) << 3))));

    //step1: lock ram_code,can't be written.
    write_csr(NDS_PMPADDR1, ((uint32_t)(&_RAMCODE_VMA_START)) >> 2);
    pmpcfg = PMPCFG_LAXWR(1, 1, 1, 1, 1);
    write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~((0xFF) << ((1 % 4) << 3)))) | (((long)pmpcfg) << ((1 % 4) << 3))));
    write_csr(NDS_PMPADDR2, ((uint32_t)(&_RAMCODE_VMA_END)) >> 2);
    pmpcfg = PMPCFG_LAXWR(1, 1, 1, 0, 1);
    write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~((0xFF) << ((2 % 4) << 3)))) | (((long)pmpcfg) << ((2 % 4) << 3))));
}

/**
 * @brief  Initialize HAL platform
 * @param  None.
 * @returns  None.
 */
void tlksys_hal_platform_init(void)
{
    (void)tlkhal_core_pmp_enable;
    // tlkhal_core_pmp_enable();
    const tlksys_hal_platform_init_cfg_t *pCfg = tlksys_hal_port_getPlatformInitCfg();
    if (pCfg == NULL) {
        return;
    }

    tlkhal_flash_init(!pCfg->flashProtectClose);
    tlkhal_flash_4line_enable();

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


    switch (pCfg->lpTmrCfg) {
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
