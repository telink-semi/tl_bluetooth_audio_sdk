/********************************************************************************************************
 * @file    tlkusb_struct.h
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
#ifndef TLKUSB_STRUCT_H
#define TLKUSB_STRUCT_H

typedef enum
{
    USB_DIR_OUT = 0,
    USB_DIR_IN  = 1,

    USB_DIR_IN_MASK = 0x80
} usb_dir_e;

typedef enum
{
    TUSB_REQ_TYPE_STANDARD = 0,
    TUSB_REQ_TYPE_CLASS,
    TUSB_REQ_TYPE_VENDOR,
    TUSB_REQ_TYPE_INVALID
} tlkusb_request_type_t;

typedef enum
{
    TUSB_REQ_RCPT_DEVICE = 0,
    TUSB_REQ_RCPT_INTERFACE,
    TUSB_REQ_RCPT_ENDPOINT,
    TUSB_REQ_RCPT_OTHER
} tlkusb_request_recipient_t;

typedef struct __attribute__((packed))
{
    union
    {
        struct __attribute__((packed))
        {
            unsigned char recipient : 5;
            unsigned char type      : 2;
            unsigned char direction : 1;
        } bmRequestType_bit;

        unsigned char bmRequestType;
    };

    unsigned char  bRequest;
    unsigned short wValue;
    unsigned short wIndex;
    unsigned short wLength;
} usb_control_request_t;

typedef struct
{
    uint8_t  bReqType; /**< Type of the request. */
    uint8_t  bRequest; /**< Request command code. */
    uint16_t wValue;   /**< wValue parameter of the request. */
    uint16_t wIndex;   /**< wIndex parameter of the request. */
    uint16_t wLength;  /**< Length of the data to transfer in bytes. */
} tlkusb_setup_req_t;

/** \brief Standard USB String Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard string descriptor. Unlike other standard descriptors, the length
 *  of the descriptor for placement in the descriptor header must be determined by the \ref USB_STRING_LEN()
 *  macro rather than by the size of the descriptor structure, as the length is not fixed.
 *
 *  This structure should also be used for string index 0, which contains the supported language IDs for
 *  the device as an array.
 *
 *  This structure uses the relevant standard's given element names to ensure compatibility with the standard.
 *
 *  \see \ref USB_Descriptor_String_t for the version of this type with with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t  bLength;         /**< Size of the descriptor, in bytes. */
    uint8_t  bDescriptorType; /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class. */
    uint16_t bString[];       /**< String data, as unicode characters (alternatively, string language IDs).
                             *   If normal ASCII characters are to be used, they must be added as an array
                             *   of characters rather than a normal C string so that they are widened to
                             *   Unicode size.
                             *
                             *   Under GCC, strings prefixed with the "L" character (before the opening string
                             *   quotation mark) are considered to be Unicode strings, and may be used instead
                             *   of an explicit array of ASCII characters.
                             */
} __attribute__((packed)) tlkusb_stdStringDesc_t;

/** \brief Standard USB Device Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Device Descriptor. This structure uses the relevant standard's given element names
 *  to ensure compatibility with the standard.
 *
 *  \see \ref USB_Descriptor_Device_t for the version of this type with non-standard LUFA specific element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;             /**< BCD of the supported USB specification. */
    uint8_t  bDeviceClass;       /**< USB device class. */
    uint8_t  bDeviceSubClass;    /**< USB device subclass. */
    uint8_t  bDeviceProtocol;    /**< USB device protocol. */
    uint8_t  bMaxPacketSize0;    /**< Size of the control (address 0) endpoint's bank in bytes. */
    uint16_t idVendor;           /**< Vendor ID for the USB product. */
    uint16_t idProduct;          /**< Unique product ID for the USB product. */
    uint16_t bcdDevice;          /**< Product release (version) number. */
    uint8_t  iManufacturer;      /**< String index for the manufacturer's name.*/
    uint8_t  iProduct;           /**< String index for the product name/details.*/
    uint8_t  iSerialNumber;      /**< String index for the product's globally unique hexadecimal serial number, in uppercase
                               Unicode ASCII.*/
    uint8_t  bNumConfigurations; /**< Total number of configurations supported by the device. */
} __attribute__((packed)) tlkusb_stdDeviceDesc_t;

/** \brief Standard USB Device Qualifier Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Device Qualifier Descriptor. This structure uses the relevant standard's given element
 * names to ensure compatibility with the standard.
 *
 *  \see \ref USB_Descriptor_DeviceQualifier_t for the version of this type with non-standard LUFA specific element
 * names.
 */
typedef struct
{
    uint8_t  bLength;            /**< Size of the descriptor, in bytes. */
    uint8_t  bDescriptorType;    /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class.*/
    uint16_t bcdUSB;             /**< BCD of the supported USB specification. */
    uint8_t  bDeviceClass;       /**< USB device class. */
    uint8_t  bDeviceSubClass;    /**< USB device subclass. */
    uint8_t  bDeviceProtocol;    /**< USB device protocol. */
    uint8_t  bMaxPacketSize0;    /**< Size of the control (address 0) endpoint's bank in bytes. */
    uint8_t  bNumConfigurations; /**< Total number of configurations supported by the device.*/
    uint8_t  bReserved;          /**< Reserved for future use, must be 0. */
} __attribute__((packed)) tlkusb_stdQualifierDesc_t;

/** \brief Standard USB Configuration Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Configuration Descriptor header. This structure uses the relevant standard's given
 * element names to ensure compatibility with the standard.
 *
 *  \see \ref USB_Descriptor_Device_t for the version of this type with non-standard LUFA specific element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t  bLength;             /**< Size of the descriptor, in bytes. */
    uint8_t  bDescriptorType;     /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class.*/
    uint16_t wTotalLength;        /**< Size of the configuration descriptor header, and all sub descriptors inside the
                               configuration.*/
    uint8_t  bNumInterfaces;      /**< Total number of interfaces in the configuration. */
    uint8_t  bConfigurationValue; /**< Configuration index of the current configuration. */
    uint8_t  iConfiguration;      /**< Index of a string descriptor describing the configuration. */
    uint8_t  bmAttributes;        /**< Configuration attributes, comprised of a mask of \c USB_CONFIG_ATTR_* masks.*/
    uint8_t  bMaxPower;           /**< Maximum power consumption of the device while in the current configuration, calculated by the
                         \ref USB_CONFIG_POWER_MA() macro.*/
} __attribute__((packed)) tlkusb_stdConfigureDesc_t;

/** \brief Standard USB Interface Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Interface Descriptor. This structure uses the relevant standard's given element names
 *  to ensure compatibility with the standard.
 *
 *  \see \ref tlkusb_stdInterfaceDesc_t for the version of this type with non-standard LUFA specific element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t bLength;            /**< Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;    /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class.*/
    uint8_t bInterfaceNumber;   /**< Index of the interface in the current configuration. */
    uint8_t bAlternateSetting;  /**< Alternate setting for the interface number.*/
    uint8_t bNumEndpoints;      /**< Total number of endpoints in the interface. */
    uint8_t bInterfaceClass;    /**< Interface class ID. */
    uint8_t bInterfaceSubClass; /**< Interface subclass ID. */
    uint8_t bInterfaceProtocol; /**< Interface protocol ID. */
    uint8_t iInterface;         /**< Index of the string descriptor describing the interface.*/
} __attribute__((packed)) tlkusb_stdInterfaceDesc_t;

/** \brief Standard USB Endpoint Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Endpoint Descriptor. This structure uses the relevant standard's given
 *  element names to ensure compatibility with the standard.
 *
 *  \see \ref tlkusb_stdEndpointDesc_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t  bLength;          /**< Size of the descriptor, in bytes. */
    uint8_t  bDescriptorType;  /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class.*/
    uint8_t  bEndpointAddress; /**< Logical address of the endpoint within the device for the current configuration,
                                including direction mask.*/
    uint8_t  bmAttributes;     /**< Endpoint attributes, comprised of a mask of the endpoint type (EP_TYPE_*) and attributes
                              (ENDPOINT_ATTR_*) masks. */
    uint16_t wMaxPacketSize;   /**< Size of the endpoint bank, in bytes. This indicates the maximum packet size that the
                              endpoint can receive at a time.*/
    uint8_t  bInterval;        /**< Polling interval in milliseconds for the endpoint if it is an INTERRUPT or ISOCHRONOUS type.*/
} __attribute__((packed)) tlkusb_stdEndpointDesc_t;

/** \brief Standard USB Interface Association Descriptor (USB-IF naming conventions).
 *
 *  Type define for a standard Interface Association Descriptor. This structure uses the relevant standard's given
 *  element names to ensure compatibility with the standard.
 *
 *  This descriptor has been added as a supplement to the USB2.0 standard, in the ECN located at
 *  <a>http://www.usb.org/developers/docs/InterfaceAssociationDescriptor_ecn.pdf</a>. It allows composite
 *  devices with multiple interfaces related to the same function to have the multiple interfaces bound
 *  together at the point of enumeration, loading one generic driver for all the interfaces in the single
 *  function. Read the ECN for more information.
 *
 *  \see \ref USB_Descriptor_Interface_Association_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
    uint8_t bLength;           /**< Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;   /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class. */
    uint8_t bFirstInterface;   /**< Index of the first associated interface. */
    uint8_t bInterfaceCount;   /**< Total number of associated interfaces. */
    uint8_t bFunctionClass;    /**< Interface class ID. */
    uint8_t bFunctionSubClass; /**< Interface subclass ID. */
    uint8_t bFunctionProtocol; /**< Interface protocol ID. */
    uint8_t iFunction;         /**< Index of the string descriptor describing the interface association. */
} __attribute__((packed)) tlkusb_stdAssociateDesc_t;

typedef struct
{
    uint8_t  Size;                   /**< Size of the descriptor, in bytes. */
    uint8_t  Type;                   /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the specific
                    class. */
    uint16_t HIDSpec;                /*BCD of the supported HID specification.*/
    uint8_t  CountryCode;            /*The country code of the country where the hardware device is located*/
    uint8_t  TotalReportDescriptors; /*The number of the accessory descriptor*/
    uint8_t  HIDReportType;          /*The type of the accessory descriptor*/
    uint16_t HIDReportLength;        /*The length of the accessory descriptor*/
} __attribute__((packed)) tlkusb_HidEndpointDesc_t;

typedef struct
{
    uint8_t bLength;            /* Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;    /**< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class. */
    uint8_t Subtype;            /*Subtype of the descriptors.*/
    uint8_t ACSpecification[2]; /*Revision of class specification*/
    uint8_t TotalLength[2];     /*Total size of class specific descriptors.*/
    uint8_t InCollection;       /*Number of streaming interfaces.*/
    uint8_t InterfaceNumber;    /*AudioStreaming interface belongs to AudioControl interface.Multiple connections follow in
                               turn*/
} __attribute__((packed)) tlkusb_uacInterfaceAcDesc_t;

typedef struct
{
    uint8_t bLength;             /* Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;     /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class. */
    uint8_t Subtype;             /*Subtype of the descriptor.*/
    uint8_t ACSpecification[2];  /*Revision of class specification - 1.0*/
    uint8_t TotalLength[2];      /*Total size of class specific descriptors.*/
    uint8_t InCollection;        /*Number of streaming interfaces.*/
    uint8_t InterfaceNumber_spk; /*AudioStreaming interface belongs to AudioControl interface.Multiple connections follow
                                   in turn*/
    uint8_t InterfaceNumber_mic; /*AudioStreaming interface belongs to AudioControl interface.Multiple connections follow
                                   in turn*/
} __attribute__((packed)) tlkusb_uacInterfaceAcTLDesc_t;

typedef struct
{
    uint8_t Length;            /* Size of the descriptor, in bytes. */
    uint8_t DescriptorType;    /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t DescriptorSubtype; /*Subtype of the descriptor.*/
    uint8_t TerminalLink;      /*The Terminal ID of the Terminal to which the endpoint of this interface is connected.*/
    uint8_t Delay;             /*Delay introduced by the data path. Expressed in number of frames.*/
    uint8_t AudioFormat[2];    /*The Audio Data Format that has to be used to communicate with this interface.*/
} __attribute__((packed)) tlkusb_uacInterfaceAsDesc_t;

typedef struct
{
    uint8_t  Length;            /* Size of the descriptor, in bytes. */
    uint8_t  DescriptorType;    /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t  DescriptorSubtype; /*Subtype of the descriptor.*/
    uint8_t  TerminalID;        /*Terminal ID,either a value in \ref TLKUSB_AUDID_ENUM,	Constant uniquely identifying
                                 theTerminal within the audio function. */
    uint16_t TerminalType;      /*Constant characterizing the type of Terminal. */
    uint8_t  AssocTerminal;     /*ID of the Output Terminal to which this Input Terminal is associated.*/
    uint8_t  TotalChannels;     /*Number of logical output channels in the Terminals output audio channel cluster.*/
    uint16_t ChannelConfig;     /*Describes the spatial location of the logical channels.*/
    uint8_t  ChannelStrIndex;   /*Index of a string descriptor, describing the name of the first logical channel.*/
    uint8_t  TerminalStrIndex;  /*Index of a string descriptor, describing the Input Terminal.*/
} __attribute__((packed)) tlkusb_uacInputDesc_t;

typedef struct
{
    uint8_t  Length;            /* Size of the descriptor, in bytes. */
    uint8_t  DescriptorType;    /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t  DescriptorSubtype; /*Subtype of the descriptor.*/
    uint8_t  TerminalID;        /*Terminal ID,either a value in \ref TLKUSB_AUDID_ENUM,	Constant uniquely identifying
                                 theTerminal within the audio function. */
    uint16_t TerminalType;      /*Constant characterizing the type of Terminal. */
    uint8_t  AssocTerminal;     /*ID of the Iutput Terminal to which this Output Terminal is associated.*/
    uint8_t  SourceID;          /*ID of the Unit or Terminal to which this Terminal is connected.*/
    uint8_t  Terminal;          /*Index of a string descriptor, describing the Output Terminal.*/
} __attribute__((packed)) tlkusb_uacOutputDesc_t;

typedef struct
{
    uint8_t Length;              /* Size of the descriptor, in bytes. */
    uint8_t DescriptorType;      /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t DescriptorSubtype;   /*Subtype of the descriptor.*/
    uint8_t UnitID;              /*Constant uniquely identifying the Unit within the audio function. This value is used in all
                             requests to address this Unit.*/
    uint8_t SourceID;            /*ID of the Unit or Terminal to which this Feature Unit is connected.*/
    uint8_t ControlSize;         /*Size in bytes of an element of the MAControls */
    uint8_t MAControls[2];       /*An array of bit-maps, each indicating the availability of certain audio Controls for a
                             specific logical channel or for the master channel 0*/
    uint8_t FeatureUnitStrIndex; /*Index of a string descriptor, describing this Feature Unit.*/
} __attribute__((packed)) tlkusb_uacSingleFeatureDesc_t;

typedef struct
{
    uint8_t Length;              /* Size of the descriptor, in bytes. */
    uint8_t DescriptorType;      /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t DescriptorSubtype;   /*Subtype of the descriptor.*/
    uint8_t UnitID;              /*Constant uniquely identifying the Unit within the audio function. This value is used in all
                             requests to address this Unit.*/
    uint8_t SourceID;            /*ID of the Unit or Terminal to which this Feature Unit is connected.*/
    uint8_t ControlSize;         /*Size in bytes of an element of the MAControls */
    uint8_t MAControls[3];       /*An array of bit-maps, each indicating the availability of certain audio Controls for a
                             specific logical channel or for the master channel 0*/
    uint8_t FeatureUnitStrIndex; /*Index of a string descriptor, describing this Feature Unit.*/
} __attribute__((packed)) tlkusb_uacDoubleFeatureDesc_t;

typedef struct
{
    uint8_t Length;              /* Size of the descriptor, in bytes. */
    uint8_t DescriptorType;      /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t DescriptorSubtype;   /*Subtype of the descriptor.*/
    uint8_t FormatType;          /*Audio format type.*/
    uint8_t NrChannels;          /*Number of physical channels supported by the interface*/
    uint8_t SubFrameSize;        /*Bytes of data per channel.*/
    uint8_t BitResolution;       /*The number of significant bits in SubFrameSize.*/
    uint8_t SampleFrequencyType; /*Sample type. 0:Continuous sampling rate, other:The number of supported discrete
                                   sampling frequencies*/
    uint8_t tSamFreq[3];         /*Sample rate*/
} __attribute__((packed)) tlkusb_uacFormatDesc_t;

typedef struct
{
    uint8_t  Length;          /* Size of the descriptor, in bytes. */
    uint8_t  DescriptorType;  /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                               specific class. */
    uint8_t  EndpointAddress; /*The address of the endpoint. bit[7]: Direction,bit[3-0]: The endpoint number,bit[6-4]:
                               Reserved.*/
    uint8_t  MAttributes;     /*Synchronization type.bit[1-0]:Transfer type. bit[3-2]:Synchronisation Type,bit[5-4]:Usage type.*/
    uint16_t MaxPacketSize;   /*The maximum packet length of the endpoint's data transfer*/
    uint8_t  Interval;        /*Interval for polling endpoint for data transfers expressed in ms.Must be set to 1.*/
    uint8_t  Refresh;         /*Reset to 0.*/
    uint8_t  SynchAddress;    /*The address of the endpoint used to communicate synchronization information if required by
                             this endpoint. Reset to zero if no synchronization pipe is used*/
} __attribute__((packed)) tlkusb_uacStdEndpointDesc_t;

typedef struct
{
    uint8_t Length;            /* Size of the descriptor, in bytes. */
    uint8_t DescriptorType;    /*< Type of the descriptor, either a value in \ref TLKUSB_TYPE_ENUM or a value given by the
                              specific class. */
    uint8_t DescriptorSubtype; /*Subtype of the descriptor.*/
    uint8_t MAttributes;       /*A bit in the range bit[6-0] set to 1 indicates that the mentioned Control is supported by this
                           endpoint.bit[0]: Sampling Frequency bit[1]: Pitch bit[6-2]: Reserved bit[7] MaxPacketsOnly.*/
    uint8_t LockDelayUnits;    /*Indicates the units used for the LockDelay field:0: Undefined  1: Milliseconds 2: Decoded
                              PCM samples 3..255: Reserved*/
    uint8_t LockDelay[2];      /*Indicates the time it takes this endpoint to reliably lock its internal clock recovery
                              circuitry. Units used depend on the value of the bLockDelayUnits field.*/
} __attribute__((packed)) tlkusb_uacSpcEndpointDesc_t;


#endif // TLKUSB_STRUCT_H
