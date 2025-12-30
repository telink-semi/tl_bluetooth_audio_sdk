/********************************************************************************************************
 * @file    ext_codec.c
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
#include "ext_codec.h"
#define TLK_CODEC_OUTPUT_DMA DMA3
#define TLK_CODEC_INPUT_DMA  DMA2

u32 gInputBufferLen  = 0;
u8 *gInputBuffer     = NULL;
u32 gInputReadOffset = 0;

u32 gOutputBufferLen   = 0;
u8 *gOutputBuffer      = NULL;
u32 gOutputWriteOffset = 0;

//#include "../../../algorithm/audio_alg/alg_audio_cfg.h"
#if (0)

/**
 *                                          i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
 *                                                  ||                  ||
 *  pll(192M default)------->div---->i2s_clk--->2 * div(div=0,bypass)--->blck----->div
 *                           ||                                         ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac(sampling rate)
 * For example:sampling rate=16K，i2s_clk_config[5]={ 8,125,6,64,64}, sampling rate=192M*(8/125)/(2*6)/64=16K
 *
 */
unsigned short audio_i2s_16k_config[5]  = {8, 125, 6, 64, 64};   /* sampling rate = 192M * (8 / 125) / (2 * 6) / 64   = 16K */
unsigned short audio_i2s_24k_config[5]  = {2, 125, 1, 64, 64};   /* sampling rate = 192M *(2 / 125) / (2 * 1) / 64   = 24k */
unsigned short audio_i2s_32k_config[5]  = {2, 125, 1, 48, 48};   /* sampling rate = 192M *(2 / 125) / (2 * 1) / 48   = 32k */
unsigned short audio_i2s_44k1_config[5] = {76, 235, 11, 64, 64}; /* sampling rate = 192M * (76 / 235) / (2 * 11) / 64 = 44.1K */
unsigned short audio_i2s_48k_config[5]  = {2, 125, 0, 64, 64};   /* sampling rate = 192M * (2 / 125) / 64             = 48K */

/* i2s config */
codec_0581_i2s_init_t i2s_init = {
#if ALG_HYBRID_ALG_EN
    .data_width  = I2S_BIT_16_DATA,
    .sample_rate = audio_i2s_16k_config,
#else
    .data_width  = I2S_BIT_16_DATA,
    .sample_rate = audio_i2s_24k_config,
#endif
};

/* i2s input config */
codec_0581_i2s_input_t i2s_input = {
#if ALG_HYBRID_ALG_EN
    .i2s_ch_sel = I2S_STEREO,
#else
    .i2s_ch_sel = I2S_MONO,
#endif
    .rx_dma_num     = TLK_CODEC_INPUT_DMA,
    .input_data_buf = NULL,
    .input_buf_size = 0,
};
/* i2s output config */
codec_0581_i2s_output_t i2s_output = {
    .tx_dma_num      = TLK_CODEC_OUTPUT_DMA,
    .output_data_buf = NULL,
    .output_buf_size = 0,
};


#ifndef CTL_AMIC_MICBIAS_0
#define CTL_AMIC_MICBIAS_0 GPIO_PB5
#endif

#ifndef CTL_AMIC_MICBIAS_1
#define CTL_AMIC_MICBIAS_1 GPIO_PB6
#endif

#ifndef CTL_AMIC_MICBIAS_2
#define CTL_AMIC_MICBIAS_2 GPIO_PB7
#endif

void audio_codec_adc_bias_init(void)
{
    gpio_function_en(CTL_AMIC_MICBIAS_0);
    gpio_output_en(CTL_AMIC_MICBIAS_0);
    gpio_input_dis(CTL_AMIC_MICBIAS_0);
    gpio_set_high_level(CTL_AMIC_MICBIAS_0);


    gpio_function_en(CTL_AMIC_MICBIAS_1);
    gpio_output_en(CTL_AMIC_MICBIAS_1);
    gpio_input_dis(CTL_AMIC_MICBIAS_1);
    gpio_set_high_level(CTL_AMIC_MICBIAS_1);


    gpio_function_en(CTL_AMIC_MICBIAS_2);
    gpio_output_en(CTL_AMIC_MICBIAS_2);
    gpio_input_dis(CTL_AMIC_MICBIAS_2);
    gpio_set_high_level(CTL_AMIC_MICBIAS_2);
}

void codec_base_init(void) //le audio support 16bit default
{
    /* audio power on */
    audio_power_on();
    /**** set i2s as master mclk = pll_clk(default 192M) * (1 / 16) = 12M ****/
    audio_i2s_set_mclk(GPIO_FC_PC3, 1, sys_clk.pll_clk / 12);

    /* I2S pin */
    i2s_pin_config_t i2s_pin_config = {
        .bclk_pin       = GPIO_FC_PB3,
        .adc_lr_clk_pin = GPIO_FC_PB1,
        .dac_lr_clk_pin = GPIO_FC_PB1,
        .adc_dat_pin    = GPIO_FC_PB2,
        .dac_dat_pin    = GPIO_FC_PB0,
    };

    audio_i2s_config_t audio_i2s_config = {
        .i2s_select        = I2S1,
        .i2s_mode          = I2S_I2S_MODE,
        .pin_config        = &i2s_pin_config,
        .data_width        = i2s_init.data_width,
        .master_slave_mode = I2S_AS_MASTER_EN,
        .sample_rate       = i2s_init.sample_rate,
    };

    /**** mcu i2s config init ****/
    audio_i2s_config_init(&audio_i2s_config);
    codec_0581_init();
    ///Initialize the input data path of ADI CODEC
    audio_codec_adc_bias_init();
}

void codec_config_input(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode)
{
    (void)freq;  //unused, remove warning
    (void)chanC; //unused, remove warning
    (void)mode;  //unused, remove warning
    /**** input path: adc -> asrco -> sap/i2s ****/
    /* adc */
    codec_0581_input_adc_config_t codec_0581_adc_config = {
        .adc_chnl = CODEC_ADC_CHNL_0,
        .adc_rate = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco */
    codec_0581_input_asrco_config_t codec_0581_asrco_config = {
        .asrco_in_fs      = CODEC_ASRC_FS_48K,
        .asrco_chnl       = CODEC_ASRCO_CHNL_0,
        .asrco_route_from = CODEC_ASRCO_ROUTE_ADC0,
    };
    /* sap */
    codec_0581_input_sap_config_t codec_0581_sap_config = {
        .slot_id        = CODEC_SAP_SLOT1_RIGHT, /* The i2s channel is mono, it must be CODEC_SAP_SLOT1_RIGHT. */
        .sap_route_from = CODEC_SAP_OUT_ROUTE_FROM_ASRCO0,
    };
    /* input config */
    codec_0581_input_t codec_0581_input_config = {
        .adc_config   = &codec_0581_adc_config,
        .asrco_config = &codec_0581_asrco_config,
        .fdec_config  = 0, /* this example fdec not used */
        .sap_config   = &codec_0581_sap_config,
    };

    audio_i2s_input_output_t audio_i2s_input = {
        .i2s_select    = I2S1,
        .data_width    = i2s_init.data_width,
        .i2s_ch_sel    = i2s_input.i2s_ch_sel,
        .fifo_chn      = FIFO1,
        .dma_num       = i2s_input.rx_dma_num,
        .data_buf      = i2s_input.input_data_buf,
        .data_buf_size = i2s_input.input_buf_size,
    };
    /* init input channel and dma */
    audio_i2s_input_init(&audio_i2s_input);

    /**** codec_0581 input path init ****/
    codec_0581_input_init(&codec_0581_input_config);

#if ALG_HYBRID_ALG_EN
    /**** input path: adc -> asrco -> sap/i2s ****/
    /* adc */
    codec_0581_input_adc_config_t codec_0581_adc_config2 = {
        .adc_chnl = CODEC_ADC_CHNL_2,
        .adc_rate = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco */
    codec_0581_input_asrco_config_t codec_0581_asrco_config2 = {
        .asrco_in_fs      = CODEC_ASRC_FS_48K,
        .asrco_chnl       = CODEC_ASRCO_CHNL_2,
        .asrco_route_from = CODEC_ASRCO_ROUTE_ADC2,
    };
    /* sap */
    codec_0581_input_sap_config_t codec_0581_sap_config2 = {
        .slot_id        = CODEC_SAP_SLOT0_LEFT, /* The i2s channel is mono, it must be CODEC_SAP_SLOT1_RIGHT. */
        .sap_route_from = CODEC_SAP_OUT_ROUTE_FROM_ASRCO2,
    };
    /* input config */
    codec_0581_input_t codec_0581_input_config2 = {
        .adc_config   = &codec_0581_adc_config2,
        .asrco_config = &codec_0581_asrco_config2,
        .fdec_config  = 0, /* this example fdec not used */
        .sap_config   = &codec_0581_sap_config2,
    };

    /**** codec_0581 input path init ****/
    codec_0581_input_init(&codec_0581_input_config2);
#endif
}

void codec_config_output(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode)
{
    (void)freq;  //unused, remove warning
    (void)chanC; //unused, remove warning
    (void)mode;  //unused, remove warning
    /**** output path: sap/i2s -> asrci -> dac ****/
    /* asrci */
    codec_0581_output_asrci_config_t codec_0581_asrci_config = {
        .asrci_out_fs = CODEC_ASRC_FS_48K,
    };
    /* dac */
    codec_0581_output_dac_config_t codec_0581_dac_config = {
        .dac_rate  = CODEC_DAC_SAMPLE_RATE_48KHz,
        .dac_input = CODEC_DAC_ROUTE_ASRCI0,
    };
    /* output config */
    codec_0581_output_t codec_0581_output_config = {
        .asrci_config = &codec_0581_asrci_config,
        .fint_config  = 0, /* this example fint not used */
        .dac_config   = &codec_0581_dac_config,
    };

    audio_i2s_input_output_t audio_i2s_output = {
        .i2s_select    = I2S1,
        .data_width    = i2s_init.data_width,
        .i2s_ch_sel    = I2S_MONO,
        .fifo_chn      = FIFO1,
        .dma_num       = i2s_output.tx_dma_num,
        .data_buf      = i2s_output.output_data_buf,
        .data_buf_size = i2s_output.output_buf_size,
    };
    /* init output channel and dma */
    audio_i2s_output_init(&audio_i2s_output);
    /**** codec_0581 output path init ****/
    codec_0581_output_init(&codec_0581_output_config);
}

void codec_input_enable(u8 *pBuffer, u16 bufferLen)
{
    gInputBufferLen          = bufferLen;
    gInputBuffer             = pBuffer;
    i2s_input.input_data_buf = pBuffer;
    i2s_input.input_buf_size = bufferLen;
    audio_rx_dma_chain_init(FIFO1, i2s_input.rx_dma_num, (unsigned short *)pBuffer, bufferLen);
    audio_rx_dma_en(i2s_input.rx_dma_num);
}

void codec_input_disable(void) {}

void codec_output_enable(u8 *pBuffer, u16 bufferLen)
{
    gOutputBufferLen           = bufferLen;
    gOutputBuffer              = pBuffer;
    i2s_output.output_data_buf = pBuffer;
    i2s_output.output_buf_size = bufferLen;
    audio_tx_dma_chain_init(FIFO1, i2s_output.tx_dma_num, (unsigned short *)pBuffer, bufferLen);
    audio_tx_dma_en(i2s_output.tx_dma_num);
}

void codec_output_disable(void)
{
    memset(gOutputBuffer, 0, gOutputBufferLen);
    dma_chn_dis(i2s_output.tx_dma_num);
}
#else

audio_codec_stream0_input_t audio_codec_input = {
    .input_src     = AMIC_STREAM0_STEREO,
    .sample_rate   = AUDIO_16K,
    .fifo_num      = FIFO0,
    .data_width    = CODEC_BIT_16_DATA,
    .dma_num       = TLK_CODEC_INPUT_DMA,
    .data_buf      = NULL,
    .data_buf_size = 0,
};

audio_codec_output_t audio_codec_output = {
    .output_src    = CODEC_DAC_STEREO,
    .sample_rate   = AUDIO_48K,
    .fifo_num      = FIFO0,
    .data_width    = CODEC_BIT_16_DATA,
    .dma_num       = TLK_CODEC_OUTPUT_DMA,
    .mode          = HP_MODE,
    .data_buf      = NULL,
    .data_buf_size = 0,
};

void codec_base_init(void) //le audio support 16bit default
{
    audio_codec_init();
}

void codec_close(void)
{
    audio_rx_dma_dis(audio_codec_input.dma_num);
    audio_tx_dma_dis(audio_codec_output.dma_num);
    audio_codec_dac_power_down();
    audio_codec_adc_power_down();
    gOutputBufferLen   = 0;
    gOutputBuffer      = NULL;
    gOutputWriteOffset = 0;

    gInputBufferLen  = 0;
    gInputBuffer     = NULL;
    gInputReadOffset = 0;
}

void codec_config_input(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode)
{
    if (mode == TLK_CODEC_MIC) {
        if (chanC == TLK_CODEC_1_CHANNEL) {
            audio_codec_input.input_src = AMIC_STREAM0_MONO_L;
        } else if (chanC == TLK_CODEC_2_CHANNEL) {
            audio_codec_input.input_src = AMIC_STREAM0_STEREO;
        }
    } else if (mode == TLK_CODEC_LINE) {
        if (chanC == TLK_CODEC_1_CHANNEL) {
            audio_codec_input.input_src = LINE_STREAM0_MONO_L;
        } else if (chanC == TLK_CODEC_2_CHANNEL) {
            audio_codec_input.input_src = LINE_STREAM0_STEREO;
        }
    } else if (mode == TLK_CODEC_I2S) {
    }
    audio_codec_input.sample_rate = freq + 1;
    audio_codec_stream0_input_init(&audio_codec_input);
}

void codec_config_output(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode)
{
    if (mode == TLK_CODEC_I2S) {
    } else {
        if (chanC == TLK_CODEC_1_CHANNEL) {
            audio_codec_output.output_src = CODEC_DAC_MONO_L;
        } else if (chanC == TLK_CODEC_2_CHANNEL) {
            audio_codec_output.output_src = CODEC_DAC_STEREO;
        }
        audio_codec_output.sample_rate = freq + 1;
        audio_codec_stream_output_init(&audio_codec_output);
    }
}

void codec_input_enable(u8 *pBuffer, u16 bufferLen)
{
    gInputBufferLen                 = bufferLen;
    gInputBuffer                    = pBuffer;
    audio_codec_input.data_buf      = pBuffer;
    audio_codec_input.data_buf_size = bufferLen;
    audio_rx_dma_chain_init(audio_codec_input.fifo_num, audio_codec_input.dma_num, (unsigned short *)audio_codec_input.data_buf, audio_codec_input.data_buf_size);
    audio_rx_dma_en(audio_codec_input.dma_num);
}

void codec_input_disable(void)
{
    audio_rx_dma_dis(audio_codec_input.dma_num);
    audio_codec_adc_power_down();
    gInputBufferLen  = 0;
    gInputBuffer     = NULL;
    gInputReadOffset = 0;
}

void codec_output_enable(u8 *pBuffer, u16 bufferLen)
{
    gOutputBufferLen                 = bufferLen;
    gOutputBuffer                    = pBuffer;
    audio_codec_output.data_buf      = pBuffer;
    audio_codec_output.data_buf_size = bufferLen;
    audio_tx_dma_chain_init(audio_codec_output.fifo_num, audio_codec_output.dma_num, (unsigned short *)audio_codec_output.data_buf, audio_codec_output.data_buf_size);
    audio_tx_dma_en(audio_codec_output.dma_num);
}

void codec_output_disable(void)
{
    memset(gOutputBuffer, 0, gOutputBufferLen);
    audio_tx_dma_dis(audio_codec_output.dma_num);
}
#endif


/*****************************************codec input******************************************/
int codec_input_getDataLen(void)
{
    u16 used;
    u32 wptr;
    u32 rptr;

    rptr = gInputReadOffset;
    wptr = (audio_get_rx_dma_wptr(TLK_CODEC_INPUT_DMA)) - ((u32)gInputBuffer);

    if (wptr >= rptr) {
        used = wptr - rptr;
    } else {
        used = gInputBufferLen + wptr - rptr;
    }
    return used;
}

u16 codec_get_InputBuffMaxlen(void)
{
    return gInputBufferLen;
}

u32 codec_get_InputWriteOffset(void)
{
    u32 wptr = (u16)((audio_get_rx_dma_wptr(TLK_CODEC_INPUT_DMA)) - ((u32)gInputBuffer));
    return wptr;
}

u32 codec_get_InputReadOffset(void)
{
    return gInputReadOffset;
}

void codec_set_InputReadOffset(u32 rptr)
{
    if (rptr > gInputBufferLen) {
        gInputReadOffset = rptr - gInputBufferLen;
    } else {
        gInputReadOffset = rptr;
    }
}

int codec_input_readData(u8 *pData, u16 pDataLen)
{
    u32 wptr;
    u32 rptr;
    u32 micDataLen;
    u32 offset;

    rptr = gInputReadOffset;
    wptr = (audio_get_rx_dma_wptr(TLK_CODEC_INPUT_DMA)) - ((u32)gInputBuffer);

    if (wptr >= rptr) {
        micDataLen = wptr - rptr;
    } else {
        micDataLen = gInputBufferLen + wptr - rptr;
    }

    if (micDataLen < pDataLen) {
        return false; //mic data not enough
    }

    if (pDataLen + rptr >= gInputBufferLen) {
        offset           = gInputBufferLen - rptr;
        gInputReadOffset = pDataLen + rptr - gInputBufferLen;
    } else {
        offset = pDataLen;
        gInputReadOffset += pDataLen;
    }
    memcpy(pData, gInputBuffer + rptr, offset);
    if (offset < pDataLen) {
        memcpy(pData + offset, gInputBuffer, pDataLen - offset);
    }

    return true;
}

/*****************************************codec output******************************************/
u16 codec_get_OutputBufferLen(void)
{
    return gOutputBufferLen;
}

int codec_output_getReadOffset(void)
{
    u32 readOffset = (audio_get_tx_dma_rptr(TLK_CODEC_OUTPUT_DMA)) - ((u32)gOutputBuffer);
    return readOffset;
}

int codec_output_getWriteOffset(void)
{
    return gOutputWriteOffset;
}

void codec_output_setWriteOffset(u32 offset)
{
    if (offset > gOutputBufferLen) {
        gOutputWriteOffset = offset - gOutputBufferLen;
    } else {
        gOutputWriteOffset = offset;
    }
}

int codec_output_writeData(u8 *pData, u16 pDataLen)
{
    u32 wptr;
    u16 offset;
    u8 *pBuffer = (u8 *)gOutputBuffer;

    wptr = gOutputWriteOffset;

    if (wptr + pDataLen > gOutputBufferLen) {
        offset = gOutputBufferLen - wptr;
    } else {
        offset = pDataLen;
    }
    memcpy(pBuffer + wptr, pData, offset);
    if (offset < pDataLen) {
        memcpy(pBuffer, pData + offset, pDataLen - offset);
    }

    wptr += pDataLen;
    if (wptr >= gOutputBufferLen) {
        wptr -= gOutputBufferLen;
    }
    gOutputWriteOffset = wptr;
    return true;
}
