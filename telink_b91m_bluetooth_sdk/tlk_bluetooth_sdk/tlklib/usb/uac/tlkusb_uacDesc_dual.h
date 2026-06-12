/********************************************************************************************************
 * @file    tlkusb_uacDesc_dual.h
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
#include "tlklib/usb/uac/tlkusb_uacDesc_dual.h"
#include "tlklib/usb/tlkusb_struct.h"

#ifndef TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE
#define TLKUSB_AUD_SINGLE_MIC_SAMPLE_RATE TLKUSB_AUD_SAMPLE_16000
#endif

typedef enum
{
    TLKUSB_UAC_MODE_SINGLE_SOUNDCARD,
    TLKUSB_UAC_MODE_DUAL_SOUNDCARD,
    TLKUSB_UAC_MODE_MAX,
} TLKUSB_UAC_MODE_ENUM;

typedef enum
{
    TLKUSB_AUD_SINGLE_INF_CTR,
#if (TLKUSB_UAC_SPK_ENABLE)
    TLKUSB_AUD_SINGLE_INF_SPK,
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    TLKUSB_AUD_SINGLE_INF_MIC,
#endif
#if (TLKUSB_UAC_HID_ENABLE)
    TLKUSB_AUD_SINGLE_INF_HID,
#endif
    TLKUSB_AUD_SINGLE_INF_MAX,
} TLKUSB_AUD_SINGLE_INF_ENUM;


typedef enum
{
    TLKUSB_AUD_DUAL_INF_CTR0,
#if (TLKUSB_UAC_SPK_ENABLE)
    TLKUSB_AUD_DUAL_INF_SPK0,
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    TLKUSB_AUD_DUAL_INF_MIC,
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    TLKUSB_AUD_DUAL_INF_CTR1,
    TLKUSB_AUD_DUAL_INF_SPK1,
#endif
#if (TLKUSB_UAC_HID_ENABLE)
    TLKUSB_AUD_DUAL_INF_HID,
#endif
    TLKUSB_AUD_DUAL_INF_MAX,
} TLKUSB_AUD_DUAL_INF_ENUM;

typedef struct
{
    tlkusb_stdConfigureDesc_t config;
#if (TLKUSB_UAC_SPK_ENABLE || TLKUSB_UAC_MIC_ENABLE)
    tlkusb_stdInterfaceDesc_t audCtrInf;
#if (TLKUSB_UAC_SPK_ENABLE && TLKUSB_UAC_MIC_ENABLE)
    tlkusb_uacInterfaceAcTLDesc_t audCtrAcInf;
#else
    tlkusb_uacInterfaceAcDesc_t audCtrAcInf;
#endif //(TLKUSB_UAC_SPK_ENABLE && TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_SPK_ENABLE)
    // Input Terminal
    tlkusb_uacInputDesc_t spkInputDesc;
    // Feature Unit
    tlkusb_uacDoubleFeatureDesc_t spkFeatureDesc;
    // Output Terminal
    tlkusb_uacOutputDesc_t spkOutputDesc;
#endif //(TLKUSB_UAC_SPK_ENABLE)

#if (TLKUSB_UAC_MIC_ENABLE)
    // Input Terminal
    tlkusb_uacInputDesc_t micInputDesc;
    // Feature Unit
    tlkusb_uacSingleFeatureDesc_t micFeatureDesc;
    // Output Terminal
    tlkusb_uacOutputDesc_t micOutputDesc;
#endif //(TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_SPK_ENABLE)
    // audio streaming interface
    tlkusb_stdInterfaceDesc_t   spkSetting0Inf;
    tlkusb_stdInterfaceDesc_t   spkSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t spkDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spkFormatDesc;
    tlkusb_uacStdEndpointDesc_t spkStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spkSpcEdpDesc;
#endif //(TLKUSB_UAC_SPK_ENABLE)

#if (TLKUSB_UAC_MIC_ENABLE)
    tlkusb_stdInterfaceDesc_t   micSetting0Inf;
    tlkusb_stdInterfaceDesc_t   micSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t micDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      micFormatDesc;
    tlkusb_uacStdEndpointDesc_t micStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t micSpcEdpDesc;
#endif // (TLKUSB_UAC_MIC_ENABLE)
#endif //(TLKUSB_UAC_SPK_ENABLE || TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_HID_ENABLE)
    tlkusb_stdInterfaceDesc_t keyboardInf;
    tlkusb_HidEndpointDesc_t  keyboardHid;
    tlkusb_stdEndpointDesc_t  keyboardEdp;
#endif // (TLKUSB_UAC_HID_ENABLE)
} __attribute__((packed)) tlkusb_uacAudConfigDesc_single_t;

typedef struct
{
    tlkusb_stdConfigureDesc_t config;
#if (TLKUSB_UAC_SPK_ENABLE || TLKUSB_UAC_MIC_ENABLE)
    tlkusb_stdInterfaceDesc_t audCtrInf0;
#if (TLKUSB_UAC_SPK_ENABLE && TLKUSB_UAC_MIC_ENABLE)
    tlkusb_uacInterfaceAcTLDesc_t audCtrAcInf;
#else
    tlkusb_uacInterfaceAcDesc_t audCtrAcInf;
#endif //(TLKUSB_UAC_SPK_ENABLE && TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_SPK_ENABLE)
    tlkusb_uacInputDesc_t         spkInputDesc0;   // Input Terminal
    tlkusb_uacDoubleFeatureDesc_t spkFeatureDesc0; // Feature Unit
    tlkusb_uacOutputDesc_t        spkOutputDesc0;  // Output Terminal
#endif                                             //(TLKUSB_UAC_SPK_ENABLE)

#if (TLKUSB_UAC_MIC_ENABLE)
    tlkusb_uacInputDesc_t         micInputDesc;   // Input Terminal
    tlkusb_uacSingleFeatureDesc_t micFeatureDesc; // Feature Unit
    tlkusb_uacOutputDesc_t        micOutputDesc;  // Output Terminal
#endif                                            //(TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_SPK_ENABLE)
    // audio streaming interface
    tlkusb_stdInterfaceDesc_t   spk0Setting0Inf;
    tlkusb_stdInterfaceDesc_t   spk0Setting1Inf;
    tlkusb_uacInterfaceAsDesc_t spk0DatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spk0FormatDesc;
    tlkusb_uacStdEndpointDesc_t spk0StdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spk0SpcEdpDesc;
#endif //(TLKUSB_UAC_SPK_ENABLE)

#if (TLKUSB_UAC_MIC_ENABLE)
    tlkusb_stdInterfaceDesc_t   micSetting0Inf;
    tlkusb_stdInterfaceDesc_t   micSetting1Inf;
    tlkusb_uacInterfaceAsDesc_t micDatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      micFormatDesc;
    tlkusb_uacStdEndpointDesc_t micStdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t micSpcEdpDesc;
#endif //(TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_SPK_ENABLE)
    tlkusb_stdInterfaceDesc_t   audCtrInf1;
    tlkusb_uacInterfaceAcDesc_t audCtrAcInf1;

    tlkusb_uacInputDesc_t         spkInputDesc1;   // Input Terminal
    tlkusb_uacDoubleFeatureDesc_t spkFeatureDesc1; // Feature Unit
    tlkusb_uacOutputDesc_t        spkOutputDesc1;  // Output Terminal
    // audio streaming interface
    tlkusb_stdInterfaceDesc_t   spk1Setting0Inf;
    tlkusb_stdInterfaceDesc_t   spk1Setting1Inf;
    tlkusb_uacInterfaceAsDesc_t spk1DatAsInf;
    // audio format
    tlkusb_uacFormatDesc_t      spk1FormatDesc;
    tlkusb_uacStdEndpointDesc_t spk1StdEdpDesc;
    tlkusb_uacSpcEndpointDesc_t spk1SpcEdpDesc;
#endif //(TLKUSB_UAC_SPK_ENABLE)
#endif //(TLKUSB_UAC_SPK_ENABLE || TLKUSB_UAC_MIC_ENABLE)

#if (TLKUSB_UAC_HID_ENABLE)
    tlkusb_stdInterfaceDesc_t keyboardInf;
    tlkusb_HidEndpointDesc_t  keyboardHid;
    tlkusb_stdEndpointDesc_t  keyboardEdp;
#endif //(TLKUSB_UAC_HID_ENABLE)
} __attribute__((packed)) tlkusb_uacAudConfigDesc_dual_t;

void tlkusb_set_uac_mode(unsigned char mode);
int  tlkusb_get_usb_mode(unsigned char mode);
int  tlkusb_get_uac_mic_inf(unsigned char inf);
int  tlkusb_get_uac_spk_inf(unsigned char inf);
int  tlkusb_get_uac_spk1_inf(unsigned char inf);
int  tlkusb_get_uac_hid_inf(unsigned char inf);
