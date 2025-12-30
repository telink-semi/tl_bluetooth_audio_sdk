/********************************************************************************************************
 * @file    bth_hcicod.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef BTH_HCICOD_H
#define BTH_HCICOD_H


// https://www.bluetooth.com/specifications/assigned-numbers/baseband/

// COD=Class Of Device
//
// 0x340404=001101000000010000000100b
// Service class    MajorDeviceClass     MinorDeviceClass    Type
// 00110100000      00100                000001              00

// Type
#define BTH_COD_TYPE_MASK   0x000003
#define BTH_COD_TYPE_BITS   2
#define BTH_COD_TYPE_OFFSET 0


// Major Service Classes
#define BTH_COD_SERVICE_CLASS_MASK        0xFFE000
#define BTH_COD_SERVICE_CLASS_BITS        11
#define BTH_COD_SERVICE_CLASS_OFFSET      13
#define BTH_COD_SERVICE_CLASS_LDM         0x002000 // Limited Discoverable Mode [Ref #1]
#define BTH_COD_SERVICE_CLASS_LE_AUDIO    0x004000 // LE audio
#define BTH_COD_SERVICE_CLASS_RESERVED    0x008000 //(reserved)
#define BTH_COD_SERVICE_CLASS_POSITION    0x010000 // Positioning (Location identification)
#define BTH_COD_SERVICE_CLASS_NETWORK     0x020000 // Networking (LAN, Ad hoc,)
#define BTH_COD_SERVICE_CLASS_RENDER      0x040000 // Rendering (Printing, Speakers,)
#define BTH_COD_SERVICE_CLASS_CAPTURE     0x080000 // Capturing (Scanner, Microphone,)
#define BTH_COD_SERVICE_CLASS_OBJECT      0x100000 // Object Transfer (v-Inbox, v-Folder,)
#define BTH_COD_SERVICE_CLASS_AUDIO       0x200000 // Audio (Speaker, Microphone, Headset service,)
#define BTH_COD_SERVICE_CLASS_TELEPHONY   0x400000 // Telephony (Cordless telephony, Modem, Headset service,)
#define BTH_COD_SERVICE_CLASS_INFORMATION 0x800000 // Information (WEB-server, WAP-server,)

// Major Device Classes
#define BTH_COD_MAJOR_CLASS_MASK          0x001F00
#define BTH_COD_MAJOR_CLASS_BITS          5
#define BTH_COD_MAJOR_CLASS_OFFSET        8
#define BTH_COD_MAJOR_CLASS_MISC          (0 << BTH_COD_MAJOR_CLASS_OFFSET)  // Miscellaneous [Ref #2]
#define BTH_COD_MAJOR_CLASS_COMPUTER      (1 << BTH_COD_MAJOR_CLASS_OFFSET)  // Computer (desktop, notebook, PDA, organizer, )
#define BTH_COD_MAJOR_CLASS_PHONE         (2 << BTH_COD_MAJOR_CLASS_OFFSET)  // Phone (cellular, cordless, pay phone, modem,)
#define BTH_COD_MAJOR_CLASS_NETAP         (3 << BTH_COD_MAJOR_CLASS_OFFSET)  // LAN /Network Access point
#define BTH_COD_MAJOR_CLASS_AUDIO         (4 << BTH_COD_MAJOR_CLASS_OFFSET)  // Audio/Video (headset, speaker, stereo, video display, VCR,
#define BTH_COD_MAJOR_CLASS_PERIPHERAL    (5 << BTH_COD_MAJOR_CLASS_OFFSET)  // Peripheral (mouse, joystick, keyboard,  )
#define BTH_COD_MAJOR_CLASS_IMAGING       (6 << BTH_COD_MAJOR_CLASS_OFFSET)  // Imaging (printer, scanner, camera, display, )
#define BTH_COD_MAJOR_CLASS_WEARABLE      (7 << BTH_COD_MAJOR_CLASS_OFFSET)  // Wearable
#define BTH_COD_MAJOR_CLASS_TOY           (8 << BTH_COD_MAJOR_CLASS_OFFSET)  // Toy
#define BTH_COD_MAJOR_CLASS_HEALTH        (9 << BTH_COD_MAJOR_CLASS_OFFSET)  // Health
#define BTH_COD_MAJOR_CLASS_UNCATEGORIZED (31 << BTH_COD_MAJOR_CLASS_OFFSET) // Uncategorized: device code not specified

// Minor Device Class
#define BTH_COD_MINOR_CLASS_MASK   0x0000FC
#define BTH_COD_MINOR_CLASS_BITS   6
#define BTH_COD_MINOR_CLASS_OFFSET 2
// The Minor Device Class field - Computer Major Class
#define BTH_COD_COMPUTER_UNCATEGORIZED (0 << BTH_COD_MINOR_CLASS_OFFSET) // Uncategorized, code for device not assigned
#define BTH_COD_COMPUTER_DESKTOP       (1 << BTH_COD_MINOR_CLASS_OFFSET) // Desktop workstation
#define BTH_COD_COMPUTER_SERVER_CLASS  (2 << BTH_COD_MINOR_CLASS_OFFSET) // Server-class computer
#define BTH_COD_COMPUTER_LAPTOP        (3 << BTH_COD_MINOR_CLASS_OFFSET) // Laptop
#define BTH_COD_COMPUTER_HANDHELD_PC   (4 << BTH_COD_MINOR_CLASS_OFFSET) // Handheld PC/PDA (clamshell)
#define BTH_COD_COMPUTER_PALM_SIZE_PC  (5 << BTH_COD_MINOR_CLASS_OFFSET) // Palm-size PC/PDA
#define BTH_COD_COMPUTER_WEARABLE      (6 << BTH_COD_MINOR_CLASS_OFFSET) // Wearable computer (watch size)
#define BTH_COD_COMPUTER_TABLET        (7 << BTH_COD_MINOR_CLASS_OFFSET) // Tablet
// Minor Device Class field  Phone Major Class
#define BTH_COD_PHONE_UNCATEGORIZED      (0 << BTH_COD_MINOR_CLASS_OFFSET) // Uncategorized, code for device not assigned
#define BTH_COD_PHONE_SELLULAR           (1 << BTH_COD_MINOR_CLASS_OFFSET) // Cellular
#define BTH_COD_PHONE_CORDLESS           (2 << BTH_COD_MINOR_CLASS_OFFSET) // Cordless
#define BTH_COD_PHONE_SMARTPHONE         (3 << BTH_COD_MINOR_CLASS_OFFSET) // Smartphone
#define BTH_COD_PHONE_WIRED_MODEM        (4 << BTH_COD_MINOR_CLASS_OFFSET) // Wired modem or voice gateway
#define BTH_COD_PHONE_COMMON_ISDN_ACCESS (5 << BTH_COD_MINOR_CLASS_OFFSET) // Common ISDN access
// Minor Device Class field LAN/Network Access Point Major Class
#define BTH_COD_NETAP_FULL_AVAILABLE       (0 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // Fully available
#define BTH_COD_NETAP_01TO17PER_UTILIZED   (1 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 1% to 17% utilized
#define BTH_COD_NETAP_17TO33PER_UTILIZED   (2 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 17% to 33% utilized
#define BTH_COD_NETAP_33TO50PER_UTILIZED   (3 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 33% to 50% utilized
#define BTH_COD_NETAP_50TO67PER_UTILIZED   (4 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 50% to 67% utilized
#define BTH_COD_NETAP_67TO83PER_UTILIZED   (5 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 67% to 83% utilized
#define BTH_COD_NETAP_83TO99PER_UTILIZED   (6 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // 83% to 99% utilized
#define BTH_COD_NETAP_NO_SERVICE_AVAILABLE (7 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // No service available
// Minor Device Class field Audio/Video Major Class
#define BTH_COD_AUDIO_UNCATEGORIZED     (0 << BTH_COD_MINOR_CLASS_OFFSET)  // Uncategorized, code not assigned
#define BTH_COD_AUDIO_WEARABLE_HEADSET  (1 << BTH_COD_MINOR_CLASS_OFFSET)  // Wearable Headset Device
#define BTH_COD_AUDIO_HANDS_FREE_DEVICE (2 << BTH_COD_MINOR_CLASS_OFFSET)  // Hands-free Device
#define BTH_COD_AUDIO_RESERVED          (3 << BTH_COD_MINOR_CLASS_OFFSET)  //(Reserved)
#define BTH_COD_AUDIO_MICROPHONE        (4 << BTH_COD_MINOR_CLASS_OFFSET)  // Microphone
#define BTH_COD_AUDIO_LOUDSPEAKER       (5 << BTH_COD_MINOR_CLASS_OFFSET)  // Loudspeaker
#define BTH_COD_AUDIO_HEADPHONES        (6 << BTH_COD_MINOR_CLASS_OFFSET)  // Headphones
#define BTH_COD_AUDIO_PORTABLE          (7 << BTH_COD_MINOR_CLASS_OFFSET)  // Portable Audio
#define BTH_COD_AUDIO_CAR_AUDIO         (8 << BTH_COD_MINOR_CLASS_OFFSET)  // Car audio
#define BTH_COD_AUDIO_SET_TOP_BOX       (9 << BTH_COD_MINOR_CLASS_OFFSET)  // Set-top box
#define BTH_COD_AUDIO_HIFI_DEVICE       (10 << BTH_COD_MINOR_CLASS_OFFSET) // HiFi Audio Device
#define BTH_COD_AUDIO_VCR               (11 << BTH_COD_MINOR_CLASS_OFFSET) // VCR
#define BTH_COD_AUDIO_CAMERA            (12 << BTH_COD_MINOR_CLASS_OFFSET) // Video Camera
#define BTH_COD_AUDIO_CAMCORDER         (13 << BTH_COD_MINOR_CLASS_OFFSET) // Camcorder
#define BTH_COD_AUDIO_VIDEO             (14 << BTH_COD_MINOR_CLASS_OFFSET) // Video Monitor
#define BTH_COD_AUDIO_MONITOR           (15 << BTH_COD_MINOR_CLASS_OFFSET) // Video Display and Loudspeaker
#define BTH_COD_AUDIO_DISPLAY           (16 << BTH_COD_MINOR_CLASS_OFFSET) // Video Conferencing
#define BTH_COD_AUDIO_CONFERENCING      (17 << BTH_COD_MINOR_CLASS_OFFSET) //(Reserved)
#define BTH_COD_AUDIO_GAMING_OR_TOY     (18 << BTH_COD_MINOR_CLASS_OFFSET) // Gaming/Toy
// Minor Device Class field -- Peripheral Major Class
#define BTH_COD_PERIPHERAL_OTHERS        (0 << (BTH_COD_MINOR_CLASS_OFFSET + 4)) // Not Keyboard / Not Pointing Device
#define BTH_COD_PERIPHERAL_KEYBOARD      (1 << (BTH_COD_MINOR_CLASS_OFFSET + 4)) // Keyboard
#define BTH_COD_PERIPHERAL_POINTING      (2 << (BTH_COD_MINOR_CLASS_OFFSET + 4)) // Pointing device
#define BTH_COD_PERIPHERAL_COMBO         (3 << (BTH_COD_MINOR_CLASS_OFFSET + 4)) // Combo keyboard/pointing device
#define BTH_COD_PERIPHERAL_UNCATEGORIZED (0 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Uncategorized device
#define BTH_COD_PERIPHERAL_JOYSTICK      (1 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Joystick
#define BTH_COD_PERIPHERAL_GAMEPAD       (2 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Gamepad
#define BTH_COD_PERIPHERAL_REMOTE        (3 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Remote control
#define BTH_COD_PERIPHERAL_SENSING       (4 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Sensing device
#define BTH_COD_PERIPHERAL_DIGITIZER     (5 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Digitizer tablet
#define BTH_COD_PERIPHERAL_CARD_READER   (6 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Card Reader (e.g. SIM Card Reader)
#define BTH_COD_PERIPHERAL_DIGITAL_PEN   (7 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Digital Pen
#define BTH_COD_PERIPHERAL_SCANNER       (8 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Handheld scanner for bar-codes, RFID, etc.
#define BTH_COD_PERIPHERAL_GESTURAL      (9 << (BTH_COD_MINOR_CLASS_OFFSET + 0)) // Handheld gestural input device (e.g.,)
// Minor Device Class field -- Imaging Major Class
#define BTH_COD_IMAGING_DISPLAY (1 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // Display
#define BTH_COD_IMAGING_CAMERA  (2 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // Camera
#define BTH_COD_IMAGING_SCANNER (4 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // Scanner
#define BTH_COD_IMAGING_PRINTER (8 << (BTH_COD_MINOR_CLASS_OFFSET + 2)) // Printer
// Minor Device Class field -- Wearable Major Class
#define BTH_COD_WEARABLE_WRISTWATCH (1 << BTH_COD_MINOR_CLASS_OFFSET) // Wristwatch
#define BTH_COD_WEARABLE_PAGER      (2 << BTH_COD_MINOR_CLASS_OFFSET) // Pager
#define BTH_COD_WEARABLE_JACKET     (3 << BTH_COD_MINOR_CLASS_OFFSET) // Jacket
#define BTH_COD_WEARABLE_HELMET     (4 << BTH_COD_MINOR_CLASS_OFFSET) // Helmet
#define BTH_COD_WEARABLE_GLASSES    (5 << BTH_COD_MINOR_CLASS_OFFSET) // Glasses
// Minor Device Class field -- Toy Major Class
#define BTH_COD_TOY_ROBOOT     (1 << BTH_COD_MINOR_CLASS_OFFSET) // Robot
#define BTH_COD_TOY_VEHICLE    (2 << BTH_COD_MINOR_CLASS_OFFSET) // Vehicle
#define BTH_COD_TOY_DOLL       (3 << BTH_COD_MINOR_CLASS_OFFSET) // Doll / Action figure
#define BTH_COD_TOY_CONTROLLER (4 << BTH_COD_MINOR_CLASS_OFFSET) // Controller
#define BTH_COD_TOY_GAME       (5 << BTH_COD_MINOR_CLASS_OFFSET) // Game
// Minor Device Class field -- Health
#define BTH_COD_HEALTH_UNDEFINED          (0 << BTH_COD_MINOR_CLASS_OFFSET)  // Undefined
#define BTH_COD_HEALTH_BLPM               (1 << BTH_COD_MINOR_CLASS_OFFSET)  // Blood Pressure Monitor
#define BTH_COD_HEALTH_THERMOMETER        (2 << BTH_COD_MINOR_CLASS_OFFSET)  // Thermometer
#define BTH_COD_HEALTH_WEIGHING_SCALE     (3 << BTH_COD_MINOR_CLASS_OFFSET)  // Weighing Scale
#define BTH_COD_HEALTH_GLUCOSE_METER      (4 << BTH_COD_MINOR_CLASS_OFFSET)  // Glucose Meter
#define BTH_COD_HEALTH_PULSE_OXIMETER     (5 << BTH_COD_MINOR_CLASS_OFFSET)  // Pulse Oximeter
#define BTH_COD_HEALTH_HEART_RATE_MONITOR (6 << BTH_COD_MINOR_CLASS_OFFSET)  // Heart/Pulse Rate Monitor
#define BTH_COD_HEALTH_DATA_DISPLAY       (7 << BTH_COD_MINOR_CLASS_OFFSET)  // Health Data Display
#define BTH_COD_HEALTH_STEP_COUNTER       (8 << BTH_COD_MINOR_CLASS_OFFSET)  // Step Counter
#define BTH_COD_HEALTH_BODY_ANALYZER      (9 << BTH_COD_MINOR_CLASS_OFFSET)  // Body Composition Analyzer
#define BTH_COD_HEALTH_PEAK_FLOW_MONITOR  (10 << BTH_COD_MINOR_CLASS_OFFSET) // Peak Flow Monitor
#define BTH_COD_HEALTH_MEDICATION_MONITOR (11 << BTH_COD_MINOR_CLASS_OFFSET) // Medication Monitor
#define BTH_COD_HEALTH_KNEE_PROSTHESIS    (12 << BTH_COD_MINOR_CLASS_OFFSET) // Knee Prosthesis
#define BTH_COD_HEALTH_ANKLE_PROSTHESIS   (13 << BTH_COD_MINOR_CLASS_OFFSET) // Ankle Prosthesis
#define BTH_COD_HEALTH_GENERIC_MANAGER    (14 << BTH_COD_MINOR_CLASS_OFFSET) // Generic Health Manager
#define BTH_COD_HEALTH_PERSIONAL_DEVICE   (15 << BTH_COD_MINOR_CLASS_OFFSET) // Personal Mobility Device


#define bth_getMajorClass(cod)            ((cod) & BTH_COD_MAJOR_CLASS_MASK)
#define bth_getMinorClass(cod)            ((cod) & BTH_COD_MINOR_CLASS_MASK)


#endif
