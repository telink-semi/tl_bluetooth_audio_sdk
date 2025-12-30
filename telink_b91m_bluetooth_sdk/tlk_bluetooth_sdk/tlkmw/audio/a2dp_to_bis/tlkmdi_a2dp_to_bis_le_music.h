/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis_le_music.h
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
#ifndef __TLKMDI_A2DP_TO_BIS_LE_MUSIC_H__
#define __TLKMDI_A2DP_TO_BIS_LE_MUSIC_H__

#if (TLK_MW_LEA_A2DP_TO_BIS_ENABLE)

struct lea_a2dp_to_bis_config
{
    uint8_t  samplingFrequency;
    uint8_t  frameDuration;
    uint16_t frameOctets;
    uint16_t iso_handle[2];
};

/**
 * @brief       Initialize LEA A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_le_init(void);

/**
 * @brief       Deinitialize LEA A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_le_deinit(void);

/**
 * @brief       Initialize A2DP to BIS configuration
 * @param[in]   p_config - Pointer to LEA A2DP to BIS configuration structure
 * @return      None
 */
void lea_a2dp_to_bis_init(struct lea_a2dp_to_bis_config *p_config);

/**
 * @brief       Deinitialize A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_deinit(void);

/**
 * @brief       Process input PCM data and encode to LC3 for BIS transmission
 * @param[in]   pcm_data - Pointer to input PCM data (stereo format)
 * @param[in]   sample_num - Number of samples to process
 * @return      None
 */
void lea_a2dp_to_bis_input_data_process(int16_t *pcm_data, uint16_t sample_num);

#endif //      TLK_MW_LEA_A2DP_TO_BIS_ENABLE

#endif
