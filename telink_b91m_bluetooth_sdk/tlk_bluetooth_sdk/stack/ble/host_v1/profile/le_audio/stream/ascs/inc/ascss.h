/********************************************************************************************************
 * @file    ascss.h
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
// ASCSS: Audio Stream Control Service Server.

enum ble_ascss_event_id
{
    ASCSS_EVENT_ID_ACL_CONNECT,
    ASCSS_EVENT_ID_RECV_CODEC_CONFIG,
    ASCSS_EVENT_ID_ASE_CTRL_CCC_DISABLE,
    ASCSS_EVENT_ID_ASE_CTRL_CCC_ENABLE,
    ASCSS_EVENT_ID_ACL_DISCONNECT,

    ASCSS_EVENT_ID_ENTER_ENABLING_STATE,
    ASCSS_EVENT_ID_ENTER_RELEASING_STATE,
    ASCSS_EVENT_ID_ENTER_IDLE_STATE,
    ASCSS_EVENT_ID_SOURCE_READY,
    ASCSS_EVENT_ID_SINK_READY,
};

struct ble_ascss_enter_state_event
{
    uint8_t ase_num;
    uint8_t ase_id[0];
};

// latency, delay unit always used in ms.
#define BLE_ASCS_SERVER_CONFIG(trans_num, latency, min_delay, max_delay) \
    {                                                                    \
        .framing                          = LEA_ASCS_UNFRAMED_SUPPORTED, \
        .preferred_phy                    = LEA_ASCS_PREPERRED_PHY_2M,   \
        .retransmission_number            = trans_num,                   \
        .max_transport_latency            = latency,                     \
        .presentation_delay_min           = min_delay * 1000,            \
        .presentation_delay_max           = max_delay * 1000,            \
        .preferred_presentation_delay_min = min_delay * 1000,            \
        .preferred_presentation_delay_max = max_delay * 1000,            \
    }

struct ble_ascs_config_param
{
    uint8_t  framing;                          /** < 0: unframed supported, 1: unframed not supported. usually 0. */
    uint8_t  preferred_phy;                    /** < BIT(0): LE 1M, BIT(1): LE 2M, BIT(2): LE Coded */
    uint8_t  retransmission_number;            /** < Range: 0x00 - 0xFF */
    uint16_t max_transport_latency;            /** < Unit: ms, Range: 0x0005-0x0FA0 */
    uint32_t presentation_delay_min;           /** < Unit: us */
    uint32_t presentation_delay_max;           /** < Unit: us */
    uint32_t preferred_presentation_delay_min; /** < Unit: us */
    uint32_t preferred_presentation_delay_max; /** < Unit: us */
};

typedef void (*ble_ascss_event_callback)(uint16_t conn_handle, enum ble_ascss_event_id event_id, const void *event_msg);

struct ble_ascss_register_param
{
    struct ble_ascs_config_param config_param;
};

/**
 *   @brief Register the Audio Stream Control Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the ASCSS module.
 *
 *   @return None.
 */
void ble_lea_register_ASCS_control_server(const struct ble_ascss_register_param *param);

/**
 *   @brief Set the event callback for the Audio Stream Control Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_lea_register_ASCS_event_callback(ble_ascss_event_callback callback);
