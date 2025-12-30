/********************************************************************************************************
 * @file    btp_browsingSend.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef BTP_BROWSING_SEND_H
#define BTP_BROWSING_SEND_H


void btp_browsing_sendGeneralRejectProc(btp_browsing_item_t *pBrowsing);


int btp_browsing_sendCmd(uint16_t aclHandle, uint8_t trid, uint8_t pduID, uint8_t *pParam, uint16_t paramLen);
int btp_browsing_sendRsp(uint16_t aclHandle, uint8_t trid, uint8_t pduID, uint8_t *pParam, uint16_t paramLen);

int btp_browsing_sendGeneralReject(uint16_t aclHandle, uint8_t errCode);
int btp_browsing_setChangePathCmd(uint16_t aclHandle, uint16_t uidCounter, uint8_t direct, uint8_t *pUID);
int btp_browsing_setBrowsedPlayerCmd(uint16_t aclHandle, uint16_t playerID);
int btp_browsing_setBrowsedPlayerRsp(uint16_t aclHandle, uint8_t status);
int btp_browsing_sendGetFolderItemsCmd(uint16_t aclHandle, uint8_t scope, uint32_t startItem, uint32_t endItem, uint8_t attCount, uint32_t *pAttList);
int btp_browsing_sendGetItemAttrCmd(uint16_t aclHandle, uint8_t scope, uint8_t *pUids, uint8_t uidCounter, uint8_t attCount, uint32_t *pAttList);
int btp_browsing_sendGetTotalNumbOfItemsCmd(uint16_t aclHandle, uint8_t scope);
int btp_browsing_sendGetTotalNumbOfItemsRsp(uint16_t aclHandle, uint8_t status, uint16_t uidCounter, uint32_t number);


#endif // BTP_BROWSING_SEND_H
