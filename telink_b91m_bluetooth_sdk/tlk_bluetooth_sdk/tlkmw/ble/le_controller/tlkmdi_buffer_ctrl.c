/********************************************************************************************************
 * @file    tlkmdi_buffer_ctrl.c
 *
 * @brief   This is the source file for TLSR/TL
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
#ifndef VENDOR_LE_CONTROLLER_APP_BUFFER_CTRL_C_
#define VENDOR_LE_CONTROLLER_APP_BUFFER_CTRL_C_
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "vendor/common/ble_common/tlkble_hal_mode.h"
#include "tlkmdi_config_ctrl.h"
#include "tlkmdi_buffer_ctrl.h"

#if (BLE_CONTROLLER_INITIAL_EN)

/***************************** HCI TX & RX data FIFO allocation, Begin *********************************************/
//_attribute_iram_noinit_data_
u8 tlkmdi_app_hci_rxAclfifo[HCI_RX_ACL_FIFO_SIZE * HCI_RX_ACL_FIFO_NUM] = { 0 };
//_attribute_iram_noinit_data_
u8 app_hci_rxfifo[HCI_RX_FIFO_SIZE * HCI_RX_FIFO_NUM] = { 0 };
//_attribute_iram_noinit_data_
u8 app_hci_txfifo[HCI_TX_FIFO_SIZE * HCI_TX_FIFO_NUM] = { 0 };

#if (TLKMDI_ACL_CENTRAL_EN || TLKMDI_ACL_PERIPHR_EN)
/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief   ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 *          user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
_attribute_ble_data_retention_ u8 app_acl_rx_fifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = { 0 };

#if TLKMDI_ACL_CENTRAL_EN
/**
 * @brief   ACL Central TX buffer, shared by all central connections to hold LinkLayer RF TX data.
 *          ACL Central TX buffer should be defined only when ACl connection central role is used.
 */
_attribute_ble_data_retention_ u8 app_acl_cen_tx_fifo[ACL_CENTRAL_TX_FIFO_SIZE * ACL_CENTRAL_TX_FIFO_NUM * TLKMW_ACL_CENTRAL_MAX_NUM] = { 0 };

#endif

#if TLKMDI_ACL_PERIPHR_EN
/**
 * @brief   ACL Peripheral TX buffer, shared by all peripheral connections to hold LinkLayer RF TX data.
 *          ACL Peripheral TX buffer should be defined only when ACl connection peripheral role is used.
 */
_attribute_ble_data_retention_ u8 app_acl_per_tx_fifo[ACL_PERIPHR_TX_FIFO_SIZE * ACL_PERIPHR_TX_FIFO_NUM * TLKMW_ACL_PERIPHR_MAX_NUM] = { 0 };

#endif

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/

#endif

#if TLKMDI_LE_EXTENDED_ADV_EN
/**
  * @brief   Extended ADV parameters buffer, used to hold extended ADV parameters which used in SDK.
  *          ADV_SET_PARAM_LENGTH is determined by SDK, user can not change !!!
  */
_attribute_ble_data_retention_ u8 app_extAdvSetParam_buf[ADV_SET_PARAM_LENGTH * EXT_ADV_SETS_NUMBER];


/**
  * @brief   Extended ADV data buffer.
  *          if any ADV Set use ADV data(e.g. Extended, Connectable), this buffer should be defined and initialized.
  *          if no  ADV Set use ADV data(e.g. Extended, Scannable), this buffer is not necessary.
  */
_attribute_ble_data_retention_ u8 app_extAdvData_buf[EXT_ADV_DATA_LENGTH * EXT_ADV_SETS_NUMBER];

#if TLKMDI_LE_EXTENDED_SCAN_RSP_EN
/**
  * @brief   Extended Scan Response data buffer.
  *          if any ADV Set use scan response data(e.g. Extended, Scannable), this buffer should be defined and initialized.
  *          if no  ADV Set use scan response data(e.g. Extended, Connectable), this buffer is not necessary.
  */
_attribute_ble_data_retention_ u8 app_extScanRspData_buf[EXT_SCANRSP_DATA_LENGTH * EXT_ADV_SETS_NUMBER];
#endif

#if TLKMDI_LE_PERIODIC_ADV_EN

_attribute_ble_data_retention_ u8 app_peridAdvSet_buffer[PERD_ADV_PARAM_LENGTH * APP_PERID_ADV_SETS_NUMBER];
_attribute_ble_data_retention_ u8          app_peridAdvData_buffer[APP_PERID_ADV_DATA_LENGTH * APP_PERID_ADV_SETS_NUMBER];

#endif

#endif

/*********************************** Extended ADV data buffer allocation, End** ************************************/


#if (TLKMDI_LE_CIS_CENTRAL_EN || TLKMDI_LE_CIS_PERIPHR_EN)
/********************************** CIS Global Data Definition, Begin ************************************************/
#if (TLKMDI_LE_CIS_CENTRAL_EN)
/**
 * @brief   CIG parameters buffer for stack use
 *          CIG_PARAM_LEN is determined by SDK, user can not change this value.
 *          CIG_NUMBER is set by user.
 */
_attribute_ble_data_retention_ u8 app_cig_param[CIG_PARAM_LEN * CIG_NUMBER];
#endif
/**
 * @brief   CIS Peripheral parameters buffer for stack use
 *          CIS_SLV_PARAM_LEN is determined by SDK, user can not change this value.
 *          CIS_PERIPHR_NUMBER is set by user.
 */
_attribute_ble_data_retention_ u8 app_cis_per_param[CIS_SLV_PARAM_LEN * CIS_PERIPHR_NUMBER];


/**
 * @brief   CIS connection parameters buffer for stack use
 *          CIS_CONN_PARAM_LENGTH is determined by SDK, user can not change this value.
 *          TOTAL_CIS_NUMBER is total number of CIS Central & CIS Peripheral.
 */
_attribute_ble_data_retention_ u8 app_cis_conn_param[CIS_CONN_PARAM_LENGTH * TOTAL_CIS_NUMBER];

/********************************** CIS Global Data Definition, End **************************************************/


/************************* CIS connection RX/TX ISO PDU buffer allocation, Begin *************************************/
/**
 * @brief   CIS RX PDU buffer. fifo_size & fifo_number defined in app_buffer.h
 * CIS RX PDU buffer is shared by all CIS connections to hold CIS RX data, user should define this buffer
 */
_attribute_ble_data_retention_ u8 app_cis_rxPduFifo[CIS_RX_PDU_FIFO_SIZE * CIS_RX_PDU_FIFO_NUM] = {0};

/**
 * @brief   CIS TX PDU buffer. fifo_size & fifo_number & cis_cnn_num defined in app_buffer.h
 * CIS TX PDU buffer can not be shared, every CIS connection has its own CIS TX PDU buffer.
 */
_attribute_ble_data_retention_ u8 app_cis_txPduFifo[CIS_TX_PDU_FIFO_SIZE * CIS_TX_PDU_FIFO_NUM * TOTAL_CIS_NUMBER] = {0};

/************************* CIS connection RX/TX ISO PDU buffer allocation, End ***************************************/


/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, Begin **************************/
/* SDU in buffer is used to hold local SDU data which will send to peer device */
_attribute_ble_data_retention_ u8 app_cis_sdu_in_fifo[CIS_SDU_IN_FIFO_SIZE * TOTAL_CIS_NUMBER];

/* SDU out buffer is used to hold SDU data received by RF from peer device */
_attribute_ble_data_retention_ u8 app_cis_sdu_out_fifo[CIS_SDU_OUT_FIFO_SIZE * TOTAL_CIS_NUMBER];

/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, End  ***************************/


#endif

#if (TLKMDI_ISOCHRONOUS_BROADCASTER_EN || TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN)
/********************************** BIS BCST/BIS SYNC common Data Definition, Begin ***************************************/
_attribute_ble_data_retention_ u8 app_bisToatlParam[BIS_PARAM_LENGTH * TOTAL_BIS_NUMBER];
/********************************** BIS BCST/BIS SYNC common Data Definition, Begin  ****************************************/


/************************* BIS BCST TX ISO PDU buffer FIFO allocation, Begin ******************************/

/**
 * @brief   BIS BCST TX buffer. size & number defined in app_buffer.h
 */
//_attribute_iram_noinit_data_
u8 app_bisBcstTxfifo[BIS_TX_PDU_FIFO_SIZE * BIS_TX_PDU_FIFO_NUM * BIS_NUM_IN_ALL_BIG_BCST] = { 0 };

/************************* BIS BCST TX ISO PDU buffer FIFO allocation, End ********************************/


/************************ BIS BCST TX ISOAL SDU buffer FIFO allocation, Begin *****************************/
//_attribute_iram_noinit_data_
u8 app_bis_sdu_in_fifo[BIS_SDU_IN_FIFO_SIZE * BIS_SDU_IN_FIFO_NUM * BIS_NUM_IN_ALL_BIG_BCST];
/************************ BIS BCST TX ISOAL SDU buffer FIFO allocation, End  ******************************/


/********************************** BIS BCST Global Data Definition, Begin ***************************************/
//_attribute_iram_noinit_data_
u8 app_bigBcstParam[BIG_BCST_PARAM_LENGTH * BIG_BCST_NUM];
/********************************** BIS BCST Global Data Definition, End  ****************************************/


/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, Begin ******************************/
/**
 * @brief   BIS SYNC RX buffer. size & number defined in app_buffer.h
 */
_attribute_ble_data_retention_ u8 app_bisSyncRxfifo[BIS_RX_PDU_FIFO_SIZE * BIS_RX_PDU_FIFO_NUM * BIS_NUM_IN_ALL_BIG_SYNC] = { 0 };

/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, End ********************************/


/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, Begin *****************************/
_attribute_ble_data_retention_ u8 app_bis_sdu_out_fifo[BIS_SDU_OUT_FIFO_SIZE * BIS_SDU_OUT_FIFO_NUM * BIS_NUM_IN_ALL_BIG_SYNC];
/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, End  ******************************/


/********************************** BIS BSYNC Global Data Definition, Begin ***************************************/
_attribute_ble_data_retention_ u8 app_bigSyncParam[BIG_SYNC_PARAM_LENGTH * BIG_SYNC_NUM];
/********************************** BIS BSYNC Global Data Definition, End  ****************************************/
#endif

#endif
#endif /* VENDOR_LE_CONTROLLER_APP_BUFFER_CTRL_C_ */
