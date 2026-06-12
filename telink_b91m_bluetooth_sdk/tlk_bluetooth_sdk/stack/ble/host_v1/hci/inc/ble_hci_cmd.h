/********************************************************************************************************
 * @file    ble_hci_cmd.h
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
#ifndef STACK_BLE_HOST_V1_BLE_HCI_CMD_H_
#define STACK_BLE_HOST_V1_BLE_HCI_CMD_H_

// #ifdef __cplusplus
// extern "C" {
// #endif


/* Generic command header */
struct ble_hci_cmd
{
    uint16_t opcode;
    uint8_t  length;
    uint8_t  data[0];
} __attribute__((packed));

struct ble_hci_cmd_h4
{
    uint8_t  type;
    uint16_t opcode;
    uint8_t  length;
    uint8_t  data[0];
} __attribute__((packed));

/* HCI command received status or complete event header, used for ACKs */
struct ble_host_hci_ack
{
    int            bha_status;
    const uint8_t *bha_params;
    int            bha_params_len;
    uint16_t       bha_opcode;
    uint8_t        bha_hci_handle;
};

/* */
#define BLE_HCI_CMD_HDR_LEN (3)

/* HCI Vendors Specific commands extension supported by Telink BLE Host stack */
#define BLE_HCI_VS_CMD_ENABLE (0x01)


/* Command Specific Definitions */
/* --- Set controller to host flow control (OGF 0x03, OCF 0x0031) --- */
#define BLE_HCI_CTLR_TO_HOST_FC_OFF  (0)
#define BLE_HCI_CTLR_TO_HOST_FC_ACL  (1)
#define BLE_HCI_CTLR_TO_HOST_FC_SYNC (2)
#define BLE_HCI_CTLR_TO_HOST_FC_BOTH (3)

/* --- LE set advertising parameters (OCF 0x0006) */
/* Advertising types */
#define BLE_HCI_ADV_TYPE_ADV_IND           (0)
#define BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD (1)
#define BLE_HCI_ADV_TYPE_ADV_SCAN_IND      (2)
#define BLE_HCI_ADV_TYPE_ADV_NONCONN_IND   (3)
#define BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD (4)
#define BLE_HCI_ADV_TYPE_MAX               (4)

#define BLE_HCI_ADV_CONN_MASK              (0x0001)
#define BLE_HCI_ADV_SCAN_MASK              (0x0002)
#define BLE_HCI_ADV_DIRECT_MASK            (0x0004)
#define BLE_HCI_ADV_SCAN_RSP_MASK          (0x0008)
#define BLE_HCI_ADV_LEGACY_MASK            (0x0010)

#define BLE_HCI_ADV_DATA_STATUS_COMPLETE   (0x0000)
#define BLE_HCI_ADV_DATA_STATUS_INCOMPLETE (0x0020)
#define BLE_HCI_ADV_DATA_STATUS_TRUNCATED  (0x0040)
#define BLE_HCI_ADV_DATA_STATUS_MASK       (0x0060)

/* Own address types */
#define BLE_HCI_ADV_OWN_ADDR_PUBLIC    (0)
#define BLE_HCI_ADV_OWN_ADDR_RANDOM    (1)
#define BLE_HCI_ADV_OWN_ADDR_PRIV_PUB  (2)
#define BLE_HCI_ADV_OWN_ADDR_PRIV_RAND (3)
#define BLE_HCI_ADV_OWN_ADDR_MAX       (3)

#define BLE_ADDR_TYPE_RANDOM_MASK      (1 << 0)
#define BLE_ADDR_TYPE_PRIV_MASK        (1 << 1)

/* Advertisement peer address Type */
#define BLE_HCI_ADV_PEER_ADDR_PUBLIC (0)
#define BLE_HCI_ADV_PEER_ADDR_RANDOM (1)
#define BLE_HCI_ADV_PEER_ADDR_MAX    (1)

/* --- LE advertising channel tx power (OCF 0x0007) */
#define BLE_HCI_ADV_CHAN_TXPWR_MIN (-20)
#define BLE_HCI_ADV_CHAN_TXPWR_MAX (10)

/* --- LE set scan enable (OCF 0x000c) */

/* Connect peer address type */
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC       (0)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM       (1)
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC_IDENT (2)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM_IDENT (3)
#define BLE_HCI_CONN_PEER_ADDR_MAX          (3)

/*
 * Advertising filter policy
 *
 * Determines how an advertiser filters scan and connection requests.
 *
 *  NONE: no filtering (default value). No whitelist used.
 *  SCAN: process all connection requests but only scans from white list.
 *  CONN: process all scan request but only connection requests from white list
 *  BOTH: ignore all scan and connection requests unless in white list.
 */
#define BLE_HCI_ADV_FILT_NONE (0)
#define BLE_HCI_ADV_FILT_SCAN (1)
#define BLE_HCI_ADV_FILT_CONN (2)
#define BLE_HCI_ADV_FILT_BOTH (3)
#define BLE_HCI_ADV_FILT_MAX  (3)

#define BLE_HCI_ADV_FILT_DEF  (BLE_HCI_ADV_FILT_NONE)

/* Advertising interval */
#define BLE_HCI_ADV_ITVL             (625)   /* usecs */
#define BLE_HCI_ADV_ITVL_MIN         (32)    /* units */
#define BLE_HCI_ADV_ITVL_MAX         (16384) /* units */
#define BLE_HCI_ADV_ITVL_NONCONN_MIN (160)   /* units */

#define BLE_HCI_ADV_ITVL_DEF         (0x800) /* 1.28 seconds */
#define BLE_HCI_ADV_CHANMASK_DEF     (0x7)   /* all channels */

#define BLE_HCI_PERIODIC_ADV_ITVL    (1250) /* usecs */

/* Set scan parameters */
#define BLE_HCI_SCAN_TYPE_PASSIVE (0)
#define BLE_HCI_SCAN_TYPE_ACTIVE  (1)

/* Scan interval and scan window timing */
#define BLE_HCI_SCAN_ITVL           (625)    /* usecs */
#define BLE_HCI_SCAN_ITVL_MIN       (0x0004) /* units */
#define BLE_HCI_SCAN_ITVL_MAX       (0x4000) /* units */
#define BLE_HCI_SCAN_ITVL_MAX_EXT   (0xffff) /* units */
#define BLE_HCI_SCAN_ITVL_DEF       (16)     /* units */
#define BLE_HCI_SCAN_WINDOW_MIN     (0x0004) /* units */
#define BLE_HCI_SCAN_WINDOW_MAX     (0x4000) /* units */
#define BLE_HCI_SCAN_WINDOW_MAX_EXT (0xffff) /* units */
#define BLE_HCI_SCAN_WINDOW_DEF     (16)     /* units */

/*
 * Scanning filter policy
 *  NO_WL:
 *      Scanner processes all advertising packets (white list not used) except
 *      directed, connectable advertising packets not sent to the scanner.
 *  USE_WL:
 *      Scanner processes advertisements from white list only. A connectable,
 *      directed advertisement is ignored unless it contains scanners address.
 *  NO_WL_INITA:
 *      Scanner process all advertising packets (white list not used). A
 *      connectable, directed advertisement shall not be ignored if the InitA
 *      is a resolvable private address.
 *  USE_WL_INITA:
 *      Scanner process advertisements from white list only. A connectable,
 *      directed advertisement shall not be ignored if the InitA is a
 *      resolvable private address.
 */
#define BLE_HCI_SCAN_FILT_NO_WL        (0)
#define BLE_HCI_SCAN_FILT_USE_WL       (1)
#define BLE_HCI_SCAN_FILT_NO_WL_INITA  (2)
#define BLE_HCI_SCAN_FILT_USE_WL_INITA (3)
#define BLE_HCI_SCAN_FILT_MAX          (3)

/* Whitelist commands */
#define BLE_HCI_ADD_WHITE_LIST_LEN (7)
#define BLE_HCI_RMV_WHITE_LIST_LEN (7)

/* Create Connection */
#define BLE_HCI_CREATE_CONN_LEN           (25)
#define BLE_HCI_CONN_ITVL                 (1250) /* usecs */
#define BLE_HCI_CONN_FILT_NO_WL           (0)
#define BLE_HCI_CONN_FILT_USE_WL          (1)
#define BLE_HCI_CONN_FILT_MAX             (1)
#define BLE_HCI_CONN_ITVL_MIN             (0x0006)
#define BLE_HCI_CONN_ITVL_MAX             (0x0c80)
#define BLE_HCI_CONN_LATENCY_MIN          (0x0000)
#define BLE_HCI_CONN_LATENCY_MAX          (0x01f3)
#define BLE_HCI_CONN_SPVN_TIMEOUT_MIN     (0x000a)
#define BLE_HCI_CONN_SPVN_TIMEOUT_MAX     (0x0c80)
#define BLE_HCI_CONN_SPVN_TMO_UNITS       (10) /* msecs */
#define BLE_HCI_INITIATOR_FILT_POLICY_MAX (1)

/* Peer Address Type */
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC  (0)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM  (1)
#define BLE_HCI_CONN_PEER_ADDR_PUB_ID  (2)
#define BLE_HCI_CONN_PEER_ADDR_RAND_ID (3)
#define BLE_HCI_CONN_PEER_ADDR_MAX     (3)


/* --- LE set data length (OCF 0x0022) */
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MIN (0x001b)
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MAX (0x00fb)
#define BLE_HCI_SET_DATALEN_TX_TIME_MIN   (0x0148)
#define BLE_HCI_SET_DATALEN_TX_TIME_MAX   (0x4290)

/* --- LE read/write suggested default data length (OCF 0x0023 and 0x0024) */
#define BLE_HCI_SUGG_DEF_DATALEN_TX_OCTETS_MIN (0x001b)
#define BLE_HCI_SUGG_DEF_DATALEN_TX_OCTETS_MAX (0x00fb)
#define BLE_HCI_SUGG_DEF_DATALEN_TX_TIME_MIN   (0x0148)
#define BLE_HCI_SUGG_DEF_DATALEN_TX_TIME_MAX   (0x4290)

/* --- LE read maximum default PHY (OCF 0x0030) */
#define BLE_HCI_LE_PHY_1M    (1)
#define BLE_HCI_LE_PHY_2M    (2)
#define BLE_HCI_LE_PHY_CODED (3)

/* --- LE set default PHY (OCF 0x0031) */
#define BLE_HCI_LE_PHY_NO_TX_PREF_MASK (0x01)
#define BLE_HCI_LE_PHY_NO_RX_PREF_MASK (0x02)
#define BLE_HCI_LE_PHY_1M_PREF_MASK    (0x01)
#define BLE_HCI_LE_PHY_2M_PREF_MASK    (0x02)
#define BLE_HCI_LE_PHY_CODED_PREF_MASK (0x04)

#define BLE_HCI_LE_PHY_PREF_MASK_ALL   (BLE_HCI_LE_PHY_1M_PREF_MASK | BLE_HCI_LE_PHY_2M_PREF_MASK | BLE_HCI_LE_PHY_CODED_PREF_MASK)

/* --- LE set PHY (OCF 0x0032) */
#define BLE_HCI_LE_PHY_CODED_ANY     (0x0000)
#define BLE_HCI_LE_PHY_CODED_S2_PREF (0x0001)
#define BLE_HCI_LE_PHY_CODED_S8_PREF (0x0002)

/* --- LE enhanced receiver test (OCF 0x0033) */
#define BLE_HCI_LE_PHY_1M    (1)
#define BLE_HCI_LE_PHY_2M    (2)
#define BLE_HCI_LE_PHY_CODED (3)

/* --- LE enhanced transmitter test (OCF 0x0034) */
#define BLE_HCI_LE_PHY_CODED_S8 (3)
#define BLE_HCI_LE_PHY_CODED_S2 (4)

/* --- LE set extended advertising parameters (OCF 0x0036) */
#define BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE    (0x0001)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE      (0x0002)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED       (0x0004)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED    (0x0008)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY         (0x0010)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV       (0x0020)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR     (0x0040)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_MASK           (0x7F)

#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_IND     (0x0013)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_LD_DIR  (0x0015)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_HD_DIR  (0x001d)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_SCAN    (0x0012)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_NONCONN (0x0010)

/* --- LE set extended advertising data (OCF 0x0037) */
#define BLE_HCI_MAX_EXT_ADV_DATA_LEN       (251)

#define BLE_HCI_LE_SET_DATA_OPER_INT       (0)
#define BLE_HCI_LE_SET_DATA_OPER_FIRST     (1)
#define BLE_HCI_LE_SET_DATA_OPER_LAST      (2)
#define BLE_HCI_LE_SET_DATA_OPER_COMPLETE  (3)
#define BLE_HCI_LE_SET_DATA_OPER_UNCHANGED (4)

/* --- LE set extended scan response data (OCF 0x0038) */
#define BLE_HCI_MAX_EXT_SCAN_RSP_DATA_LEN (251)

/* --- LE set periodic advertising parameters (OCF 0x003E) */
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR (0x0040)
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_MASK       (0x0040)

/* --- LE set periodic advertising data (OCF 0x003F) */
#define BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN (252)

/* --- LE remove device from periodic advertising list (OCF 0x0048) */
#define BLE_HCI_PERIODIC_DATA_STATUS_COMPLETE   0x00
#define BLE_HCI_PERIODIC_DATA_STATUS_INCOMPLETE 0x01
#define BLE_HCI_PERIODIC_DATA_STATUS_TRUNCATED  0x02

/* --- LE set privacy mode (OCF 0x004E) */
#define BLE_HCI_PRIVACY_NETWORK (0)
#define BLE_HCI_PRIVACY_DEVICE  (1)


/******************************************************************************
 * HCI command
 ******************************************************************************/

/* Set opcode based on OCF and OGF */
#define BLE_HCI_OP(ogf, ocf) ((ocf) | ((ogf) << 10))

/* Get the OGF and OCF from the opcode in the command */
#define BLE_HCI_OGF(opcode) (((opcode) >> 10) & 0x003F)
#define BLE_HCI_OCF(opcode) ((opcode) & 0x03FF)

/* Opcode Group definitions (note: 0x07 not defined in spec) */
#define BLE_HCI_OGF_LINK_CTRL     (0x01)
#define BLE_HCI_OGF_LINK_POLICY   (0x02) //all BT used HCI command
#define BLE_HCI_OGF_CTLR_BASEBAND (0x03)
#define BLE_HCI_OGF_INFO_PARAMS   (0x04)
#define BLE_HCI_OGF_STATUS_PARAMS (0x05)
#define BLE_HCI_OGF_TESTING       (0x06)
#define BLE_HCI_OGF_LE            (0x08)
#define BLE_HCI_OGF_VENDOR        (0x3F)

/* List of OCF for Link Control commands (OGF=0x01) */
#define BLE_HCI_OCF_DISCONNECT_CMD (0x0006)

struct ble_hci_lc_disconnect_cp
{
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));

#define BLE_HCI_OCF_RD_REM_VER_INFO (0x001D)

struct ble_hci_rd_rem_ver_info_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

/* List of OCF for Controller and Baseband commands (OGF=0x03) */
#define BLE_HCI_OCF_CB_SET_EVENT_MASK (0x0001)

struct ble_hci_cb_set_event_mask_cp
{
    uint64_t event_mask;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_RESET       (0x0003)

#define BLE_HCI_OCF_CB_READ_TX_PWR (0x002D)

struct ble_hci_cb_read_tx_pwr_cp
{
    uint16_t conn_handle;
    uint8_t  type;
} __attribute__((packed));

struct ble_hci_cb_read_tx_pwr_rp
{
    uint16_t conn_handle;
    int8_t   tx_level;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC (0x0031)

struct ble_hci_cb_ctlr_to_host_fc_cp
{
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_HOST_BUF_SIZE (0x0033)

struct ble_hci_cb_host_buf_size_cp
{
    uint16_t acl_data_len;
    uint8_t  sco_data_len;
    uint16_t acl_num;
    uint16_t sco_num;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_HOST_NUM_COMP_PKTS (0x0035)

struct ble_hci_cb_host_num_comp_pkts_entry
{
    uint16_t handle;
    uint16_t count;
} __attribute__((packed));

struct ble_hci_cb_host_num_comp_pkts_cp
{
    uint8_t                                    handles;
    struct ble_hci_cb_host_num_comp_pkts_entry h[1];
} __attribute__((packed));

#define BLE_HCI_OCF_CB_SET_EVENT_MASK2 (0x0063)

struct ble_hci_cb_set_event_mask2_cp
{
    uint64_t event_mask2;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_RD_AUTH_PYLD_TMO (0x007B)

struct ble_hci_cb_rd_auth_pyld_tmo_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_cb_rd_auth_pyld_tmo_rp
{
    uint16_t conn_handle;
    uint16_t tmo;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_WR_AUTH_PYLD_TMO (0x007C)

struct ble_hci_cb_wr_auth_pyld_tmo_cp
{
    uint16_t conn_handle;
    uint16_t tmo;
} __attribute__((packed));

struct ble_hci_cb_wr_auth_pyld_tmo_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

/* List of OCF for Info Param commands (OGF=0x04) */
#define BLE_HCI_OCF_IP_RD_LOCAL_VER (0x0001)

struct ble_hci_ip_rd_local_ver_rp
{
    uint8_t  hci_ver;
    uint16_t hci_rev;
    uint8_t  lmp_ver;
    uint16_t manufacturer;
    uint16_t lmp_subver;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_LOC_SUPP_CMD (0x0002)

struct ble_hci_ip_rd_loc_supp_cmd_rp
{
    uint8_t commands[64];
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT (0x0003)

struct ble_hci_ip_rd_loc_supp_feat_rp
{
    uint64_t features;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_BUF_SIZE (0x0005)

struct ble_hci_ip_rd_buf_size_rp
{
    uint16_t acl_data_len;
    uint8_t  sco_data_len;
    uint16_t acl_num;
    uint16_t sco_num;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_BD_ADDR (0x0009)

struct ble_hci_ip_rd_bd_addr_rp
{
    uint8_t addr[6];
} __attribute__((packed));

/* List of OCF for Status parameters commands (OGF = 0x05) */
#define BLE_HCI_OCF_RD_RSSI (0x0005)

struct ble_hci_rd_rssi_cp
{
    uint16_t handle;
} __attribute__((packed));

struct ble_hci_rd_rssi_rp
{
    uint16_t handle;
    int8_t   rssi;
} __attribute__((packed));

/*
 * Number of LE commands. NOTE: this is really just used to size the array
 * containing the lengths of the LE commands.
 */
#define BLE_HCI_NUM_LE_CMDS (79) //TODO: update this number

/* List of OCF for LE commands (OGF = 0x08) */
#define BLE_HCI_OCF_LE_SET_EVENT_MASK (0x0001)

struct ble_hci_le_set_event_mask_cp
{
    uint64_t event_mask;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_BUF_SIZE (0x0002)

struct ble_hci_le_rd_buf_size_rp
{
    uint16_t data_len;
    uint8_t  data_packets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_BUF_SIZE_V2 (0x0060)

struct ble_hci_le_rd_buf_size_v2_rp
{
    uint16_t data_len;
    uint8_t  data_packets;
    uint16_t iso_data_len;
    uint8_t  iso_data_packets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT (0x0003)

struct ble_hci_le_rd_loc_supp_feat_rp
{
    uint64_t features;
} __attribute__((packed));

/* NOTE: 0x0004 is intentionally left undefined */
#define BLE_HCI_OCF_LE_SET_RAND_ADDR (0x0005)

struct ble_hci_le_set_rand_addr_cp
{
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_PARAMS (0x0006)

struct ble_hci_le_set_adv_params_cp
{
    uint16_t min_interval;
    uint16_t max_interval;
    uint8_t  type;
    uint8_t  own_addr_type;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  chan_map;
    uint8_t  filter_policy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR (0x0007)

struct ble_hci_le_rd_adv_chan_txpwr_rp
{
    int8_t power_level;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_DATA (0x0008)
#define BLE_HCI_MAX_ADV_DATA_LEN    (31)

struct ble_hci_le_set_adv_data_cp
{
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA (0x0009)
#define BLE_HCI_MAX_SCAN_RSP_DATA_LEN    (31)

struct ble_hci_le_set_scan_rsp_data_cp
{
    uint8_t scan_rsp_len;
    uint8_t scan_rsp[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_ENABLE (0x000A)

struct ble_hci_le_set_adv_enable_cp
{
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_PARAMS (0x000B)

struct ble_hci_le_set_scan_params_cp
{
    uint8_t  scan_type;
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t  own_addr_type;
    uint8_t  filter_policy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_ENABLE (0x000C)

struct ble_hci_le_set_scan_enable_cp
{
    uint8_t enable;
    uint8_t filter_duplicates;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_CONN (0x000D)

struct ble_hci_le_create_conn_cp
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t  filter_policy;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  own_addr_type;
    uint16_t min_conn_itvl;
    uint16_t max_conn_itvl;
    uint16_t conn_latency;
    uint16_t tmo;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_CONN_CANCEL (0x000E)

#define BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE (0x000F)

struct ble_hci_le_rd_white_list_rp
{
    uint8_t size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_WHITE_LIST (0x0010)

#define BLE_HCI_OCF_LE_ADD_WHITE_LIST   (0x0011)

struct ble_hci_le_add_white_list_cp
{
    uint8_t addr_type;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RMV_WHITE_LIST (0x0012)

struct ble_hci_le_rmv_white_list_cp
{
    uint8_t addr_type;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CONN_UPDATE (0x0013)

struct ble_hci_le_conn_update_cp
{
    uint16_t conn_handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS (0x0014)

struct ble_hci_le_set_host_chan_class_cp
{
    uint8_t chan_map[5];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_CHAN_MAP (0x0015)

struct ble_hci_le_rd_chan_map_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_rd_chan_map_rp
{
    uint16_t conn_handle;
    uint8_t  chan_map[5];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_REM_FEAT (0x0016)

struct ble_hci_le_rd_rem_feat_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ENCRYPT (0x0017)

struct ble_hci_le_encrypt_cp
{
    uint8_t key[16];
    uint8_t data[16];
} __attribute__((packed));

struct ble_hci_le_encrypt_rp
{
    uint8_t data[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RAND (0x0018)

struct ble_hci_le_rand_rp
{
    uint64_t random_number;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_START_ENCRYPT (0x0019)

struct ble_hci_le_start_encrypt_cp
{
    uint16_t conn_handle;
    uint64_t rand;
    uint16_t div;
    uint8_t  ltk[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY (0x001A)

struct ble_hci_le_lt_key_req_reply_cp
{
    uint16_t conn_handle;
    uint8_t  ltk[16];
} __attribute__((packed));

struct ble_hci_le_lt_key_req_reply_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY (0x001B)

struct ble_hci_le_lt_key_req_neg_reply_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_lt_key_req_neg_reply_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_SUPP_STATES (0x001C)

struct ble_hci_le_rd_supp_states_rp
{
    uint64_t states;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST (0x001D)

struct ble_hci_le_rx_test_cp
{
    uint8_t rx_chan;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TX_TEST (0x001E)

struct ble_hci_le_tx_test_cp
{
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
} __attribute__((packed));

struct ble_hci_le_tx_test_ext_cp
{
    uint8_t  tx_chan;
    uint8_t  test_data_len;
    uint8_t  payload;
    uint16_t interval;
    uint16_t pkt_count;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TEST_END (0x001F)

struct ble_hci_le_test_end_rp
{
    uint16_t num_packets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_CONN_PARAM_RR (0x0020)

struct ble_hci_le_rem_conn_param_rr_cp
{
    uint16_t conn_handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));

struct ble_hci_le_rem_conn_param_rr_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR (0x0021)

struct ble_hci_le_rem_conn_params_nrr_cp
{
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));

struct ble_hci_le_rem_conn_params_nrr_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DATA_LEN (0x0022)

struct ble_hci_le_set_data_len_cp
{
    uint16_t conn_handle;
    uint16_t tx_octets;
    uint16_t tx_time;
} __attribute__((packed));

struct ble_hci_le_set_data_len_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN (0x0023)

struct ble_hci_le_rd_sugg_def_data_len_rp
{
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN (0x0024)

struct ble_hci_le_wr_sugg_def_data_len_cp
{
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_P256_PUBKEY (0x0025)

#define BLE_HCI_OCF_LE_GEN_DHKEY      (0x0026)

struct ble_hci_le_gen_dhkey_cp
{
    uint8_t pkey[64];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ADD_RESOLV_LIST (0x0027)

struct ble_hci_le_add_resolv_list_cp
{
    uint8_t peer_id_addr_type;
    uint8_t peer_id_addr[6];
    uint8_t peer_irk[16];
    uint8_t local_irk[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RMV_RESOLV_LIST (0x0028)

struct ble_hci_le_rmv_resolve_list_cp
{
    uint8_t peer_id_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLR_RESOLV_LIST     (0x0029)

#define BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE (0x002A)

struct ble_hci_le_rd_resolv_list_size_rp
{
    uint8_t size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR (0x002B)

struct ble_hci_le_rd_peer_resolv_addr_cp
{
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));

struct ble_hci_le_rd_peer_resolv_addr_rp
{
    uint8_t rpa[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR (0x002C)

struct ble_hci_le_rd_local_resolv_addr_cp
{
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));

struct ble_hci_le_rd_local_resolv_addr_rp
{
    uint8_t rpa[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADDR_RES_EN (0x002D)

struct ble_hci_le_set_addr_res_en_cp
{
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_RPA_TMO (0x002E)

struct ble_hci_le_set_rpa_tmo_cp
{
    uint16_t rpa_timeout;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_MAX_DATA_LEN (0x002F)

struct ble_hci_le_rd_max_data_len_rp
{
    uint16_t max_tx_octests;
    uint16_t max_tx_time;
    uint16_t max_rx_octests;
    uint16_t max_rx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_PHY (0x0030)

struct ble_hci_le_rd_phy_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_rd_phy_rp
{
    uint16_t conn_handle;
    uint8_t  tx_phy;
    uint8_t  rx_phy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DEFAULT_PHY (0x0031)

struct ble_hci_le_set_default_phy_cp
{
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PHY (0x0032)

struct ble_hci_le_set_phy_cp
{
    uint16_t conn_handle;
    uint8_t  all_phys;
    uint8_t  tx_phys;
    uint8_t  rx_phys;
    uint16_t phy_options;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST_V2 (0x0033)

struct ble_hci_le_rx_test_v2_cp
{
    uint8_t rx_chan;
    uint8_t phy;
    uint8_t index;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TX_TEST_V2 (0x0034)

struct ble_hci_le_tx_test_v2_cp
{
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
    uint8_t phy;
} __attribute__((packed));

struct ble_hci_le_tx_test_v2_ext_cp
{
    uint8_t  tx_chan;
    uint8_t  test_data_len;
    uint8_t  payload;
    uint8_t  phy;
    uint16_t interval;
    uint16_t pkt_count;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR (0x0035)

struct ble_hci_le_set_adv_set_rnd_addr_cp
{
    uint8_t adv_handle;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM (0x0036)

struct ble_hci_le_set_ext_adv_params_cp
{
    uint8_t  adv_handle;
    uint16_t props;
    uint8_t  pri_itvl_min[3];
    uint8_t  pri_itvl_max[3];
    uint8_t  pri_chan_map;
    uint8_t  own_addr_type;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  filter_policy;
    int8_t   tx_power;
    uint8_t  pri_phy;
    uint8_t  sec_max_skip;
    uint8_t  sec_phy;
    uint8_t  sid;
    uint8_t  scan_req_notif;
} __attribute__((packed));

struct ble_hci_le_set_ext_adv_params_rp
{
    int8_t tx_power;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_DATA (0x0037)

struct ble_hci_le_set_ext_adv_data_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA (0x0038)

struct ble_hci_le_set_ext_scan_rsp_data_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t scan_rsp_len;
    uint8_t scan_rsp[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE (0x0039)

struct adv_set
{
    uint8_t  adv_handle;
    uint16_t duration;
    uint8_t  max_events;
} __attribute__((packed));

struct ble_hci_le_set_ext_adv_enable_cp
{
    uint8_t        enable;
    uint8_t        num_sets;
    struct adv_set sets[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN (0x003A)

struct ble_hci_le_rd_max_adv_data_len_rp
{
    uint16_t max_adv_data_len;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS (0x003B)

struct ble_hci_le_rd_num_of_adv_sets_rp
{
    uint8_t num_sets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REMOVE_ADV_SET (0x003C)

struct ble_hci_le_remove_adv_set_cp
{
    uint8_t adv_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_ADV_SETS          (0x003D)

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS (0x003E)

struct ble_hci_le_set_periodic_adv_params_cp
{
    uint8_t  adv_handle;
    uint16_t min_itvl;
    uint16_t max_itvl;
    uint16_t props;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA (0x003F)

struct ble_hci_le_set_periodic_adv_data_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE (0x0040)

struct ble_hci_le_set_periodic_adv_enable_cp
{
    uint8_t enable;
    uint8_t adv_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM (0x0041)

struct scan_params
{
    uint8_t  type;
    uint16_t itvl;
    uint16_t window;
} __attribute__((packed));

struct ble_hci_le_set_ext_scan_params_cp
{
    uint8_t            own_addr_type;
    uint8_t            filter_policy;
    uint8_t            phys;
    struct scan_params scans[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE (0x0042)

struct ble_hci_le_set_ext_scan_enable_cp
{
    uint8_t  enable;
    uint8_t  filter_dup;
    uint16_t duration;
    uint16_t period;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_EXT_CREATE_CONN (0x0043)

struct conn_params
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint16_t conn_min_itvl;
    uint16_t conn_max_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));

struct ble_hci_le_ext_create_conn_cp
{
    uint8_t            filter_policy;
    uint8_t            own_addr_type;
    uint8_t            peer_addr_type;
    uint8_t            peer_addr[6];
    uint8_t            init_phy_mask;
    struct conn_params conn_params[0];
} __attribute__((packed));

#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_OPT_FILTER     0x01
#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_OPT_DISABLED   0x02
#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_OPT_DUPLICATES 0x04

#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC            (0x0044)

struct ble_hci_le_periodic_adv_create_sync_cp
{
    uint8_t  options;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL (0x0045)

#define BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC          (0x0046)

struct ble_hci_le_periodic_adv_term_sync_cp
{
    uint16_t sync_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST (0x0047)

struct ble_hci_le_add_dev_to_periodic_adv_list_cp
{
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t sid;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST (0x0048)

struct ble_hci_le_rem_dev_from_periodic_adv_list_cp
{
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t sid;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST   (0x0049)

#define BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE (0x004A)

struct ble_hci_le_rd_periodic_adv_list_size_rp
{
    uint8_t list_size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_TRANSMIT_POWER (0x004B)

struct ble_hci_le_rd_transmit_power_rp
{
    int8_t min_tx_power;
    int8_t max_tx_power;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION (0x004C)

struct ble_hci_le_rd_rf_path_compensation_rp
{
    int16_t tx_path_compensation;
    int16_t rx_path_compensation;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION (0x004D)

struct ble_hci_le_wr_rf_path_compensation_cp
{
    int16_t tx_path_compensation;
    int16_t rx_path_compensation;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PRIVACY_MODE (0x004E)

struct ble_hci_le_set_privacy_mode_cp
{
    uint8_t peer_id_addr_type;
    uint8_t peer_id_addr[6];
    uint8_t mode;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST_V3                      (0x004F)
#define BLE_HCI_OCF_LE_TX_TEST_V3                      (0x0050)
#define BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_PARAMS      (0x0051)
#define BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_ENABLE      (0x0052)
#define BLE_HCI_OCF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE (0x0053)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_RX_PARAMS          (0x0054)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_TX_PARAMS          (0x0055)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_REQ_ENABLE         (0x0056)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_RESP_ENABLE        (0x0057)
#define BLE_HCI_OCF_LE_RD_ANTENNA_INFO                 (0x0058)

#define BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE     (0x0059)

struct ble_hci_le_periodic_adv_receive_enable_cp
{
    uint16_t sync_handle;
    uint8_t  enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER (0x005A)

struct ble_hci_le_periodic_adv_sync_transfer_cp
{
    uint16_t conn_handle;
    uint16_t service_data;
    uint16_t sync_handle;
} __attribute__((packed));

struct ble_hci_le_periodic_adv_sync_transfer_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER (0x005B)

struct ble_hci_le_periodic_adv_set_info_transfer_cp
{
    uint16_t conn_handle;
    uint16_t service_data;
    uint8_t  adv_handle;
} __attribute__((packed));

struct ble_hci_le_periodic_adv_set_info_transfer_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS (0x005C)

struct ble_hci_le_periodic_adv_sync_transfer_params_cp
{
    uint16_t conn_handle;
    uint8_t  mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));

struct ble_hci_le_periodic_adv_sync_transfer_params_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS (0x005D)

struct ble_hci_le_set_default_periodic_sync_transfer_params_cp
{
    uint8_t  mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_GENERATE_DHKEY_V2 (0x005E)
#define BLE_HCI_OCF_LE_MODIFY_SCA        (0x005F)

#define BLE_HCI_OCF_LE_READ_ISO_TX_SYNC  (0x0061)

struct ble_hci_le_read_iso_tx_sync_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_read_iso_tx_sync_rp
{
    uint16_t conn_handle;
    uint16_t packet_seq_num;
    uint32_t tx_timestamp;
    uint8_t  time_offset[3];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_CIG_PARAMS (0x0062)

struct ble_hci_le_cis_params
{
    uint8_t  cis_id;
    uint16_t max_sdu_c_to_p;
    uint16_t max_sdu_p_to_c;
    uint8_t  phy_c_to_p;
    uint8_t  phy_p_to_c;
    uint8_t  rnt_c_to_p;
    uint8_t  rnt_p_to_c;
} __attribute__((packed));

struct ble_hci_le_set_cig_params_cp
{
    uint8_t                      cig_id;
    uint8_t                      sdu_interval_c_to_p[3];
    uint8_t                      sdu_interval_p_to_c[3];
    uint8_t                      worst_sca;
    uint8_t                      packing;
    uint8_t                      framing;
    uint16_t                     max_latency_c_to_p;
    uint16_t                     max_latency_p_to_c;
    uint8_t                      cis_count;
    struct ble_hci_le_cis_params cis[0];
} __attribute__((packed));

struct ble_hci_le_set_cig_params_rp
{
    uint8_t  cig_id;
    uint8_t  cis_count;
    uint16_t conn_handle[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_CIG_PARAMS_TEST (0x0063)

struct ble_hci_le_cis_params_test
{
    uint8_t  cis_id;
    uint8_t  nse;
    uint16_t max_sdu_c_to_p;
    uint16_t max_sdu_p_to_c;
    uint16_t max_pdu_c_to_p;
    uint16_t max_pdu_p_to_c;
    uint8_t  phy_c_to_p;
    uint8_t  phy_p_to_c;
    uint8_t  bn_c_to_p;
    uint8_t  bn_p_to_c;
} __attribute__((packed));

struct ble_hci_le_set_cig_params_test_cp
{
    uint8_t                           cig_id;
    uint8_t                           sdu_interval_c_to_p[3];
    uint8_t                           sdu_interval_p_to_c[3];
    uint8_t                           ft_c_to_p;
    uint8_t                           ft_p_to_c;
    uint16_t                          iso_interval;
    uint8_t                           worst_sca;
    uint8_t                           packing;
    uint8_t                           framing;
    uint8_t                           cis_count;
    struct ble_hci_le_cis_params_test cis[0];
} __attribute__((packed));

struct ble_hci_le_set_cig_params_test_rp
{
    uint8_t  cig_id;
    uint8_t  cis_count;
    uint16_t conn_handle[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_CIS (0x0064)

struct ble_hci_le_create_cis_params
{
    uint16_t cis_handle;
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_create_cis_cp
{
    uint8_t                             cis_count;
    struct ble_hci_le_create_cis_params cis[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REMOVE_CIG (0x0065)

struct ble_hci_le_remove_cig_cp
{
    uint8_t cig_id;
} __attribute__((packed));

struct ble_hci_le_remove_cig_rp
{
    uint8_t cig_id;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ACCEPT_CIS_REQ (0x0066)

struct ble_hci_le_accept_cis_request_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REJECT_CIS_REQ (0x0067)

struct ble_hci_le_reject_cis_request_cp
{
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));

struct ble_hci_le_reject_cis_request_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_BIG (0x0068)

struct ble_hci_le_create_big_cp
{
    uint8_t  big_handle;
    uint8_t  adv_handle;
    uint8_t  num_bis;
    uint8_t  sdu_interval[3];
    uint16_t max_sdu;
    uint16_t max_transport_latency;
    uint8_t  rtn;
    uint8_t  phy;
    uint8_t  packing;
    uint8_t  framing;
    uint8_t  encryption;
    uint8_t  broadcast_code[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_BIG_TEST (0x0069)

struct ble_hci_le_create_big_test_cp
{
    uint8_t  big_handle;
    uint8_t  adv_handle;
    uint8_t  num_bis;
    uint8_t  sdu_interval[3];
    uint16_t iso_interval;
    uint8_t  nse;
    uint16_t max_sdu;
    uint16_t max_pdu;
    uint8_t  phy;
    uint8_t  packing;
    uint8_t  framing;
    uint8_t  bn;
    uint8_t  irc;
    uint8_t  pto;
    uint8_t  encryption;
    uint8_t  broadcast_code[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TERMINATE_BIG (0x006a)

struct ble_hci_le_terminate_big_cp
{
    uint8_t big_handle;
    uint8_t reason;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_BIG_CREATE_SYNC (0x006b)

struct ble_hci_le_big_create_sync_cp
{
    uint8_t  big_handle;
    uint16_t sync_handle;
    uint8_t  encryption;
    uint8_t  broadcast_code[16];
    uint8_t  mse;
    uint16_t sync_timeout;
    uint8_t  num_bis;
    uint8_t  bis[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_BIG_TERMINATE_SYNC (0x006c)

struct ble_hci_le_big_terminate_sync_cp
{
    uint8_t big_handle;
} __attribute__((packed));

struct ble_hci_le_big_terminate_sync_rp
{
    uint8_t big_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REQ_PEER_SCA (0x006d)

struct ble_hci_le_request_peer_sca_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SETUP_ISO_DATA_PATH (0x006e)

struct ble_hci_le_setup_iso_data_path_cp
{
    uint16_t conn_handle;
    uint8_t  data_path_dir;
    uint8_t  data_path_id;
    uint8_t  codec_id[5];
    uint8_t  controller_delay[3];
    uint8_t  codec_config_len;
    uint8_t  codec_config[0];
} __attribute__((packed));

struct ble_hci_le_setup_iso_data_path_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REMOVE_ISO_DATA_PATH (0x006f)

struct ble_hci_le_remove_iso_data_path_cp
{
    uint16_t conn_handle;
    uint8_t  data_path_dir;
} __attribute__((packed));

struct ble_hci_le_remove_iso_data_path_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ISO_TRANSMIT_TEST (0x0070)

struct ble_hci_le_iso_transmit_test_cp
{
    uint16_t conn_handle;
    uint8_t  payload_type;
} __attribute__((packed));

struct ble_hci_le_iso_transmit_test_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ISO_RECEIVE_TEST (0x0071)

struct ble_hci_le_iso_receive_test_cp
{
    uint16_t conn_handle;
    uint8_t  payload_type;
} __attribute__((packed));

struct ble_hci_le_iso_receive_test_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ISO_READ_TEST_COUNTERS (0x0072)

struct ble_hci_le_iso_read_test_counters_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_iso_read_test_counters_rp
{
    uint16_t conn_handle;
    uint32_t received_sdu_count;
    uint32_t missed_sdu_count;
    uint32_t failed_sdu_count;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ISO_TEST_END (0x0073)

struct ble_hci_le_iso_test_end_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_iso_test_end_rp
{
    uint16_t conn_handle;
    uint32_t received_sdu_count;
    uint32_t missed_sdu_count;
    uint32_t failed_sdu_count;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_HOST_FEATURE (0x0074)

struct ble_hci_le_set_host_feature_cp
{
    uint8_t bit_num;
    uint8_t bit_val;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_READ_ISO_LINK_QUALITY (0x0075)

struct ble_hci_le_read_iso_link_quality_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

struct ble_hci_le_read_iso_link_quality_rp
{
    uint16_t conn_handle;
    uint32_t tx_unacked_pkts;
    uint32_t tx_flushed_pkts;
    uint32_t tx_last_subevent_pkts;
    uint32_t retransmitted_pkts;
    uint32_t crc_error_pkts;
    uint32_t rx_unreceived_pkts;
    uint32_t duplicate_pkts;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ENH_READ_TRANSMIT_POWER_LEVEL (0x0076)

struct ble_hci_le_enh_read_transmit_power_level_cp
{
    uint16_t conn_handle;
    uint8_t  phy;
} __attribute__((packed));

struct ble_hci_le_enh_read_transmit_power_level_rp
{
    uint16_t conn_handle;
    uint8_t  phy;
    uint8_t  curr_tx_power_level;
    uint8_t  max_tx_power_level;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL (0x0077)

struct ble_hci_le_read_remote_transmit_power_level_cp
{
    uint16_t conn_handle;
    uint8_t  phy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_PARAM (0x0078)

struct ble_hci_le_set_path_loss_report_param_cp
{
    uint16_t conn_handle;
    uint8_t  high_threshold;
    uint8_t  high_hysteresis;
    uint8_t  low_threshold;
    uint8_t  low_hysteresis;
    uint16_t min_time_spent;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_ENABLE (0x0079)

struct ble_hci_le_set_path_loss_report_enable_cp
{
    uint16_t conn_handle;
    uint8_t  enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_TRANS_PWR_REPORT_ENABLE (0x007A)

struct ble_hci_le_set_transmit_power_report_enable_cp
{
    uint16_t conn_handle;
    uint8_t  local_enable;
    uint8_t  remote_enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DEFAULT_SUBRATE (0x007D)

struct ble_hci_le_set_default_subrate_cp
{
    uint16_t subrate_min;
    uint16_t subrate_max;
    uint16_t max_latency;
    uint16_t cont_num;
    uint16_t supervision_tmo;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SUBRATE_REQ (0x007E)

struct ble_hci_le_subrate_req_cp
{
    uint16_t conn_handle;
    uint16_t subrate_min;
    uint16_t subrate_max;
    uint16_t max_latency;
    uint16_t cont_num;
    uint16_t supervision_tmo;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_ALL_LOC_SUPP_FEAT (0x0087)

struct ble_hci_le_rd_all_loc_supp_feat_rp
{
    uint8_t max_page;
    uint8_t features[248];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_RD_LOC_SUPP_CAP (0x0089)

struct ble_hci_le_cs_rd_loc_supp_cap_rp
{
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

#define BLE_HCI_OCF_LE_CS_RD_REM_SUPP_CAP (0x008A)

struct ble_hci_le_cs_rd_rem_supp_cap_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_WR_CACHED_REM_SUPP_CAP (0x008B)

struct ble_hci_le_cs_wr_cached_rem_supp_cap_cp
{
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

struct ble_hci_le_cs_wr_cached_rem_supp_cap_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_SEC_ENABLE (0x008C)

struct ble_hci_le_cs_sec_enable_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_SET_DEF_SETTINGS (0x008D)

struct ble_hci_le_cs_set_def_settings_cp
{
    uint16_t conn_handle;
    uint8_t  role_enable;
    uint8_t  cs_sync_antenna_selection;
    uint8_t  max_tx_power;
} __attribute__((packed));

struct ble_hci_le_cs_set_def_settings_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_RD_REM_FAE (0x008E)

struct ble_hci_le_cs_rd_rem_fae_cp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_WR_CACHED_REM_FAE (0x008F)

struct ble_hci_le_cs_wr_cached_rem_fae_cp
{
    uint16_t conn_handle;
    uint8_t  remote_fae_table[72];
} __attribute__((packed));

struct ble_hci_le_cs_wr_cached_rem_fae_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_CREATE_CONFIG (0x0090)

struct ble_hci_le_cs_create_config_cp
{
    uint16_t conn_handle;
    uint8_t  config_id;
    uint8_t  create_context;
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
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_REMOVE_CONFIG (0x0091)

struct ble_hci_le_cs_remove_config_cp
{
    uint16_t conn_handle;
    uint8_t  config_id;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_SET_CHAN_CLASS (0x0092)

struct ble_hci_le_cs_set_chan_class_cp
{
    uint8_t channel_classification[10];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_SET_PROC_PARAMS (0x0093)

struct ble_hci_le_cs_set_proc_params_cp
{
    uint16_t conn_handle;
    uint8_t  config_id;
    uint16_t max_procedure_len;
    uint16_t min_procedure_interval;
    uint16_t max_procedure_interval;
    uint16_t max_procedure_count;
    uint8_t  min_subevent_len[3];
    uint8_t  max_subevent_len[3];
    uint8_t  tone_antenna_config_selection;
    uint8_t  phy;
    uint8_t  tx_power_delta;
    uint8_t  preferred_peer_antenna;
    uint8_t  snr_control_initiator;
    uint8_t  snr_control_reflector;
} __attribute__((packed));

struct ble_hci_le_cs_set_proc_params_rp
{
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_PROC_ENABLE (0x0094)

struct ble_hci_le_cs_proc_enable_cp
{
    uint16_t conn_handle;
    uint8_t  config_id;
    uint8_t  enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_TEST (0x0095)

struct ble_hci_le_cs_test_cp
{
    uint8_t  main_mode_type;
    uint8_t  sub_mode_type;
    uint8_t  main_mode_repetition;
    uint8_t  mode_0_steps;
    uint8_t  role;
    uint8_t  rtt_type;
    uint8_t  cs_sync_phy;
    uint8_t  cs_sync_antenna_selection;
    uint8_t  subevent_len[3];
    uint16_t subevent_interval;
    uint8_t  transmit_power_level;
    uint8_t  t_ip1_time;
    uint8_t  t_ip2_time;
    uint8_t  t_fcs_time;
    uint8_t  t_pm_time;
    uint8_t  t_sw_time;
    uint8_t  tone_antenna_config_selection;
    uint8_t  companion_signal_enable;
    uint16_t drbg_nonce;
    uint16_t override_config;
    uint8_t  override_parameters_length;
    uint8_t  override_parameters_data[];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CS_TEST_END         (0x0096)

#define BLE_HCI_OCF_LE_SET_HOST_FEATURE_V2 (0x0074)

struct ble_hci_le_set_host_feature_v2_cp
{
    uint16_t bit_num;
    uint8_t  bit_val;
} __attribute__((packed));

/* --- Vendor specific commands (OGF 0x003F) */
#if (BLE_HCI_VS_CMD_ENABLE)
//TOTO:

#endif //BLE_HCI_VS_CMD_ENABLE

enum HCI_SEND_STRATEGY
{
    /* always wait for ACK time out (BLE_VHCI_TIMEOUT_MS) ms*/
    HCI_SEND_STRATEGY_WAIT_ACK_ALWAYS = 0,
    /* wait for ACK time out (BLE_VHCI_TIMEOUT_MS) ms,
     * once a timeout occurs,
     * the remaining commands to be sent will time out immediately.
     */
    HCI_SEND_STRATEGY_WAIT_ACK_BYPASS,
};

/**
 *   @brief BLE Host HCI command send and wait for ACK process.
 *
 *   @param[in] opcode      - HCI command opcode.
 *   @param[in] cmd         - Pointer to the HCI command data.
 *   @param[in] cmd_len     - Length of the HCI command data.
 *   @param[out] rsp        - Pointer to the HCI command response data.
 *   @param[in] rsp_len     - Length of the HCI command response data.
 *
 *   @return 0-0xFF for BLE controller return status.
 *            - BLE_HCI_ERR(BLE_HOST_HCI_ERR_WAIT_TIMEOUT) for wait wack timeout.
 *            - BLE_HCI_ERR(BLE_HOST_ERR_CONTROLLER) for controller return packet error.
 */
int ble_host_hci_cmd_tx(uint16_t opcode, const void *cmd, uint8_t cmd_len, void *rsp, uint8_t rsp_len);

/**
 *   @brief BLE Host HCI command send and wait for ACK process.
 *
 *   @param[in] opcode      - HCI command opcode.
 *   @param[in] cmd         - Pointer to the HCI command data.
 *   @param[in] cmd_len     - Length of the HCI command data.
 *
 *   @return None.
 */
void ble_host_hci_cmd_tx_no_rsp(uint16_t opcode, const void *cmd, uint8_t cmd_len);

/**
 *   @brief BLE Host HCI command receive command complete event process.
 *
 *   @param[in] p_evt - Pointer to the HCI command complete event.
 *
 *   @return None.
 */
void ble_host_hci_rx_cmd_complete(void *p_evt);

/**
 *   @brief BLE Host HCI command receive command status event process.
 *
 *   @param[in] p_evt - Pointer to the HCI command status event
 *
 *   @return None.
 */
void ble_host_hci_rx_cmd_status(void *p_evt);

// #ifdef __cplusplus
// }
// #endif

#endif /* STACK_BLE_HOST_V1_BLE_HCI_CMD_H_ */
