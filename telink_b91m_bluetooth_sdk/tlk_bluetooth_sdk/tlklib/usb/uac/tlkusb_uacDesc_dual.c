/********************************************************************************************************
 * @file    tlkusb_uacDesc_dual.c
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
#include "tlklib/usb/tlkusb_define.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlkusb_uacDesc_dual.h"

#if (TLK_USB_UAC_ENABLE)
#if (TLKUSB_UAC_HID_ENABLE)
unsigned char tlkusb_hid_report_intf = TLKUSB_AUD_INF_HID;
#endif
unsigned char tlkusb_uac_mode = TLKUSB_UAC_MODE_DUAL_SOUNDCARD;
#if (TLKUSB_UAC_MIC_ENABLE)
unsigned char tlkusb_uac_mic_inf = TLKUSB_AUD_INF_MIC;
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
unsigned char tlkusb_uac_spk_inf = TLKUSB_AUD_INF_SPK;
#endif
unsigned char tlkusb_uac_spk1_inf = TLKUSB_AUD_DUAL_INF_SPK1;

const uint8_t cTlkUsbHidKeyboardDesc[] = {
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

static const tlkusb_stdDeviceDesc_t sSingleUsbDeviceDesc = {sizeof(tlkusb_stdDeviceDesc_t), //
                                                            TLKUSB_TYPE_DEVICE,             // Header
                                                            0x0110,                         // USBSpecification, USB 1.1
                                                            USB_CSCP_NoDeviceClass,         // Class
                                                            USB_CSCP_NoDeviceSubclass,      // SubClass
                                                            USB_CSCP_NoDeviceProtocol,      // Protocol
                                                                                            //	USB_CSCP_IADDeviceClass, // Class
                                                                                            //	USB_CSCP_IADDeviceSubclass, // SubClass
                                                                                            //	USB_CSCP_IADDeviceProtocol, // Protocol
                                                            8,                              // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
                                                            TLKUSB_ID_VENDOR,               // VendorID
#if TLKUSB_AUD_SPK_RESOLUTION_BIT == 24
                                                            0x8423 + TLKUSB_UAC_SPK_ENABLE + TLKUSB_UAC_MIC_ENABLE + TLKUSB_UAC_HID_ENABLE, // ProductID
#else
                                                            0x8413 + TLKUSB_UAC_SPK_ENABLE + TLKUSB_UAC_MIC_ENABLE + TLKUSB_UAC_HID_ENABLE, // ProductID
#endif
                                                            TLKUSB_ID_VERSION /*0x0100*/, // .ReleaseNumber
                                                            TLKUSB_STRING_INDEX_VENDOR,   // .ManufacturerStrIndex
                                                            TLKUSB_STRING_INDEX_PRODUCT,  // .ProductStrIndex
                                                            0,                            // .SerialNumStrIndex, iSerialNumber
                                                            1};

static const tlkusb_stdDeviceDesc_t sDualUsbDeviceDesc = {sizeof(tlkusb_stdDeviceDesc_t), //
                                                          TLKUSB_TYPE_DEVICE,             // Header
                                                          0x0110,                         // USBSpecification, USB 1.1
                                                          USB_CSCP_NoDeviceClass,         // Class
                                                          USB_CSCP_NoDeviceSubclass,      // SubClass
                                                          USB_CSCP_NoDeviceProtocol,      // Protocol
                                                                                          //	USB_CSCP_IADDeviceClass, // Class
                                                                                          //	USB_CSCP_IADDeviceSubclass, // SubClass
                                                                                          //	USB_CSCP_IADDeviceProtocol, // Protocol
                                                          8,                              // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
                                                          TLKUSB_ID_VENDOR,               // VendorID
#if TLKUSB_AUD_SPK_RESOLUTION_BIT == 24
                                                          0x8523 + TLKUSB_UAC_SPK_ENABLE + TLKUSB_UAC_MIC_ENABLE + TLKUSB_UAC_HID_ENABLE, // ProductID
#else
                                                          0x8513 + TLKUSB_UAC_SPK_ENABLE + TLKUSB_UAC_MIC_ENABLE + TLKUSB_UAC_HID_ENABLE, // ProductID
#endif
                                                          TLKUSB_ID_VERSION /*0x0100*/, // .ReleaseNumber
                                                          TLKUSB_STRING_INDEX_VENDOR,   // .ManufacturerStrIndex
                                                          TLKUSB_STRING_INDEX_PRODUCT,  // .ProductStrIndex
                                                          0,                            // .SerialNumStrIndex, iSerialNumber
                                                          1};

static const tlkusb_uacAudConfigDesc_single_t sMmiUsbAudSingleConfigDesc = {
    {
        sizeof(tlkusb_stdConfigureDesc_t),        // Length
        TLKUSB_TYPE_CONFIGURE,                    // Type
        sizeof(tlkusb_uacAudConfigDesc_single_t), // TotalLength: variable
        TLKUSB_AUD_SINGLE_INF_MAX,                // NumInterfaces
        1,                                        // Configuration index
        TLKUSB_NO_DESCRIPTOR,                     // Configuration String
#if TLK_USB_REMOTEWAKEUP_EN
        TLKUSB_CFG_ATTR_RESERVED | TLKUSB_CFG_ATTR_REMOTEWAKEUP, // Attributes
#else
        TLKUSB_CFG_ATTR_RESERVED,
#endif
        TLKUSB_CONFIG_POWER(100) // MaxPower = 100mA
    },
#if (TLKUSB_UAC_MIC_ENABLE || TLKUSB_UAC_SPK_ENABLE)
    ///////////////////////AUDIO CONTROL DESCRIPT///////////////////////////////
    // audio_control_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // Length
        TLKUSB_TYPE_INTERFACE,             // Type
        TLKUSB_AUD_SINGLE_INF_CTR,         // InterfaceNumber
        0,                                 // AlternateSetting
        0,                                 // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,        // bInterfaceclass ->Printer
        TLKUSB_AUD_CSCP_ControlSubclass,   // bInterfaceSubClass -> Control
        TLKUSB_AUD_CSCP_ControlProtocol,   // bInterfaceProtocol
        3,                                 //TLKUSB_NO_DESCRIPTOR // iInterface
    },
    // audio_control_interface_ac;
    {
        sizeof(tlkusb_uacInterfaceAcTLDesc_t),    // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_Header,        // Subtype
        {0x00, 0x01},                             // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcTLDesc_t) + /*9*/
#if (TLKUSB_UAC_SPK_ENABLE)
          sizeof(tlkusb_uacInputDesc_t) +         /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +        /*9*/
          sizeof(tlkusb_uacDoubleFeatureDesc_t) + /*10*/
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
          sizeof(tlkusb_uacInputDesc_t) +       /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +      /*9*/
          sizeof(tlkusb_uacSingleFeatureDesc_t) /*9*/
#endif
          ),
         0},
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
        2, // InCollection
#else
        1,
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
        TLKUSB_AUD_SINGLE_INF_SPK, // InterfaceNumber_spk
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
        TLKUSB_AUD_SINGLE_INF_MIC // InterfaceNumber_mic
#endif
    },
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_input_terminal
    {
        sizeof(tlkusb_uacInputDesc_t),            // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,       // TerminalID
        AUDIO_TERMINAL_STREAMING,                 // TerminalType
        0,                                        // AssociatedOutputTerminal
        2,                                        // TotalChannels
        0x03,                                     // Channel Config bitmap(left and right)
        0,                                        // ChannelStrIndex
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_feature_unit
    {
        sizeof(tlkusb_uacDoubleFeatureDesc_t),
        TLKUSB_TYPE_CS_INTERFACE,           // Type
        AUDIO_DSUBTYPE_CSInterface_Feature, // Subtype
        TLKUSB_AUDID_SPK_FEATURE_UNIT_ID,   // UnitID
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID, // SourceID
        1,                                  // bControlSize
        {0x03, 0x00, 0x00},                 // bmaControls
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_SPK_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_OUT_SPEAKER,                // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUDID_SPK_FEATURE_UNIT_ID,          // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif

#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_input_terminal
    {sizeof(tlkusb_uacInputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                 // Type
     AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_IN_MIC,                    // TerminalType
     0,                                        // AssociatedOutputTerminal
     TLKUSB_AUD_MIC_CHANNEL_COUNT,             // TotalChannels
     0x01,                                     // ChannelConfig - mono
     0,                                        // ChannelStrIndex
     TLKUSB_NO_DESCRIPTOR},
    // mic_feature_unit mono
    {sizeof(tlkusb_uacSingleFeatureDesc_t), // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_Feature,    // Subtype
     TLKUSB_AUDID_MIC_FEATURE_UNIT_ID,      // UnitID
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,    // SourceID
     1,                                     // bControlSize
     {0x03, 0x00},                          // bmaControls
     TLKUSB_NO_DESCRIPTOR},
    // mic_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_MIC_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_STREAMING,                  // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUDID_MIC_FEATURE_UNIT_ID,          // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif
///////////////////SPK DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_SINGLE_INF_SPK,              // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_SINGLE_INF_SPK,              // InterfaceNumber
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
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,                               // bTerminalLink: #1 USB Streaming IN
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
#endif //(TLKUSB_UAC_SPK_ENABLE)
       ///////////////////MIC DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_SINGLE_INF_MIC,              // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // mic_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_SINGLE_INF_MIC,              // InterfaceNumber
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
        TLKUSB_AUDID_MIC_OUTPUT_TERMINAL_ID,                              // bTerminalLink: #6 USB Streaming OUT
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
      (TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE & 0xff), ((TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE >> 8) & 0xFF), ((TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE >> 16) & 0xFF)}},
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
#endif
#endif //#if (TLKUSB_UAC_MIC_ENABLE || TLKUSB_UAC_SPK_ENABLE)
       //////////////////HID DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_HID_ENABLE)
    // keyboardInterface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // length
        TLKUSB_TYPE_INTERFACE,             // type
        TLKUSB_AUD_SINGLE_INF_HID,         // interface number
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
        sizeof(cTlkUsbHidKeyboardDesc),   // HIDReportLength
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


static const tlkusb_uacAudConfigDesc_dual_t sMmiUsbAudDualConfigDesc = {
    {
        sizeof(tlkusb_stdConfigureDesc_t),      // Length
        TLKUSB_TYPE_CONFIGURE,                  // Type
        sizeof(tlkusb_uacAudConfigDesc_dual_t), // TotalLength: variable
        TLKUSB_AUD_DUAL_INF_MAX,                // NumInterfaces
        1,                                      // Configuration index
        TLKUSB_NO_DESCRIPTOR,                   // Configuration String
#if TLK_USB_REMOTEWAKEUP_EN
        TLKUSB_CFG_ATTR_RESERVED | TLKUSB_CFG_ATTR_REMOTEWAKEUP, // Attributes
#else
        TLKUSB_CFG_ATTR_RESERVED,
#endif
        TLKUSB_CONFIG_POWER(100) // MaxPower = 100mA
    },
#if (TLKUSB_UAC_MIC_ENABLE || TLKUSB_UAC_SPK_ENABLE)
    ///////////////////////AUDIO0 CONTROL DESCRIPT///////////////////////////////
    // audio_control_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // Length
        TLKUSB_TYPE_INTERFACE,             // Type
        TLKUSB_AUD_DUAL_INF_CTR0,          // InterfaceNumber
        0,                                 // AlternateSetting
        0,                                 // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,        // bInterfaceclass ->Printer
        TLKUSB_AUD_CSCP_ControlSubclass,   // bInterfaceSubClass -> Control
        TLKUSB_AUD_CSCP_ControlProtocol,   // bInterfaceProtocol
        TLKUSB_STRING_INDEX_SERIAL,        //TLKUSB_NO_DESCRIPTOR // iInterface
    },
    // audio_control_interface_ac;
    {
        sizeof(tlkusb_uacInterfaceAcTLDesc_t),    // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_Header,        // Subtype
        {0x00, 0x01},                             // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcTLDesc_t) + /*9*/
#if (TLKUSB_UAC_SPK_ENABLE)
          sizeof(tlkusb_uacInputDesc_t) +         /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +        /*9*/
          sizeof(tlkusb_uacDoubleFeatureDesc_t) + /*10*/
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
          sizeof(tlkusb_uacInputDesc_t) +       /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +      /*9*/
          sizeof(tlkusb_uacSingleFeatureDesc_t) /*9*/
#endif
          ),
         0},
#if (TLKUSB_UAC_MIC_ENABLE && TLKUSB_UAC_SPK_ENABLE)
        2, // InCollection
#else
        1,
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
        TLKUSB_AUD_DUAL_INF_SPK0, // InterfaceNumber_spk
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
        TLKUSB_AUD_DUAL_INF_MIC // InterfaceNumber_mic
#endif
    },
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_input_terminal
    {
        sizeof(tlkusb_uacInputDesc_t),            // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,       // TerminalID
        AUDIO_TERMINAL_STREAMING,                 // TerminalType
        0,                                        // AssociatedOutputTerminal
        2,                                        // TotalChannels
        0x03,                                     // Channel Config bitmap(left and right)
        0,                                        // ChannelStrIndex
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_feature_unit
    {
        sizeof(tlkusb_uacDoubleFeatureDesc_t),
        TLKUSB_TYPE_CS_INTERFACE,           // Type
        AUDIO_DSUBTYPE_CSInterface_Feature, // Subtype
        TLKUSB_AUDID_SPK_FEATURE_UNIT_ID,   // UnitID
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID, // SourceID
        1,                                  // bControlSize
        {0x03, 0x00, 0x00},                 // bmaControls
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_SPK_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_OUT_SPEAKER,                // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUDID_SPK_FEATURE_UNIT_ID,          // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif //(TLKUSB_UAC_SPK_ENABLE)

#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_input_terminal
    {sizeof(tlkusb_uacInputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                 // Type
     AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_IN_MIC,                    // TerminalType
     0,                                        // AssociatedOutputTerminal
     TLKUSB_AUD_MIC_CHANNEL_COUNT,             // TotalChannels
     0x01,                                     // ChannelConfig - mono
     0,                                        // ChannelStrIndex
     TLKUSB_NO_DESCRIPTOR},
    // mic_feature_unit mono
    {sizeof(tlkusb_uacSingleFeatureDesc_t), // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_Feature,    // Subtype
     TLKUSB_AUDID_MIC_FEATURE_UNIT_ID,      // UnitID
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,    // SourceID
     1,                                     // bControlSize
     {0x03, 0x00},                          // bmaControls
     TLKUSB_NO_DESCRIPTOR},
    // mic_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_MIC_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_STREAMING,                  // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUDID_MIC_FEATURE_UNIT_ID,          // SourceID
     TLKUSB_NO_DESCRIPTOR},
#endif // (TLKUSB_UAC_MIC_ENABLE)
       ///////////////////SPK0 DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_SPK_ENABLE)
    // spk_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_SPK0,               // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_SPK0,               // InterfaceNumber
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
        TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,                               // bTerminalLink: #1 USB Streaming IN
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
#endif
///////////////////MIC DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_MIC_ENABLE)
    // mic_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_MIC,                // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // mic_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_MIC,                // InterfaceNumber
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
        TLKUSB_AUDID_MIC_OUTPUT_TERMINAL_ID,                              // bTerminalLink: #6 USB Streaming OUT
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
      (TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE & 0xff), ((TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE >> 8) & 0xFF), ((TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE >> 16) & 0xFF)}},
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
#endif //#if (TLKUSB_UAC_MIC_ENABLE)
       ///////////////////////AUDIO1 CONTROL DESCRIPT///////////////////////////////
#if (TLKUSB_UAC_SPK_ENABLE)
    // audio_control_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // Length
        TLKUSB_TYPE_INTERFACE,             // Type
        TLKUSB_AUD_DUAL_INF_CTR1,          // InterfaceNumber
        0,                                 // AlternateSetting
        0,                                 // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,        // bInterfaceclass ->Printer
        TLKUSB_AUD_CSCP_ControlSubclass,   // bInterfaceSubClass -> Control
        TLKUSB_AUD_CSCP_ControlProtocol,   // bInterfaceProtocol
        TLKUSB_STRING_INDEX_SERIAL1,       //TLKUSB_NO_DESCRIPTOR // iInterface
    },
    // audio_control_interface_ac;
    {
        sizeof(tlkusb_uacInterfaceAcDesc_t),    // Length
        TLKUSB_TYPE_CS_INTERFACE,               // Type
        AUDIO_DSUBTYPE_CSInterface_Header,      // Subtype
        {0x00, 0x01},                           // ACSpecification, version == 1.0
        {(sizeof(tlkusb_uacInterfaceAcDesc_t) + /*9*/
          sizeof(tlkusb_uacInputDesc_t) +       /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +      /*9*/
          sizeof(tlkusb_uacDoubleFeatureDesc_t) /*10*/
          ),
         0},
        1,                        // InCollection
        TLKUSB_AUD_DUAL_INF_SPK1, // InterfaceNumber_spk
    },
    // spk_input_terminal
    {
        sizeof(tlkusb_uacInputDesc_t),            // Length
        TLKUSB_TYPE_CS_INTERFACE,                 // Type
        AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
        TLKUSB_AUDID_SPK1_INPUT_TERMINAL_ID,      // TerminalID
        AUDIO_TERMINAL_STREAMING,                 // TerminalType
        0,                                        // AssociatedOutputTerminal
        2,                                        // TotalChannels
        0x03,                                     // Channel Config bitmap(left and right)
        0,                                        // ChannelStrIndex
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_feature_unit
    {
        sizeof(tlkusb_uacDoubleFeatureDesc_t),
        TLKUSB_TYPE_CS_INTERFACE,            // Type
        AUDIO_DSUBTYPE_CSInterface_Feature,  // Subtype
        TLKUSB_AUDID_SPK1_FEATURE_UNIT_ID,   // UnitID
        TLKUSB_AUDID_SPK1_INPUT_TERMINAL_ID, // SourceID
        1,                                   // bControlSize
        {0x03, 0x00, 0x00},                  // bmaControls
        TLKUSB_NO_DESCRIPTOR,
    },
    // spk_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_SPK1_OUTPUT_TERMINAL_ID,      // TerminalID
     AUDIO_TERMINAL_OUT_SPEAKER,                // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUDID_SPK1_FEATURE_UNIT_ID,         // SourceID
     TLKUSB_NO_DESCRIPTOR},
    ///////////////////SPK1 DESCRIPT//////////////////////////////
    // spk_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_SPK1,               // InterfaceNumber
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_DUAL_INF_SPK1,               // InterfaceNumber
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
        TLKUSB_AUDID_SPK1_INPUT_TERMINAL_ID,                              // bTerminalLink: #1 USB Streaming IN
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
        TLKUSB_EDP_DIR_OUT | TLKUSB_UAC_EDP_SPK1,                                                           // Endpoint
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
#endif //(TLKUSB_UAC_SPK_ENABLE)
#endif //#if (TLKUSB_UAC_MIC_ENABLE || TLKUSB_UAC_SPK_ENABLE)
       //////////////////HID DESCRIPT//////////////////////////////
#if (TLKUSB_UAC_HID_ENABLE)
    // keyboardInterface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // length
        TLKUSB_TYPE_INTERFACE,             // type
        TLKUSB_AUD_DUAL_INF_HID,           // interface number
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
        sizeof(cTlkUsbHidKeyboardDesc),   // HIDReportLength
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
#endif //#if (TLKUSB_UAC_HID_ENABLE)
};


/**
 * @brief     Get the length of HID report descriptor based on setup request parameters
 * @param[in] pSetup Pointer to USB setup request structure containing request information
 * @return    Length of the HID descriptor
 */
static uint16_t tlkusb_uacdesc_getIntfReportLens(tlkusb_setup_req_t *pSetup)
{
    uint16_t length  = 0;
    uint8_t  value_h = (pSetup->wValue >> 8) & 0xff;
    uint8_t  index_l = (pSetup->wIndex) & 0xff;
    (void)index_l;

    if (value_h == TLKUSB_HID_DESC_HID) {
    } else if (value_h == TLKUSB_HID_DESC_REPORT) {
#if (TLKUSB_UAC_HID_ENABLE)
        if (tlkusb_get_uac_hid_inf(index_l)) {
            length = sizeof(cTlkUsbHidKeyboardDesc);
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
static uint8_t *tlkusb_uacdesc_getIntfReportDesc(tlkusb_setup_req_t *pSetup)
{
    uint8_t *pData   = NULL;
    uint8_t  value_h = (pSetup->wValue >> 8) & 0xff;
    uint8_t  index_l = (pSetup->wIndex) & 0xff;
    (void)index_l;

    if (value_h == TLKUSB_HID_DESC_HID) {
    } else if (value_h == TLKUSB_HID_DESC_REPORT) {
#if (TLKUSB_UAC_HID_ENABLE)
        if (tlkusb_get_uac_hid_inf(index_l)) {
            pData = (uint8_t *)cTlkUsbHidKeyboardDesc;
        }
#endif
    } else if (value_h == TLKUSB_HID_DESC_PHYSICAL) {
    }
    return pData;
}

static uint8_t *tlkusb_uacdesc_getSingleDeviceDesc(void)
{
    return (uint8_t *)(&sSingleUsbDeviceDesc);
}

static uint8_t *tlkusb_uacdesc_getSingleConfigDesc(void)
{
    return (uint8_t *)(&sMmiUsbAudSingleConfigDesc);
}

static uint16_t tlkusb_uacdesc_getSingleConfigLens(void)
{
    return sizeof(tlkusb_uacAudConfigDesc_single_t);
}

static uint8_t *tlkusb_uacdesc_getDualDeviceDesc(void)
{
    return (uint8_t *)(&sDualUsbDeviceDesc);
}

static uint8_t *tlkusb_uacdesc_getDualConfigDesc(void)
{
    return (uint8_t *)(&sMmiUsbAudDualConfigDesc);
}

static uint16_t tlkusb_uacdesc_getDualConfigLens(void)
{
    return sizeof(tlkusb_uacAudConfigDesc_dual_t);
}

void tlkusb_set_uac_mode(unsigned char mode)
{
    if (mode >= TLKUSB_UAC_MODE_MAX) {
        return;
    }

    tlkusb_uac_mode = mode;
    switch (mode) {
    case TLKUSB_UAC_MODE_SINGLE_SOUNDCARD:
        tlkusb_uacModeDesc_set_devDescFunc(tlkusb_uacdesc_getSingleDeviceDesc);
        tlkusb_uacModeDesc_set_configDescFunc(tlkusb_uacdesc_getSingleConfigDesc);
        tlkusb_uacModeDesc_set_configDescLenFunc(tlkusb_uacdesc_getSingleConfigLens);
        tlkusb_uacModeDesc_set_IntfDescFunc(tlkusb_uacdesc_getIntfReportDesc);
        tlkusb_uacModeDesc_set_IntfDescLenFunc(tlkusb_uacdesc_getIntfReportLens);
#if (TLKUSB_UAC_SPK_ENABLE)
        tlkusb_uac_spk_inf = TLKUSB_AUD_SINGLE_INF_SPK;
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
        tlkusb_uac_mic_inf = TLKUSB_AUD_SINGLE_INF_MIC;
#endif
#if (TLKUSB_UAC_HID_ENABLE)
        tlkusb_hid_report_intf = TLKUSB_AUD_SINGLE_INF_HID;
#endif
        break;

    case TLKUSB_UAC_MODE_DUAL_SOUNDCARD:
        tlkusb_uacModeDesc_set_devDescFunc(tlkusb_uacdesc_getDualDeviceDesc);
        tlkusb_uacModeDesc_set_configDescFunc(tlkusb_uacdesc_getDualConfigDesc);
        tlkusb_uacModeDesc_set_configDescLenFunc(tlkusb_uacdesc_getDualConfigLens);
        tlkusb_uacModeDesc_set_IntfDescFunc(tlkusb_uacdesc_getIntfReportDesc);
        tlkusb_uacModeDesc_set_IntfDescLenFunc(tlkusb_uacdesc_getIntfReportLens);
#if (TLKUSB_UAC_SPK_ENABLE)
        tlkusb_uac_spk_inf  = TLKUSB_AUD_DUAL_INF_SPK0;
        tlkusb_uac_spk1_inf = TLKUSB_AUD_DUAL_INF_SPK1;
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
        tlkusb_uac_mic_inf = TLKUSB_AUD_DUAL_INF_MIC;
#endif
#if (TLKUSB_UAC_HID_ENABLE)
        tlkusb_hid_report_intf = TLKUSB_AUD_DUAL_INF_HID;
#endif
        break;

    default:

        break;
    }
}

int tlkusb_get_usb_mode(unsigned char mode)
{
    return tlkusb_uac_mode == mode;
}

int tlkusb_get_uac_mic_inf(unsigned char inf)
{
#if (TLKUSB_UAC_MIC_ENABLE)
    return inf == tlkusb_uac_mic_inf;
#else
    return 0;
#endif
}

int tlkusb_get_uac_spk_inf(unsigned char inf)
{
#if (TLKUSB_UAC_SPK_ENABLE)
    return inf == tlkusb_uac_spk_inf;
#else
    return 0;
#endif
}

int tlkusb_get_uac_spk1_inf(unsigned char inf)
{
#if (TLKUSB_UAC_SPK_ENABLE)
    if (!tlkusb_get_usb_mode(TLKUSB_UAC_MODE_DUAL_SOUNDCARD)) {
        return 0;
    }

    return inf == tlkusb_uac_spk1_inf;
#else
    return 0;
#endif
}

int tlkusb_get_uac_hid_inf(unsigned char inf)
{
#if (TLKUSB_UAC_HID_ENABLE)
    return inf == tlkusb_hid_report_intf;
#else
    return 0;
#endif
}

#endif
