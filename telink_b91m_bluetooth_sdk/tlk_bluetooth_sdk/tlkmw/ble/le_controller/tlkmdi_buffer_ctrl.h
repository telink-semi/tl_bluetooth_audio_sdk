/********************************************************************************************************
 * @file    tlkmdi_buffer_ctrl.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef VENDOR_CONTROLLER_BLE_TEST_APP_BUFFER_H_
#define VENDOR_CONTROLLER_BLE_TEST_APP_BUFFER_H_

#if (BLE_CONTROLLER_INITIAL_EN)

    /********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/* Number of Supported ACL number */

#ifndef TLKMW_ACL_CENTRAL_MAX_NUM
#define TLKMW_ACL_CENTRAL_MAX_NUM 4
#endif

#ifndef TLKMW_ACL_PERIPHR_MAX_NUM
#define TLKMW_ACL_PERIPHR_MAX_NUM 4
#endif


/**
 * @brief   connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 * 2. for CIS peripheral, receive ll_cis_req(36Byte), must be equal to or greater than 36
 */
#ifndef ACL_CONN_MAX_RX_OCTETS
#define ACL_CONN_MAX_RX_OCTETS 40 //user set value
#endif

/**
* @brief   connMaxTxOctets
* refer to BLE SPEC: Vol 6, Part B, "4.5.10 Data PDU length management"
*                    Vol 6, Part B, "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
*  in this SDK, we separate this value into 2 parts: peripheralMaxTxOctets and centralMaxTxOctets,
*  for purpose to save some SRAM costed by when peripheral and central use different connMaxTxOctets.
*
* usage limitation for ACL_xxx_MAX_TX_OCTETS
* 1. should be in range of 27 ~ 251
* 2. for CIS central, send ll_cis_req(36Byte), ACL_CENTRAL_MAX_TX_OCTETS must be equal to or greater than 36
*/
#ifndef ACL_CENTRAL_MAX_TX_OCTETS
#define ACL_CENTRAL_MAX_TX_OCTETS 40 //user set value
#endif

#ifndef ACL_PERIPHR_MAX_TX_OCTETS
#define ACL_PERIPHR_MAX_TX_OCTETS 64 //user set value
#endif

/**
* @brief   ACL RX buffer size & number
*          ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
* usage limitation for ACL_RX_FIFO_SIZE:
* 1. must use CAL_LL_ACL_RX_FIFO_SIZE to calculate, user can not change !!!
*
* usage limitation for ACL_RX_FIFO_NUM:
* 1. must be: 2^n, (power of 2)
* 2. at least 4; recommended value: 4, 8, 16
*/
#define ACL_RX_FIFO_SIZE CAL_LL_ACL_RX_FIFO_SIZE(ACL_CONN_MAX_RX_OCTETS) //user can not change !!!

#ifndef ACL_RX_FIFO_NUM
#define ACL_RX_FIFO_NUM  8                                               //user set value
#endif

/**
* @brief   ACL TX buffer size & number
*          ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*           ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_TX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_xxx_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. for B91m IC: at least 9; recommended value: 9, 17, 33; other value not allowed.
 * 3. for B85m IC: at least 8; recommended value: 8, 16, 32; other value not allowed.
 *
 * only for B91: usage limitation for size * (number - 1)
 * 1. (ACL_xxx_TX_FIFO_SIZE * (ACL_xxx_TX_FIFO_NUM - 1)) must be less than 4096 (4K)
 *    so when ACL TX FIFO size equal to or bigger than 256, ACL TX FIFO number can only be 9(can not use 17 or 33), cause 256*(17-1)=4096
 */
#define ACL_CENTRAL_TX_FIFO_SIZE CAL_LL_ACL_TX_FIFO_SIZE(ACL_CENTRAL_MAX_TX_OCTETS) //user can not change !!!
#ifndef ACL_CENTRAL_TX_FIFO_NUM
#define ACL_CENTRAL_TX_FIFO_NUM  9                                                  //user set value
#endif

#define ACL_PERIPHR_TX_FIFO_SIZE CAL_LL_ACL_TX_FIFO_SIZE(ACL_PERIPHR_MAX_TX_OCTETS) //user can not change !!!
#ifndef ACL_PERIPHR_TX_FIFO_NUM
#define ACL_PERIPHR_TX_FIFO_NUM  9                                                  //user set value
#endif

extern u8 app_acl_rx_fifo[];
extern u8 app_acl_cen_tx_fifo[];
extern u8 app_acl_per_tx_fifo[];
    /******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/




    /*********************************** Extended ADV data buffer allocation, Begin ************************************/
    /**
 * @brief   Extended Advertising Set number and data buffer length
 *
 *  EXT_ADV_SETS_NUMBER:
 *  Number of extended ADV Sets, user set value, should be in range of 1 ~ "ADV_SETS_NUMBER_MAX"
 *
 *  EXT_ADV_DATA_LENGTH:
 *  Extended ADV Data Length, user set value, should be in range of 31 ~ 1650
 *  if ADV Set number bigger than 1, this length should be maximum value of ADV data length for all ADV Sets
 *  if all ADV Set use legacy ADV mode, 31 byte is enough
 *
 *
 *  EXT_SCANRSP_DATA_LENGTH:
 *  Extended ADV Scan Response data Length, user set value, should be in range of 31 ~ 1650
 *  if ADV set number bigger than 1, this length should be maximum value of scan response data length for all ADV Sets
 *  if all ADV Set use legacy ADV mode, 31 byte is enough
 */
#ifndef EXT_ADV_SETS_NUMBER
#define EXT_ADV_SETS_NUMBER     1   //user set value
#endif

#ifndef EXT_ADV_DATA_LENGTH
#define EXT_ADV_DATA_LENGTH     200 //2048//1664//1024   //user set value
#endif

#ifndef EXT_SCANRSP_DATA_LENGTH
#define EXT_SCANRSP_DATA_LENGTH 200 //2048//1664//1024   //user set value
#endif

extern u8 app_extAdvSetParam_buf[];
extern u8 app_extAdvData_buf[];
extern u8 app_extScanRspData_buf[];

/*********************************** Extended ADV data buffer allocation, End ************************************/

/*********************************** Periodic ADV data buffer allocation, Begin ************************************/
    /*
 * @brief   Periodic Advertising Set number and data buffer length
 *
 * APP_PERID_ADV_SETS_NUMBER:
 * Number of Supported Periodic Advertising Sets, no exceed "PERIODIC_ADV_NUMBER_MAX"
 *
 * APP_PERID_ADV_DATA_LENGTH:
 * Maximum Periodic Advertising Data Length. can not exceed 1650.
 */
#ifndef APP_PERID_ADV_SETS_NUMBER
#define APP_PERID_ADV_SETS_NUMBER 2
#endif
#ifndef APP_PERID_ADV_DATA_LENGTH
#define APP_PERID_ADV_DATA_LENGTH 100
#endif

extern u8 app_peridAdvSet_buffer[];
extern u8  app_peridAdvData_buffer[];

/*********************************** Periodic ADV data buffer allocation, End ************************************/

    /********************************** BIS BCST/Sync Global Data Definition, Begin *****************************************/
#ifndef BIG_BCST_NUM
#define BIG_BCST_NUM         1 // Number of Supported BIG_BCST number (must <= LL_BIG_BCST_NUM_MAX)
#endif

#ifndef BIG_SYNC_NUM
#define BIG_SYNC_NUM         1 // Number of Supported BIG_SYNC number (must <= LL_BIG_SYNC_NUM_MAX)
#endif

#ifndef BIS_IN_PER_BIG_BCST_NUM
#define BIS_IN_PER_BIG_BCST_NUM 2 // Number of Supported BISes in Per BIG_BCST (must <= LL_BIS_IN_PER_BIG_BCST_NUM_MAX)
#endif

#ifndef BIS_IN_PER_BIG_SYNC_NUM
#define BIS_IN_PER_BIG_SYNC_NUM 2 // Number of Supported BISes in Per BIG_SYNC (must <= LL_BIS_IN_PER_BIG_SYNC_NUM_MAX)
#endif

// The total number of BIS supported by BIG_BCST
#define BIS_NUM_IN_ALL_BIG_BCST (BIG_BCST_NUM * BIS_IN_PER_BIG_BCST_NUM)
// The total number of BIS supported by BIG_SYNC
#define BIS_NUM_IN_ALL_BIG_SYNC (BIG_SYNC_NUM * BIS_IN_PER_BIG_SYNC_NUM)
// The total number of BIS supported by BIG (BCST and SYNC)
#define TOTAL_BIS_NUMBER (BIS_IN_PER_BIG_BCST_NUM + BIS_IN_PER_BIG_SYNC_NUM)

extern u8 app_bigBcstParam[];
extern u8 app_bisToatlParam[];
    /********************************** BIS BCST Global Data Definition, end  *****************************************/


    /******************************** BIS BCST TX FIFO allocation, Begin **********************************/
    /**
 * @brief   BIS_TX_MAX_PDU is the maximum number of data octets that can be carried in a BIS Data PDU
 * refer to BLE SPEC "Vol 6, Part B, 2.6 ISOCHRONOUS PHYSICAL CHANNEL PDU"
 *
 * usage limitation:
 * 1. should be in range of 0 ~ 251
 */
#ifndef BIS_TX_MAX_PDU
#define BIS_TX_MAX_PDU (251) //44 //LC3 10ms-> 40B, mic 4B plus, 44B enough
#endif

/**
* @brief   BIS BCST TX buffer size & number
*          BIS BCST TX buffer is shared by all bis_bcst to hold LinkLayer RF TX BIS data.
*
* usage limitation for BIS_TX_PDU_FIFO_SIZE:
* 1. must use BIS_PDU_ALIGN4_TXBUFF to calculate, user can not change !!!
*
* usage limitation for BIS_TX_PDU_FIFO_NUM:
* 1. must be: 2^n, (power of 2)
* 2. 2. at least 2; recommended value: 2, 4, 8, 16, 32, 64
*/
#define BIS_TX_PDU_FIFO_SIZE BIS_PDU_ALIGN4_TXBUFF(BIS_TX_MAX_PDU)

#ifndef BIS_TX_PDU_FIFO_NUM
#define BIS_TX_PDU_FIFO_NUM  (8)
#endif
extern u8 app_bisBcstTxfifo[];

    /*
 * @brief   BIS SDU in buffer
            SDU in is SDU from host to controller, so SDU in buffer is used to hold local SDU data which will send to peer device.
 *
 * BIS_SDU_IN_OCTETS_MAX
 * 1. should be in range of 0 ~ 4095
 *
 * BIS_SDU_IN_FIFO_SIZE
 * 1. must use CAL_IAL_ISO_SDU_FIFO_SIZE to calculate, user can not change !!!
 *
 * BIS_SDU_IN_FIFO_NUM
 * 1. must be: 2^n, (power of 2)
 * 2. at least 2; recommended value: 2, 4, 8, 16, 32, 64
 */
#ifndef BIS_SDU_IN_OCTETS_MAX
#define BIS_SDU_IN_OCTETS_MAX 120                                              //503
#endif

#define BIS_SDU_IN_FIFO_SIZE  CAL_IAL_ISO_SDU_FIFO_SIZE(BIS_SDU_IN_OCTETS_MAX) //LC3 10ms-> 40B, iso hdr 8B plus, 48B enough

#ifndef BIS_SDU_IN_FIFO_NUM
#define BIS_SDU_IN_FIFO_NUM   (4)
#endif

extern u8 app_bis_sdu_in_fifo[];
    /***************************** BIS BCST TX FIFO allocation, End  ************************************/


    /******************************** BIS SYNC RX FIFO allocation, Begin **********************************/
    /**
 * @brief   BIS_RX_MAX_PDU is the maximum number of data octets that can be carried in a BIS Data PDU
 * refer to BLE SPEC "2.6 ISOCHRONOUS PHYSICAL CHANNEL PDU"
 * usage limitation:
 * 1. should be in range of 0 ~ 251
 */
#ifndef BIS_RX_MAX_PDU
#define BIS_RX_MAX_PDU 251
#endif
/**
* @brief   BIS SYNC RX buffer size & number
*          BIS SYNC RX buffer is shared by all bis_sync to hold LinkLayer RF RX BIS data.
*
* usage limitation for BIS_RX_PDU_FIFO_SIZE:
* 1. must use BIS_PDU_ALIGN4_RXBUFF to calculate, user can not change !!!
*
* usage limitation for BIS_RX_PDU_FIFO_NUM:
* 1. must be: 2^n, (power of 2)
* 2. at least 2; recommended value: 2, 4, 8, 16, 32
*/
#define BIS_RX_PDU_FIFO_SIZE BIS_PDU_ALIGN4_RXBUFF(BIS_RX_MAX_PDU)

#ifndef BIS_RX_PDU_FIFO_NUM
#define BIS_RX_PDU_FIFO_NUM  (8)
#endif
/*
* @brief   BIS SDU out buffer
           SDU out is SDU from controller to host, so SDU out buffer is used to hold SDU data received by RF from peer device.

* BIS_SDU_OUT_OCTETS_MAX
* 1. the value shall be between 1 and 4095 octets
*
* BIS_SDU_OUT_FIFO_SIZE
* 1. must use CAL_IAL_ISO_SDU_FIFO_SIZE to calculate, user can not change !!!
*
* BIS_SDU_OUT_FIFO_NUM
* 1. must be: 2^n, (power of 2)
* 2. at least 2; recommended value: 2, 4, 8, 16, 32
*/
#ifndef BIS_SDU_OUT_OCTETS_MAX
#define BIS_SDU_OUT_OCTETS_MAX 240 //
#endif

#define BIS_SDU_OUT_FIFO_SIZE  CAL_IAL_ISO_SDU_FIFO_SIZE(BIS_SDU_OUT_OCTETS_MAX)

#ifndef BIS_SDU_OUT_FIFO_NUM
#define BIS_SDU_OUT_FIFO_NUM   (4)
#endif

extern u8 app_bisSyncRxfifo[];
extern u8 app_bigSyncParam[];
extern u8 app_bis_sdu_out_fifo[];
    /***************************** BIS SYNC RX FIFO allocation, Begin, End  ************************************/


/********************************** CIS Global Data Definition, Begin ************************************************/
/* Number of Supported CIS/CIG number */
#ifndef CIG_NUMBER
#define CIG_NUMBER 1
#endif

#ifndef CIS_CENTRAL_NUMBER
#define CIS_CENTRAL_NUMBER 2
#endif

#ifndef CIS_PERIPHR_NUMBER
#define CIS_PERIPHR_NUMBER 1
#endif

/* The total number of CIS connection(CIS Central and CIS Peripheral) */
#define TOTAL_CIS_NUMBER (CIS_CENTRAL_NUMBER + CIS_PERIPHR_NUMBER) //user can not change !!!


extern u8 app_cig_param[];
extern u8 app_cis_per_param[];
extern u8 app_cis_conn_param[];
/********************************** CIS Global Data Definition, end  *************************************************/

/*********************** CIS connection RX/TX ISO PDU buffer allocation, Begin *************************************/
/**
* @brief   CIS Max_PDU is the maximum number of data octets that can be carried in a CIS Data PDU
* refer to BLE SPEC "Vol 6, Part B, 2.4.2.29 LL_CIS_REQ", Max_PDU_C_To_P & Max_PDU_P_To_C
* for central:
*                 CIS TX Max_PDU is Max_PDU_C_To_P, CIS RX Max_PDU is Max_PDU_P_To_C
* for peripheral:
*                 CIS TX Max_PDU is Max_PDU_P_To_C, CIS RX Max_PDU is Max_PDU_C_To_P
*
* usage limitation:
* 1. should be in range of 0 ~ 251
*/
#ifndef CIS_RX_MAX_PDU
#define CIS_RX_MAX_PDU 251 //user set value
#endif

#ifndef CIS_TX_MAX_PDU
#define CIS_TX_MAX_PDU 251 //user set value
#endif

/**
* @brief   CIS RX PDU buffer size & number
*          CIS RX PDU buffer is shared by all CIS connections to hold CIS RX data.
* usage limitation for CIS_RX_PDU_FIFO_SIZE:
* 1. must use CAL_LL_CIS_RX_FIFO_SIZE to calculate, user can not change !!!
*
* usage limitation for CIS_RX_PDU_FIFO_SIZE:
* 1. must be: 2^n, (power of 2)
* 2. at least 8; recommended value: 8, 16, 32, 64
*/
#define CIS_RX_PDU_FIFO_SIZE CAL_LL_CIS_RX_FIFO_SIZE(CIS_RX_MAX_PDU) //user can not change !!!
#ifndef CIS_RX_PDU_FIFO_NUM
#define CIS_RX_PDU_FIFO_NUM  8                                       //user set value
#endif

/**
* @brief   CIS TX PDU buffer size & number
*          CIS TX PDU buffer can not be shared, every CIS connection has its own CIS TX PDU buffer.
* usage limitation for CIS_TX_PDU_FIFO_SIZE
* 1. must use CAL_LL_CIS_TX_FIFO_SIZE to calculate, user can not change !!!
*
* usage limitation for CIS_TX_PDU_FIFO_SIZE
* 1. must be: 2^n, (power of 2)
* 2. at least 4; recommended value: 8, 16
*/
#define CIS_TX_PDU_FIFO_SIZE CAL_LL_CIS_TX_FIFO_SIZE(CIS_TX_MAX_PDU) //user can not change !!!
#ifndef CIS_TX_PDU_FIFO_NUM
#define CIS_TX_PDU_FIFO_NUM  8                                       //user set value
#endif

extern u8 app_cis_rxPduFifo[];
extern u8 app_cis_txPduFifo[];
/************************* CIS connection RX/TX ISO PDU buffer allocation, End ***************************************/


/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, Begin **************************/
/**
* @brief   CIS SDU in buffer
        SDU in is SDU from host to controller, so SDU in buffer is used to hold local SDU data which will send to peer device.
*
* CIS_SDU_IN_OCTETS_MAX
* 1. should be in range of 0 ~ 4095
*
* CIS_SDU_IN_FIFO_SIZE
* 1. must use CAL_IAL_ISO_SDU_FIFO_SIZE to calculate, user can not change !!!
*/
#ifndef CIS_SDU_IN_OCTETS_MAX
#define CIS_SDU_IN_OCTETS_MAX 503                                              //user set value
#endif
#define CIS_SDU_IN_FIFO_SIZE  CAL_IAL_ISO_SDU_FIFO_SIZE(CIS_SDU_IN_OCTETS_MAX) //user can not change !!!


/**
* @brief   CIS SDU out buffer
        SDU out is SDU from controller to host, so SDU out buffer is used to hold SDU data received by RF from peer device.
*
* CIS_SDU_OUT_OCTETS_MAX
* 1. should be in range of 0 ~ 4095
*
* CIS_SDU_OUT_FIFO_SIZE
* 1. must use CAL_IAL_ISO_SDU_FIFO_SIZE to calculate, user can not change !!!
*/
#ifndef CIS_SDU_OUT_OCTETS_MAX
#define CIS_SDU_OUT_OCTETS_MAX 754                                               //user set value
#endif
#define CIS_SDU_OUT_FIFO_SIZE  CAL_IAL_ISO_SDU_FIFO_SIZE(CIS_SDU_OUT_OCTETS_MAX) //user can not change !!!


extern u8 app_cis_sdu_in_fifo[];
extern u8 app_cis_sdu_out_fifo[];
/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, End  ***************************/




/***************************** HCI TX & RX data FIFO allocation, Begin *********************************************/
#define HCI_RX_ACL_FIFO_SIZE      CALCULATE_HCI_ACL_DATA_FIFO_SIZE(LE_ACL_DATA_PACKET_LENGTH)
#define HCI_RX_ACL_FIFO_NUM       8

#define ACL_CONN_MAX_TX_OCTETS    251                                              // !!! max(acl_centarl_max_tx, acl_peripheral_max_tx)
#define LE_ACL_DATA_PACKET_LENGTH ACL_CONN_MAX_TX_OCTETS

//TODO:
#define CIS_SDU_IN_FIFO_SIZE      CAL_IAL_ISO_SDU_FIFO_SIZE(CIS_SDU_IN_OCTETS_MAX)
#define CIS_SDU_OUT_FIFO_SIZE     CAL_IAL_ISO_SDU_FIFO_SIZE(CIS_SDU_OUT_OCTETS_MAX)

#define HCI_ISO_IN_OCTETS_MAX     (max2(BIS_SDU_IN_OCTETS_MAX, CIS_SDU_IN_OCTETS_MAX))
#define HCI_ISO_OUT_OCTETS_MAX    (max2(BIS_SDU_OUT_OCTETS_MAX, CIS_SDU_OUT_OCTETS_MAX))

#define HCI_TX_FIFO_SIZE          HCI_OUT_FIFO_MAX_SIZE(ACL_CONN_MAX_RX_OCTETS, HCI_ISO_OUT_OCTETS_MAX)
#define HCI_TX_FIFO_NUM 4

#define HCI_RX_FIFO_SIZE          HCI_IN_FIFO_MAX_SIZE(max2(ACL_PERIPHR_MAX_TX_OCTETS,ACL_CENTRAL_MAX_TX_OCTETS), HCI_ISO_IN_OCTETS_MAX)
#define HCI_RX_FIFO_NUM 4

extern u8 app_hci_rxfifo[];
extern u8 app_hci_txfifo[];
extern u8 tlkmdi_app_hci_rxAclfifo[];


#endif
#endif /* VENDOR_CONTROLLER_BLE_TEST_APP_BUFFER_H_ */
