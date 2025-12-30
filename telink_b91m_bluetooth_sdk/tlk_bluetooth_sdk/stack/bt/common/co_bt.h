/********************************************************************************************************
 * @file    co_bt.h
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


#ifndef CO_BT_H_
#define CO_BT_H_

/**
 ****************************************************************************************
 * @addtogroup COMMON Common SW Block
 * @ingroup ROOT
 * @brief The Common RW SW Block.
 *
 * The COMMON is the block with Bluetooth definitions and structures shared
 * to all the protocol stack blocks. This also contain software wide error code
 * definitions, mathematical functions, help functions, list and buffer definitions.
 *
 * @{
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup CO_BT Common Bluetooth defines
 * @ingroup COMMON
 * @brief Common Bluetooth definitions and structures.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h> // standard boolean definitions
#include <stddef.h>  // standard definitions
#include <stdint.h>  // standard integer definitions

#include "co_bt_defines.h" // Bluetooth defines
#include "co_lmp.h"        // Bluetooth LMP definitions
#include "co_hci.h"        // Bluetooth HCI definitions
#include "co_error.h"      // Bluetooth error codes definitions

#include "drivers.h"
/*
 * DEFINES
 ****************************************************************************************
 */


#define BREDR_IRQ_ALL                                                                                                                                               \
    (BT_ENDFRMINTSTAT_BIT | BT_SKIPFRMINTSTAT_BIT | BT_ERRORINTSTAT_BIT | BT_RXINTSTAT_BIT | BT_STARTFRMINTSTAT_BIT | BT_CLKNINTSTAT_BIT | BT_GROSSTGTINTSTAT_BIT | \
     BT_FINETGTINTSTAT_BIT | BT_AUDIOINT0STAT_BIT | BT_AUDIOINT1STAT_BIT | BT_RXHEADERINTSTAT_BIT | BT_RXDONEINTSTAT_BIT)

/**
 ****************************************************************************************
 * @brief Function to initialize the random seed.
 * @param[in] seed The seed number to use to generate the random sequence.
 ****************************************************************************************
 */
//__INLINE void co_random_init(uint32_t seed)
//{
//    srand(seed);
//}

/**
 ****************************************************************************************
 * @brief Function to get an 8 bit random number.
 * @return Random byte value.
 ****************************************************************************************
 */
__INLINE uint8_t co_rand_byte(void)
{
    return (uint8_t)(trng_rand() & 0xFF);
}

/**
 ****************************************************************************************
 * @brief Function to get an 16 bit random number.
 * @return Random half word value.
 ****************************************************************************************
 */
__INLINE uint16_t co_rand_hword(void)
{
    return (uint16_t)(trng_rand() & 0xFFFF);
}

/**
 ****************************************************************************************
 * @brief Function to get an 32 bit random number.
 * @return Random word value.
 ****************************************************************************************
 */
__INLINE uint32_t co_rand_word(void)
{
    return (uint32_t)trng_rand();
}

/// @} CO_BT
#endif // CO_BT_H_
