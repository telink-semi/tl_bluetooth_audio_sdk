/********************************************************************************************************
 * @file    bis_sync.h
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
#ifndef BIS_SYNC_H_
#define BIS_SYNC_H_


//#define 	BIG_SYNC_PARAM_LENGTH							sizeof(ll_big_sync_t)
#define BIG_SYNC_PARAM_LENGTH 1184 //user can't modify this value !!!


/**
 * @brief      for user to initialize BIG Synchronize module and allocate BIG Synchronize parameters buffer.
 * @param[in]  pBigSyncPara - start address of BIG Synchronize parameters buffer
 * @param[in]  bigSyncNum - BIG Synchronize number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_initBigSyncModule_initBigSyncParametersBuffer(u8 *pBigSyncPara, u8 bigSyncNum);


/*
 * @brief      This function is used to initialize Synchronize sdu out fifo buffer.
 *             sdu out indicate the data from controller to host.
 * @param[in]  out_fifo      - the start address of Synchronize sdu out fifo buffer.
 * @param[in]  out_fifo_size - the fifo size.
 * @param[in]  out_fifo_num  - the fifo number.
 * @return     status, 0x00:  succeed
 * 			   other: failed
 */
ble_sts_t blc_ll_initBisSyncSduOutBuffer(u8 *out_fifo, u16 out_fifo_size, u8 out_fifo_num);

/**
 * @brief      for user to initialize BIS ISO RX FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer start address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32, must be 2^n
 * @param[in]  fifo_number - bis num
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initBisRxFifo(u8 *pRxbuf, int full_size, int fifo_number, u8 bis_sync_num);


/**
 * @brief      be used to stop synchronizing or cancel the process of synchronizing to the BIG identified by the bigHandle.
 *             also terminate the reception of BISes in the BIG specified in the blc_hci_le_bigCreateSync,
 *             destroys the associated connection handles of the BISes in the BIG and removes the data paths for all BISes in the BIG.
 * @param[in]  bigHandle - Identifier of the BIG to terminate.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_bigTerminateSync(u8 bigHandle);


/**
 * @brief      For user to pop rx SDU FIFO that hold Rx data.
 * @param[in]  bis_connHandle - Identifier of the bis handle.
 * @return     sdu_packet_t, PDU packet to SDU data structure
 */
sdu_packet_t *blc_ll_popBisSyncRxSduData(u16 bis_connHandle);

/**
 * @brief	   be used to synchronize to a BIG described in the periodic advertising train specified by the sync_handle.
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.106 LE BIG Create Sync command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_hci_le_bigCreateSync(hci_le_bigCreateSyncParams_t *pCmdParam);


#endif /* BIS_SYNC_H_ */
