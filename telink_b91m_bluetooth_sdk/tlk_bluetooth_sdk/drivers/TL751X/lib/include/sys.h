/********************************************************************************************************
 * @file    sys.h
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
/** @page SYS
 *
 *  Introduction
 *  ===============
 *  Clock init and system timer delay.
 *
 *  API Reference
 *  ===============
 *  Header File: sys.h
 */

#ifndef SYS_H_
#define SYS_H_
#include "reg_include/stimer_reg.h"
#include "lib/include/clock.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 * @brief instruction delay
 */

#define _ASM_NOP_       __asm__ __volatile__("nop")

#define CLOCK_DLY_1_CYC _ASM_NOP_
#define CLOCK_DLY_2_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_3_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_4_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_5_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_6_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_7_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_8_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_9_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_10_CYC \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_
#define CLOCK_DLY_64_CYC \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_4_CYC

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 * @brief   Power type for different application
 * @note    In the application, the gear is configured according to four times the maximum peak current
 *          of the total current of the four-way DCDC ((BK1+BK2+BK3+BK4) *4).
*           The lower the gear, the less interference, the better the performance.
*           The higher the gear, the greater the interference and the larger the ripple.
*           If the gear level is set too low and the applied current exceeds the configured peak current,
*           it will cause a power failure or system crash.
 */
typedef enum
{
    LDO_AVDD_LDO_DVDD                  = 0x00, /**< AVDD-LDO & DVDD-LDO mode */
    DCDC_AVDD_DCDC_DVDD_PEAK_CUR_800MA = 0x77, /**< AVDD-DCDC & DVDD-DCDC mode, peak current 800mA default*/
} power_mode_e;

/**
 * @brief   This enumeration is used to select whether VBAT can be greater than 3.6V.
 */
typedef enum
{
    VBAT_MAX_VALUE_GREATER_THAN_3V6 = 0x00,   /**  VBAT must be greater than 2.2V. VBAT may be greater than 3.6V.
                                                <p>  In this configuration the bypass is closed
                                                <p>  and the VBAT voltage passes through the 3V3 LDO to supply power to the chip.
                                                <p>  The voltage of the GPIO pin (VOH) is the voltage after VBAT passes through the LDO (V_ldo),
                                                <p>  and the maximum value is about 3.3V floating 10% (V_ldoh).
                                                <p>  When VBAT > V_ldoh, <p>VOH = V_ldo = V_ldoh(no load).
                                                <p>  When VBAT < V_ldoh, <p>VOH = V_ldo = VBAT(no load) */
    VBAT_MAX_VALUE_LESS_THAN_3V6    = BIT(3), /**  VBAT must be below 3.6V. VBAT may be below 2.2V.
                                                <p>  In this configuration bypass is turned on.vbat is directly supplying power to the chip
                                                <p>  VOH(the output voltage of GPIO)= VBAT */
} vbat_type_e;

typedef enum
{
    D25F = BIT(0),
    N22  = BIT(1),
    DSP  = BIT(2),
} sys_core_e;

/**
 * @brief   chip version.
 * @note    this value should confirm when chip reversion.
 */
typedef enum{
    CHIP_VERSION_A0 = 0x00,
    CHIP_VERSION_A1 = 0x01,
}chip_version_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

extern unsigned int g_chip_version;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief      This function reboot mcu.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void sys_reboot_ram(void);

/**
 * @brief      This function reboot mcu.
 * @return     none
 */
_attribute_text_sec_ void sys_reboot(void);

/**
 * @brief       This function serves to initialize system.
 * @param[in]   power_mode  - power mode
 * @param[in]   vbat_v      - This parameter is used to determine whether the VBAT voltage can be greater than 3.6V.
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void sys_init(power_mode_e power_mode, vbat_type_e vbat_v);

/**
 * @brief       This function serves to initialize dsp core system.
 * @param[in]   addr - start up address
 * @return      none
 * @note        1.Only after calling this function can other DSP related functions be called.
 *              Otherwise, other DSP function settings will not take effect.
 *              2.dsp and cclk is a binding relationship, cclk is how much, dsp is how much,
 *              dsp does not have a corresponding clk configuration interface.
 */
void sys_dsp_init(unsigned int addr);

/**
 * @brief       This function serves to start dsp core system.
 * @return      none
 */
void sys_dsp_start(void);

/**
 * @brief      This function serves to initialize n22 core system.
 * @param[in]  addr - start up address
 * @return     none
 * @note        Only after calling this function can other N22 related functions be called. 
 *              Otherwise, other N22 function settings will not take effect.
 */
void sys_n22_init(unsigned int addr);

/**
 * @brief       This function serves to start n22 core system.
 * @return      none
 */
void sys_n22_start(void);

/**
* @brief       This function serves to stall n22 by dis n22 core/ram clock.
* @return      none
*/
void sys_n22_clk_dis(void);

/**
* @brief       This function serves to start n22.
* @return      none
*/
void sys_n22_clk_en(void);

/**
* @brief       This function serves to stall dsp by dis dsp clock.
* @return      none
*/
void sys_dsp_clk_dis(void);

/**
* @brief       This function serves to start dsp.
* @return      none
*/
void sys_dsp_clk_en(void);

/**
* @brief       This function serves to save n22 clk reg.
* @return      none
*/
void sys_n22_clk_reg_save(void);

/**
* @brief       This function serves to restore n22 clk reg.
* @return      none
*/
void sys_n22_clk_reg_restore(void);

/**
* @brief       This function serves to save dsp clk reg.
* @return      none
*/
void sys_dsp_clk_reg_save(void);

/**
* @brief       This function serves to restore dsp clk reg.
* @return      none
*/
void sys_dsp_clk_reg_restore(void);

// /**
//  * @brief       This function serves to judge whether the module power on.
//  * @module      - pm_pd_module_e
//  * @return      1: power down   0:power on.
//  */
// _Bool pm_dig_module_is_power_on(pm_pd_module_e module);

// /**
//  * @brief       This function serves to judge whether n22 reg write or read.
//  * @return      none
//  */
// unsigned char sys_n22_is_reg_rw_permitted(void);

// /**
//  * @brief       This function serves to judge whether dsp reg write or read.
//  * @return      none
//  */
// unsigned char sys_dsp_is_reg_rw_permitted(void);

/**
 * @brief       This function serves to judge whether dsp/n22 is init.
 * @param[in]   core  - sys_core_e(n22/dsp)
 * @return      0: is init   1/2: no init
 */
unsigned int sys_core_is_initialized(sys_core_e core);

#endif
