/********************************************************************************************************
 * @file    tlkmdi_audio_debug.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#if (TLK_MW_AUDIO_ENABLE)

#if TLK_SPI_DEBUG_ENABLE
#define SPI_ENABLE 1
#define SPI_CLK    24000000

#if SPI_ENABLE
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL752X)
#if (TLKMW_RECORDING_CARD_EN)
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = GPIO_FC_PA6,
    .spi_clk_pin      = GPIO_FC_PA5,
    .spi_mosi_io0_pin = GPIO_FC_PB0,
    .spi_miso_io1_pin = 0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
};

#else
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = GPIO_FC_PA0,
    .spi_clk_pin      = GPIO_FC_PA1,
    .spi_mosi_io0_pin = GPIO_FC_PA2,
    .spi_miso_io1_pin = 0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
};
#endif
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = LSPI_CSN_PE0_PIN,
    .spi_clk_pin      = LSPI_CLK_PE1_PIN,
    .spi_mosi_io0_pin = LSPI_MOSI_IO0_PE2_PIN,
    .spi_miso_io1_pin = LSPI_MISO_IO1_PE3_PIN, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = LSPI_IO2_PE4_PIN,      //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = LSPI_IO3_PE5_PIN,      //quad  mode is required, otherwise it is NONE_PIN.
};
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
#if (TLKMW_RECORDING_CARD_EN)
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = LSPI_CSN_PE6_PIN,
    .spi_clk_pin      = LSPI_CLK_PE1_PIN,
    .spi_mosi_io0_pin = LSPI_MOSI_IO0_PE2_PIN,
    .spi_miso_io1_pin = 0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
};
#else
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = LSPI_CSN_PE0_PIN,
    .spi_clk_pin      = LSPI_CLK_PE1_PIN,
    .spi_mosi_io0_pin = LSPI_MOSI_IO0_PE2_PIN,
    .spi_miso_io1_pin = LSPI_MISO_IO1_PE3_PIN, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0,                     //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0,                     //quad  mode is required, otherwise it is NONE_PIN.
};
#endif
#endif
#endif

/**
 * @brief       This function serves to initialize spi log for audio debug.
 * @param[in]   none
 * @return      none
 */
static void spi_log_init(void)
{
#if !SPI_ENABLE
    gpio_function_en(GPIO_CHN0);
    gpio_output_en(GPIO_CHN0);
    gpio_set_low_level(GPIO_CHN0);

    gpio_function_en(GPIO_CHN1);
    gpio_output_en(GPIO_CHN1);
    gpio_set_low_level(GPIO_CHN1);

    gpio_function_en(GPIO_CHN4);
    gpio_output_en(GPIO_CHN4);
    gpio_set_low_level(GPIO_CHN4);

    gpio_function_en(GPIO_CHN2);
    gpio_output_en(GPIO_CHN2);
    gpio_set_low_level(GPIO_CHN2);

    gpio_function_en(GPIO_CHN5);
    gpio_output_en(GPIO_CHN5);
    gpio_set_low_level(GPIO_CHN5);

    gpio_function_en(GPIO_CHN6);
    gpio_output_en(GPIO_CHN6);
    gpio_set_low_level(GPIO_CHN6);

    gpio_function_en(GPIO_CHN7);
    gpio_output_en(GPIO_CHN7);
    gpio_set_low_level(GPIO_CHN7);

    gpio_function_en(GPIO_CHN8);
    gpio_output_en(GPIO_CHN8);
    gpio_set_low_level(GPIO_CHN8);

    gpio_function_en(GPIO_CHN9);
    gpio_output_en(GPIO_CHN9);
    gpio_set_low_level(GPIO_CHN9);
#endif

#if SPI_ENABLE
    uint8_t send_data[] = {0, 1, 2, 3, 4, 5, 6, 7};
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL752X)
    spi_master_init(0, SRC_CLK_XTAL_48M, SRC_CLK_XTAL_48M / SPI_CLK, SPI_MODE0);
#else //elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    spi_master_init(0, sys_clk.pll_clk * 1000000 / SPI_CLK, SPI_MODE0);
#endif
    spi_set_tx_dma_config(0, tlkhal_dma_malloc());
    lspi_set_pin(&lspi_pin_config);
    spi_master_config(0, SPI_NORMAL);

    spi_master_write_dma(0, send_data, 8);
    while (spi_is_busy(0));
#endif
}
#endif

#if (TLK_I2S_DEBUG_ENABLE && (MCU_CORE_TYPE == MCU_CORE_TL721X))
signed int     AUDIO_BUFF[2048] __attribute__((aligned(4)));
unsigned short audio_i2s_48k_config[4][5] = {
    {8, 625, 0, 64, 64},
    {4, 625, 0, 32, 32},
    {1, 125, 0, 40, 40},
    {6, 625, 0, 48, 48},
};
// unsigned short audio_i2s_16k_config[4][5] =
//     {
//         {8, 1875, 0, 64, 64},
//         {4, 1875, 0, 32, 32},
//         {1, 375,  0, 40, 40},
//         {2, 625,  0, 48, 48},
// };
i2s_pin_config_t i2s_pin_config1 = {
    .bclk_pin       = GPIO_FC_PB0,
    .adc_lr_clk_pin = GPIO_FC_PB3,
    .dac_lr_clk_pin = GPIO_FC_PB4,
    .adc_dat_pin    = GPIO_FC_PB5,
    .dac_dat_pin    = GPIO_FC_PB6,
};
audio_i2s_config_t audio_i2s_config1 = {
    .i2s_select        = I2S2,
    .i2s_mode          = I2S_I2S_MODE,
    .pin_config        = &i2s_pin_config1,
    .data_width        = I2S_BIT_24_DATA,
    .master_slave_mode = I2S_AS_MASTER_EN,
    .sample_rate       = (unsigned short *)&audio_i2s_48k_config[0],
};
audio_i2s_input_output_t audio_i2s_output = {
    .i2s_select    = I2S2,
    .data_width    = I2S_BIT_24_DATA,
    .i2s_ch_sel    = I2S_CHANNEL_LEFT,
    .fifo_chn      = FIFO2,
    .dma_num       = 0,
    .data_buf      = AUDIO_BUFF,
    .data_buf_size = sizeof(AUDIO_BUFF),
};

/**
 * @brief       This function serves to initialize audio i2s for debug.
 * @param[in]   none
 * @return      none
 */
static void tlkmdi_audio_i2s_init(void)
{
    audio_init();
    /**** i2s config init ****/
    audio_i2s_config_init(&audio_i2s_config1);
    /**** i2s output init ****/
    audio_i2s_output.dma_num = tlkhal_dma_malloc();
    audio_i2s_output_init(&audio_i2s_output);
    /**** setting the codec clock of audio as the current mclk clock****/
    // audio_set_codec_clk_as_mclk(GPIO_FC_PC6); //only set i2s as master mclk=24*/2=12M
    /**** tx dma init ****/
    audio_tx_dma_chain_init(audio_i2s_output.fifo_chn, audio_i2s_output.dma_num, (unsigned short *)audio_i2s_output.data_buf, audio_i2s_output.data_buf_size);
    /**** i2s starts run****/
    audio_tx_dma_en(audio_i2s_output.dma_num);
    audio_i2s_clk_en(audio_i2s_config1.i2s_select);
}
#endif

/**
 * @brief       This function serves to print audio asynchronous information.
 * @param[in]   none
 * @return      none
 */
static void tlkmdi_audio_asyn_print(void)
{
#if AUDIO_TWS_MODE
    extern void bt_music_sync_loop(void);
    extern void bt_voice_loop_print(void);
    bt_music_sync_loop();
    bt_voice_loop_print();
#endif
}

/**
 * @brief       This function serves to initialize audio debug module.
 * @param[in]   none
 * @return      none
 */
void tlkmdi_audio_debug_init(void)
{
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_function_en(GPIO_PA0);
    gpio_output_en(GPIO_PA0);

    gpio_function_en(GPIO_PA1);
    gpio_output_en(GPIO_PA1);

    gpio_function_en(GPIO_PA2);
    gpio_output_en(GPIO_PA2);

    gpio_function_en(GPIO_PC0);
    gpio_output_en(GPIO_PC0);

    gpio_function_en(GPIO_PC1);
    gpio_output_en(GPIO_PC1);

    gpio_function_en(GPIO_PB0);
    gpio_output_en(GPIO_PB0);

    gpio_function_en(GPIO_PF4);
    gpio_output_en(GPIO_PF4);

    gpio_function_en(GPIO_PF5);
    gpio_output_en(GPIO_PF5);

    gpio_function_en(GPIO_PB5);
    gpio_output_en(GPIO_PB5);

    gpio_function_en(GPIO_PB6);
    gpio_output_en(GPIO_PB6);
#endif

#if TLK_SPI_DEBUG_ENABLE
    spi_log_init();
#endif
#if TLK_I2S_DEBUG_ENABLE
    tlkmdi_audio_i2s_init();
#endif
}

/**
 * @brief       This function serves to handle audio debug process.
 * @param[in]   none
 * @return      none
 */
void tlkmdi_audio_debug_handler(void)
{
    tlkmdi_audio_asyn_print();
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
