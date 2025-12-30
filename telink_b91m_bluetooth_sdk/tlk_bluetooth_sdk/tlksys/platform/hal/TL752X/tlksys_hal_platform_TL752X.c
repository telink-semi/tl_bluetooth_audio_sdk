/********************************************************************************************************
 * @file    tlksys_hal_platform_TL752X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL752X

static inline void tlkhal_platform_tl752x_aesClockInit(void)
{
	SET_PROTECT_BIT(CPR->CLK_EN_AHB, CPR_CFG_AHB_BUS_CLK_EN_MSK);
	__DRV_PMU_CLK_EN_M_AO_SET(PMU_CPU1_SYSTEM_CLK_EN_MSK);
	SET_PROTECT_BIT(CPR->CLK_EN_RF_SYSTEM, CPR_BT_SYS_HCLK_EN | CPR_BT_SYS_CLK_EN);
	SET_BIT(CPR->BT_CFG, CPR_BT_SYS_CPU_DM_GCLKEN | CPR_BT_SYS_CPU_EM_GCLKEN);
}

static inline void tlkhal_platform_tl752x_shareRamInit(void)
{
    __DRV_PMU_CLK_EN_M_AO_SET(PMU_CPU1_SYSTEM_CLK_EN | PMU_SHRAM1_CLK_EN);
    __DRV_CPR_CPU_SYSTEM_RST_CLR(CPR_RSTN_SHRAM1);
    //TODO: Now have bug,need contact shuaixing.
}

static void tlksys_hal_tl752x_clockPowerInit(void)
{
    sys_init();
    __DRV_PMU_SET_LDO_DIG_SEL(12);
    __DRV_PMU_SET_LDO_AON_SEL(12);

	PLL0_BBPLL_768M_MCLK_128M_D25F_128M_N22_128M_DSP_128M_MSPI_64M;

    tlkhal_platform_tl752x_aesClockInit();

    tlkhal_platform_tl752x_shareRamInit();

    timer_set_clk(16000000);
}
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
        //            sys_init(LDO_AVDD_LDO_DVDD, VBAT_MAX_VALUE_GREATER_THAN_3V6);
        tlksys_hal_tl752x_clockPowerInit();
        break;
    default:
        //            sys_init(DCDC_AVDD_DCDC_DVDD_PEAK_CUR_800MA, VBAT_MAX_VALUE_GREATER_THAN_3V6);
        tlksys_hal_tl752x_clockPowerInit();

        break;
    }

    switch (pCfg->gpioCfg) {
    case TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN:
        //            gpio_shutdown(GPIO_ALL);	// TODO: Platform adapt
        break;
    default:
#if MCU_CORE_TL752X_TEMP
        //gpio_init(0);
#else
        gpio_init(0);
#endif
        break;
    }

    gpio_set_up_down_res(GPIO_PA10, GPIO_PIN_PULLUP); // SWS
#if (!MCU_CORE_TL752X_TEMP)
    pm_update_status_info(1); // TODO: Platform adapt
#endif

    tlkhal_clock_setLevel(pCfg->clockLevel, NULL);

#if (!MCU_CORE_TL752X_TEMP)
    wd_32k_stop(); // TODO: Platform adapt
    wd_stop();
#endif

    switch (pCfg->calibrationCfg) {
    case TLKSYS_HAL_INIT_CALIBRATION_CFG_DIS:
        break;
    default:
        //            extern void calibration_func(void);	// TODO: ZEWEN API need define
        //            calibration_func();
        break;
    }

    switch (pCfg->lpTmrCfg) {
    default:
        clock_cal_32k_rc();
        mtime_clk_init();
        break;
    }

    flash_plic_preempt_config(1, 1);
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);
    core_interrupt_enable();
}


#endif
