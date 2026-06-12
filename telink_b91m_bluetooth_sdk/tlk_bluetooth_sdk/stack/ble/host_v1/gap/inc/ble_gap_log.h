/********************************************************************************************************
 * @file    ble_gap_log.h
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

extern const uint8_t g_ble_host_gap_log_enable;
extern const uint8_t g_ble_host_gap_acl_log_enable;
extern const uint8_t g_ble_host_gap_cis_log_enable;
#define BLE_GAP_LOG_OUTPUT(log, en, str, ...)  \
    do {                                       \
        if (en && g_ble_host_gap_log_enable) { \
            log("[GAP]" str, ##__VA_ARGS__);   \
        }                                      \
    } while (0)

#define BLE_HOST_GAP_COMMON_ERROR(str, ...) BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_gap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_GAP_COMMON_WARN(str, ...)  BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_gap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_GAP_COMMON_INFO(str, ...)  BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_gap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_GAP_COMMON_DEBUG(str, ...) BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_gap_log_enable, str, ##__VA_ARGS__)

#define BLE_HOST_GAP_ACL_ERROR(str, ...)    BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_gap_acl_log_enable, "[ACL]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_ACL_WARN(str, ...)     BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_gap_acl_log_enable, "[ACL]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_ACL_INFO(str, ...)     BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_gap_acl_log_enable, "[ACL]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_ACL_DEBUG(str, ...)    BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_gap_acl_log_enable, "[ACL]" str, ##__VA_ARGS__)

#define BLE_HOST_GAP_CIS_ERROR(str, ...)    BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_gap_cis_log_enable, "[CIS]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_CIS_WARN(str, ...)     BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_gap_cis_log_enable, "[CIS]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_CIS_INFO(str, ...)     BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_gap_cis_log_enable, "[CIS]" str, ##__VA_ARGS__)
#define BLE_HOST_GAP_CIS_DEBUG(str, ...)    BLE_GAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_gap_cis_log_enable, "[CIS]" str, ##__VA_ARGS__)
