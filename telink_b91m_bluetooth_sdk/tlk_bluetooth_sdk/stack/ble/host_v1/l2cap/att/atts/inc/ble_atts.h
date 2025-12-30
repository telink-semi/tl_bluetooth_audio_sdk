/********************************************************************************************************
 * @file    ble_atts.h
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
typedef void (*ble_host_atts_recv_pdu_callback)(uint16_t conn_handle, uint16_t cid, uint8_t opcode, const uint8_t *pdu, uint16_t pdu_len);

/**
 * @brief       Register a callback function for client initiated ATT PDU reception.
 * @param[in]   callback    - the callback function to register.
 * @return      none.
 */
void ble_host_atts_register_client_initiated_recv_pdu_callback(ble_host_atts_recv_pdu_callback callback);