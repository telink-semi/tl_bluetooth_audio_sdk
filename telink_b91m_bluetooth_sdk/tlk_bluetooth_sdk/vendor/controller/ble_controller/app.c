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

#include "stack/btble.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkapp/tlkapp.h"
#include "tlkmw/pm/tlkmw_pm.h"
#include "app_config.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/multiCoreComm/comm.h"
#include "stack/btble.h"
#include "stack/controller.h"

#if (CHECK_BLE_CONTROLLER)


/**
 * @brief       Initialize user application and hardware modules
 * @param[in]   none
 * @return      none
 */
void user_init(void)
{
#if (CHIP_TYPE != CHIP_TYPE_TL752X)
    /* initialize some basic MCU hardware */
    if (!pm_sleep_wakeup_check()) // power up normal
#endif
    {
        trng_init();

        rf_module_init();

        //rf_enable_bb_debug(); // RF debug used only

        core_interrupt_enable();

        tlk_sys_init_mcu_hardware();

        tlk_multi_core_communication_init();

        controller_init(BLE_only, HCI_TR_SOC, NULL, NULL);

        tlk_sch_init();

        // rf_set_ble_bb_debugport();
#if (CHIP_TYPE != CHIP_TYPE_TL752X)
        rf_set_power_level_index(RF_POWER_P3dBm);

        tlkmdi_pm_init();
#endif
    }
#if (CHIP_TYPE != CHIP_TYPE_TL752X)
    else /* power up by D25F suspend wake up logic */
    {
        tlk_sys_restore_mcu_hardware();
    }
#endif
}

/**
 * @brief       Main loop function for user application
 * @param[in]   none
 * @return      none
 */
void main_loop(void)
{
    tlk_multi_core_communication_loop();

    tlk_sys_main_loop();

    tlkmdi_pm_process();
}


#endif
