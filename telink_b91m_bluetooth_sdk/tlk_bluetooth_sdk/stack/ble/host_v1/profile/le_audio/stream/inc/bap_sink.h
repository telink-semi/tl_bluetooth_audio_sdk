/********************************************************************************************************
 * @file    bap_sink.h
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

enum ble_bap_sink_event_id
{
    BAP_SINK_EVENT_ID_CODEC_PARAM,
    BAP_SINK_EVENT_ID_START,
    BAP_SINK_EVENT_ID_STOP,
};

struct ble_bap_sink_event_codec_param
{
    uint8_t  frequency;
    uint8_t  frame_duration;
    uint16_t frame_octets;
    uint32_t all_location;
    uint32_t presentation_delay;
    uint8_t  num;

    struct
    {
        uint8_t  blocks;
        uint32_t location;
    } param[31];
};

struct ble_bap_sink_event_start
{
    uint8_t  num;
    uint16_t bis_handle[31];
};

typedef void (*ble_bap_sink_event_callback)(enum ble_bap_sink_event_id event, const void *event_msg);

/**
 *   @brief Initialise LE Audio Broadcast Sink(BAP layer) role.
 *
 *   @param[in] callback  Callback function to receive events from BAP layer.
 *
 *   The high layer should initialize the server module of BASS and PACS by themselves.
 */
void ble_lea_initial_bap_broadcast_sink_role(ble_bap_sink_event_callback callback);

void ble_lea_bap_sink_close_all(void);

struct ble_bap_sink_base_info_bis_info
{
    uint8_t                                 bis_index;
    struct lea_codec_id                     codec_id;
    struct lea_codec_specific_config_parsed codec_cfg;
    struct lea_metadata_parsed              metadata;
};

struct ble_bap_sink_base_info
{
    uint32_t                               presentation_delay;
    uint8_t                                num;
    struct ble_bap_sink_base_info_bis_info bis[31];
};

/**
 * @brief       Parses the base information from the broadcast data.
 * @param[in]   base - pointer to the broadcast data.
 * @param[in]   length - length of the broadcast data.
 * @param[out]  base_info - pointer to the base information structure.
 * @return      true if the base information is parsed successfully, false otherwise.
 * @note        The base information is parsed from the broadcast data and stored in the base information structure.
 */
bool ble_lea_bap_sink_parse_base_info(const uint8_t *base, uint8_t length, struct ble_bap_sink_base_info *base_info);