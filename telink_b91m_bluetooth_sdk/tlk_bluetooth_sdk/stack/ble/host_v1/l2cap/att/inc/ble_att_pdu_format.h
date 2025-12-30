/********************************************************************************************************
 * @file    ble_att_pdu_format.h
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


/**************************
 * Definition all Attribute PDU format.
*/

/****************core_v5.4 Vol 3, Part F, 3.4.1 Error Handing******************/
/**
 *  @brief Definition format of ATT_ERROR_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.1.1 (table 3.3).
*/
struct att_error_rsp_format
{
    uint8_t  opcode;        /** < 0x01 ATT_ERROR_RSP */
    uint8_t  requestOpcode; /** < The opcode of the request that caused the error */
    uint16_t handle;        /** < The handle of the attribute that caused the error */
    uint8_t  errorCode;     /** < The error code */
} __attribute__((packed));

/****************core_v5.4 Vol 3, Part F, 3.4.2 MTU Exchange******************/
/**
 *  @brief Definition format of ATT_EXCHANGE_MTU_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.2.1 (table 3.5).
*/
struct att_exchange_mtu_req_format
{
    uint8_t  opcode;      /** < 0x02 ATT_EXCHANGE_MTU_REQ */
    uint16_t clientRxMTU; /** < Client receive MTU size */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_EXCHANGE_MTU_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.2.2 (table 3.6).
*/
struct att_exchange_mtu_rsp_format
{
    uint8_t  opcode;      /** < 0x03 ATT_EXCHANGE_MTU_RSP */
    uint16_t serverRxMTU; /** < ATT Server receive MTU size */
} __attribute__((packed));

/****************core_v5.4 Vol 3, Part F, 3.4.3 Find information******************/
/**
 *  @brief Definition format of ATT_FIND_INFORMATION_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.1 (table 3.7).
*/
struct att_find_information_req_format
{
    uint8_t  opcode;      /** < 0x04 ATT_FIND_INFORMATION_REQ */
    uint16_t startHandle; /** < First requested handle number */
    uint16_t endHandle;   /** < Last requested handle number */
} __attribute__((packed));

/**
 *  @brief The format of the information data for ATT_FIND_INFORMATION_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.2 (table 3.9).
*/
enum att_info_format
{
    ATT_INFO_FORMAT_16BIT_UUID  = 0x01, /** < A list of 1 or more handles with their 16-bit Bluetooth UUIDs */
    ATT_INFO_FORMAT_128BIT_UUID = 0x02, /** < A list of 1 or more handles with their 128-bit UUIDs */
} __attribute__((packed));

/**
 *  @brief The format of the format handle and 16-bit Bluetooth UUID for ATT_INFO_16BIT_UUID.
 *  core_v5.4 Vol 3, Part F, 3.4.3.2 (table 3.10).
*/
struct att_info_16bit_uuid
{
    uint16_t handle;
    uint16_t uuid;
} __attribute__((packed));

/**
 *  @brief The format of the format handle and 128-bit UUID for ATT_INFO_128BIT_UUID.
 *  core_v5.4 Vol 3, Part F, 3.4.3.2 (table 3.11).
*/
struct att_info_128bit_uuid
{
    uint16_t handle;
    uint8_t  uuid[16];
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_FIND_INFORMATION_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.2 (table 3.8).
*/
struct att_find_information_rsp_format
{
    uint8_t opcode; /** < 0x05 ATT_FIND_INFORMATION_RSP */
    uint8_t format; /** < refer to enum att_info_format */

    union
    { // information data. must least one 16bit uuid.
        struct att_info_16bit_uuid  uuid16bit[1];
        struct att_info_128bit_uuid uuid128bit[0];
    } __attribute__((packed));
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_FIND_BY_TYPE_VALUE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.3 (table 3.12).
*/
struct att_find_by_type_value_req_format
{
    uint8_t  opcode;       /** < 0x06 ATT_FIND_BY_TYPE_VALUE_REQ */
    uint16_t startHandle;  /** < First requested handle number */
    uint16_t endHandle;    /** < Last requested handle number */
    uint16_t attrType;     /** < 2 octet UUID to find */
    uint8_t  attrValue[0]; /** < Attribute value to find */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_FIND_BY_TYPE_VALUE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.4 (table 3.14).
*/
struct attr_handle_group
{
    uint16_t groupStartHandle;
    uint16_t groupEndHandle;
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_FIND_BY_TYPE_VALUE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.3.4 (table 3.13).
*/
struct att_find_by_type_value_rsp_format
{
    uint8_t                  opcode;  /** < 0x07 ATT_FIND_BY_TYPE_VALUE_RSP */
    struct attr_handle_group list[1]; /** < A list of 1 or more Handle Information. */
} __attribute__((packed));

/****************core_v5.4 Vol 3, Part F, 3.4.4 Reading attributes******************/
/**
 *  @brief Definition format of ATT_READ_BY_TYPE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.1 (table 3.15).
*/
struct att_read_by_type_req_format
{
    uint8_t  opcode;      /** < 0x08 ATT_READ_BY_TYPE_REQ */
    uint16_t startHandle; /** < First requested handle number */
    uint16_t endHandle;   /** < Last requested handle number */
    uint8_t  attrType[2]; /** < 2 or 16 octet UUID */
} __attribute__((packed));

/**
 *  @brief The format of the attribute data.
 *  core_v5.4 Vol 3, Part F, 3.4.4.2 (table 3.17).
*/
struct attr_type_data
{
    uint16_t handle;   /** < 2 octets */
    uint8_t  value[0]; /** < (length - 2) octets */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BY_TYPE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.2 (table 3.16).
*/
struct att_read_by_type_rsp_format
{
    uint8_t               opcode;  /** < 0x09 ATT_READ_BY_TYPE_RSP */
    uint8_t               length;  /** < The size of each attribute handle value pair */
    struct attr_type_data list[1]; /** < A list of Attribute Data */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.3 (table 3.18).
*/
struct att_read_req_format
{
    uint8_t  opcode; /** < 0x0A ATT_READ_REQ */
    uint16_t handle; /** < The handle of the attribute to be read */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.4 (table 3.19).
*/
struct att_read_rsp_format
{
    uint8_t opcode;   /** < 0x0B ATT_READ_RSP */
    uint8_t value[0]; /** < The value of the attribute with the handle given */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BLOB_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.5 (table 3.20).
*/
struct att_read_blob_req_format
{
    uint8_t  opcode; /** < 0x0C ATT_READ_BLOB_REQ */
    uint16_t handle; /** < The handle of the attribute to be read. */
    uint16_t offset; /** < The offset of the first octet to be read. */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BLOB_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.6 (table 3.21).
*/
struct att_read_blob_rsp_format
{
    uint8_t opcode;       /** < 0x0D ATT_READ_BLOB_RSP */
    uint8_t partValue[0]; /** < Part of the value of the attribute with the handle given */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_MULTIPLE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.7 (table 3.22).
*/
struct att_read_multiple_req_format
{
    uint8_t  opcode;     /** < 0x0E ATT_READ_MULTIPLE_REQ */
    uint16_t handles[2]; /** < A set of two or more attribute handles.< */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_MULTIPLE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.8 (table 3.23).
*/
struct att_read_multiple_rsp_format
{
    uint8_t opcode;    /** < 0x0F ATT_READ_MULTIPLE_RSP */
    uint8_t values[0]; /** < A set of two or more values */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BY_GROUP_TYPE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.9 (table 3.24).
*/
struct att_read_by_group_type_req_format
{
    uint8_t  opcode;           /** < 0x10 ATT_READ_BY_GROUP_TYPE_REQ */
    uint16_t startHandle;      /** < First requested handle number */
    uint16_t endHandle;        /** < Last requested handle number */
    uint8_t  attrGroupType[2]; /** < 2 or 16 octet UUID */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BY_GROUP_TYPE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.10 (table 3.26).
*/
struct attr_group_type_data
{
    uint16_t startHandle;  /** < 2 octets */
    uint16_t endHandle;    /** < 2 octets */
    uint8_t  attrValue[0]; /** < (length - 4) octets */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_BY_GROUP_TYPE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.10 (table 3.25).
*/
struct att_read_by_group_type_rsp_format
{
    uint8_t                     opcode;  /** < 0x11 ATT_READ_BY_GROUP_TYPE_RSP */
    uint8_t                     length;  /** < The size of each Attribute Data */
    struct attr_group_type_data data[1]; /** < A list of Attribute Data */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_MULTIPLE_VARIABLE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.11 (table 3.27).
*/
struct att_read_multiple_variable_req_format
{
    uint8_t  opcode;     /** < 0x20 ATT_READ_MULTIPLE_VARIABLE_REQ */
    uint16_t handles[2]; /** < A set of two or more attribute handles< */
} __attribute__((packed));

/**
 *  @brief The format of the Length Value Tuple.
 *  core_v5.4 Vol 3, Part F, 3.4.4.12 (table 3.29).
*/
struct attr_value_tuple
{
    uint16_t length;
    uint8_t  attrValue[0]; /**< (value length) cotets */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_READ_MULTIPLE_VARIABLE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.4.12 (table 3.28).
*/
struct att_read_multiple_variable_rsp_format
{
    uint8_t                 opcode;   /** < 0x21 ATT_READ_MULTIPLE_VARIABLE_RSP */
    struct attr_value_tuple tuple[2]; /** < A list of Length Value Tuples */
} __attribute__((packed));

/****************core_v5.4 Vol 3, Part F, 3.4.5 Writing attributes******************/
/**
 *  @brief Definition format of ATT_WRITE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.5.1 (table 3.30).
*/
struct att_write_req_format
{
    uint8_t  opcode;   /** < 0x12 ATT_WRITE_REQ */
    uint16_t handle;   /** < The handle of the attribute to be written */
    uint8_t  value[0]; /** < The value to be written to the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_WRITE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.5.2 (table 3.31).
*/
struct att_write_rsp_format
{
    uint8_t opcode; /** < 0x13 ATT_WRITE_RSP */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_WRITE_CMD PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.5.3 (table 3.32).
*/
struct att_write_cmd_format
{
    uint8_t  opcode;   /** < 0x52 ATT_WRITE_CMD */
    uint16_t handle;   /** < The handle of the attribute to be written */
    uint8_t  value[0]; /** < The value to be written to the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_SIGNED_WRITE_CMD PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.5.4 (table 3.33).
*/
struct att_signed_write_cmd_format
{
    uint8_t  opcode;   /** < 0xD2 ATT_SIGNED_WRITE_CMD */
    uint16_t handle;   /** < The handle of the attribute to be written */
    uint8_t  value[0]; /** < The value to be written to the attribute */
    /** < Authentication signature for the Attribute Opcode, Attribute Handle and Attribute Value parameters */
    uint8_t signature[12];
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_PREPARE_WRITE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.6.1 (table 3.34).
*/
struct att_prepare_write_req_format
{
    uint8_t  opcode;   /** < 0x16 ATT_PREPARE_WRITE_REQ */
    uint16_t handle;   /** < The handle of the attribute to be written */
    uint16_t offset;   /** < The offset of the first octet to be written */
    uint8_t  value[0]; /** < The value to be written to the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_PREPARE_WRITE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.6.2 (table 3.35).
*/
struct att_prepare_write_rsp_format
{
    uint8_t  opcode;   /** < 0x17 ATT_PREPARE_WRITE_RSP */
    uint16_t handle;   /** < The handle of the attribute to be written */
    uint16_t offset;   /** < The offset of the first octet to be written */
    uint8_t  value[0]; /** < The value to be written to the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_EXECUTE_WRITE_REQ PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.6.3 (table 3.36).
*/
struct att_execute_write_req_format
{
    uint8_t opcode; /** < 0x18 ATT_EXECUTE_WRITE_REQ */
    uint8_t flags;  /** < Flags */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_EXECUTE_WRITE_RSP PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.6.4 (table 3.37).
*/
struct att_execute_write_rsp_format
{
    uint8_t opcode; /** < 0x19 ATT_EXECUTE_WRITE_RSP */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_HANDLE_VALUE_NTF PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.7.1 (table 3.38).
 */
struct att_handle_value_ntf_format
{
    uint8_t  opcode;   /** < 0x1B ATT_HANDLE_VALUE_NTF */
    uint16_t handle;   /** < The handle of the attribute */
    uint8_t  value[0]; /** < The value of the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_HANDLE_VALUE_IND PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.7.2 (table 3.39).
 */
struct att_handle_value_ind_format
{
    uint8_t  opcode;   /** < 0x1D ATT_HANDLE_VALUE_IND */
    uint16_t handle;   /** < The handle of the attribute */
    uint8_t  value[0]; /** < The value of the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_HANDLE_VALUE_CFM PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.7.3 (table 3.40).
 */
struct att_handle_value_cfm_format
{
    uint8_t opcode; /** < 0x1E ATT_HANDLE_VALUE_CFM */
} __attribute__((packed));

/**
 *  @brief Definition format of Handle length Value Tuple.
 *  core_v5.4 Vol 3, Part F, 3.4.7.4 (table 3.42).
 */
struct attr_handle_value_tuple
{
    uint16_t handle;   /** < The handle of the attribute */
    uint16_t length;   /** < The length of the attribute value */
    uint8_t  value[0]; /** < The value of the attribute */
} __attribute__((packed));

/**
 *  @brief Definition format of ATT_MULTIPLE_HANDLE_VALUE_NTF PDU.
 *  core_v5.4 Vol 3, Part F, 3.4.7.4 (table 3.41).
 */
struct att_multiple_handle_value_ntf_format
{
    uint8_t                        opcode;   /** < 0x1F ATT_MULTIPLE_HANDLE_VALUE_NTF */
    struct attr_handle_value_tuple tuple[2]; /** < A list of Handle Length Value Tuples */
} __attribute__((packed));