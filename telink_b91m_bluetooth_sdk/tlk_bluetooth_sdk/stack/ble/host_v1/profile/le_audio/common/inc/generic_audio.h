/********************************************************************************************************
 * @file    generic_audio.h
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

// Coding Format
enum bluetooth_audio_coding_format
{
    BT_CODING_FORMAT_U_LAW_LOG       = 0x00,
    BT_CODING_FORMAT_A_LAW_LOG       = 0x01,
    BT_CODING_FORMAT_CSVD            = 0x02,
    BT_CODING_FORMAT_TRANSPARENT     = 0x03,
    BT_CODING_FORMAT_LINEAR_PCM      = 0x04,
    BT_CODING_FORMAT_MSBC            = 0x05,
    BT_CODING_FORMAT_LC3             = 0x06,
    BT_CODING_FORMAT_G729A           = 0x07, /* This is a draft allocation associated with draft specifications and is subject to change */
    BT_CODING_FORMAT_VENDOR_SPECIFIC = 0xFF,
};

/** < LE Audio Location Definitions */
enum lea_location_flag
{
    LEA_LOCATION_NONE                  = 0x0000,
    LEA_LOCATION_FRONT_LEFT            = 1U << 0,                                            /** < Front Left */
    LEA_LOCATION_FRONT_RIGHT           = 1U << 1,                                            /** < Front Right */
    LEA_LOCATION_FRONT_CENTER          = 1U << 2,                                            /** < Front Center */
    LEA_LOCATION_LOW_FREQUENCY_1       = 1U << 3,                                            /** < Low Frequency Effects 1 */
    LEA_LOCATION_BACK_LEFT             = 1U << 4,                                            /** < Back Left */
    LEA_LOCATION_BACK_RIGHT            = 1U << 5,                                            /** < Back Right */
    LEA_LOCATION_FRONT_LEFT_OF_CENTER  = 1U << 6,                                            /** < Front Left of Center */
    LEA_LOCATION_FRONT_RIGHT_OF_CENTER = 1U << 7,                                            /** < Front Right of Center */
    LEA_LOCATION_BACK_CENTER           = 1U << 8,                                            /** < Back Center */
    LEA_LOCATION_LOW_FREQUENCY_2       = 1U << 9,                                            /** < Low Frequency Effects 2 */
    LEA_LOCATION_SIDE_LEFT             = 1U << 10,                                           /** < Side Left */
    LEA_LOCATION_SIDE_RIGHT            = 1U << 11,                                           /** < Side Right */
    LEA_LOCATION_TOP_FRONT_LEFT        = 1U << 12,                                           /** < Top Front Left */
    LEA_LOCATION_TOP_FRONT_RIGHT       = 1U << 13,                                           /** < Top Front Right */
    LEA_LOCATION_TOP_FRONT_CENTER      = 1U << 14,                                           /** < Top Front Center */
    LEA_LOCATION_TOP_CENTER            = 1U << 15,                                           /** < Top Center */
    LEA_LOCATION_TOP_BACK_LEFT         = 1U << 16,                                           /** < Top Back Left */
    LEA_LOCATION_TOP_BACK_RIGHT        = 1U << 17,                                           /** < Top Back Right */
    LEA_LOCATION_TOP_SIDE_LEFT         = 1U << 18,                                           /** < Top Side Left */
    LEA_LOCATION_TOP_SIDE_RIGHT        = 1U << 19,                                           /** < Top Side Right */
    LEA_LOCATION_TOP_BACK_CENTER       = 1U << 20,                                           /** < Top Back Center */
    LEA_LOCATION_BOTTOM_FRONT_CENTER   = 1U << 21,                                           /** < Bottom Front Center */
    LEA_LOCATION_BOTTOM_FRONT_LEFT     = 1U << 22,                                           /** < Bottom Front Left */
    LEA_LOCATION_BOTTOM_FRONT_RIGHT    = 1U << 23,                                           /** < Bottom Front Right */
    LEA_LOCATION_FRONT_LEFT_WIDE       = 1U << 24,                                           /** < Front Left Wide */
    LEA_LOCATION_FRONT_RIGHT_WIDE      = 1U << 25,                                           /** < Front Right Wide */
    LEA_LOCATION_LEFT_SURROUND         = 1U << 26,                                           /** < Left Surround */
    LEA_LOCATION_RIGHT_SURROUND        = 1U << 27,                                           /** < Right Surround */
    LEA_LOCATION_RESERVED              = ((1U << 28) | (1U << 29) | (1U << 30) | (1U << 31)) /** < bit28 ~ bit29 */
};

/** < Check if the location parameter contains reserved bits */
#define LEA_LOCATION_CHECK_RESERVED_BITS(param) ((param) & LEA_LOCATION_RESERVED)

/** < Extract valid bits from the location parameter */
#define LEA_LOCATION_GET_VALID_BITS(param) ((param) & (~LEA_LOCATION_RESERVED))

/** < Audio Input Type */
enum lea_audio_input_type
{
    LEA_INPUT_TYPE_UNSPECIFIED = 0x00, /** < Unspecified input */
    LEA_INPUT_TYPE_BLUETOOTH   = 0x01, /** < Bluetooth Audio Stream */
    LEA_INPUT_TYPE_MICROPHONE  = 0x02, /** < Microphone */
    LEA_INPUT_TYPE_ANALOG      = 0x03, /** < Analog Interface */
    LEA_INPUT_TYPE_DIGITAL     = 0x04, /** < Digital Interface */
    LEA_INPUT_TYPE_RADIO       = 0x05, /** < AM/FM/XM/etc. */
    LEA_INPUT_TYPE_STREAMING   = 0x06  /** < Streaming Audio Source */
};

/** < LE Audio Context Type */
enum lea_context_type
{
    LEA_CONTEXT_TYPE_PROHIBITED       = 0x0000,   /** < Prohibited. */
    LEA_CONTEXT_TYPE_UNSPECIFIED      = 1U << 0,  /** < Unspecified. Matches any audio content. */
    LEA_CONTEXT_TYPE_CONVERSATIONAL   = 1U << 1,  /** < Conversation between humans as, for example, in telephony or video calls. */
    LEA_CONTEXT_TYPE_MEDIA            = 1U << 2,  /** < Media as, for example, in music, public radio, podcast or video soundtrack. */
    LEA_CONTEXT_TYPE_GAME             = 1U << 3,  /** < Audio associated with video gaming */
    LEA_CONTEXT_TYPE_INSTRUCTIONAL    = 1U << 4,  /** < Instructional audio as */
    LEA_CONTEXT_TYPE_VOICE_ASSISTANTS = 1U << 5,  /** < Man - machine communication */
    LEA_CONTEXT_TYPE_LIVE             = 1U << 6,  /** < Live audio */
    LEA_CONTEXT_TYPE_SOUND_EFFECTS    = 1U << 7,  /** < Sound effects including keyboard and touch feedback; menu and user interface sounds; and other system sounds */
    LEA_CONTEXT_TYPE_NOTIFICATIONS    = 1U << 8,  /** < Notification and reminder sounds; attention - seeking audio */
    LEA_CONTEXT_TYPE_RINGTONE         = 1U << 9,  /** < Alerts the user to an incoming call. */
    LEA_CONTEXT_TYPE_ALERT            = 1U << 10, /** < Alarms and timers; immediate alerts */
    LEA_CONTEXT_TYPE_EMERGENCY_ALARM  = 1U << 11, /** < Emergency alarm Emergency sounds */
    LEA_CONTEXT_TYPE_RESERVED         = ((1U << 12) | (1U << 13) | (1U << 14) | (1U << 15)), /** < bit12 ~ bit15 */
    LEA_CONTEXT_TYPE_ALL_SELECT       = (unsigned short)(~(LEA_CONTEXT_TYPE_RESERVED)),
};

/** < Check if the context type parameter is either 0 or contains reserved bits */
#define LEA_CONTEXT_CHECK_RESERVED_BITS(param) ((param) == 0 || ((param) & LEA_CONTEXT_TYPE_RESERVED))

/** < Extract valid bits from the context type parameter */
#define LEA_CONTEXT_GET_VALID_BITS(param) ((param) & (~LEA_CONTEXT_TYPE_RESERVED))

/******************* Subsection: Codec Specific Capabilities LTV Structures *******************/
// Audio Codec Specific Capabilities type
enum lea_cap_type
{
    LEA_CAP_TYPE_SUPP_SAMPLE_FREQUENCY         = 0x01, /** < Supported Sampling Frequencies */
    LEA_CAP_TYPE_SUPP_FRAME_DURATION           = 0x02, /** < Supported Frame Durations */
    LEA_CAP_TYPE_SUPP_AUDIO_CHN_COUNTS         = 0x03, /** < Supported Audio Channel Counts */
    LEA_CAP_TYPE_SUPP_OCTETS_PER_CODEC_FRAME   = 0x04, /** < Supported Octets Per Codec Frame */
    LEA_CAP_TYPE_SUPP_MAX_CODEC_FRAMES_PER_SDU = 0x05  /** < Supported Max Codec Frames Per SDU */
};

// Audio Support Frame Frequency (bitField, for PACS)
enum lea_supp_freq
{
    LEA_SUPP_FREQ_8000_HZ   = 1U << 0,                               /** < 8000 Hz */
    LEA_SUPP_FREQ_11025_HZ  = 1U << 1,                               /** < 11025 Hz */
    LEA_SUPP_FREQ_16000_HZ  = 1U << 2,                               /** < 16000 Hz */
    LEA_SUPP_FREQ_22050_HZ  = 1U << 3,                               /** < 22050 Hz */
    LEA_SUPP_FREQ_24000_HZ  = 1U << 4,                               /** < 24000 Hz */
    LEA_SUPP_FREQ_32000_HZ  = 1U << 5,                               /** < 32000 Hz */
    LEA_SUPP_FREQ_44100_HZ  = 1U << 6,                               /** < 44100 Hz */
    LEA_SUPP_FREQ_48000_HZ  = 1U << 7,                               /** < 48000 Hz */
    LEA_SUPP_FREQ_88200_HZ  = 1U << 8,                               /** < 88200 Hz */
    LEA_SUPP_FREQ_96000_HZ  = 1U << 9,                               /** < 96000 Hz */
    LEA_SUPP_FREQ_176400_HZ = 1U << 10,                              /** < 176400 Hz */
    LEA_SUPP_FREQ_192000_HZ = 1U << 11,                              /** < 192000 Hz */
    LEA_SUPP_FREQ_384000_HZ = 1U << 12,                              /** < 384000 Hz */
    LEA_SUPP_FREQ_RESERVED  = ((1U << 13) | (1U << 14) | (1U << 15)) /** < Reserved bits (bit 13 - 15) */
};

/** < Extract valid bits from the supported frequency parameter */
#define LEA_SUPP_FREQ_VALID_BITS(param) ((param) & (~LEA_SUPP_FREQ_RESERVED))

/** < Check if the supported frequency parameter contains reserved bits */
#define LEA_SUPP_FREQ_CHECK_RFU(param) ((param) & LEA_SUPP_FREQ_RESERVED)

// Audio Support Frame Duration (bitField, for PACS)
enum lea_supp_frame_duration
{
    LEA_SUPP_FRAME_DURATION_7_5           = 1U << 0, /** < 7.5ms frame duration */
    LEA_SUPP_FRAME_DURATION_10            = 1U << 1, /** < 10ms frame duration */
    LEA_SUPP_FRAME_DURATION_7_5_PREFERRED = 1U << 4, /** < 7.5ms frame duration preferred */
    LEA_SUPP_FRAME_DURATION_10_PREFERRED  = 1U << 5, /** < 10ms frame duration preferred */
    LEA_SUPP_FRAME_DURATION_RESERVED      = ((1U << 2) | (1U << 3) | (1U << 6) | (1U << 7))
};

/** < Extract valid bits from the supported duration parameter */
#define LEA_SUPP_DURATION_VALID_BITS(param) ((param) & (~LEA_SUPP_FRAME_DURATION_RESERVED))

/** < Check if the supported duration parameter contains reserved bits */
#define LEA_SUPP_DURATION_RFU(param) ((param) & LEA_SUPP_FRAME_DURATION_RESERVED)

// Audio Channel counts (bitField, for PACS)
enum lea_channel_counts
{
    LEA_CHANNEL_COUNTS_1 = 1U << 0, /** < 1 audio channel */
    LEA_CHANNEL_COUNTS_2 = 1U << 1, /** < 2 audio channels */
    LEA_CHANNEL_COUNTS_3 = 1U << 2, /** < 3 audio channels */
    LEA_CHANNEL_COUNTS_4 = 1U << 3, /** < 4 audio channels */
    LEA_CHANNEL_COUNTS_5 = 1U << 4, /** < 5 audio channels */
    LEA_CHANNEL_COUNTS_6 = 1U << 5, /** < 6 audio channels */
    LEA_CHANNEL_COUNTS_7 = 1U << 6, /** < 7 audio channels */
    LEA_CHANNEL_COUNTS_8 = 1U << 7  /** < 8 audio channels */
};

/** < Check if the channel counts parameter is zero (reserved for RFU) */
#define LEA_CHANNEL_COUNTS_RFU(param) ((param) == 0)

/** < Minimum number of supported audio channels */
#define LEA_CHANNEL_COUNTS_MIN 1

/** < Maximum number of supported audio channels */
#define LEA_CHANNEL_COUNTS_MAX 8

/******************* Subsection: Codec Specific Configuration LTV Structures *******************/
#define LEA_CONFIG_TYPE_MAX_SIZE 19

// Audio Codec_Specific_Configuration parameter
enum lea_config_type
{
    LEA_CONFIG_TYPE_SAMPLE_FREQUENCY           = 0x01, /** < Sampling Frequency */
    LEA_CONFIG_TYPE_FRAME_DURATION             = 0x02, /** < Frame Duration */
    LEA_CONFIG_TYPE_CHANNELS_ALLOCATION        = 0x03, /** < Audio Channel Allocation */
    LEA_CONFIG_TYPE_OCTETS_PER_CODEC_FRAME     = 0x04, /** < Octets Per Codec Frame */
    LEA_CONFIG_TYPE_CODEC_FRAME_BLOCKS_PER_SDU = 0x05  /** < Codec Frame Blocks Per SDU */
};

// Codec_Specific_Capabilities Field Valid mask
enum lea_config_valid_mask
{
    LEA_CONFIG_TYPE_MASK_SAMPLE_FREQUENCY           = 1 << 0,
    LEA_CONFIG_TYPE_MASK_FRAME_DURATION             = 1 << 1,
    LEA_CONFIG_TYPE_MASK_CHANNELS_ALLOCATION        = 1 << 2,
    LEA_CONFIG_TYPE_MASK_OCTETS_PER_CODEC_FRAME     = 1 << 3,
    LEA_CONFIG_TYPE_MASK_CODEC_FRAME_BLOCKS_PER_SDU = 1 << 4,
};

// Audio Frame Frequency (for codec parameter)
enum lea_select_sampling_freq
{
    LEA_SELECT_SAMPLING_FREQ_MIN,            /** < Minimum value for audio sampling frequency selection */
    LEA_SELECT_SAMPLING_FREQ_8000_HZ   = 1,  /** < 8000 Hz */
    LEA_SELECT_SAMPLING_FREQ_11025_HZ  = 2,  /** < 11025 Hz */
    LEA_SELECT_SAMPLING_FREQ_16000_HZ  = 3,  /** < 16000 Hz */
    LEA_SELECT_SAMPLING_FREQ_22050_HZ  = 4,  /** < 22050 Hz */
    LEA_SELECT_SAMPLING_FREQ_24000_HZ  = 5,  /** < 24000 Hz */
    LEA_SELECT_SAMPLING_FREQ_32000_HZ  = 6,  /** < 32000 Hz */
    LEA_SELECT_SAMPLING_FREQ_44100_HZ  = 7,  /** < 44100 Hz */
    LEA_SELECT_SAMPLING_FREQ_48000_HZ  = 8,  /** < 48000 Hz */
    LEA_SELECT_SAMPLING_FREQ_88200_HZ  = 9,  /** < 88200 Hz */
    LEA_SELECT_SAMPLING_FREQ_96000_HZ  = 10, /** < 96000 Hz */
    LEA_SELECT_SAMPLING_FREQ_176400_HZ = 11, /** < 176400 Hz */
    LEA_SELECT_SAMPLING_FREQ_192000_HZ = 12, /** < 192000 Hz */
    LEA_SELECT_SAMPLING_FREQ_384000_HZ = 13, /** < 384000 Hz */
    LEA_SELECT_SAMPLING_FREQ_MAX,            /** < Maximum value for audio sampling frequency selection */
};

/** < Check if the audio sampling frequency selection parameter is out of valid range */
#define LEA_SELECT_SAMPLING_FREQ_RFU(param) ((param) <= LEA_SELECT_SAMPLING_FREQ_MIN || (param) >= LEA_SELECT_SAMPLING_FREQ_MAX)

// Audio Frame Duration (for codec parameter)
enum lea_select_frame_duration
{
    LEA_SELECT_FRAME_DURATION_7_5 = 0, /** < 7.5ms frame duration */
    LEA_SELECT_FRAME_DURATION_10  = 1  /** < 10ms frame duration */
};

/** < Check if the audio frame duration selection parameter is not a valid value */
#define LEA_SELECT_FRAME_DURATION_RFU(param) ((param) != LEA_SELECT_FRAME_DURATION_7_5 && (param) != LEA_SELECT_FRAME_DURATION_10)

/******************* Subsection: Metadata LTV Structures *******************/
/* Metadata ata length configuration (can be changed by user) */
#define METADATA_SUPP_BUFF_SIZE (64) /* min~max:1 ~ 255 */

// Metadata types
enum lea_metadata_type
{
    LEA_METADATA_TYPE_PREFERRED_CONTEXTS  = 0x01, /** < Preferred Audio Contexts */
    LEA_METADATA_TYPE_STREAMING_CONTEXTS  = 0x02, /** < Streaming Audio Contexts */
    LEA_METADATA_TYPE_PROGRAM_INFO        = 0x03, /** < Program Info */
    LEA_METADATA_TYPE_LANGUAGE            = 0x04, /** < Language, defined in ISO 693 - 3 */
    LEA_METADATA_TYPE_CCID_LIST           = 0x05, /** < CCID List, It can only changed when accepting a BAP Enable operation or a BAP Update Metadata operation by an Initiator */
    LEA_METADATA_TYPE_PARENTAL_RATING     = 0x06, /** < Parental rating */
    LEA_METADATA_TYPE_PROGRAM_INFO_URI    = 0x07, /** < UTF - 8 formatted URL link used to present more information about Program Info */
    LEA_METADATA_TYPE_AUDIO_ACTIVE_STATE  = 0x08, /** < Audio Active State */
    LEA_METADATA_TYPE_IMMEDIATE_RENDERING = 0x09, /** < Broadcast Audio Immediate Rendering Flag */
    LEA_METADATA_TYPE_EXTENDED_METADATA   = 0xFE, /** < Extended Metadata */
    LEA_METADATA_TYPE_VENDOR_SPECIFIC     = 0xFF  /** < Vendor Specific */
};

// Metadata type valid mask
enum lea_metadata_valid_mask
{
    LEA_METADATA_TYPE_MASK_PREFERRED_CONTEXTS  = 1 << 0,
    LEA_METADATA_TYPE_MASK_STREAMING_CONTEXTS  = 1 << 1,
    LEA_METADATA_TYPE_MASK_PROGRAM_INFO        = 1 << 2,
    LEA_METADATA_TYPE_MASK_LANGUAGE            = 1 << 3,
    LEA_METADATA_TYPE_MASK_CCID_LIST           = 1 << 4,
    LEA_METADATA_TYPE_MASK_PARENTAL_RATING     = 1 << 5,
    LEA_METADATA_TYPE_MASK_PROGRAM_INFO_URI    = 1 << 6,
    LEA_METADATA_TYPE_MASK_AUDIO_ACTIVE_STATE  = 1 << 7,
    LEA_METADATA_TYPE_MASK_IMMEDIATE_RENDERING = 1 << 8,
    LEA_METADATA_TYPE_MASK_EXTENDED_METADATA   = 1 << 14,
    LEA_METADATA_TYPE_MASK_VENDOR_SPECIFIC     = 1 << 15
};

#define LEA_INIT_CODEC_ID_LC3               \
    {                                       \
        .id         = BT_CODING_FORMAT_LC3, \
        .company_id = 0x0000,               \
        .vendor_id  = 0x0000,               \
    }

/** < LE Audio Codec ID */
struct lea_codec_id
{
    uint8_t  id;
    uint16_t company_id;
    uint16_t vendor_id;
} __attribute__((packed));

#define LEA_INIT_CODEC_SPEC_CONFIG(freq, dur, chn, octets, per_sdu) \
    {.sampling_freq = freq, .frame_duration = dur, .channel_allocation = chn, .per_codec_frame = octets, .per_sdu_frame_blocks = per_sdu}

/** < LE Audio codec specification configuration parameter */
struct lea_codec_spec_config_param
{
    uint8_t  sampling_freq;        //[Must]Sampling Frequency.     LEA_SELECT_SAMPLING_FREQ_8000_HZ
    uint8_t  frame_duration;       //[Must]Frame Duration.        LEA_SELECT_FRAME_DURATION_7_5
    uint32_t channel_allocation;   //[Must]Audio Channel Allocation. LEA_LOCATION_FRONT_LEFT
    uint16_t per_codec_frame;      //[Must]Octets Per Codec Frame.
    uint8_t  per_sdu_frame_blocks; //[Must]Codec Frame Blocks Per SDU. only support 1 block for now.
};

#define LEA_CODEC_CFG_LC3_8_1(allocation)  LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_8000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 26, 1)
#define LEA_CODEC_CFG_LC3_8_2(allocation)  LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_8000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 30, 1)
#define LEA_CODEC_CFG_LC3_16_1(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_16000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 30, 1)
#define LEA_CODEC_CFG_LC3_16_2(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_16000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 40, 1)
#define LEA_CODEC_CFG_LC3_24_1(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_24000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 45, 1)
#define LEA_CODEC_CFG_LC3_24_2(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_24000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 60, 1)
#define LEA_CODEC_CFG_LC3_32_1(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 60, 1)
#define LEA_CODEC_CFG_LC3_32_2(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 80, 1)
#define LEA_CODEC_CFG_LC3_44_1(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_44100_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 97, 1)
#define LEA_CODEC_CFG_LC3_44_2(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_44100_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 130, 1)
#define LEA_CODEC_CFG_LC3_48_1(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 75, 1)
#define LEA_CODEC_CFG_LC3_48_2(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 100, 1)
#define LEA_CODEC_CFG_LC3_48_3(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 90, 1)
#define LEA_CODEC_CFG_LC3_48_4(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 120, 1)
#define LEA_CODEC_CFG_LC3_48_5(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_7_5, allocation, 117, 1)
#define LEA_CODEC_CFG_LC3_48_6(allocation) LEA_INIT_CODEC_SPEC_CONFIG(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_10, allocation, 155, 1)


#define LEA_INIT_CODEC_SPEC_CAP(freq, dur, octets, chn, per_sdu) \
    {.sampling_freq = freq, .frame_durations = dur, .channel_counts = chn, .min_per_codec_frame = octets, .max_per_codec_frame = octets, .max_per_sdu = per_sdu}

/** < LE Audio codec specification capabilities parameter */
struct lea_codec_spec_cap_param
{
    uint16_t sampling_freq;   //[Must]Supported Sampling Frequencies, 0:RFU, LEA_SUPP_FREQ_8000_HZ
    uint8_t  frame_durations; //[Must]Supported Frame Durations, 0:RFU, LEA_SUPP_FRAME_DURATION_7_5
    uint8_t  channel_counts;  //[Must]Supported Audio Channel Counts, 0:RFU, LEA_CHANNEL_COUNTS_1

    struct
    {                                 //[Must]Supported Octets Per Codec Frame
        uint16_t min_per_codec_frame; //        Minimum number of octets supported per codec frame
        uint16_t max_per_codec_frame; //        Maximum number of octets supported per codec frame
    };

    uint8_t max_per_sdu; //[Must]Supported Max Codec Frames Per SDU.
};

// current only support LC3 and per sdu is 1
#define LEA_CODEC_CAP_LC3_8_1(chn)   LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_8000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 26, chn, 1)
#define LEA_CODEC_CAP_LC3_8_2(chn)   LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_8000_HZ, LEA_SUPP_FRAME_DURATION_10, 30, chn, 1)
#define LEA_CODEC_CAP_LC3_16_1(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_16000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 30, chn, 1)
#define LEA_CODEC_CAP_LC3_16_2(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_16000_HZ, LEA_SUPP_FRAME_DURATION_10, 40, chn, 1)
#define LEA_CODEC_CAP_LC3_24_1(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_24000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 45, chn, 1)
#define LEA_CODEC_CAP_LC3_24_2(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_24000_HZ, LEA_SUPP_FRAME_DURATION_10, 60, chn, 1)
#define LEA_CODEC_CAP_LC3_32_1(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_32000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 60, chn, 1)
#define LEA_CODEC_CAP_LC3_32_2(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_32000_HZ, LEA_SUPP_FRAME_DURATION_10, 80, chn, 1)
#define LEA_CODEC_CAP_LC3_441_1(chn) LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_44100_HZ, LEA_SUPP_FRAME_DURATION_7_5, 97, chn, 1)
#define LEA_CODEC_CAP_LC3_441_2(chn) LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_44100_HZ, LEA_SUPP_FRAME_DURATION_10, 130, chn, 1)
#define LEA_CODEC_CAP_LC3_48_1(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 75, chn, 1)
#define LEA_CODEC_CAP_LC3_48_2(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 100, chn, 1)
#define LEA_CODEC_CAP_LC3_48_3(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 90, chn, 1)
#define LEA_CODEC_CAP_LC3_48_4(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 120, chn, 1)
#define LEA_CODEC_CAP_LC3_48_5(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 117, chn, 1)
#define LEA_CODEC_CAP_LC3_48_6(chn)  LEA_INIT_CODEC_SPEC_CAP(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 155, chn, 1)


#define LEA_INIT_METADATA_CONTEXTS(contexts) \
    {                                        \
        .preferred_contexts = contexts,      \
        .streaming_contexts = contexts,      \
    }

#define LEA_CCID_DEFAULT_VALUE      0x00
#define LEA_CCID_VOICE_VALUE        0x01
#define LEA_CCID_MEDIA_PLAYER_VALUE 0x02

#define LEA_INIT_METADATA_STREAM_CONTEXTS(contexts, ccid) \
    (&(struct lea_metadata_param){                        \
        .streaming_contexts = contexts,                   \
        .ccid_value         = ccid,                       \
    })

/** < LE Audio metadata parameter */
struct lea_metadata_param
{
    //Parameters initialize for metadata
    uint16_t    preferred_contexts;  //Context Type is a preferred use case for this codec configuration, LEA_CONTEXT_TYPE_PROHIBITED
    uint16_t    streaming_contexts;  //Context Type is a intended use case for this Audio Stream, LEA_CONTEXT_TYPE_PROHIBITED
    const char *program_info;        //Title and/or summary of Audio Stream content: UTF-8 format
    const char *language;            //3-byte, lower case language code as defined in ISO 639-3
    uint8_t     ccid_value;          //
    uint8_t     parental_rating;     //https://www.etsi.org
    const char *program_info_uri;    //A UTF-8 formatted URL link used to present more information about Program_Info.
    uint16_t    audio_state;         //Audio Active State, 0x0000: no had this structures, 0xFFFF:mean is 0x00
    uint8_t     immediate_rendering; //Broadcast Audio Immediate Rendering Flag

    uint8_t  other_metadata_len; //
    uint8_t *other_metadata;     //use extended metadata
};

#define LEA_METADATA_PARSED_INVALID_LENGTH 0x01
#define LEA_METADATA_PARSED_UNSUPPORTED    0x02
#define LEA_METADATA_PARSED_REJECT         0x03
#define LEA_METADATA_PARSED_INVALID        0x04

struct lea_metadata_parsed
{
    uint16_t field_exist_flag;

    uint8_t failed_reason;
    uint8_t failed_type;

    uint16_t preferred_contexts; // metadata type  0x01
    uint16_t streaming_contexts; // metadata type  0x02

    const uint8_t *program_info; // metadata type  0x03

    uint32_t       language;  // metadata type  0x04
    const uint8_t *ccid_list; // metadata type  0x05

    uint8_t program_info_len;
    uint8_t ccid_list_len;
    uint8_t program_info_uri_len;
    uint8_t parental_rating; // metadata type  0x06

    const uint8_t *program_info_uri; // metadata type  0x07

    uint8_t audio_state; // metadata type  0x08
    // uint8_t immediate_rendering; // metadata type  0x09
    uint8_t extend_metadata_len;
    uint8_t vendor_specific_len;

    const uint8_t *extend_metadata; // metadata type  0xfe
    const uint8_t *vendor_specific; // metadata type  0xff
};

/**
 * @struct lea_codec_specific_config_parsed
 * @brief Structure to store the parsed codec-specific configuration parameters.
 */
struct lea_codec_specific_config_parsed
{
    uint16_t field_exist_flag; // Flag indicating which configuration fields have been parsed.

    uint8_t  frequency;                // Sampling frequency.
    uint8_t  duration;                 // Frame duration.
    uint32_t allocation;               // Audio channel allocation.
    uint16_t frame_octets;             // Octets per codec frame.
    uint8_t  codec_frame_blks_per_sdu; // Codec frame blocks per SDU.
}; // Generic audio: Codec Specific Configuration LTV structures.

/**
 *   @brief  Calculate the length of the Codec Specific Configuration parameter.
 *
 *   @param[in] codec_cfg  Codec Specific Configuration parameter.
 *
 *   @return Length of the Codec Specific Configuration parameter.
 */
uint8_t ble_lea_calculate_codec_specific_configuration_len(const struct lea_codec_spec_config_param *codec_cfg);

/**
 *   @brief  Set the Codec Specific Configuration parameter into a buffer.
 *
 *   @param[in] codec_cfg  Codec Specific Configuration parameter.
 *   @param[out] dst  Destination buffer.
 *
 *   @return Pointer to the end of the Codec Specific Configuration parameter in the buffer.
 */
uint8_t *ble_lea_set_codec_specific_configuration_into_buffer(const struct lea_codec_spec_config_param *codec_cfg, uint8_t *dst);

/**
 *   @brief  Calculate the length of the Codec Specific Capabilities parameter.
 *
 *   @param[in] codec_cap  Codec Specific Capabilities parameter.
 *
 *   @return Length of the Codec Specific Capabilities parameter.
 */
uint8_t ble_lea_calculate_codec_specific_capabilities_len(const struct lea_codec_spec_cap_param *codec_cap);

/**
 *   @brief  Set the Codec Specific Capabilities parameter into a buffer.
 *
 *   @param[in] codec_cap  Codec Specific Capabilities parameter.
 *   @param[out] dst  Destination buffer.
 *
 *   @return Pointer to the end of the Codec Specific Capabilities parameter in the buffer.
 */
uint8_t *ble_lea_set_codec_specific_capabilities_into_buffer(const struct lea_codec_spec_cap_param *codec_cap, uint8_t *dst);

/**
 *   @brief  Initialize the Codec Specific Configuration parameter.
 *
 *   @param[out] p_param  Pointer to the Codec Specific Configuration parameter.
 *
 *   @return None.
 */
void ble_lea_initialize_codec_specific_configuration(struct lea_codec_specific_config_parsed *p_param);

/**
 *   @brief  Parse the Codec Specific Configuration parameter.
 *
 *   @param[in] p_spec_cfg  Pointer to the Codec Specific Configuration parameter.
 *   @param[out] p_param  Pointer to the parsed parameters.
 *
 *   @return True if the Codec Specific Configuration parameter is valid, false otherwise.
 */
bool ble_lea_parse_codec_specific_configuration(const uint8_t *p_spec_cfg, struct lea_codec_specific_config_parsed *p_param);

/**
 *   @brief  Calculate the length of the LE Audio Metadata parameter.
 *
 *   @param[in] metadata  LE Audio Metadata parameter.
 *
 *   @return Length of the LE Audio Metadata parameter.
 */
uint8_t ble_lea_calculate_metadata_len(const struct lea_metadata_param *metadata);

/**
 *   @brief  Set the LE Audio Metadata parameter into a buffer.
 *
 *   @param[in] metadata  LE Audio Metadata parameter.
 *   @param[out] dst  Destination buffer.
 *
 *   @return Pointer to the end of the LE Audio Metadata parameter in the buffer.
 */
uint8_t *ble_lea_set_metadata_into_buffer(const struct lea_metadata_param *metadata, uint8_t *dst);

/**
 *   @brief Get the string format of the metadata type.
 *
 *   @param[in] type  Metadata type.
 *
 *   @return String format of the metadata type.
 */
const char *ble_metadata_get_type_string(uint8_t type);

/**
 *   @brief  Parse the LE Audio Metadata parameter.
 *
 *   @param[in] p_metadata  Pointer to the LE Audio Metadata parameter.
 *                       The parameter must be in the format of a total length + LTV (Length, Type, Value) sequence.
 *   @param[out] p_param  Pointer to the parsed parameters.
 *
 *   @return True if the LE Audio Metadata parameter is valid, false otherwise.
 *          if False, p_param->failed_reason and p_param->failed_type will be set.
 *          LEA_METADATA_PARSED_INVALID_LENGTH and LEA_METADATA_PARSED_UNSUPPORTED.
 */
bool ble_lea_parse_metadata(const uint8_t *p_metadata, struct lea_metadata_parsed *p_param);

/**
 *   @brief  Set the LE Audio Metadata parameter into a buffer.
 *
 *   @param[in] p_param  Pointer to the parsed parameters.
 *   @param[out] p_metadata  Pointer to the LE Audio Metadata parameter.
 *
 *   @return Length of the LE Audio Metadata parameter.
 */
uint8_t ble_lea_set_metadata(const struct lea_metadata_parsed *p_param, uint8_t *p_metadata);
