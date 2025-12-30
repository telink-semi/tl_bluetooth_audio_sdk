/********************************************************************************************************
 * @file    btp_pbap.h
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
#ifndef BTH_PBAP_H
#define BTH_PBAP_H


#define BTP_PBAP_CELL_NUMB_COUNT  3
#define BTP_PBAP_CALL_NAME_LENGTH 10
#define BTP_PBAP_CALL_TIME_LENGTH 16
#define BTP_PBAP_CELL_NUMB_LENGTH 16

typedef enum
{
    BTP_PBAP_BOOK_POSI_PHONE = 0,
    BTP_PBAP_BOOK_POSI_SIM1,
    BTP_PBAP_BOOK_POSI_SIM2,
    BTP_PBAP_BOOK_POSI_SIM3,
    BTP_PBAP_BOOK_POSI_SIM4,
    BTP_PBAP_BOOK_POSI_MAX,
} BTP_PBAP_BOOK_POSI_ENUM;

typedef enum
{
    BTP_PBAP_BOOK_TYPE_PHONE_BOOK = 0,
    BTP_PBAP_BOOK_TYPE_INCOMING_HISTORY,
    BTP_PBAP_BOOK_TYPE_OUTCOMING_HISTORY,
    BTP_PBAP_BOOK_TYPE_MISSED_CALL,
    BTP_PBAP_BOOK_TYPE_COMBINED_HISTORY,
    BTP_PBAP_BOOK_TYPE_MAX,
} BTP_PBAP_BOOK_TYPE_ENUM;

typedef enum
{
    BTP_PBAP_BOOK_SORT_INDEX = 0,
    BTP_PBAP_BOOK_SORT_ALPHA = 1,
    BTP_PBAP_BOOK_SORT_PHONE = 2,
} BTP_PBAP_BOOK_SORT_ENUM;

typedef struct
{
    uint8_t  callCnt;
    uint8_t  nameLen;
    uint8_t  bookType;                            // Refer to PBAP_BOOK_TYPE_ENUM.
    uint8_t  callType;                            // Refer to PBAP_CALL_TYPE_ENUM.
    uint16_t callName[BTP_PBAP_CALL_NAME_LENGTH]; // UNICODE
    uint8_t  cellNumb[BTP_PBAP_CELL_NUMB_COUNT][BTP_PBAP_CELL_NUMB_LENGTH];
    uint8_t  callTime[BTP_PBAP_CALL_TIME_LENGTH];
} btp_pbap_info_t;

typedef void (*BtpPbapcBookCallback)(uint16_t aclHandle, uint8_t endFlag, bool isSucc, btp_pbap_info_t *pInfo, uint8_t itemCount);


/******************************************************************************
 * Function: PBAP init interface
 * Descript: This interface be used by user to initial the pbap resource
 *           of client/server before create a connection between the entity.
 * Params:
 * 		@count[IN]--the PBAP number, refer to TLK_BT_PBAP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_pbap_init(uint8_t count);

/******************************************************************************
 * Function: PBAP Deinit interface
 * Descript: This interface be used by User to deinitial pbap resource.
 * Params: pPbap: refer to btp_pbap_item_t
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_pbap_deinit(void);

/******************************************************************************
 * Function: btp_pbap_getMemLen
 * Descript: PBAP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the PBAP number, refer to TLK_BT_PBAP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_pbap_getMemLen(uint8_t count);

int btp_pbap_connect(uint16_t aclHandle, uint8_t usrID, uint16_t psmOrChn, bool isL2cap);
int btp_pbap_disconn(uint16_t aclHandle, uint8_t usrID);

void btp_pbap_destroy(uint16_t aclHandle);


/*****************************************************************************************
** The Client PBAP interface
*****************************************************************************************/
/******************************************************************************
 * Function: PBAP Client connect interface
 * Descript: This interface be used by client user to setup a pbap connection.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @serviceID[IN]--The Service ID.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_pbapclt_connect(uint16_t aclHandle, uint16_t psmOrChn, bool isL2cap);

/******************************************************************************
 * Function: PBAP Client disconnect interface
 * Descript: This interface be used by client user to disconnect pbap connection.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @psmOrChn[IN]--The service psm.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_pbapclt_disconn(uint16_t aclHandle);

/******************************************************************************
 * Function: PBAP destroy interface
 * Descript: This interface be used by client user to release the resource.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return:null.
 *******************************************************************************/
extern void btp_pbapclt_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_pbapclt_getAnyHandle
 * Descript: Obtain any ACL Handle in the connected state.
 * Params:
 *        @offset[IN]--The starting position to get.
 * Return:null.
 *******************************************************************************/
extern uint btp_pbapclt_getAnyConnHandle(uint8_t offset);

/******************************************************************************
 * Function: PBAP register the phonebook function callback interface
 * Descript: This interface be used by user to register the pbap
 *           phonebook callback function of client/server.
 * Params:
 *        @BtpPbapcReadyCallback[IN]--The ready callback function.
 * Return: null.
 *******************************************************************************/
extern void btp_pbapclt_regBookCB(BtpPbapcBookCallback cb);

/******************************************************************************
 * Function: PBAP destroy interface
 * Descript: This interface be used by client user to release the resource.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @bookPosi[IN]--The storage position(Phone ram/sim card). Refer BTP_PBAP_BOOK_POSI_ENUM.
 *        @bookType[IN]--The Object type(phonebook or call record). Refer BTP_PBAP_BOOK_TYPE_ENUM.
 *        @bookSort[IN]--The sort type. Refer BTP_PBAP_BOOK_SORT_ENUM.
 *        @offset[IN]--The offset.
 *        @count[IN]--The count.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_pbapclt_startSyncBook(uint16_t aclHandle, int bookPosi, int bookType, int bookSort, uint16_t offset, uint16_t count);

/******************************************************************************
 * Function: Close sync phone book
 * Descript: This interface be used by client user to sync phone book.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @bookPosi[IN]--The storage position(Phone ram/sim card)
 *        @bookType[IN]--The Object type(phonebook or call record)
 *        @bookSort[IN]--The sort type
 *        @offset[IN]--The offset
 *        @count[IN]--The count
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_pbapclt_closeSyncBook(uint16_t aclHandle);


extern int btp_pbap_enableRtnMode(bool enable, uint8_t rtnMode);
extern int btp_pbapclt_sendGetReqTest(uint16_t aclHandle, bool isEnSram, bool isWait);
extern int btp_pbapclt_sendGetReqTest1(uint16_t aclHandle, bool isWait, bool isEnSram);
extern int btp_pbapclt_sendGetContinueTest(uint16_t aclHandle, bool isWait);
extern int btp_pbapclt_connectWithAuth(uint16_t aclHandle, uint16_t psmOrChn, bool isL2cap);
extern int btp_pbapclt_sendSetFolderTest(uint16_t aclHandle, uint8_t path, uint8_t type);
extern int btp_pbapclt_sendGetReqTest2(uint16_t aclHandle, bool isWait, bool isEnSram);
extern int btp_pbapclt_sendGetContinueTest2(uint16_t aclHandle, bool isWait, bool enable);

#endif
