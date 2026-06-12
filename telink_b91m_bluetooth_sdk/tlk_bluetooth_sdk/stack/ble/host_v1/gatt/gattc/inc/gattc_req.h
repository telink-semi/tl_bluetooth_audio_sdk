/********************************************************************************************************
 * @file    gattc_req.h
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

// all GATT client procedures callback function error, 0-0xFF is core spec defined, 0x100 is received timeout
#define GATT_REQUEST_SUB_PROCEDURE_COMPLETE 0x100
#define GATT_REQUEST_PROCEDURE_COMPLETE     0x101
#define GATT_REQUEST_ERR_TIMEOUT            0x102
#define GATT_REQUEST_ERR_LINK_DISCONNECT    0x103
#define GATT_REQUEST_ERR_OTHER_REASON       0x104

bool ble_host_gattc_is_connected(uint16_t conn_handle, uint16_t cid);

/**
 *  server Configuration: exchange MTU.
 *  Figure 4.1: Exchange MTU.
 */

/**
 *  @brief GATT exchange MTU procedure callback
 *
 *  @param[out] conn_handle Connection handle to be used for the procedure.
 *  @param[out] err refer to enum ble_err_code.
 *  @param[out] MTU Maximum Transfer Unit size, range 23 to 517.
*/
typedef void (*gattc_exchange_mtu_callback)(uint16_t conn_handle, uint32_t err, uint16_t MTU);

/**
 *   @brief Send exchange MTU request to the server.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] mtu Maximum Transfer Unit size, range 23 to 517.
 *   @param[in] callback Callback function to be called when the procedure is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 */
int ble_host_gattc_send_exchange_mtu_req(uint16_t conn_handle, uint16_t mtu, gattc_exchange_mtu_callback callback);

/**
 *  Primary Service Discovery
 *      Discovery All Primary Service(Figure 4.2: Discovery All Primary Services example)
 *      Discovery Primary Service by Service UUID(Figure 4.3: Discovery Primary Service by Service UUID example)
 *
*/
/** @brief Structure for primary service discovery callback. */
struct gattc_disc_services
{
    uint16_t        start_handle; /** < start handle of the service range */
    uint16_t        end_handle;   /** < end handle of the service range */
    void           *user_data;    /** < user data to be passed to the callback */
    struct att_uuid service_uuid; /** < service UUID to discover */
};

/**
 *   @brief  Typedef for a function pointer to a callback function primary service discovery.
 *
 *   @param[out] conn_handle: The ACL Connection handle.
 *   @param[out] cid: Channel ID.
 *   @param[out] err refer to enum ble_err_code.
 *   @param[out] services: Pointer to the structure containing discovered services.
 *
 *   @return true: continue to discovery, false: discovery stop.
 */
typedef bool (*gattc_disc_service_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_services *service);

/** @brief Structure for primary service discovery. */
struct gattc_disc_all_services
{
    uint16_t                    start_handle; /** < start handle of the service range */
    uint16_t                    end_handle;   /** < end handle of the service range */
    void                       *user_data;    /** < user data to be passed to the callback */
    gattc_disc_service_callback callback;     /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start primary service discovery procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the discovery parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_all_primary_services_general(uint16_t conn_handle, uint16_t cid, const struct gattc_disc_all_services *param);

/**
 *   @brief GATT client start primary service discovery procedure, fix start handle is 0x0001 and end handle is 0xFFFF.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] callback Callback function to be called when the procedure is complete, cannot be NULL.
 *   @param[in] user_data User data to be passed to the callback function.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_all_primary_services(uint16_t conn_handle, uint16_t cid, gattc_disc_service_callback callback, void *user_data);

/** @brief Structure for primary service discovery by UUID. */
struct gattc_disc_service_by_uuid
{
    const struct att_uuid      *service_uuid; /** < service UUID to discover */
    uint16_t                    start_handle; /** < start handle of the service range */
    uint16_t                    end_handle;   /** < end handle of the service range */
    void                       *user_data;    /** < user data to be passed to the callback */
    gattc_disc_service_callback callback;     /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start primary service discovery procedure by service UUID.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the discovery parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_primary_service_by_uuid_general(uint16_t conn_handle, uint16_t cid, const struct gattc_disc_service_by_uuid *param);

/**
  *   @brief GATT client start primary service discovery procedure by service UUID,
  *              fix start handle is 0x0001 and end handle is 0xFFFF.
  *
  *   @param[in] conn_handle Connection handle to be used for the procedure.
  *   @param[in] cid Channel ID.
  *   @param[in] service_uuid Pointer to the service UUID to discover.
  *   @param[in] callback Callback function to be called when the procedure is complete, cannot be NULL.
  *   @param[in] user_data User data to be passed to the callback function.
  *
  *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
  *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
  *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
  *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
  */
int ble_host_gattc_discover_primary_service_by_uuid(uint16_t conn_handle, uint16_t cid, const struct att_uuid *service_uuid, gattc_disc_service_callback callback, void *user_data);

/**
 *  Relationship Discovery
 *      Find Included Services(Figure 4.4: Find Included Services example)
*/
/** @brief Structure for find included service callback. */
struct gattc_find_incl_service
{
    uint16_t        handle;            /** < Service Included attribute handle */
    uint16_t        start_handle;      /** < start handle of the included service range */
    uint16_t        end_handle;        /** < end handle of the included service range */
    void           *user_data;         /** < user data to be passed to the callback */
    struct att_uuid incl_service_uuid; /** < included service UUID to discover */
};

/**
 *   @brief  Typedef for a function pointer to a callback function find included service.
 *
 *   @param[out] conn_handle: Connection handle.
 *   @param[out] cid: Channel ID.
 *   @param[out] err refer to enum ble_err_code.
 *   @param[out] incl_service: Pointer to the structure containing found included service.
 *
 *   @return true: continue to find, false: find stop.
 */
typedef bool (*gattc_find_incl_service_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_find_incl_service *incl_service);

/** @brief Structure for find included service. */
struct gattc_find_incl_service_param
{
    uint16_t                         start_handle; /** < start handle of the service range */
    uint16_t                         end_handle;   /** < end handle of the service range */
    void                            *user_data;    /** < user data to be passed to the callback */
    gattc_find_incl_service_callback callback;     /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start find included service procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the find parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_find_included_services(uint16_t conn_handle, uint16_t cid, const struct gattc_find_incl_service_param *param);

/**
 *  Characteristic Discovery
 *      Discovery All Characteristics of a service(Figure 4.5: Discovery All Characteristics of a service example)
 *      Discovery Characteristics by UUID(Figure 4.6: Discovery Characteristics by UUID example).
*/

/** @brief Structure for characteristic discovery callback. */
struct gattc_disc_characteristic
{
    uint16_t                        handle;              /** Characteristic attribute handle */
    union characteristic_properties properties;          /** Characteristic properties */
    uint16_t                        value_handle;        /**Characteristic value attribute handle */
    struct att_uuid                 characteristic_uuid; /** Characteristic UUID */
    void                           *user_data;           /** < user data to be passed to the callback */
};

/**
 *  @brief  Typedef for a function pointer to a callback function characteristic discovery.
 *
 *  @param[out] conn_handle: Connection handle.
 *  @param[out] cid: Channel ID.
 *  @param[out] err refer to enum ble_err_code.
 *  @param[out] characteristic: Pointer to the structure containing discovered characteristic.
 *
 *  @return true: continue to discovery, false: discovery stop.
 */
typedef bool (*disc_characteristics_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_characteristic *characteristic);

/** @brief Structure for characteristic discovery. */
struct gattc_disc_all_characteristics
{
    uint16_t                      start_handle; /** < start handle of the characteristic range */
    uint16_t                      end_handle;   /** < end handle of the characteristic range */
    void                         *user_data;    /** < user data to be passed to the callback */
    disc_characteristics_callback callback;     /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start all characteristics of a service discovery procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the discovery parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_all_characteristics_of_service(uint16_t conn_handle, uint16_t cid, const struct gattc_disc_all_characteristics *param);

/** @brief Structure for characteristic discovery by UUID. */
struct gattc_disc_all_characteristics_by_uuid
{
    const struct att_uuid        *characteristic_uuid; /** < characteristic UUID to discover */
    uint16_t                      start_handle;        /** < start handle of the characteristic range */
    uint16_t                      end_handle;          /** < end handle of the characteristic range */
    void                         *user_data;           /** < user data to be passed to the callback */
    disc_characteristics_callback callback;            /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start characteristic discovery procedure by UUID.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the discovery parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_all_characteristics_of_service_by_uuid(uint16_t conn_handle, uint16_t cid, const struct gattc_disc_all_characteristics_by_uuid *param);

/**
 *  Characteristic Description Discovery
 *      Discover All Characteristic Descriptors(Figure 4.7: Discover All Characteristic Descriptors example)
*/

/** @brief Structure for characteristic descriptor discovery. */
struct gattc_disc_characteristic_desc
{
    uint16_t        handle;          /** characteristic Descriptor attribute handle */
    struct att_uuid descriptor_uuid; /** Descriptor UUID */
    void           *user_data;       /** < user data to be passed to the callback */
};

/**
 *  @brief  Typedef for a function pointer to a callback function characteristic descriptor discovery.
 *
 *  @param[out] conn_handle: Connection handle.
 *  @param[out] cid: Channel ID.
 *  @param[out] err refer to enum ble_err_code.
 *  @param[out] descriptor: Pointer to the structure containing discovered characteristic descriptor.
 *
 *  @return true: continue to discovery, false: discovery stop.
 */
typedef bool (*disc_characteristic_desc_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_characteristic_desc *descriptor);

/** @brief Structure for characteristic descriptor discovery. */
struct gattc_disc_characteristic_desc_param
{
    uint16_t                          start_handle; /** < start handle of the descriptor range */
    uint16_t                          end_handle;   /** < end handle of the descriptor range */
    void                             *user_data;    /** < user data to be passed to the callback */
    disc_characteristic_desc_callback callback;     /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start characteristic descriptor discovery procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the discovery parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_discover_characteristic_desc(uint16_t conn_handle, uint16_t cid, const struct gattc_disc_characteristic_desc_param *param);

/**
 *   Characteristic Value Read
 *      Read Characteristic Value(Figure 4.8: Read Characteristic Value example)
 *      Read Using Characteristic UUID(Figure 4.9: Read Using Characteristic UUID example)
 *      Read Long Characteristic Value(Figure 4.10: Read Long Characteristic Value example)
 *      Read Multiple Characteristic Values(Figure 4.11: Read Multiple Characteristic Values example)
 *      Read Multiple Variable Length Characteristic Values(Figure 4.12: Read Multiple Variable Length Characteristic Values example)
 */

/** @brief enumeration for the state of the any read characteristic value procedure. */
enum gattc_read_characteristic_value_state
{
    GATTC_READ_VALUE_CONTINUE,
    GATTC_READ_VALUE_COMPLETE,
};

/** @brief Structure for read or read long or read multiple or read multiple variable length characteristic value procedure. */
struct gattc_read_characteristic_value
{
    uint16_t       state;     /** < state of the read characteristic value procedure */
    uint16_t       handle;    /** < attribute handle of the characteristic value to be read */
    uint16_t       offset;    /** < offset of the data to be read */
    uint16_t       length;    /** < length of the data to be read */
    const uint8_t *buffer;    /** < buffer to store the read data */
    void          *user_data; /** < user data to be passed to the callback */
};

/**
 *  @brief  Typedef for a function pointer to a callback function for read characteristic value procedure.
 *
 *  @param[out] conn_handle: Connection handle.
 *  @param[out] cid: Channel ID.
 *  @param[out] err refer to enum ble_err_code.
 *  @param[out] param: Pointer to the structure containing the read characteristic value procedure parameters.
 *
 *  @return true: continue to read, false: read stop.
 */
typedef bool (*gattc_read_characteristic_value_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_read_characteristic_value *param);

/** @brief Structure for read characteristic value procedure. */
struct gattc_read_characteristic_value_param
{
    uint16_t                                 handle;    /** < attribute handle of the characteristic value to be read */
    void                                    *user_data; /** < user data to be passed to the callback */
    gattc_read_characteristic_value_callback callback;  /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start read short characteristic value procedure, only read mtu-1 size data.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_short_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/**
 *   @brief GATT client start read long characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_long_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/**
 *   @brief GATT client start read characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/** @brief Structure for read blob characteristic value procedure. */
struct gattc_read_blob_characteristic_value_param
{
    uint16_t                                 handle;    /** < attribute handle of the characteristic value to be read */
    uint16_t                                 offset;    /** < offset of the data to be read, if offset is 0, same read long values*/
    void                                    *user_data; /** < user data to be passed to the callback */
    gattc_read_characteristic_value_callback callback;  /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start read blob characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters, add offset value.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_blob_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_read_blob_characteristic_value_param *param);

/** < @brief Structure for read characteristic value write value into buffer. */
struct gattc_characteristic_value_write_info
{
    uint8_t  *write_buffer;     /** < buffer to store the read attribute data */
    uint16_t *write_buffer_len; /** < length of the buffer */
    uint16_t  max_buffer_len;   /** < max length of the buffer */
    void     *user_data;        /** < user data to be passed to the callback */
    /** < callback function to be called when the procedure is updated */
    gattc_read_characteristic_value_callback callback;
};

/**
 *   @brief GATT client start read characteristic value procedure, and write data into buffer.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the characteristic value to be read.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note This procedure is used to read a characteristic value, and write data into buffer,
 */
int ble_host_gattc_read_characteristic_value_write(uint16_t conn_handle, uint16_t cid, uint16_t handle, const struct gattc_characteristic_value_write_info *param);

/** @brief Structure for read or read long or read multiple or read multiple variable length characteristic value procedure. */
struct gattc_read_characteristic_value_finish
{
    uint16_t       handle;    /** < attribute handle of the characteristic value to be read */
    uint16_t       length;    /** < length of the data to be read */
    const uint8_t *buffer;    /** < buffer to store the read data */
    void          *user_data; /** < user data to be passed to the callback */
};

/**
 *  @brief  Typedef for a function pointer to a callback function for read characteristic value finish procedure.
 *
 *  @param[out] conn_handle: Connection handle.
 *  @param[out] cid: Channel ID.
 *  @param[out] err refer to enum ble_err_code.
 *  @param[out] param: Pointer to the structure containing the read characteristic value procedure parameters.
 *
 *  @return none.
 */
typedef void (*ble_host_gattc_read_value_finish_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_read_characteristic_value_finish *param);

/** < @brief Structure for read characteristic value finish. */
struct gattc_characteristic_value_finish_info
{
    void *user_data; /** < user data to be passed to the callback */
    /** < callback function to be called when the procedure is updated */
    ble_host_gattc_read_value_finish_callback callback;
};

/**
 *   @brief GATT client start read characteristic value procedure, and read finish callback.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the characteristic value to be read.
 *   @param[in] param Pointer to the structure containing the read characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note This procedure is used to read a characteristic value, and write data into buffer,
 */
int ble_host_gattc_read_characteristic_value_finish(uint16_t conn_handle, uint16_t cid, uint16_t handle, struct gattc_characteristic_value_finish_info *param);

/** @brief Structure for read Using Characteristic UUID procedure. */
struct gattc_read_using_characteristic_uuid_param
{
    uint16_t                                 start_handle;        /** < start handle of the characteristic range */
    uint16_t                                 end_handle;          /** < end handle of the characteristic range */
    const struct att_uuid                   *characteristic_uuid; /** < characteristic UUID to read */
    void                                    *user_data;           /** < user data to be passed to the callback */
    gattc_read_characteristic_value_callback callback;            /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start read characteristic value by UUID procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic value by UUID parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_using_characteristic_uuid_general(uint16_t conn_handle, uint16_t cid, const struct gattc_read_using_characteristic_uuid_param *param);

/**
 *   @brief GATT client start read characteristic value by UUID procedure.
 *              fix start handle is 0x0001 and end handle is 0xFFFF.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] characteristic_uuid Pointer to the characteristic UUID to read.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the start or end attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_using_characteristic_uuid(uint16_t conn_handle, uint16_t cid, const struct att_uuid *characteristic_uuid, gattc_read_characteristic_value_callback callback,
                                                  void *user_data);

/**
 *   Characteristic Value Write
 *      Write Without Response(Figure 4.13: Write Without Response example)
 *      Signed Write Without Response(Figure 4.14: Signed Write Without Response example)
 *      Write Characteristic Value(Figure 4.15: Write Characteristic Value example)
 *      Write Long Characteristic Value(Figure 4.16: Write Long Characteristic Value example)
 *      Reliable Writes(Figure 4.17: Reliable Writes example)
 */

/**
 *   @brief GATT client start write characteristic value using write without response procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the characteristic value to be written.
 *   @param[in] buffer Pointer to the data to be written.
 *   @param[in] length Length of the data to be written.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note This procedure is used to write a characteristic value using write without response,
 *           if length larger than mtu-3, it will send mtu-3 octet in one packet.
 */
int ble_host_gattc_write_characteristic_value_without_response(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *buffer, uint16_t length);

/**
 *   @brief GATT client start signed write characteristic value using write without response procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] handle Attribute handle of the characteristic value to be written.
 *   @param[in] buffer Pointer to the data to be written.
 *   @param[in] length Length of the data to be written.
 *   @param[in] signature signature: The signature of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note  The Signed Write Without Response sub-procedure shall only be supported on the LE Fixed
 *          Channel Unenhanced ATT bearer.
 *          This procedure is used to write a characteristic value using signed write without response,
 *           if length larger than mtu-15, it will send mtu-15 octet in one packet.
 */
int ble_host_gattc_signed_write_characteristic_value_without_response(uint16_t conn_handle, uint16_t handle, const uint8_t *buffer, uint16_t length, const uint8_t signature[12]);

/** @brief Structure for write characteristic value procedure. */

/**
 *   @brief  Typedef for a function pointer to a callback function for write characteristic value procedure.
 *
 *   @param[out] conn_handle: Connection handle.
 *   @param[out] cid: Channel ID.
 *   @param[out] err refer to enum ble_err_code.
 *   @param[out] user_data: user data.
 *
 *   @return None.
 */
typedef void (*gattc_write_characteristic_value_callback)(uint16_t conn_handle, uint16_t cid, uint32_t err, void *user_data);

/** @brief Structure for write characteristic value procedure. */
struct gattc_write_characteristic_value_param
{
    uint16_t                                  handle;    /** < attribute handle of the characteristic value to be write */
    uint16_t                                  length;    /** < length of the data to be written */
    const uint8_t                            *buffer;    /** < buffer to the data to be written */
    void                                     *user_data; /** < user data to be passed to the callback */
    gattc_write_characteristic_value_callback callback;  /** < callback function to be called when the procedure is updated */
};

/**
 *   @brief GATT client start write characteristic value procedure, only write mtu-3 size data.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_short_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write characteristic value procedure, only write mtu-3 size data.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note param->buffer must used global variable, because it will be used in the program.
 */
int ble_host_gattc_write_short_characteristic_value_global(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write long characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_long_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write long characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note param->buffer must used global variable, because it will be used in the program.
 */
int ble_host_gattc_write_long_characteristic_value_global(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write characteristic value procedure,
 *          if length larger than mtu-3, it will choice write long characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 */
int ble_host_gattc_write_characteristic_value(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write characteristic value procedure,
 *          if length larger than mtu-3, it will choice write long characteristic value procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic value parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 *
 *   @note param->buffer must used global variable, because it will be used in the program.
 */
int ble_host_gattc_write_characteristic_value_global(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   Characteristic Descriptors
 *      Read Characteristic Descriptor(Figure 4.21: Read Characteristic Descriptor example)
 *           similar to read characteristic value.
 *      Read Long Characteristic Descriptor(Figure 4.22: Read Long Characteristic Descriptor example)
 *           similar to read long characteristic value.
 *      Write Characteristic Descriptor(Figure 4.23: Write Characteristic Descriptor example)
 *           similar to write characteristic value.
 *      Write Long Characteristic Descriptor(Figure 4.24: Write Long Characteristic Descriptor example)
 *           similar to write long characteristic value.
 */

/**
 *   @brief GATT client start read short characteristic descriptor procedure, only read mtu-1 size data.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_short_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/**
 *   @brief GATT client start read long characteristic descriptor procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_long_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/**
 *   @brief GATT client start read characteristic descriptor procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the read characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_read_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_read_characteristic_value_param *param);

/**
 *   @brief GATT client start write short characteristic descriptor procedure, only write mtu-3 size data.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_short_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write long characteristic descriptor procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_long_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client start write characteristic descriptor procedure.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] param Pointer to the structure containing the write characteristic descriptor parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_characteristic_descriptor(uint16_t conn_handle, uint16_t cid, const struct gattc_write_characteristic_value_param *param);

/**
 *   @brief GATT client write CCC value, enable or disable notification or indication.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the CCC.
 *   @param[in] notify_enable Enable or disable notification.
 *   @param[in] indicate_enable Enable or disable indication.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_ccc_value(uint16_t conn_handle, uint16_t cid, uint16_t handle, bool notify_enable, bool indicate_enable,
                                   gattc_write_characteristic_value_callback callback, void *user_data);

/**
 *   @brief GATT client write CCC value, default enable notification and indication.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the CCC.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_ccc_value_enable_notify_indicate(uint16_t conn_handle, uint16_t cid, uint16_t handle, gattc_write_characteristic_value_callback callback, void *user_data);

/**
 *   @brief GATT client write CCC value, disable notification and indication.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the CCC.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_ccc_value_disable_notify_indicate(uint16_t conn_handle, uint16_t cid, uint16_t handle, gattc_write_characteristic_value_callback callback,
                                                           void *user_data);

/**
 *   @brief GATT client write CCC value, enable notification.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the CCC.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_ccc_value_enable_notify(uint16_t conn_handle, uint16_t cid, uint16_t handle, gattc_write_characteristic_value_callback callback, void *user_data);

/**
 *   @brief GATT client write CCC value, enable indication.
 *
 *   @param[in] conn_handle Connection handle to be used for the procedure.
 *   @param[in] cid Channel ID.
 *   @param[in] handle Attribute handle of the CCC.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *   @param[in] user_data User data to be passed to the callback.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *            - BLE_GATT_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *            - BLE_GATT_ERR_INVALID_PARAM if the parameter is invalid.
 *            - BLE_GATT_ERR_INVALID_ATTR_HANDLE if the attribute handle is invalid.
 *            - BLE_GATT_ERR_INSUFFICIENT_RESOURCES if there are insufficient resources to start the procedure.
 */
int ble_host_gattc_write_ccc_value_enable_indicate(uint16_t conn_handle, uint16_t cid, uint16_t handle, gattc_write_characteristic_value_callback callback, void *user_data);
