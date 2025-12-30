/********************************************************************************************************
 * @file    hci_tr_h5.h
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
#ifndef __HCI_TR_H5_H
#define __HCI_TR_H5_H

//#include "hci_tr_api.h"
#include "hci_tr.h"
#include "common/fifo.h"
#if (HCI_TR_MODE == HCI_TR_H5)

    /*! H5 transport buffer config macro. */ //4 dma length
    #define HCI_H5_TR_RX_BUF_SIZE MEM_SIZE_ALIGN16(4 + 2 + (HCI_H5_HEAD_LEN + HCI_TR_RX_BUF_SIZE + HCI_H5_CRC_LEN) * 2) * 2
    #define HCI_H5_TR_RX_BUF_NUM  4

    #define HCI_H5_FLUSH_TO       1000 /*! in ms*/

/**
 * @brief : H5 transport initialization.
 * @param : none.
 * @param : none.
 */
void HCI_Tr_H5Init(void);

/**
 * @brief : Register slip handler
 * @param : func    Pointer point to handler function.
 * @param : none.
 */
void HCI_Tr_H5RegisterSlipHandler(HciH5PacketHandler_t func);

/**
 * @brief : H5 transport rx handler.
 * @param : none.
 * @param : none.
 */
void HCI_Tr_H5RxHandler(void);

/**
 * @brief : H5 transport IRQ handler.
 * @param : none.
 * @param : none.
 */
void HCI_Tr_H5UartIRQHandler(void);

#endif


#endif
