/********************************************************************************************************
 * @file    h0tl.h
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
#ifndef _BTC_H0TL_H_
#define _BTC_H0TL_H_


#include "stack/bt/common/rwip_config.h" // stack configuration

#if (H0TL_SUPPORT)
#include "stack/bt/common/rwip.h" // SW interface
#include "stack/bt/ke/ke_msg.h"   // kernel event
#include <stdint.h>               // standard integer definition
#include <stdbool.h>              // standard boolean definition

/*
 * DEFINES
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Function called when hci packet is sent from controller to host.

 * @param[in]  dummy  Dummy data pointer returned to callback when operation is over.
 * @param[in]  status Ok if action correctly performed, else reason status code.
 *****************************************************************************************
 */
typedef int (*h0tl_c2h_rx_callback_t)(uint8_t type, uint8_t *hci_packet, uint16_t len);

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
 * @brief H0TL transport initialization. H0TL means Host and Controller work in SoC method.
 *
 * Config the num and size for those two FIFOs both in h2c and c2h directionss
 * 
 * 
 * @param[in] h2c_fifo_num  
 * @param[in] h2c_fifo_size  
 * @param[in] c2h_fifo_num  
 * @param[in] c2h_fifo_size  
 * 
 *****************************************************************************************
 */
void tlk_bt_ctrl_h0tl_init(uint8_t h2c_fifo_num, uint16_t h2c_fifo_size, uint8_t c2h_fifo_num, uint16_t c2h_fifo_size, h0tl_c2h_rx_callback_t cb);

/**
 ****************************************************************************************
 * @brief H0TL transport Reset. H0TL means Host and Controller work in SoC method.
 *
 * free all pending message(s)
 *
 *
 * @param[in] None
 *
 *
 *
 *****************************************************************************************
 */
void tlk_bt_ctrl_h0tl_reset(void);

/**
 ****************************************************************************************
 * @brief This function is called by hci_tl module to send hci c2h packets from controller
 *        to host. This API is internally used by controller stack only.
 *
 * 
 * 
 * 
 * @param[in] msg    hci c2h packet
 * 
 *****************************************************************************************
 */
void tlk_bt_ctrl_h0tl_c2h_write(struct ke_msg *msg);

/**
 ****************************************************************************************
 * @brief H0TL write hci command API. This is called by host stack once it wants to send 
 *        an HCI command to controller in SoC mode.
 *
 * @param[in] opcode  opcode of HCI command
 *
 * @param[in] parlen  length of this HCI command
 *
 * @param[in] param   pointer to the HCI command parameters
 *****************************************************************************************
 */
// uint8_t tlk_bt_ctrl_h0tl_h2c_write_command(uint16_t opcode, uint8_t parlen, uint8_t *param);
uint8_t tlk_bt_ctrl_h0tl_h2c_write_command(uint16_t opcode, volatile uint8_t *param, uint8_t parlen);

/**
 ****************************************************************************************
 * @brief H0TL write hci acl data API. This is called by host stack once it wants to send 
 *        an HCI acl data packet to controller in SoC mode.
 *
 * @param[in] hdl_flags  acl connection handle and packet flags associated to this HCI acl
 *            data packet
 *
 * @param[in] data_len  length of this HCI acl data packet
 *
 * @param[in] data   pointer to the payload of this HCI acl data packet
 *****************************************************************************************
 */
uint8_t tlk_bt_ctrl_h0tl_h2c_write_acl_data(uint16_t hdl_flags, uint16_t data_len, uint8_t *data);

/**
 ****************************************************************************************
 * @brief H0TL write hci sync data API. This is called by host stack once it wants to send 
 *        an HCI sync data packet to controller in SoC mode.
 *
 * @param[in] hdl_flags  SCO/eSCO connection-handle and packet flags associated to this HCI
 *            sync data
 *
 * @param[in] data_total_len  length of this HCI sync data packet
 *
 * @param[in] data   pointer to the payload of this HCI sync data packet
 *****************************************************************************************
 */
#if (1)
uint8_t tlk_bt_ctrl_h0tl_h2c_write_sync_data(uint16_t conhdl_flags, uint8_t data_total_len, uint8_t *data);
#endif

typedef int (*TlkBtH4c2htxCallback)(void);

void tlk_bt_ctrl_hci_regh4CB(TlkBtH4c2htxCallback cb);

uint8_t tlk_bt_ctrl_h0tl_h2c_fifo_unused(void);

uint8_t tlk_bt_ctrl_h0tl_h2c_acl_tx_queue_free_count(void);

uint8_t tlk_bt_ctrl_h0tl_h2c_fifo_used(void);

#endif //H0TL_SUPPORT

/// @} H0TL

#endif // _BTC_H0TL_H_
