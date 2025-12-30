/********************************************************************************************************
 * @file    rf_private.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
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
#ifndef RF_PRIVATE_H
#define RF_PRIVATE_H

/**********************************************************************************************************************
 *                                         RF_PRIVATE global macro                                                    *
 *********************************************************************************************************************/

/******************************************************FOR TPLL************************************************************/

/**
 *  @brief Those setting of offset according to private tpll packet format, so this setting for ble only.
 */
#define RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN 4

/**
 *  @brief According to the packet format find the information of packet through offset.
 */

#define rf_pri_tpll_dma_rx_offset_crc(p)         (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 5)  //data len:2
#define rf_pri_tpll_dma_rx_offset_time_stamp(p)  (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 7)  //data len:4
#define rf_pri_tpll_dma_rx_offset_freq_offset(p) (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 11) //data len:2
#define rf_pri_tpll_dma_rx_offset_rssi(p)        (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 13) //data len:1, signed
#define rf_pri_tpll_packet_crc_ok(p)             ((p[((p[4] & 0x3f) + 11 + 3)] & 0x01) == 0x00)

/**
 *  @brief According to different packet format find the crc check digit.
 */
#define rf_pri_sb_packet_crc_ok(p) ((p[(reg_rf_sblen & 0x3f) + 4 + 9] & 0x01) == 0x00)

/*********************************************************************************************************************
 *                                         RF_PRIVATE function declaration                                           *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to set private_1M  mode of RF.
 * @return    none.
 */
void rf_set_pri_1M_mode(void);

/**
 * @brief     This function serves to  set private_2M  mode of RF.
 * @return    none.
 */
void rf_set_pri_2M_mode(void);

/**
 * @brief       This function serves to set pri sb mode enable.
 * @return      none.
 */
void rf_private_sb_en(void);

/**
 * @brief       This function serves to set pri sb mode payload length.
 * @param[in]   pay_len  - In private sb mode packet payload length.
 * @return      none.
 */
void rf_set_private_sb_len(int pay_len);
#endif
