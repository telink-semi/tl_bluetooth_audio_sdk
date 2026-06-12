/********************************************************************************************************
 * @file    tlkmw_usb_msc.c
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
#if TLK_USB_MSC_ENABLE
#include "tlkapi/tlkapi_common.h"
#include "drv/tlkmw_fs_diskio.h"
#include "tlklib/usb/msc/tlkusb_msc.h"
#include "tlklib/usb/tlkusb.h"

/**
 * @brief       This function reads data from the disk for MSC.
 * @param[out]  pBuff    - the buffer to store read data.
 * @param[in]   blkOffs  - the block offset.
 * @param[in]   blkNumb  - the number of blocks to read.
 * @return      Returns TLK_ENONE on success, otherwise failure.
 */
static int tlkmw_msc_diskRead(uint8_t *pBuff, uint32_t blkOffs, uint32_t blkNumb)
{
    int ret = tlkmw_fs_diskio_read(pBuff, blkOffs, blkNumb);
    return ret == 0 ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function writes data to the disk for MSC.
 * @param[in]   pData    - the data to write.
 * @param[in]   blkOffs  - the block offset.
 * @param[in]   blkNumb  - the number of blocks to write.
 * @return      Returns TLK_ENONE on success, otherwise failure.
 */
static int tlkmw_msc_diskWrite(uint8_t *pData, uint32_t blkOffs, uint32_t blkNumb)
{
    int ret = tlkmw_fs_diskio_write(pData, blkOffs, blkNumb);
    return ret == 0 ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function gets the block count for MSC.
 * @param[in]   none.
 * @return      Returns the block count.
 */
static uint32_t tlkmw_msc_diskGetBlkCount(void)
{
    return tlkmw_fs_diskio_getBlkCount();
}

static tlkusb_msc_disk_var_t   sTlkMwMscDiskVar = {0};
static const tlkusb_msc_disk_t sTlkMwMscDisk    = {
       .blkSize     = TLK_CFG_FS_SECTOR_SIZE,
       .getBlkCount = tlkmw_msc_diskGetBlkCount,
       .pVendorStr  = "TLK-DISK",         //<=8
       .pProductStr = "Telink Disk Demo", //<=16
       .pVersionStr = "1.02",             //<=4
       .Read        = tlkmw_msc_diskRead,
       .Write       = tlkmw_msc_diskWrite,
       .pVar        = &sTlkMwMscDiskVar,
};

/**
 * @brief       This function initializes the USB MSC.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_msc_init(void)
{
    tlkusb_msc_appendDisk((tlkusb_msc_disk_t *)&sTlkMwMscDisk);
    tlksys_pm_regChn(TLKSYS_PM_CHN_MSC);
    tlksys_pm_setChn(TLKSYS_PM_CHN_MSC, 0, 0);
}

/**
 * @brief       This function opens the USB MSC.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_msc_open(void)
{
#if TLK_CFG_USB_ENABLE
    sTlkMwMscDiskVar.isReady = 1;
    tlkusb_hal_disable_eventMode();
    tlksys_pm_setChn(TLKSYS_PM_CHN_MSC, 0, 1);
    tlkusb_init(TLK_CFG_USB_MSC_INDEX, 0x124);
    tlkusb_open(TLK_CFG_USB_MSC_INDEX, TLKUSB_MODTYPE_MSC);
    tlkusb_hal_enable_eventMode();
#endif
}

/**
 * @brief       This function closes the USB MSC.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_msc_close(void)
{
#if TLK_CFG_USB_ENABLE
    memset(&sTlkMwMscDiskVar, 0, sizeof(sTlkMwMscDiskVar));
    tlkusb_close(TLK_CFG_USB_MSC_INDEX);
    tlksys_pm_setChn(TLKSYS_PM_CHN_MSC, 0, 0);
#endif
}

#endif
