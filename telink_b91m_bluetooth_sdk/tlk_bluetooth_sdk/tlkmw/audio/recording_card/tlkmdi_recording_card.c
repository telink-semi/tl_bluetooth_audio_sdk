/********************************************************************************************************
 * @file    tlkmdi_recording_card.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmdi_recording_card_handler.h"
#include "tlkmdi_recording_card.h"

#if TLKMW_RECORDING_CARD_EN
tlkmdi_recording_card_t s_recording_card_env = {0};

/**
 * @brief       This function is used to initialize the recording card module.
 * @param       none.
 * @return      TLK_ENONE if success, otherwise error code.
 */
int tlkmdi_recording_card_init(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    s_recording_card_env.gain = 30;
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    s_recording_card_env.gain = 18;
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function checks if the recording card module is busy.
 * @param       none.
 * @return      true if busy, false otherwise.
 */
bool tlkmdi_recording_card_is_busy(void)
{
    return s_recording_card_env.busy;
}

/**
 * @brief       This function enables or disables the nn algorithm.
 * @param[in]   en - enable flag.
 * @return      none.
 */
static void tlkmdi_recording_card_nn_enable_core(bool en)
{
#if (TLKALG_NN_NS_VAD_ENABLE)
    if (en) {
        tlksys_pm_reqHighClock(3, TLKSYS_CLOCK_CHN_NN_ALG);
    } else {
        tlksys_pm_cancelHighClockReq(TLKSYS_CLOCK_CHN_NN_ALG);
    }
#elif (TLK_MW_DSP_COMM_ENABLE && TLKALG_BBF_ENABLE)
    if (en) {
        tlksys_pm_reqHighClock(2, TLKSYS_CLOCK_CHN_NN_ALG);
    } else {
        tlksys_pm_cancelHighClockReq(TLKSYS_CLOCK_CHN_NN_ALG);
    }
#else
    (void)en;
#endif

#if TLK_MW_DSP_COMM_ENABLE
    if (!en) {
        ipc_msg_register_data_process_done_cb(NULL, VAD_NN_NS, NN_NS_16K_20MS_ID);
        tlkmw_dsp_pause();
    } else {
        tlkmdi_recording_card_clean_dsp_queue();
        ipc_msg_register_data_process_done_cb(tlkmdi_recording_card_dsp_msg_process_callback, VAD_NN_NS, NN_NS_16K_20MS_ID);
        tlkmw_dsp_resume();
    }
#endif
}

/**
 * @brief       This function switches the recording card status.
 * @param[in]   handle - the connection handle.
 * @param[in]   status - the switch status.
 * @return      true if successful, false otherwise.
 */
bool tlkmdi_recording_card_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(0xffffffff, "<test>", "tlkmdi_recording_card_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        s_recording_card_env.first_frame = true;
        tlkmdi_recording_card_alg_init();
#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)

        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
        tlkmdi_recording_card_gain_set(s_recording_card_env.gain);
        tlkmdi_recording_card_fifo_irq_init(RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int));
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
        tlkmdi_recording_card_gain_set(s_recording_card_env.gain);
        tlkmdi_recording_card_fifo_irq_init(RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int));
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
        tlkmdi_recording_card_gain_set(s_recording_card_env.gain);
        tlkmdi_recording_card_fifo_irq_init(RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_int));
#else
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
        tlkmdi_recording_card_gain_set(s_recording_card_env.gain);
        tlkmdi_recording_card_fifo_irq_init(RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, tlkmdi_recording_card_fifo_irq_handler_func);
        tlksys_enter_critical();
        s_recording_card_env.busy = true;
        if (s_recording_card_env.nn_alg_en) {
            tlkmdi_recording_card_nn_enable_core(true);
        }
        tlksys_leave_critical();

    } else {
        tlkmdi_recording_card_alg_deinit();
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
        audio_fifo_irq_dis(AUDIO_RX_FIFO0);
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
        audio_rxfifo_dis(TLKDRV_CODEC_MIC_FIFO);
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, NULL);

        tlksys_enter_critical();
        s_recording_card_env.busy = false;
        if (s_recording_card_env.nn_alg_en) {
            tlkmdi_recording_card_nn_enable_core(false);
        }
        tlksys_leave_critical();
    }
    return 1;
}

/**
 * @brief       This function enables or disables the recording card algorithm.
 * @param[in]   en - enable flag.
 * @return      none.
 */
void tlkmdi_recording_card_alg_en(bool en)
{
    tlksys_enter_critical();
    if (en == s_recording_card_env.nn_alg_en) {
        tlksys_leave_critical();
        return;
    }
    s_recording_card_env.nn_alg_en    = en;
    s_recording_card_env.alg_stat_chg = 1;
    if (!s_recording_card_env.busy) {
        tlksys_leave_critical();
        return;
    }
    tlkmdi_recording_card_nn_enable_core(en);
    tlksys_leave_critical();
}

/**
 * @brief       This function sets the gain for the recording card.
 * @param[in]   gain - the gain value to set.
 * @return      none.
 */
void tlkmdi_recording_card_gain_set(unsigned char gain)
{
    tlksys_enter_critical();
    if (gain != s_recording_card_env.gain) {
        tlk_printf("tlkmdi_recording_card_gain_set:%d-->%d", gain, s_recording_card_env.gain);
        tlkdrv_icodec_dmic_d_gain(gain);
        s_recording_card_env.gain = gain;
    }
    tlksys_leave_critical();
}

#if ((TLKALG_GET_AUDIO_DATA_EN) && (TLKMW_RECORDING_CARD_EN))
#define SPI_ENABLE 1
#define SPI_CLK    24000000

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = GPIO_FC_PA6,
    .spi_clk_pin      = GPIO_FC_PA5,
    .spi_mosi_io0_pin = GPIO_FC_PB0,
    .spi_miso_io1_pin = 0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
};
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
lspi_pin_config_t lspi_pin_config = {
    .spi_csn_pin      = LSPI_CSN_PE6_PIN,
    .spi_clk_pin      = LSPI_CLK_PE1_PIN,
    .spi_mosi_io0_pin = LSPI_MOSI_IO0_PE2_PIN,
    .spi_miso_io1_pin = 0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0, //quad  mode is required, otherwise it is NONE_PIN.
};
#endif

/**
 * @brief       This function serves to initialize spi log for audio debug.
 * @param[in]   none
 * @return      none
 */
static void spi_data_output_init(void)
{
    uint8_t send_data[] = {0, 1, 2, 3, 4, 5, 6, 7};

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    spi_master_init(0, SRC_CLK_XTAL_48M, SRC_CLK_XTAL_48M / SPI_CLK, SPI_MODE0);
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    spi_master_init(0, sys_clk.pll_clk * 1000000 / SPI_CLK, SPI_MODE0);
#endif

    spi_set_tx_dma_config(0, tlkhal_dma_malloc());
    lspi_set_pin(&lspi_pin_config);
    spi_master_config(0, SPI_NORMAL);

    spi_master_write_dma(0, send_data, 8);
    while (spi_is_busy(0));
}

/**
 * @brief       This function serves to initialize audio debug module.
 * @param[in]   none
 * @return      none
 */
void tlkmdi_audio_spi_data_init(void)
{
    spi_data_output_init();
}
#endif

#endif // #if (TLK_MW_AUDIO_ENABLE)
