/********************************************************************************************************
 * @file    app_rc_power_auto_off.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "../app_recording_card_api.h"

#define APP_RC_AUTO_POWER_OFF_TIME 30 //S

static TlkApiTimer_t sAutoPowerOffTimer = {0};
static uint32_t      sAutoPowerOffCnt   = 0;

void app_rc_power_off(void)
{
    core_interrupt_disable();
#if (TLK_DEV_LED_ENABLE)
    tlkapp_sysLed_deInit();
#endif
    tlkmdi_tinySql_setSaveEnable(1);
    tlkmdi_tinySql_save();
    tlkhal_gpio_allShutDown();
#if MCU_CORE_TYPE == MCU_CORE_TL751X
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN | FLD_PD_USB_EN | FLD_PD_DSP_EN | FLD_PD_AUDIO_EN | FLD_PD_WT_EN | FLD_PG_CLK_EN, PM_POWER_DOWN);
#elif MCU_CORE_TYPE == MCU_CORE_TL721X
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN | FLD_PD_USB_EN | FLD_PD_AUDIO_EN, PM_POWER_DOWN);
#endif
#ifdef KEY1_ID
    pm_set_gpio_wakeup(KEY1_GPIO_IN, 0, 0);
#endif
#ifdef KEY2_ID
    pm_set_gpio_wakeup(KEY2_GPIO_IN, 0, 0);
#endif
    tlkdrv_rtc_powerOffSave();
    pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_LOW, 1);
    gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLUP_1M);
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
    pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD | PM_WAKEUP_TIMER, PM_TICK_32K, 10 * 32768); //10S
#else
    pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD | PM_WAKEUP_TIMER, PM_TICK_32K, 10 * 32000); //10S
#endif
    while (1);
}

static void app_rc_power_auto_off_timer(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)handle;
    (void)userArg;
    if (app_rc_api_is_busy()) {
        sAutoPowerOffCnt = 0;
        return;
    }
    sAutoPowerOffCnt++;
    if (sAutoPowerOffCnt < APP_RC_AUTO_POWER_OFF_TIME) {
        return;
    }
    app_rc_power_off();
    tlksys_timer_stop(APP_RC_THREAD_ID, &sAutoPowerOffTimer);
}

void app_rc_power_auto_off_init(void)
{
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAutoPowerOffTimer, 1 * 1000 * 1000, true, app_rc_power_auto_off_timer, NULL);
    tlksys_timer_start(APP_RC_THREAD_ID, &sAutoPowerOffTimer);
}
