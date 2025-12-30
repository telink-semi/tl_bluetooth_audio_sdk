/********************************************************************************************************
 * @file    btp_attInner.h
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
#ifndef BTP_ATT_INNER_H
#define BTP_ATT_INNER_H

#if (TLKBTP_CFG_ATT_ENABLE)


    #define BTP_ATT_DBG_FLAG ((TLK_MAJOR_DBGID_BTP << 24) | (TLK_MINOR_DBGID_BTP_ATT << 16) | TLK_DEBUG_DBG_FLAG_ALL)
    #define BTP_ATT_DBG_SIGN NULL

typedef struct
{
    uint8_t state;
    uint8_t usrID;
    uint16_t chnID;
    uint16_t busys;
    uint16_t flags;
    uint16_t handle;
    uint16_t mtuSize;
    uint8_t timeout;
    uint8_t reserve0;
    uint16_t reserve1;

    union
    {
        struct
        {
            uint8_t opcode;
            uint8_t reason;
            uint8_t resv000;
            uint8_t uuidLen;
            uint16_t roffset;
            uint16_t resv001;
            uint16_t mtuSize;
            uint16_t ahandle;
            uint16_t shandle; // Att Handle
            uint16_t ehandle;
            uint8_t attUUID[16];
        } server;

        struct
        {
            uint32_t resever;
        } client;
    } param;

    TlkApiTimer_t timer;
} btp_att_node_t;

typedef struct
{
    btp_att_node_t *item;
} btp_att_ctrl_t;

int btp_att_innerInit(uint8_t count);

int btp_att_innerDeinit(void);

int btp_att_innerGetMemLen(uint8_t count);

void btp_att_destroy(uint16_t aclHandle);


void btp_att_resetNode(btp_att_node_t *pItem);

uint8_t btp_att_getIdleCount(void);
uint8_t btp_att_getUsedCount(void);
uint8_t btp_att_getConnCount(void);

btp_att_node_t *btp_att_getIdleNode(void);
btp_att_node_t *btp_att_getBusyNode(uint16_t aclHandle);
btp_att_node_t *btp_att_getUsedNode(uint16_t aclHandle, uint8_t usrID);
btp_att_node_t *btp_att_getConnNode(uint16_t aclHandle, uint8_t usrID);
btp_att_node_t *btp_att_getUsedNodeByChnID(uint16_t aclHandle, uint16_t chnID);
btp_att_node_t *btp_att_getAnyUsedNode(uint16_t aclHandle);
btp_att_node_t *btp_att_getAnyConnNode(uint16_t aclHandle);


#endif // #if (TLKBTP_CFG_ATT_ENABLE)

#endif /* BTP_ATT_INNER_H */
