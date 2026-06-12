#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_
#include <stdint.h>
#include "vendor/common/app_flash.h"
#define NOR_FLASH_DEV_NAME             "telink"
#define FAL_PART_HAS_TABLE_CFG
/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev telnk_fal;
//extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &telnk_fal,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD, "disk0", NOR_FLASH_DEV_NAME, 0, CONFIG_TLK_FLASH_KVDB_SIZE, 0},  \
}
#endif /* FAL_PART_HAS_TABLE_CFG */


void *tlkos_malloc(uint32_t size);
void tlkos_free(void *ptr);

#define FAL_MALLOC                     tlkos_malloc
#define FAL_FREE                       tlkos_free
#define FAL_PRINTF(...)

#endif /* _FAL_CFG_H_ */
