/********************************************************************************************************
 * @file    tlkmdi_debug.c
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
#include "tlklib/dbg/tlkdbg.h"
#if (TLK_DEBUG_ENABLE)

static TlkApiTimer_t sTlkmdiDebugTimer = {0};

/**
 * @brief       This function is the timer callback for the debug module.
 * @param[in]   pTimer    - the timer handle.
 * @param[in]   userArg   - user argument.
 * @return      none.
 */
static void tlkmdi_debug_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    tlkdbg_handler();
    if (tlkdbg_handler_getNextIntvUs() != TLKOS_WAIT_FOREVER) {
        return;
    }
    tlksys_timer_stop(TLKSYS_TASKID_SYSTEM, pTimer);
}

/**
 * @brief       This function initializes the debug module.
 * @param[in]   none.
 * @return      Returns TLK_ENONE on success, otherwise failed.
 */
int tlkmdi_debug_init(void)
{
    tlk_debug_init();
    tlkdbg_init();
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkmdiDebugTimer, 5 * 100, true, tlkmdi_debug_timer, NULL);
    return TLK_ENONE;
}

/**
 * @brief       This function handles the debug module processing.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_debug_handler(void)
{
    tlksys_timer_start(TLKSYS_TASKID_SYSTEM, &sTlkmdiDebugTimer);
}

#endif // #if (TLK_MW_DEBUG_ENABLE)
