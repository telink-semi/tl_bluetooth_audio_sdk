/********************************************************************************************************
 * @file    analog_internal.h
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

#pragma once

#include "dma.h"

/**********************************************************************************************************************
 *                                              Only test                                                       *
 *********************************************************************************************************************/


// /**
//  * @brief      This function serves to analog register write by word using dma.
//  * @param[in]  chn  - the dma channel.
//  * @param[in]  addr - address need to be write.
//  * @param[in]  pdat - the ptr of data need to be write.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_write_reg32_dma(dma_chn_e chn, unsigned int addr, void *pdat);

// /**
//  * @brief      This function serves to analog register read by word using dma.
//  * @param[in]  chn - the dma channel.
//  * @param[in]  addr - address need to be read.
//  * @param[in]  pdat - the value need to be read.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_read_reg32_dma(dma_chn_e chn, unsigned int addr, void *pdat);

// /**
//  * @brief      This function write buffer to analog register by dma channel.
//  * @param[in]  chn  - the dma channel.
//  * @param[in]  addr - address need to be write.
//  * @param[in]  pdat - the buffer ptr need to be write.
//  * @param[in]  len  - the length of buffer.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_write_buff_dma(dma_chn_e chn, unsigned int addr, unsigned char *pdat, unsigned int len);

// /**
//  * @brief      This function write buffer to analog register by dma channel.
//  * @param[in]  chn  - the dma channel.
//  * @param[in]  addr - address need to be read from.
//  * @param[out] pdat - the buffer ptr to store read data.
//  *             note: The size of array pdat must be a multiple of 4.
//  *                  For example, if you just need read 5 byte by dma, you should
//  *                  define the size of array pdat to be greater than 8 other than 5.
//  *                  Because the dma would return 4 byte data every time, 5 byte is
//  *                  not enough to store them.
//  * @param[in]  len  - the length of read data.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_read_buff_dma(dma_chn_e chn, unsigned int addr, unsigned char *pdat, unsigned int len);

// /**
//  * @brief      This function write buffer to analog register by dma channel.
//  * @param[in]  chn  - the dma channel.
//  * @param[in]  pdat - the buffer(addr & data) ptr need to be write,
//  *             note: The array pdat should look like this,
//  *             |  pdat     |            |        |
//  *             |  :------  | :----------|  :---- |
//  *             |  pdat[0]  |   address  |  0x3a  |
//  *             |  pdat[1]  |    data    |  0x11  |
//  *             |  pdat[2]  |   address  |  0x3b  |
//  *             |  pdat[3]  |    data    |  0x22  |
//  *             |  ......   |            |        |
//  *              It means write data 0x11 to address 0x3a,
//  *                       write data 0x22 to address 0x3b,
//  *                       ......
//  * @param[in]  len - the length of read data.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_write_addr_data_dma(dma_chn_e chn, void *pdat, int len);

// typedef enum
// {
//     ALG_2_FREQUENCY_DIVISION  = 0x00,
//     ALG_4_FREQUENCY_DIVISION  = 0x01,
//     ALG_8_FREQUENCY_DIVISION  = 0x02,
//     ALG_16_FREQUENCY_DIVISION = 0x03,
// } algm_div;

// /**
//  * @brief      This function sets the division factor of the analog register for PCLK.
//  * @param[in]  div - division factor.
//  * @return     none.
//  */
// _attribute_ram_code_sec_noinline_ void analog_pclk_div(algm_div div);
