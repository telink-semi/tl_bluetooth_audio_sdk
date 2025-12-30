/********************************************************************************************************
 * @file    hci_slip.h
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
#ifndef HCI_SLIP_H_
#define HCI_SLIP_H_

#include "hci_tr.h"
#include "common/types.h"
//#include "hci_tr_api.h"
#if (HCI_TR_MODE == HCI_TR_H5)

    /*!  Slip decoder/encoder buffer config macro. */
    #define HCI_SLIP_DECODE_BUF_SIZE MEM_SIZE_ALIGN4(HCI_H5_HEAD_LEN + HCI_TR_RX_BUF_SIZE + HCI_H5_CRC_LEN)
    #define HCI_SLIP_ENCODE_BUF_SIZE MEM_SIZE_ALIGN4(2 + (HCI_H5_HEAD_LEN + HCI_TR_TX_BUF_SIZE + HCI_H5_CRC_LEN) * 2)

    /*! Slip invalid unencoded. */
    #define SLIP_INVALID_UNENCODED 0x00

    /*! Slip invalid escape. */
    #define SLIP_INVALID_ESCAPE 0x0000

    #define SLIP_DELIMITER      0xC0
    #define SLIP_ESCAPE         0xDB

/**
 * @brief : Slip initialization.
 * @param : none.
 * @param : none.
 */
void HCI_Slip_Init(void);

/**
 * @brief : Register H5 protocol handler.
 * @param : func    Pointer point to handler function.
 * @param : none.
 */
void HCI_Slip_RegisterPktHandler(HciH5PacketHandler_t func);

/**
 * @brief : Slip send.
 * @param : pPacket   Pointer point to buffer.
 * @param : len       The length of data.
 * @param : none.
 */
bool HCI_Slip_Send(u8 *pPacket, u32 len);

/**
 * @brief : Slip decoder.
 * @param : pPacket   Pointer point to buffer.
 * @param : len       The length of data.
 * @param : none.
 */
void HCI_Slip_DecodePacket(u8 *pPacket, u32 len);

/**
 * @brief : Slip encoder.
 * @param : pPacket   Pointer point to buffer.
 * @param : len       The length of data.
 * @param : none.
 */
void HCI_Slip_EncodePacket(u8 *pPacket, u32 len);

/**
 * @brief : Set OOF software flow control enable/disable.
 * @param : enable   enable/disable OOF.
 * @param : none.
 */
void HCI_SLip_SetFlowCtrlEnable(bool enable);

#endif
#endif /* HCI_SLIP_H_ */
