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
#include "stack/tpsll/tpsll.h"
#include "tlkapi/tlkapi.h"
#include "tlkapp/tlkapp.h"
#include "app_config.h"
#include "app.h"


/**
 * @brief       Overwrite this function to set platform(mcu) config.
 * @param[in]   none
 * @return      Platform initialization configuration structure
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void)
{
    static const tlksys_hal_platform_init_cfg_t cfg = {
        .clockLevel = 4,
        #if DBG_WFI_ENABLE
        .gpioCfg = TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN,
        #endif

        #if (TLK_CFG_FLASH_PROT_ENABLE)
        .flashProtectEn = 1,
        #endif
    };
    return &cfg;
} 


/**
 * @brief      Hook function called when system initialization is finished
 * @param[in]  none
 * @return     none
 * @note       Initializes RF and application
 */
void tlksys_initFinishedHook(void)
{
    extern void tlk_rf_init(void);
    tlk_rf_init();
    tlkapp_init();
}


/**
 * @brief       This is main function
 * @param[in]	none
 * @return      Integer value indicating exit status (always 0 in this implementation)
 */
int main(void)
{
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);
    while (1) {
        tlkapp_main_loop();
    }

    return 0;
}