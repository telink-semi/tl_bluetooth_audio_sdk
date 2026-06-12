/********************************************************************************************************
 * @file    btp_coverArt.h
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
#ifndef BTP_COVERART_H
#define BTP_COVERART_H

#if (TLKBTP_CFG_COVERART_ENABLE)
#include "../../obex/btp_obex.h"

#define IMAGE_HANDLE_LEN_MAX 8
#define IMAGE_TITLE_LEN_MAX  128
#define IMAGE_ARTIST_LEN_MAX 128

typedef struct
{
    uint8_t  imgHandleLen;
    uint32_t totalLen;
    uint8_t  imgTitle[IMAGE_TITLE_LEN_MAX];
    uint8_t  imgArtist[IMAGE_ARTIST_LEN_MAX];
    uint8_t  imgAblum[IMAGE_ARTIST_LEN_MAX];
    uint8_t  imgHandle[IMAGE_HANDLE_LEN_MAX];
} btp_coverArt_unit_t;

typedef struct
{
    uint8_t             state;
    uint8_t             usrID;
    uint8_t             active;
    uint8_t             errRsp;
    uint16_t            busys;
    uint16_t            flags;
    uint8_t             enAuth;
    uint8_t             rspWait; // For Test
    uint16_t            timeout;
    uint8_t             isReport;
    btp_obex_t          obex;
    TlkApiTimer_t       timer;
    btp_coverArt_unit_t unit;
} btp_coverArt_item_t;

typedef struct
{
    btp_coverArt_item_t *item;
} btp_coverArt_ctrl_t;

/******************************************************************************
 * Function: COVERART init interface
 * Descript: This interface be used by user to initial the coverArt resource
 *           of client/server before create a connection between the entity.
 * Params:
 * 		@count[IN]--the coverArt number, refer to TLK_BT_COVERART_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_coverArt_init(uint8_t count);
/******************************************************************************
 * Function: COVERART Deinit interface
 * Descript: This interface be used by User to deinitial coverArt resource.
 * Params: pcoverArt: refer to btp_coverArt_item_t
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int  btp_coverArt_deinit(void);
int  btp_coverArt_connect(uint16_t aclHandle, uint8_t usrID, uint16_t psmOrChn, bool isL2cap);
int  btp_coverArt_disconn(uint16_t aclHandle, uint8_t usrID);
void btp_coverArt_destroy(uint16_t aclHandle);

extern int  btp_coverArt_innerInit(uint8_t count);
extern int  btp_coverArtclt_init(void);
extern int  btp_coverArt_innerDeinit(void);
extern int  btp_coverArtclt_connect(uint16_t aclHandle, uint16_t psmOrChn, bool isL2cap);
extern int  btp_coverArtclt_disconn(uint16_t aclHandle);
extern void btp_coverArtclt_destroy(uint16_t aclHandle);
extern void btp_coverArtclt_getImageGoOn(btp_coverArt_item_t *pCoverArt);

#endif // #if (TLKBTP_CFG_COVERART_ENABLE)

#endif // BTP_COVERART_H
