/********************************************************************************************************
 * @file    tlksys_hal_platform_TL652X.c
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
#include "tl_common.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_TL652X
/**
 * @brief  Initialize HAL platform
 * @param  None.
 * @returns  None.
 */
void tlksys_hal_platform_init(void)
{
    sys_init(LDO_AVDD_LDO_DVDD, VBAT_MAX_VALUE_GREATER_THAN_3V6);

    // tlkhal_flash_init(0);
    gpio_shutdown(GPIO_ALL);
    gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
    gpio_init(0);
    wd_32k_stop();
    wd_stop();
    clic_preempt_feature_en();
    core_interrupt_enable();
}


#endif
