/********************************************************************************************************
 * @file    tlkalg_hybrid_interface.h
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
#ifndef TLKALG_AUDIO_HYBRID_TLKALG_HYBRID_INTERFACE_H_
#define TLKALG_AUDIO_HYBRID_TLKALG_HYBRID_INTERFACE_H_
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlka_hybrid_alg_api.h"
#if TLK_ALG_HYBRID_ENABLE
#define HYBRID_DELAY 180
extern int              hybrid_delay;
extern char             alg_ctrl;
extern void            *st_hybrid;
extern int             *ScratchBuffer;
extern HYBRID_ALG_Param hybrid_para;

extern GSC_Param      gsc_para;
extern AEC_Param      aec_para;
extern W_NS_CFG_PARAM ns_para;

/**
 * @brief       Calculate the required buffer size for the hybrid algorithm.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      The total size of the buffer required for the hybrid algorithm.
 */
uint16_t tlkalg_hybrid_get_size(uint8_t channel);

/**
 * @brief       Initialize the hybrid algorithm with the given buffer.
 * @param[in]   p_buff  - pointer to the buffer to be used by the hybrid algorithm.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      0 on success, non-zero on error.
 */
int8_t tlkalg_hybrid_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the hybrid algorithm.
 * @return      0 on success, non-zero on error.
 */
int8_t tlkalg_hybrid_deinit(void);

/**
 * @brief       Process a frame of audio data using the hybrid algorithm.
 * @param[in]   ps      - pointer to the source audio data buffer.
 * @param[out]  pd      - pointer to the destination audio data buffer.
 * @param[in]   len     - length of the audio frame in samples.
 * @param[in]   width   - width of the audio data, not used in this function.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      0 on success, non-zero on error.
 */
int tlkalg_hybrid_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif /* TLKALG_AUDIO_HYBRID_TLKALG_HYBRID_INTERFACE_H_ */
#endif
