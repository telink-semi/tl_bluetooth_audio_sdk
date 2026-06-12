/********************************************************************************************************
 * @file    svc_audio.h
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


//Bluetooth LE Audio
//CSIP-CSIS
#define SERVICE_COORDINATED_SET_IDENTIFICATION_HDL SERVICE_LE_AUDIO_START_HDL
#define CSIS_MAX_HDL_NUM                           0x10
//AICS
#define SERVICE_AUDIO_INPUT_CONTROL_HDL SERVICE_COORDINATED_SET_IDENTIFICATION_HDL + CSIS_MAX_HDL_NUM
#define AICS_MAX_HDL_NUM                0x50
//VCS
#define SERVICE_VOLUME_CONTROL_HDL SERVICE_AUDIO_INPUT_CONTROL_HDL + AICS_MAX_HDL_NUM
#define VCS_MAX_HDL_NUM            0x20
#define SERVICE_VOCS_IN_VCS_HDL    SERVICE_VOLUME_CONTROL_HDL + VCS_MAX_HDL_NUM
#define VOCS_IN_VCS_MAX_HDL_NUM    0x40
//MCP-MCS
#define SERVICE_MEDIA_CONTROL_HDL SERVICE_VOCS_IN_VCS_HDL + VOCS_IN_VCS_MAX_HDL_NUM
#define MCS_MAX_HDL_NUM           0x40
//GMCS
#define SERVICE_GENERIC_MEDIA_CONTROL_HDL SERVICE_MEDIA_CONTROL_HDL + MCS_MAX_HDL_NUM
#define GMCS_MAX_HDL_NUM                  0x40
//OTS
#define SERVICE_OBJECT_TRANSFER_HDL SERVICE_GENERIC_MEDIA_CONTROL_HDL + GMCS_MAX_HDL_NUM
#define OTS_MAX_HDL_NUM             0x20
//CCP-TBS
#define SERVICE_TELEPHONE_BEARER_HDL SERVICE_OBJECT_TRANSFER_HDL + OTS_MAX_HDL_NUM
#define TBS_MAX_HDL_NUM              0x40
//MICP-MICS
#define SERVICE_MICROPHONE_CONTROL_HDL SERVICE_TELEPHONE_BEARER_HDL + TBS_MAX_HDL_NUM
#define MICS_MAX_HDL_NUM               0x10
//ASCS
#define SERVICE_AUDIO_STREAM_CONTROL_HDL SERVICE_MICROPHONE_CONTROL_HDL + MICS_MAX_HDL_NUM
#define ASCS_MAX_HDL_NUM                 0x40
//BASS
#define SERVICE_BROADCAST_AUDIO_SCAN_HDL SERVICE_AUDIO_STREAM_CONTROL_HDL + ASCS_MAX_HDL_NUM
#define BASS_MAX_HDL_NUM                 0x20
//PACS
#define SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL SERVICE_BROADCAST_AUDIO_SCAN_HDL + BASS_MAX_HDL_NUM
#define PACS_MAX_HDL_NUM                         0x20
//CAP-CAS
#define SERVICE_COMMON_AUDIO_HDL SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL + PACS_MAX_HDL_NUM
#define CAS_MAX_HDL_NUM          0x10
//HAP-HAS
#define SERVICE_HEARING_ACCESS_HDL SERVICE_COMMON_AUDIO_HDL + CAS_MAX_HDL_NUM
#define HAS_MAX_HDL_NUM            0x10
//TMAP-TMAS
#define SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL SERVICE_HEARING_ACCESS_HDL + HAS_MAX_HDL_NUM
#define TAMS_MAX_HDL_NUM                      0x10

//Constant Tone Extension
#define SERVICE_CONSTANT_TONE_EXTENSION_HDL SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL + TAMS_MAX_HDL_NUM
#define CTES_MAX_HDL_NUM                    0x00
//CCP-GTBS
#define SERVICE_GENERIC_TELEPHONE_BEARER_HDL SERVICE_CONSTANT_TONE_EXTENSION_HDL + CTES_MAX_HDL_NUM
#define GTBS_MAX_HDL_NUM                     0x40
//Device time
#define SERVICE_DEVICE_TIME_HDL SERVICE_GENERIC_TELEPHONE_BEARER_HDL + GTBS_MAX_HDL_NUM
#define DTS_MAX_HDL_NUM         0x00
