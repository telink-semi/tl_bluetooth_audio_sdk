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
#include "stack/controller.h"
#include "stack/bt/host/bth/bth_hcicmd.h"

#include "app_config.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/dbg/tlkdbg.h"
#include "tlklib/usb/tlkusb.h"
#include "tlkmw/pm/tlkmw_pm.h"

#if (CONTROLLER_MODE == BTBLE_AUDIO_SOURCE)
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/multiCoreComm/comm.h"
#include "stack/btble.h"

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

        core_interrupt_enable();
        /* initialize some basic MCU hardware */
        tlk_sys_init_mcu_hardware();

        tlk_sch_plan_set_base_interval(PLAN_INTERVAL_10MS);

        tlk_multi_core_communication_init();
        /*initialize BR/EDR core*/
        controller_init(BT_BLE, HCI_TR_SOC, NULL, NULL);
        tlk_sch_init();
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
