/********************************************************************************************************
 * @file    ble_l2cap_log.h
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


extern uint8_t g_ble_host_l2cap_log_enable;
extern uint8_t g_ble_host_l2cap_att_log_enable;
extern uint8_t g_ble_host_l2cap_coc_log_enable;
extern uint8_t g_ble_host_l2cap_eatt_log_enable;
extern uint8_t g_ble_host_l2cap_signaling_log_enable;
extern uint8_t g_ble_host_l2cap_smp_log_enable;

#define BLE_L2CAP_LOG_OUTPUT(log, en, str, ...)  \
    do {                                         \
        if (en && g_ble_host_l2cap_log_enable) { \
            log("[L2CAP]" str, ##__VA_ARGS__);   \
        }                                        \
    } while (0)

#define BLE_HOST_L2CAP_COMMON_ERROR(str, ...)    BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COMMON_WARN(str, ...)     BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COMMON_INFO(str, ...)     BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_log_enable, str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COMMON_DEBUG(str, ...)    BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_log_enable, str, ##__VA_ARGS__)

#define BLE_HOST_L2CAP_ATT_ERROR(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_att_log_enable, "[ATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_ATT_WARN(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_att_log_enable, "[ATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_ATT_INFO(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_att_log_enable, "[ATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_ATT_DEBUG(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_att_log_enable, "[ATT]" str, ##__VA_ARGS__)

#define BLE_HOST_L2CAP_COC_ERROR(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_coc_log_enable, "[CoC]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COC_WARN(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_coc_log_enable, "[CoC]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COC_INFO(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_coc_log_enable, "[CoC]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_COC_DEBUG(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_coc_log_enable, "[CoC]" str, ##__VA_ARGS__)

#define BLE_HOST_L2CAP_EATT_ERROR(str, ...)      BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_eatt_log_enable, "[EATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_EATT_WARN(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_eatt_log_enable, "[EATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_EATT_INFO(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_eatt_log_enable, "[EATT]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_EATT_DEBUG(str, ...)      BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_eatt_log_enable, "[EATT]" str, ##__VA_ARGS__)

#define BLE_HOST_L2CAP_SIGNALING_ERROR(str, ...) BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_signaling_log_enable, "[SIGN]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SIGNALING_WARN(str, ...)  BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_signaling_log_enable, "[SIGN]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SIGNALING_INFO(str, ...)  BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_signaling_log_enable, "[SIGN]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SIGNALING_DEBUG(str, ...) BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_signaling_log_enable, "[SIGN]" str, ##__VA_ARGS__)

#define BLE_HOST_L2CAP_SMP_ERROR(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_ERROR, g_ble_host_l2cap_smp_log_enable, "[SMP]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SMP_WARN(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_WARN, g_ble_host_l2cap_smp_log_enable, "[SMP]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SMP_INFO(str, ...)        BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_INFO, g_ble_host_l2cap_smp_log_enable, "[SMP]" str, ##__VA_ARGS__)
#define BLE_HOST_L2CAP_SMP_DEBUG(str, ...)       BLE_L2CAP_LOG_OUTPUT(BLE_HOST_SAL_LOG_DEBUG, g_ble_host_l2cap_smp_log_enable, "[SMP]" str, ##__VA_ARGS__)
