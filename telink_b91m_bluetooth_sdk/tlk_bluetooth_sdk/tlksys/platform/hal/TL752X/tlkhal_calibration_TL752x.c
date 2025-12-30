/********************************************************************************************************
 * @file    tlkhal_calibration_TL752x.c
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

#include "drivers.h"

#if MCU_CORE_TYPE == MCU_CORE_TL752X

#ifndef FLASH_CAP_VALUE_ADDR_64K
    #define FLASH_CAP_VALUE_ADDR_64K 0xe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_128K
    #define FLASH_CAP_VALUE_ADDR_128K 0x1e000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_512K
    #define FLASH_CAP_VALUE_ADDR_512K 0x7e000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_1M
    #define FLASH_CAP_VALUE_ADDR_1M 0xfe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_2M
    #define FLASH_CAP_VALUE_ADDR_2M 0x1fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_4M
    #define FLASH_CAP_VALUE_ADDR_4M 0x3fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_8M
    #define FLASH_CAP_VALUE_ADDR_8M 0x7fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_16M
    #define FLASH_CAP_VALUE_ADDR_16M 0xffe000
#endif

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in efuse and flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @return      none.
 */
void calibration_func(void);

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  addr - the frequency offset value address of flash.
 * @return     1 - the frequency offset update, 0 - the frequency offset is not update.
 * @note       Attention:This function call must be made after rf_mode_init
 */
unsigned char user_calib_freq_offset(unsigned int addr);





/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  addr - the frequency offset value address of flash.
 * @return     1 - the frequency offset update, 0 - the frequency offset is not update.
 * @note       Attention:This function call must be made after rf_mode_init
 */
unsigned char user_calib_freq_offset(unsigned int addr)
{
    (void)addr;
    #if 0
    unsigned char frequency_offset_value = 0xff;

    flash_read_page(addr, 1, &frequency_offset_value);

    if (0xff != frequency_offset_value) {
        rf_update_internal_cap(frequency_offset_value);
        return 1;
    }
#endif
    return 0;
}

/**
 * @brief      This function serves to do voltage calibration
 * @param[in]  addr - the frequency offset value address of flash.
 * @return     1 - voltage calibration update, 0 - voltage calibration is not update.
 */
unsigned char flash_calib_ldo_dcdc_voltage(unsigned int addr)
{
    (void)addr;
    #if 0
    unsigned char voltage_buff[16] = {0};

    if(g_pm_status_info.mcu_status != MCU_DEEPRET_BACK)
    {
        flash_read_page(addr + 0x1c0, 16, voltage_buff);

        //check validation
        for (int i = 0; i < 15; i++) {
            if ((i < 10 && voltage_buff[i] > 8) || (i >= 10 && voltage_buff[i] > 64)) {
                pm_voltage_cal.is_cal = 0;
                return 0;
            }
        }

        pm_voltage_cal.is_cal = 1;

        pm_voltage_cal.avdd1_1p04   = voltage_buff[0] & 0x07;
        pm_voltage_cal.avdd2_1p8    = voltage_buff[1] & 0x07;
        pm_voltage_cal.dvdd1_0p8    = voltage_buff[2] & 0x07;
        pm_voltage_cal.dvdd1_0p9    = voltage_buff[3] & 0x07;
        pm_voltage_cal.dvdd2_0p8    = voltage_buff[4] & 0x07;
        pm_voltage_cal.dvdd2_0p9    = voltage_buff[5] & 0x07;
        pm_voltage_cal.lc_dvdd1_0p6 = voltage_buff[6] & 0x07;
        pm_voltage_cal.lc_dvdd2_0p6 = voltage_buff[7] & 0x07;
        pm_voltage_cal.vddo1v8      = voltage_buff[8] & 0x07;
        pm_voltage_cal.vddo3v3      = voltage_buff[9] & 0x07;

        pm_voltage_cal.bk1_1p8      = voltage_buff[10] & 0x3f;
        pm_voltage_cal.bk2_1p04     = voltage_buff[11] & 0x3f;
        pm_voltage_cal.bk3_0p85     = voltage_buff[12] & 0x3f;
        pm_voltage_cal.bk3_0p95     = voltage_buff[13] & 0x3f;
        pm_voltage_cal.bk3_spd_0p6  = voltage_buff[14] & 0x3f;
    }

    if((g_pm_status_info.mcu_status != MCU_DEEP_BACK) && (g_pm_status_info.mcu_status != MCU_DEEPRET_BACK))
    {
        if (pm_voltage_cal.is_cal == 0)
        {
            pm_set_avdd1(PM_AVDD1_CONFIG);                            //target 1.04
            /*
            * The RF requirement for AVDD2 is 1.7~1.98V, and the audio requirement for AVDD2 is 1.8~2.5V. Overall, the voltage range for AVDD2 is 1.8~1.98V.
            * In LDO mode, set the gear to the middle value of the target at around 1.9V.
            */
            pm_set_avdd2(PM_AVDD2_REF_CONFIG, PM_AVDD2_CONFIG);       //target 1.8-1.98
            pm_set_dvdd1(PM_DVDD1_0P8V_CONFIG);                       //target 0.8
            pm_set_dvdd2(PM_DVDD2_0P8V_CONFIG);                       //target 0.8

            /* TODO
            * The current configuration level has been temporarily determined by the testing team after screening 20 chips.
            * The configuration may be updated based on the ATE big data results in the future.
            * The RF requirement for AVDD2 is 1.7~1.98V, and the audio requirement for AVDD2 is 1.8~2.5V. Overall, the voltage range for AVDD2 is 1.8~1.98V.
            * In DCDC mode, according to previous test data, the default value is already greater than 1.8V, so bk1 just use default value.
            */
            // pm_set_bk1(PM_BK1_TRIM_CONFIG, PM_BK1_ADJ_CONFIG);     //target 1.8-1.98
            pm_set_bk2(PM_BK2_CONFIG);                                //target 1.04
            pm_set_bk3(PM_BK3_0P8V_CONFIG);                           //target 0.8
            // pm_set_bk4(PM_BK4_0P8V_CONFIG);                        //target 0.8

            pm_set_bk3_lp_voltage(PM_BK2_3_4_VOLTAGE_0V70);
        }
        else if (pm_voltage_cal.is_cal == 1)
        {
            pm_set_avdd1(pm_voltage_cal.avdd1_1p04);                      //target 1.04
            pm_set_avdd2(PM_AVDD2_REF_CONFIG, pm_voltage_cal.avdd2_1p8);  //target 1.8-1.98
            pm_set_dvdd1(pm_voltage_cal.dvdd1_0p8);                       //target 0.8
            pm_set_dvdd2(pm_voltage_cal.dvdd2_0p8);                       //target 0.8

            pm_set_bk1((pm_voltage_cal.bk1_1p8 & 0x38) << 1, (pm_voltage_cal.bk1_1p8 & 0x07));        //target 1.8-1.98
            pm_set_bk2(pm_voltage_cal.bk2_1p04);                      //target 1.04
            pm_set_bk3(pm_voltage_cal.bk3_0p85);                      //target 0.8
            // pm_set_bk4(PM_BK4_0P8V_CONFIG);                        //target 0.8

            pm_set_dvdd1_lp_dvdd2_lp_voltage(pm_voltage_cal.lc_dvdd1_0p6, pm_voltage_cal.lc_dvdd2_0p6);

            pm_set_bk3_lp_voltage(pm_voltage_cal.bk3_spd_0p6);

            pm_set_io1v8_io3v3_voltage(pm_voltage_cal.vddo1v8, pm_voltage_cal.vddo3v3);
        }
    }
    #endif

    return 1;
}

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in otp and flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @return      none.
 */
void calibration_func(void)
{
    #if 0
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;
    unsigned int  cap_value_addr = 0;

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check_with_device_num(0, (unsigned int *)flash_mid, flash_uid);

    if (1 == flash_mid_sure)
    {
        switch (flash_mid[2])
        {
        case FLASH_SIZE_64K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_64K;
            break;
        case FLASH_SIZE_128K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_128K;
            break;
        case FLASH_SIZE_512K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_512K;
            break;
        case FLASH_SIZE_1M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_1M;
            break;
        case FLASH_SIZE_2M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_2M;
            break;
        case FLASH_SIZE_4M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_4M;
            break;
        case FLASH_SIZE_8M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_8M;
            break;
        case FLASH_SIZE_16M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_16M;
            break;
        default:
            break;
        }

        if (cap_value_addr)
        {
            user_calib_freq_offset(cap_value_addr);
            /******get adc calibration value from FLASH********/
            flash_calib_adc_vref(cap_value_addr);

            if(g_chip_version != CHIP_VERSION_A0)
            {
                flash_calib_ldo_dcdc_voltage(cap_value_addr);
            }
        }
    }
    #endif
}


#endif
