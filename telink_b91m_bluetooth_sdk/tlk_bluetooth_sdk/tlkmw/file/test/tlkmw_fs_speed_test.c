/********************************************************************************************************
 * @file    tlkmw_fs_speed_test.c
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

#define TLK_CFG_FS_SPEED_TEST_CASE_EN 0

#if TLK_CFG_FS_SPEED_TEST_CASE_EN && TLK_CFG_FS_ENABLE
#include "drivers.h"
#include "tlkapi/tlkapi_common.h"
#include "../drv/tlkmw_fs_diskio.h"
#include "../tlkmw_fs.h"
#include "tlkmw/tlkmw.h"

#define TLK_CFG_FS_SPEED_TEST_CASE_MB 5

void tlkmw_fs_speed_test_main(void)
{
    uint8_t               buffer[512] = {0};
    TlkOsFileCtrlHandle_t file        = tlkmw_fs_mallocFileCtrl();
    if (!file) {
        return;
    }

    if (tlkmw_fs_createNewFile(file, "0:/tlk_fs_test.txt") == TLK_ENONE) {
        uint32_t begin_time = clock_time();
        for (uint32_t i = 0; i < 1024 * 2 * TLK_CFG_FS_SPEED_TEST_CASE_MB; i++) {
            tlkmw_fs_write(file, buffer, sizeof(buffer));
        }
        uint32_t end_time = clock_time();
        tlk_printf("fs write %dMB cost time %dus", TLK_CFG_FS_SPEED_TEST_CASE_MB, (end_time - begin_time) / SYSTEM_TIMER_TICK_1US);
    }
    tlkmw_fs_close(file);

    if (tlkmw_fs_openForRead(file, "0:/tlk_fs_test.txt") == TLK_ENONE) {
        uint32_t begin_time = clock_time();
        for (uint32_t i = 0; i < 1024 * 2 * TLK_CFG_FS_SPEED_TEST_CASE_MB; i++) {
            tlkmw_fs_read(file, buffer, sizeof(buffer), NULL);
        }
        uint32_t end_time = clock_time();
        tlk_printf("fs read %dMB cost time %dus", TLK_CFG_FS_SPEED_TEST_CASE_MB, (end_time - begin_time) / SYSTEM_TIMER_TICK_1US);
    }
    tlkmw_fs_close(file);
    tlkmw_fs_deleteFile("0:/tlk_fs_test.txt");
    tlkmw_fs_freeFileCtrl(file);
}

void tlkmw_fs_speed_test_task(void *arg)
{
    (void)arg;
    tlkos_task_delayMs(5000);
    tlkmw_fs_speed_test_main();
    tlkos_task_destroy(NULL);
}

void tlkapp_sys_taskInitCompletedHook(void)
{
    tlkos_task_create(tlkmw_fs_speed_test_task, "tlkmw_fs_speed_test_task", 10 * 1024, 1, NULL, NULL, NULL);
}


#endif
