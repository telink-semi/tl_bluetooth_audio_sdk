/********************************************************************************************************
 * @file    btp_attSrv.h
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
#ifndef BTP_ATT_SRV_H
#define BTP_ATT_SRV_H

#if (TLKBTP_CFG_ATTSRV_ENABLE)


    #define BTP_ATTSRV_SERVICE_NUMB 8

    #define BTP_ATTSRV_BUFFER_SIZE  256

typedef enum
{
    BTP_ATTSRV_BUSY_NONE               = 0x00,
    BTP_ATTSRV_BUSY_SEND_ERROR_RSP     = 0x01,
    BTP_ATTSRV_BUSY_SEND_FIND_INFO     = 0x02,
    BTP_ATTSRV_BUSY_SEND_FIND_TYPE     = 0x04,
    BTP_ATTSRV_BUSY_SEND_READ_TYPE     = 0x08,
    BTP_ATTSRV_BUSY_SEND_READ_GROUP    = 0x10,
    BTP_ATTSRV_BUSY_SEND_WRITE_RSP     = 0x20,
    BTP_ATTSRV_BUSY_SEND_READ_RSP      = 0x40,
    BTP_ATTSRV_BUSY_SEND_READ_BLOB_RSP = 0x80,
} BTP_ATTSRV_BUSY_ENUM;

int btp_attsrv_init(void);

int btp_attsrv_connect(uint16_t aclHandle);
int btp_attsrv_disconn(uint16_t aclHandle);

void btp_attsrv_connectEvt(btp_att_node_t *pAtt);
void btp_attsrv_disconnEvt(btp_att_node_t *pAtt);

void btp_attsrv_recvHandler(btp_att_node_t *pAtt, uint8_t *pData, uint16_t dataLen);

int  btp_attsrv_setTable(const btp_attItem_t *pTable, uint16_t count);
uint btp_attsrv_getChnID(uint16_t aclHandle);


#endif // #if (TLKBTP_CFG_ATTSRV_ENABLE)

#endif /* BTP_ATT_SRV_H */
