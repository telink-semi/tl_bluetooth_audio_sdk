/********************************************************************************************************
 * @file    bap_us.h
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

struct ad_data_lea_bap_announcement
{
    uint8_t  length;  /** < 9 + metadata length */
    uint8_t  ad_type; /** < fixed type of DT_SERVICE_DATA */
    uint16_t uuid;    /** < Fixed UUID of SERVICE_UUID_AUDIO_STREAM_CONTROL */
    uint8_t  type;    /** < Refer value to enum lea_bap_announcement_type */

    struct
    {
        uint16_t sink_available_contexts;
        uint16_t source_available_contexts;
    };

    uint8_t metadata_length; // usually 0, if non-zero, the metadata follows the structure
    uint8_t metadata[20];
} __attribute__((packed));

enum ble_bap_us_event_id
{
    BAP_US_EVENT_ID_CODEC_STATE,
    BAP_US_EVENT_ID_OPEN_INPUT,
    BAP_US_EVENT_ID_OPEN_OUTPUT,
    BAP_US_EVENT_ID_CLOSE_INPUT,
    BAP_US_EVENT_ID_CLOSE_OUTPUT,
    BAP_US_EVENT_ID_DEINIT_CODEC_STATE,
};

struct ble_bap_us_event_codec_state
{
    uint8_t  input_enable;     /** < 0: disable, 1: enable */
    uint8_t  output_enable;    /** < 0: disable, 1: enable */
    uint8_t  input_frequency;  /** < refer to enum lea_select_sampling_freq */
    uint8_t  output_frequency; /** < refer to enum lea_select_sampling_freq */
    uint32_t input_location;   /** < refer to enum lea_location_flag  */
    uint32_t output_location;  /** < refer to enum lea_location_flag  */
};

struct ble_bap_us_event_open_codec
{
    uint8_t  blocks;
    uint8_t  sampling_frequency;
    uint8_t  frame_duration;
    uint16_t frame_octets;
    uint16_t cis_handle;

    uint32_t location;
    uint32_t presentation_delay;
};

struct ble_bap_us_event_close_codec
{
    uint16_t cis_handle;
};

typedef void (*ble_bap_us_event_callback)(uint16_t conn_handle, enum ble_bap_us_event_id event, const void *event_msg);

/**
 *   @brief Initialise LE Audio Unicast Server(BAP layer) role.
 *
 *   @param[in] callback  Callback function to receive events from BAP layer.
 *
 *   The high layer should initialize the server module of ASCS and PACS by themselves.
 */
void ble_lea_initial_bap_unicast_server_role(ble_bap_us_event_callback callback);
