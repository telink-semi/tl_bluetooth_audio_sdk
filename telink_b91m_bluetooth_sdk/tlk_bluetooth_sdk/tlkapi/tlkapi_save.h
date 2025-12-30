/********************************************************************************************************
 * @file    tlkapi_save.h
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
#pragma once

typedef struct
{
    uint8_t  sign;
    uint8_t  vers;
    uint16_t prev;
    uint16_t lens; //=4+Item Length (4=sign+version+length)
    uint16_t offs; // Cur Offset
    uint32_t addr; // Flash Start Address
    uint32_t addr0;
    uint32_t addr1;
} tlkapi_save_ctrl_t;

/**
 * @brief       Initialize the save control parameters and scan valid data in flash for save method 3
 * @param[in]   pCtrl     - The control block of the save method
 * @param[in]   sign      - Information identification tag
 * @param[in]   version   - The version of the save method
 * @param[in]   length    - The length of each item in the save's method
 * @param[in]   address0  - The address of the first backup sector of the system
 * @param[in]   address1  - The address of the second backup sector of the system
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
int tlkapi_save3_init(tlkapi_save_ctrl_t *pCtrl, uint8_t sign, uint8_t version, uint16_t length, uint32_t address0, uint32_t address1);

/**
 * @brief       Load data from flash storage using save method 3
 * @param[in]   pCtrl    - The control block for save method 3
 * @param[out]  pBuff    - Buffer to store the loaded data
 * @param[in]   buffLen  - Length of the buffer
 * @return      Returns negative error code on error, or the number of bytes read on success
 * @note:       This interface should be called after "tlkapi_save3_init()".
 */
int tlkapi_save3_load(tlkapi_save_ctrl_t *pCtrl, uint8_t *pBuff, uint16_t buffLen);

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
int tlkapi_save3_save(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Clean the save sector by invalidating current data and resetting control parameters
 * @param[in]   pCtrl    - The control block for save method 3
 * @return      none
 * @note        This function marks the current sector as invalid, erases it, and then marks it as 
 *              valid again. It also resets the offset and previous offset values to zero.
 */
void tlkapi_save3_clean(tlkapi_save_ctrl_t *pCtrl);

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
int tlkapi_save3_migrate(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Smart saving function that decides whether to save directly or migrate based on available space
 * @param[in]   pCtrl    - Pointer to the save control structure
 * @param[in]   pData    - Pointer to the data to be saved
 * @param[in]   dataLen  - Length of the data to be saved
 * @return      Operating results, TLK_ENONE means success, others means failure
 * @note        This function checks if there is enough space to save data in current sector. 
 *              If not, it triggers migration to another sector; otherwise, it saves directly.
 */
int tlkapi_save3_smartSave(tlkapi_save_ctrl_t *pCtrl, uint8_t *pData, uint16_t dataLen);
