/********************************************************************************************************
 * @file    bis_bcst.h
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
#ifndef BIS_BCST_H_
#define BIS_BCST_H_


#define BIG_BCST_PARAM_LENGTH 668 //user can't modify this value !!!


/**
 * @brief      for user to initialize BIG broadcast module and allocate BIG broadcast parameters buffer.
 * @param[in]  pBigBcstPara - start address of BIG broadcast parameters buffer
 * @param[in]  bigBcstNum - BIG broadcast number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_initBigBcstModule_initBigBcstParametersBuffer(u8 *pBigBcstPara, u8 bigBcstNum);


/**
 * @brief      for user to initialize BIS ISO TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size, must use BIS_PDU_ALIGN4_TXBUFF to calculate.
 * @param[in]  fifo_number - RX FIFO number, must be: 2^n, (power of 2),recommended value: 2, 4, 8, 16, 32, 64
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initBisTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number);

/*
 * @brief      This function is used to initialize broadcast sdu in fifo buffer.
 *             sdu in indicate the data if from host to controller.
 * @param[in]  in_fifo      - the start address of broadcast sdu in fifo buffer.
 * @param[in]  in_fifo_size - the fifo size.
 * @param[in]  in_fifo_num  - the fifo number.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initBisBcstSduInBuffer(u8 *in_fifo, u16 in_fifo_size, u8 in_fifo_num);


/**
 * @brief      This function is used to get SDU buffer number valid.
 * @param[in]  bisHandle.
 * @return      Status - 0x00:  no buffer number valid or bisHandle invalid.
 * 						 other:  valid buffer number.
 */
int blc_ll_getBisSduInBufferFreeNum(u16 bisHandle);

typedef enum
{
    BIS_SDU_2_PDU_STRATEGY0 = 0, /* When sending BIS ISO data, perform SDU splitting into PDU asynchronously */
    BIS_SDU_2_PDU_STRATEGY1,     /* When sending BIS ISO data, synchronously perform the SDU split into PDU operation */
} bis_sdu_2_pdu_proc_strategy_t;

/**
 * @brief      This function is used to set the policy for SDU splitting into PDU when sending BIS ISO data
 * @param[in]  stgy - strategy that is set.
 */
void blc_ll_setBisSduSplitIntoPduStrategy(bis_sdu_2_pdu_proc_strategy_t stgy);

/**
 * @brief       This function is used to clear the first SDU synchronous reference point.
 * @param[in]   bigHandle.
 * @return      Status - 0x00:  Failure, 0x01:  Success.
 */
bool blc_ll_clr1stSduSyncRefTicks(u8 big_handle);

/**
 * @brief       This function is used to get the latest BIG anchor point tick for future moments
 * @param[in]   bigHandle.
 * @return      0x00:   failed to get tick
 *              other:  latest synchronized reference point for future moments(unit: system timer ticks)
 */
u32 blc_ll_getLatestBigBcstSyncApTick4FutMoments(u8 big_handle);

typedef void (*big_bcst_1st_sdu_ref_ap_cb)(u32 *pSduRefApTicks);
extern big_bcst_1st_sdu_ref_ap_cb gp_tlk_ble_big_bcst_1st_sdu_ref_ap;

/**
 * @brief       This function is used to register the callback function to get the
 *              first SDU synchronization reference point for BIG broadcast
 * @param[in]  cb - callback function.
 */
__INLINE void blc_ll_regBigBcstGetSduRefApCallback(big_bcst_1st_sdu_ref_ap_cb cb)
{
    gp_tlk_ble_big_bcst_1st_sdu_ref_ap = cb;
}

/**
 * @brief      be used to create a BIG with one or more BISes.All BISes in a BIG have the same value for all parameters.
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.103 LE Create BIG command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_hci_le_createBigParams(hci_le_createBigParams_t *pCmdParam);


/**
 * @brief      only be used for testing purposes
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.104 LE Create BIG Test command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_hci_le_createBigParamsTest(hci_le_createBigParamsTest_t *pCmdParam);

/**
 * @brief      be used to terminate a BIG identified by the BIG_Handle parameter. also terminate the transmission of all BISes of the BIG.
 *             destroys the associated connection handles of the BISes in the BIG and removes the data paths for all BISes in the BIG
 *             refer to "[Vol 4] Part E,7.8.105 LE Terminate BIG command"
 * @param[in]  pCmdParam - big_handle: Used to identify the BIG
 *                       - reason: is used to indicate the reason why the BIG is to be terminated
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_hci_le_terminateBig(hci_le_terminateBigParams_t *pCmdParam);

#endif /* BIS_BCST_H_ */
