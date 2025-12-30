/********************************************************************************************************
 * @file    tlkhal_api.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once

#define TLKHAL_ASSERT(x) 

#define TLKHAL_DEFAULT (0)

#include "common/types.h"
#include "core/mcu_type.h"

#include "tlksys/platform/api/tlkhal_clock.h"
#include "tlksys/platform/api/tlkhal_dma.h"
#include "tlksys/platform/api/tlkhal_gpio.h"
#if (MCU_CORE_TL752X_TEMP)     // refer to include_path_lynx
#include "drv_uart.h"
#include "drv_dmac.h"
#include "drv_gpio.h"
#include "tlksys/platform/hal/TL752X/tlkhal_uart_TL752X.h"
#endif
#include "tlksys/platform/api/tlkhal_uart.h"
#include "tlksys/platform/api/tlkhal_pwm.h"
#include "tlksys/platform/api/tlkhal_timer.h"
#include "tlksys/platform/api/tlksys_hal_platform.h"
#include "tlksys/platform/api/tlkhal_usb.h"
#include "tlksys/platform/api/tlkhal_i2c.h"

