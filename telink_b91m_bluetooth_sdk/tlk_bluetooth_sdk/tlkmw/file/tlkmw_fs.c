/********************************************************************************************************
 * @file    tlkmw_fs.c
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
#include "tlkapi/tlkapi_common.h"
#include "tlklib/fatfs/ff/source/ff.h"
#include "tlkmw_usb_msc.h"
#include "drv/tlkmw_fs_diskio.h"

static FATFS sTlkmwFatfsObj = {0};

/**
 * @brief       This function mounts the file system.
 * @param[in]   fs    - the file system object to mount.
 * @return      none.
 */
static void tlkmw_fs_mount(FATFS *fs)
{
    uint8_t sector_buf[FF_MAX_SS];
    FRESULT res = f_mount(fs, "0:", 1);

    if (res == FR_OK) {
        return;
    }
    MKFS_PARM opt = {
        .fmt     = FM_FAT32,
        .n_fat   = 2,
        .align   = 0,
        .n_root  = 0,
        .au_size = 64 * 1024,
    };
    tlk_printf("Mount FS failed, res = %d, start formatting...", res);

    res = f_mkfs("0:", &opt, sector_buf, sizeof(sector_buf));

    f_mount(NULL, "0:", 0);

    res = f_mount(fs, "0:", 1);
    f_setlabel("0:TLK-DEMO");
    tlk_printf(res == FR_OK ? "Make mount success %d!" : "Make mount failed %d!", res);
}

/**
 * @brief       This function initializes the core file system.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_core_init(void)
{
    tlkmw_fs_diskio_init();
    tlkmw_fs_mount(&sTlkmwFatfsObj);
    tlk_printf("tlkmw_fs_core_init ok");
}

/**
 * @brief       This function initializes the file system.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_fs_init(void)
{
    tlkmw_fs_core_init();
#if TLK_USB_MSC_ENABLE
    tlkmw_msc_init();
#endif
}

#endif