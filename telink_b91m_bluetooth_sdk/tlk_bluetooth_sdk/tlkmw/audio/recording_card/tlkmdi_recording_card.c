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
    tlkapi_printf(APP_LOG_EN, "tlkmdi_recording_card_init");
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    s_recording_card_env.gain = 24;
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    s_recording_card_env.gain = 12;
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
        tlksys_pm_reqHighClock(3,TLKSYS_CLOCK_CHN_NN_ALG);
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
        // gpio_set_high_level(GPIO_CHN0);

		s_recording_card_env.first_frame = true;
        tlkmdi_recording_card_alg_init();
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, 16000,0);
        tlkmdi_recording_card_gain_set(s_recording_card_env.gain);
        tlkmdi_recording_card_fifo_irq_init(RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, tlkmdi_recording_card_fifo_irq_handler_func);
        #if TLKALG_OPUS_DEC_ENABLE
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, 16000,0);
        #endif      
        tlksys_enter_critical();
        s_recording_card_env.busy = true;
        if (s_recording_card_env.nn_alg_en) {
            tlkmdi_recording_card_nn_enable_core(true);
        }
        tlksys_leave_critical();


        // gpio_set_low_level(GPIO_CHN0);
    } else {
        // gpio_set_high_level(GPIO_CHN0);


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
        // gpio_set_low_level(GPIO_CHN0);
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
    if(en == s_recording_card_env.nn_alg_en){
        tlksys_leave_critical();
        return;
    }
    s_recording_card_env.nn_alg_en = en;
    if(!s_recording_card_env.busy){
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
	if(gain != s_recording_card_env.gain){
		tlk_printf("tlkmdi_recording_card_gain_set:%d-->%d",gain,s_recording_card_env.gain);
		tlkdrv_icodec_dmic_d_gain(gain);
		s_recording_card_env.gain = gain;
	}
	tlksys_leave_critical();
}

#endif