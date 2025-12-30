/********************************************************************************************************
 * @file    prd_adv.h
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
#ifndef STACK_BLE_CONTROLLER_LL_PRDADV_PRD_ADV_H_
#define STACK_BLE_CONTROLLER_LL_PRDADV_PRD_ADV_H_


/* maximum number of periodic advertising this SDK can support */
#define PERIODIC_ADV_NUMBER_MAX 2

#define PERD_ADV_PARAM_LENGTH   460 //user can't modify this value !!! sizeof(st_prd_adv_t)


typedef void (*periodic_adv_sch_cb)(void);

/**
 * @brief       for user to register callback function for periodic advertising start.
 * @param[in]   cb - callback function pointer.
 * @return      none
 */
void blc_ll_periodic_adv_start_cb(periodic_adv_sch_cb cb);

/**
 * @brief       for user to register callback function for periodic advertising end.
 * @param[in]   cb - callback function pointer.
 * @return      none
 */
void blc_ll_periodic_adv_end_cb(periodic_adv_sch_cb cb);


/**
 * @brief      for user to initialize periodic advertising module, and to allocate periodic advertising parameters buffer
 * @param[in]  pBuff - global buffer allocated by application layer.
 * @param[in]  num_periodic_adv - number of application adv_sets
 * @return     Status - 0x00: command succeeded;
 * 						0x12: num_periodic_adv exceed maximum number of supported periodic advertising.
 */
ble_sts_t blc_ll_initPeriodicAdvModule_initPeriodicdAdvSetParamBuffer(u8 *pBuff, int num_periodic_adv);


/**
 * @brief      This function is used by the Host to set the parameters for periodic advertising.
 * @param[in]  adv_handle - - Used to identify a periodic advertising train
 * @param[in]  advInter_min - Periodic_Advertising_Interval_Min(Range: 0x0006 to 0xFFFF, Time = N * 1.25 ms Time Range: 7.5 ms to 81.91875 s)
 * @param[in]  advInter_max - Periodic_Advertising_Interval_Max
 * @param[in]  property - Periodic_Advertising_Properties
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPeriodicAdvParam(adv_handle_t adv_handle, u16 advInter_min, u16 advInter_max, perd_adv_prop_t property);

/**
 * @brief      initialize Periodic Advertising Data buffer for all adv_set
 * @param[in]  perdAdvData -
 * @param[in]  max_len_perdAdvData -
 * @return     none
 */
void blc_ll_initPeriodicAdvDataBuffer(u8 *perdAdvData, int max_len_perdAdvData);

/**
 * @brief      initialize Periodic Advertising Data buffer for specific adv_set.
 *			   notice that:
 * @param[in]  adv_handle - equal to adv_set index here. So if using single adv_set, adv_handle can only be 0x00;
 *                          if using multiple adv_set, for example 3, adv_handle can be 0x00,0x01 and 0x02
 * @param[in]  perdAdvData -
 * @param[in]  max_len_perdAdvData -
 * @return     Status - 0x00: succeed.
 * 						0x12: adv_handle out of range.
 */
ble_sts_t blc_ll_initPeriodicAdvDataBuffer_by_advHandle(u8 adv_handle, u8 *perdAdvData, int max_len_perdAdvData);

/**
 * @brief      This function is used to set the data used in periodic advertising PDUs.
 * @param[in]  adv_handle - - equal to adv_set index here.
 * @param[in]  advData_len -
 * @param[in]  *advdata -
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPeriodicAdvData(adv_handle_t adv_handle, u16 advData_len, const u8 *advdata);


/**
 * @brief      This function is used to enable or disable the periodic advertising for the advertising
 *			   set specified by the Advertising_Handle parameter
 * @param[in]  per_adv_enable - Advertising_Enable
 * @param[in]  adv_handle     - Used to identify an advertising set.
 * @return     Status         - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPeriodicAdvEnable(u8 per_adv_enable, adv_handle_t adv_handle);


#endif /* STACK_BLE_CONTROLLER_LL_PRDADV_PRD_ADV_H_ */
