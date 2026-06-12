/********************************************************************************************************
 * @file    uuid128bit.h
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


#define BLE_HOST_128BIT_UUID_DEFINE                                                                                                        \
    /** < TELINK_SPP service */                                                                                                            \
    X(tlk_spp_service_att_uuid, 0x10, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)            \
    /** < TELINK_AUDIO service */                                                                                                          \
    X(tlk_audio_service_att_uuid, 0x11, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)          \
    /** < TELINK_OTA service */                                                                                                            \
    X(tlk_ota_service_att_uuid, 0x12, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)            \
    /** < TELINK_OTA V2 service */                                                                                                         \
    X(tlk_ota_v2_service_att_uuid, 0x13, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)         \
    /** < TELINK_MESH service */                                                                                                           \
    X(tlk_mesh_service_att_uuid, 0x20, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)           \
    /** < TELINK_MESH_LT service */                                                                                                        \
    X(tlk_mesh_lt_service_att_uuid, 0x21, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)        \
    /** < TELINK_RECORD service */                                                                                                         \
    X(tlk_record_service_att_uuid, 0x30, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)         \
    /** < TELINK_SPP data from server to client */                                                                                         \
    X(tlk_spp_server2client_char_att_uuid, 0x10, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00) \
    /** < TELINK_SPP data from client to server */                                                                                         \
    X(tlk_spp_client2server_char_att_uuid, 0x11, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00) \
    /** < TELINK_SPP data for ota */                                                                                                       \
    X(tlk_ota_data_att_uuid, 0x12, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)               \
    /** < TELINK_SPP data for pair */                                                                                                      \
    X(tlk_spp_data_pair_att_uuid, 0x13, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)          \
    /** < TELINK_SPP data for userdefine data */                                                                                           \
    X(tlk_spp_data_define_att_uuid, 0x14, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)        \
    /** < TELINK_SPP data for ota v2 */                                                                                                    \
    X(tlk_ota_v2_data_att_uuid, 0x15, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)            \
    /** < TELINK_MIC data */                                                                                                               \
    X(tlk_mic_data_att_uuid, 0x18, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)               \
    /** < TELINK_SPEAKER data */                                                                                                           \
    X(tlk_speaker_data_att_uuid, 0x19, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)           \
    /** < TELINK_RECORD command from APP */                                                                                                \
    X(tlk_record_cmd_att_uuid, 0x30, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)             \
    /** < TELINK_RECORD event from device */                                                                                               \
    X(tlk_record_event_att_uuid, 0x31, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)           \
    /** < TELINK_RECORD opus data from device */                                                                                           \
    X(tlk_record_data_att_uuid, 0x32, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)            \
    /** < TELINK_RECORD file data from device */                                                                                           \
    X(tlk_record_file_att_uuid, 0x33, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)            \
    /** < AUDIO_GOOGLE service */                                                                                                          \
    X(audio_google_service_uuid, 0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB)           \
    /** < AUDIO_GOOGLE TX characteristic */                                                                                                \
    X(audio_google_tx_char_uuid, 0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB)           \
    /** < AUDIO_GOOGLE RX characteristic */                                                                                                \
    X(audio_google_rx_char_uuid, 0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB)           \
    /** < AUDIO_GOOGLE CTL characteristic */                                                                                               \
    X(audio_google_ctl_char_uuid, 0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB)

#define X(att_name, ...) extern const struct att_uuid att_name;
BLE_HOST_128BIT_UUID_DEFINE
#undef X
