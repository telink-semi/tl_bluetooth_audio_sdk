/********************************************************************************************************
 * @file    ble_smp_packet.h
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
 * @brief       Handles the legacy pairing request from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_request_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing request from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_request_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the legacy pairing confirm from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_confirm_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing confirm from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_confirm_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the legacy pairing random from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_random_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing random from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_random_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the pairing public key exchange from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_public_key_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the pairing DHKey check from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_pairing_dhkey_check_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the pairing failed message from peer device.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_pairing_failed_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the encryption information (LTK) from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_encryption_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the central identification (EDIV and RAND) from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_central_identification_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the identity information (IRK) from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_identity_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the identity address information from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_identity_address_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the signing information (CSRK) from central device in peripheral role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_peripheral_signing_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/************* Central pairing *************/
/**
 * @brief       Handles the security request from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_security_request_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the encryption information (LTK) from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_encryption_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the central identification (EDIV and RAND) from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_central_identification_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the identity information (IRK) from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_identity_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the identity address information from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_identity_address_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the signing information (CSRK) from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_signing_information_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the legacy pairing response from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_response_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing response from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_response_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the legacy pairing confirm from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_confirm_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing confirm from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_confirm_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the legacy pairing random from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_random_handler_legacy(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the secure connections pairing random from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_random_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the pairing public key exchange from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_public_key_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);

/**
 * @brief       Handles the pairing DHKey check from peripheral device in central role.
 * @param[in]   deal_info   - pointer to the SMP deal information structure.
 * @param[in]   rx_pdu      - pointer to the received SMP PDU format.
 * @return      The length of the response packet if a response is needed, 0 otherwise.
 */
uint16_t ble_host_smp_central_pairing_dhkey_check_handler(const struct ble_host_smp_deal_info *deal_info, const struct ble_smp_pdu_format *rx_pdu);
