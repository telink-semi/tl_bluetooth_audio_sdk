/********************************************************************************************************
 * @file    acl_central.h
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
#ifndef ACL_CENTRAL_H_
#define ACL_CENTRAL_H_


/**
 * @brief      for user to initialize ACL connection central role.
 * @param	   none
 * @return     none
 */
void blc_ll_initAclCentralRole_module(void);


/**
 * @brief      for user to initialize LinkLayer ACL connection central role TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - TX FIFO size
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @param[in]  conn_number - Number of supported ACL central connections
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initAclCentralTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number, int conn_number);


/**
 * @brief      for user to initialize LinkLayer ACL connection RX FIFO.
 * 			   all connection will share the FIFO.
 * @param[in]  conn_interval - Set connection interval, unit 1.25ms.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_setAclCentralBaseConnectionInterval(conn_inter_t conn_interval);


/**
 * @brief      this function is used to change the ACL connection parameters.
 * @param[in]  connHandle - Connection_Handle
 * @param[in]  conn_min - the minimum allowed connection interval.
 * @param[in]  conn_max - the maximum allowed connection interval.
 * @param[in]  conn_latency - the maximum allowed connection latency.
 * @param[in]  timeout - the link supervision timeout for the LE link.
 * @param[in]  ce_min - information parameters providing the Controller with a hint about the expected minimum length of the connection events.
 * @param[in]  ce_max - information parameters providing the Controller with a hint about the expected maximum length of the connection events.
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_updateConnection(u16 connHandle, conn_inter_t conn_min, conn_inter_t conn_max, u16 conn_latency, conn_tm_t timeout, u16 ce_min, u16 ce_max);


/**
 * @brief      This function is used to customize Connection Establish Event
 * @param[in]  enable - 1: enable;  0: disable
 * @return     none
 */
void blc_ll_customizeConnectionEstablishEvent(int enable);


u8 blc_ll_get_acl_central_device_type(u16 acl_handle);


#endif /* ACL_CENTRAL_H_ */
