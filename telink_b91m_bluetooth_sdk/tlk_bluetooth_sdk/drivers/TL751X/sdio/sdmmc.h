/********************************************************************************************************
 * @file    sdmmc.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef _SDHCI_H
#define _SDHCI_H

#include <reg_include/register.h>
#include "gpio.h"

// 0###################################################################################################
//  #Defines for command index
// 0###################################################################################################
#define HOST_GO_IDLE_STATE 0x00            //  (0) Go to idle state
#define HOST_SEND_OP_COND 0x01             //  (1) Send operation condition for MMC
#define HOST_ALL_SEND_CID 0x02             //  (2) All send cid register
#define HOST_SEND_RELATIVE_ADDR 0x03       //  (3) Send relative address
#define HOST_SET_DSR 0x04                  //  (4) Set dsr
#define HOST_IO_SEND_OP_COND 0x05          //  (5) IO Send Operation condition
#define HOST_SLEEP_AWAKE 0x05              //  (5) Sleep awake bit for MMC
#define HOST_SWITCH_FUNCTION 0x06          //  (6) Switch function
#define HOST_SELECT_DESELECT_CARD 0x07     //  (7) Select/deselect card
#define HOST_SEND_IF_COND 0x08             //  (8) Send interface condition
#define HOST_SEND_EXT_CSD 0x08             //  (8) Send ext csd data
#define HOST_SEND_CSD 0x09                 //  (9) Send csd register
#define HOST_SEND_CID 0x0A                 //  (10) Send cid register
#define HOST_VOLTAGE_SWITCH 0x0B           //  (11) Voltage switch to 1.8v
#define HOST_READ_DAT_UNTIL_STOP 0x0B      //  (11) Read data until stop command receive
#define HOST_STOP_TRANSMISSION 0x0C        //  (12) Stop transmission
#define HOST_SEND_STATUS 0x0D              //  (13) Send status
#define HOST_BUSTEST_R 0x0E                //  (14) Read Bus testing data pattern from the card
#define HOST_GO_INACTIVE_STATE 0x0F        //  (15) Go to inactive state
#define HOST_SET_BLOCKLEN 0x10             //  (16) Set block length
#define HOST_READ_SINGLE_BLOCK 0x11        //  (17) Read single block
#define HOST_READ_MULTIPLE_BLOCK 0x12      //  (18) Read multiple block
#define HOST_SEND_TUNING_BLOCK 0x13        //  (19) Send 64 byte tuning block
#define HOST_BUSTEST_W 0x13                //  (19) Write Bus testing data pattern from the card
#define HOST_SPEED_CLASS_CONTROL 0x14      //  (20) Speed class control command
#define HOST_WRITE_DAT_UNTIL_STOP 0x14     //  (20) Read data until stop command receive
#define HOST_SEND_TUNING_BLOCK_EMMC 0x15   //  (21) Send 64 byte tuning block
#define HOST_ADDRESS_EXTENSION 0x16        //  (22) Address extension for SDUC
#define HOST_SET_BLOCK_COUNT 0x17          //  (23) Defines number of block
#define HOST_WRITE_BLOCK 0x18              //  (24) Write block
#define HOST_WRITE_MULTIPLE_BLOCK 0x19     //  (25) Write multiple block
#define HOST_PROGRAM_CID 0x1A              //  (26) Program cid
#define HOST_PROGRAM_CSD 0x1B              //  (27) Program csd
#define HOST_SET_WRITE_PROT 0x1C           //  (28) Set write protect
#define HOST_CLR_WRITE_PROT 0x1D           //  (29) Clear write protect
#define HOST_SEND_WRITE_PROT 0x1E          //  (30) Send write protect
#define HOST_SEND_WRITE_PROT_TYPE 0x1F     //  (31) Send write protect type
#define HOST_ERASE_WR_BLK_START 0x20       //  (32) Erase write block start
#define HOST_ERASE_WR_BLK_END 0x21         //  (33) Erase write block end
#define HOST_ERASE_GROUP_START 0x23        //  (35) Erase group start
#define HOST_ERASE_GROUP_END 0x24          //  (36) Erase group end
#define HOST_ERASE 0x26                    //  (38) Erase
#define HOST_FAST_IO 0x27                  //  (39) Used to read write 8bit(register) data fields
#define HOST_GO_IRQ_STATE 0x28             //  (40) Sets the system into interrupt mode
#define HOST_ACMD_SEND_OP_COND 0x29        //  (41) Send application opcode condition
#define HOST_LOCK_UNLOCK 0x2A              //  (42) Lock unlock
#define HOST_Q_MANAGEMENT 0x2B             //  (43) Command Queue Management command
#define HOST_Q_TASK_INFO_A 0x2C            //  (44) Queued Tasks command
#define HOST_Q_TASK_INFO_B 0x2D            //  (45) Queued Tasks address command
#define HOST_Q_RD_TASK 0x2E                //  (46) Execute read task command
#define HOST_Q_WR_TASK 0x2F                //  (47) Execute write task command
#define HOST_READ_EXTR_SINGLE 0x30         //  (48) Single block type read
#define HOST_WRITE_EXTR_SINGLE 0x31        //  (49) Single block type write
#define HOST_IO_RW_DIRECT 0x34             //  (52) IO Read write direct
#define HOST_IO_RW_EXTENDED 0x35           //  (53) IO Read write extended
#define HOST_APP_CMD 0x37                  //  (55) Application command
#define HOST_GEN_CMD 0x38                  //  (56) Generate command
#define HOST_READ_EXTR_MULTIPLE 0x3A       //  (58) Multiple block type read
#define HOST_WRITE_EXTR_MULTIPLE 0x3B      //  (59) Multiple block type write
#define HOST_READ_OCR 0x3A                 //  (58) Read ocr register
#define HOST_CRC_ON_OFF 0x3B               //  (59) Crc on off command
#define HOST_SET_BUS_WIDTH 0x06            //  (6) Set bus width
#define HOST_SD_STATUS 0x0D                //  (13) SD status
#define HOST_SEND_NUM_WR_BLOCKS 0x16       //  (22) Send number of write blocks
#define HOST_SET_WR_BLK_ERASE_COUNT 0x17   //  (23) Set write block erase count
#define HOST_SD_SEND_OP_COND 0x29          //  (41) Send application opcode condition
#define HOST_SET_CLR_CARD_DETECT 0x2A      //  (42) Set clear card detect
#define HOST_SEND_SCR 0x33                 //  (51) Send scr
#define HOST_SECURE_READ_MULTI_BLOCK 0x12  //  (18) Secure read multiple block
#define HOST_SECURE_WRITE_MULTI_BLOCK 0x19 //  (25) Secure write multiple block
#define HOST_SECURE_WRITE_MKB 0x1A         //  (26) Secure write mkb
#define HOST_SECURE_ERASE 0x26             //  (38) Secure erase
#define HOST_SET_TIME 0x31                 //  (49) Set real clock time

// 支持的SD卡定义
#define STD_CAPACITY_SD_CARD_V1_1 ((unsigned int)0x00000000U)
#define STD_CAPACITY_SD_CARD_V2_0 ((unsigned int)0x00000001U)
#define HIGH_CAPACITY_SD_CARD ((unsigned int)0x00000002U)
#define MULTIMEDIA_CARD ((unsigned int)0x00000003U)
#define SECURE_DIGITAL_IO_CARD ((unsigned int)0x00000004U)
#define HIGH_SPEED_MULTIMEDIA_CARD ((unsigned int)0x00000005U)
#define SECURE_DIGITAL_IO_COMBO_CARD ((unsigned int)0x00000006U)
#define HIGH_CAPACITY_MMC_CARD ((unsigned int)0x00000007U)

// Mask for errors Card Status R1 (OCR Register)
#define SD_OCR_ADDR_OUT_OF_RANGE ((unsigned int)0x80000000)
#define SD_OCR_ADDR_MISALIGNED ((unsigned int)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR ((unsigned int)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR ((unsigned int)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM ((unsigned int)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION ((unsigned int)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED ((unsigned int)0x01000000)
#define SD_OCR_COM_CRC_FAILED ((unsigned int)0x00800000)
#define SD_OCR_ILLEGAL_CMD ((unsigned int)0x00400000)
#define SD_OCR_CARD_ECC_FAILED ((unsigned int)0x00200000)
#define SD_OCR_CC_ERROR ((unsigned int)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR ((unsigned int)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN ((unsigned int)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN ((unsigned int)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE ((unsigned int)0x00010000)
#define SD_OCR_WP_ERASE_SKIP ((unsigned int)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED ((unsigned int)0x00004000)
#define SD_OCR_ERASE_RESET ((unsigned int)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR ((unsigned int)0x00000008)
#define SD_OCR_ERRORBITS ((unsigned int)0xFDFFE008)

// Masks for R6 Response
#define SD_R6_GENERAL_UNKNOWN_ERROR ((unsigned int)0x00002000)
#define SD_R6_ILLEGAL_CMD ((unsigned int)0x00004000)
#define SD_R6_COM_CRC_FAILED ((unsigned int)0x00008000)

#define SD_VOLTAGE_WINDOW_SD ((unsigned int)0x80100000)
#define SD_HIGH_CAPACITY ((unsigned int)0x40000000)
#define SD_STD_CAPACITY ((unsigned int)0x00000000)
#define SD_CHECK_PATTERN ((unsigned int)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC ((unsigned int)0x80FF8000)

#define SD_MAX_VOLT_TRIAL ((unsigned int)0x0000FFFF)
#define SD_ALLZERO ((unsigned int)0x00000000)

#define SD_WIDE_BUS_SUPPORT ((unsigned int)0x00040000)
#define SD_SINGLE_BUS_SUPPORT ((unsigned int)0x00010000)
#define SD_CARD_LOCKED ((unsigned int)0x02000000)
#define SD_CARD_PROGRAMMING ((unsigned int)0x00000007)
#define SD_CARD_RECEIVING ((unsigned int)0x00000006)
#define SD_DATATIMEOUT ((unsigned int)0xFFFFFFFF)
#define SD_0TO7BITS ((unsigned int)0x000000FF)
#define SD_8TO15BITS ((unsigned int)0x0000FF00)
#define SD_16TO23BITS ((unsigned int)0x00FF0000)
#define SD_24TO31BITS ((unsigned int)0xFF000000)
#define SD_MAX_DATA_LENGTH ((unsigned int)0x01FFFFFF)

#define SD_HALFFIFO ((unsigned int)0x00000008)
#define SD_HALFFIFOBYTES ((unsigned int)0x00000020)

// Command Class Supported
#define SD_CCCC_LOCK_UNLOCK ((unsigned int)0x00000080)
#define SD_CCCC_WRITE_PROT ((unsigned int)0x00000040)
#define SD_CCCC_ERASE ((unsigned int)0x00000020)

#define SDMMC_STATIC_FLAGS ((unsigned int)0x000205FF)
#define SDMMC_CMD0TIMEOUT ((unsigned int)0x00010000)
#define SDMMC_DATATIMEOUT ((unsigned int)0xFFFFFFFF)


typedef enum
{
    NEGEDGE = 0,
    POSEDGE = 1,
} sdmmc_sample_edge_e;

typedef enum
{
    BASE_CLOCK_UNIT_1KHZ = 0,
    BASE_CLOCK_UNIT_1MHZ = 1,
} sdmmc_base_clk_unit_e;

typedef enum
{
    NORMAL = 0,
    SUSPEND = 1,
    RESUME = 2,
    ABORT = 3,
} sdmmc_cmd_type_e;

typedef enum
{
    RESPONSE_R1 = 0,
    RESPONSE_R2 = 1,
    RESPONSE_R3 = 2,
    RESPONSE_R4 = 3,
} sdmmc_response_e;

typedef enum
{
    RESPONSE_NO = 0,
    RESPONSE_LONG = 1,
    RESPONSE_SHORT = 2,
    RESPONSE_SHORT_AND_CHECK_BUSY = 3, // 检测的是什么，如何检测的
} sdmmc_resp_type_e;

typedef enum
{
    REMOVABLE_CARD_SLOTS = 0,
    EMBEDDED_SLOTS_DEVICES = 1,
    SLOTS_SHARED_BUSES = 2,
    UHS_II_MULTIPLE_EMBEDDED_DEVICES = 3,
} sdmmc_slot_type_e;

typedef enum
{
    SDMMC_INTR_CMD_COMPLETE_STS_EN = BIT(0),
    SDMMC_INTR_TRANSFER_COMPLETE_STS_EN = BIT(1),
    SDMMC_INTR_BLK_GAP_ENVENT_STS_EN = BIT(2),
    SDMMC_INTR_DMA_INTERRUPT_STS_EN = BIT(3),
    SDMMC_INTR_BUFFER_WRITE_READY_STS_EN = BIT(4),
    SDMMC_INTR_BUFFER_READ_READY_STS_EN = BIT(5),
    SDMMC_INTR_CARD_INSERTION_STS_EN = BIT(6),
    SDMMC_INTR_CARD_REMOVAL_STS_EN = BIT(7),
    SDMMC_INTR_CARD_INTERRUPT_STS_EN = BIT(8),
    SDMMC_INTR_INT_A_STS_EN = BIT(9),
    SDMMC_INTR_INT_B_STS_EN = BIT(10),
    SDMMC_INTR_INT_C_STS_EN = BIT(11),
    SDMMC_INTR_RE_TUNING_EVENT_STS_EN = BIT(12),
    SDMMC_INTR_BOOT_ACK_RCV_EN = BIT(13),
    SDMMC_INTR_BOOT_TERMINATE_EN = BIT(14),
    SDMMC_INTR_ERR_INTERRUPT_EN = BIT(15),
    SDMMC_INTR_CMD_TINEOUT_ERR_STS_EN = BIT(16),
    SDMMC_INTR_CRC_ERR_STS_EN = BIT(17),
    SDMMC_INTR_END_BIT_ERR_STS_EN = BIT(18),
    SDMMC_INTR_CMD_INDEX_ERR_TST_EN = BIT(19),
    SDMMC_INTR_DATA_TIMEOUT_ERR_STS_EN = BIT(20),
    SDMMC_INTR_DATA_CRC_ERR_STS_EN = BIT(21),
    SDMMC_INTR_DATA_END_BIT_ERR_STS_EN = BIT(22),
    SDMMC_INTR_CURRENT_LIMIT_ERR_STS_EN = BIT(23),
    SDMMC_INTR_AUTO_CMD_ERR_STS_EN = BIT(24),
    SDMMC_FLD_INTR_AUTO_CMD_ERR_STS_EN = BIT(25),
    SDMMC_INTR_ADMA_ERR_STS_EN = BIT(26),
    SDMMC_INTR_TUNING_ERR_STS_EN = BIT(27),
    SDMMC_INTR_TARGET_RESPONSE_ERR_STS_EN = BIT(28),
    SDMMC_INTR_VENDOR_SPECIFIC_ERR0_STSS_EN = BIT(29),
    SDMMC_INTR_VENDOR_SPECIFIC_ERR1_STS_EN = BIT(30),
    SDMMC_INTR_VEND0R_SPECIFIC_ERR2_STS_EN = BIT(31),

} sdmmc_irq_mask;

typedef enum
{
    SDMMC_INTR_CMD_COMPLETE_STS = BIT(0),
    SDMMC_INTR_TRANSFER_COMPLETE_STS = BIT(1),
    SDMMC_INTR_BLK_GAP_ENVENT_STS = BIT(2),
    SDMMC_INTR_DMA_INTERRUPT_STS = BIT(3),
    SDMMC_INTR_BUFFER_WRITE_READY_STS = BIT(4),
    SDMMC_INTR_BUFFER_READ_READY_STS = BIT(5),
    SDMMC_INTR_CARD_INSERTION_STS = BIT(6),
    SDMMC_INTR_CARD_REMOVAL_STS = BIT(7),
    SDMMC_INTR_CARD_INTERRUPT_STS = BIT(8),
    SDMMC_INTR_INT_A_STS = BIT(9),
    SDMMC_INTR_INT_B_STS = BIT(10),
    SDMMC_INTR_INT_C_STS = BIT(11),
    SDMMC_INTR_RE_TUNING_EVENT_STS = BIT(12),
    SDMMC_INTR_BOOT_ACK_RCV = BIT(13),
    SDMMC_INTR_BOOT_TERMINATE = BIT(14),
    SDMMC_INTR_ERR_INTERRUPT = BIT(15),
    SDMMC_INTR_CMD_TINEOUT_ERR_STS = BIT(16),
    SDMMC_INTR_CRC_ERR_STS = BIT(17),
    SDMMC_INTR_END_BIT_ERR_STS = BIT(18),
    SDMMC_INTR_CMD_INDEX_ERR_TST = BIT(19),
    SDMMC_INTR_DATA_TIMEOUT_ERR_STS = BIT(20),
    SDMMC_INTR_DATA_CRC_ERR_STS = BIT(21),
    SDMMC_INTR_DATA_END_BIT_ERR_STS = BIT(22),
    SDMMC_INTR_CURRENT_LIMIT_ERR_STS = BIT(23),
    SDMMC_INTR_AUTO_CMD_ERR_STS = BIT(24),
    SDMMC_FLD_INTR_AUTO_CMD_ERR_STS = BIT(25),
    SDMMC_INTR_ADMA_ERR_STS = BIT(26),
    SDMMC_INTR_TUNING_ERR_STS = BIT(27),
    SDMMC_INTR_TARGET_RESPONSE_ERR_STS = BIT(28),
    SDMMC_INTR_VENDOR_SPECIFIC_ERR0_STS = BIT(29),
    SDMMC_INTR_VENDOR_SPECIFIC_ERR1_STS = BIT(30),
    SDMMC_INTR_VEND0R_SPECIFIC_ERR2_STS = BIT(31),

} sdmmc_irq_status_e;

typedef enum
{
    SDMMC_INTR_CMD_COMPLETE_STS_POS = 0,
    SDMMC_INTR_TRANSFER_COMPLETE_STS_POS = 1,
    SDMMC_INTR_BLK_GAP_ENVENT_STS_POS = 2,
    SDMMC_INTR_DMA_INTERRUPT_STS_POS = 3,
    SDMMC_INTR_BUFFER_WRITE_READY_STS_POS = 4,
    SDMMC_INTR_BUFFER_READ_READY_STS_POS = 5,
    SDMMC_INTR_CARD_INSERTION_STS_POS = 6,
    SDMMC_INTR_CARD_REMOVAL_STS_POS = 7,
    SDMMC_INTR_CARD_INTERRUPT_STS_POS = 8,
    SDMMC_INTR_INT_A_STS_POS = 9,
    SDMMC_INTR_INT_B_STS_POS = 10,
    SDMMC_INTR_INT_C_STS_POS = 11,
    SDMMC_INTR_RE_TUNING_EVENT_STS_POS = 12,
    SDMMC_INTR_BOOT_ACK_RCV_POS = 13,
    SDMMC_INTR_BOOT_TERMINATE_POS = 14,
    SDMMC_INTR_ERR_INTERRUPT_POS = 15,
    SDMMC_INTR_CMD_TINEOUT_ERR_STS_POS = 16,
    SDMMC_INTR_CRC_ERR_STS_POS = 17,
    SDMMC_INTR_END_BIT_ERR_STS_POS = 18,
    SDMMC_INTR_CMD_INDEX_ERR_TST_POS = 19,
    SDMMC_INTR_DATA_TIMEOUT_ERR_STS_POS = 20,
    SDMMC_INTR_DATA_CRC_ERR_STS_POS = 21,
    SDMMC_INTR_DATA_END_BIT_ERR_STS_POS = 22,
    SDMMC_INTR_CURRENT_LIMIT_ERR_STS_POS = 23,
    SDMMC_INTR_AUTO_CMD_ERR_STS_POS = 24,
    SDMMC_FLD_INTR_AUTO_CMD_ERR_STS_POS = 25,
    SDMMC_INTR_ADMA_ERR_STS_POS = 26,
    SDMMC_INTR_TUNING_ERR_STS_POS = 27,
    SDMMC_INTR_TARGET_RESPONSE_ERR_STS_POS = 28,
    SDMMC_INTR_VENDOR_SPECIFIC_ERR0_STS_POS = 29,
    SDMMC_INTR_VENDOR_SPECIFIC_ERR1_STS_POS = 30,
    SDMMC_INTR_VEND0R_SPECIFIC_ERR2_STS_POS = 31,

} sdmmc_irq_status_pos_e;

typedef enum
{
    SDMA = 0,
    ADMA2 = 1,
    ADMA3 = 2,
} sdmmc_dma_select_e;

typedef struct
{
    bool dma_en;
    sdmmc_dma_select_e dma_select;
} sdmmc_dma_config_t;

typedef enum
{
    DATA0 = 20,
    DATA1 = 21,
    DATA2 = 22,
    DATA3 = 23,
    DATA4 = 4,
    DATA5 = 5,
    DATA6 = 6,
    DATA7 = 7,
} sdmmc_data_e;

typedef enum
{
    DIS_AUTO_CMD = 0,
    EN_AUTO_CMD12 = 1,
    EN_AUTO_CMD23 = 2,
} sdmmc_auto_cmd_e;

typedef enum
{
    HOST_TO_CARD = 0,
    CARD_TO_HOST = 1,
} sdmmc_data_dir_e;

typedef enum
{
    SINGLE_BLOCK = 0,
    MULTIPLE_BLOCK = 1,
} sdmmc_block_select_e;

typedef enum
{
    RSP_TYPE_R1 = 0,
    RSP_TYPE_R5 = 1,
} sdmmc_rsp_type_e;

typedef enum
{
    RESPONSE_NO_0BIT = 0,
    RESPONSE_LENGTH_136BIT = 1,
    RESPONSE_LENGTH_48BIT = 2,
    RESPONSE_LENGTH_48BIT_CHECK_BUSY = 3,
} sdmmc_rsp_select_type_e;

typedef enum
{
    BUF_SIZE_4KB = 0,
    BUF_SIZE_8KB = 1,
    BUF_SIZE_16KB = 2,
    BUF_SIZE_32KB = 3,
    BUF_SIZE_64KB = 4,
    BUF_SIZE_128KB = 5,
    BUF_SIZE_256KB = 6,
    BUF_SIZE_512KB = 6,
} sdmmc_buf_boundary_size_e;

typedef enum
{
    DATA_WIDTH_1BIT = 0,
    DATA_WIDTH_4BIT = 1,
    DATA_WIDTH_8BIT = 2,
} sdmmc_data_width_e;

typedef enum
{
    SDMMC_BUS_CLK_DIV1 = 1,
    SDMMC_BUS_CLK_DIV2,
    SDMMC_BUS_CLK_DIV3,
    SDMMC_BUS_CLK_DIV4,
    SDMMC_BUS_CLK_DIV5,
    SDMMC_BUS_CLK_DIV6,
    SDMMC_BUS_CLK_DIV7,
    SDMMC_BUS_CLK_DIV8,
    SDMMC_BUS_CLK_DIV9,
    SDMMC_BUS_CLK_DIV10,
    SDMMC_BUS_CLK_DIV11,
    SDMMC_BUS_CLK_DIV12,
    SDMMC_BUS_CLK_DIV13,
    SDMMC_BUS_CLK_DIV14,
    SDMMC_BUS_CLK_DIV15,
} sdmmc_bus_clock_div_e;

typedef enum
{
    SDMMC_MOD_CLK_DIV1 = 0x00,
    SDMMC_MOD_CLK_DIV2 = 0x01,
    SDMMC_MOD_CLK_DIV4 = 0x02,
    SDMMC_MOD_CLK_DIV6 = 0x03,
    SDMMC_MOD_CLK_DIV8 = 0x04,
    SDMMC_MOD_CLK_DIV10 = 0x05,
    SDMMC_MOD_CLK_DIV12 = 0x06,
    SDMMC_MOD_CLK_DIV14 = 0x07,
    SDMMC_MOD_CLK_DIV16 = 0x08,
} sdmmc_module_clock_div_e;

typedef struct
{
    gpio_func_pin_e sdmmc_clk_pin;
    gpio_func_pin_e sdmmc_cmd_pin;
    gpio_func_pin_e sdmmc_rst_pin;
    gpio_func_pin_e sdmmc_ds_pin;
    gpio_func_pin_e sdmmc_dat0_pin;
    gpio_func_pin_e sdmmc_dat1_pin;
    gpio_func_pin_e sdmmc_dat2_pin;
    gpio_func_pin_e sdmmc_dat3_pin;
    gpio_func_pin_e sdmmc_dat4_pin;
    gpio_func_pin_e sdmmc_dat5_pin;
    gpio_func_pin_e sdmmc_dat6_pin;
    gpio_func_pin_e sdmmc_dat7_pin;
    gpio_func_pin_e sdmmc_dat8_pin;
} sdmmc_pin_config_t;

typedef struct
{
    bool crc_check_en;
    bool cmd_index_check_en;
    bool sub_command_flag;
    unsigned char cmd_index;
    unsigned int sdmmc_argument;
    bool data_present_select_en;
    unsigned char sdmmc_cmd_type;
    unsigned char sdmmc_cmd_index;
    sdmmc_cmd_type_e sdmmc_type;
    sdmmc_resp_type_e sdmmc_resport;
} sdmmc_send_command_info_t;

typedef struct
{
    bool crc_check_en;
    bool cmd_index_check_en;
    unsigned int saddr;
    bool sub_command_flag;
    unsigned char cmd_index;
    unsigned int sdmmc_argument;
    bool data_present_select_en;
    unsigned char sdmmc_cmd_type;
    unsigned char sdmmc_cmd_index;
    sdmmc_cmd_type_e sdmmc_type;
    sdmmc_resp_type_e sdmmc_resport;
} sdmmc_send_data_command_info_t;

typedef struct
{
    sys_clock_src_e clock_src;
    sdmmc_bus_clock_div_e clock_div;
} sdmmc_bus_clk_t;

typedef struct
{
    sdmmc_base_clk_unit_e base_clk_unit;
    unsigned char clk_num;

} sdmmc_timerout_config_t;

typedef struct
{
    sdmmc_sample_edge_e cmd_edge;
    sdmmc_sample_edge_e read_data_edge;
    sdmmc_sample_edge_e write_data_edge;
    sdmmc_sample_edge_e resp_start_edge;
} sdmmc_sampe_edge_t;

typedef struct
{
    sdmmc_pin_config_t *pin_config;
    sdmmc_bus_clk_t *bus_clk;
    sdmmc_sampe_edge_t *sampe_edge;
    sdmmc_slot_type_e slot_type;
    sdmmc_timerout_config_t *timerout_config;
} sdmmc_config_t;

typedef struct
{
} cmd_base_config_t;

typedef struct
{
    unsigned short block_size;
    sdmmc_auto_cmd_e auto_cmd;
    sdmmc_data_width_e data_width;
    sdmmc_dma_config_t *dma_config;
} sdmmc_trans_config_t;

typedef struct
{
    unsigned cmd_set : 3;
    unsigned reserve1 : 5;
    unsigned reg_val : 8;
    unsigned reg_addr : 8;
    unsigned visits : 2;
    unsigned reserve2 : 6;
} mmc_cmd6_t;


typedef struct
{
    unsigned fun_grp1 : 4;
    unsigned fun_grp2 : 4;
    unsigned fun_grp3 : 4;
    unsigned fun_grp4 : 4;
    unsigned fun_grp5 : 4;
    unsigned fun_grp6 : 4;
    unsigned reserve  : 7;
    unsigned mode     : 1;
} sdcard_cmd6_t;

typedef struct
{
    unsigned char cmd_index;
    bool data_present_select_en;
    bool crc_check_en;
    bool sub_cmd_flag;
    bool cmd_index_check_en;
    sdmmc_cmd_type_e cmd_type;
    sdmmc_resp_type_e sdmmc_resport;
    union
    {
        unsigned int arg;
        mmc_cmd6_t mmc_cmd6;
        unsigned int ocr_arg;
        unsigned int rca_addr;
        unsigned int block_len;
        unsigned int block_addr;
        sdcard_cmd6_t sdcard_cmd6;
    } argument;
} sdmmc_send_cmd_info_t;

typedef struct
{
    union
    {
        unsigned char *dest;
        unsigned char *src;
    } buf_addr;
    sdmmc_dma_config_t *dma_config;
    sdmmc_send_cmd_info_t *cmd_info;
    unsigned short block_num;
    unsigned short block_size;
} data_transfer_t;

typedef struct
{
    unsigned reserved1 : 1;           /*!< ALWAYS 1*/
    unsigned csd_crc : 7;             /*!< 1 CSD CRC */
    unsigned ecc : 2;                 /*!< ECC CODE */
    unsigned fileformat : 2;          /*!< FILE FORMAT */
    unsigned tempwrprotect : 1;       /*!< TEMPORARY WRITE PROTECTION */
    unsigned permwrprotect : 1;       /*!< PERMANENT WRITE PROTECTION */
    unsigned copyflag : 1;            /*!< COPY FLAG (OTP) */
    unsigned fileformatgrouop : 1;    /*!< 2 FILE FORMAT GROUP */
    unsigned contentprotectapp : 1;   /*!< CONTENT PROTECTION APPLICATION */
    unsigned reserved2 : 4;           /*!< RESERVED */
    unsigned writeblockpapartial : 1; /*!< PARTIAL BLOCKS FOR WRITE ALLOWED */
    unsigned maxwrblocklen : 4;       /*!< MAX. WRITE DATA BLOCK LENGTH */
    unsigned wrspeedfact : 3;         /*!< WRITE SPEED FACTOR */
    unsigned mandeflecc : 2;          /*!< MANUFACTURER DEFAULT ECC */
    unsigned wrprotectgrenable : 1;   /*!< WRITE PROTECT GROUP ENABLE */
    unsigned wrprotectgrsize : 5;     /*!< WRITE PROTECT GROUP SIZE */
    unsigned erasegrmul : 5;          /*!< ERASE GROUP SIZE MULTIPLIER */
    unsigned erasegrsize : 5;         /*!< ERASE GROUP SIZE */
    unsigned devicesizemul : 3;       /*!< DEVICE SIZE MULTIPLIER */
    unsigned maxwrcurrentvddmax : 3;  /*!< MAX. WRITE CURRENT @ VDD MAX */
    unsigned maxwrcurrentvddmin : 3;  /*!< MAX. WRITE CURRENT @ VDD MIN */
    unsigned maxrdcurrentvddmax : 3;  /*!< MAX. READ CURRENT @ VDD MAX */
    unsigned maxrdcurrentvddmin : 3;  /*!< MAX. READ CURRENT @ VDD MIN */
    unsigned devicesize : 12;         /*!< DEVICE SIZE */
    unsigned reserved3 : 2;           /*!< RESERVED */
    unsigned dsrimpl : 1;             /*!< DSR IMPLEMENTED */
    unsigned rdblockmisalign : 1;     /*!< READ BLOCK MISALIGNMENT */
    unsigned wrblockmisalign : 1;     /*!< WRITE BLOCK MISALIGNMENT */
    unsigned partblockread : 1;       /*!< PARTIAL BLOCKS FOR READ ALLOWED */
    unsigned rdblocklen : 4;          /*!< MAX. READ DATA BLOCK LENGTH */
    unsigned cardcomdclasses : 12;    /*!< CARD COMMAND CLASSES */
    unsigned maxbusclkfrec : 8;       /*!< MAX. BUS CLOCK FREQUENCY */
    unsigned nsac : 8;                /*!< DATA READ ACCESS-TIME 2 IN CLK CYCLES */
    unsigned taac : 8;                /*!< DATA READ ACCESS-TIME 1 */
    unsigned reserved4 : 2;           /*!< RESERVED */
    unsigned sysspecversion : 4;      /*!< SYSTEM SPECIFICATION VERSION */
    unsigned csdstruct : 2;           /*!< CSD STRUCTURE */
} sdmmc_sdcard_csd;

typedef struct
{
    unsigned reserved1 : 1; /*!< ALWAYS 1 */
    unsigned crc : 7;       /*!< CID CRC */
    unsigned mdt : 8;       /*!< MANUFACTURING DATE */
    unsigned int psn[4];    /*!< PRODUCT SERIAL NUMBER */
    unsigned prv : 8;       /*!< PRODUCT REVISION */
    unsigned int pnm[0x06]; /*!< PRODUCT NAME PART*/
    unsigned oid : 8;       /*!< OEM/APPLICATION ID */
    unsigned cbx : 2;       /*!< PRODUCT NAME PART1 */
    unsigned reserved2 : 6; /*!< RESERVED2 */
    unsigned mid : 8;       /*!< MANUFACTURERID */

    //    unsigned 8 MID; /*!< ManufacturerID */
    //    unsigned short OID;   /*!< OEM/Application ID */
    //    unsigned int CBX ;       /*!< Product Name part1 */
    //    unsigned char PNM;      /*!< Product Name part2*/
    //    unsigned char ProdRev;        /*!< Product Revision */
    //    unsigned int ProdSN;          /*!< Product Serial Number */
    //    unsigned char Reserved1;      /*!< Reserved1 */
    //    unsigned short ManufactDate;  /*!< Manufacturing Date */
    //    unsigned char CID_CRC;        /*!< CID CRC */
    //    unsigned char Reserved2;      /*!< always 1 */
} sdmmc_sdcard_cid;

typedef struct
{
    sdmmc_sdcard_csd sd_csd;
    sdmmc_sdcard_cid sd_cid;
    //    long long CardCapacity;     // SD卡容量,单位:字节,最大支持2^64字节大小的卡.
    //    unsigned int CardBlockSize; // SD卡块大小
    //    unsigned short RCA;         // 卡相对地址
    //    unsigned char CardType;     // 卡类型
} sdmmc_device_info;

/**
 * @brief      This function multiplexed sdmmc pins.
 * @param[in]  bus_clk - sdmmc bus clock initialization pointer.
 * @return     none
 */
void sdmmc_set_bus_clk_init(sdmmc_bus_clk_t *bus_clk);

void sdmmc_set_pin(sdmmc_pin_config_t *sdmmc_pin_config);

void sdmmc_wait_cmd_done(void);

void sdmmc_wait_transport_done(void);

void sdmmc_set_reset_pin(gpio_pin_e pin, unsigned int delay_num);

void sdmmc_set_data_xfr_dir(sdmmc_data_dir_e data_dir);

void sdmmc_set_buf_boundary(sdmmc_buf_boundary_size_e size);

void sdmmc_set_data_block_size(unsigned short num_byte);

void sdmmc_auto_cmd_en(sdmmc_auto_cmd_e auto_cmd);

void sdmmc_set_clk_div(sdmmc_module_clock_div_e div);

void sdmmc_set_data_width(sdmmc_data_width_e data_width);

void sdmmc_set_sampling_edge(sdmmc_sampe_edge_t *sampe_edge);

void sdmmc_core_init(sdmmc_config_t *config);

void sdmmc_data_trans_init(sdmmc_trans_config_t *config);

void sdmmc_cmd_transfer(sdmmc_send_cmd_info_t *cmd_info);

void sdmmc_cmd_data_transfer(data_transfer_t *transfer);

#define sdmmc_wait_clk_ready() wait_condition_fails_or_timeout(sdmmc_clk_ready, g_drv_api_error_timeout_us, drv_timeout_handler, (unsigned int)DRV_API_ERROR_TIMEOUT_SDMMC_CLK_WAIT)

#define sdmmc_wait_internal_clk_ready() wait_condition_fails_or_timeout(sdmmc_internal_clk_ready, g_drv_api_error_timeout_us, drv_timeout_handler, (unsigned int)DRV_API_ERROR_TIMEOUT_SDMMC_CLK_WAIT)

static inline void sdmmc_set_bus_clk(sys_clock_src_e clk_source, unsigned short div)
{
    reg_sdio_clk_set = MASK_VAL(FLD_SDIO_DIV_IN_SEL, clk_source, FLD_SDIO_CLK_MOD, div);
}

static inline void sdmmc_set_write_data_sampling_edge(sdmmc_sample_edge_e sample_edge)
{
    (sample_edge == NEGEDGE) ? (BM_CLR(reg_sdmmc_driving_sampling_edge, FLD_WR_DATA_START_DRIVING_DEGE)) : (BM_SET(reg_sdmmc_driving_sampling_edge, FLD_WR_DATA_START_DRIVING_DEGE));
}

static inline void sdmmc_set_cmd_sampling_edge(sdmmc_sample_edge_e sample_edge)
{
    (sample_edge == NEGEDGE) ? (BM_CLR(reg_sdmmc_driving_sampling_edge, FLD_CMD_START_DRIVING_DEGE)) : (BM_SET(reg_sdmmc_driving_sampling_edge, FLD_CMD_START_DRIVING_DEGE));
}

static inline void sdmmc_set_read_data_sampling_edge(sdmmc_sample_edge_e sample_edge)
{
    (sample_edge == NEGEDGE) ? (BM_CLR(reg_sdmmc_driving_sampling_edge, FLD_RD_DATA_START_DRIVING_DEGE)) : (BM_SET(reg_sdmmc_driving_sampling_edge, FLD_RD_DATA_START_DRIVING_DEGE));
}

static inline void sdmmc_set_resp_start_sampling_edge(sdmmc_sample_edge_e sample_edge)
{
    (sample_edge == NEGEDGE) ? (BM_CLR(reg_sdmmc_driving_sampling_edge, FLD_RESP_START_SAMPLING_EDGE)) : (BM_SET(reg_sdmmc_driving_sampling_edge, FLD_RESP_START_SAMPLING_EDGE));
}

static inline void sdmmc_set_sdma_addr(unsigned int saddr)
{
    reg_sdmmc_blkcount_sdmasysaddr = saddr;
}

static inline void sdmmc_set_cmd_arg(unsigned int cmd_arg)
{
    reg_sdmmc_argument = cmd_arg;
}

static inline void sdmmc_set_clear_irq_status(sdmmc_irq_status_e status)
{
    reg_sdmmc_irq_sts = status;
}

static inline unsigned int sdmmc_get_irq_status(sdmmc_irq_status_e status)
{
    return reg_sdmmc_irq_sts & status;
}

static inline unsigned char sdmmc_get_write_read_status(void)
{
    return reg_sdmmc_response_state_b1;
}

static inline void sdmmc_set_cap_slot_type(sdmmc_slot_type_e solt_type)
{
    reg_sdmmc_capabilities_0 &= ~FLD_SLOT_TYPE;
    reg_sdmmc_capabilities_0 |= MASK_VAL(FLD_SLOT_TYPE, solt_type);
}

static inline void sdmmc_set_intereupt_mask(sdmmc_irq_mask irq_mask)
{
    reg_sdmmc_cq_intr_sts_en |= irq_mask;
}

static inline void sdmmc_internal_clk_en(void)
{
    BM_SET(reg_sdmmc_clock_timeout_control, FLD_INTTERNAL_CLK_EN);
}

static inline void sdmmc_intternal_clk_dis(void)
{
    BM_CLR(reg_sdmmc_clock_timeout_control, FLD_INTTERNAL_CLK_EN);
}

static inline void sdmmc_host_clk_en(void)
{
    BM_SET(reg_sdmmc_clock_timeout_control, FLD_HOST_SD_CLK_EN);
}

static inline void sdmmc_host_clk_dis(void)
{
    BM_CLR(reg_sdmmc_clock_timeout_control, FLD_HOST_SD_CLK_EN);
}

static inline void sdmmc_set_timeout_clk_unit(sdmmc_base_clk_unit_e clk_unit)
{
    reg_sdmmc_capabilities_0 |= MASK_VAL(FLD_TIMEOUT_CLK_UNIT, clk_unit);
}

static inline void sdmmc_set_timeout_clk_val(unsigned char timeout_num)

{
    reg_sdmmc_clock_timeout_control |= MASK_VAL(FLD_DATA_TIME_OUT_CNTR_VALUE, timeout_num);
}

static inline void sdmmc_set_base_clk(unsigned char clk_num)
{
    reg_sdmmc_capabilities_0 |= MASK_VAL(FLD_BASE_CLK_FREQ_FOR_SD_CLK, clk_num);
}

static inline void sdmmc_set_card_insertion_status(void)
{
    BM_SET(reg_sdmmc_irq_sts, FLD_CARD_INSERTION_STS);
}

static inline bool sdmmc_get_card_insertion_status(void)
{
    return reg_sdmmc_irq_sts & FLD_CARD_INSERTION_STS;
}

static inline unsigned int sdmmc_get_response1_data(void)
{
    return reg_sdmmc_response_0;
}

static inline unsigned int sdmmc_get_response2_data(void)
{
    return reg_sdmmc_response_1;
}

static inline unsigned int sdmmc_get_response3_data(void)
{
    return reg_sdmmc_response_2;
}

static inline unsigned int sdmmc_get_response4_data(void)
{
    return reg_sdmmc_response_3;
}

static inline unsigned char sdmmc_wait_data_ready(sdmmc_data_e data)
{
    return (reg_sdmmc_response_state & BIT(data)) >> data;
}

#endif
