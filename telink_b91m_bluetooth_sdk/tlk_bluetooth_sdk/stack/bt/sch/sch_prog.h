/********************************************************************************************************
 * @file    sch_prog.h
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


#ifndef SCH_PROG_H_
#define SCH_PROG_H_

/**
 ****************************************************************************************
 * @defgroup SCH_PROG Scheduling Programmer
 * @ingroup SCH
 * @brief Responsible for programming BT/BLE frames.
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "stack/bt/common/rwip_config.h" // stack configuration
#include "stack/system/system_internal.h"
#include <stdint.h>  // integer
#include <stdbool.h> // boolean

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Mode
enum SCH_FRAME_TYPE
{
    SCH_PROG_BT,
};

/// Frame type
enum SCH_FRAME_IRQ
{
    /// Normal End of Event
    SCH_FRAME_IRQ_EOF = 0x00,
    /// End of event due to an abort
    SCH_FRAME_IRQ_EOF_ABORT = 0x01,
    /// SKIP Event IRQ
    SCH_FRAME_IRQ_SKIP = 0x04,
    /// RX ACL IRQ
    SCH_FRAME_IRQ_RX = 0x02,
    /// TX ACL IRQ
    SCH_FRAME_IRQ_TX = 0x03,
};

/// Frame type
enum SCH_BT_FRAME_TYPE
{
    SCH_BT_FRAME_TYPE_NORMAL    = 0x00,
    SCH_BT_FRAME_TYPE_SNIFF     = 0x01,
    SCH_BT_FRAME_TYPE_ESCO      = 0x02,
    SCH_BT_FRAME_TYPE_CSB       = 0x03,
    SCH_BT_FRAME_TYPE_ESCO_RETX = 0x04,
};

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Callback for interrupt related to the frame
 *
 * @param[in]   timestamp   Timestamp of the programmed frame (in BLE slots, based on local clock)
 * @param[in]   dummy       Dummy value (provided by the driver when programming the frame via tlk_bt_ctrl_sch_prog_push)
 * @param[in]   irq_type    IRQ type (0: End Of Frame | 1: RX)
 ****************************************************************************************
 */
typedef void (*frm_cbk_t)(uint32_t timestamp, uint32_t dummy, uint8_t irq_type);


/**
 ****************************************************************************************
 * @brief Callback for interrupt related to the clock IRQ
 *
 * @param[in]   clock       Clock when the IRQ happened
 * @param[in]   id          Identification value
 ****************************************************************************************
 */
typedef void (*clk_cbk_t)(uint32_t clock, uint8_t id);

/// Additional parameters for BT frame
struct sch_prog_params_bt
{
    /// Frame type (0: normal | 1: sniff | 2: ESCO | 3: CSB)
    uint8_t frm_type;
    /// Voice channel
    uint8_t vxchan;
};

/// Parameters for a programmed frame
struct sch_prog_params
{
    /// Callback for handling interrupts related to the frame
    frm_cbk_t frm_cbk;
    /// Timestamp (in half-slots, based on local clock) and event offset (in half-us) of the programmed frame
    rwip_time_t time;
    /// Bandwidth duration of the event using priority 1 (in half us)
    uint32_t bandwidth;
    /// Dummy value reported when an event happen during the frame or the frame is completed
    uint32_t dummy;
    /// Priority during duration of bandwidth
    uint8_t prio_1;
    /// Priority after bandwidth elapsed
    uint8_t prio_2;
    /// Priority after trigger conditions
    uint8_t prio_3;
    /// Priority when specific action occurs during the event
    uint8_t pti_prio;
    /// Control structure index
    uint8_t cs_idx;
    /// Mode (0: BLE, 1:BT)
    uint8_t mode;

    union
    {
        /// Additional parameters for BT frame
        struct sch_prog_params_bt bt;
    } add;
};

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the SCH_PROG module
 *
 * @param[in]  reset    True: reset | False: init
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_init(bool reset);


/**
****************************************************************************************
* @brief Push a new programmed frame
*
* @param[in]   params     Parameters structure
****************************************************************************************
*/
void tlk_bt_ctrl_sch_prog_push(struct sch_prog_params *params);

/**
 ****************************************************************************************
 * @brief Handle "RX" interrupt
 *
 * @param[in] et_idx Entry index that trigger the ISR (@note Valid only with 5.0 + ISO HW)
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_rx_isr(uint8_t et_idx);

/**
 ****************************************************************************************
 * @brief Handle "TX" interrupt
 *
 * @param[in] et_idx Entry index that trigger the ISR (@note Valid only with 5.0 + ISO HW)
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_tx_isr(uint8_t et_idx);

/**
 ****************************************************************************************
 * @brief Handle "Skip" interrupt
 *
 * @param[in] et_idx Entry index that trigger the ISR (@note Valid only with 5.0 + ISO HW)
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_skip_isr(uint8_t et_idx);

/**
 ****************************************************************************************
 * @brief Handle "End of Event" interrupt
 *
 * @param[in] et_idx Entry index that trigger the ISR (@note Valid only with 5.0 + ISO HW)
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_end_isr(uint8_t et_idx);

/**
 ****************************************************************************************
 * @brief Handle "End of Event" interrupt
 *
 * @param[in] et_idx Entry index that trigger the ISR (@note Valid only with 5.0 + ISO HW)
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_err_isr(uint8_t et_idx);


/**
 ****************************************************************************************
 * @brief Enable the programming of an activity
 *
 * @param[in]   clk_cbk     Callback for handling interrupts related to the clock
 * @param[in]   id          Identification value
 * @param[in]   sr_size     Size of the sub rating pattern (in number of bits), 0 if no sub rating
 * @param[in]   sr_val      Sub rating pattern value, representing the clock LSBs when interrupt is raised
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_enable(clk_cbk_t clk_cbk, uint8_t id, uint8_t sr_size, uint8_t sr_val);

/**
 ****************************************************************************************
 * @brief Disable the programming of an activity
 *
 * @param[in]   id          Identification value
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_disable(uint8_t id);

/**
 ****************************************************************************************
 * @brief Handle "clock" interrupt
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_clk_isr(void);
/**
 ****************************************************************************************
 * @brief ET reset status_set
 ****************************************************************************************
 */
void tlk_bt_ctrl_sch_prog_et_reset_status_set(void);
/**
 ****************************************************************************************
 * @brief Get ET number
 ****************************************************************************************
 */
uint8_t tlk_bt_ctrl_sch_prog_number_get(void);

uint8_t tlk_bt_ctrl_sch_prog_get_current_et_index(void);

sch_time_t tlk_bt_ctrl_sch_prog_get_current_et_start_time();

/// @} SCH_PROG
#endif // SCH_PROG_H_
