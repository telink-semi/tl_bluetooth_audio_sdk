/********************************************************************************************************
 * @file    tlkusb_desc.c
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
#include "tlklib/usb/tlkusb_desc.h"

#if (TLK_CFG_USB_ENABLE)


/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
static const tlkusb_stdStringDesc_t sMmiUsbLanguageDesc = {2 + 2,              //
                                                           TLKUSB_TYPE_STRING, // bDescriptorType
                                                           {TLKUSB_LANG_ID_ENGLISH}};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const tlkusb_stdStringDesc_t sMmiUsbVendorDesc = {2 + sizeof(TLKUSB_STRING_VENDOR) - 2, //-2 is the end of the string
                                                         TLKUSB_TYPE_STRING,                   // Header
                                                         TLKUSB_STRING_VENDOR};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const tlkusb_stdStringDesc_t sMmiUsbProductDesc = {2 + sizeof(TLKUSB_STRING_PRODUCT) - 2, //-2 is the end of the string
                                                          TLKUSB_TYPE_STRING,                    // Header
                                                          TLKUSB_STRING_PRODUCT};

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
static const tlkusb_stdStringDesc_t sMmiUsbSerialDesc = {2 + sizeof(TLKUSB_STRING_SERIAL) - 2, TLKUSB_TYPE_STRING, TLKUSB_STRING_SERIAL};


static const tlkusb_stdDeviceDesc_t sMmiUsbDeviceDesc = {sizeof(tlkusb_stdDeviceDesc_t), //
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
                                                         TLKUSB_ID_PRODUCT,              // ProductID
                                                         TLKUSB_ID_VERSION /*0x0100*/,   // .ReleaseNumber
                                                         TLKUSB_STRING_INDEX_VENDOR,     // .ManufacturerStrIndex
                                                         TLKUSB_STRING_INDEX_PRODUCT,    // .ProductStrIndex
                                                         0,                              // .SerialNumStrIndex, iSerialNumber
                                                         1};

/**
 * @brief       This function gets the language descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the language descriptor.
 */
uint8_t *tlkusb_getLanguageDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getStringDesc(mode, TLKUSB_STRING_INDEX_LANGUAGE);
    if (pDesc == NULL) {
        pDesc = (uint8_t *)(&sMmiUsbLanguageDesc);
    }
    return pDesc;
}

/**
 * @brief       This function gets the vendor descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the vendor descriptor.
 */
uint8_t *tlkusb_getVendorDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getStringDesc(mode, TLKUSB_STRING_INDEX_VENDOR);
    if (pDesc == NULL) {
        pDesc = (uint8_t *)(&sMmiUsbVendorDesc);
    }
    return pDesc;
}

/**
 * @brief       This function gets the product descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the product descriptor.
 */
uint8_t *tlkusb_getProductDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getStringDesc(mode, TLKUSB_STRING_INDEX_PRODUCT);
    if (pDesc == NULL) {
        pDesc = (uint8_t *)(&sMmiUsbProductDesc);
    }
    return pDesc;
}

/**
 * @brief       This function gets the serial descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the serial descriptor.
 */
uint8_t *tlkusb_getSerialDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getStringDesc(mode, TLKUSB_STRING_INDEX_SERIAL);
    if (pDesc == NULL) {
        pDesc = (uint8_t *)(&sMmiUsbSerialDesc);
    }
    return pDesc;
}

/**
 * @brief       This function gets the language descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the language descriptor.
 */
uint16_t tlkusb_getLanguageLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getStringLens(mode, TLKUSB_STRING_INDEX_LANGUAGE);
    if (descLen == 0) {
        descLen = sizeof(TLKUSB_LANG_ID_ENGLISH);
    }
    return descLen;
}

/**
 * @brief       This function gets the product descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the product descriptor.
 */
uint16_t tlkusb_getProductLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getStringLens(mode, TLKUSB_STRING_INDEX_PRODUCT);
    if (descLen == 0) {
        descLen = sizeof(TLKUSB_STRING_PRODUCT);
    }
    return descLen;
}

/**
 * @brief       This function gets the vendor descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the vendor descriptor.
 */
uint16_t tlkusb_getVendorLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getStringLens(mode, TLKUSB_STRING_INDEX_VENDOR);
    if (descLen == 0) {
        descLen = sizeof(TLKUSB_STRING_VENDOR);
    }
    return descLen;
}

/**
 * @brief       This function gets the serial descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the serial descriptor.
 */
uint16_t tlkusb_getSerialLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getStringLens(mode, TLKUSB_STRING_INDEX_SERIAL);
    if (descLen == 0) {
        descLen = sizeof(TLKUSB_STRING_SERIAL);
    }
    return descLen;
}

/**
 * @brief       This function gets the device descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the device descriptor.
 */
uint8_t *tlkusb_getDeviceDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getDeviceDesc(mode);
    if (pDesc == NULL) {
        pDesc = (uint8_t *)(&sMmiUsbDeviceDesc);
    }
    return pDesc;
}

/**
 * @brief       This function gets the configuration descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the configuration descriptor.
 */
uint8_t *tlkusb_getConfigDesc(uint8_t mode)
{
    uint8_t *pDesc;
    pDesc = tlkusb_module_getConfigDesc(mode);
    return pDesc;
}

/**
 * @brief       This function gets the device descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the device descriptor.
 */
uint16_t tlkusb_getDeviceLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getDeviceLens(mode);
    if (descLen == 0) {
        descLen = sizeof(tlkusb_stdDeviceDesc_t);
    }
    return descLen;
}

/**
 * @brief       This function gets the configuration descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the configuration descriptor.
 */
uint16_t tlkusb_getConfigLens(uint8_t mode)
{
    uint16_t descLen;
    descLen = tlkusb_module_getConfigLens(mode);
    return descLen;
}


#endif // #if (TLK_CFG_USB_ENABLE)
