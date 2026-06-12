/********************************************************************************************************
 * @file    ble_hci.h
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
#ifndef STACK_BLE_HOST_V1_BLE_HCI_H_
#define STACK_BLE_HOST_V1_BLE_HCI_H_


#define BLE_HCI_OPCODE_NOP (0)

enum ble_host_hci_malloc_type
{
    BLE_HOST_HCI_MALLOC_TYPE = 0x100,
    BLE_HOST_HCI_MALLOC_RX_BUFFER,
    BLE_HOST_HCI_MALLOC_RX_FIFO,
    BLE_HOST_HCI_MALLOC_ACL_DATA_PACKET,
    BLE_HOST_HCI_MALLOC_ISO_DATA_PACKET,
};

/* Definition of HCI H4 packet types */
enum
{
    BLE_HCI_H4_NONE = 0x00,
    BLE_HCI_H4_CMD  = 0x01,
    BLE_HCI_H4_ACL  = 0x02,
    BLE_HCI_H4_SCO  = 0x03,
    BLE_HCI_H4_EVT  = 0x04,
    BLE_HCI_H4_ISO  = 0x05
};

enum ble_host_hci_error_code
{
    BLE_HOST_HCI_ERR_SUCCESS = 0,
    BLE_HOST_HCI_ERR_WAIT_TIMEOUT,
    BLE_HOST_ERR_CONTROLLER,                 /** Controller error */
    BLE_HOST_HCI_ERR_INSUFFICIENT_RESOURCES, /** Insufficient resources */
    BLE_HOST_HCI_ERR_NULL_POINTER,           /** Null pointer */
    BLE_HOST_HCI_ERR_INVALID_PARAM,          /** Invalid parameter */
    BLE_HOST_HCI_ERR_INVALID_CONN_HANDLE,
    BLE_HOST_HCI_ERR_CONNECTION_BUSY,
    BLE_HOST_HCI_ERR_UNSUPPORTED_FEATURE,
};

#define BLE_HOST_HCI_ASSERT(cond)                                                                      \
    do {                                                                                               \
        if (!(cond)) {                                                                                 \
            BLE_HOST_HCI_LE_CMD_ERROR("ble host hci assert func:%s, line:%d", __FUNCTION__, __LINE__); \
        }                                                                                              \
    } while (0)


/**
 *   @brief BLE Host HCI RX event callback type.
 *
 *   @param[in] p_evt - Pointer to the HCI event packet.
 *
 *   @return None.
 */
typedef void (*ble_host_hci_rx_event_callback_t)(void *p_evt);

/**
 *   @brief BLE Host HCI RX ACL data callback type.
 *
 *   @param[in] acl_data - Pointer to the ACL data packet.
 *
 *   @return None.
 */
typedef void (*ble_host_hci_rx_acl_data_callback_t)(void *acl_data);

/**
 *   @brief BLE Host HCI RX ISO data callback type.
 *
 *   @param[in] iso_data - Pointer to the ISO data packet.
 *
 *   @return None.
 */
typedef void (*ble_host_hci_rx_iso_data_callback_t)(void *iso_data);

/******************************************************************************
 * Functions
 ******************************************************************************/
/**
 *   @brief BLE Host HCI initialization.
 *
 *   @return None.
 */
void ble_host_hci_init(void);

/**
 *   @brief BLE Host HCI Receive HCI packet.
 *
 *   @param[in] data        - Pointer to the HCI packet data.
 *   @param[in] len         - Length of the HCI packet data.
 *
 *   @return None.
 *
 *   @note This function is Host HCI data input API, called by the Application Layer.
 */
void ble_host_hci_rx_packet(const uint8_t *data, uint32_t len);

/**
 *   @brief BLE Host HCI Receive HCI complete packet.
 *
 *   @param[in] data        - Pointer to the HCI complete packet data.
 *   @param[in] len         - Length of the HCI complete packet data.
 *
 *   @return None.
 *
 *   @note This function is Host HCI data input API, called by the Application Layer.
 *         If the application layer receives only complete HCI data packets, this interface can be called.
 *         Similar to the software HCI communication method within the chip, complete HCI data packets are exchanged each time.
 */
void ble_host_hci_rx_complete_packet(uint8_t *data, uint32_t len);

/**
 *   @brief BLE Host HCI memory pool malloc.
 *
 *   @param[in] size        - Size of the memory to allocate.
 *   @param[in] type_id     - Type ID of the memory to allocate.
 *
 *   @return Pointer to the allocated memory, or NULL if allocation failed.
 */
void *ble_host_hci_malloc(uint32_t size, uint16_t type_id);

/**
 *   @brief BLE Host HCI memory pool free.
 *
 *   @param[in] ptr         - Pointer to the memory to free.
 *
 *   @return None.
 */
void ble_host_hci_free(void *ptr);

/**
 *   @brief BLE Host HCI register RX event callback.
 *
 *   @param[in] callback - Pointer to the RX event callback function.
 *
 *   @return None.
 */
void ble_host_hci_register_rx_event_callback(ble_host_hci_rx_event_callback_t callback);

/**
 *   @brief BLE Host HCI register RX ACL data callback.
 *
 *   @param[in] callback - Pointer to the RX ACL data callback function.
 *
 *   @return None.
 */
void ble_host_hci_register_rx_acl_data_callback(ble_host_hci_rx_acl_data_callback_t callback);

/**
 *   @brief BLE Host HCI register RX ISO data callback.
 *
 *   @param[in] callback - Pointer to the RX ISO data callback function.
 *
 *   @return None.
 */
void ble_host_hci_register_rx_iso_data_callback(ble_host_hci_rx_iso_data_callback_t callback);

/**
 *   @brief BLE Host HCI send a vendor specific command.
 *
 *   @param[in] ocf The opcode of the command.
 *   @param[in] cmdbuf The command buffer.
 *   @param[in] cmdlen The length of the command buffer.
 *   @param[in] rspbuf The response buffer.
 *   @param[in] rsplen The length of the response buffer.
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_vs_cmd_tx(uint16_t ocf, const void *cmdbuf, uint8_t cmdlen, void *rspbuf, uint8_t rsplen);

/**
 *   @brief BLE Host HCI send a packet.
 *
 *   @param[in] data The data to send.
 *   @param[in] len The length of the data to send.
 *   @return None.
 */
void ble_host_hci_send_packet(const uint8_t *data, uint16_t len);

#endif /* STACK_BLE_HOST_V1_BLE_HCI_H_ */
