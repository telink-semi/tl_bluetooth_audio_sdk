/********************************************************************************************************
 * @file    tpdt_bt_interface.h
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
#ifndef STACK_TPSLL_TPDT_BT_INTERFACE_H_
#define STACK_TPSLL_TPDT_BT_INTERFACE_H_
#include "tl_common.h"
#if (TLK_STK_TPDT_ENABLE)
/**
 * @brief       This function retrieves the target tick based on the clock.
 * @return      The target tick.
 * @note        
 */
extern uint32_t tlk_bttpdt_get_target_tick_based_clkn(void);

/**
 * @brief       This function gets the SCO parameters.
 * @param[out]  sco_used        - pointer to store whether SCO is used.
 * @param[out]  sco_task_tick   - pointer to store the SCO task tick.
 * @param[out]  interval        - pointer to store the interval.
 * @return      none.
 * @note        
 */
extern void tlk_bttpdt_get_sco_parameters(uint8_t *sco_used, uint32_t *sco_task_tick, uint8_t *interval);

/**
 * @brief       This function handles BT disconnection timeout.
 * @param[in]   parameter     - pointer to the parameter for disconnection.
 * @return      The result of the disconnection handling.
 * @note        
 */
extern uint32_t tlk_bttpdt_bt_disconnection_tpdt_handler(void *parameter);

/**
 * @brief       This function handles BT SCO start timeout.
 * @param[in]   parameter     - pointer to the parameter for SCO start.
 * @return      The result of the SCO start handling.
 * @note        
 */
extern uint32_t tlk_bttpdt_bt_sco_start_tpdt_handler(void *parameter);

/**
 * @brief       This function handles BT SCO sync timeout.
 * @param[in]   parameter     - pointer to the parameter for SCO sync.
 * @return      The result of the SCO sync handling.
 * @note        
 */
extern uint32_t tlk_bttpdt_bt_sco_sync_tpdt_handler(void *parameter);

/**
 * @brief       This function handles BT ACL end in SCO timeout.
 * @param[in]   parameter     - pointer to the parameter for ACL end in SCO.
 * @return      The result of the ACL end in SCO handling.
 * @note        
 */
extern uint32_t tlk_bttpdt_bt_acl_end_in_sco_handler(void *parameter);

/**
 * @brief       This function gets the role of the TWS device.
 * @param[in]   parameter     - pointer to the parameter for getting TWS role.
 * @return      The role of the TWS device.
 * @note        
 */
extern uint32_t tlk_bttpdt_tws_get_role_handler(void *parameter);

/**
 * @brief       This function retrieves the BT page state.
 * @return      The BT page state.
 * @note        
 */
extern uint8_t tlk_bttpdt_get_bt_page_state(void);

/**
 * @brief       This function starts the connection setup for the headset.
 * @param[in]   timeout       - timeout for the connection setup.
 * @param[in]   setup_mode    - mode for the setup.
 * @return      Result of the connection setup.
 * @note        
 */
_attribute_le_ram_code_ uint8_t tlk_tpsll_tpdt_headset_start_connection_setup(uint32_t timeout, uint8_t setup_mode);
#endif
#endif /* STACK_TPSLL_TPDT_BT_INTERFACE_H_ */
