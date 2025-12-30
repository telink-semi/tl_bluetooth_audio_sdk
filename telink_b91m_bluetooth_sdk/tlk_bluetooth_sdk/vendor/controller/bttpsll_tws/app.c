/********************************************************************************************************
 * @file    app.c
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

#include "app.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkapp/tlkapp.h"

#include "app_config.h"
#include "stack/controller.h"
#include "stack/bt/host/bth/bth_hcicmd.h"
#include "stack/btble.h"

#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/multiCoreComm/service/service_mailbox.h"
#include "stack/multiCoreComm/comm.h"
#include "stack/controller.h"
#include "tlkmw/pm/tlkmw_pm.h"

#if (CONTROLLER_MODE == BTTPSLL_TWS)


/**
 * @brief       Initialize user application and hardware modules
 * @param[in]   none
 * @return      none
 */
void user_init(void)
{
    /* initialize some basic MCU hardware */
    if (!pm_sleep_wakeup_check()) // power up normal
    {
        trng_init();

        rf_module_init();

        core_interrupt_enable();

        /* 1. initialize some basic MCU hardware */
        tlksdk_init_mcu_hardware();

        /* 4. initialize scheduler and tph controller. */
        tlksdk_sch_init();

        tlksdk_sch_set_base_interval(PLAN_INTERVAL_10MS);

        /* initialize mailbox irq */
        tlk_multi_core_communication_init();

        /*initialize BR/EDR core*/
        controller_init(BT_TPT, HCI_TR_SOC, NULL, NULL);

        rf_set_power_level_index(RF_POWER_P0dBm);

        tlkmdi_pm_init();

#if (!TLK_LOW_POWER_ENABLE) && TPSLL_LATENCY_LED_DEBUG_EN
        gpio_function_en(GPIO_PB4); //LED_RED
        gpio_output_en(GPIO_PB4);   //LED_RED
#endif

#if (TLK_CFG_UART2USBVCD_ENABLE)
        uart2usb_vcd_init();
#endif

    } else /* power up by D25F suspend wake up logic */
    {
// DBG_MINGQIAN_CHN9_HIGH;
#ifdef SL16_n22_wakeup_tick_l
        u32 wakeup_tick = clock_time();
#endif
        tlksdk_restore_mcu_hardware();
#ifdef SL16_n22_wakeup_tick_l
        log_b16_general(DBG_VCD_PM_EN, SL16_n22_wakeup_tick_l, wakeup_tick);
#endif

#ifdef SL16_n22_wakeup_tick_h
        log_b16_general(DBG_VCD_PM_EN, SL16_n22_wakeup_tick_h, wakeup_tick >> 16);
#endif
        // DBG_MINGQIAN_CHN9_LOW;
    }
}

/**
 * @brief       Main loop function
 *              This function contains the main processing loop for the application,
 *              which includes SDK main loop processing, multi-core communication
 *              handling, and power management processing.
 * @param[in]   none
 * @return      none
 */
void main_loop(void)
{
    tlksdk_main_loop();

    tlk_multi_core_communication_loop();

    tlkmdi_pm_process();
}


#endif
