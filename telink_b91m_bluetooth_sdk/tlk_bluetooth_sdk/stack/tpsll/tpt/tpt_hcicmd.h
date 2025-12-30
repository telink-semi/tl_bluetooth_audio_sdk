/********************************************************************************************************
 * @file    tpt_hcicmd.h
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
#ifndef STACK_TPTC_TPT_HCI_CMD_H_
#define STACK_TPTC_TPT_HCI_CMD_H_
#include "tl_common.h"
#if (CONTROLLER_MODE == BTTPSLL_TWS)
// #include "common/types.h"
// #include "common/compiler.h"
// #include "core/mcu_type.h"
// #include "common/bit.h"
#if (MCU_DUAL_CORE_ENABLE)

    #if (TLK_CFG_TPSLL_HCI_ENABLE)
enum tpt_controller_hci_opcode
{
    TPTC_HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    TPTC_HCI_SET_BT_MAC_CMD_OPCODE              = 0x0001,
    TPTC_HCI_SET_GEN_MAC_CMD_OPCODE             = 0x0002,
    TPTC_HCI_CONNECT_CMD_OPCODE                 = 0x0003,
    TPTC_HCI_DISCONN_CMD_OPCODE                 = 0x0004,
    TPTC_HCI_READ_AC_CHNID_CMD_OPCODE           = 0x0005,
    TPTC_HCI_MSG_PDU_ACL_CMD_UI                 = 0x0006,
    TPTC_HCI_WRITE_AC_CHNID_CMD_OPCODE          = 0x0007,
    TPTC_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE = 0x0008,
    TPTC_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE = 0x0009,
    TPTC_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE  = 0x000A,
    TPTC_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE       = 0x000B,
    TPTC_HCI_SCO_MIC_DATA_OPCODE                = 0x000C,
    TPTC_HCI_SET_DONGLE_MAC_CMD_OPCODE          = 0x000D,
    TPTC_HCI_SET_HEADSET_MODE_OPCODE            = 0x000E,
    TPTC_HCI_CLEAR_HEADSET_MODE_OPCODE          = 0x000F,

    TPTC_HCI_READ_HEADSET_SINGLE_OPCODE     = 0x0010,
    TPTC_HCI_READ_HEADSET_ROLE_OPCODE       = 0x0011,
    TPTC_HCI_READ_HEADSET_BT_AUDIO_OPCODE   = 0x0012,
    TPTC_HCI_READ_HEADSET_LEFT_OPCODE       = 0x0013,
    TPTC_HCI_READ_HEADSET_FNO_OPCODE        = 0x0014,
    TPTC_HCI_READ_HEADSET_MODE_OPCODE       = 0x0015,
    TPTC_HCI_START_HEADSET_CONNECTION_SETUP = 0x0016,
    TPTC_HCI_HEADSET_CUR_IS_LEFT            = 0x0017,
    TPTC_HCI_WRITE_PROFILE_SYNC_INFO_OPCODE = 0x0018,
    TPTC_HCI_READ_PROFILE_SYNC_INFO_OPCODE  = 0x0019,
    TPTC_HCI_SEND_LMP_DATA_CMD              = 0x001A,
    TPTC_HCI_SEND_ACL_DATA_CMD              = 0x001B,
    TPTC_HCI_READ_LINK_INFO_CMD             = 0x001C,
    TPTC_HCI_SET_SINGLE_ROLE_CMD            = 0x001D,
    TPTC_HCI_WRITE_HEADSET_AC_CHNID_CMD     = 0x001E,
    TPTC_HCI_START_HANDOVER_CMD             = 0x001F,
    TPTC_HCI_START_HANDOVER_REQ_CMD         = 0x0020,
    TPTC_HCI_WRITE_HANDOVER_INFO_CMD        = 0x0021,
    TPTC_HCI_REQUEST_SYNC_HOST_TIMER_CMD    = 0x0022,
    TPTC_HCI_START_TPSLL_TASK               = 0x0023,
    TPTC_HCI_TX_POWER_CONTROL_CMD           = 0x0027,
    TPTC_HCI_SNIFF_REQUEST_OPCODE           = 0x0028,

};

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum
{
    TPT_HCI_TYPE_CMD      = 0x01,
    TPT_HCI_TYPE_SCO_DATA = 0x02,
} TLKTPT_HCI_TYPE_ENUM;
#endif // TLK_CFG_TPSLL_HCI_ENABLE

/**
 * @brief       This function handles HCI commands.
 * @param[in]   p       - pointer to the HCI command data.
 * @param[in]   len     - length of the HCI command data.
 * @return      none.
 * @note        
 */
extern void tpt_hci_cmd_handler(uint8_t *p, uint32_t len);


#endif

#endif // (CONTROLLER_MODE == BTTPSLL_TWS)

#endif     //STACK_TPTC_TPT_HCI_CMD_H_
