/********************************************************************************************************
 * @file    tpdt_hcicmd.h
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
#ifndef STACK_TPDTC_TPDT_HCI_CMD_H_
#define STACK_TPDTC_TPDT_HCI_CMD_H_
#include "tl_common.h"
#if (CONTROLLER_MODE == BLETPSLL_TWS)
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)

    #if (TLK_CFG_TPSLL_HCI_ENABLE)
enum tpdt_controller_hci_opcode
{
    TPDTC_HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    TPDTC_HCI_SET_BT_MAC_CMD_OPCODE              = 0x0001,
    TPDTC_HCI_SET_GEN_MAC_CMD_OPCODE             = 0x0002,
    TPDTC_HCI_CONNECT_CMD_OPCODE                 = 0x0003,
    TPDTC_HCI_DISCONN_CMD_OPCODE                 = 0x0004,
    TPDTC_HCI_READ_AC_CHNID_CMD_OPCODE           = 0x0005,
    TPDTC_HCI_MSG_PDU_ACL_CMD_UI                 = 0x0006,
    TPDTC_HCI_WRITE_AC_CHNID_CMD_OPCODE          = 0x0007,
    TPDTC_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE = 0x0008,
    TPDTC_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE = 0x0009,
    TPDTC_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE  = 0x000A,
    TPDTC_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE       = 0x000B,
    TPDTC_HCI_SCO_MIC_DATA_OPCODE                = 0x000C,
    TPDTC_HCI_SET_DONGLE_MAC_CMD_OPCODE          = 0x000D,
    TPDTC_HCI_SET_HEADSET_MODE_OPCODE            = 0x000E,
    TPDTC_HCI_CLEAR_HEADSET_MODE_OPCODE          = 0x000F,

    TPDTC_HCI_READ_HEADSET_SINGLE_OPCODE     = 0x0010,
    TPDTC_HCI_READ_HEADSET_ROLE_OPCODE       = 0x0011,
    TPDTC_HCI_READ_HEADSET_BT_AUDIO_OPCODE   = 0x0012,
    TPDTC_HCI_READ_HEADSET_LEFT_OPCODE       = 0x0013,
    TPDTC_HCI_READ_HEADSET_FNO_OPCODE        = 0x0014,
    TPDTC_HCI_READ_HEADSET_MODE_OPCODE       = 0x0015,
    TPDTC_HCI_START_HEADSET_CONNECTION_SETUP = 0x0016,
    TPDTC_HCI_HEADSET_CUR_IS_LEFT            = 0x0017,
    TPDTC_HCI_WRITE_PROFILE_SYNC_INFO_OPCODE = 0x0018,
    TPDTC_HCI_READ_PROFILE_SYNC_INFO_OPCODE  = 0x0019,
    TPDTC_HCI_SEND_LMP_DATA_CMD              = 0x001A,
    TPDTC_HCI_SEND_ACL_DATA_CMD              = 0x001B,
    TPDTC_HCI_READ_LINK_INFO_CMD             = 0x001C,
    TPDTC_HCI_SET_SINGLE_ROLE_CMD            = 0x001D,
    TPDTC_HCI_WRITE_HEADSET_AC_CHNID_CMD     = 0x001E,
    TPDTC_HCI_START_HANDOVER_CMD             = 0x001F,
    TPDTC_HCI_START_HANDOVER_REQ_CMD         = 0x0020,
    TPDTC_HCI_WRITE_HANDOVER_INFO_CMD        = 0x0021,
    TPDTC_HCI_REQUEST_SYNC_HOST_TIMER_CMD    = 0x0022,

	TPDTC_HCI_START_TPSLL_TASK				 = 0x0023,//todo by junwei for test
	TPDTC_HCI_SET_HEADSET_LE_MODE_OPCODE	 = 0x0024,
	TPDTC_HCI_CLEAR_HEADSET_LE_MODE_OPCODE	 = 0x0025,

};

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum
{
    TPDT_HCI_TYPE_CMD      = 0x01,
    TPDT_HCI_TYPE_SCO_DATA = 0x02,
} TLKTPDT_HCI_TYPE_ENUM;
#endif // TLK_CFG_TPSLL_HCI_ENABLE

/**
 * @brief       This function handles HCI commands.
 * @param[in]   p       - pointer to the HCI command data.
 * @param[in]   len     - length of the HCI command data.
 * @return      none.
 * @note        
 */
extern void tpdt_hci_cmd_handler(uint8_t *p, uint32_t len);


#endif
#endif
#endif     //STACK_TPDTC_TPDT_HCI_CMD_H_
