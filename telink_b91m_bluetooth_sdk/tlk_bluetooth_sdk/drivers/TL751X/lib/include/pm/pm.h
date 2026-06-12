/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
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
#pragma once

#include "reg_include/register.h"
#include "compiler.h"
#include "gpio.h"
#include "lib/include/clock.h"
#include "dma.h"


/**
 * @brief   active mode CORE/SRAM output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 *          As we know, reducing voltage can reduce power consumption in some extent, but for this gear, the benefits may not be significant.
 *          At this gear, it will takes nearly 150us more time to enter sleep mode, which is unacceptable in most scenarios.
 */
#define ALG0X21_DEFAULT_CONFIG  0xca //dvdd2
#define ALG0X22_DEFAULT_CONFIG  0xbc //dvdd1
#define ALG0XB2_DEFAULT_CONFIG  0x00 //bk3

#define PM_AVDD1_CONFIG            PM_AVDD1_VOLTAGE_1V075
#define PM_AVDD2_REF_CONFIG        PM_AVDD2_REF_0V740
#define PM_AVDD2_CONFIG            PM_AVDD2_VOLTAGE_1V890
#define PM_DVDD1_0P8V_CONFIG       PM_DVDD1_VOLTAGE_0V825
#define PM_DVDD2_0P8V_CONFIG       PM_DVDD2_VOLTAGE_0V850

#define PM_BK1_TRIM_CONFIG         PM_BK1_TRIM_VOLTAGE_1V949
#define PM_BK1_ADJ_CONFIG          PM_BK1_ADJ_VOLTAGE_1V850
#define PM_BK2_CONFIG              PM_BK2_3_4_VOLTAGE_1V04
#define PM_BK3_0P8V_CONFIG         PM_BK2_3_4_VOLTAGE_0V93

//When switching to 0.9v to test performance and power consumption, manually calibrate to theoretical gear according to the chip condition.
#define PM_DVDD1_0P9V_CONFIG       PM_DVDD1_VOLTAGE_0V900   //the max
#define PM_DVDD2_0P9V_CONFIG       PM_DVDD2_VOLTAGE_0V925   //the max
#define PM_BK3_0P9V_CONFIG         PM_BK2_3_4_VOLTAGE_1V04  //+4 gear





#define DCDC_WORKAROUND_MODE          0

/**
 * @brief these analog register can store data in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by watchdog, software reboot (sys_reboot()), RESET Pin, power cycle, 32k watchdog, vbus detect.
 */
#define PM_ANA_REG_WD_CLR_BUF1 0x36 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF2 0x37 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF3 0x38 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF4 0x39 // initial value 0x00.

enum
{
    FLD_DIG_LDO_DIG_E_EX = BIT(0),
};

/**
 * @brief analog register below can store information when MCU in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by power cycle, 32k watchdog, RESET Pin,vbus detect.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF1 0x3b // initial value 0x00.
#define PM_ANA_REG_POWER_ON_CLR_BUF2 0x3c // initial value 0xff.

/**
 * @brief   gpio wakeup level definition
 */
typedef enum
{
    WAKEUP_LEVEL_LOW  = 0,
    WAKEUP_LEVEL_HIGH = 1,
} pm_gpio_wakeup_level_e;

/**
 * @brief   wakeup tick type definition
 */
typedef enum
{
    PM_TICK_STIMER = 0, // 24M
    PM_TICK_32K    = 1,
} pm_wakeup_tick_type_e;

/**
 * @brief   d25f retention sleep mode.
 */
typedef enum
{
    D25F_RET_MODE_SRAM_LOW128K = 0x0010, //for boot from sram
    D25F_RET_MODE_SRAM_LOW256K = 0x0030, //for boot from sram
    D25F_RET_MODE_SRAM_LOW384K = 0x0070, //for boot from sram
} pm_d25f_ret_mode_e;

/**
 * @brief   n22 retention sleep mode.
 */
typedef enum
{
    N22_RET_MODE_SRAM_NONE    = 0x0000, //for boot from sram
    N22_RET_MODE_SRAM_LOW128K = 0x0001, //for boot from sram
    N22_RET_MODE_SRAM_LOW256K = 0x0003, //for boot from sram
    N22_RET_MODE_SRAM_LOW384K = 0x0007, //for boot from sram
    N22_RET_MODE_SRAM_LOW512K = 0x000f, //for boot from sram
} pm_n22_ret_mode_e;

/**
 * @brief   dsp retention sleep mode.
 */
typedef enum
{
    DSP_RET_MODE_SRAM_NONE    = 0x0000, //for boot from sram
    DSP_RET_MODE_SRAM_LOW128K = 0x0100, //for boot from sram
    DSP_RET_MODE_SRAM_LOW256K = 0x0300, //for boot from sram
    DSP_RET_MODE_SRAM_LOW384K = 0x0700, //for boot from sram
} pm_dsp_ret_mode_e;

/**
 * @brief   sleep mode.
 */
typedef enum
{
    //available mode for customer
    SUSPEND_MODE   = 0x0000,
    DEEPSLEEP_MODE = 0xf000,   //when use deep mode pad wakeup(low or high level), if the high(low) level always in the pad,
                               //system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20.
                               //deep retention also had this issue, but not to reboot.
    RET_MODE_SRAM_LOW128K_NONE_NONE       = 0x0010, //for boot from sram. d25f:LOW128K, n22:NONE, dsp:NONE
    RET_MODE_SRAM_LOW256K_NONE_NONE       = 0x0030, //for boot from sram. d25f:LOW256K, n22:NONE, dsp:NONE
    RET_MODE_SRAM_LOW384K_NONE_NONE       = 0x0070, //for boot from sram. d25f:LOW384K, n22:NONE, dsp:NONE
    RET_MODE_SRAM_LOW384K_LOW128K_NONE    = 0x0071, //for boot from sram. d25f:LOW384K, n22:LOW128K, dsp:NONE
    RET_MODE_SRAM_LOW384K_LOW256K_NONE    = 0x0073, //for boot from sram. d25f:LOW384K, n22:LOW256K, dsp:NONE
    RET_MODE_SRAM_LOW384K_LOW384K_NONE    = 0x0077, //for boot from sram. d25f:LOW384K, n22:LOW384K, dsp:NONE
    RET_MODE_SRAM_LOW384K_LOW512K_NONE    = 0x007F, //for boot from sram. d25f:LOW384K, n22:LOW512K, dsp:NONE
    RET_MODE_SRAM_LOW384K_LOW512K_LOW128K = 0x017F, //for boot from sram. d25f:LOW384K, n22:LOW512K, dsp:LOW128K
    RET_MODE_SRAM_LOW384K_LOW512K_LOW256K = 0x037F, //for boot from sram. d25f:LOW384K, n22:LOW512K, dsp:LOW256K
    RET_MODE_SRAM_LOW384K_LOW512K_LOW384K = 0x077F, //for boot from sram. d25f:LOW384K, n22:LOW512K, dsp:LOW384K
//  RET_MODE_SRAM_LOW384K_LOW512K_LOW384K = D25F_RET_MODE_SRAM_LOW384K|N22_RET_MODE_SRAM_LOW512K|DSP_RET_MODE_SRAM_LOW384K,

    //not available mode
    DEEPSLEEP_RETENTION_FLAG = 0x0FFF,
} pm_sleep_mode_e;

/**
 * @brief   available wake-up source for customer
 */
typedef enum
{
    PM_WAKEUP_PAD        = BIT(0),
    PM_WAKEUP_CORE       = BIT(1),
    PM_WAKEUP_TIMER      = BIT(2),
    PM_WAKEUP_COMPARATOR = BIT(3), /**<
                                            There are two things to note when using LPC wake up:
                                            1.After the LPC is configured, you need to wait 100 microseconds before go to sleep because the LPC need 1-2 32k tick to calculate the result.
                                              If you enter the sleep function at this time, you may not be able to sleep normally because the data in the result register is abnormal.

                                            2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV, otherwise can not enter sleep normally, crash occurs.
                                          */
    PM_WAKEUP_CTB        = BIT(5),
    PM_WAKEUP_WT         = BIT(6),
    //   PM_WAKEUP_SHUTDOWN     = BIT(7),
} pm_sleep_wakeup_src_e;

/**
 * @brief   wakeup status
 */
typedef enum
{
    WAKEUP_STATUS_PAD        = FLD_WAKEUP_STATUS_PAD,
    WAKEUP_STATUS_CORE       = FLD_WAKEUP_STATUS_CORE,
    WAKEUP_STATUS_TIMER      = FLD_WAKEUP_STATUS_TIMER,
    WAKEUP_STATUS_COMPARATOR = FLD_WAKEUP_STATUS_COMPARATOR,
    WAKEUP_STATUS_ALL        = FLD_WAKEUP_STATUS_ALL,
    WAKEUP_STATUS_INUSE_ALL  = FLD_WAKEUP_STATUS_INUSE_ALL,

    STATUS_GPIO_ERR_NO_ENTER_PM = BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
    STATUS_EXCEED_MAX           = BIT(27),
    STATUS_EXCEED_MIN           = BIT(28),
    STATUS_CLEAR_FAIL           = BIT(29),
    STATUS_ENTER_SUSPEND        = BIT(30),
} pm_suspend_wakeup_status_e;

/**
 * @brief   mcu status
 */
typedef enum
{
    MCU_POWER_ON                 = BIT(0), /**< power on, vbus detect or reset pin */
    //BIT(1) RSVD
    MCU_SW_REBOOT_BACK           = BIT(2), /**< Clear the watchdog status flag in time, otherwise, the system reboot may be wrongly judged as the watchdog.*/
    MCU_DEEPRET_BACK             = BIT(3),
    MCU_DEEP_BACK                = BIT(4),
    MCU_HW_REBOOT_TIMER_WATCHDOG = BIT(5), /**< If determine whether is 32k watchdog/timer watchdog,can also use the interface wd_32k_get_status()/wd_get_status() to determine. */
    MCU_HW_REBOOT_32K_WATCHDOG   = BIT(6), /**< - When the 32k watchdog/timer watchdog status is set to 1, if it is not cleared:
                                              - power cyele/vbus detect/reset pin come back, the status is lost;
                                              - but software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the status remains;
                                              */

    MCU_STATUS_POWER_ON          = MCU_POWER_ON,
    MCU_STATUS_REBOOT_BACK       = MCU_SW_REBOOT_BACK,
    MCU_STATUS_DEEPRET_BACK      = MCU_DEEPRET_BACK,
    MCU_STATUS_DEEP_BACK         = MCU_DEEP_BACK,
} pm_mcu_status;

/**
 * @brief power sel
 * 
 */
typedef enum
{
    PM_POWER_UP   = 0,
    PM_POWER_DOWN = 1,
} pm_power_sel_e;

/**
 * @brief voltage select
 *
 */
typedef enum
{
    VDD_WT           = 0x00,
    VDD_AON_RFWT_0P8 = 0x01,
    VDD_IO           = 0x02,
    VDD_CODEC        = 0x03,
    VDD_RETRAM       = 0x04,
    VDD_RAM          = 0x05,
    VDD_PMTOP        = 0x06,
    VDD_CORE         = 0x07,
} pm_vol_mux_sel_e;

/**
 * @brief   retention mode
 */
typedef struct
{
    unsigned char sleep_mode    : 4;
    unsigned char d25f_ret_mode : 4;
    unsigned char n22_ret_mode  : 4;
    unsigned char dsp_ret_mode  : 4;
} pm_sleep_mode_s;

extern volatile pm_sleep_mode_s g_pm_sleep_mode;

/**
 * @brief   active mode CORE/SRAM output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 *          As we know, reducing voltage can reduce power consumption in some extent, but for this gear, the benefits may not be significant. 
 *          At this gear, it will takes nearly 150us more time to enter sleep mode, which is unacceptable in most scenarios.
 */
typedef enum
{
    DVDD1_DVDD2_VOL_0P8V = 0,
    DVDD1_DVDD2_VOL_0P9V,
} pm_power_cfg_e;

extern pm_power_cfg_e g_dvdd_vol;

/**
 * @brief   early wake up time
 */
typedef struct
{
    unsigned short suspend_early_wakeup_time_us;  /**< suspend_early_wakeup_time_us = deep_ret_r_delay_us + xtal_stable_time + early_time*/
    unsigned short deep_ret_early_wakeup_time_us; /**< deep_ret_early_wakeup_time_us = deep_ret_r_delay_us + early_time*/
    unsigned short deep_early_wakeup_time_us;     /**< deep_early_wakeup_time_us = suspend_ret_r_delay_us*/
    unsigned short sleep_min_time_us;             /**< sleep_min_time_us = suspend_early_wakeup_time_us + 200*/
} pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;

/**
 * @brief   hardware delay time
 */
typedef struct
{
    unsigned short deep_r_delay_cycle;           /**< hardware delay time ,deep_ret_r_delay_us = deep_r_delay_cycle * 1/16k */
    unsigned short suspend_ret_r_delay_cycle;    /**< hardware delay time ,suspend_ret_r_delay_us = suspend_ret_r_delay_cycle * 1/16k */
    unsigned short deep_xtal_delay_cycle;        /**< hardware delay time ,deep_ret_xtal_delay_us = deep_xtal_delay_cycle * 1/16k */
    unsigned short suspend_ret_xtal_delay_cycle; /**< hardware delay time ,suspend_ret_xtal_delay_us = suspend_ret_xtal_delay_cycle * 1/16k */
} pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;

/**
 * @brief   deep sleep wake up status
 */
typedef struct
{
    unsigned char is_pad_wakeup;
    unsigned char wakeup_src; //The pad pin occasionally wakes up abnormally in A0. The core wakeup flag will be incorrectly set in A0.
    unsigned char mcu_status;
    unsigned char power_mode;
} pm_status_info_s;

extern _attribute_aligned_(4) pm_status_info_s g_pm_status_info;

extern _attribute_data_retention_sec_ unsigned char g_pm_vbat_v;
extern unsigned char                                g_areg_aon_7f;

/**
 * @brief       This function serves to get deep retention flag.
 * @return      1 deep retention, 0 deep.
 */
static inline unsigned char pm_get_deep_retention_flag(void)
{
    return !(analog_read_reg8(0x7f) & BIT(0));
}

/**
 * @brief       This function serves to get wakeup source.
 * @return      wakeup source.
 * @note        After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
 *              whether the wake source in use has been cleared, because some of the wake sources
 *              that are not in use may have been set up.
 */
static _always_inline pm_wakeup_status_e pm_get_wakeup_src(void)
{
    return ((pm_wakeup_status_e)analog_read_reg8(areg_aon_0x64));
}

/**
 * @brief       This function serves to clear the wakeup bit.
 * @param[in]   status  - the interrupt status that needs to be cleared.
 * @return      none.
 * @note        To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
 *              instead of FLD_WAKEUP_STATUS_INUSE_ALL.
 */
static _always_inline void pm_clr_irq_status(pm_wakeup_status_e status)
{
    analog_write_reg8(areg_aon_0x64, status);
}

/**
 * @brief       This function serves to set the wakeup source.
 * @param[in]   wakeup_src  - wake up source select.
 * @return      none.
 */
static _always_inline void pm_set_wakeup_src(pm_sleep_wakeup_src_e wakeup_src)
{
    analog_write_reg8(areg_aon_0x4b, wakeup_src);
}

/**
 * @brief       This function serves to enable usb wakeup.
 * @return      none.
 */
static inline void pm_set_usb_wakeup(void)
{
    reg_wakeup_en |= FLD_USB_PWDN_I;
}

/**
 * @brief       This function serves to power on DCDC.
 * @return      none.
 */
static inline void pm_poweron_dcdc(void)
{
    analog_write_reg8(0x07, 0x55);
    core_cclk_delay_tick((unsigned long long)((int)sys_clk.cclk_d25f_dsp * 100));
    analog_write_reg8(0x07, 0xff);
}

/**
 * @brief       This function serves to power down DCDC.
 * @return      none.
 */
static inline void pm_powerdown_dcdc(void)
{
    analog_write_reg8(0x07, 0x00);
}

/**
 * @brief       This function serves to get DCDC power.
 * @return      none.
 */
static inline unsigned char pm_get_dcdc_power(void)
{
    return analog_read_reg8(0x07);
}

/**
 * @brief       This function configures a GPIO pin as the wakeup pin.
 * @param[in]   pin - the pins can be set to all GPIO except PB0/PC5 and GPIOG groups.
 * @param[in]   pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup).
 * @param[in]   en  - enable or disable the wakeup function for the pan pin(1: enable, 0: disable).
 * @return      none.
 */
void pm_set_gpio_wakeup(gpio_pin_e pin, pm_gpio_wakeup_level_e pol, int en);

/**
 * @brief       This function configures pm wakeup time parameter.
 * @param[in]   param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=3, deep=11)
 * @return      none.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param);

/**
 * @brief       This function is used in applications where the crystal oscillator is relatively slow to start.
 *              When the start-up time is very slow, you can call this function to avoid restarting caused
 *              by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]   delay_us - The time wait for xtal stable and flash restore to the active working state in the ramcode
 *                          when wakeup from suspend sleep (default value: 200).
 * @param[in]   loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: loopnum=10).
 * @return      none.
 * @note        Those parameters will be lost after reboot or deep sleep, so it required to be reconfigured.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum);

/**
 * @brief       This function serves to set baseband/usb/npe power on/off before suspend sleep,If power
 *              on this module,the suspend current will increase;power down this module will save current,
 *              but you need to re-init this module after suspend wakeup.All module is power down default
 *              to save current.
 * @param[in]   value - whether to power on/off the baseband/usb/npe.
 * @param[in]   on_off - select power on or off, 0 - power off; other value - power on.
 * @return      none.
 */
void pm_set_suspend_power_cfg(pm_pd_module_e value, unsigned char on_off);

/**
 * @brief       This function serves to get baseband/usb/npe power on/off before suspend sleep,If power
 *              on this module,the suspend current will increase;power down this module will save current,
 *              but you need to re-init this module after suspend wakeup.All module is power down default
 *              to save current.
 * @return      whether to power on/off the baseband/usb/npe.
 */
pm_pd_module_e pm_get_suspend_power_cfg(void);

/**
 * @brief       This function serves to set the working mode of MCU based on 32k crystal,
 *              e.g. suspend mode, deep sleep mode, deep sleep with SRAM retention mode and shutdown mode.
 * @param[in]   sleep_mode          - sleep mode type select.
 * @param[in]   wakeup_src          - wake up source select.
 * @param[in]   wakeup_tick_type    - tick type select. Use 32K tick count for long-time sleep or 24M tick count for short-time sleep.
 * @param[in]   wakeup_tick         - The tick value at the time of wake-up.
                                      If wakeup_tick_type is pm_tick_timer and
                                      if system timer is 24M, the scale range that can be set is about:
                                      The current tick value + (48000 ~ 0xe0000000) ranges from 2ms ~ 156.59 seconds.
                                      If the system timer is 16M, the scale range that can be set is about:
                                      The current tick value + (32000 ~ 0xe0000000) ranges from 2ms ~ 234.88 seconds.
                                      If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K.
                                      The range of tick that can be set is approximately: 64~0xffffffff,
                                      and the corresponding sleep time is approximately: 2ms~37hours.
                                      When it exceeds this range, it cannot sleep properly.
 * @note        There are two things to note when using LPC wake up:
 *              1.After the LPC is configured, you need to wait 100 seconds before you can go to sleep.
 *                After the LPC is opened, 1-2 32k tick is needed to calculate the result.
 *                Before this, the data in the result register is random. If you enter the sleep function at this time,
 *                you may not be able to sleep normally because the data in the result register is abnormal.
 *              2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
 *                otherwise can not enter sleep normally, crash occurs.
 * @return      indicate whether the cpu is wake up successful.
 * @attention   Must ensure that all GPIOs cannot be floating status before going to sleep to prevent power leakage.
 */
_attribute_text_sec_ int pm_sleep_wakeup(pm_sleep_mode_e sleep_mode, pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int wakeup_tick);

/**
 * @brief       This function is used to obtain the cause of software reboot.
 * @return      reboot enum element of pm_poweron_clr_buf0_e.
 * @note        -# the interface pm_update_status_info() must be called before this interface can be invoked;
 */
pm_sw_reboot_reason_e pm_get_sw_reboot_event(void);

/**
 * @brief       This function serves to switch digital module power.
 * @param[in]   module - digital module.
 * @param[in]   power_sel - power up or power down.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_set_dig_module_power_switch(pm_pd_module_e module, pm_power_sel_e power_sel);

/**
 * @brief       This function serves to set dvdd
 * @param[in]   vol      - DVDD1_DVDD2_VOL_0P8V / DVDD1_DVDD2_VOL_0P9V.
 * @param[in]   dma_timeout_us - wait dma all chn complete timeout.
 * @return      DRV_API_SUCCESS - successful;
 *              DRV_API_INVALID_PARAM - equal to the current voltage configuration or dvdd1_dvdd2_vol error;
 *              DRV_API_FAILURE - core error(need contains all the cores used);
 *              DRV_API_TIMEOUT - wait for dma all chn idle timeout to exit;
 *              DRV_API_OTHER_ERROR - clear all interrupt requests failed;
 * @note        1.If the voltage goes up, after calling the interface first, then adjust the frequency;
 *                If the voltage goes down, adjust the frequency first, then call the interface;
 *              2.When adjusting this voltage, no access ram operation is allowed, so it will wait for dma idle in this interface,
 *                modifying dma_timeout_us won't work if there are dma chains working all the time, and needs to be turned off by the upper layers themselves depending on the situation.
 *              3.When adjusting this voltage, the mcu will be stalled because the ram cannot be operated, use the dma method to modify the dvdd configuration and wake up the d25f with this dma interrupt,
 *                so will turns off the general interrupt and clears all interrupt requests.
 *              4.When adjusting this voltage, no access ram operation is allowed, disable swire.
 *              5.If the check configuration fails, reboot.
 *              6.Due to the loss of digital registers during deep/deep ret sleep, while the values of 3V analog registers are maintained, this can result in a mismatch
 *                between the EMA digital register and the actual voltage after deep/deep ret wake-up, which may pose a risk of SRAM usage errors.
 *                Therefore, make sure to call this interface to restore the voltage to 0.8V before going to sleep.
 */
_attribute_ram_code_sec_noinline_ drv_api_status_e pm_set_dvdd(pm_power_cfg_e vol, unsigned int dma_timeout_us);

/**
 * @brief       This function serves to test different voltages from ANA1.
 * @param[in]   mux_sel - select different voltages from ANA1.
 * @return      none.
 */
void pm_set_probe_vol_to_ana1(pm_vol_mux_sel_e mux_sel);

/**
 * @brief       This function serves to update wakeup status.
 * @param[in]   clr_en  - Whether to set the value of the status register to a fixed value.
 *                        If the interface is called twice, the first time it is not modified, clr_en=0;
 *                        if the interface is called once, it is modified, clr_en=1.
 * @return      none.
 * @note        After calling this interface, it is necessary to clear the flag of the timer watchdog or the 32k watchdog.
 *              Otherwise, if the flag remains set, it may affect the next judgment.
 *              After calling this interface, other states are set to fixed values.
 *              Therefore, this interface cannot be called twice,
 *              and if it is called twice, the state will be fixed to one state, not the correct state.
 */
_attribute_ram_code_sec_noinline_ void pm_update_status_info(unsigned char clr_en);

/**
 * @brief       This function serves to set system power mode.
 * @param[in]   power_mode  - power mode(LDO/DCDC).
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_set_power_mode(power_mode_e power_mode);

/**
 * @brief      This function serves to do voltage calibration
 * @param[in]  addr - the frequency offset value address of flash.
 * @return     1 - voltage calibration update, 0 - voltage calibration is not update.
 */
unsigned char flash_calib_ldo_dcdc_voltage(unsigned int addr);
/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/
/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/
/**
 * @brief       When an error occurs, such as the crystal does not vibrate properly, the corresponding recording and reset operations are performed.
 * @param[in]   reboot_reason  - The bit to be configured in the power on buffer.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_sys_reboot_with_reason(pm_sw_reboot_reason_e reboot_reason, unsigned char all_ramcode_en);
