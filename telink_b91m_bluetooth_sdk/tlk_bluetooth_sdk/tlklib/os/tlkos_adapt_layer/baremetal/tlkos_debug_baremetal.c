/********************************************************************************************************
 * @file    tlkos_debug_baremetal.c
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
#include "tlkapi/tlkapi.h"
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_BAREMETAL_ENABLE && TLKOS_CFG_DEBUG_ENABLE

/**
 * @brief       Get core information for baremetal implementation
 * @return      NULL - baremetal implementation does not provide core information.
 */
const char *tlkos_debug_getCoreInfo(void)
{
    return NULL;
}

/**
 * @brief       Get CPU usage for baremetal implementation
 * @return      0XFFFFFFFF - CPU usage information is not available in baremetal implementation.
 */
uint32_t tlkos_debug_getCpuUsage(void)
{
    return 0XFFFFFFFF;
}
#endif //TLKOS_CFG_BAREMETAL_ENABLE && TLKOS_CFG_DEBUG_ENABLE