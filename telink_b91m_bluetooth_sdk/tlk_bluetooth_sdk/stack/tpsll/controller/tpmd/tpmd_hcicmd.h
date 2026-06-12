/********************************************************************************************************
 * @file    tpmd_hcicmd.h
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
#ifndef STACK_TPMDC_TPMD_HCI_CMD_H_
#define STACK_TPMDC_TPMD_HCI_CMD_H_
#include "tl_common.h"
#if (TLK_STK_TPMD_ENABLE)
// #include "common/types.h"
// #include "common/compiler.h"
// #include "core/mcu_type.h"
// #include "common/bit.h"
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)

#if (TLK_CFG_TPSLL_HCI_ENABLE)
enum tpmd_controller_hci_opcode
{
    TPMDC_HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    TPMDC_HCI_SET_BT_MAC_CMD_OPCODE              = 0x0001,
    TPMDC_HCI_SET_GEN_MAC_CMD_OPCODE             = 0x0002,
    TPMDC_HCI_CONNECT_CMD_OPCODE                 = 0x0003,
    TPMDC_HCI_DISCONN_CMD_OPCODE                 = 0x0004,
    TPMDC_HCI_READ_AC_CHNID_CMD_OPCODE           = 0x0005,
    TPMDC_HCI_MSG_PDU_ACL_CMD_UI                 = 0x0006,
    TPMDC_HCI_WRITE_AC_CHNID_CMD_OPCODE          = 0x0007,
    TPMDC_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE = 0x0008,
    TPMDC_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE = 0x0009,
    TPMDC_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE  = 0x000A,
    TPMDC_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE       = 0x000B,
    TPMDC_HCI_SCO_MIC_DATA_OPCODE                = 0x000C,
    TPMDC_HCI_SET_DONGLE_MAC_CMD_OPCODE          = 0x000D,
    TPMDC_HCI_SET_HEADSET_MODE_OPCODE            = 0x000E,
    TPMDC_HCI_CLEAR_HEADSET_MODE_OPCODE          = 0x000F,

    TPMDC_HCI_READ_HEADSET_SINGLE_OPCODE     = 0x0010,
    TPMDC_HCI_READ_HEADSET_ROLE_OPCODE       = 0x0011,
    TPMDC_HCI_READ_HEADSET_BT_AUDIO_OPCODE   = 0x0012,
    TPMDC_HCI_READ_HEADSET_LEFT_OPCODE       = 0x0013,
    TPMDC_HCI_READ_HEADSET_FNO_OPCODE        = 0x0014,
    TPMDC_HCI_READ_HEADSET_MODE_OPCODE       = 0x0015,
    TPMDC_HCI_START_HEADSET_CONNECTION_SETUP = 0x0016,
    TPMDC_HCI_HEADSET_CUR_IS_LEFT            = 0x0017,
    TPMDC_HCI_WRITE_PROFILE_SYNC_INFO_OPCODE = 0x0018,
    TPMDC_HCI_READ_PROFILE_SYNC_INFO_OPCODE  = 0x0019,
    TPMDC_HCI_SEND_LMP_DATA_CMD              = 0x001A,
    TPMDC_HCI_SEND_ACL_DATA_CMD              = 0x001B,
    TPMDC_HCI_READ_LINK_INFO_CMD             = 0x001C,
    TPMDC_HCI_SET_SINGLE_ROLE_CMD            = 0x001D,
    TPMDC_HCI_WRITE_HEADSET_AC_CHNID_CMD     = 0x001E,
    TPMDC_HCI_START_HANDOVER_CMD             = 0x001F,
    TPMDC_HCI_START_HANDOVER_REQ_CMD         = 0x0020,
    TPMDC_HCI_WRITE_HANDOVER_INFO_CMD        = 0x0021,
    TPMDC_HCI_REQUEST_SYNC_HOST_TIMER_CMD    = 0x0022,
    TPMDC_HCI_START_TPSLL_TASK               = 0x0023,
    TPMDC_HCI_TX_POWER_CONTROL_CMD           = 0x0027,
    TPMDC_HCI_SNIFF_REQUEST_OPCODE           = 0x0028,
    TPMDC_HCI_DG_SCO_SETUP                   = 0x0029,
    TPMDC_HCI_DG_EXIT_SCO                    = 0x0030,

};

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum
{
    TPMD_HCI_TYPE_CMD      = 0x01,
    TPMD_HCI_TYPE_SCO_DATA = 0x02,
} TLKTPMD_HCI_TYPE_ENUM;
#endif // TLK_CFG_TPSLL_HCI_ENABLE

/**
 * @brief       This function handles HCI commands.
 * @param[in]   p       - pointer to the HCI command data.
 * @param[in]   len     - length of the HCI command data.
 * @return      none.
 * @note        
 */
extern void tlk_tpsll_tpmd_hci_cmd_handler(uint8_t *p, uint32_t len);


#endif

#endif // (TLK_STK_TPMD_ENABLE)

#endif //STACK_TPMDC_TPMD_HCI_CMD_H_