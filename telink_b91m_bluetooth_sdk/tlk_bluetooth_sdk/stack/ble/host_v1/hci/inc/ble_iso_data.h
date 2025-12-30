/********************************************************************************************************
 * @file    ble_iso_data.h
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


/* PB_Flag  */
#define BLE_HCI_ISO_PB_FIRST        (0)
#define BLE_HCI_ISO_PB_CONTINUATION (1)
#define BLE_HCI_ISO_PB_COMPLETE     (2)
#define BLE_HCI_ISO_PB_LAST         (3)

/* Packet_Status_Flag (in packets sent by the Controller) */
#define BLE_HCI_ISO_PKT_STATUS_VALID   0x00
#define BLE_HCI_ISO_PKT_STATUS_INVALID 0x01
#define BLE_HCI_ISO_PKT_STATUS_LOST    0x10

/**
 * @brief Definition for HCI ISO data header format.
 *
 */
struct ble_host_hci_iso_hdr
{
    uint16_t handle       : 12; /** < Connection handle */
    uint16_t pbFlag       : 2;  /** < Packet boundary flag */
    uint16_t tsFlag       : 1;  /** < Timestamp flag */
    uint16_t rfu          : 1;
    uint16_t dataTotalLen : 14; /** < Total length of data */
    uint16_t rfu2         : 2;
} __attribute__((packed));

/**
 *   @brief Definition for HCI ISO data header format.
 */
struct ble_host_hci_iso_payload
{
    uint16_t packet_seq_num;
    uint16_t iso_sdu_length     : 12;
    uint16_t rfu                : 2;
    uint16_t packet_status_flag : 2;
    uint8_t  iso_data[0];
} __attribute__((packed));

/**
 *   @brief Definition for HCI ISO data payload add time stamp format.
 */
struct ble_host_hci_iso_payload_ts
{
    uint32_t                        timestamp;
    struct ble_host_hci_iso_payload payload;
} __attribute__((packed));

/**
 * @brief   Definition for HCI ISO data format.
 */
struct ble_host_hci_iso
{
    struct ble_host_hci_iso_hdr hdr;

    union
    {
        struct ble_host_hci_iso_payload    payload;
        struct ble_host_hci_iso_payload_ts payload_ts;
    };
} __attribute__((packed));

/**
 *  @brief Definition for HCI ISO data H4 format.
 */
struct ble_host_hci_iso_h4
{
    uint8_t                     type;
    struct ble_host_hci_iso_hdr hdr;
    uint8_t                     data[0];
} __attribute__((packed));

typedef void (*iso_data_receive_callback_t)(uint16_t conn_handle, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data);


/**
 *   @brief Initialize the ISO data module.
 *
 *   @param[in] iso_data_callback The callback function to be called when ISO data is received.
 *   @return None.
 */
void ble_host_iso_data_init(iso_data_receive_callback_t iso_data_callback);

/**
 *   @brief Deinitialize the ISO data module.
 *
 *   @return None.
 */
void ble_host_iso_data_deinit(void);

/**
 *   @brief Send ISO data.
 *
 *   @param[in] iso_conn_handle The ISO connection handle.
 *   @param[in] timestamp The timestamp.
 *   @param[in] p_data The data to send.
 *   @param[in] data_len The length of the data to send.
 *   @return BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_send_ble_iso_data(uint16_t iso_conn_handle, uint32_t timestamp, const uint8_t *p_data, uint16_t data_len);
