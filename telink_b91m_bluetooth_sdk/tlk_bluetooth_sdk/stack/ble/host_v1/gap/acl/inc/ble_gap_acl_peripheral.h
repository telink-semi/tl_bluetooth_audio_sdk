/********************************************************************************************************
 * @file    ble_gap_acl_peripheral.h
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

/**
 *   @brief Enable the latency for the all acl connection peripherals.
 *
 *   @return BLE_HOST_ERR_SUCC if enable latency is sent successfully, otherwise error code.
 */
int ble_host_gap_acl_peripheral_enable_latency(void);

/**
 *   @brief Disable the latency for the all acl connection peripherals.
 *
 *   @return BLE_HOST_ERR_SUCC if disable latency is sent successfully, otherwise error code.
 */
int ble_host_gap_acl_peripheral_disable_latency(void);

/**
 *   @brief ACL peripheral reconnect timeout hook.
 *
 *   @note This function is called by BLE host stack, when user call ble_host_gap_acl_peripheral_reconnect_legacy_adv() API successfully.
 *          Or user using direct ADV package to reconnect.
 */
// void ble_host_gap_acl_peripheral_reconnect_timeout_hook(void);

/**
 *    @brief Reconnect BLE device using legacy ADV package.
 *
 *    @param[in] high_duty: High duty or low duty.
 *    @param[in] index: Index of the device to reconnect, reconnect device index.
 *
 *    @return BLE_HOST_ERR_SUCC if reconnect is sent successfully, otherwise error code.
 *              - BLE_GAP_ERR(BLE_HOST_GAP_ERR_NOT_PAIRING_INFO) if device not paired device.
 *              - BLE_GAP_ERR(BLE_HOST_GAP_ERR_CONTROLLER_NOT_SUPPORT_RESOLVING_LIST) if controller not support resolving list.
 *              - BLE_GAP_ERR(BLE_HOST_GAP_ERR_CONTROLLER_NOT_DIRECTED_ADV) if controller not directed adv.
 *
 *    @note This API is used to reconnect BLE device using legacy ADV package, iOS not support this feature.
 */
int ble_host_gap_acl_peripheral_reconnect_legacy_adv(bool high_duty, uint32_t index);
