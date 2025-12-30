/********************************************************************************************************
 * @file    tlkhal_calibration_B91.c
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
#if MCU_CORE_TYPE == CHIP_TYPE_B91

#ifndef FLASH_ADC_VREF_CALIB_ADDR_64K
#define FLASH_ADC_VREF_CALIB_ADDR_64K 0xe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_128K
#define FLASH_ADC_VREF_CALIB_ADDR_128K 0x1e0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_512K
#define FLASH_ADC_VREF_CALIB_ADDR_512K 0x7e0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_1M
#define FLASH_ADC_VREF_CALIB_ADDR_1M 0xfe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_2M
#define FLASH_ADC_VREF_CALIB_ADDR_2M 0x1fe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_4M
#define FLASH_ADC_VREF_CALIB_ADDR_4M 0x3fe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_16M
#define FLASH_ADC_VREF_CALIB_ADDR_16M 0xffe0c0
#endif

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

unsigned short user_calib_rf_rx_dcoc(user_calib_from_e velfrom, unsigned int addr);

/**
 * @brief      This function is used to calibrate ADC 1.2V vref.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the calibration value address of flash or otp.
 * @return     1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(user_calib_from_e velfrom, unsigned int addr)
{
    /********************************************************************************************
    There have two kind of calibration value of GPIO sampling of ADC 1.2V vref in flash,and one calibration value of GPIO sampling in Efuse.
    Two kind of ADC calibration value in flash are one-point calibration and two-point calibration.
    The kind of ADC calibration value in efuse only is two-point calibration.
    The two-point calibration method will store two values, one is called gain and the other is offset.
    Priority of ADC calibration: two-point calibrate in flash > two-point calibrate  in efuse > one-point calibrate in flash > default of value(1175 mv)
    deviation of two-point calibrate  in flash:-11~7mv.
    deviation of two-point calibrate  in efuse:-9~24mv.
    deviation of one-point calibrate  in flash:-20~20mv.
    The above statistical results are obtained by testing only 19 9213A chips.
********************************************************************************************/
    unsigned char  adc_vref_calib_value[7] = {0};
    unsigned short gpio_calib_value        = 0;
    signed char    gpio_calib_value_offset = 0;
    if (addr == 0) {
        /****** If flash check mid fail,use the Efuse calibration value ********/
        if (0 != efuse_get_adc_calib_value(&gpio_calib_value, &gpio_calib_value_offset)) {
            if ((gpio_calib_value > 1100) && (gpio_calib_value < 1300) && (gpio_calib_value_offset > -20) && (gpio_calib_value_offset < 43)) {
                adc_set_gpio_calib_vref(gpio_calib_value);
                adc_set_gpio_two_point_calib_offset(gpio_calib_value_offset);
                return 1;
            }
        }
    } else {
        if (velfrom == USER_CALIB_FROM_FLASH) {
            flash_read_page(addr, 7, adc_vref_calib_value);
        }
        /****** Check the two-point gpio calibration value whether is exist ********/
        if ((adc_vref_calib_value[4] != 0xff) && (adc_vref_calib_value[4] <= 0x7f) && (((adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5]) != 0xffff)) {
            /****** Method of calculating two-point gpio calibration Flash_gain and Flash_offset value: ********/
            /****** Vref = [(Seven_Byte << 8) + Six_Byte + 1000]mv ********/
            /****** offset = [Five_Byte - 20] mv ********/
            gpio_calib_value        = (adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5] + 1000;
            gpio_calib_value_offset = adc_vref_calib_value[4] - 20;
            if ((gpio_calib_value > 1047) && (gpio_calib_value < 1300) && (gpio_calib_value_offset > -20) && (gpio_calib_value_offset < 107)) {
                adc_set_gpio_calib_vref(gpio_calib_value);
                adc_set_gpio_two_point_calib_offset(gpio_calib_value_offset);
                return 1;
            }
        } else {
            if (0 != efuse_get_adc_calib_value(&gpio_calib_value, &gpio_calib_value_offset)) {
                if ((gpio_calib_value > 1100) && (gpio_calib_value < 1300) && (gpio_calib_value_offset > -20) && (gpio_calib_value_offset < 43)) {
                    adc_set_gpio_calib_vref(gpio_calib_value);
                    adc_set_gpio_two_point_calib_offset(gpio_calib_value_offset);
                    return 1;
                }
            } else {
                /****** Method of calculating one-point calibration Flash_gpio_Vref value: ********/
                /****** Vref = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
                gpio_calib_value = 920 + adc_vref_calib_value[0] + adc_vref_calib_value[1];
                /****** Check the calibration value whether is correct ********/
                if ((gpio_calib_value > 1047) && (gpio_calib_value < 1300)) {
                    adc_set_gpio_calib_vref(gpio_calib_value);
                    return 1;
                }
            }
        }
    }

    return 0;
}

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
 *              This function is to read the calibration value stored in flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @return      none.
 */
void user_read_flash_value_calib(void)
{
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);

    if (1 == flash_mid_sure) {
        switch (flash_mid[2]) {
        case FLASH_SIZE_64K:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_64K);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_64K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_64K);
            break;
        case FLASH_SIZE_128K:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_128K);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_128K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_128K);
            break;
        case FLASH_SIZE_512K:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_512K);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_512K);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_512K);
            break;
        case FLASH_SIZE_1M:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_1M);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_1M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_1M);
            break;
        case FLASH_SIZE_2M:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_2M);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_2M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_2M);
            break;
        case FLASH_SIZE_4M:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_4M);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_4M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_4M);
            break;
        case FLASH_SIZE_16M:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_16M);
            user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_16M);
            user_calib_rf_rx_dcoc(USER_CALIB_FROM_FLASH, FLASH_RF_RX_DCOC_CALI_VALUE_ADDR_16M);
            break;
        default:
            user_calib_adc_vref(USER_CALIB_FROM_FLASH, 0);
            break;
        }
    } else {
        user_calib_adc_vref(USER_CALIB_FROM_FLASH, 0);
    }
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
