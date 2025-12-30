/********************************************************************************************************
 * @file    tlkhal_calibration_B92.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == CHIP_TYPE_B92

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


#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_64K
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_64K 0xe280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_128K
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_128K 0x1e280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_512K
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_512K 0x7e280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_1M
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_1M 0xfe280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_2M
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_2M 0x1fe280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_4M
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_4M 0x3fe280
#endif

#ifndef FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_16M
#define FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_16M 0xffe280
#endif

/**
 * @brief   flash voltage definition
 */
typedef enum
{
    USER_CALIB_FROM_FLASH = 0x01,
    USER_CALIB_FROM_OTP   = 0x02,
} user_calib_from_e;

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in efuse and flash ,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @param[in]   gpio_type - enum variable of the GPIO voltage.
 * @return      none.
 */
void calibration_func_ext(gpio_voltage_e gpio_type);

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return     1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(user_calib_from_e velfrom, unsigned int addr);

/**
 * @brief      This function serves to update the calibration value of rf rx dcoc.
 * @param[in]  velfrom - the calibration value from flash.
 * @param[in]  addr - the rf rx software calibration value address of flash.
 * @return        1 - the rx dcoc calibration value update, 0 - the rx dcoc calibration value is not update.
 */
unsigned short user_calib_rf_rx_dcoc(user_calib_from_e velfrom, unsigned int addr);

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return     1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(user_calib_from_e velfrom, unsigned int addr)
{
    unsigned char frequency_offset_value = 0xff;
    if (velfrom == USER_CALIB_FROM_FLASH) {
        flash_read_page(addr, 1, &frequency_offset_value);
    }
    if (0xff != frequency_offset_value) {
        rf_update_internal_cap(frequency_offset_value);
        return 1;
    }

    return 0;
}

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in efuse and flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @param[in]   gpio_type - enum variable of the GPIO voltage.
 * @return      none.
 */
void calibration_func_ext(gpio_voltage_e gpio_type)
{
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;

    /******get adc calibration value from EFUSE********/
    efuse_calib_adc_vref(gpio_type);

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);

    if (1 == flash_mid_sure) {
        switch (flash_mid[2]) {
        case FLASH_SIZE_64K:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_64K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_64K);
            break;
        case FLASH_SIZE_128K:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_128K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_128K);
            break;
        case FLASH_SIZE_512K:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_512K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_512K);
            break;
        case FLASH_SIZE_1M:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_1M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_1M);
            break;
        case FLASH_SIZE_2M:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_2M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_2M);
            break;
        case FLASH_SIZE_4M:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_4M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_4M);
            break;
        case FLASH_SIZE_16M:
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_16M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_16M);
            break;
        default:
            break;
        }
    } else {
    }
}

void calibration_func(void)
{
    calibration_func_ext(GPIO_VOLTAGE_3V3);
}

/**
 * @brief      This function serves to update the calibration value of rf rx dcoc.
 * @param[in]  velfrom - the calibration value from flash.
 * @param[in]  addr - the rf rx software calibration value address of flash.
 * @return        1 - the rx dcoc calibration value update, 0 - the rx dcoc calibration value is not update.
 */
unsigned short user_calib_rf_rx_dcoc(user_calib_from_e velfrom, unsigned int addr)
{
    unsigned short flash_iq_code = 0xffff;
    if (velfrom == USER_CALIB_FROM_FLASH) {
        flash_read_page(addr, 2, (unsigned char *)&flash_iq_code);
    }

    if ((0xffff != flash_iq_code) && (1 == (flash_iq_code & 0x0001)) && (((flash_iq_code & 0x007e) >> 1) > 0) && (((flash_iq_code & 0x007e) >> 1) < 63) &&
        (((flash_iq_code & 0x1f80) >> 7) > 0) && (((flash_iq_code & 0x1f80) >> 7) < 63)) {
        rf_update_rx_dcoc_calib_code(flash_iq_code);
        return 1;
    }
    return 0;
}


#endif