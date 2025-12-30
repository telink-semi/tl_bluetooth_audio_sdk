/********************************************************************************************************
 * @file    tlkusb_uacDesc.h
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
#ifndef TLKUSB_AUDIO_DESC_H
#define TLKUSB_AUDIO_DESC_H

#if (TLK_USB_UAC_ENABLE)


#define TLKUSB_AUD_STRING_PRODUCT             L"Telink B91 BTBLE Audio"

#define TLKUSB_AUD_STRING_SERIAL0             L"TLSR-BTBLE-MIC"
#define TLKUSB_AUD_STRING_SERIAL1             L"TLSR-BTBLE-SPK"
#define TLKUSB_AUD_STRING_SERIAL2             L"TLSR-BTBLE-MIC-SPK"

#define TLKUSB_HID_EDP_KEYBOARD_IN            USB_EDP_KEYBOARD_IN

#define TLKUSB_HID_UAC_KEYBOARD_POLL_INTERVAL 4 // in ms

#define TLKUSB_HID_RPT_TYPE_MAIN              (0x00)
#define TLKUSB_HID_RPT_TYPE_GLOBAL            (0x04)
#define TLKUSB_HID_RPT_TYPE_LOCAL             (0x08)

#define TLKUSB_HID_RPT_DATA_BITS_0            (0x00)
#define TLKUSB_HID_RPT_DATA_BITS_8            (0x01)
#define TLKUSB_HID_RPT_DATA_BITS_16           (0x02)
#define TLKUSB_HID_RPT_DATA_BITS_32           (0x03)
#define TLKUSB_HID_RPT_DATA_BITS(Data_bits)   TLKUSB_HID_RPT_DATA_BITS_##Data_bits


#define TLKUSB_HID_RPT_DATA_ENCODE_0(Data)
#define TLKUSB_HID_RPT_DATA_ENCODE_8(Data)  , (Data & 0xFF)
#define TLKUSB_HID_RPT_DATA_ENCODE_16(Data) TLKUSB_HID_RPT_DATA_ENCODE_8(Data) TLKUSB_HID_RPT_DATA_ENCODE_8(Data >> 8)
#define TLKUSB_HID_RPT_DATA_ENCODE_32(Data) \
    TLKUSB_HID_RPT_DATA_ENCODE_16(Data)     \
    TLKUSB_HID_RPT_DATA_ENCODE_16(Data >> 16)


#define TLKUSB_HID_RPT_DATA_ENCODE(Data_bits, ...)           TLKUSB_HID_RPT_DATA_ENCODE_##Data_bits(__VA_ARGS__)


#define TLKUSB_HID_RPT_DATA_ENTRY(Type, Tag, Data_bits, ...) (Type | Tag | TLKUSB_HID_RPT_DATA_BITS(Data_bits)) TLKUSB_HID_RPT_DATA_ENCODE(Data_bits, (__VA_ARGS__))


#define TLKUSB_HID_RPT_END_COLLECTION(Data_bits, ...)        TLKUSB_HID_RPT_DATA_ENTRY(TLKUSB_HID_RPT_TYPE_MAIN, 0xC0, Data_bits, __VA_ARGS__)
#define TLKUSB_HID_RPT_USAGE_PAGE(Data_bits, ...)            TLKUSB_HID_RPT_DATA_ENTRY(TLKUSB_HID_RPT_TYPE_GLOBAL, 0x00, Data_bits, __VA_ARGS__)

typedef enum
{
    TLKUSB_HID_CSCP_NonBootSubclass      = 0x00,
    TLKUSB_HID_CSCP_NonBootProtocol      = 0x00,
    TLKUSB_HID_CSCP_BootSubclass         = 0x01,
    TLKUSB_HID_CSCP_KeyboardBootProtocol = 0x01,
    TLKUSB_HID_CSCP_MouseBootProtocol,
    TLKUSB_HID_CSCP_HIDClass,
} TLKUSB_HID_CSCP_ENUM; // CSCP = ClassSubClassProtocol

typedef enum
{
    TLKUSB_HID_DESC_HID      = 0x21,
    TLKUSB_HID_DESC_REPORT   = 0x22,
    TLKUSB_HID_DESC_PHYSICAL = 0x23,
} TLKUSB_HID_DESC_ENUM;

typedef struct
{
    tlkusb_stdConfigureDesc_t config;
    tlkusb_stdInterfaceDesc_t audCtrInf;
// control interface
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
    tlkusb_uacInterfaceAcTLDesc_t audCtrAcInf;
#else
    tlkusb_uacInterfaceAcDesc_t audCtrAcInf;
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    // Input Terminal
    tlkusb_uacInputDesc_t spkInputDesc;
// Feature Unit
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
    tlkusb_uacDoubleFeatureDesc_t spkFeatureDesc;
#else
    tlkusb_uacSingleFeatureDesc_t spkFeatureDesc;
#endif
    // Output Terminal
    tlkusb_uacOutputDesc_t spkOutputDesc;
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    // Input Terminal
    tlkusb_uacInputDesc_t micInputDesc;
// Feature Unit
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
    tlkusb_uacDoubleFeatureDesc_t micFeatureDesc;
#else
    tlkusb_uacSingleFeatureDesc_t micFeatureDesc;
#endif
    // Output Terminal
    tlkusb_uacOutputDesc_t micOutputDesc;
#endif
// audio streaming interface
#if (TLKUSB_UAC_SPK_ENABLE)
    tlkusb_stdInterfaceDesc_t   spkSetting0Inf;
    tlkusb_stdInterfaceDesc_t   spkSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t spkDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spkFormatDesc;
    tlkusb_uacStdEndpointDesc_t spkStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spkSpcEdpDesc;
#if (0)
    tlkusb_stdInterfaceDesc_t   spk1Setting1Inf;
    tlkusb_uacInterfaceAsDesc_t spk1DatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spk1FormatDesc;
    tlkusb_uacStdEndpointDesc_t spk1StdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spk1SpcEdpDesc;
#endif
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    tlkusb_stdInterfaceDesc_t   micSetting0Inf;
    tlkusb_stdInterfaceDesc_t   micSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t micDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      micFormatDesc;
    tlkusb_uacStdEndpointDesc_t micStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t micSpcEdpDesc;

    tlkusb_stdInterfaceDesc_t   micSetting2Inf;
    tlkusb_uacInterfaceAsDesc_t mic2DatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      mic2FormatDesc;
    tlkusb_uacStdEndpointDesc_t mic2StdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t mic2SpcEdpDesc;
#endif
#if (TLKUSB_UAC_HID_ENABLE)
    tlkusb_stdInterfaceDesc_t keyboardInf;
    tlkusb_HidEndpointDesc_t  keyboardHid;
    tlkusb_stdEndpointDesc_t  keyboardEdp;
#endif
} __attribute__((packed)) tlkusb_uacAudConfigDesc_t;


#endif // #if (TLK_USB_UAC_ENABLE)

#endif // TLKUSB_AUDIO_DESC_H
