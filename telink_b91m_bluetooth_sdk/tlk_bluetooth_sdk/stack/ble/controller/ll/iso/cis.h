/********************************************************************************************************
 * @file    cis.h
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
#ifndef CIS_H_
#define CIS_H_


#define CIS_TX_PDU_BUF_EXT_LEN 13 //user can't modify this value !!!

//attention: if CIS_CONN_PARAM_LENGTH change,please change CIS_CONN_PARAM_LENGTH len simultaneously in cis_stack.h
#define CIS_CONN_PARAM_LENGTH 412 //user can't modify this value !!!
//#define             CIS_CONN_PARAM_LENGTH                           sizeof(ll_cis_conn_t)


/**
 * @brief      for user to initialize CIS Connection module and allocate CIS connection parameters buffer.
 * @param[in]  pCisConnParaBuf - start address of CIS connection parameters buffer
 * @param[in]  cis_cen_num - CIS Central number application layer may use
 * @param[in]  cis_per_num - CIS Peripheral number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blc_ll_initCisConnModule_initCisConnParametersBuffer(u8 *pCisConnParaBuf, u32 cis_cen_num, u32 cis_per_num);

/**
 * @brief      for user to initialize CIS ISO TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address(Tx buffer must concern all CISes).
 * @param[in]  fifo_size - TX FIFO size, size must be 4*n
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initCisTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number);


/**
 * @brief      for user to initialize CIS ISO RX FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size, size must be 4*n
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initCisRxFifo(u8 *pRxbuf, int fifo_size, int fifo_number);


/**
 * @brief      This function is used to initialize IAL CIS SDU in and out buffer.
 * @param[in]  in_fifo
 * @param[in]  in_fifo_size
 * @param[in]  out_fifo
 * @param[in]  out_fifo_size
 */
void blc_ll_initCisSduBuffer(u8 *in_fifo, int in_fifo_size, u8 *out_fifo, int out_fifo_size);


/**
 * @brief      This function is used to disconnect CIS connection.
 * @param[in]  cisHandle - CIS connection handle.
 * @param[in]  reason - Reason for disconnection..
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_cis_disconnect(u16 cisHandle, u8 reason);

typedef enum
{
    CIS_PDU_STRATEGY0 = 0, /*Link lay sends empty PDUs when the upper layer has no data*/
    CIS_PDU_STRATEGY1,     /*Link lay sends NULL PDUs when the upper layer has no data*/
    CIS_PDU_STRATEGY2,     /*Link lay Send Empty when the upper layer data has not arrived after 1 iso_interval,or send NULL PDU*/
} cis_pdu_strategy_t;

/**
 * @brief      This function is used to set sending strategy when no data is sent from the upper layer.
 * @param[in]  stgy - strategy that is set.
 */
void blc_ll_setCisSupplementPDUStrategy(cis_pdu_strategy_t stgy);


sdu_packet_t *blc_ll_popCisRxSduData(u16 cis_connHandle);

typedef void (*cis_event_start_cb)(void);
void blt_ll_cis_central_event_start_cb(cis_event_start_cb cb);
void blt_ll_cis_peripher_event_start_cb(cis_event_start_cb cb);

typedef void (*cis_sub_event_prepare_data_cb)(u8 empty);
void blt_ll_cis_sub_event_prepare_data_cb(cis_sub_event_prepare_data_cb cb);
#endif /* CIS_H_ */
