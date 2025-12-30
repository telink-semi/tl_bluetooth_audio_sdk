/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_pbap.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"
#include "tlkalg/digest/crc/tlkalg_crc16.h"

#if (TLK_MW_TINYSQL_ENABLE)

#define TINYSQL_INNER_PBAP_SECTOR_BLOCK_NUM (4)
#define TINYSQL_INNER_PBAP_SECTOR_NUM       (16)
#define TINYSQL_INNER_PBAP_ITEM_MAX_NUM     (TINYSQL_INNER_PBAP_SECTOR_NUM * TINYSQL_INNER_PBAP_SECTOR_BLOCK_NUM)
#define TINYSQL_INNER_PBAP_ITEM_MAX_SIZE    (4096 / TINYSQL_INNER_PBAP_SECTOR_BLOCK_NUM)

#define TINYSQL_FLASH_ADDR_BGEIN            0X20000000U

//sector(4096) = block(1024) * 4 = item(88) * 10 * 4
typedef enum
{
    TINYSQL_INNER_STATE_NOINIT = 0XFF,
    TINYSQL_INNER_STATE_CRASH  = 0X00,
    TINYSQL_INNER_STATE_OK     = 0XEE,
    TINYSQL_INNER_STATE_SAVING = 0XFE,
} pbapSaveState_e; //inner struct,only use in this file

/**
 * @brief Structure for the first block of PBAP data.
 */
typedef struct
{
    uint8_t  mac[6];   //!< MAC address
    uint8_t  sign;     //!< Signature
    uint8_t  ver;      //!< Version
    uint8_t  state;    //!< State
    uint8_t  resv;     //!< Reserved
    uint16_t blockNum; //!< Number of blocks
} pbapFirstBlock_t;    //inner struct,only use in this file

/**
 * @brief Structure for PBAP save header.
 */
typedef struct
{
    uint8_t  sign;    //!< Signature
    uint8_t  ver;     //!< Version
    uint16_t crc;     //!< CRC checksum
    uint16_t itemNum; //!< Number of items
    uint16_t len;     //!< Length
} pbapSaveHead_t;     //inner struct,only use in this file

/******************************************************************************
                           private code begin
******************************************************************************/
static uint16_t sTlkMdiTinySqlPbapSaveLen   = 0;
static uint8_t  sTlkMdiTinySqlPbapSaveState = 0;

/**
 * @brief       Get the save address for a PBAP item by index.
 * @param[in]   index - Index of the item.
 * @return      Flash address for the item.
 */
static inline unsigned int tlkmdi_tinySql_getPbapItemSaveAddr(uint16_t index)
{
    unsigned int baseAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK3_ADDR);
    return baseAddress + index * TINYSQL_INNER_PBAP_ITEM_MAX_SIZE;
}

/**
 * @brief       Restore the PBAP address disk.
 * @return      none.
 */
static void tlkmdi_tinySql_pbapAddressDiskRestore(void)
{
    unsigned int addr = tlkmdi_tinySql_getPbapItemSaveAddr(0);
    sql_flash_eraseSector(addr);
    sTlkMdiTinySqlPbapSaveState = TINYSQL_INNER_STATE_CRASH;
}

/**
 * @brief       Initialize the PBAP address disk.
 * @return      none.
 */
static void tlkmdi_tinySql_pbapAddressDiskInit(void)
{
    uint8_t blockBuffer[TINYSQL_INNER_PBAP_ITEM_MAX_SIZE];

    //first block save all info,including mac and blockLen,state
    unsigned int addr = tlkmdi_tinySql_getPbapItemSaveAddr(0);
    sql_flash_read(addr, blockBuffer, sizeof(pbapFirstBlock_t));
    pbapFirstBlock_t *firstBlock = (pbapFirstBlock_t *)blockBuffer;
    if (firstBlock->sign != TLKMDI_TINYSQL_SAVE_SIGN || firstBlock->ver != TLKMDI_TINYSQL_VER || firstBlock->state != TINYSQL_INNER_STATE_OK || firstBlock->blockNum == 0xFF) {
        sTlkMdiTinySqlPbapSaveState = TINYSQL_INNER_STATE_CRASH;
        return;
    } //check phone book is ok
    sTlkMdiTinySqlPbapSaveLen = firstBlock->blockNum;
    pbapSaveHead_t headBuffer;
    uint16_t       BlockI = 1;
    for (; BlockI < sTlkMdiTinySqlPbapSaveLen; BlockI++) {
        addr = tlkmdi_tinySql_getPbapItemSaveAddr(BlockI);
        sql_flash_read(addr, (uint8_t *)&headBuffer, sizeof(pbapSaveHead_t));
        if (headBuffer.sign != TLKMDI_TINYSQL_SAVE_SIGN || headBuffer.ver != TLKMDI_TINYSQL_VER) {
            break;
        }
        sql_flash_read(addr + sizeof(pbapSaveHead_t), blockBuffer, headBuffer.len);
        uint16_t crc = tlkalg_crc16_calc(blockBuffer, headBuffer.len);
        if (crc != headBuffer.crc) {
            break;
        }
    }
    if (BlockI != sTlkMdiTinySqlPbapSaveLen) {
        tlkmdi_tinySql_pbapAddressDiskRestore();
    } else {
        sTlkMdiTinySqlPbapSaveState = TINYSQL_INNER_STATE_OK;
    }
}

/**
 * @brief       Save the PBAP address disk.
 * @return      none.
 */
static void tlkmdi_tinySql_pbapAddressDiskSave(void)
{
    //need this none function do nothing.
}

const tinySqlDisk_t tinySql_pbap_disk = {
    .init           = tlkmdi_tinySql_pbapAddressDiskInit,
    .restoreFactory = tlkmdi_tinySql_pbapAddressDiskRestore,
    .save           = tlkmdi_tinySql_pbapAddressDiskSave,
};

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief       Get the phone book state.
 * @return      true if phone book is in OK state, false otherwise.
 */
bool tlkmdi_tinySql_getPhoneBookState(void)
{
    return sTlkMdiTinySqlPbapSaveState == TINYSQL_INNER_STATE_OK;
}

/**
 * @brief       Create a new phone book for the specified Bluetooth MAC address.
 * @param[in]   btMac - Bluetooth MAC address for the phone book.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_newPhoneBook(uint8_t *btMac)
{
    unsigned int addr = tlkmdi_tinySql_getPbapItemSaveAddr(0);
    sql_flash_eraseSector(addr);
    pbapFirstBlock_t firstBlock;
    tmemcpy(&firstBlock.mac, btMac, 6);
    firstBlock.blockNum = 0xFF;
    firstBlock.sign     = TLKMDI_TINYSQL_SAVE_SIGN;
    firstBlock.ver      = TLKMDI_TINYSQL_VER;
    firstBlock.state    = TINYSQL_INNER_STATE_SAVING;
    sql_flash_write(addr, (uint8_t *)&firstBlock, sizeof(pbapFirstBlock_t));
    sTlkMdiTinySqlPbapSaveLen   = 1;
    sTlkMdiTinySqlPbapSaveState = TINYSQL_INNER_STATE_SAVING;
    return TLK_ENONE;
}

/**
 * @brief       Add a PBAP item block to the phone book.
 * @param[in]   isLastOne - Indicates if this is the last block to be added.
 * @param[in]   itemsNum - Number of items in this block.
 * @param[in]   data - Pointer to the data to be added.
 * @param[in]   dataLen - Length of the data in bytes.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_addPbapItemBlock(bool isLastOne, uint16_t itemsNum, void *data, uint16_t dataLen)
{
    if (sTlkMdiTinySqlPbapSaveState != TINYSQL_INNER_STATE_SAVING) {
        return -TLK_EFAIL;
    }
    if (isLastOne) {
        pbapFirstBlock_t firstBlock;
        unsigned int     firstAddr = tlkmdi_tinySql_getPbapItemSaveAddr(0);
        sql_flash_read(firstAddr, (uint8_t *)&firstBlock, sizeof(pbapFirstBlock_t));
        firstBlock.blockNum = sTlkMdiTinySqlPbapSaveLen;
        firstBlock.state    = TINYSQL_INNER_STATE_OK;
        sql_flash_write(firstAddr, (uint8_t *)&firstBlock, sizeof(pbapFirstBlock_t));
        sTlkMdiTinySqlPbapSaveState = TINYSQL_INNER_STATE_OK;
        return TLK_ENONE;
    }
    if (data == NULL || dataLen >= TINYSQL_INNER_PBAP_ITEM_MAX_SIZE - sizeof(pbapSaveHead_t)) {
        return -TLK_EPARAM;
    }
    unsigned int saveAddress = tlkmdi_tinySql_getPbapItemSaveAddr(sTlkMdiTinySqlPbapSaveLen);
    if (sTlkMdiTinySqlPbapSaveLen != 0 && sTlkMdiTinySqlPbapSaveLen % TINYSQL_INNER_PBAP_SECTOR_BLOCK_NUM == 0) {
        sql_flash_eraseSector(saveAddress & (~0XFFF));
    }

    pbapSaveHead_t headbuff;
    headbuff.crc     = tlkalg_crc16_calc(data, dataLen);
    headbuff.len     = dataLen;
    headbuff.sign    = TLKMDI_TINYSQL_SAVE_SIGN;
    headbuff.ver     = TLKMDI_TINYSQL_VER;
    headbuff.itemNum = itemsNum;
    sql_flash_write(saveAddress, (unsigned char *)&headbuff, sizeof(pbapSaveHead_t));

    sql_flash_write(saveAddress + sizeof(pbapSaveHead_t), data, dataLen);

    sTlkMdiTinySqlPbapSaveLen++;
    return TLK_ENONE;
}

/**
 * @brief       Get the MAC address associated with the phone book.
 * @param[out]  recbuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getPhoneBookMac(uint8_t *recbuffer)
{
    if (recbuffer == NULL) {
        return -TLK_EPARAM;
    }
    if (sTlkMdiTinySqlPbapSaveState != TINYSQL_INNER_STATE_OK) {
        return -TLK_ENODATA;
    }
    tmemcpy(recbuffer, (void *)(tlkmdi_tinySql_getPbapItemSaveAddr(0) + TINYSQL_FLASH_ADDR_BGEIN), 6);
    return TLK_ENONE;
}

/**
 * @brief       Get a pointer to the MAC address associated with the phone book.
 * @return      Pointer to the MAC address, or NULL if phone book is not in OK state.
 */
const uint8_t *tlkmdi_tinySql_getPhoneBookMacPointer(void)
{
    if (sTlkMdiTinySqlPbapSaveState != TINYSQL_INNER_STATE_OK) {
        return NULL;
    }
    return (void *)(tlkmdi_tinySql_getPbapItemSaveAddr(0) + TINYSQL_FLASH_ADDR_BGEIN);
}

/**
 * @brief       Get the total number of items in the phone book.
 * @return      Total number of items in the phone book.
 */
uint16_t tlkmdi_tinySql_getPhoneBookItemNum(void)
{
    if (sTlkMdiTinySqlPbapSaveState != TINYSQL_INNER_STATE_OK) {
        return 0;
    }
    uint16_t numRes = 0;
    for (uint16_t BlockI = 1; BlockI < sTlkMdiTinySqlPbapSaveLen; BlockI++) {
        unsigned int    addr = tlkmdi_tinySql_getPbapItemSaveAddr(BlockI);
        pbapSaveHead_t *head = (pbapSaveHead_t *)(TINYSQL_FLASH_ADDR_BGEIN + addr);
        numRes += head->itemNum;
    }
    return numRes;
}

/**
 * @brief       Search the phone book using a custom search function.
 * @param[in]   searchFunc - Function pointer to the search criteria.
 * @param[in]   oneItemLen - Length of each item in the phone book.
 * @return      Pointer to the found item, or NULL if not found.
 */
const void *tlkmdi_tinySql_searchPhoneBook(tlkMdiTinySqlSearchFunc searchFunc, uint16_t oneItemLen)
{
    if (sTlkMdiTinySqlPbapSaveState != TINYSQL_INNER_STATE_OK) {
        return NULL;
    }
    pbapSaveHead_t headBuffer;

    for (uint16_t BlockI = 1; BlockI < sTlkMdiTinySqlPbapSaveLen; BlockI++) {
        unsigned int addr = tlkmdi_tinySql_getPbapItemSaveAddr(BlockI);
        sql_flash_read(addr, (uint8_t *)&headBuffer, sizeof(pbapSaveHead_t));
        if (headBuffer.len % oneItemLen != 0) {
            return NULL;
        }
        for (uint16_t i = 0; i < headBuffer.itemNum; i++) {
            const void *arg = (const void *)(TINYSQL_FLASH_ADDR_BGEIN + addr + sizeof(pbapSaveHead_t) + oneItemLen * i);
            if (searchFunc(arg)) {
                return arg;
            }
        }
    }
    return NULL;
}

#endif