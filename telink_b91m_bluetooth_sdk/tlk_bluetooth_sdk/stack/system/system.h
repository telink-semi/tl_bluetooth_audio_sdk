/********************************************************************************************************
 * @file    system.h
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
#ifndef TLKSTK_SYSTEM_SYSTEM_H_
#define TLKSTK_SYSTEM_SYSTEM_H_
#define BIT(n) (1 << (n))

/* Planner Interval, Time = N * 0.625 ms,
 * Notice that these are just part of but not all value */
typedef enum
{
    PLAN_INTERVAL_5MS    = 8,
    PLAN_INTERVAL_7P5MS  = 12,
    PLAN_INTERVAL_10MS   = 16,
    PLAN_INTERVAL_15MS   = 24,
    PLAN_INTERVAL_20MS   = 32,
    PLAN_INTERVAL_25MS   = 40,
    PLAN_INTERVAL_30MS   = 48,
    PLAN_INTERVAL_31MS25 = 50,
    PLAN_INTERVAL_40MS   = 64,
    PLAN_INTERVAL_45MS   = 64,
    PLAN_INTERVAL_50MS   = 80,
    PLAN_INTERVAL_60MS   = 96,
    PLAN_INTERVAL_70MS   = 112,
    PLAN_INTERVAL_80MS   = 128,
    PLAN_INTERVAL_90MS   = 144,
    PLAN_INTERVAL_100MS  = 160,
    PLAN_INTERVAL_150MS  = 240,
    PLAN_INTERVAL_200MS  = 320,
    PLAN_INTERVAL_250MS  = 400,
    PLAN_INTERVAL_300MS  = 480,
    PLAN_INTERVAL_350MS  = 560,
    PLAN_INTERVAL_400MS  = 640,
    PLAN_INTERVAL_450MS  = 720,
    PLAN_INTERVAL_500MS  = 800,
    PLAN_INTERVAL_600MS  = 960,
    PLAN_INTERVAL_700MS  = 1120,
    PLAN_INTERVAL_800MS  = 1280,
    PLAN_INTERVAL_900MS  = 1440,
    PLAN_INTERVAL_1000MS = 1600,
} plan_intvl_t;

typedef enum
{
    BLE_LOOP_MASK   = BIT(0),
    BT_LOOP_MASK    = BIT(1),
    TPSLL_LOOP_MASK = BIT(2),

} ctl_loop_mask;

/**
 * @brief		This function is used to initialize MCU hardware
 * @param[in]	none
 * @return      none
 */
void tlk_sys_init_mcu_hardware(void);
void tlk_sys_restore_mcu_hardware(void);

/**
 * @brief	    IRQ handler for Bluetooth SDK
 * @param[in]	irq_index - Interrupt source index
 * @return	    none
 */
void tlk_sys_irq_handler(int irq_index);


/**
 * @brief   main_loop entry for Bluetooth SDK
 * @param	none
 * @return	none
 */
void tlk_sys_main_loop(void);


/**
 * @brief		This function is used to initialize scheduler
 * @param[in]	none
 * @return      none
 */
void tlk_sch_init(void);


void tlk_sys_hci_reset(void);


void tlk_sys_stack_process_initialization_end(void);

/**
 * @brief      This function checks whether the scheduler is RUNNING
 * @param      none
 * @return     1:  scheduler is running; 0: scheduler is idle
 */
int tlk_sch_is_running(void);


/**
 ****************************************************************************************
 * @brief Set base interval for planner
 *
 * @param[in] base interval, unit: 625uS
 *
 * @return none
 ****************************************************************************************
 */
void tlk_sch_plan_set_base_interval(unsigned short interval);
#endif /* TLKSTK_SYSTEM_SYSTEM_H_ */
