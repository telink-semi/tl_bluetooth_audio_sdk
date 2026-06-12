/********************************************************************************************************
 * @file    tlkmdi_btpbap.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmdi_btpbap.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/pbap/btp_pbap.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"

#if (TLKBTP_CFG_PBAP_ENABLE)
#define TLKMDI_BTPBAP_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_PBAP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTPBAP_DBG_SIGN "[MDI]"

#if (TLKBTP_CFG_PBAPCLT_ENABLE)
void        tlkmdi_pbap_printBook(btp_pbap_info_t *pInfo);
static void tlkmdi_pbap_saveBookOver(bool isSucc);
static void tlkmdi_pbap_phoneBookReportCB(uint16_t aclHandle, uint8_t endFlag, bool isSucc, btp_pbap_info_t *pInfo, uint8_t itemCount);
#endif // #if (TLKBTP_CFG_PBAPCLT_ENABLE)
void tlkmdi_pbap_phoneBookCleanInfo(uint8_t *btMac);

static tlkmdi_phoneBookCtrl_t sTlkMdiPhoneBookCtrl;

/**
 * @brief Initializes PBAP related resources.
 *
 * @param none
 *
 * @return none
 */
void tlkmdi_pbap_init(void)
{
    tlkmdi_pbap_phoneBookinit();
}

/**
 * @brief Phone book resource initialization.
 *
 * @param none
 *
 * @return Return TLK_ENONE is success/other is failure.
 */
int tlkmdi_pbap_phoneBookinit(void)
{
    uint8_t  addr[6];
    uint16_t bookCount;

    tmemset(addr, 0, 6);
    tmemset(&sTlkMdiPhoneBookCtrl, 0, sizeof(tlkmdi_phoneBookCtrl_t));
    btp_pbapclt_regBookCB(tlkmdi_pbap_phoneBookReportCB);
    int ret = tlkmdi_tinySql_getPhoneBookMac(addr);
    if (ret != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookinit: getPhoneBookMac failure %d", ret);
    }
    bookCount = tlkmdi_tinySql_getPhoneBookItemNum();
    if (bookCount > TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB) {
        bookCount = TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB;
    }
    sTlkMdiPhoneBookCtrl.isReady   = true;
    sTlkMdiPhoneBookCtrl.bookCount = bookCount;
    tmemcpy(sTlkMdiPhoneBookCtrl.btAddr, addr, 6);
    sTlkMdiPhoneBookCtrl.posi   = BTP_PBAP_BOOK_POSI_PHONE;
    sTlkMdiPhoneBookCtrl.type   = BTP_PBAP_BOOK_TYPE_PHONE_BOOK;
    sTlkMdiPhoneBookCtrl.sort   = BTP_PBAP_BOOK_SORT_INDEX;
    sTlkMdiPhoneBookCtrl.offset = 0x0000;
    sTlkMdiPhoneBookCtrl.number = 0xFFFF;

    return TLK_ENONE;
}

/**
 * @brief       Verify that phone book information is valid.      
 * @param[in]   pBtAddr    -- The Bt address.
 * @return      Return true is valid, other value is invalid.
 * @note        
 */
bool tlkmdi_pbap_phoneBookInfoValid(uint8_t *pBtAddr)
{
    if (!sTlkMdiPhoneBookCtrl.isReady || pBtAddr == NULL) {
        return false;
    }
    if (tmemcmp(sTlkMdiPhoneBookCtrl.btAddr, pBtAddr, 6) == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       Clear the phone book information saved in the flash.
 *
 * @param none
 *
 * @return none
 */
void tlkmdi_pbap_phoneBookCleanInfo(uint8_t *btMac)
{
#if (TLKBTP_CFG_PBAPCLT_ENABLE)
    unsigned int baseAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK3_ADDR);
    if (baseAddress == 0) {
        return;
    }
    if (sTlkMdiPhoneBookCtrl.isBusy) {
        return;
    }
    sTlkMdiPhoneBookCtrl.isReady   = false;
    sTlkMdiPhoneBookCtrl.bookCount = 0;
    tlkmdi_tinySql_newPhoneBook(btMac);
#endif
}

/**
 * @brief       Start synchronizing the phone book.
 * @param[in]   aclHandle    -- The acl handle. 
 * @param[in]   pBtAddr      -- The device's Bt address.
 * @param[in]   isForce      -- Is force to sync the pb info.
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookStartSync(uint16_t aclHandle, uint8_t *pBtAddr, bool isForce)
{
#if (TLKBTP_CFG_PBAPCLT_ENABLE)
    int ret;

    if (aclHandle == 0 || pBtAddr == NULL) {
        tlkapi_error(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStartSync: failure - param");
        return -TLK_EPARAM;
    }
    if (sTlkMdiPhoneBookCtrl.isBusy) {
        tlkapi_error(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStartSync: failure - busy");
        return -TLK_EBUSY;
    } else {
        if (tmemcmp(sTlkMdiPhoneBookCtrl.btAddr, pBtAddr, 6) != 0) {
            tlkmdi_tinySql_newPhoneBook(pBtAddr);
        }
    }
    if (sTlkMdiPhoneBookCtrl.isReady) {
        if (!isForce && tmemcmp(sTlkMdiPhoneBookCtrl.btAddr, pBtAddr, 6) == 0) {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStartSync: failure - exist");
            btp_pbap_disconn(aclHandle, 3); //3 BTP_USRID_CLIENT
            return TLK_ENONE;
        }
        tlkmdi_pbap_phoneBookCleanInfo(pBtAddr);
    }

    sTlkMdiPhoneBookCtrl.bookCount = 0;
    sTlkMdiPhoneBookCtrl.aclHandle = aclHandle;
    tmemcpy(sTlkMdiPhoneBookCtrl.btAddr, pBtAddr, 6);
    ret = btp_pbapclt_startSyncBook(aclHandle, sTlkMdiPhoneBookCtrl.posi, sTlkMdiPhoneBookCtrl.type, sTlkMdiPhoneBookCtrl.sort, sTlkMdiPhoneBookCtrl.offset,
                                    sTlkMdiPhoneBookCtrl.number);
    if (ret != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStartSync: failure - reject");
    } else {
        sTlkMdiPhoneBookCtrl.isBusy = true;
        tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStartSync: success");
    }
    return ret;
#else
    (void)aclHandle;
    (void)pBtAddr;
    (void)isForce;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Stop the phone book synchronization.
 * @param[in]   aclHandle   -- The acl handle. 
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookStopSync(uint16_t aclHandle)
{
#if (TLKBTP_CFG_PBAPCLT_ENABLE)
    int ret;
    tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookStopSync ");
    if (!sTlkMdiPhoneBookCtrl.isBusy) {
        return TLK_ENONE;
    }
    if (aclHandle == 0 || sTlkMdiPhoneBookCtrl.aclHandle != aclHandle) {
        return -TLK_EPARAM;
    }
    sTlkMdiPhoneBookCtrl.isBusy    = false;
    sTlkMdiPhoneBookCtrl.aclHandle = 0;
    ret                            = btp_pbapclt_closeSyncBook(aclHandle);
    if (!sTlkMdiPhoneBookCtrl.isReady) {
        tlkmdi_pbap_phoneBookCleanInfo(sTlkMdiPhoneBookCtrl.btAddr);
    }
    return ret;
#else
    (void)aclHandle;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Set phone book parameters.
 * @param[in]   posi    -- The position.
 * @param[in]   type
 * @param[in]   sort
 * @param[in]   offset
 * @param[in]   number
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookSetParam(uint8_t posi, uint8_t type, uint8_t sort, uint16_t offset, uint16_t number)
{
    if (sTlkMdiPhoneBookCtrl.isBusy) {
        tlkapi_error(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookSetParam: failure - busy");
        return -TLK_EBUSY;
    }
    if (number == 0) {
        number = 0xFFFF;
    }

    tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookSetParam: {posi-%d,type-%d,sort-%d,count-%d}", posi, type, sort, number);

    sTlkMdiPhoneBookCtrl.posi   = posi;
    sTlkMdiPhoneBookCtrl.type   = type;
    sTlkMdiPhoneBookCtrl.sort   = sort;
    sTlkMdiPhoneBookCtrl.offset = offset;
    sTlkMdiPhoneBookCtrl.number = number;

    return TLK_ENONE;
}

/**
 * @brief       Get the contact name.
 * @param[in]   pNumber    -- The position.
 * @param[in]   numbLen
 * @param[in]   pName
 * @param[in]   nameLen
 * @param[in]   pGetLen
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookGetName(uint8_t *pNumber, uint8_t numbLen, uint8_t *pName, uint8_t nameLen, uint8_t *pGetLen)
{
#if (TLKBTP_CFG_PBAPCLT_ENABLE)
    int          ret;
    uint32_t     saddr;
    uint16_t     iIndex;
    uint8_t      jIndex;
    uint8_t      offset;
    uint8_t      buffer[TLKMDI_PHONE_BOOK_ITEM_LENGTH] = {0};
    unsigned int pbapAddr;
    /* TODO: flash save policy modify */
    // if (pbapAddr == 0) return -TLK_EFAIL;

    //	tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookGetName: %d, %d, %d, %d",
    //		        sTlkMdiPhoneBookCtrl.isReady, numbLen, sTlkMdiPhoneBookCtrl.bookCount, nameLen);
    if (!sTlkMdiPhoneBookCtrl.isReady) {
        return -TLK_ENOREADY;
    }
    if (pNumber == NULL || numbLen == 0 || numbLen > BTP_PBAP_CELL_NUMB_LENGTH || pName == NULL || nameLen == 0) {
        return -TLK_EPARAM;
    }
    saddr = pbapAddr + TLKMDI_PHONE_BOOK_HEADLEN;
    for (iIndex = 0; iIndex < sTlkMdiPhoneBookCtrl.bookCount; iIndex++) {
        /* TODO: flash save policy modify */
        offset = BTP_PBAP_CALL_NAME_LENGTH * 2;
        //		tlkapi_array(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_bookGetName: phone - ", &buffer[offset], 11);
        for (jIndex = 0; jIndex < BTP_PBAP_CELL_NUMB_COUNT; jIndex++) {
            if (tmemcmp(&buffer[offset], pNumber, numbLen) == 0) {
                break;
            }
            offset += BTP_PBAP_CELL_NUMB_LENGTH;
        }
        if (jIndex != BTP_PBAP_CELL_NUMB_COUNT) {
            break;
        }
        saddr += TLKMDI_PHONE_BOOK_ITEM_LENGTH;
    }
    if (iIndex == sTlkMdiPhoneBookCtrl.bookCount) {
        return -TLK_EFAIL;
    }

    ret = tlkapi_wcharStrlen(buffer, BTP_PBAP_CALL_NAME_LENGTH * 2);
    if (nameLen > ret) {
        nameLen = ret;
    }
    tmemcpy(pName, buffer, nameLen);
    if (pGetLen != NULL) {
        *pGetLen = nameLen;
    }
    return TLK_ENONE;
#else
    (void)pNumber;
    (void)numbLen;
    (void)pName;
    (void)nameLen;
    (void)pGetLen;
    return -TLK_ENOSUPPORT;
#endif
}

#if (TLKBTP_CFG_PBAPCLT_ENABLE)
static void tlkmdi_pbap_phoneBookReportCB(uint16_t aclHandle, uint8_t endFlag, bool isSucc, btp_pbap_info_t *pInfo, uint8_t itemCount)
{
    uint8_t index;
    uint8_t infoBuffer[1024];
    if (!sTlkMdiPhoneBookCtrl.isBusy || aclHandle != sTlkMdiPhoneBookCtrl.aclHandle) {
        tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB: error status, isBusy %d aclHandle %d PhoneBookCtrl.aclHandle %d itemCount %d",
                     sTlkMdiPhoneBookCtrl.isBusy, aclHandle, sTlkMdiPhoneBookCtrl.aclHandle, itemCount);
        btp_pbapclt_closeSyncBook(aclHandle);
        return;
    }

    if (endFlag) {
        if (isSucc) {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[END][SUCC]: bookCount-%d", sTlkMdiPhoneBookCtrl.bookCount);
        } else {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[END][FAIL]: endFlag-%d", endFlag);
        }
    } else {
        if (isSucc) {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[SUCC]: itemCount-%d", itemCount);
        } else {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[FAIL]: ");
        }
    }
    if (!isSucc) {
        tlkmdi_pbap_saveBookOver(false);
        tlkmdi_pbap_phoneBookStopSync(aclHandle);
        return;
    }
    if (!sTlkMdiPhoneBookCtrl.isBusy) {
        return;
    }
    if (sTlkMdiPhoneBookCtrl.bookCount >= TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB) {
        itemCount = 0;
    } else if (sTlkMdiPhoneBookCtrl.bookCount + itemCount > TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB) {
        itemCount = TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB - sTlkMdiPhoneBookCtrl.bookCount;
    }

    tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB 01: itemCount-%d, bookCount-%d", itemCount, sTlkMdiPhoneBookCtrl.bookCount);
    tmemset(infoBuffer, 0, sizeof(infoBuffer));
    for (index = 0; index < itemCount; index++) {
        if (pInfo->callType == sTlkMdiPhoneBookCtrl.type) {
            tmemcpy(infoBuffer + index * sizeof(btp_pbap_info_t), (void *)&pInfo[index], sizeof(btp_pbap_info_t));
            if (0) {
                tlkmdi_pbap_printBook(&pInfo[index]);
            }
            sTlkMdiPhoneBookCtrl.bookCount++;
        }
    }
    tlkmdi_tinySql_addPbapItemBlock(endFlag, itemCount, infoBuffer, sizeof(btp_pbap_info_t) * itemCount);
    if (endFlag || sTlkMdiPhoneBookCtrl.bookCount >= TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB) {
        if (endFlag) {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[COUNT]: normal stop- bookCount-%d", sTlkMdiPhoneBookCtrl.bookCount);
        } else {
            tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "tlkmdi_pbap_phoneBookReportCB[COUNT]: overflow stop- bookCount-%d", sTlkMdiPhoneBookCtrl.bookCount);
        }
        tlkmdi_pbap_saveBookOver(isSucc);
        tlkmdi_pbap_phoneBookStopSync(aclHandle);
    }
}

static void tlkmdi_pbap_saveBookOver(bool isSucc)
{
    if (!sTlkMdiPhoneBookCtrl.isBusy) {
        return;
    }
    sTlkMdiPhoneBookCtrl.isBusy = false;
    if (!isSucc) {
        sTlkMdiPhoneBookCtrl.isReady = false;
        tlkmdi_pbap_phoneBookCleanInfo(sTlkMdiPhoneBookCtrl.btAddr);
        tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "PbapSaveOver: erase sectors");
    } else {
        sTlkMdiPhoneBookCtrl.isReady = true;
    }
    btp_pbap_disconn(sTlkMdiPhoneBookCtrl.aclHandle, 3); //3 BTP_USRID_CLIENT
    sTlkMdiPhoneBookCtrl.aclHandle = 0;
}

void tlkmdi_pbap_printBook(btp_pbap_info_t *pInfo)
{
    tlkapi_trace(TLKMDI_BTPBAP_DBG_FLAG, TLKMDI_BTPBAP_DBG_SIGN, "   book: name[%d][%4x-%4x-%4x-%4x]; numb[%d][%c%c%c%c%c%c%c%c%c%c%c]", pInfo->nameLen, pInfo->callName[0],
                 pInfo->callName[1], pInfo->callName[2], pInfo->callName[3], pInfo->callCnt, pInfo->cellNumb[0][0], pInfo->cellNumb[0][1], pInfo->cellNumb[0][2],
                 pInfo->cellNumb[0][3], pInfo->cellNumb[0][4], pInfo->cellNumb[0][5], pInfo->cellNumb[0][6], pInfo->cellNumb[0][7], pInfo->cellNumb[0][8], pInfo->cellNumb[0][9],
                 pInfo->cellNumb[0][10]);
}

#endif // #if (TLKBTP_CFG_PBAP_ENABLE)
#endif
