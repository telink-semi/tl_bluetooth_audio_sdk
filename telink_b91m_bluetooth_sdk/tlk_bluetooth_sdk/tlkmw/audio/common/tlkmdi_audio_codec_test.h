/********************************************************************************************************
 * @file    tlkmdi_audio_codec_test.h
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
#ifndef TLKMW_AUDIO_COMMON_TLKMDI_AUDIO_CODEC_TEST_H_
#define TLKMW_AUDIO_COMMON_TLKMDI_AUDIO_CODEC_TEST_H_

#ifndef AUDIO_CODEC_TEST
	#define AUDIO_CODEC_TEST  1
#endif

#if AUDIO_CODEC_TEST
/**
 * @brief       Handle audio timer interrupt for codec test
 * @return      None
 */
void tlkmdi_audio_timer_irq_handler(void);

/**
 * @brief       Open codec for testing
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
void tlkmdi_audio_codec_test_open_codec(uint8_t mode);

/**
 * @brief       Play audio for codec testing
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
void tlkmdi_audio_codec_test_player(uint8_t mode);

/**
 * @brief       Main function for codec testing
 * @return      None
 */
void tlkmdi_codec_test_main(void);

/**
 * @brief       Start codec test mode
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
void tlkmdi_audio_start_codec_test(uint8_t mode);

/**
 * @brief       Stop codec test mode
 * @return      None
 */
void tlkmdi_audio_stop_codec_test(void);
#endif

#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
/**
 * @brief       Set algorithm loopback enable flag
 * @param[in]   flag - Enable flag
 * @return      None
 */
void audio_alg_set_loopback_enable(uint8_t flag);

/**
 * @brief       Get algorithm loopback enable flag
 * @return      Loopback enable flag
 */
uint8_t audio_alg_get_loopback_enable(void);

/**
 * @brief       Initialize audio algorithm for testing
 * @return      None
 */
void tlkmdi_audio_alg_init(void);
#endif

#endif /* TLKMW_AUDIO_COMMON_TLKMDI_AUDIO_CODEC_TEST_H_ */
