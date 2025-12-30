/********************************************************************************************************
 * @file    btp_coverArtInner.h
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
#ifndef BTP_COVERART_INNER_H
#define BTP_COVERART_INNER_H

#if (TLKBTP_CFG_COVERART_ENABLE)

#define BTP_COVERART_DBG_FLAG ((TLK_MAJOR_DBGID_BTP << 24) | (TLK_MINOR_DBGID_BTP_COVERART << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define BTP_COVERART_DBG_SIGN NULL


int btp_coverArt_innerInit(uint8_t count);

int btp_coverArt_innerDeinit(void);

int btp_coverArt_innerGetMemLen(uint8_t count);

int btp_coverArt_enableRtnMode(bool enable, uint8_t rtnMode);


/******************************************************************************
 * Function: COVERART init interface
 * Descript: This interface be used by user to initial the coverArt resource
 *           of client/server before create a connection between the entity.
 * Params:
 *        @serviceID[IN]--The service id.
 *        @isRfcomm[IN]--The barar is rfcomm or l2cap.
 *        @usrOrSrv[IN]--The user id or service id.
 *        @psmOrChn[IN]--The psm id or Rfcomm Channel id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_coverArt_appendService(bool isRfcomm, uint16_t aclHandle, uint8_t usrOrSrv, uint16_t psmOrChn);

/******************************************************************************
 * Function: The Obex remove interface
 * Descript: Defines trigger the Obex unregiser the callback.
 * Params:
 *        @@serviceID[IN--The Acl Handle identifier.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_coverArt_removeService(bool isRfcomm, uint16_t aclHandle, uint8_t usrOrSrv, uint16_t psmOrChn);


/******************************************************************************
 * Function: COVERART reset interface
 * Descript: This interface be used by user to reset the coverArt resource.
 * Params:
 *        @pCoverArt[IN]--The coverArt item.
 * Return: null.
 *******************************************************************************/
void btp_coverArt_resetNode(btp_coverArt_item_t *pItem);

/******************************************************************************
 * Function: COVERART get Idle Pbap control block interface
 * Descript: This interface be used by user to get the idle coverArt resource count.
 * Params:
 * Return: The coverArt item count.
 *******************************************************************************/
uint8_t btp_coverArt_getIdleNodeCount(void);

/******************************************************************************
 * Function: COVERART get Used Pbap control block interface
 * Descript: This interface be used by user to get the Used coverArt resource count.
 * Params:
 * Return: The coverArt item count.
 *******************************************************************************/
uint8_t btp_coverArt_getUsedNodeCount(void);

/******************************************************************************
 * Function: COVERART get Connected Pbap control block count interface
 * Descript: This interface be used by user to get the Connected coverArt resource count.
 * Params:
 * Return: The coverArt item count.
 *******************************************************************************/
uint8_t btp_coverArt_getConnNodeCount(void);

/******************************************************************************
 * Function: COVERART get Used Pbap control block interface
 * Descript: This interface be used by user to get the Used coverArt resource count.
 * Params:
 * Return: The coverArt item count.
 *******************************************************************************/
uint8_t btp_coverArt_getUsedNodeCountByUsrID(uint8_t usrID);

/******************************************************************************
 * Function: COVERART get Connected Pbap control block interface
 * Descript: This interface be used by user to get the Used coverArt resource count.
 * Params:
 * Return: The coverArt item count.
 *******************************************************************************/
uint8_t btp_coverArt_getConnNodeCountByUsrID(uint8_t usrID);


/******************************************************************************
 * Function: COVERART get Idle Pbap control block interface
 * Descript: This interface be used by user to get the idle coverArt resource.
 * Params:
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getIdleNode(void);

/******************************************************************************
 * Function: COVERART get Used Pbap control block interface
 * Descript: This interface be used by user to get the Used coverArt resource.
 * Params:
 *        @aclHandle[IN]--The acl link identifier.
 *        @usrID[IN]--The user id(C/S).
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getUsedNode(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: COVERART get Connected Pbap control block interface
 * Descript: This interface be used by user to get the Connected coverArt resource.
 * Params:
 *        @aclHandle[IN]--The acl link identifier.
 *        @usrID[IN]--The user id(C/S).
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getConnNode(uint16_t aclHandle, uint8_t usrId);

/******************************************************************************
 * Function: COVERART get Used Pbap control block interface
 * Descript: This interface be used by user to get the Used coverArt resource.
 * Params:
 *        @aclHandle[IN]--The acl link identifier.
 *        @channel[IN]--The channel id(C/S).
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getUsedNodeByChannel(uint16_t aclHandle, uint16_t channel, uint8_t ctype);

/******************************************************************************
 * Function: COVERART get Conn Pbap control block interface
 * Descript: This interface be conn by user to get the Used coverArt resource.
 * Params:
 *        @aclHandle[IN]--The acl link identifier.
 *        @channel[IN]--The channel id(C/S).
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getConnNodeByChannel(uint16_t aclHandle, uint16_t channel, uint8_t ctype);

/******************************************************************************
 * Function: COVERART get Connected Pbap control block interface
 * Descript: This interface be used by user to get the Connected coverArt resource.
 * Params:
 *        @aclHandle[IN]--The acl link identifier.
 *        @usrID[IN]--The user id(C/S).
 * Return: The coverArt item(success)/NULL(false).
 *******************************************************************************/
btp_coverArt_item_t *btp_coverArt_getAnyConnNodeByUsrID(uint8_t usrId, uint8_t offset);

#endif // #if (TLKBTP_CFG_COVERART_ENABLE)

#endif // BTP_COVERART_INNER_H
