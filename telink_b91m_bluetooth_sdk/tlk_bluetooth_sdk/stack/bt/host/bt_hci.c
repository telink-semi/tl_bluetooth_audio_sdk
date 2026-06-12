/********************************************************************************************************
 * @file    bt_hci.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "stack/bt/host//bt_hci.h"
#include "stack/bt/host/bth/bth_handle.h"
#include "stack/bt/host/bth/bth_define.h"

#if MCU_DUAL_CORE_ENABLE
#include "stack/multiCoreComm/service/service_shareMemory.h"
#endif

static bt_hci_param_t sTlkBtHciParam;
bt_local_version_t    sTlkBtlocalVersionParam;
bt_rd_tcf_info_t      sTlkBtTcfInfo;
#if MCU_DUAL_CORE_ENABLE
static TlkBtHciEventCallback   tlk_bt_ctrl_s_TlkBtHciEventCB   = 0;
static TlkBtHciAclDataCallback tlk_bt_ctrl_s_TlkBtHciAclDataCB = 0;
static TlkBtHciScoDataCallback tlk_bt_ctrl_s_TlkBtHciScoDataCB = 0;

/*bluetooth snoop tool.*/
extern void bluetooth_host_snoop_write_hci_tx_packet(const uint8_t *packet, uint16_t length);
extern void bluetooth_host_snoop_write_hci_rx_packet(const uint8_t *packet, uint16_t length);

void tlkbt_hci_recvC2HData(uint8_t *pData, uint32_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return;
    }

    bluetooth_host_snoop_write_hci_rx_packet(pData, dataLen);

    uint8_t type = pData[0];

    if (type == BT_HCI_TYPE_ACL_DATA) {
        uint16_t hldFlgs = pData[1] | (pData[2] << 8);
        uint16_t length  = pData[3] | (pData[4] << 8);

        if (length + 4 > (uint16_t)(dataLen + 1)) {
            return;
        }

        if (tlk_bt_ctrl_s_TlkBtHciAclDataCB != NULL) {
            tlk_bt_ctrl_s_TlkBtHciAclDataCB(hldFlgs, pData + 5, length);
        }

    } else if (type == BT_HCI_TYPE_EVENT) {
        uint8_t evtCode = pData[1];
        uint8_t length  = pData[2];
        if (dataLen < (BT_HCI_EVT_HDR_LEN + 1)) {
            return;
        }

        if (tlk_bt_ctrl_s_TlkBtHciEventCB != NULL) {
            tlk_bt_ctrl_s_TlkBtHciEventCB(evtCode, pData + 3, length);
        }
    } else if (type == BT_HCI_TYPE_SCO_DATA) {
        uint16_t hldFlgs = pData[1] | (pData[2] << 8);
        uint16_t length  = pData[3];

        if (tlk_bt_ctrl_s_TlkBtHciScoDataCB != NULL) {
            tlk_bt_ctrl_s_TlkBtHciScoDataCB(hldFlgs, pData + 4, length);
        }
    }
}
#endif

void tlkbt_hci_setAclBuffSize(uint16_t acl_pk_len, uint8_t sco_pk_len, uint16_t acl_num, uint16_t sco_num)
{
    sTlkBtHciParam.sco_pk_Len  = sco_pk_len;
    sTlkBtHciParam.acl_pk_Len  = acl_pk_len;
    sTlkBtHciParam.acl_pk_numb = acl_num;
    sTlkBtHciParam.sco_pk_numb = sco_num;
}

void tlkbt_hci_getLovalVersion(uint8_t *pdata, uint8_t dataLen)
{
    sTlkBtlocalVersionParam.status     = pdata[0];
    sTlkBtlocalVersionParam.hci_ver    = pdata[1];
    sTlkBtlocalVersionParam.hci_rev    = pdata[2] | (pdata[3] << 8);
    sTlkBtlocalVersionParam.lmp_ver    = pdata[4];
    sTlkBtlocalVersionParam.manuf_name = pdata[5] | (pdata[6] << 8);
    sTlkBtlocalVersionParam.lmp_subver = pdata[7] | (pdata[8] << 8);
    tlkapi_array(0xffffffff, "<TEST1>", "tlkbt_hci_getLovalVersion", pdata, dataLen);
}

void tlksdk_host_get_clkn_fcnt_from_tick(bt_rd_tcf_info_t *p, uint32_t t)
{
    // add 1024 half-slots offset to make sure td > 0
    uint32_t td = t + 1024 * 625 * 24 / 2 - p->tick; //time difference
    p->tick     = t - td % 24 / 2;

    uint32_t t_half_us = td / 24 / 2;
    p->clkn += t_half_us / 625;
    uint16_t tmp = t_half_us % 625;
    if (p->fcnt < tmp) { //note that the finecounter is downward
        p->clkn++;
        p->fcnt = p->fcnt + 625 - tmp;
    } else {
        p->fcnt -= tmp;
    }

    p->clkn = (p->clkn - 1024) & ((1L << 28) - 1);
}

void tlkbt_hci_getTcfInfo(uint8_t *pdata, uint8_t dataLen)
{
    sTlkBtTcfInfo.tick = pdata[0] | ((pdata[1] << 8)) | ((pdata[2] << 16)) | ((pdata[3] << 24));
    sTlkBtTcfInfo.clkn = pdata[4] | ((pdata[5] << 8)) | ((pdata[6] << 16)) | ((pdata[7] << 24));
    sTlkBtTcfInfo.fcnt = pdata[8] | ((pdata[9] << 8));
    tlkapi_array(0xffffffff, "<TEST1>", "tlkbt_hci_getTcfInfo", pdata, dataLen);
}

uint8_t tlkbt_hci_aclGetAvalSize(void)
{
    return (sTlkBtHciParam.acl_pk_numb - bth_handle_aclGetTotalUsedPktNumb());
}

void tlkbt_hci_regEventCB(TlkBtHciEventCallback cb)
{
#if MCU_DUAL_CORE_ENABLE
    tlk_bt_ctrl_s_TlkBtHciEventCB = cb;
#else
    void tlk_bt_ctrl_rwbt_hci_regEventCB(TlkBtHciEventCallback cb);
    tlk_bt_ctrl_rwbt_hci_regEventCB(cb);
#endif
}

void tlkbt_hci_regAclDataCB(TlkBtHciAclDataCallback cb)
{
#if MCU_DUAL_CORE_ENABLE
    tlk_bt_ctrl_s_TlkBtHciAclDataCB = cb;
#else
    void tlk_bt_ctrl_rwbt_hci_regAclDataCB(TlkBtHciAclDataCallback cb);
    tlk_bt_ctrl_rwbt_hci_regAclDataCB(cb);
#endif
}

void tlkbt_hci_regScoDataCB(TlkBtHciScoDataCallback cb)
{
#if MCU_DUAL_CORE_ENABLE
    tlk_bt_ctrl_s_TlkBtHciScoDataCB = cb;
#else
    void tlk_bt_ctrl_rwbt_hci_regScoDataCB(TlkBtHciScoDataCallback cb);
    tlk_bt_ctrl_rwbt_hci_regScoDataCB(cb);
#endif
}

int tlkbt_hci_sendSyncData(uint16_t conn_handle, uint8_t *pData, uint16_t dataLen)
{
//TODO: SCO also uses fifo, how to maintain fifo number.
#if MCU_DUAL_CORE_ENABLE
    uint8_t  pBuffer[128];
    uint16_t buffLen = 0;

    extern bth_acl_get_tws_role_cb_t bth_handle_acl_get_tws_role_cb;
    if (bth_handle_acl_get_tws_role_cb != NULL) {
        if ((bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_SLAVE) || (bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_OBSERVER)) {
            return TLK_ENONE;
        }
    }

    pBuffer[buffLen++] = BT_HCI_TYPE_SCO_DATA;
    pBuffer[buffLen++] = conn_handle & 0xFF;
    pBuffer[buffLen++] = (conn_handle >> 8) & 0xFF;
    pBuffer[buffLen++] = dataLen & 0xFF;

    //add byzhaowei
    tmemcpy(pBuffer + buffLen, pData, dataLen);
    buffLen += dataLen;
    bluetooth_host_snoop_write_hci_tx_packet(pBuffer, buffLen);
#if !defined(MCU_CORE_N22)
    tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, pBuffer, buffLen);
#endif
    return TLK_ENONE;
#else
    uint8_t tlk_bt_ctrl_h0tl_h2c_write_sync_data(uint16_t handle, uint8_t len, uint8_t * data);
    tlk_bt_ctrl_h0tl_h2c_write_sync_data(conn_handle, dataLen, pData);
    return TLK_ENONE;
#endif
}

int tlkbt_hci_sendH2cCmd(uint16_t opcode, uint8_t *pData, uint8_t dataLen)
{
    int ret = TLK_ENONE;

    extern bth_acl_get_tws_role_cb_t bth_handle_acl_get_tws_role_cb;
    if (bth_handle_acl_get_tws_role_cb != NULL && (bth_handle_getUsedAclCount() > 0)) { // acl complete ~ encry complete have hci cmd write to fifo
        if ((bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_SLAVE) || (bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_OBSERVER)) {
            return ret;
        }
    }

#if MCU_DUAL_CORE_ENABLE
    uint8_t  pBuffer[1024];
    uint16_t buffLen = 0;

    pBuffer[buffLen++] = BT_HCI_TYPE_CMD;
    pBuffer[buffLen++] = opcode & 0xFF;
    pBuffer[buffLen++] = (opcode >> 8) & 0xFF;
    pBuffer[buffLen++] = dataLen & 0xFF;

    tmemcpy(pBuffer + buffLen, pData, dataLen);
    buffLen += dataLen;
    bluetooth_host_snoop_write_hci_tx_packet(pBuffer, buffLen);
#if !defined(MCU_CORE_N22)
    ret = tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, pBuffer, buffLen);
#endif
#else
    uint8_t tlk_bt_ctrl_h0tl_h2c_write_command(uint16_t opcode, volatile uint8_t * param, uint8_t parlen);
    ret = tlk_bt_ctrl_h0tl_h2c_write_command(opcode, pData, dataLen);
#endif
    //send error
    if (ret != 0) {
        tlkapi_printf(1, "!!tlkbt_hci_sendH2cCmd Fail ret = [%d] opcode = [0x%x]", ret, opcode);
        ret = -TLK_EQUOTA;
    }
    return ret;
}

uint32_t tlkbt_hci_h2cFifoUnused(void)
{
    uint8_t tlk_bt_ctrl_h0tl_h2c_fifo_unused(void);
    return tlk_bt_ctrl_h0tl_h2c_fifo_unused();
}

//Remove later;Just for acl available packet check, need to open the log in bth_hcievt_numOfCompletePkts.
//extern uint16_t tlk_bt_ctrl_bt_util_get_acl_tx_free_count(void);
int tlkbt_hci_sendH2cAclData(uint16_t aclHandle, uint8_t *pUsrExt, uint16_t extLen, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen)
{
    uint16_t length;
    uint16_t maxLen;
    uint16_t tmpLen;
    uint8_t  pktCnt;
    uint16_t buffLen;
    uint8_t  pBuffer[700];
    uint16_t hdl_flags   = 0;
    uint8_t  avalPktNumb = 0;

    if (bth_handle_getUsedAcl(aclHandle) == NULL) {
        return TLK_ENONE; //Avoid duplicate sending
    }

    extern bth_acl_get_tws_role_cb_t bth_handle_acl_get_tws_role_cb;
    if (bth_handle_acl_get_tws_role_cb != NULL) {
        if ((bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_SLAVE) || (bth_handle_acl_get_tws_role_cb() == BTH_TPT_TWS_ROLE_OBSERVER)) {
            return TLK_ENONE;
        }
    }

    if (extLen + headLen > TLKBT_HCI_H2C_ACL_SIZE || extLen + headLen + dataLen == 0 || aclHandle == 0) {
        return -TLK_EPARAM;
    }

    maxLen = TLKBT_HCI_H2C_ACL_SIZE;
    length = extLen + headLen + dataLen;
    pktCnt = (length + maxLen - 1) / maxLen;

    avalPktNumb = tlkbt_hci_aclGetAvalSize();

    if ((pktCnt > avalPktNumb) || (avalPktNumb < TLKBT_HCI_H2C_ACL_RESV_NUMB)) {
        tlkapi_printf(1, "tlkbt_hci_sendH2cAclData: not space");
        return -TLK_ENOSPACE;
    }

    if (extLen + headLen + dataLen < maxLen) {
        length = extLen + headLen + dataLen;
    } else {
        length = maxLen;
    }
    tmpLen = length - extLen - headLen;

#if MCU_DUAL_CORE_ENABLE
    buffLen   = 0;
    hdl_flags = ((aclHandle) & 0x0FFF) | ((TLKBT_HCI_LLID_START) << 12);


    pBuffer[buffLen++] = BT_HCI_TYPE_ACL_DATA;
    pBuffer[buffLen++] = hdl_flags & 0xFF;
    pBuffer[buffLen++] = (hdl_flags >> 8) & 0xFF;
    pBuffer[buffLen++] = (extLen + headLen + tmpLen) & 0xFF;
    pBuffer[buffLen++] = ((extLen + headLen + tmpLen) >> 8) & 0xFF;

    if (extLen != 0) {
        tmemcpy(pBuffer + buffLen, pUsrExt, extLen);
        buffLen += extLen;
    }
    if (headLen != 0) {
        tmemcpy(pBuffer + buffLen, pHead, headLen);
        buffLen += headLen;
    }
    if (tmpLen != 0) {
        tmemcpy(pBuffer + buffLen, pData, tmpLen);
        buffLen += tmpLen;
    }
    bluetooth_host_snoop_write_hci_tx_packet(pBuffer, buffLen);
#if !defined(MCU_CORE_N22)
    if (tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, pBuffer, buffLen) == 0) {
        bth_handle_aclPktNumbUpdate(aclHandle, true, 1);
    }
#endif
#else

    buffLen   = 0;
    hdl_flags = ((aclHandle) & 0x0FFF) | ((TLKBT_HCI_LLID_START) << 12);

    if (extLen != 0) {
        tmemcpy(pBuffer + buffLen, pUsrExt, extLen);
        buffLen += extLen;
    }
    if (headLen != 0) {
        tmemcpy(pBuffer + buffLen, pHead, headLen);
        buffLen += headLen;
    }
    if (tmpLen != 0) {
        tmemcpy(pBuffer + buffLen, pData, tmpLen);
        buffLen += tmpLen;
    }
    uint8_t tlk_bt_ctrl_h0tl_h2c_write_acl_data(uint16_t hdl_flags, uint16_t data_len, uint8_t * data);
    if (tlk_bt_ctrl_h0tl_h2c_write_acl_data(hdl_flags, buffLen, pBuffer) == 0) {
        bth_handle_aclPktNumbUpdate(aclHandle, true, 1);
        //tlkapi_printf(1, "bth[%d] btc[%d]", tlkbt_hci_aclGetAvalSize(), tlk_bt_ctrl_bt_util_get_acl_tx_free_count());
    }
#endif


    while (tmpLen < dataLen) {
        if (tmpLen + maxLen >= dataLen) {
            length = maxLen;
        } else {
            length = dataLen - maxLen;
        }
#if MCU_DUAL_CORE_ENABLE
        buffLen   = 0;
        hdl_flags = ((aclHandle) & 0x0FFF) | ((TLKBT_HCI_LLID_CONTINUE) << 12);

        pBuffer[buffLen++] = BT_HCI_TYPE_ACL_DATA;
        pBuffer[buffLen++] = hdl_flags & 0xFF;
        pBuffer[buffLen++] = (hdl_flags >> 8) & 0xFF;
        pBuffer[buffLen++] = dataLen & 0xFF;
        pBuffer[buffLen++] = (dataLen >> 8) & 0xFF;

        tmemcpy(pBuffer + buffLen, pData + tmpLen, length);
        buffLen += length;
        tmpLen += length;

        bluetooth_host_snoop_write_hci_tx_packet(pBuffer, buffLen);
#if !defined(MCU_CORE_N22)
        if (tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, pBuffer, buffLen) == 0) {
            bth_handle_aclPktNumbUpdate(aclHandle, true, 1);
        }
#endif
#else
        buffLen   = 0;
        hdl_flags = ((aclHandle) & 0x0FFF) | ((TLKBT_HCI_LLID_CONTINUE) << 12);
        tmemcpy(pBuffer + buffLen, pData + tmpLen, length);
        buffLen += length;
        tmpLen += length;
        if (tlk_bt_ctrl_h0tl_h2c_write_acl_data(hdl_flags, buffLen, pBuffer) == 0) {
            bth_handle_aclPktNumbUpdate(aclHandle, true, 1);
            //tlkapi_printf(1, "bth[%d] btc[%d]", tlkbt_hci_aclGetAvalSize(), tlk_bt_ctrl_bt_util_get_acl_tx_free_count());
        }
#endif
    }
    return TLK_ENONE;
}

uint32_t tlkbt_hci_h2cDatFifoUsed(void)
{
#if MCU_DUAL_CORE_ENABLE
    return 0;
#else
    uint8_t tlk_bt_ctrl_h0tl_h2c_fifo_used(void);
    return tlk_bt_ctrl_h0tl_h2c_fifo_used();
#endif
}

uint32_t tlkbt_hci_h2cFifoSize(void)
{
    return 692 - 4; //temp code
}
