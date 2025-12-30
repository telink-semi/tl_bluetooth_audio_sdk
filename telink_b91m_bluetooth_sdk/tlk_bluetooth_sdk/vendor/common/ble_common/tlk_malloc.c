/********************************************************************************************************
 * @file    tlk_malloc.c
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
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "tlk_malloc_stack.h"
#include "common/types.h"
#include "tlklib/dbg/tlkdbg.h"

/**
 * @brief  Adjust heap break point.
 *          Allocate or deallocate memory from the heap by moving the break point.
 *
 * @param[in] memHead  Memory arena header.
 * @param[in] incr     Increment size. Positive for allocation, negative for deallocation.
 *
 * @return  Previous break point on success, (void *)-1 on failure.
 */
static void *_sbrk(struct mem_arena_header *memHead, int incr)
{
    unsigned char *prev_brk;
    unsigned char *sbrkBase  = memHead->sbrkBase;
    unsigned char *sbrkLimit = memHead->sbrkLimit;

    if (incr < 0) {
        /* Returning memory to the heap. */
        incr = -incr;
        if (memHead->brk - incr < sbrkBase) {
            prev_brk = (unsigned char *)-1;
        } else {
            prev_brk = memHead->brk;
            memHead->brk -= incr;
        }
    } else {
        /* Allocating memory from the heap. */
        if (sbrkLimit - memHead->brk >= incr) {
            prev_brk = memHead->brk;
            memHead->brk += incr;
        } else {
            prev_brk = (unsigned char *)-1;
        }
    }

    return prev_brk;
}

/**
 * @brief  Mark block as dead (for debug purposes).
 *
 * @param[in] ah  Arena header to mark as dead.
 */
static inline void mark_block_dead(struct free_arena_header *ah)
{
#ifdef DEBUG_MALLOC
    ah->a.type = ARENA_TYPE_DEAD;
#else
    (void)ah;
#endif
}

/**
 * @brief  Remove block from main chain.
 *          Unlink the block from the doubly-linked main chain.
 *
 * @param[in] ah  Arena header to remove from main chain.
 */
static inline void remove_from_main_chain(struct free_arena_header *ah)
{
    struct free_arena_header *ap, *an;

    mark_block_dead(ah);

    ap         = ah->a.prev;
    an         = ah->a.next;
    ap->a.next = an;
    an->a.prev = ap;
}

/**
 * @brief  Remove block from free chain.
 *          Unlink the block from the doubly-linked free chain.
 *
 * @param[in] ah  Arena header to remove from free chain.
 */
static inline void remove_from_free_chain(struct free_arena_header *ah)
{
    struct free_arena_header *ap, *an;

    ap            = ah->prev_free;
    an            = ah->next_free;
    ap->next_free = an;
    an->prev_free = ap;
}

/**
 * @brief  Remove block from both free chain and main chain.
 *
 * @param[in] ah  Arena header to remove from chains.
 */
static inline void remove_from_chains(struct free_arena_header *ah)
{
    remove_from_free_chain(ah);
    remove_from_main_chain(ah);
}

/**
 * @brief  Allocate memory from a free block.
 *          Split the block if it's larger than required, otherwise allocate the whole block.
 *
 * @param[in] fp       Free block header.
 * @param[in] size     Required size.
 * @param[in] usedType Type to mark the allocated block.
 *
 * @return  Pointer to allocated memory (after arena header).
 */
static void *__malloc_from_block(struct free_arena_header *fp, size_t size, unsigned short usedType)
{
    size_t                    fsize;
    struct free_arena_header *nfp, *na, *fpn, *fpp;

    fsize = fp->a.size;

    /* We need the 2* to account for the larger requirements of a
       free block */
    if (fsize >= size + 2 * sizeof(struct arena_header)) {
        /* Bigger block than required -- split block */
        nfp = (struct free_arena_header *)((char *)fp + size);
        na  = fp->a.next;

        nfp->a.type = ARENA_TYPE_FREE;
        nfp->a.size = fsize - size;
        fp->a.type  = usedType;
        fp->a.size  = size;

        /* Insert into all-block chain */
        nfp->a.prev = fp;
        nfp->a.next = na;
        na->a.prev  = nfp;
        fp->a.next  = nfp;

        /* Replace current block on free chain */
        nfp->next_free = fpn = fp->next_free;
        nfp->prev_free = fpp = fp->prev_free;
        fpn->prev_free       = nfp;
        fpp->next_free       = nfp;
    } else {
        fp->a.type = usedType; /* Allocate the whole block */
        remove_from_free_chain(fp);
    }

    return (void *)(&fp->a + 1);
}

/**
 * @brief  Free a block and coalesce with adjacent free blocks.
 *          Merge with previous and/or next free blocks if they are adjacent.
 *
 * @param[in] head  Head of the arena chain.
 * @param[in] ah    Arena header to free.
 *
 * @return  Pointer to the coalesced free block.
 */
static struct free_arena_header *__free_block(struct free_arena_header *head, struct free_arena_header *ah)
{
    struct free_arena_header *pah, *nah;

    pah = ah->a.prev;
    nah = ah->a.next;
    if (pah->a.type == ARENA_TYPE_FREE && (char *)pah + pah->a.size == (char *)ah) {
        /* Coalesce into the previous block */
        pah->a.size += ah->a.size;
        pah->a.next = nah;
        nah->a.prev = pah;
        mark_block_dead(ah);

        ah  = pah;
        pah = ah->a.prev;
    } else {
        /* Need to add this block to the free chain */
        ah->a.type               = ARENA_TYPE_FREE;
        ah->next_free            = head->next_free;
        ah->prev_free            = head;
        head->next_free          = ah;
        ah->next_free->prev_free = ah;
    }

    /* In either of the previous cases, we might be able to merge
       with the subsequent block... */
    if (nah->a.type == ARENA_TYPE_FREE && (char *)ah + ah->a.size == (char *)nah) {
        ah->a.size += nah->a.size;

        /* Remove the old block from the chains */
        remove_from_chains(nah);
    }

    /* Return the block that contains the called block */
    return ah;
}

/**
 * @brief  Add a new memory block to the malloc pool.
 *          Insert the block into the management chains and coalesce with adjacent blocks.
 *
 * @param[in] head  Head of the arena chain.
 * @param[in] buf   Buffer to add to the pool.
 * @param[in] size  Size of the buffer.
 */
static void add_malloc_block(struct free_arena_header *head, void *buf, size_t size)
{
    struct free_arena_header *fp = buf;
    struct free_arena_header *pah;

    if (size < sizeof(struct free_arena_header)) {
        return; // Too small.
    }

    /* Insert the block into the management chains.  We need to set
       up the size and the main block list pointer, the rest of
       the work is logically identical to free(). */
    fp->a.type = ARENA_TYPE_FREE;
    fp->a.size = size;

    /* We need to insert this into the main block list in the proper
       place -- this list is required to be sorted.  Since we most likely
       get memory assignments in ascending order, search backwards for
       the proper place. */
    for (pah = head->a.prev; pah->a.type != ARENA_TYPE_HEAD; pah = pah->a.prev) {
        if (pah < fp) {
            break;
        }
    }

    /* Now pah points to the node that should be the predecessor of
       the new node */
    fp->a.next         = pah->a.next;
    fp->a.prev         = pah;
    pah->a.next        = fp;
    fp->a.next->a.prev = fp;

    /* Insert into the free chain and coalesce with adjacent blocks */
    fp = __free_block(head, fp);
}

/**
 * @brief  Allocate memory from the arena.
 *          Search free blocks for a fit, or extend the heap if no fit is found.
 *
 * @param[in] memHead  Memory arena header.
 * @param[in] size     Size to allocate.
 * @param[in] usedType Type to mark the allocated block.
 *
 * @return  Pointer to allocated memory on success, NULL on failure.
 */
static void *tlk_malloc(struct mem_arena_header *memHead, size_t size, unsigned short usedType)
{
    struct free_arena_header *fp;
    void                     *more_mem;

    if (size == 0 || size > (SIZE_MAX - sizeof(struct arena_header))) {
        //printf("malloc failed 0:%d\n", size);
        return NULL;
    }

    struct free_arena_header *head = &memHead->a;

    /* Add the obligatory arena header, and round up */
    //    size = (size + 2 * sizeof(struct arena_header) - 1) & ARENA_SIZE_MASK;
    size = (size + sizeof(struct arena_header) + 3) & (~3);
    size = size > sizeof(struct free_arena_header) ? size : sizeof(struct free_arena_header); //must need

    void *result     = NULL;
    int   loop_count = 0;
#define MAX_FREE_BLOCKS 10000
retry_alloc:
    for (fp = head->next_free; fp->a.type != ARENA_TYPE_HEAD; fp = fp->next_free) {
        if ((void *)fp < (void *)memHead || (void *)fp > (void *)memHead->sbrkLimit) {
            tlk_printf("-----------------------------------------\n");
            tlk_printf("malloc failed: fp is invalid\n");
            tlk_printf("fp: %x\n", fp);
            tlk_printf("memHead: %x\n", memHead);
            tlk_printf("memHead->sbrkLimit: %x\n", memHead->sbrkLimit);
            return NULL;
        }
        if (++loop_count > MAX_FREE_BLOCKS) {
            tlk_printf("-----------------------------------------\n");
            tlk_printf("malloc failed: loop_count > MAX_FREE_BLOCKs\n");
            tlk_printf("loop_count: %d\n", loop_count);
            tlk_printf("MAX_FREE_BLOCKS: %d\n", MAX_FREE_BLOCKS);
            tlk_printf("size: %d\n", size);
            tlk_printf("usedType: %d\n", usedType);
            tlk_printf("head: %x\n", head);
            tlk_printf("fp: %x\n", fp);
            return NULL;
        }
        if (fp->a.size >= size) {
            /* Found fit -- allocate out of this block */
            result = __malloc_from_block(fp, size, usedType);
            break;
        }
    }

    if (result == NULL) {
        more_mem = _sbrk(memHead, size);
        if (more_mem != (void *)-1) {
            add_malloc_block(head, more_mem, size);
            ////printf("malloc failed: retry_alloc\n");
            goto retry_alloc;
        } else {
            //printf("malloc failed 2:%d\n", size);
        }
    } else {
        //printf("malloc[0x%p]:%d\n", result, (int)size);
    }

    return result;
}

/**
 * @brief  Free allocated memory.
 *          Merge the freed block with adjacent free blocks.
 *
 * @param[in] memHead  Memory arena header.
 * @param[in] ptr      Pointer to memory to free.
 */
static void tlk_free(struct mem_arena_header *memHead, void *ptr)
{
    struct free_arena_header *ah;

    if (!ptr) {
        //printf("free failed 0\n");
        return;
    }

    ah = (struct free_arena_header *)((struct arena_header *)ptr - 1);

    struct free_arena_header *head = &memHead->a;

    //printf("free[0x%p]\n", ptr);

    /* Merge into adjacent free blocks */
    ah = __free_block(head, ah);
}

/**
 * @brief  Reallocate memory.
 *          If the current block is large enough, return it unchanged.
 *          Otherwise, allocate a new block, copy data, and free the old block.
 *
 * @param[in] memHead  Memory arena header.
 * @param[in] ptr      Pointer to memory to reallocate.
 * @param[in] size     New size.
 * @param[in] usedType Type to mark the allocated block.
 *
 * @return  Pointer to reallocated memory on success, NULL on failure.
 */
static void *tlk_realloc(struct mem_arena_header *memHead, void *ptr, size_t size, unsigned short usedType)
{
    struct free_arena_header *ah;
    void                     *newptr;
    size_t                    oldsize;

    if (!ptr) {
        return tlk_malloc(memHead, size, usedType);
    }

    if (size == 0 || size > (SIZE_MAX - sizeof(struct arena_header))) {
        tlk_free(memHead, ptr);
        return NULL;
    }

    /* Add the obligatory arena header, and round up */
    // size = (size + 2 * sizeof(struct arena_header) - 1) & ARENA_SIZE_MASK;
    size = (size + sizeof(struct arena_header) + 3) & (~3);
    size = size > sizeof(struct free_arena_header) ? size : sizeof(struct free_arena_header); //must need

    ah = (struct free_arena_header *)((struct arena_header *)ptr - 1);

    if (ah->a.size >= size && size >= (ah->a.size >> 2)) {
        /* This field is a good size already. */
        return ptr;
    } else {
        /* Make me a new block.  This is kind of bogus; we should
        be checking the following block to see if we can do an
        in-place adjustment... fix that later. */

        oldsize = ah->a.size - sizeof(struct arena_header);

        newptr = tlk_malloc(memHead, size, usedType);

        if (newptr) {
            memcpy(newptr, ptr, (size < oldsize) ? size : oldsize);
        }

        tlk_free(memHead, ptr);

        return newptr;
    }
}


__attribute__((section(".retention_data"))) static struct mem_arena_header __retentionHeader = {
    .a =
        {
            {
                ARENA_TYPE_HEAD,
                0,
                &__retentionHeader.a,
                &__retentionHeader.a,
            },
            &__retentionHeader.a,
            &__retentionHeader.a,
        },
};

__attribute__((section(".retention_data"))) static struct mem_arena_header __nonRetentionHeader = {
    .a =
        {
            {
                ARENA_TYPE_HEAD,
                0,
                &__nonRetentionHeader.a,
                &__nonRetentionHeader.a,
            },
            &__nonRetentionHeader.a,
            &__nonRetentionHeader.a,
        },
};

/**
 * @brief Initialize a custom memory arena.
 *
 * @param[in] base  Arena base address (mem_arena_header).
 * @param[in] size  Arena size in bytes.
 */
void tlk_malloc_init(void *base, size_t size)
{
    struct mem_arena_header *memHead = base;
    memHead->a.a.type                = ARENA_TYPE_HEAD;
    memHead->a.a.size                = 0;
    memHead->a.a.prev = memHead->a.a.next = (struct free_arena_header *)&memHead->a.a;
    memHead->a.next_free                  = &memHead->a;
    memHead->a.prev_free                  = &memHead->a;
    memHead->sbrkBase                     = (unsigned char *)base + sizeof(struct mem_arena_header);
    memHead->sbrkLimit                    = (unsigned char *)base + size - 1;
    memHead->brk                          = (unsigned char *)memHead->sbrkBase;
}

/**
 * @brief Deinitialize a custom memory arena.
 *        Currently a placeholder for symmetry.
 *
 * @param[in] base  Arena base address (reserved for future use).
 */
void tlk_malloc_deinit(void *base)
{
    (void)base;
}

/**
 * @brief Allocate memory from a custom arena.
 *
 * @param[in] base      Arena base address (mem_arena_header).
 * @param[in] size      Allocation size in bytes.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *tlk_malloc_buffer(void *base, size_t size, unsigned short usedType)
{
    return tlk_malloc(base, size, usedType);
}

/**
 * @brief Free memory allocated from a custom arena.
 *
 * @param[in] base  Arena base address (mem_arena_header).
 * @param[in] ptr   Pointer previously returned by tlk_malloc_buffer.
 */
void tlk_free_buffer(void *base, void *ptr)
{
    return tlk_free(base, ptr);
}

/**
 * @brief Initialize retention memory arena header.
 *
 * @param[in] base  Retention buffer base address.
 * @param[in] size  Retention buffer size in bytes.
 */
void tlk_initialRetentionBuffer(void *base, size_t size)
{
    __retentionHeader.sbrkBase  = (unsigned char *)base;
    __retentionHeader.sbrkLimit = (unsigned char *)base + size - 1;
    __retentionHeader.brk       = (unsigned char *)base;
}

/**
 * @brief Initialize non-retention memory arena header.
 *
 * @param[in] base  Non-retention buffer base address.
 * @param[in] size  Non-retention buffer size in bytes.
 */
void tlk_initialNonRetentionBuffer(void *base, size_t size)
{
    __nonRetentionHeader.sbrkBase  = (unsigned char *)base;
    __nonRetentionHeader.sbrkLimit = (unsigned char *)base + size - 1;
    __nonRetentionHeader.brk       = (unsigned char *)base;
}

/**
 * @brief Allocate memory from retention arena.
 *
 * @param[in] size      Allocation size in bytes.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_reten(size_t size, unsigned short usedType)
{
    return tlk_malloc(&__retentionHeader, size, usedType);
}

/**
 * @brief Free memory allocated from retention arena.
 *
 * @param[in] ptr  Pointer previously returned by malloc_reten/realloc_reten.
 */
void free_reten(void *ptr)
{
    return tlk_free(&__retentionHeader, ptr);
}

/**
 * @brief Reallocate memory in retention arena.
 *
 * @param[in] ptr       Pointer to existing block (or NULL to allocate new).
 * @param[in] size      New size in bytes.
 * @param[in] usedType  Allocation tag/type for new block if moved.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_reten(void *ptr, size_t size, unsigned short usedType)
{
    return tlk_realloc(&__retentionHeader, ptr, size, usedType);
}

/**
 * @brief Allocate memory from non-retention arena with explicit type.
 *
 * @param[in] size      Allocation size in bytes.
 * @param[in] usedType  Allocation tag/type.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_nonreten1(size_t size, unsigned short usedType)
{
    return tlk_malloc(&__nonRetentionHeader, size, usedType);
}

/**
 * @brief Reallocate memory in non-retention arena with explicit type.
 *
 * @param[in] ptr       Pointer to existing block (or NULL to allocate new).
 * @param[in] size      New size in bytes.
 * @param[in] usedType  Allocation tag/type for new block if moved.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_nonreten1(void *ptr, size_t size, unsigned short usedType)
{
    return tlk_realloc(&__nonRetentionHeader, ptr, size, usedType);
}

/**
 * @brief Allocate memory from default non-retention arena (test type).
 *
 * @param[in] size  Allocation size in bytes.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *malloc_nonreten(size_t size)
{
    return tlk_malloc(&__nonRetentionHeader, size, ARENA_TYPE_TEST);
}

/**
 * @brief Free memory allocated from default non-retention arena.
 *
 * @param[in] ptr  Pointer previously returned by malloc_nonreten/realloc_nonreten.
 */
void free_nonreten(void *ptr)
{
    return tlk_free(&__nonRetentionHeader, ptr);
}

/**
 * @brief Reallocate memory in default non-retention arena.
 *
 * @param[in] ptr   Pointer to existing block (or NULL to allocate new).
 * @param[in] size  New size in bytes.
 *
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *realloc_nonreten(void *ptr, size_t size)
{
    return tlk_realloc(&__nonRetentionHeader, ptr, size, ARENA_TYPE_TEST);
}
