/********************************************************************************************************
 * @file    tlkusb_uacDesc.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_USB_UAC_ENABLE)
#include "tlklib/usb/tlkusb_desc.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"

#define TLKUSB_HID_INF_KEYBOARD 3

const uint8_t cTlkUsbHidKeyboardDesc01[] = {
    0x05,
    0x0c, // USAGE_PAGE (Consumer Devices)
    0x09,
    0x01, // USAGE (Consumer Control)
    0xa1,
    0x01, // COLLECTION (Application)
    0x85,
    USB_HID_KB_MEDIA, //global, report ID 0x03
    0x15,
    0x00, //Logical Minimum (0)
    0x25,
    0x01, //Logical Maximum (1)
    0x09,
    0xE9, //Usage (Volume Increment)
    0x09,
    0xEA, //Usage (Volume Decrement)
    0x09,
    0xE2, //Usage (Mute)
    0x09,
    0xCD, //Usage (Play/Pause)
    0x09,
    0xB5, //Usage (Scan Next Track)
    0x09,
    0xB6, //Usage (Scan Previous Track)
    0x09,
    0xB3, //Usage (Fast Forward)
    0x09,
    0xB7, //Usage (Stop)
    0x09,
    0xCA, //Usage (Tracking Increment)
    0x09,
    0xCB, //Usage (Tracking Decrement)
    0x09,
    0xCC, //Usage (Stop/Eject)
    0x09,
    0xE0, //Usage (Volume)
    0x0A,
    0x50,
    0x01, //Usage (Balance Right)
    0x0A,
    0x51,
    0x01, //Usage (Balance Left)
    0x09,
    0xB0, //Usage (Play)
    0x09,
    0xB1, //Usage (Pause)
    0x75,
    0x01, //Report Size (1)
    0x95,
    0x10, //Report Count (16)
    0x81,
    0x42, //Input (Data,Var,Abs,NWrp,Lin,Pref,Null,Bit)
    0xc0, // END_COLLECTION

#if 1                                   //report ID 5 for PC
    TLKUSB_HID_RPT_USAGE_PAGE(8, 0x01), //global,	USAGE_PAGE 1 (Generic Desktop)
    0x09,
    0x00, //usage undefined
    0xa1,
    0x01, //main collection
    0x85,
    0x05, //global report ID 0x5
    0x06,
    0x00,
    0xff, //global usage page
    0x09,
    0x01, //local,  usage ID 01  Consumer Control
    0x15,
    0x81, //global min 81
    0x25,
    0x7f, //global, max 7f
    0x75,
    0x08, //global, report size 8
    0x95,
    0x1e, //report count  7
    0xb1,
    0x02,                             //feature (data, var, abs)
    TLKUSB_HID_RPT_END_COLLECTION(0), //main, end collection
#endif

};

static uint16_t tlkusb_uacdesc_getConfigLens(void);
static uint16_t tlkusb_uacdesc_getStringLens(uint8_t index);
static uint8_t *tlkusb_uacdesc_getConfigDesc(void);
static uint8_t *tlkusb_uacdesc_getStringDesc(uint8_t index);
static uint16_t tlkusb_uacdesc_getReportLens(tlkusb_setup_req_t *pSetup);
static uint8_t *tlkusb_uacdesc_getReportDesc(tlkusb_setup_req_t *pSetup);


tlkusb_modDesc_t sTlkUsbUacModDesc = {
    NULL,                         // GetDeviceLens
    tlkusb_uacdesc_getConfigLens, // GetConfigLens
    tlkusb_uacdesc_getStringLens, // GetStringLens
    tlkusb_uacdesc_getReportLens, // GetReportLens
    NULL,                         // GetDeviceDesc
    tlkusb_uacdesc_getConfigDesc, // GetConfigDesc
    tlkusb_uacdesc_getStringDesc, // GetStringDesc
    tlkusb_uacdesc_getReportDesc, // GetReportDesc
};

static const tlkusb_stdStringDesc_t sMmiUsbAudProductDesc = {2 + sizeof(TLKUSB_AUD_STRING_PRODUCT) - 2, //-2 is the end of the string
                                                             TLKUSB_TYPE_STRING,                        // Header
                                                             TLKUSB_AUD_STRING_PRODUCT};

#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
static const tlkusb_stdStringDesc_t sMmiUsbAudMicSpkSerialDesc = {2 + sizeof(TLKUSB_AUD_STRING_SERIAL2) - 2, TLKUSB_TYPE_STRING, TLKUSB_AUD_STRING_SERIAL2};
#elif (TLKUSB_UAC_MIC_ENABLE)
static const tlkusb_stdStringDesc_t sMmiUsbAudMicSerialDesc = {2 + sizeof(TLKUSB_AUD_STRING_SERIAL0) - 2, TLKUSB_TYPE_STRING, TLKUSB_AUD_STRING_SERIAL0};
#endif
//#elif (TLKUSB_UAC_SPK_ENABLE)
static const tlkusb_stdStringDesc_t sMmiUsbAudSpk1SerialDesc = {2 + sizeof(TLKUSB_AUD_STRING_SERIAL1) - 2, TLKUSB_TYPE_STRING, TLKUSB_AUD_STRING_SERIAL1};
//    #endif


static const tlkusb_uacAudConfigDesc_t sMmiUsbAudConfigDesc = {
    {
        sizeof(tlkusb_stdConfigureDesc_t), // Length
        TLKUSB_TYPE_CONFIGURE,             // Type
        sizeof(tlkusb_uacAudConfigDesc_t), // TotalLength: variable
        TLKUSB_AUD_INF_MAX,                // NumInterfaces
        1,                                 // Configuration index
        TLKUSB_NO_DESCRIPTOR,              // Configuration String
        TLKUSB_CFG_ATTR_RESERVED,          // Attributes
        TLKUSB_CONFIG_POWER(100)           // MaxPower = 100mA
    },
    // audio_control_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // Length
        TLKUSB_TYPE_INTERFACE,             // Type
        TLKUSB_AUD_INF_CTR,                // InterfaceNumber
        0,                                 // AlternateSetting
        0,                                 // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,        // bInterfaceclass ->Printer
        TLKUSB_AUD_CSCP_ControlSubclass,   // bInterfaceSubClass -> Control
        TLKUSB_AUD_CSCP_ControlProtocol,   // bInterfaceProtocol
        3,                                 //TLKUSB_NO_DESCRIPTOR // iInterface
    },
// audio_control_interface_ac;
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
    {
        sizeof(tlkusb_uacInterfaceAcTLDesc_t),    // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_Header,        // Subtype
        {0x00, 0x01},                             // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcTLDesc_t) + /*9*/
          sizeof(tlkusb_uacInputDesc_t) +         /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +        /*9*/
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
          sizeof(tlkusb_uacDoubleFeatureDesc_t) + /*10*/
#else
          sizeof(tlkusb_uacSingleFeatureDesc_t) + /*9*/
#endif
          sizeof(tlkusb_uacInputDesc_t) +  /*12*/
          sizeof(tlkusb_uacOutputDesc_t) + /*9*/
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
          sizeof(tlkusb_uacDoubleFeatureDesc_t) /*10*/
#else
          sizeof(tlkusb_uacSingleFeatureDesc_t) /*9*/
#endif
          ),
         0},
        2,                  // InCollection
        TLKUSB_AUD_INF_SPK, // InterfaceNumber_spk
        TLKUSB_AUD_INF_MIC  // InterfaceNumber_mic
    },
#elif (TLKUSB_UAC_MIC_ENABLE)
    {
        sizeof(tlkusb_uacInterfaceAcDesc_t),    // Length
        TLKUSB_TYPE_CS_INTERFACE,               // Type
        AUDIO_DSUBTYPE_CSInterface_Header,      // Subtype
        {0x00, 0x01},                           // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcDesc_t) + /*9*/
          sizeof(tlkusb_uacInputDesc_t) +       /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +      /*9*/
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
          sizeof(tlkusb_uacDoubleFeatureDesc_t) /*10*/
#else
          sizeof(tlkusb_uacSingleFeatureDesc_t) /*9*/
#endif
          ),
         0},
        1,                 // InCollection
        TLKUSB_AUD_INF_MIC // InterfaceNumber_mic
    },
#elif (TLKUSB_UAC_SPK_ENABLE)
    {
        sizeof(tlkusb_uacInterfaceAcDesc_t),
        TLKUSB_TYPE_CS_INTERFACE,
        AUDIO_DSUBTYPE_CSInterface_Header,      // Subtype
        {0x00, 0x01},                           // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcDesc_t) + /*9*/
          sizeof(tlkusb_uacInputDesc_t) +       /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +      /*9*/
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
          sizeof(tlkusb_uacDoubleFeatureDesc_t) /*10*/
#else
          sizeof(tlkusb_uacSingleFeatureDesc_t) /*9*/
#endif
          ),
         0},
        1,                 // InCollection
        TLKUSB_AUD_INF_SPK // InterfaceNumber_spk
    },
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_input_terminal
    {
        sizeof(tlkusb_uacInputDesc_t),            // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,       // TerminalID
        AUDIO_TERMINAL_STREAMING,                 // TerminalType
        0,                                        // AssociatedOutputTerminal
        TLKUSB_AUD_SPK_CHANNEL_COUNT,             // TotalChannels
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
        0x03, // Channel Config bitmap(left and right)
#else
        0x0000, // 0x0001, // ChannelConfig - mono
#endif
        0, // ChannelStrIndex
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_feature_unit
    {
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
        sizeof(tlkusb_uacDoubleFeatureDesc_t),
#else
        sizeof(tlkusb_uacSingleFeatureDesc_t), // Length
#endif
        TLKUSB_TYPE_CS_INTERFACE,           // Type
        AUDIO_DSUBTYPE_CSInterface_Feature, // Subtype
        TLKUSB_AUDID_SPK_FEATURE_UNIT_ID,   // UnitID
        TLKUSB_AUD_SRCID_SPK_FEATURE_UNIT,  // SourceID
        1,                                  // bControlSize
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
        {0x03, 0x00, 0x00}, // bmaControls
#else
        {0x03, 0x00}, // bmaControls
#endif
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_SPK_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_OUT_SPEAKER,                // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUD_SRCID_SPK_OUTPUT_TERMINAL,      // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif //  #if (TLKUSB_UAC_SPK_ENABLE)
#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_input_terminal
    {sizeof(tlkusb_uacInputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                 // Type
     AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_IN_MIC,                    // TerminalType
     0,                                        // AssociatedOutputTerminal
     TLKUSB_AUD_MIC_CHANNEL_COUNT,             // TotalChannels
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
     0x0000, // 0x0003, // ChannelConfig - stero
#else
     0x01, // ChannelConfig - mono
#endif
     0, // ChannelStrIndex
     TLKUSB_NO_DESCRIPTOR},
    // mic_feature_unit mono
    {
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
        sizeof(tlkusb_uacDoubleFeatureDesc_t),
#else
        sizeof(tlkusb_uacSingleFeatureDesc_t), // Length
#endif
        TLKUSB_TYPE_CS_INTERFACE,           // Type
        AUDIO_DSUBTYPE_CSInterface_Feature, // Subtype
        TLKUSB_AUDID_MIC_FEATURE_UNIT_ID,   // UnitID
        TLKUSB_AUD_SRCID_MIC_FEATURE_UNIT,  // SourceID
        1,                                  // bControlSize
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
        {0x03, 0x00, 0x00}, // bmaControls
#else
        {0x03, 0x00}, // bmaControls
#endif
        TLKUSB_NO_DESCRIPTOR},
    // mic_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_MIC_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_STREAMING,                  // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUD_SRCID_MIC_OUTPUT_TERMINAL,      // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif //  #if (TLKUSB_UAC_MIC_ENABLE)
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_SPK,                     // InterfaceNumber  --2
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_SPK,                     // InterfaceNumber
     1,                                      // AlternateSetting
     1,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_audio_stream
    {
        sizeof(tlkusb_uacInterfaceAsDesc_t),                              // Length
        TLKUSB_TYPE_CS_INTERFACE,                                         // Type
        AUDIO_DSUBTYPE_CSInterface_General,                               // Subtype
        1,                                                                // bTerminalLink: #1 USB Streaming IN
        0,                                                                // bDelay
        {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    },
    // spk_audio_format
    {sizeof(tlkusb_uacFormatDesc_t),        // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
     USB_AUDIO_FORMAT_PCM,                  // FormatType
     TLKUSB_AUD_SPK_CHANNEL_COUNT,          // Channels
     TLKUSB_AUD_SPK_RESOLUTION_BIT / 8,     // SubFrameSize
     TLKUSB_AUD_SPK_RESOLUTION_BIT,         // BitsResolution
     1,                                     // TotalDiscreteSampleRates
     {                                      // spk_sample_rate
      (TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xff), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF00) >> 8), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF0000) >> 16)}},
    // spk_stream_endpoint
    {
        sizeof(tlkusb_uacStdEndpointDesc_t),                                                                // Length
        TLKUSB_TYPE_ENDPOINT,                                                                               // Type
        TLKUSB_EDP_DIR_OUT | TLKUSB_UAC_EDP_SPK,                                                            // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_SPK_CHANNEL_LENGTH,                                                                      // MaxPacketSize
        1,                                                                                                  // PollingIntervalMS
        0,                                                                                                  // Refresh
        0                                                                                                   // SyncEndpointNumber
    },
    // spk_stream_endpoint_spc
    {
        sizeof(tlkusb_uacSpcEndpointDesc_t), // Length
        TLKUSB_TYPE_CS_ENDPOINT,             // Type
        AUDIO_DSUBTYPE_CSInterface_General,  // Subtype
        AUDIO_EP_SAMPLE_FREQ_CONTROL,        //AUDIO_EP_FULL_PACKETS_ONLY | AUDIO_EP_SAMPLE_FREQ_CONTROL,//Attributes
        0,                                   // LockDelayUnits
        {0, 0}                               // LockDelay
    },
#if (0)
    //spk setting2
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_SPK,                     // InterfaceNumber
     2,                                      // AlternateSetting
     1,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},

    // spk_audio_stream
    {
        sizeof(tlkusb_uacInterfaceAsDesc_t),                              // Length
        TLKUSB_TYPE_CS_INTERFACE,                                         // Type
        AUDIO_DSUBTYPE_CSInterface_General,                               // Subtype
        1,                                                                // bTerminalLink: #1 USB Streaming IN
        0,                                                                // bDelay
        {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    },

    // spk_audio_format
    {sizeof(tlkusb_uacFormatDesc_t),        // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
     USB_AUDIO_FORMAT_PCM,                  // FormatType
     TLKUSB_AUD_SPK_CHANNEL_COUNT,          // Channels
     TLKUSB_AUD_SPK_RESOLUTION_BIT1 / 8,    // SubFrameSize
     TLKUSB_AUD_SPK_RESOLUTION_BIT1,        // BitsResolution
     1,                                     // TotalDiscreteSampleRates
     {                                      // spk_sample_rate
      (TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xff), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF00) >> 8), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF0000) >> 16)}},
    // spk_stream_endpoint
    {
        sizeof(tlkusb_uacStdEndpointDesc_t),                                                                // Length
        TLKUSB_TYPE_ENDPOINT,                                                                               // Type
        TLKUSB_EDP_DIR_OUT | TLKUSB_UAC_EDP_SPK,                                                            // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_SPK_CHANNEL_LENGTH1,                                                                     // MaxPacketSize
        1,                                                                                                  // PollingIntervalMS
        0,                                                                                                  // Refresh
        0                                                                                                   // SyncEndpointNumber
    },
    // spk_stream_endpoint_spc
    {
        sizeof(tlkusb_uacSpcEndpointDesc_t), // Length
        TLKUSB_TYPE_CS_ENDPOINT,             // Type
        AUDIO_DSUBTYPE_CSInterface_General,  // Subtype
        AUDIO_EP_SAMPLE_FREQ_CONTROL,        //AUDIO_EP_FULL_PACKETS_ONLY | AUDIO_EP_SAMPLE_FREQ_CONTROL,//Attributes
        0,                                   // LockDelayUnits
        {0, 0}                               // LockDelay
    },

#endif
#endif //  #if (TLKUSB_UAC_SPK_ENABLE)
#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_MIC,                     // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // mic_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_MIC,                     // InterfaceNumber
     1,                                      // AlternateSetting
     1,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // mic_audio_stream
    {
        sizeof(tlkusb_uacInterfaceAsDesc_t),                              // Length
        TLKUSB_TYPE_CS_INTERFACE,                                         // Type
        AUDIO_DSUBTYPE_CSInterface_General,                               // Subtype
        6,                                                                // bTerminalLink: #6 USB Streaming OUT
        0,                                                                // FrameDelay
        {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    },
    // mic_audio_format
    {sizeof(tlkusb_uacFormatDesc_t),        // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
     USB_AUDIO_FORMAT_PCM,                  // FormatType
     TLKUSB_AUD_MIC_CHANNEL_COUNT,          // Channels
     TLKUSB_AUD_MIC_RESOLUTION_BIT / 8,     // SubFrameSize
     TLKUSB_AUD_MIC_RESOLUTION_BIT,         // BitsResolution
     1,                                     // TotalDiscreteSampleRates
     {                                      // mic_sample_rate
      (TLKUSB_AUD_MIC_SAMPLE_RATE & 0xff), ((TLKUSB_AUD_MIC_SAMPLE_RATE >> 8) & 0xFF), ((TLKUSB_AUD_MIC_SAMPLE_RATE >> 16) & 0xFF)}},

    // mic_stream_endpoint
    {
        sizeof(tlkusb_uacStdEndpointDesc_t),                                                                // Length
        TLKUSB_TYPE_ENDPOINT,                                                                               // Type
        TLKUSB_EDP_DIR_IN | TLKUSB_UAC_EDP_MIC,                                                             // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_MIC_CHANNEL_LENGTH,                                                                      // MaxPacketSize
        1,                                                                                                  // PollingIntervalMS
        0,                                                                                                  // Refresh
        0                                                                                                   // SyncEndpointNumber
    },
    // mic_stream_endpoint_spc
    {
        sizeof(tlkusb_uacSpcEndpointDesc_t), // Length
        TLKUSB_TYPE_CS_ENDPOINT,             // Type
        AUDIO_DSUBTYPE_CSInterface_General,  // Subtype
        AUDIO_EP_SAMPLE_FREQ_CONTROL,        // Attributes
        0,                                   // LockDelayUnits
        {0, 0}                               // LockDelay
    },

    //mic setting2
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_INF_MIC,                     // InterfaceNumber
     2,                                      // AlternateSetting
     1,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // mic_audio_stream
    {
        sizeof(tlkusb_uacInterfaceAsDesc_t),                              // Length
        TLKUSB_TYPE_CS_INTERFACE,                                         // Type
        AUDIO_DSUBTYPE_CSInterface_General,                               // Subtype
        6,                                                                // bTerminalLink: #6 USB Streaming OUT
        0,                                                                // FrameDelay
        {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    },
    // mic_audio_format
    {sizeof(tlkusb_uacFormatDesc_t),        // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
     USB_AUDIO_FORMAT_PCM,                  // FormatType
     TLKUSB_AUD_MIC_CHANNEL_COUNT,          // Channels
     TLKUSB_AUD_MIC_RESOLUTION_BIT / 8,     // SubFrameSize
     TLKUSB_AUD_MIC_RESOLUTION_BIT,         // BitsResolution
     1,                                     // TotalDiscreteSampleRates
     {                                      // mic_sample_rate
      (TLKUSB_AUD_MIC_SAMPLE_RATE1 & 0xff), ((TLKUSB_AUD_MIC_SAMPLE_RATE1 >> 8) & 0xFF), ((TLKUSB_AUD_MIC_SAMPLE_RATE1 >> 16) & 0xFF)}},
    // mic_stream_endpoint
    {
        sizeof(tlkusb_uacStdEndpointDesc_t),                                                                // Length
        TLKUSB_TYPE_ENDPOINT,                                                                               // Type
        TLKUSB_EDP_DIR_IN | TLKUSB_UAC_EDP_MIC,                                                             // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_MIC_CHANNEL_LENGTH1,                                                                     // MaxPacketSize
        1,                                                                                                  // PollingIntervalMS
        0,                                                                                                  // Refresh
        0                                                                                                   // SyncEndpointNumber
    },
    // mic_stream_endpoint_spc
    {
        sizeof(tlkusb_uacSpcEndpointDesc_t), // Length
        TLKUSB_TYPE_CS_ENDPOINT,             // Type
        AUDIO_DSUBTYPE_CSInterface_General,  // Subtype
        AUDIO_EP_SAMPLE_FREQ_CONTROL,        // Attributes
        0,                                   // LockDelayUnits
        {0, 0}                               // LockDelay
    },
#endif //  #if (TLKUSB_UAC_MIC_ENABLE)
#if (TLKUSB_UAC_HID_ENABLE)
    // keyboardInterface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // length
        TLKUSB_TYPE_INTERFACE,             // type
        TLKUSB_HID_INF_KEYBOARD,           // interface number
        0,                                 // AlternateSetting
        1,                                 // bNumEndpoints
        TLKUSB_HID_CSCP_HIDClass,          // class
        TLKUSB_HID_CSCP_NonBootSubclass,   //TLKUSB_HID_CSCP_BootSubclass,         // subclass
        TLKUSB_HID_CSCP_NonBootSubclass,   //TLKUSB_HID_CSCP_KeyboardBootProtocol, // protocol
        TLKUSB_NO_DESCRIPTOR               // string descriptor
    },
    // keyboard_hid
    {
        sizeof(tlkusb_HidEndpointDesc_t), // length
        TLKUSB_HID_DESC_HID,              // type:HID
        0x0111,                           // HIDSpec
        0,                                //TLKUSB_HID_COUNTRY_US,            // CountryCode //0
        1,                                // TotalReportDescriptors
        TLKUSB_HID_DESC_REPORT,           // HIDReportType
        sizeof(cTlkUsbHidKeyboardDesc01), // HIDReportLength
    },
    // keyboard_in_endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),               // length
        TLKUSB_TYPE_ENDPOINT,                           // type
        TLKUSB_EDP_DIR_IN | TLKUSB_HID_EDP_KEYBOARD_IN, // endpoint
        TLKUSB_EDP_TYPE_INTERRUPT,                      // bmAttributes
        0x0008,                                         // EndpointSize
        TLKUSB_HID_UAC_KEYBOARD_POLL_INTERVAL,          // PollingIntervalMS
    },
#endif
};

/**
 * @brief     Get the length of HID report descriptor based on setup request parameters
 * @param[in] pSetup Pointer to USB setup request structure containing request information
 * @return    Length of the HID descriptor
 */
static uint16_t tlkusb_uacdesc_getReportLens(tlkusb_setup_req_t *pSetup)
{
    uint16_t length  = 0;
    uint8_t  value_h = (pSetup->wValue >> 8) & 0xff;
    uint8_t  index_l = (pSetup->wIndex) & 0xff;
    (void)index_l;

    if (value_h == TLKUSB_HID_DESC_HID) {
    } else if (value_h == TLKUSB_HID_DESC_REPORT) {
#if (TLKUSB_UAC_HID_ENABLE)
        if (index_l == TLKUSB_HID_INF_KEYBOARD) {
            length = sizeof(cTlkUsbHidKeyboardDesc01);
        }
#endif
    } else if (value_h == TLKUSB_HID_DESC_PHYSICAL) {
    }
    return length;
}

/**
 * @brief     Get HID report descriptor based on setup request parameters
 * @param[in] pSetup Pointer to USB setup request structure containing request information
 * @return    Pointer to the HID report descriptor
 */
static uint8_t *tlkusb_uacdesc_getReportDesc(tlkusb_setup_req_t *pSetup)
{
    uint8_t *pData   = NULL;
    uint8_t  value_h = (pSetup->wValue >> 8) & 0xff;
    uint8_t  index_l = (pSetup->wIndex) & 0xff;
    (void)index_l;

    if (value_h == TLKUSB_HID_DESC_HID) {
    } else if (value_h == TLKUSB_HID_DESC_REPORT) {
#if (TLKUSB_UAC_HID_ENABLE)
        if (index_l == TLKUSB_HID_INF_KEYBOARD) {
            pData = (uint8_t *)cTlkUsbHidKeyboardDesc01;
        }
#endif
    } else if (value_h == TLKUSB_HID_DESC_PHYSICAL) {
    }
    return pData;
}

/**
 * @brief     Get the length of configuration descriptor
 * @return    Length of the configuration descriptor
 */
static uint16_t tlkusb_uacdesc_getConfigLens(void)
{
    return sizeof(tlkusb_uacAudConfigDesc_t);
}

/**
 * @brief     Get the length of string descriptor by index
 * @param[in] index String descriptor index
 * @return    Length of the string descriptor
 */
static uint16_t tlkusb_uacdesc_getStringLens(uint8_t index)
{
    if (index == TLKUSB_STRING_INDEX_PRODUCT) {
        return sizeof(TLKUSB_AUD_STRING_PRODUCT);
    } else if (index == TLKUSB_STRING_INDEX_SERIAL) {
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
        return sizeof(TLKUSB_AUD_STRING_SERIAL2);
#elif (TLKUSB_UAC_MIC_ENABLE)
        return sizeof(TLKUSB_AUD_STRING_SERIAL0);
#elif (TLKUSB_UAC_SPK_ENABLE)
        return sizeof(TLKUSB_AUD_STRING_SERIAL1);
#else
        return 0;
#endif
    } else if (index == TLKUSB_STRING_INDEX_SERIAL1) {
        return sizeof(TLKUSB_AUD_STRING_SERIAL1);
    } else {
        return 0;
    }
}

/**
 * @brief     Get the configuration descriptor
 * @return    Pointer to the configuration descriptor
 */
static uint8_t *tlkusb_uacdesc_getConfigDesc(void)
{
    return (uint8_t *)(&sMmiUsbAudConfigDesc);
}

/**
 * @brief     Get the string descriptor by index
 * @param[in] index String descriptor index
 * @return    Pointer to the string descriptor
 */
static uint8_t *tlkusb_uacdesc_getStringDesc(uint8_t index)
{
    if (index == TLKUSB_STRING_INDEX_PRODUCT) {
        return (uint8_t *)(&sMmiUsbAudProductDesc);
    } else if (index == TLKUSB_STRING_INDEX_SERIAL) {
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
        return (uint8_t *)(&sMmiUsbAudMicSpkSerialDesc);
#elif (TLKUSB_UAC_MIC_ENABLE)
        return (uint8_t *)(&sMmiUsbAudMicSerialDesc);
#elif (TLKUSB_UAC_SPK_ENABLE)
        return (uint8_t *)(&sMmiUsbAudSpkSerialDesc);
#else
        return NULL;
#endif
    } else if (index == TLKUSB_STRING_INDEX_SERIAL1) {
        return (uint8_t *)(&sMmiUsbAudSpk1SerialDesc);
    } else {
        return 0;
    }
}

void tlkusb_uacModeDesc_set_devDescFunc(void *desc)
{
    sTlkUsbUacModDesc.GetDeviceDesc = desc;
}

void tlkusb_uacModeDesc_set_configDescFunc(void *desc)
{
    sTlkUsbUacModDesc.GetConfigDesc = desc;
}

void tlkusb_uacModeDesc_set_configDescLenFunc(void *desc)
{
    sTlkUsbUacModDesc.GetConfigLens = desc;
}

void tlkusb_uacModeDesc_set_IntfDescFunc(void *desc)
{
    sTlkUsbUacModDesc.GetInfDesDesc = desc;
}

void tlkusb_uacModeDesc_set_IntfDescLenFunc(void *desc)
{
    sTlkUsbUacModDesc.GetInfDesLens = desc;
}
#endif // #if (TLK_USB_UAC_ENABLE)
