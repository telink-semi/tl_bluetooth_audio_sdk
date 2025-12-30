/********************************************************************************************************
 * @file    tlkhal_calibration_TL721x.c
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
//#include "calibration.h"
#include "drivers.h"

#if MCU_CORE_TYPE == CHIP_TYPE_TL721X

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

#ifndef FLASH_CAP_VALUE_ADDR_16M
    #define FLASH_CAP_VALUE_ADDR_16M 0xffe000
#endif

/********************IEEE ADDRESS IN FLASH***********************/
#ifndef FLASH_IEEE_ADDR_LOCATION_64K
    #define FLASH_IEEE_ADDR_LOCATION_64K 0xf000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_128K
    #define FLASH_IEEE_ADDR_LOCATION_128K 0x1f000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_512K
    #define FLASH_IEEE_ADDR_LOCATION_512K 0x7f000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_1M
    #define FLASH_IEEE_ADDR_LOCATION_1M 0xff000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_2M
    #define FLASH_IEEE_ADDR_LOCATION_2M 0x1ff000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_4M
    #define FLASH_IEEE_ADDR_LOCATION_4M 0x3ff000
#endif

#ifndef FLASH_IEEE_ADDR_LOCATION_16M
    #define FLASH_IEEE_ADDR_LOCATION_16M 0xfff000
#endif

typedef enum
{
    IEEE_ADDR_NOT_EXIST,
    IEEE_ADDR_FROM_FLASH,
    IEEE_ADDR_FROM_OTP,
} ieee_addr_source_e;

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
 */
unsigned char user_calib_freq_offset(unsigned int addr);

/**
 * @brief      This function serves to read IEEE address from FLASH or OTP.
 * @param[in]  addr - the IEEE address of flash.
 * @param[out] buf  - Pointer to IEEE address buffer
 * @return     ieee_addr_source_e
 */
ieee_addr_source_e user_get_ieee_addr(unsigned int addr, unsigned char *mac_addr_buf);


/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  addr - the frequency offset value address of flash.
 * @return     1 - the frequency offset update, 0 - the frequency offset is not update.
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
    unsigned int  ieee_flash_pos = 0, cap_value_addr = 0;
    unsigned char ieee_addr[8];

    /******get adc calibration value from OTP********/
    otp_calib_adc_vref();

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check_with_device_num(0, (unsigned int *)flash_mid, flash_uid);


    if (1 == flash_mid_sure) {
        switch (flash_mid[2]) {
        case FLASH_SIZE_64K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_64K;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_64K;
            break;
        case FLASH_SIZE_128K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_128K;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_128K;
            break;
        case FLASH_SIZE_512K:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_512K;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_512K;
            break;
        case FLASH_SIZE_1M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_1M;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_1M;
            break;
        case FLASH_SIZE_2M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_2M;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_2M;
            break;
        case FLASH_SIZE_4M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_4M;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_4M;
            break;
        case FLASH_SIZE_16M:
            cap_value_addr = FLASH_CAP_VALUE_ADDR_16M;
            ieee_flash_pos = FLASH_IEEE_ADDR_LOCATION_16M;
            break;
        default:
            break;
        }
        if (cap_value_addr) {
            user_calib_freq_offset(cap_value_addr);
        }
        if (ieee_flash_pos) {
            user_get_ieee_addr(ieee_flash_pos, ieee_addr);
        }

    } else {
        user_get_ieee_addr(0, ieee_addr);
    }
}

/**
 * @brief      This function serves to check if IEEE address is legal.
 * @param[in]  value  - Pointer to value address buffer
 * @return     0: value address is illegal, 1:value address is legal
 */
unsigned char user_check_ieee_addr(unsigned char *value)
{
    //IEEE address is legal if it is not all 0x00 or all 0xFF.
    for (int i = 1; i < 8; i++) {
        if (value[i] != value[0]) {
            return 1;
        }
    }
    //If all elements are the same, check if it is all 0x00 or all 0xFF
    return (value[0] == 0x00 || value[0] == 0xFF) ? 0 : 1;
}

/**
 * @brief      This function serves to read IEEE address from OTP.
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 8bytes)
 * @return     ieee_addr_source_e
 */
ieee_addr_source_e user_get_otp_ieee_addr(unsigned char *buf)
{
    //Read IEEE address priority: FLASH > OTP
    otp_get_ieee_addr(buf);
    if (user_check_ieee_addr(buf)) {
        return IEEE_ADDR_FROM_OTP;
    } else {
        return IEEE_ADDR_NOT_EXIST;
    }
}

/**
 * @brief      This function serves to read IEEE address from FLASH or OTP.
 * @param[in]  addr - the IEEE address of flash
 *                    0:     Read the IEEE address from OTP.
 *                    other: first read the IEEE address from the flash memory.
 *                           If it is not a valid value, then read it from OTP.
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 8bytes)
 * @return     ieee_addr_source_e
 */
ieee_addr_source_e user_get_ieee_addr(unsigned int addr, unsigned char *buf)
{
    if (addr == 0) {
        return user_get_otp_ieee_addr(buf);
    } else {
        //Read IEEE address priority: FLASH > OTP
        flash_read_page(addr, 8, buf);
        if (user_check_ieee_addr(buf)) {
            return IEEE_ADDR_FROM_FLASH;
        } else {
            return user_get_otp_ieee_addr(buf);
        }
    }
}



#endif
