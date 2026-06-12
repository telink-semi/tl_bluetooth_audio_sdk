/********************************************************************************************************
 * @file    btp_a2dpEndPoint.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once

/* Media types definitions */
#define BTP_A2DP_MEDIA_TYPE_AUDIO      0x00
#define BTP_A2DP_MEDIA_TYPE_VIDEO      0x01
#define BTP_A2DP_MEDIA_TYPE_MULTIMEDIA 0x02

typedef enum
{
    BTP_A2DP_FREQUENCY_16000 = 0x08,
    BTP_A2DP_FREQUENCY_32000 = 0x04,
    BTP_A2DP_FREQUENCY_44100 = 0x02,
    BTP_A2DP_FREQUENCY_48000 = 0x01,
} BTP_A2DP_FREQUENCY_ENUM;

#ifndef BTP_A2DP_SBC_SNK_MAX_BITPOOL
#define BTP_A2DP_SBC_SNK_MAX_BITPOOL 53
#endif

#ifndef BTP_A2DP_SBC_SRC_MAX_BITPOOL
#define BTP_A2DP_SBC_SRC_MAX_BITPOOL 36
#endif

// [Octet 0] Object Type
#define A2DP_AAC_OBJECT_TYPE_MPEG2_LC       0x80 /* MPEG-2 Low Complexity */
#define A2DP_AAC_OBJECT_TYPE_MPEG4_LC       0x40 /* MPEG-4 Low Complexity */
#define A2DP_AAC_OBJECT_TYPE_MPEG4_LTP      0x20 /* MPEG-4 Long Term Prediction */
#define A2DP_AAC_OBJECT_TYPE_MPEG4_SCALABLE 0x10

// [Octet 1] Sampling Frequency - 8000 to 44100
typedef enum
{
    A2DP_AAC_SAMPLING_FREQ_MASK0 = 0xFF,
    A2DP_AAC_SAMPLING_FREQ_8000  = 0x80,
    A2DP_AAC_SAMPLING_FREQ_11025 = 0x40,
    A2DP_AAC_SAMPLING_FREQ_12000 = 0x20,
    A2DP_AAC_SAMPLING_FREQ_16000 = 0x10,
    A2DP_AAC_SAMPLING_FREQ_22050 = 0x08,
    A2DP_AAC_SAMPLING_FREQ_24000 = 0x04,
    A2DP_AAC_SAMPLING_FREQ_32000 = 0x02,
    A2DP_AAC_SAMPLING_FREQ_44100 = 0x01,
} BTP_A2DP_AAC_FREQUENCY_ENUM;

// [Octet 2], [Bits 4-7] Sampling Frequency - 48000 to 96000
// NOTE: Bits offset for the higher-order octet 16-bit integer
#define A2DP_AAC_SAMPLING_FREQ_MASK1 (0xF0 << 8)
#define A2DP_AAC_SAMPLING_FREQ_48000 (1 << 3) //(0x80 >> 4)
#define A2DP_AAC_SAMPLING_FREQ_64000 (1 << 2)
#define A2DP_AAC_SAMPLING_FREQ_88200 (1 << 1)
#define A2DP_AAC_SAMPLING_FREQ_96000 1
// [Octet 2], [Bits 2-3] Channel Mode
#define A2DP_AAC_CHANNEL_MODE_MASK   0x0C
#define A2DP_AAC_CHANNEL_MODE_MONO   (1 << 1)
#define A2DP_AAC_CHANNEL_MODE_STEREO 1
// [Octet 2], [Bits 0-1] RFA
// [Octet 3], [Bit 7] Variable Bit Rate Supported
#define A2DP_AAC_VARIABLE_BIT_RATE_MASK     0x80
#define A2DP_AAC_VARIABLE_BIT_RATE_ENABLED  0x80
#define A2DP_AAC_VARIABLE_BIT_RATE_DISABLED 0x00
// [Octet 3], [Bits 0-6] Bit Rate - Bits 16-22 in the 23-bit UiMsbf
#define A2DP_AAC_BIT_RATE_MASK0  (0x7F << 16)
#define A2DP_AAC_BIT_RATE_MASK1  (0xFF << 8)
#define A2DP_AAC_BIT_RATE_MASK2  0xFF

#define A2DP_AAC_DEFAULT_BITRATE 264630 // 265 kbps
#define A2DP_AAC_MIN_BITRATE     64000  // 64 kbps

typedef enum
{
    BTP_A2DP_LHDC_VENDOR_ID1 = 0x3a,
    BTP_A2DP_LHDC_VENDOR_ID2 = 0x05,
    BTP_A2DP_LHDC_VENDOR_ID3 = 0x00,
    BTP_A2DP_LHDC_VENDOR_ID4 = 0x00,

    BTP_A2DP_LHDC_VENDRO_CODECID = 0x4c35,

    BTP_A2DP_LHDC_VER_50 = BIT(0),
    BTP_A2DP_LHDC_VER_51 = BIT(1),
    BTP_A2DP_LHDC_VER_52 = BIT(2),
    BTP_A2DP_LHDC_VER_53 = BIT(3),

    BTP_A2DP_LHDC_SRATE_441K = 44100,
    BTP_A2DP_LHDC_SRATE_48K  = 48000,
    BTP_A2DP_LHDC_SRATE_96K  = 96000,
    BTP_A2DP_LHDC_SRATE_192K = 192000,

    BTP_A2DP_LHDC_BITS_16 = 16,
    BTP_A2DP_LHDC_BITS_24 = 24,
    BTP_A2DP_LHDC_BITS_32 = 32,
} BTP_A2DP_LHDC_NUM;

typedef enum
{
    BTP_A2DP_ENDPOINT_TYPE_SNK_SBC,
    BTP_A2DP_ENDPOINT_TYPE_SNK_AAC,
    BTP_A2DP_ENDPOINT_TYPE_SNK_LHDC,
    BTP_A2DP_ENDPOINT_TYPE_SNK_NUMS,
    BTP_A2DP_ENDPOINT_TYPE_SRC_SBC = BTP_A2DP_ENDPOINT_TYPE_SNK_NUMS,
    BTP_A2DP_ENDPOINT_TYPE_SRC_AAC,
    BTP_A2DP_ENDPOINT_TYPE_NUMS,
    BTP_A2DP_ENDPOINT_TYPE_SRC_NUMS = BTP_A2DP_ENDPOINT_TYPE_NUMS - BTP_A2DP_ENDPOINT_TYPE_SNK_NUMS,
} BTP_A2DP_ENDPOINT_TYPE_ENUM;

//note:this structure can't be changed by user
typedef struct
{
    uint32_t rfa0              : 4;
    uint32_t media_type        : 4;
    uint32_t codecType         : 8;
    uint32_t chnMode           : 4; // Channel Mode - bit[3~0]
    uint32_t frequency         : 4; // Sampling Frequency - bit[7~4]
    uint32_t allocation_method : 2; // Allocation Method - bit[1~0]
    uint32_t subbands          : 2; // Subbands - bit[3~2]
    uint32_t block_length      : 4; // Block Length - bit[7~4]
    uint8_t  min_bitpool;           // Minimum Bitpool Value
    uint8_t  max_bitpool;           // Maximum Bitpool Value
} __attribute__((__packed__)) btp_a2dp_sbc_cap_t;

typedef struct
{
    //	struct avdtp_media_codec_capability cap;
    uint8_t rfa0       : 4;
    uint8_t media_type : 4;
    uint8_t codecType;
    uint8_t object_type;
    uint8_t frequency_1;
    uint8_t rfa         : 2;
    uint8_t channels    : 2;
    uint8_t frequency_2 : 4;
    uint8_t bit_rate_h  : 7;
    uint8_t vbr         : 1;
    uint8_t bit_rate_m;
    uint8_t bit_rate_l;
} __attribute__((__packed__)) btp_a2dp_aac_cap_t;

//note:this structure can't be changed by user
typedef struct
{
    uint8_t rfa0       : 4;
    uint8_t media_type : 4;
    uint8_t codecType;  // 0xff
    uint8_t vendor_id1; //0x3a
    uint8_t vendor_id2; // 0x05

    uint8_t  vendor_id3;      // 0x0000
    uint8_t  vendor_id4;      // 0x0000
    uint16_t vendor_codec_id; //ID: 0x4c35

    uint8_t srate192000_en : 1;
    uint8_t reserve3       : 1;
    uint8_t srate96000_en  : 1;
    uint8_t reserve2       : 1;
    uint8_t srate48000_en  : 1;
    uint8_t srate44100_en  : 1;
    uint8_t reserve1       : 2;

    uint8_t bits32_en   : 1;
    uint8_t bits24_en   : 1;
    uint8_t bits16_en   : 1;
    uint8_t reserve4    : 1;
    uint8_t max_bitrate : 2;
    uint8_t min_bitrate : 2;

    uint8_t version_num : 4;
    uint8_t ms5         : 1;
    uint8_t reserve5    : 3;

    uint8_t ar       : 1;
    uint8_t jas      : 1;
    uint8_t meta     : 1;
    uint8_t reserve6 : 3;
    uint8_t ll       : 1;
    uint8_t lossless : 1;

    uint8_t reserve7;
} __attribute__((__packed__)) btp_a2dp_lhdc_cap_t;

//note:this structure can't be changed by user
typedef union
{
    btp_a2dp_sbc_cap_t  sbc;
    btp_a2dp_aac_cap_t  aac;
    btp_a2dp_lhdc_cap_t lhdc;
} btp_a2dp_codec_cap_t;

const btp_a2dp_codec_cap_t *btp_a2dp_getCapInfoByType(uint8_t type);

uint16_t btp_a2dp_getCapInfoLenByType(uint8_t type);