/********************************************************************************************************
 * @file    tlkmdi_bt_tph.c
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

#if (TLK_STK_BT_TPSLL_ENABLE)

#include "stack/bt/common/co_bt_defines.h"
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/tpsll.h"
#include "tlkmdi_bt_tph_inner.h"
#include "stack/tpsll/host/tpsll_hci.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/host/tpsll_hostevent.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/multiCoreComm/service/service_mailbox.h"

#define TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTACL_DBG_SIGN            "[MBT_TPSLL]"

tlkmdi_bt_tph_ctrl_t gTlkMdiHeadsetCtrl;

static int  tlkmdi_bt_tph_dongle_macUpdateHandler(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_bt_tph_dongle_connectHandler(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_bt_tph_dongle_disconnHandler(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_bt_tph_headset_recvAclDataHandler(uint8_t *pData, uint16_t dataLen);
static void tlkmdi_bt_tph_reconBt(uint8_t retryNum);


tlkmdi_tph_state_change_cb gInnerTlkMdiBtTphStateChgCB = NULL;

TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_MAC_UPDATE, tlkmdi_bt_tph_dongle_macUpdateHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_CONNECT, tlkmdi_bt_tph_dongle_connectHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_DISCONNECT, tlkmdi_bt_tph_dongle_disconnHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_HEADSET_ACL_MSG_DEAL, tlkmdi_bt_tph_headset_recvAclDataHandler);

/**
 * @brief       Initializes the Bluetooth TPH module.
 * @return      none.
 * @note        This function sets up the necessary callbacks, initializes control structures,
 *              and get the local address and AC/CH settings from the Tiny SQL database.
 */
void tlkmdi_bt_tph_init(void)
{
    tlksys_pm_regChn(TLK_PM_BUSY_CHN_USB);
    tlksys_pm_setChn(TLK_PM_BUSY_CHN_USB, 0, 1);

    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_recvC2HData);
    tlk_d25f_register_sync_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_sco_recvC2HData);
    tmemset(&gTlkMdiHeadsetCtrl, 0, sizeof(tlkmdi_bt_tph_ctrl_t));

    tlkmdi_tinySql_getTpsAddr(gTlkMdiHeadsetCtrl.local_addr);
    tlkmdi_bt_tpsll_key_init();
    tlkmdi_bt_tph_pair_init();
    tpsll_hci_startTpsllTaskCmd(0, NULL);
    tph_host_hal_set_local_mac(gTlkMdiHeadsetCtrl.local_addr);
    tph_host_hal_get_ac_chn_from_mac(gTlkMdiHeadsetCtrl.local_addr, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ac, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ch);

    gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_RECONNECTING;
}

/**
 * @brief       Starts the pairing process for the Bluetooth TPH module.
 * @param[in]   isRefactory - flag indicating if the pairing mode is refactory(10s) or not(3s).
 * @return      none.
 * @note        This function sets up the pairing process, cleans previous pairing data if refactory,
 *              and initiates a disconnection before starting the pairing timer.
 */
void tlkmdi_bt_tph_pair_start(uint8_t isRefactory)
{
    uint8_t reason = isRefactory;

    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_pair_start: factory_flag %d cur_status %d dongleIsConn %d", isRefactory,
                 gTlkMdiHeadsetCtrl.cur_status, gTlkMdiHeadsetCtrl.dongleIsConn);

    gTlkMdiHeadsetCtrl.startPairing = true;
    if (reason == TPH_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP) {
        tmemset(gTlkMdiHeadsetCtrl.addr_paired_dongle, 0, 6);
        gTlkMdiHeadsetCtrl.tpsll_ac = 0;
        gTlkMdiHeadsetCtrl.tpsll_ch = 0;
        tlkmdi_tinySql_cleanPairingDevices();
    }
    gTlkMdiHeadsetCtrl.disReason = reason;
    if (gTlkMdiHeadsetCtrl.disReason == TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY) {
        if (gTlkMdiHeadsetCtrl.dongleIsConn == false) {
            tlk_printf("tlkmdi_bt_tph_pair_start: lowLatency, dongle isn't exist!!!");
            return;
        }
    }
    tph_host_hal_start_disconn(gTlkMdiHeadsetCtrl.disReason);
    // if 3s paring, start timer to wait for 3s dongle disconnect.
    gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_ASYNC_DISCON_WAITING;
    gTlkMdiHeadsetCtrl.timeout    = 3000000 / TLK_MDI_TPSLL_TIMEOUT;
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &gTlkMdiHeadsetCtrl.timer);
}

/**
 * @brief       This function starts the reconnection process for a paired dongle.
 * @return      none.
 * @note        The function checks if the paired dongle address is null or invalid before attempting to reconnect.
 */
void tlkmdi_bt_tph_dongle_reconnStart(void)
{
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);
    if (tmemcmp(gTlkMdiHeadsetCtrl.addr_paired_dongle, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(gTlkMdiHeadsetCtrl.addr_paired_dongle, null_addr1, BD_ADDR_LEN) == 0) {
        return;
    }
    tph_host_hal_get_ac_chn_from_mac(gTlkMdiHeadsetCtrl.local_addr, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ac, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ch);
    tph_host_hal_set_setup_ac_chn(gTlkMdiHeadsetCtrl.tpsll_ac, gTlkMdiHeadsetCtrl.tpsll_ch);
    tph_host_hal_start_connection_setup(TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
}

/**
 * @brief       This function handles BT TPH related tasks.
 * @return      none.
 * @note        Currently, it only calls the pairing handler function.
 */
void tlkmdi_bt_tph_handler(void)
{
    tlkmdi_bt_tph_pairing_handler();
}

tlkmdi_bt_tph_appUsbSuspendCallback sTlkmdiAppUsbsuspendCb = NULL;

void tlkmdi_bt_tph_appUsbSuspend_register(tlkmdi_bt_tph_appUsbSuspendCallback cb)
{
    sTlkmdiAppUsbsuspendCb = cb;
}

/**
 * @brief       This function handles the change of the usb suspend state at headset.
 * @param[in]   pData - usb suspend state.
 * @return      none.
 * @note        none.
 */
void tlkmdi_bt_tph_usb_suspend_handler(void *pData, uint8_t dataLen)
{
    if (sTlkmdiAppUsbsuspendCb != NULL) {
        sTlkmdiAppUsbsuspendCb(pData, dataLen);
    }
}

/**
 * @brief       This function handles the update of the dongle MAC address.
 *              When dongle is connected, this event will be triggered.
 * @param[in]   pData   - pointer to the data containing the new MAC address.
 * @param[in]   dataLen - length of the data.
 * @return      TLK_ENONE on success, -TLK_EPARAM on failure.
 * @note        The function checks if the new MAC address is valid and updates the paired dongle address if valid.
 */
static int tlkmdi_bt_tph_dongle_macUpdateHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t dongleAddr[6];

    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);
    tmemcpy(dongleAddr, pData, 6);
    tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_dongle_macUpdateHandler: ", dongleAddr, 6);

    gTlkMdiHeadsetCtrl.startPairing = false;

    if (tmemcmp(gTlkMdiHeadsetCtrl.addr_paired_dongle, dongleAddr, BD_ADDR_LEN) == 0) {
        return -TLK_EPARAM;
    }
    if (tmemcmp(dongleAddr, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(dongleAddr, null_addr1, BD_ADDR_LEN) == 0) {
        return -TLK_EPARAM;
    }
    tmemcpy(gTlkMdiHeadsetCtrl.addr_paired_dongle, dongleAddr, 6);
    tph_host_hal_set_dongle_mac_existed(true);
    tph_host_hal_set_dongle_mac(gTlkMdiHeadsetCtrl.addr_paired_dongle);
    /* save dongle mac to flash */
    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_dongle_macUpdateHandler: new dongle!!! saving");
    tlkmdi_tinySql_setTpdMacAddr(gTlkMdiHeadsetCtrl.addr_paired_dongle); // todo, Choose the right time(power off, no tpsll music, phone) to save
    return TLK_ENONE;
}

/**
 * @brief       This function handles the connection of the dongle.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      TLK_ENONE on success.
 * @note        The function updates the connection status and get AC and CH from the MAC address.
 *              The connected tone and led UI will be displayed by the gInnerTlkMdiBtTphStateChgCB callback.
 */
static int tlkmdi_bt_tph_dongle_connectHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    (void)pData;
    gTlkMdiHeadsetCtrl.acl_mtu = pData[0];
    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_dongle_connHandler, acl_mtu %d", gTlkMdiHeadsetCtrl.acl_mtu);

    if (gInnerTlkMdiBtTphStateChgCB != NULL) {
        gInnerTlkMdiBtTphStateChgCB(TLKMDI_TPSLL_STATE_CHANGE_CB_CONNECT);
    }
    gTlkMdiHeadsetCtrl.dongleIsConn = true;
    /* 10s pairing mode clear ac,ch and pair a new dongle, if immediately 3s pairing and BT first connect�?????
       when reconnect dongle the ac and ch used 0, reconnect dongle fail. */
    tph_host_hal_get_ac_chn_from_mac(gTlkMdiHeadsetCtrl.local_addr, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ac, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ch);
    return TLK_ENONE;
}

/**
 * @brief       This function handles the disconnection of the dongle.
 * @param[in]   pData   - pointer to the data containing the disconnection reason.
 * @param[in]   dataLen - length of the data.
 * @return      TLK_ENONE on success.
 * @note        The function updates the connection status and handles specific disconnection reasons.
 */
static int tlkmdi_bt_tph_dongle_disconnHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t reason;

    reason = pData[0];
    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_dongle_disconnHandler, reason %x, cur_status %x", reason, gTlkMdiHeadsetCtrl.cur_status);

    gTlkMdiHeadsetCtrl.dongleIsConn = false;
    if (gInnerTlkMdiBtTphStateChgCB != NULL) {
        gInnerTlkMdiBtTphStateChgCB(TLKMDI_TPSLL_STATE_CHANGE_CB_DISCONNECT);
    }
    if (reason == TPH_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN && gTlkMdiHeadsetCtrl.isForceIdleing) {
        gTlkMdiHeadsetCtrl.isForceIdleing = 0;
        if (gTlkMdiHeadsetCtrl.idleCB != NULL) {
            gTlkMdiHeadsetCtrl.idleCB();
        }
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_NO_INIT;
        return TLK_ENONE;
    }

    /* only headset active pair have 3/10s setup reason,
       dongle active paring the reason is "dongle_setup - 5"*/
    if (reason == TPH_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP || reason == TPH_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP ||
        reason == TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY) {
        if (gTlkMdiHeadsetCtrl.timeout == 0) {
            // if 10s pairing timeout, don't care.
            tlkmdi_bt_tph_dongle_reconnStart();
            gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_CONNECT_WAITING;
            return TLK_ENONE;
        }
        if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_PAIRING_ASYNC_DISCON_WAITING) {
            gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_ASYNC_DISCONNECTED;
        }
    }

    if (reason == TPH_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT || reason == TPH_HOST_DISCONNECT_REASON_HEADSET_EXIT_ULTRA_LOW_LATENCY ||
        reason == TPH_HOST_DISCONNECT_REASON_DONGLE_USB_ENTER_SUSPEND) {
        if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_BT_STATUS_MSW_WAITING) {
            gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_IDLE;
        }
        tlkmdi_bt_tph_dongle_reconnStart();
    }
    return TLK_ENONE;
}

/**
 * @brief       This function handles the reception of ACL data from the dongle.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      TLK_ENONE on success.
 */
static int tlkmdi_bt_tph_headset_recvAclDataHandler(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 4) {
        return -TLK_EPARAM;
    }

    uint8_t dst_id;
    uint8_t msg_id;
    uint8_t cmd;
    uint8_t len;

    dst_id = pData[0]; // tph_msg_dst_id_for_host_t
    msg_id = pData[1]; // tph_msg_id_for_host_t
    cmd    = pData[2]; // tph_pdu_cmd_acl_for_host_e
    len    = pData[3]; // dongle acl max MTU is 180 Bytes
    // tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "### Recv Dongle Acl Data:", pData, dataLen);
    (void)msg_id;
    (void)len;
    if (dst_id & TPH_HOST_MSG_LINK_ID_HEADSET_ALL) {
        switch (cmd) {
        case TPH_HOST_MSG_PDU_ACL_CMD_DFU:
        {
            //relay dfu data
            tlkmw_ota_common_send_data(pData + 4, dataLen - 4, NULL);
        } break;
        case TPH_HOST_MSG_PDU_ACL_CMD_APP:
        {
            if (dataLen < 5) {
                return -TLK_EPARAM;
            }
            uint8_t app_cmd = pData[4];
            if (app_cmd == TLK_MDI_APP_VERSION_SYNC) {
                ARRAY_TO_UINT32L(pData, 5, gTlkMdiHeadsetCtrl.dongle_ver);
                if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_WAIT_VERSION_SYNC) {
                    gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_CONNECTED;
                    gTlkMdiHeadsetCtrl.timeout    = 0;
                }
            } else if (app_cmd == TLK_MDI_APP_DFU_STATUS_NOTIFY) {
                if (len < (sizeof(sTlkMwNotifyEvent_t) + 1)) {
                    return -TLK_EPARAM;
                }
                sTlkMwNotifyEvent_t *pEvent = (sTlkMwNotifyEvent_t *)(pData + 5);
                tlk_printf("TLK_MDI_APP_DFU_STATUS_NOTIFY ots status[%d]", pEvent->status); //refer to TLK_FIRMWARE_OTA_STATUS_ENUM;
            }
        } break;
        default:
            break;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       This function registers a callback for state changes.
 * @param[in]   cb      - the callback function to be registered.
 * @return      none.
 * @note        The callback function will be called on dongle connection and disconnection events.
 */
void tlkmdi_bt_tph_regStateChangeCB(tlkmdi_tph_state_change_cb cb)
{
    gInnerTlkMdiBtTphStateChgCB = cb;
}

/**
 * @brief       This function checks if the dongle is connected.
 * @return      true if connected, false otherwise.
 * @note        The function returns the current connection status of the dongle.
 */
bool tlkmdi_bt_tph_dongle_isConnect(void)
{
    return gTlkMdiHeadsetCtrl.dongleIsConn;
}

/**
 * @brief       This function forces the system to idle state.
 * @param[in]   cb      - the callback function to be called when the idling is finished.
 * @return      none.
 * @note        The function sets the system to force idle mode and starts the disconnection process.
 */
void tlkmdi_bt_tph_forceToIdle(tlkmdi_tph_force_idle_finished_cb cb)
{
    gTlkMdiHeadsetCtrl.timeout        = 0xFFFF;
    gTlkMdiHeadsetCtrl.isForceIdleing = 1;
    gTlkMdiHeadsetCtrl.idleCB         = cb;
    tph_host_hal_start_disconn(TPH_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN);
}

/**
 * @brief       This function restarts the BT TPH module.
 * @return      none.
 * @note        The function initiates a restart of the BT TPH module,
 *              setting up necessary parameters and starting reconnection.
 */
void tlkmdi_bt_tph_restart(void)
{
    tpsll_hci_startTpsllTaskCmd(0, NULL);
    tph_host_hal_set_gen_mac(gTlkMdiHeadsetCtrl.local_addr);
    tph_host_hal_set_dongle_mac_existed(1);
    tph_host_hal_set_dongle_mac(gTlkMdiHeadsetCtrl.addr_paired_dongle);
    tph_host_hal_set_local_mac(gTlkMdiHeadsetCtrl.local_addr);

    tlkmdi_bt_tph_dongle_reconnStart();
}

/**
 * @brief       This function attempts to reconnect to the last paired Bluetooth device.
 *              If the last paired device is successfully retrieved, it initiates the
 *              reconnection process; otherwise, it enables scanning mode to discover
 *              new devices.
 * @param[in]   retryNum - Number of reconnect attempts
 *
 * Note:        The actual reconnection is only performed when the TLK_MW_BTREC_ENABLE
 *              macro is enabled, otherwise only the scanning mode is set.
 */
static void tlkmdi_bt_tph_reconBt(uint8_t retryNum)
{
    uint32_t devClass;
    uint8_t  devAddr[6];

    int ret = tlkmdi_tinySql_getLastPairingDevice(devAddr, &devClass, NULL, NULL);
    if (ret == TLK_ENONE) {
#if TLK_MW_BTREC_ENABLE
        tlkmdi_btRecon_start(devAddr, devClass, retryNum);
#endif
    } else {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
    }
}

/**
 * @brief       This function handles the event when peer device exits low latency mode.
 *              It updates the connection status and triggers reconnection if needed.
 * @param[in]   data    - pointer to received data
 * @param[in]   dataLen - length of received data
 * @return      none.
 * @note
 */
void tlkmdi_bt_tph_exitLowLatencyMode(void)
{
    if (gTlkMdiHeadsetCtrl.disReason != TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY) {
        return;
    }

    /* Check if dongle is connected and no BT ACL link exists */
    if (gTlkMdiHeadsetCtrl.dongleIsConn != true) {
        tlkapi_error(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_exitLowLatencyMode: dongle not exist!");
        return;
    }

    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_exitLowLatencyMode: cur_status %d", gTlkMdiHeadsetCtrl.cur_status);

    /* Set mode to idle and status to mode switch waiting */
    gTlkMdiHeadsetCtrl.disReason  = TPH_HOST_DISCONNECT_REASON_HEADSET_EXIT_ULTRA_LOW_LATENCY;
    gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_BT_STATUS_MSW_WAITING;

    tph_host_hal_start_disconn(gTlkMdiHeadsetCtrl.disReason);
    tlkmdi_bt_tph_reconBt(TLKMDI_BTRECON_RETRY_NUM_POWERON);
}

/**
 * @brief       This function handles the OTA data from to phone and send to dongle.
 * @param[in]   data    - pointer to received data
 * @param[in]   dataLen - length of received data
 * @return      none.
 * @note
 */
void tlkmdi_bt_tph_send_dfu_data(uint8_t *pData, uint8_t dataLen)
{
    tph_host_hal_send_pdu_msg(TPH_HOST_MSG_PDU_ACL_CMD_DFU, pData, dataLen, NULL);
}
#endif // #if (TLK_STK_BT_TPSLL_ENABLE)
