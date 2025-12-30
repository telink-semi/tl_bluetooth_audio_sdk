/********************************************************************************************************
 * @file    shareMemory.h
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
#pragma once

#define TLK_SHARE_MEMORY_MAX_PAYLOAD_LEN 1024

typedef enum
{
    TLK_SHARE_MEMORY_STATUS_NOT_READY,
	TLK_SHARE_MEMORY_STATUS_HAND_SHAKING,
	TLK_SHARE_MEMORY_STATUS_READY,
} tlk_sm_status_e;

typedef enum
{
	TLK_SHARE_MEMORY_SUCCESS = 0,
	TLK_SHARE_MEMORY_SUCCESS_NONEED_NOTIFY = TLK_SHARE_MEMORY_SUCCESS,
	TLK_SHARE_MEMORY_SUCCESS_NEED_NOTIFY,
	TLK_SHARE_MEMORY_NOT_READY,
	TLK_SHARE_MEMORY_FULL,
	TLK_SHARE_MEMORY_ERROR_PARAM,
}tlk_sm_ret_e;

typedef enum
{
	TLK_SHARE_MEMORY_MESSAGE_TYPE_BLE = 0,
	TLK_SHARE_MEMORY_MESSAGE_TYPE_BT,
	TLK_SHARE_MEMORY_MESSAGE_TYPE_TPSLL,
	TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM,

	TLK_SHARE_MEMORY_MESSAGE_TYPE_LOG = 0,
	
}tlk_sm_message_type_e;

typedef void (*tlk_sm_rx_cb_f)(uint8_t *, uint32_t);
typedef tlk_sm_rx_cb_f* tlk_sm_rx_cb_array_t;
typedef struct
{
    uint8_t *pBuffer;
    uint32_t size;
    uint32_t wptr;
    uint32_t rptr;
	uint32_t mallocPtr;
	uint32_t enterCnt;
} tlk_safeFifo_t;

typedef struct 
{
    tlk_safeFifo_t fifo;
    volatile uint32_t status;
    tlk_sm_rx_cb_array_t cbArray;
} tlk_sm_fifo_t;

void share_memory_fifo_init(tlk_sm_fifo_t* smFifo, uint8_t *pBuffer, uint32_t size);

void share_memory_register_fifo_receive_cb(tlk_sm_fifo_t* smFifo,tlk_sm_rx_cb_array_t cbArray);

void share_memory_data_popAll(tlk_sm_fifo_t* smFifo);

tlk_sm_ret_e share_memory_data_push(tlk_sm_fifo_t* smFifo,uint32_t type, uint8_t *data, uint32_t dataLen);

uint8_t share_memory_fifo_isOverTargetPercent(tlk_sm_fifo_t* smFifo, uint32_t percent);

