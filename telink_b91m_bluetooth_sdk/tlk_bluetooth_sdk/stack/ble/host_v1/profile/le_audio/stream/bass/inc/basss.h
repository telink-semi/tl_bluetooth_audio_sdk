/********************************************************************************************************
 * @file    basss.h
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
// BASSS: Broadcast Audio Scan Service Server.

enum ble_basss_event_id
{
    BASSS_EVENT_ID_REMOTE_SCAN_STOPPED,
    BASSS_EVENT_ID_REMOTE_SCAN_STARTED,
    BASSS_EVENT_ID_REMOVE_SOURCE,
    BASSS_EVENT_ID_NOT_SYNC_PA,
    BASSS_EVENT_ID_PAST_TIMEOUT,
    BASSS_EVENT_ID_SYNC_TO_PA,
    BASSS_EVENT_ID_SYNC_TO_BIS,
    BASSS_EVENT_ID_SET_BROADCAST_CODE,
};

struct ble_basss_register_param
{
    uint16_t past_timer; /** < BASS using PAST timer. unit ms */
};

struct basss_sync_to_pa_event
{
    uint8_t source_id;
    uint8_t adv_addr_type;
    uint8_t adv_addr[6];
    uint8_t adv_sid;
    uint8_t broadcast_id[3];
    uint8_t pa_sync_state;
};

struct bass_sync_to_bis_event
{
    uint8_t                    source_id;
    uint8_t                    num_subgroups;
    uint8_t                    index_subgroup;
    uint32_t                   bis_sync;
    struct lea_metadata_parsed metadata;
};

typedef void (*ble_basss_event_callback)(uint16_t conn_handle, enum ble_basss_event_id event_id, const void *event_msg);

/**
 *   @brief Register the Broadcast Audio Scan Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the BASSS module.
 *
 *   @return None.
 */
void ble_lea_register_BASS_control_server(const struct ble_basss_register_param *param);

/**
 *   @brief Set the event callback for the Broadcast Audio Scan Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 */
void ble_lea_register_BASS_event_callback(ble_basss_event_callback callback);

/**
 *   @brief Update the PA sync state of the specified source.
 *
 *   @param[in] source_id The ID of the source.
 *   @param[in] synced The PA sync state, true for synced, false for failed to sync.
 *   @param[in] lost True if the source lost the PA sync, ignored synced parameter.
 *
 *   @return True if the PA sync state is updated successfully, False otherwise.
 */
bool ble_basss_update_pa_sync_state(uint8_t source_id, bool synced, bool lost);

/**
 *   @brief Set the BIG encryption state of the specified source.
 *
 *   @param[in] source_id The ID of the source.
 *   @param[in] big_encryption The BIG encryption state, 0x00: Not encrypted,
 *                              0x01: broadcast code required, 0x02: Decrypting, 0x03: Bad code.
 *   @param[in] bad_code The BAD code of the source, ignored if big_encryption is not 0x03.
 *
 *   @return True if the BIG encryption key is set successfully, False otherwise.
 */
bool ble_basss_set_big_encryption_state(uint8_t source_id, uint8_t big_encryption, uint8_t bad_code[16]);

/**
 * @brief       Updates the BIS sync state for a specific subgroup.
 * @param[in]   source_id       - identifier of the broadcast source.
 * @param[in]   index_subgroup  - index of the subgroup within the source.
 * @param[in]   bis_sync        - bit mask indicating synchronized BISes.
 * @return      true if the state is updated successfully, false otherwise.
 * @note        Use when BIS sync changes due to LL BIG events.
 */
bool ble_basss_update_bis_sync_state(uint8_t source_id, uint8_t index_subgroup, uint32_t bis_sync);

/**
 * @brief       Indicates that the PAST receive operation is complete.
 * @param[in]   conn_handle - the connection handle.
 * @return      none.
 * @note        Should be called when the PAST receive operation is complete.
 */
void ble_basss_past_recv_done(uint16_t conn_handle);
