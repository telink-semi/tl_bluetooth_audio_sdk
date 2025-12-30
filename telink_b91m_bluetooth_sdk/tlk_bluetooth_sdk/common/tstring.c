/********************************************************************************************************
 * @file    tstring.c
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
#pragma GCC optimize "no-tree-loop-distribute-patterns"
#include "tstring.h"

/**
 * @brief      Fill memory with a constant byte value
 * @param[in]  dest - pointer to the destination memory area
 * @param[in]  val  - value to fill with
 * @param[in]  len  - number of bytes to fill
 * @return     Pointer to the destination memory area
 */
_attribute_ram_code_sec_ void *memset(void *dest, int val, unsigned int len)
{
    unsigned char *p    = (unsigned char *)dest;
    unsigned char  _val = (unsigned char)val;

    if (len < 16) {
        while (len--) {
            *p++ = _val;
        }
        return dest;
    }

    while (((uintptr_t)p & 3) && len) {
        *p++ = _val;
        len--;
    }

    union
    {
        uint32_t      word;
        unsigned char bytes[4];
    } converter;

    converter.bytes[0] = _val;
    converter.bytes[1] = _val;
    converter.bytes[2] = _val;
    converter.bytes[3] = _val;

    uint32_t  word_val = converter.word;
    uint32_t *wp       = (uint32_t *)p;

    unsigned int word_cnt = len >> 2;
    unsigned int chunks   = word_cnt >> 2;

    while (chunks--) {
        *wp++ = word_val;
        *wp++ = word_val;
        *wp++ = word_val;
        *wp++ = word_val; //reduce loop cnt
    }

    word_cnt &= 3;
    while (word_cnt--) {
        *wp++ = word_val;
    }

    p = (unsigned char *)wp;
    len &= 3;
    while (len--) {
        *p++ = _val;
    }

    return dest;
}

/**
 * @brief      Compare two memory areas
 * @param[in]  m1  - pointer to the first memory area
 * @param[in]  m2  - pointer to the second memory area
 * @param[in]  len - number of bytes to compare
 * @return     0 if equal, otherwise the difference between the first differing bytes
 */
_attribute_ram_code_sec_ int tmemcmp(const void *m1, const void *m2, u32 len)
{
    uint8_t *st1 = (uint8_t *)m1;
    uint8_t *st2 = (uint8_t *)m2;

    while (len--) {
        if (*st1 != *st2) {
            return (*st1 - *st2);
        }
        st1++;
        st2++;
    }
    return 0;
}

/**
 * @brief      Compare two memory areas with 4-byte alignment
 * @param[in]  m1  - pointer to the first memory area
 * @param[in]  m2  - pointer to the second memory area
 * @param[in]  len - number of bytes to compare
 * @return     0 if equal, 1 if different
 */
_attribute_ram_code_sec_ int tmemcmp4(void *m1, void *m2, register unsigned int len)
{
    unsigned int *st1      = (unsigned int *)m1;
    unsigned int *st2      = (unsigned int *)m2;
    unsigned int  word_len = len >> 2;
    while (word_len--) {
        if (*st1 != *st2) {
            return 1; // return (*st1 - *st2)
        }
        st1++;
        st2++;
    }
    return 0;
}

/**
 * @brief      Copy memory from source to destination
 * @param[out] dest   - pointer to the destination memory area
 * @param[in]  src    - pointer to the source memory area
 * @param[in]  length - number of bytes to copy
 * @return     none
 */
_attribute_ram_code_sec_ void tmemcpy(void *dest, const void *src, unsigned int length)
{
    unsigned char *d = (unsigned char *)dest;
    unsigned char *s = (unsigned char *)src;

    unsigned int d_align_offset = (unsigned int)d % 4;
    unsigned int s_align_offset = (unsigned int)s % 4;

    if (d_align_offset == s_align_offset) {
        unsigned int prefix_len  = (4 - s_align_offset) % 4;
        unsigned int copy_prefix = (length > prefix_len) ? prefix_len : length;
        for (unsigned int i = 0; i < copy_prefix; ++i) {
            d[i] = s[i];
        }
        d += copy_prefix;
        s += copy_prefix;
        length -= copy_prefix;

        unsigned int words_num = length / 4;
        if (words_num > 0) {
            unsigned int *d_32 = (unsigned int *)d;
            unsigned int *s_32 = (unsigned int *)s;
            for (unsigned int i = 0; i < words_num; ++i) {
                d_32[i] = s_32[i];
            }
            d += words_num * 4;
            s += words_num * 4;
            length -= words_num * 4;
        }
    }

    while (length--) {
        *d++ = *s++;
    }
}

/**
 * @brief      Copy memory with 4-byte alignment assumption
 * @param[out] d      - pointer to the destination memory area
 * @param[in]  s      - pointer to the source memory area
 * @param[in]  length - number of bytes to copy
 * @return     none
 * @note       For performance, assumes length % 4 == 0, and no memory overlapped
 */
_attribute_ram_code_sec_ void tmemcpy4(void *d, void *s, unsigned int length)
{
    //	if((((int)d)%4!=0) || (((int)s)%4!=0))
    //	{
    //        __asm__ volatile("ecall");
    //        return;
    //	}
    unsigned char *source      = s;
    unsigned char *dest        = d;
    unsigned int   int_length  = length / 4;
    unsigned int   char_length = length % 4;
    if (int_length != 0) {
        int *s_4 = (int *)source;
        int *d_4 = (int *)dest;
        for (unsigned int i = 0; i < int_length; i++) {
            d_4[i] = s_4[i];
        }
    }
    if (char_length != 0) {
        source += (4 * int_length);
        dest += (4 * int_length);
        while (char_length--) {
            *dest++ = *source++;
        }
    }
    return;
}