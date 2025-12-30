/********************************************************************************************************
 * @file    tlkusb_define.h
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
#ifndef TLKUSB_DEFINE_H
#define TLKUSB_DEFINE_H
#include "tl_common.h"
#include "drivers.h"

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define TLKUSB_ID_VENDOR 0x248a // for report

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define TLKUSB_ID_PRODUCT 0x9218
// #define TLKUSB_ID_PRODUCT           0x881D
#define TLKUSB_ID_VERSION      0x0101
#define TLKUSB_STRING_VENDOR   L"Telink"
#define TLKUSB_STRING_PRODUCT  L"Telink TL721x BTBLE"
#define TLKUSB_STRING_SERIAL   L"TL721X"
#define TLKUSB_LANG_ID_ENGLISH 0x0409
#else
#define TLKUSB_ID_PRODUCT      0x9218
#define TLKUSB_ID_PRODUCT1     0x8006
#define TLKUSB_ID_VERSION      0x0101
#define TLKUSB_ID_VERSION1     0x0100
#define TLKUSB_STRING_VENDOR   L"Telink"
#define TLKUSB_STRING_PRODUCT  L"Telink B91 BTBLE"
#define TLKUSB_STRING_SERIAL   L"TLSR9218"
#define TLKUSB_LANG_ID_ENGLISH 0x0409
#endif


typedef enum
{
    TLKUSB_STRING_INDEX_LANGUAGE = 0,
    TLKUSB_STRING_INDEX_VENDOR,
    TLKUSB_STRING_INDEX_PRODUCT,
    TLKUSB_STRING_INDEX_SERIAL,
} TLKUSB_STRING_INDEX_ENUM;

// Endpoint IN/OUT
#define TLKUSB_EDP_DIR_OUT 0
#define TLKUSB_EDP_DIR_IN  0x80

// Transfer type
#define TLKUSB_EDP_TYPE_CONTROL     0
#define TLKUSB_EDP_TYPE_ISOCHRONOUS 1
#define TLKUSB_EDP_TYPE_BULK        2
#define TLKUSB_EDP_TYPE_INTERRUPT   3
/**If Isochronous endpoint,
Bits 3..2 = Synchronisation Type (Iso Mode)
00 = No Synchronisation
01 = Asynchronous
10 = Adaptive
11 = Synchronous
Bits 5..4 = Usage Type (Iso Mode)
00 = Data Endpoint
01 = Feedback Endpoint
10 = Explicit Feedback Data Endpoint
11 = Reserved
*/
#define TLKUSB_EDP_SYNC_TYPE_NO_SYNC        0
#define TLKUSB_EDP_SYNC_TYPE_ASYN           1
#define TLKUSB_EDP_SYNC_TYPE_ADAPTIVE       2
#define TLKUSB_EDP_SYNC_TYPE_SYNC           3

#define TLKUSB_EDP_USAGE_TYPE_DATA          0
#define TLKUSB_EDP_USAGE_TYPE_FEEDBACK      1
#define TLKUSB_EDP_USAGE_TYPE_FEEDBACK_DATA 2
#define TLKUSB_EDP_USAGE_TYPE_RSV           3


////////////////////////////////////////////////////////
#define TLKUSB_REQTYPE_DIR_MASK 0x80 // Director
#define TLKUSB_REQTYPE_MAJ_MASK 0x60 // Major Type
#define TLKUSB_REQTYPE_REC_MASK 0x1F // RECIPIENT
// TLKUSB_REQTYPE_DIR_MASK
#define TLKUSB_REQTYPE_DIR_HOST2DEV (0 << 7)
#define TLKUSB_REQTYPE_DIR_DEV2HOST (1 << 7)
// TLKUSB_REQTYPE_MAJ_MASK
#define TLKUSB_REQTYPE_MAJ_STAND  (0 << 5)
#define TLKUSB_REQTYPE_MAJ_CLASS  (1 << 5)
#define TLKUSB_REQTYPE_MAJ_VENDOR (2 << 5)
// TLKUSB_REQTYPE_REC_MASK
#define TLKUSB_REQTYPE_REC_DEVICE    (0 << 0)
#define TLKUSB_REQTYPE_REC_INTERFACE (1 << 0)
#define TLKUSB_REQTYPE_REC_ENDPOINT  (2 << 0)
#define TLKUSB_REQTYPE_REC_OTHER     (3 << 0)

/* Public Interface - May be used in end-application: */
/* Macros: */
/** Indicates that a given descriptor does not exist in the device. This can be used inside descriptors
 *  for string descriptor indexes, or may be use as a return value for GetDescriptor when the specified
 *  descriptor does not exist.
 */
#define TLKUSB_NO_DESCRIPTOR 0

/** Macro to calculate the power value for the configuration descriptor, from a given number of milliamperes.
 *
 *  \param[in] mA  Maximum number of milliamps the device consumes when the given configuration is selected.
 */
#define TLKUSB_CONFIG_POWER(mA) ((mA) >> 1)

/* Enums: */
/** Enumeration for the various standard request commands. These commands are applicable when the
 *  request type is \ref TLKUSB_REQTYPE_MAJ_STAND (with the exception of \ref TLKUSB_SETUP_REQ_GET_DESCRIPT, which is
 * always handled regardless of the request type value).
 *
 *  \see Chapter 9 of the USB 2.0 Specification.
 */
enum TLKUSB_SETUP_REQ_ENUM
{
    TLKUSB_SETUP_REQ_GET_STATUS    = 0,
    TLKUSB_SETUP_REQ_CLEAR_FEATURE = 1,
    TLKUSB_SETUP_REQ_SET_FEATURE   = 3,
    TLKUSB_SETUP_REQ_SET_ADDRESS   = 5,
    TLKUSB_SETUP_REQ_GET_DESCRIPT  = 6,
    TLKUSB_SETUP_REQ_SET_DESCRIPT  = 7,
    TLKUSB_SETUP_REQ_GET_CONFIGURE = 8,
    TLKUSB_SETUP_REQ_SET_CONFIGURE = 9,
    TLKUSB_SETUP_REQ_GET_INTERFACE = 10,
    TLKUSB_SETUP_REQ_SET_INTERFACE = 11,
    TLKUSB_SETUP_REQ_SYNC_FRAME    = 12,
};

// typedef enum{
//	PRNT_CSCP_PrinterClass = 0x07,
//	PRNT_CSCP_PrinterSubclass = 0x01,
//	PRNT_CSCP_BidirectionalProtocol = 0x02,
// }TLKUSB_PRINT_CSCP_ENUM;


////////////////////////////////////////////////////////

enum TLKUSB_CFG_ATTR_ENUM
{
    TLKUSB_CFG_ATTR_RESERVED     = 0x80,
    TLKUSB_CFG_ATTR_SELFPOWERED  = 0x40,
    TLKUSB_CFG_ATTR_REMOTEWAKEUP = 0x20,
};

enum TLKUSB_EDP_ATTR_ENUM
{
    TLKUSB_EDP_ATTR_NO_SYNC  = (0 << 2),
    TLKUSB_EDP_ATTR_ASYNC    = (1 << 2),
    TLKUSB_EDP_ATTR_ADAPTIVE = (2 << 2),
    TLKUSB_EDP_ATTR_SYNC     = (3 << 2),
};

enum TLKUSB_EDP_USAGE_ENUM
{
    TLKUSB_EDP_USAGE_DATA              = (0 << 4),
    TLKUSB_EDP_USAGE_FEEDBACK          = (1 << 4),
    TLKUSB_EDP_USAGE_IMPLICIT_FEEDBACK = (2 << 4),
};

/* Enums: */
/** Enum for the possible standard descriptor types, as given in each descriptor's header. */
enum TLKUSB_TYPE_ENUM
{
    TLKUSB_TYPE_DEVICE          = 0x01, /**< Indicates that the descriptor is a device descriptor. */
    TLKUSB_TYPE_CONFIGURE       = 0x02, /**< Indicates that the descriptor is a configuration descriptor. */
    TLKUSB_TYPE_STRING          = 0x03, /**< Indicates that the descriptor is a string descriptor. */
    TLKUSB_TYPE_INTERFACE       = 0x04, /**< Indicates that the descriptor is an interface descriptor. */
    TLKUSB_TYPE_ENDPOINT        = 0x05, /**< Indicates that the descriptor is an endpoint descriptor. */
    TLKUSB_TYPE_QUALIFIER       = 0x06, /**< Indicates that the descriptor is a device qualifier descriptor. */
    TLKUSB_TYPE_OTHER           = 0x07, /**< Indicates that the descriptor is of other type. */
    TLKUSB_TYPE_INTERFACE_POWER = 0x08, /**< Indicates that the descriptor is an interface power descriptor. */
    TLKUSB_TYPE_ASSOCIATE       = 0x0B, /**< Indicates that the descriptor is an interface association descriptor. */
    TLKUSB_TYPE_CS_INTERFACE    = 0x24, /**< Indicates that the descriptor is a class specific interface descriptor. */
    TLKUSB_TYPE_CS_ENDPOINT     = 0x25, /**< Indicates that the descriptor is a class specific endpoint descriptor. */
};

/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors. */
enum TLKUSB_CSCP_ENUM
{
    USB_CSCP_NoDeviceClass          = 0x00, /**< Descriptor Class value indicating that the device does not belong to a
                                         particular class at the device level. */
    USB_CSCP_NoDeviceSubclass       = 0x00, /**< Descriptor Subclass value indicating that the device does not belong to a
                                         particular subclass at the device level. */
    USB_CSCP_NoDeviceProtocol       = 0x00, /**< Descriptor Protocol value indicating that the device does not belong to a
                                         particular protocol at the device level. */
    USB_CSCP_VendorSpecificClass    = 0xFF, /**< Descriptor Class value indicating that the device/interface belongs to a vendor specific class. */
    USB_CSCP_VendorSpecificSubclass = 0xFF, /**< Descriptor Subclass value indicating that the device/interface belongs
                                               to a vendor specific subclass. */
    USB_CSCP_VendorSpecificProtocol = 0xFF, /**< Descriptor Protocol value indicating that the device/interface belongs
                                               to a vendor specific protocol. */
    USB_CSCP_IADDeviceClass         = 0xEF, /**< Descriptor Class value indicating that the device belongs to the Interface
                                          Association Descriptor class. */
    USB_CSCP_IADDeviceSubclass      = 0x02, /**< Descriptor Subclass value indicating that the device belongs to the
                                          Interface Association Descriptor subclass. */
    USB_CSCP_IADDeviceProtocol      = 0x01, /**< Descriptor Protocol value indicating that the device belongs to the
                                          Interface Association Descriptor protocol. */
};


#endif // TLKUSB_DE
