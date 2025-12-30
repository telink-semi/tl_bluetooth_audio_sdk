/********************************************************************************************************
 * @file    leg_init.h
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
#ifndef LLMS_INIT_H_
#define LLMS_INIT_H_

#include "stack/ble/hci/hci_cmd.h"


/**
 * @brief      for user to initialize legacy initiating module
 * 			   notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param	   none
 * @return     none
 */
void blc_ll_initLegacyInitiating_module(void);


/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 * @param[in]  scan_interval - recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scan_window - recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  filter_policy - used to determine whether the White List is used.
 * @param[in]  adr_type - indicates the type of address used in the connectable advertisement sent by the peer.
 * @param[in]  *mac - indicates the Peer's Public Device Address.
 * @param[in]  own_adr_type - indicates the type of address being used in the connection request packets.
 * @param[in]  conn_min - the minimum allowed connection interval.
 * @param[in]  conn_max - the maximum allowed connection interval.
 * @param[in]  conn_latency - he maximum allowed connection latency
 * @param[in]  timeout - defines the link supervision timeout for the connection.
 * @param[in]  ce_min - informative parameters providing the Controller with the expected minimum length of the connection events.
 * @param[in]  ce_max - informative parameters providing the Controller with the expected maximum length of the connection events.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_createConnection(scan_inter_t scanInter, scan_wind_t scanWindow, init_fp_t fp, u8 peerAdrType, u8 *peerAddr, own_addr_type_t ownAdrType, conn_inter_t conn_min,
                                  conn_inter_t conn_max, u16 conn_latency, conn_tm_t timeout, u16 ce_min, u16 ce_max);


#endif /* LLMS_INIT_H_ */
