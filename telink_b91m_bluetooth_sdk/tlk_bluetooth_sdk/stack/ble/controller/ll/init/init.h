/********************************************************************************************************
 * @file    init.h
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
#ifndef INIT_H_
#define INIT_H_


/**
 * @brief      This function is used to cancel the HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection commands.
 * 			   This command shall only be issued after the HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection commands have been issued.
 * @param	   none
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_createConnectionCancel(void);


/**
 * @brief      This function is used to set the timeout of ACL connection establishment
 * @param[in]  timeout_ms - timeout of ACL connection establishment, unit: mS. If not set, default value is 4000 mS
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setCreateConnectionTimeout(u32 timeout_ms);


/**
 * @brief      This function is used to check if initiation is busy
 * @param[in]  none
 * @return     1: initiation is ongoing;  0: no initiation or initiation finished
 */
bool blc_ll_isInitiationBusy(void);


/**
 * @brief      This function is used to set create connection retry number
 * @param[in]  number - retry number, must be in range of 0 ~ 5
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setCreateConnectionRetryNumber(u8 number);


/**
 * @brief      disable connection update high authority for master.
 * @param[in]  none
 * @return     none
 */
void blc_ll_disableConnUpdHighAuthority(void);


void blc_ll_set_init_connection_device_type(dev_type_t device_type);

#endif /* INIT_H_ */
