/********************************************************************************************************
 * @file    utility.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef COMMON_UTILITY_H_
#define COMMON_UTILITY_H_

#pragma once

#include "compiler.h"
#include "types.h"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef min2
#define min2(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef max2
#define max2(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef max3
#define max3(a, b, c) max2(max2(a, b), c)
#endif

#define OFFSETOF(s, m) ((unsigned int)&((s *)0)->m)

#ifndef container_of
#if (CHIP_TYPE == CHIP_TYPE_TL652X)
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
#else
#define container_of(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
#endif
#endif

#define ROUND_INT(x, r)      (((x) + (r) - 1) / (r) * (r))
#define ROUND_TO_POW2(x, r)  (((x) + (r) - 1) & ~((r) - 1))

#define MAKE_U16(h, l)       ((unsigned short)(((h) << 8) | (l)))
#define MAKE_U24(a, b, c)    ((unsigned int)(((a) << 16) | ((b) << 8) | (c)))
#define MAKE_U32(a, b, c, d) ((unsigned int)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d)))

#define IS_POWER_OF_2(x)     (!(x & (x - 1)))
#define IS_4BYTE_ALIGN(x)    (!(x & 3))

#ifndef IS_LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN (*(unsigned short *)"\0\xff" > 0x100)
#endif

// better than xor swap:
// http://stackoverflow.com/questions/3912699/why-swap-with-xor-works-fine-in-c-but-in-java-doesnt-some-puzzle
#define SWAP(x, y, T) \
    do {              \
        T tmp = (x);  \
        (x)   = (y);  \
        (y)   = tmp;  \
    } while (0)
#define SORT2(a, b, T)         \
    do {                       \
        if ((a) > (b))         \
            SWAP((a), (b), T); \
    } while (0)

#define foreach(i, n) for (int i = 0; i < (n); ++i)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define U16_HI(a)     (((a) >> 8) & 0xFF)
#define U16_LO(a)     ((a) & 0xFF)

#define U32_BYTE0(a)  ((a) & 0xFF)
#define U32_BYTE1(a)  (((a) >> 8) & 0xFF)
#define U32_BYTE2(a)  (((a) >> 16) & 0xFF)
#define U32_BYTE3(a)  (((a) >> 24) & 0xFF)


#ifndef SUCCESS
#define SUCCESS 0
#endif

/**
 * @brief      Swap the byte order of data in place
 * @param[in]  p - pointer to the data to swap
 * @param[in]  n - number of bytes to swap
 * @return     none
 */
void swapN(unsigned char *p, int n);

/**
 * @brief      Copy data from source to destination with swapped byte order
 * @param[in]  src - pointer to the source data
 * @param[out] dst - pointer to the destination buffer
 * @param[in]  len - number of bytes to copy and swap
 * @return     none
 */
void swapX(const u8 *src, u8 *dst, int len);

/**
 * @brief  two u32 type data multiply, result is u64.
 *
 * @param[in]  u: data 1
 * @param[in]  v: data 2
 * @param[out] none
 *
 * @returns u64 result
 */
__INLINE u64 mul64_32x32(u32 u, u32 v)
{
#if 0 // Eagle HW support this process
    u32  u0, v0, w0;
    u32  u1, v1, w1, w2, t;
    u32  x, y;

    u0 = u & 0xFFFF;
    u1 = u >> 16;
    v0 = v & 0xFFFF;
    v1 = v >> 16;
    w0 = u0 * v0;
    t = u1 * v0 + (w0 >> 16);
    w1 = t & 0xFFFF;
    w2 = t >> 16;
    w1 = u0 * v1 + w1;

    //x is high 32 bits, y is low 32 bits

    x = u1 * v1 + w2 + (w1 >> 16);
    y = u * v;


    return(((u64) x << 32) | y);
#else
    return (u64)u * v;
#endif
}

/**
 * @brief      Get absolute value
 * @param[in]  i - input value
 * @return     Absolute value of i
 */
__INLINE int abs_ram(signed int i)
{
    return i >= 0 ? i : -i;
}

typedef struct
{
    u32 size;
    u16 num;
    u8  wptr;
    u8  rptr;
    u8 *p;
} my_fifo_t;

#define DATA_LENGTH_ALIGN4(n)  (((n) + 3) / 4 * 4)
#define DATA_LENGTH_ALIGN16(n) (((n) + 15) / 16 * 16)


#define U16_TO_BYTES(n)        ((u8)(n)), ((u8)((n) >> 8))
#define U24_TO_BYTES(n)        ((u8)(n)), ((u8)((n) >> 8)), ((u8)((n) >> 16))
#define U32_TO_BYTES(n)        ((u8)(n)), ((u8)((n) >> 8)), ((u8)((n) >> 16)), ((u8)((n) >> 24))

#define BYTE_TO_UINT8(n, p)    (n) = *(const uint8_t *)(p)
#define BYTE_TO_UINT16(n, p)   (n) = ((uint16_t)(p)[0] + ((uint16_t)(p)[1] << 8))
#define BYTE_TO_UINT24(n, p)   (n) = ((uint32_t)(p)[0] + ((uint32_t)(p)[1] << 8) + ((uint32_t)(p)[2] << 16))
#define BYTE_TO_UINT32(n, p)   (n) = ((uint32_t)(p)[0] + ((uint32_t)(p)[1] << 8) + ((uint32_t)(p)[2] << 16) + ((uint32_t)(p)[3] << 24))
#define BYTE_TO_UINT40(n, p)   (n) = ((uint64_t)(p)[0] + ((uint64_t)(p)[1] << 8) + ((uint64_t)(p)[2] << 16) + ((uint64_t)(p)[3] << 24) + ((uint64_t)(p)[4] << 32))
#define BYTE_TO_UINT64(n, p)                                                                                                                                        \
    (n) = ((uint64_t)(p)[0] + ((uint64_t)(p)[1] << 8) + ((uint64_t)(p)[2] << 16) + ((uint64_t)(p)[3] << 24) + ((uint64_t)(p)[4] << 32) + ((uint64_t)(p)[5] << 40) + \
           ((uint64_t)(p)[6] << 48) + ((uint64_t)(p)[7] << 56))

#define STREAM_TO_U8(n, p)   \
    do {                     \
        BYTE_TO_UINT8(n, p); \
        p++;                 \
    } while (0)

#define STREAM_TO_U16(n, p)   \
    do {                      \
        BYTE_TO_UINT16(n, p); \
        p += 2;               \
    } while (0)

#define STREAM_TO_U24(n, p)   \
    do {                      \
        BYTE_TO_UINT24(n, p); \
        p += 3;               \
    } while (0)

#define STREAM_TO_U32(n, p)   \
    do {                      \
        BYTE_TO_UINT32(n, p); \
        p += 4;               \
    } while (0)

#define STREAM_TO_U40(n, p)   \
    do {                      \
        BYTE_TO_UINT40(n, p); \
        p += 5;               \
    } while (0)

#define STREAM_TO_U64(n, p)   \
    do {                      \
        BYTE_TO_UINT64(n, p); \
        p += 8;               \
    } while (0)

#define STREAM_TO_STR(n, p, l) \
    do {                       \
        memcpy(n, p, l);       \
        p += l;                \
    } while (0)

#define U8_TO_STREAM(p, n)     \
    do {                       \
        *(p)++ = (uint8_t)(n); \
    } while (0)


#define U16_TO_STREAM(p, n)       \
    do {                          \
        *((uint16_t *)(p)) = (n); \
        p += 2;                   \
    } while (0)

#define U24_TO_STREAM(p, n)         \
    do {                            \
        U16_TO_STREAM(p, n);        \
        U8_TO_STREAM(p, (n) >> 16); \
    } while (0)

#define U32_TO_STREAM(p, n)       \
    do {                          \
        *((uint32_t *)(p)) = (n); \
        p += 4;                   \
    } while (0)

#define U40_TO_STREAM(p, n)         \
    do {                            \
        U32_TO_STREAM(p, n);        \
        U8_TO_STREAM(p, (n) >> 32); \
    } while (0)

#define U64_TO_STREAM(p, n)       \
    do {                          \
        *((uint64_t *)(p)) = (n); \
        p += 8;                   \
    } while (0)

#define STR_TO_STREAM(p, n, l) \
    {                          \
        memcpy(p, n, l);       \
        p += l;                \
    }

/**
 * @brief      Convert hexadecimal data to string representation
 * @param[in]  buf - pointer to the buffer containing hexadecimal data
 * @param[in]  len - length of the data in bytes
 * @return     Pointer to a static string containing the hexadecimal representation
 * @note       The returned string is statically allocated and will be overwritten on subsequent calls
 */
const char *hex_to_str(const void *buf, u8 len);

/**
 * @brief      Convert a Bluetooth address to string representation
 * @param[in]  addr - pointer to the 6-byte Bluetooth address
 * @return     Pointer to a static string containing the formatted Bluetooth address
 * @note       The returned string is statically allocated and will be overwritten on subsequent calls
 */
const char *addr_to_str(const u8 *addr);

#define _XXXX1                               _YYYY,

#define Z_IS_ENABLED3(ignore_this, val, ...) val

#define Z_IS_ENABLED2(one_or_two_args)       Z_IS_ENABLED3(one_or_two_args 1, 0)

#define Z_IS_ENABLED1(config_macro)          Z_IS_ENABLED2(_XXXX##config_macro)

#define IS_ENABLED(config_macro)             Z_IS_ENABLED1(config_macro)

#endif /* COMMON_UTILITY_H_ */
