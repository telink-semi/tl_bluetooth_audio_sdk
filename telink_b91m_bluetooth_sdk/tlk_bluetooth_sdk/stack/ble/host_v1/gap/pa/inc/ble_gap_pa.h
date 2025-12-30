/********************************************************************************************************
 * @file    ble_gap_pa.h
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

/**
 *   @brief Initialize the GAP Periodic Advertising module.
 *
 *   This function initializes the GAP Periodic Advertising module.
 */
void ble_host_gap_pa_init(void);

/**
 *   @brief Set the parameters of a periodic advertising.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *   @param[in] interval_min Minimum interval for periodic advertising, unit 1.25ms.
 *   @param[in] interval_max Maximum interval for periodic advertising, unit 1.25ms.
 *
 *   @return BLE_STATUS_SUCCESS if the parameters were set successfully, otherwise an error code.
 */
int ble_host_gap_pa_set_param(uint8_t adv_handle, uint16_t interval_min, uint16_t interval_max);

/**
 *   @brief Set the parameters of a periodic advertising ms.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *   @param[in] interval_min Minimum interval for periodic advertising, unit 1ms.
 *   @param[in] interval_max Maximum interval for periodic advertising, unit 1ms.
 *
 *   @return BLE_STATUS_SUCCESS if the parameters were set successfully, otherwise an error code.
 */
int ble_host_gap_pa_set_param_ms(uint8_t adv_handle, uint16_t interval_min_ms, uint16_t interval_max_ms);

/**
 *   @brief Set the advertising data of a periodic advertising.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *   @param[in] adv_data Periodic advertising data.
 *   @param[in] adv_data_len Length of the periodic advertising data.
 */
int ble_host_gap_pa_set_adv_data(uint8_t adv_handle, const uint8_t *adv_data, uint8_t adv_data_len);

/**
 *   @brief set enable or disable a periodic advertising.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *   @param[in] enable Enable or disable periodic advertising.
 *   @param[in] incl_adi Include or exclude ADI in periodic advertising.
 *
 *   @return BLE_STATUS_SUCCESS if the enable/disable operation was successful, otherwise an error code.
 */
int ble_host_gap_pa_set_enable(uint8_t adv_handle, bool enable, bool incl_adi);

/**
 *   @brief Enable a periodic advertising without including ADI.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *
 *   @return BLE_STATUS_SUCCESS if the enable operation was successful, otherwise an error code.
 */
int ble_host_gap_pa_enable(uint8_t adv_handle);

/**
 *   @brief Disable a periodic advertising.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *
 *   @return BLE_STATUS_SUCCESS if the disable operation was successful, otherwise an error code.
 */
int ble_host_gap_pa_disable(uint8_t adv_handle);

/**
 *   @brief Update the channel map of a periodic advertising.
 *
 *   @param[in] adv_handle Extended advertising handle.
 *   @param[in] channel_map Channel map for periodic advertising, range Bit0 to Bit36.
 *
 *   @return BLE_STATUS_SUCCESS if the channel map was updated successfully, otherwise an error code.
 */
int ble_host_gap_pa_update_channel_map(uint8_t adv_handle, uint64_t channel_map);
