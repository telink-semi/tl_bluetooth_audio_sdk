/********************************************************************************************************
 * @file    hal_spi.c
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
#if TLK_CFG_FS_ENABLE && MCU_CORE_TYPE == CHIP_TYPE_TL721X
#include "tlkapi/tlkapi.h"
#include "hal_spi.h"
#include "drivers.h"
#include "compatibility_pack/cmpt.h"
#include "../tlkmw_fs_diskio.h"
#define PIN_SEL GPIO_PB4
#define PIN_CLK GPIO_PB6
#define PIN_MO  GPIO_PB5
#define PIN_MI  GPIO_PB7
#if TLKMW_FS_DISK_IO_SELECT == TLKMW_FS_DISK_IO_DS35X
#define PIN_QIO2 GPIO_PB2
#define PIN_QIO3 GPIO_PB0
#endif

static TlkOsSemphrHandle_t sTlkmwFsSpiSemphr   = 0;
static volatile uint8_t    sTlkmwFsSpiDoneFlag = 0;
static uint8_t             sTlkmwFsSpiMode     = 0;
static uint8_t             sTlkmwFsSpiTxDma    = 0XFF;
static uint8_t             sTlkmwFsSpiRxDma    = 0XFF;

/**
 * @brief       This function selects/deselects the SPI master.
 * @param[in]   en    - enable flag (1 to select, 0 to deselect).
 * @return      none.
 */
void hal_spi_master_sel(int en)
{
    gpio_write(PIN_SEL, en);
}

/**
 * @brief       This function waits for SPI operation to complete.
 * @param[in]   isUseSemphr - is needed use semphr to task switch.
 * @return      Returns 1 on success, 0 on timeout.
 */
int hal_spi_wait_done(uint8_t isUseSemphr)
{
    uint32_t t = stimer_get_tick();
    do {
        if (tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING && isUseSemphr) {
            tlkos_semphr_take(sTlkmwFsSpiSemphr, 1);
        }
        if (sTlkmwFsSpiDoneFlag) {
            return 1;
        }
    } while (!clock_time_exceed(t, 20000));

    return 0;
}

/**
 * @brief       This function handles the SPI interrupt.
 * @param[in]   none.
 * @return      none.
 */
void gspi_irq_handler(void)
{
    if (spi_get_irq_status(GSPI_MODULE, SPI_END_INT)) {
        spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
        sTlkmwFsSpiDoneFlag = 1;
        tlkos_semphr_giveFromISR(sTlkmwFsSpiSemphr);
    }
}

/**
 * @brief       This function writes data via SPI master.
 * @param[in]   dat    - pointer to data buffer to write.
 * @param[in]   len    - length of data to write.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_write(uint8_t *dat, int len)
{
    // Clear flag
    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    sTlkmwFsSpiDoneFlag = 0;
    // Start write
    spi_master_write_dma_plus(GSPI_MODULE, 0, (unsigned int)NULL, (unsigned char *)dat, len, SPI_MODE_WR_WRITE_ONLY);

    // Wait done
    return hal_spi_wait_done(len > 8);
}

/**
 * @brief       This function reads data via SPI master.
 * @param[out]  d      - pointer to buffer to store read data.
 * @param[in]   len    - length of data to read.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_read(uint8_t *d, int len)
{
    int ret;
    // Disable MOSI
    if (sTlkmwFsSpiMode == SPI_SINGLE_MODE) {
        gpio_write(PIN_MO, 1);
        gpio_set_gpio_en(PIN_MO);
        gpio_set_output_en(PIN_MO, 1);
    }

    // Clear flag
    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    sTlkmwFsSpiDoneFlag = 0;

    // Start read
    spi_master_read_dma_plus(GSPI_MODULE, 0, (unsigned int)NULL, (unsigned char *)d, len, SPI_MODE_RD_READ_ONLY);

    // Wait done
    ret = hal_spi_wait_done(len > 8);

    // Set as MO
    if (sTlkmwFsSpiMode == SPI_SINGLE_MODE) {
        gspi_set_pin_mux((gpio_func_pin_e)PIN_MO, GSPI_MOSI_IO);
    }

    return ret;
}

/**
 * @brief       This function performs dummy write operations on SPI master.
 * @param[in]   n    - number of dummy bytes to write.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_dummy(int n)
{
    int     cnt;
    uint8_t dummy_dat[10] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    cnt = n > 10 ? 10 : n;

    // Clear IRQ
    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    sTlkmwFsSpiDoneFlag = 0;

    // Start write
    spi_master_write_dma_plus(GSPI_MODULE, 0, (unsigned int)NULL, (unsigned char *)dummy_dat, cnt, SPI_MODE_WR_WRITE_ONLY);

    // Wait done
    return hal_spi_wait_done(cnt >= 8);
}

/**
 * @brief       This function performs byte-level SPI transfer.
 * @param[in]   w     - byte to write.
 * @param[out]  dr    - pointer to store the read byte.
 * @return      none.
 */
void hal_spi_master_byte_trans(uint8_t w, uint8_t *dr)
{
    uint32_t wd, rd;

    if (w == 0xff && sTlkmwFsSpiMode == SPI_SINGLE_MODE) {
        gpio_write(PIN_MO, 1);
        gpio_set_gpio_en(PIN_MO);
        gpio_set_output_en(PIN_MO, 1);
    }

    // Set write data
    wd = w;
    rd = 0;

    // Start byte transfer
    spi_master_write_read_full_duplex(GSPI_MODULE, (uint8_t *)&wd, (uint8_t *)&rd, 1);
    spi_tx_fifo_clr(GSPI_MODULE);
    spi_rx_fifo_clr(GSPI_MODULE);
    // Get read data
    if (dr != NULL) {
        *dr = (uint8_t)rd;
    }

    if (sTlkmwFsSpiMode == SPI_SINGLE_MODE) {
        gspi_set_pin_mux((gpio_func_pin_e)PIN_MO, GSPI_MOSI_IO);
    }
}

/**
 * @brief       This function sets the SPI master clock speed.
 * @param[in]   speed    - speed setting (use SPI_CLK_* enums).
 * @return      Returns 1 on success.
 */
int hal_spi_master_clk_set(int speed)
{
    uint32_t div_clock[SPI_CLK_NUM] = {
        [SPI_CLK_40MHZ] = 6,  [SPI_CLK_30MHZ] = 8, [SPI_CLK_20MHZ] = 12, [SPI_CLK_15MHZ] = 18,
        [SPI_CLK_10MHZ] = 24, [SPI_CLK_4MHZ] = 60, [SPI_CLK_1MHZ] = 240, [SPI_CLK_250KHZ] = 240 * 4,
    };

    speed = speed >= SPI_CLK_NUM ? SPI_CLK_250KHZ : speed;

    if (div_clock[speed] > 255) { //GSPI clock source select pll_clk when div_clock <= 255,select xtl 24m when div_clock > 255.
        clock_bbpll_config(PLL_CLK);
        reg_gspi_clk_set = ((FLD_GSPI_CLK_MOD & (unsigned char)(div_clock[speed] * 24 / (sys_clk.pll_clk))) | (1 << 8));
    } else {
        reg_gspi_clk_set = ((FLD_GSPI_CLK_MOD & (unsigned char)div_clock[speed]) | (2 << 8));
    }

    return 1;
}

/**
 * @brief       This function sets the SPI master dual mode enable/disable.
 * @param[in]   en    - enable/disable.
 * @return      none.
 */
void hal_spi_master_dual_mode_en(uint8_t en)
{
    if (en) {
        spi_set_io_mode(GSPI_MODULE, SPI_DUAL_MODE);
        sTlkmwFsSpiMode = SPI_DUAL_MODE;
    } else {
        spi_set_io_mode(GSPI_MODULE, SPI_SINGLE_MODE);
        sTlkmwFsSpiMode = SPI_SINGLE_MODE;
    }
}

/**
 * @brief       This function sets the SPI master quad mode enable/disable.
 * @param[in]   en    - enable/disable.
 * @param[in]   idleio_level -io2 and io3 level when quad mode disable.
 * @return      none.
 */
void hal_spi_master_quad_mode_en(uint8_t en, uint8_t io23_level)
{
    (void)io23_level;
    (void)en;
#if defined(PIN_QIO2) && defined(PIN_QIO3)
    if (en) {
        gpio_output_dis(PIN_QIO2);
        gpio_output_dis(PIN_QIO3);
        gspi_set_pin_mux((gpio_func_pin_e)PIN_QIO2, GSPI_IO2_IO);
        gspi_set_pin_mux((gpio_func_pin_e)PIN_QIO3, GSPI_IO3_IO);
        spi_set_io_mode(GSPI_MODULE, SPI_QUAD_MODE);
        sTlkmwFsSpiMode = SPI_QUAD_MODE;
    } else {
        gpio_write(PIN_QIO2, io23_level);
        gpio_set_gpio_en(PIN_QIO2);
        gpio_set_output_en(PIN_QIO2, io23_level);
        gpio_write(PIN_QIO3, io23_level);
        gpio_set_gpio_en(PIN_QIO3);
        gpio_set_output_en(PIN_QIO3, io23_level);
        spi_set_io_mode(GSPI_MODULE, SPI_SINGLE_MODE);
        sTlkmwFsSpiMode = SPI_SINGLE_MODE;
    }
#endif
}

/**
 * @brief       This function initializes the SPI master.
 * @param[in]   none.
 * @return      none.
 */
void hal_spi_master_init()
{
    if (sTlkmwFsSpiSemphr == NULL) {
        tlkos_semphr_createBinary(&sTlkmwFsSpiSemphr);
    }
    sTlkmwFsSpiDoneFlag               = 0;
    gspi_pin_config_t gspi_pin_config = {
        .spi_csn_pin      = 0,
        .spi_clk_pin      = (gpio_func_pin_e)PIN_CLK,
        .spi_mosi_io0_pin = (gpio_func_pin_e)PIN_MO,
        .spi_miso_io1_pin = (gpio_func_pin_e)PIN_MI, //3line mode is required, otherwise it is NONE_PIN.
#if defined(PIN_QIO2) && defined(PIN_QIO3)
        .spi_io2_pin = (gpio_func_pin_e)PIN_QIO2, //quad  mode is required, otherwise it is NONE_PIN.
        .spi_io3_pin = (gpio_func_pin_e)PIN_QIO3, //quad  mode is required, otherwise it is NONE_PIN.
#else
        .spi_io2_pin = (gpio_func_pin_e)0, //quad  mode is required, otherwise it is NONE_PIN.
        .spi_io3_pin = (gpio_func_pin_e)0, //quad  mode is required, otherwise it is NONE_PIN.
#endif
    };
    spi_wr_rd_config_t spi_b91m_slave_protocol_config = {
        .spi_io_mode     = SPI_SINGLE_MODE, /*IO mode set to SPI_3_LINE_MODE when SPI_3LINE_SLAVE.*/
        .spi_dummy_cnt   = 0,               //B92 supports up to 32 clk cycle dummy, and TL751X,TL7518,TL721X,TL321X,tl322x supports up to 256 clk cycle dummy.
        .spi_cmd_en      = 0,
        .spi_addr_en     = 0,
        .spi_addr_len    = 0, //when spi_addr_en = 0,invalid set.
        .spi_cmd_fmt_en  = 0, //when spi_cmd_en = 0,invalid set.
        .spi_addr_fmt_en = 0, //when spi_addr_en = 0,invalid set.
    };

    spi_master_init(GSPI_MODULE, 240 * 4, SPI_MODE0);

    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    spi_set_irq_mask(GSPI_MODULE, SPI_END_INT_EN);
    if (sTlkmwFsSpiTxDma == 0XFF) {
        sTlkmwFsSpiTxDma = tlkhal_dma_malloc();
    }
    if (sTlkmwFsSpiRxDma == 0XFF) {
        sTlkmwFsSpiRxDma = tlkhal_dma_malloc();
    }
    spi_set_tx_dma_config(GSPI_MODULE, sTlkmwFsSpiTxDma);
    spi_set_master_rx_dma_config(GSPI_MODULE, sTlkmwFsSpiRxDma);

    gspi_set_pin(&gspi_pin_config);
    spi_master_config_plus(GSPI_MODULE, &spi_b91m_slave_protocol_config);
    plic_interrupt_enable(IRQ_GSPI);

    // Set CS
    gpio_write(PIN_SEL, 1);
    gpio_set_output_en(PIN_SEL, 1);
    gpio_set_gpio_en(PIN_SEL);
}

/**
 * @brief       This function deinitializes the SPI master.
 * @param[in]   none.
 * @return      none.
 */
void hal_spi_master_deinit(void)
{
    gpio_shutdown(PIN_SEL);
    gpio_shutdown(PIN_CLK);
    gpio_shutdown(PIN_MO);
    gpio_shutdown(PIN_MI);
#if defined(PIN_QIO2) && defined(PIN_QIO3)
    gpio_shutdown(PIN_QIO2);
    gpio_shutdown(PIN_QIO3);
#endif
}

#endif
