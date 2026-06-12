/********************************************************************************************************
 * @file    ble_coc.h
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
struct ble_host_coc_connected_params
{
    uint16_t spsm;
    uint16_t src_cid;
    uint16_t mtu;
    uint16_t initial_credit;
};

typedef void (*ble_host_coc_connected_callback)(uint16_t conn_handle, const struct ble_host_coc_connected_params *params);

typedef void (*ble_host_coc_mtu_update_callback)(uint16_t conn_handle, uint16_t src_cid, uint16_t mtu);

typedef void (*ble_host_coc_disconnected_callback)(uint16_t conn_handle, uint16_t src_cid);

typedef void (*ble_host_coc_credit_update_callback)(uint16_t conn_handle, uint16_t src_cid);

typedef void (*ble_host_coc_send_data_callback)(uint16_t conn_handle, uint16_t src_cid, bool success, void *arg);

typedef void (*ble_host_coc_receive_data_callback)(uint16_t conn_handle, uint16_t src_cid, const uint8_t *data, uint16_t len);

struct ble_host_coc_set_parameters
{
    uint16_t                           initial_credit;
    uint16_t                           mtu;
    uint16_t                           mps;
    ble_host_coc_connected_callback    connected_callback;
    ble_host_coc_disconnected_callback disconnected_callback;
    ble_host_coc_receive_data_callback receive_data_callback;
    ble_host_coc_mtu_update_callback   mtu_update_callback;
};

void ble_host_coc_initialize(void);

void ble_host_coc_set_all_parameters(const struct ble_host_coc_set_parameters *params);

void ble_host_coc_set_credit_update_callback(ble_host_coc_credit_update_callback callback);

void ble_host_coc_set_mtu(uint16_t mtu);

void ble_host_coc_set_mps(uint16_t mps);

void ble_host_coc_set_initial_credit(uint16_t initial_credit);

int ble_host_coc_send_data(uint16_t conn_handle, uint16_t cid, const void *data, uint16_t len, ble_host_coc_send_data_callback callback, void *arg);

int ble_host_coc_disconnect(uint16_t conn_handle, uint16_t cid);

int ble_host_coc_create_connection_by_le_credit(uint16_t conn_handle, uint16_t spsm, void (*callback)(uint16_t conn_handle, bool success));

int ble_host_coc_create_connection_by_credit(uint16_t conn_handle, uint16_t spsm, uint8_t cid_num, void (*callback)(uint16_t conn_handle, bool success));

int ble_host_coc_reconfigure_mtu_mps(uint16_t conn_handle, uint16_t mtu, uint16_t mps, uint8_t cid_num, uint16_t *p_src_cid, void (*callback)(uint16_t conn_handle, bool success));

void ble_host_coc_register_eatt_data_callback(ble_host_coc_receive_data_callback receive_data_callback);
