/********************************************************************************************************
 * @file    fifo.c
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
#include "common/fifo.h"
#include "common/tstring.h"
#if 1

/**
 * @brief      Initialize a FIFO buffer
 * @param[in]  f - pointer to the FIFO structure to initialize
 * @param[in]  s - size of each element in the FIFO
 * @param[in]  n - number of elements in the FIFO
 * @param[in]  p - pointer to the data buffer
 * @return     none
 */
void tlk_fifo_init(tlk_fifo_t *f, int s, u8 n, u8 *p)
{
    f->size = s;
    f->num  = n;
    f->wptr = 0;
    f->rptr = 0;
    f->p    = p;
}

/**
 * @brief      Get the current write pointer of the FIFO
 * @param[in]  f - pointer to the FIFO structure
 * @return     Pointer to the current write position, or NULL if FIFO is full
 */
_attribute_retention_code_ u8 *tlk_fifo_wptr(tlk_fifo_t *f)
{
    if (((f->wptr - f->rptr) & 255) < f->num) {
        return f->p + (f->wptr & (f->num - 1)) * f->size;
    }
    return 0;
}

/**
 * @brief      Move the FIFO write pointer to the next position
 * @param[in]  f - pointer to the FIFO structure
 * @return     none
 */
_attribute_retention_code_ void tlk_fifo_next(tlk_fifo_t *f)
{
    f->wptr++;
}

/**
 * @brief      Push data into the FIFO buffer
 * @param[in]  f - pointer to the FIFO structure
 * @param[in]  p - pointer to the data to push
 * @param[in]  n - length of the data in bytes
 * @return     0 on success, -1 on failure (FIFO full or data too large)
 */
int tlk_fifo_push(tlk_fifo_t *f, u8 *p, u32 n)
{
    if (((f->wptr - f->rptr) & 255) >= f->num) {
        return -1;
    }

    if (n >= f->size) {
        return -1;
    }
    u8 *pd = f->p + (f->wptr++ & (f->num - 1)) * f->size;
    *pd++  = n & 0xff;
    *pd++  = (n >> 8) & 0xff;
    tmemcpy((void *)pd, (void *)p, (unsigned int)n);
    return 0;
}

#endif
