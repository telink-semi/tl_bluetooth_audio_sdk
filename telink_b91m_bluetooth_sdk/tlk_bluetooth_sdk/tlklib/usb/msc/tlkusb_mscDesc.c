/********************************************************************************************************
 * @file    tlkusb_mscDesc.c
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
#if (TLK_USB_MSC_ENABLE)
#include "tlklib/usb/msc/tlkusb_mscDefine.h"
#include "tlklib/usb/msc/tlkusb_msc.h"
#include "tlklib/usb/msc/tlkusb_mscDesc.h"


static uint16_t tlkusb_mscdesc_getDeviceLens(void);
static uint16_t tlkusb_mscdesc_getConfigLens(void);
static uint16_t tlkusb_mscdesc_getStringLens(uint8_t index);
static uint8_t *tlkusb_mscdesc_getDeviceDesc(void);
static uint8_t *tlkusb_mscdesc_getConfigDesc(void);
static uint8_t *tlkusb_mscdesc_getStringDesc(uint8_t index);

const tlkusb_modDesc_t sTlkUsbMscModDesc = {
    tlkusb_mscdesc_getDeviceLens, //GetDeviceLens
    tlkusb_mscdesc_getConfigLens, //GetConfigLens
    tlkusb_mscdesc_getStringLens, //GetStringLens
    NULL,                         //GetReportLens
    tlkusb_mscdesc_getDeviceDesc, //GetDeviceDesc
    tlkusb_mscdesc_getConfigDesc, //GetConfigDesc
    tlkusb_mscdesc_getStringDesc, //GetStringDesc
    NULL,                         //GetReportDesc
};


static const tlkusb_stdStringDesc_t sMmiUsbMscVendorDesc = {2 + sizeof(TLKUSB_MSC_STRING_VENDOR) - 2, //-2 is the end of the string
                                                            TLKUSB_TYPE_STRING,                       // Header
                                                            TLKUSB_MSC_STRING_VENDOR};

static const tlkusb_stdStringDesc_t sMmiUsbMscProductDesc = {2 + sizeof(TLKUSB_MSC_STRING_PRODUCT) - 2, //-2 is the end of the string
                                                             TLKUSB_TYPE_STRING,                        // Header
                                                             TLKUSB_MSC_STRING_PRODUCT};

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
static const tlkusb_stdStringDesc_t sMmiUsbMscSerialDesc = {2 + sizeof(TLKUSB_MSC_STRING_SERIAL) - 2, TLKUSB_TYPE_STRING, TLKUSB_MSC_STRING_SERIAL};

static const tlkusb_stdDeviceDesc_t sMmiUsbMscDeviceDesc = {sizeof(tlkusb_stdDeviceDesc_t), //Length
                                                            TLKUSB_TYPE_DEVICE,             // Header
                                                            0x0110,
                                                            USB_CSCP_NoDeviceClass,       // Class
                                                            USB_CSCP_NoDeviceSubclass,    // SubClass
                                                            USB_CSCP_NoDeviceProtocol,    // Protocol
                                                                                          //	USB_CSCP_IADDeviceClass, // Class
                                                                                          //	USB_CSCP_IADDeviceSubclass, // SubClass
                                                                                          //	USB_CSCP_IADDeviceProtocol, // Protocol
                                                            8,                            // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
                                                            TLKUSB_ID_VENDOR,             // VendorID
                                                            0x9218,                       //TLKUSB_ID_PRODUCT, // ProductID
                                                            TLKUSB_ID_VERSION /*0x0100*/, // .ReleaseNumber
                                                            TLKUSB_STRING_INDEX_VENDOR,   // .ManufacturerStrIndex
                                                            TLKUSB_STRING_INDEX_PRODUCT,  // .ProductStrIndex
                                                            TLKUSB_STRING_INDEX_SERIAL,   // .SerialNumStrIndex, iSerialNumber
                                                            1};
static const tlkusb_mscConfigDesc_t sMmiUsbMscConfigDesc = {
    {
        sizeof(tlkusb_stdConfigureDesc_t), //Length
        TLKUSB_TYPE_CONFIGURE,             // Type
        sizeof(tlkusb_mscConfigDesc_t),    // TotalLength: variable
        TLKUSB_MSC_INF_MAX,                // NumInterfaces
        1,                                 // Configuration index
        TLKUSB_NO_DESCRIPTOR,              // Configuration String
        TLKUSB_CFG_ATTR_RESERVED,          // Attributes
        TLKUSB_CONFIG_POWER(100)           // MaxPower = 100mA
    },
    // IAD for MSC interface
    //	{
    //		sizeof(tlkusb_stdAssociateDesc_t), //bLength
    //		TLKUSB_TYPE_ASSOCIATE, //bDescriptorType
    //		TLKUSB_MSC_INF_MSC, // FirstInterfaceIndex
    //		1, // TotalInterface
    //		MS_CSCP_MassStorageClass, // bInterfaceclass ->Printer
    //		MS_CSCP_SCSITransparentSubclass, // bInterfaceSubClass -> Control
    //		MS_CSCP_BulkOnlyTransportProtocol,// bInterfaceProtocol
    //		TLKUSB_NO_DESCRIPTOR
    //	},
    // MSC Interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), //Length
        TLKUSB_TYPE_INTERFACE,             //Type
        TLKUSB_MSC_INF_MSC,                // bInterfaceNumber
        0,                                 // AlternateSetting
        2,                                 // bNumEndpoints
        MS_CSCP_MassStorageClass,          // bInterfaceclass ->Printer
        MS_CSCP_SCSITransparentSubclass,   // bInterfaceSubClass -> Control
        MS_CSCP_BulkOnlyTransportProtocol, // bInterfaceProtocol
        TLKUSB_NO_DESCRIPTOR               // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
    },
    // MSC IN Endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),      //Length
        TLKUSB_TYPE_ENDPOINT,                  // bDescriptorType
        TLKUSB_EDP_DIR_IN | TLKUSB_MSC_EDP_IN, // endpoint id
        TLKUSB_EDP_TYPE_BULK,                  // endpoint type
        0x0040,                                // wMaxPacketSize
        0                                      // bInterval
    },
    // MSC OUT Endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t), //Length
        TLKUSB_TYPE_ENDPOINT,             // bDescriptorType
        TLKUSB_MSC_EDP_OUT,               // endpoint id
        TLKUSB_EDP_TYPE_BULK,             // endpoint type
        0x0040,                           // wMaxPacketSize
        0                                 // polling bInterval. valid for iso or interrupt type
    },
};

/**
 * @brief       Get the length of device descriptor.
 * @param[in]   none.
 * @return      Length of device descriptor.
 */
static uint16_t tlkusb_mscdesc_getDeviceLens(void)
{
    return sizeof(tlkusb_stdDeviceDesc_t);
}

/**
 * @brief       Get the length of configuration descriptor.
 * @param[in]   none.
 * @return      Length of configuration descriptor.
 */
static uint16_t tlkusb_mscdesc_getConfigLens(void)
{
    return sizeof(tlkusb_mscConfigDesc_t);
}

/**
 * @brief       Get the length of string descriptor.
 * @param[in]   index - String index.
 * @return      Length of string descriptor.
 */
static uint16_t tlkusb_mscdesc_getStringLens(uint8_t index)
{
    if (index == TLKUSB_STRING_INDEX_VENDOR) {
        return sizeof(TLKUSB_MSC_STRING_VENDOR);
    } else if (index == TLKUSB_STRING_INDEX_PRODUCT) {
        return sizeof(TLKUSB_MSC_STRING_PRODUCT);
    } else if (index == TLKUSB_STRING_INDEX_SERIAL) {
        return sizeof(TLKUSB_MSC_STRING_SERIAL);
    } else {
        return 0;
    }
}

/**
 * @brief       Get device descriptor.
 * @param[in]   none.
 * @return      Pointer to device descriptor.
 */
static uint8_t *tlkusb_mscdesc_getDeviceDesc(void)
{
    return (uint8_t *)(&sMmiUsbMscDeviceDesc);
}

/**
 * @brief       Get configuration descriptor.
 * @param[in]   none.
 * @return      Pointer to configuration descriptor.
 */
static uint8_t *tlkusb_mscdesc_getConfigDesc(void)
{
    return (uint8_t *)(&sMmiUsbMscConfigDesc);
}

/**
 * @brief       Get string descriptor.
 * @param[in]   index - String index.
 * @return      Pointer to string descriptor.
 */
static uint8_t *tlkusb_mscdesc_getStringDesc(uint8_t index)
{
    if (index == TLKUSB_STRING_INDEX_VENDOR) {
        return (uint8_t *)(&sMmiUsbMscVendorDesc);
    } else if (index == TLKUSB_STRING_INDEX_PRODUCT) {
        return (uint8_t *)(&sMmiUsbMscProductDesc);
    } else if (index == TLKUSB_STRING_INDEX_SERIAL) {
        return (uint8_t *)(&sMmiUsbMscSerialDesc);
    } else {
        return 0;
    }
}


#endif //#if (TLK_USB_MSC_ENABLE)
