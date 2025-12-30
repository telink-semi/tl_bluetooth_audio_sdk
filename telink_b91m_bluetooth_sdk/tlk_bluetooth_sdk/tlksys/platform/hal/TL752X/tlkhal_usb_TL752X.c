/********************************************************************************************************
 * @file    tlkhal_usb_TL752X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#include "tlklib/usb/tlkusb_hal.h"

#if MCU_CORE_TYPE == MCU_CORE_TL752X
#include "tlkhal_usb_TL752X.h"

/************************* variable definition *******************************/
USB_MEM_ALIGNX uint8_t clk_range[] = {0x05, 0x00, 0x80, 0xBB, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x88, 0x58, 0x01,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xB1, 0x02, 0x00,
                                      0x10, 0xB1, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, 0x00, 0xEE, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};

/* USB Standard Device Descriptor */
uint8_t usbd_audio_dev_desc[] = {
    0x12,                 /* bLength */
    USB_DESC_TYPE_DEVICE, /* bDescriptorType */
    0x00,                 /* bcdUSB2.0 */
    0x02,
    0x00,                 /* bDeviceClass */
    0x00,                 /* bDeviceSubClass */
    0x00,                 /* bDeviceProtocol */
    USB_CTL_EP_MPS,       /* bMaxPacketSize */
    USB_LOBYTE(USBD_VID), /* idVendor */
    USB_HIBYTE(USBD_VID), /* idVendor */
    USB_LOBYTE(USBD_PID), /* idVendor */
    USB_HIBYTE(USBD_PID), /* idVendor */
    0x00,                 /* bcdDevice rel. 2.00 */
    0x02,
    STR_DESC_ID_MFC,     /* Index of manufacturer string */
    STR_DESC_ID_PRODUCT, /* Index of product string */
    0x00,                /* Index of serial number string */
    0x01                 /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB manufacturerstr Device Descriptor */
uint8_t usbd_audio_manufacturerstr_desc[] = {
    0x20,                 /* bLength */
    USB_DESC_TYPE_STRING, /* bDescriptorType */
    'H',
    0x00,
    'E',
    0x00,
    'C',
    0x00,
    'A',
    0x00,
    'T',
    0x00,
    'E',
    0x00,
    ' ',
    0x00,
    'G',
    0x00,
    '4',
    0x00,
    ' ',
    0x00,
    'S',
    0x00,
    ' ',
    0x00,
    'P',
    0x00,
    'R',
    0x00,
    'O',
    0x00,
};

/* USB productstr Device Descriptor */
uint8_t usbd_audio_productstr_desc[] = {
    0x20,                 /* bLength */
    USB_DESC_TYPE_STRING, /* bDescriptorType */
    'H',
    0x00,
    'E',
    0x00,
    'C',
    0x00,
    'A',
    0x00,
    'T',
    0x00,
    'E',
    0x00,
    ' ',
    0x00,
    'G',
    0x00,
    '4',
    0x00,
    ' ',
    0x00,
    'S',
    0x00,
    ' ',
    0x00,
    'P',
    0x00,
    'R',
    0x00,
    'O',
    0x00,
};

/* USB AUDIO device Configuration Descriptor */
uint8_t usbd_audio_cfg_desc[] = {
    0x09, // bLength
    0x02, // bDescriptorType (Configuration)
    0xDA,
    0x00, // wTotalLength 218
    0x03, // bNumInterfaces 3
    0x01, // bConfigurationValue
    0x00, // iConfiguration (String Index)
    0xC0, // bmAttributes Self Powered
    0xFA, // bMaxPower 500mA

    0x08, // bLength
    0x0B, // bDescriptorType (Unknown)
    0x00, 0x03, 0x01, 0x00, 0x20, 0x00,
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x00, // bInterfaceNumber 0
    0x00, // bAlternateSetting
    0x00, // bNumEndpoints 0
    0x01, // bInterfaceClass (Audio)
    0x01, // bInterfaceSubClass (Audio Control)
    0x20, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    0x09, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x01, // bDescriptorSubtype (CS_INTERFACE -> HEADER)
    0x00,
    0x02, // bcdADC 2.00
    0x08,
    0x4B, // wTotalLength 19208
    0x00, // binCollection 0x00
    0x00, // baInterfaceNr 0

    0x08, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x0A, // bDescriptorSubtype Unknown
    0x29, 0x03, 0x07, 0x00, 0x00,
    0x11, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x02, // bDescriptorSubtype (CS_INTERFACE -> INPUT_TERMINAL)
    0x01, // bTerminalID
    0x01,
    0x01, // wTerminalType (USB Streaming)
    0x00, // bAssocTerminal
    0x29, // bNrChannels 41
    0x02,
    0x00, // wChannelConfig (Right Front)
    0x00, // iChannelNames
    0x00, // iTerminal
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x03, // bDescriptorSubtype (CS_INTERFACE -> OUTPUT_TERMINAL)
    0x12, // bTerminalID
    0x03,
    0x06, // wTerminalType (Line Connector)
    0x00, // bAssocTerminal
    0x01, // bSourceID
    0x29, // iTerminal
    0x00, 0x00, 0x00,
    0x11, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x02, // bDescriptorSubtype (CS_INTERFACE -> INPUT_TERMINAL)
    0x03, // bTerminalID
    0x03,
    0x06, // wTerminalType (Line Connector)
    0x00, // bAssocTerminal
    0x29, // bNrChannels 41
    0x02,
    0x00, // wChannelConfig (Right Front)
    0x00, // iChannelNames
    0x00, // iTerminal
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x03, // bDescriptorSubtype (CS_INTERFACE -> OUTPUT_TERMINAL)
    0x14, // bTerminalID
    0x01,
    0x01, // wTerminalType (USB Streaming)
    0x00, // bAssocTerminal
    0x03, // bSourceID
    0x29, // iTerminal
    0x00, 0x00, 0x00,
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x01, // bInterfaceNumber 1
    0x00, // bAlternateSetting
    0x00, // bNumEndpoints 0
    0x01, // bInterfaceClass (Audio)
    0x02, // bInterfaceSubClass (Audio Streaming)
    0x20, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x01, // bInterfaceNumber 1
    0x01, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x01, // bInterfaceClass (Audio)
    0x02, // bInterfaceSubClass (Audio Streaming)
    0x20, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    0x10, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x01, // bDescriptorSubtype (CS_INTERFACE -> AS_GENERAL)
    0x01, // bTerminalLink
    0x00, // bDelay 0
    0x01,
    0x01, // wFormatTag
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x02, // bDescriptorSubtype (CS_INTERFACE -> FORMAT_TYPE)
    0x01, // bFormatType 1
    0x04, // bNrChannels 4
    0x18, // bSubFrameSize 24
          // bSamFreqType -1

    0x07, // bLength
    0x05, // bDescriptorType (See Next Line)
    0x03, // bEndpointAddress (OUT/H2D)
    0x05, // bmAttributes (Isochronous, Async, Data EP)
    0xC8,
    0x00, // wMaxPacketSize 200
    0x01, // bInterval 1 (unit depends on device speed)

    0x08, // bLength
    0x25, // bDescriptorType (See Next Line)
    0x01, // bDescriptorSubtype (CS_ENDPOINT -> EP_GENERAL)
    0x00, // bmAttributes (None)
    0x00, // bLockDelayUnits
    0x00,
    0x00, // wLockDelay 0
    0x00,
    0x07, // bLength
    0x05, // bDescriptorType (See Next Line)
    0x82, // bEndpointAddress (IN/D2H)
    0x11, // bmAttributes (Isochronous, No Sync, Feedback EP)
    0x04,
    0x00, // wMaxPacketSize 4
    0x08, // bInterval 8 (unit depends on device speed)

    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x02, // bInterfaceNumber 2
    0x00, // bAlternateSetting
    0x00, // bNumEndpoints 0
    0x01, // bInterfaceClass (Audio)
    0x02, // bInterfaceSubClass (Audio Streaming)
    0x20, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x02, // bInterfaceNumber 2
    0x01, // bAlternateSetting
    0x01, // bNumEndpoints 1
    0x01, // bInterfaceClass (Audio)
    0x02, // bInterfaceSubClass (Audio Streaming)
    0x20, // bInterfaceProtocol
    0x00, // iInterface (String Index)

    0x10, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x01, // bDescriptorSubtype (CS_INTERFACE -> AS_GENERAL)
    0x14, // bTerminalLink
    0x00, // bDelay 0
    0x01,
    0x01, // wFormatTag
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x06, // bLength
    0x24, // bDescriptorType (See Next Line)
    0x02, // bDescriptorSubtype (CS_INTERFACE -> FORMAT_TYPE)
    0x01, // bFormatType 1
    0x02, // bNrChannels 4
    0x10, // bSubFrameSize 16
          // bSamFreqType -1

    0x07, // bLength
    0x05, // bDescriptorType (See Next Line)
    0x83, // bEndpointAddress (IN/D2H)
    0x05, // bmAttributes (Isochronous, Async, Data EP)
    0xC8,
    0x00, // wMaxPacketSize 200
    0x01, // bInterval 1 (unit depends on device speed)

    0x08, // bLength
    0x25, // bDescriptorType (See Next Line)
    0x01, // bDescriptorSubtype (CS_ENDPOINT -> EP_GENERAL)
    0x00, // bmAttributes (None)
    0x00, // bLockDelayUnits
    0x00,
    0x00, // wLockDelay 0
    0x00,
    // 218 bytes
};

/* USB Standard Device Descriptor */
uint8_t usbd_audio_dev_qualifier_desc[] = {
    0x0a, USB_DESC_TYPE_DEVICE_QUALIFIER, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00,
};

/* USB Standard Device Descriptor */
uint8_t usbd_audio_string_lang_id[] = {USB_LANGID_INIT(USBD_LANGID_STRING)};

uint8_t  IsocOutBuff[SPEAKER_TOTAL_BUF_SIZE];
uint8_t *IsocOutWrPtr = IsocOutBuff;
uint8_t *IsocOutRdPtr = IsocOutBuff;

uint32_t  IsocInBuff[MIC_TOTAL_BUF_SIZE];
uint32_t *IsocInRdPtr = IsocInBuff;

uint8_t PlayFlag     = 0;
uint8_t recordFlag   = 0;
uint8_t send_ok_flag = 0;

usbd_audio_handle_t usbaudiohandle;

/**
 * @brief  USB Device Reset callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_reset_cb(void)
{
    soc_printf("interface reset.\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device SOF callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_sof_cb(void)
{
    //soc_printf("sof.\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device Connect callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_connected_cb(void)
{
    soc_printf("USB Device Connected.\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device Disconnect callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_disconnected_cb(void)
{
    soc_printf("USB Device Disconnected.\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device Open callback of user function.
 * @param[in] intf_num - interface number
 * @param[in] alt_set - alternate setting number
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_open_cb(uint16_t intf_num, uint16_t alt_set)
{
    (void)alt_set;
    if (intf_num == 1) {
        /* setup first out ep read transfer */
        usbd_ep_preparerx(AUDIO_OUT_EP, IsocOutBuff, UAC_OUT_PACKET_LEN);
        soc_printf("OPEN1\r\n");
    } else if (intf_num == 2) {
        recordFlag = 1;
        soc_printf("OPEN2\r\n");
    }
    return USB_OK;
}

/**
 * @brief  USB Device Close callback of user function.
 * @param[in] intf_num - interface number
 * @param[in] alt_set - alternate setting number
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_close_cb(uint16_t intf_num, uint16_t alt_set)
{
    (void)alt_set;
    if (intf_num == 1) {
        soc_printf("CLOSE1\r\n");
    } else if (intf_num == 2) {
        recordFlag = 0;
        soc_printf("CLOSE2\r\n");
    }
    return USB_OK;
}

/**
 * @brief  USB Device Resume callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_resume_cb(void)
{
    soc_printf("Device Resumed in Idle Mode.\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device Suspend callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_suspend_cb(void)
{
    soc_printf("Device In suspend mode\r\n");
    return USB_OK;
}

/**
 * @brief  USB Device Configured callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_configured_cb(void)
{
    soc_printf("set configuration.\r\n");
    /* Allocate Audio structure */
    // usbaudiohandle.control.hp_mute
    usbaudiohandle.control.hp_mute    = 0;
    usbaudiohandle.control.hp_vol_cur = 98;
    usbaudiohandle.control.hp_vol_max = 129;
    usbaudiohandle.control.hp_vol_min = 2;
    usbaudiohandle.control.hp_vol_res = 1;

    usbaudiohandle.control.mic_mute    = 0;
    usbaudiohandle.control.mic_vol_cur = 102;
    usbaudiohandle.control.mic_vol_max = 102;
    usbaudiohandle.control.mic_vol_min = 2;
    usbaudiohandle.control.mic_vol_res = 1;

    return USB_OK;
}

/**
 * @brief  USB Device Set Remote Wakeup callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_set_remote_wakeup_cb(void)
{
    return USB_OK;
}

/**
 * @brief  USB Device Clear Remote Wakeup callback of user function.
 * @param  None
 * @returns usb_status_t
*/
static usb_status_t usbd_usr_clear_remote_wakeup_cb(void)
{
    return USB_OK;
}

usbd_usr_cb_t user_cb = {
    usbd_usr_reset_cb,
    usbd_usr_sof_cb,
    usbd_usr_connected_cb,
    usbd_usr_disconnected_cb,
    usbd_usr_suspend_cb,
    usbd_usr_resume_cb,
    usbd_usr_configured_cb,
    usbd_usr_open_cb,
    usbd_usr_close_cb,
    usbd_usr_set_remote_wakeup_cb,
    usbd_usr_clear_remote_wakeup_cb,
    NULL,
    NULL,
};

/**
 * @brief  USB Audio Set Cur Volume callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @param[in]  volume: volume value
 * @returns None
*/
static void usbd_audio_set_cur_volume(uint8_t ep, uint8_t ch, uint32_t volume)
{
    (void)ch;
    if (ep == AUDIO_OUT_EP) {
        usbaudiohandle.control.hp_vol_cur = volume;
    } else if (ep == AUDIO_IN_EP) {
        usbaudiohandle.control.mic_vol_cur = volume;
    }
}

/**
 * @brief  USB Audio Get Cur Volume callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @returns volume value
*/
static uint32_t usbd_audio_get_cur_volume(uint8_t ep, uint8_t ch)
{
    (void)ch;
    uint32_t volume = 0;
    if (ep == AUDIO_OUT_EP) {
        volume = usbaudiohandle.control.hp_vol_cur;
    } else if (ep == AUDIO_IN_EP) {
        volume = usbaudiohandle.control.mic_vol_cur;
    }
    return volume;
}

/**
 * @brief  USB Audio Get Min Volume callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @returns volume value
*/
static uint32_t usbd_audio_get_min_volume(uint8_t ep, uint8_t ch)
{
    (void)ch;
    uint32_t volume = 0;
    if (ep == AUDIO_OUT_EP) {
        volume = usbaudiohandle.control.hp_vol_min;
    } else if (ep == AUDIO_IN_EP) {
        volume = usbaudiohandle.control.mic_vol_min;
    }
    return volume;
}

/**
 * @brief  USB Audio Get Max Volume callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @returns volume value
*/
static uint32_t usbd_audio_get_max_volume(uint8_t ep, uint8_t ch)
{
    (void)ch;
    uint32_t volume = 0;
    if (ep == AUDIO_OUT_EP) {
        volume = usbaudiohandle.control.hp_vol_max;
    } else if (ep == AUDIO_IN_EP) {
        volume = usbaudiohandle.control.mic_vol_max;
    }
    return volume;
}

/**
 * @brief  USB Audio Get Res Volume callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @returns volume value
*/
static uint32_t usbd_audio_get_res_volume(uint8_t ep, uint8_t ch)
{
    (void)ch;
    uint32_t volume = 0;
    if (ep == AUDIO_OUT_EP) {
        volume = usbaudiohandle.control.hp_vol_res;
    } else if (ep == AUDIO_IN_EP) {
        volume = usbaudiohandle.control.mic_vol_res;
    }
    return volume;
}

/**
 * @brief  USB Audio Set Mute callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @param[in]  mute: mute value
 * @returns None
*/
static void usbd_audio_set_mute(uint8_t ep, uint8_t ch, uint8_t mute)
{
    (void)ch;
    if (ep == AUDIO_OUT_EP) {
        usbaudiohandle.control.hp_mute = mute;
    } else if (ep == AUDIO_IN_EP) {
        usbaudiohandle.control.mic_mute = mute;
    }
}

/**
 * @brief  USB Audio Get Mute callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  ch: channel number
 * @returns mute value
*/
static uint8_t usbd_audio_get_mute(uint8_t ep, uint8_t ch)
{
    (void)ch;
    uint8_t mute = 0;
    if (ep == AUDIO_OUT_EP) {
        mute = usbaudiohandle.control.hp_mute;
    } else if (ep == AUDIO_IN_EP) {
        mute = usbaudiohandle.control.mic_mute;
    }
    return mute;
}

/**
 * @brief  USB Audio Set Sample Rate callback of user function.
 * @param[in]  ep: endpoint number
 * @param[in]  sample_rate: sample rate
 * @returns None
*/
static void usbd_audio_set_sample_rate(uint8_t ep, uint32_t sample_rate)
{
    if (ep == AUDIO_OUT_EP) {
        usbaudiohandle.control.hp_freq = sample_rate;
    } else if (ep == AUDIO_IN_EP) {
        usbaudiohandle.control.mic_freq = sample_rate;
    }
}

/**
 * @brief  USB Audio Get Sample Rate callback of user function.
 * @param[in]  ep: endpoint number
 * @returns sample rate
*/
static uint32_t usbd_audio_get_sample_rate(uint8_t ep)
{
    uint32_t sample_rate = 0;
    if (ep == AUDIO_OUT_EP) {
        sample_rate = usbaudiohandle.control.hp_freq;
    } else if (ep == AUDIO_IN_EP) {
        sample_rate = usbaudiohandle.control.mic_freq;
    }
    return sample_rate;
}

/**
 * @brief  USB Audio Get Sample Rate Table callback of user function.
 * @param[in]  ep: endpoint number
 * @param[out]  sample_rate_table: sample rate table
 * @returns none
*/
static void usbd_audio_get_sample_rate_table(uint8_t ep, uint8_t **sample_rate_table)
{
    (void)ep;
    (void)sample_rate_table;
    *sample_rate_table = clk_range;
}

usbd_audio_cb_t usbd_audio_cb = {
    usbd_audio_set_cur_volume, usbd_audio_get_cur_volume, usbd_audio_get_min_volume,  usbd_audio_get_max_volume,  usbd_audio_get_res_volume,
    usbd_audio_set_mute,       usbd_audio_get_mute,       usbd_audio_set_sample_rate, usbd_audio_get_sample_rate, usbd_audio_get_sample_rate_table,
};

audio_entity_info_t audio_entity_table[] = {
    {.bEntityId = 0x29, .bDescriptorSubtype = AUDIO_CONTROL_CLOCK_SOURCE, .ep = AUDIO_OUT_EP},
};

/**
 * @brief  usbd_audio_datain
 *         Handles the audio IN data stage.
 * @param[in]  epnum: endpoint number
 * @param[in]  len: data Length
 * @returns status
 */
static usb_status_t usbd_audio_datain(uint8_t epnum, uint32_t len)
{
    (void)epnum;
    (void)len;
    drv_gpio_writepin(GPIO_PORT_B, GPIO_PIN_0, GPIO_PIN_SET);
    drv_gpio_writepin(GPIO_PORT_B, GPIO_PIN_0, GPIO_PIN_RESET);
    send_ok_flag = 1;
    return USB_OK;
}

/**
 * @brief  usbd_audio_dataout
 *         Handles the Audio Out data stage.
 * @param[in]  epnum: endpoint number
 * @param[in]  len: data Length
 * @returns status
 */
static usb_status_t usbd_audio_dataout(uint8_t epnum, uint32_t len)
{
    (void)epnum;
    (void)len;
    /* Increment the Buffer pointer or roll it back when all buffers are full */
    if (IsocOutWrPtr >= (IsocOutBuff + SPEAKER_TOTAL_BUF_SIZE - UAC_OUT_PACKET_LEN)) {
        /* All buffers are full: roll back */
        IsocOutWrPtr = IsocOutBuff;
    } else {
        /* Increment the buffer pointer */
        IsocOutWrPtr += UAC_OUT_PACKET_LEN;
    }

    /* Prepare Out endpoint to receive next audio packet */
    usbd_ep_preparerx(AUDIO_OUT_EP, (uint8_t *)(IsocOutWrPtr), UAC_OUT_PACKET_LEN);

    /* Trigger the start of streaming only when half buffer is full */
    if ((PlayFlag == 0) && (IsocOutWrPtr >= (IsocOutBuff + (SPEAKER_TOTAL_BUF_SIZE / 2)))) {
        /* Enable start of Streaming */
        PlayFlag = 1;
    }
    return USB_OK;
}

/**
 * @brief  usbd_audio_hid_in
 *         Handles the hid IN data stage.
 * @param[in]  epnum: endpoint number
 * @param[in]  len: data Length
 * @returns status
 */
static usb_status_t usbd_audio_hid_in(uint8_t epnum, uint32_t len)
{
    (void)epnum;
    (void)len;
    return USB_OK;
}

/**
 * @brief  Initialize USB hardware
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_hw_init(uint8_t port_id, void *userArg)
{
    (void)userArg;
    if (port_id >= TLK_CFG_USB_NUMB) {
        return;
    }
    if (port_id == 0) {
        //        usb0hw_init(USB0_SPEED_HIGH);
    } else if (port_id == 1) {
        usb1hw_init();
    }
}

/**
 * @brief  Initialize USB function
 * @param[in] port_id : USB port ID
 * @param[in] usb_mode : USB mode type
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_function_init(uint8_t port_id, TLKHAL_USB_MODE_ENUM usb_mode, void *userArg)
{
    (void)userArg;
    if (port_id >= TLK_CFG_USB_NUMB) {
        return;
    }

    switch (usb_mode) {
    case TLKUSB_MODTYPE_UDB:
    {
        usb1hw_set_eps_max_size(128);
        break;
    }
    case TLKUSB_MODTYPE_UAC_HS:
    {
        usbd_handle_init();

        usbd_desc_register(DESCR_ID_DEVICE, usbd_audio_dev_desc, sizeof(usbd_audio_dev_desc));
        usbd_desc_register(DESCR_ID_MANUFACTURER_STR, usbd_audio_manufacturerstr_desc, sizeof(usbd_audio_manufacturerstr_desc));
        usbd_desc_register(DESCR_ID_PRODUCT_STR, usbd_audio_productstr_desc, sizeof(usbd_audio_productstr_desc));
        usbd_desc_register(DESCR_ID_CONFIG, usbd_audio_cfg_desc, sizeof(usbd_audio_cfg_desc));
        usbd_desc_register(DESCR_ID_DEVQLF, usbd_audio_dev_qualifier_desc, sizeof(usbd_audio_dev_qualifier_desc));
        usbd_desc_register(DESCR_ID_LANGID_STR, usbd_audio_string_lang_id, sizeof(usbd_audio_string_lang_id));

        usbd_user_cb_register(&user_cb);

        usbd_endpoint_register(AUDIO_OUT_EP, usbd_audio_dataout);
        usbd_endpoint_register(AUDIO_IN_EP, usbd_audio_datain);
        usbd_endpoint_register(HID_IN_EP, usbd_audio_hid_in);

        usbd_audio_cb_register(0x0200, audio_entity_table, 3, &usbd_audio_cb);

        usbd_class_intf_register(usbd_audio_get_inf_cb(), 0);
        usbd_class_intf_register(usbd_audio_get_inf_cb(), 1);
        usbd_class_intf_register(usbd_audio_get_inf_cb(), 2);

        usbd_init();
        break;
    }
    default:
        break;
    }
}

/**
 * @brief  Handle USB unplug event
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_unpluged(uint8_t port_id, void *userArg)
{
    (void)port_id;
    (void)userArg;
}


#endif
