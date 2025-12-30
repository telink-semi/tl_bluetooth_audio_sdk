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

#if (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)
#define PIN_SEL GPIO_PB4
#define PIN_CLK GPIO_PB6
#define PIN_MO  GPIO_PB5
#define PIN_MI  GPIO_PB7
#else
#define PIN_SEL GPIO_PB4
#define PIN_CLK GPIO_PB6
#define PIN_MO  GPIO_PB5
#define PIN_MI  GPIO_PB7
#endif

#define SPI_TX_DMA_CHN DMA4
#define SPI_RX_DMA_CHN DMA5

static TlkOsSemphrHandle_t sTlkmwFsSpiSemphr   = 0;
static volatile uint8_t    sTlkmwFsSpiDoneFlag = 0;

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
 * @param[in]   none.
 * @return      Returns 1 on success, 0 on timeout.
 */
int hal_spi_wait_done()
{
    uint32_t t = stimer_get_tick();
    do {
        if (tlkos_get_kernelState() == TLKOS_KERNEL_STATE_RUNNING) {
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
    return hal_spi_wait_done();
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
    gpio_write(PIN_MO, 1);
    gpio_set_gpio_en(PIN_MO);
    gpio_set_output_en(PIN_MO, 1);

    // Clear flag
    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    sTlkmwFsSpiDoneFlag = 0;

    // Start read
    spi_master_read_dma_plus(GSPI_MODULE, 0, (unsigned int)NULL, (unsigned char *)d, len, SPI_MODE_RD_READ_ONLY);

    // Wait done
    ret = hal_spi_wait_done();

    // Set as MO
    gspi_set_pin_mux((gpio_func_pin_e)PIN_MO, GSPI_MOSI_IO);

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
    return hal_spi_wait_done();
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

    if (w == 0xff) {
        gpio_write(PIN_MO, 1);
        gpio_set_gpio_en(PIN_MO);
        gpio_set_output_en(PIN_MO, 1);
    }

    // Set write data
    wd = w;
    rd = 0;

    // Start byte transfer
    spi_master_write_read_full_duplex(GSPI_MODULE, (uint8_t *)&wd, (uint8_t *)&rd, 1);

    // Get read data
    *dr = (uint8_t)rd;

    gspi_set_pin_mux((gpio_func_pin_e)PIN_MO, GSPI_MOSI_IO);
}

/**
 * @brief       This function sets the SPI master clock speed.
 * @param[in]   speed    - speed setting (use SPI_CLK_* enums).
 * @return      Returns 1 on success.
 */
int hal_spi_master_clk_set(int speed)
{
    // Clock is 240MHz
    uint32_t div_clock[SPI_CLK_NUM] = {
        [SPI_CLK_20MHZ] = 12, [SPI_CLK_15MHZ] = 18, [SPI_CLK_10MHZ] = 24, [SPI_CLK_4MHZ] = 60, [SPI_CLK_1MHZ] = 240, [SPI_CLK_250KHZ] = 240 * 4,
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
        .spi_io2_pin      = 0,                       //quad  mode is required, otherwise it is NONE_PIN.
        .spi_io3_pin      = 0,                       //quad  mode is required, otherwise it is NONE_PIN.
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

    spi_master_init(GSPI_MODULE, 240 * 4, SPI_MODE3);

    spi_clr_irq_status(GSPI_MODULE, SPI_END_INT);
    spi_set_irq_mask(GSPI_MODULE, SPI_END_INT_EN);
    spi_set_tx_dma_config(GSPI_MODULE, SPI_TX_DMA_CHN);
    spi_set_master_rx_dma_config(GSPI_MODULE, SPI_RX_DMA_CHN);

    gspi_set_pin(&gspi_pin_config);
    spi_master_config_plus(GSPI_MODULE, &spi_b91m_slave_protocol_config);
    plic_interrupt_enable(IRQ_GSPI);

    // Set CS
    gpio_write(PIN_SEL, 1);
    gpio_set_output_en(PIN_SEL, 1);
    gpio_set_gpio_en(PIN_SEL);
}

#endif