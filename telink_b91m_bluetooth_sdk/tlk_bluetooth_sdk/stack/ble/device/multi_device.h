/********************************************************************************************************
 * @file    multi_device.h
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
#ifndef STACK_BLE_DEVICE_MULTI_DEVICE_H_
#define STACK_BLE_DEVICE_MULTI_DEVICE_H_

/**
 *  @brief  local device index
 *  if multiple device function not needed, please use DEFAULT_DEVICE_INDEX in any relative APIs
 */
typedef enum
{
    LOCAL_DEVICE_INDEX_0 = 0x00,
    DEFAULT_DEVICE_INDEX = 0x00,
    LOCAL_DEVICE_INDEX_1 = 0x01,
    LOCAL_DEVICE_INDEX_2 = 0x02,
    LOCAL_DEVICE_INDEX_3 = 0x03,
} loc_dev_idx_t;

/**
 *  @brief  multiple local device enable
 */
typedef enum
{
    MULTI_DEV_DISABLE = 0x00,
    MULTI_DEV_ENABLE  = 0x01,
} multi_dev_en_t;

/**
 * @brief      for user to enable multiple local device feature. If not set, this feature is disabled by default.
 * 			   attention: this API must be called before SMP initialization if user need this feature!!!
 * @param[in]  enable - enable or disable.
 * @return     none
 */
void blc_ll_setMultipleLocalDeviceEnable(multi_dev_en_t enable);


/**
 * @brief      for user to bond local device index and identity address
 * @param[in]  local_dev_idx - local device index
 * @param[in]  id_adrType -  local device identity address type, 0 for public, 1 for random
 * @param[in]  id_addr - local device identity address, must be public address or static random address
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_setLocalDeviceIndexAndIdentityAddress(loc_dev_idx_t local_dev_idx, u8 id_adrType, u8 *id_addr);


/**
 * @brief      for user to indicate which device to use now.
 * 			   attention: This API must be called after blc_ll_setMaxConnectionNumber().
 * @param[in]  local_dev_idx - local device index
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_setCurrentLocalDevice_by_index(loc_dev_idx_t local_dev_idx);


#endif /* STACK_BLE_DEVICE_MULTI_DEVICE_H_ */
