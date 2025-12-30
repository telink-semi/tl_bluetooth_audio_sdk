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

#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkapp/tlkapp.h"
#include "stack/tpsll/tpsll.h"
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

#if (CONTROLLER_MODE == BTBLE_TPSLL_HEADSET)


#if (CHIP_TYPE == CHIP_TYPE_TL752X)
/**
 * @brief       Initialize GPIOs for debugging purposes
 * @param[in]   none
 * @return      none
 */
void gpio_debug_init(void)
{
    gpio_function_en(GPIO_PA5);
    gpio_function_en(GPIO_PA6);
    gpio_function_en(GPIO_PA7); //inq start
    gpio_function_en(GPIO_PA8); //loop
    gpio_function_en(GPIO_PA9); //inq task
                                //    gpio_function_en(GPIO_PA10); //stimer
                                //    gpio_function_en(GPIO_PA11); //sch build
                                //    gpio_function_en(GPIO_PA12); //stimer_statu
                                //    gpio_function_en(GPIO_PA13);

    gpio_output_en(GPIO_PA5);
    gpio_output_en(GPIO_PA6);
    gpio_output_en(GPIO_PA7);
    gpio_output_en(GPIO_PA8);
    gpio_output_en(GPIO_PA9);
    //    gpio_output_en(GPIO_PA10);
    //    gpio_output_en(GPIO_PA11);
    //    gpio_output_en(GPIO_PA12);
    //    gpio_output_en(GPIO_PA13);


    gpio_set_low_level(GPIO_PA5);
    gpio_set_low_level(GPIO_PA6);
    gpio_set_low_level(GPIO_PA7);
    gpio_set_low_level(GPIO_PA8);
    gpio_set_low_level(GPIO_PA9);

    //    gpio_set_low_level(GPIO_PA10);
    //    gpio_set_low_level(GPIO_PA11);
    //    gpio_set_low_level(GPIO_PA12);
    //    gpio_set_low_level(GPIO_PA13);

    gpio_function_en(GPIO_PB5);
    gpio_function_en(GPIO_PB6);
    gpio_function_en(GPIO_PB7);
    gpio_function_en(GPIO_PB8);
    gpio_function_en(GPIO_PB9);
    gpio_function_en(GPIO_PB10);
    gpio_function_en(GPIO_PB11);

    gpio_output_en(GPIO_PB5);
    gpio_output_en(GPIO_PB6);
    gpio_output_en(GPIO_PB7);
    gpio_output_en(GPIO_PB8);
    gpio_output_en(GPIO_PB9);
    gpio_output_en(GPIO_PB10);
    gpio_output_en(GPIO_PB11);


    gpio_set_low_level(GPIO_PB5);
    gpio_set_low_level(GPIO_PB6);
    gpio_set_low_level(GPIO_PB7);
    gpio_set_low_level(GPIO_PB8);
    gpio_set_low_level(GPIO_PB9);
    gpio_set_low_level(GPIO_PB10);
    gpio_set_low_level(GPIO_PB11);
}
#endif

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

        /*initialize TPSLL / BR/EDR / BLE core*/
#if (OS_LEVEL_ENABLE == 1 && CHIP_TYPE != CHIP_TYPE_TL752X)
        controller_init(BT_TPH, HCI_TR_SOC, NULL, NULL);
#else
        controller_init(BT_TPH, HCI_TR_SOC, NULL, NULL);
#endif

/* Notice: unRegister this module can avoided TPSLL funcation abnormal.
        1. In SRC code, this project unRegister BLE task default. 
        2. In SDK code, when the SDK_is release, RELEASE_CHECK_EN will enable. Because Controller use this project build library. 
        3. When SDK is ready, open here manually. */
#if (1)
        extern void tlk_pm_unRegisterCheckSleepReadyCallback(u32 taskTypeRdyChk);
        tlk_pm_unRegisterCheckSleepReadyCallback(2); // PM_BLE_CTRL_SCH_TASKS
#endif

#if (CHIP_TYPE != CHIP_TYPE_TL752X)
        rf_set_power_level_index(RF_POWER_P0dBm);

        tlkmdi_pm_init();

#if (!TLK_LOW_POWER_ENABLE) && TPSLL_LATENCY_LED_DEBUG_EN
        gpio_function_en(GPIO_PB4); //LED_RED
        gpio_output_en(GPIO_PB4);   //LED_RED
#endif
#else
        gpio_debug_init();
#endif
    } else /* power up by D25F suspend wake up logic */
    {
        tlksdk_restore_mcu_hardware();
    }
}

/**
 * @brief       Main loop function for user application
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
