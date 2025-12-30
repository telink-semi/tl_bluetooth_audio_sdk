/********************************************************************************************************
 * @file    utility.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "types.h"
#include "utility.h"
#include <stdio.h>

/**
 * @brief      Swap the byte order of data in place
 * @param[in]  p - pointer to the data to swap
 * @param[in]  n - number of bytes to swap
 * @return     none
 */
void swapN(unsigned char *p, int n)
{
    int i, c;
    for (i = 0; i < n / 2; i++) {
        c            = p[i];
        p[i]         = p[n - 1 - i];
        p[n - 1 - i] = c;
    }
}

/**
 * @brief      Copy data from source to destination with swapped byte order
 * @param[in]  src - pointer to the source data
 * @param[out] dst - pointer to the destination buffer
 * @param[in]  len - number of bytes to copy and swap
 * @return     none
 */
void swapX(const u8 *src, u8 *dst, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        dst[len - 1 - i] = src[i];
    }
}

/**
 * @brief      Copy and swap 3-byte data (e.g., 24-bit values)
 * @param[out] dst - pointer to the destination 3-byte array
 * @param[in]  src - pointer to the source 3-byte array
 * @return     none
 */
void swap24(u8 dst[3], const u8 src[3])
{
    swapX(src, dst, 3);
}

/**
 * @brief      Copy and swap 4-byte data (e.g., 32-bit values)
 * @param[out] dst - pointer to the destination 4-byte array
 * @param[in]  src - pointer to the source 4-byte array
 * @return     none
 */
void swap32(u8 dst[4], const u8 src[4])
{
    swapX(src, dst, 4);
}

/**
 * @brief      Copy and swap 6-byte data (e.g., 48-bit values/MAC addresses)
 * @param[out] dst - pointer to the destination 6-byte array
 * @param[in]  src - pointer to the source 6-byte array
 * @return     none
 */
void swap48(u8 dst[6], const u8 src[6])
{
    swapX(src, dst, 6);
}

/**
 * @brief      Copy and swap 7-byte data
 * @param[out] dst - pointer to the destination 7-byte array
 * @param[in]  src - pointer to the source 7-byte array
 * @return     none
 */
void swap56(u8 dst[7], const u8 src[7])
{
    swapX(src, dst, 7);
}

/**
 * @brief      Copy and swap 8-byte data (e.g., 64-bit values)
 * @param[out] dst - pointer to the destination 8-byte array
 * @param[in]  src - pointer to the source 8-byte array
 * @return     none
 */
void swap64(u8 dst[8], const u8 src[8])
{
    swapX(src, dst, 8);
}

/**
 * @brief      Copy and swap 16-byte data (e.g., 128-bit values)
 * @param[out] dst - pointer to the destination 16-byte array
 * @param[in]  src - pointer to the source 16-byte array
 * @return     none
 */
void swap128(u8 dst[16], const u8 src[16])
{
    swapX(src, dst, 16);
}

/**
 * @brief      Convert hexadecimal data to string representation
 * @param[in]  buf - pointer to the buffer containing hexadecimal data
 * @param[in]  len - length of the data in bytes
 * @return     Pointer to a static string containing the hexadecimal representation
 * @note       The returned string is statically allocated and will be overwritten on subsequent calls
 */
const char *hex_to_str(const void *buf, u8 len)
{
    static const char hex[] = "0123456789abcdef";
    static char       str[301];
    const uint8_t    *b = buf;
    u8                i;

    len = min(len, (sizeof(str) - 1) / 3);

    for (i = 0; i < len; i++) {
        str[i * 3]     = hex[b[i] >> 4];
        str[i * 3 + 1] = hex[b[i] & 0xf];
        str[i * 3 + 2] = ' ';
    }

    str[i * 3] = '\0';

    return str;
}

/**
 * @brief      Convert a Bluetooth address to string representation
 * @param[in]  addr - pointer to the 6-byte Bluetooth address
 * @return     Pointer to a static string containing the formatted Bluetooth address
 * @note       The returned string is statically allocated and will be overwritten on subsequent calls
 */
const char *addr_to_str(const u8 *addr)
{
#define BDADDR_STR_LEN 18

    static char addrStr[BDADDR_STR_LEN];
    snprintf(addrStr, sizeof(addrStr), "%02X:%02X:%02X:%02X:%02X:%02X", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
    return addrStr;
}
