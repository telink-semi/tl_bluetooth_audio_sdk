/********************************************************************************************************
 * @file    hid_manage.h
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
#ifndef HID_MANAGE_H
#define HID_MANAGE_H

enum
{
    HID_SUCCESS,
    HID_FAIL,
    HID_NULL_POINTER,
    HID_INVALID_PARAM,
    HID_WAIT_ACK
};

typedef uint8_t (*hid_manage_send_data_t)(uint8_t *, const uint8_t);

/**
 * @brief Initialize the HID management module
 *
 * This function initializes the HID management module by allocating buffer cache
 * for packet storage based on the provided parameters.
 *
 * @param[in] data_len      Length of data in each packet
 * @param[in] pkg_num       Number of packets in the buffer cache
 * @param[in] flush_count   Maximum flush count for holding packets
 *
 * @return HID_SUCCESS on success, HID_FAIL on failure
 */
int hid_manage_init(uint8_t data_len, uint8_t pkg_num, uint8_t flush_count);

/**
 * @brief Set the current HID management mode
 *
 * This function sets the repetition and confirmation mode, and resets the
 * sequence number and acknowledgment sequence number.
 *
 * @param[in] repetition    Repetition mode flag (non-zero to enable)
 * @param[in] confirmation  Confirmation mode flag (non-zero to enable)
 */
void hid_manage_set_current_mode(uint8_t repetition, uint8_t confirmation);

/**
 * @brief Reset the current HID management mode
 *
 * This function resets the current mode by disabling both repetition
 * and confirmation modes.
 */
void hid_manage_reset_current_mode(void);

/**
 * @brief Set the received acknowledgment sequence number
 *
 * This function updates the received ACK sequence number and clears
 * acknowledged packets from the buffer cache.
 *
 * @param[in] ackSeqNum     The acknowledgment sequence number received
 */
void hid_manage_set_receive_ack_seq_num(uint8_t ackSeqNum);

/**
 * @brief Make hybrid mode data packet
 *
 * This function creates HID data packets in hybrid mode (stacking or FIFO)
 * based on the current repetition mode setting. It manages packet buffering
 * and sequencing.
 *
 * @param[out] sdu_data     Pointer to the output SDU data buffer
 * @param[in]  reportID     HID report ID
 * @param[in]  sample       Pointer to the input sample data
 * @param[in]  sample_len   Length of the sample data
 *
 * @return Total length of the generated SDU data, 0 on error
 */
uint8_t hid_manage_make_hybrid_mode_data(uint8_t *sdu_data, uint8_t reportID, uint8_t *sample, uint8_t sample_len);

/**
 * @brief Parse hybrid mode data packet
 *
 * This function parses received SDU data in hybrid mode, extracts report data,
 * and handles acknowledgment if confirmation mode is enabled.
 *
 * @param[in]  sdu_data              Pointer to the input SDU data
 * @param[in]  sdu_len               Length of the SDU data
 * @param[out] report_data           Pointer to store the extracted report data pointer
 * @param[in]  hid_manage_send_data  Callback function to send acknowledgment data
 *
 * @return Length of the extracted report data, 0 if ACK packet is received
 */
uint8_t hid_manage_parse_hybrid_mode_data(const uint8_t *sdu_data, uint8_t sdu_len, uint8_t **report_data, hid_manage_send_data_t hid_manage_send_data);

/**
 * @brief Check the current HID management state
 *
 * This function checks if the HID management module is in a valid state
 * to send data. It returns HID_WAIT_ACK if confirmation mode is enabled
 * and there are unacknowledged packets.
 *
 * @return HID_SUCCESS if ready to send, HID_WAIT_ACK if waiting for acknowledgment
 */
int hid_manage_check_state(void);

/**
 * @brief Send HID ISO data
 *
 * This function sends HID data over ISO (Isochronous) channel.
 *
 * @param[in] acl_handle    ACL connection handle
 * @param[in] data          Pointer to the data to send
 * @param[in] data_len      Length of the data to send
 *
 * @return Status of the send operation
 */
uint8_t hid_manage_send_hid_iso_data(uint16_t acl_handle, uint8_t *data, uint8_t data_len);
#endif //HID_MANAGE_H
