/********************************************************************************************************
 * @file    tlkapp_host_bt_hook.c
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
#if (TLK_STK_BT_ENABLE)
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"

/**
 * @brief     Provides a hook function when the BT host task init completed.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_host_bt_taskInitCompletedHook(void) {}

/**
 * @brief     Provides a hook function when the BT host task starts.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_host_bt_taskStartHook(void) {}

/**
 * @brief     Provides a hook function to select the handle that is used to trigger siri(assistant).
 * @param[in] None.
 * @returns   bt handle that is used to trigger siri(assistant).
 */
__attribute__((weak)) uint16_t tlkapp_host_bt_selectHandleToTriggerSiriHook(void)
{
#if (TLKBTP_CFG_HFPHF_ENABLE)
    return btp_hfphf_getCurHandle();
#else
    return 0;
#endif
}

#endif // #if (TLK_STK_BT_ENABLE)
