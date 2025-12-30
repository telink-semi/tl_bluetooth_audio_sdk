/********************************************************************************************************
 * @file    fifo.h
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
#ifndef FIFO_H_
#define FIFO_H_

#include "types.h"
#include "compiler.h"

/**
 * @brief      FIFO structure definition
 */
typedef struct
{
    u32 size;
    u8  num;
    u8  mask;
    u8  wptr;
    u8  rptr;
    u8 *p;
} __attribute__((aligned(4))) tlk_fifo_t;

/**
 * @brief      Multi-connection FIFO structure definition
 */
typedef struct
{
    u16 fifo_size;
    u8  fifo_num;
    u8  conn_num;
    u8 *p;
} multi_conn_fifo_t;

/**
 * @brief      Macro to initialize a FIFO buffer
 * @param[in]  name - name of the FIFO
 * @param[in]  size - size of each element in the FIFO
 * @param[in]  n    - number of elements in the FIFO
 */
#define FIFO_INIT(name, size, n) /*__attribute__ ((aligned (4)))*/      \
    u8         name##_b[size * n] __attribute__((aligned(4))) /*={0}*/; \
    tlk_fifo_t name = {size, n, 0, 0, name##_b}

/**
 * @brief      Macro to initialize a FIFO buffer in IRAM
 * @param[in]  name - name of the FIFO
 * @param[in]  size - size of each element in the FIFO
 * @param[in]  n    - number of elements in the FIFO
 */
#define FIFO_INIT_IRAM(name, size, n) /*__attribute__ ((aligned (4)))*/                            \
    _attribute_iram_data_ u8              name##_b[size * n] __attribute__((aligned(4))) /*={0}*/; \
    _attribute_data_retention_ tlk_fifo_t name = {size, n, 0, 0, name##_b}

/**
 * @brief      Macro to initialize a multi-connection FIFO buffer
 * @param[in]  name      - name of the FIFO
 * @param[in]  fifo_size - size of each element in the FIFO
 * @param[in]  fifo_num  - number of elements in the FIFO
 * @param[in]  conn_num  - number of connections
 */
#define MULTI_CONN_FIFO_INIT(name, fifo_size, fifo_num, conn_num)      \
    u8                name##_b[fifo_size * fifo_num * conn_num] = {0}; \
    multi_conn_fifo_t name                                      = {fifo_size, fifo_num, conn_num, name##_b}

/**
 * @brief      Initialize a FIFO buffer
 * @param[in]  f - pointer to the FIFO structure to initialize
 * @param[in]  s - size of each element in the FIFO
 * @param[in]  n - number of elements in the FIFO
 * @param[in]  p - pointer to the data buffer
 * @return     none
 */
void tlk_fifo_init(tlk_fifo_t *f, int s, u8 n, u8 *p);

/**
 * @brief      Get the current write pointer of the FIFO
 * @param[in]  f - pointer to the FIFO structure
 * @return     Pointer to the current write position, or NULL if FIFO is full
 */
u8 *tlk_fifo_wptr(tlk_fifo_t *f);

/**
 * @brief      Move the FIFO write pointer to the next position
 * @param[in]  f - pointer to the FIFO structure
 * @return     none
 */
void tlk_fifo_next(tlk_fifo_t *f);

/**
 * @brief      Push data into the FIFO buffer
 * @param[in]  f - pointer to the FIFO structure
 * @param[in]  p - pointer to the data to push
 * @param[in]  n - length of the data in bytes
 * @return     0 on success, -1 on failure (FIFO full or data too large)
 */
int tlk_fifo_push(tlk_fifo_t *f, u8 *p, u32 n);

/**
 * @brief      Pop data from the FIFO buffer
 * @param[in]  f - pointer to the FIFO structure
 * @return     none
 */
__INLINE void tlk_fifo_pop(tlk_fifo_t *f)
{
    if (f->rptr != f->wptr) {
        f->rptr++;
    }
}

/**
 * @brief      Reset the FIFO buffer
 * @param[in]  f - pointer to the FIFO structure
 * @return     none
 */
__INLINE void tlk_fifo_reset(tlk_fifo_t *f)
{
    f->rptr = f->wptr;
}

/**
 * @brief      Get the current read pointer of the FIFO
 * @param[in]  f - pointer to the FIFO structure
 * @return     Pointer to the current read position, or NULL if FIFO is empty
 */
__INLINE u8 *tlk_fifo_get(tlk_fifo_t *f)
{
    if (f->rptr != f->wptr) {
        u8 *p = f->p + (f->rptr & (f->num - 1)) * f->size;
        return p;
    }
    return 0;
}

/**
 * @brief      Check if the FIFO buffer is full
 * @param[in]  f - pointer to the FIFO structure
 * @return     true if FIFO is full, false otherwise
 */
__INLINE bool tlk_fifo_is_full(tlk_fifo_t *f)
{
    if (f == NULL) {
        return 1;
    }
    u8 n = f->wptr - f->rptr;
    return n >= f->num;
}

/**
 * @brief      Check if the FIFO buffer is half full
 * @param[in]  f - pointer to the FIFO structure
 * @return     true if FIFO is half full, false otherwise
 */
__INLINE bool tlk_fifo_half_full(tlk_fifo_t *f)
{
    if (f == NULL) {
        return 1;
    }
    uint8_t n = f->wptr - f->rptr;
    return n >= (f->num / 2);
}
#endif /* FIFO_H_ */