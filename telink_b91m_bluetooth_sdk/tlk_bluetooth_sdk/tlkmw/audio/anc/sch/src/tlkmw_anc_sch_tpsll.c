/********************************************************************************************************
 * @file    tlkmw_anc_sch_tpsll.c
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
#if TLKALG_ANC_ENABLE

#include "tlkmw/tlkmw.h"
#include "../inc/tlkmw_anc_sch_inner.h"

typedef struct
{
    uint16_t handle;
} AncSchTpsll_t;

static AncSchTpsll_t sAncSchTpsll = {0};

/**
 * @brief       TPSLL state change callback
 * @param[in]   mode - Mode value
 * @return      None
 */
static void tlkmw_anc_sch_tpsll_state_change_cb(uint16_t mode)
{
#if (TLK_STK_TPH_ENABLE)
    if (mode & TPH_HOST_MODE_DONGLE_AUDIO) {
#elif (TLK_STK_TPT_ENABLE)
    if (mode & TPT_HOST_MODE_DONGLE_AUDIO) {
#endif
        if (sAncSchTpsll.handle != 0) {
            return;
        }
        sAncSchTpsll.handle = 0XFFF0;
        tlkmw_anc_sch_request_update(TLKMW_ANC_SCH_CB_TYPE_TPSLL, 0XFFF0, true);
    } else {
        if (sAncSchTpsll.handle == 0) {
            return;
        }
        sAncSchTpsll.handle = 0;
        tlkmw_anc_sch_request_update(TLKMW_ANC_SCH_CB_TYPE_TPSLL, 0XFFF0, false);
    }
}

/**
 * @brief       Initialize TPSLL scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_tpsll_init(void)
{
    tlkmdi_audio_tpsif_addStateChgCB(tlkmw_anc_sch_tpsll_state_change_cb);
}

/**
 * @brief       Get running handle of TPSLL
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmw_anc_tpsll_getRunningHandle(void)
{
    return sAncSchTpsll.handle;
}
#endif