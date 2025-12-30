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

// #define abs(a)   (((a)>0)?((a)):(-(a)))

#define cat2(i, j)    i##j
#define cat3(i, j, k) i##j##k

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef min2
#define min2(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef min3
#define min3(a, b, c) min2(min2(a, b), c)
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

#define OFFSETOF(s, m)      ((unsigned int)&((s *)0)->m)
#define ROUND_INT(x, r)     (((x) + (r) - 1) / (r) * (r))
#define ROUND_TO_POW2(x, r) (((x) + (r) - 1) & ~((r) - 1))

//  direct memory access
#define U8_GET(addr)          (*(volatile unsigned char *)(addr))
#define U16_GET(addr)         (*(volatile unsigned short *)(addr))
#define U32_GET(addr)         (*(volatile unsigned int *)(addr))

#define U8_SET(addr, v)       (*(volatile unsigned char *)(addr) = (unsigned char)(v))
#define U16_SET(addr, v)      (*(volatile unsigned short *)(addr) = (unsigned short)(v))
#define U32_SET(addr, v)      (*(volatile unsigned int *)(addr) = (v))

#define U8_INC(addr)          U8_GET(addr) += 1
#define U16_INC(addr)         U16_GET(addr) += 1
#define U32_INC(addr)         U32_GET(addr) += 1

#define U8_DEC(addr)          U8_GET(addr) -= 1
#define U16_DEC(addr)         U16_GET(addr) -= 1
#define U32_DEC(addr)         U32_GET(addr) -= 1

#define U8_CPY(addr1, addr2)  U8_SET(addr1, U8_GET(addr2))
#define U16_CPY(addr1, addr2) U16_SET(addr1, U16_GET(addr2))
#define U32_CPY(addr1, addr2) U32_SET(addr1, U32_GET(addr2))

#define MAKE_U16(h, l)        ((unsigned short)(((h) << 8) | (l)))
#define MAKE_U24(a, b, c)     ((unsigned int)(((a) << 16) | ((b) << 8) | (c)))
#define MAKE_U32(a, b, c, d)  ((unsigned int)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d)))

#define BOUND(x, l, m)        ((x) < (l) ? (l) : ((x) > (m) ? (m) : (x)))
#define SET_BOUND(x, l, m)    ((x) = BOUND(x, l, m))
#define BOUND_INC(x, m)            \
    do {                           \
        ++(x);                     \
        (x) = (x) < (m) ? (x) : 0; \
    } while (0)
#define BOUND_INC_POW2(x, m)       \
    do {                           \
        STATIC_ASSERT_POW2(m);     \
        (x) = ((x) + 1) & (m - 1); \
    } while (0)

#define IS_POWER_OF_2(x)  (!(x & (x - 1)))
#define IS_4BYTE_ALIGN(x) (!(x & 3))

#ifndef IS_LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN (*(unsigned short *)"\0\xff" > 0x100)
#endif

#define IMPLIES(x, y) (!(x) || (y))

// x > y ? 1 : (x ==y : 0 ? -1)
#define COMPARE(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(x)       COMPARE(x, 0)

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

#define foreach(i, n)          for (int i = 0; i < (n); ++i)
#define foreach_range(i, s, e) for (int i = (s); i < (e); ++i)
#define foreach_arr(i, arr)    for (int i = 0; i < (int)ARRAY_SIZE(arr); ++i)
#define foreach_hint(i, n, h)  for (int i = 0, ++h, h = h < n ? h : 0; i < n; ++h, h = h < n ? h : 0)

#define ARRAY_SIZE(a)          (sizeof(a) / sizeof(*a))

#define everyN(i, n)           \
    ++(i);                     \
    (i) = ((i) < N ? (i) : 0); \
    if (0 == (i))

#define U16_HI(a)    (((a) >> 8) & 0xFF)
#define U16_LO(a)    ((a) & 0xFF)

#define U32_BYTE0(a) ((a) & 0xFF)
#define U32_BYTE1(a) (((a) >> 8) & 0xFF)
#define U32_BYTE2(a) (((a) >> 16) & 0xFF)
#define U32_BYTE3(a) (((a) >> 24) & 0xFF)


#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE 1
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
 * @brief      Copy and swap 3-byte data (e.g., 24-bit values)
 * @param[out] dst - pointer to the destination 3-byte array
 * @param[in]  src - pointer to the source 3-byte array
 * @return     none
 */
void swap24(u8 dst[3], const u8 src[3]);

/**
 * @brief      Copy and swap 4-byte data (e.g., 32-bit values)
 * @param[out] dst - pointer to the destination 4-byte array
 * @param[in]  src - pointer to the source 4-byte array
 * @return     none
 */
void swap32(u8 dst[4], const u8 src[4]);

/**
 * @brief      Copy and swap 6-byte data (e.g., 48-bit values/MAC addresses)
 * @param[out] dst - pointer to the destination 6-byte array
 * @param[in]  src - pointer to the source 6-byte array
 * @return     none
 */
void swap48(u8 dst[6], const u8 src[6]);

/**
 * @brief      Copy and swap 7-byte data
 * @param[out] dst - pointer to the destination 7-byte array
 * @param[in]  src - pointer to the source 7-byte array
 * @return     none
 */
void swap56(u8 dst[7], const u8 src[7]);

/**
 * @brief      Copy and swap 8-byte data (e.g., 64-bit values)
 * @param[out] dst - pointer to the destination 8-byte array
 * @param[in]  src - pointer to the source 8-byte array
 * @return     none
 */
void swap64(u8 dst[8], const u8 src[8]);

/**
 * @brief      Copy and swap 16-byte data (e.g., 128-bit values)
 * @param[out] dst - pointer to the destination 16-byte array
 * @param[in]  src - pointer to the source 16-byte array
 * @return     none
 */
void swap128(u8 dst[16], const u8 src[16]);

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

#define BYTE_TO_UINT16(n, p)                    \
    {                                           \
        n = ((u16)(p)[0] + ((u16)(p)[1] << 8)); \
    }
#define BYTE_TO_UINT24(n, p)                                          \
    {                                                                 \
        n = ((u32)(p)[0] + ((u32)(p)[1] << 8) + ((u32)(p)[2] << 16)); \
    }
#define BYTE_TO_UINT32(n, p)                                                                \
    {                                                                                       \
        n = ((u32)(p)[0] + ((u32)(p)[1] << 8) + ((u32)(p)[2] << 16) + ((u32)(p)[3] << 24)); \
    }

#define STREAM_TO_U8(n, p) \
    {                      \
        n = *(p);          \
        p++;               \
    }
#define STREAM_TO_U16(n, p)   \
    {                         \
        BYTE_TO_UINT16(n, p); \
        p += 2;               \
    }
#define STREAM_TO_U24(n, p)   \
    {                         \
        BYTE_TO_UINT24(n, p); \
        p += 3;               \
    }
#define STREAM_TO_U32(n, p)   \
    {                         \
        BYTE_TO_UINT32(n, p); \
        p += 4;               \
    }
#define STREAM_TO_STR(n, p, l) \
    {                          \
        memcpy(n, p, l);       \
        p += l;                \
    }

#define U8_TO_STREAM(p, n) \
    {                      \
        *(p)++ = (u8)(n);  \
    }
#define U16_TO_STREAM(p, n)      \
    {                            \
        *(p)++ = (u8)(n);        \
        *(p)++ = (u8)((n) >> 8); \
    }
#define U24_TO_STREAM(p, n)       \
    {                             \
        *(p)++ = (u8)(n);         \
        *(p)++ = (u8)((n) >> 8);  \
        *(p)++ = (u8)((n) >> 16); \
    }
#define U32_TO_STREAM(p, n)       \
    {                             \
        *(p)++ = (u8)(n);         \
        *(p)++ = (u8)((n) >> 8);  \
        *(p)++ = (u8)((n) >> 16); \
        *(p)++ = (u8)((n) >> 24); \
    }
#define U40_TO_STREAM(p, n)       \
    {                             \
        *(p)++ = (u8)(n);         \
        *(p)++ = (u8)((n) >> 8);  \
        *(p)++ = (u8)((n) >> 16); \
        *(p)++ = (u8)((n) >> 24); \
        *(p)++ = (u8)((n) >> 32); \
    }

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

/**
 * @brief      Convert a 16-bit value to little-endian byte stream.
 * @param[in]  val - 16-bit value to convert.
 * @param[out] dst - Pointer to 2-byte destination array.
 * @return     none.
 */
static inline void u16_to_bstream_le(u16 val, u8 dst[2])
{
    dst[0] = val;
    dst[1] = val >> 8;
}

/**
 * @brief      Convert a 24-bit value to little-endian byte stream.
 * @param[in]  val - 24-bit value to convert.
 * @param[out] dst - Pointer to 3-byte destination array.
 * @return     none.
 */
static inline void u24_to_bstream_le(u32 val, u8 dst[3])
{
    u16_to_bstream_le(val, dst);
    dst[2] = val >> 16;
}

/**
 * @brief      Convert a 32-bit value to little-endian byte stream.
 * @param[in]  val - 32-bit value to convert.
 * @param[out] dst - Pointer to 4-byte destination array.
 * @return     none.
 */
static inline void u32_to_bstream_le(u32 val, u8 dst[4])
{
    u16_to_bstream_le(val, dst);
    u16_to_bstream_le(val >> 16, &dst[2]);
}

/**
 * @brief      Convert a 48-bit value to little-endian byte stream.
 * @param[in]  val - 48-bit value to convert.
 * @param[out] dst - Pointer to 6-byte destination array.
 * @return     none.
 */
static inline void u48_to_bstream_le(u64 val, u8 dst[6])
{
    u32_to_bstream_le(val, dst);
    u16_to_bstream_le(val >> 32, &dst[4]);
}

/**
 * @brief      Convert a 64-bit value to little-endian byte stream.
 * @param[in]  val - 64-bit value to convert.
 * @param[out] dst - Pointer to 8-byte destination array.
 * @return     none.
 */
static inline void u64_to_bstream_le(u64 val, u8 dst[8])
{
    u32_to_bstream_le(val, dst);
    u32_to_bstream_le(val >> 32, &dst[4]);
}

/**
 * @brief      Convert a little-endian byte stream to a 16-bit value.
 * @param[in]  src - Pointer to 2-byte source array.
 * @return     16-bit value.
 */
static inline u16 bstream_to_u16_le(const u8 src[2])
{
    return ((u16)src[1] << 8) | src[0];
}

/**
 * @brief      Convert a little-endian byte stream to a 24-bit value.
 * @param[in]  src - Pointer to 3-byte source array.
 * @return     24-bit value.
 */
static inline u32 bstream_to_u24_le(const u8 src[3])
{
    return ((u32)src[2] << 16) | bstream_to_u16_le(&src[0]);
}

/**
 * @brief      Convert a little-endian byte stream to a 32-bit value.
 * @param[in]  src - Pointer to 4-byte source array.
 * @return     32-bit value.
 */
static inline u32 bstream_to_u32_le(const u8 src[4])
{
    return ((u32)bstream_to_u16_le(&src[2]) << 16) | bstream_to_u16_le(&src[0]);
}

/**
 * @brief      Convert a little-endian byte stream to a 48-bit value.
 * @param[in]  src - Pointer to 6-byte source array.
 * @return     48-bit value.
 */
static inline u64 bstream_to_u48_le(const u8 src[6])
{
    return ((u64)bstream_to_u32_le(&src[2]) << 16) | bstream_to_u16_le(&src[0]);
}

/**
 * @brief      Convert a little-endian byte stream to a 64-bit value.
 * @param[in]  src - Pointer to 8-byte source array.
 * @return     64-bit value.
 */
static inline u64 bstream_to_u64_le(const u8 src[8])
{
    return ((u64)bstream_to_u32_le(&src[4]) << 32) | bstream_to_u32_le(&src[0]);
}

#define _XXXX1                               _YYYY,

#define Z_IS_ENABLED3(ignore_this, val, ...) val

#define Z_IS_ENABLED2(one_or_two_args)       Z_IS_ENABLED3(one_or_two_args 1, 0)

#define Z_IS_ENABLED1(config_macro)          Z_IS_ENABLED2(_XXXX##config_macro)

#define IS_ENABLED(config_macro)             Z_IS_ENABLED1(config_macro)

#endif /* COMMON_UTILITY_H_ */
