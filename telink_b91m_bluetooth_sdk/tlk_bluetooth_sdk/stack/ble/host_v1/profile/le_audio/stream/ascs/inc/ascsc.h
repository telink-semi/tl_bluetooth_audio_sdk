/********************************************************************************************************
 * @file    ascsc.h
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
 * @brief ASCS Client error codes
 */
enum ascs_error_code
{
    BLE_ASCSC_ERR_START = 0x80,
    BLE_ASCSC_ERR_INVALID_ASE_STATE,
    BLE_ASCSC_ERR_CONTROL_POINT_NOT_FOUND,
    BLE_ASCSC_ERR_CODEC_INVALID_LATENCY,
    BLE_ASCSC_ERR_CODEC_INVALID_PHY,
    BLE_ASCSC_ERR_CODEC_INVALID_ALLOCATION,
    BLE_ASCSC_ERR_CODEC_INVALID_SAMPLING_FREQ,
    BLE_ASCSC_ERR_CODEC_INVALID_FRAME_DURATION,
    BLE_ASCSC_ERR_CODEC_INVALID_PER_CODEC_FRAME,

};

/**
 * @brief ASCS Client event IDs
 */
enum ble_ascsc_event_id
{
    ASCSC_EVT_ID_IDLE,             // refer to NULL.
    ASCSC_EVT_ID_CODEC_CONFIGURED, // refer to struct ascsc_codec_config_param
    ASCSC_EVT_ID_QOS_CONFIGURED,   // refer to NULL.
    ASCSC_EVT_ID_ENABLING,         // refer to NULL.
    ASCSC_EVT_ID_STREAMING,        // refer to NULL.
    ASCSC_EVT_ID_DISABLING,        // refer to NULL.
    ASCSC_EVT_ID_RELEASING,        // refer to NULL.

    ASCSC_EVT_ID_RECV_OPERATE_START = ASCSC_EVT_ID_RELEASING,
    // all receive operation parameter refer to struct ascsc_recv_operate_param.
    ASCSC_EVT_ID_RECV_CONFIG_CODEC,
    ASCSC_EVT_ID_RECV_CONFIG_QOS,
    ASCSC_EVT_ID_RECV_ENABLE,
    ASCSC_EVT_ID_RECV_RECEIVER_START_READY,
    ASCSC_EVT_ID_RECV_DISABLE,
    ASCSC_EVT_ID_RECV_RECEIVER_STOP_READY,
    ASCSC_EVT_ID_RECV_UPDATE_METADATA,
    ASCSC_EVT_ID_RECV_RELEASE,

    // all state parameter check failed, parameter is NULL.
    ASCSC_EVT_ID_IDLE_FAILED,
    ASCSC_EVT_ID_CODEC_CONFIGURED_FAILED,
    ASCSC_EVT_ID_QOS_CONFIGURED_FAILED,
    ASCSC_EVT_ID_ENABLING_FAILED,
    ASCSC_EVT_ID_STREAMING_FAILED,
    ASCSC_EVT_ID_DISABLING_FAILED,
    ASCSC_EVT_ID_RELEASING_FAILED,
};

/**
 * @brief ASCS Client event callback function type
 */
typedef void (*ble_ascsc_event_callback)(uint16_t conn_handle, enum ble_ascsc_event_id event_id, const void *event_msg);

/**
 * @brief Structure containing parameters for registering the ASCS client
 */
struct ble_ascsc_register_param
{
};

struct ble_ase_codec_config
{
    uint8_t                            latency; /** < refer to enum lea_ascs_target_latency */
    uint8_t                            phy;     /** < refer to enum lea_ascs_target_phy  */
    struct lea_codec_id                codec_id;
    struct lea_codec_spec_config_param codec_cfg;
};

#define LEA_ASE_CODEC_CONFIG(lc3_id, target_latency, target_phy, allocation) \
    (&(struct ble_ase_codec_config){                                         \
        .latency   = target_latency,                                         \
        .phy       = target_phy,                                             \
        .codec_id  = LEA_INIT_CODEC_ID_LC3,                                  \
        .codec_cfg = LEA_CODEC_CFG_##lc3_id(allocation),                     \
    })

#define LEA_ASE_CODEC_CONFIG_LOW_LATENCY(lc3_id, allocation)      LEA_ASE_CODEC_CONFIG(lc3_id, ASCS_TARGET_LOW_LATENCY, ASCS_TARGET_LE_2M_PHY, allocation)
#define LEA_ASE_CODEC_CONFIG_BALANCED(lc3_id, allocation)         LEA_ASE_CODEC_CONFIG(lc3_id, ASCS_TARGET_BALANCED_LATENCY_AND_RELIABILITY, ASCS_TARGET_LE_2M_PHY, allocation)
#define LEA_ASE_CODEC_CONFIG_HIGH_RELIABILITY(lc3_id, allocation) LEA_ASE_CODEC_CONFIG(lc3_id, ASCS_TARGET_HIGH_RELIABILITY, ASCS_TARGET_LE_2M_PHY, allocation)

struct ble_ase_qos_config
{
    uint8_t  cig_id;                /** < CIG ID */
    uint8_t  cis_id;                /** < CIS ID */
    uint32_t sdu_interval;          /** < SDU interval */
    uint8_t  framing;               /** < Framing */
    uint8_t  phy;                   /** < PHY */
    uint16_t max_sdu_size;          /** < Max SDU size */
    uint8_t  rtn;                   /** < retransmission number */
    uint16_t max_transport_latency; /** < Max transport latency */
    uint32_t presentation_delay;    /** < Presentation delay */
};

struct ble_ase_qos_config_default
{
    uint8_t cig_id; /** < CIG ID */
    uint8_t cis_id; /** < CIS ID */
};

#define LEA_ASE_QOS_CONFIG_DEFAULT(cig_id_num, cis_id_num) \
    {                                                      \
        .cig_id = cig_id_num,                              \
        .cis_id = cis_id_num,                              \
    }


/**
 *   @brief Register the LE Audio Audio Stream Control Service (ASCS) client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the ASCS module.
 *
 *   @return None.
 */
void ble_lea_register_ASCS_control_client(const struct ble_ascsc_register_param *param);

/**
 *   @brief Register the LE Audio Audio Stream Control Service (ASCS) client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_ASCS_client_event_callback(ble_ascsc_event_callback event_callback);

struct ascsc_enter_idle_config
{
    uint8_t ase_id;
};

struct ascsc_enter_codec_config
{
    uint8_t  ase_id;
    bool     unframed_supp;
    uint8_t  preferred_phy;
    uint8_t  retransmission_number;
    uint16_t max_transport_latency;
    uint32_t presentation_delay_min;
    uint32_t presentation_delay_max;
    uint32_t preferred_presentation_delay_min;
    uint32_t preferred_presentation_delay_max;
};

struct ascsc_enter_qos_config
{
    uint8_t ase_id;
    uint8_t cig_id;
    uint8_t cis_id;
};

struct ascsc_enter_enable
{
    uint8_t ase_id;
};

struct ascsc_enter_stream
{
    uint8_t ase_id;
};

struct ascsc_releasing
{
    uint8_t ase_id;
};

struct ascsc_receive_operate_result
{
    uint8_t ase_id;
    uint8_t response_code;
    uint8_t reason;
};

bool ble_ascsc_check_ase_id(uint16_t conn_handle, uint8_t ase_id);

enum ascsc_ase_state
{
    ASCSC_ASE_STATE_CONFIG_CODEC,
    ASCSC_ASE_STATE_CONFIG_QOS,
    ASCSC_ASE_STATE_ENABLE,
    ASCSC_ASE_STATE_RECEIVER_START_READY,
    ASCSC_ASE_STATE_DISABLE,
    ASCSC_ASE_STATE_RECEIVER_STOP_READY,
    ASCSC_ASE_STATE_UPDATE_METADATA,
    ASCSC_ASE_STATE_RELEASE,
    ASCSC_ASE_STATE_IDLE,
};

/**
 * @brief       Checks whether a specific ASE is currently in the expected state.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[in]   state       - expected ASE state.
 * @return      true if the ASE is in the specified state, false otherwise.
 */
bool ble_ascsc_check_ase_state(uint16_t conn_handle, uint8_t ase_id, enum ascsc_ase_state state);

/**
 * @brief       Sets the target codec configuration for an ASE locally.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[in]   p_codec     - pointer to the codec configuration.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 * @note        Must be called before `ble_ascsc_write_config_codec`.
 */
int ble_ascsc_set_ase_config_codec(uint16_t conn_handle, uint8_t ase_id, const struct ble_ase_codec_config *p_codec);

/**
 * @brief       Writes the previously configured codec settings to the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 * @note        Requires a prior call to `ble_ascsc_set_ase_config_codec`.
 */
int ble_ascsc_write_config_codec(uint16_t conn_handle);

/**
 * @brief       Sets the target QoS configuration for an ASE locally.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[in]   p_qos       - pointer to the QoS configuration.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 * @note        Must be called before `ble_ascsc_write_config_qos`.
 */
int ble_ascsc_set_ase_config_qos(uint16_t conn_handle, uint8_t ase_id, const struct ble_ase_qos_config *p_qos);

/**
 * @brief       Sets the default QoS configuration for an ASE locally.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[in]   p_qos       - pointer to the default QoS configuration.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 */
int ble_ascsc_set_ase_config_qos_default(uint16_t conn_handle, uint8_t ase_id, const struct ble_ase_qos_config_default *p_qos);

/**
 * @brief       Writes the previously configured QoS settings to the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 * @note        Requires a prior call to `ble_ascsc_set_ase_config_qos`.
 */
int ble_ascsc_write_config_qos(uint16_t conn_handle);

/**
 * @brief       Sets ASE enable metadata locally before sending the enable command.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[in]   p_enable_metadata - pointer to metadata parameters.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 */
int ble_ascsc_set_ase_enable(uint16_t conn_handle, uint8_t ase_id, const struct lea_metadata_param *p_enable_metadata);

/**
 * @brief       Writes the ASE enable command to the peer using previously stored metadata.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_ascsc_write_enable(uint16_t conn_handle);

/**
 * @brief       Sets the ASE receive start-ready flag locally.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 */
int ble_ascsc_set_ase_receive_start_ready(uint16_t conn_handle, uint8_t ase_id);

/**
 * @brief       Writes the receiver start-ready command to the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_ascsc_write_receiver_start_ready(uint16_t conn_handle);

/**
 * @brief       Sets the ASE release flag locally.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @return      BLE_PRF_SUCCESS if stored successfully, otherwise an error code.
 */
int ble_ascsc_set_ase_release(uint16_t conn_handle, uint8_t ase_id);

/**
 * @brief       Writes the ASE release command to the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_ascsc_write_release(uint16_t conn_handle);

/**
 * @brief       Retrieves sink ASE identifiers for the specified connection.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_num     - maximum number of ASE IDs to return.
 * @param[out]  out_aes_id  - buffer to store the ASE IDs.
 * @return      true if at least one ASE ID is returned, false otherwise.
 */
bool ble_ascsc_get_sink_ase_id(uint16_t conn_handle, uint8_t ase_num, uint8_t out_aes_id[]);

/**
 * @brief       Retrieves source ASE identifiers for the specified connection.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_num     - maximum number of ASE IDs to return.
 * @param[out]  out_aes_id  - buffer to store the ASE IDs.
 * @return      true if at least one ASE ID is returned, false otherwise.
 */
bool ble_ascsc_get_source_ase_id(uint16_t conn_handle, uint8_t ase_num, uint8_t out_aes_id[]);

/**
 * @brief       Checks whether an ASE identifier is valid for the connection.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @return      true if the ASE ID is valid, false otherwise.
 */
bool ble_ascsc_ase_id_is_valid(uint16_t conn_handle, uint8_t ase_id);

/**
 * @brief       Checks whether an ASE identifier represents a sink ASE.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @return      true if the ASE is a sink, false otherwise.
 */
bool ble_ascsc_ase_id_is_sink(uint16_t conn_handle, uint8_t ase_id);

/**
 * @brief       Checks whether an ASE identifier represents a source ASE.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @return      true if the ASE is a source, false otherwise.
 */
bool ble_ascsc_ase_id_is_source(uint16_t conn_handle, uint8_t ase_id);

/**
 * @brief       ASE codec parameter container for direction-specific settings.
 */
struct ble_ascsc_ase_codec_param
{
    struct
    {
        uint16_t max_sdu;
        uint16_t max_transport_latency;
        uint8_t  retransmission_number;
        uint8_t  preferred_phy;
    } c_to_p, p_to_c;
};

/**
 * @brief       Retrieves the ASE codec parameters negotiated with the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_id      - ASE identifier.
 * @param[out]  p_param     - pointer to structure receiving the parameters.
 * @return      BLE_PRF_SUCCESS if parameters are available, otherwise an error code.
 */
int ble_ascsc_get_ase_codec_param(uint16_t conn_handle, uint8_t ase_id, struct ble_ascsc_ase_codec_param *p_param);
