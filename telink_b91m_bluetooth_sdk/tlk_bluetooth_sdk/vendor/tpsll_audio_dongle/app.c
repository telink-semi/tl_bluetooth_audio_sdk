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
#include "stack/tpsll/tpsll.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkapp/tlkapp.h"

#include "app_config.h"
#include "app.h"
#include "tlklib/dbg/tlkdbg.h"

#if (TLK_DEV_KEY_ENABLE)
/**
 * @brief       This function initializes the default configuration for the key module.
 * @return      none.
 * @note    
 */
extern void app_key_defaultConfig_initial(void);
#endif

/**
 * @brief       This function initializes the tpsll audio dongle controller.
 * @return      none.
 * @note    
 */
extern void tpd_controller_init(void);



/**
 * @brief      User initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]  none
 * @return     TLK_ENONE on success
 * @note       Initializes TRNG, flash configuration, basic MCU hardware and scheduler
 */
int tlkapp_init(void)
{

           //////////////////////////// basic hardware Initialization  Begin //////////////////////////////////
    /* random number generator must be initiated here(in the beginning of user initialization).
	 * When deepSleep retention wakeUp, no need initialize again */
    trng_init();

    /* read flash and configure parameter automatically*/
    tlk_readFlashSize_autoConfigCustomFlashSector();

    /* initialize some basic MCU hardware */
    tlksdk_init_mcu_hardware();

    //////////////////////////// basic hardware Initialization  End /////////////////////////////////
    tlksdk_sch_init();

    tlksdk_sch_set_base_interval(PLAN_INTERVAL_10MS);


    return TLK_ENONE;
}

/**
 * @brief     Bluetooth main loop
 * @param[in]  none
 * @return     none
 * @note       Calls the SDK main loop and system handler functions
 */
_attribute_no_inline_ void tlkapp_main_loop(void)
{
    tlksdk_main_loop();

    tlksys_handler();

}

/**
 * @brief		TPSLL PWM interrupt handler
 * @param[in]	none
 * @return      none
 * @note        Handles PWM interrupts for TPSLL, with support for simulation mode
 */


/**
 * @brief       This function handles the PWM IRQ for tpsll audio dongle.
 * @return      none.
 * @note    
 */
extern void tpd_pwm_irq_handler(void);

/**
 * @brief       This function handles the PWM IRQ for tpsll.
 * @return      none.
 * @note    
 */
_attribute_retention_code_ void tpsll_pwm_irq_handler(void)
{
#if (SCHEDULER_SIMULATE_TASK_ENABLE)
    tlkstk_tpsll_simu_pwm_irq_handler();
#else
    DBG_CHN10_HIGH;
    tpd_pwm_irq_handler();
    DBG_CHN10_LOW;
#endif
}
PLIC_ISR_REGISTER(tpsll_pwm_irq_handler, IRQ_PWM)
