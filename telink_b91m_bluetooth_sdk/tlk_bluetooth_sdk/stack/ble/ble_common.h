/********************************************************************************************************
 * @file    ble_common.h
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
#ifndef BLE_COMMON_H
#define BLE_COMMON_H

#include "tl_common.h"

#include "ble_hci_error.h"

/**
 *  @brief  error code for user initialization error
 */
typedef enum
{
    INIT_SUCCESS = 0,

    ///////// Controller ///////////
    LL_ACL_RX_BUF_NO_INIT = 0x1001,
    LL_ACL_RX_BUF_PARAM_INVALID,
    LL_ACL_RX_BUF_SIZE_NOT_MEET_MAX_RX_OCT,
    LL_ACL_TX_BUF_NO_INIT,
    LL_ACL_TX_BUF_PARAM_INVALID,
    LL_ACL_TX_BUF_SIZE_NOT_MEET_MAX_TX_OCT,

    HCI_ACL_DATA_BUF_PARAM_INVALID,
    HCI_ACL_DATA_BUF_SIZE_NOT_MEET_MAX_TX_OCT,


    LL_ACL_TX_BUF_SIZE_MUL_NUM_EXCEED_4K = 0x1071, /* special, B91 only */


    LL_BIS_TX_BUF_NO_INIT = 0x1081,
    LL_BIS_TX_BUF_PARAM_INVALID,
    LL_BIS_RX_BUF_NO_INIT,
    LL_BIS_RX_BUF_PARAM_INVALID,
    LL_BIS_RX_PDU_INVALID,
    LL_BIS_RX_PDU_EMPTY,
    LL_BIS_RX_EVT_BUF_NO_INIT,
    LL_BIS_RX_EVT_BUF_PARAM_INVALID,
    LL_BIS_RX_IAL_BUF_NO_INIT,
    LL_BIS_TX_IAL_BUF_NO_INIT,


    LL_CIS_RX_BUF_NO_INIT = 0x1091,
    LL_CIS_RX_BUF_PARAM_INVALID,
    LL_CIS_TX_BUF_NO_INIT,
    LL_CIS_TX_BUF_PARAM_INVALID,
    LL_CIS_RX_EVT_BUF_NO_INIT,
    LL_CIS_RX_EVT_BUF_PARAM_INVALID,
    LL_CIS_RX_IAL_BUF_NO_INIT,
    LL_CIS_TX_IAL_BUF_NO_INIT,


    ////////////////// Host /////////////////////
    //GAP
    INIT_ERR_GAP_PARAM_INVALID = 0x2001,

    //L2CAP
    INIT_ERR_L2CAP_PARAM_INVALID = 0x2101,


    //ATT
    INIT_ERR_ATT_PARAM_INVALID = 0x2201,


    //GATT
    INIT_ERR_GATT_PARAM_INVALID = 0x2301,

    //SMP
    INIT_ERR_SMP_PARAM_INVALID = 0x2401,
    INIT_ERR_SMP_BONDING_MAX_NUMBER_EXCEED,


} init_err_t;

////////////////////// Bluetooth Device Address Type ////////////////////////
//Definition for BLE device address types
#define BLE_ADDR_PUBLIC    0 //BDA: Public
#define BLE_ADDR_RANDOM    1 //BDA: Random
#define BLE_ADDR_PUBLIC_ID 2 //BDA: Public Identity Address, RPA
#define BLE_ADDR_RANDOM_ID 3 //BDA: Random (Static) Identity Address, RPA

#define BLE_ADDR_LEN       6
#define BLE_ADDR_INVALID   0xff

//Definition for BLE Common Address Type
/*
 *
 *				  |--public  ..................................................... BLE_DEVICE_ADDRESS_PUBLIC
 *                |
 * Address Type --|		      |-- random static  ................................. BLE_DEVICE_ADDRESS_RANDOM_STATIC
 *           	  |           |
 *    			  |--random --|
 * 			   				  |				       |-- non_resolvable private  ... BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE
 * 			 				  |-- random private --|
 *           					                   |-- resolvable private  ....... BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE
 *
 */

#define BLE_DEVICE_ADDRESS_PUBLIC                 1
#define BLE_DEVICE_ADDRESS_RANDOM_STATIC          2
#define BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE 3
#define BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE     4


#define IS_PUBLIC_ADDR(Type, Addr)  					( (Type)==BLE_ADDR_PUBLIC) )
#define IS_RANDOM_STATIC_ADDR(Type, Addr)          ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0xC0)
#define IS_NON_RESOLVABLE_PRIVATE_ADDR(Type, Addr) ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x00)
#define IS_RESOLVABLE_PRIVATE_ADDR(Type, Addr)     ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x40)


#define MAC_MATCH16(md, ms)                        (md[0] == ms[0] && md[1] == ms[1] && md[2] == ms[2])

/////////////////////////////////////////////////////////////////////////////


typedef enum
{
    LE_DEVTYPE_NONE    = 0,
    LE_DEVTYPE_UNKNOWN = BIT(0),

    LE_DEVTYPE_MOUSE   = BIT(1),
    LE_DEVTYPE_GAMEPAD = BIT(2),
    LE_DEVTYPE_KB      = BIT(3),
    LE_DEVTYPE_GENERAL = BIT(4),

    LE_DEVTYPE_AUDIO = BIT(7),
} dev_type_t;

#define LE_DEVTYPE_HID (LE_DEVTYPE_MOUSE | LE_DEVTYPE_GAMEPAD | LE_DEVTYPE_KB)


/**
 * @brief	HCI ACL DATA buffer length = LE_ACL_Data_Packet_Length + 4, pkt_len is integer multiple of 4, so result is 4 Byte align
 *			HCI ACL Data packet 4 = 2(connHandle  + PBFlag) + 2(length)
 *          Telink Controller Used 5 = 2(connHandle)  + 1(PBFlag) + 2(length)
 */
#define CALCULATE_HCI_ACL_DATA_FIFO_SIZE(pkt_len) ((pkt_len + 5 + 3) / 4 * 4)


/**
 * @brief	6 = header(2)+l2cap_len(2)+CID(2)
 */
#define CAL_L2CAP_BUFF_SIZE(n) (((n + 6) + 3) / 4 * 4)

/**
 * @brief	12 = type(1) + len(1) + l2cap_len(2) + cid(2) + sud_len(2) + mic(4)
 */
#define L2CAP_ALIGN4_KFRAM_DMA_BUFF(n) (((n + 12) + 3) / 4 * 4)

/**
 * @brief	CIS TX FIFO Size = n + CIS_TX_PDU_BUF_EXT_LEN + TLK_RF_TX_EXT_LEN
 * CIS_TX_PDU_BUF_EXT_LEN is extra data length designed by Stack
 * TLK_RF_TX_EXT_LEN depend on MCU
 * CIS TX FIFO do not use hardware FIFO, so no 16 byte align limitation
 * TX dma_len must be 4 byte align, so total buffer need 4 byte align */
#define CAL_LL_CIS_TX_FIFO_SIZE(n) DATA_LENGTH_ALIGN4(n + CIS_TX_PDU_BUF_EXT_LEN + TLK_RF_TX_EXT_LEN)

#define BIS_PDU_ALIGN4_TXBUFF(n)   DATA_LENGTH_ALIGN4((CAL_LL_ISO_TX_FIFO_SIZE(n) + 12)) //12=OFFSETOF(bis_tx_pdu_t, isoTxPdu)

#define BIS_PDU_ALIGN4_RXBUFF(n)   DATA_LENGTH_ALIGN4(BIS_LL_RX_PDU_FIFO_SIZE(n))


/*
++++++++++
| 2            | 2            | 4          | 2          | 1          | 1       | 1  | 1  | iso_sdu_len  |
++++++++++
| pkt_seq_num  | iso_sdu_len  | timestamp  | sduOffset  | numHciPkt  | pkt_st  | PB | TS | SDU_Data     |
++++++++++

total len = 14
*/

#define IAL_SDU_HDR_EN               14
#define CAL_IAL_ISO_SDU_FIFO_SIZE(n) (((n + IAL_SDU_HDR_EN) + 3) / 4 * 4)


/*
HCI TX fifo include ACL data report, HCI Event report (controller to host)

1. HCI ACL out Data format in telink
++++++
| 2         | 1     | 2       | 2         | n               |
++++++
| uart_len  | type  | handle  | data_len  | data_total_len  |
++++++
 */
#define HCI_ACL_OUT_FIFO_SIZE(n) ((n + 7 + 3) / 4 * 4)

/* HCI ISO out DATA format in telink
+++++++++
| 2         | 1     | 2       | 2                     | 4          | 2                    | 2               | n         |
+++++++++
| uart_len  | type  | handle  | ISO_data_load_length  | timestamp  | packet_sequence_num  | iso_sdu_length  | sdu_data  |
+++++++++
 */
#define HCI_ISO_OUT_FIFO_SIZE(n) ((n + 15 + 3) / 4 * 4)


//HCI OUT hold ISO data and ACL data, Event
#define HCI_OUT_FIFO_MAX_SIZE(acl_len_max, iso_len_max) (max2(HCI_ACL_OUT_FIFO_SIZE(acl_len_max), HCI_ISO_OUT_FIFO_SIZE(iso_len_max)))


/* HCI ACL in data format in telink  (host to controller)
 *
+++++
| 1     | 2       | 2         | n               |
+++++
| type  | handle  | data_len  | data_total_len  |
+++++
 */
#define HCI_ACL_IN_FIFO_SIZE(n) ((n + 5 + 3) / 4 * 4)

/* HCI ISO in data format in telink  (host to controller)
++++++++
| 1     | 2       | 2                     | 4          | 2                    | 2               | n         |
++++++++
| type  | handle  | ISO_data_load_length  | timestamp  | packet_sequence_num  | iso_sdu_length  | sdu_data  |
++++++++
 */
#define HCI_ISO_IN_FIFO_SIZE(n) (((n + 13) + 3) / 4 * 4)


/*
 * HCI in fifo include HCI ACL data and CMD, ISO data
 */
#define HCI_IN_FIFO_MAX_SIZE(acl_len_max, iso_len_max) (max2(HCI_ACL_IN_FIFO_SIZE(acl_len_max), HCI_ISO_IN_FIFO_SIZE(iso_len_max)))


/* for BLE B91 old SDK macro compatible with new SDK */
/**
 * @brief	6 = header(2)+l2cap_len(2)+CID(2)
 */
#define CAL_MTU_BUFF_SIZE(n) (((n + 6) + 3) / 4 * 4)


/**
 * @brief      ota crc16 related function.
 * @param[in]  pD: input data.
 * @param[in]  len: data length.
 * @return     crc result.
 */
unsigned short crc16(unsigned char *pD, int len);


/**
 * @brief      get SDK and Lib version. Now the version is 16Bytes.
 * 						Struction				Example
 * 					- SDK Version (4B)		:	"04 00 01 01" means V4.0.1.1
 * 					- Patch Version (1B) 	:	"01" means Patch_01
 * 					- Lib built date (5B)	:	"44 65 63 14 23" means Dec 14 2023
 * 					- Lib built time (2B)	:	"20 56" means 20:46
 * 					- Reserved (4B)			:	"FF FF FF FF"
 * @param[in]  pbuf - the pointer to the version buffer.
 * @param[in]  number - the value is reserved for future use.
 * @return     the length or version char array.
 */
unsigned char blc_get_sdk_version(unsigned char *pbuf, unsigned char number);


#endif
