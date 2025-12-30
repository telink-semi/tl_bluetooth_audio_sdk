/********************************************************************************************************
 * @file    analog_afe3v_aon_reg.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef ANALOG_AFE3V_AON_REG_H
#define ANALOG_AFE3V_AON_REG_H
#include "soc.h"


#define areg_aon_0x05 0x05

enum
{
    FLD_32K_RC_PD      = BIT(0),
    FLD_32K_XTAL_PD    = BIT(1),
    FLD_24M_RC_PD      = BIT(2),
    FLD_48M_XTL_PD     = BIT(3),
    FLD_PLL_ALL_3V     = BIT(4),
    FLD_VBAT_LCLDO_0P8 = BIT(5),
    FLD_PL_VBAT_LDO_3V = BIT(6),
    FLD_ANA_LDO        = BIT(7),
};

#define areg_aon_0x06 0x06

enum
{
    FLD_EN_CURLIMIT    = BIT(0), //Soft start signal of LDOs(avdd1, avdd2, dvdd1, dvdd2)
    FLD_RFVDD_SELECT   = BIT(1), //rfvdd select signal: 1:select dvdd1 0:select vdd_pmtop
    FLD_PD_LC_COMP_3V  = BIT(2), //Power down of low current comparator
    FLD_PD_VBAT_SW     = BIT(3), //power down of bypass switch(VBAT LDO)
    FLD_PD_LDO_DCORE   = BIT(4), //power down of digital core ldo
    FLD_PD_SRAM_LDO    = BIT(5), //power down of ram ldo
    FLD_PD_VDD1P8_OTP  = BIT(6), //power down of vdd1p8 otp
    FLD_PD_DIG_RET_LDO = BIT(7), //power down of retention  ldo
};

#define areg_aon_0x08 0x08

enum
{
    FLD_PD_LDO_AVDD1   = BIT(0),
    FLD_PD_LDO_AVDD2   = BIT(1),
    FLD_PD_LDO_DVDD1   = BIT(2),
    FLD_PD_LDO_DVDD2   = BIT(3),
    FLD_PD_LCLDO_AVDD1 = BIT(4),
    FLD_PD_LCLDO_AVDD2 = BIT(5),
    FLD_PD_LCLDO_DVDD1 = BIT(6),
    FLD_PD_LCLDO_DVDD2 = BIT(7),

    FLD_PD_LCLDO_ANA        = BIT(8),
    FLD_PD_VDD_CORE         = BIT(9),
    FLD_PD_VDD_RAM          = BIT(10),
    FLD_PD_VDD_RETRAM       = BIT(11),
    FLD_PD_BBPLL_AUDIO      = BIT(12),
    FLD_PD_POWER_BBPLL      = BIT(13),
    FLD_EN_BYPASS_LDODIG_3V = BIT(14),
    FLD_EN_BYPASS_LDORAM_3V = BIT(15),
};

#define areg_aon_0x0b 0x0b
#define areg_aon_0x0c 0x0c
#define areg_aon_0x20 0x20

enum
{
    FLD_ENABLE_AUTO_BYPASS_AVDD3 = BIT(0),
    FLD_ENABLE_AUTO_BYPASS_AVDD2 = BIT(1),
    FLD_MSCN_PULLUP_RES_ENB = BIT(4), //mscn 1M pullup enb signal.
};

#define areg_aon_0x21 0x21
#define areg_aon_0x22 0x22
#define areg_aon_0x24 0x24

enum
{
    FLD_BBPLL_ATB              = BIT(0),          //connect bbpll_atb_o to GPIO for test
    FLD_AVDD0P8_TRIM3P3V       = BIT_RNG(1, 3),   //trim bandgap's internal 0.8V voltage
    FLD_BG_TRIM_3V             = BIT_RNG(4, 6),   //trim bandgap's 1.2V voltage REFERENCE
    FLD_EN_BYPASS_AVDD2_3V     = BIT_RNG(7, 9),   //bypass avdd2(short to vbat)
};

#define areg_aon_0x2a 0x2a

enum
{
    FLD_AUTO_PD_32K_RC   = BIT(0),
    FLD_AUTO_PD_32K_XTAL = BIT(1),
    //RSVD
    FLD_AUTO_PD_48M_XTAL       = BIT(3),
    FLD_AUTO_PD_PL_ALL         = BIT(4),
    FLD_AUTO_PD_VBAT_LCLDO_0V8 = BIT(5),
    FLD_AUTO_PD_PL_VBAT_LDO_3V = BIT(6),
    FLD_AUTO_PD_ANA_LDO        = BIT(7),

    FLD_AUTO_EN_CURLIMT = BIT(8),
    //RSVD
    FLD_AUTO_PD_LC_COMP_3V = BIT(10),
    FLD_AUTO_PD_VBAT_SW    = BIT(11),
    FLD_AUTO_PD_LDO_DCORE  = BIT(12),
    FLD_AUTO_PD_LDO_SRAM   = BIT(13),
    FLD_AUTO_PD_VDD1P8_OTP = BIT(14),
    FLD_AUTO_PD_DIG_RET    = BIT(15),

    FLD_AUTO_DCDC_EN_BK1_BIAS_B = BIT(16),
    FLD_AUTO_DCDC_EN_BK1_CNTR_B = BIT(17),
    FLD_AUTO_DCDC_EN_BK2_BIAS_B = BIT(18),
    FLD_AUTO_DCDC_EN_BK2_CNTR_B = BIT(19),
    FLD_AUTO_DCDC_EN_BK3_BIAS_B = BIT(20),
    FLD_AUTO_DCDC_EN_BK3_CNTR_B = BIT(21),
    FLD_AUTO_DCDC_EN_BK4_BIAS_B = BIT(22),
    FLD_AUTO_DCDC_EN_BK4_CNTR_B = BIT(23),

    FLD_AUTO_PD_LDO_AVDD1 = BIT(24),
    FLD_AUTO_PD_LDO_AVDD2 = BIT(25),
    FLD_AUTO_PD_LDO_DVDD1 = BIT(26),
    FLD_AUTO_PD_LDO_DVDD2 = BIT(27),
    FLD_AUTO_AVDD2_BYPASS = BIT(28),
    //RSVD
    FLD_AUTO_ISO_EN         = BIT(30),
    FLD_AUTO_PD_SEQUENCE_EN = BIT(31),
};

#define areg_aon_0x2e 0x2e

enum
{
    //RSVD
    FLD_AUTO_PD_VDD_CORE         = BIT(1),
    FLD_AUTO_PD_VDD_RAM          = BIT(2),
    FLD_AUTO_PD_VDD_RETRAM       = BIT(3),
    FLD_AUTO_PD_BBPLL_AUDIO      = BIT(4),
    FLD_AUTO_PD_POWER_BBPLL      = BIT(5),
    FLD_AUTO_EN_BYPASS_LDODIG_3V = BIT(6),
    FLD_AUTO_EN_BYPASS_LDORAM_3V = BIT(7),
};

/**
 * Customers cannot use analog register 0x35 because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot or power on has occurred. If this bit is 0, it means that sleep has occurred.
 * [Bit1~7]: These bits are used by the driver and cannot be used by the customer.
 */
#define PM_ANA_REG_WD_CLR_BUF0 0x35 // initial value 0xff.

enum
{
    POWERON_FLAG = BIT(0),
};

/**
 * Customers cannot use analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * APPLIC_INTERFACE, it means that manually invoke the reboot interface.
 * XTAL_UNSTABLE, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * PM_CLR_PLIC_REQUEST_FAIL, it means that the pm_sleep_wakeup function failed to clear the PM wake flag bit when using the deep wake source, and the software called sys_reboot().
 * Bit0 has been used and can no longer be used, any combination of other bits can be used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0 0x3a // initial value 0x00.

typedef enum
{
    REBOOT_FLAG      = BIT(0),
    SW_REBOOT_REASON = BIT_RNG(1, 7),
} pm_poweron_clr_buf0_e;

//Since there are only 7 bits, this enumeration ranges from 0 to 127.
typedef enum
{
    APPLIC_INTERFACE         = 0x00,
    XTAL_UNSTABLE            = 0x01,
    PM_CLR_PLIC_REQUEST_FAIL = 0x02,
    WAIT_TIMEOUT             = 0x03,
    PLL_DONE                 = 0x04,
} pm_sw_reboot_reason_e;

#define areg_aon_0x3d 0x3d

#define areg_aon_0x3e 0x3e

#define areg_aon_0x4b 0x4b

#define areg_aon_0x4e 0x4e

enum
{
    FLD_XTAL_QUICK = BIT_RNG(0, 2),
    //RSVD
    FLD_CLK32K_SEL = BIT(7), //0: 32k rc, 1: 32k xtal
};

#define areg_aon_0x4f 0x4f

enum
{
    FLD_RC_32K_CAP     = BIT_RNG(0, 5),
    FLD_RC_32K_CAP_SEL = BIT(6),
    FLD_RC_24M_CAP_SEL = BIT(7),
};

#define areg_aon_0x51 0x51
#define areg_aon_0x52 0x52
#define areg_aon_0x54 0x54

#define areg_aon_0x60 0x60

#define areg_aon_0x64 0x64

typedef enum
{
    FLD_WAKEUP_STATUS_PAD        = BIT(0),
    FLD_WAKEUP_STATUS_CORE       = BIT(1),
    FLD_WAKEUP_STATUS_TIMER      = BIT(2),
    FLD_WAKEUP_STATUS_COMPARATOR = BIT(3),

    //To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
    //instead of FLD_WAKEUP_STATUS_INUSE_ALL.
    FLD_WAKEUP_STATUS_ALL = 0xff,

    //After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
    //whether the wake source in use has been cleared, because some of the wake sources
    //that are not in use may have been set up.
    FLD_WAKEUP_STATUS_INUSE_ALL = 0x0f,
} pm_wakeup_status_e;

#define areg_aon_0x65                0x65
enum {
    //RSVD
    FLD_RESET_XTAL_QICK_START        = BIT(6),
    FLD_RESET_32K_TIMER_CNT          = BIT(7),
};

#define areg_aon_0x69                0x69
enum
{
    FLD_PD_SM_BUSY                   = BIT(5), /*
                                            The pd_sm_busy bit just represents the completion of power switch.
                                            During power switch it will be 1, and after the switch is completed, it will become 0.
                                        */
    FLD_32K_WD_OVERFLOW_STATUS       = BIT(7),
};

#define areg_aon_0x78 0x78

#define areg_aon_0x7d 0x7d

typedef enum
{
    FLD_PD_ZB_EN    = BIT(0), //baseband, for both RF and N22. //weather to power on the BASEBAND before suspend.
    FLD_PD_USB_EN   = BIT(1), //weather to power on the USB before suspend.
    FLD_PD_AUDIO_EN = BIT(2), //weather to power on the NPE before suspend.
    //RSVD
    FLD_PD_DSP_EN = BIT(4),
    FLD_PD_WT_EN  = BIT(5),
    //RSVD
    FLD_PG_CLK_EN = BIT(7), //1:enable change power sequence clk
} pm_pd_module_e;

#define areg_aon_0x7e 0x7e

#define areg_aon_0x7f 0x7f

enum
{
    FLD_BOOTFROMBROM  = BIT(0),
    FLD_PAD_FILTER_EN = BIT(5),
};

#define areg_aon_0xaf 0xaf

enum
{
    FLD_ADJUST_LP_SWITCH_EN  = BIT(4),
};

#define areg_aon_0xb2 0xb2
#endif
