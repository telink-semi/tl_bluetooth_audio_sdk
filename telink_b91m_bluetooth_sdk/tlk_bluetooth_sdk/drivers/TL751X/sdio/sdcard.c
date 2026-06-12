#include <string.h>
#include "sdcard.h"
#include "mmc.h"
#include "sdmmc.h"

sdmmc_send_cmd_info_t sdcard_cmd0_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_NO,
    .data_present_select_en = false,
    .cmd_index              = HOST_GO_IDLE_STATE,
};

sdmmc_send_cmd_info_t sdcard_cmd1_info = {
    .argument.arg           = 0xC0ff8080,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_OP_COND,
};

sdmmc_send_cmd_info_t sdcard_cmd2_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index              = HOST_ALL_SEND_CID,
};

sdmmc_send_cmd_info_t sdcard_cmd3_info = {
    .argument.rca_addr      = 1 << 0x10,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_RELATIVE_ADDR,
};

sdmmc_send_cmd_info_t sdcard_cmd6_info = {
    .argument.rca_addr      = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SWITCH_FUNCTION,
};

sdmmc_send_cmd_info_t sdcard_cmd7_info = {
    .argument.rca_addr      = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SELECT_DESELECT_CARD,
};

sdmmc_send_cmd_info_t sdcard_cmd9_info = {
    .argument.rca_addr      = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_CSD,
};

sdmmc_send_cmd_info_t sdcard_cmd10_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_CID,
};

sdmmc_send_cmd_info_t sdcard_cmd12_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT_AND_CHECK_BUSY,
    .data_present_select_en = false,
    .cmd_index              = HOST_STOP_TRANSMISSION,
};

sdmmc_send_cmd_info_t sdcard_cmd13_info = {
    .argument.rca_addr      = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_STATUS,
};

sdmmc_send_cmd_info_t sdcard_cmd17_info = {
    .argument.block_addr    = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index              = HOST_READ_SINGLE_BLOCK,
};

sdmmc_send_cmd_info_t sdcard_cmd18_info = {
    .argument.block_addr    = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index              = HOST_READ_MULTIPLE_BLOCK,
};

sdmmc_send_cmd_info_t sdcard_cmd23_info = {
    .argument.block_len     = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SET_BLOCK_COUNT,
};

sdmmc_send_cmd_info_t sdcard_cmd24_info = {
    .argument.block_addr    = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index              = HOST_WRITE_BLOCK,
};

sdmmc_send_cmd_info_t sdcard_cmd25_info = {
    .argument.block_addr    = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = true,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index              = HOST_WRITE_MULTIPLE_BLOCK,
};

sdmmc_send_cmd_info_t sdcard_cmd8_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_SEND_EXT_CSD,
};

sdmmc_send_cmd_info_t sdcard_cmd32_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_ERASE_WR_BLK_START,
};

sdmmc_send_cmd_info_t sdcard_cmd33_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_ERASE_WR_BLK_END,
};

sdmmc_send_cmd_info_t sdcard_cmd38_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_ERASE,
};

sdmmc_send_cmd_info_t sdcard_cmd41_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = false,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_ACMD_SEND_OP_COND,
};

sdmmc_send_cmd_info_t sdcard_cmd55_info = {
    .argument.arg           = 0x00,
    .cmd_type               = NORMAL,
    .crc_check_en           = true,
    .sub_cmd_flag           = false,
    .cmd_index_check_en     = false,
    .sdmmc_resport          = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index              = HOST_APP_CMD,
};

static sdmmc_send_cmd_info_t *sdcard_cmd_array[64] = {
    &sdcard_cmd0_info,  &sdcard_cmd1_info,  &sdcard_cmd2_info,  &sdcard_cmd3_info,  &sdcard_cmd6_info,  &sdcard_cmd7_info,
    &sdcard_cmd9_info,  &sdcard_cmd10_info, &sdcard_cmd12_info, &sdcard_cmd13_info, &sdcard_cmd17_info, &sdcard_cmd18_info,
    &sdcard_cmd23_info, &sdcard_cmd24_info, &sdcard_cmd25_info, &sdcard_cmd8_info,  &sdcard_cmd55_info, &sdcard_cmd41_info,
};

static sdcard_t g_sdcard_info;

void sdcard_cmd_info_table_init(sdmmc_send_cmd_info_t **cmd_info)
{
    unsigned char i, index;
    if (cmd_info == NULL)
    {
        return;
    }
    memset(cmd_info, 0x00, sizeof(sdmmc_send_cmd_info_t **) * 64);

    for (i = 0x00; i < 64; i++)
    {
        if(sdcard_cmd_array[i] == NULL) {
            continue;
        }
        index           = sdcard_cmd_array[i]->cmd_index;
        cmd_info[index] = sdcard_cmd_array[i];
    }
}

void sdcard_wait_card_ready(void)
{
    do
    {
        sdcard_cmd55_info.argument.arg = 0x0;
        sdmmc_cmd_transfer(&sdcard_cmd55_info);
        sdcard_cmd41_info.argument.arg = 0x40300000;
        sdmmc_cmd_transfer(&sdcard_cmd41_info);
    } while (0 == (reg_sdmmc_response_0 & 0x80000000));
    g_sdcard_info.ocr = reg_sdmmc_response_0;
}

unsigned int sdcard_get_card_status(void)
{
    sdcard_cmd13_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd13_info);

    return reg_sdmmc_response_0;
}

void sdcard_set_block_num(unsigned short blk_num)
{
    sdcard_cmd23_info.argument.arg = blk_num;
    sdmmc_cmd_transfer(&sdcard_cmd23_info);
}

void sdcard_set_data_width(sdcard_data_width_e width)
{
    sdcard_cmd55_info.argument.arg = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd55_info);

    if (SDCARD_DATA_WIDTH_4BIT == width)
    {
        sdcard_cmd6_info.argument.arg = width + 1;
    }
    else
    {
        sdcard_cmd6_info.argument.arg = width;
    }
    sdmmc_cmd_transfer(&sdcard_cmd6_info);

    sdmmc_set_data_width((sdmmc_data_width_e)width);
}

void sdcard_get_csd(void)
{
    sdcard_cmd9_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd9_info);
    unsigned int csd_res[4];

    csd_res[0] = sdmmc_get_response1_data();
    csd_res[1] = sdmmc_get_response2_data();
    csd_res[2] = sdmmc_get_response3_data();
    csd_res[3] = sdmmc_get_response4_data();
    memcpy(&g_sdcard_info.read_buf[1], csd_res, 15);
}

void sdcard_decode_csd(void)
{
    unsigned int c_size;

    /* CSD_STRUCTURE : CSD[127:126] */
    g_sdcard_info.csd_structure = (g_sdcard_info.read_buf[15] >> 6) & 0x3;

    /* CSD v2.0 (SDHC / SDXC) */
    if (g_sdcard_info.csd_structure == 1)
    {
        /* C_SIZE : CSD[69:48] */
        c_size = ((unsigned int)(g_sdcard_info.read_buf[8] & 0x3F)) << 16;
        c_size |= ((unsigned int)g_sdcard_info.read_buf[7]) << 8;
        c_size |= ((unsigned int)g_sdcard_info.read_buf[6]);

        g_sdcard_info.card_capacity = (unsigned long long)(c_size + 1) * 512 * 1024;
        g_sdcard_info.card_block_size = 512;
    }
}

sdcard_t *sdcard_cmd_init(sdcard_cmd_config_t *config)
{
    if (config == NULL)
    {
        return NULL;
    }

    memset(&g_sdcard_info, 0, sizeof(g_sdcard_info));

    sdmmc_cmd_transfer(config->cmd_info[0]);

    sdcard_cmd8_info.argument.arg = 1 << 8 | 0xAA;
    sdmmc_cmd_transfer(config->cmd_info[8]);

    if ((reg_sdmmc_response_0 & 0xff) != 0xAA)
    {
        while (1)
            ;
    }

    sdcard_wait_card_ready();

    sdmmc_cmd_transfer(config->cmd_info[2]);

    sdcard_cmd3_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd3_info);
    g_sdcard_info.rca = reg_sdmmc_response_0 & 0xffff0000;

    sdcard_get_csd();
    sdcard_decode_csd();

    sdcard_cmd10_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd10_info);

    sdcard_cmd13_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd13_info);
    g_sdcard_info.status = reg_sdmmc_response_0;

    sdcard_cmd7_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd7_info);

    sdcard_cmd13_info.argument.rca_addr = g_sdcard_info.rca;
    sdmmc_cmd_transfer(&sdcard_cmd13_info);
    g_sdcard_info.status = reg_sdmmc_response_0;

    return &g_sdcard_info;
}

void sdcard_write_block_buf(sdcard_data_config_t *config, unsigned int *src, unsigned int *dest, unsigned int block_num)
{
    if ((config == NULL) || (src == NULL))
    {
        return;
    }
    config->data_trans->block_num                     = block_num;
    config->data_trans->buf_addr.src                  = (unsigned char *)src;
    config->data_trans->cmd_info                      = config->cmd_info[config->cmd_index];
    config->data_trans->cmd_info->argument.block_addr = (unsigned int)dest;
    sdmmc_cmd_data_transfer(config->data_trans);
}

void sdcard_read_block_buf(sdcard_data_config_t *config, unsigned int *dest, unsigned int *src, unsigned int block_num)
{
    if ((config == NULL) || (dest == NULL))
    {
        return;
    }
    config->data_trans->block_num                     = block_num;
    config->data_trans->buf_addr.dest                 = (unsigned char *)dest;
    config->data_trans->cmd_info                      = config->cmd_info[config->cmd_index];
    config->data_trans->cmd_info->argument.block_addr = (unsigned int)src;
    sdmmc_cmd_data_transfer(config->data_trans);
}
