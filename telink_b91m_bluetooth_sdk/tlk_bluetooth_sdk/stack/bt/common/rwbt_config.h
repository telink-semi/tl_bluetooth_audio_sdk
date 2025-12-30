/********************************************************************************************************
 * @file    rwbt_config.h
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


#ifndef RWBT_CONFIG_H_
#define RWBT_CONFIG_H_
#include "tl_common.h"
/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 *
 * @name RWBT Stack Configuration
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/******************************************************************************************/
/* --------------------------   GENERAL SETUP       --------------------------------------*/
/******************************************************************************************/

/// Maximum number of ACL links
#ifndef MAX_NB_ACTIVE_ACL
#if(CHIP_TYPE==CHIP_TYPE_TL752X)
    #define MAX_NB_ACTIVE_ACL 1
#else
    #define MAX_NB_ACTIVE_ACL 2
#endif
#endif
/// BT Heap size
#define BT_HEAP_MSG_SIZE (1024 + 1024 * MAX_NB_ACTIVE_ACL)
#define BT_HEAP_ENV_SIZE (1024 + 1024 * MAX_NB_ACTIVE_ACL)

/// Number of BT tasks
#define BT_TASK_SIZE 4

/// Number of RX descriptors
#define BT_RXDESC_NB (4)

/// Number of RX/TX ACL data buffers
#define ACL_DATA_BUF_NB_RX (BT_RXDESC_NB + 6)
#define ACL_DATA_BUF_NB_TX (MAX_NB_ACTIVE_ACL + 10)

/// Number of TX LMP buffers
#if (MAX_NB_ACTIVE_ACL > 3)
    #define BT_LMP_BUF_NB_TX (2 * MAX_NB_ACTIVE_ACL)
#else  // (MAX_NB_ACTIVE_ACL > 3)
    #define BT_LMP_BUF_NB_TX (20)
#endif // (MAX_NB_ACTIVE_ACL > 3)
#if EBQ_BT_CTRLER_ONLY_TEST_MODE
     /// Maximum ACL Data Packet Size
#define ACL_DATA_BUF_SIZE 15//Run #1763 - /LMP/ENC/BI-02-C	[Encryption, Central, Reject Role Switch]

#else
     /// Maximum ACL Data Packet Size
#define ACL_DATA_BUF_SIZE DH5_3_PACKET_SIZE

#endif

/// Normal Rx window size
#define NORMAL_WIN_SIZE 70

#if(CHIP_TYPE==CHIP_TYPE_TL752X)
/// Advance margin for slave acl.
#define LD_SLAVE_ET_ADVANCE_MARGIN_US  30//((NORMAL_WIN_SIZE - 10)/2)
#else
#define LD_SLAVE_ET_ADVANCE_MARGIN_US  20//((NORMAL_WIN_SIZE - 10)/2)
#endif
/// Range of key size supported by the FW
#define ENC_KEY_SIZE_MAX  16
#define ENC_KEY_SIZE_MIN  1
#define ENC_KEY_SIZE_MASK 0xFFFF

/// Inquiry response TX power (the value code depends on radio used)
//#define INQ_RSP_TX_PWR  rwip_rf.txpwr_max
#define INQ_RSP_TX_PWR 0x28

/// Size of the inquiry result filtering list
#define INQ_FILT_LEN 10
//power need changed
#if(CHIP_TYPE==CHIP_TYPE_TL752X)
#define ACCESS_TX_POWER 0x07
#else
#define ACCESS_TX_POWER 0x28
#endif
/******************************************************************************************/
/* --------------------------   SYNC SETUP      ------------------------------------------*/
/******************************************************************************************/

/// Maximum number of Synchronous connections (0 to 2)
#define CFG_VOHCI   1
#ifndef MAX_NB_SYNC
	#define MAX_NB_SYNC 1
#endif

/// Flag indicating if debug mode is activated or not
#if defined(CFG_VOHCI) && (MAX_NB_SYNC > 0)
#if !BT_TEST_CODE_HCI_DIS
    #define VOICE_OVER_HCI 1
#endif
#else
    #define VOICE_OVER_HCI 0
#endif // defined (CFG_VOHCI) && (MAX_NB_SYNC > 0)

#if VOICE_OVER_HCI
    /// Number of Synchronous TX buffers used for one VoHCI link
	#define SYNC_TX_BUF_NB 8
    /// Number of Synchronous RX buffers used for one VoHCI link
	#define SYNC_RX_BUF_NB 8
    /// Guard space reserved for HCI header in Sync RX buffers
    #define SYNC_RX_BUF_HEADER_SPACE (HCI_SYNC_HDR_LEN + 1)
#endif //(VOICE_OVER_HCI)

/// Synchronous link negotiation number of retries
#define SYNC_NEGO_TRY 1

/******************************************************************************************/
/* -------------------------   FIRMWARE SETUP      ---------------------------------------*/
/******************************************************************************************/

///Number of HCI Event filter
#define HCI_FILTER_NB 0x5

/*
 * Test mode option to be able to enter test mode as defined
 * in part I1: BT Test Mode, of the BT specification.
 */
/// Test mode support
#define TEST_MODE 1

/******************************************************************************************/
/* -------------------------   COEXISTENCE SETUP      ------------------------------------*/
/******************************************************************************************/

/// WLAN Coexistence
#define RW_BT_WLAN_COEX RW_WLAN_COEX
///WLAN test mode
#define RW_BT_WLAN_COEX_TEST RW_WLAN_COEX_TEST

/// MWS Coexistence
#define RW_BT_MWS_COEX RW_MWS_COEX
///WLAN test mode
#define RW_BT_MWS_COEX_TEST RW_MWS_COEX_TEST

///To let the HW using the default values set in the registers
#define RW_BT_PTI_PRIO_AUTO 31

/// Coexistence Packet traffic information - Index Definitions
enum rwbt_pti_config_idx
{
    BT_PTI_PAGERES_IDX,
    BT_PTI_MCONPEND_IDX,
    BT_PTI_PSCANRES_IDX,
    BT_PTI_SCONPEND_IDX,
    BT_PTI_MSSWITCH_IDX,
    BT_PTI_SNIFFANC_IDX,
    BT_PTI_BEACON_IDX,
    BT_PTI_SNIFFATT_IDX,
    BT_PTI_ACCWIN_IDX,
    BT_PTI_POLLINT_IDX,
    BT_PTI_PAGE_IDX,
    BT_PTI_PSCAN_IDX,
    BT_PTI_INQ_IDX,
    BT_PTI_INQSCAN_IDX,
    BT_PTI_INQRES_IDX,
    BT_PTI_SCORSVD_IDX,
    BT_PTI_SCORETX_IDX,
    BT_PTI_ACLTX_IDX,
    BT_PTI_ACLRETX_IDX,
    BT_PTI_LMPTX_IDX,
    BT_PTI_BCAST_IDX,
    BT_PTI_CONNECT_IDX,
    BT_PTI_IDX_MAX
};

/******************************************************************************************/
/* -------------------------        AFH SETUP         ------------------------------------*/
/******************************************************************************************/

/// RSSI threshold for interference detection (in dBm)
#define BT_AFH_RSSI_INTERF_THR (25)

/// Minimum classification interval requested to slave devices (in slots) (2 secs)
#define BT_AFH_CH_CLASS_INT_MIN 0x0C80
/// Maximum classification interval requested to slave devices (in slots) (4 secs)
#define BT_AFH_CH_CLASS_INT_MAX 0x1900

/// AFH update period (in sec)
#define BT_AFH_UPDATE_PERIOD 4

/// Maximum duration without receiving a packet before considering a channel quality unknown  (in slots) (2 secs)
#define BT_AFH_ASSESS_VALID_TO 0x1900

/// Maximum duration without receiving a packet before reintroducing a channel to the map (in slots) (20 secs)
#define BT_AFH_REASSESS_TO 0x7D00

/// Maximum counter value for channel assessment
#define BT_AFH_ASSESS_COUNT_MAX 4
/// Counter threshold to consider a channel good
#define BT_AFH_ASSESS_COUNT_THR_GOOD 2
/// Counter threshold to consider a channel bad
#define BT_AFH_ASSESS_COUNT_THR_BAD -2
/// Minimum counter value for channel assessment
#define BT_AFH_ASSESS_COUNT_MIN -4

/******************************************************************************************/
/* --------------------------     CSB SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Support of Connectionless Slave Broadcast
#if defined(CFG_CSB)
    #define CSB_SUPPORT 1
#else
    #define CSB_SUPPORT 0
#endif // defined (CFG_CSB)

/******************************************************************************************/
/* --------------------------     PCA SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Support of Piconet Clock Adjust
#if defined(CFG_PCA)
    #define PCA_SUPPORT 1
#else
    #define PCA_SUPPORT 0
#endif // defined (CFG_PCA)

/// PCA update period (in sec)
#define BT_PCA_UPDATE_PERIOD 30

/// PCA request clk_adj_slots in addition to clk_adj_us: enable=1, disable=0
#define BT_PCA_CLK_ADJ_SLOTS 0 // No mandate to request a clk_adj_slots currently - for test only

/******************************************************************************************/
/* ------------------------   CONNECTION HANDLE   ----------------------------------------*/
/******************************************************************************************/

/// Bit set for BT ACL connection handles
#define BT_ACL_CONHDL_BIT BIT(3)
#define BT_ACL_CONHDL_MIN (BT_ACL_CONHDL_BIT)
#define BT_ACL_CONHDL_MAX (BT_ACL_CONHDL_BIT + MAX_NB_ACTIVE_ACL - 1)

#define LE_ACL_M_HANDLE   BIT(7)
#define LE_ACL_S_HANDLE   BIT(6)
#define LE_CIS_HANDLE     BIT(5)
#define LE_BIS_HANDLE     BIT(4)
/// Bit set for BT SCO connection handles
#define BT_SYNC_CONHDL_OFF                   (8)
#define BT_SYNC_CONHDL_MSK                   (((1 << MAX_NB_SYNC) - 1) << BT_SYNC_CONHDL_OFF)
#define BT_SYNC_CONHDL_BIT(sco_link_id)      ((sco_link_id + 1) << BT_SYNC_CONHDL_OFF)
#define BT_SYNC_CONHDL_LID(sco_conhdl)       ((sco_conhdl >> BT_SYNC_CONHDL_OFF) - 1)
#define BT_SYNC_CONHDL(link_id, sco_link_id) (BT_ACL_CONHDL_MIN + link_id + BT_SYNC_CONHDL_BIT(sco_link_id))

/// @} BT Stack Configuration
/// @} ROOT

#endif //RWBT_CONFIG_H_
