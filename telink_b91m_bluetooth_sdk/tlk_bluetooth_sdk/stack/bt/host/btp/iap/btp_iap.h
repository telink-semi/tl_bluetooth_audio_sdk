/********************************************************************************************************
 * @file    btp_iap.h
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
#ifndef BTP_IAP_H
#define BTP_IAP_H


#define BTP_IAP_DBG_FLAG ((TLK_MAJOR_DBGID_BTP << 24) | (TLK_MINOR_DBGID_BTP_IAP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define BTP_IAP_DBG_SIGN NULL


typedef void (*BtpIapRecvDataCB)(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen);

typedef struct
{
    uint8_t state;
    uint8_t rfcHandle;
    uint16_t aclHandle;
    uint8_t credit;
    uint8_t resv001;
    uint16_t mtuSize;
} btp_iap_item_t;

typedef struct
{
    btp_iap_item_t *item;
} btp_iap_ctrl_t;

/******************************************************************************
 * Function: IAP initial interface
 * Descript: Defines trigger the initial flow of IAP source
 * Params:
 * 		@count[IN]--the IAP number, refer to TLK_BT_IAP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int btp_iap_init(uint8_t count);

/******************************************************************************
 * Function: IAP deinitial interface
 * Descript: Defines trigger the initial flow of IAP source
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_iap_deinit(void);

/******************************************************************************
 * Function: btp_iap_getMemLen
 * Descript: IAP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the IAP number, refer to TLK_BT_IAP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_iap_getMemLen(uint8_t count);

/******************************************************************************
 * Function: IAP Register Callback interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
void btp_iap_regDataCB(BtpIapRecvDataCB dataCB);


/******************************************************************************
 * Function: IAP send Connect interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_iap_connect(uint16_t aclHandle, uint8_t channel);

/******************************************************************************
 * Function: IAP send DisConnect interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_iap_disconn(uint16_t aclHandle);

/******************************************************************************
 * Function: IAP send Destroy interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
void btp_iap_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: IAP send Data interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_iap_sendData(uint16_t aclHandle, uint8_t *pHead, uint8_t headLen, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: IAP get Idle count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_iap_getIdleCount(void);

/******************************************************************************
 * Function: IAP get Used count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_iap_getUsedCount(void);

/******************************************************************************
 * Function: IAP get Connect count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_iap_getConnCount(void);

/******************************************************************************
 * Function: IAP get Idle interface
 * Descript:
 * Params:
 * Return:The iap control block.
 *******************************************************************************/
btp_iap_item_t *btp_iap_getIdleItem(void);

/******************************************************************************
 * Function: IAP get used interface
 * Descript:
 * Params:
 * Return:The iap control block.
 *******************************************************************************/
btp_iap_item_t *btp_iap_getUsedItem(uint16_t aclHandle);

/******************************************************************************
 * Function: IAP get Connected interface
 * Descript:
 * Params:
 * Return:The iap control block.
 *******************************************************************************/
btp_iap_item_t *btp_iap_getConnItem(uint16_t aclHandle);


#endif /* BTP_IAP_H */
