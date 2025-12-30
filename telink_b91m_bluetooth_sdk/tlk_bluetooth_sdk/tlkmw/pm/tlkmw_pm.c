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
//#include "stack/debug/debug_internal.h"

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
_attribute_ram_code_ static void tlkmw_pm_enter(u8 e, u8 *p, int n)
{
    (void)e;
    (void)p;
    (void)n;
    //DBG_COMMON_CHN5_HIGH;
    /* suspend time > 50ms.add GPIO wake_up */
    if (tlkmw_pm_tick1_exceed_tick2(tlksdk_pm_getWakeupSystemTick(), clock_time() + 50 * SYSTEM_TIMER_TICK_1MS)) {
        tlksdk_pm_setWakeupSource(PM_WAKEUP_PAD); //GPIO PAD wake_up
    }
}

/**
 * @brief      callback function of PM Event "PM_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ static void tlkmw_pm_exit(u8 e, u8 *p, int n)
{
    (void)e;
    (void)p;
    (void)n;

    /* Only for TL751x suspend solution */
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X && !defined(MCU_CORE_N22))
    if (!tlksdk_pm_isSleepTickCheckBypass()) {
        u8 suspend_entered = *(u8 *)p;

        if (suspend_entered) {
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
            sys_n22_init(0x50020000);
            sys_n22_start();
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;

        } else {
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
            //DBG_COMMON_CHN5_TOGGLE;
        }
    }
#endif

    //DBG_COMMON_CHN5_LOW;
}

/**
 * @brief      Initialize power management module
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_init(void)
{
    //DBG_COMMON_CHN5_TOGGLE;
    //DBG_COMMON_CHN6_TOGGLE;
    //DBG_COMMON_CHN7_TOGGLE;
    //DBG_COMMON_CHN8_TOGGLE;
    //DBG_COMMON_CHN9_TOGGLE;
    //DBG_COMMON_CHN5_TOGGLE;
    //DBG_COMMON_CHN6_TOGGLE;
    //DBG_COMMON_CHN7_TOGGLE;
    //DBG_COMMON_CHN8_TOGGLE;
    //DBG_COMMON_CHN9_TOGGLE;

    tlksdk_pm_init();
    tlksdk_pm_enableWfiMode(1);
    //tlksdk_pm_setWakeupEarlyUs(TLK_CFG_RTOS_ENABLE ? 100 : 80); //TODO:
    tlksdk_pm_setWakeupSource(PM_WAKEUP_PAD); //default enable PAD wake_up src
    tlksdk_pm_registerPmEventCallback(PM_EV_FLAG_SLEEP_ENTER, &tlkmw_pm_enter);
    tlksdk_pm_registerPmEventCallback(PM_EV_FLAG_SUSPEND_EXIT, &tlkmw_pm_exit);
    tlksdk_pm_enableSleep(PM_SLEEP_ENABLE);

/* Only for TL751x suspend solution */
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X && !defined(MCU_CORE_N22))
    pm_set_suspend_power_cfg(FLD_PD_ZB_EN, 0);
    tlksdk_pm_setDeepsleepRetentionEarlyWakeupTiming(2450);
    tlkmw_pm_enableControllerCoreSleepCheck(0);
#endif
}

/**
 * @brief      Process power management tasks
 * @param      none
 * @return     none
 */
__attribute__((weak)) void tlkmdi_pm_process(void) //Only used for dual core mode, N22 controller project
{
#if (0) // UI logic if needed
    if (0) {
        tlksdk_pm_enableSleep(PM_SLEEP_DISABLE);
    } else {
        tlksdk_pm_enableSleep(PM_SLEEP_ENABLE);
    }
#endif
#if (!MCU_CORE_TL752X_TEMP)
    tlksdk_pm_enterSleep(SUSPEND_MODE, 0);
#else

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

/**
 * @brief      Enable or disable controller core sleep check
 * @param[in]  en - Enable flag. Non-zero to disable sleep check, zero to enable sleep check
 * @return     none
 */
void tlkmw_pm_enableControllerCoreSleepCheck(uint8_t en)
{
    (void)en;
#if TLK_CFG_SUSPEND_ENABLE
    uint8_t enable = en ? 0 : 1;
    tlksdk_pm_bypassSleepTickCheck(enable);
#endif
}
