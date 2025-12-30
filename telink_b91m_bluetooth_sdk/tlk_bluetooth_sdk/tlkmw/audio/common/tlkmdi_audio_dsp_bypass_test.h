/********************************************************************************************************
 * @file    tlkmdi_audio_dsp_bypass_test.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLKMW_AUDIO_COMMON_TLKMDI_AUDIO_DSP_BYPASS_TEST_H_
#define TLKMW_AUDIO_COMMON_TLKMDI_AUDIO_DSP_BYPASS_TEST_H_



#ifndef TLK_AUDIO_DSP_BYPASS_TEST_ENABLE
	#define TLK_AUDIO_DSP_BYPASS_TEST_ENABLE  0
#endif

#if TLK_AUDIO_DSP_BYPASS_TEST_ENABLE

/**
 * @brief   Audio timer interrupt handler
 * @param   None
 * @return  None
 */
void tlkmw_audio_timer_irq_handler(void);

/**
 * @brief   Open codec for test
 * @param   None
 * @return  None
 */
void tlkmw_audio_codec_test_open_codec(void);

/**
 * @brief   Codec test player
 * @param   mode - test mode
 * @return  None
 */
void tlkmw_audio_codec_test_player(uint8_t mode);

/**
 * @brief   Main function for codec test
 * @param   None
 * @return  None
 */
void tlkmw_audio_codec_test_main(void);

/**
 * @brief   Main function for dsp test
 * @param   None
 * @return  None
 */
void tlkmw_audio_dsp_test_main(void);

/**
 * @brief   Start codec test
 * @param   mode - test mode
 * @return  None
 */
void tlkmw_audio_start_codec_test(uint8_t mode);

/**
 * @brief   Stop codec test
 * @param   None
 * @return  None
 */
void tlkmw_audio_stop_codec_test(void);

#endif
#endif
