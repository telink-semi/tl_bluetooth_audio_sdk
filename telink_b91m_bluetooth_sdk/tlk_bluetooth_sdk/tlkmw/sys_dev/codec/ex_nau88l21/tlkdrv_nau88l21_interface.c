/********************************************************************************************************
 * @file    tlkdrv_nau88l21_interface.c
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
#include "../tlkdrv_i2s_master_tl721x.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_I2S_MASTER_ENABLE)
#if (I2S_CODEC_SEL == I2S_CODEC_CHIP_NAU88L21)

#include "tlkdrv_nau88l21_interface.h"

tlkdrv_nau8821_config_t nau8821_sys_clk_config;

extern unsigned short audio_i2s_16k_config[5];
extern unsigned short audio_i2s_8k_config[5];

void hal_nau8821_i2c_init(void)
{
    static uint8_t hal_nau8821_i2c_tx_dma = 0xff;
    static uint8_t hal_nau8821_i2c_rx_dma = 0xff;
    if (hal_nau8821_i2c_rx_dma == 0xff) {
        hal_nau8821_i2c_rx_dma = tlkhal_dma_malloc();
    }
    if (hal_nau8821_i2c_tx_dma == 0xff) {
        hal_nau8821_i2c_tx_dma = tlkhal_dma_malloc();
    }
    i2c_set_pin(I2C_GPIO_SDA_PIN, I2C_GPIO_SCL_PIN);
    i2c_set_tx_dma_config(hal_nau8821_i2c_tx_dma);
    i2c_set_rx_dma_config(hal_nau8821_i2c_rx_dma);
    i2c_master_init();
    i2c_set_master_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / (4 * I2C_CLK_SPEED)));
}

void hal_nau8821_i2c_write(uint8_t addr, uint16_t reg, uint16_t value)
{
    unsigned char write_buf[4];
    int           ref = 0;

    write_buf[0] = (reg & 0xff00) >> 8;
    write_buf[1] = (reg & 0x00ff);
    write_buf[2] = (value & 0xff00) >> 8;
    write_buf[3] = (value & 0x00ff);

    i2c_master_write_dma(addr, (unsigned char *)write_buf, 4);
    ref = (stimer_get_tick() - 2) | 1;
    while (i2c_master_busy() && !clock_time_exceed(ref, 50 * 1000));
}

uint8_t hal_nau8821_i2c_read(uint8_t addr, uint16_t reg, uint16_t *data, uint8_t len)
{
    (void)len;
    unsigned char write_buf[4];
    int           ref = 0;

    write_buf[0] = (reg & 0xff00) >> 8;
    write_buf[1] = (reg & 0x00ff);

    i2c_master_write_dma(addr, write_buf, 2);
    ref = (stimer_get_tick() - 2) | 1;
    while (i2c_master_busy() && !clock_time_exceed(ref, 50 * 1000));


    i2c_master_read_dma(addr, (unsigned char *)data, 2);
    ref = (stimer_get_tick() - 2) | 1;
    while (i2c_master_busy() && !clock_time_exceed(ref, 50 * 1000));

    return 0;
}

uint8_t hal_nau8821_i2c_update_bits(uint8_t addr, uint16_t reg, uint16_t mask, uint16_t value)
{
    //    int     ref = 0;
    uint16_t old_val;
    //    unsigned char  write_buf[4];

    regmap_read(addr, reg, &old_val, 2);

    old_val &= ~mask;
    old_val |= value;

    regmap_write(addr, reg, old_val);
    return 0;
}

i2s_pin_config_t hal_nau8821_i2s_pin_config = {
    //        .bclk_pin       = GPIO_FC_PB0,
    //        .adc_lr_clk_pin = GPIO_FC_PB1,
    //        .dac_lr_clk_pin = GPIO_FC_PB1,
    //        .adc_dat_pin    = GPIO_FC_PB2,
    //        .dac_dat_pin    = GPIO_FC_PB3,

    .bclk_pin = TLKDRV_I2S_MST_BCLIK_PIN,         .adc_lr_clk_pin = TLKDRV_I2S_MST_ADC_CLK_PIN, .adc_dat_pin = TLKDRV_I2S_MST_ADC_DAT_PIN,
    .dac_lr_clk_pin = TLKDRV_I2S_MST_DAC_CLK_PIN, .dac_dat_pin = TLKDRV_I2S_MST_DAC_DAT_PIN,
};


audio_i2s_config_t hal_nau8821_i2s_config = {
    .i2s_select        = I2S1,
    .i2s_mode          = I2S_I2S_MODE,
    .pin_config        = &hal_nau8821_i2s_pin_config,
    .data_width        = I2S_BIT_16_DATA,
    .master_slave_mode = I2S_AS_MASTER_EN,
    .sample_rate       = audio_i2s_16k_config,
};

audio_i2s_input_output_t hal_nau8821_i2s_input = {
    .i2s_select    = I2S1,
    .data_width    = I2S_BIT_16_DATA,
    .i2s_ch_sel    = I2S_CHANNEL_STEREO,
    .fifo_chn      = TLKDRV_CODEC_MIC_FIFO,
    .dma_num       = 6,
    .data_buf      = NULL,
    .data_buf_size = 0,
};

audio_i2s_input_output_t hal_nau8821_i2s_output = {
    .i2s_select    = I2S1,
    .data_width    = I2S_BIT_16_DATA,
    .i2s_ch_sel    = I2S_CHANNEL_STEREO,
    .fifo_chn      = TLKDRV_CODEC_SPK_FIFO,
    .dma_num       = 7,
    .data_buf      = NULL,
    .data_buf_size = 0,
};

void hal_nau8821_i2s_init(unsigned int *mic_buff, unsigned int *spk_buff, unsigned int mic_buff_size, unsigned int spk_buff_size)
{
    hal_nau8821_i2s_input.data_buf       = mic_buff;
    hal_nau8821_i2s_input.data_buf_size  = mic_buff_size;
    hal_nau8821_i2s_output.data_buf      = spk_buff;
    hal_nau8821_i2s_output.data_buf_size = spk_buff_size;

    /***initialize the audio module***/
    audio_init();
    /***Initialize i2s configuration***/
    audio_i2s_config_init(&hal_nau8821_i2s_config);
    /***initialize the configuration of the input path of the i2s***/
    audio_i2s_input_init(&hal_nau8821_i2s_input);
    /***initialize the configuration of the output path of the i2s***/
    audio_i2s_output_init(&hal_nau8821_i2s_output);
    /***setting the clock of i2s1 as the current i2s clock***/
    audio_set_i2s_clk_as_mclk(I2S0, GPIO_FC_PB4, 0x01, 0x14);

    /***configuring external codec-related registers via i2c***/
    /***initialize audio's rx dma channel***/
    audio_rx_dma_chain_init(hal_nau8821_i2s_input.fifo_chn, hal_nau8821_i2s_input.dma_num, (unsigned short *)hal_nau8821_i2s_input.data_buf, hal_nau8821_i2s_input.data_buf_size);
    /***initialize audio's tx dma channel***/
    audio_tx_dma_chain_init(hal_nau8821_i2s_output.fifo_chn, hal_nau8821_i2s_output.dma_num, (unsigned short *)hal_nau8821_i2s_output.data_buf,
                            hal_nau8821_i2s_output.data_buf_size);

    /***enable rx channel for i2s***/
    audio_rx_dma_en(hal_nau8821_i2s_input.dma_num);
    /***enable tx channel for i2s***/
    audio_tx_dma_en(hal_nau8821_i2s_output.dma_num);

    /***enable clocking of i2s***/
    audio_i2s_clk_en(hal_nau8821_i2s_config.i2s_select);
}

#endif
#endif
