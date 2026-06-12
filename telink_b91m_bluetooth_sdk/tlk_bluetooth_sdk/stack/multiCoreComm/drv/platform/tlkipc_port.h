/********************************************************************************************************
 * @file    tlkipc_port.h
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
uint32_t tlkipc_enterCritical(void);

uint32_t tlkipc_leaveCritical(uint32_t en);

void tlkipc_memcpy(void *dest, const void *src, unsigned int length);

void *tlkipc_memset(void *dest, int val, unsigned int len);

uint8_t tlkipc_mailbox_first_receive_check_hook(uint32_t data);

void tlkipc_mailbox_receive_hook(uint8_t *pData);

void tlkipc_hal_mailbox_send_by_ram(void *data, uint32_t dataLen);

void tlkipc_hal_mailbox_get_by_ram(void *buffer);