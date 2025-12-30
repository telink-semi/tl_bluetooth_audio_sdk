/********************************************************************************************************
 * @file    analog.h
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
/*******************************      analog control registers: 0xb8      ******************************/
/** @page ANALOG
 *
 *  Introduction
 *  ===============
 *  Analog support dma and normal mode, in each mode, support byte/halfword/word/buffer write and read.
 *
 *  API Reference
 *  ===============
 *  Header File: analog.h
 */
#pragma once


#include "reg_include/register.h"
#include "compiler.h"
#include "error_handler/error_handler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/*
 * @note:
 * 1.the destination and source addresses, four-byte alignment.
 * 2.the destination address is configured fixed.
 * 3.dma size needs to be configured DMA_CTR_WORD_WIDTH,the length needs to be configured as a multiple of four.
 */
#define ANALOG_FIXED_ADDR_TX_DMA_CFG                               \
    (DMA_REQ_ALGM_TX << DMA_CHACTRL_DST_REQ_SEL_OFFSET) |          \
        (0 << DMA_CHACTRL_SRC_REQ_SEL_OFFSET) |                    \
        (DMA_ADDR_FIX << DMA_CHACTRL_DST_ADDR_CTRL_OFFSET) |       \
        (DMA_ADDR_FIX << DMA_CHACTRL_SRC_ADDR_CTRL_OFFST) |        \
        (DMA_HANDSHAKE_MODE << DMA_CHACTRL_DSTMODE_OFFSET) |       \
        (DMA_NORMAL_MODE << DMA_CHACTRL_SRCMODE_OFFSET) |          \
        (DMA_CTR_WORD_WIDTH << DMA_CHACTRL_DSTWIDTH_SIZE_OFFSET) | \
        (DMA_CTR_WORD_WIDTH << DMA_CHACTRL_SRCWIDTH_SIZE_OFFSET) | \
        (0 << DMA_CHACTRL_SRC_BURST_SIZE_OFFSET) |                 \
        (0 << DMA_CHACTRL_READ_NUM_EN_OFFSET) |                    \
        (0 << DMA_CHACTRL_PRIORITY_OFFSET) |                       \
        (0 << DMA_CHACTRL_WRITE_NUM_EN_OFFSET) |                   \
        (0 << DMA_CHACTRL_AUTO_EN_OFFSET)

/*
 * @note:
 * 1.the destination and source addresses, four-byte alignment.
 * 2.the destination address is configured incrementally.
 * 3.dma size needs to be configured DMA_CTR_WORD_WIDTH,the length needs to be configured as a multiple of four.
 */
#define ANALOG_INC_ADDR_TX_DMA_CFG                                 \
    (DMA_REQ_ALGM_TX << DMA_CHACTRL_DST_REQ_SEL_OFFSET) |          \
        (0 << DMA_CHACTRL_SRC_REQ_SEL_OFFSET) |                    \
        (DMA_ADDR_FIX << DMA_CHACTRL_DST_ADDR_CTRL_OFFSET) |       \
        (DMA_ADDR_INCREMENT << DMA_CHACTRL_SRC_ADDR_CTRL_OFFST) |  \
        (DMA_HANDSHAKE_MODE << DMA_CHACTRL_DSTMODE_OFFSET) |       \
        (DMA_NORMAL_MODE << DMA_CHACTRL_SRCMODE_OFFSET) |          \
        (DMA_CTR_WORD_WIDTH << DMA_CHACTRL_DSTWIDTH_SIZE_OFFSET) | \
        (DMA_CTR_WORD_WIDTH << DMA_CHACTRL_SRCWIDTH_SIZE_OFFSET) | \
        (0 << DMA_CHACTRL_SRC_BURST_SIZE_OFFSET) |                 \
        (0 << DMA_CHACTRL_READ_NUM_EN_OFFSET) |                    \
        (0 << DMA_CHACTRL_PRIORITY_OFFSET) |                       \
        (0 << DMA_CHACTRL_WRITE_NUM_EN_OFFSET) |                   \
        (0 << DMA_CHACTRL_AUTO_EN_OFFSET)

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief      This function serves to analog clock divide.
 * @param[in]  div - 0: 2 frequency division of  pclk(default)
 *                   1: 4 frequency division of  pclk
 *                   2: 8 frequency division of  pclk
 *                   3: 16 frequency division of  pclk
 * @return     none.
 */
_attribute_ram_code_sec_ void analog_set_clk(unsigned char div);

/**
 * @brief      This function serves to analog register read by byte.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned char analog_read_reg8(unsigned int addr);

/**
 * @brief      This function serves to analog register write by byte.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_reg8(unsigned int addr, unsigned char data);

/**
 * @brief      This function serves to analog register read by halfword.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned short analog_read_reg16(unsigned int addr);

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_reg16(unsigned int addr, unsigned short data);

/**
 * @brief      This function serves to analog register read by word.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned int analog_read_reg32(unsigned int addr);

/**
  * @brief      This function serves to analog register write by word.
  * @param[in]  addr - address need to be write.
  * @param[in]  data - the value need to be write.
  * @return     none.
  */
_attribute_ram_code_sec_noinline_ void analog_write_reg32(unsigned int addr, unsigned int data);
/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr  - address need to be read.
 * @param[out] buff  - the ptr of buffer to store the read data.
 * @param[in]  len   - the length of read value.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_read_buff(unsigned int addr, unsigned char *buff, unsigned char len);

/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr  - address need to be write.
 * @param[in]  buff  - the ptr of value need to be write.
 * @param[in]  len   - the length of write value.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_buff(unsigned int addr, unsigned char *buff, unsigned char len);
