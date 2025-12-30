/********************************************************************************************************
 * @file    tlkmw_fs_diskio.h
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
    void (*init)(void);
    void (*sleep)(void);
    void (*awake)(void);
    int (*write)(uint8_t* buff, uint32_t lba, uint32_t cnt);
    int (*read)(uint8_t* buff, uint32_t lba, uint32_t cnt);
    uint32_t (*getSectorNum)(void);
    uint32_t (*getSectorSize)(void);
}tlkmw_fs_diskio_t;

/**
 * @brief       This function writes data to the disk IO.
 * @param[in]   buff    - the buffer containing data to write.
 * @param[in]   lba     - the logical block address to write to.
 * @param[in]   cnt     - the number of blocks to write.
 * @return      Returns RES_OK on success, otherwise failure.
 */
int tlkmw_fs_diskio_write(uint8_t* buff, uint32_t lba, uint32_t cnt);

/**
 * @brief       This function reads data from the disk IO.
 * @param[out]  buff    - the buffer to store read data.
 * @param[in]   lba     - the logical block address to read from.
 * @param[in]   cnt     - the number of blocks to read.
 * @return      Returns RES_OK on success, otherwise failure.
 */
int tlkmw_fs_diskio_read(uint8_t* buff, uint32_t lba, uint32_t cnt);

/**
 * @brief       This function gets the block count.
 * @param[in]   none.
 * @return      Returns the block count.
 */
uint32_t tlkmw_fs_diskio_getBlkCount(void);

/**
 * @brief       This function initializes the disk IO.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_fs_diskio_init(void);