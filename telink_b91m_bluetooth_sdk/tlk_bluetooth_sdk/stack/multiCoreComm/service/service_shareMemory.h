/********************************************************************************************************
 * @file    service_shareMemory.h
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
#include "../drv/shareMemory.h"

#ifndef TLK_SM_LOG_ENABLE
#define TLK_SM_LOG_ENABLE (1)
#endif

#define TLK_SM_HCI_TX_BUFFER_SIZE (8 * 1024)

#define TLK_SM_HCI_RX_BUFFER_SIZE (8 * 1024)

#define TLK_SM_LOG_BUFFER_SIZE    (2 * 1024)

#define TLK_SM_SYNC_BUFFER_SIZE   (1 * 1024)

void tlk_share_memory_service_init(void);

void tlk_share_memory_service_loop(void);

void tlk_share_memory_service_hci_handler(void);

void tlk_share_memory_service_log_handler(void);

uint8_t tlk_share_memory_service_hci_isOverTargetPercent(uint32_t percent);


void tlk_d25f_register_hci_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb);

void tlk_d25f_register_sync_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb);

void tlk_d25f_register_log_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb);

tlk_sm_ret_e tlk_d25f_hci_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen);


void tlk_share_memory_n22_get_address_handler(uint32_t addr);

tlk_sm_ret_e tlk_n22_hci_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen);

tlk_sm_ret_e tlk_n22_sync_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen);

tlk_sm_ret_e tlk_n22_log_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen);

void tlk_n22_register_hci_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb);

void *tlkipc_share_memory_get_ctrlblock_addr(void);
