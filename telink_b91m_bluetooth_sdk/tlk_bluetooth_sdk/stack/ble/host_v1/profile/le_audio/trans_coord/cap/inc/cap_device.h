/********************************************************************************************************
 * @file    cap_device.h
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
#ifndef CAP_DEVICE_H
#define CAP_DEVICE_H

struct cap_device_adv_data_value
{
    uint8_t flags;
    bool    lea_audio_flags;
    uint8_t complete_name_len;
    char    complete_name[32];
    bool    rsi_flags;
    uint8_t rsi[6];
};

enum
{
    CAP_DEVICE_STATE_DISCONNECTED,
    CAP_DEVICE_STATE_CONNECTED,
};

typedef void (*cap_device_state_cb_t)(uint8_t device_id, uint8_t conn_state);

/**
 * @brief       Initializes the CAP device module.
 * @param[in]   cb  - callback invoked when device connection state changes.
 * @return      none.
 * @note        Call once before using other CAP device APIs.
 */
void cap_device_init(cap_device_state_cb_t cb);

/**
 * @brief       Inserts advertising information for a discovered device.
 * @param[in]   addr_type   - peer address type.
 * @param[in]   addr        - pointer to the 6-byte peer address.
 * @param[in]   adv_data    - pointer to parsed advertising data.
 * @return      0 if inserted successfully, negative value otherwise.
 */
int cap_device_insert_adv(uint8_t addr_type, const uint8_t addr[6], const struct cap_device_adv_data_value *adv_data);

/**
 * @brief       Retrieves the SIRK associated with a connection handle.
 * @param[in]   handle  - ACL connection handle.
 * @return      Pointer to the 16-byte SIRK buffer, or NULL if not available.
 */
uint8_t *cap_device_get_sirk(uint16_t handle);

/**
 * @brief       Gets the remaining device count available for a connection.
 * @param[in]   handle  - ACL connection handle.
 * @return      Number of remaining devices.
 */
uint8_t cap_device_get_remain_device_number(uint16_t handle);

/**
 * @brief       Gets the number of paired CSIS sets stored.
 * @return      Total paired set count.
 */
uint8_t cap_device_get_paired_set_number(void);

/**
 * @brief       Gets the CSIS set ID associated with a connection handle.
 * @param[in]   handle  - ACL connection handle.
 * @return      Set ID value, or 0xFF if not found.
 */
uint8_t cap_device_get_device_set_id(uint16_t handle);

/**
 * @brief       Retrieves the peer address for a given connection handle.
 * @param[in]   handle      - ACL connection handle.
 * @param[out]  addr_type   - pointer to store the address type.
 * @param[out]  mac         - pointer to store the 6-byte address.
 * @return      0 on success, negative value if the handle is not tracked.
 */
int cap_device_get_device_mac_by_handle(uint16_t handle, uint8_t addr_type[], uint8_t mac[6]);

/**
 * @brief       Retrieves paired device addresses by set identifier.
 * @param[in]   id          - paired device set identifier.
 * @param[out]  addr_type   - pointer to store the address type.
 * @param[out]  mac         - 2D array storing addresses per set.
 * @return      Number of addresses copied, or 0 on failure.
 */
uint8_t cap_device_get_paired_device_mac_by_set_id(uint8_t id, uint8_t addr_type[], uint8_t mac[][6]);

/**
 * @brief       Runs profile discovery for a device and reports the event.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   addr_type   - peer address type.
 * @param[in]   addr        - pointer to the 6-byte peer address.
 * @param[in]   event_id    - discovery event identifier.
 * @param[in]   event_msg   - pointer to event-specific data.
 * @return      0 on success, negative value if discovery cannot proceed.
 */
int cap_device_profile_discover(uint16_t conn_handle, uint8_t addr_type, const uint8_t addr[6], uint8_t event_id, const void *event_msg);

/**
 * @brief       Removes a connection from the CAP device tracking table.
 * @param[in]   handle  - ACL connection handle to remove.
 * @return      0 on success, negative value if the handle is not stored.
 */
int cap_device_remove_conn(uint16_t handle);
#endif //CAP_DEVICE_H