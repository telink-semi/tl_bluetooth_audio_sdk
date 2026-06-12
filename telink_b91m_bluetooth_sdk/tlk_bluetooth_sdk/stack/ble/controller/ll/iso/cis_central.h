/********************************************************************************************************
 * @file    cis_central.h
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
#ifndef CIS_CENTRAL_H_
#define CIS_CENTRAL_H_


//#define 	    CIG_PARAM_LEN							        sizeof(ll_cig_mst_t)
#define CIG_PARAM_LEN 972


/**
 * @brief      for user to initialize CIS central module and allocate CIG parameters buffer.
 * @param[in]  pCigParamBuf - start address of CIG parameters buffer
 * @param[in]  cig_num - CIG number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_initCisCentralModule_initCigParametersBuffer(u8 *pCigParamBuf, int cig_num);


/**
 * @brief	   set CIG anchor point offset from ACL Central anchor point
 * 			   attention: this is special timing customization !!!
 * @param[in]  acl_cen_index - ACL central index
 * @param[in]  offset_custom_us - customized offset uS
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_setCigTimingOffsetOfAclCentral(u8 acl_cen_index, u16 offset_custom_us);

/**
 * @brief	   create a CIG and to set the parameters of one or more CISes that are associated with a CIG in the Controller.
 * @param[in]  pCmdParam - Command Parameters buffer pointer
 * @param[out] pRetParam - return Parameters buffer pointer
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_hci_le_setCigParams(hci_le_setCigParam_cmdParam_t *pCmdParam, hci_le_setCigParam_retParam_t *pRetParam);

#if (ULL_FOR_CIS_EN)
ble_sts_t blc_hci_le_setCigParamsULL(hci_le_setCigParamTest_cmdParam_t *pCmdParam, hci_le_setCigParam_retParam_t *pRetParam);
#endif

/**
 * @brief	  create one or more CISes using the connections identified by the ACL_Connection_Handle arrayed parameter.
 * @param[in] pCisPara - Command Parameters buffer pointer
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_hci_le_createCis(hci_le_CreateCisParams_t *pCisPara);

ble_sts_t blc_hci_le_removeCig(u8 cigId, hci_le_removeCig_retParam_t *);

#endif /* CIS_CENTRAL_H_ */
