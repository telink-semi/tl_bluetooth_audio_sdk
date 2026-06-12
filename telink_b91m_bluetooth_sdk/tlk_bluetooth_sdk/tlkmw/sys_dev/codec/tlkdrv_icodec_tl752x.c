/********************************************************************************************************
 * @file    tlkdrv_icodec_tl752x.c
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
#include "core/mcu_type.h"
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "../../common/tlkaudio_debug_data.h"
#if (TLKDRV_CODEC_ICODEC_ENABLE)
#include "tlkdrv_icodec_tl752x.h"
#include "drivers.h"

static bool tlkdrv_icodec_isOpen(uint8_t subDev);
static int  tlkdrv_icodec_init(uint8_t subDev);
static int  tlkdrv_icodec_open(uint8_t subDev);
static int  tlkdrv_icodec_close(uint8_t subDev);
static int  tlkdrv_icodec_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);

static int tlkdrv_icodec_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_mic_enable(bool enMic);
static int tlkdrv_icodec_spk_enable(bool enSpk);

//static int  tlkdrv_icodec_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk);

extern uint16_t             gTlkDrvCodecSpkBuffLen;
extern uint16_t             gTlkDrvCodecMicBuffLen;
extern uint8_t             *gpTlkDrvCodecSpkBuffer;
extern uint8_t             *gpTlkDrvCodecMicBuffer;
const tlkdrv_codec_modinf_t gcTlkDrvIcodecInf = {
    .IsOpen = tlkdrv_icodec_isOpen,
    .Init   = tlkdrv_icodec_init,
    .Open   = tlkdrv_icodec_open,
    .Close  = tlkdrv_icodec_close,
    .Config = tlkdrv_icodec_config,
};


static tlkdrv_icodec_t sTlkDrvIcodecCtrl = {0};
//static tlkdrv_icodec_status_t sTlkDrvIcodecAdcStat = {0};
//static tlkdrv_icodec_status_t sTlkDrvIcodecDacStat = {0};

uint32_t tl751x_codec_hd_freq_table[8]     = {16000, 44100, 48000, 96000, 192000, 384000, 768000, 0xffffffff};
uint8_t  tl751x_codec_hd_bitdepth_table[4] = {16, 24, 0xff};
uint8_t  tl751x_codec_hd_channal_table[4]  = {1, 2, 3, 0xff};
uint8_t  tl751x_codec_cur_power_mode       = 0;
uint32_t st_audio_sys_clk;

extern unsigned int frac_pll_freq;

typedef enum
{
    AUDIO_BUF_EMPTY,
    AUDIO_BUF_FULL,
} e_audio_buf_stat;

typedef struct
{
    uint32_t         bufAddr;
    e_audio_buf_stat stat;
} block_buf_t;

// static volatile uint32_t rx_index = 0;
static volatile uint32_t wx_index = 0;

static volatile uint32_t codec_tx_index       = 0;
volatile uint16_t        g_codec_fifo_rptr    = 0;
volatile uint16_t        g_codec_micfifo_wptr = 0;

#define CODEC_BUF_BLOCK_SIZE 64

#if AUDIO_PATH_24BITS_EN
uint8_t codec_data_width = 4;
#else
uint8_t codec_data_width = 2;
#endif
//extern signed int sin_48k_stereo_24bit[] __attribute__((aligned(4)));
#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
uint32_t audio_buf[DATA_BUFF_LEN];
uint32_t audio_adc_buf[DATA_BUFF_LEN];
#else
_attribute_iram_data_ uint32_t audio_buf[DATA_BUFF_LEN];
_attribute_iram_data_ uint32_t audio_adc_buf[DATA_BUFF_LEN];
#endif
uint32_t    zero_buf[DATA_BLOCK_SIZE];
block_buf_t audio_block_buf[DATA_BLOCK_NUM];
block_buf_t audio_adc_block_buf[DATA_BLOCK_NUM];

dmac_handle_t audio_adc_dmac_handle;
dmac_handle_t audio_dac_dmac_handle;

dmac_llp_node_t adc_dmac_llp_config;
dmac_llp_node_t dac_dmac_llp_config;

/**
 * @brief DAC DMA transmission complete callback function
 * @param[in] arg - Callback argument
 * @return None
 */
void codec_dac_dma_tx_complete_cb(void *arg)
{
    (void)arg;
#if (CODEC_MODE_SEL == BUFF_TO_LINE_OUT_CASE)
    //drv_audio_dac_dma_tx((uint32_t *)((unsigned int)sin_48k_stereo_24bit + DLM_BUS_READ_OFFSET), 128);

    codec_int *p_codec_buf = (codec_int *)gpTlkDrvCodecSpkBuffer;
    codec_int *data        = (codec_int *)audio_block_buf[codec_tx_index].bufAddr;

    for (uint8_t i = 0; i < DATA_BLOCK_SIZE / (codec_data_width / 2); i++) {
        *data++ = p_codec_buf[g_codec_fifo_rptr++];
        if (g_codec_fifo_rptr == CODEC_SPK_FIFO_SAMPLES) {
            g_codec_fifo_rptr = 0;
        }
    }

    drv_audio_dac_dma_tx((uint32_t *)(audio_block_buf[codec_tx_index].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);

    audio_block_buf[codec_tx_index].stat = AUDIO_BUF_EMPTY;
    codec_tx_index++;
    if (codec_tx_index == DATA_BLOCK_NUM) {
        codec_tx_index = 0;
    }

#else
    //soc_printf("rx_index %d!\n", rx_index);
    if (audio_block_buf[rx_index].stat == AUDIO_BUF_EMPTY) {
        //soc_printf("audio buff empty!\n");
        drv_audio_dac_dma_tx((uint32_t *)((uint32_t)zero_buf + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);
        return;
    }
    drv_audio_dac_dma_tx((uint32_t *)(audio_block_buf[rx_index].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);
    audio_block_buf[rx_index].stat = AUDIO_BUF_EMPTY;
    rx_index++;
    if (rx_index == DATA_BLOCK_NUM) {
        rx_index = 0;
    }
#endif
}

/**
 * @brief ADC DMA reception complete callback function
 * @param[in] arg - Callback argument
 * @return None
 */
void codec_adc_dma_rx_complete_cb(void *arg)
{
    (void)arg;
    codec_int *record_buff = (codec_int *)gpTlkDrvCodecMicBuffer;

    if (audio_adc_block_buf[wx_index].stat == AUDIO_BUF_FULL) {
        //    	tlkapi_trace(0xFFFFFFFF, "[tl752x_codec]", "audio buff full!");
    }
    audio_adc_block_buf[wx_index].stat = AUDIO_BUF_FULL;

    wx_index++;
    if (wx_index == DATA_BLOCK_NUM) {
        wx_index = 0;
    }

    // gpio_set_high_level(GPIO_PB11);
    // gpio_set_low_level(GPIO_PB11);

    codec_int *data = (codec_int *)audio_adc_block_buf[wx_index].bufAddr;

    drv_audio_adc_dma_rx((uint32_t *)(audio_adc_block_buf[wx_index].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);

    for (int i = 0; i < DATA_BLOCK_SIZE / (codec_data_width / 2); i++) {
        record_buff[g_codec_micfifo_wptr++] = *data++;
        if (g_codec_micfifo_wptr == CODEC_MIC_FIFO_SAMPLES) {
            g_codec_micfifo_wptr = 0;
        }
    }
}

/**
 * @brief Initialize audio buffer
 * @param[in] block_base - Base address of block buffer
 * @param[in] buf_base - Base address of buffer
 * @param[in] block_size - Size of each block
 * @param[in] len - Length of buffer
 * @return None
 */
void audio_buf_init(block_buf_t *block_base, uint32_t buf_base, uint32_t block_size, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        block_base[i].bufAddr = buf_base + block_size * i;
        block_base[i].stat    = AUDIO_BUF_EMPTY;
    }
}

/**
 * @brief Check if internal codec sub-device is open
 * @param[in] subDev - Sub-device to check
 * @return True if sub-device is open, false otherwise
 */
static bool tlkdrv_icodec_isOpen(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return false;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen) {
            return true;
        } else {
            return false;
        }
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

/**
 * @brief Initialize internal codec
 * @param[in] subDev - Sub-device to initialize
 * @return Operation result status
 */
static int tlkdrv_icodec_init(uint8_t subDev)
{
    (void)subDev;
    if (!sTlkDrvIcodecCtrl.isInit) {
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_icodec_t)));
        memset(&sTlkDrvIcodecCtrl, 0, sizeof(tlkdrv_icodec_t));
    }

    //    sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
    //    if (audio_hd_anc_enable)
    //    {
    //    	sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_184P32M;
    //    }
    //
    //    clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);

    sTlkDrvIcodecCtrl.isInit = true;

    return TLK_ENONE;
}

/**
 * @brief Initialize TL752X internal codec hardware
 * @param None
 * @return None
 */
void tlkdrv_tl752x_codec_hd_init(void)
{
    audio_buf_init(audio_block_buf, (uint32_t)audio_buf, DATA_BLOCK_SIZE * sizeof(int), DATA_BLOCK_NUM);
    audio_buf_init(audio_adc_block_buf, (uint32_t)audio_adc_buf, DATA_BLOCK_SIZE * sizeof(int), DATA_BLOCK_NUM);

    uint32_t         pll_freq   = FRACPLL_OUT_FREQ;
    cpr_mclk1_init_t mclk1_init = {0};
    if (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate == 44100) {
        pll_freq        = 282240000UL;
        mclk1_init.freq = 5644800;
    } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate == 48000) {
        pll_freq        = 196608000UL;
        mclk1_init.freq = 6144000;
    }

    tlkapi_trace(0xFFFFFFFF, "[tl752x_codec]", "pll_freq %d", pll_freq);

    frac_pll_freq = pll_freq;

    drv_cpr_enable_fracpll(pll_freq);

    mclk1_init.source = DRV_CPR_MCLK1_SOURCE_DIVIDED_PLL1;

    drv_cpr_config_mclk1(&mclk1_init);

#if 0
	//drv_cpr_enable_fracpll(FRACPLL_OUT_FREQ);
	cpr_mclk1_init_t mclk1_init;
	mclk1_init.source = DRV_CPR_MCLK1_SOURCE_DIVIDED_PLL0;
	mclk1_init.freq = 6144000;// FRACPLL_OUT_FREQ;
	drv_cpr_config_mclk1(&mclk1_init);
#endif

    core_interrupt_enable();

    drv_cpr_audio_system_powerup();
    drv_soc_codec_msp_init();
    __DRV_CPR_CPU2_DMA_CLK_ENABLE();
    drv_hw_int_enable(CPU2_DMA_IRQn);
    plic_set_priority(CPU2_DMA_IRQn, 2);

#if 0
    sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
    if (audio_hd_anc_enable)
    {
    	sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_184P32M;
    }

    clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
    audio_init(sTlkDrvIcodecCtrl.sys_mclk);                                     /* must configured first. */

    if (CODEC_INPUT_MODE == CODEC_INPUT_DMIC) {
        audio_codec0_power_on(AUDIO_CODEC0_DIGITAL_AND_DAC, AUDIO_CODEC0_1P8V); /* power on codec digital and dac. */
    } else {
        audio_codec0_power_on(AUDIO_CODEC0_ADC_AND_DAC, AUDIO_CODEC0_1P8V);     /* power on adc and dac. */
    }
#endif //#if 0
}

/**
 * @brief Open internal codec sub-device
 * @param[in] subDev - Sub-device to open
 * @return Operation result status
 */
static int tlkdrv_icodec_open(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }

    tlkdrv_tl752x_codec_hd_init();
    tlkapi_send_string_data(APP_LOG_EN, " Audio cold start-up", NULL, 0);

    if ((subDev & TLKDRV_CODEC_SUBDEV_BOTH)) {
        if (((sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate == 44100) || (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate == 44100)) &&
            (sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate != sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate)) {
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_open(Err):Codec NO Support!!!", NULL, 0);
            return -TLK_ENOSUPPORT;
        }
    }

    ///MIC enable
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }

        sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn = true;
        tlkdrv_icodec_mic_enable(true);
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = true;
    }

    ///SPK enable
    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }

        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn = true;
        tlkdrv_icodec_spk_enable(true);
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = true;
    }

    return TLK_ENONE;
}

/**
 * @brief Close internal codec sub-device
 * @param[in] subDev - Sub-device to close
 * @return Operation result status
 */
static int tlkdrv_icodec_close(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }
    //#if 0
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn   = false;
//        audio_codec0_set_input_snr_opt(AUDIO_AMIC_ADC_A1_A2,0);
#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        //	        audio_codec0_set_micbias(AUDIO_ADC_CH_0 | AUDIO_ADC_CH_4, 0);
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_codec0_dmic_clk_en(AUDIO_DMIC_ADC_A1_A2, 0);
        audio_codec0_reset_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);

        audio_codec0_adc_power_down(AUDIO_DMIC_ADC_A1_A2);
        reg_audio_codec0_cr_vic = reg_audio_codec0_cr_vic | 0x0f;
        BM_CLR(reg_audio_clk_en_0, FLD_CLK_CODEC0_EN); /* After clock disable, cannot access codec registers. */

        pm_audio_pll_power_down();
        g_audio_pll_is_used = 0;
#endif
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = false;
        //		audio_dac_mute();
        drv_audio_dac_set_autx_gain(AUDIO_DAC_CH_STEREO, AUDIO_DAC_AUTX_GAIN_N_18_06DB);
        drv_audio_dac_set_dig_gain(AUDIO_DAC_CH_STEREO, AUDIO_DAC_DIG_GAIN_P_24DB);

        drv_audio_dac_close(AUDIO_DAC_CH_STEREO);
        drv_audio_dac_stop(AUDIO_DAC_CH_STEREO);

        //		audio_codec0_set_output_snr_opt(0);
    }
    //#endif	//#if 0
    return TLK_ENONE;
}

/**
 * @brief Configure internal codec sub-device
 * @param[in] subDev - Sub-device to configure
 * @param[in] opcode - Configuration operation code
 * @param[in] param0 - First parameter
 * @param[in] param1 - Second parameter
 * @return Operation result status
 */
static int tlkdrv_icodec_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1)
{
    int ret = -TLK_ENOSUPPORT;
    switch (opcode) {
    case TLKDRV_CODEC_OPCODE_SET_CHANNEL: // param:[uint8_t]chnCnt-1/2/3
        ret = tlkdrv_icodec_setChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_CHANNEL:
        ret = tlkdrv_icodec_getChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH: // param:[uint8_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_icodec_setBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH:
        ret = tlkdrv_icodec_getBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE: // param:[uint32_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_icodec_setSampleRateDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE:
        ret = tlkdrv_icodec_getSampleRateDeal(subDev, param0, param1);
        break;
    }
    return ret;
}

/**
 * @brief Check if channel configuration is supported
 * @param[in] value - Channel configuration value to check
 * @return Operation result status
 */
static int icodec_channal_check(uint8_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_channal_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_channal_table[i]) {
            break;
        } else if (tl751x_codec_hd_channal_table[i] == 0xff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Set channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Channel parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_icodec_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param1;
    uint8_t channel = param0 & 0xFF;

    int ret = icodec_channal_check(channel);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.Channel = channel;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.Channel = channel;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.Channel = channel;
        sTlkDrvIcodecCtrl.codec_mic_cfg.Channel = channel;
    }
    return TLK_ENONE;
}

/**
 * @brief Get channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current channel configuration
 */
static int tlkdrv_icodec_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.Channel;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.Channel;
    }
}

/**
 * @brief Check if bit depth is supported
 * @param[in] value - Bit depth value to check
 * @return Operation result status
 */
static int icodec_bitdepth_check(uint8_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_bitdepth_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_bitdepth_table[i]) {
            break;
        } else if (tl751x_codec_hd_bitdepth_table[i] == 0xff) {
            return -TLK_ENOSUPPORT;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Set bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_icodec_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param1;
    uint8_t bitDepth = param0 & 0xFF;


    int ret = icodec_bitdepth_check(bitDepth);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth = bitDepth;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth = bitDepth;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth = bitDepth;
        sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth = bitDepth;
    }
    return TLK_ENONE;
}

/**
 * @brief Get bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current bit depth configuration
 */
static int tlkdrv_icodec_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth;
    }
}

/**
 * @brief Check if frequency is supported
 * @param[in] value - Frequency value to check
 * @return Operation result status
 */
static int icodec_freq_check(uint32_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_freq_table[i]) {
            break;
        } else if (tl751x_codec_hd_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Set sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_icodec_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param1;
    uint32_t sampleRate = param0;

    int ret = icodec_freq_check(sampleRate);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate = sampleRate;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate = sampleRate;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate = sampleRate;
        sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate = sampleRate;
    }
    return TLK_ENONE;
}

/**
 * @brief Get sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current sample rate configuration
 */
static int tlkdrv_icodec_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate;
    }
}

/**
 * @brief Convert audio ADC sample rate to index
 * @param[in] audio_rate - Audio sample rate
 * @return Index corresponding to the sample rate
 */
static inline int audio_adc_sample_rate_to_index(int audio_rate)
{
    int rate_index;
#if 0
    rate_index = audio_rate == 16000 ? AUDIO_16K :
                     audio_rate == 48000 ? AUDIO_48K :
                    		 	 	 	   0xffffffff;
#else
    if (audio_rate == 8000) {
        rate_index = AUDIO_ADC_FS_8K;
    } else if (audio_rate == 16000) {
        rate_index = AUDIO_ADC_FS_16K;
    } else if (audio_rate == 24000) {
        rate_index = AUDIO_ADC_FS_24K;
    } else if (audio_rate == 32000) {
        rate_index = AUDIO_ADC_FS_32K;
    } else if (audio_rate == 48000) {
        rate_index = AUDIO_ADC_FS_48K;
    } else if (audio_rate == 96000) {
        rate_index = AUDIO_ADC_FS_96K;
    } else if (audio_rate == 192000) {
        rate_index = AUDIO_ADC_FS_192K;
    } else if (audio_rate == 384000) {
        rate_index = AUDIO_ADC_FS_384K;
    } else if (audio_rate == 768000) {
        rate_index = AUDIO_ADC_FS_768K;
    } else if (audio_rate == 44100) {
        rate_index = AUDIO_ADC_FS_44_1K;
    } else {
        rate_index = 0x00;
    }

#endif
    return rate_index;
}

/**
 * @brief Convert audio DAC sample rate to index
 * @param[in] audio_rate - Audio sample rate
 * @return Index corresponding to the sample rate
 */
static inline int audio_dac_sample_rate_to_index(int audio_rate)
{
    int rate_index;
#if 0
    rate_index = audio_rate == 16000 ? AUDIO_16K :
                     audio_rate == 48000 ? AUDIO_48K :
                    		 	 	 	   0xffffffff;
#else
    if (audio_rate == 8000) {
        rate_index = AUDIO_DAC_FS_8K;
    } else if (audio_rate == 16000) {
        rate_index = AUDIO_DAC_FS_16K;
    } else if (audio_rate == 24000) {
        rate_index = AUDIO_DAC_FS_24K;
    } else if (audio_rate == 32000) {
        rate_index = AUDIO_DAC_FS_32K;
    } else if (audio_rate == 48000) {
        rate_index = AUDIO_DAC_FS_48K;
    } else if (audio_rate == 96000) {
        rate_index = AUDIO_DAC_FS_96K;
    } else if (audio_rate == 192000) {
        rate_index = AUDIO_DAC_FS_192K;
    } else if (audio_rate == 384000) {
        rate_index = AUDIO_DAC_FS_384K;
    } else if (audio_rate == 768000) {
        rate_index = AUDIO_DAC_FS_768K;
    } else if (audio_rate == 44100) {
        rate_index = AUDIO_DAC_FS_44_1K;
    } else {
        rate_index = 0x00;
    }

#endif
    return rate_index;
}

/**
 * @brief Enable/disable microphone
 * @param[in] enMic - Enable flag
 * @return Operation result status
 */
static int tlkdrv_icodec_mic_enable(bool enMic)
{
    uint8_t micSrc    = 0xFF;
    uint8_t micDWdith = 0xFF;
    int     micSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_mic_enable:", &enMic, 1);

    if (enMic) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 16) {
            micDWdith = AUDIO_ADC_16BIT;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 24) {
            micDWdith = AUDIO_ADC_24BIT;
        }
        //        tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micDWdith> ",&micDWdith,1);

#if (CODEC_INPUT_MODE == CODEC_INPUT_LINEIN)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_LINEIN_ADC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_LINEIN_ADC_A2;
        } else {
            micSrc = AUDIO_LINEIN_ADC_A1;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_ADC_CH_0 | AUDIO_ADC_CH_4;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_ADC_CH_4;
        } else {
            micSrc = AUDIO_ADC_CH_0;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_DMIC_ADC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_DMIC_ADC_A2;
        } else {
            micSrc = AUDIO_DMIC_ADC_A1;
        }
        audio_codec0_set_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
#endif

        micSRate = audio_adc_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate);
        //      tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micSRate> ",&micSRate,4);

        //        if (gpTlkDrvCodecMicBuffer == NULL || gTlkDrvCodecMicBuffLen == 0 || micSrc == 0xFFU || micSRate == (int)0xffffffff || micDWdith == 0xFFU) {
        if (micSrc == 0xFFU || micSRate == (int)0xffffffff || micDWdith == 0xFFU) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "Param Err:gpTlkDrvCodecMicBuffer[%x],gTlkDrvCodecMicBuffLen[%x],micSrc[%x],micSRate[%x],micDWdith[%x]",
                         gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen, micSrc, micSRate, micDWdith);
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_Mic: enable_4 ", 0, 0);
            return -TLK_EPARAM;
        }

        //		tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
        //					 TLKDRV_CODEC_DBG_SIGN,
        //					 "tlkdrv_icodec_enable: <MIC> %d %d %d",
        //					 micSrc,
        //					 micSRate,
        //					 micDWdith);
    }

    //    if (enMic && gpTlkDrvCodecMicBuffer != NULL && gTlkDrvCodecMicBuffLen != 0) {
    if (enMic) {
#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        //    	audio_buf_init(audio_block_buf, (uint32_t)audio_buf, DATA_BLOCK_SIZE * sizeof(int), DATA_BLOCK_NUM);
        drv_audio_adc_init();

        audio_adc_cfg_t inputParam = {
            .fifo_src = AUDIO_ADC_FIFO_SRC_ADC,
            .fs       = micSRate,
            // .dig_gain = AUDIO_ADC_DIG_GAIN_0DB,
            // .pga_gain = AUDIO_ADC_PGA_GAIN_0DB,
        };
        drv_audio_adc_open(micSrc, &inputParam);

        drv_codec_adc_pga_gain_set(micSrc, AUDIO_ADC_PGA_GAIN_0DB);
        drv_codec_adc_dig_gain_set(micSrc, AUDIO_ADC_DIG_GAIN_0DB);

        audio_adc_data_format_t adc_data_format;
        memset(&adc_data_format, 0, sizeof(audio_adc_data_format_t));
        adc_data_format.hdma = &audio_adc_dmac_handle;
        adc_data_format.wide = micDWdith;
        if (micSrc == (AUDIO_ADC_CH_0 | AUDIO_ADC_CH_4)) {
            adc_data_format.ch_mode = AUDIO_ADC_MODE_STEREO;
        } else {
            adc_data_format.ch_mode        = AUDIO_ADC_MODE_MONO;
            adc_data_format.fifo_en_bitmap = AUDIO_ADC_CH_0;
        }
        adc_data_format.fifo_en_bitmap = micSrc;
        drv_audio_adc_set_dma_data_format(&adc_data_format);
#if AUDIO_ADC_DMA_LLP_EN
        drv_audio_adc_dmac_llp_head_set(&adc_dmac_llp_config);
        drv_audio_adc_dmac_llp_node_add(&adc_dmac_llp_config, &adc_dmac_llp_config, (uint32_t *)((uint32_t)gpTlkDrvCodecMicBuffer + DLM_BUS_READ_OFFSET),
                                        gTlkDrvCodecMicBuffLen / 4);
#else
        drv_audio_adc_rx_complete_cb_register(codec_adc_dma_rx_complete_cb);
        drv_audio_adc_dma_rx((uint32_t *)(audio_adc_block_buf[0].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);
#endif
        drv_audio_adc_start(micSrc);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
    }

    return TLK_ENONE;
}

/**
 * @brief Enable/disable speaker
 * @param[in] enSpk - Enable flag
 * @return Operation result status
 */
static int tlkdrv_icodec_spk_enable(bool enSpk)
{
    uint8_t spkSrc    = 0xFF;
    uint8_t spkDWdith = 0xFF;
    int     spkSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_spk_enable:", &enSpk, 1);
    if (enSpk) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 16) {
            spkDWdith = AUDIO_DAC_16BIT;
        } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 24) {
            spkDWdith = AUDIO_DAC_24BIT;
        }

        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkDWdith> ",&spkDWdith,1);

        if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x03) {
            spkSrc = AUDIO_DAC_CH_STEREO;
        } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x02) {
            spkSrc = AUDIO_DAC_CH_1;
        } else {
            spkSrc = AUDIO_DAC_CH_0;
        }

        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkSrc> ",&spkSrc,1);

        spkSRate = audio_dac_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate);

        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkSRate> ",&spkSRate,4);

        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable_2 ",0,0);
        //        if (gpTlkDrvCodecSpkBuffer == NULL || gTlkDrvCodecSpkBuffLen == 0 || spkSrc == 0xFFU || spkSRate == (int)0xffffffff || spkDWdith == 0xFFU) {
        if (spkSrc == 0xFFU || spkSRate == (int)0xffffffff || spkDWdith == 0xFFU) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "Param Err:gpTlkDrvCodecSpkBuffer[%x],gTlkDrvCodecSpkBuffLen[%x],spkSrc[%x],spkSRate[%x],spkDWdith[%x]",
                         gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen, spkSrc, spkSRate, spkDWdith);
            //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable_4 ",0,0);
            return -TLK_EPARAM;
        }

        //		tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
        //							 TLKDRV_CODEC_DBG_SIGN,
        //							 "tlkdrv_icodec_enable: <SPK>: %d %d %d",
        //							 spkSrc,
        //							 spkSRate,
        //							 spkDWdith);
    }

    //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable1 ",0,0);
    //    if (enSpk && gpTlkDrvCodecSpkBuffer != NULL && gTlkDrvCodecSpkBuffLen != 0) {
    if (enSpk) {
#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        //    	audio_buf_init(audio_block_buf, (uint32_t)audio_buf, DATA_BLOCK_SIZE * sizeof(int), DATA_BLOCK_NUM);
        drv_audio_dac_init();

        audio_dac_cfg_t outputParam = {
            .src       = AUDIO_DAC_SRC_PCM,
            .fs        = spkSRate,
            .autx_gain = AUDIO_DAC_AUTX_GAIN_0DB,
            .dig_gain  = AUDIO_DAC_DIG_GAIN_0DB,
        };

        drv_audio_dac_open(spkSrc, &outputParam);

        // drv_audio_dac_set_autx_gain(spkSrc, AUDIO_DAC_AUTX_GAIN_0DB);
        // drv_audio_dac_set_dig_gain(spkSrc, AUDIO_DAC_DIG_GAIN_0DB);

        audio_dac_data_format_t dac_data_format;
        memset(&dac_data_format, 0, sizeof(audio_dac_data_format_t));
        dac_data_format.hdma = &audio_dac_dmac_handle;
        if (spkSrc == AUDIO_DAC_CH_STEREO) {
            dac_data_format.ch_mode = AUDIO_DAC_MODE_STEREO;
        } else {
            dac_data_format.ch_mode = AUDIO_DAC_MODE_MONO;
        }
        dac_data_format.wide           = spkDWdith;
        dac_data_format.fifo_en_bitmap = spkSrc;
        drv_audio_dac_set_dma_data_format(&dac_data_format);

#if (CODEC_MODE_SEL == BUFF_TO_LINE_OUT_CASE)
        //drv_audio_dac_dma_tx((uint32_t *)((unsigned int)sin_48k_stereo_24bit + DLM_BUS_READ_OFFSET), 128);

#if AUDIO_DAC_DMA_LLP_EN
        drv_audio_dac_dmac_llp_head_set(&dac_dmac_llp_config);
        drv_audio_dac_dmac_llp_node_add(&dac_dmac_llp_config, &dac_dmac_llp_config, (uint32_t *)((uint32_t)gpTlkDrvCodecSpkBuffer + DLM_BUS_READ_OFFSET),
                                        gTlkDrvCodecSpkBuffLen / 4);
#else
        drv_audio_dac_tx_complete_cb_register(codec_dac_dma_tx_complete_cb);
        drv_audio_dac_dma_tx((uint32_t *)(audio_block_buf[0].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);
#endif
#elif (CODEC_MODE_SEL == LINE_IN_TO_LINE_OUT_CASE)
        drv_audio_dac_dma_tx((uint32_t *)(audio_block_buf[0].bufAddr + DLM_BUS_READ_OFFSET), DATA_BLOCK_SIZE);
        //wait for adc write half full
        while (wx_index < DATA_BLOCK_NUM / 2) {};
#endif //#if (CODEC_MODE_SEL == BUFF_TO_LINE_OUT_CASE)
        drv_audio_dac_start(spkSrc);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
    }

    return TLK_ENONE;
}

#endif // #if (TLKDRV_CODEC_ICODEC_ENABLE)
#endif // #if (MCU_CORE_TYPE == MCU_CORE_TL752X)
