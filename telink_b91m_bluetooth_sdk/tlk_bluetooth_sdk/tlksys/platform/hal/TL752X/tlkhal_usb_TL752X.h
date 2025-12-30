/********************************************************************************************************
 * @file    tlkhal_usb_TL752X.h
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
#ifndef TLKHAL_USB_TL752X_H
#define TLKHAL_USB_TL752X_H

/*************************** macro definition ********************************/
#define USBD_VID 0x2d99
#define USBD_PID 0xe032

#define USBD_LANGID_STRING 0x409

#define AUDIO_OUT_EP 0x03
#define AUDIO_IN_EP  0x83

#define HID_OUT_EP 0x01
#define HID_IN_EP  0x81

#define USBD_AUDIO_FREQ    48000
#define USBD_AUDIO_WIDTH   16
#define USBD_AUDIO_CHANNEL 2

#define USBD_MIC_FREQ    48000
#define USBD_MIC_WIDTH   16
#define USBD_MIC_CHANNEL 2

//#define UAC_OUT_PACKET_LEN (uint32_t)(((USBD_AUDIO_FREQ * (USBD_AUDIO_WIDTH / 8) * USBD_AUDIO_CHANNEL) / 1000))
//#define UAC_IN_PACKET_LEN  (uint32_t)(((USBD_MIC_FREQ * (USBD_MIC_WIDTH / 8) * USBD_MIC_CHANNEL) / 1000))
#define UAC_OUT_PACKET_LEN (48)
#define UAC_IN_PACKET_LEN  (24)

#define UAC_OUT_PACKET_NUM     20
#define UAC_IN_PACKET_NUM      20
#define SPEAKER_TOTAL_BUF_SIZE ((uint32_t)(UAC_OUT_PACKET_LEN * UAC_OUT_PACKET_NUM))
#define MIC_TOTAL_BUF_SIZE     ((uint32_t)(UAC_IN_PACKET_LEN * UAC_IN_PACKET_NUM))

extern void usbd_audio_init(void);

#endif // TLKHAL_USB_TL752X_H
