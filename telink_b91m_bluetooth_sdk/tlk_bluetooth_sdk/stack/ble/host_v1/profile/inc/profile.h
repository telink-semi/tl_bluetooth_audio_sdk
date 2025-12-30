/********************************************************************************************************
 * @file    profile.h
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

#define BLE_HOST_PROFILE_INVALID_ATTR_HANDLE 0x0000

#define BLE_HOST_PROFILE_INVALID_CONN_HANDLE 0xFFFF

enum ble_prf_error_code
{
    BLE_PRF_SUCCESS = 0x00,
    BLE_PRF_ERR_INVALID_CONN_HANDLE,
    BLE_PRF_ERR_INPUT_NULL,
    BLE_PRF_ERR_INSUFFICIENT_RESOURCES,
    BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND,
    BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE,
    BLE_PRF_ERR_ATTR_PROPERTIES_NOT_SUPPORTED,
    BLE_PRF_ERR_SPEC_START = 0x80,
};

/** < Profile process type enumeration. */
enum prf_process_type
{
    PRF_PROCESS_INIT,   /* < when register profile node */
    PRF_PROCESS_DEINIT, /* < when unregister profile node */
};

/** < profile bound ACL role enumeration. */
enum prf_used_acl_role
{
    PRF_USED_ACL_ROLE_CENTRAL    = 0x01, /** < only used in ACL central. */
    PRF_USED_ACL_ROLE_PERIPHERAL = 0x02, /** < only used in ACL peripheral. */
    PRF_USED_ACL_ROLE_CONNECT    = PRF_USED_ACL_ROLE_CENTRAL | PRF_USED_ACL_ROLE_PERIPHERAL,
};

/** < profile ACL change state enumeration. */
enum prf_acl_state_change
{
    PRF_ACL_CONNECT,    /** < When ACL connected. */
    PRF_ACL_DISCONNECT, /** < When ACL disconnected. */
    PRF_ACL_RECONNECT,  /** < When ACL reconnect. */
};

/** < profile discovery type enumeration. */
enum prf_disc_type
{
    // if first connect or remote change service(read database hash value changed), need discover all service.
    PRF_DISC_TYPE_SVC, /** < First Discover service, need discover service and characteristic. */
    // if connected paired device or read database hash value is same, only need read characteristic value.
    PRF_DISC_TYPE_RECONNECT, /** < Reconnect service, only need read characteristic value. */
    // current not supported.
    PRF_DISC_TYPE_SERVICE_CHANGED, /** < Received service changed indication, check if service changed. */
};

/** < profile NV state change enumeration. */
enum prf_nv_state_change
{
    PRF_NV_STORE, /** < When store NV. */
    PRF_NV_LOAD,  /** < When load NV. */
};

/** < Profile service id enumeration. */
enum prf_service_id
{
    PRF_SERVICE_ID_NULL = 0x00,
    PRF_BASIC_SERVICE_ID_START,                                         //for GATT, GAP, BAS, DIS.
    PRF_BASIC_SERVICE_ID_END = PRF_BASIC_SERVICE_ID_START + 0x0E,       //used:4, other reserved
    PRF_LE_AUDIO_SERVICE_ID_START,                                      //for LE AUDIO,.
    PRF_LE_AUDIO_SERVICE_ID_END = PRF_LE_AUDIO_SERVICE_ID_START + 0x1F, //used:10, other reserved.
    PRF_HID_SERVICE_ID_START,                                           //for HID, HID ISO.
    PRF_HID_SERVICE_ID_END = PRF_HID_SERVICE_ID_START + 0x0E,           //
    // PRF_CHANNEL_SOUNDING_CLIENT_START,
    // PRF_CHANNEL_SOUNDING_CLIENT_END = PRF_CHANNEL_SOUNDING_CLIENT_START + 0x03,             //used:1 other reserved.
    // PRF_HUMAN_INTERFACE_DEVICE_CLIENT_START,
    // PRF_HUMAN_INTERFACE_DEVICE_CLIENT_ENC = PRF_HUMAN_INTERFACE_DEVICE_CLIENT_START + 0x03, //used:2 other reserved
    // PRF_TEST_PROFILE_CLIENT_START = 0x70,                                           //only user 1
    // PRF_USE_DEFINE_CLIENT_START = 0x71,
    PRF_TELINK_SERVICE_ID_START = 0x80,
    PRF_TELINK_SERVICE_ID_END   = PRF_TELINK_SERVICE_ID_START + 0x07,
    PRF_SERVICE_ID_USER_START   = 0xE0,
};

/** < Profile common event ID enumeration. */
enum prf_common_event_id
{
    PRF_EVT_ID_ACL_CONNECT,
    PRF_EVT_ID_ACL_DISCONNECT,
    PRF_EVT_ID_SECURITY_DONE,

    PRF_EVT_ID_SDP_FOUND,     /** < message for SDP found, refer to struct ble_prf_sdp_info */
    PRF_EVT_ID_SDP_NOT_FOUND, /** < message for SDP not found, refer to struct ble_prf_sdp_info */
    PRF_EVT_ID_SDP_FOUND_END, /** < message for SDP found end, refer to struct ble_prf_sdp_info */
    PRF_EVT_ID_SDP_FINISH,    /** < all service discover finish */
};

enum prf_sec_level
{
    PRF_SEC_LEVEL_NO_ENCRYPT = 0x00,
    PRF_SEC_LEVEL_ENCRYPT    = 0x01,
};

/** < Profile NV parameter structure. */
struct prf_nv_param
{
    //if load nv data, ptr is NV block start address.
    //if store nv data, ptr is current write NV data address.
    uint8_t *dataPtr;

    union
    {
        uint16_t currentTotalLen; //only user in store mode.
        uint16_t nvItemLen;       //only used in load mode.
    };
};

typedef void (*prf_event_callback)(uint16_t conn_handle, enum prf_common_event_id event_id, const void *event_msg);

/** < BLE profile parameters structure. */
struct ble_prf_client_param
{
    uint8_t service_id;    /** < refer to enum prf_service_id */
    uint8_t used_acl_role; /** < refer to enum prf_used_acl_role */
    void (*init)(enum prf_process_type type, const void *param);
    void (*connect)(uint16_t conn_handle, enum prf_acl_state_change state);
    void (*discovery)(uint16_t conn_handle, enum prf_disc_type type);
    int (*store)(uint16_t conn_handle, enum prf_nv_state_change state, struct prf_nv_param *param);
};

struct ble_prf_server_param
{
    uint8_t service_id;    /** < refer to enum prf_service_id */
    uint8_t used_acl_role; /** < refer to enum prf_used_acl_role */
    void (*init)(enum prf_process_type type, const void *param);
    void (*connect)(uint16_t conn_handle, enum prf_acl_state_change state);
    int (*store)(uint16_t conn_handle, enum prf_nv_state_change state, struct prf_nv_param *param);
};

struct ble_prf_client_process
{
    struct
    {
        struct ble_prf_client_process *sle_next; /* next element */
    } next;                                      // SLIST_ENTRY(ble_prf_client_process) next;

    void                              *event_cb;
    const struct ble_prf_client_param *client_param;
};

struct ble_prf_server_process
{
    struct
    {
        struct ble_prf_server_process *sle_next; /* next element */
    } next;                                      // SLIST_ENTRY(ble_prf_server_process) next;

    void                              *event_cb;
    const struct ble_prf_server_param *server_param;
};

struct ble_prf_sdp_info
{
    uint8_t     service_id;
    const char *service_name;
    uint16_t    start_handle; /** < start handle of service only used event PRF_EVT_ID_SDP_FOUND */
    uint16_t    end_handle;   /** < end handle of service only used event PRF_EVT_ID_SDP_FOUND */
};

typedef void (*prf_read_callback)(uint16_t conn_handle, uint32_t err);
typedef void (*prf_write_callback)(uint16_t conn_handle, uint32_t err);

/**
 *   @brief  Initialize BLE profile module.
 *
 *   @param[in]  param  Pointer to BLE profile initialization parameter, refer to struct ble_prf_init_param.
 *
 *   @return none.
 */
void ble_prf_initial(void);

/**
 *   @brief  Register BLE profile event handler.
 *
 *   @param[in]  event_cb  Pointer to BLE profile event callback function.
 *
 *   @return none.
 */
void ble_prf_register_event_handler(prf_event_callback event_cb);

/**
 *   @brief  Register BLE profile client module.
 *
 *   @param[in]  p_module  Pointer to client module, refer to struct ble_prf_client_process.
 *   @param[in]  param     Pointer to client parameter.
 *
 *   @return none.
 */
void ble_prf_register_client_module(struct ble_prf_client_process *p_module, const void *param);

/**
 *   @brief  Unregister BLE profile client module.
 *
 *   @param[in]  p_module  Pointer to client module, refer to struct ble_prf_client_process.
 *
 *   @return none.
 */
void ble_prf_unregister_client_module(struct ble_prf_client_process *p_module, const void *param);

/**
 *    @brief  Register BLE profile server module.
 *
 *    @param[in]  p_module  Pointer to server module, refer to struct ble_prf_server_process.
 *    @param[in]  param     Pointer to server parameter.
 *
 *    @return none.
 */
void ble_prf_register_server_module(struct ble_prf_server_process *p_module, const void *param);

/**
 *    @brief  Unregister BLE profile server module.
 *
 *    @param[in]  p_module  Pointer to server module, refer to struct ble_prf_server_process.
 *
 *    @return none.
 */
void ble_prf_unregister_server_module(struct ble_prf_server_process *p_module, const void *param);

/**
 * @brief       Reports that SDP discovery finished for the given service.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   service_id  - profile service ID.
 * @param[in]   service_name- pointer to service name string.
 * @return      none.
 * @note        Sent after all characteristics/descriptor discovery completes.
 */
void ble_prf_report_sdp_found_end_event(uint16_t conn_handle, uint8_t service_id, const char *service_name);

/**
 * @brief       Reports that the requested service was not found during SDP discovery.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   service_id  - profile service ID.
 * @param[in]   service_name- pointer to service name string.
 * @return      none.
 * @note        Allows profiles to handle missing services per connection.
 */
void ble_prf_report_sdp_not_found_event(uint16_t conn_handle, uint8_t service_id, const char *service_name);

/**
 * @brief       Reports that a service was found and provides its handle range.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   service_id  - profile service ID.
 * @param[in]   service_name- pointer to service name string.
 * @param[in]   start_handle- start handle of the service.
 * @param[in]   end_handle  - end handle of the service.
 * @return      none.
 * @note        Typically followed by characteristic discovery.
 */
void ble_prf_report_sdp_found_event(uint16_t conn_handle, uint8_t service_id, const char *service_name, uint16_t start_handle, uint16_t end_handle);

/**
 * @brief       Parameters used when reading attribute values via the profile helper.
 */
struct ble_prf_read_value
{
    uint8_t          *data;         /** < buffer to store the value */
    uint16_t         *length;       /** < pointer to actual length field */
    prf_read_callback callback;     /** < read completion callback */
    uint16_t          max_data_len; /** < maximum buffer length */
};

/**
 * @brief       Reads an attribute value using ATT read requests.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   handle      - attribute handle to read.
 * @param[in]   prf_read_param - pointer to read parameters structure.
 * @return      BLE_PRF_SUCCESS if accepted, otherwise an error code.
 * @note        Callback is invoked asynchronously when the operation completes.
 */
int ble_prf_read_attribute_value(uint16_t conn_handle, uint16_t handle, struct ble_prf_read_value *prf_read_param);

/**
 * @brief       Writes an attribute value using ATT write requests.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   handle      - attribute handle to write.
 * @param[in]   data        - pointer to the value buffer.
 * @param[in]   length      - number of bytes to write.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if accepted, otherwise an error code.
 * @note        Requires the peer attribute to support Write Request property.
 */
int ble_prf_write_attribute_value(uint16_t conn_handle, uint16_t handle, const uint8_t *data, uint16_t length, prf_write_callback callback);

/**
 * @brief       Writes an attribute value using ATT Write Command (no response).
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   handle      - attribute handle to write.
 * @param[in]   data        - pointer to the value buffer.
 * @param[in]   length      - number of bytes to write.
 * @return      BLE_PRF_SUCCESS if accepted, otherwise an error code.
 * @note        Caller does not receive completion status from the peer.
 */
int ble_prf_write_attribute_value_without_response(uint16_t conn_handle, uint16_t handle, const uint8_t *data, uint16_t length);
