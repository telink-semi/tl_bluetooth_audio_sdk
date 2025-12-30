/********************************************************************************************************
 * @file    tlkmdi_btiap.c
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
#if (TLKBTP_CFG_IAP_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"
#include "stack/bt/host/btp/btp_stdio.h"

#include <stdint.h>
#include "tlkmdi_btiap.h"

#include "stack/bt/host/btp/iap/btp_iap.h"
#include "tlkmw/sys_dev/mfi/tlkdrv_mfi.h"
#include "tlklib/ios/iap2/iAP2LinkConfig.h"
#include "tlklib/ios/iap2/iAP2Defines.h"
#include "tlklib/ios/iap2/iAP2Log.h"
#include "tlklib/ios/iap2/iAP2FSM.h"
#include "tlklib/ios/iap2/iAP2Time.h"
#include "tlklib/ios/iap2/iAP2ListArray.h"
#include "tlklib/ios/iap2/iAP2BuffPool.h"
#include "tlklib/ios/iap2/iAP2Packet.h"
#include "tlklib/ios/iap2/iAP2Link.h"
#include "tlklib/ios/iap2/iAP2LinkRunLoop.h"
#include "tlklib/ios/iap2/iAP2Memory.h"


#define TLKMDI_BTIAP_DBG_FLAG                       ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_IAP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTIAP_DBG_SIGN                       "[MW_IAP]"

#define IAP2_PACKET_SYNC                            0xFF
#define IAP2_PACKET_SOP                             0x5A
#define IAP2_PACKET_SOP_ORIG                        0x55
#define IAP2_PACKET_CONTROL_SYN                     0x80 /* synchronization */
#define IAP2_PACKET_CONTROL_ACK                     0x40 /* acknowledgement */
#define IAP2_PACKET_CONTROL_EAK                     0x20 /* extended acknowledgement */
#define IAP2_PACKET_CONTROL_RST                     0x10 /* reset */
#define IAP2_PACKET_CONTROL_SUS                     0x08 /* suspend (sleep) */
#define IAP2_SESSION_START_MSB                      0x40
#define IAP2_SESSION_START_LSB                      0x40

#define IPA2_CONTROL_SESSION_VERSION                1
#define IAP2_RequestAuthenticationCertificate       0xAA00
#define IAP2_RequestAuthenticationChallengeResponse 0xAA02
#define IAP2_AuthenticationFailed                   0xAA04
#define IAP2_AuthenticationSucceeded                0xAA05

#define IAP2_StartIdentification                    0x1D00
#define IAP2_IdentificationAccepted                 0x1D02

#define IAP2_StartExternalAccessoryProtocolSession  0xEA00
#define IAP2_StopExternalAccessoryProtocolSession   0xEA01


// static iap2_manager_t iap2_manager;

static void tlkmdi_btiap_dataRecv(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen);
static BOOL tlkmdi_btiap_eap2RecvDeal(struct iAP2Link_st *pLink, uint8_t *pData, uint32_t dataLen, uint8_t session);
static void tlkmdi_btiap_eap2SendDeal(struct iAP2Link_st *pLink, iAP2Packet_t *packet);

static tlkmdi_btiap_t     sTlkMdiBtIapCtrl;
static iAP2Packet_t       sTlkMdiBtIapPacket;
static iAP2LinkRunLoop_t *sTlkMdiBtIapRanLoop = NULL;
static iAP2PacketData_t   sTlkMdiBtIappktdata;

uint16_t eap_session_id = 0;
uint16_t eap_flag       = 0;

const uint8_t identification_information_part1[] = {
    0x00, 0x0C, 0x00, 0x01, 'C',  'X',  'P',  'L',  '3',  '0',  '1',  0x00, 0x00, 0x11, 0x00, 0x02, 'C',  'r',  'i',  'c',  'u',  't',  ',',  ' ',  'I',  'n',  'c',  '.',
    0x00, 0x00, 0x0E, 0x00, 0x03, '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  0x00, 0x00, 0x0a, 0x00, 0x04, '1',  '.',  '0',  '.',  '0',  0x00, 0x00, 0x0a, 0x00,
    0x05, '2',  '.',  '0',  '.',  '0',  0x00, 0x00, 0x06, 0x00, 0x06, 0xea, 0x02, 0x00, 0x08, 0x00, 0x07, 0xea, 0,    0xea, 1,    0x00, 0x05, 0x00, 0x08, 0x00, 0x00, 0x06,
    0x00, 0x09, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x0a, 0,    5,    0,    0,    1,    0,    0x20, 0,    1,    'c',  'o',  'm',  '.',  'c',  'r',  'i',  'c',  'u',  't',  '.',
    'e',  'x',  'p',  'l',  'o',  'r',  'e',  '.',  'p',  'r',  'o',  't',  'o',  'c',  'o',  'l',  0,    0,    5,    0,    2,    0,    0x00, 0x25, 0x00, 0x0a, 0,    5,
    0,    0,    2,    0,    0x17, 0,    1,    'c',  'o',  'm',  '.',  'c',  'r',  'i',  'c',  'u',  't',  '.',  'e',  'x',  'p',  'l',  'o',  'r',  'e',  0,    0,    5,
    0,    2,    0,    0x00, 0x0f, 0x00, 0x0b, '2',  '5',  '6',  '2',  '7',  'Z',  'F',  'V',  'T',  '7',  0,    0x00, 0x07, 0x00, 0x0C, 0x65, 0x6E, 0x00, 0x00, 0x07, 0x00,
    0x0D, 0x65, 0x6E, 0x00, 0x00, 0x15, 0x00, 0x22, '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'A',  'B',  'C',  'D',  'E',  'F',  0};
const uint8_t identification_information_part2[] = {0x00, 0x38, 0x00, 0x11, 0,   6,   0,   0,   0,   0,    0,   0x20, 0,    1,    'B',  'l',  'u',  'e', 't',
                                                    'o',  'o',  't',  'h',  'T', 'r', 'a', 'n', 's', 'p',  'o', 'r',  't',  'C',  'o',  'm',  'p',  'o', 'n',
                                                    'e',  'n',  't',  0,    0,   4,   0,   2,   0,   0x0a, 0,   3,    0x01, 0xda, 0x20, 0xe9, 0x28, 0xe3};

iAP2PacketSYNData_t test_synParam = { //<Accessory Interface Specification R36.pdf> P394
    1,                                //version
    5,                                //maxOutstandingPackets
    30,                               //maxRetransmissions
    3,                                //maxCumAck
    650,                              //maxPacketSize
    1500,                             //retransmitTimeout
    73,                               //cumAckTimeout
    2,                                //numSessionInfo
    5,                                //peerMaxOutstandingPackets
    650,                              //peerMaxPacketSize
    {{0xa, 0, 1}, {0x0b, 2, 1}}};

static TlkApiTimer_t sTlkmdiBtIapTimer;

/**
 * @brief       This function wakes up the IAP thread
 * @param[in]   none
 * @return      none
 */
static void tlkmdi_btiap_wakeUpThread(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_IAP2);
}

/**
 * @brief       This function checks if the system runs in RTOS
 * @param[in]   none
 * @return      true if runs in RTOS, false otherwise
 */
static bool tlkmdi_btiap_isRunInRtOS(void)
{
    return TLK_CFG_RTOS_ENABLE;
}

/**
 * @brief       This function handles the IAP timer event
 * @param[in]   pTimer    - timer handle
 * @param[in]   userArg   - user argument
 * @return      none
 */
static void tlkmdi_btiap_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    iAP2LinkRunLoopRunOnce(sTlkMdiBtIapRanLoop, (void *)(&sTlkMdiBtIapPacket));
}

/**
 * @brief       This function initializes the IAP module
 * @param[in]   none
 * @return      0 if success, otherwise error code
 */
int tlkmdi_btiap_init(void)
{
#if (TLK_DEV_MFI_ENABLE)
    int ret;
#endif

    tmemset(&sTlkMdiBtIapCtrl, 0, sizeof(tlkmdi_btiap_t));

#if (TLK_DEV_MFI_ENABLE)
    ret = tlkdev_mfi_init();
    if (ret == TLK_ENONE) {
        ret = tlkdev_mfi_open();
    }
    if (ret != TLK_ENONE) {
        tlkdev_mfi_close();
        return ret;
    }
#endif

    tlkmdi_btiap_setName((uint8_t *)"Telink-IAP", strlen("Telink-IAP"));
#if (IPA2_CONTROL_SESSION_VERSION == 2)
    test_synParam.sessionInfo[0].version = 2;
#endif
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkmdiBtIapTimer, 2 * 1000, true, tlkmdi_btiap_timer, NULL);
    btp_iap_regDataCB(tlkmdi_btiap_dataRecv);

    return TLK_ENONE;
}

/**
 * @brief       This function sets the BD address for IAP
 * @param[in]   bdaddr    - pointer to the BD address
 * @return      none
 */
void tlkmdi_btiap_setAddr(uint8_t bdaddr[6])
{
    tmemcpy(sTlkMdiBtIapCtrl.bdaddr, bdaddr, 6);
}

/**
 * @brief       This function sets the device name for IAP
 * @param[in]   pName     - pointer to the name
 * @param[in]   nameLen   - length of the name
 * @return      none
 */
void tlkmdi_btiap_setName(uint8_t *pName, uint8_t nameLen)
{
    if (pName == NULL || nameLen == 0) {
        return;
    }
    if (nameLen > 32) {
        nameLen = 32;
    }
    sTlkMdiBtIapCtrl.nameLen = nameLen;
    tmemcpy(sTlkMdiBtIapCtrl.name, pName, nameLen);
}

/**
 * @brief       This function sets ACL handle for IAP connection
 * @param[in]   isConn     - connection status
 * @param[in]   aclHandle  - ACL handle
 * @return      none
 */
void tlkmdi_btiap_setAclHandle(bool isConn, uint16_t aclHandle)
{
    tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_setAclHandle: %d 0x%x", isConn, aclHandle);
    tmemset(&sTlkMdiBtIapPacket, 0, sizeof(iAP2Packet_t));
    if (isConn && sTlkMdiBtIapCtrl.aclHandle == 0) {
        sTlkMdiBtIapCtrl.aclHandle = aclHandle;
        sTlkMdiBtIapCtrl.detFlag   = 0x55;

        iap2_memInit();
        memset(&sTlkMdiBtIapPacket, 0, sizeof(iAP2Packet_t));
        sTlkMdiBtIapPacket.pckData = &sTlkMdiBtIappktdata;

        sTlkMdiBtIapRanLoop = iAP2LinkRunLoopCreateAccessory(&test_synParam, (void *)0, (iAP2LinkSendPacketCB_t)tlkmdi_btiap_eap2SendDeal,
                                                             (iAP2LinkDataReadyCB_t)tlkmdi_btiap_eap2RecvDeal, (void *)0, (void *)0, 0, 1, NULL);
        if (sTlkMdiBtIapRanLoop != NULL) {
            sTlkMdiBtIapPacket.link = sTlkMdiBtIapRanLoop->link;
        }
    } else if (!isConn && sTlkMdiBtIapCtrl.aclHandle == aclHandle) {
        sTlkMdiBtIapCtrl.aclHandle = 0;
        sTlkMdiBtIapCtrl.detFlag   = 0xAA;
        sTlkMdiBtIapRanLoop        = NULL;
        sTlkMdiBtIapPacket.link    = NULL;
        sTlkMdiBtIapPacket.pckData = NULL;
    }
    tlkmdi_btiap_wakeUpThread();
}

/**
 * @brief       This function handles IAP events
 * @param[in]   none
 * @return      none
 */
void tlkmdi_btiap_handler(void)
{
    if (sTlkMdiBtIapCtrl.aclHandle == 0 || sTlkMdiBtIapRanLoop == NULL) {
        if (tlkmdi_btiap_isRunInRtOS() == true) {
            tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkmdiBtIapTimer);
        }
        return;
    }

    if (sTlkMdiBtIapCtrl.detFlag == 0x55) {
        uint8_t kIap2PacketDetectData1[] = {0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10};
        if (btp_iap_sendData(sTlkMdiBtIapCtrl.aclHandle, NULL, 0, kIap2PacketDetectData1, 6) == TLK_ENONE) {
            tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_handler: send_detect");
            sTlkMdiBtIapCtrl.detFlag = 0;
        }
    } else if (sTlkMdiBtIapCtrl.detFlag == 0xaa) {
        iAP2LinkRunLoopDetached(sTlkMdiBtIapRanLoop);
        sTlkMdiBtIapCtrl.detFlag = 0;
    }

    iAP2LinkRunLoopRunOnce(sTlkMdiBtIapRanLoop, (void *)(&sTlkMdiBtIapPacket));
    if (tlkmdi_btiap_isRunInRtOS() == false) {
        tlkmdi_btiap_wakeUpThread();
    } else {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkmdiBtIapTimer);
    }
}

/**
 * @brief       This function receives data from IAP connection
 * @param[in]   aclHandle   - ACL handle
 * @param[in]   rfcHandle   - RFCOMM handle
 * @param[in]   pData       - pointer to received data
 * @param[in]   dataLen     - length of received data
 * @return      none
 */
static void tlkmdi_btiap_dataRecv(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen)
{
    (void)aclHandle;
    (void)rfcHandle;
    uint8_t  detect;
    uint32_t failedChecksums;
    uint32_t sopdetect;

    if (sTlkMdiBtIapRanLoop == NULL) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_dataRecv error");
        return;
    }

    tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_dataRecv", pData, dataLen);
    iAP2PacketParseBuffer(pData, dataLen, &sTlkMdiBtIapPacket, 650, (BOOL *)&detect, &failedChecksums, &sopdetect);
    if (detect == 1) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "detect device");
        iAP2LinkRunLoopAttached(sTlkMdiBtIapRanLoop);
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "sTlkMdiBtIapPacket.bufferLen=%d", sTlkMdiBtIapPacket.bufferLen);
    }
}

/**
 * @brief       This function handles data sent callback
 * @param[in]   link      - iAP2 link
 * @param[in]   context   - context data
 * @return      none
 */
static void LinkDataSentCB(struct iAP2Link_st *link, void *context)
{
    (void)link;
    (void)context;
    tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "LinkDataSentCB");
}

/**
 * @brief       This function handles sending EAP2 packets
 * @param[in]   link     - iAP2 link
 * @param[in]   packet   - packet to send
 * @return      none
 */
static void tlkmdi_btiap_eap2SendDeal(struct iAP2Link_st *link, iAP2Packet_t *packet)
{
    (void)link;
    *(uint8_t *)((uint8_t *)packet->pckData + 8) = iAP2PacketCalcHeaderChecksum(packet);
    if (packet->packetLen > 9) {
        *(uint8_t *)((uint8_t *)packet->pckData + packet->packetLen - 1) = iAP2PacketCalcPayloadChecksum(packet); //packet->dataChecksum;
    }
    tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_eap2SendDeal data=", (uint8_t *)packet->pckData, packet->packetLen);
    btp_iap_sendData(sTlkMdiBtIapCtrl.aclHandle, NULL, 0, (uint8_t *)packet->pckData, packet->packetLen);
}

/**
 * @brief       This function handles receiving EAP2 packets
 * @param[in]   link      - iAP2 link
 * @param[in]   data      - received data
 * @param[in]   dataLen   - length of received data
 * @param[in]   session   - session identifier
 * @return      TRUE if handled successfully, FALSE otherwise
 */
static BOOL tlkmdi_btiap_eap2RecvDeal(struct iAP2Link_st *link, uint8_t *data, uint32_t dataLen, uint8_t session)
{
    BOOL     ret;
    uint16_t len;
    uint16_t pos         = 0;
    uint8_t  buffer[628] = {0xff, 0x5a, 2, 0x74, 0x40, 0x2d, 6, 0x0a, 0x2b, 0x40, 0x40, 0x02, 0x6a, 0xaa, 0x1, 2, 0x64, 0, 0};
    uint8_t  ack[]       = {0xff, 0x5a, 0, 0x9, 0x40, 0x2b, 0xdc, 0, 0x2c};
    uint16_t command     = (uint16_t)((data[4] << 8) | data[5]);

    tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "tlkmdi_btiap_eap2RecvDeal data=", data, dataLen);
    ret = true;
    if (command == IAP2_RequestAuthenticationCertificate && data[0] == IAP2_SESSION_START_MSB) {
        buffer[0] = 0x40;
        buffer[1] = 0x40;
        buffer[2] = 0x02;
        buffer[3] = 0x6a;
        buffer[4] = 0xaa;
        buffer[5] = 0x01;
        buffer[6] = 0x02;
        buffer[7] = 0x64;
        buffer[8] = 0x00;
        buffer[9] = 0x00;
#if (TLK_DEV_MFI_ENABLE)
        ret = tlkdev_mfi_loadCertificateData(&buffer[10], 608);
#endif
        len = 10 + 608;
        ret = iAP2LinkQueueSendData(link, buffer, len, session, NULL, LinkDataSentCB);
    } else if (command == IAP2_RequestAuthenticationChallengeResponse && data[0] == IAP2_SESSION_START_MSB) {
        buffer[0] = 0x40;
        buffer[1] = 0x40;
        buffer[2] = 0x00;
        buffer[3] = 0x4a;
        buffer[4] = 0xaa;
        buffer[5] = 0x03;
        buffer[6] = 0x00;
        buffer[7] = 0x44;
        buffer[8] = 0x00;
        buffer[9] = 0x00;
#if (TLK_DEV_MFI_ENABLE)
        tlkdev_mfi_loadChallengeData(&data[10], ((data[6] << 8) | data[7]) - 4, &buffer[10], 64);
#endif
        len = 10 + 64;
        ret = iAP2LinkQueueSendData(link, buffer, len, session, NULL, LinkDataSentCB);
    } else if (command == IAP2_AuthenticationSucceeded && data[0] == IAP2_SESSION_START_MSB) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "authentication succ ");
    } else if (command == IAP2_StartIdentification && data[0] == IAP2_SESSION_START_MSB) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_StartIdentification");
        //identification
        buffer[0] = 0x40;
        buffer[1] = 0x40;
        buffer[4] = 0x1d;
        buffer[5] = 0x01;
        buffer[6] = (uint8_t)((sTlkMdiBtIapCtrl.nameLen + 5) >> 8);
        buffer[7] = (uint8_t)(sTlkMdiBtIapCtrl.nameLen + 5);
        //param id =0 : name
        buffer[8] = 0;
        buffer[9] = 0;
        pos       = 10;
        if (sTlkMdiBtIapCtrl.nameLen != 0) {
            tmemcpy(&buffer[pos], sTlkMdiBtIapCtrl.name, sTlkMdiBtIapCtrl.nameLen);
        }
        pos += sTlkMdiBtIapCtrl.nameLen;
        buffer[pos] = 0;
        pos++;
        tmemcpy(&buffer[pos], identification_information_part1, sizeof(identification_information_part1));
        pos += (sizeof(identification_information_part1));
        tmemcpy(&buffer[pos], identification_information_part2, sizeof(identification_information_part2));
        pos += (sizeof(identification_information_part2));
        tmemcpy(&buffer[pos - 6], sTlkMdiBtIapCtrl.bdaddr, 6);
        buffer[2] = (uint8_t)(pos >> 8);
        buffer[3] = (uint8_t)pos;
        len       = pos;
        ret       = iAP2LinkQueueSendData(link, buffer, len, session, NULL, LinkDataSentCB);
    } else if (command == IAP2_IdentificationAccepted && data[0] == IAP2_SESSION_START_MSB) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "identification accept");
    } else if (command == IAP2_StartExternalAccessoryProtocolSession && data[0] == IAP2_SESSION_START_MSB) {
        tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_StartExternalAccessoryProtocolSession", data, dataLen);
        ack[5] = link->sentSeq;
        ack[6] = link->recvSeq;
        ack[8] = iAP2PacketCalcChecksum(ack, 8);
        tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_StartExternalAccessoryProtocolSession succ ack=", (uint8_t *)ack, 9);
        btp_iap_sendData(sTlkMdiBtIapCtrl.aclHandle, NULL, 0, (uint8_t *)ack, sizeof(ack));
        eap_session_id = (uint16_t)((uint16_t)(data[15] << 8) | (uint16_t)data[16]);
        eap_flag       = 0x55;
    } else if (command == IAP2_StopExternalAccessoryProtocolSession && data[0] == IAP2_SESSION_START_MSB) {
        tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_StopExternalAccessoryProtocolSession", data, dataLen);
        ack[5] = link->sentSeq;
        ack[6] = link->recvSeq;
        ack[8] = iAP2PacketCalcChecksum(ack, 8);
        tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_StopExternalAccessoryProtocolSession succ ack=", (uint8_t *)ack, 9);
        btp_iap_sendData(sTlkMdiBtIapCtrl.aclHandle, NULL, 0, (uint8_t *)ack, sizeof(ack));
        eap_flag = 0;
    }
    //EAP
    if (data[0] == (eap_session_id >> 8) && data[1] == (uint8_t)eap_session_id && eap_flag == 0x55) {
        tlkapi_trace(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "recv data", data, dataLen);
        ack[5] = link->sentSeq;
        ack[6] = link->recvSeq;
        ack[8] = iAP2PacketCalcChecksum(ack, 8);
        tlkapi_array(TLKMDI_BTIAP_DBG_FLAG, TLKMDI_BTIAP_DBG_SIGN, "IAP2_EAP succ ack=", (uint8_t *)ack, 9);
        btp_iap_sendData(sTlkMdiBtIapCtrl.aclHandle, NULL, 0, (uint8_t *)ack, sizeof(ack));
    }

    return ret;
}


#endif // #if (TLKBTP_CFG_IAP_ENABLE)
