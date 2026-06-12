/********************************************************************************************************
 * @file    ble_hci_error.h
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
typedef enum
{
    BLE_SUCCESS = 0,

    //// HCI Status, See the Core_v5.0(Vol 2/Part D/1.3 "list of Error Codes") for more information)
    HCI_ERR_UNKNOWN_HCI_CMD                       = 0x01,
    HCI_ERR_UNKNOWN_CONN_ID                       = 0x02,
    HCI_ERR_HW_FAILURE                            = 0x03,
    HCI_ERR_PAGE_TIMEOUT                          = 0x04,
    HCI_ERR_AUTH_FAILURE                          = 0x05,
    HCI_ERR_PIN_KEY_MISSING                       = 0x06,
    HCI_ERR_MEM_CAP_EXCEEDED                      = 0x07,
    HCI_ERR_CONN_TIMEOUT                          = 0x08,
    HCI_ERR_CONN_LIMIT_EXCEEDED                   = 0x09,
    HCI_ERR_SYNCH_CONN_LIMIT_EXCEEDED             = 0x0A,
    HCI_ERR_CONN_ALREADY_EXISTS                   = 0x0B,
    HCI_ERR_CMD_DISALLOWED                        = 0x0C,
    HCI_ERR_CONN_REJ_LIMITED_RESOURCES            = 0x0D,
    HCI_ERR_CONN_REJECTED_SECURITY_REASONS        = 0x0E,
    HCI_ERR_CONN_REJECTED_UNACCEPTABLE_BDADDR     = 0x0F,
    HCI_ERR_CONN_ACCEPT_TIMEOUT_EXCEEDED          = 0x10,
    HCI_ERR_UNSUPPORTED_FEATURE_PARAM_VALUE       = 0x11,
    HCI_ERR_INVALID_HCI_CMD_PARAMS                = 0x12,
    HCI_ERR_REMOTE_USER_TERM_CONN                 = 0x13,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES = 0x14,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_POWER_OFF     = 0x15,
    HCI_ERR_CONN_TERM_BY_LOCAL_HOST               = 0x16,
    HCI_ERR_REPEATED_ATTEMPTS                     = 0x17,
    HCI_ERR_PAIRING_NOT_ALLOWED                   = 0x18,
    HCI_ERR_UNKNOWN_LMP_PDU                       = 0x19,
    HCI_ERR_UNSUPPORTED_REMOTE_FEATURE            = 0x1A,
    HCI_ERR_SCO_OFFSET_REJ                        = 0x1B,
    HCI_ERR_SCO_INTERVAL_REJ                      = 0x1C,
    HCI_ERR_SCO_AIR_MODE_REJ                      = 0x1D,
    HCI_ERR_INVALID_LMP_PARAMS                    = 0x1E,
    HCI_ERR_UNSPECIFIED_ERROR                     = 0x1F,
    HCI_ERR_UNSUPPORTED_LMP_PARAM_VAL             = 0x20,
    HCI_ERR_ROLE_CHANGE_NOT_ALLOWED               = 0x21,
    HCI_ERR_LMP_LL_RESP_TIMEOUT                   = 0x22,
    HCI_ERR_LMP_ERR_TRANSACTION_COLLISION         = 0x23,
    HCI_ERR_LMP_PDU_NOT_ALLOWED                   = 0x24,
    HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE           = 0x25,
    HCI_ERR_LINK_KEY_CAN_NOT_BE_CHANGED           = 0x26,
    HCI_ERR_REQ_QOS_NOT_SUPPORTED                 = 0x27,
    HCI_ERR_INSTANT_PASSED                        = 0x28,
    HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED   = 0x29,
    HCI_ERR_DIFFERENT_TRANSACTION_COLLISION       = 0x2A,
    HCI_ERR_RESERVED1                             = 0x2B,
    HCI_ERR_QOS_UNACCEPTABLE_PARAM                = 0x2C,
    HCI_ERR_QOS_REJ                               = 0x2D,
    HCI_ERR_CHAN_ASSESSMENT_NOT_SUPPORTED         = 0x2E,
    HCI_ERR_INSUFFICIENT_SECURITY                 = 0x2F,
    HCI_ERR_PARAM_OUT_OF_MANDATORY_RANGE          = 0x30,
    HCI_ERR_RESERVED2                             = 0x31,
    HCI_ERR_ROLE_SWITCH_PENDING                   = 0x32,
    HCI_ERR_RESERVED3                             = 0x33,
    HCI_ERR_RESERVED_SLOT_VIOLATION               = 0x34,
    HCI_ERR_ROLE_SWITCH_FAILED                    = 0x35,
    HCI_ERR_EXTENDED_INQUIRY_RESP_TOO_LARGE       = 0x36,
    HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST  = 0x37,
    HCI_ERR_HOST_BUSY_PAIRING                     = 0x38,
    HCI_ERR_CONN_REJ_NO_SUITABLE_CHAN_FOUND       = 0x39,
    HCI_ERR_CONTROLLER_BUSY                       = 0x3A,
    HCI_ERR_UNACCEPTABLE_CONN_INTERVAL            = 0x3B,
    HCI_ERR_UNACCEPTABLE_CONN_PARAMETERS          = 0x3B,
    HCI_ERR_ADVERTISING_TIMEOUT                   = 0x3C,
    HCI_ERR_CONN_TERM_MIC_FAILURE                 = 0x3D,
    HCI_ERR_CONN_FAILED_TO_ESTABLISH              = 0x3E,
    HCI_ERR_MAC_CONN_FAILED                       = 0x3F,
    HCI_ERR_COARSE_CLOCK_ADJUSTMENT_REJECT        = 0x40,
    HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED              = 0x41,
    HCI_ERR_UNKNOWN_ADV_IDENTIFIER                = 0x42,
    HCI_ERR_LIMIT_REACHED                         = 0x43,
    HCI_ERR_OP_CANCELLED_BY_HOST                  = 0x44,
    HCI_ERR_PACKET_TOO_LONG                       = 0x45,
    //DBG used only for CIS
    HCI_ERR_CONN_TERM_CIS_MIC_FAILURE = 0xD3,


    ///////////////////////// TELINK define status /////////////////////////////

    //LL status
    LL_ERR_CONNECTION_NOT_ESTABLISH = 0x80,
    LL_ERR_TX_FIFO_NOT_ENOUGH,
    LL_ERR_ENCRYPTION_BUSY,
    LL_ERR_CURRENT_STATE_NOT_SUPPORTED_THIS_CMD,
    LL_ERR_INVALID_PARAMETER,
    LL_ERR_UNKNOWN_OPCODE,


    //L2CAP status
    L2CAP_ERR_INVALID_PARAMETER = 0x90,
    L2CAP_ERR_INVALID_HANDLE,
    L2CAP_ERR_INSUFFICIENT_RESOURCES,
    L2CAP_ERR_PSM_NOT_REGISTER,
    L2CAP_ERR_CONTROL_NOT_READY,
    L2CAP_ERR_COC_CREATING,
    L2CAP_ERR_COC_DATA_STILL_SENT,
    L2CAP_ERR_NO_CID_AVAILABLE,
    L2CAP_ERR_ALL_CID_ALLOCATED,
    L2CAP_ERR_NO_CREATE_COC_HANDLER,

    //SMP status
    SMP_ERR_INVALID_PARAMETER = 0xA0,
    SMP_ERR_PAIRING_BUSY,
    SMP_ERR_SC_MTU_TOO_SHORT,

    //GATT status
    GATT_ERR_INVALID_PARAMETER = 0xB0,
    GATT_ERR_PREVIOUS_INDICATE_DATA_HAS_NOT_CONFIRMED,
    GATT_ERR_SERVICE_DISCOVERY_TIMEOUT,
    GATT_ERR_NOTIFY_INDICATION_BUSY,
    GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY,
    GATT_ERR_DATA_LENGTH_EXCEED_MTU_SIZE,
    GATT_ERR_DATA_LENGTH_EXCEED_MEM_RESTRICTION,
    GATT_ERR_UNSPECIFIED,

    //GAP status
    GAP_ERR_INVALID_PARAMETER = 0xC0,
    GAP_ERR_STATE_NO_IDLE,
    GAP_ERR_WRITE_BUSY,

    //IAL
    IAL_ERR_SDU_LEN_EXCEED_SDU_MAX,
    IAL_ERR_LOSS_SDU_INTERVAL,
    IAL_ERR_ISO_TX_FIFO_NOT_ENOUGH,
    IAL_ERR_SDU_BUFF_INVALID,
    IAL_ERR_EVENT_PASSED,
    IAL_ERR_INVALID_PDU,
    IAL_HCI_BUFFER_INVALID,

    //Service status
    SERVICE_ERR_INVALID_PARAMETER = 0xD0,

    //Profile common error
    PRF_ERR_INVALID_ATTR_HANDLE,
    PRF_ERR_INVALID_PARAMETER,

    //Audio Profile status
    AUDIO_ERR_NO_MEMORY = 0xE0,
    AUDIO_ERR_INVALID_PARAMETER,
    AUDIO_ERR_DISCOVERY_FAILED,
    AUDIO_ERR_BUSY,
    AUDIO_ERR_STATUS,

    //LE Audio Server
    LE_AUDIO_SERVER_INVALID_SERVICE = 0xF0,
    LE_AUDIO_SERVER_INVALID_HANDLE,

} ble_sts_t;
