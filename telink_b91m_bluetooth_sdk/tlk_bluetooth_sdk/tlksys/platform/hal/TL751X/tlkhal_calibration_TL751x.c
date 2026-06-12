/********************************************************************************************************
 * @file    tlkhal_calibration_TL751x.c
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

#if MCU_CORE_TYPE == MCU_CORE_TL751X

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
    unsigned char frequency_offset_value = 0xff;

    flash_read_page(addr, 1, &frequency_offset_value);

    if (0xff != frequency_offset_value) {
        rf_update_internal_cap(frequency_offset_value);
        return 1;
    }

    return 0;
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
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;
    unsigned int  cap_value_addr = 0;

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check_with_device_num(0, (unsigned int *)flash_mid, flash_uid);

    if (1 == flash_mid_sure) {
        switch (flash_mid[2]) {
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

        if (cap_value_addr) {
            user_calib_freq_offset(cap_value_addr);
            /******get adc calibration value from FLASH********/
            flash_calib_adc_vref(cap_value_addr);

            if (g_chip_version != CHIP_VERSION_A0) {
                flash_calib_ldo_dcdc_voltage(cap_value_addr);
            }
        }
    }
}


#endif
