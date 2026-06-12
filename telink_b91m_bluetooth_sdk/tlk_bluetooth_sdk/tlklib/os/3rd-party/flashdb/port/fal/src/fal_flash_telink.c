#include <string.h>
#include "../inc/fal.h"
#include "tl_common.h"
#include "drivers.h"

#define FDB_TELINK_FLASH_ADDR_OFFSET CONFIG_TLK_FLASH_KVDB_ADDR
#define FDB_TELINK_FLASH_USE_XIP_READ 0  //xip will cause cache bug
#define FDB_TELINK_FLASH_SIZE  CONFIG_TLK_FLASH_KVDB_SIZE
#define FDB_TELINK_FLASH_DEBUG 0

#ifndef FLASH_R_BASE_ADDR
#define FLASH_R_BASE_ADDR (0x20000000)
#endif

#if FDB_TELINK_FLASH_DEBUG
#define fdb_tlk_printf(fmt, ...)  {tlkos_task_delayMs(2);tlk_printf(fmt, ##__VA_ARGS__);}
#else
#define fdb_tlk_printf(fmt, ...)
#endif
#define FDB_TELINK_FLASH_RAM_SIMU 0

#if FDB_TELINK_FLASH_RAM_SIMU
static uint8_t s_flash[32 * 1024];
#endif

static int fal_telink_flash_init(void)
{
#if FDB_TELINK_FLASH_RAM_SIMU
    memset(s_flash,0xff,sizeof(s_flash));
#endif
    return 1;
}

static int fal_telink_flash_read(long offset, uint8_t *buf, size_t size)
{
#if FDB_TELINK_FLASH_RAM_SIMU
    memcpy(buf,s_flash + offset,size);
#else
    uint32_t addr = tlkhal_flash_get_size() + FDB_TELINK_FLASH_ADDR_OFFSET - 0x100000 + offset;
    fdb_tlk_printf("[fdb] read %x %d",offset,size);
#if FDB_TELINK_FLASH_USE_XIP_READ
    memcpy(buf,(void*)(addr | FLASH_R_BASE_ADDR),size);
#else
    flash_read_page(addr, size, buf);
#endif
#endif    
    return 0;
}


static int fal_telink_flash_write(long offset, const uint8_t *buf, size_t size)
{
    (void) offset;
    (void) buf;
    (void) size;
#if FDB_TELINK_FLASH_RAM_SIMU
    memcpy(s_flash + offset,buf,size);
#else
    uint32_t addr = tlkhal_flash_get_size() + FDB_TELINK_FLASH_ADDR_OFFSET - 0x100000;
    fdb_tlk_printf("[fdb] write %x %d",offset,size);
    flash_write_page(addr + offset, size, (uint8_t *)buf);
#endif
    return 0;
}


static int fal_telink_flash_erase(long offset, size_t size)
{
    (void) offset;
    (void) size;
#if FDB_TELINK_FLASH_RAM_SIMU
    memset(s_flash+offset,0xff,size);
#else
    uint32_t addr = tlkhal_flash_get_size() + FDB_TELINK_FLASH_ADDR_OFFSET - 0x100000;
    uint32_t sec_num = (size + (4095)) / 4096;
    fdb_tlk_printf("[fdb] erase %x %d",offset,size);
    for(uint32_t i = 0; i < sec_num ;i++) {
        flash_erase_sector(addr + offset + i * 0x1000);
    }
#endif
    return 0;
}

const struct fal_flash_dev telnk_fal =
{
    .name       = "telink",
    .addr       = 0,
    .len        = FDB_TELINK_FLASH_SIZE,
    .blk_size   = 4096,
    .ops        = {
        .init = fal_telink_flash_init,
        .read = fal_telink_flash_read,
        .write = fal_telink_flash_write,
        .erase = fal_telink_flash_erase,
    },
    .write_gran = 1,
};



