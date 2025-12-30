/********************************************************************************************************
 * @file    tlkmw_fs_drv_emmc.c
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
#if TLK_CFG_FS_ENABLE && MCU_CORE_TYPE == CHIP_TYPE_TL751X
#include "drivers.h"
#include "../tlkmw_fs_diskio.h"

typedef enum
{
    MMC_TIMING_LEGACY = 0U,
    MMC_TIMING_HS,
    MMC_TIMING_HS200,
    MMC_TIMING_HS400
} mmc_timing_mode;

typedef struct
{
    uint8_t hs400_ddr_1_2v : 1;
    uint8_t hs400_ddr_1_8v : 1;
    uint8_t hs200_sdr_1_2v : 1;
    uint8_t hs200_sdr_1_8v : 1;
    uint8_t hs_ddr_1_2v    : 1;
    uint8_t hs_ddr_1_8v    : 1;
    uint8_t hs_52_dv       : 1;
    uint8_t hs_26_dv       : 1;
} mmc_dev_type_t;

typedef enum
{
    MMC_V5_1 = 8U,
    MMC_V5_0 = 7U,
    MMC_V4_5 = 6U,
    MMC_V4_4 = 5U,
    MMC_V4_3 = 3U,
    MMC_V4_2 = 2U,
    MMC_V4_1 = 1U,
    MMC_V4_0 = 0U
} mmc_ver_t;

typedef struct
{
    uint32_t        sec_count; /** Sector Count [215:212] */
    uint32_t        sec_size;
    uint8_t         bus_width;  /** Bus width mode [183] */
    mmc_timing_mode mmc_timing; /** High Speed Timing Mode [185] */
    mmc_dev_type_t  dev_type;   /** Device type [196] */
    mmc_ver_t       ver;        /** Extend CSD revision [192] */
} mmc_ext_csd_t;

typedef struct
{
    /** Manufacturer ID [127:120] */
    uint8_t manufacturer;
    /** OEM/Application ID [119:104] */
    uint16_t application;
    /** Product name [103:64] */
    uint8_t name[5];
    /** Product revision [63:56] */
    uint8_t version;
    /** Product serial number [55:24] */
    uint32_t serial_num;
    /** Manufacturing date [19:8] */
    uint16_t date;
} mmc_cid_t;

typedef struct
{
    mmc_cid_t     cid;
    mmc_ext_csd_t ext_csd;
} sdio_mmc_t;

static sdmmc_bus_clk_t sdmmc_bus_clk = {
    .clock_src = XTAL_48M,
    .clock_div = SDMMC_BUS_CLK_DIV4,
};

static sdmmc_sampe_edge_t sdmmc_sampe_edge = {
    .cmd_edge        = NEGEDGE,
    .read_data_edge  = POSEDGE,
    .write_data_edge = NEGEDGE,
    .resp_start_edge = POSEDGE,
};

static sdmmc_pin_config_t sdmmc_emmc_pin_config = {

#if (TLKHW_TYPE != TLKHW_TL751X_EVK_C1T368A87_V1_0)
    .sdmmc_clk_pin  = GPIO_FC_PA0,
    .sdmmc_cmd_pin  = GPIO_FC_PA1,
    .sdmmc_rst_pin  = GPIO_FC_PB6,
    .sdmmc_ds_pin   = GPIO_NONE_PIN,
    .sdmmc_dat0_pin = GPIO_FC_PC1,
    .sdmmc_dat1_pin = GPIO_FC_PB7,
    .sdmmc_dat2_pin = GPIO_FC_PF4,
    .sdmmc_dat3_pin = GPIO_FC_PA2,
    .sdmmc_dat4_pin = GPIO_NONE_PIN,
    .sdmmc_dat5_pin = GPIO_NONE_PIN,
    .sdmmc_dat6_pin = GPIO_NONE_PIN,
    .sdmmc_dat7_pin = GPIO_NONE_PIN,

#else
    .sdmmc_clk_pin  = GPIO_FC_PG0,
    .sdmmc_cmd_pin  = GPIO_FC_PB7,
    .sdmmc_rst_pin  = GPIO_FC_PG5,
    .sdmmc_ds_pin   = GPIO_NONE_PIN,
    .sdmmc_dat0_pin = GPIO_FC_PG3,
    .sdmmc_dat1_pin = GPIO_FC_PG2,
    .sdmmc_dat2_pin = GPIO_FC_PG1,
    .sdmmc_dat3_pin = GPIO_FC_PG4,
    .sdmmc_dat4_pin = GPIO_NONE_PIN,
    .sdmmc_dat5_pin = GPIO_NONE_PIN,
    .sdmmc_dat6_pin = GPIO_NONE_PIN,
    .sdmmc_dat7_pin = GPIO_NONE_PIN,
#endif
};

static sdmmc_timerout_config_t timerout_config = {
    .clk_num       = 24,
    .base_clk_unit = BASE_CLOCK_UNIT_1MHZ,
};

static sdmmc_config_t sdmmc_config = {
    .bus_clk         = &sdmmc_bus_clk,
    .sampe_edge      = &sdmmc_sampe_edge,
    .slot_type       = EMBEDDED_SLOTS_DEVICES,
    .timerout_config = &timerout_config,
    .pin_config      = &sdmmc_emmc_pin_config,
};

static sdmmc_dma_config_t sdmmc_dma_config = {
    .dma_en     = true,
    .dma_select = SDMA,
};

static sdmmc_trans_config_t trans_config = {
    .block_size = 512,
    .auto_cmd   = DIS_AUTO_CMD,
    .data_width = DATA_WIDTH_1BIT,
    .dma_config = &sdmmc_dma_config,
};

static sdmmc_send_cmd_info_t *mmc_cmd_table[CMD_NUM];


static mmc_cmd_config_t cmd_config = {
    .sdmmc_rca_addr = 1 << 0x10,
    .data_width     = DATA_WIDTH_1BIT,
    .cmd_info       = mmc_cmd_table,
};

static data_transfer_t mmc_data_trans = {
    .dma_config = &sdmmc_dma_config,
    .block_size = 512,
    .cmd_info   = NULL,
};

static mmc_data_config_t cmd8_data_config = {
    .cmd_info   = mmc_cmd_table,
    .data_trans = &mmc_data_trans,
    .cmd_index  = HOST_SEND_EXT_CSD,
};

//static mmc_data_config_t cmd23_data_config = {
//    .cmd_info = mmc_cmd_table,
//    .data_trans = &mmc_data_trans,
//    .cmd_index = HOST_SET_BLOCK_COUNT,
//};

static mmc_data_config_t cmd24_data_config = {
    .cmd_info   = mmc_cmd_table,
    .data_trans = &mmc_data_trans,
    .cmd_index  = HOST_WRITE_BLOCK,
};

//static mmc_data_config_t cmd25_data_config = {
//    .cmd_info = mmc_cmd_table,
//    .data_trans = &mmc_data_trans,
//    .cmd_index = HOST_WRITE_MULTIPLE_BLOCK,
//};

static mmc_data_config_t cmd17_data_config = {
    .cmd_info   = mmc_cmd_table,
    .data_trans = &mmc_data_trans,
    .cmd_index  = HOST_READ_SINGLE_BLOCK,
};

//static mmc_data_config_t cmd18_data_config = {
//    .cmd_info = mmc_cmd_table,
//    .data_trans = &mmc_data_trans,
//    .cmd_index = HOST_READ_MULTIPLE_BLOCK,
//};

static uint8_t             ecsd_buf[EXT_CSD_NUM] = {0x00};
static mmc_ext_csd_t       s_mmc_ext_csd;
static TlkOsSemphrHandle_t sTlkmwFsEmmcSemphr    = 0;
static volatile uint8_t    sTlkmwFsEmmcCmdDone   = 0;
static volatile uint8_t    sTlkmwFsEmmcTransDone = 0;

/**
 * @brief       This function powers on the eMMC device.
 * @param[in]   none.
 * @return      none.
 */
static void emmc_power_on(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0)
    gpio_function_en(GPIO_PF4);
    gpio_output_en(GPIO_PF4);
    gpio_input_dis(GPIO_PF4);
    gpio_set_high_level(GPIO_PF4);
    gpio_set_up_down_res(GPIO_PF4, GPIO_PIN_PULLUP_1M);
    delay_us(500);
#endif
}

/**
 * @brief       This function powers down the eMMC device.
 * @param[in]   none.
 * @return      none.
 */
static void emmc_power_down(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0)
    gpio_set_low_level(GPIO_PF4);
    gpio_set_up_down_res(GPIO_PF4, GPIO_PIN_PULLDOWN_100K);
#endif
}

/**
 * @brief       This function waits for command completion.
 * @param[in]   none.
 * @return      none.
 */
void sdmmc_wait_cmd_done(void)
{
    while (!sTlkmwFsEmmcCmdDone) {
        if (tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING) {
            tlkos_semphr_take(sTlkmwFsEmmcSemphr, 10);
        }
    }
    sTlkmwFsEmmcCmdDone = 0;
}

/**
 * @brief       This function waits for data transfer completion.
 * @param[in]   none.
 * @return      none.
 */
void sdmmc_wait_transport_done(void)
{
    while (!sTlkmwFsEmmcTransDone) {
        if (tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING) {
            tlkos_semphr_take(sTlkmwFsEmmcSemphr, 10);
        }
    }
    sTlkmwFsEmmcTransDone = 0;
}

/**
 * @brief       This function handles SDIO interrupts.
 * @param[in]   none.
 * @return      none.
 */
void sdio_irq_handler(void)
{
    unsigned int status = reg_sdmmc_irq_sts & reg_sdmmc_cq_intr_sts_en;

    if (status & SDMMC_INTR_CMD_COMPLETE_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_CMD_COMPLETE_STS);
        sTlkmwFsEmmcCmdDone = 1;
        tlkos_semphr_giveFromISR(sTlkmwFsEmmcSemphr);
    }

    if (status & SDMMC_INTR_TRANSFER_COMPLETE_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_TRANSFER_COMPLETE_STS);
        sTlkmwFsEmmcTransDone = 1;
        tlkos_semphr_giveFromISR(sTlkmwFsEmmcSemphr);
    }

    if (status & SDMMC_INTR_DATA_TIMEOUT_ERR_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_DATA_TIMEOUT_ERR_STS);
        //have problem
    }
}

/**
 * @brief       This function updates extended CSD information.
 * @param[out]  ext    - pointer to extended CSD structure.
 * @param[in]   raw    - pointer to raw extended CSD data.
 * @return      Returns 0 on success.
 */
int emmc_ext_csd_update(mmc_ext_csd_t *ext, uint8_t *raw)
{
    ext->sec_count  = (raw[215] << 24) + (raw[214] << 16) + (raw[213] << 8) + (raw[212] << 0);
    ext->bus_width  = raw[183];
    ext->mmc_timing = raw[185];

    ext->dev_type.hs400_ddr_1_2v = ((1 << 7) & raw[196]) ? 1 : 0;
    ext->dev_type.hs400_ddr_1_8v = ((1 << 6) & raw[196]) ? 1 : 0;
    ext->dev_type.hs200_sdr_1_2v = ((1 << 5) & raw[196]) ? 1 : 0;
    ext->dev_type.hs200_sdr_1_8v = ((1 << 4) & raw[196]) ? 1 : 0;
    ext->dev_type.hs_ddr_1_2v    = ((1 << 3) & raw[196]) ? 1 : 0;
    ext->dev_type.hs_ddr_1_8v    = ((1 << 2) & raw[196]) ? 1 : 0;
    ext->dev_type.hs_52_dv       = ((1 << 1) & raw[196]) ? 1 : 0;
    ext->dev_type.hs_26_dv       = ((1 << 0) & raw[196]) ? 1 : 0;

    ext->sec_size = raw[61] == 0 ? 512 : raw[61] == 1 ? 4096 : raw[61] == 2 ? 8192 : 512;

    ext->ver = (mmc_ver_t)raw[192];

    return 0;
}

/**
 * @brief       This function updates CID information.
 * @param[out]  cid    - pointer to CID structure.
 * @param[in]   raw    - pointer to raw CID data.
 * @return      Returns 0 on success.
 */
int emmc_cid_update(mmc_cid_t *cid, uint32_t *raw)
{
    cid->manufacturer = (uint8_t)((raw[3] & 0xFF000000) >> 24);
    cid->application  = (uint16_t)((raw[3] & 0xFFFF00) >> 8);

    cid->name[0] = (uint8_t)((raw[3] & 0xFF));
    cid->name[1] = (uint8_t)((raw[2] & 0xFF000000U) >> 24);
    cid->name[2] = (uint8_t)((raw[2] & 0xFF0000) >> 16);
    cid->name[3] = (uint8_t)((raw[2] & 0xFF00) >> 8);
    cid->name[4] = (uint8_t)((raw[2] & 0xFF));

    cid->version = (uint8_t)((raw[1] & 0xFF000000U) >> 24);

    cid->serial_num = (uint32_t)((raw[1] & 0xFFFFFFU) << 8);
    cid->serial_num |= (uint32_t)((raw[0] & 0xFF000000U) >> 24);

    cid->date = (uint16_t)((raw[0U] & 0xFFF00) >> 8);
    return 0;
}

/**
 * @brief       This function initializes the eMMC driver.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_emmc_init(void)
{
    if (sTlkmwFsEmmcSemphr == NULL) {
        tlkos_semphr_createBinary(&sTlkmwFsEmmcSemphr);
    }
    emmc_power_on();
    // SDIO reset & enable clock
    BM_CLR(reg_rst_1, FLD_RST4_SDIO);
    BM_SET(reg_rst_1, FLD_RST4_SDIO);
    reg_clk_en_1 |= FLD_CLK4_SDIO_EN;

    // SDIO hw init
    sdmmc_core_init(&sdmmc_config);
    sdmmc_data_trans_init(&trans_config);
    sdmmc_set_clear_irq_status(0x1FFFFFFF);
    plic_interrupt_enable(IRQ_SDIO);
    sdmmc_set_intereupt_mask(SDMMC_INTR_CMD_COMPLETE_STS_EN | SDMMC_INTR_TRANSFER_COMPLETE_STS_EN | SDMMC_INTR_DATA_TIMEOUT_ERR_STS_EN);

    // eMMC init
    mmc_cmd_info_table_init(mmc_cmd_table);
    mmc_cmd_init(&cmd_config);
    mmc_get_all_ecsd(&cmd8_data_config, (unsigned int *)ecsd_buf);

    // Change to high speed
    mmc_set_speed(&cmd_config, NORMAL_SPEED);
    sdmmc_set_clk_div(0);

    mmc_set_data_width(&cmd_config, DATA_WIDTH_4BIT);

    emmc_ext_csd_update(&s_mmc_ext_csd, ecsd_buf);
}

/**
 * @brief       This function puts the eMMC device to sleep.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_emmc_sleep(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0)
    mmc_cmd_table[HOST_SEND_STATUS]->argument.arg = 1 << 16;
    sdmmc_cmd_transfer(mmc_cmd_table[HOST_SEND_STATUS]);

    mmc_cmd_table[HOST_SELECT_DESELECT_CARD]->argument.rca_addr = 0;
    mmc_cmd_table[HOST_SELECT_DESELECT_CARD]->sdmmc_resport     = RESPONSE_NO;
    sdmmc_cmd_transfer(mmc_cmd_table[HOST_SELECT_DESELECT_CARD]);

    mmc_cmd_table[HOST_SLEEP_AWAKE]->argument.rca_addr = 0X00018000;
    sdmmc_cmd_transfer(mmc_cmd_table[HOST_SLEEP_AWAKE]);
    sdmmc_host_clk_dis();

    emmc_power_down();
#endif
}

/**
 * @brief       This function wakes up the eMMC device.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_emmc_awake(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0)
    tlkmw_fs_drv_emmc_init();
    delay_us(200);
#endif
}

/**
 * @brief       This function writes data to the eMMC device.
 * @param[in]   buff    - pointer to data buffer to write.
 * @param[in]   lba     - logical block address to write to.
 * @param[in]   cnt     - number of blocks to write.
 * @return      Returns 0 on success.
 */
static int tlkmw_fs_drv_emmc_write(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    mmc_write_block_buf(&cmd24_data_config, (unsigned int *)buff, (unsigned int *)lba, cnt);
    return 0;
}

/**
 * @brief       This function reads data from the eMMC device.
 * @param[out]  buff    - pointer to buffer to store read data.
 * @param[in]   lba     - logical block address to read from.
 * @param[in]   cnt     - number of blocks to read.
 * @return      Returns 0 on success.
 */
static int tlkmw_fs_drv_emmc_read(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    mmc_read_block_buf(&cmd17_data_config, (unsigned int *)buff, (unsigned int *)lba, cnt);
    return 0;
}

/**
 * @brief       This function gets the sector size of the eMMC device.
 * @param[in]   none.
 * @return      Returns the sector size (512 bytes).
 */
static uint32_t tlkmw_fs_drv_emmc_get_sector_size(void)
{
    return 512;
}

/**
 * @brief       This function gets the sector number of the eMMC device.
 * @param[in]   none.
 * @return      Returns the sector number.
 */
static uint32_t tlkmw_fs_drv_emmc_get_sector_num(void)
{
    return s_mmc_ext_csd.sec_count;
}

const tlkmw_fs_diskio_t gTlkmwFsDiskIoEmmc = {
    .init          = tlkmw_fs_drv_emmc_init,
    .sleep         = tlkmw_fs_drv_emmc_sleep,
    .awake         = tlkmw_fs_drv_emmc_awake,
    .write         = tlkmw_fs_drv_emmc_write,
    .read          = tlkmw_fs_drv_emmc_read,
    .getSectorSize = tlkmw_fs_drv_emmc_get_sector_size,
    .getSectorNum  = tlkmw_fs_drv_emmc_get_sector_num,
};

#endif