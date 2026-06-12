/********************************************************************************************************
 * @file    tlkapp_host_tpd.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "tlklib/usb/tlkusb.h"
#include "stack/tpsll/tpsll.h"
#if (TLK_STK_TPD_ENABLE)

/**
 * @brief       Callback function for TPSLL Dongle state changes.
 * @param[in]   state - The new state of the TPD.
 * @return      none.
 */
static void tlkapp_host_tpd_stateChgCB(uint8_t state)
{
    // tlkapi_printf(1, "tlkapp_host_tpd_stateChgCB: %d", state);
    if (state == TLKMDI_TPD_STATE_CHANGE_CB_CONNECT) {
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_TPSLL, 0xFFF0, TLKAPP_UI_STATE_CONNECTED);
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE,
            .optype    = TLKAUD_TYPE_TPD_AUDIO,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_TPD_AUDIO),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };

        uint32_t taskID = 0xFFF0 + ((uint32_t)TLKAUD_TYPE_TPD_AUDIO << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
    } else if (state == TLKMDI_TPD_STATE_CHANGE_CB_DISCONNECT) {
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_TPSLL, 0xFFF0, TLKAPP_UI_STATE_IDLE);

        uint32_t taskID = 0xFFF0 + ((uint32_t)TLKAUD_TYPE_TPD_AUDIO << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    } else if (state == TLKMDI_TPD_STATE_CHANGE_CB_PAIR) {
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_PARING);
    }
}

/**
 * @brief       Initializes the TPSLL Dongle module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_tpd_init(void)
{
    tlk_tpsll_tpd_controller_init();
    tlkmdi_tpsll_audio_dongle_init();
    tlkmdi_tpd_regStateChgCB(tlkapp_host_tpd_stateChgCB);
}

/**
 * @brief       Starts the TPSLL Dongle module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_tpd_start(void)
{
    tlkmdi_tpsll_audio_dongle_powerOnReconHeadset();
}

/**
 * @brief       Handles input messages for the TPSLL Dongle module.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
static int tlkapp_host_tpd_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;

    if (msgID == TLKSYS_TPD_MSGID_START_PAIR) {
        tlkmdi_tpsll_audio_dongle_pairing_init(NULL, 0);
        return TLK_ENONE;
    } else if (msgID == TLKSYS_TPD_MSGID_SWITCH_USB_MODE) {
        extern uint8_t tlkusb_get_curMode(uint8_t index);
        uint8_t        mode = tlkusb_get_curMode(TLK_CFG_USB_UDB_INDEX);

        tlkusb_close(TLK_CFG_USB_UDB_INDEX);
        delay_ms(30);

        if (mode == TLKUSB_MODTYPE_UDB) {
            tlkusb_setModule(TLK_CFG_USB_UDB_INDEX, TLKUSB_MODTYPE_UAC);
        } else if (mode == TLKUSB_MODTYPE_UAC) {
            tlkusb_setModule(TLK_CFG_USB_UDB_INDEX, TLKUSB_MODTYPE_UDB);
        }
        tlkapi_printf(1, "tpsll dongle usb mode switch to %d", !mode);
        return TLK_ENONE;
    }
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       Gets the TPSLL Dongle module.
 * @param[in]   none.
 * @return      Returns a pointer to the TPSLL Dongle module.
 */
TlkAppHostModule_t *tlkapp_host_tpd_getModule(void)
{
    static const TlkAppHostModuleCfg_t cfgs = {
        .hostType = TLKSYS_MSG_HOST_TYPE_TPD,
        .init     = tlkapp_host_tpd_init,
        .start    = tlkapp_host_tpd_start,
        .input    = tlkapp_host_tpd_input,
    };
    static TlkAppHostModule_t module = {
        .cfgs = &cfgs,
    };
    return &module;
}

#endif
