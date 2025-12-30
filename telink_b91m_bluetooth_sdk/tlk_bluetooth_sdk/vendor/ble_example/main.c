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
#include "stack/ble/ble.h"
#include "app_example.h"
#include "stack/controller.h"
#include "vendor/common/ble_common/tlkble_hal_mode.h"

/**
 * @brief  Initialize BLE stack.
 *          Initialize MAC address from flash and initialize BLE host with the MAC address.
 */
void ble_stack_init(void)
{
    uint8_t mac[6], mac_random[6];

    blc_initMacAddress(flash_sector_mac_address, mac, mac_random);

    ble_host_v1_init();
    ble_host_v1_set_bd_addr(mac);
}

/**
 * @brief  Get platform (MCU) initialization configuration.
 *          Overwrite this function to set platform-specific configuration.
 *
 * @return  Pointer to platform initialization configuration structure.
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void)
{
    static const tlksys_hal_platform_init_cfg_t cfg = {
        .clockLevel = TLK_CFG_AUDIO_CLOCK_LEVEL,
    };
    return &cfg;
}

/**
 * @brief  System initialization finished hook function.
 *          Initialize RF module, MCU hardware, BLE controller and scheduler.
 *          Called after system initialization is completed.
 */
void tlksys_initFinishedHook(void)
{
#if (BLE_CONTROLLER_INITIAL_EN) /*bt controller initial*/
#if (!MCU_DUAL_CORE_ENABLE && (MCU_CORE_TYPE == CHIP_TYPE_TL751X) ||  (MCU_CORE_TYPE == CHIP_TYPE_TL322X))
	sys_n22_init(0x20100000);
#endif
    rf_module_init();

    /* initialize some basic MCU hardware */
    tlksdk_init_mcu_hardware();

    /*initialize BR/EDR core*/
    controller_init(BLE_only, HCI_TR_SOC, NULL, NULL);
    //////////////////////////// basic hardware Initialization  End /////////////////////////////////


    tlksdk_sch_init();
#endif


}

int INIT(APP_DEMO_SELECT)(void);
/**
 * @brief  Initialize BLE host LE application.
 *          Initialize BLE stack and selected demo application.
 */
void tlkapp_host_le_init(void)
{
    ble_stack_init();
    INIT(APP_DEMO_SELECT)();
}

int START(APP_DEMO_SELECT)(void);
/**
 * @brief  Start BLE host LE application.
 *          Start the selected demo application.
 */
void tlkapp_host_le_start(void)
{
    START(APP_DEMO_SELECT)();
}

/**
 * @brief  Main application entry point.
 *          Initialize system, start tasks and enter main loop (if RTOS is disabled).
 *
 * @return  0 on exit (should not return in normal operation).
 */
int main(void)
{
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);

#if (!TLK_CFG_RTOS_ENABLE)
    while (1) {
    #if (BLE_CONTROLLER_INITIAL_EN)
        tlksdk_main_loop();
    #endif
        tlksys_handler();
    }
#endif

    return 0;
}