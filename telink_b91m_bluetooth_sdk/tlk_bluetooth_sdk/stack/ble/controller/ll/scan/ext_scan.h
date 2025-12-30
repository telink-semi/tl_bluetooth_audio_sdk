/********************************************************************************************************
 * @file    ext_scan.h
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
#ifndef LL_SCAN_EXT_H_
#define LL_SCAN_EXT_H_

#include "stack/ble/hci/hci_cmd.h"


/**
 * @brief      for user to initialize extended scanning module
 * 			   notice that only one module can be selected between legacy scanning module and extended scanning module
 * @param	   none
 * @return     none
 */
void blc_ll_initExtendedScanning_module(void);


/**
 * @brief      This function is used to set the extended scan parameters to be used on the advertising physical channels
 *			   attention: when scan_phys is SCAN_PHY_1M, 	scanType_1 & scanInter_1 & scanWindow_1 are invalid parameters,
 *			   			  when scan_phys is SCAN_PHY_CODED, scanType_0 & scanInter_0 & scanWindow_0 are invalid parameters,
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @param[in]  scan_phys - Scanning_PHYs, "SCAN_PHY_1M" or "SCAN_PHY_CODED"
 *
 * 			   Attention:
 * 			   scanType_0/scanInter_0/scanWindow_0 are only for 1M    PHY.  If    1M PHY is not supported, these parameters are ignored.
 * 			   scanType_1/scanInter_1/scanWindow_1 are only for Coded PHY.  If Coded PHY is not supported, these parameters are ignored.
 *
 * @param[in]  scanType_0 - Scan_Type for 1M PHY, Passive Scanning or Active Scanning.
 * @param[in]  scanInter_0 - Scan_Interval for 1M PHY, Time interval from when the Controller started its last scan until it
							   begins the subsequent scan on the primary advertising physical channel.
 * @param[in]  scanWindow_0 - Duration of the scan on the primary advertising physical channel for 1M PHY
 *
 * @param[in]  scanType_1 - Scan_Type for Coded PHY, Passive Scanning or Active Scanning.
 * @param[in]  scanInter_1 - Scan_Interval for Coded PHY, Time interval from when the Controller started its last scan until it
							   begins the subsequent scan on the primary advertising physical channel.
 * @param[in]  scanWindow_1 - Duration of the scan on the primary advertising physical channel  for Coded PHY
 *
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtScanParam(own_addr_type_t ownAddrType, scan_fp_type_t scan_fp, scan_phy_t scan_phys, scan_type_t scanType_0, scan_inter_t scanInter_0,
                                 scan_wind_t scanWindow_0, scan_type_t scanType_1, scan_inter_t scanInter_1, scan_wind_t scanWindow_1);


/**
 * @brief	   This function is used to enable or disable scanning.
 * @param[in]  extScan_en - 0x00: Scanning disabled; 0x01: Scanning enabled
 * @param[in]  filter_duplicate - Filter_Duplicates
 * @param[in]  duration - Scan duration
 * @param[in]  period - Time interval from when the Controller started its last Scan_Duration until it begins the
 * 			   			subsequent Scan_Duration.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtScanEnable(scan_en_t extScan_en, dupe_fltr_en_t filter_duplicate, scan_durn_t duration, scan_period_t period);


/* special use API */
void blc_ll_setExtendedScanSecondaryChannelRxDataProcessEnable(u8 enable);

int blt_ext_scan_disable_process(void);
#endif /* LL_SCAN_EXT_H_ */
