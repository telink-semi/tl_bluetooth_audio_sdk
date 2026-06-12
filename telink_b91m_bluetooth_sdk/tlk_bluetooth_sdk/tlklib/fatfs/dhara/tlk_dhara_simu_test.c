/********************************************************************************************************
 * @file    tlk_dhara_simu_test.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tlklib/fatfs/dhara/nand.h"
#include "tlklib/fatfs/dhara/map.h"
#include <stdint.h>
#include <stdbool.h>

#define TLK_DHARA_SIMU_TEST 0
#if TLK_DHARA_SIMU_TEST

#define MOCK_NAND_BLOCKS        128     
#define MOCK_NAND_PAGES_PER_BLOCK 4   
#define MOCK_NAND_PAGE_SIZE     512

#define my_printf(fmt, ...)  {tlkos_task_delayMs(2);tlk_printf(fmt, ##__VA_ARGS__);}
    



struct mock_nand_context {
    uint8_t data[MOCK_NAND_BLOCKS][MOCK_NAND_PAGES_PER_BLOCK][MOCK_NAND_PAGE_SIZE];
    uint8_t is_written[MOCK_NAND_BLOCKS][MOCK_NAND_PAGES_PER_BLOCK];
    uint8_t is_bad[MOCK_NAND_BLOCKS];
    uint32_t  erase_cnt[MOCK_NAND_BLOCKS];
};

struct mock_nand_context g_tlk_dhara_test_nand = {0};

void tlk_dhara_test_my_nand_init(void)
{
    uint32_t *data = (uint32_t *)g_tlk_dhara_test_nand.data;
    for(size_t i = 0; i < MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK * MOCK_NAND_PAGE_SIZE / sizeof(uint32_t) ;i++){
        data[i] = 0xFFFFFFFF;
    }
}


int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t b) {
    (void) n;
    if (b >= MOCK_NAND_BLOCKS) return 1;
    return g_tlk_dhara_test_nand.is_bad[b] ? 1 : 0;
}

void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t b) {
    (void) n;
    if (b < MOCK_NAND_BLOCKS) {
        g_tlk_dhara_test_nand.is_bad[b] = 1;
        my_printf("Marked bad block: %d\n", b);
    }
}

int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) 
{
    (void) n;
    // my_printf("dhara_nand_erase %d",b);
    if (b >= MOCK_NAND_BLOCKS || g_tlk_dhara_test_nand.is_bad[b]) {
        if (err) *err = DHARA_E_BAD_BLOCK;
        return -1;
    }
    for(size_t i = 0;i < MOCK_NAND_PAGES_PER_BLOCK;i++){
        g_tlk_dhara_test_nand.is_written[b][i] = 0;
        for(size_t index = 0;index < MOCK_NAND_PAGE_SIZE;index++){
            g_tlk_dhara_test_nand.data[b][i][index] = 0xFF;
        }
    }
    g_tlk_dhara_test_nand.erase_cnt[b]++;
    
    if (err) *err = DHARA_E_NONE;
    return 0;
}

int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
    (void) n;
    
    if (p >= MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK) {
        return -1;
    }
    
    dhara_block_t block = p / MOCK_NAND_PAGES_PER_BLOCK;
    if (g_tlk_dhara_test_nand.is_bad[block]) {
        if (err) *err = DHARA_E_BAD_BLOCK;
        return -1;
    }
    g_tlk_dhara_test_nand.is_written[block][p % MOCK_NAND_PAGES_PER_BLOCK] = 1;
    for(size_t index = 0;index < MOCK_NAND_PAGE_SIZE;index++){
        g_tlk_dhara_test_nand.data[block][p % MOCK_NAND_PAGES_PER_BLOCK][index] = data[index];
    }
    if (err) *err = DHARA_E_NONE;
    return 0;
}

int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p) {
    (void) n;
    if (p >= MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK) return 0;
    dhara_block_t block = p / MOCK_NAND_PAGES_PER_BLOCK;
    if(g_tlk_dhara_test_nand.is_written[block][p % MOCK_NAND_PAGES_PER_BLOCK]){
        return 0;
    }
    
    return 1;
}

int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p,
                    size_t offset, size_t length, uint8_t *data,
                    dhara_error_t *err) {
    (void) n;
    if (p >= MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK) {
        return -1;
    }
    
    if (offset + length > MOCK_NAND_PAGE_SIZE) {
        return -1;
    }
    dhara_block_t block = p / MOCK_NAND_PAGES_PER_BLOCK;
    for(size_t index = 0;index < length;index++){
        data[index] = g_tlk_dhara_test_nand.data[block][p % MOCK_NAND_PAGES_PER_BLOCK][index + offset];
    }
    
    
    if (err) *err = DHARA_E_NONE;
    return 0;
}

int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src,dhara_page_t dst, dhara_error_t *err) {
    (void) n;
    if (src >= MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK ||
        dst >= MOCK_NAND_BLOCKS * MOCK_NAND_PAGES_PER_BLOCK) {
        return -1;
    }
    
    dhara_block_t dst_block = dst / MOCK_NAND_PAGES_PER_BLOCK;
    dhara_block_t src_block = src / MOCK_NAND_PAGES_PER_BLOCK;
    for(size_t index = 0;index < MOCK_NAND_PAGE_SIZE;index++){
        g_tlk_dhara_test_nand.data[dst_block][dst % MOCK_NAND_PAGES_PER_BLOCK][index] = g_tlk_dhara_test_nand.data[src_block][src % MOCK_NAND_PAGES_PER_BLOCK][index];
    }
    g_tlk_dhara_test_nand.is_written[dst_block][dst % MOCK_NAND_PAGES_PER_BLOCK] = 1;
    
    if (err) *err = DHARA_E_NONE;
    return 0;
}

void mock_nand_dump_info(void) {
    my_printf("\n=== NAND Simulator Statistics ===\n");
    
    uint32_t total_erases = 0;
    uint32_t max_erases = 0;
    uint32_t min_erases = UINT32_MAX;
    
    for (int i = 0; i < MOCK_NAND_BLOCKS; i++) {
        uint32_t count = g_tlk_dhara_test_nand.erase_cnt[i];
        total_erases += count;
        if (count > max_erases) max_erases = count;
        if (count < min_erases) min_erases = count;
    }
    
    my_printf("Total erase operations: %u\n", total_erases);
    my_printf("Maximum erase count: %u\n", max_erases);
    my_printf("Minimum erase count: %u\n", min_erases);
    my_printf("Wear leveling difference: %u\n", max_erases - min_erases);

    for(uint32_t i = 0;i < MOCK_NAND_BLOCKS ; i+=8){
        my_printf("%d %d %d %d %d %d %d %d", g_tlk_dhara_test_nand.erase_cnt[i],g_tlk_dhara_test_nand.erase_cnt[i+1],g_tlk_dhara_test_nand.erase_cnt[i+2],g_tlk_dhara_test_nand.erase_cnt[i+3]
        ,g_tlk_dhara_test_nand.erase_cnt[i+4],g_tlk_dhara_test_nand.erase_cnt[i+5],g_tlk_dhara_test_nand.erase_cnt[i+6],g_tlk_dhara_test_nand.erase_cnt[i+7]);
    } 
}

static struct dhara_nand my_nand = {
    .log2_page_size = 9,   // 512 bytes page size (2^9)
    .log2_ppb = 2,          // 4 pages per block (2^2)
    .num_blocks = 128       // 128 blocks
};

static void tlk_dhara_test_wear_leveling(void) {
    my_printf("\n=== Wear Leveling Test ===\n");
    
    struct dhara_map map = {0};
    uint8_t page_buffer[MOCK_NAND_PAGE_SIZE] = {0};
    dhara_map_init(&map, &my_nand, page_buffer, 0);
    dhara_map_clear(&map);

    my_printf("Performing 100000 random writes...\n");
    for (int i = 0; i < 100000; i++) {
        uint8_t data[MOCK_NAND_PAGE_SIZE];
        memset(data, i & 0xff, sizeof(data));
        dhara_sector_t sector = trng_rand() % 64;  // 64 logical sectors
        dhara_error_t error = 0;
        int ret = dhara_map_write(&map, sector, data, &error);
        if(ret != 0){
            my_printf("dhara_map_write error %d",error);
        }
        
        if (i % 1000 == 0) {
            my_printf("Completed %d writes\n", i);
        }
    }
    
    mock_nand_dump_info();
    tlkos_task_delayMs(100);

    my_printf("Performing 100000 writes...\n");
    for (int i = 0; i < 100000; i++) {
        uint8_t data[MOCK_NAND_PAGE_SIZE];
        memset(data, i & 0xff, sizeof(data));
        dhara_sector_t sector = i % 64;  // 64 logical sectors
        dhara_error_t error = 0;
        int ret = dhara_map_write(&map, sector, data, &error);
        if(ret != 0){
            my_printf("dhara_map_write error %d",error);
        }
        
        if (i % 1000 == 0) {
            my_printf("Completed %d writes\n", i);
        }
    }
    
    mock_nand_dump_info();
    tlkos_task_delayMs(100);

    my_printf("Performing 100000 1 block writes...\n");
    for (int i = 0; i < 100000; i++) {
        uint8_t data[MOCK_NAND_PAGE_SIZE];
        memset(data, i & 0xff, sizeof(data));
        dhara_sector_t sector = 0;  // 64 logical sectors
        dhara_error_t error = 0;
        int ret = dhara_map_write(&map, sector, data, &error);
        if(ret != 0){
            my_printf("dhara_map_write error %d",error);
        }
        
        if (i % 1000 == 0) {
            my_printf("Completed %d writes\n", i);
        }
    }
    
    mock_nand_dump_info();
    tlkos_task_delayMs(100);
}


void tlk_dhara_test_power_loss_recovery(void) {
    my_printf("\n=== Power Loss Recovery Test ===\n");
    
    struct dhara_map map = {0};
    uint8_t page_buffer[MOCK_NAND_PAGE_SIZE] = {0};
    dhara_map_init(&map, &my_nand, page_buffer, 0);
    
    uint8_t data1[MOCK_NAND_PAGE_SIZE], data2[MOCK_NAND_PAGE_SIZE], read_data[MOCK_NAND_PAGE_SIZE];
    
    // Prepare different datasets
    for (int i = 0; i < MOCK_NAND_PAGE_SIZE; i++) {
        data1[i] = i & 0xFF;
        data2[i] = (~i) & 0xFF;
    }
    
    // Write data
    my_printf("Writing first dataset...\n");
    dhara_map_clear(&map);
    dhara_map_write(&map, 0, data1, NULL);
    dhara_map_write(&map, 1, data1, NULL);
    
    // Sync point
    dhara_map_sync(&map, NULL);
    my_printf("Reached sync point\n");
    
    // Continue writing
    my_printf("Writing second dataset...\n");
    dhara_map_write(&map, 0, data2, NULL);
    dhara_map_write(&map, 2, data2, NULL);
    
    // Simulate power loss: reinitialize
    my_printf("Simulating power loss, reinitializing...\n");
    dhara_map_resume(&map, NULL);
    
    // Verify data
    dhara_map_read(&map, 0, read_data, NULL);
    if (memcmp(data1, read_data, MOCK_NAND_PAGE_SIZE) == 0) {
        my_printf("Sector 0 recovered correctly (rolled back to sync point)\n");
    } else if (memcmp(data2, read_data, MOCK_NAND_PAGE_SIZE) == 0) {
        my_printf("Sector 0 contains new data\n");
    } else {
        my_printf("Sector 0 data abnormal\n");
    }
    
    dhara_map_read(&map, 1, read_data, NULL);
    if (memcmp(data1, read_data, MOCK_NAND_PAGE_SIZE) == 0) {
        my_printf("Sector 1 data correct\n");
    } else {
        my_printf("Sector 1 data abnormal\n");
    }
}


static void tlk_dhara_test_main(void *arg)
{
    (void) arg;
    (void) tlk_dhara_test_power_loss_recovery;
    (void) tlk_dhara_test_wear_leveling;
    tlk_dhara_test_my_nand_init();
    tlkos_task_delayMs(2000);
    // tlk_dhara_test_wear_leveling();
    tlk_dhara_test_power_loss_recovery();
    tlkos_task_destroy(NULL);
    while (1);    
}

void tlkapp_sys_taskInitCompletedHook(void)
{
    _attribute_os_heap_sec_
    static uint8_t buffer[50 * 1024];
    TlkosTaskExtCfg_t cfg = {
        .pStaticBuffer = buffer,
        .staticBufferSize = sizeof(buffer),        
    };
    tlkos_task_create(tlk_dhara_test_main,"dhara_test",50 * 1024,1,NULL,&cfg,NULL);
}

#endif
