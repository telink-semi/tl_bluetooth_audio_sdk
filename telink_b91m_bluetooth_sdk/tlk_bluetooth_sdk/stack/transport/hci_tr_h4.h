/********************************************************************************************************
 * @file    hci_tr_h4.h
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
#ifndef HCI_TR_H4_H_
#define HCI_TR_H4_H_

#include "hci_tr.h"
#include "common/fifo.h"

/*!  H4 transport buffer config macro. */ //4 dma length
#if (!HCI_UART_SoftwareRxDone_EN)
#define HCI_H4_TR_RX_BUF_SIZE                   \
    (MEM_SIZE_ALIGN16(4 + HCI_TR_RX_BUF_SIZE) * \
     4) //80 /*! The value of this size must be set according to the size of the HCI RX buffer, and at least twice the size of the HCI RX buffer.*/
#define HCI_H4_TR_RX_BUF_NUM 4

#define HCI_H4_FLUSH_TO      10 * 1000 /*! in ms*/
#else
#define HCI_H4_TR_RX_BUF_SIZE   2048
#define HCI_H4_TR_RING_BUF_SIZE 4096
#define HCI_H4_TR_FLUSH_TO      500 //unit:ms
#endif
/**
 * @brief : H4 transport initialization.
 * @param : pFifo       Pointer point to HCI rx fifo.
 * @return: none
 */
void HCI_Tr_H4Init(tlk_fifo_t *pFifo, HCI_TR_UART *hci_uart);

/**
 * @brief : H4 transport rx handler.
 * @param : none.
 * @return: none
 */
void HCI_Tr_H4RxHandler(void);

/**
 * @brief : H4 UART IRQ handler.
 * @param : none.
 * @return: none
 */
void HCI_Tr_H4UartIRQHandler(void);


#endif
