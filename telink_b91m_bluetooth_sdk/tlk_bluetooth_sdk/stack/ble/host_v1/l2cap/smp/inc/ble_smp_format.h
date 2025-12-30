/********************************************************************************************************
 * @file    ble_smp_format.h
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

struct ble_smp_pdu_format
{
    uint8_t code;
    uint8_t data[0];
};

struct ble_host_smp_deal_info
{
    uint16_t                   conn_handle;
    uint8_t                    role; /** < Connected role, 0x00 is central, 0x01 is peripheral. */
    struct ble_host_conn      *conn;
    void                      *smp_info;
    struct ble_smp_pdu_format *tx_buffer;
};

typedef uint16_t (*ble_host_smp_handler)(const struct ble_host_smp_deal_info *, const struct ble_smp_pdu_format *);


#define BLE_HOST_SMP_MAX_KEY_SIZE_MIN_VALUE                    7
#define BLE_HOST_SMP_MAX_KEY_SIZE_MAX_VALUE                    16

#define BLE_HOST_SMP_MAX_KEY_SIZE_DEFAULT_VALUE                BLE_HOST_SMP_MAX_KEY_SIZE_MAX_VALUE

#define BLE_HOST_SMP_CHECK_MAX_KEY_SIZE_VALID(max_key_size)    ((max_key_size) >= BLE_HOST_SMP_MAX_KEY_SIZE_MIN_VALUE && (max_key_size) <= BLE_HOST_SMP_MAX_KEY_SIZE_MAX_VALUE)

#define BLE_HOST_SMP_CHECK_LE_KEY_DISTRIBUTION_VALID(key_dist) (((key_dist) & 0xF0) == 0x00)

enum ble_smp_opcode
{
    SMP_OPCODE_PAIRING_REQUEST               = 0x01,
    SMP_OPCODE_PAIRING_RESPONSE              = 0x02,
    SMP_OPCODE_PAIRING_CONFIRM               = 0x03,
    SMP_OPCODE_PAIRING_RANDOM                = 0x04,
    SMP_OPCODE_PAIRING_FAILED                = 0x05,
    SMP_OPCODE_ENCRYPTION_INFORMATION        = 0x06,
    SMP_OPCODE_CENTRAL_IDENTIFICATION        = 0x07,
    SMP_OPCODE_IDENTITY_INFORMATION          = 0x08,
    SMP_OPCODE_IDENTITY_ADDRESS_INFORMATION  = 0x09,
    SMP_OPCODE_SIGNING_INFORMATION           = 0x0A,
    SMP_OPCODE_SECURITY_REQUEST              = 0x0B,
    SMP_OPCODE_PAIRING_PUBLIC_KEY            = 0x0C,
    SMP_OPCODE_PAIRING_DHKEY_CHECK           = 0x0D,
    SMP_OPCODE_PAIRING_KEYPRESS_NOTIFICATION = 0x0E,
};

#define BLE_HOST_SMP_CHECK_OPCODE_VALID(opcode) ((opcode) >= SMP_OPCODE_PAIRING_REQUEST && (opcode) <= SMP_OPCODE_PAIRING_KEYPRESS_NOTIFICATION)

/**
 *  @brief Definition SMP authentication requirements flags,
 * core_v5.4 Vol 3, Part H, 3.5.1(figure 3.3).
*/
struct smp_auth_req
{
    uint8_t bonding_flags : 2; /** < refer to enum ble_host_smp_bonding_flags */
    uint8_t MITM          : 1; /** < man-in-the-middle */
    uint8_t sc            : 1; /** < LE secure connections */
    uint8_t keypress      : 1; /** < the Passkey Entry protocol */
    uint8_t ct2           : 1; /** < 1 upon transmission to indicate support for the h7 function.*/
    uint8_t rfu           : 2; /** < Reserved for future use */
};

/**
 *  @brief Definition SMP key distribution,
 * core_v5.4 Vol 3, Part H, 3.6.1(figure 3.11).
*/
struct smp_key_distribution
{
    uint8_t encKey  : 1; /** < EDIV and rand */
    uint8_t idKey   : 1; /** < the device shall distribute IRK and public device or static random address using Identity Address Information */
    uint8_t signKey : 1; /** < the device shall distribute CSRK using the Signing Information command */
    uint8_t linkKey : 1; /** < the device would like to derive the Link Key from the LTK */
    uint8_t rtu     : 4; /** < Reserved for future use */
};

#define BLE_HOST_SMP_ENC_KEY_DISTRIBUTION_FLAG            0x01
#define BLE_HOST_SMP_ID_KEY_DISTRIBUTION_FLAG             0x02
#define BLE_HOST_SMP_SIGN_KEY_DISTRIBUTION_FLAG           0x04
#define BLE_HOST_SMP_LINK_KEY_DISTRIBUTION_FLAG           0x08

#define BLE_HOST_SMP_CHECK_KEY_DISTRIBUTION_ENC_KEY(key)  ((key) & BLE_HOST_SMP_ENC_KEY_DISTRIBUTION_FLAG)
#define BLE_HOST_SMP_CHECK_KEY_DISTRIBUTION_ID_KEY(key)   ((key) & BLE_HOST_SMP_ID_KEY_DISTRIBUTION_FLAG)
#define BLE_HOST_SMP_CHECK_KEY_DISTRIBUTION_SIGN_KEY(key) ((key) & BLE_HOST_SMP_SIGN_KEY_DISTRIBUTION_FLAG)
#define BLE_HOST_SMP_CHECK_KEY_DISTRIBUTION_LINK_KEY(key) ((key) & BLE_HOST_SMP_LINK_KEY_DISTRIBUTION_FLAG)

/**
 *  @brief Definition pairing failed reason codes,
 * core_v5.4 Vol 3, Part H, 3.5.5(table 3.7).
*/
enum smp_pairing_failed_reason
{
    SMP_PAIRING_SUCCESS = 0x00,
    //  The user input of passkey failed, for example, the user cancelled the operation
    SMP_FAILED_PASSKEY_ENTRY_FAILED = 0x01,
    // The OOB data is not available
    SMP_FAILED_OOB_NOT_AVAILABLE = 0x02,
    // The pairing procedure cannot be performed as authentication requirements
    // cannot be met due to IO capabilities of one or both devices
    SMP_FAILED_AUTH_REQ = 0x03,
    // The confirm value does not match the calculated compare value
    SMP_FAILED_CONFIRM_VALUE_FAILED = 0x04,
    // Pairing is not supported by the device
    SMP_FAILED_PAIRING_NOT_SUPPORTED = 0x05,
    // The resultant encryption key size is not long enough for the security requirements of this device
    SMP_FAILED_ENCRYPTION_KEY_SIZE = 0x06,
    // The SMP command received is not supported on this device
    SMP_FAILED_COMMAND_NOT_SUPPORTED = 0x07,
    // Pairing failed due to an unspecified reason
    SMP_FAILED_UNSPECIFIED_REASON = 0x08,
    // Pairing or authentication procedure is disallowed because too little time has elapsed since last pairing request or security request
    SMP_FAILED_REPEATED_ATTEMPTS = 0x09,
    // The Invalid Parameters error code indicates that the command length is invalid or that a parameter is outside of the specified range.
    SMP_FAILED_INVALID_PARAMETERS = 0x0A,
    // Indicates to the remote device that the DHKey Check value received doesn't match the one calculated by the local device.
    SMP_FAILED_DHKEY_CHECK_FAILED = 0x0B,
    // Indicates that the confirm values in the numeric comparison protocol do not match
    SMP_FAILED_NUMERIC_COMPARISON_FAILED = 0x0C,
    // Indicates that the pairing over the LE transport failed due to a Pairing Request sent over the BR/EDR transport in progress.
    SMP_FAILED_BR_EDR_PAIRING_IN_PROGRESS = 0x0D,
    // Indicates that the BR/EDR Link Key generated on the BR/EDR transport cannot be used to derive and distribute keys for the LE transport
    // or the LE LTK generated on the LE transport cannot be used to derive a key for the BR/EDR transport.
    SMP_FAILED_CROSS_TRANSPORT_KEY_DERIVATION_GENERATION_NOT_ALLOWED = 0x0E,
    // Indicates that the device chose not to accept a distributed key.
    SMP_FAILED_KEY_REJECTED = 0x0F,
    // Define error, pairing timeout.
    SMP_FAILED_PAIRING_TIMEOUT = 0x80,
    // Define error, ACL Disconnected.
    SMP_FAILED_ACL_DISCONNECTED = 0x81,
};

struct smp_public_key
{
    uint8_t key[32]; /** < Public Key 32 octets */
};

struct smp_DHkey_key
{
    uint8_t key[16]; /** < DH Key 16 octets */
};

/**
 *  @brief Definition notification type in Pairing Keypress Notification PDU,
 * core_v5.4 Vol 3, Part H, 3.5.8(table 3.8).
*/
enum smp_notification_type
{
    SMP_PASSKEY_ENTRY_STARTED   = 0x00,
    SMP_PASSKEY_DIGIT_ENTERED   = 0x01,
    SMP_PASSKEY_DIGIT_ERASED    = 0x02,
    SMP_PASSKEY_CLEARED         = 0x03,
    SMP_PASSKEY_ENTRY_COMPLETED = 0x04,
};

/////////Definition all Pairing methods format

/**
 *  @brief Definition format of Pairing Request PDU,
 * core_v5.4 Vol 3, Part H, 3.5.1(figure 3.2).
*/
struct smp_pairing_request_format
{
    uint8_t code;          /** < 0x01 SMP_OPCODE_PAIRING_REQUEST */
    uint8_t io_capability; /** < IO Capability */
    uint8_t oob_data_flag; /** < OOB data flag */

    union
    { /** < authentication requirement */
        struct smp_auth_req auth_req;
        uint8_t             auth_req_raw;
    };

    uint8_t max_key_size; /** < Maximum Encryption Key Size */

    union
    { /** < Initiator Key Distribution */
        struct smp_key_distribution initiator_key;
        uint8_t                     initiator_key_raw;
    };

    union
    { /** < Responder Key Distribution */
        struct smp_key_distribution responder_key;
        uint8_t                     responder_key_raw;
    };
};

/**
 *  @brief Definition format of Pairing Response PDU,
 * core_v5.4 Vol 3, Part H, 3.5.2(figure 3.4).
*/
struct smp_pairing_response_format
{
    uint8_t code;          /** < 0x01 SMP_OPCODE_PAIRING_RESPONSE */
    uint8_t io_capability; /** < IO Capability */
    uint8_t oob_data_flag; /** < OOB data flag */

    union
    { /** < authentication requirement */
        struct smp_auth_req auth_req;
        uint8_t             auth_req_raw;
    };

    uint8_t max_key_size; /** < Maximum Encryption Key Size */

    union
    { /** < Initiator Key Distribution */
        struct smp_key_distribution initiator_key;
        uint8_t                     initiator_key_raw;
    };

    union
    { /** < Responder Key Distribution */
        struct smp_key_distribution responder_key;
        uint8_t                     responder_key_raw;
    };
};

/**
 *  @brief Definition format of Pairing Confirm PDU,
 * core_v5.4 Vol 3, Part H, 3.5.3(figure 3.5).
*/
struct smp_pairing_confirm_format
{
    uint8_t code;        /** < 0x03 SMP_OPCODE_PAIRING_CONFIRM */
    uint8_t confirm[16]; /** < Confirm Value */
};

/**
 *  @brief Definition format of Pairing Random PDU,
 * core_v5.4 Vol 3, Part H, 3.5.4(figure 3.6).
*/
struct smp_pairing_random_format
{
    uint8_t code;       /** < 0x04 SMP_OPCODE_PAIRING_RANDOM */
    uint8_t random[16]; /** < random Value */
};

/**
 *  @brief Definition format of Pairing Failed PDU,
 * core_v5.4 Vol 3, Part H, 3.5.5(figure 3.7).
*/
struct smp_pairing_failed_format
{
    uint8_t code;   /** < 0x05 SMP_OPCODE_PAIRING_FAILED */
    uint8_t reason; /** < pairing failed reason */
};

/**
 *  @brief Definition format of Encryption Information PDU,
 * core_v5.4 Vol 3, Part H, 3.6.2(figure 3.12).
*/
struct smp_encryption_information_format
{
    uint8_t code;    /** < 0x06 SMP_OPCODE_ENCRYPTION_INFORMATION */
    uint8_t ltk[16]; /** < The generated Long Term Key(LTK) value being distributed */
};

/**
 *  @brief Definition format of Central Identification PDU,
 * core_v5.4 Vol 3, Part H, 3.6.3(figure 3.13).
*/
struct smp_central_identification_format
{
    uint8_t  code; /** < 0x07 SMP_OPCODE_CENTRAL_IDENTIFICATION */
    uint16_t ediv; /** < Encrypted Diversifier(EDIV) */
    uint64_t rand; /** < Random Number(RAND) */
} __attribute__((packed));

/**
 *  @brief Definition format of Identity Information  PDU,
 * core_v5.4 Vol 3, Part H, 3.6.4(figure 3.14).
*/
struct smp_identity_information_format
{
    uint8_t code;    /** < 0x08 SMP_OPCODE_IDENTITY_INFORMATION */
    uint8_t irk[16]; /** < Identity Resolving Key */
};

/**
 *  @brief Definition format of Identity Address Information PDU,
 * core_v5.4 Vol 3, Part H, 3.6.5(figure 3.15).
*/
struct smp_identity_address_information_format
{
    uint8_t code;     /** < 0x09 SMP_OPCODE_IDENTITY_ADDRESS_INFORMATION */
    uint8_t addrType; /** < BD_ADDR type */
    uint8_t addr[6];  /** < device’s public device address or static random address.*/
};

/**
 *  @brief Definition format of Signing Information PDU,
 * core_v5.4 Vol 3, Part H, 3.6.6(figure 3.16).
*/
struct smp_signing_information_format
{
    uint8_t code;              /** < 0x0A SMP_OPCODE_SIGNING_INFORMATION */
    uint8_t signature_key[16]; /** < Signature Key(CSRK) */
};

/**
 *  @brief Definition format of Security Request PDU,
 * core_v5.4 Vol 3, Part H, 3.6.7(figure 3.17).
*/
struct smp_security_request_format
{
    uint8_t code; /** < 0x0B SMP_OPCODE_SECURITY_REQUEST */

    union
    { /** < authentication requirement */
        struct smp_auth_req auth_req;
        uint8_t             auth_req_raw;
    };
};

/**
 *  @brief Definition format of Pairing Public Key PDU,
 * core_v5.4 Vol 3, Part H, 3.5.6(figure 3.8).
*/
struct smp_pairing_public_key_format
{
    uint8_t code;             /** < 0x0C SMP_OPCODE_PAIRING_PUBLIC_KEY */
    uint8_t public_key_x[32]; /** < Public Key X */
    uint8_t public_key_y[32]; /** < Public Key Y */
};

/**
 *  @brief Definition format of Pairing DHKey Check PDU,
 * core_v5.4 Vol 3, Part H, 3.5.7(figure 3.9).
*/
struct smp_pairing_DHkey_format
{
    uint8_t code;            /** < 0x0D SMP_OPCODE_PAIRING_DHKEY_CHECK */
    uint8_t DHKey_check[16]; /** < DH Key check(E) */
};

/**
 *  @brief Definition format of Pairing Keypress Notification PDU,
 * core_v5.4 Vol 3, Part H, 3.5.7(figure 3.10).
*/
struct smp_keypress_notification_format
{
    uint8_t code;              /** < 0x0E SMP_KEYPRESS_NOTIFICATION */
    uint8_t notification_type; /** < notification type */
};
