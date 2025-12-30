/********************************************************************************************************
 * @file    ble_misc.h
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

#pragma once

#define LTV_UNPACK_SUCCESS         0x00
#define LTV_UNPACK_ERR_LENGTH_ZERO 0x01
#define LTV_UNPACK_ERR_LENGTH_LOW  0x02

/**
 *  @brief LTV(length, type, value) data structure
 *  @details This structure is used to represent LTV data.
*/
struct ltv_data
{
    uint8_t length;   /** < Length of the value field */
    uint8_t type;     /** < Type of the value field */
    uint8_t value[0]; /** < Value field */
} __attribute__((packed));

/**
 * @brief       Packs an array of LTV entries into a serialized buffer.
 * @param[in]   ltv         - NULL-terminated array of pointers to LTV entries.
 * @param[out]  out_data    - pointer to the destination buffer for serialized bytes.
 * @return      Total number of bytes written to the buffer.
 * @note        The buffer must be large enough to hold all entries.
 */
int32_t ltv_pack(const struct ltv_data *ltv[], void *out_data);

/**
 * @brief       Unpacks serialized LTV data and invokes a handler for each entry.
 * @param[in]   in_data     - pointer to the serialized LTV data buffer.
 * @param[in]   in_data_len - length of the serialized data in bytes.
 * @param[in]   handler     - callback invoked per entry; returning 0 stops further parsing.
 * @param[in]   user        - user-defined context passed to the handler.
 * @return      LTV_UNPACK_SUCCESS on success, otherwise an LTV_UNPACK_ERR_* code.
 * @note        Parsing stops if the handler returns 0 or an error is detected.
 */
int32_t ltv_unpack(const uint8_t *in_data, int32_t in_data_len, int (*handler)(uint8_t length, uint8_t type, const uint8_t *value, void *user), void *user);
