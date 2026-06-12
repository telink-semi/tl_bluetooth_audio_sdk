/********************************************************************************************************
 * @file    tlkusb_msc.h
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
    uint8_t prevent;
    uint8_t isReady;
    uint8_t unitAttention;
} tlkusb_msc_disk_var_t;

typedef struct
{
    uint32_t blkSize;              //Block size
    uint32_t (*getBlkCount)(void); //get block count func
    const char *pVendorStr;        //VENDOR_STRING Length<=8
    const char *pProductStr;       //PRODUCT_STRING Length<=16
    const char *pVersionStr;       //VERSION_STRING Length<=4
    int (*Init)(void);
    int (*Read)(uint8_t *pBuff, uint32_t blkOffs, uint32_t blkNumb);
    int (*Write)(uint8_t *pData, uint32_t blkOffs, uint32_t blkNumb);
    tlkusb_msc_disk_var_t *pVar;
} tlkusb_msc_disk_t;

/**
 * @brief       Append a disk to the MSC module.
 * @param[in]   pUnit - Pointer to the disk unit to append.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkusb_msc_appendDisk(tlkusb_msc_disk_t *pUnit);

/**
 * @brief       Remove a disk from the MSC module.
 * @param[in]   pUnit - Pointer to the disk unit to remove.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkusb_msc_removeDisk(tlkusb_msc_disk_t *pUnit);


/**
 * @brief       Get the count of disks in the MSC module.
 * @param[in]   none.
 * @return      Number of disks.
 */
uint8_t tlkusb_msc_getDiskCount(void);

/**
 * @brief       Get a disk by volume number.
 * @param[in]   volNum - Volume number.
 * @return      Pointer to the disk unit.
 */
tlkusb_msc_disk_t *tlkusb_msc_getDisk(uint8_t volNum);
