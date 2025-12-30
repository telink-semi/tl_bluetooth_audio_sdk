/********************************************************************************************************
 * @file    btp_custp.h
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
#ifndef BTP_CUSTP_H_
#define BTP_CUSTP_H_

#if (TLKBTP_CFG_CUSTP_ENABLE)

typedef void (*BtpCustpRecvDataCallback)(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: CUSTP initial interface
 * Descript: Defines trigger the initial flow of CUSTP sink and CUSTP source
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value(-TLK_ENOMEM) is returned means the initial process fail.
 *******************************************************************************/
int btp_custp_init(uint8_t count);

/******************************************************************************
 * Function: CUSTP deinitial interface
 * Descript: Defines trigger the initial flow of CUSTP sink and CUSTP source
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_custp_deinit(void);

/******************************************************************************
 * Function: btp_custp_getMemLen
 * Descript: CUSTP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_custp_getMemLen(uint8_t count);

/******************************************************************************
 * Function: btp_custp_connect
 * Descript: Defines trigger the custp connect cmd to peer custp entity and setup
 *           a connection with devices.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_custp_connect(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: btp_custp_disconn
 * Descript: Defines trigger the custp disconnect cmd to peer custp entity and
 *           tear up a connection which specify by aclhandle and usrid.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_custp_disconn(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: btp_custp_destroy
 * Descript: Defines trigger to release the resource which allocated.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: null
 *******************************************************************************/
void btp_custp_destroy(uint16_t aclHandle);

int btp_custp_sendData(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen);

void btp_custp_regRecvDataCB(BtpCustpRecvDataCallback datCallback);

#endif

#endif /* BTP_CUSTP_H_ */
