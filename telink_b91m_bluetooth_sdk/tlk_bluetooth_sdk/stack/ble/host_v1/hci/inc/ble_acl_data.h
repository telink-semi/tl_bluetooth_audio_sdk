/********************************************************************************************************
 * @file    ble_acl_data.h
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
#ifndef STACK_BLE_HOST_V1_BLE_ACL_DATA_H_
#define STACK_BLE_HOST_V1_BLE_ACL_DATA_H_

// #ifdef __cplusplus
// extern "C" {
// #endif


#define BLE_HCI_PB_FIRST_NON_FLUSH 0
#define BLE_HCI_PB_MIDDLE          1
#define BLE_HCI_PB_FIRST_FLUSH     2
#define BLE_HCI_PB_FULL            3

/**
 *  @brief Definition packet boundary flag. core_v5.4 page 1802.
 *  note: H-C mean 'Host to Controller', C-H mean 'Controller to Host'.
 *  NA mean 'Not allowed', A mean 'Allowed'.
 *  From left to right, they are APB-U, ACL-U, and LE-U.
*/
enum hci_acl_pb_flag
{
    // First non-automatically-flushable packet of a higher layer message
    //(start of a non - automatically - flushable L2CAP PDU) from Host to Controller.
    HCI_ACL_PB_FLAG_FIRST_NAF_PACKET = 0x00, /** < H-C : NA/ A/ A; C-H : NA/ NA/ A */
    // Continuing fragment of a higher layer message
    HCI_ACL_PB_FLAG_CONTINUE_PACKET = 0x01, /** < H-C : A/ A/ A; C-H : A/ A/ A */
    //First automatically flushable packet of a higher layer message
    // (start of an automatically - flushable L2CAP PDU).
    HCI_ACL_PB_FLAG_FIRST_AP_PACKET = 0x02, /** < H-C : A/ A/ NA; C-H : A/ A/ A */
    // Previously used
    HCI_ACL_PB_FLAG_PREVIOUSLY_USER = 0x03,

    // only LE ACL connect, host to controller , start L2CAP PDU, LLID:0b10
    HCI_LE_ACL_PB_FLAG_H_TO_C_START_L2CAP = 0x00,
    // only LE ACL connect, Continue L2CAP PDU, LLID:0b01
    HCI_LE_ACL_PB_FLAG_CONTINUE_L2CAP = 0x01,
    // only LE ACL connect, controller to host , start L2CAP PDU, LLID:0b10
    HCI_LE_ACL_PB_FLAG_C_TO_H_START_L2CAP = 0x02
};

/**
 *  @brief Definition broadcast flag. core_v5.4 page 1802.
*/
enum hci_acl_bc_flag
{
    HCI_ACL_PC_FLAG_POINT_TO_POINT   = 0x00, /** < Point-to-point (ACL-U or LE-U) */
    HCI_ACL_PC_FLAG_BR_EDR_BROADCAST = 0x01, /** < BR/EDR broadcast (APB-U) */
    HCI_ACL_PC_FLAG_RFU,                     /** < Reserved for future use */
};

// status is only valid if send_finish_callback is not NULL
typedef void (*finish_callback)(struct ble_host_conn *p_conn, const uint8_t *p_data, bool status);

typedef void (*recv_ble_acl_data_cb_t)(struct ble_host_conn *p_conn, bool is_start, const uint8_t *p_data, uint16_t data_len);

/**
 *  @brief Definition for HCI ACL data format.
*/
struct ble_host_hci_acl_data
{
    uint16_t handle : 12; /** < connect handle. */
    uint16_t pbFlag : 2;  /** < refer to enum hci_acl_pb_flag */
    uint16_t bcFlag : 2;  /** < refer to enum hci_acl_bc_flag */
    uint16_t dataTotalLength;
    uint8_t  data[0];
} __attribute__((packed));

/**
 *  @brief Definition for HCI ACL data format.
*/
struct ble_host_hci_acl_data_h4
{
    uint8_t  type;
    uint16_t handle : 12; /** < connect handle. */
    uint16_t pbFlag : 2;  /** < refer to enum hci_acl_pb_flag */
    uint16_t bcFlag : 2;  /** < refer to enum hci_acl_bc_flag */
    uint16_t dataTotalLength;
    uint8_t  data[0];
} __attribute__((packed));

struct ble_acl_data_pkt
{
    const uint8_t  *p_data;
    uint16_t        data_length;
    finish_callback cb;
};

#define BLE_HCI_DATA_HDR_SZ               4
#define BLE_HCI_DATA_HANDLE(handle_pb_bc) (((handle_pb_bc) & 0x0fff) >> 0)
#define BLE_HCI_DATA_PB(handle_pb_bc)     (((handle_pb_bc) & 0x3000) >> 12)
#define BLE_HCI_DATA_BC(handle_pb_bc)     (((handle_pb_bc) & 0xc000) >> 14)

struct hci_acl_dat
{
    uint16_t hdh_handle_pb_bc;
    uint16_t hdh_len;
    uint8_t  data[0];
};

/**
 * @brief Initialize the BLE ACL data module
 *
 * This function initializes the ACL data transmission and reception module.
 * It registers the receive callback, schedules ACL data transmission events,
 * registers ACL connection callbacks, reads the controller buffer size,
 * and initializes the send message queues.
 *
 * @param[in] cb    Callback function to be called when ACL data is received
 */
void ble_host_acl_data_init(const recv_ble_acl_data_cb_t cb);

/**
 * @brief Set the maximum pending number of ACL data packets per connection
 *
 * This function sets the maximum number of ACL data packets that can be
 * pending for each connection. The value will be clamped to the total
 * number of available packets if it exceeds the limit.
 *
 * @param[in] num   Maximum pending number of ACL data packets per connection
 */
void ble_host_hci_acl_data_set_max_pending_num(uint8_t num);

/**
 * @brief Enable or disable controller-to-host flow control
 *
 * This function enables or disables the flow control mechanism from controller
 * to host. When enabled, it also sets the host buffer size to inform the
 * controller about the host's buffer capacity.
 *
 * @param[in] enable     true to enable flow control, false to disable
 * @param[in] total_num  Total number of ACL data packets the host can buffer
 */
void ble_host_hci_acl_data_enable_flow(bool enable, uint8_t total_num);

// #ifdef __cplusplus
// }
// #endif

#endif /* STACK_BLE_HOST_V1_BLE_ACL_DATA_H_ */
