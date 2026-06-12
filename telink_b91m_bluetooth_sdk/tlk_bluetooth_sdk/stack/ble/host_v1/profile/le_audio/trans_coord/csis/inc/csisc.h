/********************************************************************************************************
 * @file    csisc.h
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
// CSISC: Coordinated Set Identification Service Client

enum csisc_error_code
{
    BLE_CSISC_ERR_START = 0x80,
};

enum ble_csisc_event_id
{
    CSISC_EVT_ID_,
};

typedef void (*ble_csisc_event_callback)(uint16_t conn_handle, enum ble_csisc_event_id event_id, const void *event_msg);

struct ble_csisc_register_param
{
};

#define BLE_CSISC_INITIAL_PARAMS NULL

/**
 *   @brief Register the LE Audio Coordinated Set Identification Service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the CSIS module.
 *
 *   @return None.
 */
void ble_lea_register_CSIS_control_client(const struct ble_csisc_register_param *param);

/**
 *   @brief Register the LE Audio Coordinated Set Identification Service client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_CSIS_client_event_callback(ble_csisc_event_callback event_callback);

/**
 *   @brief Get the LE Audio CSIS client coordinated set size.
 *
 *   @param[in] conn_handle Connection handle to the LE Audio CSIS client.
 *   @param[out] p_coordinated_set_size Pointer to the variable to store the coordinated set size.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful.
 *          BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) - if the connection handle is invalid.
 */
int ble_csisc_get_coordinated_set_size(uint16_t conn_handle, uint8_t *p_coordinated_set_size);

/**
 *   @brief Get the LE Audio CSIS client set member rank.
 *
 *   @param[in] conn_handle Connection handle to the LE Audio CSIS client.
 *   @param[out] p_set_member_rank Pointer to the variable to store the set member rank.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful.
 *          BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) - if the connection handle is invalid.
 */
int ble_csisc_get_set_member_rank(uint16_t conn_handle, uint8_t *p_set_member_rank);

/**
 *   @brief Get the LE Audio CSIS client resolving key plain.
 *
 *   @param[in] conn_handle Connection handle to the LE Audio CSIS client.
 *   @param[out] p_resolving_key Pointer to the variable to store the resolving key plain.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful.
 *          BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) - if the connection handle is invalid.
 */
int ble_csisc_get_resolving_key_plain(uint16_t conn_handle, uint8_t p_resolving_key[16]);

/**
 *    @brief Write the LE Audio CSIS locked command to peer.
 *
 *    @param[in] conn_handle ACL Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the write operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 *            BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) - if the connection handle is invalid.
 */
int ble_csisc_write_locked(uint16_t conn_handle, prf_write_callback callback);

/**
 *    @brief Write the LE Audio CSIS unlocked command to peer.
 *
 *    @param[in] conn_handle ACL Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the write operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 *            BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) - if the connection handle is invalid.
 */
int ble_csisc_write_unlocked(uint16_t conn_handle, prf_write_callback callback);
