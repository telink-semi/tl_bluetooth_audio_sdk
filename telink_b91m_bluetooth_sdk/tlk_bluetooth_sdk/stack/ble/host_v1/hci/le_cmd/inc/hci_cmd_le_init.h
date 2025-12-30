/********************************************************************************************************
 * @file    hci_cmd_le_init.h
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
#define BLE_HCI_LE_MAX_CONN_PARAMS_NUM 3

struct ble_hci_le_ext_create_conn_full_cp
{
    uint8_t            filter_policy;
    uint8_t            own_addr_type;
    uint8_t            peer_addr_type;
    uint8_t            peer_addr[6];
    uint8_t            init_phy_mask;
    struct conn_params conn_params[BLE_HCI_LE_MAX_CONN_PARAMS_NUM];
} __attribute__((packed));

/**
 * @brief       Create a LE connection using legacy connection creation.
 * @param[in]   p_create_conn  - pointer to the connection creation parameters.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_create_connection(const struct ble_hci_le_create_conn_cp *p_create_conn);

/**
 * @brief       Cancel the LE connection creation process.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_create_connection_cancel(void);

/**
 * @brief       Create a LE connection using extended connection creation with multiple PHY support.
 * @param[in]   p_create_conn  - pointer to the extended connection creation parameters.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_ext_create_connection(const struct ble_hci_le_ext_create_conn_full_cp *p_create_conn);
