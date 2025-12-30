/********************************************************************************************************
 * @file    btp_avctp.h
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
#ifndef BTP_AVCTP_H
#define BTP_AVCTP_H

typedef enum
{
    BTP_AVCTP_PKT_TYPE_SINGLE   = 0,
    BTP_AVCTP_PKT_TYPE_START    = 1,
    BTP_AVCTP_PKT_TYPE_CONTINUE = 2,
    BTP_AVCTP_PKT_TYPE_END      = 3,
} BTP_AVCTP_PKT_TYPE_ENUM;

/******************************************************************************
 * Function: AVRCP Send Command interface
 * Descript: Defines trigger the avrcp cmd transfer to peer avrcp entity.
 * Params:
 *        @aclHandle--The Acl Handle identifier.
 *        @scid--The service channel id.
 *        @trid--The transaction id.
 *        @pid--product id.
 *        @pHead--The data header which to transfer.
 *        @headLen--The header length.
 *        @pData--payload data.
 *        @dataLen--the data length.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_sendCmd(uint16_t aclHandle, uint16_t scid, uint8_t trid, uint16_t pid, uint8_t *pHead, uint8_t headLen, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: AVRCP Send response interface
 * Descript: Defines trigger the avrcp rsp transfer to peer avrcp entity.
 * Params:
 *        @aclHandle--The Acl Handle identifier.
 *        @scid--The service channel id.
 *        @trid--The transaction id.
 *        @pid--product id.
 *        @pHead--The data header which to transfer.
 *        @headLen--The header length.
 *        @pData--payload data.
 *        @dataLen--the data length.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_sendRsp(uint16_t aclHandle, uint16_t scid, uint8_t trid, uint16_t pid, uint8_t *pHead, uint8_t headLen, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: AVRCP Send error interface
 * Descript: Defines trigger the avrcp error rsp transfer to peer avrcp entity.
 * Params:
 *        @aclHandle--The Acl Handle identifier.
 *        @scid--The service channel id.
 *        @trid--The transaction id.
 *        @pid--product id.
 *        @pHead--The data header which to transfer.
 *        @headLen--The header length.
 *        @pData--payload data.
 *        @dataLen--the data length.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_sendErr(uint16_t aclHandle, uint16_t scid, uint8_t trid, uint16_t pid, uint8_t *pHead, uint8_t headLen, uint8_t *pData, uint8_t dataLen);


#endif // BTP_AVCTP_H
