/********************************************************************************************************
 * @file    tlksys_define.h
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
#pragma once

#include "core/mcu_type.h"
#if MCU_CORE_TYPE == MCU_CORE_TL752X
#define _attribute_extend_low_speed_bss_sec_ __attribute__((section(".extend_low_speed_bss")))
#else
#define _attribute_extend_low_speed_bss_sec_
#endif

#ifndef TLKSYS_TASK_EXTRA_STATIC_BUFFER_SIZE
#define TLKSYS_TASK_EXTRA_STATIC_BUFFER_SIZE (400)
#endif

typedef enum
{
    TLKSYS_MSG_HOST_TYPE_BT = 0x00,
    TLKSYS_MSG_HOST_TYPE_LE,
    TLKSYS_MSG_HOST_TYPE_TPH,
    TLKSYS_MSG_HOST_TYPE_TPT,
    TLKSYS_MSG_HOST_TYPE_TPD,
    TLKSYS_MSG_HOST_TYPE_TPMD,
} TLKSYS_MSG_HOST_TYPE_ENUM;

typedef enum
{
    TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG = 0x00,
    TLKSYS_MSG_BT_HOST_EVT_TYPE_AUD_STATE_CHG,
} TLKSYS_MSG_BT_HOST_EVT_TYPE_ENUM;

typedef enum
{
    TLKSYS_MSG_TPS_HOST_EVT_TYPE_AUD_STATE_CHG = 0x00,
} TLKSYS_MSG_TPS_HOST_EVT_TYPE_ENUM;

typedef struct
{
    uint8_t  hostType;
    uint8_t  msgID;
    uint16_t dataLen;
    uint8_t  data[0];
} tlksys_msg_hostEvt_t;

typedef struct
{
    uint8_t  isMusic;
    uint8_t  volume;
    uint16_t handle;
} tlksys_msg_hostEvt_btVolChg_t;

typedef struct
{
    uint8_t  isMusic;
    uint8_t  state;
    uint8_t  codecType;
    uint8_t  resv;
    uint16_t handle;
    uint16_t scoHandle;
} tlksys_msg_hostEvt_btAudStateChg_t;

typedef struct
{
    uint16_t mode;
} tlksys_msg_hostEvt_tpsAudStateChg_t;

typedef enum
{
    TLKAUD_TYPE_TONE, // Play the prompt tone locally
    TLKAUD_TYPE_CC_BT_VOICE,
    TLKAUD_TYPE_CC_BT_MUSIC,
    TLKAUD_TYPE_TPH_AUDIO,
    TLKAUD_TYPE_A2DP_OUT,
    TLKAUD_TYPE_LEA_UC_MUSIC, // LE Audio unicast client (LE phone) with music
    TLKAUD_TYPE_LEA_UC_VOICE, // LE Audio unicast client (LE phone) with voice
    TLKAUD_TYPE_LEA_US_MUSIC, // LE Audio unicast server (LE earphone) with music
    TLKAUD_TYPE_LEA_US_VOICE, // LE Audio unicast server (LE earphone) with voice
    TLKAUD_TYPE_LEA_BMS,      // LE Audio Broadcast Media Sender(BAP Broadcast source).
    TLKAUD_TYPE_LEA_BMR,      // LE Audio Broadcast Media Receiver(BAP Broadcast sink).
    TLKAUD_TYPE_U2H_VOICE,
    TLKAUD_TYPE_A2DP_TO_BIS,
    TLKAUD_TYPE_HRA,
    TLKAUD_TYPE_SIDETONE,
    TLKAUD_TYPE_INTRTPHONE,
    TLKAUD_TYPE_RECORDING_CARD,
    TLKAUD_TYPE_ANC,
    TLKAUD_TYPE_UAC_AUD,
    TLKAUD_TYPE_TPD_AUDIO,
    TLKAUD_TYPE_UAC_LOCAL_AUDIO,
    TLKAUD_TYPE_BT_VOICE_FORWARD, //BT 1_TO_2_FORWARD
    TLKAUD_TYPE_BT_MUSIC_FORWARD,

    TLKAUD_TYPE_MAX,
} TLKAUD_TYPE_ENUM;

typedef enum
{
    TLKAUD_OPCODE_NONE = 0,
    TLKAUD_OPCODE_PLAY_START,
    TLKAUD_OPCODE_PLAY_CLOSE,
    TLKAUD_OPCODE_PLAY_NEXT,
    TLKAUD_OPCODE_PLAY_PREV,
    TLKAUD_OPCODE_ISBUSY,
    TLKAUD_OPCODE_TIMER,
    TLKAUD_OPCODE_VOLUME_INC,
    TLKAUD_OPCODE_VOLUME_DEC,
    TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX,
    TLKAUD_OPCODE_TONE_MIX_ENABLE,
    TLKAUD_OPCODE_TONE_MIX_DISABLE,
    TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE,
    TLKAUD_OPCODE_TYPE_GET_TYPE,
    TLKAUD_OPCODE_CALL_ACCEPT,
    TLKAUD_OPCODE_CALL_HUNGUP,
    TLKAUD_OPCODE_IS_PLAY_RING,
    TLKAUD_OPCODE_IS_CUSTOMIZED_PLAYPAUSE,
    TLKAUD_OPCODE_TRIGGER_CUSTOMIZED_PLAYPAUSE,
    TLKAUD_OPCODE_MAX
} TLKAUD_OPCODE_ENUM;

typedef enum
{
    TLKAUD_START_MODE_NONE           = 0,
    TLKAUD_START_MODE_PLAY_PREV      = 0X4,
    TLKAUD_START_MODE_PLAY_NEXT      = 0X8,
    TLKAUD_START_MODE_PLAY_WITH_TIME = 0XF,
} TLKAUD_START_MODE_ENUM;

#define TLKPTI_SYS_SERIAL_HEADLEN 6

typedef enum
{
    TLKPRT_COMM_RSP_STATUE_SUCCESS = 0,
    TLKPRT_COMM_RSP_STATUE_FAILURE,
    TLKPRT_COMM_RSP_STATUE_COMPLETE, // Completed, but no results yet.
} TLKPRT_COMM_RSP_STATUE_ENUM;

typedef enum
{
    TLKPRT_COMM_SYS_DAT_PORT = 0,
    TLKPRT_COMM_OTA_DAT_PORT,

    TLKPRT_COMM_DAT_PORT_NUM
} TLKPRT_COMM_DAT_PORT_ENUM;

typedef enum
{
    TLKPRT_COMM_SYS_DAT_NONE  = 0x00,
    TLKPRT_COMM_SYS_DAT_LOG   = 0x01,
    TLKPRT_COMM_SYS_DAT_VCD   = 0x02,
    TLKPRT_COMM_SYS_DAT_OTA   = 0x03, // FW-Firmware
    TLKPRT_COMM_SYS_DAT_COVER = 0x04, // Only used in bip
} TLKPRT_COMM_SYS_DAT_ENUM;

typedef enum
{
    TLKPRT_COMM_BT_CHN_ACL            = 0x00,
    TLKPRT_COMM_BT_CHN_SCO            = 0x01,
    TLKPRT_COMM_BT_CHN_SPP            = 0x10,
    TLKPRT_COMM_BT_CHN_HFP_HF         = 0x11,
    TLKPRT_COMM_BT_CHN_HFP_AG         = 0x12,
    TLKPRT_COMM_BT_CHN_A2DP_SNK       = 0x13,
    TLKPRT_COMM_BT_CHN_A2DP_SRC       = 0x14,
    TLKPRT_COMM_BT_CHN_AVRCP          = 0x15,
    TLKPRT_COMM_BT_CHN_MAP            = 0x16,
    TLKPRT_COMM_BT_CHN_PBAP           = 0x17,
    TLKPRT_COMM_BT_CHN_ATT            = 0x21,
    TLKPRT_COMM_BT_CHN_HID            = 0x22,
    TLKPRT_COMM_BT_CHN_IAP            = 0x23,
    TLKPRT_COMM_BT_CHN_AVRCP_BROWSING = 0x24,
    TLKPRT_COMM_BT_CHN_COVER_ART      = 0x25,
} TLKPRT_COMM_BT_CHN_ENUM;

// Packet Type
typedef enum
{
    TLKPRT_COMM_PTYPE_CMD = 0x00,
    TLKPRT_COMM_PTYPE_RSP = 0x01,
    TLKPRT_COMM_PTYPE_EVT = 0x02,
    TLKPRT_COMM_PTYPE_DAT = 0x03,
    TLKPRT_COMM_PTYPE_FCS = 0x0F,
} TLKPRT_COMM_PTYPE_ENUM;

// Module Type
typedef enum
{
    TLKPRT_COMM_MTYPE_NONE  = 0x00, // For Inner Used
    TLKPRT_COMM_MTYPE_SYS   = 0x01,
    TLKPRT_COMM_MTYPE_BT    = 0x02,
    TLKPRT_COMM_MTYPE_LE    = 0x03,
    TLKPRT_COMM_MTYPE_DBG   = 0x04,
    TLKPRT_COMM_MTYPE_FILE  = 0x05,
    TLKPRT_COMM_MTYPE_CALL  = 0x06,
    TLKPRT_COMM_MTYPE_AUDIO = 0x07,
    TLKPRT_COMM_MTYPE_MAX,
} TLKPRT_COMM_MTYPE_ENUM;

// Note : some msgID bind to Protocol's code,can't change
typedef enum
{

    TLKSYS_SYS_MSGID_RUN_FUNC = 0,
    TLKSYS_SYS_MSGID_VERSION,
    TLKSYS_SYS_MSGID_SERIAL_SEND,
    TLKSYS_SYS_MSGID_KEY_ACTION,
    TLKSYS_SYS_MSGID_POWEROFF,
    TLKSYS_SYS_MSGID_CLEAN_SQL    = 0x12,
    TLKSYS_SYS_MSGID_SET_USB_MODE = 0x13,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_1 = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_2,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_3,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_4,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_5,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_6,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_7,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_8,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_9,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_10,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_11,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_12,
    TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_END,
    TLKSYS_SYS_MSGID_PM_STATE_CHANGE,
    TLKSYS_SYS_MSGID_PM_CLEAR_USB_SUSPEND,

    TLKSYS_HOST_MSGID_RUN_FUNC = 0,
    TLKSYS_BT_MSGID_CONNECT    = 0x01,
    TLKSYS_BT_MSGID_DISCONN    = 0x02,
    TLKSYS_BT_MSGID_CONN_PROF  = 0x03,
    TLKSYS_BT_MSGID_DISC_PROF  = 0x04,
    TLKSYS_BT_MSGID_INQUIRY    = 0x05,
    TLKSYS_BT_MSGID_INQ_CANCEL = 0x06,
    TLKSYS_BT_MSGID_GET_NAME   = 0x11,
    TLKSYS_BT_MSGID_GET_ADDR,
    TLKSYS_BT_MSGID_SET_NAME,
    TLKSYS_BT_MSGID_SET_ADDR,
    TLKSYS_BT_MSGID_VOL_INC,
    TLKSYS_BT_MSGID_VOL_DEC,
    TLKSYS_BT_MSGID_GET_LINKEY,
    TLKSYS_BT_MSGID_GET_PEER_NAME,
    TLKSYS_BT_MSGID_GET_PDL = 0x21,
    TLKSYS_BT_MSGID_DEL_PDL,
    TLKSYS_BT_MSGID_CLS_PDL,
    TLKSYS_BT_MSGID_GET_CDL,

    TLKSYS_BT_MSGID_START_PAIR = 0x31,
    TLKSYS_BT_MSGID_CLOSE_PAIR,
    TLKSYS_BT_MSGID_TRIGGER_SIRI,
    TLKSYS_BT_MSGID_SET_HFP_VOLUME,
    TLKSYS_BT_MSGID_SET_AVRCP_VOLUME,
    TLKSYS_BT_MSGID_SET_SCAN,
    TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT,
    TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP,
    TLKSYS_BT_MSGID_AVRCP_SEND_KEY,
    TLKSYS_BT_MSGID_HID_MODE_SWITCH,
    TLKSYS_BT_MSGID_HID_XINPUT_TEST,

    TLKSYS_TPH_MSGID_BEGIN   = (TLKSYS_MSG_HOST_TYPE_TPH << 8),
    TLKSYS_TPH_MSGID_3S_PAIR = TLKSYS_TPH_MSGID_BEGIN,
    TLKSYS_TPH_MSGID_10S_PAIR,
    TLKSYS_TPH_MSGID_SEND_KEY,
    TLKSYS_TPH_MSGID_ENTER_LOW_LATENCY_MODE,

    TLKSYS_TPT_MSGID_BEGIN   = (TLKSYS_MSG_HOST_TYPE_TPT << 8),
    TLKSYS_TPT_MSGID_3S_PAIR = TLKSYS_TPT_MSGID_BEGIN,
    TLKSYS_TPT_MSGID_10S_PAIR,
    TLKSYS_TPT_MSGID_ENTER_LOW_LATENCY_MODE,
    TLKSYS_TPT_MSGID_START_HANDOVER,
    TLKSYS_TPT_MSGID_SEND_KEY,
    TLKSYS_TPT_MSGID_SHUT_DOWN,

    TLKSYS_TPD_MSGID_BEGIN      = (TLKSYS_MSG_HOST_TYPE_TPD << 8),
    TLKSYS_TPD_MSGID_START_PAIR = TLKSYS_TPD_MSGID_BEGIN,
    TLKSYS_TPD_MSGID_SWITCH_USB_MODE,

    TLKSYS_TPMD_MSGID_BEGIN      = (TLKSYS_MSG_HOST_TYPE_TPMD << 8),
    TLKSYS_TPMD_MSGID_START_PAIR = TLKSYS_TPMD_MSGID_BEGIN,
    TLKSYS_TPMD_MSGID_SWITCH_USB_MODE,

    TLKSYS_LE_MSGID_START_EXT_SCAN     = 0x01,
    TLKSYS_LE_MSGID_STOP_EXT_SCAN      = 0x02,
    TLKSYS_LE_MSGID_SET_SCAN_FILTER    = 0x03,
    TLKSYS_LE_MSGID_CONNECT            = 0x04,
    TLKSYS_LE_MSGID_DISCONNECT         = 0x05,
    TLKSYS_LE_MSGID_OPEN_REC           = 0x06,
    TLKSYS_LE_MSGID_CLOSED_REC         = 0x07,
    TLKSYS_LE_MSGID_GET_BOUND_TABLE    = 0x08,
    TLKSYS_LE_MSGID_DELETE_BOUND_TABLE = 0x09,
    TLKSYS_LE_MSGID_DELETE_BOUND_ITEM  = 0x0A,

    TLKSYS_AUD_MSGID_RUN_FUNC       = 0,
    TLKSYS_AUD_MSGID_PLAY_START     = 0x01,
    TLKSYS_AUD_MSGID_PLAY_CLOSE     = 0x02,
    TLKSYS_AUD_MSGID_PLAY_PAUSE     = 0x03,
    TLKSYS_AUD_MSGID_PLAY_NEXT      = 0x05,
    TLKSYS_AUD_MSGID_PLAY_PREV      = 0x06,
    TLKSYS_AUD_MSGID_START_EVT      = 0x07,
    TLKSYS_AUD_MSGID_CLOSE_EVT      = 0x08,
    TLKSYS_AUD_MSGID_START_TONE_CMD = 0x09,
    TLKSYS_AUD_MSGID_HOST_EVT_COME  = 0x0a,
    TLKSYS_AUD_MSGID_TASK_OPERATE   = 0X0b,
    TLKSYS_AUD_MSGID_INEAR_STATE_CHANGE,
    TLKSYS_AUD_MSGID_INC_VOLUME = 0x25,
    TLKSYS_AUD_MSGID_DEC_VOLUME,
    TLKSYS_AUD_MSGID_CALL_ACCEPT,
    TLKSYS_AUD_MSGID_CALL_HUNGUP,
    TLKSYS_AUD_MSGID_RING_PLAY,
    TLKSYS_AUD_MSGID_DONGLE_SWITCH,
    TLKSYS_AUD_MSGID_SWITCH_MIC,
    TLKSYS_AUD_MSGID_TOGGLE_ANC,
    TLKSYS_AUD_MSGID_ENABLE_ANC,
} TLKSYS_THREAD_MSGID_ENUM;

typedef enum
{
    // TLKPRT_COMM_MTYPE_SYS
    TLKPRT_COMM_EVTID_SYS_READY = 0x01,

    // TLKPRT_COMM_MTYPE_BT
    TLKPRT_COMM_EVTID_BT_CONNECT          = 0x01, // ACL,SCO
    TLKPRT_COMM_EVTID_BT_DISCONN          = 0x02,
    TLKPRT_COMM_EVTID_BT_PROF_CONN        = 0x03, // SPP,HFP(HF/AG),MAP,PBAP,A2DP(TG/AG)
    TLKPRT_COMM_EVTID_BT_PROF_DISC        = 0x04,
    TLKPRT_COMM_EVTID_BT_INQUIRY          = 0x05,
    TLKPRT_COMM_EVTID_BT_INQUIRY_COMPLETE = 0x06,

    // TLKPRT_COMM_MTYPE_LE
    TLKPRT_COMM_EVTID_LE_EXT_SCAN_Data = 0x01,
    TLKPRT_COMM_EVTID_LE_SCAN_START    = 0x02,
    TLKPRT_COMM_EVTID_LE_SCAN_COMPLETE = 0x03,
    TLKPRT_COMM_EVTID_LE_CONNECT       = 0x04,
    TLKPRT_COMM_EVTID_LE_DISCONN       = 0x05,
    TLKPRT_COMM_EVTID_LE_ADV_START     = 0x06,
    TLKPRT_COMM_EVTID_LE_ADV_COMPLETE  = 0x07,
    TLKPRT_COMM_EVTID_LE_ADV_REPORT    = 0x08,

    //TLKPRT_COMM_MTYPE_AUDIO Just test for CoverArt.
    TLKPRT_COMM_EVTID_AUDIO_LYRICS_REPORT = 0x05,


} TLKPRT_COMM_EVTID_ENUM;

typedef enum
{
    TLKSYS_PM_CHN_DEVICE = 0,
    TLKSYS_PM_CHN_SYS,
    TLKSYS_PM_CHN_OTA,
    TLKSYS_PM_CHN_BT_TPT,
    TLKSYS_PM_CHN_AUD,
    TLKSYS_PM_CHN_CODEC,
    TLKSYS_PM_CHN_DSP,
    TLKSYS_PM_CHN_MSC,
    TLKSYS_PM_CHN_APP0,
    TLKSYS_PM_CHN_APP1,
    TLKSYS_PM_CHN_APP2,
    TLKSYS_PM_CHN_APP3,
} TLKSYS_PM_CHN_ENUM;

typedef enum
{
    TLKSYS_CLOCK_CHN_AUDIO = 1,
    TLKSYS_CLOCK_CHN_NN_ALG,
    TLKSYS_CLOCK_CHN_APP0,
    TLKSYS_CLOCK_CHN_APP1,
    TLKSYS_CLOCK_CHN_APP2,
    TLKSYS_CLOCK_CHN_APP3,
} TLKSYS_CLOCK_CHN_ENUM;

typedef void (*tlksys_requestRunFunc_t)(void);

typedef void (*tlksys_requestRunFuncWithArg_t)(void *pData, uint8_t dataLen);

/**
 * @brief run function in task
 * @param[in] taskId task id
 * @param[in] func run function
 * @return none
*/
static inline void tlksys_runFuncInTask(uint16_t taskId, tlksys_requestRunFunc_t func)
{
    tlksys_sendMsg(taskId, 0, &func, sizeof(tlksys_requestRunFunc_t));
}

/**
 * @brief run function in task with arg
 * @param[in] taskId task id
 * @param[in] func run function
 * @param[in] pData run function arg
 * @param[in] dataLen run function arg len
 * @return none
*/
static inline void tlksys_runFuncInTaskWithArg(uint16_t taskId, tlksys_requestRunFuncWithArg_t func, void *pData, uint8_t dataLen)
{
    uint8_t buffer[260];
    memcpy(buffer, &func, sizeof(tlksys_requestRunFuncWithArg_t));
    memcpy(buffer + sizeof(tlksys_requestRunFuncWithArg_t), pData, dataLen);
    tlksys_sendMsg(taskId, 0, buffer, dataLen + sizeof(tlksys_requestRunFuncWithArg_t));
}