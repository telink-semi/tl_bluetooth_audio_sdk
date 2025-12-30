/********************************************************************************************************
 * @file    tlkmw_fs_diskio.c
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
#if TLK_CFG_FS_ENABLE
#include "tlklib/fatfs/ff/source/ff.h"
#include "tlklib/fatfs/ff/source/diskio.h"
#include "tlklib/fatfs/tlkfs_diskio.h"
#include "tlkmw_fs_diskio.h"

static TlkOsMutexHandle_t sTlkmwFsMutex = NULL;
static uint8_t sTlkmwFsIsDiskSleep = 0;
static TlkApiTimer_t sTlkmwFsSleepTmr = {0};
static tlksys_devmgr_item_t sTlkmwFsDev = {0};


#if MCU_CORE_TYPE == CHIP_TYPE_TL751X
extern const tlkmw_fs_diskio_t gTlkmwFsDiskIoEmmc;
static const tlkmw_fs_diskio_t* spTlkmwFsDiskIo = &gTlkmwFsDiskIoEmmc;
#elif MCU_CORE_TYPE == CHIP_TYPE_TL721X
extern const tlkmw_fs_diskio_t gTlkmwFsDiskIoSpisd;
static const tlkmw_fs_diskio_t* spTlkmwFsDiskIo = &gTlkmwFsDiskIoSpisd;
#endif

/**
 * @brief       This function is the sleep timer callback for disk IO.
 * @param[in]   handle    - the timer handle.
 * @param[in]   userArg   - user argument.
 * @return      none.
 */
static void tlkmw_fs_diskio_sleepTmr(TlkApiTimerHandle_t handle, void* userArg)
{
    (void) handle;
    (void) userArg;
    tlkos_recursiveMutex_lock(sTlkmwFsMutex);
    if(sTlkmwFsIsDiskSleep == 0){
        spTlkmwFsDiskIo->sleep();
        sTlkmwFsIsDiskSleep = 1;  
    }
    tlkos_recursiveMutex_unlock(sTlkmwFsMutex);
}

/**
 * @brief       This function checks and wakes up the disk IO if it's sleeping.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_diskio_sleepCheck(void)
{
    if(sTlkmwFsIsDiskSleep){
        tlksys_devmgr_setBusy(&sTlkmwFsDev,1);
        spTlkmwFsDiskIo->awake();
        sTlkmwFsIsDiskSleep = 0;
        tlksys_devmgr_setBusy(&sTlkmwFsDev,0);
    }
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM,&sTlkmwFsSleepTmr);
}

/**
 * @brief       This function writes data to the disk IO.
 * @param[in]   buff    - the buffer containing data to write.
 * @param[in]   lba     - the logical block address to write to.
 * @param[in]   cnt     - the number of blocks to write.
 * @return      Returns RES_OK on success, otherwise failure.
 */
int tlkmw_fs_diskio_write(uint8_t* buff, uint32_t lba, uint32_t cnt)
{
    tlkos_recursiveMutex_lock(sTlkmwFsMutex);
    tlkmw_fs_diskio_sleepCheck();
    spTlkmwFsDiskIo->write(buff, lba, cnt);
    tlkos_recursiveMutex_unlock(sTlkmwFsMutex);
    return RES_OK;
}

/**
 * @brief       This function reads data from the disk IO.
 * @param[out]  buff    - the buffer to store read data.
 * @param[in]   lba     - the logical block address to read from.
 * @param[in]   cnt     - the number of blocks to read.
 * @return      Returns RES_OK on success, otherwise failure.
 */
int tlkmw_fs_diskio_read(uint8_t* buff, uint32_t lba, uint32_t cnt)
{
    tlkos_recursiveMutex_lock(sTlkmwFsMutex);
    tlkmw_fs_diskio_sleepCheck();
    spTlkmwFsDiskIo->read(buff, lba, cnt);
    tlkos_recursiveMutex_unlock(sTlkmwFsMutex);
    return RES_OK;
}

/**
 * @brief       This function initializes the disk IO.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_fs_diskio_init(void)
{
    tlksys_devmgr_mount(&sTlkmwFsDev,NULL);
    tlkos_recursiveMutex_create(&sTlkmwFsMutex);
    spTlkmwFsDiskIo->init();
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM,&sTlkmwFsSleepTmr,10 * 1000 * 1000,false,tlkmw_fs_diskio_sleepTmr,NULL);
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM,&sTlkmwFsSleepTmr);
}

/**
 * @brief       This function gets the block count.
 * @param[in]   none.
 * @return      Returns the block count.
 */
uint32_t tlkmw_fs_diskio_getBlkCount(void)
{
    return spTlkmwFsDiskIo->getSectorNum();
}

//rewrite fs api:
/**
 * @brief       This function writes data to the file system disk IO.
 * @param[in]   buff    - the buffer containing data to write.
 * @param[in]   sector  - the sector address to write to.
 * @param[in]   cnt     - the number of sectors to write.
 * @return      Returns 0 on success, otherwise failure.
 */
int tlkfs_diskio_write(uint8_t *buff, uint32_t sector, uint32_t cnt)
{
    return tlkmw_fs_diskio_write(buff,sector,cnt);
}

/**
 * @brief       This function reads data from the file system disk IO.
 * @param[out]  buff    - the buffer to store read data.
 * @param[in]   sector  - the sector address to read from.
 * @param[in]   cnt     - the number of sectors to read.
 * @return      Returns 0 on success, otherwise failure.
 */
int tlkfs_diskio_read(uint8_t *buff, uint32_t sector, uint32_t cnt)
{
    return tlkmw_fs_diskio_read(buff,sector,cnt);
}

/**
 * @brief       This function gets the disk information.
 * @param[in]   none.
 * @return      Returns the disk information.
 */
tlkfs_diskio_info_t tlkfs_diskio_getDiskInfo(void)
{
    tlkfs_diskio_info_t info = {
        .sector_num = spTlkmwFsDiskIo->getSectorNum(),
        .sector_size = spTlkmwFsDiskIo->getSectorSize(),
    };
    return info;
}


#endif