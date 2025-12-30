/********************************************************************************************************
 * @file    hci_tr.h
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
#include "common/types.h"
#include "common/compiler.h"
#include "hci_tr_def.h"

#define TR_LOG_EN 1
///*! HCI transport layer protocol selection. */
//#define HCI_TR_H4            0
//#define HCI_TR_H5            1
//#define HCI_TR_USB           2
//#define HCI_TR_MODE          HCI_TR_H4


typedef void (*HciH5PacketHandler_t)(u8 *pPacket, u32 len);


typedef void (*TRH2CCallback)(uint8_t *pData, uint32_t len);
/*! HCI transport layer API. */

/**
 * @brief : HCI transport initialization.
 * @param : none.
 * @param : none.
 */
void HCI_TransportInit(HCI_TR_MODE tr_mode, HCI_TR_UART *hci_uart);

/**
 * @brief : HCI transport main loop.
 * @param : none.
 * @param : none.
 */
void HCI_TransportPoll(void);

/**
 * @brief : HCI transport bt and le distribute.
 * @param : none.
 * @param : none.
 */
void tlk_btble_hci_distribute(uint8_t *pData, uint32_t len);
/**
 * @brief : HCI transport bt distribute.
 * @param : none.
 * @param : none.
 */
void tlk_bt_hci_distribute(uint8_t *pData, uint32_t len);
/**
 * @brief : HCI transport ble distribute.
 * @param : none.
 * @param : none.
 */
void tlk_ble_hci_distribute(uint8_t *pData, uint32_t len);
/**
 * @brief : HCI transport BT C2H.
 * @param : none.
 * @param : none.
 */
int tr_bt_hci_c2h_callback(uint8_t type, uint8_t *pData, uint16_t dataLen);
/**
 * @brief : HCI transport dual BLE C2H.
 * @param : none.
 * @param : none.
 */
int tr_blc_hci_send_data(u32 h, u8 *para, int n);
/**
 * @brief : HCI transport single BLE C2H.
 * @param : none.
 * @param : none.
 */
int tr_single_blc_hci_send_data(u32 h, u8 *para, int n);
/**
 * @brief : HCI transport h2c reg.
 * @param : none.
 * @param : none.
 */
void tr_hci_h2cCB_reg(TRH2CCallback cb);
