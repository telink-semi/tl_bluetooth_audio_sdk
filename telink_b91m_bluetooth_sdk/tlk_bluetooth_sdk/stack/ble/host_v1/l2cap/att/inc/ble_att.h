/********************************************************************************************************
 * @file    ble_att.h
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


#define ATT_AUTH_SIG_LEN 12 /*!< \brief Authentication signature length */

/** \name ATT PDU Length Fields
 * Length constants of PDU fixed length fields
 */
/**@{*/
#define ATT_ERR_RSP_LEN             5                                      /*!< \brief Error response length. */
#define ATT_MTU_REQ_LEN             3                                      /*!< \brief MTU request length. */
#define ATT_MTU_RSP_LEN             3                                      /*!< \brief MTU response length. */
#define ATT_FIND_INFO_REQ_LEN       5                                      /*!< \brief Find information request length. */
#define ATT_FIND_INFO_RSP_LEN       2                                      /*!< \brief Find information response length. */
#define ATT_FIND_TYPE_REQ_LEN       7                                      /*!< \brief Find type request length. */
#define ATT_FIND_TYPE_RSP_LEN       1                                      /*!< \brief Find type response length. */
#define ATT_READ_TYPE_REQ_LEN       5                                      /*!< \brief Read type request length. */
#define ATT_READ_TYPE_RSP_LEN       2                                      /*!< \brief Read type response length. */
#define ATT_READ_REQ_LEN            3                                      /*!< \brief Read request length. */
#define ATT_READ_RSP_LEN            1                                      /*!< \brief Read response length. */
#define ATT_READ_BLOB_REQ_LEN       5                                      /*!< \brief Read blob request length. */
#define ATT_READ_BLOB_RSP_LEN       1                                      /*!< \brief Read blob response length. */
#define ATT_READ_MULT_REQ_LEN       1                                      /*!< \brief Read multiple request length. */
#define ATT_READ_MULT_RSP_LEN       1                                      /*!< \brief Read multiple response length. */
#define ATT_READ_GROUP_TYPE_REQ_LEN 5                                      /*!< \brief Read group type request length. */
#define ATT_READ_GROUP_TYPE_RSP_LEN 2                                      /*!< \brief Read group type response length. */
#define ATT_WRITE_REQ_LEN           3                                      /*!< \brief Write request length. */
#define ATT_WRITE_RSP_LEN           1                                      /*!< \brief Write response length. */
#define ATT_WRITE_CMD_LEN           3                                      /*!< \brief Write command length. */
#define ATT_SIGNED_WRITE_CMD_LEN    (ATT_WRITE_CMD_LEN + ATT_AUTH_SIG_LEN) /*!< \brief Signed write command length. */
#define ATT_PREP_WRITE_REQ_LEN      5                                      /*!< \brief Prepared write command length. */
#define ATT_PREP_WRITE_RSP_LEN      5                                      /*!< \brief Prepared write response length. */
#define ATT_EXEC_WRITE_REQ_LEN      2                                      /*!< \brief Execute write request length. */
#define ATT_EXEC_WRITE_RSP_LEN      1                                      /*!< \brief Execute write response length. */
#define ATT_VALUE_NTF_LEN           3                                      /*!< \brief Value notification length. */
#define ATT_VALUE_IND_LEN           3                                      /*!< \brief Value indication length. */
#define ATT_VALUE_CFM_LEN           1                                      /*!< \brief Value confirmation length. */
#define ATT_READ_MULT_VAR_REQ_LEN   1                                      /*!< \brief Base read multiple variable request length. */
#define ATT_READ_MULT_VAR_RSP_LEN   1                                      /*!< \brief Base read multiple variable response length. */
#define ATT_PDU_MULT_VALUE_NTF_LEN  1                                      /*!< \brief Base multiple variable notification length. */


/**
 *  @brief Definition ATT minimum and maximum MTU.
*/
#define ATT_MINIMUM_MTU     23
#define ATT_MAXIMUM_MTU     527

#define BLE_ATT_DEFAULT_MTU ATT_MINIMUM_MTU
#define ATT_MTU_CHECK(mtu)  ((mtu) >= ATT_MINIMUM_MTU)

/**
 *  @brief  Definition for error codes of ATT_ERROR_RSP PDU.
 *  See the Core_v5.4(Vol 3/Part F/3.4.1.1, table 34) for more information.
 */
enum attribute_error_code
{
    ATT_SUCCESS                           = 0x00,
    ATT_ERR_INVALID_HANDLE                = 0x01, /** < The attribute handle given was not valid on this server. */
    ATT_ERR_READ_NOT_PERMITTED            = 0x02, /** < The attribute cannot be read. */
    ATT_ERR_WRITE_NOT_PERMITTED           = 0x03, /** < The attribute cannot be written */
    ATT_ERR_INVALID_PDU                   = 0x04, /** < The attribute PDU was invalid. */
    ATT_ERR_INSUFFICIENT_AUTH             = 0x05, /** < The attribute requires authentication before it can be read or written. */
    ATT_ERR_REQ_NOT_SUPPORTED             = 0x06, /** < ATT Server does not support the request received from the client. */
    ATT_ERR_INVALID_OFFSET                = 0x07, /** < Offset specified was past the end of the attribute. */
    ATT_ERR_INSUFFICIENT_AUTHOR           = 0x08, /** < The attribute requires authorization before it can be read or written. */
    ATT_ERR_PREPARE_QUEUE_FULL            = 0x09, /** < Too many prepare writes have been queued. */
    ATT_ERR_ATTRIBUTE_NOT_FOUND           = 0x0A, /** < No attribute found within the given attribute handle range. */
    ATT_ERR_ATTRIBUTE_NOT_LONG            = 0x0B, /** < The attribute cannot be read using the ATT_READ_BLOB_REQ PDU. */
    ATT_ERR_ENCRYPTION_KEY_SIZE_TOO_SHORT = 0x0C, /** < The Encryption Key Size used for encrypting this link is too short. */
    ATT_ERR_INVALID_ATTR_VALUE_LEN        = 0x0D,
    ATT_ERR_INVALID_ATTRIBUTE_VALUE_LEN   = 0x0D, /** < The attribute value length is invalid for the operation. */
    /**
     * < The attribute request that was requested has encountered an error that was unlikely,
     * and therefore could not be completed as requested.
     * */
    ATT_ERR_UNLIKELY_ERR            = 0x0E,
    ATT_ERR_UNLIKELY_ERROR          = 0x0E,
    ATT_ERR_INSUFFICIENT_ENCRYPT    = 0x0F,
    ATT_ERR_INSUFFICIENT_ENCRYPTION = 0x0F, /** < The attribute requires encryption before it can be read or written */
    ATT_ERR_UNSUPPORTED_GRP_TYPE    = 0x10,
    ATT_ERR_UNSUPPORTED_GROUP_TYPE  = 0x10, /** < The attribute type is not a supported grouping attribute as defined by a higher layer specification.*/
    ATT_ERR_INSUFFICIENT_RESOURCES  = 0x11, /** < Insufficient Resources to complete the request. */
    ATT_ERR_DATABASE_OUT_OF_SYNC    = 0x12, /** < The server requests the client to rediscover the database. */
    ATT_ERR_VALUE_NOT_ALLOWED       = 0x13, /** < The attribute parameter value was not allowed. */

    /** Application error code defined by a higher layer specification*/
    ATT_ERR_APPLICATION_ERROR_START = 0x80,
    ATT_ERR_APPLICATION_ERROR_END   = 0x9F,

    /** Common profile and service error codes defined in Core Specification Supplement, Part B */
    ATT_ERR_WRITE_REQUEST_REJECT       = 0xFC, /** < Write Request Rejected. */
    ATT_ERR_CCC_DESC_IMPROPERLY_CONFIG = 0xFD, /** < Client Characteristic Configuration Descriptor Improperly Configured. */
    ATT_ERR_PROC_ALREADY_IN_PROGRESS   = 0xFE, /** < Procedure Already in Progress. */
    ATT_ERR_OUT_OF_RANGE               = 0xFF, /** < Out of Range */
};

/**
 *  @brief  Definition for Attribute opcode, core_v5.4 Vol 3, Part F, 3.4.8 (table 3.43).
 */
enum ble_att_opcode
{
    ATT_OPCODE_ERROR_RSP                  = 0x01,
    ATT_OPCODE_EXCHANGE_MTU_REQ           = 0x02,
    ATT_OPCODE_EXCHANGE_MTU_RSP           = 0x03,
    ATT_OPCODE_FIND_INFORMATION_REQ       = 0x04,
    ATT_OPCODE_FIND_INFORMATION_RSP       = 0x05,
    ATT_OPCODE_FIND_BY_TYPE_VALUE_REQ     = 0x06,
    ATT_OPCODE_FIND_BY_TYPE_VALUE_RSP     = 0x07,
    ATT_OPCODE_READ_BY_TYPE_REQ           = 0x08,
    ATT_OPCODE_READ_BY_TYPE_RSP           = 0x09,
    ATT_OPCODE_READ_REQ                   = 0x0A,
    ATT_OPCODE_READ_RSP                   = 0x0B,
    ATT_OPCODE_READ_BLOB_REQ              = 0x0C,
    ATT_OPCODE_READ_BLOB_RSP              = 0x0D,
    ATT_OPCODE_READ_MULTIPLE_REQ          = 0x0E,
    ATT_OPCODE_READ_MULTIPLE_RSP          = 0x0F,
    ATT_OPCODE_READ_BY_GROUP_TYPE_REQ     = 0x10,
    ATT_OPCODE_READ_BY_GROUP_TYPE_RSP     = 0x11,
    ATT_OPCODE_WRITE_REQ                  = 0x12,
    ATT_OPCODE_WRITE_RSP                  = 0x13,
    ATT_OPCODE_WRITE_CMD                  = 0x52,
    ATT_OPCODE_PREPARE_WRITE_REQ          = 0x16,
    ATT_OPCODE_PREPARE_WRITE_RSP          = 0x17,
    ATT_OPCODE_EXECUTE_WRITE_REQ          = 0x18,
    ATT_OPCODE_EXECUTE_WRITE_RSP          = 0x19,
    ATT_OPCODE_READ_MULTIPLE_VARIABLE_REQ = 0x20, //core_5.2
    ATT_OPCODE_READ_MULTIPLE_VARIABLE_RSP = 0x21, //core_5.2
    ATT_OPCODE_MULTIPLE_HANDLE_VALUE_NTF  = 0x23, //core_5.2
    ATT_OPCODE_HANDLE_VALUE_NTF           = 0x1B,
    ATT_OPCODE_HANDLE_VALUE_IND           = 0x1D,
    ATT_OPCODE_HANDLE_VALUE_CFM           = 0x1E,
    ATT_OPCODE_SIGNED_WRITE_CMD           = 0xD2,
};

/**
 *  @brief Definition Attribute PDU operation code.
*/
enum ble_attribute_opcode_flag
{
    ATTR_OP_FLAG_AUTH_SIGNATURE = 0x80, /** < bit7: Authentication Signature Flag */
    ATTR_OP_FLAG_COMMAND        = 0x40, /** < bit6: Command Flag */
    ATTR_OP_FLAG_METHOD         = 0x3F, /** < bit5-0: Method */
    ATTR_OP_FLAG_SERVER         = 0x01, /** < bit0: Server request */
};

enum ble_attr_handle
{
    ATTR_HANDLE_NONE      = 0x0000, /** < attribute handle none. */
    ATTR_HANDLE_START_MIN = 0x0001, /** < start attribute handle min value. */
    ATTR_HANDLE_END_MAX   = 0xFFFF, /** < end attribute handle max value. */
};

struct att_initial_param
{
    uint16_t mtu;             /** < ATT MTU size, range 23-517. */
    uint16_t max_ntf_pending; /** < maximum number of pending notifications. */
};

/**
 *  @brief Definition Attribute PDU format, core_v5.4 Vol 3, Part F, 3.3.1 (table 3.2).
*/
struct ble_att_pdu_format
{
    uint8_t opcode;
    uint8_t param[0];
};

struct ble_host_att_deal_info
{
    uint16_t                   conn_handle;
    uint16_t                   cid;
    uint16_t                   mtu;
    struct ble_att_pdu_format *tx_buffer;
};

struct ble_host_att_process_handle
{
    uint8_t expect_length; /** < expect min attribute PDU length */
    /** < ATT RX handler, return ack response length. */
    uint16_t (*att_rx_handler)(const struct ble_host_att_deal_info *, const struct ble_att_pdu_format *, uint16_t);
};

/**
 *   @brief Initialize ATT layer.
 *
 *   @param[in] init_param Pointer to the ATT initialization parameters.
 *
 *   @return BLE_HOST_ERR_SUCC on success.
 *          -  BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_PARAMS) if init_param is NULL.
 *          -  BLE_L2CAP_ERR(BLE_L2CAP_ERR_ATT_MTU_EXCEEDED) if the ATT MTU is exceeded.
 */
int ble_host_att_init(struct att_initial_param *init_param);

/**
 *    @brief Initialize EATT layer.
 *
 *    @return None.
 */
void ble_host_eatt_initialize(void);

/**
 *   @brief ATT Layer set MTU size for system.
 *
 *   @param[in] mtu MTU size, range 23-65535.
 *
 *   @return BLE_HOST_ERR_SUCC on success.
 */
int ble_host_set_att_mtu(uint16_t mtu);

/**
 *   @brief ATT Layer set maximum number of pending notifications for system.
 *
 *   @param[in] max_ntf_pending Maximum number of pending notifications.
 *
 *   @return BLE_HOST_ERR_SUCC on success.
 */
int ble_host_set_max_ntf_pending(uint16_t max_ntf_pending);

/**
 *   @brief ATT Layer get MTU size by connection handle.
 *
 *   @param[in] conn_handle Connection handle.
 *
 *   @return MTU size, if the connection handle is invalid, return 0.
 */
uint16_t ble_host_att_get_mtu(uint16_t conn_handle);

/**
 *   @brief ATT Layer update MTU size hook function.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] mtu MTU size.
 *
 *   @note This function is called by L2CAP layer when MTU size is updated.
 */
// void ble_host_att_mtu_update_hook(uint16_t conn_handle, uint16_t mtu);

/**
 *   @brief ATT Layer get local MTU size by connection handle.
 *
 *   @param[in] conn_handle Connection handle.
 *
 *   @return MTU size, if the connection handle is invalid, return 0.
 */
uint16_t ble_host_att_get_local_mtu(uint16_t conn_handle);

/**
 *   @brief ATT Layer set MTU size by connection handle.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] mtu MTU size, range 23-517.
 *
 *   @return BLE_HOST_ERR_SUCC on success.
 *          -  BLE_L2CAP_ERR(BLE_L2CAP_ERR_CONN_HANDLE_INVALID) if connection handle is invalid.
 *          -  BLE_L2CAP_ERR(BLE_L2CAP_ERR_ATT_MTU_EXCEEDED) if the ATT MTU is exceeded.
 */
int ble_host_att_set_mtu(uint16_t conn_handle, uint16_t mtu);

/**
 *   @brief L2CAP Layer get MTU size by connection handle and channel id, support EATT.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] cid Channel id.
 *
 *   @return MTU size, if the connection handle or channel id is invalid, return 0.
 */
uint16_t ble_host_get_att_mtu(uint16_t conn_handle, uint16_t cid);

uint16_t ble_host_eatt_cid_info(uint16_t conn_handle, uint16_t src_cid[5]);
