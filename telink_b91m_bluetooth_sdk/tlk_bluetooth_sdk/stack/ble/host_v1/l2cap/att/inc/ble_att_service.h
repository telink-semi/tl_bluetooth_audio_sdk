/********************************************************************************************************
 * @file    ble_att_service.h
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

/** @} start ATT_PERMISSIONS_BITMAPS */
/**
 *  @brief ATT service permissions bit fields.
 *
 *  Bit fields used to define the access permissions for an ATT attribute.
 *  The permissions are defined as follows:
 *
 *  - Read(BIT0): The attribute can be read by a client.
 *  - Write(BIT1): The attribute can be written by a client.
 *  - RFU(BIT2-3): Reserved for future use.
 *  - Authorization(BIT4): The attribute read or write requires, should check the authorization.
 *  - Encryption(BIT5): The attribute read or write requires, should use encryption.
 *  - Authentication(BIT6): The attribute read or write requires, should use authentication.
 *  - Secure Connection(BIT7): The attribute read or write requires, should use secure connection.
 *
 *  (See the Core_v5.4(Vol 3/Part C/10.3.1/Table 10.2) for more information).
 *
 */
#define ATT_PERMISSIONS_NONE              0x00 /** < Attribute has no permissions */
#define ATT_PERMISSIONS_READ              0x01 /** < Attribute is Readable */
#define ATT_PERMISSIONS_WRITE             0x02 /** < Attribute is Writable */
#define ATT_PERMISSIONS_RDWR              0x03 /** < Attribute is Readable & Writable */

#define ATT_PERMISSIONS_AUTHOR            0x10 /** < Attribute requires Authorization */
#define ATT_PERMISSIONS_ENCRYPT           0x20 /** < Attribute requires Encryption */
#define ATT_PERMISSIONS_AUTHEN            0x40 /** < Attribute requires Authentication(MITM protection) */
#define ATT_PERMISSIONS_SECURE_CONN       0x80 /** < Attribute requires Secure_Connection */

#define ATT_PERMISSIONS_AUTHOR_READ       (ATT_PERMISSIONS_READ | ATT_PERMISSIONS_AUTHOR)  /** < Attribute Read requires Authorization */
#define ATT_PERMISSIONS_AUTHOR_WRITE      (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_AUTHOR) /** < Attribute Write requires Authorization */
#define ATT_PERMISSIONS_AUTHOR_RDWR       (ATT_PERMISSIONS_RDWR | ATT_PERMISSIONS_AUTHOR)  /** < Attribute Read & Write requires Authorization */

#define ATT_PERMISSIONS_ENCRYPT_READ      (ATT_PERMISSIONS_READ | ATT_PERMISSIONS_ENCRYPT)  /** < Attribute Read requires Encryption */
#define ATT_PERMISSIONS_ENCRYPT_WRITE     (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_ENCRYPT) /** < Attribute Write requires Encryption */
#define ATT_PERMISSIONS_ENCRYPT_RDWR      (ATT_PERMISSIONS_RDWR | ATT_PERMISSIONS_ENCRYPT)  /** < Attribute Read & Write requires Encryption */

#define ATT_PERMISSIONS_AUTHEN_READ       (ATT_PERMISSIONS_ENCRYPT_READ | ATT_PERMISSIONS_AUTHEN)  /** < Attribute Read requires Authentication*/
#define ATT_PERMISSIONS_AUTHEN_WRITE      (ATT_PERMISSIONS_ENCRYPT_WRITE | ATT_PERMISSIONS_AUTHEN) /** < Attribute Write requires Authentication */
#define ATT_PERMISSIONS_AUTHEN_RDWR       (ATT_PERMISSIONS_ENCRYPT_RDWR | ATT_PERMISSIONS_AUTHEN)  /** < Attribute Read & Write requires Authentication */

#define ATT_PERMISSIONS_SECURE_CONN_READ  (ATT_PERMISSIONS_AUTHEN_READ | ATT_PERMISSIONS_SECURE_CONN)  /** < Attribute Read requires Secure_Connection */
#define ATT_PERMISSIONS_SECURE_CONN_WRITE (ATT_PERMISSIONS_AUTHEN_WRITE | ATT_PERMISSIONS_SECURE_CONN) /** < Attribute Write requires Secure_Connection */
#define ATT_PERMISSIONS_SECURE_CONN_RDWR  (ATT_PERMISSIONS_AUTHEN_RDWR | ATT_PERMISSIONS_SECURE_CONN)  /** < Attribute Read & Write requires Secure_Connection */

/** @} End ATT_PERMISSIONS_BITMAPS */

/** @{ start ATT_SETTINGS_BITMAPS */
/**
 *  @brief ATT service settings bit fields.
 *
 *  Bit fields used to define the settings for an ATT attribute.
 *  The settings are defined as follows:
 *
 *  - Write callback(BIT0): When the client want to write the attribute value, stack will first call write callback function.
 *      If write callback is not set, stack will check Allow to write bit.
 *      If write callback is set, stack will call write callback function.
 *          If write callback function return ATT_SUCCESS, stack will check Allow to write bit, and replay response.
 *          If write callback function return value range 1 to 0xFF, stack will reply ATT_ERROR_RSP with error code that return value.
 *          If write callback function return other value, stack skip this ATT request packet.
 *  - Read callback(BIT1): When the client want read the attribute value, stack will first call read callback function.
 *  - Variable length(BIT2): The attribute value length is variable and can change length, when allow write bit is set.
 *  - Allow write(BIT3): The attribute value can be written by client.
 *      If variable length is supported,
 *        If attribute value length is less than maxAttrLen, stack will write attribute value and length to attrValue and attrValueLen.
 *        If attribute value length is greater than maxAttrLen, stack reply ATT_ERROR_RSP with error code ATT_ERR_INVALID_ATTRIBUTE_VALUE_LEN.
 *      If variable length is not supported,
 *        If attribute value length is equal to maxAttrLen, stack will write attribute value to attrValue.
 *        If attribute value length is not equal maxAttrLen, stack reply ATT_ERROR_RSP with error code ATT_ERR_INVALID_ATTRIBUTE_VALUE_LEN.
 *  - Characteristic attribute properties(BIT4): The attribute value is a Characteristic attribute.
 *      If this bit is set, the attribute uuid must be characteristic uuid(DECLARATIONS_UUID_CHARACTERISTIC:0x2803).
 *      attribute value is characteristic attribute properties, next attribute uuid is characteristic uuid.
 *      (See the Core_v5.4(Vol 3/Part G/3.3.1/Table 3.3) for more information).
 *      Why use need this bit? because attribute declaration is fixed structure, include properties, value attribute handle and uuid.
 *        value attribute handle and uuid is include the next attribute handle. so it don't need like this attribute value.
          xxxUUIDCharacteristicDeclaration = { characteristic properties, value attribute handle, characteristic uuid };
        only attribute properties must need.
 *  - RFU(BIT5-7): Reserved for future use.
 */

#define ATTS_SET_WRITE_CALLBACK        0x01
#define ATTS_SET_READ_CALLBACK         0x02
#define ATTS_SET_VARIABLE_LEN          0x04 /** < allow write value length is variable and can change length. */
#define ATTS_SET_ALLOW_WRITE           0x08 /** < allow write value copy to attribute value ptr */
#define ATTS_SET_ATTR_VALUE_PROPERTIES 0x10 /** < attribute value is Characteristic attribute properties. */

/** @} End ATT_SETTINGS_BITMAPS */

/**
 *  @brief ATT service attribute structure.
 *
 *  This structure defines an ATT attribute. It contains the following fields:
 *
 *  - perm: The attribute permissions.
 *  - uuidLen: The UUID length.
 *  - uuid: The UUID value.
 *  - attrValueLen: The attribute value length.
 *  - maxAttrLen: The maximum attribute value length.
 *  - attrValue: The attribute value.
 *  - settings: The attribute settings.
*/
struct atts_attribute
{
    uint8_t        perm;         // refer to ATT_PERMISSIONS_BITMAPS.
    uint8_t        uuidLen;      // UUID length, usually 2 or 16, 4 maybe used.
    const uint8_t *uuid;         // UUID value, 16 or 128 bits.
    uint16_t      *attrValueLen; // attribute value length, point to the actual length of attribute value.
    uint16_t       maxAttrLen;   // maximum attribute value length, only used for attribute value.
    uint8_t       *attrValue;    // attribute value, point to the actual attribute value.
    uint8_t        settings;     // refer to ATT_SETTINGS_BITMAPS.
};

/**
 *  @brief ATT service write callback function type.
 *
 *  This function is called when the client wants to write an attribute value, and ATTS_SET_WRITE_CALLBACK is set.
 *
 *  @param[in] conn_handle Connection handle.
 *  @param[in] opcode ATT opcode, such as ATT_WRITE_REQ or ATT_WRITE_CMD, etc.
 *  @param[in] attr_handle Attribute handle.
 *  @param[in] value Pointer to the attribute value to be written.
 *  @param[in] value_len Attribute value length.
 *
 *  @return ATT_SUCCESS if the attribute value is written successfully, stack will reply corresponding response.
 *          range 1 to 0xFF if the attribute value is written failed, stack will reply ATT_ERROR_RSP with error code that return value.
 *          other value stack skip this ATT request packet, and do nothing, high layer must reply corresponding response.
*/
typedef int (*atts_w_cb_t)(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);

/**
 *  @brief ATT service read callback function type.
 *
 *  This function is called when the client wants to read an attribute value, and ATTS_SET_READ_CALLBACK is set.
 *
 *  @param[in] conn_handle Connection handle.
 *  @param[in] opcode ATT opcode, such as ATT_READ_REQ or ATT_READ_BLOB_REQ, etc.
 *  @param[in] attr_handle Attribute handle.
 *  @param[out] out_value Pointer to want to reply attribute value.
 *  @param[out] out_value_len want to reply attribute value length.
 *
 *  @return ATT_SUCCESS if the attribute value is read successfully, stack will reply corresponding response.
 *          range 1 to 0xFF if the attribute value is read failed, stack will reply ATT_ERROR_RSP with error code that return value.
 *          other value stack skip this ATT request packet, and do nothing, high layer must reply corresponding response.
 */
typedef int (*atts_r_cb_t)(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t **out_value, uint16_t *out_value_len);

/**
 *  @brief ATT service attribute group structure.
 *
 *  This structure defines an ATT attribute group. It contains the following fields:
 *
 *  - pNext: Pointer to the next attribute group.
 *  - pAttr: Pointer to the attribute services table in the group.
 *  - readCallback: Pointer to the read callback function.
 *  - writeCallback: Pointer to the write callback function.
 *  - startHandle: The start attribute handle of the group.
 *  - endHandle: The end attribute handle of the group.
*/
struct atts_group
{
    struct atts_group           *pNext;         /** < Pointer to the next attribute group. */
    const struct atts_attribute *pAttr;         /** < Pointer to the attribute services table in the group. */
    atts_r_cb_t                  readCallback;  /** < Pointer to the read callback function. */
    atts_w_cb_t                  writeCallback; /** < Pointer to the write callback function. */
    uint16_t                     startHandle;   /** < The start attribute handle of the group. */
    uint16_t                     endHandle;     /** < The end attribute handle of the group. */
};

/**
 *  @brief  Compare uuid that attribute handle and input uuid are same.
 *
 *  @param[in]  pAttr  Pointer to attribute service.
 *  @param[in]  uuidLen  Length of uuid.
 *  @param[in]  pUuid  Pointer to uuid.
 *
 *  @return  true if uuid is same, false otherwise.
*/
bool ble_attribute_uuid_cmp_uuid(const struct atts_attribute *pAttr, uint8_t uuidLen, const uint8_t *pUuid);

/**
 * @brief       Compare the attribute value UUID with the input UUID.
 * @param[in]   pAttr       - pointer to the attribute service.
 * @param[in]   uuidLen     - length of the UUID (2 or 16 bytes).
 * @param[in]   pUuid       - pointer to the UUID to compare.
 * @return      true if the UUIDs match, false otherwise.
 */
bool ble_attribute_value_cmp_uuid(const struct atts_attribute *pAttr, uint8_t uuidLen, const uint8_t *pUuid);

/**
 * @brief       Compare the attribute UUID with a 16-bit UUID.
 * @param[in]   pAttr       - pointer to the attribute service.
 * @param[in]   uuid16      - the 16-bit UUID to compare.
 * @return      true if the UUIDs match, false otherwise.
 */
bool ble_attribute_uuid_cmp_uuid16(const struct atts_attribute *pAttr, uint16_t uuid16);

/**
 * @brief       Get the attribute service group by connection handle.
 * @param[in]   conn_handle    - the connection handle.
 * @return      pointer to the attribute service group, or NULL if not found.
 */
struct atts_group *ble_host_get_attribute_service_group_by_conn_handle(uint16_t conn_handle);

/**
 * @brief       Add an attribute service group by connection handle.
 * @param[in]   conn_handle    - the connection handle.
 * @param[in]   pGroup          - pointer to the attribute service group to add.
 * @return      none.
 */
void ble_host_add_attribute_service_group_by_conn_handle(uint16_t conn_handle, struct atts_group *pGroup);

/**
 * @brief       Remove an attribute service group by connection handle and start handle.
 * @param[in]   conn_handle    - the connection handle.
 * @param[in]   startHandle    - the start handle of the service group to remove.
 * @return      none.
 */
void ble_host_remove_attribute_service_group_by_conn_handle(uint16_t conn_handle, uint16_t startHandle);

/**
 * @brief       Get the attribute service group.
 * @return      pointer to the attribute service group, or NULL if not found.
 */
struct atts_group *ble_host_get_attribute_service_group(void);

/**
 * @brief       Add an attribute service group.
 * @param[in]   pGroup      - pointer to the attribute service group to add.
 * @return      none.
 */
void ble_host_add_attribute_service_group(struct atts_group *pGroup);

/**
 * @brief       Remove an attribute service group by start handle.
 * @param[in]   startHandle - the start handle of the service group to remove.
 * @return      none.
 */
void ble_host_remove_attribute_service_group(uint16_t startHandle);

/**
 * @brief       Get the attribute information by attribute handle.
 * @param[in]   attr_handle - the attribute handle.
 * @return      pointer to the attribute, or NULL if not found.
 */
const struct atts_attribute *ble_host_get_attribute_info(uint16_t attr_handle);
