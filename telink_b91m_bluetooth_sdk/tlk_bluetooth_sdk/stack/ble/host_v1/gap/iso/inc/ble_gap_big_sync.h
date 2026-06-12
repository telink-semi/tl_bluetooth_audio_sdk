/********************************************************************************************************
 * @file    ble_gap_big_sync.h
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

/** < Structure for BLE GAP Layer create BIG Sync Param */
struct ble_host_gap_big_sync_create_param
{
    uint8_t  big_handle;         /** < Broadcast Isochronous Group(BIG) Synchronization handle. */
    uint16_t sync_handle;        /** < Periodic Advertising Synchronization handle. */
    bool     enc;                /** < Encryption enable or not. */
    uint8_t  broadcast_code[16]; /** < Broadcast code, only used for enc is true. */
    uint8_t  num_bis;            /** < Number of Broadcast Isochronous Groups(BIGs). */
    uint8_t  bis_index[31];      /** < Index of each Broadcast Isochronous Group(BIG). */
};

/** < Structure for BLE GAP Layer report BIG Info */
struct ble_host_gap_big_info
{
    uint16_t pa_sync_handle;
    uint8_t  bis_num;
    uint8_t  nse;
    uint16_t iso_interval; /** < unit: 1.25ms */
    bool     enc;
};

/** < Structure for BLE GAP Layer report BIG Sync Param */
struct ble_host_gap_big_sync_param
{
    uint32_t transport_latency; /** < unit: 1us */
    uint8_t  num_bis;
    uint16_t iso_handles[31]; /** < connection handle of BIG-Sync connection, bis maximal number is 31. */
};

/**
 *   @brief Callback function to receive BIG Info.
 *
 *   This function is called when a BIG Info is received from a peer device.
 *
 *   @param[in] big_info    Pointer to a structure containing the BIG Info.
 */
typedef void (*ble_host_gap_big_info_callback)(struct ble_host_gap_big_info *big_info);

/**
 *   @brief Callback function to receive BIG Sync Param.
 *
 *   This function is called when a BIG Sync established.
 *
 *   @param[in] status      Status of the BIG Sync establishment.
 *   @param[in] param       Pointer to a structure containing the BIG Sync Param.
 */
typedef void (*ble_host_gap_big_sync_established_callback)(uint8_t status, struct ble_host_gap_big_sync_param *param);

/**
 *   @brief Callback function to receive BIG Sync lost.
 *
 *   This function is called when a BIG Sync lost.
 *
 *   @param[in] big_handle  The handle of the lost BIG.
 *   @param[in] reason      Reason of the lost.
 */
typedef void (*ble_host_gap_big_sync_lost_callback)(uint8_t big_handle, uint8_t reason);

/** < Structure for BLE GAP Layer BIG Sync Callbacks */
struct ble_host_gap_big_sync_callbacks
{
    ble_host_gap_big_sync_established_callback established_cb; /** < Callback function to receive BIG Sync established. */
    ble_host_gap_big_sync_lost_callback        lost_cb;        /** < Callback function to receive BIG Sync lost. */
};

/**
 *   @brief Initialize the Broadcast Isochronous Group(BIG) Synchronization module.
 *
 *   This function should be called before any other Broadcast Isochronous Group(BIG) Synchronization functions.
 */
void ble_host_gap_big_sync_init(void);

/**
 *   @brief Create a new Broadcast Isochronous Group(BIG) Synchronization handle.
 *
 *   @return A new Broadcast Isochronous Group(BIG) Synchronization handle.
 */
uint8_t ble_host_gap_big_sync_create_big_handle(void);

/**
 *   @brief Delete a Broadcast Isochronous Group(BIG) Synchronization handle.
 *
 *   @param[in] big_handle  The handle of the Broadcast Isochronous Group(BIG) Synchronization to be deleted.
 *
 *   @return None.
 */
void ble_host_gap_big_sync_delete_big_handle(uint8_t big_handle);

/**
 *   @brief Register a callback function to receive BIG Info, filter by PA synced handle.
 *
 *   @param[in] sync_handle PA synced handle.
 *   @param[in] big_info    Callback function to receive BIG Info.
 *
 *   @return None.
 */
void ble_host_gap_big_sync_register_big_info(uint16_t sync_handle, ble_host_gap_big_info_callback callback);

/**
 *   @brief Create a BIG Synchronization instance.
 *
 *   @param[in] param       Pointer to a structure containing the parameters for creating a BIG Synchronization instance.
 *   @param[in] callbacks   The callback functions for the BIG Synchronization, must not be NULL and global pointer..
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan parameters successfully, otherwise return error code.
 */
int ble_host_gap_big_sync_create(struct ble_host_gap_big_sync_create_param *param, const struct ble_host_gap_big_sync_callbacks *callbacks);

/**
 *   @brief Terminate a BIG Synchronization instance.
 *
 *   @param[in] big_handle  The handle of the BIG Synchronization instance to be terminated.
 *
 *   @return BLE_HOST_ERR_SUCC if terminate successfully, otherwise return error code.
 */
int ble_host_gap_big_sync_terminate(uint8_t big_handle);
