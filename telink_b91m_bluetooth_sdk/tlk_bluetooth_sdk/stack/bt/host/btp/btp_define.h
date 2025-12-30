/********************************************************************************************************
 * @file    btp_define.h
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
#ifndef BTP_DEFINE_H
#define BTP_DEFINE_H

typedef enum
{
    BTP_PTYPE_SDP            = 0,
    BTP_PTYPE_RFC            = 1,
    BTP_PTYPE_HFP            = 2,
    BTP_PTYPE_IAP            = 3,
    BTP_PTYPE_SPP            = 4,
    BTP_PTYPE_HID            = 5,
    BTP_PTYPE_ATT            = 6,
    BTP_PTYPE_A2DP           = 7,
    BTP_PTYPE_AVRCP          = 8,
    BTP_PTYPE_PBAP           = 9,
    BTP_PTYPE_AVRCP_BROWSING = 10,
    BTP_PTYPE_PTS_L2C        = 11,
    BTP_PTYPE_COVERART       = 12,

    BTP_PTYPE_CUSTP          = 0xF0,
    BTP_PTYPE_MAX,
} BTP_PTYPE_ENUM; // Profile Type

typedef enum
{
    BTP_PFLAG_SDP   = (1 << BTP_PTYPE_SDP),
    BTP_PFLAG_RFC   = (1 << BTP_PTYPE_RFC),
    BTP_PFLAG_HFP   = (1 << BTP_PTYPE_HFP),
    BTP_PFLAG_IAP   = (1 << BTP_PTYPE_IAP),
    BTP_PFLAG_SPP   = (1 << BTP_PTYPE_SPP),
    BTP_PFLAG_ATT   = (1 << BTP_PTYPE_ATT),
    BTP_PFLAG_A2DP  = (1 << BTP_PTYPE_A2DP),
    BTP_PFLAG_AVRCP = (1 << BTP_PTYPE_AVRCP),
    BTP_PFLAG_PBAP  = (1 << BTP_PTYPE_PBAP),
    BTP_PFLAG_BIP   = (1 << BTP_PTYPE_COVERART),

    BTP_PFLAG_OBEX_MASK   = BTP_PFLAG_PBAP,
    BTP_PFLAG_L2CAP_MASK  = BTP_PFLAG_ATT | BTP_PFLAG_A2DP | BTP_PFLAG_AVRCP,
    BTP_PFLAG_RFCOMM_MASK = BTP_PFLAG_HFP | BTP_PFLAG_IAP | BTP_PFLAG_SPP | BTP_PFLAG_OBEX_MASK,
    BTP_PFLAG_TWS_DEF_PROF_MASK = BTP_PFLAG_HFP | BTP_PFLAG_A2DP | BTP_PFLAG_AVRCP,
} BTP_PFLAG_ENUM;

typedef enum
{
    BTP_PSMID_SDP            = 0x0001,
    BTP_PSMID_RFCOMM         = 0x0003,
    BTP_PSMID_HIDC           = 0x0011,
    BTP_PSMID_HIDI           = 0x0013,
    BTP_PSMID_UPNP           = 0x0015,
    BTP_PSMID_AVCTP          = 0x0017,
    BTP_PSMID_AVDTP          = 0x0019,
    BTP_PSMID_ATT            = 0x001F,
    BTP_PSMID_AIRPODS        = 0x1001,
    BTP_PSMID_AVCTP_BROWSING = 0x001B,
    BTP_PSMID_EATT           = 0x0027,
    BTP_PSMID_PROP_SIGNAL    = 0x1003,  // Proprietary Signal
    
    BTP_PSMID_PTS_L2C = 0x0EF1, // temp for PTS
} BTP_PSMID_ENUM;

typedef enum
{
    BTP_USRID_NONE = 0,
    BTP_USRID_IGNORE,
    BTP_USRID_SERVER,
    BTP_USRID_CLIENT,
    BTP_USRID_EXTSRV,
    BTP_USRID_EXTCLT,
} BTP_USRID_ENUM;


#endif // BTP_DEFINE_H
