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
#include "tlkapp/tlkapp.h"
#include "app_ui.h"

/**
 * @brief       Get platform initialization configuration
 * @param[in]   none
 * @return      Pointer to platform initialization configuration structure
 * @note        This function provides platform-specific configuration including
 *              GPIO settings and clock levels
 */
const tlksys_hal_platform_init_cfg_t * tlksys_hal_port_getPlatformInitCfg(void)
{

    static const tlksys_hal_platform_init_cfg_t cfg = {
        .gpioCfg = TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN,
        .clockLevel = TLK_CFG_AUDIO_CLOCK_LEVEL,

        #if (TLK_CFG_FLASH_PROT_ENABLE)
        .flashProtectEn = 1,
        #endif
    };
    return &cfg;
}

/**
 * @brief       Main entry point of the application
 * @param       None
 * @return      0 on successful execution
 * @note        Initializes system, starts tasks, and enters main loop.
 *              Handles system events in non-RTOS configuration.
 */
int main(void)
{
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);
    //*note: if use rtos,code will end in tlksys_start*/

	#if (APP_UI_ENABLE)
	app_ui_init();
	#endif

    while(1){
        #if !TLK_CFG_RTOS_ENABLE
        tlksys_handler();
        #if (!MCU_DUAL_CORE_ENABLE)
            void tlksdk_main_loop(void);
            tlksdk_main_loop();
        #endif
        #endif
    }
    return 0;
}