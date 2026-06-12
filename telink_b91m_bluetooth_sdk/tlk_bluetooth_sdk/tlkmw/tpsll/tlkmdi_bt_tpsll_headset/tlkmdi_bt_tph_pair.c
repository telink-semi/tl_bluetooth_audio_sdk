/********************************************************************************************************
 * @file    tlkmdi_bt_tph_pair.c
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
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/tpsll.h"
#include "tlkmdi_bt_tph_inner.h"
#include "core/sdk_version.h"


#define TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTACL_DBG_SIGN            "[TPSLL_PAIR]"

/**
 * @brief       Sync version to dongle.
 * @return      None
 */
__attribute__((weak)) void tlkmdi_tph_version_sync(void)
{
    uint8_t  pData[8]    = {0};
    uint16_t dataLen     = 0;
    uint32_t sdk_version = 0;
    sdk_version |= MINOR_VERSION & 0xFF;
    sdk_version |= (MAJOR_VERSION & 0xFF) << 8;
    sdk_version |= (SOFT_STRUCTURE & 0xFF) << 16;
    sdk_version |= (CERTIFICATION_MARK & 0xFF) << 24;

    pData[dataLen++] = TLK_MDI_APP_VERSION_SYNC;
    UINT32L_TO_ARRAY(sdk_version, pData, dataLen); //version
    dataLen += 4;

    tph_host_hal_send_pdu_msg(TPH_HOST_MSG_PDU_ACL_CMD_APP, pData, dataLen, NULL);
}

/**
 * @brief       Disconnects the Bluetooth ACL connection.
 * @return      -TLK_EFAIL if no item is found, -TLK_ENOSUPPORT if Bluetooth is not enabled. 
 *              Other value is handler result.
 * @note    
 */
static int tlkmdi_bt_tph_disconnBtAcl(void)
{
#if (TLK_STK_BT_ENABLE)
    tlkmdi_btacl_item_t *pItem;
    for (uint8_t i = 0; i < TLKMDI_BTACL_ITEM_NUMB; i++) {
        pItem = tlkmdi_btacl_getConnItemByIndex(i);
        if (pItem != NULL) {
            tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_disconnBtAcl: discon btaddr", pItem->btaddr, 6);
            if (pItem->handle == 0) {
                return tlkmdi_btacl_cancel(pItem->btaddr);
            } else {
                return tlkmdi_btacl_disconn(pItem->handle, 0x13);
            }
        }
    }

    return -TLK_EFAIL;
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Handles the timer event for Bluetooth pairing state machine.
 * @param[in]   pTimer    - the timer handle.
 * @param[in]   userArg   - user-defined argument (unused).
 * @return      none.
 * @note    
 */
static void tlkmdi_bt_tph_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    (void)pTimer;

    if (gTlkMdiHeadsetCtrl.timeout == 0xFFFF) {
        return;
    }
    if (gTlkMdiHeadsetCtrl.timeout % 5 == 0) {
        tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_timer: timeout %d, cur_status %d", gTlkMdiHeadsetCtrl.timeout,
                     gTlkMdiHeadsetCtrl.cur_status);
    }

    if (gTlkMdiHeadsetCtrl.timeout > 0) {
        gTlkMdiHeadsetCtrl.timeout--;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
        return;
    }
    //following code: timeout handler:
    if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_PAIRING_CONNECT_WAITING) {
        tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_timer: tpsll pairing timeout, poweroff system now!");
        if (gInnerTlkMdiBtTphStateChgCB != NULL) { /*only for update led patter as idle*/
            gInnerTlkMdiBtTphStateChgCB(TLKMDI_TPSLL_STATE_CHANGE_CB_DISCONNECT);
        }
        /* After 2 mins(if 3s pairing), there are no new dongle pairs, restore the pairing info of the original dongle*/
        tph_host_hal_start_disconn(gTlkMdiHeadsetCtrl.disReason);
    }

    if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_PAIRING_ASYNC_DISCON_WAITING) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_CRASH;
        tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_timer: Dongle Disconn Timeout,CRASH!");
    }

    if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_NO_INIT) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_CRASH;
    }

    if (gTlkMdiHeadsetCtrl.cur_status == TLKMDI_TPSLL_WAIT_VERSION_SYNC) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_CRASH;
    }
}

/**
 * @brief       Handles the asynchronous disconnect state in the pairing state machine.
 * @return      true.
 * @note        psm:pairing state machine
 */
static bool tlkmdi_bt_tph_nowPSM_asyncDiscon(void)
{
    uint8_t bt_link_count = tlkmdi_btacl_getUsedCount();
    bool    bt_recon_busy = tlkmdi_btRecon_isInBusy();
    if (bt_link_count || bt_recon_busy) {
        /*need add cancel btreconnect process */
        if (bt_link_count) {
            tlkmdi_bt_tph_disconnBtAcl();
        }
        if (bt_recon_busy) {
            tlkmdi_btRecon_close();
        }
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_BT_DSICON_WAITING;
    } else {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_BT_DISCONNECTED;
    }
    return true;
}

/**
 * @brief       Waits for Bluetooth disconnection in the pairing state machine.
 * @return      true if disconnection is confirmed, false otherwise.
 * @note        psm:pairing state machine
 */
static bool tlkmdi_bt_tph_nowPSM_waitBtDiscon(void)
{
    uint8_t bt_link_count = tlkmdi_btacl_getUsedCount();
    bool    bt_recon_busy = tlkmdi_btRecon_isInBusy();
    if (bt_recon_busy == false && bt_link_count == 0) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_BT_DISCONNECTED;
        return true;
    }
    return false;
}

/**
 * @brief       Starts the Bluetooth disconnection process in the pairing state machine.
 * @return      true.
 * @note        psm:pairing state machine
 */
static bool tlkmdi_bt_tph_nowPSM_btDiscon(void)
{
    /* pairing mode, avoid pairing LED effect being overridden by disconnect LED */
    tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_nowPSM_btDiscon: no acl, start tpsll paring 0x%x cur_status %d",
                 gTlkMdiHeadsetCtrl.startPairing, gTlkMdiHeadsetCtrl.cur_status);

    if (gTlkMdiHeadsetCtrl.disReason == TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_IDLE;
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        tph_host_hal_get_ac_chn_from_mac(gTlkMdiHeadsetCtrl.local_addr, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ac, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ch);
        tph_host_hal_set_setup_ac_chn(gTlkMdiHeadsetCtrl.tpsll_ac, gTlkMdiHeadsetCtrl.tpsll_ch);
        tph_host_hal_start_connection_setup(TPH_HOST_DONGLE_SETUP_MODE_ULTRA_LOW_LATENCY);
        return false;
    }

    /* normal -> low latency mode not play paring tone. */
    if (gTlkMdiHeadsetCtrl.startPairing && gInnerTlkMdiBtTphStateChgCB != NULL) {
        gInnerTlkMdiBtTphStateChgCB(TLKMDI_TPSLL_STATE_CHANGE_CB_PAIR);
    }

    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, TLK_MDI_TPSLL_TPH_PAIRING_TIMEOUT);

    tph_host_hal_set_dongle_mac_existed(0);
    tph_host_hal_set_gen_mac(gTlkMdiHeadsetCtrl.local_addr);
    tph_host_hal_set_setup_ac_chn(TPH_HOST_DONGLE_SETUP_COMMON_ACCESSCODE, TPH_HOST_DONGLE_SETUP_COMMON_CHN);
    tph_host_hal_start_connection_setup(TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET);

    gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_CONNECT_WAITING;
    gTlkMdiHeadsetCtrl.timeout    = (TLK_MDI_TPSLL_TPH_PAIRING_TIMEOUT * 1000 * 1000) / TLK_MDI_TPSLL_TIMEOUT; /* counter pairing mode timeout 2mins*/
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &gTlkMdiHeadsetCtrl.timer);
    return true;
}

/**
 * @brief       Waits for connection in the pairing state machine.
 * @return      true if connection status changes, false otherwise.
 * @note    
 */
static bool tlkmdi_bt_tph_nowPSM_waitCon(void)
{
    bool    ret           = false;
    uint8_t bt_link_count = tlkmdi_btacl_getUsedCount();
    if (gTlkMdiHeadsetCtrl.dongleIsConn) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, TLK_MDI_TPSLL_TPH_PAIRING_TIMEOUT);

        tlkmdi_tph_version_sync();
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_WAIT_VERSION_SYNC;
        gTlkMdiHeadsetCtrl.timeout    = (TLK_MDI_TPSLL_TPH_PAIRING_TIMEOUT * 1000 * 1000) / TLK_MDI_TPSLL_TIMEOUT;

        ret = true;
        tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_bt_tph_nowPSM_waitCon: dongle connect");
    }
    if (bt_link_count > 0) {
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_BT_CONN_ASYNC_DISCON;
        tph_host_hal_set_setup_ac_chn(gTlkMdiHeadsetCtrl.tpsll_ac, gTlkMdiHeadsetCtrl.tpsll_ch);
        tph_host_hal_start_connection_setup(TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
        ret = true;
    }
    return ret;
}

/**
 * @brief       This function starts the reconnection process for a paired dongle.
 * @return      none.
 * @note        The function checks if the paired dongle address is null or invalid before attempting to reconnect.
 */
static bool tlkmdi_bt_tph_nowPSM_powerOnReconDongle(void)
{
#if MCU_CORE_TL752X_TEMP
    return false;
#endif
    uint8_t null_addr0[BD_ADDR_LEN];
    uint8_t null_addr1[BD_ADDR_LEN];

    tmemset(null_addr0, 0, BD_ADDR_LEN);
    tmemset(null_addr1, 0xff, BD_ADDR_LEN);
    tlkmdi_tinySql_getTpdMacAddr(gTlkMdiHeadsetCtrl.addr_paired_dongle);
    tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "power_on_recon_dongle: dongle mac", gTlkMdiHeadsetCtrl.addr_paired_dongle, 6);
    if (tmemcmp(gTlkMdiHeadsetCtrl.addr_paired_dongle, null_addr0, BD_ADDR_LEN) == 0 || tmemcmp(gTlkMdiHeadsetCtrl.addr_paired_dongle, null_addr1, BD_ADDR_LEN) == 0) {
        tph_host_hal_set_dongle_mac_existed(0);
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_CRASH;
    } else {
        tlkapi_trace(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "power_on_recon_dongle: find paired device");
        tph_host_hal_set_gen_mac(gTlkMdiHeadsetCtrl.local_addr);
        tph_host_hal_set_dongle_mac_existed(1);
        tph_host_hal_set_dongle_mac(gTlkMdiHeadsetCtrl.addr_paired_dongle);
        tph_host_hal_get_ac_chn_from_mac(gTlkMdiHeadsetCtrl.local_addr, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ac, (uint8_t *)&gTlkMdiHeadsetCtrl.tpsll_ch);
        tph_host_hal_set_setup_ac_chn(gTlkMdiHeadsetCtrl.tpsll_ac, gTlkMdiHeadsetCtrl.tpsll_ch);
        tph_host_hal_start_connection_setup(TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET);
        gTlkMdiHeadsetCtrl.cur_status = TLKMDI_TPSLL_PAIRING_CONNECT_WAITING;
    }
    return true;
}

/**
 * @brief       Main handler for the Bluetooth pairing state machine.
 * @return      true if any state transition occurs, false otherwise.
 * @note    
 */
static bool tlkmdi_bt_tph_pairing_state_machine(void)
{
    switch (gTlkMdiHeadsetCtrl.cur_status) {
    case TLKMDI_TPSLL_PAIRING_ASYNC_DISCONNECTED:
        return tlkmdi_bt_tph_nowPSM_asyncDiscon();
    case TLKMDI_TPSLL_PAIRING_BT_DSICON_WAITING:
        return tlkmdi_bt_tph_nowPSM_waitBtDiscon();
    case TLKMDI_TPSLL_PAIRING_BT_DISCONNECTED:
        return tlkmdi_bt_tph_nowPSM_btDiscon();
    case TLKMDI_TPSLL_PAIRING_CONNECT_WAITING:
        return tlkmdi_bt_tph_nowPSM_waitCon();
    case TLKMDI_TPSLL_RECONNECTING:
        return tlkmdi_bt_tph_nowPSM_powerOnReconDongle();
    case TLKMDI_TPSLL_WAIT_VERSION_SYNC:
        return false;
    }
    return false;
}

/**
 * @brief       Initializes the Bluetooth pairing process.
 * @return      TLK_ENONE on success.
 * @note    
 */
int tlkmdi_bt_tph_pair_init(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &gTlkMdiHeadsetCtrl.timer, TLK_MDI_TPSLL_TIMEOUT, false, tlkmdi_bt_tph_timer, NULL);
    /*In the early stage, it was to wait for the N22 to start up; now close, if N22 need vcd, timeout is 20 */
    // gTlkMdiHeadsetCtrl.timeout = 2;
    // tlksys_timer_reStart(TLKSYS_TASKID_HOST, &gTlkMdiHeadsetCtrl.timer);
    return TLK_ENONE;
}

/**
 * @brief       Handles the Bluetooth pairing process.
 * @return      TLK_ENONE on success.
 * @note    
 */
int tlkmdi_bt_tph_pairing_handler(void)
{
    bool isNeedRefresh;
    do {
        isNeedRefresh = tlkmdi_bt_tph_pairing_state_machine();
    } while (isNeedRefresh);
    return TLK_ENONE;
}


#endif
