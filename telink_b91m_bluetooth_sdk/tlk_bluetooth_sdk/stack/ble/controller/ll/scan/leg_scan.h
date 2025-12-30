/********************************************************************************************************
 * @file    leg_scan.h
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
#ifndef LLMS_SCAN_H_
#define LLMS_SCAN_H_


#include "stack/ble/hci/hci_cmd.h"


/**
 * @brief      for user to initialize legacy scanning module
 * 			   notice that only one module can be selected between legacy scanning module and extended scanning module
 * @param	   none
 * @return     none
 */
void blc_ll_initLegacyScanning_module(void);


/**
 * @brief      This function is used to set the scan parameters
 * @param[in]  scan_type - passive Scanning or active scanning.
 * @param[in]  scan_interval - time interval from when the Controller started its last LE scan until it begins the subsequent LE scan
 * @param[in]  scan_window - The duration of the LE scan.
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanParameter(scan_type_t scan_type, scan_inter_t scan_interval, scan_wind_t scan_window, own_addr_type_t ownAddrType, scan_fp_type_t scan_fp);


/**
 * @brief	   enable or disable legacy scanning.
 * @param[in]  scan_enable
 * @param[in]  filter_duplicate - controls whether the Link Layer should filter out
 * 								  duplicate advertising reports (Filtering_Enabled) to the Host,
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanEnable(scan_en_t scan_enable, dupFilter_en_t filter_duplicate);


#endif /* LLMS_SCAN_H_ */
