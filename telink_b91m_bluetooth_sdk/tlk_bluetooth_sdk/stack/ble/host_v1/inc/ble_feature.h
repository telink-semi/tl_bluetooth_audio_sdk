/********************************************************************************************************
 * @file    ble_feature.h
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

// This file contains the BLE feature definitions. FeatureSet field`s bit mapping to Controller features.


// LE Encryption
#define BLE_HOST_FEATURE_LE_ENCRYPTION 0
// Connection Parameters Request Procedure
#define BLE_HOST_FEATURE_CONN_PARAMS 1
// Extended Reject Indication
#define BLE_HOST_FEATURE_EXT_REJ_IND 2
// Peripheral Initiated Features Exchange
#define BLE_HOST_FEATURE_PERIPHERAL_INIT_FEAT_EXCHANGE 3
// LE Ping
#define BLE_HOST_FEATURE_LE_PING 4
// LE Data Packet Length Extension
#define BLE_HOST_FEATURE_LE_DATA_LEN_EXT 5
// LL Privacy
#define BLE_HOST_FEATURE_LL_PRIVACY 6
// Extended Scanner Filter Policies
#define BLE_HOST_FEATURE_EXT_SCAN_FILT_POL 7
// LE 2M PHY
#define BLE_HOST_FEATURE_LE_2M_PHY 8
// Stable Modulation Index - Transmitter
#define BLE_HOST_FEATURE_STABLE_MOD_IDX_TX 9
// Stable Modulation Index - Receiver
#define BLE_HOST_FEATURE_STABLE_MOD_IDX_RX 10
// LE Coded PHY
#define BLE_HOST_FEATURE_LE_CODED_PHY 11
// LE Extended Advertising
#define BLE_HOST_FEATURE_LE_EXT_ADV 12
// LE Periodic Advertising
#define BLE_HOST_FEATURE_LE_PERIODIC_ADV 13
// Channel Selection Algorithm #2
#define BLE_HOST_FEATURE_CHAN_SEL_2 14
// LE Power Class 1
#define BLE_HOST_FEATURE_LE_POWER_CLASS_1 15
// Minimum Number of Used Channels Procedure
#define BLE_HOST_FEATURE_MIN_USED_CHAN_PROC 16
// Connection CTE Request
#define BLE_HOST_FEATURE_CONN_CTE_REQ 17
// Connection CTE Response
#define BLE_HOST_FEATURE_CONN_CTE_RESP 18
// Connectionless CTE Transmitter
#define BLE_HOST_FEATURE_CONNLESS_CTE_TX 19
// Connectionless CTE Receiver
#define BLE_HOST_FEATURE_CONNLESS_CTE_RX 20
// Antenna Switching During CTE Transmission(AoD)
#define BLE_HOST_FEATURE_AOD 21
// Antenna Switching During CTE Reception(AoA)
#define BLE_HOST_FEATURE_AOA 22
// Receiving Constant Tone Extension
#define BLE_HOST_FEATURE_RC_CTE 23
// Periodic Advertising Sync TransferSender
#define BLE_HOST_FEATURE_PA_SYNC_TX 24
// Periodic Advertising Sync TransferRecipient
#define BLE_HOST_FEATURE_PA_SYNC_RX 25
// Sleep Clock Accuracy Updates
#define BLE_HOST_FEATURE_SCA_UPD 26
// Remote Public Key Validation
#define BLE_HOST_FEATURE_RPK_VAL 27
// Connection Isochronous Stream - Central
#define BLE_HOST_FEATURE_CIS_CENTRAL 28
// Connection Isochronous Stream - Peripheral
#define BLE_HOST_FEATURE_CIS_PERIPHERAL 29
// Isochronous Broadcaster
#define BLE_HOST_FEATURE_ISO_BROADCASTER 30
// Synchronized Receiver
#define BLE_HOST_FEATURE_SYNC_RECEIVER 31
// Connected Isochronous Stream(Host Support)
#define BLE_HOST_FEATURE_CIS_HOST_SUPPORT 32
// if support LE power control request BIT 33 and BTI 34 always set to 1.
// LE Power Control Request
#define BLE_HOST_FEATURE_LE_POWER_CTRL_REQ 33
// LE Power Control Request
#define BLE_HOST_FEATURE_LE_POWER_CTRL_REQ_2 34
// LE Path Loss Monitoring
#define BLE_HOST_FEATURE_LE_PATH_LOSS_MONITOR 35
// Periodic Advertising ADI support
#define BLE_HOST_FEATURE_PA_ADI_SUPPORT 36
// Connection Subrating
#define BLE_HOST_FEATURE_CONN_SUBRATING 37
// Connection Subrating(Host Support)
#define BLE_HOST_FEATURE_CONN_SUBRATING_HOST_SUPPORT 38
// Channel Classification
#define BLE_HOST_FEATURE_CHAN_CLASSIFICATION 39
// Advertising Coding Selection
#define BLE_HOST_FEATURE_ADV_CODING_SEL 40
// Advertising Coding Selection(Host Support)
#define BLE_HOST_FEATURE_ADV_CODING_SEL_HOST_SUPPORT 41
// Periodic Advertising with Responses Advertiser
#define BLE_HOST_FEATURE_PAwR_ADVERTISER 43
// Periodic Advertising with Responses Scanner
#define BLE_HOST_FEATURE_PAwR_SCANNER 44
// Unsegmented Framed Mode
#define BLE_HOST_FEATURE_UNSEGMENTED_FRM_MODE 45
// Channel Sounding
#define BLE_HOST_FEATURE_CHAN_SOUNDING 46
// Channel Sounding (Host Support
#define BLE_HOST_FEATURE_CHAN_SOUNDING_HOST_SUPPORT 47
// Channel Sounding Tone Quality Indication
#define BLE_HOST_FEATURE_CHAN_SOUNDING_TQI 48
// LL Extended Feature Set
#define BLE_HOST_FEATURE_LL_EXT_FEATURE_SET 63
// Monitoring Advertisers
#define BLE_HOST_EXT_FEATURE_MONITORING_ADVERTISERS 0
// Frame Space Update
#define BLE_HOST_EXT_FEATURE_FRAME_SPACE_UPDATE 1
// UTP OTA mode
#define BLE_HOST_EXT_FEATURE_UTP_OTA_MODE 2
// UTP HCI mode
#define BLE_HOST_EXT_FEATURE_UTP_HCI_MODE 3
// Shorter Connection Intervals
#define BLE_HOST_EXT_FEATURE_SHORT_CONN_INT 8
// Shorter Connection Intervals (Host Support)
#define BLE_HOST_EXT_FEATURE_SHORT_CONN_INT_HOST_SUPPORT 9
// LE Flushable ACL Data
#define BLE_HOST_EXT_FEATURE_LE_FLUSHABLE_ACL_DATA 10
