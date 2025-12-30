/********************************************************************************************************
 * @file    pm_internal.h
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
#include "lib/include/analog.h"
#include "lib/include/pm/pm.h"

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/

/********************************************************************************************************
 *              This is currently included in the H file for compatibility with other SDKs.
 *******************************************************************************************************/

//When the watchdog comes back, the Eagle chip does not clear 0x7f[0]. To avoid this problem, this macro definition is added.
#ifndef WDT_REBOOT_RESET_ANA7F_WORK_AROUND
    #define WDT_REBOOT_RESET_ANA7F_WORK_AROUND 1
#endif

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG                 0
//1 PB4, 2 PB5
#define PM_SUSPEND_WHILE_DEBUG   0
#define PM_SUSPEND_WHILE_DEBUG_2 0
#define PM_MIN_CODE_DEBUG        0
#define PM_START_CODE_DEBUG      0
#define PM_XTAL_READY_DEBUG      0
#define PM_XTAL_ONCE_DEBUG       0
#define PM_XTAL_READY_TIME       0
#define PM_MANUAL_SETTLE_DEBUG   0

//system timer clock source is constant 24M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through TIMER_32K_LAT_CAL.
//register, the conversion error(use 32k:64 cycle, count 24M sys tmr's ticks), at least the introduction of 64ppm.
#define STIMER_CLOCK_16M 1
#define STIMER_CLOCK_24M 2
#define STIMER_CLOCK     STIMER_CLOCK_24M

#if (STIMER_CLOCK == STIMER_CLOCK_16M)
    #define CRYSTAL32768_TICK_PER_32CYCLE 15625
#elif (STIMER_CLOCK == STIMER_CLOCK_24M)
    #define CRYSTAL32768_TICK_PER_64CYCLE 46875
#endif


extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_calib;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_cur;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_cur;
extern _attribute_data_retention_sec_ unsigned char g_pm_long_suspend;

/**
 * @brief registers related to sleep
 * 
 */
typedef struct {
    unsigned char ana_0x20;//bit2
    unsigned char ana_0x54;//bit6
} pm_sleep_reg_t;

/**
 * @brief for voltage calibration
 */
typedef struct
{
    /* for ldo */
    unsigned char avdd1_1p04;
    unsigned char avdd2_1p8;
    unsigned char dvdd1_0p8;
    unsigned char dvdd1_0p9;
    unsigned char dvdd2_0p8;
    unsigned char dvdd2_0p9;

    unsigned char lc_dvdd1_0p6;
    unsigned char lc_dvdd2_0p6;
    unsigned char vddo1v8;
    unsigned char vddo3v3;

    /* for dcdc */
    unsigned char bk1_1p8;//[2:0]->0xb0[2:0], [5-3]->0xb6[6-4]
    unsigned char bk2_1p04;
    unsigned char bk3_0p85;
    unsigned char bk3_0p95;
    unsigned char bk3_spd_0p6;

    unsigned char is_cal;
}pm_voltage_cal_t;

extern _attribute_data_retention_sec_ pm_voltage_cal_t pm_voltage_cal;

/**
 * @brief   active mode 1.8V IO output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_1V8O_VOLTAGE_1V65 = 0x00, /**< 1.8V IO output 1.65V */
    PM_1V8O_VOLTAGE_1V70 = 0x01, /**< 1.8V IO output 1.70V */
    PM_1V8O_VOLTAGE_1V75 = 0x02, /**< 1.8V IO output 1.75V */
    PM_1V8O_VOLTAGE_1V80 = 0x03, /**< 1.8V IO output 1.80V (default) */
    PM_1V8O_VOLTAGE_1V85 = 0x04, /**< 1.8V IO output 1.85V */
    PM_1V8O_VOLTAGE_1V90 = 0x05, /**< 1.8V IO output 1.90V */
    PM_1V8O_VOLTAGE_1V95 = 0x06, /**< 1.8V IO output 1.95V */
    PM_1V8O_VOLTAGE_2V00 = 0x07, /**< 1.8V IO output 2.00V */
} pm_1v8o_voltage_e;

/**
 * @brief   active mode 3.3V IO output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_3V3O_VOLTAGE_3V10 = 0x00, /**< 3.3V IO output 3.10V */
    PM_3V3O_VOLTAGE_3V15 = 0x01, /**< 3.3V IO output 3.15V */
    PM_3V3O_VOLTAGE_3V20 = 0x02, /**< 3.3V IO output 3.20V */
    PM_3V3O_VOLTAGE_3V25 = 0x03, /**< 3.3V IO output 3.25V */
    PM_3V3O_VOLTAGE_3V30 = 0x04, /**< 3.3V IO output 3.30V (default) */
    PM_3V3O_VOLTAGE_3V35 = 0x05, /**< 3.3V IO output 3.35V */
    PM_3V3O_VOLTAGE_3V40 = 0x06, /**< 3.3V IO output 3.40V */
    PM_3V3O_VOLTAGE_3V45 = 0x07, /**< 3.3V IO output 3.45V */
} pm_3v3o_voltage_e;

/**
 * @brief   active mode AVDD1 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_AVDD1_VOLTAGE_1V050 = 0x00, /**< AVDD1 output 1.050V */
    PM_AVDD1_VOLTAGE_1V075 = 0x01, /**< AVDD1 output 1.075V */
    PM_AVDD1_VOLTAGE_1V100 = 0x02, /**< AVDD1 output 1.100V */
    PM_AVDD1_VOLTAGE_1V125 = 0x03, /**< AVDD1 output 1.125V */
    PM_AVDD1_VOLTAGE_1V150 = 0x04, /**< AVDD1 output 1.150V (default) */
    PM_AVDD1_VOLTAGE_1V175 = 0x05, /**< AVDD1 output 1.175V */
    PM_AVDD1_VOLTAGE_1V200 = 0x06, /**< AVDD1 output 1.200V */
    PM_AVDD1_VOLTAGE_1V225 = 0x07, /**< AVDD1 output 1.225V */
} pm_avdd1_voltage_e;

/**
 * @brief   active mode AVDD2 reference trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_AVDD2_REF_0V700 = 0x00, /**< AVDD2 reference 0.700V */
    PM_AVDD2_REF_0V720 = 0x01, /**< AVDD2 reference 0.720V */
    PM_AVDD2_REF_0V740 = 0x02, /**< AVDD2 reference 0.740V (default) */
    PM_AVDD2_REF_0V760 = 0x03, /**< AVDD2 reference 0.760V */
    PM_AVDD2_REF_0V780 = 0x04, /**< AVDD2 reference 0.780V */
    PM_AVDD2_REF_0V800 = 0x05, /**< AVDD2 reference 0.800V */
    PM_AVDD2_REF_0V820 = 0x06, /**< AVDD2 reference 0.820V */
    PM_AVDD2_REF_0V840 = 0x07, /**< AVDD2 reference 0.840V */
    PM_AVDD2_REF_0V860 = 0x08, /**< AVDD2 reference 0.860V */
    PM_AVDD2_REF_0V880 = 0x09, /**< AVDD2 reference 0.880V */
    PM_AVDD2_REF_0V900 = 0x0a, /**< AVDD2 reference 0.900V */
    PM_AVDD2_REF_0V920 = 0x0b, /**< AVDD2 reference 0.920V */
    PM_AVDD2_REF_0V940 = 0x0c, /**< AVDD2 reference 0.940V */
    PM_AVDD2_REF_0V960 = 0x0d, /**< AVDD2 reference 0.960V */
    PM_AVDD2_REF_0V980 = 0x0e, /**< AVDD2 reference 0.980V */
    PM_AVDD2_REF_1V000 = 0x0f, /**< AVDD2 reference 1.000V */
} pm_avdd2_ref_e;

/**
 * @brief   active mode AVDD2 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_AVDD2_VOLTAGE_1V687 = 0x00, /**< AVDD2 output 1.687V(2) or 2.346(d) */
    PM_AVDD2_VOLTAGE_1V713 = 0x01, /**< AVDD2 output 1.713V(2) or 2.382(d) */
    PM_AVDD2_VOLTAGE_1V740 = 0x02, /**< AVDD2 output 1.740V(2) or 2.420(d) */
    PM_AVDD2_VOLTAGE_1V768 = 0x03, /**< AVDD2 output 1.768V(2) or 2.459(d) */
    PM_AVDD2_VOLTAGE_1V797 = 0x04, /**< AVDD2 output 1.797V(2) or 2.500(d) (default) */
    PM_AVDD2_VOLTAGE_1V827 = 0x05, /**< AVDD2 output 1.827V(2) or 2.541(d) */
    PM_AVDD2_VOLTAGE_1V858 = 0x06, /**< AVDD2 output 1.858V(2) or 2.584(d) */
    PM_AVDD2_VOLTAGE_1V890 = 0x07, /**< AVDD2 output 1.890V(2) or 2.629(d) */
} pm_avdd2_voltage_e;

/**
 * @brief   active mode DVDD1 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_DVDD1_VOLTAGE_0V725 = 0x00, /**< DVDD1 output 0.725V */
    PM_DVDD1_VOLTAGE_0V750 = 0x01, /**< DVDD1 output 0.750V */
    PM_DVDD1_VOLTAGE_0V775 = 0x02, /**< DVDD1 output 0.775V */
    PM_DVDD1_VOLTAGE_0V800 = 0x03, /**< DVDD1 output 0.800V (default) */
    PM_DVDD1_VOLTAGE_0V825 = 0x04, /**< DVDD1 output 0.825V */
    PM_DVDD1_VOLTAGE_0V850 = 0x05, /**< DVDD1 output 0.850V */
    PM_DVDD1_VOLTAGE_0V875 = 0x06, /**< DVDD1 output 0.875V */
    PM_DVDD1_VOLTAGE_0V900 = 0x07, /**< DVDD1 output 0.900V */
} pm_dvdd1_voltage_e;

/**
 * @brief   active mode DVDD2 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_DVDD2_VOLTAGE_0V750 = 0x00, /**< DVDD2 output 0.750V */
    PM_DVDD2_VOLTAGE_0V775 = 0x01, /**< DVDD2 output 0.775V */
    PM_DVDD2_VOLTAGE_0V800 = 0x02, /**< DVDD2 output 0.800V (default) */
    PM_DVDD2_VOLTAGE_0V825 = 0x03, /**< DVDD2 output 0.825V */
    PM_DVDD2_VOLTAGE_0V850 = 0x04, /**< DVDD2 output 0.850V */
    PM_DVDD2_VOLTAGE_0V875 = 0x05, /**< DVDD2 output 0.875V */
    PM_DVDD2_VOLTAGE_0V900 = 0x06, /**< DVDD2 output 0.900V */
    PM_DVDD2_VOLTAGE_0V925 = 0x07, /**< DVDD2 output 0.925V */
} pm_dvdd2_voltage_e;

/**
 * @brief   low power mode DVDD1 or DVDD2 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_DVDD1_LC_DVDD2_LC_0V575 = 0x00, /**< DVDD1 LC or DVDD2 LC output 0.575V */
    PM_DVDD1_LC_DVDD2_LC_0V625 = 0x01, /**< DVDD1 LC or DVDD2 LC output 0.625V */
    PM_DVDD1_LC_DVDD2_LC_0V675 = 0x02, /**< DVDD1 LC or DVDD2 LC output 0.675V */
    PM_DVDD1_LC_DVDD2_LC_0V725 = 0x03, /**< DVDD1 LC or DVDD2 LC output 0.725V (default) */
    PM_DVDD1_LC_DVDD2_LC_0V775 = 0x04, /**< DVDD1 LC or DVDD2 LC output 0.775V */
    PM_DVDD1_LC_DVDD2_LC_0V825 = 0x05, /**< DVDD1 LC or DVDD2 LC output 0.825V */
    PM_DVDD1_LC_DVDD2_LC_0V875 = 0x06, /**< DVDD1 LC or DVDD2 LC output 0.875V */
    PM_DVDD1_LC_DVDD2_LC_0V925 = 0x07, /**< DVDD1 LC or DVDD2 LC output 0.925V */
} pm_dvdd1_lc_dvdd2_lc_voltage_e;

/**
 * @brief   active mode BK1 output adjust definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_BK1_ADJ_VOLTAGE_1V850 = 0x00, /**< BK1 output 1.850V (default) */
    PM_BK1_ADJ_VOLTAGE_1V962 = 0x01, /**< BK1 output 1.962V */
    PM_BK1_ADJ_VOLTAGE_2V083 = 0x02, /**< BK1 output 2.083V */
    PM_BK1_ADJ_VOLTAGE_2V258 = 0x03, /**< BK1 output 2.258V */
    PM_BK1_ADJ_VOLTAGE_2V363 = 0x04, /**< BK1 output 2.363V */
    PM_BK1_ADJ_VOLTAGE_2V462 = 0x05, /**< BK1 output 2.462V */
    PM_BK1_ADJ_VOLTAGE_2V556 = 0x06, /**< BK1 output 2.556V */
} pm_bk1_adj_voltage_e;

/**
 * @brief   active mode BK1 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_BK1_TRIM_VOLTAGE_1V949 = 0x00, /**< BK1 output 1.949V (default) */
    PM_BK1_TRIM_VOLTAGE_1V917 = 0x10, /**< BK1 output 1.917V */
    PM_BK1_TRIM_VOLTAGE_1V882 = 0x20, /**< BK1 output 1.882V */
    PM_BK1_TRIM_VOLTAGE_1V850 = 0x30, /**< BK1 output 1.850V */
    PM_BK1_TRIM_VOLTAGE_2V097 = 0x40, /**< BK1 output 2.097V */
    PM_BK1_TRIM_VOLTAGE_2V056 = 0x50, /**< BK1 output 2.056V */
    PM_BK1_TRIM_VOLTAGE_2V019 = 0x60, /**< BK1 output 2.019V */
    PM_BK1_TRIM_VOLTAGE_1V985 = 0x70, /**< BK1 output 1.985V */
} pm_bk1_trim_voltage_e;

/**
 * @brief   active mode BK1 or BK3 or BK4 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_BK2_3_4_VOLTAGE_0V60 = 0x2C, /**< BK2 or BK3 or BK4 output 0.60V */
    PM_BK2_3_4_VOLTAGE_0V65 = 0x31, /**< BK2 or BK3 or BK4 output 0.65V */
    PM_BK2_3_4_VOLTAGE_0V70 = 0x36, /**< BK2 or BK3 or BK4 output 0.70V */
    PM_BK2_3_4_VOLTAGE_0V78 = 0x3E, /**< BK2 or BK3 or BK4 output 0.78V */
    PM_BK2_3_4_VOLTAGE_0V80 = 0x00, /**< BK2 or BK3 or BK4 output 0.80V (default) */
    PM_BK2_3_4_VOLTAGE_0V82 = 0x02, /**< BK2 or BK3 or BK4 output 0.82V */
    PM_BK2_3_4_VOLTAGE_0V90 = 0x0A, /**< BK2 or BK3 or BK4 output 0.90V */
    PM_BK2_3_4_VOLTAGE_0V93 = 0x0D, /**< BK2 or BK3 or BK4 output 0.93V */
    PM_BK2_3_4_VOLTAGE_0V96 = 0x10, /**< BK2 or BK3 or BK4 output 0.96V */
    PM_BK2_3_4_VOLTAGE_1V00 = 0x14, /**< BK2 or BK3 or BK4 output 1.00V */
    PM_BK2_3_4_VOLTAGE_1V02 = 0x16, /**< BK2 or BK3 or BK4 output 1.02V */
    PM_BK2_3_4_VOLTAGE_1V04 = 0x18, /**< BK2 or BK3 or BK4 output 1.04V */
    PM_BK2_3_4_VOLTAGE_1V10 = 0x1E, /**< BK2 or BK3 or BK4 output 1.10V */
    PM_BK2_3_4_VOLTAGE_1V20 = 0x28, /**< BK2 or BK3 or BK4 output 1.20V */
} pm_bk2_3_4_voltage_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    RET_LDO_TRIM_0P55V = 0x00, /**< retention LDO output 0.550V */
    RET_LDO_TRIM_0P60V = 0x01, /**< retention LDO output 0.600V */
    RET_LDO_TRIM_0P65V = 0x02, /**< retention LDO output 0.650V */
    RET_LDO_TRIM_0P70V = 0x03, /**< retention LDO output 0.700V */
    RET_LDO_TRIM_0P75V = 0x04, /**< retention LDO output 0.750V */
    RET_LDO_TRIM_0P80V = 0x05, /**< retention LDO output 0.800V (default) */
    RET_LDO_TRIM_0P85V = 0x06, /**< retention LDO output 0.850V */
    RET_LDO_TRIM_0P90V = 0x07, /**< retention LDO output 0.900V */
} pm_ret_ldo_trim_e;

/**
 * @brief       This function serves to reboot system.
 * @return      none
 */
_always_inline void sys_reset_all(void)
{
#if (PM_DEBUG)
    while (1)
        ;
#endif
    reg_pwdn_en = 0x20;
}

/**
 * @brief       This function serves to set AVDD1.
 * @param[in]   voltage - avdd1 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_avdd1(pm_avdd1_voltage_e voltage)
{
    analog_write_reg8(0x04, (analog_read_reg8(0x04) & 0xf8) | voltage);
}

/**
 * @brief       This function serves to set AVDD2 voltage.
 * @param[in]   voltage - avdd2 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_avdd2(pm_avdd2_ref_e ref, pm_avdd2_voltage_e voltage)
{
    analog_write_reg8(0x04, (analog_read_reg8(0x04) & 0x0f) | (ref << 4));
    analog_write_reg8(0x22, (analog_read_reg8(0x22) & 0xf8) | voltage);
}

/**
 * @brief       This function serves to set DVDD1 voltage.
 * @param[in]   voltage - dvdd1 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_dvdd1(pm_dvdd1_voltage_e voltage)
{
    analog_write_reg8(0x22, (analog_read_reg8(0x22) & 0x8f) | (voltage << 4));
}

/**
 * @brief       This function serves to set DVDD2 voltage.
 * @param[in]   voltage - dvdd2 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_dvdd2(pm_dvdd2_voltage_e voltage)
{
    analog_write_reg8(0x21, (analog_read_reg8(0x21) & 0xf8) | voltage);
}

/**
 * @brief       This function serves to set BK1 voltage.
 * @param[in]   v_trim - bk1 trim gear.
 * @param[in]   v_adj  - bk1 adj gear.
 * @return      none.
 */
static _always_inline void pm_set_bk1(pm_bk1_trim_voltage_e v_trim, pm_bk1_adj_voltage_e v_adj)
{
    analog_write_reg8(0xb6, (analog_read_reg8(0xb6) & 0x8f) | v_trim);//aon_regb6<6:4>-trim_bk1_vddh<2:0>
    analog_write_reg8(0xb0, (analog_read_reg8(0xb0) & 0xf8) | v_adj);//aon_regb0<2:0>-adjust_bk1_vddh<2:0>
}

/**
 * @brief       This function serves to set BK2 voltage.
 * @param[in]   voltage - bk2 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_bk2(pm_bk2_3_4_voltage_e voltage)
{
    analog_write_reg8(0xb1, (analog_read_reg8(0xb1) & 0xc0) | voltage);
}

/**
 * @brief       This function serves to set BK3 voltage.
 * @param[in]   voltage - bk3 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_bk3(pm_bk2_3_4_voltage_e voltage)
{
    analog_write_reg8(0xb2, (analog_read_reg8(0xb2) & 0xc0) | voltage);
}

/**
 * @brief       This function serves to set BK4 voltage.
 * @param[in]   voltage - bk4 setting gear.
 * @return      none.
 */
static _always_inline void pm_set_bk4(pm_bk2_3_4_voltage_e voltage)
{
    analog_write_reg8(0xb3, (analog_read_reg8(0xb3) & 0xc0) | voltage);
}

/**
 * @brief       This function serves to trim deep retention LDO voltage
 * @param[in]   ret_ldo_trim - deep retention LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_ret_ldo_voltage(pm_ret_ldo_trim_e ret_ldo_trim)
{
    analog_write_reg8(0x10, (analog_read_reg8(0x10) & 0xf8) | ret_ldo_trim);
}

/**
 * @brief       This function serves to trim bk3 in low power mode.
 * @param[in]   bk3_lp_trim - bk3 voltage in low power mode
 * @return      none
 */
static _always_inline void pm_set_bk3_lp_voltage(pm_bk2_3_4_voltage_e bk3_lp_trim)
{
    analog_write_reg8(0xad, (analog_read_reg8(0xad) & 0xc0) | bk3_lp_trim);
}

/**
 * @brief       This function serves to trim dvdd1 and dvdd2 voltage in low power mode.
 * @param[in]   dvdd1_lc_trim - dvdd1 voltage in low power mode
 * @param[in]   dvdd2_lc_trim - dvdd2 voltage in low power mode
 * @return      none
 */
static _always_inline void pm_set_dvdd1_lp_dvdd2_lp_voltage(pm_dvdd1_lc_dvdd2_lc_voltage_e dvdd1_lc_trim, pm_dvdd1_lc_dvdd2_lc_voltage_e dvdd2_lc_trim)
{
    analog_write_reg8(0x0b, (analog_read_reg8(0x0b) & 0x88) | dvdd1_lc_trim | (dvdd2_lc_trim<<4));
}

/**
 * @brief       This function serves to trim IV8O and 3V3O voltage.
 * @param[in]   io1v8_trim - 1.8V IO voltage
 * @param[in]   io3v3_trim - 3.3V IO voltage
 * @return      none
 */
static _always_inline void pm_set_io1v8_io3v3_voltage(pm_1v8o_voltage_e io1v8_trim, pm_3v3o_voltage_e io3v3_trim)
{
    analog_write_reg8(0x0c, (analog_read_reg8(0x0c) & 0x88) | io1v8_trim | (io3v3_trim<<4));
}

/**
 * @brief       This function configures the values of xtal_delay cycle and r_delay cycle.
 * @param[in]   xtal_delay - xtal_delay cycle.
 * @param[in]   r_delay - r_delay cycle.
 * @return      none.
 */
static _always_inline void pm_set_delay_cycle(unsigned char xtal_delay, unsigned char r_delay)
{
    //(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
    analog_write_reg8(areg_aon_0x3d, xtal_delay);
    analog_write_reg8(areg_aon_0x3e, r_delay);
}

/**
 * @brief       This function is used to set reboot reason.
 * @return      none.
 */
static _always_inline void pm_set_reboot_reason(pm_sw_reboot_reason_e reboot_reason)
{
    analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_FLAG | (reboot_reason << 1));
}

/**
 * @brief       This function is used to put the chip into low power mode.
 * @return      none.
 */
static _always_inline void pm_trigger_sleep(void)
{
    //0x80 is to enter low power mode immediately. 0x81 is to wait for D25F to enter wfi mode before entering low power,this way is more secure.
    //Once in the WFI mode, memory transactions that are started before the execution of WFI are guaranteed to have been completed,
    //all transient states of memory handling are flushed and no new memory accesses will take place.
    //only suspend requires this process, after waking up to resume the scene.
    //(add by bingyu.li, confirmed by jianzhi.chen 20230810)
    write_reg8(0x14082f, 0x81); //stall mcu trig
    __asm__ __volatile__("wfi");
}

/**
 * @brief       This function is used to setup the registers before sleep.
 * @return      none.
 */
static _always_inline void pm_setup_regs_before_sleep(pm_sleep_reg_t regs)
{
    //[2]enable the switch between dvdd2 and retram, only deep ret mode need to set this bit
    analog_write_reg8(0x20, regs.ana_0x20);

    //[6]force_sleep_ana, only suspend mode need to set this bit
    //[7]if wakeup src is wt, this bit should never set to 0.
    analog_write_reg8(0x54, regs.ana_0x54);
}

/**
 * @brief       This function is used to recovery the registers after sleep.
 * @return      none.
 */
static _always_inline void pm_recovery_regs_after_sleep(void)
{
    analog_write_reg8(0x20, analog_read_reg8(0x20) & ~(BIT(2)));
    //if wakeup src is wt, 0x54<7> should never set to 0.
    analog_write_reg8(0x54, analog_read_reg8(0x54) & ~(BIT(6)));
}

/**
 * @brief       This function is used to power up 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 */
static _always_inline void pm_24mrc_power_up(void)
{
    if (!g_24m_rc_is_used) {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) & ~(FLD_24M_RC_PD)); //power on 24M RC

        /*
         * the calibration of 24m RC should wait for 1us if just power it up.
         * (added by jilong.liu, confirmed by yangya at 20240805)
        */
        core_cclk_delay_tick((unsigned long long)((int)sys_clk.cclk_d25f_dsp * 2));
    }
}

/**
 * @brief       This function is used to power down 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 * @note        In the following case, please make sure the 24m rc can not be power down.
 *              1. Doing clock switch
 *              2. XTAL start up
 *              3. Doing digital module power switch
 *              4. Enter sleep.
 */
static _always_inline void pm_24mrc_power_down_if_unused(void)
{
    if (!g_24m_rc_is_used) {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) | FLD_24M_RC_PD); //power down 24M RC
    }
}

/**
 * @brief       This function servers to power up BBPLL.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_bbpll_power_up(unsigned char all_ramcode_en);

/**
 * @brief       This function servers to power down BBPLL.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_bbpll_power_down(void);

/**
 * @brief       This function servers to wait BBPLL clock lock.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_bbpll_done(unsigned char all_ramcode_en);

/**
 * @brief       This function servers to powen on bbpll to audio clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_audio_pll_power_up(void);

/**
 * @brief       This function servers to powen down bbpll to audio clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_audio_pll_power_down(void);

/**
 * @brief       This function servers to wait bbpll to audio clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_audio_pll_done(void);

/**
 * @brief       This function is used to determine the stability of the crystal oscillator.
 *              To judge the stability of the crystal oscillator, xo_ready_ana is invalid, and use an alternative solution to judge.
 *              Alternative principle: Because the clock source of the stimer is the crystal oscillator,
 *              if the crystal oscillator does not vibrate, the tick value of the stimer does not increase or increases very slowly (when there is interference).
 *              So first use 24M RC to run the program and wait for a fixed time, calculate the number of ticks that the stimer should increase during this time,
 *              and then read the tick value actually increased by the stimer.
 *              When it reaches 50% of the calculated value, it proves that the crystal oscillator has started.
 *              If it is not reached for a long time, the system will reboot.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @attention   This function can only be called with the 24M clock configuration
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_xtal_ready(unsigned char all_ramcode_en);

// /**
//  * @brief       This function serves to configure power supply.
//  * @param[in]   power_cfg - power config
//  * @return      none
//  * @note        There are three modes can be configure, different configurations correspond to different system power supply modes.
//  *              The configured power supply modules are CORE voltage, BASEBAND voltage and SRAM voltage. The configuration requirements are:
//  *              1. The working voltage of Baseband should be maintained at the same voltage as the CORE
//  *              2. The working voltage of SRAM should not be lower than the CORE voltage and not lower than 0.8V
//  */
// _attribute_ram_code_sec_optimize_o2_noinline_ void pm_power_supply_config(pm_power_cfg_e power_cfg);

/**
 * @brief       this function serves to clear all irq status.
 * @return      Indicates whether clearing irq status was successful.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned char pm_clr_all_irq_status(void);

/**
 * @brief       This function serves to recover system timer.
 *              The code is placed in the ram code section, in order to shorten the time.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_stimer_recover(void);

