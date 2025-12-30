/********************************************************************************************************
 * @file    btp_a2dpEndPoint.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#include "btp_a2dp.h"
#include "btp_a2dpEndPoint.h"

#if (TLKBTP_CFG_A2DP_ENABLE)

static const uint8_t scBtpA2dpEndpointCapsLen[BTP_A2DP_ENDPOINT_TYPE_NUMS] = {
    [BTP_A2DP_ENDPOINT_TYPE_SNK_SBC] = sizeof(btp_a2dp_sbc_cap_t),   [BTP_A2DP_ENDPOINT_TYPE_SNK_AAC] = sizeof(btp_a2dp_aac_cap_t),
    [BTP_A2DP_ENDPOINT_TYPE_SNK_LHDC] = sizeof(btp_a2dp_lhdc_cap_t), [BTP_A2DP_ENDPOINT_TYPE_SRC_SBC] = sizeof(btp_a2dp_sbc_cap_t),
    [BTP_A2DP_ENDPOINT_TYPE_SRC_AAC] = sizeof(btp_a2dp_aac_cap_t),

};
static const btp_a2dp_codec_cap_t scBtpA2dpEndpointCaps[BTP_A2DP_ENDPOINT_TYPE_NUMS] = {

    [BTP_A2DP_ENDPOINT_TYPE_SNK_SBC] =
        {
            .sbc.media_type        = BTP_A2DP_MEDIA_TYPE_AUDIO,
            .sbc.codecType         = BTP_A2DP_CODEC_SBC,
            .sbc.frequency         = BTP_A2DP_FREQUENCY_48000 | BTP_A2DP_FREQUENCY_44100,
            .sbc.chnMode           = BTP_A2DP_CHN_MODE_JOINT_STEREO | BTP_A2DP_CHN_MODE_STEREO | BTP_A2DP_CHN_MODE_DUAL | BTP_A2DP_CHN_MODE_MONO,
            .sbc.block_length      = A2DP_BLOCK_LENGTH_16 | A2DP_BLOCK_LENGTH_12 | A2DP_BLOCK_LENGTH_8 | A2DP_BLOCK_LENGTH_4,
            .sbc.subbands          = A2DP_SUBBANDS_8 | A2DP_SUBBANDS_4,
            .sbc.allocation_method = A2DP_ALLOCATION_LOUDNESS | A2DP_ALLOCATION_SNR,
            .sbc.min_bitpool       = 2,
            .sbc.max_bitpool       = BTP_A2DP_SBC_MAX_BITPOOL,
        },

    [BTP_A2DP_ENDPOINT_TYPE_SNK_AAC] =
        {
            .aac.media_type  = BTP_A2DP_MEDIA_TYPE_AUDIO,
            .aac.codecType   = BTP_A2DP_CODEC_MPEG24,
            .aac.object_type = A2DP_AAC_OBJECT_TYPE_MPEG2_LC,
            .aac.frequency_1 = A2DP_AAC_SAMPLING_FREQ_44100,
            .aac.channels    = A2DP_AAC_CHANNEL_MODE_STEREO | A2DP_AAC_CHANNEL_MODE_MONO,
            .aac.frequency_2 = A2DP_AAC_SAMPLING_FREQ_48000,
            .aac.bit_rate_h  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK0) >> 16,
            .aac.vbr         = 0,
            .aac.bit_rate_m  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK1) >> 8,
            .aac.bit_rate_l  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK2),
        },
    [BTP_A2DP_ENDPOINT_TYPE_SNK_LHDC] =
        {
            .lhdc.media_type      = BTP_A2DP_MEDIA_TYPE_AUDIO,
            .lhdc.codecType       = BTP_A2DP_CODEC_VENDOR,
            .lhdc.vendor_id1      = BTP_A2DP_LHDC_VENDOR_ID1,
            .lhdc.vendor_id2      = BTP_A2DP_LHDC_VENDOR_ID2,
            .lhdc.vendor_id3      = BTP_A2DP_LHDC_VENDOR_ID3,
            .lhdc.vendor_id4      = BTP_A2DP_LHDC_VENDOR_ID4,
            .lhdc.vendor_codec_id = BTP_A2DP_LHDC_VENDRO_CODECID,
            .lhdc.srate44100_en   = 1,
            .lhdc.srate48000_en   = 1,
            .lhdc.srate96000_en   = 1,
            .lhdc.srate192000_en  = 0,
            .lhdc.min_bitrate     = 0x01,
            .lhdc.max_bitrate     = 0x00, // 900kbps
            .lhdc.bits16_en       = 1,
            .lhdc.bits24_en       = 1,
            .lhdc.bits32_en       = 0,
            .lhdc.ms5             = 1,
            .lhdc.version_num     = BTP_A2DP_LHDC_VER_50,
            .lhdc.lossless        = 0,
            .lhdc.ll              = 0,
            .lhdc.meta            = 0,
            .lhdc.jas             = 0,
            .lhdc.ar              = 0,
        },

    [BTP_A2DP_ENDPOINT_TYPE_SRC_SBC] =
        {
            .sbc.media_type        = BTP_A2DP_MEDIA_TYPE_AUDIO,
            .sbc.codecType         = BTP_A2DP_CODEC_SBC,
            .sbc.frequency         = BTP_A2DP_FREQUENCY_44100 | BTP_A2DP_FREQUENCY_48000,
            .sbc.chnMode           = BTP_A2DP_CHN_MODE_JOINT_STEREO,
            .sbc.block_length      = A2DP_BLOCK_LENGTH_16,
            .sbc.subbands          = A2DP_SUBBANDS_8,
            .sbc.allocation_method = A2DP_ALLOCATION_LOUDNESS,
            .sbc.min_bitpool       = 2,
            .sbc.max_bitpool       = BTP_A2DP_SBC_MAX_BITPOOL,
        },

    [BTP_A2DP_ENDPOINT_TYPE_SRC_AAC] =
        {
            .aac.media_type  = BTP_A2DP_MEDIA_TYPE_AUDIO,
            .aac.codecType   = BTP_A2DP_CODEC_MPEG24,
            .aac.object_type = A2DP_AAC_OBJECT_TYPE_MPEG2_LC,
            .aac.frequency_1 = A2DP_AAC_SAMPLING_FREQ_44100,
            .aac.channels    = A2DP_AAC_CHANNEL_MODE_STEREO | A2DP_AAC_CHANNEL_MODE_MONO,
            .aac.frequency_2 = A2DP_AAC_SAMPLING_FREQ_48000,
            .aac.bit_rate_h  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK0) >> 16,
            .aac.vbr         = 1,
            .aac.bit_rate_m  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK1) >> 8,
            .aac.bit_rate_l  = (A2DP_AAC_DEFAULT_BITRATE & A2DP_AAC_BIT_RATE_MASK2),
        },
};

const btp_a2dp_codec_cap_t *btp_a2dp_getCapInfoByType(uint8_t type)
{
    if (type >= BTP_A2DP_ENDPOINT_TYPE_NUMS) {
        return NULL;
    }
    return &scBtpA2dpEndpointCaps[type];
}

uint16_t btp_a2dp_getCapInfoLenByType(uint8_t type)
{
    if (type >= BTP_A2DP_ENDPOINT_TYPE_NUMS) {
        return 0;
    }
    return scBtpA2dpEndpointCapsLen[type];
}
#endif // #if (TLKBTP_CFG_A2DP_ENABLE)
