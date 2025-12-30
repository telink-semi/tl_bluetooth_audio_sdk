/********************************************************************************************************
 * @file    ble_hci_evt.h
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
#ifndef STACK_BLE_HOST_V1_BLE_HCI_EVT_H_
#define STACK_BLE_HOST_V1_BLE_HCI_EVT_H_

// #ifdef __cplusplus
// extern "C" {
// #endif


/* Generic event header */
struct ble_hci_evt
{
    uint8_t opcode;
    uint8_t length;
    uint8_t data[0];
} __attribute__((packed));

/* Generic event header */
struct ble_hci_evt_h4
{
    uint8_t type;
    uint8_t opcode;
    uint8_t length;
    uint8_t data[0];
} __attribute__((packed));

#define BLE_HCI_EVT_HDR_LEN (2)

/* HCI Vendors Specific commands extension supported by Telink BLE Host stack */
#define BLE_HCI_VS_EVT_ENABLE (0x01)


/* Data buffer overflow event */
#define BLE_HCI_EVENT_ACL_BUF_OVERFLOW (0x01)

/* Advertising report */
#define BLE_HCI_ADV_RPT_EVT_TYPE_ADV_IND     (0)
#define BLE_HCI_ADV_RPT_EVT_TYPE_DIR_IND     (1)
#define BLE_HCI_ADV_RPT_EVT_TYPE_SCAN_IND    (2)
#define BLE_HCI_ADV_RPT_EVT_TYPE_NONCONN_IND (3)
#define BLE_HCI_ADV_RPT_EVT_TYPE_SCAN_RSP    (4)

/* Bluetooth 5, Vol 2, Part E, 7.7.65.13 */
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_ADV_IND               (0x13)
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_ADV_DIRECT_IND        (0x15)
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_ADV_SCAN_IND          (0x12)
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_ADV_NONCON_IND        (0x10)
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_SCAN_RSP_ADV_IND      (0x1b)
#define BLE_HCI_LEGACY_ADV_EVT_TYPE_SCAN_RSP_ADV_SCAN_IND (0x1a)

/* LE connection complete event (sub event 0x01) */
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_MASTER (0x00)
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_SLAVE  (0x01)

/* Maximum valid connection handle value */
#define BLE_HCI_LE_CONN_HANDLE_MAX (0x0eff)

/* LE advertising report event. (sub event 0x02) */
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN     (1)
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX     (0x19)
#define BLE_HCI_LE_DIR_ADV_RPT_NUM_RPTS_MAX (0x0F)
#define BLE_HCI_LE_EXT_ADV_RPT_NUM_RPTS_MAX (0x0A)

/* LE (enhanced) connection complete event (sub event (0x29) / 0x01) */
#define BLE_HCI_CONN_ROLE_CENTRAL    (0)
#define BLE_HCI_CONN_ROLE_PERIPHERAL (1)

/* Event Codes */
#define BLE_HCI_EVCODE_INQUIRY_CMP    (0x01)
#define BLE_HCI_EVCODE_INQUIRY_RESULT (0x02)
#define BLE_HCI_EVCODE_CONN_DONE      (0x03)
#define BLE_HCI_EVCODE_CONN_REQUEST   (0x04)
#define BLE_HCI_EVCODE_DISCONNECT_CMP (0x05)

struct ble_hci_ev_disconnect_cmp
{
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));

#define BLE_HCI_EVCODE_AUTH_CMP         (0x06)
#define BLE_HCI_EVCODE_REM_NAME_REQ_CMP (0x07)

#define BLE_HCI_EVCODE_ENCRYPT_CHG      (0x08)

struct ble_hci_ev_encrypt_chg
{
    uint8_t  status;
    uint16_t connection_handle;
    uint8_t  enabled;
} __attribute__((packed));

#define BLE_HCI_EVCODE_CHG_LINK_KEY_CMP     (0x09)
#define BLE_HCI_EVCODE_MASTER_LINK_KEY_CMP  (0x0A)
#define BLE_HCI_EVCODE_RD_REM_SUPP_FEAT_CMP (0x0B)
#define BLE_HCI_EVCODE_RD_REM_VER_INFO_CMP  (0x0C)

struct ble_hci_ev_rd_rem_ver_info_cmp
{
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  version;
    uint16_t manufacturer;
    uint16_t subversion;
} __attribute__((packed));

#define BLE_HCI_EVCODE_QOS_SETUP_CMP    (0x0D)

#define BLE_HCI_EVCODE_COMMAND_COMPLETE (0x0E)

struct ble_hci_ev_command_complete
{
    uint8_t  num_packets;
    uint16_t opcode;
    uint8_t  status;
    uint8_t  return_params[0];
} __attribute__((packed));

/* NOP is exception and has no return parameters */
struct ble_hci_ev_command_complete_nop
{
    uint8_t  num_packets;
    uint16_t opcode;
} __attribute__((packed));

#define BLE_HCI_EVCODE_COMMAND_STATUS (0x0F)

struct ble_hci_ev_command_status
{
    uint8_t  status;
    uint8_t  num_packets;
    uint16_t opcode;
} __attribute__((packed));

#define BLE_HCI_EVCODE_HW_ERROR (0x10)

struct ble_hci_ev_hw_error
{
    uint8_t hw_code;
} __attribute__((packed));

#define BLE_HCI_EVCODE_NUM_COMP_PKTS (0x13)

struct comp_pkt
{
    uint16_t handle;
    uint16_t packets;
} __attribute__((packed));

;

struct ble_hci_ev_num_comp_pkts
{
    uint8_t         count;
    struct comp_pkt completed[0];
} __attribute__((packed));

#define BLE_HCI_EVCODE_MODE_CHANGE       (0x14)
#define BLE_HCI_EVCODE_RETURN_LINK_KEYS  (0x15)
#define BLE_HCI_EVCODE_PIN_CODE_REQ      (0x16)
#define BLE_HCI_EVCODE_LINK_KEY_REQ      (0x17)
#define BLE_HCI_EVCODE_LINK_KEY_NOTIFY   (0x18)
#define BLE_HCI_EVCODE_LOOPBACK_CMD      (0x19)

#define BLE_HCI_EVCODE_DATA_BUF_OVERFLOW (0x1A)

struct ble_hci_ev_data_buf_overflow
{
    uint8_t link_type;
} __attribute__((packed));

#define BLE_HCI_EVCODE_MAX_SLOTS_CHG     (0x1B)
#define BLE_HCI_EVCODE_READ_CLK_OFF_COMP (0x1C)
#define BLE_HCI_EVCODE_CONN_PKT_TYPE_CHG (0x1D)
#define BLE_HCI_EVCODE_QOS_VIOLATION     (0x1E)
/* NOTE: 0x1F not defined */
#define BLE_HCI_EVCODE_PSR_MODE_CHG      (0x20)
#define BLE_HCI_EVCODE_FLOW_SPEC_COMP    (0x21)
#define BLE_HCI_EVCODE_INQ_RESULT_RSSI   (0x22)
#define BLE_HCI_EVCODE_READ_REM_EXT_FEAT (0x23)
/* NOTE: 0x24 - 0x2B not defined */
#define BLE_HCI_EVCODE_SYNCH_CONN_COMP (0x2C)
#define BLE_HCI_EVCODE_SYNCH_CONN_CHG  (0x2D)
#define BLE_HCI_EVCODE_SNIFF_SUBRATING (0x2E)
#define BLE_HCI_EVCODE_EXT_INQ_RESULT  (0x2F)

#define BLE_HCI_EVCODE_ENC_KEY_REFRESH (0x30)

struct ble_hci_ev_enc_key_refresh
{
    uint8_t  status;
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_EVOCDE_IO_CAP_REQ       (0x31)
#define BLE_HCI_EVCODE_IO_CAP_RSP       (0x32)
#define BLE_HCI_EVCODE_USER_CONFIRM_REQ (0x33)
#define BLE_HCI_EVCODE_PASSKEY_REQ      (0x34)
#define BLE_HCI_EVCODE_REM_OOB_DATA_REQ (0x35)
#define BLE_HCI_EVCODE_SIMPLE_PAIR_COMP (0x36)
/* NOTE: 0x37 not defined */
#define BLE_HCI_EVCODE_LNK_SPVN_TMO_CHG    (0x38)
#define BLE_HCI_EVCODE_ENH_FLUSH_COMP      (0x39)
#define BLE_HCI_EVCODE_USER_PASSKEY_NOTIFY (0x3B)
#define BLE_HCI_EVCODE_KEYPRESS_NOTIFY     (0x3C)
#define BLE_HCI_EVCODE_REM_HOST_SUPP_FEAT  (0x3D)

#define BLE_HCI_EVCODE_LE_META             (0x3E)

struct ble_hci_ev_le_meta
{
    uint8_t subevent;
    uint8_t data[0];
} __attribute__((packed));

/* NOTE: 0x3F not defined */

/* NOTE: 0x40 - 0x4D no longer used */

#define BLE_HCI_EVCODE_TRIG_CLK_CAPTURE     (0x4E)
#define BLE_HCI_EVCODE_SYNCH_TRAIN_COMP     (0x4F)
#define BLE_HCI_EVCODE_SYNCH_TRAIN_RCVD     (0x50)
#define BLE_HCI_EVCODE_SLAVE_BCAST_RX       (0x51)
#define BLE_HCI_EVCODE_SLAVE_BCAST_TMO      (0x52)
#define BLE_HCI_EVCODE_TRUNC_PAGE_COMP      (0x53)
#define BLE_HCI_EVCODE_SLAVE_PAGE_RSP_TMO   (0x54)
#define BLE_HCI_EVCODE_SLAVE_BCAST_CHAN_MAP (0x55)
#define BLE_HCI_EVCODE_INQ_RSP_NOTIFY       (0x56)

#define BLE_HCI_EVCODE_AUTH_PYLD_TMO        (0x57)

struct ble_hci_ev_auth_pyld_tmo
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_EVCODE_SAM_STATUS_CHG (0x58)


/******************************************************************************
 * LE meta event codes (0x3E) - LE sub-event codes (1 ~ )
 ******************************************************************************/
/* LE sub-event codes */
#define BLE_HCI_LE_SUBEV_CONN_COMPLETE (0x01)

struct ble_hci_ev_le_subev_conn_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  role;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint8_t  mca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ADV_RPT (0x02)

struct adv_reports
{
    uint8_t type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t data_len;
    uint8_t data[0];
    // uint8_t rssi;    // Note add data[data_len] size.
} __attribute__((packed));

struct ble_hci_ev_le_subev_adv_rpt
{
    uint8_t            subev_code;
    uint8_t            num_reports;
    struct adv_reports reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE (0x03)

struct ble_hci_ev_le_subev_conn_upd_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT (0x04)

struct ble_hci_ev_le_subev_rd_rem_used_feat
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  features[8];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_LT_KEY_REQ (0x05)

struct ble_hci_ev_le_subev_lt_key_req
{
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint64_t rand;
    uint16_t div;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_REM_CONN_PARAM_REQ (0x06)

struct ble_hci_ev_le_subev_rem_conn_param_req
{
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t latency;
    uint16_t timeout;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_DATA_LEN_CHG (0x07)

struct ble_hci_ev_le_subev_data_len_chg
{
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
    uint16_t max_rx_octets;
    uint16_t max_rx_time;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_RD_LOC_P256_PUBKEY (0x08)

struct ble_hci_ev_le_subev_rd_loc_p256_pubkey
{
    uint8_t subev_code;
    uint8_t status;
    uint8_t public_key[64];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_GEN_DHKEY_COMPLETE (0x09)

struct ble_hci_ev_le_subev_gen_dhkey_complete
{
    uint8_t subev_code;
    uint8_t status;
    uint8_t dh_key[32];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE (0x0A)

struct ble_hci_ev_le_subev_enh_conn_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  role;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  local_rpa[6];
    uint8_t  peer_rpa[6];
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint8_t  mca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_DIRECT_ADV_RPT (0x0B)

struct dir_adv_reports
{
    uint8_t type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t dir_addr_type;
    uint8_t dir_addr[6];
    int8_t  rssi;
} __attribute__((packed));

struct ble_hci_ev_le_subev_direct_adv_rpt
{
    uint8_t                subev_code;
    uint8_t                num_reports;
    struct dir_adv_reports reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE (0x0C)

struct ble_hci_ev_le_subev_phy_update_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  tx_phy;
    uint8_t  rx_phy;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_EXT_ADV_RPT (0x0D)

struct ext_adv_reports
{
    uint16_t evt_type;
    uint8_t  addr_type;
    uint8_t  addr[6];
    uint8_t  pri_phy;
    uint8_t  sec_phy;
    uint8_t  sid;
    int8_t   tx_power;
    int8_t   rssi;
    uint16_t periodic_itvl;
    uint8_t  dir_addr_type;
    uint8_t  dir_addr[6];
    uint8_t  data_len;
    uint8_t  data[0];
} __attribute__((packed));

struct ble_hci_ev_le_subev_ext_adv_rpt
{
    uint8_t                subev_code;
    uint8_t                num_reports;
    struct ext_adv_reports reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_ESTAB (0x0E)

struct ble_hci_ev_le_subev_periodic_adv_sync_estab
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t sync_handle;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  phy;
    uint16_t interval;
    uint8_t  aca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_RPT (0x0F)

struct ble_hci_ev_le_subev_periodic_adv_rpt
{
    uint8_t  subev_code;
    uint16_t sync_handle;
    int8_t   tx_power;
    int8_t   rssi;
    uint8_t  cte_type;
    uint8_t  data_status;
    uint8_t  data_len;
    uint8_t  data[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_LOST (0x10)

struct ble_hci_ev_le_subev_periodic_adv_sync_lost
{
    uint8_t  subev_code;
    uint16_t sync_handle;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_SCAN_TIMEOUT (0x11)

struct ble_hci_ev_le_subev_scan_timeout
{
    uint8_t subev_code;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED (0x12)

struct ble_hci_ev_le_subev_adv_set_terminated
{
    uint8_t  subev_code;
    uint8_t  status;
    uint8_t  adv_handle;
    uint16_t conn_handle;
    uint8_t  num_events;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD (0x13)

struct ble_hci_ev_le_subev_scan_req_rcvd
{
    uint8_t subev_code;
    uint8_t adv_handle;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CHAN_SEL_ALG (0x14)

struct ble_hci_ev_le_subev_chan_sel_alg
{
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint8_t  csa;
} __attribute__((packed));

// not supported in current version of BLE stack
#define BLE_HCI_LE_SUBEV_CONNLESS_IQ_RPT            (0x15)
#define BLE_HCI_LE_SUBEV_CONN_IQ_RPT                (0x16)
#define BLE_HCI_LE_SUBEV_CTE_REQ_FAILED             (0x17)

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_TRANSFER (0x18)

struct ble_hci_ev_le_subev_periodic_adv_sync_transfer
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint16_t service_data;
    uint16_t sync_handle;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  phy;
    uint16_t interval;
    uint8_t  aca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CIS_ESTABLISHED (0x19)

struct ble_hci_ev_le_subev_cis_established
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  cig_sync_delay[3];
    uint8_t  cis_sync_delay[3];
    uint8_t  transport_latency_c_to_p[3];
    uint8_t  transport_latency_p_to_c[3];
    uint8_t  phy_c_to_p;
    uint8_t  phy_p_to_c;
    uint8_t  nse;
    uint8_t  bn_c_to_p;
    uint8_t  bn_p_to_c;
    uint8_t  ft_c_to_p;
    uint8_t  ft_p_to_c;
    uint16_t max_pdu_c_to_p;
    uint16_t max_pdu_p_to_c;
    uint16_t iso_interval;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CIS_REQUEST (0x1A)

struct ble_hci_ev_le_subev_cis_request
{
    uint8_t  subev_code;
    uint16_t acl_conn_handle;
    uint16_t cis_conn_handle;
    uint8_t  cig_id;
    uint8_t  cis_id;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CREATE_BIG_COMPLETE (0x1B)

struct ble_hci_ev_le_subev_create_big_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint8_t  big_handle;
    uint8_t  big_sync_delay[3];
    uint8_t  transport_latency_big[3];
    uint8_t  phy;
    uint8_t  nse;
    uint8_t  bn;
    uint8_t  pto;
    uint8_t  irc;
    uint16_t max_pdu;
    uint16_t iso_interval;
    uint8_t  num_bis;
    uint16_t conn_handle[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_TERMINATE_BIG_COMPLETE (0x1C)

struct ble_hci_ev_le_subev_terminate_big_complete
{
    uint8_t subev_code;
    uint8_t big_handle;
    uint8_t reason;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_BIG_SYNC_ESTABLISHED (0x1D)

struct ble_hci_ev_le_subev_big_sync_established
{
    uint8_t  subev_code;
    uint8_t  status;
    uint8_t  big_handle;
    uint8_t  transport_latency_big[3];
    uint8_t  nse;
    uint8_t  bn;
    uint8_t  pto;
    uint8_t  irc;
    uint16_t max_pdu;
    uint16_t iso_interval;
    uint8_t  num_bis;
    uint16_t conn_handle[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_BIG_SYNC_LOST (0x1E)

struct ble_hci_ev_le_subev_big_sync_lost
{
    uint8_t subev_code;
    uint8_t big_handle;
    uint8_t reason;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_REQ_PEER_SCA_COMP (0x1F)

struct ble_hci_ev_le_subev_peer_sca_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  sca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PATH_LOSS_THRESHOLD (0x20)

struct ble_hci_ev_le_subev_path_loss_threshold
{
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint8_t  current_path_loss;
    uint8_t  zone_entered;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_TRANSMIT_POWER_REPORT (0x21)

struct ble_hci_ev_le_subev_transmit_power_report
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  reason;
    uint8_t  phy;
    int8_t   transmit_power_level;
    uint8_t  transmit_power_level_flag;
    int8_t   delta;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_BIGINFO_ADV_REPORT (0x22)

struct ble_hci_ev_le_subev_biginfo_adv_report
{
    uint8_t  subev_code;
    uint16_t sync_handle;
    uint8_t  bis_cnt;
    uint8_t  nse;
    uint16_t iso_interval;
    uint8_t  bn;
    uint8_t  pto;
    uint8_t  irc;
    uint16_t max_pdu;
    uint8_t  sdu_interval[3];
    uint16_t max_sdu;
    uint8_t  phy;
    uint8_t  framing;
    uint8_t  encryption;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_SUBRATE_CHANGE (0x23)

struct ble_hci_ev_le_subev_subrate_change
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint16_t subrate_factor;
    uint16_t periph_latency;
    uint16_t cont_num;
    uint16_t supervision_tmo;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_RD_REM_SUPP_CAP_COMPLETE (0x2C)

struct ble_hci_ev_le_subev_cs_rd_rem_supp_cap_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  num_config_supported;
    uint16_t max_consecutive_procedures_supported;
    uint8_t  num_antennas_supported;
    uint8_t  max_antenna_paths_supported;
    uint8_t  roles_supported;
    uint8_t  optional_modes_supported;
    uint8_t  rtt_capability;
    uint8_t  rtt_aa_only_n;
    uint8_t  rtt_sounding_n;
    uint8_t  rtt_random_payload_n;
    uint16_t optional_nadm_sounding_capability;
    uint16_t optional_nadm_random_capability;
    uint8_t  optional_cs_sync_phys_supported;
    uint16_t optional_subfeatures_supported;
    uint16_t optional_t_ip1_times_supported;
    uint16_t optional_t_ip2_times_supported;
    uint16_t optional_t_fcs_times_supported;
    uint16_t optional_t_pm_times_supported;
    uint8_t  t_sw_time_supported;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_RD_REM_FAE_COMPLETE (0x2D)

struct ble_hci_ev_le_subev_cs_rd_rem_fae_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  remote_fae_table[72];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_SEC_ENABLE_COMPLETE (0x2E)

struct ble_hci_ev_le_subev_cs_sec_enable_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_CONFIG_COMPLETE (0x2F)

struct ble_hci_ev_le_subev_cs_config_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  config_id;
    uint8_t  action;
    uint8_t  main_mode_type;
    uint8_t  sub_mode_type;
    uint8_t  min_main_mode_steps;
    uint8_t  max_main_mode_steps;
    uint8_t  main_mode_repetition;
    uint8_t  mode_0_steps;
    uint8_t  role;
    uint8_t  rtt_type;
    uint8_t  cs_sync_phy;
    uint8_t  channel_map[10];
    uint8_t  channel_map_repetition;
    uint8_t  channel_selection_type;
    uint8_t  ch3c_shape;
    uint8_t  ch3c_jump;
    uint8_t  companion_signal_enable;
    uint8_t  t_ip1_time;
    uint8_t  t_ip2_time;
    uint8_t  t_fcs_time;
    uint8_t  t_pm_time;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_PROC_ENABLE_COMPLETE (0x30)

struct ble_hci_ev_le_subev_cs_proc_enable_complete
{
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  config_id;
    uint8_t  state;
    uint8_t  tone_antenna_config_selection;
    uint8_t  selected_tx_power;
    uint8_t  subevent_len[3];
    uint8_t  subevents_per_event;
    uint16_t subevent_interval;
    uint16_t event_interval;
    uint16_t procedure_interval;
    uint16_t procedure_count;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_SUBEVENT_RESULT (0x31)

struct cs_steps_data
{
    uint8_t mode;
    uint8_t channel;
    uint8_t data_len;
    uint8_t data[];
} __attribute__((packed));

struct ble_hci_ev_le_subev_cs_subevent_result
{
    uint8_t              subev_code;
    uint16_t             conn_handle;
    uint8_t              config_id;
    uint16_t             start_acl_conn_event_counter;
    uint16_t             procedure_counter;
    uint16_t             frequency_compensation;
    uint8_t              reference_power_level;
    uint8_t              procedure_done_status;
    uint8_t              subevent_done_status;
    uint8_t              abort_reason;
    uint8_t              num_antenna_paths;
    uint8_t              num_steps_reported;
    struct cs_steps_data steps[];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_SUBEVENT_RESULT_CONTINUE (0x32)

struct ble_hci_ev_le_subev_cs_subevent_result_continue
{
    uint8_t              subev_code;
    uint16_t             conn_handle;
    uint8_t              config_id;
    uint8_t              procedure_done_status;
    uint8_t              subevent_done_status;
    uint8_t              abort_reason;
    uint8_t              num_antenna_paths;
    uint8_t              num_steps_reported;
    struct cs_steps_data steps[];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CS_TEST_END_COMPLETE (0x33)

struct ble_hci_ev_le_subev_cs_test_end_complete
{
    uint8_t subev_code;
    uint8_t status;
} __attribute__((packed));

/* NOTE: 0xFE is reserved for specification development purposes */

/* NOTE: 0xFF is reserved for vendor-specific events.*/

/******************************************************************************
 * Vendor Specific HCI Events (0xFF)
 ******************************************************************************/
#if (BLE_HCI_VS_EVT_ENABLE)
#define BLE_HCI_EVCODE_VS (0xff)

#endif /* (BLE_HCI_VS_EVT_ENABLE) */


/**
 * @brief       Dispatch an HCI event.
 * @param[in]   ev    - the event to dispatch.
 * @return      none.
 */
void ble_host_hci_evt_dispatch_process(void *ev);


// #ifdef __cplusplus
// }
// #endif

#endif /* STACK_BLE_HOST_V1_BLE_HCI_EVT_H_ */
