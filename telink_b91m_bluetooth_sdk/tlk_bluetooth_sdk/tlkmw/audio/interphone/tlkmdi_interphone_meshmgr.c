/********************************************************************************************************
 * @file    tlkmdi_interphone_meshmgr.c
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
#if TLKMW_INTERPHONE_EN
#include "tlkmw/tlkmw.h"

typedef struct
{
    uint8_t                    isRunning;
    uint8_t                    isMeshEn;
    TlkmdiInterPhoneMeshCtrlCB cb;
} Meshmgr_t;

static Meshmgr_t sTlkmdiInterPhoneMeshmgr = {0};

/**
 * @brief       Default callback function for mesh control
 * @param[in]   isStart - Start flag
 * @return      None
 */
static void tlkmdi_interphone_meshmgr_default_cb(uint8_t isStart)
{
    (void)isStart;
}

/**
 * @brief       Initialize interphone mesh manager
 * @param[in]   cb - Callback function
 * @return      None
 */
void tlkmdi_interphone_meshmgr_init(TlkmdiInterPhoneMeshCtrlCB cb)
{
    if (cb == NULL) {
        cb = tlkmdi_interphone_meshmgr_default_cb;
    }
    sTlkmdiInterPhoneMeshmgr.cb = cb;
}

/**
 * @brief       Pause interphone mesh manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_meshmgr_pause(void)
{
    if (!sTlkmdiInterPhoneMeshmgr.isRunning) {
        return;
    }
    sTlkmdiInterPhoneMeshmgr.isRunning = 0;
    if (sTlkmdiInterPhoneMeshmgr.isMeshEn) {
        sTlkmdiInterPhoneMeshmgr.cb(false);
    }
}

/**
 * @brief       Resume interphone mesh manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_meshmgr_resume(void)
{
    if (sTlkmdiInterPhoneMeshmgr.isRunning) {
        return;
    }
    sTlkmdiInterPhoneMeshmgr.isRunning = 1;
    if (sTlkmdiInterPhoneMeshmgr.isMeshEn) {
        sTlkmdiInterPhoneMeshmgr.cb(true);
    }
}

/**
 * @brief       Enable interphone mesh
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_api_mesh_enable(void)
{
    if (sTlkmdiInterPhoneMeshmgr.isMeshEn) {
        return;
    }
    sTlkmdiInterPhoneMeshmgr.isMeshEn = 1;
    uint8_t isRunning                 = sTlkmdiInterPhoneMeshmgr.isRunning;
    tlkmdi_interphone_set_mode(INTERPHONE_MODE_MESH);
    if (isRunning) {
        sTlkmdiInterPhoneMeshmgr.cb(true);
    }
}

/**
 * @brief       Disable interphone mesh
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_api_mesh_disable(void)
{
    if (!sTlkmdiInterPhoneMeshmgr.isMeshEn) {
        return;
    }
    tlkmdi_interphone_clear_mode(INTERPHONE_MODE_MESH);
    if (sTlkmdiInterPhoneMeshmgr.isRunning) {
        sTlkmdiInterPhoneMeshmgr.cb(false);
    }
    sTlkmdiInterPhoneMeshmgr.isMeshEn = 0;
}

/**
 * @brief       Check if interphone mesh is enabled
 * @param       None
 * @return      1 if mesh is enabled, 0 otherwise
 */
uint8_t tlkmdi_interphone_api_is_mesh_enable(void)
{
    return sTlkmdiInterPhoneMeshmgr.isMeshEn;
}


#endif //TLKMW_INTERPHONE_EN
