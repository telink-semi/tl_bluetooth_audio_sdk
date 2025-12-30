/********************************************************************************************************
 * @file    bt_common.h
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


#ifndef BT_COMMON_H_
#define BT_COMMON_H_

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
 * @addtogroup BT_COMMON Bluetooth defines
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
#include "co_bt_defines.h"
#include "co_bt.h"
#include "co_endian.h"
#include "co_error.h"
#include "co_hci.h"
#if (CHIP_TYPE == CHIP_TYPE_TL752X)
#include "co_time.h"
#endif
#include "co_lmp.h"
#include "co_utils.h"
#include "co_version.h"
#include "ecc_p256.h"
#include "em_map_bt.h"
#include "em_map_config.h"
#include "em_map.h"
#include "incs.h"
#include "rwbt_config.h"
#include "rwip_config.h"
#include "rwip_int.h"
#include "rwip_task.h"
#include "rwip.h"
/// @} BT_COMMON
#endif // BT_COMMON_H_
