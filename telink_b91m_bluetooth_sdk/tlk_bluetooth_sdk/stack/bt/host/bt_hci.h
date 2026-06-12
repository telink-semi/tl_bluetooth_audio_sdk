/********************************************************************************************************
 * @file    bt_hci.h
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


#ifndef BT_HCI_H_
#define BT_HCI_H_

#define TLKBT_HCI_H2C_ACL_SIZE 692 - 13 //679 // Data(2-DH5)=679


//If available buffer size is less than TLKBT_HCI_H2C_ACL_RESV_NUMB, it is not allowed to continue sending
#define TLKBT_HCI_H2C_ACL_RESV_NUMB 3

#define BT_HCI_EVT_HDR_LEN          2

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum
{
    BT_HCI_TYPE_CMD      = 0x01,
    BT_HCI_TYPE_ACL_DATA = 0x02,
    BT_HCI_TYPE_SCO_DATA = 0x03,
    BT_HCI_TYPE_EVENT    = 0x04,
    BT_HCI_TYPE_ISO_DATA = 0x05,
} TLKBT_HCI_TYPE_ENUM;

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum hci_type_e
{
    HCI_TYPE_CMD = 0x01,
    HCI_TYPE_ACL_DATA,
    HCI_TYPE_SCO_DATA,
    HCI_TYPE_EVENT,
    HCI_TYPE_ISO_DATA,
} hci_type_t;

typedef enum
{
    TLKBT_HCI_LLID_ISO      = 0, /// Reserved for future use
    TLKBT_HCI_LLID_CONTINUE = 1, /// Continue
    TLKBT_HCI_LLID_START    = 2, /// Start
    TLKBT_HCI_LLID_COMPLETE = 3, /// Control
} TLKBT_HCI_LLID_ENUM;

typedef int (*TlkBtHciEventCallback)(uint8_t evtID, uint8_t *pData, uint16_t dataLen);
typedef int (*TlkBtHciAclDataCallback)(uint16_t handle, uint8_t *pData, uint16_t dataLen);
typedef int (*TlkBtHciScoDataCallback)(uint16_t handle, uint8_t *pData, uint16_t dataLen);

typedef struct
{
    uint8_t  sco_pk_Len; //Max sco data length
    uint8_t  reserved;
    uint16_t acl_pk_Len;  //Max acl data length
    uint16_t acl_pk_numb; //Max acl data number
    uint16_t sco_pk_numb; //Max sco data number
} bt_hci_param_t;

typedef struct
{
    /// Status of the command reception
    uint8_t status;
    ///HCI version number
    uint8_t hci_ver;
    ///HCI revision number
    uint16_t hci_rev;
    ///LMP version
    uint8_t lmp_ver;
    ///manufacturer name
    uint16_t manuf_name;
    ///LMP Subversion
    uint16_t lmp_subver;
} bt_local_version_t;

typedef struct
{
    u32 tick; //tick of free running system timer

    u32 clkn; //28-bit BT Clock, CLKN counter counts between 0 and 2^28-1 with a 312.5us precision
    u32 fcnt; //Fine Counter counts between 624 and 0 and has a 0.5us precision, note that it counts DOWNWARD
} bt_rd_tcf_info_t;

void    tlkbt_hci_setAclBuffSize(uint16_t acl_pk_len, uint8_t sco_pk_len, uint16_t acl_num, uint16_t sco_num);
uint8_t tlkbt_hci_aclGetAvalSize(void);

void tlkbt_hci_regEventCB(TlkBtHciEventCallback cb);
void tlkbt_hci_regAclDataCB(TlkBtHciAclDataCallback cb);
void tlkbt_hci_regScoDataCB(TlkBtHciScoDataCallback cb);

int  tlkbt_hci_sendSyncData(uint16_t conn_handle, uint8_t *pData, uint16_t dataLen);
void tlkbt_hci_recvC2HData(uint8_t *pData, uint32_t dataLen);

uint32_t tlkbt_hci_h2cFifoSize(void);
uint32_t tlkbt_hci_h2cFifoUnused(void);

uint32_t tlkbt_hci_h2cDatFifoUsed(void);

int tlkbt_hci_sendH2cCmd(uint16_t opcode, uint8_t *pData, uint8_t dataLen);
int tlkbt_hci_sendH2cAclData(uint16_t aclHandle, uint8_t *pUsrExt, uint16_t extLen, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);

void tlkbt_hci_getLovalVersion(uint8_t *pdata, uint8_t dataLen);

void tlkbt_hci_getTcfInfo(uint8_t *pdata, uint8_t dataLen);
void tlksdk_host_get_clkn_fcnt_from_tick(bt_rd_tcf_info_t *p, uint32_t t);

typedef enum
{
    BTHCI_ECODE_NO_ERROR                       = 0x00,
    BTHCI_ECODE_UNKNOWN_HCI_COMMAND            = 0x01,
    BTHCI_ECODE_UNKNOWN_CONNECTION_ID          = 0x02,
    BTHCI_ECODE_HARDWARE_FAILURE               = 0x03,
    BTHCI_ECODE_PAGE_TIMEOUT                   = 0x04,
    BTHCI_ECODE_AUTH_FAILURE                   = 0x05,
    BTHCI_ECODE_PIN_OR_KEY_MISSING             = 0x06,
    BTHCI_ECODE_MEMORY_CAPA_EXCEED             = 0x07,
    BTHCI_ECODE_CONNECT_TIMEOUT                = 0x08,
    BTHCI_ECODE_CONN_LIMIT_EXCEED              = 0x09,
    BTHCI_ECODE_SYNC_CONN_LIMIT_DEV_EXCEED     = 0x0A,
    BTHCI_ECODE_CONN_ALREADY_EXISTS            = 0x0B,
    BTHCI_ECODE_COMMAND_DISALLOWED             = 0x0C,
    BTHCI_ECODE_CONN_REJ_LIMITED_RESOURCES     = 0x0D,
    BTHCI_ECODE_CONN_REJ_SECURITY_REASONS      = 0x0E,
    BTHCI_ECODE_CONN_REJ_UNACCEPTABLE_BDADDR   = 0x0F,
    BTHCI_ECODE_CONN_ACCEPT_TIMEOUT_EXCEED     = 0x10,
    BTHCI_ECODE_UNSUPPORTED                    = 0x11,
    BTHCI_ECODE_INVALID_HCI_PARAM              = 0x12,
    BTHCI_ECODE_REMOTE_USER_TERM_CONN          = 0x13,
    BTHCI_ECODE_REMOTE_DEV_TERM_LOW_RESOURCES  = 0x14,
    BTHCI_ECODE_REMOTE_DEV_POWER_OFF           = 0x15,
    BTHCI_ECODE_CONN_TERM_BY_LOCAL_HOST        = 0x16,
    BTHCI_ECODE_REPEATED_ATTEMPTS              = 0x17,
    BTHCI_ECODE_PAIRING_NOT_ALLOWED            = 0x18,
    BTHCI_ECODE_UNKNOWN_LMP_PDU                = 0x19,
    BTHCI_ECODE_UNSUPPORTED_REMOTE_FEATURE     = 0x1A,
    BTHCI_ECODE_SCO_OFFSET_REJECTED            = 0x1B,
    BTHCI_ECODE_SCO_INTERVAL_REJECTED          = 0x1C,
    BTHCI_ECODE_SCO_AIR_MODE_REJECTED          = 0x1D,
    BTHCI_ECODE_INVALID_LMP_PARAM              = 0x1E,
    BTHCI_ECODE_UNSPECIFIED_ERROR              = 0x1F,
    BTHCI_ECODE_UNSUPPORTED_LMP_PARAM_VALUE    = 0x20,
    BTHCI_ECODE_ROLE_CHANGE_NOT_ALLOWED        = 0x21,
    BTHCI_ECODE_LMP_RSP_TIMEOUT                = 0x22,
    BTHCI_ECODE_LMP_COLLISION                  = 0x23,
    BTHCI_ECODE_LMP_PDU_NOT_ALLOWED            = 0x24,
    BTHCI_ECODE_ENC_MODE_NOT_ACCEPT            = 0x25,
    BTHCI_ECODE_LINK_KEY_CANT_CHANGE           = 0x26,
    BTHCI_ECODE_QOS_NOT_SUPPORTED              = 0x27,
    BTHCI_ECODE_INSTANT_PASSED                 = 0x28,
    BTHCI_ECODE_PAIRING_WITH_UNIT_KEY_NOT_SUPP = 0x29,
    BTHCI_ECODE_DIFF_TRANSACTION_COLLISION     = 0x2A,
    BTHCI_ECODE_QOS_UNACCEPTABLE_PARAM         = 0x2C,
    BTHCI_ECODE_QOS_REJECTED                   = 0x2D,
    BTHCI_ECODE_CHANNEL_CLASS_NOT_SUPP         = 0x2E,
    BTHCI_ECODE_INSUFFICIENT_SECURITY          = 0x2F,
    BTHCI_ECODE_PARAM_OUT_OF_MAND_RANGE        = 0x30,
    BTHCI_ECODE_ROLE_SWITCH_PEND               = 0x32, /* LM_ROLE_SWITCH_PENDING               */
    BTHCI_ECODE_RESERVED_SLOT_VIOLATION        = 0x34, /* LM_RESERVED_SLOT_VIOLATION           */
    BTHCI_ECODE_ROLE_SWITCH_FAIL               = 0x35, /* LM_ROLE_SWITCH_FAILED                */
    BTHCI_ECODE_EIR_TOO_LARGE                  = 0x36, /* LM_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE */
    BTHCI_ECODE_SP_NOT_SUPPORTED_HOST          = 0x37,
    BTHCI_ECODE_HOST_BUSY_PAIRING              = 0x38,
    BTHCI_ECODE_CONTROLLER_BUSY                = 0x3A,
    BTHCI_ECODE_UNACCEPTABLE_CONN_PARAM        = 0x3B,
    BTHCI_ECODE_ADV_TIMEOUT                    = 0x3C,
    BTHCI_ECODE_TERMINATED_MIC_FAILURE         = 0x3D,
    BTHCI_ECODE_CONN_FAILED_TO_BE_EST          = 0x3E,
    BTHCI_ECODE_CCA_REJ_USE_CLOCK_DRAG         = 0x40,
    BTHCI_ECODE_TYPE0_SUBMAP_NOT_DEFINED       = 0x41,
    BTHCI_ECODE_UNKNOWN_ADVERTISING_ID         = 0x42,
    BTHCI_ECODE_LIMIT_REACHED                  = 0x43,
    BTHCI_ECODE_OPERATION_CANCELED_BY_HOST     = 0x44,

    BTHCI_ECODE_TASK_ALREADY_EXIST = 0x80,
    BTHCI_ECODE_TASK_NOT_EXIST     = 0x81,

    BTHCI_ECODE_UNDEFINED = 0xFF,


    /*****************************************************
     ***              HW ERROR CODES                   ***
     *****************************************************/

    BTHCI_ECODE_HW_UART_OUT_OF_SYNC = 0x00,
    BTHCI_ECODE_HW_MEM_ALLOC_FAIL   = 0x01,
} BT_HCI_ECODE_ENUM;

#endif // BT_HCI_H_
