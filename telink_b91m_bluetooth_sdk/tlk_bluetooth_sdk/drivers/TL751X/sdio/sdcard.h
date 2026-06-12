#pragma once

#include <stdbool.h>
#include "sdmmc.h"

typedef enum
{
    SDCARD_DATA_WIDTH_1BIT = 0,
    SDCARD_DATA_WIDTH_4BIT = 1,
} sdcard_data_width_e;

typedef struct
{
    unsigned int ocr;
    unsigned int rca;
    unsigned int status;
    unsigned char read_buf[16];
    unsigned char csd_structure;
    unsigned long long card_capacity;
    unsigned int card_block_size;
} sdcard_t;

typedef struct
{
    unsigned int sdmmc_rca_addr;
    sdmmc_data_width_e data_width;
    sdmmc_send_cmd_info_t **cmd_info;
    sdmmc_device_info *info;
} sdcard_cmd_config_t;

typedef struct
{
    unsigned char cmd_index;
    data_transfer_t *data_trans;
    sdmmc_send_cmd_info_t **cmd_info;
} sdcard_data_config_t;

void sdcard_cmd_info_table_init(sdmmc_send_cmd_info_t **cmd_info);
sdcard_t *sdcard_cmd_init(sdcard_cmd_config_t *config);
void sdcard_set_data_width(sdcard_data_width_e width);
void sdcard_set_block_num(unsigned short blk_num);
void sdcard_write_block_buf(sdcard_data_config_t *config, unsigned int *src, unsigned int *dest, unsigned int block_num);
void sdcard_read_block_buf(sdcard_data_config_t *config, unsigned int *dest, unsigned int *src, unsigned int block_num);
