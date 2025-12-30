/********************************************************************************************************
 * @file    tlkmdi_btpbap.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLKMDI_BTPBAP_H_
#define TLKMDI_BTPBAP_H_

#if (TLKBTP_CFG_PBAP_ENABLE)
#define TLKMDI_PHONE_NAME_MAX_LEN       32

#define TLKMDI_PHONE_BOOK_HEADLEN       16


#define TLKMDI_PHONE_BOOK_FLAG_INIT     0xFF
#define TLKMDI_PHONE_BOOK_FLAG_WRITE    0xFE
#define TLKMDI_PHONE_BOOK_FLAG_COMPLETE 0xF0

#define TLKMDI_PHONE_BOOK_SIGN          0x50424150
#define TLKMDI_PHONE_BOOK_ITEM_LENGTH   (BTP_PBAP_CALL_NAME_LENGTH * 2 + BTP_PBAP_CELL_NUMB_LENGTH * BTP_PBAP_CELL_NUMB_COUNT) // 68
#define TLKMDI_PHONE_BOOK_MAX_ITEM_NUMB (64 * 40)                                                                              // 64 blocks * 40 preson/block = 2560

typedef struct
{
    uint32_t pbapSign;
    uint8_t  btAddr[6];
    uint8_t  itemLens;
    uint8_t  callFlag;
    uint32_t callNumb;
} __attribute__((__packed__)) tlkmdi_phoneBookHead_t;

typedef struct
{
    uint8_t  isBusy;
    uint8_t  isReady;
    uint8_t  posi;
    uint8_t  type;
    uint8_t  sort;
    uint16_t offset;
    uint16_t number;
    uint16_t aclHandle;
    uint8_t  btAddr[6];
    uint32_t bookCount;
} tlkmdi_phoneBookCtrl_t;

/**
 * @brief Initializes PBAP related resources.
 *
 * @param none
 *
 * @return none
 */
void tlkmdi_pbap_init(void);

/**
 * @brief Phone book resource initialization.
 *
 * @param none
 *
 * @return Return TLK_ENONE is success/other is failure.
 */
int tlkmdi_pbap_phoneBookinit(void);

/**
 * @brief       Verify that phone book information is valid.      
 * @param[in]   pBtAddr    -- The Bt address.
 * @return      Return true is valid, other value is invalid.
 * @note        
 */
bool tlkmdi_pbap_phoneBookInfoValid(uint8_t *pBtAddr);
/**
 * @brief       Clear the phone book information saved in the flash.
 *
 * @param none
 *
 * @return none
 */
void tlkmdi_pbap_phoneBookCleanInfo(uint8_t *btMac);

/**
 * @brief       Start synchronizing the phone book.
 * @param[in]   aclHandle    -- The acl handle. 
 * @param[in]   pBtAddr      -- The device's Bt address.
 * @param[in]   isForce      -- Is force to sync the pb info.
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookStartSync(uint16_t aclHandle, uint8_t *pBtAddr, bool isForce);

/**
 * @brief       Stop the phone book synchronization.
 * @param[in]   aclHandle   -- The acl handle. 
 * @return      Return TLK_ENONE is success/other is failure.
 * @note        
 */
int tlkmdi_pbap_phoneBookStopSync(uint16_t aclHandle);

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
int tlkmdi_pbap_phoneBookSetParam(uint8_t posi, uint8_t type, uint8_t sort, uint16_t offset, uint16_t number);

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
int tlkmdi_pbap_phoneBookGetName(uint8_t *pNumber, uint8_t numbLen, uint8_t *pName, uint8_t nameLen, uint8_t *pGetLen);
#endif
#endif /* TLKMDI_BTPBAP_H_ */
