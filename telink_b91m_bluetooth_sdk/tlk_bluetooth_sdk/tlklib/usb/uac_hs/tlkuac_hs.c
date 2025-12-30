/********************************************************************************************************
 * @file    tlkuac_hs.c
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
#include "tlklib/usb/tlkusb_hal.h"
#include "tlksys/platform/api/tlkhal_usb.h"
#include "../tlkusb_define.h"
#include "../tlkusb_struct.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "drivers.h"

#if (TLK_USB_UAC_HS_ENABLE)
/*USB Descriptors, will remove later.*/
extern unsigned char audio_buffer[192];
extern unsigned char in_audio_buffer[32];

static signed short sin_16k[] __attribute__((aligned(4))) = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539,
};

enum
{
    TLKUSB_AUD_HS_INF_CTR,
    TLKUSB_AUD_HS_INF_SPK,
    TLKUSB_AUD_HS_INF_MIC,
    TLKUSB_AUD_HS_INF_MAX,
};

#define TLKUSB_HS_UAC_EDP_HID     USB0_EP1
#define TLKUSB_HS_UAC_EDP_MIC     USB0_EP7
#define TLKUSB_HS_UAC_EDP_SPK     USB0_EP6

#define TLKUSB_AUD_STRING_PRODUCT L"Telink TL322X USB Audio"

#define TLKUSB_AUD_STRING_SERIAL0 L"TLSR-BTBLE-MIC"
#define TLKUSB_AUD_STRING_SERIAL1 L"TLSR-BTBLE-SPK"
#define TLKUSB_AUD_STRING_SERIAL2 L"TLSR-BTBLE-MIC-SPK"

typedef struct
{
    tlkusb_stdConfigureDesc_t config;
    tlkusb_stdInterfaceDesc_t audCtrInf;
    // control interface
    tlkusb_uacInterfaceAcTLDesc_t audCtrAcInf;
    // tlkusb_uacInterfaceAcDesc_t audCtrAcInf;

    // spk Input Terminal
    tlkusb_uacInputDesc_t spkInputDesc;
    // spk Feature Unit
    tlkusb_uacDoubleFeatureDesc_t spkFeatureDesc;
    //spk Output Terminal
    tlkusb_uacOutputDesc_t spkOutputDesc;

    //mic Input Terminal
    tlkusb_uacInputDesc_t micInputDesc;
    //mic Feature Unit
    tlkusb_uacSingleFeatureDesc_t micFeatureDesc;
    //mic Output Terminal
    tlkusb_uacOutputDesc_t micOutputDesc;

    // audio streaming interface
    tlkusb_stdInterfaceDesc_t   spkSetting0Inf;
    tlkusb_stdInterfaceDesc_t   spkSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t spkDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spkFormatDesc;
    tlkusb_uacStdEndpointDesc_t spkStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spkSpcEdpDesc;

    // audio streaming interface
    tlkusb_stdInterfaceDesc_t   micSetting0Inf;
    tlkusb_stdInterfaceDesc_t   micSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t micDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      micFormatDesc;
    tlkusb_uacStdEndpointDesc_t micStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t micSpcEdpDesc;

    // tlkusb_stdInterfaceDesc_t   micSetting2Inf;
    // tlkusb_uacInterfaceAsDesc_t mic2DatAsInf;
    // // audio format
    // tlkusb_uacFormatDesc_t      mic2FormatDesc;
    // tlkusb_uacStdEndpointDesc_t mic2StdEdpDesc;
    // tlkusb_uacSpcEndpointDesc_t mic2SpcEdpDesc;
} __attribute__((packed)) tlkusb_uacHsAudConfigDesc_t;

static const tlkusb_stdDeviceDesc_t sMmiUsbUacHsDeviceDesc = {sizeof(tlkusb_stdDeviceDesc_t), //
                                                              TLKUSB_TYPE_DEVICE,             // Header
                                                              0x0200,                         // USBSpecification, USB 2.0
                                                              USB_CSCP_NoDeviceClass,         // Class
                                                              USB_CSCP_NoDeviceSubclass,      // SubClass
                                                              USB_CSCP_NoDeviceProtocol,      // Protocol
                                                                                              //	USB_CSCP_IADDeviceClass, // Class
                                                                                              //	USB_CSCP_IADDeviceSubclass, // SubClass
                                                                                              //	USB_CSCP_IADDeviceProtocol, // Protocol
                                                              0x40,                           // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
                                                              TLKUSB_ID_VENDOR,               // VendorID
                                                              TLKUSB_ID_PRODUCT1,             // ProductID
                                                              TLKUSB_ID_VERSION1 /*0x0100*/,  // .ReleaseNumber
                                                              TLKUSB_STRING_INDEX_VENDOR,     // .ManufacturerStrIndex
                                                              TLKUSB_STRING_INDEX_PRODUCT,    // .ProductStrIndex
                                                              0x03,                           // .SerialNumStrIndex, iSerialNumber
                                                              0x01};

static const tlkusb_uacHsAudConfigDesc_t sMmiUsbUacHsConfigDesc = {
    /*Configuration Descriptor*/
    {
        sizeof(tlkusb_stdConfigureDesc_t), TLKUSB_TYPE_CONFIGURE, sizeof(tlkusb_uacHsAudConfigDesc_t), TLKUSB_AUD_HS_INF_MAX, 0x01,
        TLKUSB_NO_DESCRIPTOR,     // Configuration String
        TLKUSB_CFG_ATTR_RESERVED, // Attributes
        TLKUSB_CONFIG_POWER(50)   // MaxPower = 50mA
    },
    {
        sizeof(tlkusb_stdInterfaceDesc_t), // Length
        TLKUSB_TYPE_INTERFACE,             // Type
        TLKUSB_AUD_HS_INF_CTR, 0x00, 0x00,
        TLKUSB_AUD_CSCP_AudioClass,      // bInterfaceclass ->Printer
        TLKUSB_AUD_CSCP_ControlSubclass, // bInterfaceSubClass -> Control
        TLKUSB_AUD_CSCP_ControlProtocol, // bInterfaceProtocol
        0x03,                            // iInterface
    },
    {
        sizeof(tlkusb_uacInterfaceAcTLDesc_t),
        TLKUSB_TYPE_CS_INTERFACE,
        AUDIO_DSUBTYPE_CSInterface_Header,
        {0x00, 0x01},
        {(sizeof(tlkusb_uacInterfaceAcTLDesc_t) + /*9*/
          sizeof(tlkusb_uacInputDesc_t) +         /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +        /*9*/
          sizeof(tlkusb_uacDoubleFeatureDesc_t) + /*10*/
          sizeof(tlkusb_uacInputDesc_t) +         /*12*/
          sizeof(tlkusb_uacOutputDesc_t) +        /*9*/
          sizeof(tlkusb_uacSingleFeatureDesc_t)   /*9*/
          ),
         0},
        0x02,
        TLKUSB_AUD_HS_INF_SPK,
        TLKUSB_AUD_HS_INF_MIC,
    },
    // spk_input_terminal
    {sizeof(tlkusb_uacInputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                 // Type
     AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
     TLKUSB_AUDID_SPK_INPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_STREAMING, 0x00,
     TLKUSB_AUD_SPK_CHANNEL_COUNT, //channel
#if (TLKUSB_AUD_SPK_CHANNEL_COUNT == 2)
     0x03, // Channel Config bitmap(left and right)
#else
     0x0000, // 0x0001, // ChannelConfig - mono
#endif
     0x00, TLKUSB_NO_DESCRIPTOR},
    // spk_feature_unit
    {sizeof(tlkusb_uacDoubleFeatureDesc_t),
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
     TLKUSB_NO_DESCRIPTOR},
    // spk_output_terminal
    {sizeof(tlkusb_uacOutputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                  // Type
     AUDIO_DSUBTYPE_CSInterface_OutputTerminal, // Subtype
     TLKUSB_AUDID_SPK_OUTPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_OUT_SPEAKER,                // TerminalType
     0,                                         // AssociatedOutputTerminal
     TLKUSB_AUD_SRCID_SPK_OUTPUT_TERMINAL,      // SourceID
     TLKUSB_NO_DESCRIPTOR},

    // mic_input_terminal
    {sizeof(tlkusb_uacInputDesc_t),            // Length
     TLKUSB_TYPE_CS_INTERFACE,                 // Type
     AUDIO_DSUBTYPE_CSInterface_InputTerminal, // Subtype
     TLKUSB_AUDID_MIC_INPUT_TERMINAL_ID,       // TerminalID
     AUDIO_TERMINAL_IN_MIC, 0x00,
     TLKUSB_AUD_MIC_CHANNEL_COUNT, //channel
#if (TLKUSB_AUD_MIC_CHANNEL_COUNT == 2)
     0x0000, // 0x0003, // ChannelConfig - stero
#else
     0x01, // ChannelConfig - mono
#endif
     0x00, TLKUSB_NO_DESCRIPTOR},
    // mic_feature_unit
    {sizeof(tlkusb_uacSingleFeatureDesc_t),
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

    // spk_setting0
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_HS_INF_SPK,                  // InterfaceNumber  --2
     0,                                      // AlternateSetting
     0,                                      // bNumEndpoints
     TLKUSB_AUD_CSCP_AudioClass,             // Class
     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
     TLKUSB_NO_DESCRIPTOR},
    // spk_setting1
    {sizeof(tlkusb_stdInterfaceDesc_t),      // Length
     TLKUSB_TYPE_INTERFACE,                  // Type
     TLKUSB_AUD_HS_INF_SPK,                  // InterfaceNumber
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
        1,                                                                // bDelay
        {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    },
    // spk_audio_stream
    {sizeof(tlkusb_uacFormatDesc_t),        // Length
     TLKUSB_TYPE_CS_INTERFACE,              // Type
     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
     USB_AUDIO_FORMAT_PCM,                  // FormatType
     0x02,                                  // Channels
     0x02,                                  // SubFrameSize
     0x10,                                  // BitsResolution
     0x01,                                  // TotalDiscreteSampleRates
     {                                      // spk_sample_rate
      (TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xff), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF00) >> 8), ((TLKUSB_AUD_SPK_SAMPLE_RATE_DEF & 0xFF0000) >> 16)}},
    // spk_stream_endpoint
    {
        sizeof(tlkusb_uacStdEndpointDesc_t),                                                                // Length
        TLKUSB_TYPE_ENDPOINT,                                                                               // Type
        TLKUSB_EDP_DIR_OUT | TLKUSB_HS_UAC_EDP_SPK,                                                         // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_SPK_CHANNEL_LENGTH,                                                                      // MaxPacketSize
        0x04,                                                                                               //1,                                       // PollingIntervalMS
        0,                                                                                                  // Refresh
        0                                                                                                   // SyncEndpointNumber
    },
    // spk_stream_endpoint_spc
    {
        sizeof(tlkusb_uacSpcEndpointDesc_t),                       // Length
        TLKUSB_TYPE_CS_ENDPOINT,                                   // Type
        AUDIO_DSUBTYPE_CSInterface_General,                        // Subtype
        AUDIO_EP_FULL_PACKETS_ONLY | AUDIO_EP_SAMPLE_FREQ_CONTROL, //Attributes
        0x00,                                                      // LockDelayUnits
        {0x00, 0x00}                                               // LockDelay
    },

    // mic_setting0
    {
        sizeof(tlkusb_stdInterfaceDesc_t),      // Length
        TLKUSB_TYPE_INTERFACE,                  // Type
        TLKUSB_AUD_HS_INF_MIC,                  // InterfaceNumber
        0,                                      // AlternateSetting
        0,                                      // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,             // Class
        TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
        TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
        TLKUSB_NO_DESCRIPTOR,
    },
    // mic_setting1
    {
        sizeof(tlkusb_stdInterfaceDesc_t),      // Length
        TLKUSB_TYPE_INTERFACE,                  // Type
        TLKUSB_AUD_HS_INF_MIC,                  // InterfaceNumber
        1,                                      // AlternateSetting
        1,                                      // bNumEndpoints
        TLKUSB_AUD_CSCP_AudioClass,             // Class
        TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
        TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
        TLKUSB_NO_DESCRIPTOR,
    },
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
        TLKUSB_EDP_DIR_IN | TLKUSB_HS_UAC_EDP_MIC,                                                          // Endpoint
        TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) | (TLKUSB_EDP_USAGE_TYPE_DATA << 4), // Attributes
        TLKUSB_AUD_MIC_CHANNEL_LENGTH,                                                                      // MaxPacketSize
        4,                                                                                                  // PollingIntervalMS
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

    // //mic setting2
    // {
    //  sizeof(tlkusb_stdInterfaceDesc_t),      // Length
    //     TLKUSB_TYPE_INTERFACE,                  // Type
    //     TLKUSB_AUD_HS_INF_MIC,                  // InterfaceNumber
    //     2,                                      // AlternateSetting
    //     1,                                      // bNumEndpoints
    //     TLKUSB_AUD_CSCP_AudioClass,             // Class
    //     TLKUSB_AUD_CSCP_AudioStreamingSubclass, // Subclass
    //     TLKUSB_AUD_CSCP_StreamingProtocol,      // Protocol
    //     TLKUSB_NO_DESCRIPTOR},
    // // mic_audio_stream
    // {
    //  sizeof(tlkusb_uacInterfaceAsDesc_t),                              // Length
    //     TLKUSB_TYPE_CS_INTERFACE,                                         // Type
    //     AUDIO_DSUBTYPE_CSInterface_General,                               // Subtype
    //     6,                                                                // bTerminalLink: #6 USB Streaming OUT
    //     0,                                                                // FrameDelay
    //     {USB_AUDIO_FORMAT_PCM & 0xff, (USB_AUDIO_FORMAT_PCM >> 8) & 0xff} // StdioFormat
    // },
    // // mic_audio_format
    // {
    //  sizeof(tlkusb_uacFormatDesc_t),        // Length
    //     TLKUSB_TYPE_CS_INTERFACE,              // Type
    //     AUDIO_DSUBTYPE_CSInterface_FormatType, // Subtype
    //     USB_AUDIO_FORMAT_PCM,                  // FormatType
    //     TLKUSB_AUD_MIC_CHANNEL_COUNT,          // Channels
    //     TLKUSB_AUD_MIC_RESOLUTION_BIT/8,                                     // SubFrameSize
    //     TLKUSB_AUD_MIC_RESOLUTION_BIT,         // BitsResolution
    //     1,                                     // TotalDiscreteSampleRates
    //     {                                      // mic_sample_rate
    //         (TLKUSB_AUD_MIC_SAMPLE_RATE1 & 0xff),
    //         ((TLKUSB_AUD_MIC_SAMPLE_RATE1 >> 8) & 0xFF),
    //         ((TLKUSB_AUD_MIC_SAMPLE_RATE1 >> 16) & 0xFF)
    //     }
    // },
    // // mic_stream_endpoint
    // {
    //  sizeof(tlkusb_uacStdEndpointDesc_t),    // Length
    //     TLKUSB_TYPE_ENDPOINT,                   // Type
    //     TLKUSB_EDP_DIR_IN | TLKUSB_HS_UAC_EDP_MIC, // Endpoint
    //     TLKUSB_EDP_TYPE_ISOCHRONOUS | (TLKUSB_EDP_SYNC_TYPE_SYNC << 2) |
    //         (TLKUSB_EDP_USAGE_TYPE_DATA << 4),  // Attributes
    //     TLKUSB_AUD_MIC_CHANNEL_LENGTH1,         // MaxPacketSize
    //     1,                                      // PollingIntervalMS
    //     0,                                      // Refresh
    //     0                                       // SyncEndpointNumber
    // },
    // // mic_stream_endpoint_spc
    // {
    //  sizeof(tlkusb_uacSpcEndpointDesc_t), // Length
    //     TLKUSB_TYPE_CS_ENDPOINT,             // Type
    //     AUDIO_DSUBTYPE_CSInterface_General,  // Subtype
    //     AUDIO_EP_SAMPLE_FREQ_CONTROL,        // Attributes
    //     0,                                   // LockDelayUnits
    //     {0, 0}                               // LockDelay
    // },
};

static const tlkusb_stdStringDesc_t sMmiUsbLanguageDesc = {2 + 2,              //
                                                           TLKUSB_TYPE_STRING, // bDescriptorType
                                                           {TLKUSB_LANG_ID_ENGLISH}};

static const tlkusb_stdStringDesc_t sMmiUsbhsVendorDesc = {2 + sizeof(TLKUSB_STRING_VENDOR) - 2, //-2 is the end of the string
                                                           TLKUSB_TYPE_STRING,                   // Header
                                                           TLKUSB_STRING_VENDOR};

static const tlkusb_stdStringDesc_t sMmiUsbhsAudProductDesc = {2 + sizeof(TLKUSB_AUD_STRING_PRODUCT) - 2, //-2 is the end of the string
                                                               TLKUSB_TYPE_STRING,                        // Header
                                                               TLKUSB_AUD_STRING_PRODUCT};

static const tlkusb_stdStringDesc_t sMmiUsbhsAudSpkSerialDesc = {2 + sizeof(TLKUSB_AUD_STRING_SERIAL1) - 2, TLKUSB_TYPE_STRING, TLKUSB_AUD_STRING_SERIAL1};

/**
 * @brief       Process USB descriptor requests from the host
 * @param[in]   bus    - USB bus number (unused)
 * @param[in]   setup  - Pointer to USB control request structure containing descriptor request details
 * @return      Returns true if the descriptor was successfully processed and sent, false otherwise
 */
static unsigned char usbd_get_descriptor_process(const unsigned char bus, const usb_control_request_t *setup)
{
    (void)bus;
    unsigned char *desc_ptr = 0;

    unsigned char desc_type = (setup->wValue >> 8) & 0xFF;
    switch (desc_type) {
    case TLKUSB_TYPE_DEVICE:
    {
        desc_ptr = (uint8_t *)&sMmiUsbUacHsDeviceDesc;
        if (desc_ptr) {
            tlkhal_usb_write_ep_data(0, 0, desc_ptr, MIN((unsigned int)desc_ptr[0], setup->wLength), NULL);
        } else {
            return false;
        }
        break;
    }
    case TLKUSB_TYPE_CONFIGURE:
    {
        desc_ptr = (uint8_t *)&sMmiUsbUacHsConfigDesc;
        if (desc_ptr) {
            unsigned int write_len = (desc_ptr[3] << 8) | desc_ptr[2]; /* total len. */
            tlkhal_usb_write_ep_data(0, 0, desc_ptr, MIN(write_len, setup->wLength), NULL);
        } else {
            return false;
        }
        break;
    }
    case TLKUSB_TYPE_STRING:
    {
        switch (setup->wValue & 0xff) {
        case TLKUSB_STRING_INDEX_LANGUAGE:
        {
            desc_ptr = (uint8_t *)(&sMmiUsbLanguageDesc);
        } break;
        case TLKUSB_STRING_INDEX_VENDOR:
        {
            desc_ptr = (uint8_t *)(&sMmiUsbhsVendorDesc);
        } break;
        case TLKUSB_STRING_INDEX_PRODUCT:
        {
            desc_ptr = (uint8_t *)(&sMmiUsbhsAudProductDesc);
        } break;
        case TLKUSB_STRING_INDEX_SERIAL:
        {
            desc_ptr = (uint8_t *)(&sMmiUsbhsAudSpkSerialDesc);
        } break;
        default:
        {
            desc_ptr = 0;
        }
        }

        if (desc_ptr) {
            tlkhal_usb_write_ep_data(0, 0, desc_ptr, MIN(desc_ptr[0], setup->wLength), NULL);
        } else {
            return false;
        }
        break;
    }
    default:
        return false;
        break;
    }

    return true;
}

/**
 * @brief       This function handles USB descriptor requests.
 * @param[in]   bus    - the USB bus number.
 * @param[in]   setup  - pointer to the USB control request structure.
 * @return      Returns true if the descriptor was successfully processed, false otherwise.
 */
unsigned char usbd_audio_interface_cb(unsigned char bus, usb_control_request_t const *setup)
{
    (void)bus;
    (void)setup;
    unsigned char entity_id;
    unsigned char control_selector;

    control_selector = (setup->wValue >> 8) & 0xFF;
    entity_id        = (setup->wIndex >> 8) & 0xFF;

    switch (entity_id) {
    case 2:
    {
        if (control_selector == AUDIO_FEATURE_MUTE) {
            tlkhal_usb_write_ep_data(0, 0, &g_tlk_usb_cfg.out_mute, setup->wLength, NULL);
        } else if (control_selector == AUDIO_FEATURE_VOLUME) {
            /* code */
            switch (setup->bRequest) {
            case 0x81:
                tlkhal_usb_write_ep_data(0, 0, (uint8_t *)&g_tlk_usb_cfg.out_volume, setup->wLength, NULL);
                break;
            case 0x82:
                uint16_t min = TLKUSB_AUDSPK_VOL_MIN;
                tlkhal_usb_write_ep_data(0, 0, (uint8_t *)&min, setup->wLength, NULL);
                break;
            case 0x83:
                uint16_t max = TLKUSB_AUDSPK_VOL_MAX;
                tlkhal_usb_write_ep_data(0, 0, (uint8_t *)&max, setup->wLength, NULL);
                break;
            case 0x84:
                uint16_t res = TLKUSB_AUDSPK_VOL_RES;
                tlkhal_usb_write_ep_data(0, 0, (uint8_t *)&res, setup->wLength, NULL);
                break;
            case 0x01:
                tlkhal_usb_read_ep_data(0, 0, (uint8_t *)&g_tlk_usb_cfg.out_volume, setup->wLength, NULL);
                break;
            default:
                return false;
                break;
            }
        }

        break;
    }
    case 5:
    {
        if (control_selector == AUDIO_FEATURE_MUTE) {
            tlkhal_usb_write_ep_data(0, 0, &g_tlk_usb_cfg.in_mute, setup->wLength, NULL);
        } else if (control_selector == AUDIO_FEATURE_VOLUME) {
            /* code */
            switch (setup->bRequest) {
            case 0x81:
                tlkhal_usb_write_ep_data(0, 0, (unsigned char *)&g_tlk_usb_cfg.in_volume, setup->wLength, NULL);
                break;
            case 0x82:
                uint16_t min = TLKUSB_AUDMIC_VOL_MIN;
                tlkhal_usb_write_ep_data(0, 0, (unsigned char *)&min, setup->wLength, NULL);
                break;
            case 0x83:
                uint16_t max = TLKUSB_AUDMIC_VOL_MAX;
                tlkhal_usb_write_ep_data(0, 0, (unsigned char *)&max, setup->wLength, NULL);
                break;
            case 0x84:
                uint16_t res = TLKUSB_AUDMIC_VOL_RES;
                tlkhal_usb_write_ep_data(0, 0, (unsigned char *)&res, setup->wLength, NULL);
                break;
            case 0x87:
                //     uint16_t res = TLKUSB_AUDMIC_VOL_RES;
                //     usb0hw_write_ep_data(0, (unsigned char *)&res, setup->wLength);
                break;
            default:
                return false;
            }
        }
    } break;
    default:
        return false;
        break;
    }

    return true;
}

/**
 * @brief       This function handles audio interface requests.
 * @param[in]   bus         - the USB bus number.
 * @param[in]   setup       - pointer to the USB control request structure.
 * @param[in]   setup_stage - the setup stage of the request.
 * @return      Returns true if the request was successfully processed, false otherwise.
 */
unsigned char usbd_audio_interface_request_handler(unsigned char bus, usb_control_request_t const *setup, unsigned char setup_stage)
{
    (void)bus;
    (void)setup;
    (void)setup_stage;

    unsigned char const audio_alt_itf = setup->wValue & 0xFF;
    unsigned char const audio_itf     = setup->wIndex & 0xFF;

    switch (setup->bmRequestType_bit.type) {
    case 0:
    {
        /* standard request. */
        switch (setup->bRequest) {
        case TLKUSB_SETUP_REQ_SET_INTERFACE:
        {
            if (audio_itf == TLKUSB_AUD_HS_INF_SPK) {
                if (audio_alt_itf) {
                    g_tlk_usb_cfg.iso_out_en = true;
                    tlkhal_usb_read_ep_data(0, 0x06, audio_buffer, 192, NULL);
                } else {
                    g_tlk_usb_cfg.iso_out_en = false;
                }
            } else if (audio_itf == TLKUSB_AUD_HS_INF_MIC) {
                if (audio_alt_itf) {
                    g_tlk_usb_cfg.iso_in_en = true;
                    (void)sin_16k;
                    tlkhal_usb_write_ep_data(0, 0x07, (uint8_t *)in_audio_buffer, 32, NULL);
                } else {
                    g_tlk_usb_cfg.iso_in_en = false;
                }
            }

            tlkhal_usb_write_ep_data(0, 0, 0, 0, NULL);

            if (sTlkUsbReportUacStatusCB != NULL) {
                sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en);
            }

            break;
        }
        default:
            return false;
            break;
        }
        break;
    }
    case 1:
    {
        /* class request. */
        switch (setup->bmRequestType_bit.recipient) {
        case 1:
        {
            /* recipient interface */
            if (setup_stage) {
                if (((setup->wLength) && ((setup->bmRequestType_bit.direction) == USB_DIR_OUT))) {
                    usb0hw_read_ep_data(0, (unsigned char *)&g_tlk_usb_cfg.out_volume, setup->wLength);
                    return true;
                }

                return usbd_audio_interface_cb(bus, setup);
            }
        } break;
        case 2:
        {
            /* recipient endpoint. */
            uint8_t ep_num = (setup->wIndex) & 0xFF;

            if (setup->bmRequestType_bit.direction == USB_DIR_IN) {
                return usbd_audio_interface_cb(bus, setup);
            } else {
                if (setup_stage) {
                    /* setup stage. */
                    if (ep_num == 0x06) {
                        usb0hw_read_ep_data(0, (unsigned char *)&g_tlk_usb_cfg.out_sample_rate, setup->wLength);
                    } else if (ep_num == 0x87) {
                        usb0hw_read_ep_data(0, (unsigned char *)&g_tlk_usb_cfg.in_sample_rate, setup->wLength);
                    }

                } else {
                    tlkhal_usb_write_ep_data(0, 0, 0, 0, NULL);
                }
            }
            break;
        }
        default:
            return false;
        }
        break;
    }
    case 2:
    {
        /* vendor request. */
        return false;
    }
    default:
        return false;
    }

    return true;
}

/**
 * @brief       This function handles standard USB requests.
 * @param[in]   bus         - the USB bus number.
 * @param[in]   setup       - pointer to the USB control request structure.
 * @param[in]   setup_stage - the setup stage of the request.
 * @return      Returns true if the request was successfully processed, false otherwise.
 */
unsigned char usbd_standard_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage)
{
    (void)bus;

    switch (setup->bmRequestType_bit.recipient) {
    case 0:
    {
        /* recipient device. */
        switch (setup->bRequest) {
        case TLKUSB_SETUP_REQ_SET_ADDRESS:
        {
            tlk_printf("set address");
            usb0hw_set_address(setup->wValue & 0xFF);
            tlkhal_usb_write_ep_data(0, 0, 0, 0, NULL);
            break;
        }
        case TLKUSB_SETUP_REQ_GET_DESCRIPT:
        {
            return usbd_get_descriptor_process(bus, setup);
            break;
        }
        case TLKUSB_SETUP_REQ_SET_CONFIGURE:
        {
            uint8_t config_value = (setup->wValue) & 0xFF;
            if (config_value) {
                usb0hw_ep_open(0x06, 0x00, TLKUSB_EDP_TYPE_ISOCHRONOUS, 192);
                usb0hw_ep_open(0x07, 0x01, TLKUSB_EDP_TYPE_ISOCHRONOUS, 96);
            }
            tlkhal_usb_write_ep_data(0, 0, 0, 0, NULL);
            break;
        }
        default:
            return false;
            break;
        }
        break;
    }
    case 1:
    {
        /* recipient interface. */
        return usbd_audio_interface_request_handler(bus, setup, setup_stage);
    }
    case 2:
    {
        /* recipient endpoint. */
        return usbd_audio_interface_request_handler(bus, setup, setup_stage);
    }
    case 3:
        /* recipient other. */
        return false;
        break;
    default:
        return false;
        break;
    }

    return true;
}

/**
 * @brief       This function handles USB class requests.
 * @param[in]   bus         - the USB bus number.
 * @param[in]   setup       - pointer to the USB control request structure.
 * @param[in]   setup_stage - the setup stage of the request.
 * @return      Returns true if the request was successfully processed, false otherwise.
 */
unsigned char usbd_class_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage)
{
    (void)bus;

    switch (setup->bmRequestType_bit.recipient) {
    case 0:
        /* recipient device. */
        return false;
        break;
    case 1:
    {
        /* recipient interface. */
        return usbd_audio_interface_request_handler(bus, setup, setup_stage);
        break;
    }
    case 2:
    {
        /* recipient endpoint. */
        return usbd_audio_interface_request_handler(bus, setup, setup_stage);
        break;
    }
    case 3:
        /* recipient other. */
        return false;
        break;
    default:
        return false;
        break;
    }

    return true;
}

/**
 * @brief       Process USB control requests based on request type
 * @param[in]   bus - USB bus number
 * @param[in]   setup - Pointer to USB control request structure
 * @param[in]   setup_stage - Current stage of setup request processing
 * @return      Returns true if request type is vendor-specific or reserved, false for standard and class requests
 *              which are handled by their respective handlers
 */
unsigned char usbd_control_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage)
{
    (void)bus;

    switch (setup->bmRequestType_bit.type) {
    case 0:
        /* standard request. */
        return usbd_standard_request_process(bus, setup, setup_stage);
        break;
    case 1:
        /* class request. */
        return usbd_class_request_process(bus, setup, setup_stage);
        break;
    case 2:
        break;
    default:
        return false;
        break;
    }

    return true;
}

/**
 * @brief       This function handles UAC HS events and reports mic and spk status to upper layer.
 * @return      none.
 */
void tlkusb_uachs_handler(void)
{
    /*Report mic and spk status to upper layer.*/
    if (sTlkUsbReportUacStatusCB != NULL) {
        sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en);
    }
}

#else
/**
 * @brief       This function handles UAC HS events and reports mic and spk status to upper layer.
 * @return      none.
 */
void tlkusb_uachs_handler(void) {}

/**
 * @brief       Process USB control requests based on request type
 * @param[in]   bus - USB bus number
 * @param[in]   setup - Pointer to USB control request structure
 * @param[in]   setup_stage - Current stage of setup request processing
 * @return      Returns true if request type is vendor-specific or reserved, false for standard and class requests
 *              which are handled by their respective handlers
 */
unsigned char usbd_control_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage)
{
    (void)bus;
    (void)setup;
    (void)setup_stage;
    return false;
}
#endif
