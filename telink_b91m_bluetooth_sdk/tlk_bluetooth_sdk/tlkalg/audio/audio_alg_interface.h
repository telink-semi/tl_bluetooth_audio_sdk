/********************************************************************************************************
 * @file    audio_alg_interface.h
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
#ifndef __AUDIO_ALG_INTERFACE__
#define __AUDIO_ALG_INTERFACE__

#include "tl_common.h"
#include "tlkalg/audio/sbc/tlkalg_sbc_interface.h"

#ifndef TLKALG_ALG_LOOPBACK_TEST_ENABLE
#define TLKALG_ALG_LOOPBACK_TEST_ENABLE 0
#endif

#ifndef TLKALG_SBC_DEC_ENABLE
#define TLKALG_SBC_DEC_ENABLE 0
#endif
#ifndef TLKALG_SBC_ENC_ENABLE
#define TLKALG_SBC_ENC_ENABLE 0
#endif
#ifndef TLKALG_MSBC_ENABLE
#define TLKALG_MSBC_ENABLE 0
#endif
#ifndef TLKALG_MSBC_ENABLE_CH2
#define TLKALG_MSBC_ENABLE_CH2 0
#endif
#ifndef TLKALG_CVSD_ENABLE
#define TLKALG_CVSD_ENABLE 0
#endif
#ifndef TLKALG_CVSD_ENABLE_CH2
#define TLKALG_CVSD_ENABLE_CH2 0
#endif
#ifndef TLKALG_AAC_DEC_ENABLE
#define TLKALG_AAC_DEC_ENABLE 0
#endif
#ifndef TLKALG_LC3_24BIT_ENC_ENABLE
#define TLKALG_LC3_24BIT_ENC_ENABLE 0
#endif
#ifndef TLKALG_LC3_24BIT_DEC_ENABLE
#define TLKALG_LC3_24BIT_DEC_ENABLE 0
#endif
#ifndef TLKALG_LC3_PLUS_ENC_ENABLE
#define TLKALG_LC3_PLUS_ENC_ENABLE 0
#endif
#ifndef TLKALG_LC3_PLUS_DEC_ENABLE
#define TLKALG_LC3_PLUS_DEC_ENABLE 0
#endif
#ifndef TLKALG_OPUS_ENC_ENABLE
#define TLKALG_OPUS_ENC_ENABLE 0
#endif
#ifndef TLKALG_OPUS_DEC_ENABLE
#define TLKALG_OPUS_DEC_ENABLE 0
#endif


#ifndef TLKALG_PPM_SPK_ENABLE
#define TLKALG_PPM_SPK_ENABLE 0
#endif
#ifndef TLKALG_PPM_MIC_ENABLE
#define TLKALG_PPM_MIC_ENABLE 0
#endif
#ifndef TLKALG_TWS_PPM_SPK_ENABLE
#define TLKALG_TWS_PPM_SPK_ENABLE 0
#endif
#ifndef TLKALG_HIGH_PERFORMANCE_EN
#define TLKALG_HIGH_PERFORMANCE_EN 0
#endif
#ifndef TLKALG_PPM_SPK_24BIT_ENABLE
#define TLKALG_PPM_SPK_24BIT_ENABLE 0
#endif
#ifndef TLKALG_PPM_MIC_24BIT_ENABLE
#define TLKALG_PPM_MIC_24BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_48TO441_ENABLE
#define TLKALG_ASRC_48TO441_ENABLE 0
#endif
#ifndef TLKALG_ASRC_48TO16_24BIT_ENABLE
#define TLKALG_ASRC_48TO16_24BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_48TO16_16BIT_ENABLE
#define TLKALG_ASRC_48TO16_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_16TO48_24BIT_ENABLE
#define TLKALG_ASRC_16TO48_24BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_16TO48_16BIT_ENABLE
#define TLKALG_ASRC_16TO48_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_441TO48_16BIT_ENABLE
#define TLKALG_ASRC_441TO48_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_441TO16_16BIT_ENABLE
#define TLKALG_ASRC_441TO16_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_16TO441_16BIT_ENABLE
#define TLKALG_ASRC_16TO441_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_48TO32_16BIT_ENABLE
#define TLKALG_ASRC_48TO32_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_32TO48_16BIT_ENABLE
#define TLKALG_ASRC_32TO48_16BIT_ENABLE 0
#endif
#ifndef TLKALG_ASRC_32TO16_16BIT_ENABLE
#define TLKALG_ASRC_32TO16_16BIT_ENABLE 0
#endif


#ifndef TLKALG_VAD_ENABLE
#define TLKALG_VAD_ENABLE 0
#endif
#ifndef TLKALG_NN_NS_ENABLE
#define TLKALG_NN_NS_ENABLE 0
#endif
#ifndef TLKALG_NN_NS_VAD_ENABLE
#define TLKALG_NN_NS_VAD_ENABLE 0
#endif
#ifndef TLKALG_BBF_ENABLE
#define TLKALG_BBF_ENABLE 0
#endif
#ifndef TLKALG_ANS_SPK_ENABLE
#define TLKALG_ANS_SPK_ENABLE 0
#endif
#ifndef TLKALG_AEC_ENABLE
#define TLKALG_AEC_ENABLE 0
#endif
#ifndef TLKALG_AGC_ENABLE
#define TLKALG_AGC_ENABLE 0
#endif
#ifndef TLKALG_EQ_ENABLE
#define TLKALG_EQ_ENABLE 0
#endif


#ifndef TLKALG_LC3_ENC_ENABLE
#define TLKALG_LC3_ENC_ENABLE (TLK_MW_LEA_A2DP_TO_BIS_ENABLE || TLK_MW_LEA_BMS_ENABLE || TLK_MW_LEA_US_VOICE_ENABLE || TLK_MW_LEA_UC_MUSIC_ENABLE || TLK_MW_LEA_UC_VOICE_ENABLE)
#endif

#ifndef TLKALG_LC3_DEC_ENABLE
#define TLKALG_LC3_DEC_ENABLE (TLK_MW_LEA_BMR_ENABLE || TLK_MW_LEA_US_ENABLE || TLK_MW_LEA_UC_MIC_ENABLE || TLK_MW_LEA_UC_VOICE_ENABLE)
#endif

typedef enum
{
    ALG_CHANNEL_DEFAULT = 0, /* the output channels is matched with input bitstream */
    ALG_CHANNEL_LEFT    = 1, /* only output left channel */
    ALG_CHANNEL_RIGHT   = 2, /* only output right channel */
    ALG_CHANNEL_STEREO  = 3, /* only output stereo channel */
} ALG_CHANNEL_IDX;

typedef enum
{
    ALG_WIDTH_16 = 16,
    ALG_WIDTH_20 = 20,
    ALG_WIDTH_24 = 24,
} ALG_BITDEPTH;

typedef enum
{
    ALG_AAC_DEC = 0,
    ALG_SBC_ENC,
    ALG_SBC_DEC,
    ALG_LC3_ENC,
    ALG_LC3_DEC,
    ALG_CVSD_ENC,
    ALG_CVSD_DEC,
    ALG_CVSD_ENC_CH2,
    ALG_CVSD_DEC_CH2,
    ALG_MSBC_ENC,
    ALG_MSBC_DEC,
    ALG_MSBC_ENC_CH2,
    ALG_MSBC_DEC_CH2,
    ALG_LC3_PLUS_ENC,
    ALG_LC3_PLUS_DEC,
    ALG_OPUS_ENC,
    ALG_OPUS_DEC,
    ALG_LHDC_DEC,
    ALG_EQ,
    ALG_ADPCM,
    ALG_ASRC,
    ALG_DRC,
    ALG_AEC,
    ALG_ENC,
    ALG_ANS,
    ALG_SPK_ANS,
    ALG_HYBRID,
    ALG_AGC,
    ALG_ASRC_48TO16_16BIT,
    ALG_ASRC_48TO16_24BIT,
    ALG_ASRC_16TO48_16BIT,
    ALG_ASRC_16TO48_24BIT,
    ALG_ASRC_48TO441,
    ALG_ASRC_441TO48_16BIT,
    ALG_ASRC_441TO16_16BIT,
    ALG_ASRC_16TO441_16BIT,
    ALG_ASRC_48TO32_16BIT,
    ALG_ASRC_32TO48_16BIT,
    ALG_ASRC_32TO16_16BIT,
    ALG_PPM_SPK,
    ALG_PPM_MIC,
    ALG_PPM_TWS_SPK,
    ALG_LC3_24BIT_ENC,
    ALG_LC3_24BIT_DEC,
    ALG_VAD,
    ALG_PPM_SPK_24BIT,
    ALG_PPM_MIC_24BIT,
    ALG_NN_NS,
    ALG_NN_NS_VAD,
    ALG_BBF,
    ALG_DEFAULT,
    ALG_TYPE_MAX
} audio_alg_type_e;

typedef struct
{
    /* Reset algorithm parameter settings */
    uint8_t (*audio_alg_param_reset)(void);

    /* Set algorithm parameter settings */
    uint8_t (*audio_alg_param_set)(uint8_t type, void *param);

    /* Get the memory size used by the algorithm */
    uint16_t (*audio_alg_get_size)(uint8_t channel);

    /* channel: mono or stereo */
    int8_t (*audio_alg_init)(uint8_t *p_buff, uint8_t channel);

    /* deinit */
    int8_t (*audio_alg_deinit)(void);

    int (*audio_alg_process)(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
} audio_alg_interface_t;

extern const audio_alg_interface_t audio_alg_if[];

/**
 * @brief       Weak implementation of memory allocation function
 * @param[in]   size - Size of memory to allocate
 * @return      Pointer to allocated memory or NULL if allocation fails
 */
void *tlkalg_malloc_func(uint32_t size);

/**
 * @brief       Weak implementation of memory deallocation function
 * @param[in]   pbuff - Pointer to memory buffer to free
 * @return      None
 */
void tlkalg_free_func(void *pbuff);

/**
 * @brief       Get audio algorithm interface by type
 * @param[in]   alg_type - Algorithm type identifier
 * @return      Pointer to audio algorithm interface structure
 */
audio_alg_interface_t *audio_alg_get_interface_by_type(uint8_t alg_type);

/**
 * @brief       Get default audio algorithm interface
 * @param[in]   None
 * @return      Pointer to default audio algorithm interface structure
 */
audio_alg_interface_t *audio_alg_get_interface(void);

#endif
