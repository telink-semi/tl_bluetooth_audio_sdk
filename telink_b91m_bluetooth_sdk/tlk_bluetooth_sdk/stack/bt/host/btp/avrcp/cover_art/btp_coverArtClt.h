/********************************************************************************************************
 * @file    btp_coverArtClt.h
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
#ifndef BTP_COVERART_CLT_H
#define BTP_COVERART_CLT_H

#if (TLKBTP_CFG_COVERARTCLT_ENABLE)

typedef enum
{
    BTP_COVERARTC_BUSY_NONE           = 0x00,
    BTP_COVERARTC_BUSY_SEND_CONN_REQ  = 0x0001,
    BTP_COVERARTC_BUSY_SEND_CONN_RSP  = 0x0002,
    BTP_COVERARTC_BUSY_SEND_DISC_REQ  = 0x0004,
    BTP_COVERARTC_BUSY_SEND_DISC_RSP  = 0x0008,
    BTP_COVERARTC_BUSY_SEND_ERR_RSP   = 0x0010,
    BTP_COVERARTC_BUSY_SEND_ABORT_REQ = 0x0020,

    BTP_COVERARTC_BUSY_SEND_GET_CONTINUE = 0x0100,
    BTP_COVERARTC_BUSY_SEND_GET_IMAGE    = 0x0200,
} BTP_COVERARTC_BUSYS_ENUM;

typedef enum
{
    BTP_COVERARTC_FLAG_NONE           = 0x0000,
    BTP_COVERARTC_FLAG_CONN           = 0x8000,
    BTP_COVERARTC_FLAG_WAIT_CONN_REQ  = 0x0001,
    BTP_COVERARTC_FLAG_WAIT_CONN_RSP  = 0x0002,
    BTP_COVERARTC_FLAG_WAIT_DISC_REQ  = 0x0004,
    BTP_COVERARTC_FLAG_WAIT_DISC_RSP  = 0x0008,
    BTP_COVERARTC_FLAG_WAIT_ABORT_RSP = 0x0010,

    BTP_COVERARTC_FLAG_WAIT_GET_IMAGE_HANDLE  = 0x0100,
    BTP_COVERARTC_FLAG_WAIT_GET_IMAGE         = 0x0200,
    BTP_COVERARTC_FLAG_ABORT_RESEND_GET_IMG   = 0x0400,
    BTP_COVERARTC_FLAG_WAIT_APP_IDLE_CONTINUE = 0x0800, // when app busy, wait for calling 'btp_coverArtclt_getImageGoOn' to get continue.
} BTP_COVERARTC_FLAGS_ENUM;

typedef enum
{
    BTP_COVERARTC_EVT_STATUS_DATA    = 0x00,
    BTP_COVERARTC_EVT_STATUS_ABORT   = 0x01,
    BTP_COVERARTC_EVT_STATUS_TIMEOUT = 0x02,
} BTP_COVERARTC_EVT_STATUS_ENUM;

int btp_coverArtclt_init(void);
/******************************************************************************
 * Function: COVERART Client connect interface
 * Descript: This interface be used by client user to setup a coverArt connection.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @channelID[IN]--The Channel ID.
 *        @isL2cap[IN]--Whether it is based on l2cap.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *
 * Note: avrcp cover art support obex over l2cap only.
 *******************************************************************************/
int btp_coverArtclt_connect(uint16_t aclHandle, uint16_t psmOrChn, bool isL2cap);
int btp_coverArtclt_connectWithAuth(uint16_t aclHandle, uint16_t psmOrChn, bool isL2cap);
/******************************************************************************
 * Function: COVERART Client disconnect interface
 * Descript: This interface be used by client user to disconnect coverArt connection.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_coverArtclt_disconn(uint16_t aclHandle);
/******************************************************************************
 * Function: COVERART destroy interface
 * Descript: This interface be used by client user to release the resource.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return:null.
 *******************************************************************************/
void btp_coverArtclt_destroy(uint16_t aclHandle);
void btp_coverArtclt_connectEvt(btp_coverArt_item_t *pCoverArt);
void btp_coverArtclt_disconnEvt(btp_coverArt_item_t *pCoverArt);
void btp_coverArtclt_recvDataCB(btp_coverArt_item_t *pCoverArt, uint8_t *pData, uint16_t dataLen);
/******************************************************************************
 * Function: COVERART Client start get image interface
 * Descript: This interface be used by client user to get coverArt data.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *
 * Note: avrcp cover art support obex over l2cap only.
 *******************************************************************************/
int  btp_coverArtclt_startGetImage(uint16_t aclHandle);
void btp_coverArtclt_sendAbortReq(btp_coverArt_item_t *pCoverArt);
void btp_coverArtclt_abortAndStartAgain(btp_coverArt_item_t *pCoverArt);

#endif // #if (TLKBTP_CFG_COVERARTCLT_ENABLE)

#endif // BTP_COVERART_CLT_H
