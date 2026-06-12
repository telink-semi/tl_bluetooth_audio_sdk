/********************************************************************************************************
 * @file    tlkapi_save.c
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
#include <stdbool.h>
#include "common/tstring.h"
#include "drivers.h"
#include "tlkapi/tlkapi_save.h"
#include "tlkapi/tlkapi_common.h"

#define TLKAPI_FLASH_PAGE_SIZE                       256
#define TLKAPI_FLASH_SECTOR_SIZE                     4096


#define TLKAPI_SAVE_FLASH_SIGN                       0x55

#define TLKAPI_SAVE_FLASH_VALID_SIGN                 0x55
#define TLKAPI_SAVE_FLASH_MIGRATE_SIGN               0x5F // The sign of the sector which data is being moved out.
#define TLKAPI_SAVE_FLASH_INVALID_SIGN               0x00

#define TLKAPI_SAVE_SECTOR_VALID_SIZE                (TLKAPI_FLASH_SECTOR_SIZE - 1)

#define TLKAPI_SAVE_ONE_ITEM_MAX_LEN                 884

#define tlkapi_save_flashRead(addr, pBuff, buffLen)  flash_read_page(addr, buffLen, pBuff)
#define tlkapi_save_flashWrite(addr, pData, buffLen) flash_write_page(addr, buffLen, pData)
#define tlkapi_save_flashEraseSector(addr)           flash_erase_sector(addr)

/**
 * @brief       Calculate CRC checksum of data
 * @param[in]   data     - Pointer to the data buffer
 * @param[in]   datalen  - Length of the data buffer
 * @return      16-bit CRC checksum value
 */
static uint16_t tlkapi_save_getCRC(uint8_t *data, uint16_t datalen)
{
    uint16_t sum = 0;
    for (size_t i = 0; i < datalen; i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * @brief       Write a single byte to flash at specified address
 * @param[in]   addr  - The flash address to write to
 * @param[in]   byte  - The byte value to write
 * @return      none
 */
static inline void tlkapi_save_flashWriteByte(uint32_t addr, uint8_t byte)
{
    flash_write_page(addr, 1, &byte);
}

/**
 * @brief       Initialize the save control parameters for double sector save method
 * @param[in]   pCtrl     - The control block of the save method
 * @param[in]   sign      - Information identification tag
 * @param[in]   version   - The version of the save method
 * @param[in]   length    - The length of each item in the save's method
 * @param[in]   address0  - The address of the 2 backup sector(8k) of the system
 * @return      Operating results, TLK_ENONE means success, others means failure
 */
static int tlkapi_save34_preinit(tlkapi_save_ctrl_t *pCtrl, uint8_t sign, uint8_t version, uint16_t length, uint32_t address0)
{
    bool    isEarse;
    uint8_t buffer0[4] = {0};
    uint8_t buffer1[4] = {0};
    if (pCtrl == NULL || sign == 0xFF || length > TLKAPI_SAVE_ONE_ITEM_MAX_LEN) {
        return -TLK_EPARAM;
    }
    if ((address0 & (TLKAPI_FLASH_SECTOR_SIZE - 1)) != 0 || version == 0x00 || version == 0xFF) {
        return -TLK_EPARAM;
    }
    pCtrl->sign       = sign;
    pCtrl->vers       = version;
    pCtrl->addr       = address0;
    pCtrl->lens       = length + 4;
    pCtrl->offs       = 0;
    pCtrl->prev       = 0;
    pCtrl->addr0      = address0;
    uint32_t address1 = address0 + 4096;
    tlkapi_save_flashRead(address0 + 0, &buffer0[0], 2); // Read sector start flag
    tlkapi_save_flashRead(address0 + TLKAPI_SAVE_SECTOR_VALID_SIZE, &buffer0[2], 1);
    tlkapi_save_flashRead(address1 + 0, &buffer1[0], 2);
    tlkapi_save_flashRead(address1 + TLKAPI_SAVE_SECTOR_VALID_SIZE, &buffer1[2], 1);
    // Validates all fields in sector 0, erases sector 0 if invalid
    isEarse = false;
    if (buffer0[0] == 0xFF) {
        if (buffer0[2] != 0xFF && buffer0[2] != TLKAPI_SAVE_FLASH_VALID_SIGN) {
            isEarse = true;
        }
    } else if (buffer0[2] != TLKAPI_SAVE_FLASH_VALID_SIGN || buffer0[1] != pCtrl->vers) {
        isEarse = true;
    }
    if (isEarse) {
        buffer0[0] = 0xFF;
        buffer0[2] = 0xFF;
        tlkapi_save_flashEraseSector(address0);
    }

    // Validates all fields in sector 1, erases sector 1 if invalid
    isEarse = false;
    if (buffer1[0] == 0xFF) {
        if (buffer1[2] != 0xFF && buffer1[2] != TLKAPI_SAVE_FLASH_VALID_SIGN) {
            isEarse = true;
        }
    } else if (buffer1[2] != TLKAPI_SAVE_FLASH_VALID_SIGN || buffer1[1] != pCtrl->vers) {
        isEarse = true;
    }
    if (isEarse) {
        buffer1[0] = 0xFF;
        buffer1[2] = 0xFF;
        tlkapi_save_flashEraseSector(address1);
    }

    // Check the sector that is currently working
    if (buffer0[0] == 0xFF || buffer1[0] == 0xFF) {
        bool isWrite = false;
        // There is item in "sector 0" or "sector 1"
        if (buffer1[0] == 0xFF && buffer1[2] == 0xFF) {
            pCtrl->addr = address0; // The current operating sector pointer points to sector 0
            if (buffer0[2] != TLKAPI_SAVE_FLASH_VALID_SIGN) {
                isWrite = true;
            }
        } else {
            pCtrl->addr = address1; // The current operating sector pointer points to sector 1
            if (buffer1[2] != TLKAPI_SAVE_FLASH_VALID_SIGN) {
                isWrite = true;
            }
        }
        if (isWrite) {
            tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_VALID_SIGN);
        }
    } else {
        // There is item in "sector 0" or "sector 1". And sectors are being operated.
        if (buffer0[2] == TLKAPI_SAVE_FLASH_VALID_SIGN) {
            pCtrl->addr = address0;
            tlkapi_save_flashEraseSector(address1);
        } else if (buffer1[2] == TLKAPI_SAVE_FLASH_VALID_SIGN) {
            pCtrl->addr = address1;
            tlkapi_save_flashEraseSector(address0);
        } else {
            pCtrl->addr = address0;
            tlkapi_save_flashEraseSector(address0);
            tlkapi_save_flashEraseSector(address1);
            tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_VALID_SIGN);
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       Initialize the save control parameters and scan valid data in flash for save method 3
 * @param[in]   pCtrl     - The control block of the save method
 * @param[in]   sign      - Information identification tag
 * @param[in]   version   - The version of the save method
 * @param[in]   length    - The length of each item in the save's method
 * @param[in]   address0  - The address of the 2 backup sector(8k) of the system
 * @return      Operating results, TLK_ENONE means success, others means failure
 *              -TLK_ENODATA means no valid data found
 * @note:       
 *              1.Save a single item in double sectors. The length of Item is fixed,
 *                During the saving process, the saved data will be automatically
 *                restored in case of abnormal power failure.
 *              2.If the "sign" does not match the loaded item in flash, the sector will
 *                be erased.
 *              3.If the "version" is different from the loaded item in flash, the
 *                sector will be erased.
 *              4.This interface is used to initialize control block parameters, traverse
 *                automatically the two sectors, find the sector containing valid
 *                information, and record the sector address. If there is no valid
 *                information, the default "address0" is used as the current save sector.
 *              5.If data migration is terminated abnormally (such as power failure)
 *                during the last migration, the interface continues to migrate data.
 */
int tlkapi_save3_init(tlkapi_save_ctrl_t *pCtrl, uint8_t sign, uint8_t version, uint16_t length, uint32_t address0)
{
    int     ret;
    uint8_t buffer[4] = {0};
    uint8_t savedData[TLKAPI_SAVE_ONE_ITEM_MAX_LEN]; //in heap is better then stack
    ret = tlkapi_save34_preinit(pCtrl, sign, version, length, address0);
    if (ret != TLK_ENONE) {
        return ret;
    }

    pCtrl->offs            = 0;
    pCtrl->prev            = 0;
    uint8_t IsGetRightData = 0;
    while (pCtrl->offs + 4 <= TLKAPI_SAVE_SECTOR_VALID_SIZE) {
        tlkapi_save_flashRead(pCtrl->addr + pCtrl->offs, buffer, 4);
        if (buffer[0] == 0xFF) {
            break;
        }
        if (buffer[1] == pCtrl->vers && buffer[0] == pCtrl->sign) {
            //check sign and version, then check crc.
            uint16_t crc = (((uint16_t)buffer[3]) << 8) | buffer[2];
            tlkapi_save_flashRead(pCtrl->addr + pCtrl->offs + 4, savedData, pCtrl->lens - 4);
            if (crc == tlkapi_save_getCRC(savedData, pCtrl->lens - 4)) {
                pCtrl->prev    = pCtrl->offs;
                IsGetRightData = 1;
            }
        }
        pCtrl->offs += pCtrl->lens;
    }
    if (IsGetRightData == 0) { //now have no right data
        pCtrl->prev = pCtrl->offs;
        return -TLK_ENODATA;
    }
    return TLK_ENONE;
}

/**
 * @brief       Load data from flash storage using save method 3
 * @param[in]   pCtrl    - The control block for save method 3
 * @param[out]  pBuff    - Buffer to store the loaded data
 * @param[in]   buffLen  - Length of the buffer
 * @return      Returns negative error code on error, or the number of bytes read on success
 * @note:       This interface should be called after "tlkapi_save3_init()".
 */
int tlkapi_save3_load(tlkapi_save_ctrl_t *pCtrl, uint8_t *pBuff, uint16_t buffLen)
{
    uint16_t readLen;

    if (pCtrl == NULL || pBuff == NULL || buffLen == 0) {
        return -TLK_EPARAM;
    }
    if (pCtrl->prev == pCtrl->offs) {
        return -TLK_ENOOBJECT;
    }

    if (buffLen <= pCtrl->lens - 4) {
        readLen = buffLen;
    } else {
        readLen = pCtrl->lens - 4;
    }

    tlkapi_save_flashRead(pCtrl->addr + pCtrl->prev + 4, pBuff, readLen);
    return readLen;
}

/**
 * @brief       Save data to flash using save method 3 with CRC checksum
 * @param[in]   pCtrl    - The control block for save method 3
 * @param[in]   pData    - Pointer to the data buffer to be saved
 * @param[in]   dataLen  - Length of the data to be saved
 * @return      Operating results, TLK_ENONE means success, others means failure
 * @note        
 *              1.This function saves data to flash with a CRC checksum for integrity verification.
 *                The data is written in a specific format including signature, version and CRC.
 *              2.This interface should be called after "tlkapi_save3_init()".
 *              3.If the saved information exceeds the available space of the sector,
 *                the saving fails.
 */
int tlkapi_save3_save(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen)
{
    uint8_t buffer[4];

    if (pCtrl == NULL || pData == NULL || dataLen + 4 != pCtrl->lens) {
        return -TLK_EPARAM;
    }
    if (pCtrl->offs + pCtrl->lens > TLKAPI_SAVE_SECTOR_VALID_SIZE) {
        return -TLK_EOVERFLOW;
    }
    uint16_t crc = tlkapi_save_getCRC(pData, dataLen);
    buffer[0]    = pCtrl->sign;
    buffer[1]    = pCtrl->vers;
    buffer[2]    = (uint8_t)crc;
    buffer[3]    = (uint8_t)(crc >> 8);
    tlkapi_save_flashWrite(pCtrl->addr + pCtrl->offs + 0, buffer, 1);
    tlkapi_save_flashWrite(pCtrl->addr + pCtrl->offs + 4, pData, dataLen);
    tlkapi_save_flashWrite(pCtrl->addr + pCtrl->offs + 1, buffer + 1, 3);
    pCtrl->prev = pCtrl->offs;
    pCtrl->offs += pCtrl->lens;

    return TLK_ENONE;
}

/**
 * @brief       Clean the save sector by invalidating current data and resetting control parameters
 * @param[in]   pCtrl    - The control block for save method 3
 * @return      none
 * @note        This function marks the current sector as invalid, erases it, and then marks it as 
 *              valid again. It also resets the offset and previous offset values to zero.
 */
void tlkapi_save3_clean(tlkapi_save_ctrl_t *pCtrl)
{
    if (pCtrl == NULL) {
        return;
    }
    tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_INVALID_SIGN);
    tlkapi_save_flashEraseSector(pCtrl->addr);
    tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_VALID_SIGN);
    pCtrl->offs = 0;
    pCtrl->prev = 0;
}

/**
 * @brief       Save user information in migration mode.
 * @param[in]   pCtrl    - The control block for save method 3
 * @param[in]   pData    - The data to be saved.
 * @param[in]   dataLen  - The length of the data to be saved
 * @return      Operating results, TLK_ENONE means success, others means failure
 * @note        
 *              1.This interface should be called after "tlkapi_save3_init()".
 *              2.Different from "tlkapi_save3_save()", the interface first calls the
 *                function that erasing the sector and then saves the information, that
 *                is, the interface always saves the user information at the original
 *                location of the sector.
 */
int tlkapi_save3_migrate(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen)
{
    uint32_t curAddr;
    uint8_t  buffer[4];
    if (pCtrl == NULL || pData == NULL || dataLen + 4 != pCtrl->lens) {
        return -TLK_EPARAM;
    }

    curAddr = pCtrl->addr;
    if (pCtrl->addr == pCtrl->addr0) {
        pCtrl->addr = pCtrl->addr0 + 4096;
    } else {
        pCtrl->addr = pCtrl->addr0;
    }

    uint16_t crc = tlkapi_save_getCRC(pData, dataLen);
    buffer[0]    = pCtrl->sign;
    buffer[1]    = pCtrl->vers;
    buffer[2]    = (uint8_t)crc;
    buffer[3]    = (uint8_t)(crc >> 8);
    // Start migrating data,retry count = 3
    uint8_t savedData[TLKAPI_SAVE_ONE_ITEM_MAX_LEN]; //in heap is better than stack
    int     retryCnt = 0;
    for (; retryCnt < 3; retryCnt++) {
        tlkapi_save_flashEraseSector(pCtrl->addr);
        tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_MIGRATE_SIGN);
        tlkapi_save_flashWrite(pCtrl->addr + 0, buffer, 1);
        tlkapi_save_flashWrite(pCtrl->addr + 4, pData, dataLen);
        tlkapi_save_flashWrite(pCtrl->addr + 1, buffer + 1, 3);

        tlkapi_save_flashRead(pCtrl->addr, buffer, 4);
        if (buffer[1] == pCtrl->vers && buffer[0] == pCtrl->sign) {
            //check sign and version, then check crc.
            crc = (((uint16_t)buffer[3]) << 8) | buffer[2];
            tlkapi_save_flashRead(pCtrl->addr + 4, savedData, pCtrl->lens - 4);
            if (crc == tlkapi_save_getCRC(savedData, pCtrl->lens - 4)) {
                break; //save ok
            }
        }
    }
    if (retryCnt == 3) { //save 3 times all failed
        pCtrl->addr = curAddr;
        return -TLK_EFAIL;
    }
    tlkapi_save_flashWriteByte(pCtrl->addr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_VALID_SIGN); // Save data OK
    // Deletes data in the current sector
    tlkapi_save_flashWriteByte(curAddr + TLKAPI_SAVE_SECTOR_VALID_SIZE, TLKAPI_SAVE_FLASH_INVALID_SIGN);
    tlkapi_save_flashEraseSector(curAddr);
    pCtrl->offs = pCtrl->lens;
    pCtrl->prev = 0;
    return TLK_ENONE;
}

/**
 * @brief       Smart saving function that decides whether to save directly or migrate based on available space
 * @param[in]   pCtrl    - Pointer to the save control structure
 * @param[in]   pData    - Pointer to the data to be saved
 * @param[in]   dataLen  - Length of the data to be saved
 * @return      Operating results, TLK_ENONE means success, others means failure
 * @note        This function checks if there is enough space to save data in current sector. 
 *              If not, it triggers migration to another sector; otherwise, it saves directly.
 */
int tlkapi_save3_smartSave(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen)
{
    if (pCtrl == NULL) {
        return -TLK_EPARAM;
    } //no need to check pData == NULL || dataLen + 4 != pCtrl->lens again
    if (pCtrl->offs + pCtrl->lens > TLKAPI_SAVE_SECTOR_VALID_SIZE) {
        return tlkapi_save3_migrate(pCtrl, pData, dataLen);
    } else {
        return tlkapi_save3_save(pCtrl, pData, dataLen);
    }
}
