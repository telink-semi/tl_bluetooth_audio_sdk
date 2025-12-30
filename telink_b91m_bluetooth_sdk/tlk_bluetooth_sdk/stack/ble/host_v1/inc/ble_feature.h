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
#define BLE_HOST_FEATURE_LE_ENCRYPTION (1 << 0)
// Connection Parameters Request Procedure
#define BLE_HOST_FEATURE_CONN_PARAMS (1 << 1)
// Extended Reject Indication
#define BLE_HOST_FEATURE_EXT_REJ_IND (1 << 2)
// Peripheral Initiated Features Exchange
#define BLE_HOST_FEATURE_PERIPHERAL_INIT_FEAT_EXCHANGE (1 << 3)
// LE Ping
#define BLE_HOST_FEATURE_LE_PING (1 << 4)
// LE Data Packet Length Extension
#define BLE_HOST_FEATURE_LE_DATA_LEN_EXT (1 << 5)
// LL Privacy
#define BLE_HOST_FEATURE_LL_PRIVACY (1 << 6)
// Extended Scanner Filter Policies
#define BLE_HOST_FEATURE_EXT_SCAN_FILT_POL (1 << 7)
// LE 2M PHY
#define BLE_HOST_FEATURE_LE_2M_PHY (1 << 8)
// Stable Modulation Index - Transmitter
#define BLE_HOST_FEATURE_STABLE_MOD_IDX_TX (1 << 9)
// Stable Modulation Index - Receiver
#define BLE_HOST_FEATURE_STABLE_MOD_IDX_RX (1 << 10)
// LE Coded PHY
#define BLE_HOST_FEATURE_LE_CODED_PHY (1 << 11)
// LE Extended Advertising
#define BLE_HOST_FEATURE_LE_EXT_ADV (1 << 12)
// LE Periodic Advertising
#define BLE_HOST_FEATURE_LE_PERIODIC_ADV (1 << 13)
// Channel Selection Algorithm #2
#define BLE_HOST_FEATURE_CHAN_SEL_2 (1 << 14)
// LE Power Class 1
#define BLE_HOST_FEATURE_LE_POWER_CLASS_1 (1 << 15)
// Minimum Number of Used Channels Procedure
#define BLE_HOST_FEATURE_MIN_USED_CHAN_PROC (1 << 16)
// Connection CTE Request
#define BLE_HOST_FEATURE_CONN_CTE_REQ (1 << 17)
// Connection CTE Response
#define BLE_HOST_FEATURE_CONN_CTE_RESP (1 << 18)
// Connectionless CTE Transmitter
#define BLE_HOST_FEATURE_CONNLESS_CTE_TX (1 << 19)
// Connectionless CTE Receiver
#define BLE_HOST_FEATURE_CONNLESS_CTE_RX (1 << 20)
// Antenna Switching During CTE Transmission(AoD)
#define BLE_HOST_FEATURE_AOD (1 << 21)
// Antenna Switching During CTE Reception(AoA)
#define BLE_HOST_FEATURE_AOA (1 << 22)
// Receiving Constant Tone Extension
#define BLE_HOST_FEATURE_RC_CTE (1 << 23)
// Periodic Advertising Sync TransferSender
#define BLE_HOST_FEATURE_PA_SYNC_TX (1 << 24)
// Periodic Advertising Sync TransferRecipient
#define BLE_HOST_FEATURE_PA_SYNC_RX (1 << 25)
// Sleep Clock Accuracy Updates
#define BLE_HOST_FEATURE_SCA_UPD (1 << 26)
// Remote Public Key Validation
#define BLE_HOST_FEATURE_RPK_VAL (1 << 27)
// Connection Isochronous Stream - Central
#define BLE_HOST_FEATURE_CIS_CENTRAL (1 << 28)
// Connection Isochronous Stream - Peripheral
#define BLE_HOST_FEATURE_CIS_PERIPHERAL (1 << 29)
// Isochronous Broadcaster
#define BLE_HOST_FEATURE_ISO_BROADCASTER (1 << 30)
// Synchronized Receiver
#define BLE_HOST_FEATURE_SYNC_RECEIVER (1 << 31)
// Connected Isochronous Stream(Host Support)
#define BLE_HOST_FEATURE_CIS_HOST_SUPPORT (1 << 32)
// if support LE power control request BIT 33 and BTI 34 always set to 1.
// LE Power Control Request
#define BLE_HOST_FEATURE_LE_POWER_CTRL_REQ (1 << 33)
// LE Power Control Request
#define BLE_HOST_FEATURE_LE_POWER_CTRL_REQ_2 (1 << 34)
// LE Path Loss Monitoring
#define BLE_HOST_FEATURE_LE_PATH_LOSS_MONITOR (1 << 35)
// Periodic Advertising ADI support
#define BLE_HOST_FEATURE_PA_ADI_SUPPORT (1 << 36)
// Connection Subrating
#define BLE_HOST_FEATURE_CONN_SUBRATING (1 << 37)
// Connection Subrating(Host Support)
#define BLE_HOST_FEATURE_CONN_SUBRATING_HOST_SUPPORT (1 << 38)
// Channel Classification
#define BLE_HOST_FEATURE_CHAN_CLASSIFICATION (1 << 39)
// Advertising Coding Selection
#define BLE_HOST_FEATURE_ADV_CODING_SEL (1 << 40)
// Advertising Coding Selection(Host Support)
#define BLE_HOST_FEATURE_ADV_CODING_SEL_HOST_SUPPORT (1 << 41)
// Periodic Advertising with Responses Advertiser
#define BLE_HOST_FEATURE_PAwR_ADVERTISER (1 << 43)
// Periodic Advertising with Responses Scanner
#define BLE_HOST_FEATURE_PAwR_SCANNER (1 << 44)

struct ble_host_features_bits_mapping
{
    uint64_t le_encryption               : 1;
    uint64_t conn_params                 : 1;
    uint64_t ext_rej_ind                 : 1;
    uint64_t periph_init_feat_exch       : 1;
    uint64_t le_ping                     : 1;
    uint64_t le_data_len_ext             : 1;
    uint64_t ll_privacy                  : 1;
    uint64_t ext_scan_filt_pol           : 1;
    uint64_t le_2m_phy                   : 1;
    uint64_t stable_mod_idx_tx           : 1;
    uint64_t stable_mod_idx_rx           : 1;
    uint64_t le_coded_phy                : 1;
    uint64_t le_ext_adv                  : 1;
    uint64_t le_periodic_adv             : 1;
    uint64_t chan_sel_2                  : 1;
    uint64_t le_power_class_1            : 1;
    uint64_t min_used_chan_proc          : 1;
    uint64_t conn_cte_req                : 1;
    uint64_t conn_cte_resp               : 1;
    uint64_t connless_cte_tx             : 1;
    uint64_t connless_cte_rx             : 1;
    uint64_t aod                         : 1;
    uint64_t aoa                         : 1;
    uint64_t rc_cte                      : 1;
    uint64_t pa_sync_tx                  : 1;
    uint64_t pa_sync_rx                  : 1;
    uint64_t sca_upd                     : 1;
    uint64_t rpk_val                     : 1;
    uint64_t cis_central                 : 1;
    uint64_t cis_peripheral              : 1;
    uint64_t iso_broadcaster             : 1;
    uint64_t sync_receiver               : 1;
    uint64_t cis_host_support            : 1;
    uint64_t le_power_ctrl_req           : 1;
    uint64_t le_power_ctrl_req_2         : 1;
    uint64_t le_path_loss_monitor        : 1;
    uint64_t pa_adi_support              : 1;
    uint64_t conn_subrating              : 1;
    uint64_t conn_subrating_host_support : 1;
    uint64_t chan_classification         : 1;
    uint64_t adv_coding_sel              : 1;
    uint64_t adv_coding_sel_host_support : 1;
    uint64_t bit42_rfu                   : 1;
    uint64_t pawr_advertiser             : 1;
    uint64_t pawr_scanner                : 1;
    uint64_t bits_44_to_55_rfu           : 12;
    uint64_t bits_56_to_62_rfu           : 7;
    uint64_t bits_63_rfu                 : 1;
};
