/********************************************************************************************************
 * @file    ble_gap_pa_sync.h
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

/** < Structure for periodic advertising synchronization parameters. */
struct ble_host_gap_pa_sync_param
{
    uint16_t sync_handle;   /** < The handle of the periodic advertising synchronization instance. */
    uint8_t  adv_sid;       /** < The advertising SID to synchronize with. */
    uint8_t  adv_addr_type; /** < The address type of the peer device. */
    uint8_t  adv_addr[6];   /** < The peer device address. */
    uint16_t interval;      /** < unit:1.25ms */
};

/**
 *   @brief Callback function for periodic advertising synchronization established.
 *
 *   This function is called when a periodic advertising synchronization is established.
 *
 *   @param[in] status The status of the synchronization, 0 means success, otherwise means failure.
 *   @param[in] param The parameters of the synchronization.
 */
typedef void (*ble_host_gap_pa_sync_established_callback)(uint8_t status, struct ble_host_gap_pa_sync_param *param);

/**
 *   @brief Callback function for periodic advertising synchronization lost.
 *
 *   This function is called when a periodic advertising synchronization is lost.
 *
 *   @param[in] sync_handle The handle of the periodic advertising synchronization instance.
 */
typedef void (*ble_host_gap_pa_sync_lost_callback)(uint16_t sync_handle);

/**
 *   @brief Callback function for periodic advertising synchronization report advertising data.
 *
 *   This function is called when a periodic advertising synchronization report advertising data and adv data is complete.
 *
 *   @param[in] sync_handle The handle of the periodic advertising synchronization instance.
 *   @param[in] adv_data The advertising data reported by the peer device.
 *   @param[in] data_len The length of the advertising data.
 */
typedef void (*ble_host_gap_pa_sync_report_adv_callback)(uint16_t sync_handle, const uint8_t *adv_data, uint16_t data_len);

/**
 *   @brief Callback function for periodic advertising synchronization PAST received.
 *
 *   This function is called when a periodic advertising synchronization PAST received.
 *
 *   @param[in] status The status of the synchronization, 0 means success, otherwise means failure.
 *   @param[in] conn_handle The handle of the ACL connection.
 *   @param[in] service_data The service data received.
 *   @param[in] param The parameters of the synchronization.
 */
typedef void (*ble_host_gap_pa_sync_past_received_callback)(uint8_t status, uint16_t conn_handle, uint16_t service_data, struct ble_host_gap_pa_sync_param *param);

/** < Structure for periodic advertising synchronization callback functions. */
struct ble_host_gap_pa_sync_callbacks
{
    ble_host_gap_pa_sync_established_callback   established_cb; /**< Callback function for periodic advertising synchronization established. */
    ble_host_gap_pa_sync_lost_callback          lost_cb;        /**< Callback function for periodic advertising synchronization lost. */
    ble_host_gap_pa_sync_report_adv_callback    report_adv_cb;  /**< Callback function for periodic advertising synchronization report advertising data. */
    ble_host_gap_pa_sync_past_received_callback past_cb;        /**< Callback function for periodic advertising synchronization PAST received. */
};

/**
 *   @brief Initialize the periodic advertising synchronization module.
 *
 *   This function should be called before any other periodic advertising synchronization functions.
 */
void ble_host_gap_pa_sync_init(void);

/**
 *   @brief Create a periodic advertising synchronization instance.
 *
 *   This function creates a periodic advertising synchronization instance with the specified parameters.
 *
 *   @param[in] sid The advertising set ID to synchronize with.
 *   @param[in] addr_type The address type of the peer device.
 *   @param[in] addr The peer device address.
 *   @param[in] timeout The timeout value for the synchronization, unit: 1ms.
 *   @param[in] callbacks The callback functions for the synchronization, must not be NULL and global pointer.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan parameters successfully, otherwise return error code.
 */
int ble_host_gap_pa_sync_create(uint8_t sid, uint8_t addr_type, const uint8_t addr[6], uint16_t timeout, const struct ble_host_gap_pa_sync_callbacks *callbacks);

/**
 *   @brief Cancel a periodic advertising synchronization instance.
 *
 *   This function cancels a periodic advertising synchronization instance.
 *
 *   @return BLE_HOST_ERR_SUCC if cancel periodic advertising synchronization successfully, otherwise return error code.
 *
 *   @note If there is periodic advertising synchronization instance, callback established_cb and status is cancel by host.
 */
int ble_host_gap_pa_sync_cancel(void);

/**
 *   @brief Terminate a periodic advertising synchronization instance.
 *
 *   This function terminates a periodic advertising synchronization instance.
 *
 *   @param[in] sync_handle The handle of the periodic advertising synchronization instance to terminate.
 *
 *   @return BLE_HOST_ERR_SUCC if terminate periodic advertising synchronization successfully, otherwise return error code.
 */
int ble_host_gap_pa_sync_terminate(uint16_t sync_handle);

/**
 *   @brief Set PAST in current acl connection parameters.
 *
 *   This function sets PAST in current acl connection parameters.
 *
 *   @param[in] acl_conn_handle The handle of the acl connection.
 *   @param[in] timeout The timeout value for the synchronization, unit: 1ms.
 *   @param[in] callbacks The callback functions for the synchronization, must not be NULL and global pointer.
 *
 *   @return BLE_HOST_ERR_SUCC if set PAST successfully, otherwise return error code.
 */
int ble_host_gap_pa_sync_set_past(uint16_t acl_conn_handle, uint16_t timeout, const struct ble_host_gap_pa_sync_callbacks *callbacks);

/**
 *   @brief Cancel PAST in current acl connection parameters.
 *
 *   This function cancels PAST in current acl connection parameters.
 *
 *   @param[in] acl_conn_handle The handle of the acl connection.
 *
 *   @return BLE_HOST_ERR_SUCC if cancel PAST successfully, otherwise return error code.
 */
int ble_host_gap_pa_sync_cancel_past(uint16_t acl_conn_handle);

/**
 *   @brief Check a periodic advertising synchronization handle is valid or not.
 *
 *   @param[in] sync_handle The handle of the periodic advertising synchronization instance to check.
 *
 *   @return true if the handle is valid, otherwise false.
 */
bool ble_host_gap_pa_sync_is_valid_handle(uint16_t sync_handle);
