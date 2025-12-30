/********************************************************************************************************
 * @file    tlk_debug.h
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
#ifndef TLK_DEBUG_H
#define TLK_DEBUG_H

#define TLK_DEBUG_DBG_FLAG_WARN   0x02
#define TLK_DEBUG_DBG_FLAG_INFO   0x04
#define TLK_DEBUG_DBG_FLAG_TRACE  0x08
#define TLK_DEBUG_DBG_FLAG_ERROR  0x10
#define TLK_DEBUG_DBG_FLAG_FATAL  0x20
#define TLK_DEBUG_DBG_FLAG_ARRAY  0x40
#define TLK_DEBUG_DBG_FLAG_ASSERT 0x80
#define TLK_DEBUG_DBG_FLAG_ALL    0xFE

typedef enum
{
    TLK_MAJOR_DBGID_BTH = 0,
    TLK_MAJOR_DBGID_BTP,
    TLK_MAJOR_DBGID_MW,
    TLK_MAJOR_DBGID_MDI_AUDIO,
    TLK_MAJOR_DBGID_MDI_BT,
    TLK_MAJOR_DBGID_MAX,
} TLK_DEBUG_MAJOR_ID_ENUM;

typedef enum
{
    TLK_MINOR_DBGID_NONE = 0,
    // TLK_MAJOR_DBGID_BTH
    TLK_MINOR_DBGID_BTH = 0,
    TLK_MINOR_DBGID_BTH_ACL,
    TLK_MINOR_DBGID_BTH_SCO,
    TLK_MINOR_DBGID_BTH_CMD, // HCI-CMD
    TLK_MINOR_DBGID_BTH_EVT, // HCI-EVT
    TLK_MINOR_DBGID_BTH_L2C,
    TLK_MINOR_DBGID_BTH_SIG,
    TLK_MINOR_DBGID_BTH_MAX,
    // TLK_MAJOR_DBGID_BTP
    TLK_MINOR_DBGID_BTP = 0,
    TLK_MINOR_DBGID_BTP_SDP,
    TLK_MINOR_DBGID_BTP_SDPC,
    TLK_MINOR_DBGID_BTP_SDPS,
    TLK_MINOR_DBGID_BTP_RFCOMM,
    TLK_MINOR_DBGID_BTP_IAP,
    TLK_MINOR_DBGID_BTP_SPP,
    TLK_MINOR_DBGID_BTP_ATT,
    TLK_MINOR_DBGID_BTP_A2DP,
    TLK_MINOR_DBGID_BTP_A2DPS,
    TLK_MINOR_DBGID_BTP_A2DPC,
    TLK_MINOR_DBGID_BTP_HID,
    TLK_MINOR_DBGID_BTP_HIDS,
    TLK_MINOR_DBGID_BTP_HIDC,
    TLK_MINOR_DBGID_BTP_HFP,
    TLK_MINOR_DBGID_BTP_HFPAG,
    TLK_MINOR_DBGID_BTP_HFPHF,
    TLK_MINOR_DBGID_BTP_AVRCP,
    TLK_MINOR_DBGID_BTP_OBEX,
    TLK_MINOR_DBGID_BTP_PBAP,
    TLK_MINOR_DBGID_BTP_BROWSE,
    TLK_MINOR_DBGID_BTP_COVERART,
    TLK_MINOR_DBGID_BTP_TWS,
    TLK_MINOR_DBGID_BTP_MAX,
    // TLK_MAJOR_DBGID_MW
    TLK_MINOR_DBGID_MW_CODEC = 0,
    TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET,
    TLK_MINOR_DBGID_MW_MFI,
    TLK_MINOR_DBGID_MW_MAX,
    // TLK_MAJOR_DBGID_MDI_AUDIO
    TLK_MINOR_DBGID_MDI_AUD = 0,
    TLK_MINOR_DBGID_MDI_AUD_PLAY,
    TLK_MINOR_DBGID_MDI_AUD_TONE,
    TLK_MINOR_DBGID_MDI_AUD_MP3,
    TLK_MINOR_DBGID_MDI_AUD_HFP,
    TLK_MINOR_DBGID_MDI_AUD_SCO,
    TLK_MINOR_DBGID_MDI_AUD_SNK,
    TLK_MINOR_DBGID_MDI_AUD_SRC,
    TLK_MINOR_DBGID_MDI_AUD_UAC,
    TLK_MINOR_DBGID_MDI_AUD_DSCO,
    TLK_MINOR_DBGID_MDI_AUD_RELAY,
    TLK_MINOR_DBGID_MDI_AUD_CC_BT_AUDIO,
    TLK_MINOR_DBGID_MDI_AUD_CC_LL_AUDIO,
    TLK_MINOR_DBGID_MDI_AUD_U2S,
    TLK_MINOR_DBGID_MDI_AUD_LEA_UC,  // LE Audio unicast client.
    TLK_MINOR_DBGID_MDI_AUD_LEA_US,  // LE Audio unicast server.
    TLK_MINOR_DBGID_MDI_AUD_LEA_BMR, // LE Audio unicast server.
    TLK_MINOR_DBGID_MDI_AUD_LEA_BMS, // LE Audio unicast server.
    TLK_MINOR_DBGID_MDI_AUD_U2H_VOICE,
    TLK_MINOR_DBGID_MDI_AUD_MAX,
    // TLK_MAJOR_DBGID_MDI_BT
    TLK_MINOR_DBGID_MDI_BT = 0,
    TLK_MINOR_DBGID_MDI_BT_ACL,
    TLK_MINOR_DBGID_MDI_BT_ATT,
    TLK_MINOR_DBGID_MDI_BT_HFP,
    TLK_MINOR_DBGID_MDI_BT_HID,
    TLK_MINOR_DBGID_MDI_BT_INQ,
    TLK_MINOR_DBGID_MDI_BT_REC,
    TLK_MINOR_DBGID_MDI_BT_IAP,
    TLK_MINOR_DBGID_MDI_BT_A2DP,
    TLK_MINOR_DBGID_MDI_BT_SCO,
    TLK_MINOR_DBGID_MDI_BT_AG,
    TLK_MINOR_DBGID_MDI_BT_PBAP,
    TLK_MINOR_DBGID_MDI_BT_MAX,
} TLK_DEBUG_MINOR_ID_ENUM;

void tlk_debug_init(void);
void tlk_debug_logLoad(void);


#endif // TLK_DEBUG_H
