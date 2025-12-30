/********************************************************************************************************
 * @file    tlkusb_udbDesc.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_USB_UDB_ENABLE)
#include "tlklib/usb/udb/tlkusb_udbDefine.h"
#include "tlklib/usb/udb/tlkusb_udbDesc.h"
#include "tlklib/usb/udb/tlkusb_udb.h"


static uint16_t tlkusb_udbdesc_getConfigLens(void);
static uint8_t *tlkusb_udbdesc_getConfigDesc(void);

const tlkusb_modDesc_t sTlkUsbUdbModDesc = {
    NULL,                         // GetDeviceLens
    tlkusb_udbdesc_getConfigLens, // GetConfigLens
    NULL,                         // GetStringLens
    NULL,                         // GetReportLens
    NULL,                         // GetDeviceDesc
    tlkusb_udbdesc_getConfigDesc, // GetConfigDesc
    NULL,                         // GetStringDesc
    NULL,                         // GetReportDesc
};


static const tlkusb_udbConfigDesc_t sMmiUsbUdbConfigDesc = {
    {
        sizeof(tlkusb_stdConfigureDesc_t),
        TLKUSB_TYPE_CONFIGURE,          // Length, type
        sizeof(tlkusb_udbConfigDesc_t), // TotalLength: variable
        TLKUSB_UDB_INF_MAX,             // NumInterfaces
        1,                              // Configuration index
        TLKUSB_NO_DESCRIPTOR,           // Configuration String
        TLKUSB_CFG_ATTR_RESERVED,       // Attributes
        TLKUSB_CONFIG_POWER(100)        // MaxPower = 100mA
    },
    // IAD for UDB interface
    //	{
    //		sizeof(tlkusb_stdAssociateDesc_t), //bLength
    //		TLKUSB_TYPE_ASSOCIATE, //bDescriptorType
    //		TLKUSB_INF_UDB, // FirstInterfaceIndex
    //		1, // TotalInterface
    //		PRNT_CSCP_PrinterClass, // bInterfaceclass ->Printer
    //		PRNT_CSCP_PrinterSubclass, // bInterfaceSubClass -> Control
    //		PRNT_CSCP_BidirectionalProtocol,// bInterfaceProtocol
    //		TLKUSB_NO_DESCRIPTOR
    //	},
    // printer_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), TLKUSB_TYPE_INTERFACE,
        TLKUSB_UDB_INF_DBG,                     // bInterfaceNumber
        0,                                      // AlternateSetting
        2,                                      // bNumEndpoints
        TLKUSB_PRNT_CSCP_PrinterClass,          // bInterfaceclass ->Printer
        TLKUSB_PRNT_CSCP_PrinterSubclass,       // bInterfaceSubClass -> Control
        TLKUSB_PRNT_CSCP_BidirectionalProtocol, // bInterfaceProtocol
        TLKUSB_NO_DESCRIPTOR                    // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
    },
    // printer_in_endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),
        TLKUSB_TYPE_ENDPOINT,                      // length, bDescriptorType
        TLKUSB_EDP_DIR_IN | TLKUSB_UDB_EDP_DBG_IN, // endpoint id
        TLKUSB_EDP_TYPE_BULK,                      // endpoint type
        0x0040,                                    // wMaxPacketSize
        0                                          // bInterval
    },
    // printer_out_endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),
        TLKUSB_TYPE_ENDPOINT,   // length, bDescriptorType
        TLKUSB_UDB_EDP_DBG_OUT, // endpoint id
        TLKUSB_EDP_TYPE_BULK,   // endpoint type
        0x0040,                 // wMaxPacketSize
        0                       // polling bInterval. valid for iso or interrupt type
    },
#if (TLKUSB_UDB_VCD_ENABLE)
    // IAD for VCD interface
    //	{
    //		sizeof(tlkusb_stdAssociateDesc_t), //bLength
    //		TLKUSB_TYPE_ASSOCIATE, //bDescriptorType
    //		TLKUSB_INF_VCD, // FirstInterfaceIndex
    //		1, // TotalInterface
    //		PRNT_CSCP_PrinterClass, // bInterfaceclass ->Printer
    //		PRNT_CSCP_PrinterSubclass, // bInterfaceSubClass -> Control
    //		PRNT_CSCP_BidirectionalProtocol,// bInterfaceProtocol
    //		TLKUSB_NO_DESCRIPTOR
    //	},
    // printer_interface
    {
        sizeof(tlkusb_stdInterfaceDesc_t), TLKUSB_TYPE_INTERFACE,
        TLKUSB_UDB_INF_VCD,                     // bInterfaceNumber
        0,                                      // AlternateSetting
        2,                                      // bNumEndpoints
        TLKUSB_PRNT_CSCP_PrinterClass,          // bInterfaceclass ->Printer
        TLKUSB_PRNT_CSCP_PrinterSubclass,       // bInterfaceSubClass -> Control
        TLKUSB_PRNT_CSCP_BidirectionalProtocol, // bInterfaceProtocol
        TLKUSB_NO_DESCRIPTOR                    // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
    },
    // printer_in_endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),
        TLKUSB_TYPE_ENDPOINT,                      // length, bDescriptorType
        TLKUSB_EDP_DIR_IN | TLKUSB_UDB_EDP_VCD_IN, // endpoint id
        TLKUSB_EDP_TYPE_BULK,                      // endpoint type
        0x0040,                                    // wMaxPacketSize
        0                                          // bInterval
    },
    // printer_out_endpoint
    {
        sizeof(tlkusb_stdEndpointDesc_t),
        TLKUSB_TYPE_ENDPOINT,   // length, bDescriptorType
        TLKUSB_UDB_EDP_VCD_OUT, // endpoint id
        TLKUSB_EDP_TYPE_BULK,   // endpoint type
        0x0040,                 // wMaxPacketSize
        0                       // polling bInterval. valid for iso or interrupt type
    },
#endif
};


/**
 * @brief       Get the length of UDB configuration descriptor
 * @return      uint16_t - Size of the configuration descriptor structure in bytes
 */
static uint16_t tlkusb_udbdesc_getConfigLens(void)
{
    return sizeof(tlkusb_udbConfigDesc_t);
}

/**
 * @brief       Get the pointer to UDB configuration descriptor
 * @return      uint8_t* - Pointer to the configuration descriptor structure
 */
static uint8_t *tlkusb_udbdesc_getConfigDesc(void)
{
    return (uint8_t *)(&sMmiUsbUdbConfigDesc);
}


#endif // #if (TLK_USB_UDB_ENABLE)
