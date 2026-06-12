/********************************************************************************************************
 * @file    tlkapp_sysPm.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#if (TLK_CFG_SYSTEM_ENABLE)

#if (TLK_CFG_WDG_ENABLE)
/**
 * @brief       Timer callback to feed the watchdog.
 * @param[in]   pTimer    - Handle of the timer.
 * @param[in]   userArg   - User-defined argument.
 * @return      none.
 */
static void tlkapp_sys_pm_feedWatchDogTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    wd_clear();
}
#endif

/**
 * @brief       Shuts down the system.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_sys_pm_shutDown(void)
{
#if MCU_CORE_TYPE == MCU_CORE_TL751X
    core_interrupt_disable();
#if (TLK_DEV_LED_ENABLE)
    tlkapp_sysLed_deInit();
#endif
    tlkmdi_tinySql_setSaveEnable(1);
    tlkmdi_tinySql_save();
#if (TLK_DEV_KEY_ENABLE)
#ifdef KEY1_ID
    pm_set_gpio_wakeup(KEY1_GPIO_IN, 0, 0);
#endif
#ifdef KEY2_ID
    pm_set_gpio_wakeup(KEY2_GPIO_IN, 0, 0);
#endif
#ifdef KEY3_ID
    pm_set_gpio_wakeup(KEY3_GPIO_IN, 0, 0);
#endif
#ifdef KEY4_ID
    pm_set_gpio_wakeup(KEY4_GPIO_IN, 0, 0);
#endif
#endif // #if (TLK_DEV_KEY_ENABLE)
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN | FLD_PD_USB_EN | FLD_PD_DSP_EN | FLD_PD_AUDIO_EN | FLD_PD_WT_EN | FLD_PG_CLK_EN, PM_POWER_DOWN);
    tlkhal_gpio_allShutDown();
    pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_LOW, 1);
    gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLUP_1M);
    pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, PM_TICK_32K, 0);
    while (1);
#endif
}

/**
 * @brief       Restarts the watchdog.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_sys_restart_wd(void)
{
#if (TLK_CFG_WDG_ENABLE)
    wd_stop();
    wd_set_interval_ms(5000);
    wd_clear();
    wd_start();
#endif
}

/**
 * @brief       Initializes the power management module.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_pm_init(void)
{
    tlkapp_sys_restart_wd();
#if (TLK_CFG_WDG_ENABLE)
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkappSysFeedWDTimer, 3 * 1000 * 1000, true, tlkapp_sys_pm_feedWatchDogTimer, NULL);
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &sTlkappSysFeedWDTimer);
#endif
}

/**
 * @brief       Handles power management events.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_pm_handler(void)
{
    bool isforce = false;
    if (tlksys_pm_isPowerOffInProgress(&isforce)) {
        tlkapp_sys_pm_shutDown();
    }
    if (tlksys_pm_isHaveClockSwitch(true)) {
        //clock switch ,temp code ,use cb later TODO:ZIYU
        tlkapi_printf(1, "****** cpu clock switch ******");
#if (TLK_CFG_UART_TOOL_ENABLE)
        tlkmdi_comm_reset();
#endif

        tlkapp_sys_restart_wd();
    }
}

/**
 * @brief       USB suspend event
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_usb_suspend_handler(void)
{
    tlkapi_printf(1, "****** usb suspend ******");
#if TLK_STK_TPD_ENABLE
    uint8_t suspend_state;
    suspend_state = TLKMDI_TPD_USB_STATE_ENTER_SUSPEND;
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_tpsll_audio_dongle_usb_suspend_handler, &suspend_state, 1);
#endif

#if (TLK_STK_BT_TPSLL_ENABLE)
    uint8_t suspend_state = true;
    tlkdrv_codec_hd_deinit();
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_bt_tph_usb_suspend_handler, &suspend_state, 1);
#endif
}

/**
 * @brief       USB exit suspend event
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_usb_exit_suspend_handler(void)
{
    tlkapi_printf(1, "****** usb exit suspend ******");
#if TLK_STK_TPD_ENABLE
    uint8_t suspend_state;
    suspend_state = TLKMDI_TPD_USB_STATE_EXIT_SUSPEND;
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_tpsll_audio_dongle_usb_suspend_handler, &suspend_state, 1);
#endif

#if (TLK_STK_BT_TPSLL_ENABLE)
    tlkmw_codec_init();
    uint8_t suspend_state = false;
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_bt_tph_usb_suspend_handler, &suspend_state, 1);
#endif
}
#endif //TLK_CFG_SYSTEM_ENABLE
