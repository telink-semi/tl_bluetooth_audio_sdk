/********************************************************************************************************
 * @file    ble_att_uuid.h
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


#define CHECK_ATT_UUID(uuid) ((uuid) == NULL || ((uuid)->uuidLength != ATT_16_UUID_LEN && (uuid)->uuidLength != ATT_128_UUID_LEN))

/** @Bluetooth UUID Lengths */
#define ATT_16_UUID_LEN  2  /** Length in bytes of a 16 bit UUID */
#define ATT_128_UUID_LEN 16 /** Length in bytes of a 128 bit UUID */

struct att_uuid
{
    uint8_t uuidLength; /** Length of the UUID in bytes 2 or 16 */

    union
    {
        uint8_t  uuid[0];
        uint16_t uuid16;
        uint8_t  uuid128[ATT_128_UUID_LEN];
    };
};

/** @brief Initialize a UUID structure with a 16 bit UUID */
#define UUID16BIT_INIT(uuid) {.uuidLength = ATT_16_UUID_LEN, .uuid16 = (uuid)}

/** @brief Initialize a UUID structure with a 128 bit UUID */
#define UUID128BIT_INIT(...)                                       \
    {                                                              \
        .uuidLength = ATT_128_UUID_LEN, .uuid128 = { __VA_ARGS__ } \
    }

#define UUID16BIT_DEF(name, uuid) const struct att_uuid name = UUID16BIT_INIT(uuid)

#define UUID128BIT_DEF(name, ...) const struct att_uuid name = UUID128BIT_INIT(__VA_ARGS__)

const char *ble_att_uuid_format(const struct att_uuid *uuid);

/**
 *  @brief Compare two attribute UUIDs.
 *
 *  @param[in] uuid1 First UUID to compare.
 *  @param[in] uuid2 Second UUID to compare.
 *
 *  @return 0 if the UUIDs are equal, otherwise if the UUIDs are not equal or if one of the UUIDs is invalid.
*/
int ble_att_uuid_cmp(const struct att_uuid *uuid1, const struct att_uuid *uuid2);

/**
 *  @brief Compare a 16-bit UUID with a 128-bit UUID.
 *
 *  @param[in] pUuid16 Pointer to a 16-bit UUID.
 *  @param[in] pUuid128 Pointer to a 128-bit UUID.
 *
 *  @return true if the 16-bit UUID matches the 128-bit UUID, false otherwise.
*/
bool ble_uuid_cmp_uuid16_uuid128(const uint8_t *pUuid16, const uint8_t *pUuid128);

/**
 *  @brief Compare a 16-bit UUID with a UUID of any length.
 *
 *  @param[in] uuid16 a 16-bit UUID.
 *  @param[in] uuidLen Length of the UUID to compare.
 *  @param[in] pUuid Pointer to the UUID to compare.
 *
 *  @return true if the 16-bit UUID matches the UUID of any length, false otherwise.
*/
bool ble_uuid_cmp_uuid16_uuid(uint16_t uuid16, uint8_t uuidLen, const uint8_t *pUuid);
