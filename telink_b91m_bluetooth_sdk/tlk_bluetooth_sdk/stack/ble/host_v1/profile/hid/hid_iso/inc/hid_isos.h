/********************************************************************************************************
 * @file    hid_isos.h
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
#define HID_ISO_HYBRID_MODE_ULL_REPORT_COUNT 6

struct hybrid_mode_reports_support
{
    uint8_t report_id;
    uint8_t report_type;
    uint8_t confirmation_supp;
    uint8_t repetition_supp;
};

struct hybrid_mode_reports_enable
{
    uint8_t report_id;
    uint8_t report_type;
    uint8_t confirmation_enable;
    uint8_t repetition_enable;
};

struct ble_hid_isos_register_param
{
    uint8_t                             device_mode_change_supp;
    union hid_iso_supp_report_intervals supp_intervals;

    uint8_t max_sdu_size_for_input;
    uint8_t max_sdu_size_for_output;

    uint8_t                            hybrid_supp_report_count;
    struct hybrid_mode_reports_support hybrid_supp_report[HID_ISO_HYBRID_MODE_ULL_REPORT_COUNT];
};

enum ble_hid_isos_event_id
{
    HID_ISOS_EVT_SELECT_HYBRID_MODE,  //refer to struct ble_hid_isos_selectHybridModeEvt
    HID_ISOS_EVT_SELECT_DEFAULT_MODE, //refer to NULL.
};

typedef void (*ble_hid_isos_event_callback)(uint16_t conn_handle, enum ble_hid_isos_event_id event_id, const void *event_msg);

struct ble_hid_isos_selectHybridModeEvt //Event ID: HID_ISOS_EVT_SELECT_HYBRID_MODE
{
    uint32_t report_interval; //unit 1us
    uint8_t  CIG_ID;
    uint8_t  CIS_ID;

    uint8_t                           enable_count; //range 0 to 2
    struct hybrid_mode_reports_enable enable_reports[HID_ISO_HYBRID_MODE_ULL_REPORT_COUNT];
};

/**
 * @brief       Registers the HID ISO control server with the provided parameters.
 * @param[in]   param   - pointer to the server registration parameters.
 * @return      none.
 * @note        Call during initialization before enabling the service.
 */
void ble_hid_iso_register_HID_ISO_control_server(const struct ble_hid_isos_register_param *param);

/**
 * @brief       Unregisters the HID ISO control server and releases related resources.
 * @return      none.
 * @note        Use when disabling the service or shutting down the stack.
 */
void ble_hid_iso_unregister_HID_ISO_control_server(void);

/**
 * @brief       Registers the HID ISO server event callback.
 * @param[in]   event_callback  - callback invoked for HID ISO server events.
 * @return      none.
 * @note        Only one callback can be active at a time; registering again overwrites the previous one.
 */
void ble_hid_register_HID_ISO_server_event_callback(ble_hid_isos_event_callback event_callback);

/**
 * @brief       Extracts the report interval bit mask from the encoded value.
 * @param[in]   intervalBit - encoded interval bit value.
 * @return      Decoded interval bit mask.
 * @note        Helper for interpreting characteristic values.
 */
uint32_t ble_hid_iso_getReportIntervalBit(uint16_t intervalBit);

/**
 * @brief       Converts an encoded interval bit into its standardized representation.
 * @param[in]   intervalBit - encoded interval bit value.
 * @return      Converted interval bit mask.
 * @note        Use before programming intervals into characteristics.
 */
uint32_t ble_hid_iso_convertReportIntervalBit(uint16_t intervalBit);

/**
 * @brief       Converts a report interval from microseconds to the encoded format.
 * @param[in]   interval    - interval in microseconds.
 * @return      Encoded interval value.
 * @note        Used to build control-point payloads.
 */
uint8_t ble_hid_iso_convertReportInterval(uint32_t interval);

/**
 * @brief       Sends a Select Hybrid Mode indication to the peer.
 * @param[in]   connHandle  - ACL connection handle.
 * @param[in]   param       - pointer to the select hybrid parameters.
 * @param[in]   cb          - confirmation callback invoked after peer acknowledges.
 * @return      BLE_PRF_SUCCESS if the indication is queued, otherwise an error code.
 * @note        Requires the peer to support the hybrid-mode control characteristic.
 */
int ble_hid_isos_indSelectHybridMode(uint16_t connHandle, const struct ble_hid_isos_select_hybrid_param *param, gatts_confirm_callback cb);

/**
 * @brief       Sends a Select Default Mode indication to the peer.
 * @param[in]   connHandle  - ACL connection handle.
 * @param[in]   cb          - confirmation callback invoked after peer acknowledges.
 * @return      BLE_PRF_SUCCESS if the indication is queued, otherwise an error code.
 * @note        Use to revert from hybrid mode back to default operation.
 */
int ble_hid_isos_indSelectDefaultMode(uint16_t connHandle, gatts_confirm_callback cb);

/**
 * @brief       Sends HID ISO hybrid data over the specified ACL connection.
 * @param[in]   acl_handle  - ACL connection handle.
 * @param[in]   report_id   - report identifier for the payload.
 * @param[in]   data        - pointer to the data buffer.
 * @param[in]   length      - number of bytes to send.
 * @return      BLE_PRF_SUCCESS if the data is queued successfully, otherwise an error code.
 * @note        Data length must not exceed the negotiated SDU size.
 */
int ble_hid_iso_send_hybrid_data(uint16_t acl_handle, uint8_t report_id, uint8_t *data, uint16_t length);