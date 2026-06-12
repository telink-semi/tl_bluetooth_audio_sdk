/********************************************************************************************************
 * @file    btp_spp.h
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
#ifndef BTP_SPP_H
#define BTP_SPP_H


typedef void (*BtpSppRecvDataCB)(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen);

typedef struct
{
    uint8_t  state;
    uint8_t  rfcHandle;
    uint16_t aclHandle;
    uint8_t  credit;
    uint8_t  resv001;
    uint16_t mtuSize;
    uint16_t channelID;
    uint16_t res;
} btp_spp_item_t;

typedef struct
{
    btp_spp_item_t *item;
} btp_spp_ctrl_t;

/******************************************************************************
 * Function: SPP Init interface
 * Descript: This interface be used by User to initial spp resource.
 * Params:
 * 		@count[IN]: refer to TLK_BT_SPP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int btp_spp_init(uint8_t count);

/******************************************************************************
 * Function: SPP deinit interface
 * Descript: This interface be used by User to deinitial spp resource.
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_spp_deinit(void);

/******************************************************************************
 * Function: btp_spp_getMemLen
 * Descript: SPP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]: refer to TLK_BT_SPP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_spp_getMemLen(uint8_t count);

/******************************************************************************
 * Function: SPP Register Callback interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
void btp_spp_regDataCB(BtpSppRecvDataCB dataCB);

/******************************************************************************
 * Function: SPP send Connect interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_spp_connect(uint16_t aclHandle, uint8_t channel);


/******************************************************************************
 * Function: SPP send DisConnect interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_spp_disconn(uint16_t aclHandle);

/******************************************************************************
 * Function: SPP send Destroy interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
void btp_spp_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_spp_sendRemoteLineStatus
 * Descript: This interface be used by profile or user to send RLS CMD.
 * Params:
 *     @aclHandle[IN]---The connection handle.
 *     @status[IN]--The L1-L4 bits indicates the Line Status.
 *         If L1 is set to 0, no error have occurred.
 *           L1 = 1 indicates the following errors:
 *         L2-L4:
 *           100 Overrun Error - Received character overwrote an unread character
 *           010 Parity Error - Received character's parity was incorrect
 *           001 Framing Error - a character did not terminate with a stop bit.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_spp_sendRemoteLineStatus(uint16_t aclHandle, uint8_t status);

/******************************************************************************
 * Function: SPP send Data interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
int btp_spp_sendData(uint16_t aclHandle, uint8_t *pHead, uint8_t headLen, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: SPP get Idle count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_spp_getIdleCount(void);

/******************************************************************************
 * Function: SPP get Used count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_spp_getUsedCount(void);

/******************************************************************************
 * Function: SPP get Used count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
btp_spp_item_t *btp_spp_getItem(uint8_t index);

/******************************************************************************
 * Function: SPP get Connect count interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
uint8_t btp_spp_getConnCount(void);

/******************************************************************************
 * Function: SPP get Idle interface
 * Descript:
 * Params:
 * Return:The spp idle control block.
 *******************************************************************************/
btp_spp_item_t *btp_spp_getIdleItem(void);

/******************************************************************************
 * Function: SPP get used interface
 * Descript:
 * Params:
 * Return:The spp used control block.
 *******************************************************************************/
btp_spp_item_t *btp_spp_getUsedItem(uint16_t aclHandle);

/******************************************************************************
 * Function: SPP get Connect interface
 * Descript:
 * Params:
 * Return:The spp connect control block.
 *******************************************************************************/
btp_spp_item_t *btp_spp_getConnItem(uint16_t aclHandle);


#endif /* BTP_SPP_H */
