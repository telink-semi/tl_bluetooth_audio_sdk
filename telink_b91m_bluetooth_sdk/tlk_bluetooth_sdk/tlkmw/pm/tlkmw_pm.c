/********************************************************************************************************
 * @file    tlkmw_pm.c
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
#include "stack/system/system.h"
#include "stack/pm/pm_sys.h"
#include "tlkmw_pm.h"
#include "tlkmw/tpsll/tlkmdi_tpsll_audio_dongle.h"
//for N22 controller: default enable, for D25F Host: configure by user
#if (TLK_CFG_SUSPEND_ENABLE)


/**
 * @brief      Check if tick1 exceeds tick2 considering timer overflow
 * @param[in]  tick1 - First timestamp to compare
 * @param[in]  tick2 - Second timestamp to compare
 * @return     1 if tick1 exceeds tick2, 0 otherwise
 */
static _always_inline int tlkmw_pm_tick1_exceed_tick2(uint32_t tick1, uint32_t tick2)
{
    return (uint32_t)(tick1 - tick2) < BIT(30);
}

/**
 * @brief      callback function of PM Event "PM_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ static void tlkmw_pm_enter(uint8_t e, uint8_t *p, int n)
{
    (void)e;
    (void)p;
    (void)n;
#if defined(MCU_CORE_TL752X_D25F)
    tlksdk_pm_setWakeupSource(PM_D25F_WAKEUP_RTC); //default configuration,timer wakeup
#elif defined(MCU_CORE_TL752X_N22)
    tlksdk_pm_setWakeupSource(PM_N22_WAKEUP_RTC); //default configuration,timer wakeup
#else
    tlksdk_pm_setWakeupSource(PM_WAKEUP_TIMER); //default configuration,timer wakeup
#endif
    /* suspend time > 50ms.add GPIO wake_up */
    if (tlkmw_pm_tick1_exceed_tick2(tlksdk_pm_getWakeupSystemTick(), clock_time() + 50 * SYSTEM_TIMER_TICK_1MS)) {
#if defined(MCU_CORE_TL752X_D25F)
        tlksdk_pm_setWakeupSource(PM_D25F_WAKEUP_PAD); //GPIO PAD wake_up
#elif defined(MCU_CORE_TL752X_N22)
        tlksdk_pm_setWakeupSource(PM_N22_WAKEUP_PAD); //GPIO PAD wake_up
#else
        tlksdk_pm_setWakeupSource(PM_WAKEUP_PAD); //GPIO PAD wake_up
#endif
    }

#if (MCU_CORE_TYPE == CHIP_TYPE_TL721X) && (TLK_STK_TPD_ENABLE)
    tlksdk_pm_setWakeupSource(PM_WAKEUP_PAD);
    /* tpsll audio dongle suspend without connection. */
    tlksdk_pm_removeWakeupSource(PM_WAKEUP_TIMER);
    tlkmdi_tpd_pm_enter_config();
#endif

#if (TLK_USB_REMOTEWAKEUP_EN)
    tlksdk_pm_setWakeupSource(PM_WAKEUP_CORE);
#endif
}

/**
 * @brief      callback function of PM Event "PM_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ static void tlkmw_pm_exit(uint8_t e, uint8_t *p, int n)
{
    (void)e;
    (void)p;
    (void)n;

    uint8_t  buffer[8];
    uint8_t  buffLen   = 0;
    uint32_t exit_tick = clock_time() | 1;
    (void)buffer;
    (void)buffLen;
    (void)exit_tick;

    buffer[buffLen++] = false; // false - exit PM   true - enter PM
    buffer[buffLen++] = (exit_tick & 0xFF000000) >> 24;
    buffer[buffLen++] = (exit_tick & 0xFF0000) >> 16;
    buffer[buffLen++] = (exit_tick & 0xFF00) >> 8;
    buffer[buffLen++] = (exit_tick & 0xFF);

    /* Only for TL751x suspend solution */
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X && !defined(MCU_CORE_N22))
    uint8_t suspend_entered = *p;

    if (suspend_entered) {
        extern unsigned int g_sleep_n22_tick;
        g_sleep_n22_tick = 0;
        sys_n22_init(0x50020000);
        sys_n22_start();
    }
    if ((p[0] & PM_WAKEUP_PAD) || (p[0] & PM_WAKEUP_CORE)) {
/* here executed earlier than TLKSYS_TASK_EVT_SYS_USB_EXIT_SUSPEND, set PM busy first. */
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        tlksys_pm_setChn(TLK_PM_BUSY_CHN_USB, 0, 1);
        if ((p[0] & PM_WAKEUP_PAD)) {
            tlksys_sendMsgFromTimer(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_PM_STATE_CHANGE, buffer, buffLen);
        }
#endif
    }
#elif (MCU_CORE_TYPE == CHIP_TYPE_TL721X)
#if TLK_STK_TPD_ENABLE
    uint8_t wakeup_src = p[0];
    if (wakeup_src & PM_WAKEUP_PAD) {
        tlksys_sendMsgFromTimer(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_PM_STATE_CHANGE, buffer, buffLen);
        tlkmdi_tpsll_audio_dongle_ReconHeadset_fromSuspend();
    }
#endif //TLK_STK_TPD_ENABLE

#endif
}

/**
 * @brief      Initialize power management module
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_init(void)
{
    tlksdk_pm_init();
    tlksdk_pm_enableWfiMode(1);
#if defined(MCU_CORE_TL752X_D25F)
    tlksdk_pm_setWakeupSource(PM_D25F_WAKEUP_PAD | PM_D25F_WAKEUP_RTC); //default enable PAD wake_up src
#elif defined(MCU_CORE_TL752X_N22)
    tlksdk_pm_setWakeupSource(PM_N22_WAKEUP_PAD | PM_N22_WAKEUP_RTC); //default enable PAD wake_up src
#else
    tlksdk_pm_setWakeupSource(PM_WAKEUP_PAD | PM_WAKEUP_TIMER); //default enable PAD wake_up src
#endif
    tlksdk_pm_registerPmEventCallback(PM_EV_FLAG_SLEEP_ENTER, &tlkmw_pm_enter);
    tlksdk_pm_registerPmEventCallback(PM_EV_FLAG_SUSPEND_EXIT, &tlkmw_pm_exit);
    tlksdk_pm_enableSleep(PM_SLEEP_ENABLE);

/* Only for TL751x suspend solution */
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X && !defined(MCU_CORE_N22))
    pm_ext_32k_rc_set_suspend_power_cfg(FLD_PD_ZB_EN, 0);
    tlksdk_pm_setDeepsleepRetentionEarlyWakeupTiming(3000);
#endif
}

/**
 * @brief      Process power management tasks
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_process(void) //Only used for dual core mode, N22 controller project
{
#if (!MCU_CORE_TL752X_TEMP)
    tlksdk_pm_enterSleep(SUSPEND_MODE, 0);
#endif
}

#else
/**
 * @brief      Initialize power management module
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_init(void) {}

/**
 * @brief      Process power management tasks
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_process(void) {}

#endif
