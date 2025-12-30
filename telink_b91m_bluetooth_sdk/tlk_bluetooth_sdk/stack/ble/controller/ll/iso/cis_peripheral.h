/********************************************************************************************************
 * @file    cis_peripheral.h
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
#ifndef CIS_PERIPHERAL_H_
#define CIS_PERIPHERAL_H_


//#define 	    CIS_SLV_PARAM_LEN							 sizeof(ll_cis_slv_t)
#define CIS_SLV_PARAM_LEN 920 //user can't modify this value !!!


typedef void (*cis_sub_event_start_cb)(unsigned int, unsigned int);
void blt_ll_cis_peripher_sub_event_start_cb(cis_sub_event_start_cb cb);


/**
 * @brief      for user to initialize CIS peripheral module and allocate CIS peripheral parameters buffer.
 * @param[in]  pCisPerParamBuf - start address of CIS peripheral parameters buffer
 * @param[in]  cis_per_num - CIS peripheral number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_initCisPeriphrModule_initCisPeriphrParametersBuffer(u8 *pCisPerParamBuf, int cis_per_num);


/**
 * @brief	   this function is used by the Peripheral's Host to inform the Controller to accept the request for the CIS
 *             that is identified by the Connection_Handle
 * @param[in]  cisHandle - Connection handle of the CIS
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_acceptCisRequest(u16 cisHandle);


/**
 * @brief	   this function  is used by the Peripheral's Host to inform the Controller to reject the request for the CIS
 *             that is identified by the Connection_Handle.
 * @param[in]  cisHandle - Connection handle of the CIS to be rejected
 * @param[in]  reason - Reason the CIS request was rejected
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_rejectCisReq(u16 cisHandle, u8 reason);


#endif /* CIS_PERIPHERAL_H_ */
