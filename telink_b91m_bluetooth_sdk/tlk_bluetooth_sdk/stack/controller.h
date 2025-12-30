/********************************************************************************************************
 * @file    controller.h
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
#ifndef TLKSTK_CONTROLLER_H_
#define TLKSTK_CONTROLLER_H_

#include "stack/stack.h"

#include "transport/hci_tr_def.h"
#include "stack/hci/hci_const.h"
#include "stack/hci/hci_cmd.h"

typedef enum
{
    BT_only    = BIT(0),
    BLE_only   = BIT(1),
    TPH_only   = BIT(2),
	TPT_only   = BIT(3),
	TPD_only   = BIT(4),
	TPDT_only  = BIT(5),
    TPMD_only  = BIT(6),

    BT_BLE     = BT_only | BLE_only,
    BT_TPH     = BT_only | TPH_only,
    BLE_TPH    = BLE_only | TPH_only,
    BT_TPT     = BT_only | TPT_only,
    BLE_TPT    = BLE_only | TPT_only,
    BT_BLE_TPH = BT_BLE | TPH_only,
	BT_BLE_TPDT= BT_only | BLE_only | TPDT_only,

} controller_mode;

/**
 ****************************************************************************************
 * @brief This function initialize controller.
 *
 * @param[in] none
 *
 * @return Status
 *
 ****************************************************************************************
 */
void controller_init(controller_mode mode, HCI_TR_MODE tr_mode, HCI_TR_UART *hci_tr_uart,  void *parameter);

#endif /* TLKSTK_CONTROLLER_H_ */
