/********************************************************************************************************
 * @file    btp_att.h
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
#ifndef BTP_ATT_H
#define BTP_ATT_H

#if (TLKBTP_CFG_ATT_ENABLE)


    #include "stack/bt/host/btp/att/btp_attStdio.h"

typedef struct
{
    uint8_t  attNum;
    uint8_t  permit;
    uint8_t  uuidLen;
    uint16_t  attrLen;
    uint8_t *pUUID;
    uint8_t *pAttr;
    uint8_t (*Read)(uint16_t handle, uint16_t chnID, uint8_t *pData, uint16_t dataLen);
    uint8_t (*Write)(uint16_t handle, uint16_t chnID, uint8_t *pData, uint16_t dataLen);
} btp_attItem_t, btp_gattItem_t;

/******************************************************************************
 * Function: ATT initial interface
 * Descript: Defines trigger the initial flow of A2DP sink and ATT source
 * Params:
 * 		@count[IN]--the ATT number, refer to TLK_BT_ATT_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int btp_att_init(uint8_t count);

/******************************************************************************
 * Function: ATT deinitial interface
 * Descript: Defines trigger the initial flow of A2DP sink and ATT source
 * Params:
 * 		@count[IN]--the ATT number, refer to TLK_BT_ATT_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_att_deinit(void);

/******************************************************************************
 * Function: btp_att_getMemLen
 * Descript: ATT get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the ATT number, refer to TLK_BT_ATT_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_att_getMemLen(uint8_t count);

int  btp_att_connect(uint16_t handle, uint8_t usrID);
int  btp_att_disconn(uint16_t handle, uint8_t usrID);
void btp_att_destroy(uint16_t handle);


extern int  btp_attsrv_setTable(const btp_attItem_t *pTable, uint16_t count);
extern uint btp_attsrv_getChnID(uint16_t aclHandle);


#endif // #if (TLKBTP_CFG_ATT_ENABLE)

#endif /* BTP_ATT_H */
