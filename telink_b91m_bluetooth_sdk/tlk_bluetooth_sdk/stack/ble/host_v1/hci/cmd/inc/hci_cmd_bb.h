/********************************************************************************************************
 * @file    hci_cmd_bb.h
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
 * @brief Reset hci.
 *
 * @param None
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_send_reset(void);

/**
 * @brief Sets the event mask for the HCI callback.
 *
 * @param[in] p_event_mask Pointer to the structure containing the event mask.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_set_event_mask(const struct ble_hci_cb_set_event_mask_cp *p_event_mask);

/**
 * @brief Sets Controller to Host Flow Control for the controller.
 *
 * @param[in] p_flow_ctrl Pointer to the structure containing the Controller to Host Flow Control.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_set_controller_to_host_flow_control(const struct ble_hci_cb_ctlr_to_host_fc_cp *p_flow_ctrl);

/**
 * @brief Sets Host Buffer Size for the controller.
 *
 * @param[in] p_host_buf_size Pointer to the structure containing the Host Buffer Size.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_set_host_buffer_size(const struct ble_hci_cb_host_buf_size_cp *p_host_buf_size);

/**
 * @brief Host Number of Completed Packets for the controller.
 *
 * @param[in] p_host_num_comp_pkts Pointer to the structure containing the Host Number of Completed Packets.
 *
 * @return none.
 */
void ble_host_hci_host_num_of_completed_packets(const struct ble_hci_cb_host_num_comp_pkts_cp *p_host_num_comp_pkts);

/**
 * @brief Sets the event mask 2 for the HCI callback.
 *
 * @param[in] p_event_mask2 Pointer to the structure containing the event mask 2.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_set_event_mask2(const struct ble_hci_cb_set_event_mask2_cp *p_event_mask2);