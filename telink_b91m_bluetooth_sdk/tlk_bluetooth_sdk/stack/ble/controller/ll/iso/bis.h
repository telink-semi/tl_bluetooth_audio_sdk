/********************************************************************************************************
 * @file    bis.h
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
#ifndef BIS_H_
#define BIS_H_


#define BIS_PARAM_LENGTH (204) //user can't modify this value !!!


/*
 * @brief      for user to allocate bis parameters buffer. both broadcast and Synchronize use the API.
 * @param[in]  pBisPara - start address of BIS parameters buffer.
 * @param[in]  bis_bcst_num - the bis broadcast number application layer may use.
 * @param[in]  bis_sync_num - the bis Synchronized number application layer may use.
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_InitBisParametersBuffer(u8 *pBisPara, u8 bis_bcst_num, u8 bis_sync_num);


#endif
