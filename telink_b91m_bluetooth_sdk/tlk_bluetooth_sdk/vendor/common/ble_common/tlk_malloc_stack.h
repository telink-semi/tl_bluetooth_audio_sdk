/********************************************************************************************************
 * @file    tlk_malloc_stack.h
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
#pragma once
#include <stddef.h>

/*
 * This structure should be a power of two.  This becomes the
 * alignment unit.
 */
struct free_arena_header;

struct arena_header
{
    unsigned short            type;
    unsigned short            size;
    struct free_arena_header *next, *prev;
};

#define ARENA_SIZE_MASK (~(sizeof(struct arena_header) - 1))

/*
 * This structure should be no more than twice the size of the
 * previous structure.
 */
struct free_arena_header
{
    struct arena_header       a;
    struct free_arena_header *next_free, *prev_free;
};

struct mem_arena_header
{
    struct free_arena_header a;
    unsigned char           *sbrkBase;
    unsigned char           *sbrkLimit;
    unsigned char           *brk;
};

/**
 * @brief Initialize retention memory arena.
 *
 * @param[in] base  Retention memory base address.
 * @param[in] size  Retention memory size.
 */
void tlk_initialRetentionBuffer(void *base, size_t size);

/**
 * @brief Initialize non-retention memory arena.
 *
 * @param[in] base  Non-retention memory base address.
 * @param[in] size  Non-retention memory size.
 */
void tlk_initialNonRetentionBuffer(void *base, size_t size);

/**
 * @brief Allocate memory from retention arena.
 *
 * @param[in] size      Allocation size.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_reten(size_t size, unsigned short usedType);

/**
 * @brief Free memory allocated from retention arena.
 *
 * @param[in] ptr  Pointer previously returned by malloc_reten/realloc_reten.
 */
void free_reten(void *ptr);

/**
 * @brief Reallocate memory in retention arena.
 *
 * @param[in] ptr       Pointer to existing block (or NULL to allocate new).
 * @param[in] size      New size.
 * @param[in] usedType  Allocation tag/type for new block.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_reten(void *ptr, size_t size, unsigned short usedType);

/**
 * @brief Allocate memory from non-retention arena with explicit type.
 *
 * @param[in] size      Allocation size.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_nonreten1(size_t size, unsigned short usedType);

/**
 * @brief Reallocate memory in non-retention arena with explicit type.
 *
 * @param[in] ptr       Pointer to existing block (or NULL to allocate new).
 * @param[in] size      New size.
 * @param[in] usedType  Allocation tag/type for new block.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_nonreten1(void *ptr, size_t size, unsigned short usedType);

/**
 * @brief Allocate memory from default non-retention arena (test type).
 *
 * @param[in] size  Allocation size.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_nonreten(size_t size);

/**
 * @brief Free memory allocated from default non-retention arena.
 *
 * @param[in] ptr  Pointer previously returned by malloc_nonreten/realloc_nonreten.
 */
void free_nonreten(void *ptr);

/**
 * @brief Reallocate memory in default non-retention arena.
 *
 * @param[in] ptr   Pointer to existing block (or NULL to allocate new).
 * @param[in] size  New size.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_nonreten(void *ptr, size_t size);

/**
 * @brief Initialize a custom memory arena.
 *
 * @param[in] base  Arena base address.
 * @param[in] size  Arena size.
 */
void tlk_malloc_init(void *base, size_t size);

/**
 * @brief Deinitialize a custom memory arena.
 *
 * @param[in] base  Arena base address (reserved for future use).
 */
void tlk_malloc_deinit(void *base);

/**
 * @brief Allocate memory from a custom arena.
 *
 * @param[in] base      Arena base address (mem_arena_header).
 * @param[in] size      Allocation size.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *tlk_malloc_buffer(void *base, size_t size, unsigned short usedType);

/**
 * @brief Free memory allocated from a custom arena.
 *
 * @param[in] base  Arena base address (mem_arena_header).
 * @param[in] ptr   Pointer to block allocated by tlk_malloc_buffer.
 */
void tlk_free_buffer(void *base, void *ptr);

enum
{
    ARENA_TYPE_FREE = 0x0001,
    ARENA_TYPE_HEAD,
    ARENA_TYPE_TAIL = 0x0010,
    ARENA_TYPE_TEST = 0xF000,
};
