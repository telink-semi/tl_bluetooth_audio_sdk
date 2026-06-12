/********************************************************************************************************
 * @file    co_hci.h
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


#ifndef CO_HCI_H_
#define CO_HCI_H_

/**
 ****************************************************************************************
 * @addtogroup COMMON Common SW Block
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h> // standard boolean definitions
#include <stddef.h>  // standard definitions
#include <stdint.h>  // standard integer definitions

#include "stack/bt/common/rwip_config.h"   // IP configuration
#include "stack/bt/common/co_bt_defines.h" // IP configuration

#include "common/compiler.h" // compiler definitions

/*
 * DEFINES
 ****************************************************************************************
 */


/******************************************************************************************/
/* -------------------------   H4TL DEFINITIONS Part IV.A    -----------------------------*/
/******************************************************************************************/

///HCI Transport Header length - change if different transport
#define HCI_TRANSPORT_HDR_LEN 0x01

///UART header: command message type
#define HCI_CMD_MSG_TYPE 0x01

///UART header: ACL data message type
#define HCI_ACL_MSG_TYPE 0x02

///UART header: Synchronous data message type
#define HCI_SYNC_MSG_TYPE 0x03

///UART header: event message type
#define HCI_EVT_MSG_TYPE 0x04

/******************************************************************************************/
/* -------------------------   HCI DEFINITIONS Part II.E     -----------------------------*/
/******************************************************************************************/
//HCI TYPE LEN
#define HCI_TYPE_LEN (0x01)

///HCI Command Opcode byte length
#define HCI_CMD_OPCODE_LEN (0x02)

///HCI Event code byte length
#define HCI_EVT_CODE_LEN (0x01)

///HCI Command/Event parameter length field byte length
#define HCI_CMDEVT_PARLEN_LEN (0x01)

///HCI Command header length
#define HCI_CMD_HDR_LEN (HCI_CMD_OPCODE_LEN + HCI_CMDEVT_PARLEN_LEN)

///HCI Event header length
#define HCI_EVT_HDR_LEN (HCI_EVT_CODE_LEN + HCI_CMDEVT_PARLEN_LEN)

/// HCI ACL header: handle and flags decoding
enum hci_acl_hdr_fields
{
    /// bits[00:11]: Connection handle
    HCI_ACL_HDR_HDL_LSB  = (0),
    HCI_ACL_HDR_HDL_MASK = (0x0FFF),
    /// bits[12:13]: Packet boundary flag
    HCI_ACL_HDR_PB_FLAG_LSB  = (12),
    HCI_ACL_HDR_PB_FLAG_MASK = (0x3000),
    /// bits[14:15]: Broadcast flag
    HCI_ACL_HDR_BC_FLAG_LSB  = (14),
    HCI_ACL_HDR_BC_FLAG_MASK = (0xC000),
    /// Packet boundary and Broadcast flags
    HCI_ACL_HDR_DATA_FLAGS_LSB  = (12),
    HCI_ACL_HDR_DATA_FLAGS_MASK = (0xF000),
};

#define HCI_ACL_HDR_HDL_FLAGS_POS (0)
#define HCI_ACL_HDR_HDL_FLAGS_LEN (2)
/// HCI ACL header: data length field length
#define HCI_ACL_HDR_DATA_LEN_POS (HCI_ACL_HDR_HDL_FLAGS_LEN)
#define HCI_ACL_HDR_DATA_LEN_LEN (2)

///HCI ACL data packet header length
#define HCI_ACL_HDR_LEN (HCI_ACL_HDR_HDL_FLAGS_LEN + HCI_ACL_HDR_DATA_LEN_LEN)

/// HCI Synchronous header: handle and flags decoding
enum hci_syn_hdr_fields
{
    /// bits[00:11]: Connection handle
    HCI_SYNC_HDR_HDL_LSB  = (0),
    HCI_SYNC_HDR_HDL_MASK = (0x0FFF),
    /// bits[12:13]: Packet status flag
    HCI_SYNC_HDR_PSF_LSB  = (12),
    HCI_SYNC_HDR_PSF_MASK = (0x3000),
    /// bits[14:15]: RFU
    HCI_SYNC_HDR_RFU_LSB  = (14),
    HCI_SYNC_HDR_RFU_MASK = (0xC000),
};

#define HCI_SYNC_HDR_HDL_FLAGS_POS (0)
#define HCI_SYNC_HDR_HDL_FLAGS_LEN (2)


/// HCI Synchronous header: data length field length
#define HCI_SYNC_HDR_DATA_LEN_POS (HCI_SYNC_HDR_HDL_FLAGS_LEN)
#define HCI_SYNC_HDR_DATA_LEN_LEN (1)
#define HCI_SYNC_MAX_DATA_SIZE    (255)

///HCI sync data packet header length
#define HCI_SYNC_HDR_LEN          (HCI_SYNC_HDR_HDL_FLAGS_LEN + HCI_SYNC_HDR_DATA_LEN_LEN)


#define HCI_SYNC_HDR_PSF_FLAG_POS (12)


///HCI Command Complete Event minimum parameter length: 1(nb_pk)+2(opcode)
#define HCI_CCEVT_HDR_PARLEN (0x03)

///HCI Command Complete Event header length:1(code)+1(len)+1(pk)+2(opcode)
#define HCI_CCEVT_HDR_LEN (HCI_EVT_HDR_LEN + HCI_CCEVT_HDR_PARLEN)

///HCI Basic Command Complete Event packet length
#define HCI_CCEVT_BASIC_LEN (HCI_CCEVT_HDR_LEN + 1)

///HCI Command Status Event parameter length - constant
#define HCI_CSEVT_PARLEN (0x04)

///HCI Command Status Event length:1(code)+1(len)+1(st)+1(pk)+2(opcode)
#define HCI_CSEVT_LEN (HCI_EVT_HDR_LEN + HCI_CSEVT_PARLEN)

///HCI Reset Command parameter length
#define HCI_RESET_CMD_PARLEN 0

/// Default return parameter length for HCI Command Complete Event
#define HCI_CCEVT_BASIC_RETPAR_LEN 1

/// Max HCI commands param size
#define HCI_MAX_CMD_PARAM_SIZE 255

/// Macro to extract OCF from OPCODE
#define HCI_OP2OCF(opcode) ((opcode) & 0x03FF)

/// Macro to extract OGF from OPCODE
#define HCI_OP2OGF(opcode) ((opcode) >> 10 & 0x003F)

/// Macro to create OPCODE from OGF and OCF
#define HCI_OPCODE(ocf, ogf) (((ogf) << 10) | ocf)

/// Maximum length of HCI advertising data fragments
#define HCI_ADV_DATA_FRAG_MAX_LEN 252

/**************************************************************************************
 **************                       HCI COMMANDS                     ****************
 **************************************************************************************/

///HCI enumeration of possible Command OGF values.
enum
{
    ///HCI Link Control Commands Group OGF code
    LK_CNTL_OGF = 0x01,
    ///HCI Link Policy Commands Group OGF code
    LK_POL_OGF,
    ///HCI Controller and Baseband Commands Group OGF code
    CNTLR_BB_OGF,
    ///HCI Information Parameters Commands Group OGF code
    INFO_PAR_OGF,
    ///HCI Status Commands Group OGF code
    STAT_PAR_OGF,
    ///HCI Test Commands Group OGF code
    TEST_OGF,
    ///HCI Low Energy Commands Group OGF code
    LE_CNTLR_OGF = 0x08,
    ///HCI Vendor Specific Group OGF code
    VS_OGF = 0x3F,
    MAX_OGF
};

///Commands Opcodes: OGF(6b) | OCF(10b)
/* Some Abbreviation used in names:
 *  - LK   = Link Key
 *  - RD   = Read
 *  - WR   = Write
 *  - REM  = Remote
 *  - STG  = Settings
 *  - CON  = Connection
 *  - CHG  = Change
 *  - DFT  = Default
 *  - PER  = Periodic
 */

///HCI enumeration of possible Command OP Codes.
/*@TRACE*/
enum hci_opcode
{
    HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    //Link Control Commands
    HCI_INQ_CMD_OPCODE                        = 0x0401,
    HCI_INQ_CANCEL_CMD_OPCODE                 = 0x0402,
    HCI_PER_INQ_MODE_CMD_OPCODE               = 0x0403,
    HCI_EXIT_PER_INQ_MODE_CMD_OPCODE          = 0x0404,
    HCI_CREATE_CON_CMD_OPCODE                 = 0x0405,
    HCI_DISCONNECT_CMD_OPCODE                 = 0x0406,
    HCI_CREATE_CON_CANCEL_CMD_OPCODE          = 0x0408,
    HCI_ACCEPT_CON_REQ_CMD_OPCODE             = 0x0409,
    HCI_REJECT_CON_REQ_CMD_OPCODE             = 0x040A,
    HCI_LK_REQ_REPLY_CMD_OPCODE               = 0x040B,
    HCI_LK_REQ_NEG_REPLY_CMD_OPCODE           = 0x040C,
    HCI_PIN_CODE_REQ_REPLY_CMD_OPCODE         = 0x040D,
    HCI_PIN_CODE_REQ_NEG_REPLY_CMD_OPCODE     = 0x040E,
    HCI_CHG_CON_PKT_TYPE_CMD_OPCODE           = 0x040F,
    HCI_AUTH_REQ_CMD_OPCODE                   = 0x0411,
    HCI_SET_CON_ENC_CMD_OPCODE                = 0x0413,
    HCI_CHG_CON_LK_CMD_OPCODE                 = 0x0415,
    HCI_MASTER_LK_CMD_OPCODE                  = 0x0417,
    HCI_REM_NAME_REQ_CMD_OPCODE               = 0x0419,
    HCI_REM_NAME_REQ_CANCEL_CMD_OPCODE        = 0x041A,
    HCI_RD_REM_SUPP_FEATS_CMD_OPCODE          = 0x041B,
    HCI_RD_REM_EXT_FEATS_CMD_OPCODE           = 0x041C,
    HCI_RD_REM_VER_INFO_CMD_OPCODE            = 0x041D,
    HCI_RD_CLK_OFF_CMD_OPCODE                 = 0x041F,
    HCI_RD_LMP_HDL_CMD_OPCODE                 = 0x0420,
    HCI_SETUP_SYNC_CON_CMD_OPCODE             = 0x0428,
    HCI_ACCEPT_SYNC_CON_REQ_CMD_OPCODE        = 0x0429,
    HCI_REJECT_SYNC_CON_REQ_CMD_OPCODE        = 0x042A,
    HCI_IO_CAP_REQ_REPLY_CMD_OPCODE           = 0x042B,
    HCI_USER_CFM_REQ_REPLY_CMD_OPCODE         = 0x042C,
    HCI_USER_CFM_REQ_NEG_REPLY_CMD_OPCODE     = 0x042D,
    HCI_USER_PASSKEY_REQ_REPLY_CMD_OPCODE     = 0x042E,
    HCI_USER_PASSKEY_REQ_NEG_REPLY_CMD_OPCODE = 0x042F,
    HCI_REM_OOB_DATA_REQ_REPLY_CMD_OPCODE     = 0x0430,
    HCI_REM_OOB_DATA_REQ_NEG_REPLY_CMD_OPCODE = 0x0433,
    HCI_IO_CAP_REQ_NEG_REPLY_CMD_OPCODE       = 0x0434,
    HCI_ENH_SETUP_SYNC_CON_CMD_OPCODE         = 0x043D,
    HCI_ENH_ACCEPT_SYNC_CON_CMD_OPCODE        = 0x043E,
    HCI_TRUNC_PAGE_CMD_OPCODE                 = 0x043F,
    HCI_TRUNC_PAGE_CAN_CMD_OPCODE             = 0x0440,
    HCI_SET_CON_SLV_BCST_CMD_OPCODE           = 0x0441,
    HCI_SET_CON_SLV_BCST_REC_CMD_OPCODE       = 0x0442,
    HCI_START_SYNC_TRAIN_CMD_OPCODE           = 0x0443,
    HCI_REC_SYNC_TRAIN_CMD_OPCODE             = 0x0444,
    HCI_REM_OOB_EXT_DATA_REQ_REPLY_CMD_OPCODE = 0x0445,

    //Link Policy Commands
    HCI_HOLD_MODE_CMD_OPCODE           = 0x0801,
    HCI_SNIFF_MODE_CMD_OPCODE          = 0x0803,
    HCI_EXIT_SNIFF_MODE_CMD_OPCODE     = 0x0804,
    HCI_PARK_STATE_CMD_OPCODE          = 0x0805,
    HCI_EXIT_PARK_STATE_CMD_OPCODE     = 0x0806,
    HCI_QOS_SETUP_CMD_OPCODE           = 0x0807,
    HCI_ROLE_DISCOVERY_CMD_OPCODE      = 0x0809,
    HCI_SWITCH_ROLE_CMD_OPCODE         = 0x080B,
    HCI_RD_LINK_POL_STG_CMD_OPCODE     = 0x080C,
    HCI_WR_LINK_POL_STG_CMD_OPCODE     = 0x080D,
    HCI_RD_DFT_LINK_POL_STG_CMD_OPCODE = 0x080E,
    HCI_WR_DFT_LINK_POL_STG_CMD_OPCODE = 0x080F,
    HCI_FLOW_SPEC_CMD_OPCODE           = 0x0810,
    HCI_SNIFF_SUB_CMD_OPCODE           = 0x0811,

    //Controller and Baseband Commands
    HCI_SET_EVT_MASK_CMD_OPCODE               = 0x0C01,
    HCI_RESET_CMD_OPCODE                      = 0x0C03,
    HCI_SET_EVT_FILTER_CMD_OPCODE             = 0x0C05,
    HCI_FLUSH_CMD_OPCODE                      = 0x0C08,
    HCI_RD_PIN_TYPE_CMD_OPCODE                = 0x0C09,
    HCI_WR_PIN_TYPE_CMD_OPCODE                = 0x0C0A,
    HCI_CREATE_NEW_UNIT_KEY_CMD_OPCODE        = 0x0C0B,
    HCI_RD_STORED_LK_CMD_OPCODE               = 0x0C0D,
    HCI_WR_STORED_LK_CMD_OPCODE               = 0x0C11,
    HCI_DEL_STORED_LK_CMD_OPCODE              = 0x0C12,
    HCI_WR_LOCAL_NAME_CMD_OPCODE              = 0x0C13,
    HCI_RD_LOCAL_NAME_CMD_OPCODE              = 0x0C14,
    HCI_RD_CON_ACCEPT_TO_CMD_OPCODE           = 0x0C15,
    HCI_WR_CON_ACCEPT_TO_CMD_OPCODE           = 0x0C16,
    HCI_RD_PAGE_TO_CMD_OPCODE                 = 0x0C17,
    HCI_WR_PAGE_TO_CMD_OPCODE                 = 0x0C18,
    HCI_RD_SCAN_EN_CMD_OPCODE                 = 0x0C19,
    HCI_WR_SCAN_EN_CMD_OPCODE                 = 0x0C1A,
    HCI_RD_PAGE_SCAN_ACT_CMD_OPCODE           = 0x0C1B,
    HCI_WR_PAGE_SCAN_ACT_CMD_OPCODE           = 0x0C1C,
    HCI_RD_INQ_SCAN_ACT_CMD_OPCODE            = 0x0C1D,
    HCI_WR_INQ_SCAN_ACT_CMD_OPCODE            = 0x0C1E,
    HCI_RD_AUTH_EN_CMD_OPCODE                 = 0x0C1F,
    HCI_WR_AUTH_EN_CMD_OPCODE                 = 0x0C20,
    HCI_RD_CLASS_OF_DEV_CMD_OPCODE            = 0x0C23,
    HCI_WR_CLASS_OF_DEV_CMD_OPCODE            = 0x0C24,
    HCI_RD_VOICE_STG_CMD_OPCODE               = 0x0C25,
    HCI_WR_VOICE_STG_CMD_OPCODE               = 0x0C26,
    HCI_RD_AUTO_FLUSH_TO_CMD_OPCODE           = 0x0C27,
    HCI_WR_AUTO_FLUSH_TO_CMD_OPCODE           = 0x0C28,
    HCI_RD_NB_BDCST_RETX_CMD_OPCODE           = 0x0C29,
    HCI_WR_NB_BDCST_RETX_CMD_OPCODE           = 0x0C2A,
    HCI_RD_HOLD_MODE_ACTIVITY_CMD_OPCODE      = 0x0C2B,
    HCI_WR_HOLD_MODE_ACTIVITY_CMD_OPCODE      = 0x0C2C,
    HCI_RD_TX_PWR_LVL_CMD_OPCODE              = 0x0C2D,
    HCI_RD_SYNC_FLOW_CTRL_EN_CMD_OPCODE       = 0x0C2E,
    HCI_WR_SYNC_FLOW_CTRL_EN_CMD_OPCODE       = 0x0C2F,
    HCI_SET_CTRL_TO_HOST_FLOW_CTRL_CMD_OPCODE = 0x0C31,
    HCI_HOST_BUF_SIZE_CMD_OPCODE              = 0x0C33,
    HCI_HOST_NB_CMP_PKTS_CMD_OPCODE           = 0x0C35,
    HCI_RD_LINK_SUPV_TO_CMD_OPCODE            = 0x0C36,
    HCI_WR_LINK_SUPV_TO_CMD_OPCODE            = 0x0C37,
    HCI_RD_NB_SUPP_IAC_CMD_OPCODE             = 0x0C38,
    HCI_RD_CURR_IAC_LAP_CMD_OPCODE            = 0x0C39,
    HCI_WR_CURR_IAC_LAP_CMD_OPCODE            = 0x0C3A,
    HCI_SET_AFH_HOST_CH_CLASS_CMD_OPCODE      = 0x0C3F,
    HCI_RD_INQ_SCAN_TYPE_CMD_OPCODE           = 0x0C42,
    HCI_WR_INQ_SCAN_TYPE_CMD_OPCODE           = 0x0C43,
    HCI_RD_INQ_MODE_CMD_OPCODE                = 0x0C44,
    HCI_WR_INQ_MODE_CMD_OPCODE                = 0x0C45,
    HCI_RD_PAGE_SCAN_TYPE_CMD_OPCODE          = 0x0C46,
    HCI_WR_PAGE_SCAN_TYPE_CMD_OPCODE          = 0x0C47,
    HCI_RD_AFH_CH_ASSESS_MODE_CMD_OPCODE      = 0x0C48,
    HCI_WR_AFH_CH_ASSESS_MODE_CMD_OPCODE      = 0x0C49,
    HCI_RD_EXT_INQ_RSP_CMD_OPCODE             = 0x0C51,
    HCI_WR_EXT_INQ_RSP_CMD_OPCODE             = 0x0C52,
    HCI_REFRESH_ENC_KEY_CMD_OPCODE            = 0x0C53,
    HCI_RD_SP_MODE_CMD_OPCODE                 = 0x0C55,
    HCI_WR_SP_MODE_CMD_OPCODE                 = 0x0C56,
    HCI_RD_LOC_OOB_DATA_CMD_OPCODE            = 0x0C57,
    HCI_RD_INQ_RSP_TX_PWR_LVL_CMD_OPCODE      = 0x0C58,
    HCI_WR_INQ_TX_PWR_LVL_CMD_OPCODE          = 0x0C59,
    HCI_RD_DFT_ERR_DATA_REP_CMD_OPCODE        = 0x0C5A,
    HCI_WR_DFT_ERR_DATA_REP_CMD_OPCODE        = 0x0C5B,
    HCI_ENH_FLUSH_CMD_OPCODE                  = 0x0C5F,
    HCI_SEND_KEYPRESS_NOTIF_CMD_OPCODE        = 0x0C60,
    HCI_SET_EVT_MASK_PAGE_2_CMD_OPCODE        = 0x0C63,
    HCI_RD_FLOW_CNTL_MODE_CMD_OPCODE          = 0x0C66,
    HCI_WR_FLOW_CNTL_MODE_CMD_OPCODE          = 0x0C67,
    HCI_RD_ENH_TX_PWR_LVL_CMD_OPCODE          = 0x0C68,
    HCI_RD_LE_HOST_SUPP_CMD_OPCODE            = 0x0C6C,
    HCI_WR_LE_HOST_SUPP_CMD_OPCODE            = 0x0C6D,
    HCI_SET_MWS_CHANNEL_PARAMS_CMD_OPCODE     = 0x0C6E,
    HCI_SET_EXTERNAL_FRAME_CONFIG_CMD_OPCODE  = 0x0C6F,
    HCI_SET_MWS_SIGNALING_CMD_OPCODE          = 0x0C70,
    HCI_SET_MWS_TRANSPORT_LAYER_CMD_OPCODE    = 0x0C71,
    HCI_SET_MWS_SCAN_FREQ_TABLE_CMD_OPCODE    = 0x0C72,
    HCI_SET_MWS_PATTERN_CONFIG_CMD_OPCODE     = 0x0C73,
    HCI_SET_RES_LT_ADDR_CMD_OPCODE            = 0x0C74,
    HCI_DEL_RES_LT_ADDR_CMD_OPCODE            = 0x0C75,
    HCI_SET_CON_SLV_BCST_DATA_CMD_OPCODE      = 0x0C76,
    HCI_RD_SYNC_TRAIN_PARAM_CMD_OPCODE        = 0x0C77,
    HCI_WR_SYNC_TRAIN_PARAM_CMD_OPCODE        = 0x0C78,
    HCI_RD_SEC_CON_HOST_SUPP_CMD_OPCODE       = 0x0C79,
    HCI_WR_SEC_CON_HOST_SUPP_CMD_OPCODE       = 0x0C7A,
    HCI_RD_AUTH_PAYL_TO_CMD_OPCODE            = 0x0C7B,
    HCI_WR_AUTH_PAYL_TO_CMD_OPCODE            = 0x0C7C,
    HCI_RD_LOC_OOB_EXT_DATA_CMD_OPCODE        = 0x0C7D,
    HCI_RD_EXT_PAGE_TO_CMD_OPCODE             = 0x0C7E,
    HCI_WR_EXT_PAGE_TO_CMD_OPCODE             = 0x0C7F,
    HCI_RD_EXT_INQ_LEN_CMD_OPCODE             = 0x0C80,
    HCI_WR_EXT_INQ_LEN_CMD_OPCODE             = 0x0C81,
    HCI_SET_ECOSYS_BASE_CMD_OPCODE            = 0x0C82,
    HCI_CFG_DATA_PATH_CMD_OPCODE              = 0x0C83,
    HCI_SET_ENC_KEY_SIZE_CMD_OPCODE           = 0x0C84,

    //Info Params
    HCI_RD_LOCAL_VER_INFO_CMD_OPCODE       = 0x1001,
    HCI_RD_LOCAL_SUPP_CMDS_CMD_OPCODE      = 0x1002,
    HCI_RD_LOCAL_SUPP_FEATS_CMD_OPCODE     = 0x1003,
    HCI_RD_LOCAL_EXT_FEATS_CMD_OPCODE      = 0x1004,
    HCI_RD_BUFF_SIZE_CMD_OPCODE            = 0x1005,
    HCI_RD_BD_ADDR_CMD_OPCODE              = 0x1009,
    HCI_RD_LOCAL_SUPP_CODECS_CMD_OPCODE    = 0x100B,
    HCI_RD_LOCAL_SP_OPT_CMD_OPCODE         = 0x100C,
    HCI_RD_LOCAL_SUPP_CODECS_V2_CMD_OPCODE = 0x100D,
    HCI_RD_LOCAL_SUPP_CODEC_CAP_CMD_OPCODE = 0x100E,
    HCI_RD_LOCAL_SUPP_CON_DELAY_CMD_OPCODE = 0x100F,

    //Status Params
    HCI_RD_FAIL_CONTACT_CNT_CMD_OPCODE            = 0x1401,
    HCI_RST_FAIL_CONTACT_CNT_CMD_OPCODE           = 0x1402,
    HCI_RD_LINK_QUAL_CMD_OPCODE                   = 0x1403,
    HCI_RD_RSSI_CMD_OPCODE                        = 0x1405,
    HCI_RD_AFH_CH_MAP_CMD_OPCODE                  = 0x1406,
    HCI_RD_CLK_CMD_OPCODE                         = 0x1407,
    HCI_RD_ENC_KEY_SIZE_CMD_OPCODE                = 0x1408,
    HCI_GET_MWS_TRANSPORT_LAYER_CONFIG_CMD_OPCODE = 0x140C,

    //Testing Commands
    HCI_RD_LOOPBACK_MODE_CMD_OPCODE     = 0x1801,
    HCI_WR_LOOPBACK_MODE_CMD_OPCODE     = 0x1802,
    HCI_EN_DUT_MODE_CMD_OPCODE          = 0x1803,
    HCI_WR_SP_DBG_MODE_CMD_OPCODE       = 0x1804,
    HCI_WR_SEC_CON_TEST_MODE_CMD_OPCODE = 0x180A,

    /// LE Commands Opcodes
    HCI_LE_SET_EVT_MASK_CMD_OPCODE                = 0x2001,
    HCI_LE_RD_BUFF_SIZE_CMD_OPCODE                = 0x2002,
    HCI_LE_RD_LOCAL_SUPP_FEATS_CMD_OPCODE         = 0x2003,
    HCI_LE_SET_RAND_ADDR_CMD_OPCODE               = 0x2005,
    HCI_LE_SET_ADV_PARAM_CMD_OPCODE               = 0x2006,
    HCI_LE_RD_ADV_CHNL_TX_PW_CMD_OPCODE           = 0x2007,
    HCI_LE_SET_ADV_DATA_CMD_OPCODE                = 0x2008,
    HCI_LE_SET_SCAN_RSP_DATA_CMD_OPCODE           = 0x2009,
    HCI_LE_SET_ADV_EN_CMD_OPCODE                  = 0x200A,
    HCI_LE_SET_SCAN_PARAM_CMD_OPCODE              = 0x200B,
    HCI_LE_SET_SCAN_EN_CMD_OPCODE                 = 0x200C,
    HCI_LE_CREATE_CON_CMD_OPCODE                  = 0x200D,
    HCI_LE_CREATE_CON_CANCEL_CMD_OPCODE           = 0x200E,
    HCI_LE_RD_WLST_SIZE_CMD_OPCODE                = 0x200F,
    HCI_LE_CLEAR_WLST_CMD_OPCODE                  = 0x2010,
    HCI_LE_ADD_DEV_TO_WLST_CMD_OPCODE             = 0x2011,
    HCI_LE_RMV_DEV_FROM_WLST_CMD_OPCODE           = 0x2012,
    HCI_LE_CON_UPDATE_CMD_OPCODE                  = 0x2013,
    HCI_LE_SET_HOST_CH_CLASS_CMD_OPCODE           = 0x2014,
    HCI_LE_RD_CHNL_MAP_CMD_OPCODE                 = 0x2015,
    HCI_LE_RD_REM_FEATS_CMD_OPCODE                = 0x2016,
    HCI_LE_ENC_CMD_OPCODE                         = 0x2017,
    HCI_LE_RAND_CMD_OPCODE                        = 0x2018,
    HCI_LE_START_ENC_CMD_OPCODE                   = 0x2019,
    HCI_LE_LTK_REQ_REPLY_CMD_OPCODE               = 0x201A,
    HCI_LE_LTK_REQ_NEG_REPLY_CMD_OPCODE           = 0x201B,
    HCI_LE_RD_SUPP_STATES_CMD_OPCODE              = 0x201C,
    HCI_LE_RX_TEST_CMD_OPCODE                     = 0x201D,
    HCI_LE_TX_TEST_CMD_OPCODE                     = 0x201E,
    HCI_LE_TEST_END_CMD_OPCODE                    = 0x201F,
    HCI_LE_REM_CON_PARAM_REQ_REPLY_CMD_OPCODE     = 0x2020,
    HCI_LE_REM_CON_PARAM_REQ_NEG_REPLY_CMD_OPCODE = 0x2021,
    HCI_LE_SET_DATA_LEN_CMD_OPCODE                = 0x2022,
    HCI_LE_RD_SUGGTED_DFT_DATA_LEN_CMD_OPCODE     = 0x2023,
    HCI_LE_WR_SUGGTED_DFT_DATA_LEN_CMD_OPCODE     = 0x2024,
    HCI_LE_RD_LOC_P256_PUB_KEY_CMD_OPCODE         = 0x2025,
    HCI_LE_GEN_DHKEY_CMD_OPCODE                   = 0x2026,
    HCI_LE_ADD_DEV_TO_RSLV_LIST_CMD_OPCODE        = 0x2027,
    HCI_LE_RMV_DEV_FROM_RSLV_LIST_CMD_OPCODE      = 0x2028,
    HCI_LE_CLEAR_RSLV_LIST_CMD_OPCODE             = 0x2029,
    HCI_LE_RD_RSLV_LIST_SIZE_CMD_OPCODE           = 0x202A,
    HCI_LE_RD_PEER_RSLV_ADDR_CMD_OPCODE           = 0x202B,
    HCI_LE_RD_LOC_RSLV_ADDR_CMD_OPCODE            = 0x202C,
    HCI_LE_SET_ADDR_RESOL_EN_CMD_OPCODE           = 0x202D,
    HCI_LE_SET_RSLV_PRIV_ADDR_TO_CMD_OPCODE       = 0x202E,
    HCI_LE_RD_MAX_DATA_LEN_CMD_OPCODE             = 0x202F,
    HCI_LE_RD_PHY_CMD_OPCODE                      = 0x2030,
    HCI_LE_SET_DFT_PHY_CMD_OPCODE                 = 0x2031,
    HCI_LE_SET_PHY_CMD_OPCODE                     = 0x2032,
    HCI_LE_ENH_RX_TEST_CMD_OPCODE                 = 0x2033,
    HCI_LE_ENH_TX_TEST_CMD_OPCODE                 = 0x2034,
    HCI_LE_SET_ADV_SET_RAND_ADDR_CMD_OPCODE       = 0x2035,
    HCI_LE_SET_EXT_ADV_PARAM_CMD_OPCODE           = 0x2036,
    HCI_LE_SET_EXT_ADV_DATA_CMD_OPCODE            = 0x2037,
    HCI_LE_SET_EXT_SCAN_RSP_DATA_CMD_OPCODE       = 0x2038,
    HCI_LE_SET_EXT_ADV_EN_CMD_OPCODE              = 0x2039,
    HCI_LE_RD_MAX_ADV_DATA_LEN_CMD_OPCODE         = 0x203A,
    HCI_LE_RD_NB_SUPP_ADV_SETS_CMD_OPCODE         = 0x203B,
    HCI_LE_RMV_ADV_SET_CMD_OPCODE                 = 0x203C,
    HCI_LE_CLEAR_ADV_SETS_CMD_OPCODE              = 0x203D,
    HCI_LE_SET_PER_ADV_PARAM_CMD_OPCODE           = 0x203E,
    HCI_LE_SET_PER_ADV_DATA_CMD_OPCODE            = 0x203F,
    HCI_LE_SET_PER_ADV_EN_CMD_OPCODE              = 0x2040,
    HCI_LE_SET_EXT_SCAN_PARAM_CMD_OPCODE          = 0x2041,
    HCI_LE_SET_EXT_SCAN_EN_CMD_OPCODE             = 0x2042,
    HCI_LE_EXT_CREATE_CON_CMD_OPCODE              = 0x2043,
    HCI_LE_PER_ADV_CREATE_SYNC_CMD_OPCODE         = 0x2044,
    HCI_LE_PER_ADV_CREATE_SYNC_CANCEL_CMD_OPCODE  = 0x2045,
    HCI_LE_PER_ADV_TERM_SYNC_CMD_OPCODE           = 0x2046,
    HCI_LE_ADD_DEV_TO_PER_ADV_LIST_CMD_OPCODE     = 0x2047,
    HCI_LE_RMV_DEV_FROM_PER_ADV_LIST_CMD_OPCODE   = 0x2048,
    HCI_LE_CLEAR_PER_ADV_LIST_CMD_OPCODE          = 0x2049,
    HCI_LE_RD_PER_ADV_LIST_SIZE_CMD_OPCODE        = 0x204A,
    HCI_LE_RD_TX_PWR_CMD_OPCODE                   = 0x204B,
    HCI_LE_RD_RF_PATH_COMP_CMD_OPCODE             = 0x204C,
    HCI_LE_WR_RF_PATH_COMP_CMD_OPCODE             = 0x204D,
    HCI_LE_SET_PRIV_MODE_CMD_OPCODE               = 0x204E,

    ///Debug commands - OGF = 0x3F (spec)
    /// 	///VS commands - OGF = 0x3F (spec)
    HCI_DBG_SET_BT_BD_ADDR_CMD_OPCODE = 0xFC01,
    HCI_DBG_ESCO_MUTE_CMD_OPCODE      = 0xFC02,
    HCI_DBG_ACL_LINK_PRIO_OPCODE      = 0xFC03,
    HCI_SET_MAX_SLOTS_CMD_OPCODE      = 0xFC04,
    //    HCI_DBG_RD_MEM_CMD_OPCODE                      = 0xFC01,
    //    HCI_DBG_WR_MEM_CMD_OPCODE                      = 0xFC02,
    //    HCI_DBG_DEL_PAR_CMD_OPCODE                     = 0xFC03,
    HCI_DBG_ID_FLASH_CMD_OPCODE              = 0xFC05,
    HCI_DBG_ER_FLASH_CMD_OPCODE              = 0xFC06,
    HCI_DBG_WR_FLASH_CMD_OPCODE              = 0xFC07,
    HCI_DBG_RD_FLASH_CMD_OPCODE              = 0xFC08,
    HCI_DBG_RD_PAR_CMD_OPCODE                = 0xFC09,
    HCI_DBG_WR_PAR_CMD_OPCODE                = 0xFC0A,
    HCI_DBG_WLAN_COEX_CMD_OPCODE             = 0xFC0B,
    HCI_DBG_WLAN_COEXTST_SCEN_CMD_OPCODE     = 0xFC0D,
    HCI_DBG_BT_SEND_LMP_CMD_OPCODE           = 0xFC0E,
    HCI_DBG_SET_LOCAL_CLOCK_CMD_OPCODE       = 0xFC0F,
    HCI_DBG_RD_KE_STATS_CMD_OPCODE           = 0xFC10,
    HCI_DBG_PLF_RESET_CMD_OPCODE             = 0xFC11,
    HCI_DBG_RD_MEM_INFO_CMD_OPCODE           = 0xFC12,
    HCI_VS_SET_PREF_SLAVE_LATENCY_CMD_OPCODE = 0xFC13,
    HCI_VS_SET_PREF_SLAVE_EVT_DUR_CMD_OPCODE = 0xFC14,
    HCI_SET_MORE_DATA_MODE_CMD_OPCODE        = 0xFC15,
    HCI_SET_LINK_MAX_NB_CMD_OPCODE           = 0xFC16,
    HCI_SET_LOCAL_VER_INFO_CMD_OPCODE        = 0xFC17,
    HCI_SET_INC_PEER_PWR_MAX_CMD_OPCODE      = 0xFC18,
    HCI_RD_TCF_INFO_CMD_OPCODE               = 0xFC19,

    HCI_DBG_BLE_REG_RD_CMD_OPCODE     = 0xFC30,
    HCI_DBG_BLE_REG_WR_CMD_OPCODE     = 0xFC31,
    HCI_DBG_SEND_LLCP_CMD_OPCODE      = 0xFC35,
    HCI_DBG_LLCP_DISCARD_CMD_OPCODE   = 0xFC36,
    HCI_DBG_RF_REG_RD_CMD_OPCODE      = 0xFC39,
    HCI_DBG_RF_REG_WR_CMD_OPCODE      = 0xFC3A,
    HCI_DBG_RF_SWITCH_CLK_CMD_OPCODE  = 0xFC3C,
    HCI_DBG_RF_WR_DATA_TX_CMD_OPCODE  = 0xFC3D,
    HCI_DBG_RF_RD_DATA_RX_CMD_OPCODE  = 0xFC3E,
    HCI_DBG_RF_CNTL_TX_CMD_OPCODE     = 0xFC3F,
    HCI_DBG_RF_SYNC_P_CNTL_CMD_OPCODE = 0xFC40,
    HCI_SET_LE_FEAT_SUPP_CMD_OPCODE   = 0xFC41,


#if (RW_DEBUG && BT_EMB_PRESENT)
    HCI_DBG_BT_DISCARD_LMP_EN_CMD_OPCODE = 0xFC44,
#endif //RW_DEBUG && BT_EMB_PRESENT

    HCI_DBG_MWS_COEX_CMD_OPCODE         = 0xFC45,
    HCI_DBG_MWS_COEXTST_SCEN_CMD_OPCODE = 0xFC46,

#if (BLE_ISO_MODE_0)
    /// Vendor Specific commands for ISO Mode 0
    HCI_VS_SETUP_AM0_CHAN_CMD_OPCODE   = 0xFC54,
    HCI_VS_REMOVE_AM0_CHAN_CMD_OPCODE  = 0xFC55,
    HCI_VS_CONTROL_AM0_CHAN_CMD_OPCODE = 0xFC56,
#endif // (BLE_ISO_MODE_0)

#if CRYPTO_UT
    HCI_DBG_TEST_CRYPTO_FUNC_CMD_OPCODE = 0xFC60,
#endif //CRYPTO_UT

#if RW_DEBUG
    HCI_DBG_TEST_SCH_PLAN_SET_CMD_OPCODE = 0xFC61,
    HCI_DBG_TEST_SCH_PLAN_REM_CMD_OPCODE = 0xFC62,
    HCI_DBG_TEST_SCH_PLAN_CHK_CMD_OPCODE = 0xFC63,
    HCI_DBG_TEST_SCH_PLAN_REQ_CMD_OPCODE = 0xFC64,
#endif //RW_DEBUG
};

/**************************************************************************************
 **************                        HCI EVENTS                      ****************
 **************************************************************************************/

///Event Codes
/*@TRACE*/
enum hci_evt_code
{
    HCI_INQ_CMP_EVT_CODE                   = 0x01,
    HCI_INQ_RES_EVT_CODE                   = 0x02,
    HCI_CON_CMP_EVT_CODE                   = 0x03,
    HCI_CON_REQ_EVT_CODE                   = 0x04,
    HCI_DISC_CMP_EVT_CODE                  = 0x05,
    HCI_AUTH_CMP_EVT_CODE                  = 0x06,
    HCI_REM_NAME_REQ_CMP_EVT_CODE          = 0x07,
    HCI_ENC_CHG_EVT_CODE                   = 0x08,
    HCI_CHG_CON_LK_CMP_EVT_CODE            = 0x09,
    HCI_MASTER_LK_CMP_EVT_CODE             = 0x0A,
    HCI_RD_REM_SUPP_FEATS_CMP_EVT_CODE     = 0x0B,
    HCI_RD_REM_VER_INFO_CMP_EVT_CODE       = 0x0C,
    HCI_QOS_SETUP_CMP_EVT_CODE             = 0x0D,
    HCI_CMD_CMP_EVT_CODE                   = 0x0E,
    HCI_CMD_STATUS_EVT_CODE                = 0x0F,
    HCI_HW_ERR_EVT_CODE                    = 0x10,
    HCI_FLUSH_OCCURRED_EVT_CODE            = 0x11,
    HCI_ROLE_CHG_EVT_CODE                  = 0x12,
    HCI_NB_CMP_PKTS_EVT_CODE               = 0x13,
    HCI_MODE_CHG_EVT_CODE                  = 0x14,
    HCI_RETURN_LINK_KEYS_EVT_CODE          = 0x15,
    HCI_PIN_CODE_REQ_EVT_CODE              = 0x16,
    HCI_LK_REQ_EVT_CODE                    = 0x17,
    HCI_LK_NOTIF_EVT_CODE                  = 0x18,
    HCI_DATA_BUF_OVFLW_EVT_CODE            = 0x1A,
    HCI_MAX_SLOT_CHG_EVT_CODE              = 0x1B,
    HCI_RD_CLK_OFF_CMP_EVT_CODE            = 0x1C,
    HCI_CON_PKT_TYPE_CHG_EVT_CODE          = 0x1D,
    HCI_QOS_VIOL_EVT_CODE                  = 0x1E,
    HCI_PAGE_SCAN_REPEAT_MODE_CHG_EVT_CODE = 0x20,
    HCI_FLOW_SPEC_CMP_EVT_CODE             = 0x21,
    HCI_INQ_RES_WITH_RSSI_EVT_CODE         = 0x22,
    HCI_RD_REM_EXT_FEATS_CMP_EVT_CODE      = 0x23,
    HCI_SYNC_CON_CMP_EVT_CODE              = 0x2C,
    HCI_SYNC_CON_CHG_EVT_CODE              = 0x2D,
    HCI_SNIFF_SUB_EVT_CODE                 = 0x2E,
    HCI_EXT_INQ_RES_EVT_CODE               = 0x2F,
    HCI_ENC_KEY_REFRESH_CMP_EVT_CODE       = 0x30,
    HCI_IO_CAP_REQ_EVT_CODE                = 0x31,
    HCI_IO_CAP_RSP_EVT_CODE                = 0x32,
    HCI_USER_CFM_REQ_EVT_CODE              = 0x33,
    HCI_USER_PASSKEY_REQ_EVT_CODE          = 0x34,
    HCI_REM_OOB_DATA_REQ_EVT_CODE          = 0x35,
    HCI_SP_CMP_EVT_CODE                    = 0x36,
    HCI_LINK_SUPV_TO_CHG_EVT_CODE          = 0x38,
    HCI_ENH_FLUSH_CMP_EVT_CODE             = 0x39,
    HCI_USER_PASSKEY_NOTIF_EVT_CODE        = 0x3B,
    HCI_KEYPRESS_NOTIF_EVT_CODE            = 0x3C,
    HCI_REM_HOST_SUPP_FEATS_NOTIF_EVT_CODE = 0x3D,
    HCI_LE_META_EVT_CODE                   = 0x3E,
    HCI_MAX_EVT_MSK_PAGE_1_CODE            = 0x40,
    HCI_SYNC_TRAIN_CMP_EVT_CODE            = 0x4F,
    HCI_SYNC_TRAIN_REC_EVT_CODE            = 0x50,
    HCI_CON_SLV_BCST_REC_EVT_CODE          = 0x51,
    HCI_CON_SLV_BCST_TO_EVT_CODE           = 0x52,
    HCI_TRUNC_PAGE_CMP_EVT_CODE            = 0x53,
    HCI_SLV_PAGE_RSP_TO_EVT_CODE           = 0x54,
    HCI_CON_SLV_BCST_CH_MAP_CHG_EVT_CODE   = 0x55,
    HCI_AUTH_PAYL_TO_EXP_EVT_CODE          = 0x57,
    HCI_SAM_STATUS_CHANGE_EVT_CODE         = 0x58,
    HCI_MAX_EVT_MSK_PAGE_2_CODE            = 0x59,
    HCI_DBG_META_EVT_CODE                  = 0xFF,
};

/*@TRACE*/
enum hci_le_evt_subcode
{
    /// LE Events Subcodes
    HCI_LE_CON_CMP_EVT_SUBCODE                 = 0x01,
    HCI_LE_ADV_REPORT_EVT_SUBCODE              = 0x02,
    HCI_LE_CON_UPDATE_CMP_EVT_SUBCODE          = 0x03,
    HCI_LE_RD_REM_FEATS_CMP_EVT_SUBCODE        = 0x04,
    HCI_LE_LTK_REQUEST_EVT_SUBCODE             = 0x05,
    HCI_LE_REM_CON_PARAM_REQ_EVT_SUBCODE       = 0x06,
    HCI_LE_DATA_LEN_CHG_EVT_SUBCODE            = 0x07,
    HCI_LE_RD_LOC_P256_PUB_KEY_CMP_EVT_SUBCODE = 0x08,
    HCI_LE_GEN_DHKEY_CMP_EVT_SUBCODE           = 0x09,
    HCI_LE_ENH_CON_CMP_EVT_SUBCODE             = 0x0A,
    HCI_LE_DIR_ADV_REP_EVT_SUBCODE             = 0x0B,
    HCI_LE_PHY_UPD_CMP_EVT_SUBCODE             = 0x0C,
    HCI_LE_EXT_ADV_REPORT_EVT_SUBCODE          = 0x0D,
    HCI_LE_PER_ADV_SYNC_EST_EVT_SUBCODE        = 0x0E,
    HCI_LE_PER_ADV_REPORT_EVT_SUBCODE          = 0x0F,
    HCI_LE_PER_ADV_SYNC_LOST_EVT_SUBCODE       = 0x10,
    HCI_LE_SCAN_TIMEOUT_EVT_SUBCODE            = 0x11,
    HCI_LE_ADV_SET_TERMINATED_EVT_SUBCODE      = 0x12,
    HCI_LE_SCAN_REQ_RCVD_EVT_SUBCODE           = 0x13,
    HCI_LE_CH_SEL_ALGO_EVT_SUBCODE             = 0x14,
};

#if (RW_DEBUG || BLE_ISOGEN)
/*@TRACE*/
enum hci_vs_evt_subcode
{
#if (RW_DEBUG)
    /// DBG Events Subcodes
    HCI_DBG_ASSERT_EVT_SUBCODE = 0x02,
#endif //(RW_DEBUG)

#if (BLE_ISOGEN)
    /// VS ISO Gen Statistics Status
    HCI_VS_ISOGEN_STAT_EVT_SUBCODE = 0x03,
#endif // (BLE_ISOGEN)
};
#endif //(RW_DEBUG || BLE_ISOGEN)

/// Event mask page enum
enum hci_evt_mask_page
{
    /// page 0
    HCI_PAGE_0,
    /// page 1
    HCI_PAGE_1,
    /// page 2
    HCI_PAGE_2,
    /// Default
    HCI_PAGE_DFT,
    /// LE event
    HCI_PAGE_LE,
};

#if (BLE_ISO_PRESENT)
#if (BLE_ISO_MODE_0)
/// Current audio mode
enum iso_am0_ctrl
{
    // Stop Audio Mode 0 Stream
    ISO_AM0_CRL_DISABLE,
    // Start Audio Mode 0 Stream
    ISO_AM0_CRL_ENABLE,
};
#endif // (BLE_ISO_MODE_0)


/// Isochronous Channel Direction selection
enum iso_rx_tx_select
{
    /// Isochronous tx buffer selection: Host to Controller
    ISO_TX_SEL,
    /// Isochronous rx buffer selection: Controller to Host
    ISO_RX_SEL
};

/// Isochronous Channel data path selection
enum iso_dp_type
{
    /// Data Path direction is disabled
    ISO_DP_DISABLE = 0x00,
    /// Voice over HCI Data Path
    ISO_DP_VOHCI = 0x01,

    // -------- VENDOR SPECIFIC --------- //

    /// ISO over HCI Data Path
    ISO_DP_ISOOHCI = 0xF0,
    /// PCM Data path
    ISO_DP_PCM = 0xF1,
    /// ISO Payload Generator
    ISO_DP_ISOGEN = 0xF2,
};

#endif // (BLE_ISO_PRESENT)


/**************************************************************************************
 **************                 HCI MESSAGE STRUCTURES                 ****************
 **************************************************************************************/

/// HCI ACL data packet structure
/*@TRACE*/
struct hci_acl_data
{
    /// bits[00:11]: Connection handle
    /// bits[12:13]: Packet boundary flag
    /// bits[14:15]: Broadcast flag
    uint16_t conhdl_pb_bc_flag;
    /// length of the data
    uint16_t length;
    /// Memory Pointer address
    uint32_t buf_ptr;
} __attribute__((packed));

/// HCI Synchronous data packet structure
/*@TRACE*/
struct hci_sync_data
{
    /// bits[00:11]: Connection handle
    /// bits[12:13]: Packet status flag
    uint16_t conhdl_psf;
    /// length of the data
    uint8_t length;
    /// EM buffer pointer
    uint16_t buf_ptr;
} __attribute__((packed));

/*
 * HCI COMMANDS PARAMETERS (to classify)
 ****************************************************************************************
 */

/// HCI basic command structure with connection handle
/*@TRACE
 * hci_rd_rssi_cmd = hci_basic_conhdl_cmd
 * hci_le_rd_chnl_map_cmd = hci_basic_conhdl_cmd
 * hci_le_ltk_req_neg_reply_cmd = hci_basic_conhdl_cmd*/
struct hci_basic_conhdl_cmd
{
    /// connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI basic command structure with BD address
struct hci_basic_bd_addr_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI Accept connection request command structure
/*@TRACE*/
struct hci_accept_con_req_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Page Scan Repetition Mode
    uint8_t role;
} __attribute__((packed));

/// HCI Accept synchronous connection request command structure
/*@TRACE*/
struct hci_accept_sync_con_req_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Transmit bandwidth
    uint32_t tx_bw;
    ///Receive bandwidth
    uint32_t rx_bw;
    ///Max latency
    uint16_t max_lat;
    ///Voice settings
    uint16_t vx_set;
    ///Retransmission effort
    uint8_t retx_eff;
    ///Packet type
    uint16_t pkt_type;
} __attribute__((packed));

/// HCI Enhanced Accept synchronous connection request command structure
/*@TRACE*/
struct hci_enh_accept_sync_con_cmd
{
    struct bd_addr bd_addr;         // BD address
    uint32_t       tx_bw;           // Transmit Bandwidth (in B/sec)
    uint32_t       rx_bw;           // Receive Bandwidth (in B/sec)
    uint8_t        tx_cod_fmt[5];   // Transmit Coding Format
    uint8_t        rx_cod_fmt[5];   // Receive Coding Format
    uint16_t       tx_cod_fr_sz;    // Transmit Codec Frame Size (in B)
    uint16_t       rx_cod_fr_sz;    // Receive Codec Frame Size (in B)
    uint32_t       in_bw;           // Input Bandwidth (in B/sec)
    uint32_t       out_bw;          // Output Bandwidth (in B/sec)
    uint8_t        in_cod_fmt[5];   // Input Coding Format
    uint8_t        out_cod_fmt[5];  // Output Coding Format
    uint16_t       in_cod_data_sz;  // Input Coded Data Size (in bits)
    uint16_t       out_cod_data_sz; // Output Coded Data Size (in bits)
    uint8_t        in_data_fmt;     // Input PCM Data Format
    uint8_t        out_data_fmt;    // Output PCM Data Format
    uint8_t        in_msb_pos;      // Input PCM Sample Payload MSB Position (in bits)
    uint8_t        out_msb_pos;     // Output PCM Sample Payload MSB Position (in bits)
    uint8_t        in_data_path;    // Input Data Path
    uint8_t        out_data_path;   // Output Data Path
    uint8_t        in_tr_unit_sz;   // Input Transport Unit Size (in bits)
    uint8_t        out_tr_unit_sz;  // Output Transport Unit Size (in bits)
    uint16_t       max_lat;         // Max Latency (in ms)
    uint16_t       packet_type;     // Packet Type
    uint8_t        retx_eff;        // Retransmission Effort
} __attribute__((packed));

/// HCI reject connection request command structure
/*@TRACE*/
struct hci_reject_con_req_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Reason
    uint8_t reason;
} __attribute__((packed));

/// HCI reject synchronous connection request command structure
/*@TRACE*/
struct hci_reject_sync_con_req_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Reason
    uint8_t reason;
} __attribute__((packed));

/// HCI link key request reply command structure
/*@TRACE*/
struct hci_lk_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Key
    struct ltk key;
} __attribute__((packed));

/// HCI link key request reply command structure
/*@TRACE*/
struct hci_pin_code_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Pin code length
    uint8_t pin_len;
    ///Key
    struct pin_code pin;
} __attribute__((packed));

/// HCI switch role command structure
/*@TRACE*/
struct hci_switch_role_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Read all flag
    uint8_t role;
} __attribute__((packed));

/// HCI flow specification command parameters structure
/*@TRACE*/
struct hci_flow_spec_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Flags
    uint8_t flags;
    ///Flow direction
    uint8_t flow_dir;
    ///Service type
    uint8_t serv_type;
    ///Token rate
    uint32_t tk_rate;
    ///Token buffer size
    uint32_t tk_buf_sz;
    ///Peak bandwidth
    uint32_t pk_bw;
    ///Access latency
    uint32_t acc_lat;
} __attribute__((packed));

/// HCI enhanced flush command parameters structure
/*@TRACE*/
struct hci_enh_flush_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Packet Type
    uint8_t pkt_type;
} __attribute__((packed));

/// HCI command complete event structure for the read auto flush TO command
struct hci_rd_auto_flush_to_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Flush timeout
    uint16_t flush_to;
} __attribute__((packed));

/// HCI write flush timeout command parameters structure
/*@TRACE*/
struct hci_wr_auto_flush_to_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Flush timeout
    uint16_t flush_to;
} __attribute__((packed));

/// HCI change connection packet type command parameters structure
/*@TRACE*/
struct hci_chg_con_pkt_type_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Packet type
    uint16_t pkt_type;
} __attribute__((packed));

/// HCI read link policy settings command parameters structure
/*@TRACE*/
struct hci_rd_link_pol_stg_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Link policy
    uint16_t lnk_policy;
} __attribute__((packed));

/// HCI read link policy settings command parameters structure
/*@TRACE*/
struct hci_wr_link_pol_stg_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Link policy
    uint16_t lnk_policy;
} __attribute__((packed));

/// HCI sniff mode request command parameters structure
/*@TRACE*/
struct hci_sniff_mode_cmd
{
    ///Connection handle
    uint16_t conhdl;
    /// Maximum interval (in slots)
    uint16_t max_int;
    /// Minimum interval (in slots)
    uint16_t min_int;
    /// Attempts (number of receive slots) (in slots)
    uint16_t attempt;
    /// Timeout (number of receive slots) (in slots)
    uint16_t timeout;
} __attribute__((packed));

/// HCI sniff subrating mode request command parameters structure
/*@TRACE*/
struct hci_sniff_sub_cmd
{
    ///Connection handle
    uint16_t conhdl;
    /// Maximum latency used to calculate the maximum sniff subrate that the remote device may use (in slots)
    uint16_t max_lat;
    /// Minimum base sniff subrate timeout that the remote device may use (in slots)
    uint16_t min_rem_to;
    /// Minimum base sniff subrate timeout that the local device may use (in slots)
    uint16_t min_loc_to;
} __attribute__((packed));

/// HCI role discovery complete event parameters structure
/*@TRACE*/
struct hci_role_discovery_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Role
    uint8_t role;
} __attribute__((packed));

/// HCI read failed contact counter command parameters structure
/*@TRACE*/
struct hci_rd_fail_contact_cnt_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Fail contact counter
    uint16_t fail_cnt;
} __attribute__((packed));

/// HCI read link quality complete event parameters structure
/*@TRACE*/
struct hci_rd_link_qual_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Quality type
    uint8_t quality;
} __attribute__((packed));

/// HCI read afh channel map complete event parameters structure
/*@TRACE*/
struct hci_rd_afh_ch_map_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    /// AFH mode
    uint8_t afh_mode;
    /// AFH channel map
    struct chnl_map afh_map;
} __attribute__((packed));

/// HCI read lmp handle complete event parameters structure
/*@TRACE*/
struct hci_rd_lmp_hdl_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///lmp handle
    uint8_t lmp_hdl;
    ///rsvd
    uint32_t rsvd;
} __attribute__((packed));

/// HCI read remote extended features command parameters structure
/*@TRACE*/
struct hci_rd_rem_ext_feats_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///page number
    uint8_t pg_nb;
} __attribute__((packed));

/// HCI read encryption key size complete event parameters structure
/*@TRACE*/
struct hci_rd_enc_key_size_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Key size
    uint8_t key_sz;
} __attribute__((packed));

/// HCI read enhanced transmit power command parameters structure
/*@TRACE*/
struct hci_rd_enh_tx_pwr_lvl_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Type
    uint8_t type;
} __attribute__((packed));

/// HCI read enhanced transmit power complete event parameters structure
/*@TRACE*/
struct hci_rd_enh_tx_pwr_lvl_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Transmit power GFSK
    uint8_t pw_gfsk;
    ///Transmit power DQPSK
    uint8_t pw_dqpsk;
    ///Transmit power 8DPSK
    uint8_t pw_8dpsk;
} __attribute__((packed));

/*
 * HCI LINK CONTROL COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// Format of the message of the Group: LINK_CONTROL_COMMANDS
/// HCI Inquiry command parameters structure
/*@TRACE*/
struct hci_inq_cmd
{
    ///Lap
    struct lap lap;
    ///Inquiry Length in units of 1.28 s
    uint8_t inq_len;
    ///Number of response
    uint8_t nb_rsp;
} __attribute__((packed));

/*@TRACE*/
struct hci_per_inq_mode_cmd
{
    ///Maximum period length
    uint16_t max_per_len;
    ///Minimum period length
    uint16_t min_per_len;
    ///lap
    struct lap lap;
    ///Inquiry length in units of 1.28 s
    uint8_t inq_len;
    ///Number of response
    uint8_t nb_rsp;
} __attribute__((packed));

/*@TRACE*/
struct hci_create_con_cmd
{
    /// BdAddr
    struct bd_addr bd_addr;
    /// Packet Type
    uint16_t pkt_type;
    /// Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    /// Reserved
    uint8_t rsvd;
    /**
     * Clock Offset
     *
     * Bits 14-0 : Bits 16-2 of CLKNslave-CLK
     * Bit 15 : Clock_Offset_Valid_Flag
     *   Invalid Clock Offset = 0
     *   Valid Clock Offset = 1
     */
    uint16_t clk_off;
    /// Allow Switch
    uint8_t switch_en;
} __attribute__((packed));

/// HCI disconnect command structure
/*@TRACE*/
struct hci_disconnect_cmd
{
    /// connection handle
    uint16_t conhdl;
    /// reason
    uint8_t reason;
} __attribute__((packed));

/// HCI master link key command structure
/*@TRACE*/
struct hci_master_lk_cmd
{
    ///Key flag
    uint8_t key_flag;
} __attribute__((packed));

/// HCI authentication request command parameters structure
/*@TRACE*/
struct hci_set_con_enc_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Encryption mode
    uint8_t enc_en;
} __attribute__((packed));

/*@TRACE*/
struct hci_rem_name_req_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    ///Reserved
    uint8_t rsvd;
    /**
     * Clock Offset
     *
     * Bits 14-0 : Bits 16-2 of CLKNslave-CLK
     * Bit 15 : Clock_Offset_Valid_Flag
     *   Invalid Clock Offset = 0
     *   Valid Clock Offset = 1
     */
    uint16_t clk_off;
} __attribute__((packed));

/// HCI remote name request complete event structure
/*@TRACE*/
struct hci_rem_name_req_cmp_evt
{
    /// Status
    uint8_t status;
    /// BD Addr
    struct bd_addr bd_addr;
    /// Name
    struct device_name name;
} __attribute__((packed));

/// HCI setup synchronous connection command structure
/*@TRACE*/
struct hci_setup_sync_con_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Transmit bandwidth
    uint32_t tx_bw;
    ///Receive bandwidth
    uint32_t rx_bw;
    ///Max latency
    uint16_t max_lat;
    ///Voice setting
    uint16_t vx_set;
    ///Retransmission effort
    uint8_t retx_eff;
    ///Packet type
    uint16_t pkt_type;
} __attribute__((packed));

/// HCI setup synchronous connection command structure
/*@TRACE*/
struct hci_enh_setup_sync_con_cmd
{
    uint16_t conhdl;          // Connection Handle
    uint32_t tx_bw;           // Transmit Bandwidth (in B/sec)
    uint32_t rx_bw;           // Receive Bandwidth (in B/sec)
    uint8_t  tx_cod_fmt[5];   // Transmit Coding Format
    uint8_t  rx_cod_fmt[5];   // Receive Coding Format
    uint16_t tx_cod_fr_sz;    // Transmit Codec Frame Size (in B)
    uint16_t rx_cod_fr_sz;    // Receive Codec Frame Size (in B)
    uint32_t in_bw;           // Input Bandwidth (in B/sec)
    uint32_t out_bw;          // Output Bandwidth (in B/sec)
    uint8_t  in_cod_fmt[5];   // Input Coding Format
    uint8_t  out_cod_fmt[5];  // Output Coding Format
    uint16_t in_cod_data_sz;  // Input Coded Data Size (in bits)
    uint16_t out_cod_data_sz; // Output Coded Data Size (in bits)
    uint8_t  in_data_fmt;     // Input PCM Data Format
    uint8_t  out_data_fmt;    // Output PCM Data Format
    uint8_t  in_msb_pos;      // Input PCM Sample Payload MSB Position (in bits)
    uint8_t  out_msb_pos;     // Output PCM Sample Payload MSB Position (in bits)
    uint8_t  in_data_path;    // Input Data Path
    uint8_t  out_data_path;   // Output Data Path
    uint8_t  in_tr_unit_sz;   // Input Transport Unit Size (in bits)
    uint8_t  out_tr_unit_sz;  // Output Transport Unit Size (in bits)
    uint16_t max_lat;         // Max Latency (in ms)
    uint16_t packet_type;     // Packet Type
    uint8_t  retx_eff;        // Retransmission Effort
} __attribute__((packed));

/// HCI io capability request reply command structure
/*@TRACE*/
struct hci_io_cap_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///IO capability
    uint8_t io_capa;
    ///OOB data present
    uint8_t oob_data_pres;
    ///Authentication requirements
    uint8_t auth_req;
} __attribute__((packed));

/// HCI io capability request negative reply command structure
/*@TRACE*/
struct hci_io_cap_req_neg_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Reason
    uint8_t reason;
} __attribute__((packed));

/// HCI user pass key request reply command structure
/*@TRACE*/
struct hci_user_passkey_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Numeric value
    uint32_t num_val;
} __attribute__((packed));

/// HCI remote oob data request reply command structure
/*@TRACE*/
struct hci_rem_oob_data_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///hash part
    struct hash oob_c;
    ///random part
    struct randomizer oob_r;
} __attribute__((packed));

/// HCI send key press notification command structure
/*@TRACE*/
struct hci_send_keypress_notif_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Notification type
    uint8_t notif_type;
} __attribute__((packed));

/// HCI truncated page command structure
/*@TRACE*/
struct hci_trunc_page_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    /// Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    /**
     * Clock Offset
     *
     * Bits 14-0 : Bits 16-2 of CLKNslave-CLK
     * Bit 15 : Clock_Offset_Valid_Flag
     *   Invalid Clock Offset = 0
     *   Valid Clock Offset = 1
     */
    uint16_t clk_off;
} __attribute__((packed));

/// HCI truncated page cancel command structure
/*@TRACE*/
struct hci_trunc_page_can_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI set connectionless slave broadcast command structure
/*@TRACE*/
struct hci_set_con_slv_bcst_cmd
{
    /// Enable
    uint8_t enable;
    /// LT_ADDR
    uint8_t lt_addr;
    /// LPO_Allowed
    uint8_t lpo_allowed;
    /// Packet_Type
    uint16_t packet_type;
    /// Interval_Min (in slots)
    uint16_t interval_min;
    /// Interval_Max (in slots)
    uint16_t interval_max;
    /// CSB_supervisionTO (in slots)
    uint16_t csb_supv_to;
} __attribute__((packed));

/// HCI set connectionless slave broadcast command complete event structure
/*@TRACE*/
struct hci_set_con_slv_bcst_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// LT_ADDR
    uint8_t lt_addr;
    /// Interval (in slots)
    uint16_t interval;
} __attribute__((packed));

/// HCI set connectionless slave broadcast receive command structure
/*@TRACE*/
struct hci_set_con_slv_bcst_rec_cmd
{
    /// Enable
    uint8_t enable;
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// LT_ADDR
    uint8_t lt_addr;
    /// Interval (in slots)
    uint16_t interval;
    /// Clock_Offset (28 bits) - (CLKNslave � CLK) modulo 2^28
    uint32_t clock_offset;
    /// Next_Connectionless_Slave_Broadcast_Clock (28 bits)
    uint32_t next_csb_clock;
    /// CSB_supervisionTO (in slots)
    uint16_t csb_supv_to;
    /// Remote_Timing_Accuracy (in ppm)
    uint8_t remote_timing_accuracy;
    /// Skip
    uint8_t skip;
    /// Packet_Type
    uint16_t packet_type;
    /// AFH_Channel_Map
    struct chnl_map afh_ch_map;
} __attribute__((packed));

/// HCI set connectionless slave broadcast receive command complete event structure
/*@TRACE*/
struct hci_set_con_slv_bcst_rec_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Receive Synchronization Train command structure
/*@TRACE*/
struct hci_rec_sync_train_cmd
{
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// Synchronization_scanTO (in slots)
    uint16_t sync_scan_to;
    /// Sync_Scan_Window (in slots)
    uint16_t sync_scan_win;
    /// Sync_Scan_Interval (in slots)
    uint16_t sync_scan_int;
} __attribute__((packed));

/// HCI remote oob extended data request reply command structure
/*@TRACE*/
struct hci_rem_oob_ext_data_req_reply_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///hash part
    struct hash oob_c_192;
    ///random part
    struct randomizer oob_r_192;
    ///hash part
    struct hash oob_c_256;
    ///random part
    struct randomizer oob_r_256;
} __attribute__((packed));

/*@TRACE
 * hci_le_gen_dhkey = hci_le_generate_dh_key_cmd*/
struct hci_le_generate_dh_key_cmd
{
    uint8_t public_key[64];
} __attribute__((packed));

/*
 * HCI LINK POLICY COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// HCI setup quality of service command structure
/*@TRACE*/
struct hci_qos_setup_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Flags
    uint8_t flags;
    ///Service type
    uint8_t serv_type;
    ///Token rate
    uint32_t tok_rate;
    ///Peak bandwidth
    uint32_t pk_bw;
    ///Latency
    uint32_t lat;
    ///Delay variation
    uint32_t del_var;
} __attribute__((packed));

/// HCI command complete event structure for read default link policy command structure
/*@TRACE*/
struct hci_rd_dft_link_pol_stg_cmd_cmp_evt
{
    ///Status of the command reception
    uint8_t status;
    ///Link policy
    uint16_t link_pol_stg;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_dft_link_pol_stg_cmd
{
    ///Link policy
    uint16_t link_pol_stg;
} __attribute__((packed));

/*
 * HCI CONTROL & BASEBAND COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// HCI set event mask command structure
/*@TRACE*/
struct hci_set_evt_mask_cmd
{
    ///Event Mask
    struct evt_mask event_mask;
} __attribute__((packed));

/// HCI set event filter command structure
/*@TRACE*/
struct hci_set_evt_filter_cmd
{
    /// Filter type
    uint8_t filter_type;

    /// Filters
    union hci_filter
    {
        uint8_t clear_all_filter_reserved;

        /// Inquiry Result Filter
        struct inq_res_filter
        {
            /// Filter Condition type
            uint8_t cond_type;

            /// Filter conditions
            union hci_inq_filter_cond
            {
                /// Reserved value (Inquiry Result Filter - condition type 0x00 has no condition)
                uint8_t cond_0_reserved;

                /// Inquiry Result Filter Condition - condition type 0x01
                struct inq_res_filter_cond_1
                {
                    /// Class_of_Device
                    struct devclass class_of_dev;
                    /// Class_of_Device_Mask
                    struct devclass class_of_dev_msk;
                } cond_1;

                /// Inquiry Result Filter Condition - condition type 0x02
                struct inq_res_filter_cond_2
                {
                    /// BD Address
                    struct bd_addr bd_addr;
                } cond_2;
            } cond;
        } inq_res;

        /// Connection Setup Filter
        struct con_set_filter
        {
            /// Filter Condition type
            uint8_t cond_type;

            /// Filter conditions
            union hci_con_filter_cond
            {
                /// Connection Setup Filter Condition - condition type 0x00
                struct con_set_filter_cond_0
                {
                    /// Auto_Accept_Flag
                    uint8_t auto_accept;
                } cond_0;

                /// Connection Setup Filter Condition - condition type 0x01
                struct con_set_filter_cond_1
                {
                    /// Class_of_Device
                    struct devclass class_of_dev;
                    /// Class_of_Device_Mask
                    struct devclass class_of_dev_msk;
                    /// Auto_Accept_Flag
                    uint8_t auto_accept;
                } cond_1;

                /// Connection Setup Filter Condition - condition type 0x02
                struct con_set_filter_cond_2
                {
                    /// BD Address
                    struct bd_addr bd_addr;
                    /// Auto_Accept_Flag
                    uint8_t auto_accept;
                } cond_2;
            } cond;

        } con_set;

    } filter;
} __attribute__((packed));

/// HCI command completed event structure for the flush command
/*@TRACE*/
struct hci_flush_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for the Read pin type command
/*@TRACE*/
struct hci_rd_pin_type_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///PIN type
    uint8_t pin_type;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_pin_type_cmd
{
    ///PIN type
    uint8_t pin_type;
} __attribute__((packed));

/*@TRACE*/
struct hci_rd_stored_lk_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Read all flag
    uint8_t rd_all_flag;
} __attribute__((packed));

/// HCI command complete event structure for read stored link key command
/*@TRACE*/
struct hci_rd_stored_lk_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Maximum number of key
    uint16_t num_key_max;
    ///Read number of key
    uint16_t num_key_rd;
} __attribute__((packed));

#if BT_EMB_PRESENT
/*@TRACE*/
struct hci_wr_stored_lk_cmd
{
    /// Number of key to write
    uint8_t num_key_wr;

    /// BD Address + Key table
    struct bd_addr_plus_key link_keys[HCI_MAX_CMD_PARAM_SIZE / sizeof(struct bd_addr_plus_key)];
} __attribute__((packed));
#endif //BT_EMB_PRESENT

/// HCI command complete event structure for write stored link key command
/*@TRACE*/
struct hci_wr_stored_lk_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///number of key written
    uint8_t num_key_wr;
} __attribute__((packed));

/*@TRACE*/
struct hci_del_stored_lk_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Delete all flag
    uint8_t del_all_flag;
} __attribute__((packed));

/// HCI command complete event structure for delete stored link key command
/*@TRACE*/
struct hci_del_stored_lk_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Read number of key
    uint16_t num_key_del;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_local_name_cmd
{
    ///Name
    struct device_name name;
} __attribute__((packed));

/// HCI command complete event structure for the read local name command
/*@TRACE*/
struct hci_rd_local_name_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///Name
    uint8_t name[BD_NAME_SIZE];
} __attribute__((packed));

/// HCI command complete event structure for the Read connection accept to command
/*@TRACE*/
struct hci_rd_con_accept_to_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    /// Connection accept timeout (in slots)
    uint16_t con_acc_to;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_con_accept_to_cmd
{
    /// Connection accept timeout (in slots)
    uint16_t con_acc_to;
} __attribute__((packed));

/// HCI command complete event structure for the Read page to command
/*@TRACE*/
struct hci_rd_page_to_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    /// Page timeout (in slots)
    uint16_t page_to;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_page_to_cmd
{
    /// Page timeout (in slots)
    uint16_t page_to;
} __attribute__((packed));

/// HCI command complete event structure for the Read scan enable command
/*@TRACE*/
struct hci_rd_scan_en_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///Status of the scan enable
    uint8_t scan_en;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_scan_en_cmd
{
    ///Status of the scan enable
    uint8_t scan_en;
} __attribute__((packed));

/// HCI command complete event structure for the Read scan activity command
/*@TRACE*/
struct hci_rd_page_scan_act_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    /// Page scan interval (in slots)
    uint16_t page_scan_intv;
    /// Page scan window (in slots)
    uint16_t page_scan_win;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_page_scan_act_cmd
{
    /// Page scan interval (in slots)
    uint16_t page_scan_intv;
    /// Page scan window (in slots)
    uint16_t page_scan_win;
} __attribute__((packed));

/// HCI command complete event structure for the Read inquiry scan activity command
/*@TRACE*/
struct hci_rd_inq_scan_act_cmd_cmp_evt
{
    /// Status of the command
    uint8_t status;
    /// Inquiry scan interval (in slots)
    uint16_t inq_scan_intv;
    /// Inquiry scan window (in slots)
    uint16_t inq_scan_win;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_inq_scan_act_cmd
{
    /// Inquiry scan interval (in slots)
    uint16_t inq_scan_intv;
    /// Inquiry scan window (in slots)
    uint16_t inq_scan_win;
} __attribute__((packed));

/// HCI command complete event structure for the Read authentication command
/*@TRACE*/
struct hci_rd_auth_en_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///Value of the authentication
    uint8_t auth_en;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_auth_en_cmd
{
    ///Value of the authentication
    uint8_t auth_en;
} __attribute__((packed));

/// HCI command complete event structure for the read class of device command
/*@TRACE*/
struct hci_rd_class_of_dev_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///Class of device
    struct devclass class_of_dev;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_class_of_dev_cmd
{
    ///Class of device
    struct devclass class_of_dev;
} __attribute__((packed));

/// HCI read voice settings complete event
/*@TRACE*/
struct hci_rd_voice_stg_cmd_cmp_evt
{
    ///Status of the command reception
    uint8_t status;
    /// Voice setting
    uint16_t voice_stg;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_voice_stg_cmd
{
    /// voice setting
    uint16_t voice_stg;
} __attribute__((packed));

/// HCI command complete event structure for read number of broadcast retrans command
/*@TRACE*/
struct hci_rd_nb_bdcst_retx_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Read number of broadcast retransmission
    uint8_t num_bcst_ret;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_nb_bdcst_retx_cmd
{
    ///Read number of broadcast retransmission
    uint8_t num_bcst_ret;
} __attribute__((packed));

/// HCI command complete event structure for the Read Synchronous Flow Control command
/*@TRACE*/
struct hci_rd_sync_flow_ctrl_en_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Synchronous flow control enable
    uint8_t sync_flow_ctrl_en;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_sync_flow_ctrl_en_cmd
{
    /// Synchronous Flow Control enable
    uint8_t sync_flow_ctrl_en;
} __attribute__((packed));

///HCI set controller to host flow control command
/*@TRACE*/
struct hci_set_ctrl_to_host_flow_ctrl_cmd
{
    ///Flow control enable for controller
    uint8_t flow_cntl;
} __attribute__((packed));

///HCI host buffer size command
/*@TRACE*/
struct hci_host_buf_size_cmd
{
    ///Host ACL packet length
    uint16_t acl_pkt_len;
    ///Host synchronous packet length
    uint8_t sync_pkt_len;
    ///Host Total number of ACL packets allowed
    uint16_t nb_acl_pkts;
    ///Host total number of synchronous packets allowed
    uint16_t nb_sync_pkts;
} __attribute__((packed));

#if BT_EMB_PRESENT
///HCI host number of completed packets command
/*@TRACE*/
struct hci_host_nb_cmp_pkts_cmd
{
    ///Number of handles for which the completed packets number is given
    uint8_t nb_of_hdl;
    ///Array of connection handles
    uint16_t con_hdl[MAX_NB_ACTIVE_ACL];
    ///Array of number of completed packets values for connection handles.
    uint16_t nb_comp_pkt[MAX_NB_ACTIVE_ACL];
} __attribute__((packed));
#elif BLE_EMB_PRESENT || BLE_HOST_PRESENT
///HCI host number of completed packets command
/*@TRACE*/
struct hci_host_nb_cmp_pkts_cmd
{
    ///Number of handles for which the completed packets number is given
    uint8_t nb_of_hdl;
    ///Array of connection handles
    uint16_t con_hdl[BLE_ACTIVITY_MAX + 1]; // ensure that at least 1 element is present
    ///Array of number of completed packets values for connection handles.
    uint16_t nb_comp_pkt[BLE_ACTIVITY_MAX + 1]; // ensure that at least 1 element is present
} __attribute__((packed));
#endif //BLE_EMB_PRESENT || BLE_HOST_PRESENT

/// HCI read link supervision timeout command parameters structure
struct hci_rd_link_supv_to_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Link supervision timeout
    uint16_t lsto_val;
} __attribute__((packed));

/// HCI write link supervision timeout command parameters structure
/*@TRACE*/
struct hci_wr_link_supv_to_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Link supervision timeout
    uint16_t lsto_val;
} __attribute__((packed));

/// HCI command complete event structure for the nb of supported IAC command
/*@TRACE*/
struct hci_rd_nb_supp_iac_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///nb_of iac
    uint8_t nb_iac;
} __attribute__((packed));

/// HCI command complete event structure for read current IAC LAP command
/*@TRACE*/
struct hci_rd_curr_iac_lap_cmd_cmp_evt
{
    ///Status of the command
    uint8_t status;
    ///nb of current iac
    uint8_t nb_curr_iac;
    ///lap
    struct lap iac_lap;
} __attribute__((packed));

/// HCI write current IAC LAP command structure
/*@TRACE*/
struct hci_wr_curr_iac_lap_cmd
{
    /// Number of current iac laps
    uint8_t nb_curr_iac;
    ///lap
    struct lap iac_lap[(HCI_MAX_CMD_PARAM_SIZE / BD_ADDR_LAP_LEN) - 1];
} __attribute__((packed));

/*@TRACE*/
struct hci_set_afh_host_ch_class_cmd
{
    ///AFH channel map
    struct chnl_map afh_ch;
} __attribute__((packed));

/// HCI command complete event structure for write inquiry scan type command structure
/*@TRACE*/
struct hci_rd_inq_scan_type_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    /// Inquiry scan type
    uint8_t inq_scan_type;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_inq_scan_type_cmd
{
    /// Inquiry scan type
    uint8_t inq_scan_type;
} __attribute__((packed));

/// HCI command complete event structure for read inquiry mode command structure
/*@TRACE*/
struct hci_rd_inq_mode_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    /// Inquiry mode
    uint8_t inq_mode;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_inq_mode_cmd
{
    /// Inquiry mode
    uint8_t inq_mode;
} __attribute__((packed));

/// HCI command complete event structure for write page scan type command structure
/*@TRACE*/
struct hci_rd_page_scan_type_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    /// Page scan type
    uint8_t page_scan_type;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_page_scan_type_cmd
{
    /// Page scan type
    uint8_t page_scan_type;
} __attribute__((packed));

/// HCI command complete event structure for read assessment mode command structure
/*@TRACE*/
struct hci_rd_afh_ch_assess_mode_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///AFH channel assessment mode
    uint8_t afh_ch_ass_mode;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_afh_ch_assess_mode_cmd
{
    ///AFH channel assessment mode
    uint8_t afh_ch_ass_mode;
} __attribute__((packed));

/// HCI command complete event structure for remote name request cancel command
/*@TRACE*/
struct hci_rd_ext_inq_rsp_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///FEC required
    uint8_t fec_req;
    ///Extended inquiry response
    struct eir eir;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_ext_inq_rsp_cmd
{
    ///FEC required
    uint8_t fec_req;
    ///Extended inquiry response
    struct eir eir;
} __attribute__((packed));

/// HCI command complete event structure for remote name request cancel command
/*@TRACE*/
struct hci_rd_sp_mode_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///Simple pairing mode
    uint8_t sp_mode;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_sp_mode_cmd
{
    ///Simple pairing mode
    uint8_t sp_mode;
} __attribute__((packed));

/// HCI command complete event structure for read oob data command
/*@TRACE*/
struct hci_rd_loc_oob_data_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///hash part
    struct hash oob_c;
    ///random part
    struct randomizer oob_r;
} __attribute__((packed));

/// HCI command complete event structure for read inquiry response transmit power command
/*@TRACE*/
struct hci_rd_inq_rsp_tx_pwr_lvl_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///TX power
    uint8_t tx_pwr;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_inq_tx_pwr_lvl_cmd
{
    ///TX power
    int8_t tx_pwr;
} __attribute__((packed));

/// HCI command complete event structure for read erroneous data reporting command
/*@TRACE*/
struct hci_rd_dft_err_data_rep_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///Erroneous data reporting
    uint8_t err_data_rep;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_dft_err_data_rep_cmd
{
    ///Erroneous data reporting
    uint8_t err_data_rep;
} __attribute__((packed));

/// HCI read LE Host Supported complete event
/*@TRACE*/
struct hci_rd_le_host_supp_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///LE_Supported_Host
    uint8_t le_supported_host;
    ///Simultaneous_LE_Host
    uint8_t simultaneous_le_host;
} __attribute__((packed));

/// HCI write LE Host Supported command
/*@TRACE*/
struct hci_wr_le_host_supp_cmd
{
    ///LE_Supported_Host
    uint8_t le_supported_host;
    ///Simultaneous_LE_Host
    uint8_t simultaneous_le_host;
} __attribute__((packed));

/// HCI Set MWS Channel Parameters command
/*@TRACE*/
struct hci_set_mws_channel_params_cmd
{
    ///MWS_Channel_Enable
    uint8_t mws_channel_enable;
    ///MWS_RX_Center_Frequency
    uint16_t mws_rx_center_frequency;
    ///MWS_TX_Center_Frequency
    uint16_t mws_tx_center_frequency;
    ///MWS_RX_Channel_Bandwidth
    uint16_t mws_rx_channel_bandwidth;
    ///MWS_TX_Channel_Bandwidth
    uint16_t mws_tx_channel_bandwidth;
    ///MWS_Channel_Type
    uint8_t mws_channel_type;
} __attribute__((packed));

/// HCI Set External Frame Configuration command
/*@TRACE*/
struct hci_set_external_frame_config_cmd
{
    /// Ext_Frame_Duration
    uint16_t ext_fr_duration;
    /// Ext_Frame_Sync_Assert_Offset
    int16_t ext_fr_sync_assert_offset;
    /// Ext_Frame_Sync_Assert_Jitter
    uint16_t ext_fr_sync_assert_jitter;
    /// Ext_Frame_Num_Periods
    uint8_t ext_fr_num_periods;
    /// Period Durations & Types
    struct ext_fr_period period[1 /*__ARRAY_EMPTY*/];
} __attribute__((packed));

/// HCI Set MWS Signaling command
/*@TRACE*/
struct hci_set_mws_signaling_cmd
{
    ///MWS_RX_Assert_Offset
    int16_t mws_rx_assert_offset;
    ///MWS_RX_Assert_Jitter
    uint16_t mws_rx_assert_jitter;
    ///MWS_RX_Deassert_Offset
    int16_t mws_rx_deassert_offset;
    ///MWS_RX_Deassert_Jitter
    uint16_t mws_rx_deassert_jitter;
    ///MWS_TX_Assert_Offset
    int16_t mws_tx_assert_offset;
    ///MWS_TX_Assert_Jitter
    uint16_t mws_tx_assert_jitter;
    ///MWS_TX_Deassert_Offset
    int16_t mws_tx_deassert_offset;
    ///MWS_TX_Deassert_Jitter
    uint16_t mws_tx_deassert_jitter;
    ///MWS_Pattern_Assert_Offset
    int16_t mws_pattern_assert_offset;
    ///MWS_Pattern_Assert_Jitter
    uint16_t mws_pattern_assert_jitter;
    ///MWS_Inactivity_Duration_Assert_Offset
    int16_t mws_inactivity_duration_assert_offset;
    ///MWS_Inactivity_Duration_Assert_Jitter
    uint16_t mws_inactivity_duration_assert_jitter;
    ///MWS_Scan_Frequency_Assert_Offset
    int16_t mws_scan_frequency_assert_offset;
    ///MWS_Scan_Frequency_Assert_Jitter
    uint16_t mws_scan_frequency_assert_jitter;
    ///MWS_Priority_Assert_Offset_Request
    uint16_t mws_priority_assert_offset_request;
} __attribute__((packed));

/// HCI Set MWS Signaling command complete event
/*@TRACE*/
struct hci_set_mws_signaling_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///Bluetooth_Rx_Priority_Assert_Offset
    int16_t bt_rx_prio_assert_offset;
    ///Bluetooth_Rx_Priority_Assert_Jitter
    uint16_t bt_rx_prio_assert_jitter;
    ///Bluetooth_Rx_Priority_Deassert_Offset
    int16_t bt_rx_prio_deassert_offset;
    ///Bluetooth_Rx_Priority_Deassert_Jitter
    uint16_t bt_rx_prio_deassert_jitter;
    ///802_Rx_Priority_Assert_Offset
    int16_t _802_rx_prio_assert_offset;
    ///802_Rx_Priority_Assert_Jitter
    uint16_t _802_rx_prio_assert_jitter;
    ///802_Rx_Priority_Deassert_Offset
    int16_t _802_rx_prio_deasssert_offset;
    ///802_Rx_Priority_Deassert_Jitter
    uint16_t _802_rx_prio_deassert_jitter;
    ///Bluetooth_Tx_On_Assert_Offset
    int16_t bt_tx_on_assert_offset;
    ///Bluetooth_Tx_On_Assert_Jitter
    uint16_t bt_tx_on_assert_jitter;
    ///Bluetooth_Tx_On_Deassert_Offset
    int16_t bt_tx_on_deassert_offset;
    ///Bluetooth_Tx_On_Deassert_Jitter
    uint16_t bt_tx_on_deassert_jitter;
    ///802_Tx_On_Assert_Offset
    int16_t _802_tx_on_assert_offset;
    ///802_Tx_On_Assert_Jitter
    uint16_t _802_tx_on_assert_jitter;
    ///802_Tx_On_Deassert_Offset
    int16_t _802_tx_on_deassert_offset;
    ///802_Tx_On_Deassert_Jitter
    uint16_t _802_tx_on_deassert_jitter;
} __attribute__((packed));

/// HCI Set MWS Transport Layer command
/*@TRACE*/
struct hci_set_mws_transport_layer_cmd
{
    ///Transport_Layer
    uint8_t transport_layer;
    ///To_MWS_Baud_Rate
    uint32_t to_mws_baud_rate;
    ///From_MWS_Baud_Rate
    uint32_t from_mws_baud_rate;
} __attribute__((packed));

/// HCI Set MWS Scan Frequency Table command
/*@TRACE*/
struct hci_set_mws_scan_freq_table_cmd
{
    ///Num_Scan_Frequencies
    uint8_t num_scan_frequencies;
    ///Scan_Frequencys Low & High
    struct mws_scan_freq scan_freq[1 /*__ARRAY_EMPTY*/];
} __attribute__((packed));

/// HCI Set MWS Pattern Configuration command
/*@TRACE*/
struct hci_set_mws_pattern_config_cmd
{
    ///MWS_PATTERN_Index
    uint8_t mws_pattern_index;
    ///MWS_PATTERN_NumIntervals
    uint8_t num_intervals;
    ///MWS_PATTERN_Interval Duration & Type
    struct mws_pattern_intv intv[1 /*__ARRAY_EMPTY*/];
} __attribute__((packed));

/// Hci Get MWS Transport Layer Configuration command complete event
/*@TRACE*/
struct hci_get_mws_transport_layer_config_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///Num_Transports
    uint8_t num_transports;
    ///Transport_Layers
    struct mws_transport tran[1 /*__ARRAY_EMPTY*/];
} __attribute__((packed));

/// HCI read Secure Connections Host Support complete event
/*@TRACE*/
struct hci_rd_sec_con_host_supp_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Secure Connections Host Support
    uint8_t sec_con_host_supp;
} __attribute__((packed));

/// HCI write Secure Connections Host Support command
/*@TRACE*/
struct hci_wr_sec_con_host_supp_cmd
{
    /// Secure Connections Host Support
    uint8_t sec_con_host_supp;
} __attribute__((packed));

/// HCI write Secure Connections Test Mode command
struct hci_wr_sec_con_test_mode_cmd
{
    /// Connection handle
    uint16_t conhdl;
    /// DM1 ACL-U mode
    uint8_t dm1_acl_u_mode;
    /// eSCO loopback mode
    uint8_t esco_loopback_mode;
} __attribute__((packed));

/// HCI write Secure Connections Test Mode complete event
struct hci_wr_sec_con_test_mode_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI set max slots  command
struct hci_set_max_slots_cmd
{
    /// connection handle
    uint16_t conhdl;
    /// max_slots
    uint8_t max_slots;
} __attribute__((packed));

/// HCI set max slots  command
struct hci_set_more_data_mode_cmd
{
    /// connection handle
    uint16_t conhdl;
    /// more data mode : music  spp etc.
    uint8_t more_data_mode;  //BIT[0]: moredata or not/ BIT[1-3]: acl window div
    uint8_t music_expect_nb; //Fore detect music data
} __attribute__((packed));

/// HCI increase peer power max
struct hci_set_inc_peer_pwr_max_cmd
{
    /// connection handle
    uint16_t conhdl;
    /// increase peer power max
    uint8_t inc_pwr_en;
} __attribute__((packed));

/// HCI increase peer power max
struct hci_rd_tcf_info_cmd
{
    /// connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI Set Reserved LT_ADDR command
/*@TRACE*/
struct hci_set_res_lt_addr_cmd
{
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Set Reserved LT_ADDR command complete event
/*@TRACE*/
struct hci_set_res_lt_addr_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Delete Reserved LT_ADDR command
/*@TRACE*/
struct hci_del_res_lt_addr_cmd
{
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Delete Reserved LT_ADDR command complete event
/*@TRACE*/
struct hci_del_res_lt_addr_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Set Connectionless Slave Broadcast Data command
/*@TRACE*/
struct hci_set_con_slv_bcst_data_cmd
{
    /// LT_ADDR
    uint8_t lt_addr;
    /// Fragment
    uint8_t fragment;
    /// Data_Length (in bytes)
    uint8_t data_length;
    /// Data
    uint8_t data[__ARRAY_EMPTY];
} __attribute__((packed));

/// HCI Set Connectionless Slave Broadcast Data command complete event
/*@TRACE*/
struct hci_set_con_slv_bcst_data_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Read Synchronization Train Parameters command complete event
/*@TRACE*/
struct hci_rd_sync_train_param_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Sync_Train_Interval (in slots)
    uint16_t sync_train_int;
    /// synchronization_trainTO (in slots)
    uint32_t sync_train_to;
    /// Service_Data
    uint8_t service_data;
} __attribute__((packed));

/// HCI Write Synchronization Train Parameters command
/*@TRACE*/
struct hci_wr_sync_train_param_cmd
{
    /// Interval_Min (in slots)
    uint16_t int_min;
    /// Interval_Max (in slots)
    uint16_t int_max;
    /// synchronization_trainTO (in slots)
    uint32_t sync_train_to;
    /// Service_Data
    uint8_t service_data;
} __attribute__((packed));

/// HCI Write Synchronization Train Parameters command complete event
/*@TRACE*/
struct hci_wr_sync_train_param_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Sync_Train_Interval (in slots)
    uint16_t sync_train_int;
} __attribute__((packed));

// HCI Synchronization Train Complete event
struct hci_sync_train_cmp_evt
{
    /// Status
    uint8_t status;
} __attribute__((packed));

/// HCI read authenticated payload timeout command
/*@TRACE*/
struct hci_rd_auth_payl_to_cmd
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for the Read Authenticated Payload Timeout Command
/*@TRACE*/
struct hci_rd_auth_payl_to_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Authenticated payload timeout
    uint16_t auth_payl_to;
} __attribute__((packed));

/// HCI command complete event structure for read oob extended data command
/*@TRACE*/
struct hci_rd_loc_oob_ext_data_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///hash part
    struct hash oob_c_192;
    ///random part
    struct randomizer oob_r_192;
    ///hash part
    struct hash oob_c_256;
    ///random part
    struct randomizer oob_r_256;
} __attribute__((packed));

/// HCI read Extended Page Timeout CC event
/*@TRACE*/
struct hci_rd_ext_page_to_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /**
     * Extended Page Timeout measured in Number of Baseband slots. Interval Length = N * 0.625 msec (1 Baseband slot)
     * Range for N: 0x0000 (default) - 0xFFFF
     * Time Range: 0 - 40.9 Seconds
     */
    uint16_t ext_page_to;
} __attribute__((packed));

/// HCI write Extended Page Timeout
/*@TRACE*/
struct hci_wr_ext_page_to_cmd
{
    /**
     * Extended Page Timeout measured in Number of Baseband slots. Interval Length = N * 0.625 msec (1 Baseband slot)
     * Range for N: 0x0000 (default) - 0xFFFF
     * Time Range: 0 - 40.9 Seconds
     */
    uint16_t ext_page_to;
} __attribute__((packed));

/// HCI read Extended Inquiry Length CC event
/*@TRACE*/
struct hci_rd_ext_inq_len_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Extended Inquiry Length
    uint16_t ext_inq_len;
} __attribute__((packed));

/// HCI write Extended Inquiry Length
/*@TRACE*/
struct hci_wr_ext_inq_len_cmd
{
    /// Extended Inquiry Length
    uint16_t ext_inq_len;
} __attribute__((packed));

/*
 * HCI INFORMATIONAL PARAMETERS COMMANDS PARAMETERS
 ****************************************************************************************
 */

///HCI command complete event structure for read local version information
/*@TRACE*/
struct hci_rd_local_ver_info_cmd_cmp_evt
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
} __attribute__((packed));

///HCI command complete event structure for read local supported commands
/*@TRACE*/
struct hci_rd_local_supp_cmds_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Supported Commands structure
    struct supp_cmds local_cmds;
} __attribute__((packed));

/// HCI command complete event structure for read local supported features command
/*@TRACE*/
struct hci_rd_local_supp_feats_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Local  supported features
    struct features feats;
} __attribute__((packed));

/*@TRACE*/
struct hci_rd_local_ext_feats_cmd
{
    ///Page number
    uint8_t page_nb;
} __attribute__((packed));

/// HCI command complete event structure for read local extended features command
/*@TRACE*/
struct hci_rd_local_ext_feats_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Page number
    uint8_t page_nb;
    ///Maximum page number
    uint8_t page_nb_max;
    ///Extended LMP features
    struct features ext_feats;
} __attribute__((packed));

///HCI command complete event structure for the Read Buffer Size Command
/*@TRACE*/
struct hci_rd_buff_size_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///ACL data packet length controller can receive from host
    uint16_t hc_data_pk_len;
    ///Synchronous data packet length controller can receive from host
    uint8_t hc_sync_pk_len;
    ///Total number of ACL data packets controller can receive from host
    uint16_t hc_tot_nb_data_pkts;
    ///Total number of synchronous data packets controller can receive from host
    uint16_t hc_tot_nb_sync_pkts;
} __attribute__((packed));

///HCI command complete event structure for read bd address
/*@TRACE*/
struct hci_rd_bd_addr_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///BD address
    struct bd_addr local_addr;
} __attribute__((packed));

/// HCI command complete event structure for read local supported codecs
/*@TRACE*/
struct hci_rd_local_supp_codecs_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    uint8_t nb_supp_codecs;
    uint8_t nb_supp_vendor_specific_codecs;

    //    ///Supported Codecs structure
    //    struct supp_codecs    local_codecs;
} __attribute__((packed));

/// HCI command complete event structure for read local supported codecs v2
/*@TRACE*/
struct hci_rd_local_supp_codecs_v2_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    uint8_t nb_supp_codecs;
    uint8_t nb_supp_vendor_specific_codecs;

    //    ///Supported Codecs structure
    //    struct supp_codecs    local_codecs;
} __attribute__((packed));

/// HCI command complete event structure for read local simple pairing options
/*@TRACE*/
struct hci_rd_local_sp_opt_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    /// Simple Pairing options (bit 0: Remote public key validation)
    uint8_t sp_opt;
    /// Maximum Encryption Key Size (in octets)
    uint8_t max_enc_key_size;
} __attribute__((packed));

/*
 * HCI STATUS PARAMETERS COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// HCI command complete event structure for read rssi
/*@TRACE*/
struct hci_rd_rssi_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///RSSI value
    uint8_t rssi;
} __attribute__((packed));

/*@TRACE*/
struct hci_rd_clk_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Which clock
    uint8_t clk_type;
} __attribute__((packed));

/// HCI read clock command structure
/*@TRACE*/
struct hci_rd_clk_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///clock
    uint32_t clk;
    ///Accuracy
    uint16_t clk_acc;
} __attribute__((packed));

/// HCI read clock command structure
/*@TRACE*/
struct hci_rd_tcf_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Accuracy
    uint32_t stick;
    ///clock
    uint32_t clk;
    uint16_t fcnt;
} __attribute__((packed));

/*
 * HCI TESTING COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// HCI command complete event structure for read loop back mode command
/*@TRACE*/
struct hci_rd_loopback_mode_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Local  supported features
    uint8_t lb_mode;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_loopback_mode_cmd
{
    ///Local  supported features
    uint8_t lb_mode;
} __attribute__((packed));

/*@TRACE*/
struct hci_wr_sp_dbg_mode_cmd
{
    ///Simple pairing mode
    uint8_t sp_mode;
} __attribute__((packed));

/*
 * HCI LE CONTROLLER COMMANDS PARAMETERS
 ****************************************************************************************
 */

///HCI LE Set Event Mask Command parameters structure
/*@TRACE*/
struct hci_le_set_evt_mask_cmd
{
    ///LE Event Mask
    struct evt_mask le_mask;
} __attribute__((packed));

///HCI LE Set Random Address Command parameters structure
/*@TRACE*/
struct hci_le_set_rand_addr_cmd
{
    ///Random address to set
    struct bd_addr rand_addr;
} __attribute__((packed));

///HCI LE Set Advertising Parameters Command parameters structure
/*@TRACE*/
struct hci_le_set_adv_param_cmd
{
    ///Minimum interval for advertising
    uint16_t adv_intv_min;
    ///Maximum interval for advertising
    uint16_t adv_intv_max;
    ///Advertising type
    uint8_t adv_type;
    ///Own address type:  public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t own_addr_type;
    ///Peer address type: public=0 / random=1
    uint8_t peer_addr_type;
    ///Peer Bluetooth device address
    struct bd_addr peer_addr;
    ///Advertising channel map
    uint8_t adv_chnl_map;
    ///Advertising filter policy
    uint8_t adv_filt_policy;
} __attribute__((packed));

///HCI LE Set Advertising Data Command parameters structure
/*@TRACE*/
struct hci_le_set_adv_data_cmd
{
    ///Advertising data length
    uint8_t adv_data_len;
    ///Advertising data - maximum 31 bytes
    struct adv_data data;
} __attribute__((packed));

///HCI LE Set Scan Response Data Command parameters structure
/*@TRACE*/
struct hci_le_set_scan_rsp_data_cmd
{
    ///Scan response data length
    uint8_t scan_rsp_data_len;
    ///Scan response data - maximum 31 bytes
    struct scan_rsp_data data;
} __attribute__((packed));

///HCI LE Set Advertise Enable Command parameters structure
/*@TRACE*/
struct hci_le_set_adv_en_cmd
{
    ///Advertising enable - 0=disabled, 1=enabled
    uint8_t adv_en;
} __attribute__((packed));

///HCI LE Set Scan Parameters Command parameters structure
/*@TRACE*/
struct hci_le_set_scan_param_cmd
{
    ///Scan type - 0=passive / 1=active
    uint8_t scan_type;
    ///Scan interval
    uint16_t scan_intv;
    ///Scan window size
    uint16_t scan_window;
    ///Own address type - public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t own_addr_type;
    ///Scan filter policy
    uint8_t scan_filt_policy;
} __attribute__((packed));

///HCI LE Set Scan Enable Command parameters structure
/*@TRACE*/
struct hci_le_set_scan_en_cmd
{
    ///Scan enable - 0=disabled, 1=enabled
    uint8_t scan_en;
    ///Enable for duplicates filtering - 0 =disabled/ 1=enabled
    uint8_t filter_duplic_en;
} __attribute__((packed));

///HCI LE Create Connection Command parameters structure
/*@TRACE*/
struct hci_le_create_con_cmd
{
    ///Scan interval (N * 0.625 ms)
    uint16_t scan_intv;
    ///Scan window size (N * 0.625 ms)
    uint16_t scan_window;
    ///Initiator filter policy
    uint8_t init_filt_policy;
    ///Peer address type - public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t peer_addr_type;
    ///Peer BD address
    struct bd_addr peer_addr;
    ///Own address type - public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t own_addr_type;
    ///Minimum of connection interval (N * 1.25 ms)
    uint16_t con_intv_min;
    ///Maximum of connection interval (N * 1.25 ms)
    uint16_t con_intv_max;
    ///Connection latency
    uint16_t con_latency;
    ///Link supervision timeout
    uint16_t superv_to;
    ///Minimum CE length (N * 0.625 ms)
    uint16_t ce_len_min;
    ///Maximum CE length (N * 0.625 ms)
    uint16_t ce_len_max;
} __attribute__((packed));

///HCI LE Add Device to White List Command parameters structure
/*@TRACE*/
struct hci_le_add_dev_to_wlst_cmd
{
    ///Type of address of the device to be added to the White List - 0=public/1=random
    uint8_t dev_addr_type;
    ///Address of device to be added to White List
    struct bd_addr dev_addr;
} __attribute__((packed));

///HCI LE Remove Device from White List Command parameters structure
/*@TRACE*/
struct hci_le_rmv_dev_from_wlst_cmd
{
    ///Type of address of the device to be removed from the White List - 0=public/1=random
    uint8_t dev_addr_type;
    ///Address of device to be removed from White List
    struct bd_addr dev_addr;
} __attribute__((packed));

///HCI LE Set Extended Scan Parameters Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_scan_param_cmd
{
    ///Own address type public/random/rpa
    uint8_t own_addr_type;
    ///Scanning filter policy
    uint8_t scan_filt_policy;
    ///Indicates the PHY(s) on which the advertising packets should be received
    uint8_t scan_phys;

    ///Parameters for PHY(s)
    struct scan_phy_param
    {
        ///Scanning Type: passive/active
        uint8_t scan_type;
        ///Scan interval (slots)
        uint16_t scan_intv;
        ///Scan window size (slots)
        uint16_t scan_window;
    } phy[MAX_SCAN_PHYS];
} __attribute__((packed));

///HCI LE Set Extended Scan Enable Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_scan_en_cmd
{
    ///Scan enable - 0=disabled, 1=enabled
    uint8_t scan_en;
    ///Filter duplicates - 0=disabled, 1=enabled, 2=enabled & reset each scan period
    uint8_t filter_duplic_en;
    ///Scan duration (Time=N*10ms)
    uint16_t duration;
    ///Scan period (Time=N*1.28sec)
    uint16_t period;
} __attribute__((packed));

/*@TRACE*/
struct init_phy_param
{
    ///Scan interval (N * 0.625 ms)
    uint16_t scan_interval;
    ///Scan window size (N * 0.625 ms)
    uint16_t scan_window;
    ///Minimum of connection interval (N * 1.25 ms)
    uint16_t con_intv_min;
    ///Maximum of connection interval (N * 1.25 ms)
    uint16_t con_intv_max;
    ///Connection latency
    uint16_t con_latency;
    ///Link supervision timeout
    uint16_t superv_to;
    ///Minimum CE length (N * 0.625 ms)
    uint16_t ce_len_min;
    ///Maximum CE length (N * 0.625 ms)
    uint16_t ce_len_max;
} __attribute__((packed));

///HCI LE Extended Create Connection Command parameters structure
/*@TRACE*/
struct hci_le_ext_create_con_cmd
{
    ///Initiator filter policy
    uint8_t init_filter_policy;
    ///Own address type public/random/rpa
    uint8_t own_addr_type;
    ///Peer address type public/random/rpa
    uint8_t peer_addr_type;
    ///Peer address
    struct bd_addr peer_addr;
    ///Indicates the PHY(s) on which the advertising packets should be received
    uint8_t init_phys;
    ///Parameters for PHY(s)
    struct init_phy_param phy[MAX_INIT_PHYS];
} __attribute__((packed));

///HCI LE Periodic Advertising Create Sync Command parameters structure
/*@TRACE*/
struct hci_le_per_adv_create_sync_cmd
{
    ///Filter policy
    uint8_t filter_policy;
    ///Advertising SID
    uint8_t adv_sid;
    ///Advertising address type
    uint8_t adv_addr_type;
    ///Advertiser address
    struct bd_addr adv_addr;
    ///max Skip after receive
    uint16_t skip;
    ///Sync timeout (Time=N*10ms)
    uint16_t sync_to;
    ///Unused (supplemental type)
    uint8_t unused;
} __attribute__((packed));

///HCI LE Periodic Advertising Terminate Sync Command parameters structure
/*@TRACE*/
struct hci_le_per_adv_term_sync_cmd
{
    ///Sync handle
    uint16_t sync_handle;
} __attribute__((packed));

///HCI LE Add Device to Periodic Advertiser List Command parameters structure
/*@TRACE*/
struct hci_le_add_dev_to_per_adv_list_cmd
{
    ///Advertiser address type
    uint8_t adv_addr_type;
    ///Advertiser address
    struct bd_addr adv_addr;
    ///Advertising SID
    uint8_t adv_sid;
} __attribute__((packed));

///HCI LE Remove Device from Periodic Advertiser List Command parameters structure
/*@TRACE*/
struct hci_le_rmv_dev_from_per_adv_list_cmd
{
    ///Advertiser address type
    uint8_t adv_addr_type;
    ///Advertiser address
    struct bd_addr adv_addr;
    ///Advertising SID
    uint8_t adv_sid;
} __attribute__((packed));

///HCI LE Set Privacy Mode Command parameters structure
/*@TRACE*/
struct hci_le_set_priv_mode_cmd
{
    ///Peer identity address type
    uint8_t peer_addr_type;
    ///Peer identity address
    struct bd_addr peer_addr;
    ///Privacy mode
    uint8_t priv_mode;
} __attribute__((packed));

///HCI LE Set Host Channel Classification Command parameters structure
/*@TRACE*/
struct hci_le_set_host_ch_class_cmd
{
    ///Channel map
    struct le_chnl_map chmap;
} __attribute__((packed));

///HCI LE Receiver Test Command parameters structure
/*@TRACE*/
struct hci_le_rx_test_cmd
{
    ///RX frequency for Rx test
    uint8_t rx_freq;
} __attribute__((packed));

///HCI LE Transmitter Test Command parameters structure
/*@TRACE*/
struct hci_le_tx_test_cmd
{
    ///TX frequency for Tx test
    uint8_t tx_freq;
    ///TX test data length
    uint8_t test_data_len;
    ///TX test payload type - see enum
    uint8_t pk_payload_type;
} __attribute__((packed));

///HCI LE Encrypt Command parameters structure
/*@TRACE*/
struct hci_le_enc_cmd
{
    ///Long term key structure
    struct ltk key;
    ///Pointer to buffer with plain data to encrypt - 16 bytes
    uint8_t plain_data[16];
} __attribute__((packed));

/// HCI LE Connection Update Command parameters structure
/*@TRACE*/
struct hci_le_con_update_cmd
{
    ///Connection Handle
    uint16_t conhdl;
    ///Minimum of connection interval (units of 1.25 ms)
    uint16_t con_intv_min;
    ///Maximum of connection interval (units of 1.25 ms)
    uint16_t con_intv_max;
    ///Connection latency (units of connection event)
    uint16_t con_latency;
    ///Link supervision timeout (units of 10 ms)
    uint16_t superv_to;
    ///Minimum of CE length (units of 0.625 ms)
    uint16_t ce_len_min;
    ///Maximum of CE length (units of 0.625 ms)
    uint16_t ce_len_max;
} __attribute__((packed));

/// HCI LE Start Encryption Command parameters structure
/*@TRACE*/
struct hci_le_start_enc_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Random number - 8B
    struct rand_nb nb;
    ///Encryption Diversifier
    uint16_t enc_div;
    ///Long term key
    struct ltk ltk;
} __attribute__((packed));

/// HCI long term key request reply command parameters structure
/*@TRACE*/
struct hci_le_ltk_req_reply_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Long term key
    struct ltk ltk;
} __attribute__((packed));

/// HCI long term key request negative reply command parameters structure
/*@TRACE*/
struct hci_le_ltk_req_neg_reply_cmd
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI LE remote connection parameter request reply command parameters structure
/*@TRACE*/
struct hci_le_rem_con_param_req_reply_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Interval_Min
    uint16_t interval_min;
    ///Interval_Max
    uint16_t interval_max;
    ///Latency
    uint16_t latency;
    ///Timeout
    uint16_t timeout;
    ///Minimum_CE_Length (N * 0.625 ms)
    uint16_t min_ce_len;
    ///Maximum_CE_Length (N * 0.625 ms)
    uint16_t max_ce_len;
} __attribute__((packed));

/// HCI LE remote connection parameter request negative reply command parameters structure
/*@TRACE*/
struct hci_le_rem_con_param_req_neg_reply_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Reason
    uint8_t reason;
} __attribute__((packed));

/// HCI LE Set Data Length  Command parameters structure
/*@TRACE*/
struct hci_le_set_data_len_cmd
{
    ///Connection Handle
    uint16_t conhdl;
    ///Preferred maximum number of payload octets that the local Controller should include
    ///in a single Link Layer Data Channel PDU.
    uint16_t tx_octets;
    ///Preferred maximum number of microseconds that the local Controller should use to transmit
    ///a single Link Layer Data Channel PDU
    uint16_t tx_time;
} __attribute__((packed));

/// HCI LE Read Suggested Default Data Length Command
/*@TRACE*/
struct hci_le_wr_suggted_dft_data_len_cmd
{
    ///Suggested value for the Controller's maximum transmitted number of payload octets to be used
    uint16_t suggted_max_tx_octets;
    ///Suggested value for the Controller's maximum packet transmission time to be used
    uint16_t suggted_max_tx_time;
} __attribute__((packed));

/// HCI LE Add Device to Resolving List Command
/*@TRACE*/
struct hci_le_add_dev_to_rslv_list_cmd
{
    /// Peer Identity Address Type
    uint8_t peer_id_addr_type;
    /// Peer Identity Address
    struct bd_addr peer_id_addr;
    /// Peer IRK
    struct irk peer_irk;
    /// Local IRK
    struct irk local_irk;
} __attribute__((packed));

/// HCI LE Remove Device From Resolving List Command
/*@TRACE*/
struct hci_le_rmv_dev_from_rslv_list_cmd
{
    /// Peer Identity Address Type
    uint8_t peer_id_addr_type;
    /// Peer Identity Address
    struct bd_addr peer_id_addr;
} __attribute__((packed));

/// HCI LE Read Peer Resolvable Address Command
/*@TRACE*/
struct hci_le_rd_peer_rslv_addr_cmd
{
    /// Peer Identity Address Type
    uint8_t peer_id_addr_type;
    /// Peer Identity Address
    struct bd_addr peer_id_addr;
} __attribute__((packed));

/// HCI LE Read Local Resolvable Address Command
/*@TRACE*/
struct hci_le_rd_loc_rslv_addr_cmd
{
    /// Peer Identity Address Type
    uint8_t peer_id_addr_type;
    /// Peer Identity Address
    struct bd_addr peer_id_addr;
} __attribute__((packed));

/// HCI LE Set Address Resolution Enable Command
/*@TRACE*/
struct hci_le_set_addr_resol_en_cmd
{
    /// Address Resolution Enable
    uint8_t enable;
} __attribute__((packed));

/// HCI LE Set Resolvable Private Address Timeout Command
/*@TRACE*/
struct hci_le_set_rslv_priv_addr_to_cmd
{
    /// RPA Timeout
    uint16_t rpa_timeout;
} __attribute__((packed));

/*
 * HCI EVENTS PARAMETERS
 ****************************************************************************************
 */

/// HCI inquiry complete event structure
/*@TRACE*/
struct hci_inq_cmp_evt
{
    ///Status of the procedure
    uint8_t status;
} __attribute__((packed));

/// HCI Inquiry result event structure (with only 1 result)
/*@TRACE*/
struct hci_inq_res_evt
{
    ///Number of response
    uint8_t nb_rsp;
    ///BdAddr
    struct bd_addr bd_addr;
    ///Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    ///Reserved
    uint8_t reserved1;
    ///Reserved
    uint8_t reserved2;
    ///class of device
    struct devclass class_of_dev;
    ///Clock Offset
    uint16_t clk_off;
} __attribute__((packed));

/// HCI Inquiry result with rssi event structure (with only 1 result)
/*@TRACE*/
struct hci_inq_res_with_rssi_evt
{
    ///Number of response
    uint8_t nb_rsp;
    ///BdAddr
    struct bd_addr bd_addr;
    ///Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    ///Reserved
    uint8_t reserved1;
    ///class of device
    struct devclass class_of_dev;
    ///Clock Offset
    uint16_t clk_off;
    ///Rssi
    uint8_t rssi;
} __attribute__((packed));

/// HCI Extended inquiry result indication structure (with only 1 result)
/*@TRACE*/
struct hci_ext_inq_res_evt
{
    ///Number of response
    uint8_t nb_rsp;
    ///BdAddr
    struct bd_addr bd_addr;
    ///Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    ///Reserved
    uint8_t reserved1;
    ///class of device
    struct devclass class_of_dev;
    ///Clock Offset
    uint16_t clk_off;
    ///RSSi
    uint8_t rssi;
    ///Extended inquiry response data
    struct eir eir;
} __attribute__((packed));

/// HCI disconnect complete event structure
/*@TRACE*/
struct hci_disc_cmp_evt
{
    ///Status of received command
    uint8_t status;
    ///Connection Handle
    uint16_t conhdl;
    ///Reason for disconnection
    uint8_t reason;
} __attribute__((packed));

/// HCI basic command complete event structure
/*@TRACE
 * hci_dbg_wr_par_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_dbg_llcp_discard_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_reset_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_set_evt_mask_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_per_adv_en_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_create_con_cancel_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_evt_mask_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_host_ch_class_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_wr_suggted_dft_data_len_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_dft_phy_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_rmv_dev_from_wlst_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_adv_param_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_adv_data_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_scan_rsp_data_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_adv_en_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_scan_param_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_scan_en_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_rand_addr_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_addr_resol_en_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_set_rslv_priv_addr_to_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_rmv_dev_from_rslv_list_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_create_con_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_dbg_plf_reset_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * hci_le_wr_rf_path_comp_cmd_cmp_evt = hci_basic_cmd_cmp_evt
 * */
struct hci_basic_cmd_cmp_evt
{
    ///Status of the command reception
    uint8_t status;
} __attribute__((packed));

/// HCI basic command complete event structure with connection handle
/*@TRACE*/
struct hci_basic_conhdl_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI basic command complete event structure with BD address
struct hci_basic_bd_addr_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI basic command complete event structure with BD address
struct hci_basic_tcf_cmd_cmp_evt
{
    ///status
    uint8_t status;
    ///BdAddr
    struct tcf_info tcf_info;
} __attribute__((packed));

/// HCI basic event structure with status and BD address
struct hci_basic_stat_bd_addr_evt
{
    ///status
    uint8_t status;
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI basic event including a connection handle as parameter
struct hci_basic_conhdl_evt
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI complete event with status only.
/*@TRACE*/
struct hci_cmd_stat_event
{
    /// Status of the command reception
    uint8_t status;
} __attribute__((packed));

/// HCI number of packet complete event structure
/*@TRACE*/
struct hci_nb_cmp_pkts_evt
{
    /// number of handles
    uint8_t nb_of_hdl;
    /// connection handle
    uint16_t conhdl[1];
    /// number of completed packets
    uint16_t nb_comp_pkt[1];
} __attribute__((packed));

/// HCI data buffer overflow event structure
/*@TRACE*/
struct hci_data_buf_ovflw_evt
{
    ///Link type
    uint8_t link_type;
} __attribute__((packed));

/// HCI Hardware Error Event parameters structure
/*@TRACE*/
struct hci_hw_err_evt
{
    /// HW error code
    uint8_t hw_code;
} __attribute__((packed));

/// HCI encryption change event structure
struct hci_enc_change_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Encryption enabled information
    uint8_t enc_stat;
} __attribute__((packed));

/// HCI encryption key refresh complete event structure
/*@TRACE
 * hci_enc_key_refresh_evt = hci_enc_key_ref_cmp_evt */
struct hci_enc_key_ref_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI Authenticated Payload Timeout Expired Event structure
/*@TRACE*/
struct hci_auth_payl_to_exp_evt
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for create connection
/*@TRACE*/
struct hci_con_cmp_evt
{
    /// Status
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Bluetooth Device address
    struct bd_addr bd_addr;
    ///Link type
    uint8_t link_type;
    ///Encryption state
    uint8_t enc_en;
} __attribute__((packed));

/// HCI command complete event structure for qos setup
/*@TRACE*/
struct hci_qos_setup_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Flags
    uint8_t flags;
    ///Service type
    uint8_t serv_type;
    ///Token rate
    uint32_t tok_rate;
    ///Peak bandwidth
    uint32_t pk_bw;
    ///Latency
    uint32_t lat;
    ///Delay variation
    uint32_t del_var;
} __attribute__((packed));

/// HCI flow specification complete event parameters structure
/*@TRACE*/
struct hci_flow_spec_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Flags
    uint8_t flags;
    ///Flow direction
    uint8_t flow_dir;
    ///Service type
    uint8_t serv_type;
    ///Token rate
    uint32_t tk_rate;
    ///Token buffer size
    uint32_t tk_buf_sz;
    ///Peak bandwidth
    uint32_t pk_bw;
    ///Access latency
    uint32_t acc_lat;
} __attribute__((packed));

/// HCI role change event parameters structure
/*@TRACE*/
struct hci_role_chg_evt
{
    ///Status
    uint8_t status;
    ///BD address
    struct bd_addr bd_addr;
    ///New role
    uint8_t new_role;
} __attribute__((packed));

/// HCI complete event structure for the read clock offset command
/*@TRACE*/
struct hci_rd_clk_off_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Clock offset
    uint16_t clk_off_val;
} __attribute__((packed));

/// HCI event structure for the flush occurred event
/*@TRACE*/
struct hci_flush_occurred_evt
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI max slot change event structure
/*@TRACE*/
struct hci_max_slot_chg_evt
{
    ///Connection handle
    uint16_t conhdl;
    ///Max slot
    uint8_t max_slot;
} __attribute__((packed));

/// HCI sniff subrating event parameters structure
/*@TRACE*/
struct hci_sniff_sub_evt
{
    ///Status.
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Maximum transmit latency
    uint16_t max_lat_tx;
    ///Maximum receive latency
    uint16_t max_lat_rx;
    ///Minimum remote TO
    uint16_t min_rem_to;
    ///Minimum local TO
    uint16_t min_loc_to;
} __attribute__((packed));

/// HCI read remote extended features complete event parameters structure
/*@TRACE*/
struct hci_rd_rem_ext_feats_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///page number
    uint8_t pg_nb;
    ///page number max
    uint8_t pg_nb_max;
    ///ext LMP features
    struct features ext_feats;
} __attribute__((packed));

/// HCI read remote extended features complete event parameters structure
/*@TRACE*/
struct hci_rem_host_supp_feats_notif_evt
{
    ///BD address
    struct bd_addr bd_addr;
    ///ext lmp features
    struct features ext_feats;
} __attribute__((packed));

/// HCI command complete event structure for the read remote supported features command
/*@TRACE*/
struct hci_rd_rem_supp_feats_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Remote features
    struct features rem_feats;
} __attribute__((packed));

/// HCI command complete event structure for the read remote information version command
/*@TRACE*/
struct hci_rd_rem_ver_info_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///LMP version
    uint8_t vers;
    ///Manufacturer name
    uint16_t compid;
    ///LMP subversion
    uint16_t subvers;
} __attribute__((packed));

/// HCI encryption change event structure
/*@TRACE*/
struct hci_enc_chg_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Encryption enabled information
    uint8_t enc_stat;
} __attribute__((packed));

/// HCI mode change event structure
/*@TRACE*/
struct hci_mode_chg_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Current mode
    uint8_t cur_mode;
    /// Interval
    uint16_t interv;
} __attribute__((packed));

/// HCI simple pairing complete event structure
/*@TRACE*/
struct hci_sp_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Bluetooth Device address
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI Authentication complete event structure
/*@TRACE*/
struct hci_auth_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI change connection link key complete event structure
/*@TRACE*/
struct hci_chg_con_lk_cmp_evt
{
    ///Status
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI encryption key refresh complete event structure
/*@TRACE*/
struct hci_enc_key_refresh_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI master link key complete event structure
/*@TRACE*/
struct hci_master_lk_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Key flag
    uint8_t key_flag;
} __attribute__((packed));

/// HCI synchronous link connection complete event structure
/*@TRACE*/
struct hci_sync_con_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///BD address
    struct bd_addr bd_addr;
    ///Link type
    uint8_t lk_type;
    ///Transmit interval
    uint8_t tx_int;
    ///Retransmission window
    uint8_t ret_win;
    ///rx packet length
    uint16_t rx_pkt_len;
    ///tx packet length
    uint16_t tx_pkt_len;
    ///Air mode
    uint8_t air_mode;
} __attribute__((packed));

/// HCI synchronous connection change event structure
/*@TRACE*/
struct hci_sync_con_chg_evt
{
    ///Status for command reception
    uint8_t status;
    ///Synchronous Connection handle
    uint16_t sync_conhdl;
    ///Transmit interval
    uint8_t tx_int;
    ///Retransmission window
    uint8_t ret_win;
    ///rx packet length
    uint16_t rx_pkt_len;
    ///tx packet length
    uint16_t tx_pkt_len;
} __attribute__((packed));

/// HCI connection packet type change event structure
/*@TRACE*/
struct hci_con_pkt_type_chg_evt
{
    ///Status for command reception
    uint8_t status;
    ///Synchronous Connection handle
    uint16_t sync_conhdl;
    ///Synchronous packet type
    uint16_t pkt_type;
} __attribute__((packed));

/// HCI link supervision timeout change event structure
/*@TRACE*/
struct hci_link_supv_to_chg_evt
{
    ///Connection handle
    uint16_t conhdl;
    ///Link supervision timeout
    uint16_t lsto_val;
} __attribute__((packed));

/// HCI link key request event structure
/*@TRACE*/
struct hci_lk_req_evt
{
    ///BD address
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI encryption key refresh event structure
/*@TRACE*/
struct hci_enc_key_refresh_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI connection request event structure
/*@TRACE*/
struct hci_con_req_evt
{
    ///BD address
    struct bd_addr bd_addr;
    ///Class of device
    struct devclass classofdev;
    ///link type
    uint8_t lk_type;
} __attribute__((packed));

/// HCI quality of service violation event structure
/*@TRACE*/
struct hci_qos_viol_evt
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI io capability response event structure
/*@TRACE*/
struct hci_io_cap_rsp_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///IO capability
    uint8_t io_capa;
    ///OOB data present
    uint8_t oob_data_pres;
    ///Authentication requirements
    uint8_t auth_req;
} __attribute__((packed));

/// HCI IO capability response event structure
/*@TRACE*/
struct hci_io_cap_req_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI Return link keys event structure
/*@TRACE*/
struct hci_return_link_keys_evt
{
    ///Number of Keys
    uint8_t num_keys;
    ///BdAddr
    struct bd_addr bd_addr;
    ///Key
    struct ltk key;
} __attribute__((packed));

/// HCI pin code request event structure
/*@TRACE*/
struct hci_pin_code_req_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI user passkey request event structure
/*@TRACE*/
struct hci_user_passkey_req_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI user passkey notification event structure
/*@TRACE*/
struct hci_user_passkey_notif_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Passkey
    uint32_t passkey;
} __attribute__((packed));

/// HCI remote OOB data request event structure
/*@TRACE*/
struct hci_rem_oob_data_req_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

/// HCI user confirmation request event structure
/*@TRACE*/
struct hci_user_cfm_req_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Passkey
    uint32_t passkey;
} __attribute__((packed));

/// HCI keypress notification event structure
/*@TRACE*/
struct hci_keypress_notif_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///type
    uint8_t type;
} __attribute__((packed));

/// HCI link key notification event structure
/*@TRACE*/
struct hci_lk_notif_evt
{
    ///BdAddr
    struct bd_addr bd_addr;
    ///Key
    struct ltk key;
    ///type
    uint8_t key_type;
} __attribute__((packed));

/// HCI SAM status change event structure
struct hci_sam_status_change_evt
{
    ///Connection handle
    uint16_t conhdl;
    ///Local SAM index
    uint8_t loc_idx;
    ///Local SAM TX availability
    uint8_t loc_tx_av;
    ///Local SAM RX availability
    uint8_t loc_rx_av;
    ///Remote SAM index
    uint8_t rem_idx;
    ///Remote SAM TX availability
    uint8_t rem_tx_av;
    ///Remote SAM RX availability
    uint8_t rem_rx_av;
} __attribute__((packed));

/*
 * HCI LE META EVENTS PARAMETERS
 ****************************************************************************************
 */


// LE event structures

/// HCI command complete event structure for the Read Local Supported Features
/*@TRACE*/
struct hci_le_rd_local_supp_feats_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Local LE supported features
    struct le_features feats;
} __attribute__((packed));

/// HCI command complete event structure for the Read Advertising Channel Tx Power Command
/*@TRACE
 * hci_le_rd_adv_chnl_tx_pw_cmd_cmp_evt = hci_rd_adv_chnl_tx_pw_cmd_cmp_evt*/
struct hci_rd_adv_chnl_tx_pw_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Advertising channel Tx power level
    int8_t adv_tx_pw_lvl;
} __attribute__((packed));

///HCI command complete event structure for the Read White List Size Command
/*@TRACE*/
struct hci_le_rd_wlst_size_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///White List size
    uint8_t wlst_size;
} __attribute__((packed));

///HCI command complete event structure for the Read Buffer Size Command
/*@TRACE*/
struct hci_le_rd_buff_size_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///ACL data packet length that can be sent from host to controller
    uint16_t hc_data_pk_len;
    ///Total number of ACL data packets that can be sent from host to controller.
    uint8_t hc_tot_nb_data_pkts;
} __attribute__((packed));

///HCI command complete event structure for LE Rand Command
/*@TRACE*/
struct hci_le_rand_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Random number
    struct rand_nb nb;
} __attribute__((packed));

///HCI command complete event structure for Read Supported States Command
/*@TRACE*/
struct hci_le_rd_supp_states_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///LE supported states response
    struct le_states states;
} __attribute__((packed));

///HCI command complete event structure for Test End
/*@TRACE
 *  * hci_le_test_end_cmd_cmp_evt = hci_test_end_cmd_cmp_evt*/
struct hci_test_end_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Number of RX packets - null if TX test was the ended one
    uint16_t nb_packet_received;
} __attribute__((packed));

///HCI LE Encrypt complete event structure
/*@TRACE*/
struct hci_le_enc_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Encrypted data to return to command source.
    uint8_t encrypted_data[ENC_DATA_LEN];
} __attribute__((packed));

#if BLE_EMB_PRESENT || BLE_HOST_PRESENT
///HCI LE advertising report event structure
/*@TRACE*/
struct hci_le_adv_report_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Number of advertising reports in this event
    uint8_t nb_reports;
    ///Advertising reports structures array
    struct adv_report adv_rep[BLE_ADV_REPORTS_MAX];
} __attribute__((packed));

///HCI LE extended advertising report event structure
/*@TRACE*/
struct hci_le_ext_adv_report_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Number of advertising reports in this event
    uint8_t nb_reports;
    ///Parameters for each report
    struct ext_adv_report adv_rep[BLE_ADV_REPORTS_MAX];
} __attribute__((packed));

///HCI LE periodic advertising sync established event structure
/*@TRACE
 * hci_le_periodic_adv_sync_est_evt = hci_le_per_adv_sync_est_evt*/
struct hci_le_per_adv_sync_est_evt
{
    ///LE Subevent code
    uint8_t subcode;
    /// Status of the advertising sync
    uint8_t status;
    /// Sync Handle to be used
    uint16_t sync_handle;
    /// Advertising SID
    uint8_t adv_sid;
    ///Advertising address type: public/random
    uint8_t adv_addr_type;
    ///Advertising address value
    struct bd_addr adv_addr;
    /// Advertiser PHY
    uint8_t phy;
    /// Advertising interval (Time=N*1.25ms)
    uint16_t interval;
    /// Advertiser clock accuracy
    uint8_t adv_ca;
} __attribute__((packed));

///HCI LE periodic advertising report event structure
/*@TRACE
 * hci_le_periodic_adv_report_evt = hci_le_per_adv_report_evt*/
struct hci_le_per_adv_report_evt
{
    ///LE Subevent code
    uint8_t subcode;
    /// Sync Handle to be used
    uint16_t sync_handle;
    /// Tx Power
    uint8_t tx_power;
    /// RSSI
    uint8_t rssi;
    ///Unused
    uint8_t unused;
    /// Data Status
    uint8_t status;
    ///Data length in advertising packet
    uint8_t data_len;
    ///Data of advertising packet
    uint8_t data[PER_ADV_DATA_MAX_LEN];
} __attribute__((packed));

///HCI LE periodic advertising sync lost event structure
/*@TRACE
 * hci_le_periodic_adv_sync_lost_evt = hci_le_per_adv_sync_lost_evt*/
struct hci_le_per_adv_sync_lost_evt
{
    ///LE Subevent code
    uint8_t subcode;
    /// Sync Handle to be used
    uint16_t sync_handle;
} __attribute__((packed));

///HCI LE scan timeout event structure
/*@TRACE*/
struct hci_le_scan_timeout_evt
{
    ///LE Subevent code
    uint8_t subcode;
} __attribute__((packed));

#endif //BLE_EMB_PRESENT || BLE_HOST_PRESENT

/// HCI command complete event structure for Read Channel Map Command
/*@TRACE*/
struct hci_le_rd_chnl_map_cmd_cmp_evt
{
    ///Status of command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Channel map
    struct le_chnl_map ch_map;
} __attribute__((packed));

/// HCI command complete event structure for Long Term Key Request Reply Command
/*@TRACE*/
struct hci_le_ltk_req_reply_cmd_cmp_evt
{
    ///Status of command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for Long Term Key Request Negative Reply Command
/*@TRACE*/
struct hci_le_ltk_req_neg_reply_cmd_cmp_evt
{
    ///Status of command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for LE Read Suggested Default Data Length Command
/*@TRACE*/
struct hci_le_rd_suggted_dft_data_len_cmd_cmp_evt
{
    ///Status of command reception
    uint8_t status;
    ///Host's suggested value for the Controller's maximum transmitted number of payload octets
    uint16_t suggted_max_tx_octets;
    ///Host's suggested value for the Controller's maximum packet transmission time
    uint16_t suggted_max_tx_time;
} __attribute__((packed));

/// HCI command complete event structure for LE Read Maximum Data Length Command
/*@TRACE*/
struct hci_le_rd_max_data_len_cmd_cmp_evt
{
    ///Status of command reception
    uint8_t status;
    ///Maximum number of payload octets that the local Controller supports for transmission
    uint16_t suppted_max_tx_octets;
    ///Maximum time, in microseconds, that the local Controller supports for transmission
    uint16_t suppted_max_tx_time;
    ///Maximum number of payload octets that the local Controller supports for reception
    uint16_t suppted_max_rx_octets;
    ///Maximum time, in microseconds, that the local Controller supports for reception
    uint16_t suppted_max_rx_time;
} __attribute__((packed));

/// HCI LE Read Peer Resolvable Address Command Complete Event
/*@TRACE*/
struct hci_le_rd_peer_rslv_addr_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Peer Resolvable Address
    struct bd_addr peer_rslv_addr;
} __attribute__((packed));

/// HCI LE Read Local Resolvable Address Command Complete Event
/*@TRACE*/
struct hci_le_rd_loc_rslv_addr_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Local Resolvable Address
    struct bd_addr loc_rslv_addr;
} __attribute__((packed));

/// HCI LE Read Resolving List Size Command Complete Event
/*@TRACE*/
struct hci_le_rd_rslv_list_size_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Resolving List Size
    uint8_t size;
} __attribute__((packed));

/// HCI write authenticated payload timeout command
/*@TRACE*/
struct hci_wr_auth_payl_to_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Authenticated payload timeout (N*10ms)
    uint16_t auth_payl_to;
} __attribute__((packed));

/// HCI command complete event structure for the Write Authenticated Payload Timeout Command
/*@TRACE*/
struct hci_wr_auth_payl_to_cmd_cmp_evt
{
    /// Status of the command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for HCI LE Connection Update Command
/*@TRACE*/
struct hci_le_con_update_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Connection interval value
    uint16_t con_interval;
    ///Connection latency value
    uint16_t con_latency;
    ///Supervision timeout
    uint16_t sup_to;
} __attribute__((packed));

/// HCI command complete event structure for create connection
/*@TRACE*/
struct hci_le_con_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Device role - 0=Master/ 1=Slave
    uint8_t role;
    ///Peer address type - 0=public/1=random
    uint8_t peer_addr_type;
    ///Peer address
    struct bd_addr peer_addr;
    ///Connection interval
    uint16_t con_interval;
    ///Connection latency
    uint16_t con_latency;
    ///Link supervision timeout
    uint16_t sup_to;
    ///Master clock accuracy
    uint8_t clk_accuracy;
} __attribute__((packed));

/// HCI LE read remote used feature command parameters structure
/*@TRACE*/
struct hci_le_rd_rem_feats_cmd
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for HCI LE read remote feature Command
/*@TRACE*/
struct hci_le_rd_rem_feats_cmd_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Le Features
    struct le_features le_feats;
} __attribute__((packed));

/// HCI command structure for the read transmit power level command
/*@TRACE*/
struct hci_rd_tx_pwr_lvl_cmd
{
    ///Connection handle
    uint16_t conhdl;
    ///Power Level type: current or maximum
    uint8_t type;
} __attribute__((packed));

/// HCI command complete event structure for the read transmit power level command
/*@TRACE*/
struct hci_rd_tx_pwr_lvl_cmd_cmp_evt
{
    ///Status for command reception
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Value of TX power level
    uint8_t tx_pow_lvl;
} __attribute__((packed));

/// HCI read remote information version command parameters structure
/*@TRACE*/
struct hci_rd_rem_ver_info_cmd
{
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI LE remote connection parameter request event
/*@TRACE*/
struct hci_le_rem_con_param_req_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Connection handle
    uint16_t conhdl;
    ///Interval_Min
    uint16_t interval_min;
    ///Interval_Max
    uint16_t interval_max;
    ///Latency
    uint16_t latency;
    ///Timeout
    uint16_t timeout;
} __attribute__((packed));

/// HCI command complete event structure for enhance create connection
/*@TRACE*/
struct hci_le_enh_con_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///Device role - 0=Master/ 1=Slave
    uint8_t role;
    ///Peer address type - 0=public/1=random
    uint8_t peer_addr_type;
    ///Peer address
    struct bd_addr peer_addr;
    ///Local Resolvable Private Address
    struct bd_addr loc_rslv_priv_addr;
    ///Peer Resolvable Private Address
    struct bd_addr peer_rslv_priv_addr;
    ///Connection interval
    uint16_t con_interval;
    ///Connection latency
    uint16_t con_latency;
    ///Link supervision timeout
    uint16_t sup_to;
    ///Master clock accuracy
    uint8_t clk_accuracy;
} __attribute__((packed));

struct hci_generate_dhkey_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    /// The 32 byte Diffie Helman Key
    uint8_t dh_key[32];
} __attribute__((packed));

struct hci_rd_local_p256_public_key_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    /// The 32 byte Diffie Helman Key
    uint8_t public_key[64];
} __attribute__((packed));

#if BLE_EMB_PRESENT || BLE_HOST_PRESENT
/// HCI LE Direct Advertising Report Event
/*@TRACE*/
struct hci_le_dir_adv_rep_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Number of reports
    uint8_t nb_reports;
    ///Direct Advertising reports structures array
    struct dir_adv_report adv_rep[BLE_ADV_REPORTS_MAX];
} __attribute__((packed));
#endif //BLE_EMB_PRESENT || BLE_HOST_PRESENT

/// Connected LE event
struct hci_le_con_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Connection handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI command complete event structure for HCI LE read remote used feature Command
/*@TRACE*/
struct hci_le_ltk_request_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Connection handle
    uint16_t conhdl;
    ///Random number
    struct rand_nb rand;
    ///Encryption diversifier
    uint16_t ediv;
} __attribute__((packed));

/// HCI LE META event LE Data Length Change Event
/*@TRACE*/
struct hci_le_data_len_chg_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Connection handle
    uint16_t conhdl;
    ///The maximum number of payload octets in TX
    uint16_t max_tx_octets;
    ///The maximum time that the local Controller will take to TX
    uint16_t max_tx_time;
    ///The maximum number of payload octets in RX
    uint16_t max_rx_octets;
    ///The maximum time that the local Controller will take to RX
    uint16_t max_rx_time;
} __attribute__((packed));

/// HCI Synchronization Train Received Event
/*@TRACE*/
struct hci_sync_train_rec_evt
{
    /// Status
    uint8_t status;
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// Clock_Offset (28 bits) - (CLKNslave - CLK) modulo 2^28
    uint32_t clock_offset;
    /// AFH_Channel_Map
    struct chnl_map afh_ch_map;
    /// LT_ADDR
    uint8_t lt_addr;
    /// Next_Broadcast_Instant (28 bits)
    uint32_t next_bcst_instant;
    /// Connectionless_Slave_Broadcast_Interval (in slots)
    uint16_t csb_int;
    /// Service_Data
    uint8_t service_data;
} __attribute__((packed));

/// HCI Connectionless Slave Broadcast Receive Event
/*@TRACE*/
struct hci_con_slv_bcst_rec_evt
{
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// LT_ADDR
    uint8_t lt_addr;
    /// CLK (28 bits)
    uint32_t clk;
    /// Offset (28 bits) - (CLKNslave - CLK) modulo 2^28
    uint32_t offset;
    /// Receive Status
    uint8_t receive_status;
    /// Fragment
    uint8_t fragment;
    /// Data_Length (in bytes)
    uint8_t data_length;
    /// Data
    uint8_t data[__ARRAY_EMPTY];
} __attribute__((packed));

/// HCI Connectionless Slave Broadcast Timeout Event
/*@TRACE*/
struct hci_con_slv_bcst_to_evt
{
    /// BD_ADDR
    struct bd_addr bd_addr;
    /// LT_ADDR
    uint8_t lt_addr;
} __attribute__((packed));

/// HCI Connectionless Slave Broadcast Channel Map Change Event
/*@TRACE*/
struct hci_con_slv_bcst_ch_map_chg_evt
{
    /// Channel_Map
    struct chnl_map ch_map;
} __attribute__((packed));

/*@TRACE
 * hci_le_gen_dhkey_cmp_evt = hci_le_generate_dhkey_cmp_evt*/
struct hci_le_generate_dhkey_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    uint8_t status;
    uint8_t dh_key[32];
} __attribute__((packed));

/*@TRACE
 * hci_le_rd_loc_p256_pub_key_cmp_evt = hci_le_generate_p256_public_key_cmp_evt*/
struct hci_le_generate_p256_public_key_cmp_evt
{
    ///LE Subevent code
    uint8_t      subcode;
    uint8_t      status;
    t_public_key public_key;
} __attribute__((packed));

/*
 * HCI VENDOR SPECIFIC COMMANDS PARAMETERS
 ****************************************************************************************
 */

/// Buffer structure
/*@TRACE*/
struct buffer_tag
{
    /// length of buffer
    uint8_t length;
    /// data of 128 bytes length
    uint8_t data[128];
} __attribute__((packed));

///HCI Debug write memory variable command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_set_bt_addr_cmd
{
    ///BdAddr
    struct bd_addr bd_addr;
} __attribute__((packed));

///HCI set bt links max number - vendor specific
/*@TRACE*/
struct hci_set_link_max_nb_cmd
{
    ///max bt link number
    uint8_t link_max_num;
} __attribute__((packed));

///HCI set LE feature support state - vendor specific
/*@TRACE*/
struct hci_set_le_feat_supp_cmd
{
    /// 0x00 disable LE supported feature reporting, 0x01 enable
    uint8_t le_feat_supp;
} __attribute__((packed));

///HCI set local version info - vendor specific
/*@TRACE*/
struct hci_set_local_ver_info_cmd
{
    ///HCI version number
    uint8_t hci_ver;
    ///HCI revision number
    uint16_t hci_rev;
    ///LMP version
    uint8_t lmp_ver;
    ///manufacturer name
    //    uint16_t manuf_name;
    ///LMP Subversion
    uint16_t lmp_subver;
} __attribute__((packed));

/// Common structure for Command Complete Event of HCI Debug Read Memory/Flash/Param complete event parameters - vendor specific
/*@TRACE
 * hci_dbg_rd_mem_cmd_cmp_evt = hci_dbg_basic_rd_data_cmd_cmp_evt
 * hci_dbg_rd_flash_cmd_cmp_evt = hci_dbg_basic_rd_data_cmd_cmp_evt
 * hci_dbg_rd_par_cmd_cmp_evt = hci_dbg_basic_rd_data_cmd_cmp_evt
 * */
struct hci_dbg_basic_rd_data_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///buffer structure to return
    struct buffer_tag buf;
} __attribute__((packed));

///HCI Debug read memory variable command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_rd_mem_cmd
{
    ///Start address to read
    uint32_t start_addr;
    ///Access size
    uint8_t type;
    ///Length to read
    uint8_t length;
} __attribute__((packed));

///HCI Debug write memory variable command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_wr_mem_cmd
{
    ///Start address to read
    uint32_t start_addr;
    ///Access size
    uint8_t type;
    ///buffer structure to return
    struct buffer_tag buf;
} __attribute__((packed));

///HCI Debug delete parameter command parameters - vendor specific
/*@TRACE
 * hci_dbg_del_par_cmd = hci_dbg_del_param_cmd*/
struct hci_dbg_del_param_cmd
{
    ///Parameter tag
    uint16_t param_tag;
} __attribute__((packed));

///HCI Debug erase flash command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_er_flash_cmd
{
    ///Flash type
    uint8_t flashtype;
    ///Start offset address
    uint32_t startoffset;
    ///Size to erase
    uint32_t size;
} __attribute__((packed));

///HCI Debug write flash command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_wr_flash_cmd
{
    ///Flash type
    uint8_t flashtype;
    ///Start offset address
    uint32_t startoffset;
    ///buffer structure
    struct buffer_tag buf;
} __attribute__((packed));

///HCI Debug read flash command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_rd_flash_cmd
{
    ///Flash type
    uint8_t flashtype;
    ///Start offset address
    uint32_t startoffset;
    ///Size to read
    uint8_t size;
} __attribute__((packed));

///HCI Debug read parameter command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_rd_par_cmd
{
    ///Parameter tag
    uint16_t param_tag;
} __attribute__((packed));

///HCI Debug read parameters command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_wr_par_cmd
{
    ///Parameter tag
    uint16_t param_tag;
    ///Structure buffer
    struct buffer_tag buf;
} __attribute__((packed));

#if CRYPTO_UT
///HCI Debug Test cryptographic functions command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_crypto_func_cmd
{
    /// Id of Function to be tested
    uint8_t function;
    /// Structure buffer
    struct buffer_tag buf;
} __attribute__((packed));
#endif //CRYPTO_UT

#if RW_DEBUG
///HCI Debug Test scheduling planner set function command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_set_cmd
{
    /// Activity identifier
    uint32_t id;
    /// Interval (in slots)
    uint32_t interval;
    /// Offset (in slots)
    uint32_t offset;
    /// Minimum duration (in slots)
    uint32_t duration_min;
    /// Maximum duration (in slots)
    uint32_t duration_max;
    /// Indicate activity is movable
    bool mobility_level;
} __attribute__((packed));

///HCI Debug Test scheduling planner set function command complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_set_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Activity identifier
    uint32_t moved_id;
} __attribute__((packed));

///HCI Debug Test scheduling planner remove function command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_rem_cmd
{
    /// Activity identifier
    uint32_t id;
} __attribute__((packed));

///HCI Debug Test scheduling planner check function command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_chk_cmd
{
    /// Activity identifier
    uint32_t id;
    /// Interval (in slots)
    uint32_t interval;
    /// Offset (in slots)
    uint32_t offset;
    /// Minimum duration (in slots)
    uint32_t duration_min;
} __attribute__((packed));

///HCI Debug Test scheduling planner request function command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_req_cmd
{
    /// Activity identifier
    uint32_t id;
    /// Minimum interval (in slots)
    uint32_t interval_min;
    /// Maximum interval (in slots)
    uint32_t interval_max;
    /// Minimum duration (in slots)
    uint32_t duration_min;
    /// Maximum duration (in slots)
    uint32_t duration_max;
    /// Period (in slots)
    uint8_t period;
} __attribute__((packed));

///HCI Debug Test scheduling planner request function command complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_test_sch_plan_req_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Interval (in slots)
    uint32_t interval;
    /// Minimum offset (in slots)
    uint32_t offset_min;
    /// Maximum offset (in slots)
    uint32_t offset_max;
} __attribute__((packed));
#endif //RW_DEBUG

///HCI Debug Read Kernel Statistics complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_rd_ke_stats_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///Max message sent
    uint8_t max_msg_sent;
    ///Max message saved
    uint8_t max_msg_saved;
    ///Max timer used
    uint8_t max_timer_used;
    ///Max heap used
    uint16_t max_heap_used;
    ///Max stack used
    uint16_t max_stack_used;
} __attribute__((packed));

/// HCI Debug Read information about memory usage. - vendor specific
/*@TRACE*/
struct hci_dbg_rd_mem_info_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// memory size currently used into each heaps.
    uint16_t mem_used[KE_MEM_BLOCK_MAX];
    /// peak of memory usage measured
    uint32_t max_mem_used;
} __attribute__((packed));

///HCI Debug identify Flash command complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_id_flash_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    ///Flash identity
    uint8_t flash_id;
} __attribute__((packed));

///HCI Debug RF Register read command
/*@TRACE*/
struct hci_dbg_rf_reg_rd_cmd
{
    /// register address
    uint16_t addr;
} __attribute__((packed));

///HCI Debug RF Register read command complete event
/*@TRACE*/
struct hci_dbg_rf_reg_rd_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// register address
    uint16_t addr;
    /// register value
    uint32_t value;
} __attribute__((packed));

///HCI Debug RF Register write command
/*@TRACE*/
struct hci_dbg_rf_reg_wr_cmd
{
    /// register address
    uint16_t addr;
    /// register value
    uint32_t value;
} __attribute__((packed));

///HCI Debug RF Register write command complete event
/*@TRACE*/
struct hci_dbg_rf_reg_wr_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// address
    uint16_t addr;
} __attribute__((packed));

///HCI Debug platform reset command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_plf_reset_cmd
{
    /// reason
    uint8_t reason;
} __attribute__((packed));

#if (RW_DEBUG && BT_EMB_PRESENT)
/// Send LMP Packets
/*@TRACE*/
struct hci_dbg_bt_send_lmp_cmd
{
    /// Connection handle
    uint16_t conhdl;
    ///buffer structure to return
    struct buffer_tag buf;
} __attribute__((packed));

/// Discard LMP Packets
/*@TRACE*/
struct hci_dbg_bt_discard_lmp_en_cmd
{
    /// Connection handle
    uint16_t conhdl;
    /// Enable/Disable LMP discard (0: disable / 1: enable)
    uint8_t enable;
} __attribute__((packed));

/// Set local clock
/*@TRACE*/
struct hci_dbg_set_local_clock_cmd
{
    /// Clock (in half-slots)
    uint32_t clock;
} __attribute__((packed));
#endif //(RW_DEBUG && BT_EMB_PRESENT)

#if (RW_WLAN_COEX)
///HCI Debug wlan coexistence command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_wlan_coex_cmd
{
    /// State
    uint8_t state;
} __attribute__((packed));
#if (RW_WLAN_COEX_TEST)
///HCI Debug wlan coexistence test scenario command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_wlan_coextst_scen_cmd
{
    /// Scenario
    uint32_t scenario;
} __attribute__((packed));
#endif //RW_WLAN_COEX_TEST
#endif //RW_WLAN_COEX

#if (RW_MWS_COEX)
///HCI Debug mws coexistence command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_mws_coex_cmd
{
    /// State
    uint8_t state;
} __attribute__((packed));
#if (RW_MWS_COEX_TEST)
///HCI Debug mws coexistence test scenario command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_mws_coextst_scen_cmd
{
    /// Scenario
    uint32_t scenario;
} __attribute__((packed));
#endif //RW_MWS_COEX_TEST
#endif //RW_MWS_COEX

#if (BLE_ISO_MODE_0)
/// HCI VS Setup Audio Mode 0 channel command parameters
/*@TRACE*/
struct hci_vs_setup_am0_chan_cmd
{
    /// Handle that identify an ACL link between a Master and a Slave device that is requesting
    /// an Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t conhdl;

    // *** Data path settings ***
    /// Host to Controller nominal data rate in octets per second
    uint32_t tx_bandwidth;
    /// Controller to Host nominal data rate in octets per second
    uint32_t rx_bandwidth;
    /// Host to Controller Data path type
    /// 0x00       Disabled
    /// 0x01       HCI
    /// 0x02-0xFE  Logical_Channel_Number. The meaning of the logical channels will be vendor specific.
    /// 0xFF       Test Mode
    uint8_t tx_data_path;
    /// Controller to Host Data path type
    /// 0x00       Disabled
    /// 0x01       HCI
    /// 0x02-0xFE  Logical_Channel_Number. The meaning of the logical channels will be vendor specific.
    /// 0xFF       Test Mode
    uint8_t rx_data_path;
    /// The number of bits in each unit of data received from the Host over the data transport. (Range 0x01 - 0xFB)
    uint8_t tx_size;
    /// The number of bits in each unit of data sent to the Host over the data transport. (Range 0x01 - 0xFB)
    uint8_t rx_size;
} __attribute__((packed));

/// Used to read the maximum size of the data portion of isochronous packets
/// no parameter on command, only need to specify returned parameters
struct hci_vs_setup_am0_chan_cmd_cmp_evt
{
    /// 0x00 - Request succeed ; 0x01-0xFF Failed reason
    uint8_t status;
    /// Handle that identify an ACL link between a Master and a Slave device that is requesting
    /// an Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t conhdl;
    /// Channel_Handle to be used to identify an Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t am0_hdl;
} __attribute__((packed));

/// Removes existing Audio Mode 0 channel command parameters
/*@TRACE*/
struct hci_vs_remove_am0_chan_cmd
{
    /// Channel_Handles used to identify Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t am0_hdl;
} __attribute__((packed));

/// Used to Control the channel that will b.e part of an Audio Mode 0 Connection
/*@TRACE*/
struct hci_vs_control_am0_chan_cmd
{
    /// Channel_Handles used to identify Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t am0_hdl;
    /// Control if the stream should be Enabled (0x01) or Disabled (0x00)
    uint8_t enable;
    /// Control if slave audio source is enabled or not
    uint8_t slv_src_enable;
} __attribute__((packed));

/// HCI basic command complete event structure with AM0 Channel handle
/*@TRACE*/
struct hci_vs_basic_am0_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// Channel_Handles used to identify Audio Mode 0 Channel (range 0x0000-0x0EFF)
    uint16_t am0_hdl;
} __attribute__((packed));
#endif // (BLE_ISO_MODE_0)


///HCI Debug HW Register Read command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_ble_reg_rd_cmd
{
    /// register address
    uint16_t reg_addr;
} __attribute__((packed));

///HCI Debug HW Register write command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_ble_reg_wr_cmd
{
    /// register address
    uint16_t reg_addr;
    /// extra parameter
    uint16_t reserved;
    /// register value
    uint32_t reg_value;
} __attribute__((packed));

///HCI Debug HW Register Read Complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_ble_reg_rd_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// register address
    uint16_t reg_addr;
    /// register value
    uint32_t reg_value;
} __attribute__((packed));

///HCI Debug HW Register Write Complete event parameters - vendor specific
/*@TRACE*/
struct hci_dbg_ble_reg_wr_cmd_cmp_evt
{
    /// status
    uint8_t status;
    /// register address
    uint16_t reg_addr;
} __attribute__((packed));

#if (BLE_EMB_PRESENT)
///HCI Vendor Specific Set Preferred Slave Latency command parameters
/*@TRACE*/
struct hci_vs_set_pref_slave_latency_cmd
{
    /// Connection handle
    uint16_t conhdl;
    /// Preferred latency (in number of connection events)
    uint16_t latency;
} __attribute__((packed));

///HCI Vendor Specific Set Preferred Slave event duration command parameters
/*@TRACE*/
struct hci_vs_set_pref_slave_evt_dur_cmd
{
    /// Connection handle
    uint16_t conhdl;
    /// Preferred event duration (N * 0.625 ms)
    uint16_t duration;
    /// Slave transmits a single packet per connection event (False/True)
    uint8_t single_tx;
} __attribute__((packed));

/// Send LLCP Packets
/*@TRACE*/
struct hci_dbg_send_llcp_cmd
{
    /// Connection handle
    uint16_t conhdl;
    ///buffer structure to return
    struct buffer_tag buf;
} __attribute__((packed));

///HCI Debug LLC discard command parameters - vendor specific
/*@TRACE*/
struct hci_dbg_llcp_discard_cmd
{
    /// Handle pointing to the connection for which LLCP commands have to be discarded
    uint16_t conhdl;
    /// Flag indicating if the discarding has to be enabled or disabled
    uint8_t enable;
} __attribute__((packed));
#endif //BLE_EMB_PRESENT

#if (RW_DEBUG)
/// HCI DBG Meta Event indicating a SW assertion
//TODO[AAL] exception (associated to HCI_DBG_EVT)
/*@TRACE*/
struct hci_dbg_assert_evt
{
    ///DBG Subevent code
    uint8_t subcode;
    /// Assert type (warning/error)
    //@trc_ref assert_type
    uint8_t type;
    /// Line number
    uint32_t line;
    /// Param0
    uint32_t param0;
    /// Param1
    uint32_t param1;
    /// File name
    uint8_t file[__ARRAY_EMPTY];
} __attribute__((packed));
#endif //(RW_DEBUG)

#if (BLE_ISOGEN)
/// Event is used to provide statistics about ISO Gen
/*@TRACE*/
struct hci_vs_isogen_stat_evt
{
    /// VS Subevent code
    uint8_t subcode;

    /// ISO Handle of the isochronous channel (Range 0x0000-0x0EFF)
    uint16_t iso_hdl;

    /// Statistics - Number of transmission attempts
    uint32_t nb_tx;
    /// Statistics - Number of transmission attempts that succeed
    uint32_t nb_tx_ok;
    /// Statistics - Number of Not granted packet packets
    uint32_t nb_tx_not_granted;

    /// Statistics - Number of reception attempt
    uint32_t nb_rx;
    /// Statistics - Number of reception attempts that succeed
    uint32_t nb_rx_ok;
    /// Statistics - Number of Not granted packet packets
    uint32_t nb_rx_not_granted;
    /// Statistics - Number of wrongly received packet (invalid data)
    uint32_t nb_rx_data_err;
    /// Statistics - Number of CRC Errors
    uint32_t nb_rx_crc_err;
    /// Statistics - Number of SYNC Errors
    uint32_t nb_rx_sync_err;
    /// Statistics - Number of received empty packets
    uint32_t nb_rx_empty;
} __attribute__((packed));
#endif // (BLE_ISOGEN)


/// HCI LE Read PHY command
/*@TRACE*/
struct hci_le_rd_phy_cmd
{
    /// Connection Handle
    uint16_t conhdl;
} __attribute__((packed));

/// HCI LE Set Default PHY Command
/*@TRACE*/
struct hci_le_set_dft_phy_cmd
{
    /// Preferred PHYS selection
    uint8_t all_phys;
    /// Preferred PHYS for TX
    uint8_t tx_phys;
    /// Preferred PHYS for RX
    uint8_t rx_phys;
} __attribute__((packed));

/// HCI LE Set PHY Command
/*@TRACE*/
struct hci_le_set_phy_cmd
{
    /// Connection Handle
    uint16_t conhdl;
    /// Preferred PHYS selection
    uint8_t all_phys;
    /// Preferred PHYS for TX
    uint8_t tx_phys;
    /// Preferred PHYS for RX
    uint8_t rx_phys;
    /// PHY options
    uint16_t phy_opt;
} __attribute__((packed));

/// HCI LE Set Extended Advertising Parameters Command Complete Event
/*@TRACE*/
struct hci_le_set_ext_adv_param_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Selected Tx power
    int8_t sel_tx_pwr;
} __attribute__((packed));

/// HCI LE Enhanced Receiver Test Command
/*@TRACE*/
struct hci_le_enh_rx_test_cmd
{
    /// Reception channel value
    uint8_t channel;
    /// Reception PHY rate
    uint8_t phys;
    /// Modulation index
    uint8_t modulation_idx;
} __attribute__((packed));

/// HCI LE Enhanced Transmitter Test Command
/*@TRACE*/
struct hci_le_enh_tx_test_cmd
{
    /// Transmit channel value
    uint8_t channel;
    /// Length of the data to be transmitted in a packet
    uint8_t payload_length;
    /// Type of the data contained in a packet
    uint8_t payload_type;
    /// Transmit PHY rate
    uint8_t phys;
} __attribute__((packed));

///HCI LE Set Advertising Set Random Address Command parameters structure
/*@TRACE*/
struct hci_le_set_adv_set_rand_addr_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /// Advertising random address
    struct bd_addr rand_addr;
} __attribute__((packed));

///HCI LE Set Extended Advertising Parameters Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_adv_param_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /// Advertising event properties
    uint16_t adv_evt_properties;
    /// Primary advertising minimum interval
    uint8_t prim_adv_intv_min[3];
    /// Primary advertising maximum interval
    uint8_t prim_adv_intv_max[3];
    /// Primary advertising channel map
    uint8_t prim_adv_chnl_map;
    /// Own address type:  public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t own_addr_type;
    /// Peer address type: public=0 / random=1
    uint8_t peer_addr_type;
    /// Peer Bluetooth device address
    struct bd_addr peer_addr;
    /// Advertising filter policy
    uint8_t adv_filt_policy;
    /// Advertising Tx power
    int8_t adv_tx_pwr;
    /// Primary advertising PHY
    uint8_t prim_adv_phy;
    /// Secondary advertising max skip
    uint8_t sec_adv_max_skip;
    /// Secondary advertising PHY
    uint8_t sec_adv_phy;
    /// Advertising SID
    uint8_t adv_sid;
    /// Scan request notification enable
    uint8_t scan_req_notif_en;
} __attribute__((packed));

///HCI LE Set Extended Advertising Data Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_adv_data_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /**
     *  Operation
     *  0x00 Intermediate fragment of fragmented extended advertising data
     *  0x01 First fragment of fragmented extended advertising data
     *  0x02 Last fragment of fragmented extended advertising data
     *  0x03 Complete extended advertising data
     *  0x04 [ID7300_r06] Unchanged data (just update the Advertising DID)
     *  All other values Reserved for future use
     */
    uint8_t operation;
    /**
     *  Fragment preference
     *  0x00 The Controller may fragment all Host advertising data
     *  0x01 The Controller should not fragment nor minimize fragmentation of Host advertising data
     *  All other values Reserved for future use
     */
    uint8_t frag_pref;
    /// Advertising Data Length (0-252 bytes)
    uint8_t data_len;
    /// Advertising data
    uint8_t data[__ARRAY_EMPTY];
} __attribute__((packed));

///HCI LE Set Extended Scan Response Data Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_scan_rsp_data_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /**
     *  Operation
     *  0x00 Intermediate fragment of fragmented extended advertising data
     *  0x01 First fragment of fragmented extended advertising data
     *  0x02 Last fragment of fragmented extended advertising data
     *  0x03 Complete extended advertising data
     *  0x04 [ID7300_r06] Unchanged data (just update the Advertising DID)
     *  All other values Reserved for future use
     */
    uint8_t operation;
    /**
     *  Fragment preference
     *  0x00 The Controller may fragment all Host advertising data
     *  0x01 The Controller should not fragment nor minimize fragmentation of Host advertising data
     *  All other values Reserved for future use
     */
    uint8_t frag_pref;
    /// Scan Response Data Length (0-252 bytes)
    uint8_t data_len;
    /// Advertising data
    uint8_t data[__ARRAY_EMPTY];
} __attribute__((packed));

#if BLE_EMB_PRESENT || BLE_HOST_PRESENT
///HCI LE Set Extended Advertising Enable Command parameters structure
/*@TRACE*/
struct hci_le_set_ext_adv_en_cmd
{
    /// Enable
    uint8_t enable;
    /// Number of sets (1 - 0x3F)
    uint8_t nb_sets;
    /// Advertising handle
    uint8_t adv_hdl[BLE_ACTIVITY_MAX];
    /// Duration (N * 10 ms), 0x0000 No advertising duration. Advertising to continue until the Host disables it.
    uint16_t duration[BLE_ACTIVITY_MAX];
    /// Maximum number of extended advertising events
    uint8_t max_ext_adv_evt[BLE_ACTIVITY_MAX];
} __attribute__((packed));
#endif //BLE_EMB_PRESENT || BLE_HOST_PRESENT

///HCI LE Read Maximum Advertising Data Length Command complete event
/*@TRACE*/
struct hci_le_rd_max_adv_data_len_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Maximum advertising data length
    uint16_t max_adv_data_len;
} __attribute__((packed));

///HCI LE Remove Advertising Set Command parameters structure
/*@TRACE
 * hci_le_rmv_adv_set_cmd = hci_le_rem_adv_set_cmd*/
struct hci_le_rem_adv_set_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
} __attribute__((packed));

///HCI LE Read Number of Supported Advertising Sets Command complete event
/*@TRACE*/
struct hci_le_rd_nb_supp_adv_sets_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Number of supported advertising sets
    uint8_t nb_supp_adv_sets;
} __attribute__((packed));

///HCI LE Read Transmit Power Command complete event
/*@TRACE*/
struct hci_le_rd_tx_pwr_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// Minimum TX Power
    uint8_t min_tx_pwr;
    /// Maximum TX Power
    uint8_t max_tx_pwr;
} __attribute__((packed));

///HCI LE Read RF Path Compensation Command complete event
/*@TRACE*/
struct hci_le_rd_rf_path_comp_cmd_cmp_evt
{
    /// Status
    uint8_t status;
    /// RF TX Path Compensation
    int16_t tx_path_comp;
    /// RF RX Path Compensation
    int16_t rx_path_comp;
} __attribute__((packed));

///HCI LE Write RF Path Compensation Command complete event
/*@TRACE*/
struct hci_le_wr_rf_path_comp_cmd
{
    /// RF TX Path Compensation
    int16_t tx_path_comp;
    /// RF RX Path Compensation
    int16_t rx_path_comp;
} __attribute__((packed));

///HCI LE Set Periodic Advertising Parameters Command parameters structure
/*@TRACE*/
struct hci_le_set_per_adv_param_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /// Minimum advertising interval for periodic advertising
    uint16_t adv_intv_min;
    /// Maximum advertising interval for periodic advertising
    uint16_t adv_intv_max;
    /// Advertising properties
    uint16_t adv_prop;
} __attribute__((packed));

///HCI LE Set Periodic Advertising Data Command parameters structure
/*@TRACE*/
struct hci_le_set_per_adv_data_cmd
{
    /// Advertising handle
    uint8_t adv_hdl;
    /**
     *  Operation
     *  0x00 Intermediate fragment of fragmented periodic advertising data
     *  0x01 First fragment of fragmented periodic advertising data
     *  0x02 Last fragment of fragmented periodic advertising data
     *  0x03 Complete periodic advertising data
     *  All other values Reserved for future use
     */
    uint8_t operation;
    /// Advertising Data Length (0-252 bytes)
    uint8_t data_len;
    /// Advertising data
    uint8_t data[__ARRAY_EMPTY];
} __attribute__((packed));

///HCI LE Set Periodic Advertising Enable Command parameters structure
/*@TRACE*/
struct hci_le_set_per_adv_en_cmd
{
    /// Enable
    uint8_t enable;
    /// Advertising handle
    uint8_t adv_hdl;
} __attribute__((packed));

///HCI LE Advertising Set Terminated event
/*@TRACE
 * hci_le_adv_set_terminated_evt = hci_le_adv_set_term_evt*/
struct hci_le_adv_set_term_evt
{
    /// LE Subevent code
    uint8_t subcode;
    /// Status
    uint8_t status;
    /// Advertising handle
    uint8_t adv_hdl;
    /// Connection handle
    uint16_t conhdl;
    /// Num_Completed_Extended_Advertising_Events
    uint8_t nb_cmp_ext_adv_evt;
} __attribute__((packed));

///HCI LE Scan Request Received event
/*@TRACE*/
struct hci_le_scan_req_rcvd_evt
{
    /// LE Subevent code
    uint8_t subcode;
    /// Advertising handle
    uint8_t adv_hdl;
    /// Scanner address type:  public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t scan_addr_type;
    /// Scanner address
    struct bd_addr scan_addr;
} __attribute__((packed));

///HCI LE Read Periodic Advertiser List Size Command complete event
/*@TRACE*/
struct hci_le_rd_per_adv_list_size_cmd_cmp_evt
{
    ///Status
    uint8_t status;
    /// Periodic Advertiser List Size
    uint8_t size;
} __attribute__((packed));

///HCI LE Channel Selection Algorithm event
/*@TRACE*/
struct hci_le_ch_sel_algo_evt
{
    /// LE Subevent code
    uint8_t subcode;
    /// Connection handle
    uint16_t conhdl;
    /// Channel selection algorithm
    uint8_t ch_sel_algo;
} __attribute__((packed));

/// HCI LE read PHY Command complete event
/*@TRACE*/
struct hci_le_rd_phy_cmd_cmp_evt
{
    ///Status of received command
    uint8_t status;
    /// Connection Handle
    uint16_t conhdl;
    /// Current configured PHY for TX
    //@trc_ref le_phy_value
    uint8_t tx_phy;
    /// Current configured PHY for RX
    //@trc_ref le_phy_value
    uint8_t rx_phy;
} __attribute__((packed));

/// HCI LE PHY Update Complete event
/*@TRACE*/
struct hci_le_phy_upd_cmp_evt
{
    ///LE Subevent code
    uint8_t subcode;
    ///Status of received command
    uint8_t status;
    ///Connection handle
    uint16_t conhdl;
    ///TX phy chosen
    //@trc_ref le_phy_value
    uint8_t tx_phy;
    ///RX phy chosen
    //@trc_ref le_phy_value
    uint8_t rx_phy;
} __attribute__((packed));

/// HCI LE event
/*@TRACE*/
struct hci_le_evt
{
    ///event payload buffer
    uint8_t *buf_ptr;
} __attribute__((packed));

#if (BLE_ISO_PRESENT && BLE_HW_50_ISO)
// HCI ISO definitions for CIS and BIS
#include "co_hci_iso.h"
#endif // (BLE_ISO_PRESENT && BLE_HW_50_ISO)

/// @} CO_BT
#endif // CO_HCI_H_
