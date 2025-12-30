/********************************************************************************************************
 * @file    acl_peripheral.h
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
#ifndef ACL_PERIPHERAL_H_
#define ACL_PERIPHERAL_H_


/**
 * @brief      for user to initialize ACL connection peripheral role.
 * @param	   none
 * @return     none
 */
void blc_ll_initAclPeriphrRole_module(void);


/**
 * @brief      for user to initialize LinkLayer ACL connection peripheral role TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - TX FIFO size
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @param[in]  conn_number - Number of supported ACL peripheral connections
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initAclPeriphrTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number, int conn_number);


#endif /* ACL_PERIPHERAL_H_ */
