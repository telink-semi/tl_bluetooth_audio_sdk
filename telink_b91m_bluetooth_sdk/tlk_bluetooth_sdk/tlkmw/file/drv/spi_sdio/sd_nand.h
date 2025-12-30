/********************************************************************************************************
 * @file    sd_nand.h
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
#pragma once

#include "stdint.h"

#define SD_ERR_IDLE                 0x01
#define SD_ERR_NO_ERR               0x00

/*
    7 6 5 4 3 2 1 0
    x x x 0 Statu 1
 The meaning of the status bits is defined as follows: 
    '010' - Data accepted. 
    '101' - Data rejected due to a CRC error. 
    '110' - Data Rejected due to a Write Error
*/

#define TOKEN_START_READ            0xfe
#define TOKEN_START_WRITE           0xfc
#define TOKEN_STOP_TRANS            0xfd

#define DRT_MASK                    0x1f
#define DRT_ACCEPT                  0x05
#define DRT_CRC_ERROR               0x0B
#define DRT_REJECT                  0x0D


#define _SD_CMD_GO_IDLE_STATE       ( 0)  /* CMD0  = 0x40 */
#define _SD_CMD_SEND_OP_COND        ( 1)  /* CMD1  = 0x41 */
#define _SD_CMD_SEND_IF_COND        ( 8)  /* CMD8  = 0x48 */
#define _SD_CMD_SEND_CSD            ( 9)  /* CMD9  = 0x49 */
#define _SD_CMD_SEND_CID            (10)  /* CMD10 = 0x4A */
#define _SD_CMD_STOP_TRANSMISSION   (12)  /* CMD12 = 0x4C */
#define _SD_CMD_SEND_STATUS         (13)  /* CMD13 = 0x4D */
#define _SD_CMD_SET_BLOCKLEN        (16)  /* CMD16 = 0x50 */
#define _SD_CMD_READ_SINGLE_BLOCK   (17)  /* CMD17 = 0x51 */
#define _SD_CMD_READ_MULT_BLOCK     (18)  /* CMD18 = 0x52 */
#define _SD_CMD_SET_BLOCK_COUNT     (23)  /* CMD23 = 0x57 */
#define _SD_CMD_WRITE_SINGLE_BLOCK  (24)  /* CMD24 = 0x58 */
#define _SD_CMD_WRITE_MULT_BLOCK    (25)  /* CMD25 = 0x59 */
#define _SD_CMD_PROG_CSD            (27)  /* CMD27 = 0x5B */
#define _SD_CMD_SET_WRITE_PROT      (28)  /* CMD28 = 0x5C */
#define _SD_CMD_CLR_WRITE_PROT      (29)  /* CMD29 = 0x5D */
#define _SD_CMD_SEND_WRITE_PROT     (30)  /* CMD30 = 0x5E */
#define _SD_CMD_SD_ERASE_GRP_START  (32)  /* CMD32 = 0x60 */
#define _SD_CMD_SD_ERASE_GRP_END    (33)  /* CMD33 = 0x61 */
#define _SD_CMD_UNTAG_SECTOR        (34)  /* CMD34 = 0x62 */
#define _SD_CMD_ERASE_GRP_START     (35)  /* CMD35 = 0x63 */
#define _SD_CMD_ERASE_GRP_END       (36)  /* CMD36 = 0x64 */
#define _SD_CMD_UNTAG_ERASE_GROUP   (37)  /* CMD37 = 0x65 */
#define _SD_CMD_ERASE               (38)  /* CMD38 = 0x66 */
#define _SD_CMD_SD_APP_OP_COND      (41)  /* CMD41 = 0x69 */
#define _SD_CMD_APP_CMD             (55)  /* CMD55 = 0x77 */
#define _SD_CMD_READ_OCR            (58)  /* CMD55 = 0x79 */

/* R1 answer value */
#define _SD_R1_NO_ERROR             (0x00)
#define _SD_R1_IN_IDLE_STATE        (0x01)
#define _SD_R1_ERASE_RESET          (0x02)
#define _SD_R1_ILLEGAL_COMMAND      (0x04)
#define _SD_R1_COM_CRC_ERROR        (0x08)
#define _SD_R1_ERASE_SEQUENCE_ERROR (0x10)
#define _SD_R1_ADDRESS_ERROR        (0x20)
#define _SD_R1_PARAMETER_ERROR      (0x40)
/* R2 answer value */
#define _SD_R2_NO_ERROR             (0x00)
#define _SD_R2_CARD_LOCKED          (0x01)
#define _SD_R2_LOCKUNLOCK_ERROR     (0x02)
#define _SD_R2_ERROR                (0x04)
#define _SD_R2_CC_ERROR             (0x08)
#define _SD_R2_CARD_ECC_FAILED      (0x10)
#define _SD_R2_WP_VIOLATION         (0x20)
#define _SD_R2_ERASE_PARAM          (0x40)
#define _SD_R2_OUTOFRANGE           (0x80)
/* Data response error */
#define _SD_DATA_OK                 (0x05)
#define _SD_DATA_CRC_ERROR          (0x0B)
#define _SD_DATA_WRITE_ERROR        (0x0D)
#define _SD_DATA_OTHER_ERROR        (0xFF)


/* SD SPI Response define */

typedef struct
{
    uint8_t idle        : 1; // Idle state
    uint8_t erase_reset : 1; // Erase reset
    uint8_t illegal_cmd : 1; // Illegal command
    uint8_t err_crc     : 1; // Com CRC error
    uint8_t err_seq     : 1; // Erase sequence error
    uint8_t err_addr    : 1; // Address error
    uint8_t err_para    : 1; // Parameter error
    uint8_t rsv0        : 1; // Always set to 0
}__attribute__ ((packed)) sd_spi_r1_t;

typedef struct
{
    // B1
    uint8_t rsv1        : 4; // Reserved
    uint8_t cmd_ver     : 4; // command version

    // B2
    uint8_t rsv2        : 8; // Reserved

    // B3
    uint8_t va          : 4; // Voltage accept
    uint8_t rsv3        : 4; // Reserved

    // B4
    uint8_t echo_back   : 8; // Echo back
}__attribute__ ((packed)) sd_spi_r7_t;


typedef struct
{
    uint32_t ocr;
}__attribute__ ((packed)) sd_spi_r3_t;

typedef enum
{
    SD_NAND_ST_IDLE,
    SD_NAND_ST_DUMMY_WRITE,
    SD_NAND_ST_CMD0,
    SD_NAND_ST_CMD8,
    SD_NAND_ST_CMD58_READ_OCR,
    SD_NAND_ST_CMD9,
    SD_NAND_ST_ACMD41,
    SD_NAND_ST_CMD58_GET_CCS,
    SD_NAND_ST_INIT_DONE,
}sd_nand_st_t;

typedef enum
{
    SDC_UNKNOWN = 0,
    SDC_V1,
    SDC_V2,
    SDC_V2_SDHC,
}sd_cart_t;

typedef struct
{
    sd_nand_st_t state;
    uint32_t cmd_retry;
    uint32_t tick_ref;

    // SD card info
    uint8_t  csd_ver;
    uint8_t  card_type;
    uint32_t block_size;
    uint32_t c_size;
}sd_nand_t;