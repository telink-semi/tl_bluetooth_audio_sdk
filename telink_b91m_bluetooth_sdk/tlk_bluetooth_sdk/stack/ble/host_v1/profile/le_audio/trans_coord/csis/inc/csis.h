/********************************************************************************************************
 * @file    csis.h
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
// CSIS: Coordinated Set Identification Service

/** @brief Set Identity Resolving Key Type */
enum csis_set_identity_resolving_key_type
{
    BLE_CSIS_SIRK_ENCRYPTED = 0x00,
    BLE_CSIS_SIRK_PLAIN     = 0x01,
    BLE_CSIS_SIRK_OOB       = 0x02,
};

/**
 * @brief       Checks if the RSI is valid.
 * @param[in]   rsi - pointer to the RSI buffer (6 bytes).
 * @param[in]   sirk - pointer to the SIRK buffer (16 bytes).
 * @return      true if the RSI is valid, false otherwise.
 * @note        The RSI is valid if it is generated from the SIRK.
 */
bool ble_csis_check_rsi_valid(uint8_t rsi[6], uint8_t sirk[16]);
