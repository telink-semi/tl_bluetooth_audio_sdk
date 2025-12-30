/********************************************************************************************************
 * @file    hci_cmd_le_bis.h
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

struct ble_hci_le_big_create_sync_full_cp
{
    uint8_t  big_handle;
    uint16_t sync_handle;
    uint8_t  encryption;
    uint8_t  broadcast_code[16];
    uint8_t  mse;
    uint16_t sync_timeout;
    uint8_t  num_bis;
    uint8_t  bis[31];
} __attribute__((packed));

/**
 * @brief Creates a BIG (Broadcast Isochronous Group).
 *
 * @param[in] p_create_big Pointer to the structure containing the BIG parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_create_big(const struct ble_hci_le_create_big_cp *p_create_big);

/**
 * @brief Creates a BIG test.
 *
 * @param[in] p_create_big_test Pointer to the structure containing the BIG test parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_create_big_test(const struct ble_hci_le_create_big_test_cp *p_create_big_test);

/**
 * @brief Terminates a BIG.
 *
 * @param[in] p_terminate_big Pointer to the structure containing the BIG termination parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_terminate_big(const struct ble_hci_le_terminate_big_cp *p_terminate_big);

/**
 * @brief Creates a BIG sync.
 *
 * @param[in] p_create_sync Pointer to the structure containing the BIG sync parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_big_create_sync(const struct ble_hci_le_big_create_sync_full_cp *p_create_sync);

/**
 * @brief Terminates a BIG sync.
 *
 * @param[in] p_terminate_sync Pointer to the structure containing the BIG sync termination parameters.
 * @param[out] p_terminate_sync_rp Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_big_terminate_sync(const struct ble_hci_le_big_terminate_sync_cp *p_terminate_sync, struct ble_hci_le_big_terminate_sync_rp *p_terminate_sync_rp);