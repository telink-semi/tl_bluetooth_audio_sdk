/********************************************************************************************************
 * @file    contr_comp.h
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
#ifndef STACK_BLE_CONTROLLER_CONTR_API_COMP_H_
#define STACK_BLE_CONTROLLER_CONTR_API_COMP_H_


#define PM_SLEEP_ACL_SLAVE  PM_SLEEP_ACL_PERIPHR
#define PM_SLEEP_ACL_MASTER PM_SLEEP_ACL_CENTRAL


/* for BLE controller old API name compatible with new SDK */

#define blc_ll_initInitiating_module    blc_ll_initLegacyInitiating_module


#define blc_ll_initAclSlaveRole_module  blc_ll_initAclPeriphrRole_module
#define blc_ll_initAclConnSlaveTxFifo   blc_ll_initAclPeriphrTxFifo


#define blc_ll_initAclMasterRole_module blc_ll_initAclCentralRole_module
#define blc_ll_initAclConnMasterTxFifo  blc_ll_initAclCentralTxFifo


#define blc_ll_setAdvCustomedChannel    blc_ll_setCustomizedAdvertisingScanningChannel


#define ll_whiteList_reset              blc_ll_clearWhiteList
#define ll_whiteList_add                blc_ll_addDeviceToWhiteList
#define ll_whiteList_delete             blc_ll_removeDeviceFromWhiteList


#endif /* STACK_BLE_CONTROLLER_CONTR_API_COMP_H_ */
