/********************************************************************************************************
 * @file    ble_signaling.h
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


struct signaling_initial_param
{
};

#define BLE_SIGNALING_INITIAL_PARAM_DEFAULT NULL

struct ble_host_signaling_conn_param_update_param
{
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t latency;
    uint16_t timeout;
};

typedef bool (*ble_host_signaling_receive_conn_param_update_callback)(uint16_t conn_handle, const struct ble_host_signaling_conn_param_update_param *param);

typedef void (*ble_host_signaling_send_conn_param_update_callback)(uint16_t conn_handle, bool result);

/**
 * @brief       Initialize the signaling module.
 * @param[in]   param - pointer to the initialization parameters.
 * @return      the result of the initialization.
 */
int ble_host_signaling_init(struct signaling_initial_param *param);

/**
 * @brief       Register a callback function to receive connection parameter update requests.
 * @param[in]   callback - pointer to the callback function.
 * @return      none.
 */
void ble_host_signaling_register_receive_conn_param_update_callback(ble_host_signaling_receive_conn_param_update_callback callback);

/**
 * @brief       Send a connection parameter update request.
 * @param[in]   conn_handle - the connection handle.
 * @param[in]   param - pointer to the connection parameter update parameters.
 * @param[in]   callback - pointer to the callback function.
 * @return      the result of the request.
 */
int ble_host_signaling_send_conn_param_update(uint16_t conn_handle, const struct ble_host_signaling_conn_param_update_param *param,
                                              ble_host_signaling_send_conn_param_update_callback callback);
