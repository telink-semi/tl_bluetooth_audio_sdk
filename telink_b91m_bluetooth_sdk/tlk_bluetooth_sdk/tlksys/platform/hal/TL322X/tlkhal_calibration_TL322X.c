/********************************************************************************************************
 * @file    tlkhal_calibration_TL322X.c
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
#if MCU_CORE_TYPE == CHIP_TYPE_TL322X
extern drv_api_status_e efuse_calib_sar_adc_vref(void);

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in efuse and flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @param[in]   none.
 * @return      none.
 */
void calibration_func_ext(void)
{
    /******get sar adc calibration value from EFUSE********/
    efuse_calib_sar_adc_vref();
    /******get sd_adc calibration value from EFUSE********/
    extern drv_api_status_e efuse_calib_sd_adc_vref(void);
    efuse_calib_sd_adc_vref();
}

void calibration_func(void)
{
    calibration_func_ext();
}

#endif