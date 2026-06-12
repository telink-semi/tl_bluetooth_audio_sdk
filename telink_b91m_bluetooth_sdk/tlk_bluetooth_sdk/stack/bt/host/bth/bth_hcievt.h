/********************************************************************************************************
 * @file    bth_hcievt.h
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
#ifndef BTH_HCI_EVT_H
#define BTH_HCI_EVT_H

#define HCI_OPCODE_GET_OGF(opcode) ((opcode >> 10) & 0x3F)
#define HCI_OPCODE_GET_OCF(opcode) (opcode & 0x3FF)

#define BTH_HCI_EIR_DT_NAME_PART   0x08 /* Shortened local name */
#define BTH_HCI_EIR_DT_NAME        0x09 /* Complete local name */


/******************************************************************************
 * Function: bth_hcievt_init
 * Descript: Initial the hci event callback.
 * Params: None.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_hcievt_init(void);

/******************************************************************************
 * Function: bth_send_commandCompleteEvt
 * Descript:
 * Params:
 *     @handle[IN]--the resut of set bt scan.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_commandCompleteEvt(uint8_t opcode, uint8_t status);
#endif // BTH_HCI_EVT_H
