/********************************************************************************************************
 * @file    hci_vendor.h
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
#ifndef HCI_VENDOR_H_
#define HCI_VENDOR_H_


#include "stack/ble/ble_format.h"

#define IUT_HCI_VENDOR_LOG_EN   (1 & IUT_HCI_LOG_EN)
#define IUT_HCI_VENDOR_WARN_EN  (1 & IUT_HCI_VENDOR_LOG_EN)
#define IUT_HCI_VENDOR_ERROR_EN (1 & IUT_HCI_VENDOR_LOG_EN)


#define HCI_EVT_VENDOR_SPECIFIC_SUBCODE_CHN_MAP_REPORT     0X01

typedef struct
{
	u8  subCode;
	u8  chn[5];
	u16 handle;
}hci_evt_vendor_reportMap_t;


//HCI VS Command (Controller and baseband.)
//-- OGF --
#define HCI_VENDOR_CMD_CBC_OPCODE_OGF 0x00

//-- OCF --
typedef enum
{
    HCI_TELINK_READ_REG          = 0x01,
    HCI_TELINK_WRITE_REG         = 0x02,
    HCI_TELINK_SET_TX_PWR        = 0x03,
    HCI_TELINK_REBOOT_MCU        = 0x04,
    HCI_TELINK_SET_RXTX_DATA_LEN = 0x05,
    HCI_TELINK_SET_BD_ADDR       = 0x06,
    HCI_TELINK_READ_TX_PWR       = 0x07,
    HCI_TELINK_SET_FREQ_OFFSET   = 0x08,
    HCI_TELINK_READ_FREQ_OFFSET  = 0x09,
    HCI_TELINK_SET_SCAN_FILTER   = 0x0a, //MESH

    HCI_EBQ_TEST_CASE_LOG = 0xE2,        //EBQ log case name and timestamp
    HCI_TELINK_VENDOR_MAX_CBC
} hci_vendor_cab_e;

//HCI VS Command (future use)
//-- OGF --
#define HCI_VENDOR_CMD_FU_OPCODE_OGF 0x01
//-- OCF --
/*! HCI VS Command OCF define. */


//HCI VS Command (LE audio)
//-- OGF --
#define HCI_VENDOR_CMD_LEA_OPCODE_OGF 0x02

//-- OCF --
/*! HCI VS Command OCF define. */
typedef enum
{
    HCI_OCF_VS_START_LEA = 0x00,
    HCI_OCF_VS_START_LEA_MAX_LEA
} hci_vendor_lea_e;

//HCI VS Command (DFU)
//-- OGF --
#define HCI_VENDOR_CMD_DFU_OPCODE_OGF 0x03
//-- OCF --
/*! HCI VS Command OCF define. */
//#define HCI_OCF_VS_START_DFU            					0x00
//#define HCI_OCF_VS_END_DFU              					0x01
//#define HCI_OCF_VS_FW_DATA              					0x02


typedef unsigned char hci_vendor_CmdParams_t;

typedef unsigned char hci_vendor_EndStatusParam_t;

typedef unsigned char (*blt_vendor_FuCallback_t)(u8 pCmdparaLen, u8 opCode_ocf, hci_vendor_CmdParams_t *pCmd, hci_vendor_EndStatusParam_t *pRetParam);

/**
 * @brief      This function sets the eventcode returned to the host
 * @param[in]  result: HCI_EVT_CMD_COMPLETE | HCI_EVT_CMD_STATUS .Others not supported
 * @return     NONE
 */
void blt_hci_vendor_setEventCode(u8 result);

ble_sts_t blt_hci_vendor_setFuVendorCallback(blt_vendor_FuCallback_t handler);
#endif /* HCI_VENDOR_H_ */
