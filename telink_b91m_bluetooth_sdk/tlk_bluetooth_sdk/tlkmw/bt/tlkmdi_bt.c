/********************************************************************************************************
 * @file    tlkmdi_bt.c
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
#if (TLK_STK_BT_ENABLE)
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_event.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "stack/bt/common/co_hci.h"
#include "tlkmw/tlkmw.h"
static const uint8_t hci_env_tag[8] = {
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff,
}; // default set num of packets complete as 0
static const uint8_t hci_env_tag_page2[8] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/**
 * @brief       Handle HCI command complete event for Bluetooth initialization.
 * @param[in]   pData    - pointer to the HCI event data.
 * @param[in]   dataLen  - length of the HCI event data.
 * @return      TLK_ENONE if success, otherwise error code.
 */
static int tlkmdi_bt_mgrCtrl_HCICompleteEvt(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 2) {
        return -TLK_EPARAM;
    }
    uint8_t opcode = pData[0];

    if (opcode == (HCI_WR_EXT_INQ_RSP_CMD_OPCODE & 0xFF)) {
        bth_hci_sendReadBufferSizeCmd();
        bth_hci_sendSetEvtMaskCmd((uint8_t *)hci_env_tag);
        bth_hci_sendSetEvtMaskPage2Cmd((uint8_t *)hci_env_tag_page2);
    } else if (opcode == (HCI_SET_EVT_MASK_PAGE_2_CMD_OPCODE & 0xFF)) {
        bth_hci_sendWriteSimplePairingModeCmd(1); // enable simple pairing mode
        bth_hci_sendWriteDefaultLinkPolicy(0x00);
        bth_hci_sendWriteInquiryScanTypeCmd(1);
        bth_hci_sendWritePageScanTypeCmd(1);
    }
    return TLK_ENONE;
}

BTH_EVT_REGISTER(BTH_EVTID_CMD_COMPLETE, tlkmdi_bt_mgrCtrl_HCICompleteEvt);

/**
 * @brief       Initialize the Bluetooth manager control module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_mgr_ctrlInit(void)
{
    uint8_t nameLen = 0;
    uint8_t bttemp[TINYSQL_BT_NAME_LENS + 3];
    uint8_t nameBuffer[TINYSQL_BT_NAME_LENS + 1] = {0};
    uint8_t addr[6];

    tlkmdi_tinySql_getBtName(nameBuffer);
    tlkapi_printf(1, "[BT]<TRACE> Bt name:%s", (char *)nameBuffer);
    tlkmdi_tinySql_getBtMacAddress(addr);
    bth_hci_sendSetLinkMaxNbCmd((TLK_STK_BTACL_NUMB & 0xF) | ((TLK_STK_BTSCO_NUMB & 0xF) << 4));
    bth_hci_sendSetBtAddrCmd(addr);
    bth_hci_sendWriteLocalNameCmd(nameBuffer);

    nameLen = strlen((char *)(nameBuffer));
    tmemset(bttemp, 0, TINYSQL_BT_NAME_LENS + 3);
    bttemp[0] = nameLen + 1; //data length:
    bttemp[1] = 0x09;        //data type: complete local name
    tmemcpy(&bttemp[2], nameBuffer, nameLen);
    bth_hci_sendWriteInquiryScanActivityCmd(64, 32);
    //Central device 0x5a020c for temp test
    bth_hci_sendWriteClassOfDeviceCmd(TLKCFG_BT_MGR_DEVICE_CLASS);
    bth_hci_sendWritePageScanActivityCmd(64, 48);
#if (MCU_CORE_TYPE != MCU_CORE_TL751X && !PROJ_BTTPSLL_TWS)
    bth_hci_sendWriteSecureConnHostSupportCmd(1);
#endif
    bth_hci_sendWriteExtendedInquiryRspCmd(0, bttemp, nameLen + 2);
}

/**
 * @brief       Get the Bluetooth device name.
 * @param[out]  recBuffer - Pointer to buffer where the device name will be stored.
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_getName(uint8_t *recBuffer)
{
    return tlkmdi_tinySql_getBtName(recBuffer);
}

/**
 * @brief       Get the Bluetooth device address.
 * @param[out]  recBuffer - Pointer to buffer where the device address will be stored (6 bytes).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_getAddr(uint8_t *recBuffer)
{
    return tlkmdi_tinySql_getBtMacAddress(recBuffer);
}

/**
 * @brief       Set the Bluetooth device name.
 * @param[in]   pName - Pointer to the new device name string.
 * @param[in]   nameLen - Length of the name string (excluding null terminator).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_setName(uint8_t *pName, uint8_t nameLen)
{
    int ret = tlkmdi_tinySql_setBtName(pName, nameLen);
    if (ret == TLK_ENONE) {
        uint8_t nameBuffer[TINYSQL_BT_NAME_LENS];
        tlkmdi_tinySql_getBtName(nameBuffer);
        bth_hci_sendWriteLocalNameCmd(nameBuffer);
    }
    return ret;
}

/**
 * @brief       Set the Bluetooth device address.
 * @param[in]   pAddr - Pointer to the new device address (6 bytes).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_setAddr(uint8_t *pAddr)
{
    int ret = tlkmdi_tinySql_setBtMacAddress(pAddr);
    if (ret == TLK_ENONE) {
        uint8_t addr[6];
        tlkmdi_tinySql_getBtMacAddress(addr);
        bth_hci_sendSetBtAddrCmd(addr);
    }
    return ret;
}

/**
 * @brief       Initialize the Bluetooth module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_init(void)
{
#if (TLK_STK_BT_ENABLE)
    tlkstk_init();
    tlkmdi_btacl_init();
    tlkmdi_btScan_process_init();
#endif
#if (TLK_MW_BTINQ_ENABLE)
    tlkmdi_btinq_init();
#endif
#if (TLKBTP_CFG_ATTSRV_ENABLE)
    tlkmdi_btatt_init();
#endif
#if (TLKBTP_CFG_HID_ENABLE)
    tlkmdi_bthid_init();
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    tlkmdi_bthfp_init();
#endif

#if (TLKBTP_CFG_A2DP_ENABLE)
    tlkmdi_bta2dp_init();
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    tlkmdi_btsco_init();
#endif
    tlkmdi_bt_mgr_ctrlInit();
#if (TLKBTP_CFG_PBAP_ENABLE)
    tlkmdi_pbap_init();
#endif
#if (TLKBTP_CFG_IAP_ENABLE)
    tlkmdi_btiap_init();
#endif

#if (TLKBTP_CFG_SPP_ENABLE)
    tlkmdi_btspp_init();
#endif

    bth_acl_setWaitTimeout(30000);
}


#endif // #if (TLK_STK_BT_ENABLE)
