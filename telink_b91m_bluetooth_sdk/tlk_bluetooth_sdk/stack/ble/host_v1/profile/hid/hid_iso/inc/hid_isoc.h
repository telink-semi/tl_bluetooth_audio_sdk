/********************************************************************************************************
 * @file    hid_isoc.h
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
enum ble_hid_isoc_event_id
{
    HID_ISOC_EVT_SELECT_HYBRID_MODE,  //refer to struct ble_hid_isoc_selectHybridModeEvt
    HID_ISOC_EVT_SELECT_DEFAULT_MODE, //refer to NULL.
};

struct ble_hid_isoc_selectHybridModeEvt //Event ID: HID_ISOS_EVT_SELECT_HYBRID_MODE
{
    uint32_t report_interval; //unit 1us
    uint8_t  max_sdu_size_for_input;
    uint8_t  max_sdu_size_for_output;

    uint8_t                           enable_count; //range 0 to 2
    struct hybrid_mode_reports_enable enable_reports[HID_ISO_HYBRID_MODE_ULL_REPORT_COUNT];
};

struct hid_iso_properties_read
{
    uint8_t                             device_mode_change_supp;
    union hid_iso_supp_report_intervals supp_interval;
    uint8_t                             max_sdu_size_for_input;
    uint8_t                             prefer_sdu_size_for_input;
    uint8_t                             max_sdu_size_for_output;
    uint8_t                             prefer_sdu_size_for_output;

    uint8_t reports_count;

    struct supported_report_context
    {
        uint8_t report_id;
        uint8_t report_type;
        uint8_t confirmation_supp;
        uint8_t repetition_supp;
    } reports[HID_ISO_HYBRID_MODE_ULL_REPORT_COUNT];
};

typedef void (*ble_hid_isoc_event_callback)(uint16_t conn_handle, enum ble_hid_isoc_event_id event_id, const void *event_msg);

struct ble_hid_isosc_register_param
{
};

struct hybrid_mode_ctx
{
    uint16_t interval;
    uint8_t  index;
    bool     repeat_en;
    bool     confirm_en;
};

/**
 * @brief       Reads the properties of the HID ISO client.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[out]  properties_read - pointer to the properties read structure.
 * @return      BLE_PRF_SUCCESS if the properties are read successfully, otherwise an error code.
 */
int ble_hid_isoc_get_properties(uint16_t conn_handle, struct hid_iso_properties_read *properties_read);

/**
 * @brief       Registers the HID ISO control client module.
 * @param[in]   param   - pointer to the control client register parameters.
 * @return      none.
 * @note        Call during initialization before establishing connections.
 */
void ble_hid_register_HID_ISO_control_client(const struct ble_hid_isosc_register_param *param);

/**
 * @brief       Registers the HID ISO client event callback.
 * @param[in]   event_callback  - callback invoked for HID ISO events.
 * @return      none.
 * @note        Only one callback can be registered at a time.
 */
void ble_hid_register_HID_ISO_client_event_callback(ble_hid_isoc_event_callback event_callback);

/**
 * @brief       Sends a write request selecting hybrid mode on the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   param       - pointer to hybrid mode parameters.
 * @param[in]   write_cb    - completion callback for the write request.
 * @return      BLE_PRF_SUCCESS if the request is started, otherwise an error code.
 * @note        Requires the peer to support hybrid-mode control characteristics.
 */
int ble_hid_isoc_write_select_hybrid_mode(uint16_t conn_handle, struct ble_hid_isos_select_hybrid_param *param, prf_write_callback write_cb);

/**
 * @brief       Sends a write request selecting default mode on the peer.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   write_cb    - completion callback for the write request.
 * @return      BLE_PRF_SUCCESS if the request is started, otherwise an error code.
 * @note        Use when reverting from hybrid to default transmission mode.
 */
int ble_hid_isoc_write_select_default_mode(uint16_t conn_handle, prf_write_callback write_cb);

/**
 * @brief       Selects the hybrid mode locally and applies the provided context.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ctx         - pointer to hybrid mode runtime context.
 * @return      none.
 * @note        Should be called after hybrid-mode negotiation succeeds.
 */
void ble_hid_isoc_select_hybrid(uint16_t conn_handle, struct hybrid_mode_ctx *ctx);

/**
 * @brief       Selects the default mode locally for the specified connection.
 * @param[in]   conn_handle - ACL connection handle.
 * @return      none.
 * @note        Use to stop hybrid mode and resume default behavior.
 */
void ble_hid_isoc_select_default(uint16_t conn_handle);
