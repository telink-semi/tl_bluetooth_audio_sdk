/********************************************************************************************************
 * @file    tlkmw_fs_drv_ds35x.c
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
#include "../tlkmw_fs_diskio.h"
#if TLK_CFG_FS_ENABLE && MCU_CORE_TYPE == CHIP_TYPE_TL721X && TLKMW_FS_DISK_IO_SELECT == TLKMW_FS_DISK_IO_DS35X
#include "drivers.h"
#include "hal_spi.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/fatfs/dhara/nand.h"
#include "tlklib/fatfs/dhara/map.h"

#define DS35X_PAGE_SIZE       2112
#define DS35X_PAGE_DATA_SIZE  2048
#define DS35X_PAGE_SPARE_SIZE 64
#define DS35X_PAGES_PER_BLK   64
#define DS35X_TOTAL_BLOCKS    1024

//nand spi cmd,refer to ds35x's data sheet
#define DS35X_CMD_WREN         0x06
#define DS35X_CMD_PAGE_READ    0x13
#define DS35X_CMD_READ_CACHE   0x03
#define DS35X_CMD_READ_CACHE2  0x3b
#define DS35X_CMD_READ_CACHE4  0x6b
#define DS35X_CMD_PROG_LOAD    0x02
#define DS35X_CMD_PROG_LOAD4   0x32
#define DS35X_CMD_PROG_EXEC    0x10
#define DS35X_CMD_BLK_ERASE    0xD8
#define DS35X_CMD_GET_FEATURE  0x0F
#define DS35X_CMD_SET_FEATURE  0x1F
#define DS35X_CMD_READ_ID      0x9F
#define DS35X_CMD_RESET        0xFF

#define DS35X_FEATURE_BLK_LOCK 0xA0
#define DS35X_FEATURE_OTP      0xB0
#define DS35X_FEATURE_STATUS   0xC0

#define DS35X_DUMMY_BYTE       0X00 //do not use 0XFF,0xFF is reset cmd...

// #define DS35X_OPCODE_UNLOCK_ALL 0
// #define DS35X_OPCODE_ONLY_LOCK_BLCOK0 0

#define DS35X_SPI_QUAD_MODE_EN   0 //not ok now,device may have bug

#define DS35X_DEBUG_ERR_LOG_EN   1
#define DS35X_DEBUG_TRACE_LOG_EN 0

#if DS35X_DEBUG_TRACE_LOG_EN
#define ds35_trace(fmt, ...)            \
    {                                   \
        tlkos_task_delayMs(4);          \
        tlk_printf(fmt, ##__VA_ARGS__); \
    }
#else
#define ds35_trace(fmt, ...)
#endif
typedef struct
{
    uint8_t test;
} tlkmw_fs_drv_ds35x_core_info_t;

typedef enum
{
    FS_DRV_DS35X_NOINIT = 0,
    FS_DRV_DS35X_NOT_FOUND,
    FS_DRV_DS35X_ERR,
    FS_DRV_DS35X_READY,
} tlkmw_fs_drv_ds35x_state_e;

typedef struct
{
    uint32_t         state; //refer to tlkmw_fs_drv_ds35x_state_e
    uint8_t          badMark[DS35X_TOTAL_BLOCKS / 8];
    uint8_t          cache[DS35X_PAGE_SIZE];
    uint8_t          dhara_mempool[DS35X_PAGE_DATA_SIZE];
    struct dhara_map dhara;
} tlkmw_fs_drv_ds35x_ctrl_t;

static tlkmw_fs_drv_ds35x_ctrl_t s_tlkmw_fs_drv_ds35x_ctrl = {0};
#undef tlk_this
#define tlk_this ((tlkmw_fs_drv_ds35x_ctrl_t *)(&s_tlkmw_fs_drv_ds35x_ctrl))

static inline void tlkmw_fs_drv_ds35x_delayMs(uint32_t ms)
{
    if (tlkos_task_delayMs(ms) != TLK_ENONE) {
        delay_ms(ms);
    }
}

static inline void tlkmw_fs_drv_ds35x_spi_enable_fast_read_mode(uint8_t en)
{
    (void)en;
#if DS35X_SPI_QUAD_MODE_EN
    hal_spi_master_quad_mode_en(en, 1);
#endif
}

static inline void tlkmw_fs_drv_ds35x_spi_enable_fast_write_mode(uint8_t en)
{
    (void)en;
#if DS35X_SPI_QUAD_MODE_EN
    hal_spi_master_quad_mode_en(en, 1);
#endif
}

/**
bit0  OIP  1=Busy
bit1  WEL  1=Write Enable
bit2  E_FAIL 1=Erase Fail
bit3  P_FAIL 1=Program Fail
bit[5:4] ECC 00 = No error 
             01 = 1~4bit error,can get correct result
             10 = over 4 bit error,GG...make it bad block.
**/

static uint8_t tlkmw_fs_drv_ds35x_wait_ready(uint32_t info)
{
    //must wait st == 0
    uint8_t  st   = 0x01;
    uint32_t tick = clock_time();
    while (st & BIT(0)) {
        hal_spi_master_sel(0);
        hal_spi_master_byte_trans(DS35X_CMD_GET_FEATURE, NULL);
        hal_spi_master_byte_trans(DS35X_FEATURE_STATUS, NULL);
        hal_spi_master_byte_trans(DS35X_DUMMY_BYTE, &st);
        hal_spi_master_sel(1);
        if (clock_time_exceed(tick, 1 * 1000 * 1000)) {
            tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "<DS35><error> timeout:%x %d", st, info);
            return false;
        }
    }
    if (st & (BIT(2) | BIT(3))) {
        tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "<DS35><error> erase/prog error state:%x %d", st, info);
        return false;
    }
    if ((st & (BIT(4) | BIT(5))) == BIT(5)) {
        tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "<DS35><error> ecc error state:%x %d", st, info);
        return false;
    }
    return true;
}

static void tlkmw_fs_drv_ds35x_reset(void)
{
    hal_spi_master_sel(0);
    hal_spi_master_byte_trans(DS35X_CMD_RESET, NULL);
    hal_spi_master_sel(1);
}

static uint8_t tlkmw_fs_drv_ds35x_check_id(void)
{
    uint8_t id[2] = {0};
    hal_spi_master_sel(0);
    hal_spi_master_byte_trans(DS35X_CMD_READ_ID, NULL);
    hal_spi_master_byte_trans(DS35X_DUMMY_BYTE, NULL);
    hal_spi_master_byte_trans(DS35X_DUMMY_BYTE, id);
    hal_spi_master_byte_trans(DS35X_DUMMY_BYTE, id + 1);
    hal_spi_master_sel(1);
    if (id[0] != 0xE5) {
        return false;
    }
    if (id[1] != 0x71 && id[1] != 0x21) {
        return false;
    }
    return true;
}

static uint8_t tlkmw_fs_drv_ds35x_enable_write(void)
{
    hal_spi_master_sel(0);
    hal_spi_master_byte_trans(DS35X_CMD_WREN, NULL);
    hal_spi_master_sel(1);
    return true;
}

static uint8_t tlkmw_fs_drv_ds35x_unlock_all_blocks(void)
{
    tlkmw_fs_drv_ds35x_enable_write();
    uint8_t cmd[3] = {DS35X_CMD_SET_FEATURE, DS35X_FEATURE_BLK_LOCK, 0x00};
    hal_spi_master_sel(0);
    hal_spi_master_write(cmd, 3);
    hal_spi_master_sel(1);
    return tlkmw_fs_drv_ds35x_wait_ready(0);
}

static void tlkmw_fs_drv_ds35x_set_qe(void)
{
    tlkmw_fs_drv_ds35x_enable_write();
    uint8_t cmd[3] = {DS35X_CMD_SET_FEATURE, DS35X_FEATURE_OTP, 0x11};
    hal_spi_master_sel(0);
    hal_spi_master_write(cmd, 3);
    hal_spi_master_sel(1);
    tlkmw_fs_drv_ds35x_wait_ready(0);
}

static uint8_t tlkmw_fs_drv_ds35x_page_read(uint32_t page, uint16_t offset, uint8_t *buf)
{
    uint8_t read_page_cmd[4] = {
        [0] = DS35X_CMD_PAGE_READ,
        [1] = 0x00,
        [2] = page >> 8,
        [3] = page & 0xFF,
    };
    hal_spi_master_sel(0);
    hal_spi_master_write(read_page_cmd, sizeof(read_page_cmd));
    hal_spi_master_sel(1);
    uint8_t ret = tlkmw_fs_drv_ds35x_wait_ready(page);
    if (ret == false) {
        return false;
    }
    uint8_t read_cache_cmd[4] = {
#if DS35X_SPI_QUAD_MODE_EN
        [0] = DS35X_CMD_READ_CACHE4,
#else
        [0] = DS35X_CMD_READ_CACHE,
#endif
        [1] = offset >> 8,
        [2] = offset & 0xff,
        [3] = DS35X_DUMMY_BYTE,
    };
    hal_spi_master_sel(0);
    hal_spi_master_write(read_cache_cmd, sizeof(read_cache_cmd));

    tlkmw_fs_drv_ds35x_spi_enable_fast_read_mode(1);
    hal_spi_master_read(buf, DS35X_PAGE_SIZE - offset);
    tlkmw_fs_drv_ds35x_spi_enable_fast_read_mode(0);

    hal_spi_master_sel(1);
    return true;
}

static uint8_t tlkmw_fs_drv_ds35x_page_prog(uint32_t page, uint8_t *buf)
{
    tlkmw_fs_drv_ds35x_enable_write();

    uint8_t load_cmd[3] = {
#if DS35X_SPI_QUAD_MODE_EN
        [0] = DS35X_CMD_PROG_LOAD4,
#else
        [0] = DS35X_CMD_PROG_LOAD,
#endif
        [1] = 0x00,
        [2] = 0x00,
    };
    hal_spi_master_sel(0);
    hal_spi_master_write(load_cmd, sizeof(load_cmd));

    tlkmw_fs_drv_ds35x_spi_enable_fast_write_mode(1);
    hal_spi_master_write(buf, DS35X_PAGE_SIZE);
    tlkmw_fs_drv_ds35x_spi_enable_fast_write_mode(0);

    hal_spi_master_sel(1);

    uint8_t exec_cmd[4] = {DS35X_CMD_PROG_EXEC, 0x00, page >> 8, page & 0xFF};
    hal_spi_master_sel(0);
    hal_spi_master_write(exec_cmd, sizeof(exec_cmd));
    hal_spi_master_sel(1);
    return tlkmw_fs_drv_ds35x_wait_ready(page);
}

static uint8_t tlkmw_fs_drv_ds35x_block_erase(uint32_t block)
{
    tlkmw_fs_drv_ds35x_enable_write();
    block                = block * DS35X_PAGES_PER_BLK;
    uint8_t erase_cmd[4] = {DS35X_CMD_BLK_ERASE, 0x00, block >> 8, block & 0xFF};
    hal_spi_master_sel(0);
    hal_spi_master_write(erase_cmd, sizeof(erase_cmd));
    hal_spi_master_sel(1);
    return tlkmw_fs_drv_ds35x_wait_ready(block);
}

static void tlkmw_fs_drv_ds35x_check_bad_blocks(void)
{
    uint8_t  buffer[DS35X_PAGE_SPARE_SIZE];
    uint32_t bad_cnt = 0;
    for (size_t block = 0; block < DS35X_TOTAL_BLOCKS; block++) {
        buffer[0] = 0;
        tlkmw_fs_drv_ds35x_page_read(block * DS35X_PAGES_PER_BLK, DS35X_PAGE_DATA_SIZE, buffer);
        if (buffer[0] != 0xff) {
            bad_cnt++;
            tlk_this->badMark[block / 8] |= 1 << (block % 8);
            continue;
        }
        buffer[0] = 0;
        tlkmw_fs_drv_ds35x_page_read(block * DS35X_PAGES_PER_BLK + 1, DS35X_PAGE_DATA_SIZE, buffer);
        if (buffer[0] != 0xff) {
            bad_cnt++;
            tlk_this->badMark[block / 8] |= 1 << (block % 8);
            continue;
        }
    }
    tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "<DS35><trace> bad block count %d", bad_cnt)
}

static void tlkmw_fs_drv_ds35x_mark_bad_block(uint32_t block)
{
    tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "<DS35><error> mark bad block %d", block);
    uint8_t *buf = tlk_this->cache;
    memset(buf, 0, DS35X_PAGE_SIZE);
    tlkmw_fs_drv_ds35x_page_prog(block * DS35X_PAGES_PER_BLK, buf);
    tlkmw_fs_drv_ds35x_page_prog(block * DS35X_PAGES_PER_BLK + 1, buf);
    tlk_this->badMark[block / 8] |= (1 << (block % 8));
}

static uint8_t tlkmw_fs_drv_ds35x_init(void)
{
    hal_spi_master_init();
    hal_spi_master_quad_mode_en(0, 1);
    hal_spi_master_clk_set(SPI_CLK_40MHZ);
    tlkmw_fs_drv_ds35x_delayMs(5);
    tlkmw_fs_drv_ds35x_reset();
    tlkmw_fs_drv_ds35x_delayMs(5);
    if (tlkmw_fs_drv_ds35x_check_id() == false) {
        return false;
    }
    if (tlkmw_fs_drv_ds35x_unlock_all_blocks() == false) {
        return false;
    }
    tlkmw_fs_drv_ds35x_set_qe();
    tlkmw_fs_drv_ds35x_check_bad_blocks();
    return true;
}

/* dhara adapt */
static struct dhara_nand ds35x_nand = {
    .log2_page_size = 11, /* 2048 => 1<<11 */
    .log2_ppb       = 6,  /* 64   => 1<<6  */
    .num_blocks     = 1024,
};

int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t b)
{
    if (b >= n->num_blocks) {
        return 1;
    }
    if (tlk_this->badMark[b / 8] & (1 << (b % 8))) {
        return 1;
    }
    return 0;
}

int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p)
{
    (void)n;
    ds35_trace("nand_is_free page[%d]", p);
    uint8_t *buf = tlk_this->cache;
    uint8_t  ret = tlkmw_fs_drv_ds35x_page_read(p, 0, buf);
    if (ret == false) {
        return 0;
    }
    uint32_t *ptr = (uint32_t *)buf;
    for (size_t i = 0; i < DS35X_PAGE_DATA_SIZE / sizeof(uint32_t); i++) {
        if (ptr[i] != 0XFFFFFFFF) {
            return 0;
        }
    }
    return 1;
}

static inline void dhara_my_set_error(dhara_error_t *err, dhara_error_t res)
{
    if (err == NULL) {
        return;
    }
    *err = res;
}

void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t b)
{
    (void)n;
    tlkmw_fs_drv_ds35x_mark_bad_block(b);
}

int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err)
{
    ds35_trace("nand_erase block[%d]", b);
    if (dhara_nand_is_bad(n, b)) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    if (!tlkmw_fs_drv_ds35x_block_erase(b)) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    return 0;
}

int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
    ds35_trace("nand_prog page[%d]", p);
    dhara_block_t b = (p >> (n->log2_ppb));
    if (dhara_nand_is_bad(n, b)) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    uint8_t *buf = tlk_this->cache;
    memcpy(buf, data, DS35X_PAGE_DATA_SIZE);
    memset(buf + DS35X_PAGE_DATA_SIZE, 0xFF, DS35X_PAGE_SPARE_SIZE);
    if (!tlkmw_fs_drv_ds35x_page_prog(p, (uint8_t *)buf)) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    return 0;
}

int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err)
{
    ds35_trace("nand_read page[%d]", p);
    uint8_t      *buf = tlk_this->cache;
    dhara_block_t b   = (p >> (n->log2_ppb));
    if (dhara_nand_is_bad(n, b)) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    if (!tlkmw_fs_drv_ds35x_page_read(p, offset, buf)) {
        dhara_my_set_error(err, DHARA_E_ECC);
        dhara_nand_mark_bad(n, b);
        return -1;
    }
    memcpy(data, buf, length);
    return 0;
}

int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err)
{
    (void)n;
    (void)err;
    ds35_trace("nand_copy src[%d] dst[%d]", src, dst);
    uint8_t *buf = tlk_this->cache;
    if (tlkmw_fs_drv_ds35x_page_read(src, 0, buf) == false) {
        dhara_my_set_error(err, DHARA_E_ECC);
        return -1;
    }
    if (tlkmw_fs_drv_ds35x_page_prog(dst, buf) == false) {
        dhara_my_set_error(err, DHARA_E_BAD_BLOCK);
        return -1;
    }
    return 0;
}

/*fs adapt */
static void tlkmw_fs_drv_ds35x_diskio_init(void)
{
    dhara_error_t err;
    if (false == tlkmw_fs_drv_ds35x_init()) {
        tlk_this->state = FS_DRV_DS35X_ERR;
        return;
    }
    dhara_map_init(&tlk_this->dhara, &ds35x_nand, tlk_this->dhara_mempool, 4);
    if (dhara_map_resume(&tlk_this->dhara, &err) == -1) {
        tlkapi_printf(DS35X_DEBUG_ERR_LOG_EN, "dhara resume err:need format, err=%d\n", err);
        dhara_map_clear(&tlk_this->dhara);
    }
    tlk_this->state = FS_DRV_DS35X_READY;
}

static void tlkmw_fs_drv_ds35x_diskio_sleep(void)
{
    dhara_map_sync(&tlk_this->dhara, NULL);
}

static void tlkmw_fs_drv_ds35x_diskio_awake(void) {}

static int tlkmw_fs_drv_ds35x_diskio_read(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    if (tlk_this->state != FS_DRV_DS35X_READY) {
        return 1;
    }
    while (cnt--) {
        if (dhara_map_read(&tlk_this->dhara, lba++, buff, NULL) != 0) {
            return 1;
        }
        buff += DS35X_PAGE_DATA_SIZE;
    }
    return 0;
}

static int tlkmw_fs_drv_ds35x_diskio_write(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    if (tlk_this->state != FS_DRV_DS35X_READY) {
        return 1;
    }
    while (cnt--) {
        if (dhara_map_write(&tlk_this->dhara, lba++, buff, NULL) != 0) {
            return 1;
        }
        buff += DS35X_PAGE_DATA_SIZE;
    }
    return 0;
}

static uint32_t tlkmw_fs_drv_ds35x_diskio_get_sector_size(void)
{
    return DS35X_PAGE_DATA_SIZE;
}

static uint32_t tlkmw_fs_drv_ds35x_diskio_get_sector_num(void)
{
    return dhara_map_capacity(&tlk_this->dhara);
}

static const tlkmw_fs_parm_cfg_t sTlkmwFsDiskIoDs35xParm = {
    .fmt         = 1,
    .clusterSize = 8 * 1024,
};

const tlkmw_fs_diskio_t gTlkmwFsDiskIoDs35x = {
    .init          = tlkmw_fs_drv_ds35x_diskio_init,
    .sleep         = tlkmw_fs_drv_ds35x_diskio_sleep,
    .awake         = tlkmw_fs_drv_ds35x_diskio_awake,
    .write         = tlkmw_fs_drv_ds35x_diskio_write,
    .read          = tlkmw_fs_drv_ds35x_diskio_read,
    .getSectorSize = tlkmw_fs_drv_ds35x_diskio_get_sector_size,
    .getSectorNum  = tlkmw_fs_drv_ds35x_diskio_get_sector_num,
    .fsParmCfg     = &sTlkmwFsDiskIoDs35xParm,
};

#endif
