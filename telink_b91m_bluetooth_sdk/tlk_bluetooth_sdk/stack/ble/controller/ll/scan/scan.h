/********************************************************************************************************
 * @file    scan.h
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
#ifndef SCAN_H_
#define SCAN_H_

typedef enum
{
    SCAN_STRATEGY_0 = 0, /*!< The number of scans corresponds to the number of connections,
	                              that is to say, after the number of connections reaches the
	                              maximum allowed number, scan cannot be started. >!*/
    SCAN_STRATEGY_1 = 1, /*!< When 1m is configured, if a master establishes a link,
	                              scan continues to work and scan can be turned on and off normally by the scan enable API---
	                              "blc_ll_setExtScanEnable" or "blc_ll_setScanEnable". >!*/
} scan_en_strtg_t;

void blc_ll_configScanEnableStrategy(scan_en_strtg_t scanStrategy);


#endif /* SCAN_H_ */
