/********************************************************************************************************
 * @file    tph_bt_interface.h
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
#ifndef STACK_TPSLL_TPH_BT_INTERFACE_H_
#define STACK_TPSLL_TPH_BT_INTERFACE_H_
#include "tl_common.h"
#if (TLK_STK_TPH_ENABLE)


/**
 * @brief       This function retrieves the target tick based on the clock.
 * @return      The target tick.
 * @note
 */
extern uint32_t tlk_bttph_get_target_tick_based_clkn(void);

/**
 * @brief       This function fetches the SCO parameters.
 * @param[out]  sco_used    - pointer to store whether SCO is used.
 * @param[out]  sco_task_tick - pointer to store the SCO task tick.
 * @param[out]  interval    - pointer to store the SCO interval.
 * @return      none.
 * @note
 */
extern void tlk_bttph_get_sco_parameters(uint8_t *sco_used, uint32_t *sco_task_tick, uint8_t *interval);

/**
 * @brief       This function handles BT disconnection events.
 * @param[in]   parameter - pointer to the parameter structure.
 * @return      The handler result.
 * @note
 */
extern uint32_t tlk_bttph_bt_disconnection_tph_handler(void *parameter);

/**
 * @brief       This function handles BT SCO start event.
 * @param[in]   parameter - pointer to the parameter structure.
 * @return      The handler result.
 * @note
 */
extern uint32_t tlk_bttph_bt_sco_start_tph_handler(void *parameter);

/**
 * @brief       This function handles BT SCO sync event.
 * @param[in]   parameter - pointer to the parameter structure.
 * @return      The handler result.
 * @note
 */
extern uint32_t tlk_bttph_bt_sco_sync_tph_handler(void *parameter);

/**
 * @brief       This function handles BT ACL end in SCO event.
 * @param[in]   parameter - pointer to the parameter structure.
 * @return      The handler result.
 * @note
 */
extern uint32_t tlk_bttph_bt_acl_end_in_sco_handler(void *parameter);

/**
 * @brief       This function retrieves the BT page state.
 * @return      The current BT page state.
 * @note
 */
extern uint8_t tlk_bttph_get_bt_page_state(void);

/**
 * @brief       This function retrieves the BT TX traffic.
 * @return      The current BT TX traffic status.
 * @note
 */
extern uint8_t tlk_bttph_get_bt_tx_traffic(void);

/**
 * @brief       This function retrieves the BT access mode.
 * @return      The current BT access mode.
 * @note
 */
extern bool tlk_bttph_get_bt_access_mode(void);

/**
 * @brief       This function handles BT sniff mode synchronization parameters.
 * @param[out]  bt_sniff_task_end_tick - pointer to store the end tick of the sniff task.
 * @param[out]  bt_sniff_intv_us       - pointer to store the sniff interval in microseconds.
 * @return      The result of the handler.
 * @note
 */
extern uint8_t tlk_bttph_bt_sniff_para_sync_handler(uint32_t *bt_sniff_task_end_tick, uint32_t *bt_sniff_intv_us);


#endif

#endif /* STACK_TPSLL_TPH_BT_INTERFACE_H_ */
