/********************************************************************************************************
 * @file    tlk_flashdb_test.c
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
#include "../inc/flashdb.h"

#define TLK_FLASHDB_TEST 0
#if TLK_FLASHDB_TEST

static struct fdb_kvdb kvdb = {0};

static void tlk_flashdb_clean(void)
{
    struct fdb_kv_iterator itr;

    fdb_kv_iterator_init(&kvdb, &itr);

    while (fdb_kv_iterate(&kvdb,&itr)) {
        fdb_kv_del(&kvdb, itr.curr_kv.name); 
    }
}

static void tlk_flashdb_basic_test(void)
{
    struct fdb_blob blob;
    uint32_t open_cnt = 0;
    fdb_kv_get_blob(&kvdb, "key1", fdb_blob_make(&blob, &open_cnt, sizeof(open_cnt)));
    tlk_printf("open_cnt %d",open_cnt);
    open_cnt++;
    fdb_kv_set_blob(&kvdb, "key1", fdb_blob_make(&blob, &open_cnt, sizeof(open_cnt)));
}

static void tlk_flashdb_write_speed_test(void)
{
    struct fdb_blob blob;
    uint8_t buffer[2048] = {0};
    for(uint32_t val_len = 1; val_len < 1024; val_len = val_len * 2) {
        uint32_t timeUs = 0;
        uint32_t minTime = 0XFFFFFFFFU;
        uint32_t maxTime = 0;
        fdb_err_t ret = 0;
        uint32_t cnt = 0;
        for(; cnt < 256; cnt++) {
            tlkos_task_suspendAll();
            uint32_t begin_time = clock_time();
            ret = fdb_kv_set_blob(&kvdb, "1", fdb_blob_make(&blob, buffer, val_len));
            uint32_t end_time = clock_time();
            tlkos_task_resumeAll();
            uint32_t pastTimeUs = (end_time - begin_time) / SYSTEM_TIMER_TICK_1US;
            timeUs += pastTimeUs;
            minTime = min(minTime,pastTimeUs);
            maxTime = max(maxTime,pastTimeUs);
            if(ret){
                break;
            }
        }
        tlk_printf("flashdb set %d %d",ret,cnt);
        tlk_printf("write len = %d avg %dus min %dus max %dus",val_len, timeUs / cnt,minTime,maxTime);
        fdb_kv_del(&kvdb,"1");
    }
}

static int fal_telink_flash_erase(long offset, size_t size)
{
    (void) offset;
    (void) size;
    uint32_t addr = tlkhal_flash_get_size() + 0xE0000 - 0x100000;
    uint32_t sec_num = (size + (4095)) / 4096;
    for(uint32_t i = 0; i < sec_num ;i++) {
        flash_erase_sector(addr + offset + i * 0x1000);
    }
    return 0;
}

static void tlk_flashdb_test_main(void *arg)
{
    (void) arg;
    (void) tlk_flashdb_clean;
    (void) tlk_flashdb_basic_test;
    (void) tlk_flashdb_write_speed_test;
    tlkos_task_delayMs(1500);
    fal_telink_flash_erase(0,32 * 1024);
    fdb_err_t result = fdb_kvdb_init(&kvdb, "telink", "disk0", NULL, NULL);
    if (result != FDB_NO_ERR) {
        tlk_printf("flashdb init %d",result);
        tlkos_task_destroy(NULL);   
    }
    tlk_flashdb_clean();
    // tlk_flashdb_basic_test();
    tlk_flashdb_write_speed_test();

    tlkos_task_destroy(NULL);
    while(1);
}

void tlkapp_sys_taskInitCompletedHook(void)
{
    _attribute_os_heap_sec_
    static uint8_t buffer[10 * 1024];
    TlkosTaskExtCfg_t cfg = {
        .pStaticBuffer = buffer,
        .staticBufferSize = sizeof(buffer),        
    };
    tlkos_task_create(tlk_flashdb_test_main,"flashdb_test",10 * 1024,1,NULL,&cfg,NULL);
}

#endif
