/********************************************************************************************************
 * @file    main.c
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
#include "tlklib/usb/tlkusb.h"
#include "tlkapp/tlkapp.h"
#include "stack/system/system.h"
#include "stack/btble.h"
#include "stack/controller.h"
#include "vendor/common/ble_common/tlkble_hal_mode.h"

/**
 * @brief       Overwrite this function to set platform(mcu) config.
 * @param[in]	none
 * @return      Platform initialization configuration structure
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void)
{
    static const tlksys_hal_platform_init_cfg_t cfg = {
        .clockLevel = TLK_CFG_AUDIO_CLOCK_LEVEL,
        
        #if (TLK_CFG_FLASH_PROT_ENABLE)
        .flashProtectEn = 1,
        #endif
    };
    return &cfg;
}

//TODO:in controller thread do following init
/**
 * @brief      Hook function called when system initialization is finished
 * @param[in]  none
 * @return     none
 */
void tlksys_initFinishedHook(void)
{
#if (BLE_CONTROLLER_INITIAL_EN) /*bt controller initial*/
#if (!MCU_DUAL_CORE_ENABLE && (MCU_CORE_TYPE == CHIP_TYPE_TL751X) ||  (MCU_CORE_TYPE == CHIP_TYPE_TL322X))
	sys_n22_init(0x20100000);
#endif
    rf_module_init();
    //////////////////////////// basic hardware Initialization  Begin //////////////////////////////////
    /* random number generator must be initiated here(in the beginning of user initialization).
    * When deepSleep retention wakeUp, no need initialize again */
    trng_init();
#ifndef TL753X_ADAPT
    /* read flash and configure parameter automatically*/
    tlk_readFlashSize_autoConfigCustomFlashSector();

#endif
    /* initialize some basic MCU hardware */
    tlksdk_init_mcu_hardware();

    /*initialize BR/EDR core*/
    controller_init(BT_BLE, HCI_TR_SOC, NULL, NULL);
    //////////////////////////// basic hardware Initialization  End /////////////////////////////////

    tlksdk_sch_init();
#endif
}

/**
 * @brief      Main entry point of the application
 * @param[in]  none
 * @return     Integer value indicating exit status (always 0 in this implementation)
 */
int main(void)
{
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);

#if (!TLK_CFG_RTOS_ENABLE)
    while (1) {
    #if (!MCU_DUAL_CORE_ENABLE)
        tlksdk_main_loop();
    #endif
        tlksys_handler();
    }
#endif

    return 0;
}