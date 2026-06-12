/********************************************************************************************************
 * @file    tph_hcicmd.h
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
#ifndef STACK_TPHC_TPH_HCI_CMD__H_
#define STACK_TPHC_TPH_HCI_CMD__H_
#include "tl_common.h"
#if (TLK_STK_TPH_ENABLE)
#if (MCU_DUAL_CORE_ENABLE)

enum tph_controller_hci_opcode
{
    TPHC_HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    TPHC_HCI_SET_BT_MAC_CMD_OPCODE              = 0x0001,
    TPHC_HCI_SET_GEN_MAC_CMD_OPCODE             = 0x0002,
    TPHC_HCI_CONNECT_CMD_OPCODE                 = 0x0003,
    TPHC_HCI_DISCONN_CMD_OPCODE                 = 0x0004,
    TPHC_HCI_READ_AC_CHNID_CMD_OPCODE           = 0x0005,
    TPHC_HCI_MSG_PDU_ACL_CMD                    = 0x0006,
    TPHC_HCI_WRITE_AC_CHNID_CMD_OPCODE          = 0x0007,
    TPHC_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE = 0x0008,
    TPHC_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE = 0x0009,
    TPHC_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE  = 0x000A,
    TPHC_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE       = 0x000B,
    TPHC_HCI_SCO_MIC_DATA_OPCODE                = 0x000C,
    TPHC_HCI_SET_DONGLE_MAC_CMD_OPCODE          = 0x000D,
    TPHC_HCI_SET_HEADSET_MODE_OPCODE            = 0x000E,
    TPHC_HCI_CLEAR_HEADSET_MODE_OPCODE          = 0x000F,

    TPHC_HCI_READ_HEADSET_SINGLE_OPCODE   = 0x0010,
    TPHC_HCI_READ_HEADSET_ROLE_OPCODE     = 0x0011,
    TPHC_HCI_READ_HEADSET_BT_AUDIO_OPCODE = 0x0012,
    TPHC_HCI_READ_HEADSET_LEFT_OPCODE     = 0x0013,
    TPHC_HCI_READ_HEADSET_FNO_OPCODE      = 0x0014,
    TPHC_HCI_READ_HEADSET_MODE_OPCODE     = 0x0015,
    TPHC_HCI_HEADSET_CUR_IS_LEFT          = 0x0017,
    /* for three-mode-switch reinit tpsll controller. */
    TPHC_HCI_START_TPSLL_TASK = 0x0023,

    TPHC_HCI_TX_POWER_CONTROL_CMD  = 0x0027,
    TPHC_HCI_SNIFF_REQUEST_OPCODE  = 0x0028,
    TPHC_HCI_DONGLE_SET_OTA_STATUS = 0x0031,
};

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum
{
    TPH_HCI_TYPE_CMD      = 0x01,
    TPH_HCI_TYPE_SCO_DATA = 0x02,
} TLKTPH_HCI_TYPE_ENUM;

/**
 * @brief       This function handles HCI commands.
 * @param[in]   p       - pointer to the HCI command data.
 * @param[in]   len     - length of the HCI command data.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_hci_cmd_handler(uint8_t *p, uint32_t len);

#endif
#endif
#endif //STACK_TPSLL_TPH_HCI_CMD__H_
