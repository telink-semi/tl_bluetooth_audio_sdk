/********************************************************************************************************
 * @file    csiss.h
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
// CSISS: Coordinated Set Identification Service Server.

/** < @brief CSIS service error codes. */
enum ble_csiss_error_code
{
    BLE_CSISS_ERR_START = 0x80,
    BLE_CSISS_ERR_SIRK_TYPE_INVALID,
    BLE_CSISS_ERR_SIRK_TYPE_OOB,
};

/** < @brief CSIS service register parameter structure. */
struct ble_csiss_register_param
{
    uint8_t set_size; //Coordinated Set Size:1-255
    uint8_t set_rank; //Set Member Rank, must less than or equal to set_size

    enum csis_set_identity_resolving_key_type SIRK_type;      //exposes SIRK type, 0:Encrypted, 1:plain text, 2:only OOB
    uint8_t                                   plain_SIRK[16]; //Set Identity Resolving Key

    const char *set_member_name; //Set Member Name, max length 128 bytes
};

/**
 *   @brief Register the LE Audio Coordinated Set Identification Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the CSIS module.
 *
 *   @return None.
 */
void ble_lea_register_CSIS_control_server(const struct ble_csiss_register_param *param);

/**
 *    @brief Set the Set Identity Resolving Key value.
 *
 *    @param[in] type SIRK type, 0:Encrypted, 1:plain text, 2:only OOB
 *    @param[in] plain_SIRK Set Identity Resolving Key value
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 *            - BLE_CSISS_ERR_SIRK_TYPE_INVALID if the SIRK type is invalid.
 */
int ble_csiss_set_SIRK_value(enum csis_set_identity_resolving_key_type type, const uint8_t plain_SIRK[16]);

/**
 *    @brief Notify the Set Identity Resolving Key value to the client.
 *
 *    @param[in] conn_handle Connection handle to the client.
 *    @param[in] type SIRK type, 0:Encrypted, 1:plain text, 2:only OOB
 *    @param[in] plain_SIRK Set Identity Resolving Key value
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 *            - BLE_CSISS_ERR_SIRK_TYPE_INVALID if the SIRK type is invalid.
 *            - BLE_CSISS_ERR_SIRK_TYPE_OOB if the SIRK type is OOB and cannot be notified.
 */
int ble_csiss_notify_SIRK_value(uint16_t conn_handle, enum csis_set_identity_resolving_key_type type, const uint8_t plain_SIRK[16]);

/**
 *    @brief Set the Coordinated Set Size.
 *
 *    @param[in] size Coordinated Set Size.
 *
 *    @return None.
 */
void ble_csiss_set_size(uint8_t size);

/**
 *    @brief Notify the Coordinated Set Size to the client.
 *
 *    @param[in] conn_handle Connection handle to the client.
 *    @param[in] size Coordinated Set Size.
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 */
int ble_csiss_notify_size(uint16_t conn_handle, uint8_t size);

/**
 *    @brief Notify the current lock state to the client.
 *
 *    @param[in] conn_handle Connection handle to the client.
 *
 *    @return BLE_HOST_ERR_SUCC if the operation is successful.
 */
int ble_csiss_notify_current_lock_state(uint16_t conn_handle);

/**
 *    @brief Generate a random RSI value.
 *
 *    @param[out] rsi Pointer to the buffer to store the RSI value.
 *
 *    @return None.
 */
void ble_csiss_generate_random_rsi(uint8_t rsi[6]);
