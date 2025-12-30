/********************************************************************************************************
 * @file    gmcss.h
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
// GMCSS: Generic Media Control Service Server.

enum ble_gmcss_event_id
{
    GMCSS_EVENT_ID_
};

struct ble_gmcss_register_param
{
};

typedef void (*ble_gmcss_event_callback)(uint16_t conn_handle, enum ble_gmcss_event_id event_id, const void *event_msg);

/**
 *   @brief Register the Generic Media Control Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the GMCSS module.
 *
 *   @return None.
 */
void ble_lea_register_GMCS_control_server(const struct ble_gmcss_register_param *param);

/**
 *   @brief Set the event callback for the Generic Media Control Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_lea_register_GMCS_event_callback(ble_gmcss_event_callback callback);
