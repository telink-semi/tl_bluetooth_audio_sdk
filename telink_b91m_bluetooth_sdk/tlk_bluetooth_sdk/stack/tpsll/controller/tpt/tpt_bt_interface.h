/********************************************************************************************************
 * @file    tpt_bt_interface.h
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
#ifndef STACK_TPSLL_TPT_BT_INTERFACE_H_
#define STACK_TPSLL_TPT_BT_INTERFACE_H_
#include "tl_common.h"

#if (TLK_STK_TPT_ENABLE)

/**
 * @brief       This function retrieves the target tick based on the clock.
 * @return      The target tick.
 * @note
 */
extern uint32_t tlk_bttpt_get_target_tick_based_clkn(void);

/**
 * @brief       This function fetches SCO parameters.
 * @param[out]  sco_used        - pointer to store whether SCO is used.
 * @param[out]  sco_task_tick   - pointer to store the SCO task tick.
 * @param[out]  interval        - pointer to store the SCO interval.
 * @return      none.
 * @note
 */
extern void tlk_bttpt_get_sco_parameters(uint8_t *sco_used, uint32_t *sco_task_tick, uint8_t *interval);

/**
 * @brief       This function handles BT sniff mode synchronization parameters.
 * @param[out]  bt_sniff_task_end_tick - pointer to store the end tick of the sniff task.
 * @param[out]  bt_sniff_intv_us       - pointer to store the sniff interval in microseconds.
 * @return      The result of the handler.
 * @note
 */
extern uint8_t tlk_bttpt_bt_sniff_para_sync_handler(uint32_t *bt_sniff_task_end_tick, uint32_t *bt_sniff_intv_us);

/**
 * @brief       This function handles BT disconnection transport protocol tasks.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_bt_disconnection_tpt_handler(void *parameter);

/**
 * @brief       This function handles BT SCO start transport protocol tasks.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_bt_sco_start_tpt_handler(void *parameter);

/**
 * @brief       This function handles BT SCO synchronization transport protocol tasks.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_bt_sco_sync_tpt_handler(void *parameter);

/**
 * @brief       This function handles the end of BT ACL task in SCO.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_bt_acl_end_in_sco_handler(void *parameter);

/**
 * @brief       This function retrieves the current role in TWS setup.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_tws_get_role_handler(void *parameter);

/**
 * @brief       This function checks if the TWS handover is busy.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_tws_get_handover_busy_handler(void *parameter);

/**
 * @brief       This function synchronizes AFH parameters in TWS setup.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_tws_afh_sync_parameters_handler(void *parameter);

/**
 * @brief       This function synchronizes encryption parameters in TWS setup.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_tws_enc_sync_parameters_handler(void *parameter);

/**
 * @brief       This function synchronizes sniff parameters in TWS setup.
 * @param[in]   parameter - pointer to the parameter required by the handler.
 * @return      The result of the handler.
 * @note
 */
extern uint32_t tlk_bttpt_tws_sniff_sync_parameters_handler(void *parameter);

/**
 * @brief       This function retrieves the BT page state.
 * @return      The current BT page state.
 * @note
 */
extern uint8_t tlk_bttpt_get_bt_page_state(void);

/**
 * @brief       This function retrieves the BT transmit traffic status.
 * @return      The current BT transmit traffic status.
 * @note
 */
extern uint8_t tlk_bttpt_get_bt_tx_traffic(void);

/**
 * @brief       This function retrieves the BT access mode.
 * @return      The current BT access mode.
 * @note
 */
extern bool tlk_bttpt_get_bt_access_mode(void);

/**
 * @brief       This function starts the connection setup for a headset.
 * @param[in]   timeout     - the timeout value for the connection setup.
 * @param[in]   setup_mode  - the setup mode for the connection.
 * @return      The result of the connection setup.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_start_connection_setup(uint32_t timeout, uint8_t setup_mode);

#endif
#endif /* STACK_TPSLL_TPT_BT_INTERFACE_H_ */
