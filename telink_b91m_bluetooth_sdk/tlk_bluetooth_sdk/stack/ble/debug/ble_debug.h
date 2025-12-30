/********************************************************************************************************
 * @file    ble_debug.h
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
#ifndef STACK_BLE_DEBUG_H_
#define STACK_BLE_DEBUG_H_

/**
 *  @brief stack log
 */
typedef enum
{
    STK_LOG_NONE = 0,

    STK_LOG_LL_CMD = BIT(0),

    STK_LOG_ATT_RX = BIT(5),
    STK_LOG_ATT_TX = BIT(6),

    STK_LOG_SMP_LTK = BIT(10),

    STK_LOG_PRF_CS = BIT(12),

    STK_LOG_OTA_FLOW = BIT(15),
    STK_LOG_OTA_DATA = BIT(16),

    STK_LOG_HCI_CS = BIT(20),

    STK_LOG_LL_RX = BIT(26),
    STK_LOG_LL_TX = BIT(27),

    STK_LOG_ALL = 0xFFFFFFFF,
} stk_log_msk_t;

/**
 * @brief      for user to configure which type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_enableStackLog(stk_log_msk_t mask);


/**
 * @brief      for user to add some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_addStackLog(stk_log_msk_t mask);


/**
 * @brief      for user to remove some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_removeStackLog(stk_log_msk_t mask);




#endif /* STACK_BLE_DEBUG_H_ */
