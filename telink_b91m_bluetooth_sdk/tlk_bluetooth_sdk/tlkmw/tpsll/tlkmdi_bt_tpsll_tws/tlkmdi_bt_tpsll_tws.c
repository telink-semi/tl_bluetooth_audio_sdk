/********************************************************************************************************
 * @file    tlkmdi_bt_tpsll_tws.c
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
#include "tlkmdi_bt_tpsll_tws.h"
#if (TLKSTK_BTTPSLL_TWS_ENABLE)
#include "stack/bt/common/co_bt_defines.h"
#include "tlkmw/tlkmw.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hci.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/host/tpsll_hostevent.h"
#include "stack/bt/host/bt_tws/btp_tws_master.h"
#include "stack/bt/host/bt_tws/btp_tws_slave.h"
#include "stack/bt/host/bth/bth_define.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/multiCoreComm/service/service_mailbox.h"

#define TLKMDI_BT_TPT_TWS_DBG_FLAG 0xFFFFFFFF
#define TLKMDI_BT_TPT_TWS_DBG_SIGN "[MBT_TWS]"


static tlkmdi_bt_tpt_ctrl_t       sTlkMdiBtTpsllTwsCtrl;
static tlkmdi_tpt_state_change_cb sTlkMdiBtTptStateChgCB;


static int tlkmdi_bt_tpt_headset_connected_CB(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_bt_tpt_headset_disconnect_CB(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_bt_tpt_dongle_macUpdateHandler(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_bt_tpt_dongle_connectHandler(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_bt_tpt_dongle_disconnHandler(uint8_t *pData, uint16_t dataLen);
void       tlkmdi_bt_tpt_dongle_reconStart(void);
int        tlkmdi_bt_tpt_dongle_powerOnRecon(void);
void       tlkmdi_bt_tpt_dongle_paringStart(uint16_t timeout);

static void tlkmdi_bt_tpt_handover_timer(TlkApiTimerHandle_t pTimer, void *userArg);
void        tlkmdi_bt_tpt_handover_getAppInfo(uint16_t aclHandle, btp_tws_get_bt_app_state_info_t *pInfo);
static void tlkmdi_bt_tpt_handover_extraceHostInfoHandler(uint8_t *pData);
static int  tlkmdi_bt_tpt_handover_success(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_bt_tpt_handover_slaveInfoSync(uint8_t *pData, uint16_t dataLen);
void        tlkmdi_bt_tpt_handover_setMask(uint8_t *pData);
void        tlkmdi_bt_tpt_handover_clearMask(uint8_t *pData);

static void tlkmdi_bt_tpt_timer(TlkApiTimerHandle_t pTimer, void *userArg);
static void tlkmdi_bt_tpt_powerOffTimer(TlkApiTimerHandle_t pTimer, void *userArg);
static int  tlkmdi_bt_tpt_headset_setupTimeoutCB(uint8_t *pData, uint16_t dataLen);
static bool tlkmdi_bt_tpt_check_terminate_bt(bool isClose);
static void tlkmdi_bt_tpt_reconBt(uint8_t retryNum);

int tlkmdi_bt_tpt_virtualLink_slaveScoSetupCompleteHandler(uint8_t *pData, uint16_t dataLen);
int tlkmdi_bt_tpt_virtualLink_masterSyncProfileInfoHandler(uint8_t *pData, uint16_t dataLen);
int tlkmdi_bt_tpt_virtualLink_slaveProfileInfoSyncHandler(uint8_t *pData, uint16_t dataLen);

static void tlkmdi_bt_tpt_wired_getGenMac(uint8_t *out, uint8_t *localMac, uint8_t *peerMac);
static void tlkmdi_bt_tpt_tpsllHandler(void);
static bool tlkmdi_bt_tpt_pageSuccessAndConnecting(uint16_t *pHandle);
static void tlkmdi_bt_tpt_latencyModeChangeHandler(void);
static void tlkmdi_bt_tpt_musicStatusNotifyHandler(void);

TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_CONNECTED, tlkmdi_bt_tpt_headset_connected_CB);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_DISCONNECTED, tlkmdi_bt_tpt_headset_disconnect_CB);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_CONNECT_SETUP_TIMEOUT, tlkmdi_bt_tpt_headset_setupTimeoutCB);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_SLAVE_PROFILE_INFO_SYNC, tlkmdi_bt_tpt_virtualLink_slaveProfileInfoSyncHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_SLAVE_HANDOVER_HOST_STATUS_SYNC, tlkmdi_bt_tpt_handover_slaveInfoSync);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_MASTER_SLAVE_HANDOVER_SUCC, tlkmdi_bt_tpt_handover_success);
TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_MAC_UPDATE, tlkmdi_bt_tpt_dongle_macUpdateHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_CONNECT, tlkmdi_bt_tpt_dongle_connectHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_DONGLE_DISCONNECT, tlkmdi_bt_tpt_dongle_disconnHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_SLAVE_SCO_SETUP_COMPLETE, tlkmdi_bt_tpt_virtualLink_slaveScoSetupCompleteHandler);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_MASTER_SYNC_PROFILE, tlkmdi_bt_tpt_virtualLink_masterSyncProfileInfoHandler);

/**
 * @brief       Initialize the TWS (True Wireless Stereo) Bluetooth module and TPSLL protocol.
 * @return      none.
 * @note        none.
 */
void tlkmdi_bt_tpt_init(void)
{
    /* Register and enable power management channel for BT TPT module */
    tlksys_pm_regChn(TLKSYS_PM_CHN_BT_TPT);
    tlksys_pm_setChn(TLKSYS_PM_CHN_BT_TPT, 1, 0);

    /* Register HCI and SCO data receive callbacks for TPSLL share memory */
    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_recvC2HData);
    tlk_d25f_register_sync_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL, tlktpsll_hci_sco_recvC2HData);

    /* Register mailbox message callbacks for handover operations */
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_MASK_SET_EVT, tlkmdi_bt_tpt_handover_setMask);
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_MASK_CLEAR_EVT, tlkmdi_bt_tpt_handover_clearMask);
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_INFO_EXTRACT, tlkmdi_bt_tpt_handover_extraceHostInfoHandler);

    /* Register task event callbacks for various TPSLL events */
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_TPSLL, tlkmdi_bt_tpt_tpsllHandler);
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_LOW_LATENCY_STATE, tlkmdi_bt_tpt_latencyModeChangeHandler);
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_MUSIC_STATUS_NOTIFY, tlkmdi_bt_tpt_musicStatusNotifyHandler);

    /* Initialize TPSLL TWS control structure */
    tmemset(&sTlkMdiBtTpsllTwsCtrl, 0, sizeof(tlkmdi_bt_tpt_ctrl_t));

    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    sTlkMdiBtTpsllTwsCtrl.cur_role      = BTH_TPT_TWS_ROLE_IDLE;
    sTlkMdiBtTpsllTwsCtrl.dongle_status = TLKMDI_BT_TPT_CONNECT_STATUS_IDLE;

    /*Load tws info from flash.*/
    tlkmdi_tinySql_getBtMacAddress(sTlkMdiBtTpsllTwsCtrl.const_addr);
    tlkmdi_tinySql_getPeerAddr(sTlkMdiBtTpsllTwsCtrl.peer_addr);
    tlkmdi_tinySql_getNewAllocAddr(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr);
    tlkmdi_tinySql_getGenAcAddr(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr);
    tlkmdi_tinySql_getTpdMacAddr(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr);

    /*Init tws param.*/
    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, null_addr1, BD_ADDR_LEN) == 0 || tmemcmp(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, null_addr0, BD_ADDR_LEN) == 0) {
        sTlkMdiBtTpsllTwsCtrl.p_cur_addr = sTlkMdiBtTpsllTwsCtrl.const_addr;
    } else {
        sTlkMdiBtTpsllTwsCtrl.p_cur_addr = sTlkMdiBtTpsllTwsCtrl.new_alloc_addr;
    }

    /* Start TPSLL task based on device side (left/right) */
    if (tlkmdi_bt_tpt_isLeft()) {
        tpsll_hci_startTpsllTaskCmd(1, NULL);
    } else {
        tpsll_hci_startTpsllTaskCmd(0, NULL);
    }

    /* Set constant BT address for TPSLL */
    tpsll_hci_sendSetBtAddrCmd(true, sTlkMdiBtTpsllTwsCtrl.const_addr);

    /* Set current BT address for host */
    bth_hci_sendSetBtAddrCmd(sTlkMdiBtTpsllTwsCtrl.p_cur_addr);

    /*  calc ac and chn.
        1.if GenMac exist use GenMac to calc ac and chn.
        2.if not exist use const_addr to calc ac and chn.*/
    tlkapi_array(0xffffffff, "[TWS]", "tlkmdi_bt_tpt_init: GenMac-", sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, 6);

    /* Calculate access code and channel based on GenMac or const address */
    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, null_addr0, BD_ADDR_LEN) != 0 && tmemcmp(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, null_addr1, BD_ADDR_LEN) != 0) {
        tph_host_hal_get_ac_chn_from_mac(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ac, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ch);
    } else {
        tph_host_hal_get_ac_chn_from_mac(sTlkMdiBtTpsllTwsCtrl.const_addr, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ac, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ch);
    }

    /* Register profile virtual link event */
    bth_handle_acl_register_tws_role_cb(tlkmdi_bt_tpt_getRole);
    bth_handle_acl_register_tws_handover_cb(tlkmdi_bt_tpt_handover_isBusy);
    btp_tws_get_bt_app_state_info_register(tlkmdi_bt_tpt_handover_getAppInfo);

    /* Create static timers for various TWS operations */
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer, TLK_MDI_BT_TPT_TIMER_PERIOD_US, false, tlkmdi_bt_tpt_timer, NULL);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.powerOffTimer, 500 * 1000, false, tlkmdi_bt_tpt_powerOffTimer, NULL);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.handoverTimer, 10 * 1000, false, tlkmdi_bt_tpt_handover_timer, NULL);

    /* Initialize key handling and data synchronization modules */
    tlkmdi_bt_tpsll_key_init();
    tlkmdi_bt_tpt_data_sync_init();

#if TLK_DEV_SY5500_ENABLE
    tlkmdi_bt_tpt_case_init();
#endif

    // gpio_function_en(GPIO_PA0);
    // gpio_output_en(GPIO_PA0);
    // gpio_input_dis(GPIO_PA0);
}

/**
 * @brief       Start the TWS pairing process for dual headset reconnection
 * @param[in]   none
 * @return      none
 * @note        This function initializes the pairing mode to normal mode, sets up the headset
 *              access code and channel ID, configures timeout and status for waiting N22 ready,
 *              and starts the timer for the TWS pairing process.
 */
void tlkmdi_bt_tpt_start(void)
{
    sTlkMdiBtTpsllTwsCtrl.pair_mode = TPT_HOST_HEADSET_SETUP_MODE_NORMAL;

    /*dual headset recon*/
    tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
    sTlkMdiBtTpsllTwsCtrl.timeout    = TLK_MDI_BT_TPT_WAIT_N22_READY_TIMEOUT_MS;
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_RECON_STATUS_WAIT_N22_READY;
    tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer, TLK_MDI_BT_TPT_TIMER_PERIOD_US);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer);
}

/**
 * @brief       Start the TWS pairing request process
 * @param[in]   isRefactory - Pairing mode, refer to tpt_headset_setup_mode_for_host_e
 *              - TPT_HOST_HEADSET_SETUP_MODE_NORMAL: Normal mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_3S: 3-second pairing mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_10S: 10-second pairing mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY: Ultra low latency mode
 * @param[in]   peerMac - Peer headset MAC address, if NULL, will trigger wireless pairing
 * @return      None
 * @note        This function initiates the TWS pairing process. It checks connection status,
 *              handles special cases like ultra-low latency mode, synchronizes pairing between
 *              TWS headsets, and manages the pairing sequence based on the specified mode.
 */
void tlkmdi_bt_tpt_pair_start_req(uint8_t isRefactory, uint8_t *peerMac)
{
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
    tlk_printf("tlkmdi_bt_tpt_pair_start_req isRefactory 0x%x peerMac 0x%x ", isRefactory, peerMac);
    sTlkMdiBtTpsllTwsCtrl.pair_mode = isRefactory;
    if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY) {
        if (sTlkMdiBtTpsllTwsCtrl.dongle_status != TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED) {
            tlk_printf("tlkmdi_bt_tpt_pair_start_req: lowLatency, dongle isn't exist!!!");
            return;
        }
    }

    /* Single headset trigger wirless paring notify another tws headset enter pairing */
    if (peerMac == NULL && tlkmdi_bt_tpt_isConnected()) {
        tlkmdi_bt_tpt_sync_tws_pairing(isRefactory);

        /* Wait remote headset disconnect tpsll(10s pairing mode),make sure pairing sync message send success */
        if (!tlkmdi_bt_tpt_check_terminate_bt(false)) {
            if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_10S) {
                sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCON_WAITING;
            } else {
                sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED;
            }
            sTlkMdiBtTpsllTwsCtrl.timeout = TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS;
        }
    }
    tlkmdi_bt_tpt_pair_start(isRefactory, peerMac);
}

/**
 * @brief       Start pairing synchronization from remote device, triggered when remote single headset starts wireless pairing
 *              and notifies another TWS headset to enter pairing mode
 * @param[in]   data: Pointer to the data containing pairing mode information
 * @param[in]   dataLen: Length of the data (unused in current implementation)
 * @return      none
 * @note
 */
void tlkmdi_bt_tpt_pair_start_sync_from_remote(void *data, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t isRefactory              = *((uint8_t *)data);
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
    sTlkMdiBtTpsllTwsCtrl.pair_mode  = isRefactory;
    tlkmdi_bt_tpt_pair_start(isRefactory, NULL);
}

/**
 * @brief       Start the TWS pairing process
 * @param[in]   isRefactory - Pairing mode, refer to tpt_headset_setup_mode_for_host_e
 * @param[in]   peerMac - Peer device MAC address, if NULL triggers wireless pairing,
 *              if not NULL indicates pairing with charging box
 * @return      none
 * @note        This function initializes the pairing process based on the specified mode.
 *              For 10-second pairing mode, it clears pairing information and handles
 *              charging box pairing if peerMac is provided. It also handles termination
 *              of existing Bluetooth connections if needed and sets up appropriate
 *              timeouts and statuses for the pairing process.
 */
void tlkmdi_bt_tpt_pair_start(uint8_t isRefactory, uint8_t *peerMac)
{
    tlk_printf("tlkmdi_bt_tpt_pair_start isRefactory %d, cur_state %d, headset_status %d", isRefactory, sTlkMdiBtTpsllTwsCtrl.cur_status, sTlkMdiBtTpsllTwsCtrl.headset_status);

    /* Handle 10-second pairing mode initialization */
    if (isRefactory == TPT_HOST_HEADSET_SETUP_MODE_10S) {
        /* Clear all pairing-related addresses and information */
        tmemset(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, 0, 6);
        tmemset(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, 0, 6);
        tmemset(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, 0, 6);
        tmemset(sTlkMdiBtTpsllTwsCtrl.peer_addr, 0, 6);
        sTlkMdiBtTpsllTwsCtrl.ble_ac = 0;
        sTlkMdiBtTpsllTwsCtrl.ble_ch = 0;
        tlkmdi_tinySql_cleanPairingDevices();

        /* Handle pairing with charging box */
        if (peerMac != NULL) {
            tlkmdi_bt_tpt_wired_getGenMac(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, sTlkMdiBtTpsllTwsCtrl.const_addr, peerMac);
            tph_host_hal_get_ac_chn_from_mac(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ac, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ch);
            sTlkMdiBtTpsllTwsCtrl.pair_inBox = 1;
        }
    }

    sTlkMdiBtTpsllTwsCtrl.pair_mode = isRefactory;

    /* Check if we need to terminate existing BT connection */
    if (tlkmdi_bt_tpt_check_terminate_bt(false)) {
        /* If connected and master, or just connected, terminate BT connection */
        if (tlkmdi_bt_tpt_isConnected()) {
            if (tlkmdi_bt_tpt_isMaster()) {
                tlkmdi_bt_tpt_check_terminate_bt(true);
            }
        } else {
            tlkmdi_bt_tpt_check_terminate_bt(true);
        }

        /* Set timeout for checking BT disconnection and update status */
        sTlkMdiBtTpsllTwsCtrl.timeout    = 4000; // Check whether BT is disconn within 2s
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCON_WAITING;

    } else {
        /* If not terminating BT and not waiting for TWS dongle disconnection, mark BT as disconnected */
        if (sTlkMdiBtTpsllTwsCtrl.cur_status != TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCON_WAITING) {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED;
        }
    }

    tlk_printf("tlkmdi_bt_tpt_pair_start22 isRefactory %d, cur_state %d, headset_status %d", isRefactory, sTlkMdiBtTpsllTwsCtrl.cur_status, sTlkMdiBtTpsllTwsCtrl.headset_status);

    /* Restart the timer for pairing process */
    tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer, TLK_MDI_BT_TPT_TIMER_PERIOD_US);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer);
}

/**
 * @brief       Enter the pairing mode for TWS headset
 * @param[in]   isSingle - Flag indicating if the headset should operate in single mode
 *              - true: Set headset as single device
 *              - false: Continue with normal dual headset pairing flow, if slave-headset close bt scan and recon.
 * @return      none
 * @note        This function handles entering pairing mode differently based on whether the headset
 *              should act as a single device or as part of a TWS pair. For slave devices in dual mode,
 *              it stops reconnection and pairing processes and disables scanning. For other configurations,
 *              it enables scanning for 2 minutes and may start dongle pairing.
 */
static void tlkmdi_bt_tpt_pair_enter(bool isSingle)
{
    uint8_t null_addr0[BD_ADDR_LEN], null_addr1[BD_ADDR_LEN];
    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    if (sTlkMdiBtTptStateChgCB != NULL) {
        sTlkMdiBtTptStateChgCB(TLKMDI_TPT_STATE_CHANGE_CB_PAIR);
    }
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING;

    /* For dual-mode slave, stop reconnection and pairing, disable scanning and return early */
    if (!isSingle && tlkmdi_bt_tpt_isSlave()) {
        if (tlkmdi_btRecon_isInBusy()) {
            tlkmdi_btRecon_close();
        }
        tlkmdi_btParing_stop();

        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 120);
        return;
    }

    tlk_printf("tlkmdi_bt_tpt_pair_enter isSingle %d, cur_status %d, cur_role %d", isSingle, sTlkMdiBtTpsllTwsCtrl.cur_status, sTlkMdiBtTpsllTwsCtrl.cur_role);

    if (isSingle) {
        tpsll_hci_sendSetHeadsetIsSingleCmd();
        sTlkMdiBtTpsllTwsCtrl.cur_role = BTH_TPT_TWS_ROLE_SINGLE;
    }
    /* Enable both inquiry and page scanning for 2 minutes to allow pairing */
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);

    if (!isSingle) {
        tlkmdi_bt_tpt_dongle_paringStart(300);
    } else {
        tlk_printf("[MBT_TWS]<error>tlkmdi_bt_tpt_pair_enter: single mode not allowed enter paring with dongle, cur_state-%d", sTlkMdiBtTpsllTwsCtrl.cur_status);
    }
}

/**
 * @brief       Process the TWS pairing state machine
 * @return      bool - Returns true to continue processing, false when pairing is completed
 * @note        This function handles the different states of the TWS pairing process,
 *              managing transitions between states such as disconnecting existing connections,
 *              setting up new connections, and handling timeouts.
 *
 *              The function operates as a state machine with the following key states:
 *              - TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCON_WAITING: Waiting for BT disconnection
 *              - TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED: BT has been disconnected
 *              - TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCON_WAITING: Waiting for dongle disconnection
 *              - TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCONNECTED: Dongle has been disconnected
 *              - TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_COMPLETE: TWS synchronization is complete
 *              - TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING: Waiting for new connections
 */
bool tlkmdi_bt_tpt_pair_procs()
{
    //tlk_printf("pro isSingle s %d, m %d,ds %d,to %d", sTlkMdiBtTpsllTwsCtrl.cur_status,sTlkMdiBtTpsllTwsCtrl.pair_mode,sTlkMdiBtTpsllTwsCtrl.dongle_status,
    //sTlkMdiBtTpsllTwsCtrl.timeout);

    switch (sTlkMdiBtTpsllTwsCtrl.cur_status) {
    case TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCON_WAITING:
    {
        /* Check if BT disconnection is complete */
        if (!tlkmdi_bt_tpt_check_terminate_bt(false)) {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED;
        }
    } break;
    case TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED:
    {
        /* Handle different pairing modes after BT disconnection */
        if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_10S) { // 10s paring dongle and dual headset will disconn if exist.
            tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_10S);
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCON_WAITING;
        } else if (sTlkMdiBtTpsllTwsCtrl.dongle_status == TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED) {
            /* Disconnect dongle connection based on device role and pairing mode */
            if (tlkmdi_bt_tpt_isMaster()) {
                if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_3S) {
                    tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_3S);
                }
                if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY) {
                    tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY);
                }
            }
            if (tlkmdi_bt_tpt_isSingle()) {
                if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY) {
                    tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY);
                }
            }
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCON_WAITING; /*same as disconnect BT LINK, tws master is initiator,slave wait timeout  */
        } else {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCONNECTED;
        }
        sTlkMdiBtTpsllTwsCtrl.timeout = TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS;
    } break;

    case TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCON_WAITING:
    {
    } break;

    case TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCONNECTED:
    {
        tlk_printf("tlkmdi_bt_tpt_pair_procs [STATUS_TPT_DISCONNECTED] pair_mode %d,TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS %d,timeout 0x%x", sTlkMdiBtTpsllTwsCtrl.pair_mode,
                   TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS, sTlkMdiBtTpsllTwsCtrl.timer.timeout);

        /* Configure headset connection setup based on pairing mode */
        if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_10S) {
            if (sTlkMdiBtTpsllTwsCtrl.pair_inBox) {
                sTlkMdiBtTpsllTwsCtrl.pair_inBox = 0;
                tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
                tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_10S, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
            } else {
                tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(TPT_HOST_HEADSET_SETUP_COMMON_ACCESSCODE, TPT_HOST_HEADSET_SETUP_COMMON_CHN);
                tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_10S, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
            }

        } else if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_3S) {
            tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
            tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_3S, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
        }
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING;
        sTlkMdiBtTpsllTwsCtrl.timeout    = TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS;
    } break;
    case TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCONNECTED:
    {
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_COMPLETE;
    } break;
    case TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_COMPLETE:
    {
        /* Enter pairing mode and finish the pairing process */
        tlkmdi_bt_tpt_pair_enter(false);
        return false;
    } break;
    case TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING:
    {
        int     ret           = true;
        uint8_t bt_link_count = 0;
        /* TODO: before TLKMDI_BT_TPT_PAIR_STATUS_IDLE valid dongle connected?*/
        bt_link_count = tlkmdi_btacl_getUsedCount();

        /* Check if dongle is already connected */
        if (sTlkMdiBtTpsllTwsCtrl.dongle_status == TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED) {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
            if (tlkmdi_bt_tpt_isMaster()) {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
            }
            tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_pair_procs: dongle connect before BT");
            ret = false;
        }

        /* Handle ultra low latency mode */
        if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY) {
            bt_link_count = 0xFF;
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 120);
        }

        /* Check if BT link is already established */
        if (bt_link_count > 0) { //TODO: MUST CONNECTED LINK ZEWEN,CC HEADSET HAVE SAME PROBLEM
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
            tlkmdi_bt_tpt_dongle_reconStart();
            tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_pair_procs: BT connect before dongle, bt_link_count-%d", bt_link_count);
            ret = false;
        }
        sTlkMdiBtTpsllTwsCtrl.peerAclSetup = TLK_STATE_CONNING;
        return ret;
    } break;
    default:
        return true;
    }

    return true;
}

/**
 * @brief       Enter the reconnection process for TWS headset
 * @param[in]   isSingle - Flag indicating if the headset should operate in single mode
 *              - true: Set headset as single device
 *              - false: Continue with normal dual headset reconnection flow
 * @return      bool - Returns true if operating in single mode, false otherwise
 * @note        This function handles the reconnection process for TWS headsets.
 *              In single mode, it configures the headset as a standalone device.
 *              In dual mode, it manages reconnection of both Bluetooth and dongle connections.
 *              Slave headsets in dual mode will exit without performing reconnection.
 */
static bool tlkmdi_bt_tpt_recon_enter(bool isSingle)
{
    uint8_t null_addr0[BD_ADDR_LEN], null_addr1[BD_ADDR_LEN];
    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    if (sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_IDLE) {
        if (isSingle) {
            tpsll_hci_sendSetHeadsetIsSingleCmd();
            sTlkMdiBtTpsllTwsCtrl.cur_role = BTH_TPT_TWS_ROLE_SINGLE;
            /* Avoid the failure of the dongle to reconnect after the dual headset connect timeout */
            if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING) {
                sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
            }
        }
        tlkmdi_bt_tpt_dongle_reconStart();
    }

    /*Recon bt*/
    tlkmdi_bt_tpt_reconBt(TLKMDI_BTRECON_RETRY_NUM_POWERON);

    /*Recon dongle*/
    if (sTlkMdiBtTpsllTwsCtrl.dongle_status != TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED) {
        tlkmdi_bt_tpt_dongle_reconStart();
    }
    /*Bt recon manage by bt, here end.*/
    if (!isSingle) {
        return false;
    }
    return true;
}

/**
 * @brief       Process TWS reconnection procedures based on current connection status and device role
 * @param[in]   none
 * @return      bool - Returns true if TWS is connected and device is master, otherwise returns false or true based on conditions
 * @note        This function handles reconnection logic differently for master and slave devices.
 *              Master device initiates reconnection procedure, while slave device stops pairing
 *              and disables scanning when TWS is connected.
 */
bool tlkmdi_bt_tpt_recon_procs()
{
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_TWS_CONNECTED) {
        if (tlkmdi_bt_tpt_isMaster()) {
            return tlkmdi_bt_tpt_recon_enter(false);
        } else {
            /*TODO: slave need to check whether start handover or not. Now end timer.*/
            if (tlkmdi_btRecon_isInBusy()) {
                tlkmdi_btRecon_close();
            }
            tlkmdi_btParing_stop();

            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 120);
            return false;
        }
    }

    return true;
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
static void tlkmdi_bt_tpt_reconBt(uint8_t retryNum)
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
 * @brief       Timer callback function for TWS bluetooth module, handles various state transitions for device reconnection and pairing
 * @param[in]   pTimer  - Timer handle pointer
 * @param[in]   userArg - User-defined argument
 * @return      none
 * @note        This function performs different operations based on current status and timeout conditions,
 *              including reconnection and pairing processes
 */
static void tlkmdi_bt_tpt_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    //gpio_write(GPIO_PA0, 1);
    if (sTlkMdiBtTpsllTwsCtrl.timeout == 0) {
        tlkapi_trace(0xffffffff, "[TWS]", "tlkmdi_bt_tpt_timer: timeout, cur_status %d", sTlkMdiBtTpsllTwsCtrl.cur_status);
    }

    if (tlkmdi_bt_tpt_recon_procs() == false) {
        // gpio_write(GPIO_PA0, 0);
        return;
    }

    if (tlkmdi_bt_tpt_pair_procs() == false) {
        //gpio_write(GPIO_PA0, 0);
        return;
    }

    /* Countdown processing */
    if (sTlkMdiBtTpsllTwsCtrl.timeout > 0) {
        //if (sTlkMdiBtTpsllTwsCtrl.timeout%5 == 0) {
        // tlk_printf("tlkmdi_bt_tpt_timer: timeout %d",sTlkMdiBtTpsllTwsCtrl.timeout);
        //}
        sTlkMdiBtTpsllTwsCtrl.timeout--;

        //gpio_write(GPIO_PA0, 0);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
        return;
    }

    /* Power on wait for N22 ready 2000ms timeout */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_WAIT_N22_READY) {
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING;
        sTlkMdiBtTpsllTwsCtrl.timeout    = TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS;
        tpsll_hci_sendSetGeneralMacCmd(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr);
        tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_NORMAL, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
        return;
    }

    /* Power on 500ms timeout, start reconnecting dongle and bt as single device */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING) {
        tlk_printf("tlkmdi_bt_tpt_timer: headset connecting timeout.");
        tlkmdi_bt_tpt_recon_enter(true);
    }

    /* Pairing mode */

    /* Pairing mode: tws disconnect timeout */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCON_WAITING) {
        tlk_printf("tlkmdi_bt_tpt_timer: tws disconn timeout.");
        //TODO
    }

    /* BT disconnect timeout */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCON_WAITING) {
        tlk_printf("tlkmdi_bt_tpt_timer: BT disconn timeout.");
        //TODO
    }

    /* 500ms timeout. As single device enter bt/dongle pairing mode */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING) {
        tlk_printf("tlkmdi_bt_tpt_timer: TWS_SYNC timeout.");

        bth_hci_sendSetBtAddrCmd(sTlkMdiBtTpsllTwsCtrl.const_addr);
        sTlkMdiBtTpsllTwsCtrl.p_cur_addr = sTlkMdiBtTpsllTwsCtrl.const_addr;
        tlkmdi_bt_tpt_pair_enter(true);
    }

    /* 30s timeout. Start reconnecting previously paired dongle */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING) {
        tlk_printf("tlkmdi_bt_tpt_timer:  timeout no new bt/dongle dev connected");
        if (sTlkMdiBtTptStateChgCB != NULL) { /* Only for updating LED pattern as idle */
            sTlkMdiBtTptStateChgCB(TLKMDI_TPT_STATE_CHANGE_CB_DISCONNECT);
        }
        if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP) {
            tlkmdi_bt_tpt_dongle_reconStart();
        }
    }
}

/**
 * @brief       TWS headset connection callback function. Called when TWS headset is successfully connected,
 *              handles various state settings and address configuration after connection
 * @param[in]   pData    - Pointer to received data containing connection related information
 *                       First byte represents the role. Subsequent bytes contain address information
 * @param[in]   dataLen  - Data length
 * @return      TLK_ENONE  Execution successful
 *              -TLK_EPARAM Parameter error, pData is null or dataLen less than 7
 */
static int tlkmdi_bt_tpt_headset_connected_CB(uint8_t *pData, uint16_t dataLen)
{
    uint8_t len = 0;
    if (pData == NULL || dataLen < 7) {
        return -TLK_EPARAM;
    }
    tlk_printf("###TWS_CONNECTED  role[%d],cur_status[%d],pair_mode[%d]", pData[0], sTlkMdiBtTpsllTwsCtrl.cur_status, sTlkMdiBtTpsllTwsCtrl.pair_mode);

    sTlkMdiBtTpsllTwsCtrl.cur_role = pData[0];

    sTlkMdiBtTpsllTwsCtrl.headset_status = TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED;

    tlkmdi_bt_tpt_setAutoLedSync(true);
    tlkmdi_bt_tpt_boxStateSync(tlkmdi_bt_tpt_getLocalCaseState());
    if (tlkmdi_bt_tpt_isMaster()) {
        tlkmdi_bt_tpt_syncAncState(tlkmw_anc_is_enable());
    }

    /* Handle TWS headset reconnection state */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING) {
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_RECON_STATUS_TWS_CONNECTED;

        /* Master device starts dongle reconnection */
        if (tlkmdi_bt_tpt_isMaster()) {
            tlkmdi_bt_tpt_dongle_powerOnRecon();
        }
        /* Handle TWS pairing synchronization state */
    } else if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING) {
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_COMPLETE;
        if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_10S) {
            /* Save new address information */

            len = 1;
            tmemcpy(sTlkMdiBtTpsllTwsCtrl.peer_addr, pData + len, 6);
            len += 6;
            tmemcpy(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, pData + len, 6);
            len += 6;
            tmemcpy(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, pData + len, 6);
            len += 6;

            tlkapi_array(0xffffffff, "[TWS]", "peer_addr:", sTlkMdiBtTpsllTwsCtrl.peer_addr, 6);
            tlkapi_array(0xffffffff, "[TWS]", " gen_addr:", sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, 6);
            tlkapi_array(0xffffffff, "[TWS]", " new_addr:", sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, 6);


            tlkmdi_tinySql_setPeerAddr(sTlkMdiBtTpsllTwsCtrl.peer_addr);
            tlkmdi_tinySql_setGenAcAddr(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr);
            tlkmdi_tinySql_setNewAllocAddr(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr);
            sTlkMdiBtTpsllTwsCtrl.p_cur_addr = sTlkMdiBtTpsllTwsCtrl.new_alloc_addr;
            bth_hci_sendSetBtAddrCmd(sTlkMdiBtTpsllTwsCtrl.new_alloc_addr);
            tph_host_hal_get_ac_chn_from_mac(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ac, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.ble_ch);
        }
    } else {
        tlk_printf("tlkmdi_bt_tpt_headset_connected_CB err status[%d]", sTlkMdiBtTpsllTwsCtrl.cur_status);
    }

    return TLK_ENONE;
}

/**
 * @brief       Callback function for TWS headset disconnection event handling.
 *              Processes different disconnection reasons and updates system state accordingly.
 * @param[in]   pData    - Pointer to received data containing disconnection reason.
 *                       First byte represents the disconnection reason code.
 * @param[in]   dataLen  - Length of the data in bytes
 * @return      TLK_ENONE  - Operation completed successfully
 *              -TLK_EPARAM - Invalid parameters (null pointer or data length less than 1)
 * @note        Handles various disconnection scenarios including 10s pairing mode initiation,
 *              link supervision timeouts, and headset shutdown events.
 *              Updates headset status to DISCONNECTED and adjusts system behavior based on
 *              the specific disconnection reason.
 */
static int tlkmdi_bt_tpt_headset_disconnect_CB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 1) {
        return -TLK_EPARAM;
    }

    tlk_printf("###TWS_DISCONNECTED role[%d] reason[%d],cur_status[%d],pair_mode[%d]", sTlkMdiBtTpsllTwsCtrl.cur_role, pData[0], sTlkMdiBtTpsllTwsCtrl.cur_status,
               sTlkMdiBtTpsllTwsCtrl.pair_mode);

    tlkmdi_bt_tpt_setAutoLedSync(false);

    sTlkMdiBtTpsllTwsCtrl.headset_status = TLKMDI_BT_TPT_CONNECT_STATUS_DISCONNECTED;
    //TODO: update sTlkMdiBtTpsllTwsCtrl.cur_role;
    switch (pData[0]) {
        // case TPT_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP:/*only 10s pairing mode trigger tws disconnection*/
    case TPT_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP:
        sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCONNECTED;
        sTlkMdiBtTpsllTwsCtrl.cur_role   = BTH_TPT_TWS_ROLE_IDLE;
        sTlkMdiBtTpsllTwsCtrl.timeout    = TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS; //restart 500ms timer.
        break;
    case TPT_HOST_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT:
        sTlkMdiBtTpsllTwsCtrl.cur_role = BTH_TPT_TWS_ROLE_IDLE;
        if (tlkmdi_bt_tpt_isMaster()) {
            if (tlkmdi_bt_tpt_check_terminate_bt(false) || (sTlkMdiBtTpsllTwsCtrl.dongle_status == TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED)) {
                tpsll_hci_sendSetHeadsetIsSingleCmd();
                sTlkMdiBtTpsllTwsCtrl.cur_role = BTH_TPT_TWS_ROLE_SINGLE;
            }
        } else if (tlkmdi_bt_tpt_isSlave()) {
            tlk_printf("tlkmdi_bt_tpt_headset_disconnect_CB: HEADSET_LINKSUPERVISION_TIMEOUT master_lost");
        } else {
            tlk_printf("tlkmdi_bt_tpt_headset_disconnect_CB: HEADSET_LINKSUPERVISION_TIMEOUT state_error");
        }
        tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
        tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_NORMAL, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
        break;
    case TPT_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT:
        tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "headset_disconnect_CB: TPT_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT");
        tlkmdi_bt_tpt_dongle_reconStart();
        break;
    case TPT_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN:
        if (sTlkMdiBtTpsllTwsCtrl.is_shutdown) {
            tlksys_pm_setChn(TLKSYS_PM_CHN_BT_TPT, 0, 0);
            return TLK_ENONE;
        }
        if (tlkmdi_bt_tpt_isMaster()) {
            tpsll_hci_sendSetHeadsetIsSingleCmd();
            sTlkMdiBtTpsllTwsCtrl.cur_role = BTH_TPT_TWS_ROLE_SINGLE;
            tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
            tpsll_hci_sendHeadsetConnectSetupCmd(TPT_HOST_HEADSET_SETUP_MODE_NORMAL, TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US);
        }
        break;
    default:
        break;
    }

    return TLK_ENONE;
}

/**
 * @brief       TWS headset setup timeout callback function
 * @param[in]   pData - pointer to data (unused)
 * @param[in]   dataLen - data length (unused)
 * @return      TLK_ENONE if operation succeeds, -TLK_EFAULT if current status is not
 *              TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING or TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING
 *
 * @note        This function only operates when the device is in specific states, resetting
 *              the timeout counter to prevent timeout from occurring
 */
int tlkmdi_bt_tpt_headset_setupTimeoutCB(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (sTlkMdiBtTpsllTwsCtrl.cur_status != TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING && sTlkMdiBtTpsllTwsCtrl.cur_status != TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING) {
        return -TLK_EFAULT;
    }
    sTlkMdiBtTpsllTwsCtrl.timeout = 0;
    //set timeout and will set single
    return TLK_ENONE;
}

/**
 * @brief       Exit the ultra low latency mode for TWS headset
 * @return      none
 * @note        This function handles the transition from ultra low latency mode back to normal mode.
 *              It performs necessary synchronization with the peer headset and initiates
 *              reconnection procedures. The function only operates when currently in
 *              ultra low latency mode and when dongle is connected without active BT ACL links.
 */
void tlkmdi_bt_tpt_headset_exitLowLatencyMode(void)
{
    /* Check if currently in ultra low latency mode, if not, return immediately */
    if (sTlkMdiBtTpsllTwsCtrl.pair_mode != TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY) {
        return;
    }

    /* Check if dongle is connected and no BT ACL link exists */
    if (sTlkMdiBtTpsllTwsCtrl.dongle_status != TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED || tlkmdi_bt_tpt_check_terminate_bt(false)) {
        tlkapi_error(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_headset_exitLowLatencyMode: dongle not exist or bt acl exist!");
        return;
    }

    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_headset_exitLowLatencyMode: cur_status %d", sTlkMdiBtTpsllTwsCtrl.cur_status);

    /* Set mode to idle and status to mode switch waiting */
    sTlkMdiBtTpsllTwsCtrl.pair_mode  = TPT_HOST_HEADSET_SETUP_MODE_IDLE;
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_RECON_STATUS_MSW_WAITING;

    /* Synchronize with peer headset to exit low latency mode */
    tlkmdi_bt_tpt_sync_tws_exitLowLatency();

    /* For master or single device, disconnect and initiate reconnection */
    if (!tlkmdi_bt_tpt_isSlave()) {
        tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_EXIT_ULTRA_LOW_LATENCY);
        tlkmdi_bt_tpt_reconBt(TLKMDI_BTRECON_RETRY_NUM_POWERON);
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
void tlkmdi_bt_tpt_headset_recvPeerExitLowLatencyHandler(void *data, uint16_t dataLen)
{
    (void)data;
    (void)dataLen;

    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_headset_recvPeerExitLowLatencyHandler: cur_status %d", sTlkMdiBtTpsllTwsCtrl.cur_status);
    sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_RECON_STATUS_MSW_WAITING;
    sTlkMdiBtTpsllTwsCtrl.pair_mode  = TPT_HOST_HEADSET_SETUP_MODE_IDLE;

    if (!tlkmdi_bt_tpt_isSlave()) {
        tpsll_hci_sendDisconnCmd(TPT_HOST_HEADSET_SETUP_MODE_EXIT_ULTRA_LOW_LATENCY);
        tlkmdi_bt_tpt_reconBt(TLKMDI_BTRECON_RETRY_NUM_POWERON);
    }
}

/**
 * @brief       This function starts the dongle reconnection process.
 *              It checks if the paired dongle address is valid and if the device
 *              is not in slave mode, then sends access code and channel ID command
 *              and starts connection setup.
 * @return      none.
 * @note
 */
void tlkmdi_bt_tpt_dongle_reconStart(void)
{
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);
    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, null_addr1, BD_ADDR_LEN) == 0) {
        return;
    }

    if (!tlkmdi_bt_tpt_isSlave()) {
        tpsll_hci_sendWriteAccessCodeAndChnIDCmd(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
        tph_host_hal_start_connection_setup(TPT_HOST_DONGLE_SETUP_MODE_NORMAL);
    }
}

/**
 * @brief       Power on reconnection for TWS dongle.
 * @param[in]   none.
 * @return      TLK_ENONE - success.
 * @note
 */
int tlkmdi_bt_tpt_dongle_powerOnRecon(void)
{
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);

    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "powerOnReconDongle: dongle mac", sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, 6);
    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, null_addr1, BD_ADDR_LEN) == 0) {
        tpsll_hci_sendSetDongleMacExistedCmd(0);
    } else {
        tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "powerOnReconDongle: find paired device");
        tpsll_hci_sendSetGeneralMacCmd(sTlkMdiBtTpsllTwsCtrl.gen_ac_addr);
        tpsll_hci_sendSetDongleMacExistedCmd(1);
        tpsll_hci_sendSetDongleMacCmd(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr);
        tph_host_hal_set_setup_ac_chn(sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
        tph_host_hal_start_connection_setup(TPT_HOST_DONGLE_SETUP_MODE_NORMAL);
    }
    return TLK_ENONE;
}

/**
 * @brief       This function starts the pairing process between TWS master headset and dongle.
 * @param[in]   timeout    - pairing timeout value in units of 100ms.
 * @return      none.
 * @note
 */
void tlkmdi_bt_tpt_dongle_paringStart(uint16_t timeout) // unit:ms  timeout*100ms
{
    /*Tws master headset start pairing dongle  */
    tpsll_hci_sendSetDongleMacExistedCmd(0);
    tpsll_hci_sendWriteAccessCodeAndChnIDCmd(TPT_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPT_HOST_DONGLE_SETUP_COMMON_CHN);
    tpsll_hci_sendCreateConnectCmd(TPT_HOST_DONGLE_SETUP_MODE_PAIRING);

    sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing = 1;
    sTlkMdiBtTpsllTwsCtrl.timeout                      = timeout;
    tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer, 100 * 1000);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.timer);
}

/**
 * @brief       This function handles the update of dongle MAC address.
 * @param[in]   pData    - pointer to the new dongle MAC address data.
 * @param[in]   dataLen  - length of the data.
 * @return      TLK_ENONE if successful, -TLK_EPARAM if parameters invalid.
 * @note
 */
static int tlkmdi_bt_tpt_dongle_macUpdateHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t dongleAddr[6];
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);
    tmemcpy(dongleAddr, pData, 6);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "dongleMacUpdateHandler: update dongle mac", dongleAddr, 6);

    // sTlkMdiBtTpsllTwsCtrl.startPairing = false;

    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, dongleAddr, BD_ADDR_LEN) == 0) {
        return -TLK_EPARAM;
    }
    if (tmemcmp(dongleAddr, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(dongleAddr, null_addr1, BD_ADDR_LEN) == 0) {
        return -TLK_EPARAM;
    }
    tmemcpy(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr, dongleAddr, 6);
    tpsll_hci_sendSetDongleMacExistedCmd(true);
    tpsll_hci_sendSetDongleMacCmd(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr);
    /* save dongle mac to flash */
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "dongleMacUpdateHandler: new dongle!!! saving");
    tlkmdi_tinySql_setTpdMacAddr(sTlkMdiBtTpsllTwsCtrl.paired_dongle_addr); // todo, Choose the right time(power off, no tpsll music, phone) to save
    return TLK_ENONE;
}

/**
 * @brief       Handle the event when dongle connects successfully.
 * @param[in]   pData    - pointer to the data buffer.
 * @param[in]   dataLen  - length of the data.
 * @return      TLK_ENONE - success.
 * @note        This function processes the dongle connection event, updates internal state,
 *              clears pairing flags and sets connection status.
 */
static int tlkmdi_bt_tpt_dongle_connectHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    (void)pData;
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "##Dongle_CONNECTED");
    if (sTlkMdiBtTptStateChgCB != NULL) {
        sTlkMdiBtTptStateChgCB(TLKMDI_TPT_STATE_CHANGE_CB_CONNECT);
    }

    sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing = 0;
    sTlkMdiBtTpsllTwsCtrl.dongle_status                = TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED;
    /* 10s pairing mode clear ac,ch and pair a new dongle, if immediately 3s pairing and BT first connect
       when reconnect dongle the ac and ch used 0, reconnect dongle fail. */
    // tph_host_hal_get_ac_chn_from_mac(sTlkMdiBtTpsllTwsCtrl.local_addr, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.tpsll_ac, (uint8_t *)&sTlkMdiBtTpsllTwsCtrl.tpsll_ch);

    return TLK_ENONE;
}

/**
 * @brief       Handle the event when the dongle is disconnected
 * @param[in]   pData - Pointer to the data containing disconnect reason
 * @param[in]   dataLen - Length of the data
 * @return      TLK_ENONE - Always succeeds
 * @note        This function processes dongle disconnection events, updates internal states,
 *              and triggers appropriate actions based on the disconnection reason and current state
 */
static int tlkmdi_bt_tpt_dongle_disconnHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t reason;

    reason = pData[0];
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "##Dongle_DISCONNECTED, %x, %x", reason, sTlkMdiBtTpsllTwsCtrl.cur_status);

    sTlkMdiBtTpsllTwsCtrl.ll_audio_mode = 0;
    sTlkMdiBtTpsllTwsCtrl.dongle_status = TLKMDI_BT_TPT_CONNECT_STATUS_DISCONNECTED;

    /* Update current status based on disconnection reason when waiting for dongle disconnection */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCON_WAITING) {
        if (reason == TPT_HOST_DISCONNECT_REASON_ENTER_ULTRA_LOW_LATENCY_MODE) {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING;
        } else {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCONNECTED;
        }
    }
    if (sTlkMdiBtTptStateChgCB != NULL) {
        sTlkMdiBtTptStateChgCB(TLKMDI_TPT_STATE_CHANGE_CB_DISCONNECT);
    }
    //TODO: ZEWEN. need to deal power off and dfu status later.
    sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing = 0;

    /* Handle reconnection scenarios based on current status and disconnection reason */
    if (sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_PAIR_STATUS_IDLE || sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_TWS_CONNECTED ||
        sTlkMdiBtTpsllTwsCtrl.cur_status == TLKMDI_BT_TPT_RECON_STATUS_MSW_WAITING) {
        if (reason == TPT_HOST_DISCONNECT_REASON_EXIT_ULTRA_LOW_LATENCY_MODE) {
            sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_IDLE;
        }
        /* controller will clear status to idle, so need to set single again, otherwise can't reconn dongle */
        if (tlkmdi_bt_tpt_isSingle()) {
            tpsll_hci_sendSetHeadsetIsSingleCmd();
        }
        tlkmdi_bt_tpt_dongle_reconStart();
    }
    return TLK_ENONE;
}

/**
 * @brief       This function handles the master synchronization of profile information in TWS (True Wireless Stereo) mode.
 * @param[in]   pData    - pointer to the data buffer containing profile information.
 * @param[in]   dataLen  - length of the data in the buffer.
 * @return      TLK_ENONE - operation completed successfully.
 * @note
 */
int tlkmdi_bt_tpt_virtualLink_masterSyncProfileInfoHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint16_t       s_profile_info, m_profile_info;
    static uint8_t cnt = 0;

    s_profile_info = pData[6] | (pData[7] << 8);
    m_profile_info = pData[8] | (pData[9] << 8);

    tlkapi_printf(1, "[MBT_TWS]<info>masterSyncProfileInfoHandler: S-[0x%2x] M-[0x%2x] cnt-%d", s_profile_info, m_profile_info, ++cnt);

    if (tmemcmp(sTlkMdiBtTpsllTwsCtrl.btAddr, pData, 6) != 0) {
        tmemcpy(sTlkMdiBtTpsllTwsCtrl.btAddr, pData, 6);
    }
    sTlkMdiBtTpsllTwsCtrl.peerAclSetup = TLK_STATE_CONNECT;
    btp_tws_master_sync_profile_handler(pData, &s_profile_info, &m_profile_info);

    return TLK_ENONE;
}

/**
 * @brief       Handle slave profile information synchronization for TWS virtual link
 * @param[in]   pData - Pointer to the received data buffer
 * @param[in]   dataLen - Length of the data in the buffer
 * @return      TLK_ENONE - Always returns success
 * @note        This function processes profile information synchronization from the TWS slave device.
 *              It parses the data according to the profile type and routes it to appropriate handlers
 *              for A2DP, AVRCP, HFP, or SPP/GATT profiles. For HFP and SPP profiles, it also stores
 *              the RFCOMM channel IDs in the pairing database.
 */
int tlkmdi_bt_tpt_virtualLink_slaveProfileInfoSyncHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t        type;
    uint16_t       info;
    uint8_t        btAddr[6];
    static uint8_t cnt = 0;

    type = pData[0];
    info = pData[1] | (pData[2] << 8);
    tmemcpy(btAddr, pData + 3, 6);
    bttpsll_tws_sync_info_t *sync_info = (bttpsll_tws_sync_info_t *)&info;

    tlkapi_printf(1, "[MBT_TWS]<info>slaveProfileInfoSyncHandler: S-0x%x pType-%d cnt-%d", info, type, ++cnt);

    switch (type) {
    case BTTPSLL_TWS_SLAVE_BTP_A2DP_INFO:
    {
        bttpsll_tws_a2dpavrcp_info_t *a2dpInfo = (bttpsll_tws_a2dpavrcp_info_t *)(pData + 9);
        btp_tws_slave_a2dp_event_handler(sync_info, a2dpInfo);
    } break;
    case BTTPSLL_TWS_SLAVE_BTP_AVRCP_INFO:
    {
        bttpsll_tws_a2dpavrcp_info_t *avrcpInfo = (bttpsll_tws_a2dpavrcp_info_t *)(pData + 9);
        btp_tws_slave_avrcp_event_handler(sync_info, avrcpInfo);
    } break;
    case BTTPSLL_TWS_SLAVE_BTP_HFP_INFO:
    {
        bttpsll_tws_hfp_info_t *hfpInfo = (bttpsll_tws_hfp_info_t *)(pData + 9);
        btp_tws_slave_hfp_event_handler(sync_info, hfpInfo);
        /* Store HFP RFCOMM channel ID for paired device */
        if (hfpInfo->hfp_channel != 0) {
            tlkmdi_tinySql_setPairingDeviceRfcChid(btAddr, hfpInfo->hfp_channel, TLKMDI_BT_RFC_CHID_HFP);
        }
    } break;
    case BTTPSLL_TWS_SLAVE_BTP_SPPGATT_INFO:
    {
        bttpsll_tws_sppgatt_info_t *sppgattInfo = (bttpsll_tws_sppgatt_info_t *)(pData + 9);
        btp_tws_slave_sppgatt_event_handler(sync_info, sppgattInfo);
        /* Store SPP RFCOMM channel ID for paired device */
        if (sppgattInfo->spp_channel != 0) {
            tlkmdi_tinySql_setPairingDeviceRfcChid(btAddr, sppgattInfo->spp_channel, TLKMDI_BT_RFC_CHID_SPP);
        }
    } break;
    default:
        break;
    }

    return TLK_ENONE;
}

/**
 * @brief       This function handles the completion of SCO setup for slave virtual link in TWS mode.
 * @param[in]   pData    - pointer to the data containing air mode and connection information.
 * @param[in]   dataLen  - length of the data in bytes.
 * @return      TLK_ENONE if successful, -TLK_ENOSUPPORT if device is master, -TLK_EEXIST if HFP is already connected.
 * @note        Determines HFP codec based on air mode and sets it accordingly.
 */
int tlkmdi_bt_tpt_virtualLink_slaveScoSetupCompleteHandler(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t airMode;
    uint8_t hfp_codec;

    airMode = pData[0];
    if (tlkmdi_bt_tpt_isMaster()) {
        return -TLK_ENOSUPPORT;
    }

    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_searchConnItem(pData + 1);
    if (pItem != NULL) {
        if ((pItem->connFlag & BTP_PFLAG_HFP) != 0) {
            return -TLK_EEXIST;
        }
    } else {
        tlk_printf("<bt_tws> [info] tws_slaveScoSetupCompleteHandler: Sco setup before HFP connected");
    }
    if (airMode == BTH_SCO_AIRMODE_TRANS) {
        hfp_codec = HFP_CODEC_ID_MSBC;
    } else {
        hfp_codec = HFP_CODEC_ID_CVSD;
    }
    btif_set_hfp_codec(SCO_ENC_QUEUE_ID_HF, hfp_codec);
    tlk_printf("<bt_tws> [info] tws_slaveScoSetupCompleteHandler: airMode-%d, hfp_codec-%d", airMode, hfp_codec);

    return TLK_ENONE;
}

/**
 * @brief       This function initiates the handover process for TWS (True Wireless Stereo) devices.
 *              Depending on the device role (master or slave), it either starts the handover command
 *              or requests a handover from the master device.
 * @return      none.
 * @note
 */
void tlkmdi_bt_tpt_handover_start()
{
    tlk_printf("tlkmdi_bt_tpt_handover_start: cur_role[%d] %d", sTlkMdiBtTpsllTwsCtrl.cur_role);
    sTlkMdiBtTpsllTwsCtrl.handover_status = BTH_TPT_TWS_HANDOVER_STATUS_START;
    if (tlkmdi_bt_tpt_isMaster()) {
        if (tlkmdi_bt_tpt_pageSuccessAndConnecting(&sTlkMdiBtTpsllTwsCtrl.busyHandleWhenHo)) {
            tlk_printf("tlkmdi_bt_tpt_handover_start: ACL-0x%x is being created, pending...", sTlkMdiBtTpsllTwsCtrl.busyHandleWhenHo);
            sTlkMdiBtTpsllTwsCtrl.timeout = 300;
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.handoverTimer);
        } else {
            tpsll_hci_sendStartHandoverCmd();
        }
    } else if (tlkmdi_bt_tpt_isSlave()) {
        tlkmdi_bt_tpt_reqHandOver();
    } else {
        tlk_printf("tlkmdi_bt_tpt_handover_start cur_role error");
    }
}

/**
 * @brief       Handle the extraction of host information during handover process
 *              This function sets the handover status to EXTRACER, copies the Bluetooth
 *              address from input data to control structure, and triggers a TPSLL event
 *              in the HOST task
 * @param[in]   pData: pointer to the data containing Bluetooth address information
 *                      expected to be 6 bytes long
 */
_attribute_ram_code_sec_ void tlkmdi_bt_tpt_handover_extraceHostInfoHandler(uint8_t *pData)
{
    if (pData == NULL) {
        return;
    }

    sTlkMdiBtTpsllTwsCtrl.handover_status = BTH_TPT_TWS_HANDOVER_STATUS_EXTRACER;
    tmemcpy(sTlkMdiBtTpsllTwsCtrl.btHoAddr, pData, 6);
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_TPSLL);

    return;
}

/**
 * @brief       When handover, the master headset collects some states and parameters from app
 * @param[in]   aclHandle - the acl handle
 * @param[out]  pInfo - Synchronize the app parameters for the future new master headset
 * @return      none
 * @note
 */
void tlkmdi_bt_tpt_handover_getAppInfo(uint16_t aclHandle, btp_tws_get_bt_app_state_info_t *pInfo)
{
    if (tlkmdi_bt_tpt_isSlave()) {
        return;
    }
    uint8_t audioMode = 0;
    if (aclHandle == 0 || pInfo == NULL) {
        tlk_printf("tlkmdi_bt_tpt_handover_getAppInfo: error param aclHandle[0x%x] pInfo[0x%x]", aclHandle, pInfo);
    }
    tlkmdi_hfphf_ctrl_t *pCtrl = tlkmdi_hfphf_getItem(aclHandle);

    pInfo->is_bt_recon       = tlkmdi_btRecon_isInBusy();
    pInfo->scanState         = tlkmdi_btscan_getCurScanState();
    pInfo->scanTimeout       = tlkmdi_btscan_getRemainedScanTime();
    pInfo->reconRetryNum     = tlkmdi_bt_recon_getRemindRetryNum();
    pInfo->isDongleParing    = sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing;
    pInfo->donglePairTimeout = sTlkMdiBtTpsllTwsCtrl.timeout;
    if (pCtrl != NULL) {
        pInfo->status       = (pCtrl->status & 0x0F);
        pInfo->status_setup = (pCtrl->setup_status & 0x0F);
        pInfo->status_held  = (pCtrl->held_status & 0x0F);
    }
    audioMode = app_tph_headset_get_mode();
    if (audioMode & TPT_HOST_MODE_DONGLE_ACTIVE) {
        if (audioMode & TPT_HOST_MODE_DONGLE_PHONE) {
            pInfo->dongleVoice = 1;
        }
    }
    if (pInfo->is_bt_recon) {
        tlkmdi_btRecon_close();
    }
}

uint8_t bt_tpt_handover_success = 0;

/**
 * @brief       Handle TWS handover success event
 * @param[in]   pData    - Pointer to the data containing handover information,
 *                         with the first byte representing the new role
 * @param[in]   dataLen  - Length of the data in bytes
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function processes the handover success event, updates the device role,
 *              handles specific actions based on the new role (master/slave/single),
 *              and manages reconnection of Bluetooth and dongle connections as needed.
 */
static int tlkmdi_bt_tpt_handover_success(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    tlk_printf("tlkmdi_bt_tpt_handover_success: ###TWS_HANDOVER ROLE[%d]", pData[0]);
    sTlkMdiBtTpsllTwsCtrl.cur_role = pData[0];

    sTlkMdiBtTpsllTwsCtrl.handover_status = BTH_TPT_TWS_HANDOVER_STATUS_IDLE;

    bt_tpt_handover_success = 1;

    if (tlkmdi_bt_tpt_isSlave()) {
        /*Exit dongle setup*/
        tpsll_hci_sendExitDongleConnSetupCmd();
        /*CLose bt*/
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 120);
    }

    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
        /*TODO: send local battery*/

        if (tlkmdi_btacl_getConnCount() == 0) {
            /*Recon bt*/
            if (sTlkMdiBtTpsllTwsCtrl.handover_info.is_bt_recon) {
                tlkmdi_bt_tpt_reconBt(sTlkMdiBtTpsllTwsCtrl.handover_info.reconRetryNum + 1);
            } else {
                tlkmdi_btSet_scan(sTlkMdiBtTpsllTwsCtrl.handover_info.scanState, sTlkMdiBtTpsllTwsCtrl.handover_info.scanTimeout);
            }
        } else {
            if (sTlkMdiBtTpsllTwsCtrl.handover_info.status) {
                tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_HO_MIC);
            }
        }

        /*If dongle is idle, reconn dongle*/
        if (sTlkMdiBtTpsllTwsCtrl.dongle_status != TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED) {
            if (sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing) {
                sTlkMdiBtTpsllTwsCtrl.cur_status = TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING;
                tlkmdi_bt_tpt_dongle_paringStart(sTlkMdiBtTpsllTwsCtrl.handover_info.donglePairTimeout);
            } else {
                tlkmdi_bt_tpt_dongle_reconStart();
            }
        } else {
            if (sTlkMdiBtTpsllTwsCtrl.handover_info.dongleVoice) {
                tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_HO_MIC);
            }
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       Check if the TWS handover process is currently busy
 * @return      Returns true if handover is in progress and device is not busy, otherwise returns false
 * @note        This function checks if a handover operation is active (handover_status is non-zero)
 *              and the device is not marked as busy during handover (isBusyWhenHo is false).
 *              It is typically used to determine if handover operations can proceed.
 */
uint8_t tlkmdi_bt_tpt_handover_isBusy()
{
    return sTlkMdiBtTpsllTwsCtrl.handover_status && !sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo;
}

/**
 * @brief       When handover, the slave headset receives the HOST sync info sent by the master headset
 *              and makes this info effective at the upper layer of slave headset.
 * @param[in]   pData - refoer to "bttpsll_tws_handover_host_status_info_t"
 * @param[in]   dataLen - the "pData" length
 * @return      none
 * @note
 */
static int tlkmdi_bt_tpt_handover_slaveInfoSync(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_array(0xffffffff, "[BT_TPT]", "tlkmdi_bt_tpt_handover_slaveInfoSync", pData, 16);
    tlkapi_array(0xffffffff, "[BT_TPT]", "tlkmdi_bt_tpt_handover_slaveInfoSync", pData + 16, dataLen - 16);

    uint8_t                                  addr[6];
    bttpsll_tws_handover_host_status_info_t *pInfo;
    tmemcpy(addr, pData, 6);

    sTlkMdiBtTpsllTwsCtrl.handover_status = BTH_TPT_TWS_HANDOVER_STATUS_HOSTSYNC;

    pInfo                                                 = (bttpsll_tws_handover_host_status_info_t *)(pData + 6);
    sTlkMdiBtTpsllTwsCtrl.handover_info.status            = pInfo->hf_state;
    sTlkMdiBtTpsllTwsCtrl.handover_info.status_held       = pInfo->hf_statusCallHeld;
    sTlkMdiBtTpsllTwsCtrl.handover_info.status_setup      = pInfo->hf_statusCallSetup;
    sTlkMdiBtTpsllTwsCtrl.handover_info.is_bt_recon       = pInfo->is_bt_auto_recon;
    sTlkMdiBtTpsllTwsCtrl.handover_info.scanState         = pInfo->scanState;
    sTlkMdiBtTpsllTwsCtrl.handover_info.scanTimeout       = pInfo->scanTimeout;
    sTlkMdiBtTpsllTwsCtrl.handover_info.dongleVoice       = pInfo->dongleVoice;
    sTlkMdiBtTpsllTwsCtrl.handover_info.reconRetryNum     = pInfo->reconRetryNum;
    sTlkMdiBtTpsllTwsCtrl.handover_info.isDongleParing    = pInfo->isDongleParing;
    sTlkMdiBtTpsllTwsCtrl.handover_info.donglePairTimeout = pInfo->donglePairTimeout;
    btp_tws_slave_handover_host_status_sync(addr, pInfo);
    return TLK_ENONE;
}

/**
 * @brief       TWS handover timer callback function
 * @param[in]   pTimer   - Timer handle
 * @param[in]   userArg  - User argument passed to timer
 * @return      none
 * @note        This function handles the TWS (True Wireless Stereo) handover process timing.
 *              It checks various conditions to determine when to initiate the handover:
 *              1. If all required profiles are connected, starts handover immediately
 *              2. If slave ACL and master profiles are both established, starts handover
 *              3. If timeout occurs but some profiles are connected, starts handover with warning
 *              4. Otherwise, continues waiting until timeout
 */
static void tlkmdi_bt_tpt_handover_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    (void)pTimer;

    uint8_t sAclState  = 0;
    uint8_t mProfState = 0;

    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(sTlkMdiBtTpsllTwsCtrl.busyHandleWhenHo);

    if (sTlkMdiBtTpsllTwsCtrl.timeout == 0) {
        tlkapi_trace(0xffffffff, "[TWS]", "tlkmdi_bt_tpt_handover_timer: timeout, connFlag-0x%x", pItem->connFlag);
    }

    /* page success process when handover start */
    if ((pItem->connFlag & (uint16_t)(~TLKMDI_BT_TPT_TWS_WITHOUT_TWS_MASK)) == BTP_PFLAG_TWS_DEF_PROF_MASK) {
        tlk_printf("tlkmdi_bt_tpt_handover_timer: ACL creation is completed, start handover!!!");
        sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo = 0;
        tpsll_hci_sendStartHandoverCmd();
        return;
    }

    if (sTlkMdiBtTpsllTwsCtrl.peerAclSetup == TLK_STATE_CONNECT) {
        tlk_printf("tlkmdi_bt_tpt_handover_timer: Slave ACL creation is completed");
        sAclState = true;
    }
    if ((pItem->connFlag & (uint16_t)(~TLKMDI_BT_TPT_TWS_WITHOUT_TWS_MASK)) == BTP_PFLAG_TWS_DEF_PROF_MASK) {
        mProfState = true;
    }
    if (mProfState && sAclState) {
        tlk_printf("tlkmdi_bt_tpt_handover_timer: M Profile and S ACL creation is completed, start handover!!!");
        sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo = 0;
        sTlkMdiBtTpsllTwsCtrl.peerAclSetup = TLK_STATE_CLOSED;
        tpsll_hci_sendStartHandoverCmd();
        return;
    }

    if (sTlkMdiBtTpsllTwsCtrl.timeout > 0) {
        sTlkMdiBtTpsllTwsCtrl.timeout--;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
        return;
    }

    /*3s timeout. if some profiles connected, such as only A2DP and AVRCP connected, HFP doesnot*/
    if (sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo) {
        if ((pItem->connFlag & (BTP_PFLAG_A2DP | BTP_PFLAG_AVRCP)) != 0 || (pItem->connFlag & BTP_PFLAG_HFP) != 0) {
            sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo = 0;
            tlk_printf("[warning]tlkmdi_bt_tpt_handover_timer: Only some profiles-0x%x are connected", pItem->connFlag);
            tpsll_hci_sendStartHandoverCmd();
        }
    }
}

/**
 * @brief       Set the handover mask to indicate a handover operation is pending
 * @param[in]   pData - Pointer to data (contains btclock information)
 * @return      none
 * @note        This function sets the handover pending flag in the TWS control structure
 *              to indicate that a handover operation is in progress. It is typically called
 *              when a true wireless stereo handover event occurs.
 */
_attribute_ram_code_sec_ void tlkmdi_bt_tpt_handover_setMask(uint8_t *pData)
{
    (void)pData; // the pData is btclock
    // gpio_set_high_level(GPIO_PA0);
    sTlkMdiBtTpsllTwsCtrl.hoPending = true;
}

/**
 * @brief       Clear the handover mask to indicate a handover operation is pending
 * @param[in]   pData - Pointer to data (contains btclock information)
 * @return      none
 * @note        This function clears the handover pending flag in the TWS control structure
 *              to indicate that a handover operation is in progress. It is typically called
 *              when a true wireless stereo handover event occurs.
 */
_attribute_ram_code_sec_ void tlkmdi_bt_tpt_handover_clearMask(uint8_t *pData)
{
    (void)pData;
    // gpio_set_low_level(GPIO_PA0);
    sTlkMdiBtTpsllTwsCtrl.hoPending = false;
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_HCI);
}

/**
 * @brief       This function checks if there is a successful page and connecting status for TWS handover.
 * @param[out]  pHandle    - pointer to store the handle of the connected ACL link.
 * @return      true if there is a valid ACL link and handover can proceed, false otherwise.
 * @note
 */
static bool tlkmdi_bt_tpt_pageSuccessAndConnecting(uint16_t *pHandle)
{
    tlkmdi_btacl_item_t *pItem[TLK_STK_BTACL_NUMB] = {0};

    if (sTlkMdiBtTpsllTwsCtrl.handover_status != BTH_TPT_TWS_HANDOVER_STATUS_START) {
        return false;
    }

    /* check bt valid acl link */
    for (int iIndex = 0; iIndex < TLK_STK_BTACL_NUMB; iIndex++) {
        pItem[iIndex] = tlkmdi_btacl_getConnItemByIndex(iIndex);
        if (pItem[iIndex] != NULL) {
            sTlkMdiBtTpsllTwsCtrl.isBusyWhenHo = 1;
            *pHandle                           = pItem[iIndex]->handle;
            return true;
        }
    }

    return false;
}

/**
 * @brief       Timer callback function for handling TPT power off operations
 * @param[in]   pTimer   - Timer handle that triggered this callback
 * @param[in]   userArg  - User argument passed to timer (unused)
 * @return      None
 * @note        This function checks if the device is in shutdown state and if so,
 *              disables the BT_TPT power management channel. It's called when the
 *              TPT power off timer expires.
 */
static void tlkmdi_bt_tpt_powerOffTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    (void)pTimer;
    if (sTlkMdiBtTpsllTwsCtrl.is_shutdown) {
        tlksys_pm_setChn(TLKSYS_PM_CHN_BT_TPT, 0, 0);
    }
}

/**
 * @brief       check bt status.
 * @param[in]   isClose - true: check BT status, if recon or pair is busy or connected close and disconn BT.
 *                        false: only check bt status.
 * @return      when true return disconn result, when false return BT status.
 * @note
 */
static bool tlkmdi_bt_tpt_check_terminate_bt(bool isClose)
{
    bool    isRecBusy     = tlkmdi_btRecon_isInBusy();
    bool    isPairBusy    = tlkmdi_btPairing_isInProgress();
    uint8_t bt_link_count = tlkmdi_btacl_getUsedCount();

    if (isClose) {
        if (isRecBusy) {
            tlkmdi_btRecon_close();
        }

        tlkmdi_btParing_stop();

        tlkmdi_btacl_item_t *pItem;
        for (uint8_t i = 0; i < TLKMDI_BTACL_ITEM_NUMB; i++) {
            pItem = tlkmdi_btacl_getConnItemByIndex(i);
            if (pItem != NULL) {
                tlkapi_array(0xffffffff, "[TWS]", "tlkmdi_bt_tpt_check_terminate_bt: discon", pItem->btaddr, 6);
                if (pItem->handle == 0) {
                    return tlkmdi_btacl_cancel(pItem->btaddr);
                } else {
                    return tlkmdi_btacl_disconn(pItem->handle, 0x13);
                }
            }
        }
    }
    return (isRecBusy || isPairBusy || bt_link_count > 0);
}

/**
 * @brief       Check if the current device is acting as Master or Arbiter in TWS connection
 * @return      true - Device is currently Master or Arbiter
 *              false - Device is not Master or Arbiter
 * @note        In TWS (True Wireless Stereo) systems, both Master and Arbiter roles
 *              are considered as master-like roles in terms of control and coordination
 */
bool tlkmdi_bt_tpt_isMaster()
{
    if (sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_MASTER || sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_ARBITER) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       Check if the current device is acting as slave role
 * @return      true if device is in slave or observer role, false otherwise
 * @note        This function checks if the current TWS role is either slave or observer,
 *              which are considered as slave roles in the TWS (True Wireless Stereo) system
 */
bool tlkmdi_bt_tpt_isSlave()
{
    if (sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_SLAVE || sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_OBSERVER) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       Check if the current TWS role is single mode
 * @return      true if current role is single mode, false otherwise
 * @note        This function checks whether the TWS is operating in
 *              single mode, which means only one device is active without a paired TWS partner.
 */
bool tlkmdi_bt_tpt_isSingle()
{
    return (sTlkMdiBtTpsllTwsCtrl.cur_role == BTH_TPT_TWS_ROLE_SINGLE) ? true : false;
}

/**
 * @brief       This function checks if the TWS connection is established.
 * @return      bool - TRUE if connected, FALSE if not connected.
 * @note        Connection is determined by checking that the current role is neither IDLE nor SINGLE.
 */
bool tlkmdi_bt_tpt_isConnected()
{
    return (sTlkMdiBtTpsllTwsCtrl.cur_role != BTH_TPT_TWS_ROLE_IDLE && sTlkMdiBtTpsllTwsCtrl.cur_role != BTH_TPT_TWS_ROLE_SINGLE);
}

#if !TLK_DEV_SY5500_ENABLE
/**
 * @brief       Check if the current device is the left earbud of a TWS pair
 * @return      true if the device is the left earbud, false otherwise
 * @note        This function determines the earbud side (left or right) based on USB ID.
 *              A USB ID of 0X120 indicates the left earbud.
 */
bool tlkmdi_bt_tpt_isLeft()
{
    uint16_t usbID = tlkmdi_tinySql_getUsbID();
    return usbID == 0X120;
}
#endif

/**
 * @brief       Get the current TWS role of the device
 * @return      Current TWS role value
 *              Possible values:
 *              - BTH_TPT_TWS_ROLE_IDLE: No TWS connection
 *              - BTH_TPT_TWS_ROLE_SINGLE: Single device mode (no TWS partner)
 *              - BTH_TPT_TWS_ROLE_MASTER: Master role in TWS connection
 *              - BTH_TPT_TWS_ROLE_ARBITER: Arbiter role in TWS connection
 *              - BTH_TPT_TWS_ROLE_SLAVE: Slave role in TWS connection
 *              - BTH_TPT_TWS_ROLE_OBSERVER: Observer role in TWS connection
 * @note        This function returns the current TWS (True Wireless Stereo) role
 *              stored in the global control structure. The role determines the
 *              device's behavior in a TWS system.
 */
uint8_t tlkmdi_bt_tpt_getRole()
{
    return sTlkMdiBtTpsllTwsCtrl.cur_role;
}

/**
 * @brief       Generate a general MAC address based on the device side (left or right)
 * @param[out]  out - Pointer to the output buffer where generated MAC address will be stored
 * @param[in]   localMac - Pointer to the local device MAC address
 * @param[in]   peerMac - Pointer to the peer device MAC address
 * @return      none
 * @note        For left device: uses first 3 bytes of local MAC and last 3 bytes of peer MAC
 *              For right device: uses first 3 bytes of peer MAC and last 3 bytes of local MAC
 */
static void tlkmdi_bt_tpt_wired_getGenMac(uint8_t *out, uint8_t *localMac, uint8_t *peerMac)
{
    if (tlkmdi_bt_tpt_isLeft()) {
        tmemcpy(out, localMac, 3);
        tmemcpy(out + 3, peerMac + 3, 3);
    } else {
        tmemcpy(out, peerMac, 3);
        tmemcpy(out + 3, localMac + 3, 3);
    }
}

/**
 * @brief       Register callback function for TWS state change notifications
 * @param[in]   cb - Callback function to be invoked when TWS state changes
 * @return      none.
 * @note        none.
 */
void tlkmdi_bt_tpt_regStateChangeCB(tlkmdi_tpt_state_change_cb cb)
{
    sTlkMdiBtTptStateChgCB = cb;
}

/**
 * @brief       Shut down the TWS device
 * @param[in]   none.
 * @return      none.
 * @note        This function handles the shutdown process for TWS devices,
 *              sending disconnection command if device is slave and peer is out of case,
 *              otherwise setting power management channel accordingly.
 */
void tlkmdi_bt_tpt_shut_down(void)
{
    /* Mark the device as shutting down */
    sTlkMdiBtTpsllTwsCtrl.is_shutdown = 1;

    /* If device is slave and peer device is not in case, send disconnect command */
    if (tlkmdi_bt_tpt_isSlave()) {
        if (tlkmdi_bt_tpt_getPeerCaseState().isLeavebox != TLKMDI_BT_TPT_STATE_FALSE) {
            tpsll_hci_sendDisconnCmd(TPT_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN);
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtTpsllTwsCtrl.powerOffTimer);
            return;
        }
    }

    /* Set power management channel for BT TPT */
    tlksys_pm_setChn(TLKSYS_PM_CHN_BT_TPT, 0, 0);
}

/**
 * @brief       This function gets the current audio channel of the TWS device.
 * @param[in]   none.
 * @param[out]  none.
 * @return      uint8_t - the current audio channel, ALG_CHANNEL_LEFT or ALG_CHANNEL_RIGHT.
 * @note
 */
uint8_t tlkmdi_bt_tpt_audio_getCurChannel(void)
{
    uint8_t channel = 0;

    if (tlkmdi_bt_tpt_isLeft()) {
        channel = ALG_CHANNEL_LEFT;
    } else {
        channel = ALG_CHANNEL_RIGHT;
    }

    return channel;
}

/**
 * @brief       This function extracts mono PCM data from stereo PCM data according to the specified channel.
 * @param[in]   pcm_stereo    - the start address of the stereo PCM data buffer.
 * @param[out]  pcm_mono      - the start address of the mono PCM data buffer.
 * @param[in]   length        - the length of PCM data to process.
 * @param[in]   channel       - the audio channel to extract (ALG_CHANNEL_LEFT, ALG_CHANNEL_RIGHT or ALG_CHANNEL_STEREO).
 * @return      1 if successfully extracted mono data from stereo, 0 if stereo channel is selected.
 * @note
 */
uint8_t tlkmdi_bt_tpt_audio_getMonoPcmData(codec_mono_int *pcm_stereo, codec_mono_int *pcm_mono, uint16_t length, uint8_t channel)
{
    switch (channel) {
    case ALG_CHANNEL_LEFT:
    {
        for (int i = 0; i < length; i++) {
            pcm_mono[i] = pcm_stereo[2 * i];
        }
        return 1;
    } break;
    case ALG_CHANNEL_RIGHT:
    {
        for (int i = 0; i < length; i++) {
            pcm_mono[i] = pcm_stereo[2 * i + 1];
        }
        return 1;
    } break;
    case ALG_CHANNEL_STEREO:
    default:
    {
        return 0;
    } break;
    }

    return 1;
}

/**
 * @brief       Print debug information about various Bluetooth addresses and channel information via trace output
 * @param       none
 * @return      none
 * @note        This function prints local address, peer address, generated address,
 *              new allocated address, current address pointer, AC channel and BLE channel info,
 *              and the local Bluetooth device name for debugging purposes.
 */
void tlkmdi_bt_tpt_debug_printf_addr_info(void)
{
    uint8_t nameBuffer[TINYSQL_BT_NAME_LENS];
    tlkmdi_tinySql_getBtName(nameBuffer);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "@Local addr:", sTlkMdiBtTpsllTwsCtrl.const_addr, 6);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "peer_addr:", sTlkMdiBtTpsllTwsCtrl.peer_addr, 6);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "gen_addr:", sTlkMdiBtTpsllTwsCtrl.gen_ac_addr, 6);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "new_addr:", sTlkMdiBtTpsllTwsCtrl.new_alloc_addr, 6);
    tlkapi_array(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "p_cur_addr:", sTlkMdiBtTpsllTwsCtrl.p_cur_addr, 6);
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "ac_ch: 0x%x,0x%x", sTlkMdiBtTpsllTwsCtrl.ble_ac, sTlkMdiBtTpsllTwsCtrl.ble_ch);
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "@Local Name:%s", nameBuffer);
}

/**
 * @brief       Handle TPSLL-related TWS operations by extracting handover information
 * @return      none
 * @note        This function extracts the handover information and stores it in the
 *              btHoAddr buffer of the global control structure
 */
static void tlkmdi_bt_tpt_tpsllHandler(void)
{
    btp_handover_info_extract(sTlkMdiBtTpsllTwsCtrl.btHoAddr);
}

/**
 * @brief       This event will trigger within 40ms, before audio path actually switch to 5ms sequence.
 *              when in dongle music, this event will trigger per 5ms.
 * @return      none
 * @note
 */
static void tlkmdi_bt_tpt_latencyModeChangeHandler(void)
{
    if (sTlkMdiBtTpsllTwsCtrl.pair_mode == TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY && sTlkMdiBtTpsllTwsCtrl.ll_audio_mode == 0) {
        tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "tlkmdi_bt_tpt_latencyModeChangeHandler: urtal low latency mode");
        sTlkMdiBtTpsllTwsCtrl.ll_audio_mode = 1;
    }
}

/**
 * @brief       This event is used to notify the slave of the current A2DP status of the music
 *              when exiting the BT sniff mode.
 * @return      none
 * @note
 */
static void tlkmdi_bt_tpt_musicStatusNotifyHandler(void)
{
    if (tlkmdi_bt_tpt_isMaster()) {
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItemByIndex(0);
        if (pItem != NULL) {
            tlkmdi_bt_tpt_syncA2dpStatusDataHandler(pItem->handle, NULL, 0);
        }
    }
}

/**
 * @brief       Check if there is a pending handover operation
 * @return      true if handover is pending, false otherwise
 * @note        This function checks the internal control structure's hoPending flag
 *              which indicates whether a handover operation is currently pending
 */
bool tlkmw_host_isC2hPending(void)
{
    return sTlkMdiBtTpsllTwsCtrl.hoPending;
}

#endif
