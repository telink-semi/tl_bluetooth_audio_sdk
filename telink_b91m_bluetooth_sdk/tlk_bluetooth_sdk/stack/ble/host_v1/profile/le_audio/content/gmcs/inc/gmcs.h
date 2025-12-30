/********************************************************************************************************
 * @file    gmcs.h
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

struct ble_object_id
{
    uint8_t objectId[6];
};

/** < enum for mcs media control opcode */
enum ble_mcs_media_control_opcode
{
    MCS_MEDIA_CONTROL_OPCODE_PLAY         = 0x01,
    MCS_MEDIA_CONTROL_OPCODE_PAUSE        = 0x02,
    MCS_MEDIA_CONTROL_OPCODE_FAST_REWIND  = 0x03,
    MCS_MEDIA_CONTROL_OPCODE_FAST_FORWARD = 0x04,
    MCS_MEDIA_CONTROL_OPCODE_STOP         = 0x05,

    MCS_MEDIA_CONTROL_OPCODE_MOVE_RELATIVE = 0x10,

    MCS_MEDIA_CONTROL_OPCODE_PREVIOUS_SEGMENT = 0x20,
    MCS_MEDIA_CONTROL_OPCODE_NEXT_SEGMENT     = 0x21,
    MCS_MEDIA_CONTROL_OPCODE_FIRST_SEGMENT    = 0x22,
    MCS_MEDIA_CONTROL_OPCODE_LAST_SEGMENT     = 0x23,
    MCS_MEDIA_CONTROL_OPCODE_GOTO_SEGMENT     = 0x24,

    MCS_MEDIA_CONTROL_OPCODE_PREVIOUS_TRACK = 0x30,
    MCS_MEDIA_CONTROL_OPCODE_NEXT_TRACK     = 0x31,
    MCS_MEDIA_CONTROL_OPCODE_FIRST_TRACK    = 0x32,
    MCS_MEDIA_CONTROL_OPCODE_LAST_TRACK     = 0x33,
    MCS_MEDIA_CONTROL_OPCODE_GOTO_TRACK     = 0x34,

    MCS_MEDIA_CONTROL_OPCODE_PREVIOUS_GROUP = 0x40,
    MCS_MEDIA_CONTROL_OPCODE_NEXT_GROUP     = 0x41,
    MCS_MEDIA_CONTROL_OPCODE_FIRST_GROUP    = 0x42,
    MCS_MEDIA_CONTROL_OPCODE_LAST_GROUP     = 0x43,
    MCS_MEDIA_CONTROL_OPCODE_GOTO_GROUP     = 0x44,
};

enum ble_mcs_search_control_opcode
{
    MCS_SEARCH_CONTROL_TYPE_TRACK_NAME = 0x01,
    MCS_SEARCH_CONTROL_TYPE_ARTIST_NAME,
    MCS_SEARCH_CONTROL_TYPE_ALBUM_NAME,
    MCS_SEARCH_CONTROL_TYPE_GROUP_NAME,
    MCS_SEARCH_CONTROL_TYPE_EARLIEST_YEAR,
    MCS_SEARCH_CONTROL_TYPE_LATEST_YEAR,
    MCS_SEARCH_CONTROL_TYPE_GENRE,
    MCS_SEARCH_CONTROL_TYPE_ONLY_TRACKS,
    MCS_SEARCH_CONTROL_TYPE_ONLY_GROUPS,
    MCS_SEARCH_CONTROL_TYPE_RFU,
};
