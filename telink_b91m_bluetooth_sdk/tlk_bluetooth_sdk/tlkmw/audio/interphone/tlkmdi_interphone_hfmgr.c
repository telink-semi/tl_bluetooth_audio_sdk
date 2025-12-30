/********************************************************************************************************
 * @file    tlkmdi_interphone_hfmgr.c
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
    uint8_t                  isRunning;
    uint8_t                  codec;
    uint16_t                 handle;
    TlkmdiInterPhoneHfCtrlCB cb;
} Hfmgr_t;

static Hfmgr_t sTlkmdiInterPhoneHfmgr = {0};

/**
 * @brief       Default callback function for interphone HF manager
 * @param[in]   handle - Connection handle
 * @param[in]   isStart - Start flag
 * @param[in]   codec - Codec type
 * @return      None
 */
static void tlkmdi_interphone_hfmgr_default_cb(uint16_t handle, uint8_t isStart, uint8_t codec)
{
    (void)isStart;
    (void)handle;
    (void)codec;
}

/**
 * @brief       Voice state change callback function
 * @param[in]   acl_handle - ACL connection handle
 * @param[in]   sco_handle - SCO connection handle
 * @param[in]   is_start - Start flag
 * @param[in]   codec - Codec type
 * @return      None
 */
static void tlkmdi_interphone_voice_state_change_cb(uint16_t acl_handle, uint16_t sco_handle, uint8_t is_start, uint8_t codec)
{
    (void)sco_handle;
    if (is_start) {
        if (!tlkmw_audio_btif_handle_is_ag(acl_handle) || sTlkmdiInterPhoneHfmgr.handle != 0) {
            return; //if we are not ag(peer device earphone) or have connect sco ,reject
        }
        sTlkmdiInterPhoneHfmgr.handle = acl_handle;
        sTlkmdiInterPhoneHfmgr.codec  = codec;
        uint8_t isRunning             = sTlkmdiInterPhoneHfmgr.isRunning;
        tlkmdi_interphone_set_mode(INTERPHONE_MODE_AG);
        if (isRunning) {
            sTlkmdiInterPhoneHfmgr.cb(acl_handle, true, codec);
        }
    } else {
        if (sTlkmdiInterPhoneHfmgr.handle != acl_handle) {
            return;
        }
        tlkmdi_interphone_clear_mode(INTERPHONE_MODE_AG);
        if (sTlkmdiInterPhoneHfmgr.isRunning) {
            sTlkmdiInterPhoneHfmgr.cb(acl_handle, false, codec);
        }
        sTlkmdiInterPhoneHfmgr.handle = 0;
    }
}

/**
 * @brief       Initialize interphone HF manager
 * @param[in]   cb - Callback function
 * @return      None
 */
void tlkmdi_interphone_hfmgr_init(TlkmdiInterPhoneHfCtrlCB cb)
{
    if (cb == NULL) {
        cb = tlkmdi_interphone_hfmgr_default_cb;
    }
    sTlkmdiInterPhoneHfmgr.cb = cb;
    tlkmdi_audio_btif_addVoiceStateChgCB(tlkmdi_interphone_voice_state_change_cb);
}

/**
 * @brief       Pause interphone HF manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_hfmgr_pause(void)
{
    if (!sTlkmdiInterPhoneHfmgr.isRunning) {
        return;
    }
    sTlkmdiInterPhoneHfmgr.isRunning = 0;
    if (sTlkmdiInterPhoneHfmgr.handle) {
        sTlkmdiInterPhoneHfmgr.cb(sTlkmdiInterPhoneHfmgr.handle, false, sTlkmdiInterPhoneHfmgr.codec);
    }
}

/**
 * @brief       Resume interphone HF manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_hfmgr_resume(void)
{
    if (sTlkmdiInterPhoneHfmgr.isRunning) {
        return;
    }
    sTlkmdiInterPhoneHfmgr.isRunning = 1;
    if (sTlkmdiInterPhoneHfmgr.handle) {
        sTlkmdiInterPhoneHfmgr.cb(sTlkmdiInterPhoneHfmgr.handle, true, sTlkmdiInterPhoneHfmgr.codec);
    }
}

/**
 * @brief       Get connection handle
 * @param       None
 * @return      Connection handle
 */
uint16_t tlkmdi_interphone_hfmgr_get_handle(void)
{
    return sTlkmdiInterPhoneHfmgr.handle;
}
#endif //TLKMW_INTERPHONE_EN
