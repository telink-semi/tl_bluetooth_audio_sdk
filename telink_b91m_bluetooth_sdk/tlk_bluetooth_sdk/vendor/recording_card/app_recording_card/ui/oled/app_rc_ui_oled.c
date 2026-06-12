/********************************************************************************************************
 * @file    app_rc_ui_oled.c
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
#include "app_oled_fontlibrary.h"
#include "app_oled.h"

void app_rc_ui_oled_init(void)
{
#if (TLK_OLED_UI_EN)
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "welcome", OLED_6X8);
    OLED_ShowString(0, 8, "telink", OLED_6X8);
    OLED_ShowString(0, 16, "test:", OLED_6X8);
    if (OLED_HasChanged()) {
        OLED_Update();
    }
#endif
}
