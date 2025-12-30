/********************************************************************************************************
 * @file    ble_sdp.h
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
 *   @brief  Initialize the service discovery produce(SDP) module.
 *
 *   This function initializes the SDP module. It should be called once at the
 *   beginning of the application.
 *
 *   @return None.
 */
void ble_host_gatt_sdp_init(void);

/**
 *   @brief  Start the SDP service on a connection.
 *
 *   @param[in] conn_handle  Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *
 *   @note   If SDP starts successfully, it can not stop by user.
 */
int ble_host_gatt_sdp_start(uint16_t conn_handle);
