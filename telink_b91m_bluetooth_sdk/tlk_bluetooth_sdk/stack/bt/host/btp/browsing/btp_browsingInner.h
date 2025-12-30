/********************************************************************************************************
 * @file    btp_browsingInner.h
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
#ifndef BTP_BROWSING_INNER_H
#define BTP_BROWSING_INNER_H


#define BTP_BROWSING_DBG_FLAG ((TLK_MAJOR_DBGID_BTP << 24) | (TLK_MINOR_DBGID_BTP_BROWSE << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define BTP_BROWSING_DBG_SIGN "[BROWSE]"

typedef enum
{
    BTP_BROWSING_FLAG_NONE                = 0x0000,
    BTP_BROWSING_BUSY_WAIT_SET_PLAYER_RSP = 0x0001,
} BTP_BROWSING_FLAGS_ENUM;

typedef enum
{
    BTP_BROWSING_BUSY_NONE                = 0x0000,
    BTP_BROWSING_BUSY_SEND_GENERAL_REJECT = 0x0001,
    BTP_BROWSING_BUSY_SEND_SET_PLAYER_CMD = 0x0002,
} BTP_BROWSING_BUSYS_ENUM;

typedef struct
{
    uint8_t  state;
    uint8_t  stage;
    uint8_t  usrID;
    uint8_t  ecode;
    uint16_t busys;
    uint16_t flags;

    uint8_t  fscIsEn;
    uint8_t  cmdTrid;
    uint8_t  rspTrid;
    uint8_t  rtnMode;
    uint16_t ehnCtrl;

    uint16_t chnID;
    uint16_t timeout;
    uint16_t aclHandle;

    uint16_t setPlayerID;

    TlkApiTimer_t timer;
} btp_browsing_item_t;

typedef struct
{
    btp_browsing_item_t *item;
} btp_browsing_ctrl_t;

int btp_browsing_innerInit(uint8_t count);

int btp_browsing_innerDeinit(void);

int btp_browsing_innerGetMemLen(uint8_t count);

/******************************************************************************
 * Function: BROWSING reset interface
 * Descript: This interface be used to reset the specify browsing control block.
 * Params:
 * Return:
 *******************************************************************************/
void btp_browsing_resetNode(btp_browsing_item_t *pAvrcp);

/******************************************************************************
 * Function: BROWSING get a unused browsing control block num interface
 * Descript: This interface be used to get the total unused control block
 *           num before a new connection need to setup.
 * Params:
 * Return: Returning value means the number of unused browsing control block.
 *******************************************************************************/
uint8_t btp_browsing_getIdleCount(void);

/******************************************************************************
 * Function: BROWSING get the number of used browsing control block interface
 * Descript: This interface be used to get the total used control block num.
 * Params:
 * Return: Returning value means the number of used browsing control block.
 *******************************************************************************/
uint8_t btp_browsing_getUsedCount(void);

/******************************************************************************
 * Function: BROWSING get the number of connected browsing control block interface
 * Descript: This interface be used to get the total connected control block num.
 * Params:
 * Return: Returning value means the number of connected browsing control block.
 *******************************************************************************/
uint8_t btp_browsing_getConnCount(void);

/******************************************************************************
 * Function: BROWSING get the browsing control block index interface
 * Descript: This interface be used to get the index connected control block.
 *           which specify by index.
 * Params:
 * Return: Returning value means the index of browsing control block.
 *******************************************************************************/
uint8_t btp_browsing_getNodeIndex(btp_browsing_item_t *pAvrcp);

/******************************************************************************
 * Function: BROWSING get the idle browsing control block interface
 * Descript: This interface be used to get the idle browsing control block.
 * Params:
 * Return: Returning a unused browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getNode(uint8_t index);

/******************************************************************************
 * Function: BROWSING get the idle browsing control block interface
 * Descript: This interface be used to get the idle browsing control block.
 * Params:
 * Return: Returning a unused browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getIdleNode(void);

/******************************************************************************
 * Function: BROWSING get the used browsing control block interface
 * Descript: This interface be used to get the used browsing control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the used browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getUsedNode(uint16_t aclHandle);

/******************************************************************************
 * Function: BROWSING get the connected browsing control block interface
 * Descript: This interface be used to get the connected browsing control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the connected browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getConnNode(uint16_t aclHandle);

/******************************************************************************
 * Function: BROWSING get the connected browsing control block interface
 * Descript: This interface be used to get the connected browsing control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getConnNodeByScid(uint16_t scid);

/******************************************************************************
 * Function: BROWSING get the connected browsing control block interface
 * Descript: This interface be used to get the connected browsing control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getConnNodeByNone(uint8_t offset);

/******************************************************************************
 * Function: BROWSING get the connected browsing control block interface
 * Descript: This interface be used to get the connected browsing control block
 *           by index.
 * Params:
 *        @index[IN]--The index of a browsing control block.
 * Return: Returning the connected browsing control block.
 *******************************************************************************/
btp_browsing_item_t *btp_browsing_getConnNodeByIndex(uint8_t index);


#endif // BTP_BROWSING_INNER_H
