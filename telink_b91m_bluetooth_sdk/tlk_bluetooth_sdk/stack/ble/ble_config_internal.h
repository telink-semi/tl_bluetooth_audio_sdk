/********************************************************************************************************
 * @file    ble_config_internal.h
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
//Remove file when merge to SDK //
#pragma once

//////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Definition for Device info
 */
#include "drivers.h"
#include "tl_common.h"

#include "stack/ble/controller/ll/ll_bqb_stack.h"
#include "stack/hal/hal_ble_internal.h"

///////////////////  Feature ////////////////////////////
#ifndef MULTIPLE_LOCAL_DEVICE_ENABLE
    #define MULTIPLE_LOCAL_DEVICE_ENABLE 0
#endif

#ifndef LL_EXT_ADV_DURATION_OPTIMIZE_EN
    #define LL_EXT_ADV_DURATION_OPTIMIZE_EN 0
#endif

#ifndef CUSTOM_DARWIN_FMN_ENABLE
    #define CUSTOM_DARWIN_FMN_ENABLE 0 //Enable by default, controlled by API. FMN use this
#endif

#ifndef ULL_FOR_CIS_EN
    #define ULL_FOR_CIS_EN 0 //Enable by app layer,ULL use this
#endif

#ifndef ULL_FOR_CIS_PATCH1
    #define ULL_FOR_CIS_PATCH1 0 //Enable by app layer,ULL use this
#endif

#ifndef LL_ASYNC_LEA_EN
    #define LL_ASYNC_LEA_EN 0
#endif

#ifndef ADV_DURATION_STALL_EN
    #define ADV_DURATION_STALL_EN 0
#endif

#ifndef LL_HCI_CMD_PROC_OPTIMIZE_EN
    #define LL_HCI_CMD_PROC_OPTIMIZE_EN 1
#endif

#ifndef LL_CIS_SUPPORT_ASYMMETRIC_PHY_EN
    #define LL_CIS_SUPPORT_ASYMMETRIC_PHY_EN 1 //CIS supports asymmetric PHY.
#endif
//////////////////////////////////////////////////////////////////////

#ifndef STACK_SUPPORT_FLASH_PROTECTION_ENABLE
    #define STACK_SUPPORT_FLASH_PROTECTION_ENABLE 0
#endif


/*
 * for single priority interrupt IC, such as B85m, not support break nesting, generally use GD flash,  add Zbit flash vendor
 * defect:
 * when temperature rise, writing 16B flash(particular for OTA) time may bigger than 200uS, or even greater than 300uS, this time
 * of IRQ disabling will cost BLE stack data errors(e.g. RX data loss cause decryption MIC fail, then BLE disconnect)
 *
 */
#ifndef ZBIT_FLASH_ON_SINGLE_PRIORITY_IRQ_IC_WORKAROUND_EN
    #define ZBIT_FLASH_ON_SINGLE_PRIORITY_IRQ_IC_WORKAROUND_EN 0
#endif

/*
 * for single priority interrupt IC, such as B85m, not support break nesting, generally use GD flash,  add second flash vendor which
 * provide Sonos architecture flash(such as PUYA)
 * feature:
 * write 1B, 2B, ... 255B timing cost equal to write a page(256B),
 * about 1~2 mS. In flash_write_page function, disable IRQ may lead to RX packet loss for BTX/BRX
 * so we should take some action to workaround this problem
 */
#ifndef SONOS_ARCH_FLASH_ON_SINGLE_PRIORITY_IRQ_IC_WORKAROUND_EN
    #define SONOS_ARCH_FLASH_ON_SINGLE_PRIORITY_IRQ_IC_WORKAROUND_EN 0
#endif


//Link layer feature enable flag default setting
#ifndef LL_FEATURE_SUPPORT_LE_DATA_LENGTH_EXTENSION
    #define LL_FEATURE_SUPPORT_LE_DATA_LENGTH_EXTENSION 1
#endif

#ifndef LL_FEATURE_SUPPORT_PRIVACY
    #define LL_FEATURE_SUPPORT_PRIVACY 1 //must be enable, because we need process peer device RPA
#endif

#ifndef LL_FEATURE_SUPPORT_EXTENDED_SCANNER_FILTER_POLICIES
    #define LL_FEATURE_SUPPORT_EXTENDED_SCANNER_FILTER_POLICIES 0
#endif

#ifndef LL_FEATURE_SUPPORT_LE_2M_PHY
    #define LL_FEATURE_SUPPORT_LE_2M_PHY 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_CODED_PHY
    #define LL_FEATURE_SUPPORT_LE_CODED_PHY 0
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PAST_SENDER
    #define LL_FEATURE_SUPPORT_LE_PAST_SENDER 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PAST_RECIPIENT
    #define LL_FEATURE_SUPPORT_LE_PAST_RECIPIENT 1
#endif

#ifndef LL_FEATURE_SUPPORT_SLEEP_CLK_ACCURACY_UPDATE
    #define LL_FEATURE_SUPPORT_SLEEP_CLK_ACCURACY_UPDATE 0
#endif

#ifndef LL_FEATURE_SUPPORT_REMOTE_PUBLIC_KEY_VALIDATION
    #define LL_FEATURE_SUPPORT_REMOTE_PUBLIC_KEY_VALIDATION 0
#endif

#ifndef LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING
    #define LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_EXTENDED_SCANNING
    #define LL_FEATURE_SUPPORT_LE_EXTENDED_SCANNING 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_EXTENDED_INITIATE
    #define LL_FEATURE_SUPPORT_LE_EXTENDED_INITIATE 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING
    #define LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING 1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING_SYNC
    #define LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING_SYNC 1
#endif

#ifndef LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2
    #define LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2 1
#endif

#ifndef LL_FEATURE_SUPPORT_MIN_USED_OF_USED_CHANNELS
    #define LL_FEATURE_SUPPORT_MIN_USED_OF_USED_CHANNELS 0
#endif

#ifndef LL_FEATURE_SUPPORT_PHY_TEST_MODE
#define LL_FEATURE_SUPPORT_PHY_TEST_MODE                            1
#endif

//core_5.1 feature begin
#ifndef LL_FEATURE_SUPPORT_LE_AOA_AOD
    #define LL_FEATURE_SUPPORT_LE_AOA_AOD 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTION_CTE_REQUEST
    #define LL_FEATURE_SUPPORT_CONNECTION_CTE_REQUEST 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTION_CTE_RESPONSE
    #define LL_FEATURE_SUPPORT_CONNECTION_CTE_RESPONSE 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTIONLESS_CTE_TRANSMITTER
    #define LL_FEATURE_SUPPORT_CONNECTIONLESS_CTE_TRANSMITTER 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTIONLESS_CTE_RECEIVER
    #define LL_FEATURE_SUPPORT_CONNECTIONLESS_CTE_RECEIVER 0
#endif

#ifndef LL_FEATURE_SUPPORT_ANTENNA_SWITCHING_CTE_TRANSMISSION_AOD
    #define LL_FEATURE_SUPPORT_ANTENNA_SWITCHING_CTE_TRANSMISSION_AOD 0
#endif

#ifndef LL_FEATURE_SUPPORT_ANTENNA_SWITCHING_CTE_RECEPTION_AOA
    #define LL_FEATURE_SUPPORT_ANTENNA_SWITCHING_CTE_RECEPTION_AOA 0
#endif

#ifndef LL_FEATURE_SUPPORT_RECEIVING_CONSTANT_TONE_EXTENSIONS
    #define LL_FEATURE_SUPPORT_RECEIVING_CONSTANT_TONE_EXTENSIONS 0
#endif
//core_5.1 feature end


//core_5.2 feature begin
#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER
    #define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER 1
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE
    #define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE 1
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER
    #define LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER 1 //broadcast sender
#endif

#ifndef LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER
    #define LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER 1 //broadcast receiver
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS
    #define LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS 0
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_TEST_MODE
    #define LL_FEATURE_SUPPORT_ISOCHRONOUS_TEST_MODE 1
#endif

#ifndef LL_FEATURE_SUPPORT_POWER_CONTROL_REQUEST
    #define LL_FEATURE_SUPPORT_POWER_CONTROL_REQUEST 1
#endif

#ifndef LL_FEATURE_SUPPORT_POWER_LOSS_MONITORING
    #define LL_FEATURE_SUPPORT_POWER_LOSS_MONITORING 1
#endif
//core_5.2 feature end

#ifdef TL753X_ADAPT
#undef LL_CIS_SUPPORT_ASYMMETRIC_PHY_EN
#undef LL_FEATURE_SUPPORT_POWER_CONTROL_REQUEST
#undef LL_FEATURE_SUPPORT_POWER_LOSS_MONITORING
#endif
//core_5.3 feature begin
#ifndef LL_FEATURE_SUPPORT_PERIODIC_ADV_ADI_SUPPORT
    #define LL_FEATURE_SUPPORT_PERIODIC_ADV_ADI_SUPPORT 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTION_SUBRATING
    #define LL_FEATURE_SUPPORT_CONNECTION_SUBRATING 0
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTION_SUBRATING_HOST
    #define LL_FEATURE_SUPPORT_CONNECTION_SUBRATING_HOST 0
#endif

#ifndef LL_FEATURE_SUPPORT_CHANNEL_CLASSIFICATION
    #define LL_FEATURE_SUPPORT_CHANNEL_CLASSIFICATION 0
#endif


//core_5.4 begin
#ifndef LL_FEATURE_SUPPORT_ADVERTISING_CODING_SELECTION
    #define LL_FEATURE_SUPPORT_ADVERTISING_CODING_SELECTION 0
#endif

#ifndef LL_FEATURE_SUPPORT_ADVERTISING_CODING_SELECT_HOST_SUPPORT
    #define LL_FEATURE_SUPPORT_ADVERTISING_CODING_SELECT_HOST_SUPPORT 0
#endif

#ifndef LL_FEATURE_SUPPORT_PERIODIC_ADVERTISING_WITH_RESPONSES_ADVERTISER
    #define LL_FEATURE_SUPPORT_PERIODIC_ADVERTISING_WITH_RESPONSES_ADVERTISER 0
#endif

#ifndef LL_FEATURE_SUPPORT_PERIODIC_ADVERTISING_WITH_RESPONSES_SCANNER
    #define LL_FEATURE_SUPPORT_PERIODIC_ADVERTISING_WITH_RESPONSES_SCANNER 0
#endif
//core_5.4 end

//core_5.4+  begin


#ifndef LL_FEATURE_SUPPORT_CHANNEL_SOUNDING_REFLECTOR
    #define LL_FEATURE_SUPPORT_CHANNEL_SOUNDING_REFLECTOR (0 && HARDWARE_CHANNEL_SOUNDING_SUPPORT_EN)
#endif

#ifndef LL_FEATURE_SUPPORT_CHANNEL_SOUNDING_INITIATOR
    #define LL_FEATURE_SUPPORT_CHANNEL_SOUNDING_INITIATOR (0 && HARDWARE_CHANNEL_SOUNDING_SUPPORT_EN)
#endif

//core_5.4+  end


#if LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER
    //BIS sync, no matter either by extended scanning or by PAST procedure, both require periodic sync.
    #if (!LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING_SYNC)
        #error "must support periodic sync"
    #endif
#endif

//core_5.3 feature end


/* 2 CIS slave allocate on same one ACL slave, BQB 5.2 need  */
#ifndef ONE_ACL_SLAVE_MATCH_2_CIS_SLAVE_ENABLE
    #define ONE_ACL_SLAVE_MATCH_2_CIS_SLAVE_ENABLE 0
#endif

#ifndef CIS_WINDOW_WIDENING_FOR_BIG_PPM
    #define CIS_WINDOW_WIDENING_FOR_BIG_PPM 0
#endif


#ifndef HCI_SEND_NUM_OF_CMP_AFT_ACK
    #define HCI_SEND_NUM_OF_CMP_AFT_ACK 0
#endif

#ifndef HCI_TX_FIFO_OPTIMIZE_EN
    #define HCI_TX_FIFO_OPTIMIZE_EN 0
#endif

#ifndef HCI_CONTROLLER_TO_HOST_FLOW_CTRL_EN
    #define HCI_CONTROLLER_TO_HOST_FLOW_CTRL_EN 1
#endif

#ifndef L2CAP_DATA_2_HCI_DATA_BUFFER_ENABLE
    #define L2CAP_DATA_2_HCI_DATA_BUFFER_ENABLE 0 //SiHui: just for debug
#endif

#ifndef L2CAP_CREDIT_BASED_FLOW_CONTROL_MODE_EN
    #define L2CAP_CREDIT_BASED_FLOW_CONTROL_MODE_EN 1
#endif

#ifndef L2CAP_SERVER_FEATURE_SUPPORTED_EATT
    #define L2CAP_SERVER_FEATURE_SUPPORTED_EATT 1
#endif

//gen p256, dhkey supported by controller
#ifndef CONTROLLER_GEN_P256KEY_ENABLE
    #define CONTROLLER_GEN_P256KEY_ENABLE 0
#endif

#ifndef SMP_SEC_LEVEL_CHECK_EN
    #define SMP_SEC_LEVEL_CHECK_EN 0
#endif


#ifndef LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN
    #define LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN 0
#endif


#if (LL_FEATURE_SUPPORT_PRIVACY)
    /* privacy relative */
    #ifndef LL_FEATURE_SUPPORT_LOCAL_RPA
        #define LL_FEATURE_SUPPORT_LOCAL_RPA 1
    #endif

    //if network privacy mode ignore IDA
    #ifndef NETWORK_PRIVACY_IGNORE_IDA_CHECK
        #define NETWORK_PRIVACY_IGNORE_IDA_CHECK 0 //BQB need enable, can be disable in SDK
    #endif


    #ifndef LL_FEATURE_SUPPORT_RPA_ADV_DATA_RELATED_ADDRESS_CHANGE
        #define LL_FEATURE_SUPPORT_RPA_ADV_DATA_RELATED_ADDRESS_CHANGE 0 //BQB need enable, can be disable in SDK
    #endif


    #ifndef EXTADV_DATA_CHANGE_MANUAL_DATA_BUFFER
        #define EXTADV_DATA_CHANGE_MANUAL_DATA_BUFFER 0 //BQB need enable, can be disable in SDK
    #endif


    #ifndef SMP_LOCAL_IRK_MATCH_CONTROLLER_NEW_PRIVACY
        #define SMP_LOCAL_IRK_MATCH_CONTROLLER_NEW_PRIVACY 1
    #endif
#endif

// Stack OTA module
#ifndef BLE_STACK_FEATURE_OTA_SUPPORT
    #define BLE_STACK_FEATURE_OTA_SUPPORT 0
#endif

/* 1: multiple connection SDK; 0 : single connection SDK */
#define BLE_MULTIPLE_CONNECTION_ENABLE 1

// fanqh start
#ifndef FIX_CIS_EVT_OVERFLOW
    #define FIX_CIS_EVT_OVERFLOW 0
#endif


#ifndef CIS_ADD_CIE
    #define CIS_ADD_CIE 1
#endif

#ifndef CIS_CIE_CENTRAL_OPTIMIZE
    #define CIS_CIE_CENTRAL_OPTIMIZE 1
#endif

#ifndef BIS_BRD_SET_PARAM
    #define BIS_BRD_SET_PARAM 0
#endif


#ifndef CIS_TSK_JUMP_ADD_PDU
    #define CIS_TSK_JUMP_ADD_PDU 1
#endif


#ifndef CUSTOM_CONNECTION_ESTABLISH_EVT_ENABLE
    #define CUSTOM_CONNECTION_ESTABLISH_EVT_ENABLE 0
#endif

#ifndef FANQH_OPTIMIZE_BIS_API
    #define FANQH_OPTIMIZE_BIS_API 1
#endif

#ifndef SMP_REAL_ENCRYPTION_BUSY_ENABLE
    #define SMP_REAL_ENCRYPTION_BUSY_ENABLE 1 //Enable by default, controlled by API. FMN and Google Fast Pair need this.
#endif

#ifndef NOW_NOT_IMPLEMENT_FUNCTION
    #define NOW_NOT_IMPLEMENT_FUNCTION 0
#endif

#ifndef CS_IOP_EN
    #define CS_IOP_EN 0
#endif


#ifndef CS_IOP_USE_ERROR_OPCODE
    #define CS_IOP_USE_ERROR_OPCODE 0
#endif


#ifndef BIS_CENTRAL_ACL_CENTRAL_TIMING_STAGGERED
    #define BIS_CENTRAL_ACL_CENTRAL_TIMING_STAGGERED 0
#endif

/*
 * Bypassing the pair process
 * 1. master : The ll enc req process is directly initiated to trigger encryption  LTK all zero 16byte
 * 2.
 */
#ifndef BROADCOM_WORKAROUND
    #define BROADCOM_WORKAROUND 0
#endif

#ifndef SAMSUNG_WORKAROUND
    #define SAMSUNG_WORKAROUND 0
#endif

//////////// OS Support //////////////////////////////
#ifndef OS_SUP_EN
    #define OS_SUP_EN 0
#endif

#ifndef OS_SUP_LONG_SLEEP
    #define OS_SUP_LONG_SLEEP 0
#endif
/* do not expose OS compile optimize to users in general SDK */
#ifndef OS_COMPILE_OPTIMIZE_EN
    #define OS_COMPILE_OPTIMIZE_EN 0
#endif


#ifndef FIX_HW_CRC24_EN
    #define FIX_HW_CRC24_EN 0
#endif


//////////// RSSI sniffer feature begin //////////////////////////////
/*
 * Sniffer sub-node monitor peer-master use Sync status, basic feature
 */
#ifndef LL_RSSI_SNIFFER_SLAVE_ENABLE
    #define LL_RSSI_SNIFFER_SLAVE_ENABLE 0
#endif

/*
 * Sniffer sub-node add monitor peer-slave while monitor peer-master
 */
#ifndef LL_RSSI_SNIFFER_MASTER_ENABLE
    #define LL_RSSI_SNIFFER_MASTER_ENABLE 0
#endif

/*
 * Sniffer feature common used
 */
#ifndef LL_RSSI_SNIFFER_MODE_ENABLE
    #define LL_RSSI_SNIFFER_MODE_ENABLE (LL_RSSI_SNIFFER_SLAVE_ENABLE || LL_RSSI_SNIFFER_MASTER_ENABLE)
#endif

/*
 * Sniffer main-node add get reboot reason
 */
#ifndef DRV_RSSI_SNIFFER_MODE_ENABLE
    #define DRV_RSSI_SNIFFER_MODE_ENABLE 0
#endif
//////////// RSSI sniffer feature end //////////////////////////////


/******************************* conn_config start ******************************************************************/

#define CONN_MAX_NUM_C0_P1 1
#define CONN_MAX_NUM_C0_P2 2
#define CONN_MAX_NUM_C0_P4 3
#define CONN_MAX_NUM_C1_P0 4
#define CONN_MAX_NUM_C1_P1 5
#define CONN_MAX_NUM_C1_P2 6
#define CONN_MAX_NUM_C1_P4 7
#define CONN_MAX_NUM_C2_P0 8
#define CONN_MAX_NUM_C2_P2 9
#define CONN_MAX_NUM_C2_P4 10
#define CONN_MAX_NUM_C4_P0 11
#define CONN_MAX_NUM_C4_P2 12
#define CONN_MAX_NUM_C4_P4 13


#ifndef CONN_MAX_NUM_CONFIG
    #define CONN_MAX_NUM_CONFIG CONN_MAX_NUM_C4_P4
#endif

#if (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P1)
    #define LL_MAX_ACL_CEN_NUM 0
    #define LL_MAX_ACL_PER_NUM 1

    #define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P2)
    #define LL_MAX_ACL_CEN_NUM 0
    #define LL_MAX_ACL_PER_NUM 2

    #define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P4)
    #define LL_MAX_ACL_CEN_NUM 0
    #define LL_MAX_ACL_PER_NUM 4

    #define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P0)
    #define LL_MAX_ACL_CEN_NUM 1
    #define LL_MAX_ACL_PER_NUM 0

    #define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P1)
    #define LL_MAX_ACL_CEN_NUM 1
    #define LL_MAX_ACL_PER_NUM 1
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P2)
    #define LL_MAX_ACL_CEN_NUM 1
    #define LL_MAX_ACL_PER_NUM 2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P4)
    #define LL_MAX_ACL_CEN_NUM 1
    #define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P0)
    #define LL_MAX_ACL_CEN_NUM 2
    #define LL_MAX_ACL_PER_NUM 0

    #define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P2)
    #define LL_MAX_ACL_CEN_NUM 2
    #define LL_MAX_ACL_PER_NUM 2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P4)
    #define LL_MAX_ACL_CEN_NUM 2
    #define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P0)
    #define LL_MAX_ACL_CEN_NUM 4
    #define LL_MAX_ACL_PER_NUM 0

    #define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P2)
    #define LL_MAX_ACL_CEN_NUM 2
    #define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P4)
    #define LL_MAX_ACL_CEN_NUM 4
    #define LL_MAX_ACL_PER_NUM 4
#else
    #error "unsupported CONN_MAX_NUM_CONFIG"
#endif


#define LL_MAX_ACL_CONN_NUM (LL_MAX_ACL_CEN_NUM + LL_MAX_ACL_PER_NUM)


#define ACL_CONN_IDX_CEN0   0
#define ACL_CONN_IDX_PER0   LL_MAX_ACL_CEN_NUM


#ifndef LL_ACL_CEN_EN
    #define LL_ACL_CEN_EN 1
#endif

#ifndef LL_ACL_PER_EN
    #define LL_ACL_PER_EN 1
#endif
