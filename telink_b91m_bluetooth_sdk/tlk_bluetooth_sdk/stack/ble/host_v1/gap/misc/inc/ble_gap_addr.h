/********************************************************************************************************
 * @file    ble_gap_addr.h
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

/********************** BLE Device Address ****************/
/*
 * The random device address include static address, private address that include non-resolvable private address and resolvable private address.
 * - Address [47:46] = 0b00: Non-resolvable private address
 * - Address [47:46] = 0b01: Resolvable private address
 * - Address [47:46] = 0b10: Reserved for future use
 * - Address [47:46] = 0b11: Static device address
 *
 * ble_host_gap_set_resolvable_private_addr and ble_host_gap_set_random_addr can not change the smp BD address.
 * ble_host_gap_set_set_static_addr will change the smp BD address.
*/

/**
 *    @brief BLE host set random address.
 *
 *    @return BLE_HOST_ERR_SUCC if set random address is successful, otherwise error code.
 */
int ble_host_gap_set_random_addr(const uint8_t addr[6]);

/**
 *    @brief BLE host generate resolvable private address.
 *
 *    @param[out] addr generated resolvable private address.
 *
 *    @return none.
 *
 *    @note This function must be called after BLE SMP is initialized.
 */
void ble_host_gap_generate_resolvable_private_addr(uint8_t addr[6]);

/**
 *    @brief BLE host set resolvable private address.
 *
 *    @return BLE_HOST_ERR_SUCC if set resolvable private address is successful, otherwise error code.
 *
 *    @note This function must be called after BLE SMP is initialized.
 */
int ble_host_gap_set_resolvable_private_addr(void);

/**
 *    @brief BLE host generate non-resolvable private address.
 *
 *    @param[out] addr generated non-resolvable private address.
 *
 *    @return none.
 */
void ble_host_gap_generate_non_resolvable_private_addr(uint8_t addr[6]);

/**
 *    @brief BLE host generate static address.
 *
 *    @param[out] addr generated static address.
 *
 *    @return none.
 */
void ble_host_gap_generate_static_addr(uint8_t addr[6]);

/**
 *    @brief BLE host set static address.
 *
 *    @param[in] addr static address to set.
 *
 *    @return BLE_HOST_ERR_SUCC if set static address is successful, otherwise error code.
 */
int ble_host_gap_set_set_static_addr(const uint8_t addr[6]);
