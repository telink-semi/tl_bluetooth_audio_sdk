/********************************************************************************************************
 * @file    tlkmdi_tinySql_inner.h
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

//This headfile is private code.do not include in other .c file (only used in tinysql).

/**
 * @brief   Reads data from flash.
 * @param[in] addr - The flash address to read from.
 * @param[out] pBuff - Buffer to store the read data.
 * @param[in] buffLen - Number of bytes to read.
 */
#define sql_flash_read(addr, pBuff, buffLen)  flash_read_page(addr, buffLen, pBuff)

/**
 * @brief   Writes data to flash.
 * @param[in] addr - The flash address to write to.
 * @param[in] pData - Data to be written.
 * @param[in] buffLen - Number of bytes to write.
 */
#define sql_flash_write(addr, pData, buffLen) flash_write_page(addr, buffLen, pData)

/**
 * @brief   Erases a sector in flash.
 * @param[in] addr - The flash address within the sector to erase.
 */
#define sql_flash_eraseSector(addr)           flash_erase_sector(addr)

/**
 * @brief   TinySQL private enumeration for disk indices and special values.
 */
typedef enum
{
    tinySql_notFind = 0XFFFF,     //!< Value indicating item not found
    tinySql_full    = 0XFFFF,     //!< Value indicating storage is full
    tinySql_nullptr = 0XFFFF,     //!< Value representing null pointer
    //why use 0xFFFF? After erasing flash, the data is 0xFF

    tinySql_disk0SaveIndex = 0,   //!< Index for disk 0
    tinySql_disk1SaveIndex,       //!< Index for disk 1
    tinySql_disk2SaveIndex,       //!< Index for disk 2
    tinySql_disk3SaveIndex,       //!< Index for disk 3
    tinySql_disk4SaveIndex,       //!< Index for disk 4
    tinySql_disk5SaveIndex,       //!< Index for disk 5
    tinySql_maxSaveIndex,         //!< Maximum number of save indices

    tinySql_macSaveIndex            = tinySql_disk0SaveIndex,        //!< MAC address disk index
    tinySql_userSettingsSaveIndex   = tinySql_disk1SaveIndex,        //!< User settings disk index
    tinySql_pairingDevicesSaveIndex = tinySql_disk2SaveIndex,        //!< Pairing devices disk index
    tinySql_pbapSaveIndex           = tinySql_disk3SaveIndex,        //!< PBAP disk index
    tinySql_leSaveIndex             = tinySql_disk4SaveIndex,        //!< LE disk index
    tinySql_audioSaveIndex          = tinySql_disk5SaveIndex,        //!< Audio disk index

} TinySql_private_e; //only use in this module,equl c++ private code

/**
 * @brief   TinySQL disk interface structure.
 */
typedef struct
{
    void (*init)(void);              //!< Initialization function
    void (*save)(void);              //!< Save function
    void (*restoreFactory)(void);    //!< Factory reset function
} tinySqlDisk_t;

// External disk interface declarations
extern const tinySqlDisk_t tinySql_userSetting_disk;
extern const tinySqlDisk_t tinySql_pairingDevice_disk;
extern const tinySqlDisk_t tinySql_mac_disk;
extern const tinySqlDisk_t tinySql_pbap_disk;
extern const tinySqlDisk_t tinySql_le_disk;
extern const tinySqlDisk_t tinySql_audio_disk;

/**
 * @brief   Calculates the actual save address in flash.
 * @param[in] offset - The offset from the base address.
 * @return  The actual flash address for saving data.
 */
unsigned int tlkmdi_tinySql_getSaveAddr(unsigned int offset);

/**
 * @brief   Requests a save operation for a specific disk.
 * @param[in] whichDisk - The index of the disk that needs to be saved.
 */
void tlkmdi_tinySql_requestSave(uint8_t whichDisk);