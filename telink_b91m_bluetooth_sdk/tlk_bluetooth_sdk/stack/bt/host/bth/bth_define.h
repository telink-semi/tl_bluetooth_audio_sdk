/********************************************************************************************************
 * @file    bth_define.h
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
#ifndef BTH_DEFINE_H
#define BTH_DEFINE_H


#define BTH_LINK_TYPE_SCO            0
#define BTH_LINK_TYPE_ACL            1
#define BTH_LINK_TYPE_ESCO           2
#define BTH_LINK_TYPE_UNKNOWN        3 // Used in LM
#define BTH_LINK_TYPE_LE             4


#define BTH_HCI_SYNC_HDR_PSF_GOOD    0
#define BTH_HCI_SYNC_HDR_PSF_INVALID 1
#define BTH_HCI_SYNC_HDR_PSF_NO_RCVD 2
#define BTH_HCI_SYNC_HDR_PSF_LOST    3


#define PAGE_SCAN_REPETITION_MODE_R0 0x00
#define PAGE_SCAN_REPETITION_MODE_R1 0x01
#define PAGE_SCAN_REPETITION_MODE_R2 0x02

#define PAGE_SCAN_PERIOD_MODE_P0     0x00
#define PAGE_SCAN_PERIOD_MODE_P1     0x01
#define PAGE_SCAN_PERIOD_MODE_P2     0x02

/* ACL */
#define HCI_PKT_TYPE_DM1      0x0008
#define HCI_PKT_TYPE_DH1      0x0010
#define HCI_PKT_TYPE_DM3      0x0400
#define HCI_PKT_TYPE_DH3      0x0800
#define HCI_PKT_TYPE_DM5      0x4000
#define HCI_PKT_TYPE_DH5      0x8000
#define HCI_PKT_TYPE_ACL      0xcc18
#define HCI_PKT_TYPE_ACL_LINK 0xee1c // after link setup set as 0xee1c;


/* link policy */
#define HCI_LP_DISABLE_ALL_MODE        0x00
#define HCI_LP_ENABLE_ROLE_SWITCH_MASK 0x01
#define HCI_LP_ENABLE_HOLD_MODE_MASK   0x02
#define HCI_LP_ENABLE_SNIFF_MODE_MASK  0x04
#define HCI_LP_ENABLE_PARK_MODE_MASK   0x08

#define HCI_CFG_SNIFF_MAX_INTERVAL     0x180 // 240ms
#define HCI_CFG_SNIFF_MIN_INTERVAL     0x170
#define HCI_CFG_SNIFF_ATTEMPT          0x02
#define HCI_CFG_SNIFF_TIMEOUT          0x00

#define ALLOW_ROLE_SWITCH_YES          0x01
#define ALLOW_ROLE_SWITCH_NO           0x00

#define BTH_ROLE_MASTER                0 // we set as master in all acl link
#define BTH_ROLE_SLAVE                 1 // we set as slave  in all acl link
#define BTH_ROLE_NOT_SET               2 // we not specified master or slave in any acl link

#define BTH_ROLE_CENTRAL               0
#define BTH_ROLE_PERIPHERAL            1

/// LM modes HCI:5.2.20
#define BTH_LM_ACTIVE_MODE 0x00
#define BTH_LM_HOLD_MODE   0x01
#define BTH_LM_SNIFF_MODE  0x02
#define BTH_LM_PARK_MODE   0x03

typedef enum
{
    BTH_HCI_TYPE_CMD = 0x01,
    BTH_HCI_TYPE_ACL_DATA,
    BTH_HCI_TYPE_SCO_DATA,
    BTH_HCI_TYPE_EVENT,
    BTH_HCI_TYPE_ISO_DATA,
} BTH_HCI_TYPE_ENUM;

typedef enum
{
    BTH_LP_MODE_BUSY = 0,
    BTH_LP_MODE_SNIFF,
    BTH_LP_MODE_EXIT,
} BTH_LP_MODE_ENUM;

typedef enum
{
    BTH_TPT_TWS_ROLE_IDLE = 0,
    BTH_TPT_TWS_ROLE_SINGLE,
    BTH_TPT_TWS_ROLE_MASTER,
    BTH_TPT_TWS_ROLE_ARBITER,
    BTH_TPT_TWS_ROLE_SLAVE,
    BTH_TPT_TWS_ROLE_OBSERVER,
} BTH_TWS_ROLE_ENUM;

typedef enum
{
    BTH_TPT_TWS_HANDOVER_STATUS_IDLE = 0,
    BTH_TPT_TWS_HANDOVER_STATUS_START,
    BTH_TPT_TWS_HANDOVER_STATUS_EXTRACER,
    BTH_TPT_TWS_HANDOVER_STATUS_HOSTSYNC,
    BTH_TPT_TWS_HANDOVER_STATUS_OVER,
    BTH_TPT_TWS_HANDOVER_STATUS_MAX,
} BTH_TWS_HANDOVER_STATUS;

typedef enum
{
    BTH_HCI_ERROR_NONE                          = 0x00,
    BTH_HCI_ERROR_UNKNOWN_HCI_COMMAND           = 0x01,
    BTH_HCI_ERROR_UNKNOWN_CONNECTION_ID         = 0x02,
    BTH_HCI_ERROR_HARDWARE_FAILURE              = 0x03,
    BTH_HCI_ERROR_PAGE_TIMEOUT                  = 0x04,
    BTH_HCI_ERROR_AUTH_FAILURE                  = 0x05,
    BTH_HCI_ERROR_PIN_OR_KEY_MISSING            = 0x06,
    BTH_HCI_ERROR_MEMORY_CAPA_EXCEED            = 0x07,
    BTH_HCI_ERROR_CONN_TIMEOUT                  = 0x08,
    BTH_HCI_ERROR_CONN_LIMIT_EXCEED             = 0x09,
    BTH_HCI_ERROR_SYNC_CON_LIMIT_DEV_EXCEED     = 0x0A,
    BTH_HCI_ERROR_CONN_ALREADY_EXISTS           = 0x0B,
    BTH_HCI_ERROR_COMMAND_DISALLOWED            = 0x0C,
    BTH_HCI_ERROR_CONN_REJ_LIMITED_RESOURCES    = 0x0D,
    BTH_HCI_ERROR_CONN_REJ_SECURITY_REASONS     = 0x0E,
    BTH_HCI_ERROR_CONN_REJ_UNACCEPTABLE_BDADDR  = 0x0F,
    BTH_HCI_ERROR_CONN_ACCEPT_TIMEOUT_EXCEED    = 0x10,
    BTH_HCI_ERROR_UNSUPPORTED                   = 0x11,
    BTH_HCI_ERROR_INVALID_HCI_PARAM             = 0x12,
    BTH_HCI_ERROR_REMOTE_USER_TERM_CON          = 0x13,
    BTH_HCI_ERROR_REMOTE_DEV_TERM_LOW_RESOURCES = 0x14,
    BTH_HCI_ERROR_REMOTE_DEV_POWER_OFF          = 0x15,
    BTH_HCI_ERROR_CON_TERM_BY_LOCAL_HOST        = 0x16,
    BTH_HCI_ERROR_REPEATED_ATTEMPTS             = 0x17,
    BTH_HCI_ERROR_PAIRING_NOT_ALLOWED           = 0x18,
    BTH_HCI_ERROR_UNKNOWN_LMP_PDU               = 0x19,
    BTH_HCI_ERROR_UNSUPPORTED_REMOTE_FEATURE    = 0x1A,
    BTH_HCI_ERROR_SCO_OFFSET_REJECTED           = 0x1B,
    BTH_HCI_ERROR_SCO_INTERVAL_REJECTED         = 0x1C,
    BTH_HCI_ERROR_SCO_AIR_MODE_REJECTED         = 0x1D,
    BTH_HCI_ERROR_INVALID_LMP_PARAM             = 0x1E,
    BTH_HCI_ERROR_UNSPECIFIED_ERROR             = 0x1F,
    BTH_HCI_ERROR_UNSUPPORTED_LMP_PARAM_VALUE   = 0x20,
    BTH_HCI_ERROR_ROLE_CHANGE_NOT_ALLOWED       = 0x21,
    BTH_HCI_ERROR_LMP_RSP_TIMEOUT               = 0x22,
    BTH_HCI_ERROR_LMP_COLLISION                 = 0x23,
    BTH_HCI_ERROR_LMP_PDU_NOT_ALLOWED           = 0x24,
    BTH_HCI_ERROR_ENC_MODE_NOT_ACCEPT           = 0x25,
    BTH_HCI_ERROR_LINK_KEY_CANT_CHANGE          = 0x26,
    BTH_HCI_ERROR_QOS_NOT_SUPPORTED             = 0x27,
    BTH_HCI_ERROR_INSTANT_PASSED                = 0x28,
    BTH_HCI_ERROR_PAIRING_WITH_UNIT_KEY_NOT_SUP = 0x29,
    BTH_HCI_ERROR_DIFF_TRANSACTION_COLLISION    = 0x2A,
    BTH_HCI_ERROR_QOS_UNACCEPTABLE_PARAM        = 0x2C,
    BTH_HCI_ERROR_QOS_REJECTED                  = 0x2D,
    BTH_HCI_ERROR_CHANNEL_CLASS_NOT_SUP         = 0x2E,
    BTH_HCI_ERROR_INSUFFICIENT_SECURITY         = 0x2F,
    BTH_HCI_ERROR_PARAM_OUT_OF_MAND_RANGE       = 0x30,
    BTH_HCI_ERROR_ROLE_SWITCH_PEND              = 0x32, /* LM_ROLE_SWITCH_PENDING               */
    BTH_HCI_ERROR_RESERVED_SLOT_VIOLATION       = 0x34, /* LM_RESERVED_SLOT_VIOLATION           */
    BTH_HCI_ERROR_ROLE_SWITCH_FAIL              = 0x35, /* LM_ROLE_SWITCH_FAILED                */
    BTH_HCI_ERROR_EIR_TOO_LARGE                 = 0x36, /* LM_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE */
    BTH_HCI_ERROR_SP_NOT_SUPPORTED_HOST         = 0x37,
    BTH_HCI_ERROR_HOST_BUSY_PAIRING             = 0x38,
    BTH_HCI_ERROR_CONTROLLER_BUSY               = 0x3A,
    BTH_HCI_ERROR_UNACCEPTABLE_CONN_PARAM       = 0x3B,
    BTH_HCI_ERROR_ADV_TO                        = 0x3C,
    BTH_HCI_ERROR_TERMINATED_MIC_FAILURE        = 0x3D,
    BTH_HCI_ERROR_CONN_FAILED_TO_BE_EST         = 0x3E,
    BTH_HCI_ERROR_CCA_REJ_USE_CLOCK_DRAG        = 0x40,
    BTH_HCI_ERROR_TYPE0_SUBMAP_NOT_DEFINED      = 0x41,
    BTH_HCI_ERROR_UNKNOWN_ADVERTISING_ID        = 0x42,
    BTH_HCI_ERROR_LIMIT_REACHED                 = 0x43,
    BTH_HCI_ERROR_OPERATION_CANCELED_BY_HOST    = 0x44,

    BTH_HCI_ERROR_TASK_ALREADY_EXIST = 0x80,
    BTH_HCI_ERROR_TASK_NOT_EXIST     = 0x81,

    BTH_HCI_ERROR_UNDEFINED = 0xFF,


    /*****************************************************
     ***              HW ERROR CODES                   ***
     *****************************************************/
    BTH_HCI_ERROR_HW_UART_OUT_OF_SYNC = 0x00,
    BTH_HCI_ERROR_HW_MEM_ALLOC_FAIL   = 0x01,
} BTH_HCI_ERROR_ENUM;


#endif // BTH_DEFINE_H
