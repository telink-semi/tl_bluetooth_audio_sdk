/********************************************************************************************************
 * @file    tlkmdi_bthid.c
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
#if (TLKBTP_CFG_HID_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"
#include "stack/bt/host/btp/btp_stdio.h"


#define TLKMDI_BTHID_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_HID << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTHID_DBG_SIGN "[MDI]"

static uint8_t sTlkMdiBtHidProtocolMode;

/**
 * @brief       This function handles HID protocol mode setting
 * @param[in]   aclHandle   - ACL connection handle
 * @param[in]   protoMode   - protocol mode to set
 * @return      BTP_HID_HSHK_SUCCESS if success, otherwise error code
 */
static uint tlkmdi_bthid_setProtocolCB(uint16_t aclHandle, uint8_t protoMode)
{
    (void)aclHandle;
    if (protoMode > 2) {
        return BTP_HID_HSHK_ERR_INVALID_PARAMETER;
    }
    sTlkMdiBtHidProtocolMode = protoMode;
    return BTP_HID_HSHK_SUCCESS;
}

/**
 * @brief       This function gets the current HID protocol mode
 * @param[in]   aclHandle     - ACL connection handle
 * @param[out]  pProtoMode    - pointer to store the protocol mode
 * @return      BTP_HID_HSHK_SUCCESS if success, otherwise error code
 */
static uint tlkmdi_bthid_getProtocolCB(uint16_t aclHandle, uint8_t *pProtoMode)
{
    (void)aclHandle;
    if (pProtoMode == NULL) {
        return BTP_HID_HSHK_ERR_INVALID_PARAMETER;
    }
    *pProtoMode = sTlkMdiBtHidProtocolMode;
    return BTP_HID_HSHK_SUCCESS;
}

/**
 * @brief       This function handles setting HID report
 * @param[in]   aclHandle    - ACL connection handle
 * @param[in]   reportType   - report type
 * @param[in]   reportID     - report ID
 * @param[in]   pData        - pointer to report data
 * @param[in]   dataLen      - length of report data
 * @return      BTP_HID_HSHK_SUCCESS if success, otherwise error code
 */
static uint tlkmdi_bthid_setReportCB(uint16_t aclHandle, uint8_t reportType, uint8_t reportID, uint8_t *pData, uint16_t dataLen)
{
    (void)aclHandle;
    (void)reportType;
    (void)reportID;
    if (pData == NULL || dataLen == 0) {
        return BTP_HID_HSHK_ERR_INVALID_PARAMETER;
    }
    // Bond with gcBtpSdpHidReportMap.
    return BTP_HID_HSHK_SUCCESS;
}

/**
 * @brief       This function handles getting HID report
 * @param[in]   aclHandle     - ACL connection handle
 * @param[in]   reportType    - report type
 * @param[in]   reportID      - report ID
 * @param[out]  pBuff         - buffer to store report data
 * @param[out]  pBuffLen      - pointer to buffer length
 * @return      BTP_HID_HSHK_SUCCESS if success, otherwise error code
 */
static uint tlkmdi_bthid_getReportCB(uint16_t aclHandle, uint8_t reportType, uint8_t reportID, uint8_t *pBuff, uint16_t *pBuffLen)
{
    (void)aclHandle;
    (void)reportType;
    if (pBuff == NULL || pBuffLen == NULL || (*pBuffLen) == 0) {
        return BTP_HID_HSHK_ERR_INVALID_PARAMETER;
    }

    uint16_t buffLen = *pBuffLen;
    // Bond with gcBtpSdpHidReportMap.
    if (reportID == 0x01) { // keyboard
        if (buffLen > 20) {
            buffLen = 20;
        }
        tmemset(pBuff, 0, buffLen);
    } else if (reportID == 0x02) {
        if (buffLen > 2) {
            buffLen = 2;
        }
        tmemset(pBuff, 0, buffLen);
    }

    return BTP_HID_HSHK_SUCCESS;
}

/**
 * @brief       This function handles interrupt data reception
 * @param[in]   handle    - connection handle
 * @param[in]   pData     - pointer to received data
 * @param[in]   dataLen   - length of received data
 * @return      TLK_ENONE
 */
static int tlkmdi_bthid_irqRecvDataCB(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    (void)handle;
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

/**
 * @brief       This function initializes the HID module
 * @param[in]   none
 * @return      TLK_ENONE
 */
int tlkmdi_bthid_init(void)
{
    sTlkMdiBtHidProtocolMode = BTP_HID_PROTO_BOOT;
    btp_hidd_regCB(tlkmdi_bthid_setReportCB, tlkmdi_bthid_getReportCB, tlkmdi_bthid_setProtocolCB, tlkmdi_bthid_getProtocolCB, NULL, tlkmdi_bthid_irqRecvDataCB);

    return TLK_ENONE;
}

/**
 * @brief       This function connects to HID device
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hidd_connect result
 */
int tlkmdi_bthid_connect(uint16_t aclHandle)
{
    return btp_hidd_connect(aclHandle);
}

/**
 * @brief       This function disconnects from HID device
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hidd_disconn result
 */
int tlkmdi_bthid_disconn(uint16_t aclHandle)
{
    return btp_hidd_disconn(aclHandle);
}

/**
 * @brief       This function sends HID data with report ID
 * @param[in]   aclHandle    - ACL connection handle
 * @param[in]   reportID     - report ID
 * @param[in]   reportType   - report type
 * @param[in]   pData        - pointer to data to send
 * @param[in]   dataLen      - length of data to send
 * @return      btp_hidd_sendData result
 */
int tlkmdi_bthid_sendData(uint16_t aclHandle, uint8_t reportID, uint8_t reportType, uint8_t *pData, uint16_t dataLen)
{
    return btp_hidd_sendData(aclHandle, reportID, reportType, pData, dataLen);
}

/**
 * @brief       This function sends HID data without report ID
 * @param[in]   aclHandle    - ACL connection handle
 * @param[in]   reportType   - report type
 * @param[in]   pData        - pointer to data to send
 * @param[in]   dataLen      - length of data to send
 * @return      btp_hidd_sendDataWithoutReportID result
 */
int tlkmdi_bthid_sendDataWithoutReportID(uint16_t aclHandle, uint8_t reportType, uint8_t *pData, uint16_t dataLen)
{
    return btp_hidd_sendDataWithoutReportID(aclHandle, reportType, pData, dataLen);
}

/**
 * @brief       This function increases volume via HID consumer control
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlVolInc(void)
{
    int      ret;
    uint16_t aclHandle;
    uint16_t consumeKey;
    aclHandle = btp_hidd_getAnyConnHandle();
    if (aclHandle == 0) {
        return -TLK_ENOREADY;
    }
    consumeKey = 0x00e9;
    ret        = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_CONSUMER_INPUT, BTP_HID_DATA_RTYPE_INPUT, (uint8_t *)&consumeKey, 2);
    consumeKey = 0x0000;
    if (ret == TLK_ENONE) {
        ret = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_CONSUMER_INPUT, BTP_HID_DATA_RTYPE_INPUT, (uint8_t *)&consumeKey, 2);
    }
    return ret;
}

/**
 * @brief       This function decreases volume via HID consumer control
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlVolDec(void)
{
    int      ret;
    uint16_t aclHandle;
    uint16_t consumeKey;
    aclHandle = btp_hidd_getAnyConnHandle();
    if (aclHandle == 0) {
        return -TLK_ENOREADY;
    }
    consumeKey = 0x00ea;
    ret        = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_CONSUMER_INPUT, BTP_HID_DATA_RTYPE_INPUT, (uint8_t *)&consumeKey, 2);
    consumeKey = 0x0000;
    if (ret == TLK_ENONE) {
        ret = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_CONSUMER_INPUT, BTP_HID_DATA_RTYPE_INPUT, (uint8_t *)&consumeKey, 2);
    }
    return ret;
}

static bool s_button_pressed = false;

/**
 * @brief       This function tests XInput gamepad functionality
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlXinputTest(void)
{
    uint16_t aclHandle;
    int      ret;

    uint8_t key_buf[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    aclHandle = btp_hidd_getAnyConnHandle();
    if (aclHandle == 0) {
        return -TLK_ENOREADY;
    }
    tlkapi_trace(0xffffffff, "test", "tlkmdi_bthid_ctrlGamePad>>>>>>2");
    if (s_button_pressed == false) {
        key_buf[0]       = 0x01;
        ret              = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_GAMEPAD_INPUT, BTP_HID_DATA_RTYPE_INPUT, key_buf, sizeof(key_buf));
        s_button_pressed = true;

    } else {
        key_buf[0]       = 0x00;
        ret              = tlkmdi_bthid_sendData(aclHandle, TLKMDI_BTHID_REPORT_ID_GAMEPAD_INPUT, BTP_HID_DATA_RTYPE_INPUT, key_buf, sizeof(key_buf));
        s_button_pressed = false;
    }

    return ret;
}

#else  // #if (TLKBTP_CFG_HID_ENABLE)

/**
 * @brief       This function increases volume via HID consumer control
 * @param[in]   none
 * @return      -TLK_ENOSUPPORT
 */
int tlkmdi_bthid_ctrlVolInc(void)
{
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       This function decreases volume via HID consumer control
 * @param[in]   none
 * @return      -TLK_ENOSUPPORT
 */
int tlkmdi_bthid_ctrlVolDec(void)
{
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       This function tests XInput gamepad functionality
 * @param[in]   none
 * @return      -TLK_ENOSUPPORT
 */
int tlkmdi_bthid_ctrlXinputTest(void)
{
    return -TLK_ENOSUPPORT;
}
#endif // #if (TLKBTP_CFG_HID_ENABLE)
