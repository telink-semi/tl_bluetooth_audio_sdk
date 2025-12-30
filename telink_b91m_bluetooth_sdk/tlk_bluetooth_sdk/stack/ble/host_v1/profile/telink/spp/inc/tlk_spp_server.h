/********************************************************************************************************
 * @file    tlk_spp_server.h
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
enum ble_spps_event_id
{
    SPPS_EVENT_ID_RECEIVE_CLIENT2SERVER_DATA,
};

struct ble_spps_register_param
{
};

struct tlk_spp_client2server_data_evt
{
    uint16_t       len;
    const uint8_t *value;
};

typedef void (*ble_spps_event_callback)(uint16_t conn_handle, enum ble_spps_event_id event_id, const void *event_msg);

/**
 *   @brief Register the SPP Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the SPPS module.
 *
 *   @return None.
 */
void ble_register_SPP_control_server(const struct ble_spps_register_param *param);

/**
 *   @brief Set the event callback for the SPP Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_register_SPP_event_callback(ble_spps_event_callback callback);
/**
 * @brief       Notifies the client of the server data.
 * @param[in]   conn_handle - the connection handle.
 * @param[in]   data - pointer to the data to be notified.
 * @param[in]   len - the length of the data.
 * @return      BLE_HOST_ERR_SUCC if the notification is successful, otherwise an error code.
 * @note        The data is notified to the client.
 */
int ble_spps_server_notify_client2server(uint16_t conn_handle, uint8_t *data, uint16_t len);
