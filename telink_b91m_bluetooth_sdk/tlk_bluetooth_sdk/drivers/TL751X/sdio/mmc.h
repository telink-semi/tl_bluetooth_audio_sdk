
/********************************************************************************************************
 * @file    mmc.h
 *
 * @brief   This is the header file for tl751x
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
#ifndef MMC_H
#define MMC_H

#include "sdmmc.h"

#define CMD_NUM 64
#define EXT_CSD_NUM 512
/*
 * EXT_CSD fields
 */
#define EXT_CSD_CMDQ_MODE_EN 15                 /* R/W */
#define EXT_CSD_FLUSH_CACHE 32                  /* W */
#define EXT_CSD_CACHE_CTRL 33                   /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION 34       /* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX 35         /* RO */
#define EXT_CSD_PACKED_CMD_STATUS 36            /* RO */
#define EXT_CSD_EXP_EVENTS_STATUS 54            /* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL 56              /* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE 61             /* R */
#define EXT_CSD_GP_SIZE_MULT 143                /* R/W */
#define EXT_CSD_PARTITION_SETTING_COMPLETED 155 /* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE 156         /* R/W */
#define EXT_CSD_PARTITION_SUPPORT 160           /* RO */
#define EXT_CSD_HPI_MGMT 161                    /* R/W */
#define EXT_CSD_RST_N_FUNCTION 162              /* R/W */
#define EXT_CSD_BKOPS_EN 163                    /* R/W */
#define EXT_CSD_BKOPS_START 164                 /* W */
#define EXT_CSD_SANITIZE_START 165              /* W */
#define EXT_CSD_WR_REL_PARAM 166                /* RO */
#define EXT_CSD_RPMB_MULT 168                   /* RO */
#define EXT_CSD_FW_CONFIG 169                   /* R/W */
#define EXT_CSD_BOOT_WP 173                     /* R/W */
#define EXT_CSD_ERASE_GROUP_DEF 175             /* R/W */
#define EXT_CSD_PART_CONFIG 179                 /* R/W */
#define EXT_CSD_ERASED_MEM_CONT 181             /* RO */
#define EXT_CSD_BUS_WIDTH 183                   /* R/W */
#define EXT_CSD_STROBE_SUPPORT 184              /* RO */
#define EXT_CSD_HS_TIMING 185                   /* R/W */
#define EXT_CSD_POWER_CLASS 187                 /* R/W */
#define EXT_CSD_REV 192                         /* RO */
#define EXT_CSD_STRUCTURE 194                   /* RO */
#define EXT_CSD_CARD_TYPE 196                   /* RO */
#define EXT_CSD_DRIVER_STRENGTH 197             /* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME 198       /* RO */
#define EXT_CSD_PART_SWITCH_TIME 199            /* RO */
#define EXT_CSD_PWR_CL_52_195 200               /* RO */
#define EXT_CSD_PWR_CL_26_195 201               /* RO */
#define EXT_CSD_PWR_CL_52_360 202               /* RO */
#define EXT_CSD_PWR_CL_26_360 203               /* RO */
#define EXT_CSD_SEC_CNT 212                     /* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT 217                 /* RO */
#define EXT_CSD_REL_WR_SEC_C 222                /* RO */
#define EXT_CSD_HC_WP_GRP_SIZE 221              /* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT 223          /* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE 224           /* RO */
#define EXT_CSD_BOOT_MULT 226                   /* RO */
#define EXT_CSD_SEC_TRIM_MULT 229               /* RO */
#define EXT_CSD_SEC_ERASE_MULT 230              /* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT 231         /* RO */
#define EXT_CSD_TRIM_MULT 232                   /* RO */
#define EXT_CSD_PWR_CL_200_195 236              /* RO */
#define EXT_CSD_PWR_CL_200_360 237              /* RO */
#define EXT_CSD_PWR_CL_DDR_52_195 238           /* RO */
#define EXT_CSD_PWR_CL_DDR_52_360 239           /* RO */
#define EXT_CSD_BKOPS_STATUS 246                /* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME 247         /* RO */
#define EXT_CSD_GENERIC_CMD6_TIME 248           /* RO */
#define EXT_CSD_CACHE_SIZE 249                  /* RO, 4 bytes */
#define EXT_CSD_PWR_CL_DDR_200_360 253          /* RO */
#define EXT_CSD_FIRMWARE_VERSION 254            /* RO, 8 bytes */
#define EXT_CSD_PRE_EOL_INFO 267                /* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A 268  /* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B 269  /* RO */
#define EXT_CSD_CMDQ_DEPTH 307                  /* RO */
#define EXT_CSD_CMDQ_SUPPORT 308                /* RO */
#define EXT_CSD_SUPPORTED_MODE 493              /* RO */
#define EXT_CSD_TAG_UNIT_SIZE 498               /* RO */
#define EXT_CSD_DATA_TAG_SUPPORT 499            /* RO */
#define EXT_CSD_MAX_PACKED_WRITES 500           /* RO */
#define EXT_CSD_MAX_PACKED_READS 501            /* RO */
#define EXT_CSD_BKOPS_SUPPORT 502               /* RO */
#define EXT_CSD_HPI_FEATURES 503

/*
 * EXT_CSD field definitions
 */
#define EXT_CSD_CMD_SET_NORMAL (1 << 0)
#define EXT_CSD_CMD_SET_SECURE (1 << 1)
#define EXT_CSD_CMD_SET_CPSECURE (1 << 2)

/* -- EXT_CSD[183] BUS_WIDTH */
#define EXT_CSD_BUS_WIDTH_1 0 /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4 1 /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8 2 /* Card is in 8 bit mode */
#define EXT_CSD_BUS_DDR_4 5   /* Card is in 4 bit ddr mode */
#define EXT_CSD_BUS_DDR_8 6   /* Card is in 8 bit ddr mode */

/* -- EXT_CSD[185] HS_TIMING */
#define EXT_CSD_TIMING_BC 0    /* Backwards compatibility */
#define EXT_CSD_TIMING_HS 1    /* High speed */
#define EXT_CSD_TIMING_HS200 2 /* HS200 */
#define EXT_CSD_TIMING_HS400 3 /* HS400 */

typedef enum
{
    NORMAL_SPEED = 0,
    HIGH_SPEED = 1,
    HIGH_SPEED_200 = 2,
    HIGH_SPEED_400 = 3,
} mmc_speed_e;

typedef struct
{
    unsigned int sdmmc_rca_addr;
    sdmmc_data_width_e data_width;
    sdmmc_send_cmd_info_t **cmd_info;
    sdmmc_device_info *info;
} mmc_cmd_config_t;

typedef struct
{
    unsigned char cmd_index;
    data_transfer_t *data_trans;
    sdmmc_send_cmd_info_t **cmd_info;
} mmc_data_config_t;

void mmc_cmd_init(mmc_cmd_config_t *config);

void mmc_cmd_info_table_init(sdmmc_send_cmd_info_t **cmd_info);

void mmc_set_speed(mmc_cmd_config_t *config, mmc_speed_e spe_mode);

void mmc_set_data_width(mmc_cmd_config_t *config, sdmmc_data_width_e width);

void mmc_get_all_ecsd(mmc_data_config_t *config,unsigned int* ecsd_buf);

void mmc_set_block_num(mmc_cmd_config_t *config, unsigned int num);

void mmc_write_block_buf(mmc_data_config_t *config, unsigned int *src, unsigned int *dest, unsigned int block_num);

void mmc_read_block_buf(mmc_data_config_t *config, unsigned int *dest, unsigned int *src, unsigned int block_num);
#endif

