/********************************************************************************************************
 * @file    whitelist.h
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
#ifndef LL_WHITELIST_H_
#define LL_WHITELIST_H_


/**
 * @brief      This function is used to clear WhiteList
 * @param[in]  none
 * @return     Status - 0x00: command succeeded;
 *  * 			        Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_clearWhiteList(void);


/**
 * @brief      This function is used to add a device to WhiteList
 *             attention: maximum device number in Whitelist list is 4 by default. Please contact Telink if this number can not meet your requirement.
 * @param[in]  adr_type - device address type
 * @param[in]  addr - device address
 * @return     Status - 0x00: command succeeded;
 * 			            Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_addDeviceToWhiteList(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to delete a device from WhiteList
 * @param[in]  adr_type - device address type
 * @param[in]  addr - device address
 * @return     Status - 0x00: command succeeded;
 * 			            Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_removeDeviceFromWhiteList(u8 adr_type, u8 *addr);


#endif /* LL_WHITELIST_H_ */
