/********************************************************************************************************
 * @file    mmc.c
 *
 * @brief   This is the source file for tl751x
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include <string.h>
#include "mmc.h"
#include "sdmmc.h"

sdmmc_send_cmd_info_t mmc_cmd0_info = {
    .argument.arg = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = false,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_NO,
    .data_present_select_en = false,
    .cmd_index = HOST_GO_IDLE_STATE,
};

sdmmc_send_cmd_info_t mmc_cmd1_info = {
    .argument.arg = 0xC0ff8080,
    .cmd_type = NORMAL,
    .crc_check_en = false,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SEND_OP_COND,
};

sdmmc_send_cmd_info_t mmc_cmd2_info = {
    .argument.arg = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index = HOST_ALL_SEND_CID,
};

sdmmc_send_cmd_info_t mmc_cmd3_info = {
    .argument.rca_addr = 1 << 0x10,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SEND_RELATIVE_ADDR,
};

sdmmc_send_cmd_info_t mmc_cmd5_info = {
    .argument.rca_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SLEEP_AWAKE,
};

sdmmc_send_cmd_info_t mmc_cmd6_info = {
    .argument.mmc_cmd6.cmd_set = 0x00,
    .argument.mmc_cmd6.reserve1 = 0x00,
    .argument.mmc_cmd6.reg_val = 0x00,
    .argument.mmc_cmd6.reg_addr = 0x00,
    .argument.mmc_cmd6.visits = 0x00,
    .argument.mmc_cmd6.reserve2 = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SWITCH_FUNCTION,
};

sdmmc_send_cmd_info_t mmc_cmd7_info = {
    .argument.rca_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SELECT_DESELECT_CARD,
};

sdmmc_send_cmd_info_t mmc_cmd9_info = {
    .argument.rca_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index = HOST_SEND_CSD,
};

sdmmc_send_cmd_info_t mmc_cmd10_info = {
    .argument.arg = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_LONG,
    .data_present_select_en = false,
    .cmd_index = HOST_SEND_CID,
};

sdmmc_send_cmd_info_t mmc_cmd12_info = {
    .argument.arg = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT_AND_CHECK_BUSY,
    .data_present_select_en = false,
    .cmd_index = HOST_STOP_TRANSMISSION,
};

sdmmc_send_cmd_info_t mmc_cmd13_info = {
    .argument.rca_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SEND_STATUS,
};

sdmmc_send_cmd_info_t mmc_cmd17_info = {
    .argument.block_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index = HOST_READ_SINGLE_BLOCK,
};

sdmmc_send_cmd_info_t mmc_cmd18_info = {
    .argument.block_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index = HOST_READ_MULTIPLE_BLOCK,
};

sdmmc_send_cmd_info_t mmc_cmd23_info = {
    .argument.block_len = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = false,
    .cmd_index = HOST_SET_BLOCK_COUNT,
};

sdmmc_send_cmd_info_t mmc_cmd24_info = {
    .argument.block_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index = HOST_WRITE_BLOCK,
};

sdmmc_send_cmd_info_t mmc_cmd25_info = {
    .argument.block_addr = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = true,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index = HOST_WRITE_MULTIPLE_BLOCK,
};

sdmmc_send_cmd_info_t mmc_cmd8_info = {
    .argument.arg = 0x00,
    .cmd_type = NORMAL,
    .crc_check_en = true,
    .sub_cmd_flag = false,
    .cmd_index_check_en = false,
    .sdmmc_resport = RESPONSE_SHORT,
    .data_present_select_en = true,
    .cmd_index = HOST_SEND_EXT_CSD,
};

static sdmmc_send_cmd_info_t *mmc_cmd_array[CMD_NUM] = {
    &mmc_cmd0_info,
    &mmc_cmd1_info,
    &mmc_cmd2_info,
    &mmc_cmd3_info,
    &mmc_cmd5_info,
    &mmc_cmd6_info,
    &mmc_cmd7_info,
    &mmc_cmd9_info,
    &mmc_cmd10_info,
    &mmc_cmd12_info,
    &mmc_cmd13_info,
    &mmc_cmd17_info,
    &mmc_cmd18_info,
    &mmc_cmd23_info,
    &mmc_cmd24_info,
    &mmc_cmd25_info,
    &mmc_cmd8_info,
};

void mmc_cmd_info_table_init(sdmmc_send_cmd_info_t **cmd_info)
{
    unsigned char i, index;
    if (cmd_info == NULL)
    {
        return;
    }
    memset(cmd_info, 0x00, sizeof(sdmmc_send_cmd_info_t **) * CMD_NUM);

    for (i = 0x00; i < CMD_NUM; i++)
    {
        if(mmc_cmd_array[i] == NULL) {
            continue;
        }
        index = mmc_cmd_array[i]->cmd_index;
        cmd_info[index] = mmc_cmd_array[i];
    }
}

void mmc_get_cid_reg_info(mmc_cmd_config_t *config, unsigned char *buf)
{
    if ((config == NULL) || (buf == NULL))
    {
        return;
    }
    if(config->info == NULL) {
        return;
    }
    config->info->sd_cid.mid = buf[15];
    config->info->sd_cid.cbx = buf[14] & 0x03;
    config->info->sd_cid.oid = buf[13];
    config->info->sd_cid.pnm[5] = buf[12];
    config->info->sd_cid.pnm[4] = buf[11];
    config->info->sd_cid.pnm[3] = buf[10];
    config->info->sd_cid.pnm[2] = buf[9];
    config->info->sd_cid.pnm[1] = buf[8];
    config->info->sd_cid.pnm[0] = buf[7];
    config->info->sd_cid.prv = buf[6];
    config->info->sd_cid.psn[3] = buf[5];
    config->info->sd_cid.psn[2] = buf[4];
    config->info->sd_cid.psn[1] = buf[3];
    config->info->sd_cid.psn[0] = buf[2];
    config->info->sd_cid.mdt = buf[1];
    config->info->sd_cid.crc = buf[0] >> 0x01;
}

void mmc_get_csd_reg_info(mmc_cmd_config_t *config, unsigned char *buf)
{
    unsigned tmp;
    if ((config == NULL) || (buf == NULL))
    {
        return;
    }
    if(config->info == NULL) {
        return;
    }
    config->info->sd_csd.csdstruct = (buf[15] >> 0x06) & 0x03;
    config->info->sd_csd.sysspecversion = (buf[15] >> 0x02) & 0x0f;
    config->info->sd_csd.taac = buf[14];
    config->info->sd_csd.nsac = buf[13];
    config->info->sd_csd.maxbusclkfrec = buf[12];
    config->info->sd_csd.cardcomdclasses = (((unsigned short)(buf[11] << 0x08) || buf[10]) >> 0x04);
    config->info->sd_csd.rdblocklen = buf[10] & 0x0f;
    config->info->sd_csd.partblockread = (buf[9] >> 0x07) & 0x01;
    config->info->sd_csd.wrblockmisalign = (buf[9] >> 0x06) & 0x01;
    config->info->sd_csd.rdblockmisalign = (buf[9] >> 0x05) & 0x01;
    config->info->sd_csd.dsrimpl = (buf[9] >> 0x04) & 0x01;
    config->info->sd_csd.devicesize = (((unsigned short)(buf[9] << 0x08) || buf[8]) & 0xfff);
    config->info->sd_csd.maxrdcurrentvddmin = (buf[7] >> 0x05) & 0x7;
    config->info->sd_csd.maxrdcurrentvddmax = (buf[7] >> 0x02) & 0x7;

    tmp = 0x00;
    tmp = buf[7] & 0x03;
    tmp <<= 0x01;
    tmp |= buf[6] >> 0x07;
    config->info->sd_csd.maxwrcurrentvddmin = tmp;
    config->info->sd_csd.maxwrcurrentvddmax = (buf[6] >> 0x04) & 0x07;
    config->info->sd_csd.devicesizemul = (buf[6] >> 0x03) & 0x07;

    tmp = 0x00;
    tmp = buf[6] & 0x01;
    tmp <<= 0x04;
    tmp |= (buf[5] >> 0x04) & 0x0f;
    config->info->sd_csd.erasegrsize = tmp;

    tmp = 0x00;
    tmp = buf[5] & 0x0f;
    tmp <<= 0x01;
    tmp |= (buf[4] >> 0x07) & 0x01;
    config->info->sd_csd.erasegrmul = tmp;
    config->info->sd_csd.wrprotectgrsize = (buf[4] >> 0x02) & 0x1f;
    config->info->sd_csd.wrprotectgrenable = (buf[4] >> 0x01) & 0x1;

    tmp = 0x00;
    tmp = buf[4] & 0x01;
    tmp <<= 0x01;
    tmp |= (buf[3] >> 0x07) & 0x01;
    config->info->sd_csd.mandeflecc = tmp;
    config->info->sd_csd.wrspeedfact = (buf[3] >> 0x04) & 0x07;
    config->info->sd_csd.maxwrblocklen = buf[3] & 0x0f;
    config->info->sd_csd.writeblockpapartial = (buf[2] >> 0x07) & 0x01;
    config->info->sd_csd.contentprotectapp = buf[2] & 0x01;

    config->info->sd_csd.fileformatgrouop = (buf[1] >> 0x07) & 0x01;
    config->info->sd_csd.copyflag = (buf[1] >> 0x06) & 0x01;
    config->info->sd_csd.permwrprotect = (buf[1] >> 0x05) & 0x01;
    config->info->sd_csd.tempwrprotect = (buf[1] >> 0x04) & 0x01;
    config->info->sd_csd.fileformat = (buf[1] >> 0x02) & 0x03;
    config->info->sd_csd.ecc = buf[1] & 0x03;
    config->info->sd_csd.csd_crc = (buf[0] >> 0x01) & 0x0ef;
};

void mmc_out_reg_info(sdmmc_device_info *info)
{
    if (info == NULL)
    {
        return;
    }
#if 0
    printf("info->sd_cid.mid = 0x%x\r\n", info->sd_cid.mid);
    printf("info->sd_cid.cbx = 0x%x\r\n", info->sd_cid.cbx);
    printf("info->sd_cid.oid = 0x%x\r\n", info->sd_cid.cbx);
    printf("info->sd_cid.pnm[5] = 0x%x\r\n", info->sd_cid.pnm[5]);
    printf("info->sd_cid.pnm[4] = 0x%x\r\n", info->sd_cid.pnm[4]);
    printf("info->sd_cid.pnm[3] = 0x%x\r\n", info->sd_cid.pnm[3]);
    printf("info->sd_cid.pnm[2] = 0x%x\r\n", info->sd_cid.pnm[2]);
    printf("info->sd_cid.pnm[1] = 0x%x\r\n", info->sd_cid.pnm[1]);
    printf("info->sd_cid.pnm[0] = 0x%x\r\n", info->sd_cid.pnm[0]);
    printf("info->sd_cid.prv = 0x%x\r\n", info->sd_cid.prv);
    printf("info->sd_cid.psn[3] = 0x%x\r\n", info->sd_cid.psn[3]);
    printf("info->sd_cid.psn[2] = 0x%x\r\n", info->sd_cid.psn[2]);
    printf("info->sd_cid.psn[1] = 0x%x\r\n", info->sd_cid.psn[1]);
    printf("info->sd_cid.psn[0] = 0x%x\r\n", info->sd_cid.psn[0]);
    printf("info->sd_cid.mdt = 0x%x\r\n", info->sd_cid.mdt);
    printf("info->sd_cid.crc = 0x%x\r\n", info->sd_cid.crc);

    printf("info->sd_csd.csdstruct = 0x%x\r\n", info->sd_csd.csdstruct);
    printf("info->sd_csd.sysspecversion = 0x%x\r\n", info->sd_csd.sysspecversion);
    printf("info->sd_csd.taac = 0x%x\r\n", info->sd_csd.taac);
    printf("info->sd_csd.nsac = 0x%x\r\n", info->sd_csd.nsac);
    printf("info->sd_csd.maxbusclkfrec = 0x%x\r\n", info->sd_csd.maxbusclkfrec);
    printf("info->sd_csd.cardcomdclasses = 0x%x\r\n", info->sd_csd.cardcomdclasses);
    printf("info->sd_csd.rdblocklen = 0x%x\r\n", info->sd_csd.rdblocklen);
    printf("info->sd_csd.partblockread = 0x%x\r\n", info->sd_csd.partblockread);
    printf("info->sd_csd.wrblockmisalign = 0x%x\r\n", info->sd_csd.wrblockmisalign);
    printf("info->sd_csd.rdblockmisalign = 0x%x\r\n", info->sd_csd.rdblockmisalign);
    printf("info->sd_csd.dsrimpl = 0x%x\r\n", info->sd_csd.dsrimpl);
    printf("info->sd_csd.devicesize = 0x%x\r\n", info->sd_csd.devicesize);
    printf("info->sd_csd.maxrdcurrentvddmin = 0x%x\r\n", info->sd_csd.maxrdcurrentvddmin);
    printf("info->sd_csd.maxrdcurrentvddmax = 0x%x\r\n", info->sd_csd.maxrdcurrentvddmax);
    printf("info->sd_csd.maxwrcurrentvddmin = 0x%x\r\n", info->sd_csd.maxwrcurrentvddmin);
    printf("info->sd_csd.maxwrcurrentvddmax = 0x%x\r\n", info->sd_csd.maxwrcurrentvddmax);
    printf("info->sd_csd.devicesizemul = 0x%x\r\n", info->sd_csd.devicesizemul);
    printf("info->sd_csd.erasegrsize = 0x%x\r\n", info->sd_csd.erasegrsize);
    printf("info->sd_csd.erasegrmul = 0x%x\r\n", info->sd_csd.erasegrmul);
    printf("info->sd_csd.wrprotectgrsize = 0x%x\r\n", info->sd_csd.wrprotectgrsize);
    printf("info->sd_csd.wrprotectgrenable = 0x%x\r\n", info->sd_csd.wrprotectgrenable);
    printf("info->sd_csd.mandeflecc = 0x%x\r\n", info->sd_csd.mandeflecc);
    printf("info->sd_csd.wrspeedfact = 0x%x\r\n", info->sd_csd.wrspeedfact);
    printf("info->sd_csd.maxwrblocklen = 0x%x\r\n", info->sd_csd.maxwrblocklen);
    printf("info->sd_csd.writeblockpapartial = 0x%x\r\n", info->sd_csd.writeblockpapartial);
    printf("info->sd_csd.contentprotectapp = 0x%x\r\n", info->sd_csd.contentprotectapp);
    printf("info->sd_csd.fileformatgrouop = 0x%x\r\n", info->sd_csd.fileformatgrouop);
    printf("info->sd_csd.copyflag = 0x%x\r\n", info->sd_csd.copyflag);
    printf("info->sd_csd.permwrprotect = 0x%x\r\n", info->sd_csd.permwrprotect);
    printf("info->sd_csd.tempwrprotect = 0x%x\r\n", info->sd_csd.tempwrprotect);
    printf("info->sd_csd.fileformat = 0x%x\r\n", info->sd_csd.fileformat);
    printf("info->sd_csd.ecc = 0x%x\r\n", info->sd_csd.ecc);
    printf("info->sd_csd.csd_crc = 0x%x\r\n", info->sd_csd.csd_crc);
#endif
}

void mmc_out_ecsd_reg_info(unsigned int* ecsd_buf)
{
    if (ecsd_buf == NULL)
    {
        return;
    }
#if 0
    printf("emmc usable capacity = %f GB\r\n", (float)ecsd_buf[53]/1000/1000/1000*512);
#endif
}

void mmc_cmd_init(mmc_cmd_config_t *config)
{
    unsigned char i;
    unsigned int reg_buf[0x04];

    if (config == NULL)
    {
        return;
    }

    config->cmd_info[HOST_SEND_CSD]->argument.rca_addr = config->sdmmc_rca_addr;
    config->cmd_info[HOST_SEND_CID]->argument.rca_addr = config->sdmmc_rca_addr;
    config->cmd_info[HOST_SEND_RELATIVE_ADDR]->argument.rca_addr = config->sdmmc_rca_addr;
    config->cmd_info[HOST_SELECT_DESELECT_CARD]->argument.rca_addr = config->sdmmc_rca_addr;

    for (i = 0x00; i < 0x04; i++)
    {
        sdmmc_cmd_transfer(config->cmd_info[i]);
    }

    sdmmc_cmd_transfer(config->cmd_info[HOST_SEND_CSD]);
    reg_buf[0x00] = sdmmc_get_response1_data();
    reg_buf[0x01] = sdmmc_get_response2_data();
    reg_buf[0x02] = sdmmc_get_response3_data();
    reg_buf[0x03] = sdmmc_get_response4_data();
    mmc_get_cid_reg_info(config,(unsigned char *)reg_buf);
    mmc_get_csd_reg_info(config, (unsigned char *)reg_buf);
    sdmmc_cmd_transfer(config->cmd_info[HOST_SEND_CID]);
    reg_buf[0x00] = sdmmc_get_response1_data();
    reg_buf[0x01] = sdmmc_get_response2_data();
    reg_buf[0x02] = sdmmc_get_response3_data();
    reg_buf[0x03] = sdmmc_get_response4_data();
    mmc_get_cid_reg_info(config, (unsigned char *)reg_buf);
    sdmmc_cmd_transfer(config->cmd_info[HOST_SELECT_DESELECT_CARD]);

    mmc_out_reg_info(config->info);
}

void mmc_set_speed(mmc_cmd_config_t *config, mmc_speed_e spe_mode)
{
    if (config == NULL)
    {
        return;
    }
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.cmd_set = EXT_CSD_CMD_SET_NORMAL;
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.visits = 0x03;
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_addr = EXT_CSD_HS_TIMING;

    switch (spe_mode)
    {
    case NORMAL_SPEED:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_TIMING_BC;
        break;
    case HIGH_SPEED:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_TIMING_HS;
        break;
    case HIGH_SPEED_200:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_TIMING_HS200;
        break;
    case HIGH_SPEED_400:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_TIMING_HS400;
        break;
    default:
        break;
    }

    sdmmc_cmd_transfer(config->cmd_info[HOST_SWITCH_FUNCTION]);
}

void mmc_set_data_width(mmc_cmd_config_t *config, sdmmc_data_width_e width)
{
    if (config == NULL)
    {
        return;
    }
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.cmd_set = EXT_CSD_CMD_SET_NORMAL;
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.visits = 0x03;
    config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_addr = EXT_CSD_BUS_WIDTH;

    switch (width)
    {
    case DATA_WIDTH_1BIT:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_BUS_WIDTH_1;
        break;
    case DATA_WIDTH_4BIT:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_BUS_WIDTH_4;
        break;
    case DATA_WIDTH_8BIT:
        config->cmd_info[HOST_SWITCH_FUNCTION]->argument.mmc_cmd6.reg_val = EXT_CSD_BUS_WIDTH_8;
        break;
    default:
        break;
    }
    sdmmc_set_data_width(width);
    sdmmc_cmd_transfer(config->cmd_info[HOST_SWITCH_FUNCTION]);
}

void mmc_get_all_ecsd(mmc_data_config_t *config,unsigned int* ecsd_buf)
{
    if ((config == NULL)||(ecsd_buf == NULL))
    {
        return;
    }

    config->data_trans->block_num = 1;
    config->data_trans->buf_addr.dest = (unsigned char *)ecsd_buf;
    config->data_trans->cmd_info = config->cmd_info[config->cmd_index];
    config->data_trans->cmd_info->argument.block_addr = (unsigned int)0x00;
    sdmmc_cmd_data_transfer(config->data_trans);
    mmc_out_ecsd_reg_info(ecsd_buf);
}

void mmc_set_block_num(mmc_cmd_config_t *config, unsigned int num)
{
    if (config == NULL)
    {
        return;
    }
    config->cmd_info[HOST_SET_BLOCK_COUNT]->argument.arg = num;
    sdmmc_cmd_transfer(config->cmd_info[HOST_SET_BLOCK_COUNT]);
}

void mmc_write_block_buf(mmc_data_config_t *config, unsigned int *src, unsigned int *dest, unsigned int block_num)
{
    if ((config == NULL) || (src == NULL))
    {
        return;
    }
    config->data_trans->block_num = block_num;
    config->data_trans->buf_addr.src = (unsigned char *)src;
    config->data_trans->cmd_info = config->cmd_info[config->cmd_index];
    config->data_trans->cmd_info->argument.block_addr = (unsigned int)dest;
    sdmmc_cmd_data_transfer(config->data_trans);
}

void mmc_read_block_buf(mmc_data_config_t *config, unsigned int *dest, unsigned int *src, unsigned int block_num)
{
    if ((config == NULL) || (dest == NULL))
    {
        return;
    }
    config->data_trans->block_num = block_num;
    config->data_trans->buf_addr.dest = (unsigned char *)dest;
    config->data_trans->cmd_info = config->cmd_info[config->cmd_index];
    config->data_trans->cmd_info->argument.block_addr = (unsigned int)src;
    sdmmc_cmd_data_transfer(config->data_trans);
}
