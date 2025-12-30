/********************************************************************************************************
 * @file    audio_alg_interface.c
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
#include "vendor/common/user_config.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/cvsd/tlkalg_cvsd_interface.h"
#include "tlkalg/audio/sbc/tlkalg_sbc_interface.h"
#include "tlkalg/audio/aac/tlkalg_aac_interface.h"
#include "tlkalg/audio/lc3/tlkalg_lc3_interface.h"
#include "tlkalg/audio/agc/tlkalg_agc_interface.h"
#include "tlkalg/audio/opus/tlkalg_opus_interface.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"
#include "tlkalg/audio/aec/tlkalg_aec_interface.h"
#if TLK_ALG_HYBRID_ENABLE
#include "tlkalg/audio/hybrid/tlkalg_hybrid_interface.h"
#endif
#include "tlkalg/audio/ppm/tlkalg_ppm_interface.h"
#include "tlkalg/audio/lc3_24bit/tlkalg_lc3_24bit_interface.h"
#if TLKALG_VAD_ENABLE
#include "tlkalg/audio/vad/tlkalg_vad_interface.h"
#endif
#include "tlkalg/audio/eq/tlkalg_eq_interface.h"
#if 1 //(MCU_CORE_TYPE == MCU_CORE_TL721X)
#include "tlkalg/audio/asrc_24bit/tlkalg_asrc_24bit_interface.h"
#else
#include "tlkalg/audio/asrc/tlkalg_asrc_interface.h"
#endif

#if TLKALG_NN_NS_ENABLE
#include "tlkalg/audio/nn_ns/tlkalg_nn_ns_interface.h"
#endif

#if TLKALG_NN_NS_VAD_ENABLE
#include "tlkalg/audio/nn_ns_vad/tlkalg_nn_ns_vad_interface.h"
#endif

#if (TLKALG_ANS_SPK_ENABLE | TLKALG_ANS_ENABLE)
#include "tlkalg/audio/ans/tlkalg_ans_interface.h"
#endif

const audio_alg_interface_t audio_alg_if[ALG_TYPE_MAX] = {
#if TLKALG_AAC_DEC_ENABLE
    [ALG_AAC_DEC] =
        {
            .audio_alg_get_size = tlkalg_aac_dec_get_size,
            .audio_alg_init     = tlkalg_aac_dec_init,
            .audio_alg_deinit   = tlkalg_aac_dec_deinit,
            .audio_alg_process  = tlkalg_aac_dec_process,
        },
#endif

#if TLKALG_SBC_ENC_ENABLE
    [ALG_SBC_ENC] =
        {
            .audio_alg_get_size  = tlkalg_sbc_enc_get_size,
            .audio_alg_init      = tlkalg_sbc_enc_init,
            .audio_alg_param_set = tlkalg_sbc_enc_param_set,
            .audio_alg_deinit    = tlkalg_sbc_enc_deinit,
            .audio_alg_process   = tlkalg_sbc_enc_process,
        },
#endif

#if TLKALG_SBC_DEC_ENABLE
    [ALG_SBC_DEC] =
        {
            .audio_alg_get_size = tlkalg_sbc_dec_get_size,
            .audio_alg_init     = tlkalg_sbc_dec_init,
            .audio_alg_deinit   = tlkalg_sbc_dec_deinit,
            .audio_alg_process  = tlkalg_sbc_dec_process,
        },
#endif

#if TLKALG_OPUS_ENC_ENABLE
    [ALG_OPUS_ENC] =
        {
            .audio_alg_get_size = tlkalg_opus_enc_get_size,
            .audio_alg_init     = tlkalg_opus_enc_init,
            .audio_alg_deinit   = tlkalg_opus_enc_deinit,
            .audio_alg_process  = tlkalg_opus_enc_process,
        },
#endif

#if TLKALG_OPUS_DEC_ENABLE
    [ALG_OPUS_DEC] =
        {
            .audio_alg_get_size = tlkalg_opus_dec_get_size,
            .audio_alg_init     = tlkalg_opus_dec_init,
            .audio_alg_deinit   = tlkalg_opus_dec_deinit,
            .audio_alg_process  = tlkalg_opus_dec_process,
        },
#endif

#if TLKALG_MSBC_ENABLE
    [ALG_MSBC_DEC] =
        {
            .audio_alg_get_size = tlkalg_msbc_dec_get_size,
            .audio_alg_init     = tlkalg_msbc_dec_init,
            .audio_alg_deinit   = tlkalg_msbc_dec_deinit,
            .audio_alg_process  = tlkalg_msbc_dec_process,
        },

    [ALG_MSBC_ENC] =
        {
            .audio_alg_get_size = tlkalg_msbc_enc_get_size,
            .audio_alg_init     = tlkalg_msbc_enc_init,
            .audio_alg_deinit   = tlkalg_msbc_enc_deinit,
            .audio_alg_process  = tlkalg_msbc_enc_process,
        },
#endif

#if TLKALG_CVSD_ENABLE
    [ALG_CVSD_DEC] =
        {
            .audio_alg_get_size = tlkalg_cvsd_dec_get_size,
            .audio_alg_init     = tlkalg_cvsd_dec_init,
            .audio_alg_deinit   = tlkalg_cvsd_dec_deinit,
            .audio_alg_process  = tlkalg_cvsd_dec_process,
        },

    [ALG_CVSD_ENC] =
        {
            .audio_alg_get_size = tlkalg_cvsd_enc_get_size,
            .audio_alg_init     = tlkalg_cvsd_enc_init,
            .audio_alg_deinit   = tlkalg_cvsd_enc_deinit,
            .audio_alg_process  = tlkalg_cvsd_enc_process,
        },
#endif

#if TLKALG_AGC_ENABLE
    [ALG_AGC] =
        {
            .audio_alg_get_size = tlkalg_agc_get_size,
            .audio_alg_init     = tlkalg_agc_init,
            .audio_alg_deinit   = tlkalg_agc_deinit,
            .audio_alg_process  = tlkalg_agc_process,
        },
#endif

#if TLKALG_ANS_ENABLE
    [ALG_ANS] =
        {
            .audio_alg_get_size = tlkalg_ans_get_size,
            .audio_alg_init     = tlkalg_ans_init,
            .audio_alg_deinit   = tlkalg_ans_deinit,
            .audio_alg_process  = tlkalg_ans_process,
        },
#endif

#if TLKALG_ANS_SPK_ENABLE
    [ALG_SPK_ANS] =
        {
            .audio_alg_get_size = tlkalg_ans_get_size,
            .audio_alg_init     = tlkalg_ans_spk_init,
            .audio_alg_deinit   = tlkalg_ans_spk_deinit,
            .audio_alg_process  = tlkalg_ans_spk_process,
        },
#endif

#if TLK_ALG_HYBRID_ENABLE
    [ALG_HYBRID] =
        {
            .audio_alg_get_size = tlkalg_hybrid_get_size,
            .audio_alg_init     = tlkalg_hybrid_init,
            .audio_alg_deinit   = tlkalg_hybrid_deinit,
            .audio_alg_process  = tlkalg_hybrid_process,
        },
#endif

#if TLKALG_ASRC_48TO16_24BIT_ENABLE
    [ALG_ASRC_48TO16_24BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_48to16_24bit_get_size,
            .audio_alg_init     = tlkalg_asrc_48to16_24bit_init,
            .audio_alg_deinit   = tlkalg_asrc_48to16_24bit_deinit,
            .audio_alg_process  = tlkalg_asrc_48to16_24bit_process,
        },
#endif


#if TLKALG_ASRC_48TO16_16BIT_ENABLE
    [ALG_ASRC_48TO16_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_48to16_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_48to16_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_48to16_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_48to16_16bit_process,
        },
#endif

#if TLKALG_ASRC_16TO48_24BIT_ENABLE
    [ALG_ASRC_16TO48_24BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_16to48_24bit_get_size,
            .audio_alg_init     = tlkalg_asrc_16to48_24bit_init,
            .audio_alg_deinit   = tlkalg_asrc_16to48_24bit_deinit,
            .audio_alg_process  = tlkalg_asrc_16to48_24bit_process,
        },
#endif

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
#if (!TLK_CFG_TEMP_DRAM_OPTM)
    [ALG_ASRC_16TO48_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_16to48_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_16to48_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_16to48_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_16to48_16bit_process,
        },
#else
    [ALG_ASRC_16TO48_16BIT] =
        {
            .audio_alg_get_size = NULL,
            .audio_alg_init     = NULL,
            .audio_alg_deinit   = NULL,
            .audio_alg_process  = NULL,
        },
#endif
#endif

#if TLKALG_ASRC_48TO441_ENABLE
    [ALG_ASRC_48TO441] =
        {
            .audio_alg_get_size = tlkalg_asrc_48to441_get_size,
            .audio_alg_init     = tlkalg_asrc_48to441_init,
            .audio_alg_deinit   = tlkalg_asrc_48to441_deinit,
            .audio_alg_process  = tlkalg_asrc_48to441_process,
        },
#endif

#if TLKALG_ASRC_441TO48_16BIT_ENABLE
    [ALG_ASRC_441TO48_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_441to48_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_441to48_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_441to48_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_441to48_16bit_process,
        },
#endif

#if TLKALG_ASRC_441TO48_16BIT_BACKUP_ENABLE
    [ALG_ASRC_441TO48_16BIT_BACKUP] =
        {
            .audio_alg_get_size = tlkalg_asrc_441to48_16bit_backup_get_size,
            .audio_alg_init     = tlkalg_asrc_441to48_16bit_backup_init,
            .audio_alg_deinit   = tlkalg_asrc_441to48_16bit_backup_deinit,
            .audio_alg_process  = tlkalg_asrc_441to48_16bit_backup_process,
        },
#endif

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
    [ALG_ASRC_441TO16_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_441to16_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_441to16_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_441to16_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_441to16_16bit_process,
        },
#endif

#if TLKALG_ASRC_16TO441_16BIT_ENABLE
    [ALG_ASRC_16TO441_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_16to441_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_16to441_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_16to441_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_16to441_16bit_process,
        },
#endif


#if TLKALG_ASRC_48TO32_16BIT_ENABLE
    [ALG_ASRC_48TO32_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_48to32_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_48to32_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_48to32_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_48to32_16bit_process,
        },
#endif

#if TLKALG_ASRC_32TO48_16BIT_ENABLE
    [ALG_ASRC_32TO48_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_32to48_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_32to48_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_32to48_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_32to48_16bit_process,
        },
#endif

#if TLKALG_ASRC_32TO16_16BIT_ENABLE
    [ALG_ASRC_32TO16_16BIT] =
        {
            .audio_alg_get_size = tlkalg_asrc_32to16_16bit_get_size,
            .audio_alg_init     = tlkalg_asrc_32to16_16bit_init,
            .audio_alg_deinit   = tlkalg_asrc_32to16_16bit_deinit,
            .audio_alg_process  = tlkalg_asrc_32to16_16bit_process,
        },
#endif

#if TLKALG_PPM_SPK_ENABLE
#if TLKALG_HIGH_PERFORMANCE_EN
    [ALG_PPM_SPK] =
        {
            .audio_alg_get_size  = tlkalg_ppm_spk_16bit_get_size,
            .audio_alg_param_set = tlkalg_ppm_spk_16bit_param_set,
            .audio_alg_init      = tlkalg_ppm_spk_16bit_init,
            .audio_alg_deinit    = tlkalg_ppm_spk_16bit_deinit,
            .audio_alg_process   = tlkalg_ppm_spk_16bit_process,
        },
#else
    [ALG_PPM_SPK] =
        {
            .audio_alg_get_size  = tlkalg_ppm_spk_get_size,
            .audio_alg_param_set = tlkalg_ppm_spk_param_set,
            .audio_alg_init      = tlkalg_ppm_spk_init,
            .audio_alg_deinit    = tlkalg_ppm_spk_deinit,
            .audio_alg_process   = tlkalg_ppm_spk_process,
        },
#endif
#endif

#if TLKALG_HIGH_PERFORMANCE_24BITS_EN
    [ALG_PPM_SPK_24BIT] =
        {
            .audio_alg_get_size  = tlkalg_ppm_spk_24bit_get_size,
            .audio_alg_param_set = tlkalg_ppm_spk_24bit_param_set,
            .audio_alg_init      = tlkalg_ppm_spk_24bit_init,
            .audio_alg_deinit    = tlkalg_ppm_spk_24bit_deinit,
            .audio_alg_process   = tlkalg_ppm_spk_24bit_process,
        },
#endif

#if TLKALG_PPM_MIC_ENABLE
#if TLKALG_HIGH_PERFORMANCE_EN
    [ALG_PPM_MIC] =
        {
            .audio_alg_get_size  = tlkalg_ppm_mic_16bit_get_size,
            .audio_alg_param_set = tlkalg_ppm_mic_16bit_param_set,
            .audio_alg_init      = tlkalg_ppm_mic_16bit_init,
            .audio_alg_deinit    = tlkalg_ppm_mic_16bit_deinit,
            .audio_alg_process   = tlkalg_ppm_mic_16bit_process,
        },
#else
    [ALG_PPM_MIC] =
        {
            .audio_alg_get_size  = tlkalg_ppm_mic_get_size,
            .audio_alg_param_set = tlkalg_ppm_mic_param_set,
            .audio_alg_init      = tlkalg_ppm_mic_init,
            .audio_alg_deinit    = tlkalg_ppm_mic_deinit,
            .audio_alg_process   = tlkalg_ppm_mic_process,
        },
#endif
#endif

#if TLKALG_TWS_PPM_SPK_ENABLE
    [ALG_PPM_TWS_SPK] =
        {
            .audio_alg_get_size  = tlkalg_ppm_spk_get_size,
            .audio_alg_param_set = tlkalg_ppm_spk_param_set,
            .audio_alg_init      = tlkalg_ppm_spk_init,
            .audio_alg_deinit    = tlkalg_ppm_spk_deinit,
            .audio_alg_process   = tlkalg_ppm_spk_process,
        },
#endif

#if TLKALG_EQ_ENABLE
    [ALG_EQ] =
        {
            .audio_alg_param_set = tlkalg_eq_param_set,
            .audio_alg_init      = tlkalg_eq_init,
            .audio_alg_deinit    = tlkalg_eq_deinit,
            .audio_alg_process   = tlkalg_eq_process,
        },
#endif

#if TLKALG_LC3_ENC_ENABLE
    [ALG_LC3_ENC] =
        {
            .audio_alg_param_set = tlkalg_lc3_enc_set_param,
            .audio_alg_get_size  = tlkalg_lc3_enc_get_size,
            .audio_alg_init      = tlkalg_lc3_enc_init,
            .audio_alg_deinit    = tlkalg_lc3_enc_deinit,
            .audio_alg_process   = tlkalg_lc3_enc_process,
        },
#endif

#if TLKALG_LC3_DEC_ENABLE
    [ALG_LC3_DEC] =
        {
            .audio_alg_param_set = tlkalg_lc3_dec_set_param,
            .audio_alg_get_size  = tlkalg_lc3_dec_get_size,
            .audio_alg_init      = tlkalg_lc3_dec_init,
            .audio_alg_deinit    = tlkalg_lc3_dec_deinit,
            .audio_alg_process   = tlkalg_lc3_dec_process,
        },
#endif

#if TLKALG_LC3_24BIT_ENC_ENABLE
    [ALG_LC3_24BIT_ENC] =
        {
            .audio_alg_get_size = tlkalg_lc3_24bit_enc_get_size,
            .audio_alg_init     = tlkalg_lc3_24bit_enc_init,
            .audio_alg_deinit   = tlkalg_lc3_24bit_enc_deinit,
            .audio_alg_process  = tlkalg_lc3_24bit_enc_process,
        },
#endif

#if TLKALG_LC3_24BIT_DEC_ENABLE
    [ALG_LC3_24BIT_DEC] =
        {
            .audio_alg_get_size = tlkalg_lc3_24bit_dec_get_size,
            .audio_alg_init     = tlkalg_lc3_24bit_dec_init,
            .audio_alg_deinit   = tlkalg_lc3_24bit_dec_deinit,
            .audio_alg_process  = tlkalg_lc3_24bit_dec_process,
        },
#endif

#if TLKALG_LC3_PLUS_ENC_ENABLE
    [ALG_LC3_PLUS_ENC] =
        {
            .audio_alg_param_set = tlkalg_lc3_plus_enc_set_param,
            .audio_alg_get_size  = tlkalg_lc3_plus_enc_get_size,
            .audio_alg_init      = tlkalg_lc3_plus_enc_init,
            .audio_alg_deinit    = tlkalg_lc3_plus_enc_deinit,
            .audio_alg_process   = tlkalg_lc3_plus_enc_process,
        },
#else
    [ALG_LC3_PLUS_ENC] =
        {
            .audio_alg_param_set = NULL,
            .audio_alg_get_size  = NULL,
            .audio_alg_init      = NULL,
            .audio_alg_deinit    = NULL,
            .audio_alg_process   = NULL,
        },
#endif

#if TLKALG_LC3_PLUS_DEC_ENABLE
    [ALG_LC3_PLUS_DEC] =
        {
            .audio_alg_param_set = tlkalg_lc3_plus_dec_set_param,
            .audio_alg_get_size  = tlkalg_lc3_plus_dec_get_size,
            .audio_alg_init      = tlkalg_lc3_plus_dec_init,
            .audio_alg_deinit    = tlkalg_lc3_plus_dec_deinit,
            .audio_alg_process   = tlkalg_lc3_plus_dec_process,
        },
#else
    [ALG_LC3_PLUS_DEC] =
        {
            .audio_alg_param_set = NULL,
            .audio_alg_get_size  = NULL,
            .audio_alg_init      = NULL,
            .audio_alg_deinit    = NULL,
            .audio_alg_process   = NULL,
        },
#endif

#if TLKALG_AEC_ENABLE
    [ALG_AEC] =
        {
            .audio_alg_get_size = tlkalg_aec_get_size,
            .audio_alg_init     = tlkalg_aec_init,
            .audio_alg_deinit   = tlkalg_aec_deinit,
            .audio_alg_process  = tlkalg_aec_process,
        },
#endif

#if TLKALG_ENC_ENABLE
    [ALG_ENC] =
        {
            .audio_alg_init    = tlkalg_enc_init,
            .audio_alg_process = tlkalg_enc_process,
        },
#endif

#if TLKALG_VAD_ENABLE
    [ALG_VAD] =
        {
            .audio_alg_get_size = tlkalg_vad_get_size,
            .audio_alg_init     = tlkalg_vad_init,
            .audio_alg_process  = tlkalg_vad_process,
        },
#endif

#if TLKALG_NN_NS_ENABLE
    [ALG_NN_NS] =
        {
            .audio_alg_get_size  = tlkalg_nn_ns_get_size,
            .audio_alg_init      = tlkalg_nn_ns_init,
            .audio_alg_deinit    = tlkalg_nn_ns_deinit,
            .audio_alg_param_set = tlkalg_nn_ns_set_param,
            .audio_alg_process   = tlkalg_nn_ns_process,
        },
#endif

#if TLKALG_NN_NS_VAD_ENABLE
    [ALG_NN_NS_VAD] =
        {
            .audio_alg_get_size  = tlkalg_nn_ns_get_size,
            .audio_alg_init      = tlkalg_nn_ns_init,
            .audio_alg_deinit    = tlkalg_nn_ns_deinit,
            .audio_alg_param_set = tlkalg_nn_ns_set_param,
            .audio_alg_process   = tlkalg_nn_ns_process,
        },
#endif

    [ALG_DEFAULT] =
        {
            .audio_alg_get_size  = NULL,
            .audio_alg_init      = NULL,
            .audio_alg_deinit    = NULL,
            .audio_alg_param_set = NULL,
            .audio_alg_process   = NULL,
        },
};

/**
 * @brief       Weak implementation of memory allocation function
 * @param[in]   size - Size of memory to allocate
 * @return      Pointer to allocated memory or NULL if allocation fails
 */
__attribute__((weak)) void *tlkalg_malloc_func(uint32_t size)
{
    (void)size;
    return NULL;
}

/**
 * @brief       Weak implementation of memory deallocation function
 * @param[in]   pbuff - Pointer to memory buffer to free
 * @return      None
 */
__attribute__((weak)) void tlkalg_free_func(void *pbuff)
{
    (void)pbuff;
}

/**
 * @brief       Get audio algorithm interface by type
 * @param[in]   alg_type - Algorithm type identifier
 * @return      Pointer to audio algorithm interface structure
 */
audio_alg_interface_t *audio_alg_get_interface_by_type(uint8_t alg_type)
{
    return (audio_alg_interface_t *)(size_t)&audio_alg_if[alg_type];
}

/**
 * @brief       Get default audio algorithm interface
 * @param[in]   None
 * @return      Pointer to default audio algorithm interface structure
 */
audio_alg_interface_t *audio_alg_get_interface(void)
{
    return (audio_alg_interface_t *)(size_t)&audio_alg_if[0];
}
