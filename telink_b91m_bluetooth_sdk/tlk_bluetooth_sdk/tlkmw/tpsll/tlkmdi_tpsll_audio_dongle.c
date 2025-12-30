/********************************************************************************************************
 * @file    tlkmdi_tpsll_audio_dongle.c
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

#if (TLK_STK_TPD_ENABLE)

#include "stack/bt/common/co_bt_defines.h"
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/tpsll.h"

/////////////////////////////  debug only ////////////////////////////////
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/host/tpsll_hostevent.h"
#include "stack/tpsll/tpd/tpd_host_interface.h"
/////////////////////////////  debug only ////////////////////////////////


//TODO by liuqi

#define DUMP_APP_MSG                       1


#define TLKMDI_TPSLL_AUDIO_DONGLE_DBG_FLAG ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET << 16) | TLK_DEBUG_DBG_FLAG_ALL)

static tlkmdi_tpsll_audio_dongle_item_t sTlkMdiDongleCtrl;

static void tlkmdi_tpsll_audio_dongle_headset_connected_handler(uint8_t *mac_address);
static void tlkmdi_tpsll_audio_dongle_headset_disconnected_handler(uint8_t disconnect_reason);
static void tlkmdi_tpd_audio_ctrl_cb(uint8_t *data);

static tlkmdi_tpd_state_change_cb sTlkMdiTpdStateChgCB = NULL;
static tlkmdi_tpd_hidCmdCB        sTlkMdiTpdHidCmdCB   = NULL;

/**
 * @brief       Initialize the tpsll audio dongle control structure and registers necessary handlers.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpsll_audio_dongle_init(void)
{
    tmemset(&sTlkMdiDongleCtrl, 0, sizeof(tlkmdi_tpsll_audio_dongle_item_t));

    tlkmdi_tinySql_getTpdAddr(sTlkMdiDongleCtrl.local_addr);

    tpd_headset_connected_handler_cb_register(tlkmdi_tpsll_audio_dongle_headset_connected_handler);
    tpd_headset_disconnected_handler_cb_register(tlkmdi_tpsll_audio_dongle_headset_disconnected_handler);

    tpd_host_hal_set_local_mac(sTlkMdiDongleCtrl.local_addr);
    tpd_cmd_ui_handler_cb_register(tlkmdi_tpd_audio_ctrl_cb);
}

/**
 * @brief       Power onreconnection of the headset by checking the MAC address and initiating a connection scan.
 * @return      TLK_ENONE - success.
 * @note        none.
 */
int tlkmdi_tpsll_audio_dongle_powerOnReconHeadset(void)
{
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    tlkmdi_tinySql_getBtMacAddress(sTlkMdiDongleCtrl.local_addr);

    if ((tmemcmp(sTlkMdiDongleCtrl.local_addr, null_addr0, BD_ADDR_LEN) == 0) || (tmemcmp(sTlkMdiDongleCtrl.local_addr, null_addr1, BD_ADDR_LEN) == 0)) {
        tlkapi_sendData(DUMP_APP_MSG, "!! Invalid Mac Address can not enter the pairing mode", &sTlkMdiDongleCtrl.local_addr[0], 6);
#if BT_ERROR_CODE
        TLKSTK_ERROR_DEBUG(ACCESS_DEBUG_EN, 0x50010000);
#endif
        return -TLK_ENONE;
    }

    tlkmdi_tinySql_getTphMacAddr(sTlkMdiDongleCtrl.addr_paired_headset);

    tpd_dongle_set_headset_mac(sTlkMdiDongleCtrl.addr_paired_headset);
    tpd_get_ac_chn_from_mac(sTlkMdiDongleCtrl.addr_paired_headset, (uint8_t *)&sTlkMdiDongleCtrl.tpsll_ac, (uint8_t *)&sTlkMdiDongleCtrl.tpsll_ch);

    tlkapi_sendU32s(APP_LOG_EN, "app_user_data_update", sTlkMdiDongleCtrl.tpsll_ac, sTlkMdiDongleCtrl.tpsll_ch, 0, 0);

    if (sTlkMdiDongleCtrl.tpsll_ac == 0 || sTlkMdiDongleCtrl.tpsll_ac == 0xffffffff || sTlkMdiDongleCtrl.tpsll_ch == 0 || sTlkMdiDongleCtrl.tpsll_ch == 0xff) {
        tpd_dongle_set_setup_ac_chn(TPD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPD_HOST_DONGLE_SETUP_COMMON_CHN);
        tpd_host_dongle_start_connection_scan();
    } else {
        tpd_dongle_set_setup_ac_chn(sTlkMdiDongleCtrl.tpsll_ac, sTlkMdiDongleCtrl.tpsll_ch);
        tpd_host_dongle_start_connection_scan();
    }
    return TLK_ENONE;
}

/**
 * @brief       Register the state change callback function.
 * @param[in]   cb      - The callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpd_regStateChgCB(tlkmdi_tpd_state_change_cb cb)
{
    sTlkMdiTpdStateChgCB = cb;
}

/**
 * @brief       Register the hid command callback function.
 * @param[in]   cb      - The callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpd_regGetHidCmdCB(tlkmdi_tpd_hidCmdCB cb)
{
    sTlkMdiTpdHidCmdCB = cb;
}

/**
 * @brief       Delete all user data by resetting dongle control parameters.
 * @return      none.
 * @note        none.
 */
void tlkmdi_delete_all_user_data(void)
{
    sTlkMdiDongleCtrl.tpsll_ac = 0;
    sTlkMdiDongleCtrl.tpsll_ch = 0;
    tmemset(sTlkMdiDongleCtrl.addr_paired_headset, 0x00, BD_ADDR_LEN);
    tpd_dongle_set_headset_mac(sTlkMdiDongleCtrl.addr_paired_headset);

    tlkmdi_tinySql_tpdHeadsetPairingReset();
}

/**
 * @brief       Initialize the pairing process for the tpsll audio dongle.
 * @param[in]   pData   - Pointer to data (not used in this function).
 * @param[in]   dataLen - Length of data (not used in this function).
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpsll_audio_dongle_pairing_init(uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    tlkmdi_tinySql_getBtMacAddress(sTlkMdiDongleCtrl.local_addr);

    if ((tmemcmp(sTlkMdiDongleCtrl.local_addr, null_addr0, BD_ADDR_LEN) == 0) || (tmemcmp(sTlkMdiDongleCtrl.local_addr, null_addr1, BD_ADDR_LEN) == 0)) {
        tlkapi_sendData(DUMP_APP_MSG, "!! Invalid Mac Address can not enter the pairing mode", &sTlkMdiDongleCtrl.local_addr[0], 6);
#if BT_ERROR_CODE
        TLKSTK_ERROR_DEBUG(ACCESS_DEBUG_EN, 0x50010000);
#endif
    }

    uint8_t result = 0;
    result         = tpd_dongle_start_disconnection(TPD_DISCONNECT_REASON_DONGLE_SETUP);
    tlkapi_sendU32s(DUMP_APP_MSG, "[[]]app_dongle_pairing_handle discon result", result, 0, 0, 0);
    if (result) {
        tlkmdi_delete_all_user_data();

        tpd_dongle_set_setup_ac_chn(TPD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPD_HOST_DONGLE_SETUP_COMMON_CHN);
        tpd_host_dongle_start_connection_scan();
    }
}

/**
 * @brief       This function handles the connection of the headset.
 * @param[in]   mac_address - the MAC address of the connected headset.
 * @return      none.
 * @note        none.
 */
static void tlkmdi_tpsll_audio_dongle_headset_connected_handler(uint8_t *mac_address)
{
    if (sTlkMdiTpdStateChgCB != NULL) {
        sTlkMdiTpdStateChgCB(TLKMDI_TPD_STATE_CHANGE_CB_CONNECT);
    }

    u32 ble_ac = 0;
    u8  ble_ch = 0;

    tpd_headset_get_setup_ac_chn(&ble_ac, &ble_ch);
    tmemcpy(sTlkMdiDongleCtrl.addr_paired_headset, mac_address, 6);
    tlkmdi_tinySql_setTphMacAddr(sTlkMdiDongleCtrl.addr_paired_headset);

    if (ble_ac != sTlkMdiDongleCtrl.tpsll_ac || ble_ch != sTlkMdiDongleCtrl.tpsll_ch) {
        sTlkMdiDongleCtrl.tpsll_ac = ble_ac;
        sTlkMdiDongleCtrl.tpsll_ch = ble_ch;
    }
}

/**
 * @brief       This function handles the disconnection of the headset.
 * @param[in]   disconnect_reason - the reason for the disconnection.
 * @return      none.
 * @note        Calls a callback function if set and handles specific disconnection reasons.
 */
static void tlkmdi_tpsll_audio_dongle_headset_disconnected_handler(uint8_t disconnect_reason)
{
    (void)disconnect_reason;

    if (sTlkMdiTpdStateChgCB != NULL) {
        sTlkMdiTpdStateChgCB(TLKMDI_TPD_STATE_CHANGE_CB_DISCONNECT);
    }
    if (TPD_DISCONNECT_REASON_DONGLE_SETUP == disconnect_reason) {
        tlkmdi_delete_all_user_data();

        tpd_dongle_set_setup_ac_chn(TPT_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPD_HOST_DONGLE_SETUP_COMMON_CHN);
        tpd_host_dongle_start_connection_scan();

        if (sTlkMdiTpdStateChgCB != NULL) {
            sTlkMdiTpdStateChgCB(TLKMDI_TPD_STATE_CHANGE_CB_PAIR);
        }
    } else {
        tpd_dongle_set_setup_ac_chn(sTlkMdiDongleCtrl.tpsll_ac, sTlkMdiDongleCtrl.tpsll_ch);
        tpd_host_dongle_start_connection_scan();
    }
}

/**
 * @brief       This function processes audio control commands received.
 * @return      none.
 * @note        Checks for a valid callback and specific command type before executing corresponding actions.
 */
static void tlkmdi_tpd_audio_ctrl_cb(uint8_t *data)
{
    if (sTlkMdiTpdHidCmdCB == NULL) {
        return;
    }
    if (data[0] != TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY) {
        return;
    }
    uint8_t hidKey = data[1];
    switch (hidKey) {
    case TLK_MDI_HID_AUDIO_VOL_UP:
        sTlkMdiTpdHidCmdCB(0xfff0, TLKMDI_TPD_HID_CMD_VOL_UP);
        break;
    case TLK_MDI_HID_AUDIO_VOL_DOWN:
        sTlkMdiTpdHidCmdCB(0xfff0, TLKMDI_TPD_HID_CMD_VOL_DOWN);
        break;
    case TLK_MDI_HID_AUDIO_PLAY_PAUSE:
        sTlkMdiTpdHidCmdCB(0xfff0, TLKMDI_TPD_HID_CMD_PLAY_PAUSE);
        break;
    case TLK_MDI_HID_AUDIO_BACKWARD:
        sTlkMdiTpdHidCmdCB(0xfff0, TLKMDI_TPD_HID_CMD_PREV);
        break;
    case TLK_MDI_HID_AUDIO_FORWARD:
        sTlkMdiTpdHidCmdCB(0xfff0, TLKMDI_TPD_HID_CMD_NEXT);
        break;
    default:
        break;
    }
}

#endif // #if (TLK_STK_TPD_ENABLE)
