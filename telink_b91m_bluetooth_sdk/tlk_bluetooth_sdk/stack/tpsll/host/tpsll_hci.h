/********************************************************************************************************
 * @file    tpsll_hci.h
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
#ifndef TPSLL_HCI_H_
#define TPSLL_HCI_H_
#endif

#if (TLK_CFG_TPSLL_HCI_ENABLE)
typedef enum
{
    TPSLL_HCI_TYPE_CMD      = 0x01,
    TPSLL_HCI_TYPE_SCO_DATA = 0x02,
} TLKTPSLL_HCI_TYPE_ENUM;

typedef void (*tlktpsll_hci_rxC2HSpeakerDataCB)(uint32_t tick, uint8_t rx_packet_id, uint8_t wptr, uint8_t *p_data, uint8_t len);
void tlktpsll_hci_registerC2HSpeakerData(tlktpsll_hci_rxC2HSpeakerDataCB cb);
void tlktpsll_hci_recvC2HData(uint8_t *pData, uint32_t dataLen);
void tlktpsll_hci_sco_recvC2HData(uint8_t *pData, uint32_t dataLen);
int  tlktpsll_hci_sendH2cCmd(uint16_t opcode, uint8_t *pData, uint16_t dataLen);
int  tlktpsll_hci_sendScoData(uint16_t opcode, uint8_t *pData, uint16_t dataLen);
#endif
