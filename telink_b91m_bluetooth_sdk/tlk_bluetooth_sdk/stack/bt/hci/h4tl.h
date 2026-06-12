/********************************************************************************************************
 * @file    h4tl.h
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


#ifndef H4TL_H_
#define H4TL_H_

/**
 ****************************************************************************************
 * @addtogroup H4TL H4 UART Transport Layer
 * @ingroup H4TL
 * @brief H4 UART Transport Layer
 *
 * This module creates the abstraction between External UART driver and HCI generic functions
 * (designed for H4 UART transport layer).
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "stack/bt/common/rwip_config.h" // stack configuration

#if (H4TL_SUPPORT)
#include "stack/bt/common/rwip.h" // SW interface

#include <stdint.h>  // standard integer definition
#include <stdbool.h> // standard boolean definition

/*
 * DEFINES
 ****************************************************************************************
 */

/// Size of the logical channel identifier for H4 messages
#define H4TL_LOGICAL_CHANNEL_LEN (8) //this must be integral multiple of 4

/**
 * Number of H4TL interfaces
 *
 *  * NB=2: AHI and HCI: for Host-only stack with external app
 *       - HCI has index 0
 *       - AHI has index 1
 *  * NB=1: AHI or HCI: for all other partitions
 *
 * Note: it is not possible to have no channel (H4TL must not be included in build in this case)
 */
#if (!BLE_EMB_PRESENT && HCI_TL_SUPPORT && AHI_TL_SUPPORT)
#define H4TL_NB_CHANNEL 2
#else // (!BLE_EMB_PRESENT && HCI_TL_SUPPORT && AHI_TL_SUPPORT)
#define H4TL_NB_CHANNEL 1
#endif // (!BLE_EMB_PRESENT && HCI_TL_SUPPORT && AHI_TL_SUPPORT)


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief H4TL transport initialization.
 *
 * Puts the External Interface driver in reception, waiting for simple 1 byte message type. Space for
 * reception is allocated with tlk_bt_ctrl_ke_msg_alloc and the pointer is handed to env.rx. RX
 * interrupt is enabled.
 *
 * @param[in] tl_type  Transport Layer Interface (@see enum h4tl_itf)
 * @param[in] eif      External interface API
 *
 *****************************************************************************************
 */
void tlk_bt_ctrl_h4tl_init(uint8_t tl_type, const struct rwip_eif_api *eif);


/**
 ****************************************************************************************
 * @brief H4TL write function.
 *
 * @param[in] type  Type of the buffer to be transmitted. It can take one of the following
 *                  values:
 *      - @ref HCI_EVT_MSG_TYPE for event message
 *      - @ref HCI_ACL_MSG_TYPE for ACL data
 *      - @ref HCI_SYNC_MSG_TYPE for synchronous data
 *
 * @param[in] buf   Pointer to the buffer to be transmitted. @note The buffer passed as
 *  parameter must have one free byte before the first payload byte, so that the H4TL
 *  module can put the type byte as first transmitted data.
 *
 * @param[in] len   Length of the buffer to be transmitted.
 * @param[in] tx_callback   Callback for indicating the end of transfer
 *****************************************************************************************
 */
void tlk_bt_ctrl_h4tl_write(uint8_t type, uint8_t *buf, uint16_t len, void (*tx_callback)(void));

/**
 ****************************************************************************************
 * @brief Start External Interface input flow
 *
 *****************************************************************************************
 */
void tlk_bt_ctrl_h4tl_start(void);

/**
 ****************************************************************************************
 * @brief Stop External Interface input flow if possible
 *
 * @return true if External Interface flow was stopped, false otherwise
 *****************************************************************************************
 */
bool tlk_bt_ctrl_h4tl_stop(void);

#endif //H4TL_SUPPORT

/// @} H4TL

#endif // H4TL_H_
