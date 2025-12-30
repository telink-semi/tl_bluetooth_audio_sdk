/********************************************************************************************************
 * @file    ble_rand.h
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
 *   @brief BLE Host Generate Random Number.asm
 *
 *   @return random number
 *
 *   @note This function generates a random number using the HCI LE Rand command.
 */
int ble_host_rand(void);

/**
 *   @brief BLE Host Generate Random Number, fixed 128-bits buffer.
 *
 *   @param[out] buffer Pointer to 128-bits buffer to store the random number.
 *
 *   @note This function generates a random number using the HCI LE Rand command
 *           and stores it in a 128-bits buffer.
 */
void ble_host_rand_128bit_buffer(uint8_t buffer[16]);

/**
 *   @brief BLE Host Generate Random Number, variable length buffer.
 *
 *   @param[out] buffer Pointer to buffer to store the random number.
 *   @param[in] len Length of the buffer in bytes, range 1-65535.
 *
 *   @note This function generates a random number using the HCI LE Rand command
 *          and stores it in a buffer of variable length.
 */
void ble_host_rand_buffer(uint8_t *buffer, int len);
