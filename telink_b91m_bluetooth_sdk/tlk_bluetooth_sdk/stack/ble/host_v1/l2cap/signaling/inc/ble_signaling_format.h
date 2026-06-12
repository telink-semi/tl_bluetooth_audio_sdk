/********************************************************************************************************
 * @file    ble_signaling_format.h
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

/**
 *  @brief  Definition for L2CAP signaling packet formats
 */
enum ble_host_l2cap_signaling_code
{
    L2CAP_SIGNALING_COMMAND_REJECT_RSP             = 0x01, //BR & LE
    L2CAP_SIGNALING_CONNECTION_REQ                 = 0x02, //BR
    L2CAP_SIGNALING_CONNECTION_RSP                 = 0x03, //BR
    L2CAP_SIGNALING_CONFIGURATION_REQ              = 0x04, //BR
    L2CAP_SIGNALING_CONFIGURATION_RSP              = 0x05, //BR
    L2CAP_SIGNALING_DISCONNECTION_REQ              = 0x06, //BR & LE
    L2CAP_SIGNALING_DISCONNECTION_RSP              = 0x07, //BR & LE
    L2CAP_SIGNALING_ECHO_REQ                       = 0x08, //BR
    L2CAP_SIGNALING_ECHO_RSP                       = 0x09, //BR
    L2CAP_SIGNALING_INFORMATION_REQ                = 0x0A, //BR
    L2CAP_SIGNALING_INFORMATION_RSP                = 0x0B, //BR
    L2CAP_SIGNALING_CREATE_CHANNEL_REQ             = 0x0C, //no used, core_5.3
    L2CAP_SIGNALING_CREATE_CHANNEL_RSP             = 0x0D, //no used, core_5.3
    L2CAP_SIGNALING_MOVE_CHANNEL_REQ               = 0x0E, //no used, core_5.3
    L2CAP_SIGNALING_MOVE_CHANNEL_RSP               = 0x0F, //no used, core_5.3
    L2CAP_SIGNALING_MOVE_CHANNEL_CONFIRMATION_REQ  = 0x10, //no used, core_5.3
    L2CAP_SIGNALING_MOVE_CHANNEL_CONFIRMATION_RSP  = 0x11, //no used, core_5.3
    L2CAP_SIGNALING_CONN_PARAM_UPDATE_REQ          = 0x12, //LE
    L2CAP_SIGNALING_CONN_PARAM_UPDATE_RSP          = 0x13, //LE
    L2CAP_SIGNALING_LE_CREDIT_BASED_CONNECTION_REQ = 0x14, //LE
    L2CAP_SIGNALING_LE_CREDIT_BASED_CONNECTION_RSP = 0x15, //LE
    L2CAP_SIGNALING_FLOW_CONTROL_CREDIT_IND        = 0x16, //BR & LE
    L2CAP_SIGNALING_CREDIT_BASED_CONNECTION_REQ    = 0x17, //BR & LE
    L2CAP_SIGNALING_CREDIT_BASED_CONNECTION_RSP    = 0x18, //BR & LE
    L2CAP_SIGNALING_CREDIT_BASED_RECONFIGURE_REQ   = 0x19, //BR & LE
    L2CAP_SIGNALING_CREDIT_BASED_RECONFIGURE_RSP   = 0x1A, //BR & LE
};

#define BLE_HOST_SIGNALING_CHECK_OPCODE_VALID(opcode) ((opcode) >= L2CAP_SIGNALING_COMMAND_REJECT_RSP && (opcode) <= L2CAP_SIGNALING_CREDIT_BASED_RECONFIGURE_RSP)

/**
 *  @brief  Definition for Error Response of signal packet
 *  See the Core_v5.0(Vol 3/Part A/4.1) for more information.
 */
enum ble_signaling_reject_reason
{
    SIGNALING_REJECT_REASON_CMD_NOT_UNDERSTAND  = 0,
    SIGNALING_REJECT_REASON_MTU_EXCEEDED        = 1,
    SIGNALING_REJECT_REASON_INVALID_CID_REQUEST = 2,
};

//Result values for the L2CAP_LE_CREDIT_BASED_CONNECTION_RSP packet.
enum
{
    LE_CREDIT_BASED_CONN_SUCCESSFUL                            = 0x0000,
    LE_CREDIT_BASED_CONN_REFUSED_SPSM_NOT_SUPPORT              = 0x0002,
    LE_CREDIT_BASED_CONN_REFUSED_NO_RESOURCES_AVAILABLE        = 0x0004,
    LE_CREDIT_BASED_CONN_REFUSED_INSUFFICIENT_AUTHENTICATION   = 0x0005, //not used
    LE_CREDIT_BASED_CONN_REFUSED_INSUFFICIENT_AUTHORIZATION    = 0x0006, //not used
    LE_CREDIT_BASED_CONN_REFUSED_ENCRYPTION_KEY_SIZE_TOO_SHORT = 0x0007, //not used
    LE_CREDIT_BASED_CONN_REFUSED_INSUFFICIENT_ENCRYPTION       = 0x0008, //not used
    LE_CREDIT_BASED_CONN_REFUSED_INVALID_SOURCE_CID            = 0x0009,
    LE_CREDIT_BASED_CONN_REFUSED_SOURCE_CID_ALREADY_ALLOCATED  = 0x000A,
    LE_CREDIT_BASED_CONN_REFUSED_UNACCEPTABLE_PARAMETERS       = 0x000B,
};

//Result values for the L2CAP_CREDIT_BASED_CONNECTION_RSP packet.
enum
{
    CREDIT_BASED_ALL_CONN_SUCCESSFUL                            = 0x0000,
    CREDIT_BASED_ALL_CONN_REFUSED_SPSM_NOT_SUPPORTED            = 0x0002,
    CREDIT_BASED_SOME_CONN_REFUSED_INSUFFICIENT_RESOURCES_AVA   = 0x0004,
    CREDIT_BASED_ALL_CONN_REFUSED_INSUFFICIENT_AUTHENTICATION   = 0x0005,
    CREDIT_BASED_ALL_CONN_REFUSED_INSUFFICIENT_AUTHORIZATION    = 0x0006,
    CREDIT_BASED_ALL_CONN_REFUSED_ENCRYPTION_KEY_SIZE_TOO_SHORT = 0x0007,
    CREDIT_BASED_ALL_CONN_REFUSED_INSUFFICIENT_ENCRYPTION       = 0x0008,
    CREDIT_BASED_SOME_CONN_REFUSED_INVALID_SOURCE_CID           = 0x0009,
    CREDIT_BASED_SOME_CONN_REFUSED_SOURCE_CID_ALREADY_ALLOCATED = 0x000A,
    CREDIT_BASED_ALL_CONN_REFUSED_UNACCEPTABLE_PARAMETERS       = 0x000B,
    CREDIT_BASED_ALL_CONN_REFUSED_INVALID_PARAMETERS            = 0x000C,
    CREDIT_BASED_ALL_CONN_REFUSED_NO_FURTHER_INFO_AVA           = 0x000D,
    CREDIT_BASED_ALL_CONN_REFUSED_AUTHENTICATION_PENDING        = 0x000E,
    CREDIT_BASED_ALL_CONN_REFUSED_AUTHORIZATION_PENDING         = 0x000F,
};

//Result values for the L2CAP_CREDIT_BASED_RECONFIGURE_RSP packet.
enum
{
    CREDIT_BASED_RECONFIGURATION_SUCCESSFUL                       = 0x0000,
    CREDIT_BASED_RECONFIGURATION_FAILED_MTU_NOT_ALLOWED           = 0x0001,
    CREDIT_BASED_RECONFIGURATION_FAILED_MPS_NOT_ALLOWED           = 0x0002,
    CREDIT_BASED_RECONFIGURATION_FAILED_DST_CIDS_INVALID          = 0x0003,
    CREDIT_BASED_RECONFIGURATION_FAILED_OTHER_UNACCEPTABLE_PARAMS = 0x0004,
};

/**
 *  @brief Definition L2CAP Signaling Command PDU format, core_v5.4 Vol 3, Part A, 4 (figure 4.2).
*/
struct ble_signaling_pdu_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint8_t  data[0];
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_COMMAND_REJECT_RSP packet format, core_v5.4 Vol 3, Part A, 4.1 (figure 4.3).
 */
struct ble_signaling_command_reject_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t reason;
    uint8_t  reasonData[0];
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_DISCONNECTION_REQ packet format, core_v5.4 Vol 3, Part A, 4.6 (figure 4.10).
 */
struct ble_signaling_disconnection_req_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t destination_cid;
    uint16_t source_cid;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_DISCONNECTION_RSP packet format, core_v5.4 Vol 3, Part A, 4.7 (figure 4.11).
 */
struct ble_signaling_disconnection_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t destination_cid;
    uint16_t source_cid;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_CONNECTION_PARAMMETER_UPDATE_REQ packet format, core_v5.4 Vol 3, Part A, 4.20 (figure 4.16).
 */
struct ble_signaling_conn_param_update_req_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t latency;
    uint16_t timeout;
} __attribute__((__packed__));

enum l2cap_connection_parameter_update_result
{
    L2CAP_CONNECTION_PARAMETERS_ACCEPTED = 0x0000,
    L2CAP_CONNECTION_PARAMETERS_REJECTED = 0x0001,
};

/**
 *  @brief Definition L2CAP_CONNECTION_PARAMMETER_UPDATE_RSP packet format, core_v5.4 Vol 3, Part A, 4.21 (figure 4.17).
 */
struct ble_signaling_conn_param_update_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t result;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_LE_CREDIT_BASED_CONNECTION_REQ packet format, core_v5.4 Vol 3, Part A, 4.22 (figure 4.18).
 */
struct ble_signaling_le_credit_based_connection_req_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t spsm;
    uint16_t source_id;
    uint16_t mtu;
    uint16_t mps;
    uint16_t initial_credits;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_LE_CREDIT_BASED_CONNECTION_RSP packet format, core_v5.4 Vol 3, Part A, 4.23 (figure 4.19).
 */
struct ble_signaling_le_credit_based_connection_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t destination_id;
    uint16_t mtu;
    uint16_t mps;
    uint16_t initial_credits;
    uint16_t result;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_FLOW_CONTROL_CREDIT_IND packet format, core_v5.4 Vol 3, Part A, 4.24 (figure 4.20).
 */
struct ble_signaling_flow_control_credit_ind_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t cid;
    uint16_t credits;
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_CREDIT_BASED_CONNECTION_REQ packet format, core_v5.4 Vol 3, Part A, 4.25 (figure 4.21).
 */
struct ble_signaling_credit_based_connection_req_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t spsm;
    uint16_t mtu;
    uint16_t mps;
    uint16_t initial_credits;
    uint16_t source_cid[1];
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_CREDIT_BASED_CONNECTION_RSP packet format, core_v5.4 Vol 3, Part A, 4.26 (figure 4.22).
 */
struct ble_signaling_credit_based_connection_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t mtu;
    uint16_t mps;
    uint16_t initial_credits;
    uint16_t result;
    uint16_t destination_cid[1];
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_CREDIT_BASED_RECONFIGURE_REQ packet format, core_v5.4 Vol 3, Part A, 4.27 (figure 4.23).
 */
struct ble_signaling_credit_based_reconfigure_req_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t mtu;
    uint16_t mps;
    uint16_t destination_cid[1];
} __attribute__((__packed__));

/**
 *  @brief Definition L2CAP_CREDIT_BASED_RECONFIGURE_RSP packet format, core_v5.4 Vol 3, Part A, 4.28 (figure 4.24).
 */
struct ble_signaling_credit_based_reconfigure_rsp_format
{
    uint8_t  code;
    uint8_t  identifier;
    uint16_t data_length;
    uint16_t result;
} __attribute__((__packed__));

struct ble_host_signaling_deal_info
{
    uint16_t                         conn_handle;
    struct ble_signaling_pdu_format *tx_buffer;
    void                            *p_conn_info;
};

typedef uint16_t (*ble_host_signaling_handler)(const struct ble_host_signaling_deal_info *, const struct ble_signaling_pdu_format *);
