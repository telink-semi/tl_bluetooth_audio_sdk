/********************************************************************************************************
 * @file    ble_gap_big.h
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

/** < Structure for BIG create parameters. */
struct ble_host_gap_big_create_param
{
    uint8_t  big_handle;         /** < handle of the BIG. */
    uint8_t  adv_handle;         /** < handle of the extend advertising. */
    uint8_t  bis_number;         /** < number of BISes, max of 31. */
    uint32_t sdu_interval;       /** < sdu interval, unit: 1us. */
    uint16_t iso_interval;       /** < iso interval, unit: sdu interval. */
    uint16_t transport_latency;  /** < transport latency. unit: sdu interval. */
    uint16_t sdu_size;           /** < sdu size, unit: octets. */
    uint8_t  retry_count;        /** < number of retransmissions. */
    bool     enc;                /** < encryption enabled or not. */
    uint8_t  broadcast_code[16]; /** < broadcast code. if encryption is enabled, it should be 16 bytes. */
};

/** < Struct for BIG create test parameters. */
struct ble_host_gap_big_create_test_param
{
    uint8_t  big_handle;         /** < handle of the BIG. */
    uint8_t  adv_handle;         /** < handle of the extend advertising. */
    uint8_t  bis_number;         /** < number of BISes, max of 31. */
    uint32_t sdu_interval;       /** < sdu interval, unit: 1us. */
    uint16_t iso_interval;       /** < iso interval, unit: sdu interval. */
    uint16_t sdu_size;           /** < sdu size, unit: octets. */
    uint8_t  nse;                /** < number of subevents. */
    uint8_t  bn;                 /** < burst number. */
    uint8_t  irc;                /** < Immediate Repetition Count. */
    uint8_t  pto;                /** < Previous Transmission Offset. */
    bool     enc;                /** < encryption enabled or not. */
    uint8_t  broadcast_code[16]; /** < broadcast code. if encryption is enabled, it should be 16 bytes. */
};

/** < Structure for BIG create callback parameters. */
struct ble_host_ble_host_gap_big_create_callback_param
{
    uint8_t  big_handle;        /** < handle of the BIG. */
    uint32_t sync_delay;        /** < BIG synchronization delay, unit 1us. */
    uint32_t transport_latency; /** < transport latency. unit: sdu interval. */
    uint8_t  bis_number;        /** < number of BISes, max of 31. */
    uint16_t bis_handles[31];   /** < connection handle of BISes, max of 31. */
};

/**
 *   @brief Callback function for BIG create.
 *
 *   @param status      Status of the operation.
 *   @param param       Pointer to the callback parameters.
 *
 *   @return None.
 */
typedef void (*ble_host_gap_big_create_callback)(uint8_t status, struct ble_host_ble_host_gap_big_create_callback_param *param);

/**
 *   @brief Callback function for BIG terminate.
 *
 *   @param big_handle  Handle of the BIG.
 *
 *   @return None.
 */
typedef void (*ble_host_gap_big_terminate_callback)(uint8_t big_handle);

/**
 *   @brief Initialize the BIG Broadcaster module.
 *
 *   @return None.
 */
void ble_host_gap_big_init(void);

/**
 *   @brief Create a handle for a new BIG.
 *
 *   @return Handle of the new BIG.
 */
uint8_t ble_host_gap_big_create_handle(void);

/**
 *   @brief Delete a handle of a BIG.
 *
 *   @param big_handle  Handle of the BIG to be deleted.
 *
 *   @return None.
 */
void ble_host_gap_big_delete_handle(uint8_t big_handle);

/**
 *    @brief Create a BIG with the specified parameters.
 *
 *    @param param       Pointer to the create parameters.
 *    @param callback    Pointer to the callback function, should be NULL if not needed.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_big_create_param(const struct ble_host_gap_big_create_param *param, ble_host_gap_big_create_callback callback);

/**
 *   @brief Create a BIG with the specified test parameters.
 *
 *   @param param       Pointer to the create test parameters.
 *   @param callback    Pointer to the callback function, should be NULL if not needed.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_big_create_test_param(const struct ble_host_gap_big_create_test_param *param, ble_host_gap_big_create_callback callback);

/**
 *   @brief Terminate a BIG.
 *
 *   @param big_handle  Handle of the BIG to be terminated.
 *   @param callback    Pointer to the callback function, should be NULL if not needed.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_big_terminate(uint8_t big_handle, ble_host_gap_big_terminate_callback callback);
