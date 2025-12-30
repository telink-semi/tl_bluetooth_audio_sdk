/********************************************************************************************************
 * @file    btp_custpInner.h
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
#ifndef BTP_CUSTPINNER_H_
#define BTP_CUSTPINNER_H_

#if (TLKBTP_CFG_CUSTP_ENABLE)

#define BTP_CUSTP_DBG_FLAG 0xFFFFFFFF
#define BTP_CUSTP_DBG_SIGN "[custp]"

typedef struct
{
    uint8_t state;
    uint8_t stage;
    uint8_t usrID;
    uint8_t ecode;

    uint8_t  cmdTrid;
    uint8_t  rspTrid;
    uint16_t chnID;

    uint16_t evtBusy;
    uint16_t cmdBusy;
    uint16_t evtFlag;
    uint16_t cmdFlag;

    uint16_t timeout;
    uint16_t aclHandle;

    TlkApiTimer_t timer;
} btp_custp_item_t;

typedef struct
{
    btp_custp_item_t *item;
} btp_custp_ctrl_t;

int btp_custp_innerInit(uint8_t count);

int btp_custp_innerDeinit(void);

int btp_custp_innerGetMemLen(uint8_t count);

/******************************************************************************
 * Function: CUSTP reset interface
 * Descript: This interface be used to reset the specify custp control block.
 * Params:
 * Return:
 *******************************************************************************/
void btp_custp_resetNode(btp_custp_item_t *pCustp);

/******************************************************************************
 * Function: CUSTP get a unused custp control block num interface
 * Descript: This interface be used to get the total unused control block
 *           num before a new connection need to setup.
 * Params:
 * Return: Returning value means the number of unused custp control block.
 *******************************************************************************/
uint8_t btp_custp_getIdleCount(void);

/******************************************************************************
 * Function: CUSTP get the number of used custp control block interface
 * Descript: This interface be used to get the total used control block num.
 * Params:
 * Return: Returning value means the number of used custp control block.
 *******************************************************************************/
uint8_t btp_custp_getUsedCount(void);

/******************************************************************************
 * Function: CUSTP get the number of connected custp control block interface
 * Descript: This interface be used to get the total connected control block num.
 * Params:
 * Return: Returning value means the number of connected custp control block.
 *******************************************************************************/
uint8_t btp_custp_getConnCount(void);

/******************************************************************************
 * Function: CUSTP get the custp control block index interface
 * Descript: This interface be used to get the index connected control block.
 *           which specify by index.
 * Params:
 * Return: Returning value means the index of custp control block.
 *******************************************************************************/
uint8_t btp_custp_getNodeIndex(btp_custp_item_t *pCustp);

/******************************************************************************
 * Function: CUSTP get the idle custp control block interface
 * Descript: This interface be used to get the idle custp control block.
 * Params:
 * Return: Returning a unused custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getNode(uint8_t index);

/******************************************************************************
 * Function: CUSTP get the idle custp control block interface
 * Descript: This interface be used to get the idle custp control block.
 * Params:
 * Return: Returning a unused custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getIdleNode(void);

/******************************************************************************
 * Function: CUSTP get the used custp control block interface
 * Descript: This interface be used to get the used custp control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the used custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getUsedNode(uint16_t aclHandle);

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNode(uint16_t aclHandle);

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByScid(uint16_t scid);

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByNone(uint8_t offset);

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by index.
 * Params:
 *        @index[IN]--The index of a custp control block.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByIndex(uint8_t index);
#endif
#endif /* BTP_CUSTPINNER_H_ */
