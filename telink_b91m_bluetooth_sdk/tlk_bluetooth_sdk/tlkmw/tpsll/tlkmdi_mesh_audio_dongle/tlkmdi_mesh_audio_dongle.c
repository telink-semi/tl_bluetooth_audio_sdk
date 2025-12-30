/********************************************************************************************************
 * @file    tlkmdi_mesh_audio_dongle.c
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

#if (TLK_STK_TPMD_ENABLE)

#include "stack/bt/common/co_bt_defines.h"
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hci.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/host/tpsll_hostevent.h"
#include "stack/tpsll/tpmd/tpmd_host_interface.h"
#include "stack/multiCoreComm/service/service_mailbox.h"

#define TPMD_DONGLE_SETUP_COMMON_ACCESSCODE 0x56291435
#define TPMD_DONGLE_SETUP_COMMON_CHN        0x12
#define TPMD_CONNECTED_AFH_COMMON_CHN       0x20
#define DUMP_APP_MSG                        1


#define TLKMDI_MESH_AUDIO_DONGLE_DBG_FLAG   ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET << 16) | TLK_DEBUG_DBG_FLAG_ALL)

static tlkmdi_mesh_audio_dongle_item_t sTlkMdiMeshAudioDongleCtrl;

static int tlkmdi_mesh_audio_dongle_connected_handler(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_mesh_audio_dongle_disconnected_handler(uint8_t *pData, uint16_t dataLen);

static tlkmdi_tpmd_state_change_cb sTlkMdiTpmdStateChgCB = NULL;
static tlkmdi_tpmd_hidCmdCB        sTlkMdiTpmdHidCmdCB   = NULL;

TPSLL_EVT_REGISTER(TPSLL_EVTID_HEADSET_CONNECT, tlkmdi_mesh_audio_dongle_connected_handler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_HEADSET_DISCONNECT, tlkmdi_mesh_audio_dongle_disconnected_handler);

/**
 * @brief       Initialize the mesh audio dongle.
 * @return      none.
 * @note        none.
 */
void tlkmdi_mesh_audio_dongle_init(void)
{
    tlkapi_printf(1, "tlkmdi_mesh_audio_dongle_init");

    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_recvC2HData);
    // tlk_d25f_register_sync_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_sco_recvC2HData);

    tmemset(&sTlkMdiMeshAudioDongleCtrl, 0, sizeof(tlkmdi_mesh_audio_dongle_item_t));
    tmemset(sTlkMdiMeshAudioDongleCtrl.null_addr0, 0x00, BD_ADDR_LEN);
    tmemset(sTlkMdiMeshAudioDongleCtrl.null_addr1, 0xff, BD_ADDR_LEN);

    tlkmdi_tinySql_getTpdAddr(sTlkMdiMeshAudioDongleCtrl.local_addr);

    tpmd_host_hal_set_local_mac(sTlkMdiMeshAudioDongleCtrl.local_addr);
}

/**
 * @brief      Power on reconnection for the paired headset.
 * @return     TLK_ENONE - success.
 * @note       none.
 */
int tlkmdi_mesh_audio_dongle_powerOnReconHeadset(void)
{
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    tlkmdi_tinySql_getBtMacAddress(sTlkMdiMeshAudioDongleCtrl.local_addr);

    if ((tmemcmp(sTlkMdiMeshAudioDongleCtrl.local_addr, &sTlkMdiMeshAudioDongleCtrl.null_addr0, BD_ADDR_LEN) == 0) ||
        (tmemcmp(sTlkMdiMeshAudioDongleCtrl.local_addr, sTlkMdiMeshAudioDongleCtrl.null_addr1, BD_ADDR_LEN) == 0)) {
        tlkapi_sendData(DUMP_APP_MSG, "!! Invalid Mac Address can not enter the pairing mode", &sTlkMdiMeshAudioDongleCtrl.local_addr[0], 6);
#if BT_ERROR_CODE
        TLKSTK_ERROR_DEBUG(ACCESS_DEBUG_EN, 0x50010000);
#endif
        return -TLK_ENONE;
    }

    tlkmdi_tinySql_getTphMacAddr(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset);

    tpsll_hci_sendSetGeneralMacCmd(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset);
    tph_host_hal_get_ac_chn_from_mac(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset, (uint8_t *)&sTlkMdiMeshAudioDongleCtrl.tpsll_ac,
                                     (uint8_t *)&sTlkMdiMeshAudioDongleCtrl.tpsll_ch);

    tlkapi_sendU32s(APP_LOG_EN, "app_user_data_update", sTlkMdiMeshAudioDongleCtrl.tpsll_ac, sTlkMdiMeshAudioDongleCtrl.tpsll_ch, 0, 0);

    if (sTlkMdiMeshAudioDongleCtrl.tpsll_ac == 0 || sTlkMdiMeshAudioDongleCtrl.tpsll_ac == 0xffffffff || sTlkMdiMeshAudioDongleCtrl.tpsll_ch == 0 ||
        sTlkMdiMeshAudioDongleCtrl.tpsll_ch == 0xff) {
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(TPMD_DONGLE_SETUP_COMMON_ACCESSCODE, TPMD_DONGLE_SETUP_COMMON_CHN);
    } else {
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(sTlkMdiMeshAudioDongleCtrl.tpsll_ac, sTlkMdiMeshAudioDongleCtrl.tpsll_ch); //sTlkMdiMeshAudioDongleCtrl.tpsll_ch);
    }
    tpsll_hci_sendCreateConnectCmd(TPMD_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
    return TLK_ENONE;
}

/**
 * @brief       Register the state change callback function.
 * @param[in]   cb    - the callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpmd_regStateChgCB(tlkmdi_tpmd_state_change_cb cb)
{
    sTlkMdiTpmdStateChgCB = cb;
}

/**
 * @brief       Register the hid command callback function.
 * @param[in]   cb    - the callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpmd_regGetHidCmdCB(tlkmdi_tpmd_hidCmdCB cb)
{
    sTlkMdiTpmdHidCmdCB = cb;
}

/**
 * @brief       Delete all user data.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpmd_delete_all_user_data(void)
{
    sTlkMdiMeshAudioDongleCtrl.tpsll_ac = 0;
    sTlkMdiMeshAudioDongleCtrl.tpsll_ch = 0;
    tmemset(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset, 0x00, BD_ADDR_LEN);

    tpsll_hci_sendSetGeneralMacCmd(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset);

    tlkmdi_tinySql_tpdHeadsetPairingReset();
}

/**
 * @brief       Initialize the pairing process for the mesh audio dongle.
 * @param[in]   pData   - pointer to the data buffer.
 * @param[in]   dataLen - length of the data buffer.
 * @return      none.
 * @note        This function checks for a valid MAC address and sends a message if the MAC is invalid.
 */
void tlkmdi_mesh_audio_dongle_pairing_init(uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;

    tlkmdi_tinySql_getBtMacAddress(sTlkMdiMeshAudioDongleCtrl.local_addr);

    if ((tmemcmp(sTlkMdiMeshAudioDongleCtrl.local_addr, &sTlkMdiMeshAudioDongleCtrl.null_addr0, BD_ADDR_LEN) == 0) ||
        (tmemcmp(sTlkMdiMeshAudioDongleCtrl.local_addr, sTlkMdiMeshAudioDongleCtrl.null_addr1, BD_ADDR_LEN) == 0)) {
        tlkapi_sendData(DUMP_APP_MSG, "!! Invalid Mac Address can not enter the pairing mode", &sTlkMdiMeshAudioDongleCtrl.local_addr[0], 6);
#if BT_ERROR_CODE
        TLKSTK_ERROR_DEBUG(ACCESS_DEBUG_EN, 0x50010000);
#endif
    }

    uint8_t result = 0;
    // result = tpmd_dongle_start_disconnection(TPMD_DISCONNECT_REASON_DONGLE_SETUP);
    tlkapi_sendU32s(DUMP_APP_MSG, "[[]]app_dongle_pairing_handle discon result", result, 0, 0, 0);
    //    if(result)
    //    {
    //        tlkmdi_tpmd_delete_all_user_data();
    //
    //    	// tpmd_dongle_set_setup_ac_chn(TPMD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPMD_HOST_DONGLE_SETUP_COMMON_CHN);
    //    	// tpmd_host_dongle_start_connection_scan();
    //    }
}

/**
 * @brief       This function handles the connected event.
 * @param[in]   pData   - pointer to the data buffer containing connection information.
 * @param[in]   dataLen - length of the data buffer.
 * @return      TLK_ENONE - success.
 * @note        Update the paired headset MAC address and connection parameters.
 */
static int tlkmdi_mesh_audio_dongle_connected_handler(uint8_t *pData, uint16_t dataLen) //(uint8_t *mac_address)
{
    (void)pData;
    (void)dataLen;

    //    if (sTlkMdiTpmdStateChgCB != NULL) {
    //        sTlkMdiTpmdStateChgCB(TLKMDI_TPMD_STATE_CHANGE_CB_CONNECT);
    //    }

    u32 ble_ac = 0;
    u8  ble_ch = 0;

    tmemcpy(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset, pData, 6);
    tmemcpy(&ble_ch, pData + 6, 1);
    tmemcpy(&ble_ac, pData + 8, 4);

    tlkmdi_tinySql_setTphMacAddr(sTlkMdiMeshAudioDongleCtrl.addr_paired_headset);

    if (ble_ac != sTlkMdiMeshAudioDongleCtrl.tpsll_ac || ble_ch != sTlkMdiMeshAudioDongleCtrl.tpsll_ch) {
        sTlkMdiMeshAudioDongleCtrl.tpsll_ac = ble_ac;
        sTlkMdiMeshAudioDongleCtrl.tpsll_ch = ble_ch;
    }

    return TLK_ENONE;
}

/**
 * @brief       This function handles the disconnected event.
 * @param[in]   pData   - pointer to the data buffer containing the disconnect reason.
 * @param[in]   dataLen - length of the data buffer.
 * @return      TLK_ENONE - success.
 * @note        Delete all user data if the disconnect reason is setup and send commands to write access code and channel ID.
 */
static int tlkmdi_mesh_audio_dongle_disconnected_handler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t disconnect_reason;

    disconnect_reason = pData[0];
    if (TPMD_DISCONNECT_REASON_DONGLE_SETUP == disconnect_reason) {
        tlkmdi_tpmd_delete_all_user_data();
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(TPMD_DONGLE_SETUP_COMMON_ACCESSCODE, TPMD_DONGLE_SETUP_COMMON_CHN);
        tpsll_hci_sendCreateConnectCmd(TPMD_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
    } else {
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(sTlkMdiMeshAudioDongleCtrl.tpsll_ac, sTlkMdiMeshAudioDongleCtrl.tpsll_ch);
        tpsll_hci_sendCreateConnectCmd(TPMD_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
    }

    return TLK_ENONE;
}

#if 0
/**
 * @brief       This function processes audio control commands received.
 * @return      none.
 * @note        Checks for a valid callback and specific command type before executing corresponding actions.
 */
static void tlkmdi_tpmd_audio_ctrl_cb(uint8_t *data)
{
   if(sTlkMdiTpmdHidCmdCB == NULL){
       return;
   }
   if(data[0] != TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY){
       return;
   }
   uint8_t hidKey = data[1];
   switch(hidKey){
       case TLK_MDI_HID_AUDIO_VOL_UP:
           sTlkMdiTpmdHidCmdCB(0xfff0,TLKMDI_TPMD_HID_CMD_VOL_UP);
           break;
       case TLK_MDI_HID_AUDIO_VOL_DOWN:
           sTlkMdiTpmdHidCmdCB(0xfff0,TLKMDI_TPMD_HID_CMD_VOL_DOWN);
           break;
       case TLK_MDI_HID_AUDIO_PLAY_PAUSE:
           sTlkMdiTpmdHidCmdCB(0xfff0,TLKMDI_TPMD_HID_CMD_PLAY_PAUSE);
           break;
       case TLK_MDI_HID_AUDIO_BACKWARD:
           sTlkMdiTpmdHidCmdCB(0xfff0,TLKMDI_TPMD_HID_CMD_PREV);
           break;
       case TLK_MDI_HID_AUDIO_FORWARD:
           sTlkMdiTpmdHidCmdCB(0xfff0,TLKMDI_TPMD_HID_CMD_NEXT);
           break;
       default:
           break;
   }
}
#endif
#endif // #if (TLK_STK_TPMD_ENABLE)
