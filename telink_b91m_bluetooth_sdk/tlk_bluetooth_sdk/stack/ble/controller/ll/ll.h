/********************************************************************************************************
 * @file    ll.h
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
#ifndef LL_H_
#define LL_H_

#include "tl_common.h"
#include "common/types.h"
#include "stack/ble/ble_common.h"

/**
 * @brief	Telink defined LinkLayer Event Callback
 */
typedef void (*user_irq_handler_cb_t)(void);
extern user_irq_handler_cb_t usr_irq_handler_cb;

/**
 * @brief	irq_handler for BLE stack, process system tick interrupt and RF interrupt
 * @param	none
 * @return	none
 */
void blc_sdk_irq_handler(void);

/**
 * @brief   main_loop for BLE stack, process data and event
 * @param	none
 * @return	none
 */
void blc_sdk_main_loop(void);


/**
 * @brief      for user to initialize link layer Standby state
 * @param	   none
 * @return     none
 */
void blc_ll_initStandby_module(u8 *public_adr);


/**
 * @brief      this function is used to read MAC address
 * @param[in]  *addr -  The address where the read value(MAC address) prepare to write.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_readBDAddr(u8 *addr);


/**
 * @brief      this function is used to write MAC address
 * @param[in]  *addr -  The address where the write value(MAC address) prepare to write.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_writeBDAddr(u8 *addr);

/**
 * @brief      this function is used to set the LE Random Device Address in the Controller
 * @param[in]  *randomAddr -  Random Device Address
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_setRandomAddr(u8 *randomAddr);


/**
 * @brief      this function is used check if any controller buffer initialized by application incorrect.
 * 			   attention: this function must be called at the end of BLE LinkLayer Initialization.
 * @param	   none
 * @return     status, 0x00:  succeed, no buffer error
 * 					   other: buffer error code
 */
init_err_t blc_contr_checkControllerInitialization(void);

#define blc_controller_check_appBufferInitialization blc_contr_checkControllerInitialization

/**
 * @brief      this function is used by the Host to specify a channel classification based on its local information,
 *             only the ACL Central role is valid.
 * @param[in]  *map - channel map
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_setHostChannel(u8 *chnMap);


/**
 * @brief      this function is used to reset module of all.
 * @param	   none
 * @return     status, 0x00:  succeed, no buffer error
 * 					   other: buffer error code
 */
ble_sts_t blc_hci_reset(void);


/**
 * @brief      This function is used to set some other channel to replace advertising & scanning channel 37/38/39.
 * @param[in]  chn0 - channel to replace channel 37
 * @param[in]  chn1 - channel to replace channel 38
 * @param[in]  chn2 - channel to replace channel 39
 * @return     none
 */
void blc_ll_setCustomizedAdvertisingScanningChannel(u8 chn0, u8 chn1, u8 chn2);


#define blc_ll_initBasicMCU()


void blc_ll_addBLEFeature(void);

#endif /* LL_H_ */
