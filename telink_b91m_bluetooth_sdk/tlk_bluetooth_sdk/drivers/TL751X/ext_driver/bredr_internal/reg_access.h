/********************************************************************************************************
 * @file    reg_access.h
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


#ifndef REG_ACCESS_H_
#define REG_ACCESS_H_

/**
 ****************************************************************************************
 * @addtogroup REG REG_ACCESS
 * @ingroup DRIVERS
 *
 * @brief Basic primitives for register access.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "tl_common.h"
//#include <string.h>           // string functions
#include "reg_em_base.h"
//#include "bredr_ext.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */
/// Macro to read a common ip register
#define REG_IP_RD(addr) (*(volatile uint32_t *)(addr))

/// Macro to write a common ip register
#define REG_IP_WR(addr, value) (*(volatile uint32_t *)(addr)) = (value)

/// Macro to read a BT register
#define REG_BT_RD(addr) (*(volatile uint32_t *)(addr))

/// Macro to write a BT register
#define REG_BT_WR(addr, value) (*(volatile uint32_t *)(addr)) = (value)

/// Macro to read a BT control structure field (16-bit wide)
#define EM_BT_RD(addr) (*(volatile uint16_t *)(addr))

/// Macro to write a BT control structure field (16-bit wide)
#define EM_BT_WR(addr, value) (*(volatile uint16_t *)(addr)) = (value)

/// Macro to read a EM field (16-bit wide)
#define EM_RD(addr) (*(volatile uint16_t *)(addr))

/// Macro to write a EM field (16-bit wide)
#define EM_WR(addr, value) (*(volatile uint16_t *)(addr)) = (value)

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Read a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @return The 16 bits value.
 ****************************************************************************************
 */
__INLINE uint16_t ext_co_read16p(void const *ptr16)
{
    uint16_t value = ((uint8_t *)ptr16)[0] | ((uint8_t *)ptr16)[1] << 8;
    return value;
}

/**
 ****************************************************************************************
 * @brief Write a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void ext_co_write16p(void const *ptr16, uint16_t value)
{
    uint8_t *ptr = (uint8_t *)ptr16;

    *ptr++ = value & 0xff;
    *ptr   = (value & 0xff00) >> 8;
}

/**
 ****************************************************************************************
 * @brief Read a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @return The 24 bits value.
 ****************************************************************************************
 */
__INLINE uint32_t ext_co_read24p(void const *ptr24)
{
    uint16_t addr_l, addr_h;
    addr_l = ext_co_read16p(ptr24);
    addr_h = *((uint8_t *)ptr24 + 2) & 0x00FF;
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}

/**
 ****************************************************************************************
 * @brief Write a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void ext_co_write24p(void const *ptr24, uint32_t value)
{
    uint8_t *ptr = (uint8_t *)ptr24;

    *ptr++ = (uint8_t)(value & 0xff);
    *ptr++ = (uint8_t)((value & 0xff00) >> 8);
    *ptr++ = (uint8_t)((value & 0xff0000) >> 16);
}

/**
 ****************************************************************************************
 * @brief Read a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @return The 32 bits value.
 ****************************************************************************************
 */
__INLINE uint32_t ext_co_read32p(void const *ptr32)
{
    uint16_t addr_l, addr_h;
    addr_l = ext_co_read16p(ptr32);
    addr_h = ext_co_read16p((uint8_t *)ptr32 + 2);
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}

/**
 ****************************************************************************************
 * @brief Write a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void ext_co_write32p(void const *ptr32, uint32_t value)
{
    uint8_t *ptr = (uint8_t *)ptr32;

    *ptr++ = (uint8_t)(value & 0xff);
    *ptr++ = (uint8_t)((value & 0xff00) >> 8);
    *ptr++ = (uint8_t)((value & 0xff0000) >> 16);
    *ptr   = (uint8_t)((value & 0xff000000) >> 24);
}

__INLINE uint16_t sys_addr_to_em_addr(void *sys_addr)
{
    return (uint16_t)(sys_addr - (void *)(EM_BASE_ADDR));
}

/// Read bytes from EM
__INLINE void em_rd(void *sys_addr, uint16_t em_addr, uint16_t len)
{
    tmemcpy(sys_addr, (void *)(em_addr + EM_BASE_ADDR), len);
}

/// Write bytes to EM
__INLINE void em_wr(void const *sys_addr, uint16_t em_addr, uint16_t len)
{
    tmemcpy((void *)(em_addr + EM_BASE_ADDR), sys_addr, len);
}

// copy two exchange memory area
__INLINE void em_cpy(uint16_t dst_em_addr, uint16_t src_em_addr, uint16_t len)
{
    tmemcpy((void *)(dst_em_addr + EM_BASE_ADDR), (void *)(src_em_addr + EM_BASE_ADDR), len);
}

/// Fill an EM space with the same value
__INLINE void em_set(int value, uint16_t em_addr, uint16_t len)
{
    tmemset((void *)(em_addr + EM_BASE_ADDR), value, len);
}

/// Read 24-bits value from EM
__INLINE uint32_t em_rd24p(uint16_t em_addr)
{
    return ext_co_read24p((void *)(em_addr + EM_BASE_ADDR));
}

/// Write 24-bits value to EM
__INLINE void em_wr24p(uint16_t em_addr, uint32_t value)
{
    ext_co_write24p((void *)(em_addr + EM_BASE_ADDR), value);
}

/// Read 32-bits value from EM
__INLINE uint32_t em_rd32p(uint16_t em_addr)
{
    return ext_co_read32p((void *)(em_addr + EM_BASE_ADDR));
}

/// Write 32-bits value to EM
__INLINE void em_wr32p(uint16_t em_addr, uint32_t value)
{
    ext_co_write32p((void *)(em_addr + EM_BASE_ADDR), value);
}

/// Read 16-bits value from EM
__INLINE uint16_t em_rd16p(uint16_t em_addr)
{
    return ext_co_read16p((void *)(em_addr + EM_BASE_ADDR));
}

/// Write 16-bits value to EM
__INLINE void em_wr16p(uint16_t em_addr, uint16_t value)
{
    ext_co_write16p((void *)(em_addr + EM_BASE_ADDR), value);
}

/// Read 8-bits value from EM
__INLINE uint16_t em_rd8p(uint16_t em_addr)
{
    return *((uint8_t *)(em_addr + EM_BASE_ADDR));
}

/// Write 8-bits value to EM
__INLINE void em_wr8p(uint16_t em_addr, uint8_t value)
{
    *(uint8_t *)(em_addr + EM_BASE_ADDR) = value;
}

/// @} REG

#endif // REG_ACCESS_H_
