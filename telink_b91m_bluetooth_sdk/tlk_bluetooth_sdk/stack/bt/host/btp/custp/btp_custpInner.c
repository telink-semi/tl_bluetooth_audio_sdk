/********************************************************************************************************
 * @file    btp_custpInner.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/btp_adapt.h"
#include "stack/bt/host/btp/custp/btp_custpInner.h"
#include "stack/bt/host/btp/custp/btp_custp.h"

#if (TLKBTP_CFG_CUSTP_ENABLE)

static uint8_t gBtpCustpNumber = 0;

static btp_custp_ctrl_t *spBtpCustpCtrl = NULL;

int btp_custp_innerInit(uint8_t count)
{
    uint32_t mallocSize;

    gBtpCustpNumber = 0;
    spBtpCustpCtrl  = NULL;
    if (count == 0) {
        return TLK_ENONE;
    }

    mallocSize     = btp_custp_innerGetMemLen(count);
    spBtpCustpCtrl = (btp_custp_ctrl_t *)tlkstk_calloc(mallocSize);
    if (spBtpCustpCtrl == NULL) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_innerInit: - malloc failure");
        return -TLK_ENOMEM;
    }

    gBtpCustpNumber      = count;
    spBtpCustpCtrl->item = (btp_custp_item_t *)(spBtpCustpCtrl + 1);

    return TLK_ENONE;
}

int btp_custp_innerDeinit(void)
{
    uint8_t index;

    if (spBtpCustpCtrl != NULL) {
        for (index = 0; index < gBtpCustpNumber; index++) {
            btp_custp_resetNode(&spBtpCustpCtrl->item[index]);
        }
        tlkstk_free(spBtpCustpCtrl);
        spBtpCustpCtrl  = NULL;
        gBtpCustpNumber = 0;
    }

    return TLK_ENONE;
}

int btp_custp_innerGetMemLen(uint8_t count)
{
    if (count == 0) {
        return 0;
    }
    return TLKSTK_BT_STRUCT_ALIGN_4(btp_custp_ctrl_t) + TLKSTK_BT_STRUCT_ALIGN_4(btp_custp_item_t) * count;
}

/******************************************************************************
 * Function: CUSTP reset interface
 * Descript: This interface be used to reset the specify custp control block.
 * Params:
 * Return:
 *******************************************************************************/
void btp_custp_resetNode(btp_custp_item_t *pCustp)
{
    if (pCustp == NULL) {
        return;
    }
    pCustp->state     = TLK_STATE_CLOSED;
    pCustp->chnID     = 0;
    pCustp->aclHandle = 0;
    btp_timer_stop(&pCustp->timer);
    tmemset(pCustp, 0, sizeof(btp_custp_item_t));
}

/******************************************************************************
 * Function: CUSTP get a unused custp control block num interface
 * Descript: This interface be used to get the total unused control block
 *           num before a new connection need to setup.
 * Params:
 * Return: Returning value means the number of unused custp control block.
 *******************************************************************************/
uint8_t btp_custp_getIdleCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CLOSED) {
            count++;
        }
    }
    return count;
}

/******************************************************************************
 * Function: CUSTP get the number of used custp control block interface
 * Descript: This interface be used to get the total used control block num.
 * Params:
 * Return: Returning value means the number of used custp control block.
 *******************************************************************************/
uint8_t btp_custp_getUsedCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state != TLK_STATE_CLOSED) {
            count++;
        }
    }
    return count;
}

/******************************************************************************
 * Function: CUSTP get the number of connected custp control block interface
 * Descript: This interface be used to get the total connected control block num.
 * Params:
 * Return: Returning value means the number of connected custp control block.
 *******************************************************************************/
uint8_t btp_custp_getConnCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CONNECT) {
            count++;
        }
    }
    return count;
}

/******************************************************************************
 * Function: CUSTP get the custp control block index interface
 * Descript: This interface be used to get the index connected control block.
 *           which specify by index.
 * Params:
 * Return: Returning value means the index of custp control block.
 *******************************************************************************/
uint8_t btp_custp_getNodeIndex(btp_custp_item_t *pCustp)
{
    uint8_t index;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (pCustp == &spBtpCustpCtrl->item[index]) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return 0;
    }
    return index;
}

/******************************************************************************
 * Function: CUSTP get the idle custp control block interface
 * Descript: This interface be used to get the idle custp control block.
 * Params:
 * Return: Returning a unused custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getNode(uint8_t index)
{
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the idle custp control block interface
 * Descript: This interface be used to get the idle custp control block.
 * Params:
 * Return: Returning a unused custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getIdleNode(void)
{
    uint8_t index;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CLOSED) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the used custp control block interface
 * Descript: This interface be used to get the used custp control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the used custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getUsedNode(uint16_t aclHandle)
{
    uint8_t index;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state != TLK_STATE_CLOSED && spBtpCustpCtrl->item[index].aclHandle == aclHandle) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by aclHandle.
 * Params:
 *        @aclhandle[IN]--The acl link handle.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNode(uint16_t aclHandle)
{
    uint8_t index;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CONNECT && spBtpCustpCtrl->item[index].aclHandle == aclHandle) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByScid(uint16_t scid)
{
    uint8_t index;
    for (index = 0; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CONNECT && spBtpCustpCtrl->item[index].chnID == scid) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by scid.
 * Params:
 *        @scid[IN]--The psm id.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByNone(uint8_t offset)
{
    uint8_t index;
    if (offset >= gBtpCustpNumber) {
        return NULL;
    }
    for (index = offset; index < gBtpCustpNumber; index++) {
        if (spBtpCustpCtrl->item[index].state == TLK_STATE_CONNECT) {
            break;
        }
    }
    if (index == gBtpCustpNumber) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

/******************************************************************************
 * Function: CUSTP get the connected custp control block interface
 * Descript: This interface be used to get the connected custp control block
 *           by index.
 * Params:
 *        @index[IN]--The index of a custp control block.
 * Return: Returning the connected custp control block.
 *******************************************************************************/
btp_custp_item_t *btp_custp_getConnNodeByIndex(uint8_t index)
{
    if (index >= gBtpCustpNumber) {
        return NULL;
    }
    if (spBtpCustpCtrl->item[index].state != TLK_STATE_CONNECT) {
        return NULL;
    }
    return &spBtpCustpCtrl->item[index];
}

#endif