/********************************************************************************************************
 * @file    tlkmw_fs_drv_sdcard.c
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
#if TLK_CFG_FS_ENABLE && TLKMW_FS_DISK_IO_SELECT == TLKMW_FS_DISK_IO_SDCARD
#include "drivers.h"

sdmmc_pin_config_t sdcard_pin_config = {
    .sdmmc_dat1_pin = GPIO_FC_PG0,
    .sdmmc_dat0_pin = GPIO_FC_PG1,
    .sdmmc_clk_pin  = GPIO_FC_PG2,
    .sdmmc_cmd_pin  = GPIO_FC_PG3,
    .sdmmc_dat3_pin = GPIO_FC_PG4,
    .sdmmc_dat2_pin = GPIO_FC_PG5,
    .sdmmc_rst_pin  = GPIO_NONE_PIN,
    .sdmmc_ds_pin   = GPIO_NONE_PIN,
    .sdmmc_dat4_pin = GPIO_NONE_PIN,
    .sdmmc_dat5_pin = GPIO_NONE_PIN,
    .sdmmc_dat6_pin = GPIO_NONE_PIN,
    .sdmmc_dat7_pin = GPIO_NONE_PIN,
};

static sdmmc_send_cmd_info_t *sdcard_cmd_table[64];

sdmmc_bus_clk_t sdmmc_bus_clk = {
    .clock_src = XTAL_48M,
    .clock_div = SDMMC_BUS_CLK_DIV1,
};

sdmmc_sampe_edge_t sdmmc_sampe_edge = {
    .cmd_edge        = NEGEDGE,
    .read_data_edge  = POSEDGE,
    .write_data_edge = NEGEDGE,
    .resp_start_edge = POSEDGE,
};

sdmmc_timerout_config_t timerout_config = {
    .clk_num       = 10,
    .base_clk_unit = BASE_CLOCK_UNIT_1MHZ,
};

sdmmc_dma_config_t sdmmc_dma_config = {
    .dma_en     = true,
    .dma_select = SDMA,
};

sdmmc_config_t sdmmc_config = {
    .bus_clk         = &sdmmc_bus_clk,
    .sampe_edge      = &sdmmc_sampe_edge,
    .slot_type       = EMBEDDED_SLOTS_DEVICES,
    .timerout_config = &timerout_config,
    .pin_config      = &sdcard_pin_config,
};

sdmmc_trans_config_t trans_config = {
    .block_size = 512,
    .auto_cmd   = DIS_AUTO_CMD,
    .data_width = DATA_WIDTH_1BIT,
    .dma_config = &sdmmc_dma_config,
};

sdcard_cmd_config_t cmd_config = {
    .sdmmc_rca_addr = 1 << 0x10,
    .data_width     = DATA_WIDTH_1BIT,
    .cmd_info       = sdcard_cmd_table,
};

data_transfer_t sdcard_data_trans = {
    .dma_config = &sdmmc_dma_config,
    .block_size = 512,
    .cmd_info   = NULL,
};

sdcard_data_config_t cmd24_data_config = {
    .cmd_info   = sdcard_cmd_table,
    .data_trans = &sdcard_data_trans,
    .cmd_index  = HOST_WRITE_BLOCK,
};

sdcard_data_config_t cmd25_data_config = {
    .cmd_info   = sdcard_cmd_table,
    .data_trans = &sdcard_data_trans,
    .cmd_index  = HOST_WRITE_MULTIPLE_BLOCK,
};

sdcard_data_config_t cmd17_data_config = {
    .cmd_info   = sdcard_cmd_table,
    .data_trans = &sdcard_data_trans,
    .cmd_index  = HOST_READ_SINGLE_BLOCK,
};

sdcard_data_config_t cmd18_data_config = {
    .cmd_info   = sdcard_cmd_table,
    .data_trans = &sdcard_data_trans,
    .cmd_index  = HOST_READ_MULTIPLE_BLOCK,
};

static sdcard_t           *sTlkmwFsSdcardInfo;
static TlkOsSemphrHandle_t sTlkmwFsSdcardSemphr    = 0;
static volatile uint8_t    sTlkmwFsSdcardCmdDone   = 0;
static volatile uint8_t    sTlkmwFsSdcardTransDone = 0;

/**
 * @brief       This function powers on the sdcard device.
 * @param[in]   none.
 * @return      none.
 */
static void sdcard_power_on(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
    gpio_function_en(GPIO_PF4);
    gpio_output_en(GPIO_PF4);
    gpio_input_dis(GPIO_PF4);
    gpio_set_high_level(GPIO_PF4);
    gpio_set_up_down_res(GPIO_PF4, GPIO_PIN_PULLUP_1M);
    delay_us(500);
#endif
}

/**
 * @brief       This function powers down the sdcard device.
 * @param[in]   none.
 * @return      none.
 */
static void sdcard_power_down(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
    gpio_set_low_level(GPIO_PF4);
    gpio_set_up_down_res(GPIO_PF4, GPIO_PIN_PULLDOWN_100K);
#endif
}
#if 0
/**
 * @brief       This function waits for command completion.
 * @param[in]   none.
 * @return      none.
 */
void sdmmc_wait_cmd_done(void)
{
    while(!sTlkmwFsSdcardCmdDone){
        if(tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING){
            tlkos_semphr_take(sTlkmwFsSdcardSemphr,10);
        }
    }
    sTlkmwFsSdcardCmdDone = 0;
}

/**
 * @brief       This function waits for data transfer completion.
 * @param[in]   none.
 * @return      none.
 */
void sdmmc_wait_transport_done(void)
{
    while(!sTlkmwFsSdcardTransDone){
        if(tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING){
            tlkos_semphr_take(sTlkmwFsSdcardSemphr,10);
        }
    }
    sTlkmwFsSdcardTransDone = 0;
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
        sTlkmwFsSdcardCmdDone = 1;
        tlkos_semphr_giveFromISR(sTlkmwFsSdcardSemphr); 
    }

    if (status & SDMMC_INTR_TRANSFER_COMPLETE_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_TRANSFER_COMPLETE_STS);
        sTlkmwFsSdcardTransDone = 1;
        tlkos_semphr_giveFromISR(sTlkmwFsSdcardSemphr);
    }

    if (status & SDMMC_INTR_DATA_TIMEOUT_ERR_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_DATA_TIMEOUT_ERR_STS);
        //have problem
    }
}
#endif
/**
 * @brief       This function initializes the sdcard driver.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_sdcard_init(void)
{
    if (sTlkmwFsSdcardSemphr == NULL) {
        tlkos_semphr_createBinary(&sTlkmwFsSdcardSemphr);
    }
    BM_CLR(reg_rst_1, FLD_RST4_SDIO);
    BM_SET(reg_rst_1, FLD_RST4_SDIO);
    reg_clk_en_1 |= FLD_CLK4_SDIO_EN;

    sdcard_power_on();

    sdcard_cmd_info_table_init(sdcard_cmd_table);
    sdmmc_core_init(&sdmmc_config);
    sdmmc_data_trans_init(&trans_config);

    sdmmc_set_clear_irq_status(0x1FFFFFFF);
    core_interrupt_enable();
    plic_interrupt_enable(IRQ_SDIO);
    sdmmc_set_intereupt_mask(SDMMC_INTR_CMD_COMPLETE_STS_EN | SDMMC_INTR_TRANSFER_COMPLETE_STS_EN | SDMMC_INTR_DATA_TIMEOUT_ERR_STS_EN);

    sTlkmwFsSdcardInfo = sdcard_cmd_init(&cmd_config);

    sdmmc_set_clk_div(1);

    sdcard_set_data_width(SDCARD_DATA_WIDTH_4BIT);
}

/**
 * @brief       This function puts the sdcard device to sleep.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_sdcard_sleep(void)
{
    (void)sdcard_power_down;
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
    // sdcard_power_down();
#endif
}

/**
 * @brief       This function wakes up the sdcard device.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_sdcard_awake(void)
{
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
    // tlkmw_fs_drv_sdcard_init();
    // delay_us(200);
#endif
}

/**
 * @brief       This function writes data to the sdcard device.
 * @param[in]   buff    - pointer to data buffer to write.
 * @param[in]   lba     - logical block address to write to.
 * @param[in]   cnt     - number of blocks to write.
 * @return      Returns 0 on success.
 */
static int tlkmw_fs_drv_sdcard_write(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    if (cnt > 1) {
        return 1;
    }
    sdcard_write_block_buf(&cmd24_data_config, (unsigned int *)buff, (unsigned int *)lba, 0x01);
    return 0;
}

/**
 * @brief       This function reads data from the sdcard device.
 * @param[out]  buff    - pointer to buffer to store read data.
 * @param[in]   lba     - logical block address to read from.
 * @param[in]   cnt     - number of blocks to read.
 * @return      Returns 0 on success.
 */
static int tlkmw_fs_drv_sdcard_read(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    if (cnt > 1) {
        return 1;
    }
    sdcard_read_block_buf(&cmd17_data_config, (unsigned int *)buff, (unsigned int *)lba, 0x01);
    return 0;
}

/**
 * @brief       This function gets the sector size of the sdcard device.
 * @param[in]   none.
 * @return      Returns the sector size (512 bytes).
 */
static uint32_t tlkmw_fs_drv_sdcard_get_sector_size(void)
{
    return 512;
}

/**
 * @brief       This function gets the sector number of the sdcard device.
 * @param[in]   none.
 * @return      Returns the sector number.
 */
static uint32_t tlkmw_fs_drv_sdcard_get_sector_num(void)
{
    return sTlkmwFsSdcardInfo->card_capacity / 512;
}

const tlkmw_fs_diskio_t gTlkmwFsDiskIoSdcard = {
    .init          = tlkmw_fs_drv_sdcard_init,
    .sleep         = tlkmw_fs_drv_sdcard_sleep,
    .awake         = tlkmw_fs_drv_sdcard_awake,
    .write         = tlkmw_fs_drv_sdcard_write,
    .read          = tlkmw_fs_drv_sdcard_read,
    .getSectorSize = tlkmw_fs_drv_sdcard_get_sector_size,
    .getSectorNum  = tlkmw_fs_drv_sdcard_get_sector_num,
};

#endif