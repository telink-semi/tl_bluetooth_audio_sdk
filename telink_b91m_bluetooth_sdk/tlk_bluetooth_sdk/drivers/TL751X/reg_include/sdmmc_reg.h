/********************************************************************************************************
 * @file    sdmmc_reg.h
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
#ifndef _SDMMC_REG_H
#define _SDMMC_REG_H
#include "soc.h"

/*******************************  sdmmc registers ******************************/
#define SDMMC_BASE_ADDR                  0x80242000

#define reg_sdmmc_blkcount_sdmasysaddr       REG_ADDR32(SDMMC_BASE_ADDR+0x00)
enum{
    FLD_BLKCOUNT_SDMSSYSADDR0               =BIT_RNG(0,7),
    FLD_BLKCOUNT_SDMSSYSADDR1               =BIT_RNG(8,15),
    FLD_BLKCOUNT_SDMSSYSADDR2               =BIT_RNG(16,23),
    FLD_BLKCOUNT_SDMSSYSADDR3               =BIT_RNG(24,31),
};

#define reg_blk_ctrl                      REG_ADDR32(SDMMC_BASE_ADDR+0x04)
enum{
    FLD_BLOCK_SIZE_REGISTER                 =BIT_RNG(0,11),
    FLD_BUUFER_BOUNDARY                     =BIT_RNG(12,14),
    FLD_RESERVED_BITS                       =BIT(15),
    FLD_BLOCK_COUNTREGISTER                 =BIT_RNG(16,31),
};

#define reg_sdmmc_argument                 REG_ADDR32(SDMMC_BASE_ADDR+0x08)
enum{
    FLD_ARGUMENT0                           =BIT_RNG(0,7),
    FLD_ARGUMENT1                           =BIT_RNG(8,15),
    FLD_ARGUMENT2                           =BIT_RNG(16,23),
    FLD_ARGUMENT3                           =BIT_RNG(24,31),
};

#define reg_sdmmc_cmd_transfer_mode_byte          REG_ADDR8(SDMMC_BASE_ADDR+0x0c)
#define reg_sdmmc_cmd_transfer_mode_half_word     REG_ADDR16(SDMMC_BASE_ADDR+0x0c)
#define reg_sdmmc_cmd_transfer_mode             REG_ADDR32(SDMMC_BASE_ADDR+0x0c)
enum{
    FLD_DMA_EN                              =BIT(0),
    FLD_DMA_BLOCK_COUNT_EN                  =BIT(1),
    FLD_AUTO_CMD_EN                         =BIT_RNG(2,3),
    FLD_DATA_XFR_DIR_SELECT                 =BIT(4),
    FLD_BLOCK_SELECT                        =BIT(5),
    FLD_REP_TYPE_R1_R15                     =BIT(6),
    FLD_REP_ERR_CHECK_EN                    =BIT(7),
    FLD_RSP_INTERRUPT_DIS                   =BIT(8),
    FLD_RESERVED_FOR_XFR_MODE               =BIT_RNG(9,15),
    FLD_RSP_TYPE_SELECT                     =BIT_RNG(16,17),
    FLD_SUB_CMD_FLAG                        =BIT(18),
    FLD_CMD_CRC_CHECK_EN                    =BIT(19),
    FLD_CMD_INDEX_CHENK_EN                  =BIT(20),
    FLD_DATA_PRESENT_SELECT                 =BIT(21),
    FLD_CMD_TYPE                            =BIT_RNG(22,23),
    FLD_CMD_INDEX                           =BIT_RNG(24,29),
    FLD_RESERVED_CMD                        =BIT_RNG(30,31),
};

#define RSP_TYPE_SELECT_POS                 0
#define SUB_CMD_FLAG_POS                    2
#define CMD_CRC_CHECK_EN_POS                3
#define CMD_INDEX_CHENK_EN_POS              4
#define DATA_PRESENT_SELECT_POS             5
#define CMD_TYPE_POS                        6
#define CMD_INDEX_POS                       8

#define reg_sdmmc_cmd_transfer_mode_b1        REG_ADDR16(SDMMC_BASE_ADDR+0x0e)
enum{
    FLD_RSP_TYPE_SELECT_B1                    =BIT_RNG(0,1),
    FLD_SUB_CMD_FLAG_B1                       =BIT(2),
    FLD_CMD_CRC_CHECK_EN_B1                   =BIT(3),
    FLD_CMD_INDEX_CHENK_EN_B1                 =BIT(4),
    FLD_DATA_PRESENT_SELECT_B1                =BIT(5),
    FLD_CMD_TYPE_B1                           =BIT_RNG(6,7),
    FLD_CMD_INDEX_B1                          =BIT_RNG(8,13),
    FLD_RESERVED_CMD_B1                       =BIT_RNG(14,15),
};
#define reg_sdmmc_response_0               REG_ADDR32(SDMMC_BASE_ADDR+0x10)
enum{
    FLD_REDPONSE_0_0                        =BIT_RNG(0,7),
    FLD_REDPONSE_0_1                        =BIT_RNG(8,15),
    FLD_REDPONSE_0_2                        =BIT_RNG(16,23),
    FLD_REDPONSE_0_3                        =BIT_RNG(24,31),
};

#define reg_sdmmc_response_1               REG_ADDR32(SDMMC_BASE_ADDR+0x14)
enum{
    FLD_REDPONSE_1_0                        =BIT_RNG(0,7),
    FLD_REDPONSE_1_1                        =BIT_RNG(8,15),
    FLD_REDPONSE_1_2                        =BIT_RNG(16,23),
    FLD_REDPONSE_1_3                        =BIT_RNG(24,31),
};
#define reg_sdmmc_response_2               REG_ADDR32(SDMMC_BASE_ADDR+0x18)
enum{
    FLD_REDPONSE_2_0                        =BIT_RNG(0,7),
    FLD_REDPONSE_2_1                        =BIT_RNG(8,15),
    FLD_REDPONSE_2_2                        =BIT_RNG(16,23),
    FLD_REDPONSE_2_3                        =BIT_RNG(24,31),
};
#define reg_sdmmc_response_3               REG_ADDR32(SDMMC_BASE_ADDR+0x1c)
enum{
    FLD_REDPONSE_3_0                        =BIT_RNG(0,7),
    FLD_REDPONSE_3_1                        =BIT_RNG(8,15),
    FLD_REDPONSE_3_2                        =BIT_RNG(16,23),
    FLD_REDPONSE_3_3                        =BIT_RNG(24,31),
};
#define reg_sdmmc_data_port                REG_ADDR32(SDMMC_BASE_ADDR+0x20)
enum{
    FLD_DATA_PORT_0                         =BIT_RNG(0,7),
    FLD_DATA_PORT_1                         =BIT_RNG(8,15),
    FLD_DATA_PORT_2                         =BIT_RNG(16,23),
    FLD_DATA_PORT_3                         =BIT_RNG(24,31),
};
#define reg_sdmmc_response_state             REG_ADDR32(SDMMC_BASE_ADDR+0x24)
#define reg_sdmmc_response_state_b0          REG_ADDR8(SDMMC_BASE_ADDR+0x24)
enum{
    FLD_C0MMAND_INHIBIT_CMD                 =BIT(0),
    FLD_COMMAND_INHIBIT_DAT                 =BIT(1),
    FLD_DAR_LINE_ACTIVE                     =BIT(2),
    FLD_RETUNING_REQUEST                   =BIT(3),
    FLD_DAT_07_04_LINE_SIGNAK_LEVEL         =BIT_RNG(4,7),
};


#define reg_sdmmc_response_state_b1          REG_ADDR8(SDMMC_BASE_ADDR+0x25)
enum{
    FLD_WRITE_TRANSFER_ACTIVE               =BIT(0),
    FLD_READ_TRANSFER_ACTIVE                =BIT(1),
    FLD_WRITE_BUFFER_EN                     =BIT(2),
    FLD_READ_BUFFER_EN                      =BIT(3),
};

#define reg_sdmmc_response_state_b2          REG_ADDR8(SDMMC_BASE_ADDR+0x26)
enum{
    FLD_CARD_INSERTED                       =BIT(0),
    FLD_CARD_SRATE_STABLE                   =BIT(1),
    FLD_CARD_DETECT_PIN_LEVEL               =BIT(2),
    FLD_CARD_WRITE_PROTECT_SWITCH_PIN_LEVEL =BIT(3),
    FLD_03_00_LINE_SIGNAL_LEVEL             =BIT_RNG(4,7),
};

#define reg_sdmmc_response_state_b3          REG_ADDR8(SDMMC_BASE_ADDR+0x27)
enum{
    FLD_LINE_SIGNAL_LEVEL                   =BIT(0),
    FLD_HOST_REGULATOR_VOLTAGE_STABLE       =BIT(1),
    FLD_RESERVED_PRESENT_STATE              =BIT(2),
    FLD_CMD_NOT_ISSUDE_BY_ERR               =BIT(3),
    FLD_SUB_COMMAND_STS                     =BIT(4),
    FLD_PRESENT_STATE_RFU                   =BIT_RNG(5,7),
};

#define reg_sdmmc_host_ctrl_b0               REG_ADDR8(SDMMC_BASE_ADDR+0x28)
enum{
    FLD_LED_CONTROL                        =BIT(0),
    FLD_DATA_TRANSFER_WIDTH                =BIT(1),
    FLD_HIGH_SPEEND_EN                     =BIT(2),
    FLD_DMA_SELECT                         =BIT_RNG(3,4),
    FLD_EXTENDED_DATA_TRANSFER_WIDTH       =BIT(5),
    FLD_CARD_DETECT_TEST_LEVEL             =BIT(6),
    FLD_CARD_DETECT_SIGNEAL_DETECTION      =BIT(7),
};

#define reg_sdmmc_host_ctrl_b1               REG_ADDR8(SDMMC_BASE_ADDR+0x29)
enum{
    FLD_SD_BUS_POWER_FOR_VDD1              =BIT(0),
    FLD_SD_BUS_VOLTAGE_SELECT_FOR_VDD1     =BIT_RNG(1,2),
    FLD_SD_BUS_POWER_FOR_VDD2              =BIT(3),
    FLD_SD_BUS_VOLTAGE_SELECT_FOR_VDD2     =BIT_RNG(4,7),
};

#define reg_sdmmc_host_ctrl_b2               REG_ADDR8(SDMMC_BASE_ADDR+0x2a)
enum{
    FLD_STOP_AT_BLK_GAP_REQUEST            =BIT(0),
    FLD_CONTTINUE_REQUEST                  =BIT(1),
    FLD_RFU_READ_WAIT_CONTROL              =BIT(2),
    FLD_INTERRUPT_AT_BLK_GAP               =BIT(3),
    FLD_SPI_MODE_BLK_GAP_CTRL              =BIT(4),
    FLD_BOOT_EN                            =BIT(5),
    FLD_ALT_BOOT_EN                        =BIT(6),
    FLD_BOOT_ACK_CHECK                     =BIT(7),
};

#define reg_sdmmc_host_ctrl_b3               REG_ADDR8(SDMMC_BASE_ADDR+0x2b)
enum{
    RESERVED_WAKEUP_CTRL                   =BIT_RNG(0,7),
};

#define reg_sdmmc_clock_timeout_control    REG_ADDR32(SDMMC_BASE_ADDR+0x2c)
enum{
    FLD_INTTERNAL_CLK_EN                   =BIT(0),
    FLD_INTERNAL_CLK_STABLE                =BIT(1),
    FLD_HOST_SD_CLK_EN                     =BIT(2),
    FLD_RFU_POLL_EN                        =BIT(3),
    FLD_RESERVED_CLK_CONTROL_4             =BIT(4),
    FLD_CLK_GENERATOR_SELECT               =BIT(5),
    FLD_SDCLK_FREQ_SELECT0                 =BIT_RNG(6,7),
    FLD_SDCLK_FREQ_SELECT1                 =BIT_RNG(8,15),
    FLD_DATA_TIME_OUT_CNTR_VALUE           =BIT_RNG(16,19),
    FLD_RESERVDE_TIMEOUT_CTRL_7_4          =BIT_RNG(20,23),
    FLD_SOFTWARE_RST_FOR_ALL               =BIT(24),
    FLD_SOFTWARE_RST_FOR_CMD_LINE          =BIT(25),
    FLD_SOFTWARE_RST_FOR_DAT_LINE          =BIT(26),
    FLD_RESERVED_SOFTWARE_RST_7_3          =BIT_RNG(27,31),
};
#define reg_sdmmc_irq_sts                   REG_ADDR32(SDMMC_BASE_ADDR+0x30)
enum{
    FLD_CMD_COMPLETE_STS                   =BIT(0),
    FLD_TRANSFER_COMPLETE_STS              =BIT(1),
    FLD_BLK_GAP_EVENT_STS                  =BIT(2),
    FLD_DMA_INTERRUPT_STS                  =BIT(3),
    FLD_BUFFER_WRITE_READY_STS             =BIT(4),
    FLD_BUFFER_READ_READY_STS              =BIT(5),
    FLD_CARD_INSERTION_STS                 =BIT(6),
    FLD_CARD_REMOVAL_STS                   =BIT(7),

    FLD_IRQ_CARD_INTERRUPT_STS             =BIT(8),
    FLD_IRQ_INT_A_STS                      =BIT(9),
    FLD_IRQ_INT_B_STS                      =BIT(10),
    FLD_IRQ_INT_C_STS                      =BIT(11),
    FLD_IRQ_RE_TURNNING_EVENT_STS          =BIT(12),
    FLD_IRQ_BOOT_RCV                       =BIT(13),
    FLD_IRQ_BOOT_TERMINATE_INTERRUPT       =BIT(14),
    FLD_IRQ_ERR_INTERRUPT                  =BIT(15),

    FLD_IRQ_CMD_TINEOUT_ERR_STS            =BIT(16),
    FLD_IRQ_CMD_CRC_ERR_STS                =BIT(17),
    FLD_IRQ_END_BIT_ERR_STS                =BIT(18),
    FLD_IRQ_CMD_INDEX_ERR_TST              =BIT(19),
    FLD_IRQ_DATA_TIMEOUT_ERR_STS           =BIT(20),
    FLD_IRQ_DATA_CRC_ERR_STS               =BIT(21),
    FLD_IRQ_DATA_END_BIT_ERR_STS           =BIT(22),
    FLD_IRQ_CURRENT_LIMIT_ERR_STS          =BIT(23),

    FLD_IRQ_AUTO_CMD_ERR_STS               =BIT(24),
    FLD_IRQ_ADMA_ERR_STS                   =BIT(25),
    FLD_IRQ_TUNING_ERR_STS                =BIT(26),
    FLD_IRQ_RESPONSE_ERR_STS               =BIT(27),
    FLD_IRQ_TARGET_RESPONSE_ERR_STS        =BIT(28),
    FLD_IRQ_VENDOR_SPECIFIC_ERR0_STS       =BIT(29),
    FLD_IRQ_VENDOR_SPECIFIC_ERR1_STS       =BIT(30),
    FLD_IRQ_VEND0R_SPECIFIC_ERR2_STS       =BIT(31),
};

#define reg_sdmmc_cq_intr_sts_en            REG_ADDR32(SDMMC_BASE_ADDR+0x34)
enum{
    FLD_INTR_CMD_COMPLETE_STS               =BIT(0),
    FLD_INTR_TRANSFER_COMPLETE_STS          =BIT(1),
    FLD_INTR_BLK_GAP_ENVENT_STS             =BIT(2),
    FLD_INTR_DMA_INTERRUPT_STS              =BIT(3),
    FLD_INTR_BUFFER_WRITE_READY_STS         =BIT(4),
    FLD_INTR_BUFFER_READ_READY_STS          =BIT(5),
    FLD_INTR_CARD_INSERTION_STS             =BIT(6),
    FLD_INTR_CARD_REMOVAL_STS               =BIT(7),

    FLD_INTR_CARD_INTERRUPT_STS             =BIT(8),
    FLD_INTR_INT_A_STS                      =BIT(9),
    FLD_INTR_INT_B_STS                      =BIT(10),
    FLD_INTR_INT_C_STS                      =BIT(11),
    FLD_INTR_RE_TUNING_EVENT_STS            =BIT(12),
    FLD_INTR_BOOT_ACK_RCV                   =BIT(13),
    FLD_INTR_BOOT_TERMINATE                 =BIT(14),
    FLD_INTR_ERR_INTERRUPT                  =BIT(15),

    FLD_INTR_CMD_TINEOUT_ERR_STS            =BIT(16),
    FLD_INTR_CMD_CRC_ERR_STS                =BIT(17),
    FLD_INTR_END_BIT_ERR_STS                =BIT(18),
    FLD_INTR_CMD_INDEX_ERR_TST              =BIT(19),
    FLD_INTR_DATA_TIMEOUT_ERR_STS           =BIT(20),
    FLD_INTR_DATA_CRC_ERR_STS               =BIT(21),
    FLD_INTR_DATA_END_BIT_ERR_STS           =BIT(22),
    FLD_INTR_CURRENT_LIMIT_ERR_STS          =BIT(23),

    FLD_INTR_AUTO_CMD_ERR_STS               =BIT(24),
    FLD_INTR_ADMA_ERR_STS                   =BIT(25),
    FLD_INTR_TUNING_ERR_STS                 =BIT(26),
    FLD_INTR_RESPONSE_ERR_STS               =BIT(27),
    FLD_INTR_TARGET_RESPONSE_ERR_STS        =BIT(28),
    FLD_INTR_VENDOR_SPECIFIC_ERR0_STS       =BIT(29),
    FLD_INTR_VENDOR_SPECIFIC_ERR1_STS       =BIT(30),
    FLD_INTR_VEND0R_SPECIFIC_ERR2_STS       =BIT(31),
};

#define reg_sdmmc_intr_sig_en_b0             REG_ADDR8(SDMMC_BASE_ADDR+0x38)
enum{
    FLD_CMD_COMPLETE_SIGNAL_EN             =BIT(0),
    FLD_TRANSFER_COMPLETE_SIGNAL_EN        =BIT(1),
    FLD_BLK_GAP_ENVENT_SIGNAL_EN           =BIT(2),
    FLD_DMA_INTERRUPT_SIGNAL_EN            =BIT(3),
    FLD_BUFFER_WRITE_READYSIGNAL_EN        =BIT(4),
    FLD_BUFFER_READ_READYSIGNAL_EN         =BIT(5),
    FLD_CARD_INSERTION_SIGNAL_EN           =BIT(6),
    FLD_CARD_REMOVAL_SIGNAL_EN             =BIT(7),
};
#define reg_sdmmc_intr_sig_en_b1             REG_ADDR8(SDMMC_BASE_ADDR+0x39)
enum{
    FLD_CARD_INTERRUPT_SIGNAL_EN            =BIT(0),
    FLD_INT_A_SIGNAL_EN                     =BIT(1),
    FLD_INT_B_SIGNAL_EN                     =BIT(2),
    FLD_INT_C_SIGNAL_EN                     =BIT(3),
    FLD_RE_TURNNING_EVENT_SIGNAL_EN         =BIT(4),
    FLD_BOOT_RCV_SIGNAL_EN                  =BIT(5),
    FLD_BOOT_TERMINATE_INTERRUPT_SIGNAL_EN  =BIT(6),
    FLD_ERR_INTERRUPT_SIGNAL_EN             =BIT(7),
};
#define reg_sdmmc_intr_sig_en_b2             REG_ADDR8(SDMMC_BASE_ADDR+0x3a)
enum{
    FLD_CMD_TIMEOUT_ERR_SIGNAL_EN           =BIT(0),
    FLD_CRC_ERR_SIGNAL_EN                   =BIT(1),
    FLD_CMD_END_BIT_ERR_SIGNAL_EN           =BIT(2),
    FLD_CMD_INDEX_ERR_SIGNAL_EN             =BIT(3),
    FLD_TIMEOUT_ERR_SIGNAL_EN               =BIT(4),
    FLD_DATA_CRC_ERR_SIGNAL_EN              =BIT(5),
    FLD_DATA_END_BIT_ERR_SIGNAL_EN          =BIT(6),
    FLD_CURRENT_LIMIT_ERR_SIGNAL_EN         =BIT(7)
};
#define reg_sdmmc_intr_sig_en_b3             REG_ADDR8(SDMMC_BASE_ADDR+0x3b)
enum{
    FLD_AUTO_CMD_ERR_SIGNAL_EN              =BIT(0),
    FLD_ADMA_ERR_SIGNAL_EN                  =BIT(1),
    FLD_TUNING_ERR_SIGNAL_EN                =BIT(2),
    FLD_RESPOMCE_ERR_SIGNAL_EN              =BIT(3),
    FLD_VENDOR_SPECIFIC_ERR_SIGNAL_EN       =BIT_RNG(4,7),
};
#define reg_sdmmc_host_ctrl2_b0              REG_ADDR8(SDMMC_BASE_ADDR+0x3c)
enum{
    FLD_ACMD_12_NOT_EXECUTED                =BIT(0),
    FLD_ACMD_TIMEOUT_ERR                    =BIT(1),
    FLD_ACMD_CRC_ERR                        =BIT(2),
    FLD_ACMD_END_BIT_ERR                    =BIT(3),
    FLD_ACMD_INDEX_ERR                      =BIT(4),
    FLD_ACMD_RESPONSE_ERR                   =BIT(5),
    FLD_ACMD_HOST_CTRL2_RESERVED            =BIT(6),
    FLD_CMD_NOT_ISSUED_BY_ACMD_12_ERR       =BIT(7),
};
#define reg_sdmmc_host_ctrl2_b1              REG_ADDR8(SDMMC_BASE_ADDR+0x3d)
enum{
    FLD_RESERVED_AUTO_CMD_ERR_STS_15_8      =BIT_RNG(0,7),
};
#define reg_sdmmc_host_ctrl2_b2              REG_ADDR8(SDMMC_BASE_ADDR+0x3e)
enum{
    FLD_MODE_SELECT                         =BIT_RNG(0,2),
    FLD_SIGNALLING_EN_1_8_V                 =BIT(3),
    FLD_DRIVER_STRENGTH_SELECT              =BIT_RNG(4,5),
    FLD_EXECUTE_TUNINGING                  =BIT(6),
    FLD__SAMPLING_CLK_SELECT                =BIT(7),

};
#define reg_sdmmc_host_ctrl2_b3              REG_ADDR8(SDMMC_BASE_ADDR+0x3f)
enum{
    FLD_HOST_CTRL2_RSV2                     =BIT_RNG(0,1),
    FLD_ADMA2_LENGTH_MODE                   =BIT(2),
    FLD_CMD_23_EN                           =BIT(3),
    FLD_HOST_VERSION_4_EN                   =BIT(4),
    FLD_ADDRESSING_CLOCK_TIMEOUT_CONTROL_64BIT =BIT(5),
    FLD_ASYNCHRONOUS_INTERRUPT_EN           =BIT(6),
    FLD_PRESET_VALUE_EN                     =BIT(7),
};
#define reg_sdmmc_capabilities_0           REG_ADDR32(SDMMC_BASE_ADDR+0x40)
enum{
    FLD_TIMEOUT_CLK_FREQ                    =BIT_RNG(0,5),
    FLD_RESERVED_APABILITIES_07_00          =BIT(6),
    FLD_TIMEOUT_CLK_UNIT                    =BIT(7),

    FLD_BASE_CLK_FREQ_FOR_SD_CLK            =BIT_RNG(8,15),

    FLD_MAX_BLK_LENGTH                      =BIT_RNG(16,17),
    FLD_EMBEDDED_DEVICE_8BIT                =BIT(18),
    FLD_ADMA2_SUPPORT                       =BIT(19),
    FLD_RESERVED_CAPABILITIES_23_16         =BIT(20),
    FLD_HIGH_SPEED_SUPPORT                  =BIT(21),
    FLD_SDMA_SUPPORT                        =BIT(22),
    FLD_SUSPEND_RESUME_SUPPORT              =BIT(23),

    FLD_VOLTAGE_SUPPORT_3V3                 =BIT(24),
    FLD_VOLTAGE_SUPPORT_3V                  =BIT(25),
    FLD_VOLTAGE_SUPPORT_1V8                 =BIT(26),
    FLD_SYS_ADDR_SUPPORT_FOR_V4_64BIT       =BIT(27),
    FLD_SYS_ADDR_SUPPORT_FOR_V3_64BIT       =BIT(28),
    FLD_ASYNCHRONOUS_INTERRUPT_SUPPORT      =BIT(29),
    FLD_SLOT_TYPE                           =BIT_RNG(30,31),
};

#define reg_sdmmc_capabilities_1_b0          REG_ADDR8(SDMMC_BASE_ADDR+0x44)
enum{
    FLD_CAPABILITIES_1_RSV2                 =BIT_RNG(0,1),
    FLD_DDR_SUPPORT                         =BIT(2),
    FLD_CAPABILITIES_1_RSV1                 =BIT_RNG(3,7),
};
#define reg_sdmmc_capabilities_1_b1          REG_ADDR8(SDMMC_BASE_ADDR+0x45)
enum{
    FLD_TIMER_COUNT_FOR_RE_TUNING          =BIT_RNG(0,3),
    FLD_RESERVED_CAPABILITIES_44            =BIT(4),
    FLD_USE_TUNING_FOR_SDR                 =BIT(5),
    FLD_TUNING_MODES                       =BIT_RNG(6,7),
};
#define reg_sdmmc_capabilities_1_b2          REG_ADDR8(SDMMC_BASE_ADDR+0x46)
enum{
    FLD_CLK_MULTIPLIER                      =BIT_RNG(0,7),
};
#define reg_sdmmc_capabilities_1_b3          REG_ADDR8(SDMMC_BASE_ADDR+0x47)
enum{
    FLD_CAPABILITIES_1_RESERVED             =BIT_RNG(0,1),
    FLD_RESERCED_CAPBILITIES_58             =BIT(2),
    FLD_ADMA3_SUPPORT                       =BIT(3),
    FLD_VDD2_SUPPORT_1V8                    =BIT(4),
    FLD_RESERVED_FOR_FUTURE_VDD2            =BIT(5),
    FLD_RESERVED2_CAPABILITIES_62           =BIT(6),
    FLD_HS_400_SUPPORT                      =BIT(7),
};
#define reg_sdmmc_max_current_cap_0_b0       REG_ADDR8(SDMMC_BASE_ADDR+0x48)
enum{
    FLD_MAX_CURRENT_CAP_0_0                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_0_b1       REG_ADDR8(SDMMC_BASE_ADDR+0x49)
enum{
    FLD_MAX_CURRENT_CAP_0_1                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_0_b2       REG_ADDR8(SDMMC_BASE_ADDR+0x4a)
enum{
    FLD_MAX_CURRENT_CAP_0_2                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_0_b3       REG_ADDR8(SDMMC_BASE_ADDR+0x4b)
enum{
    FLD_MAX_CURRENT_CAP_0_3                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_1_b0       REG_ADDR8(SDMMC_BASE_ADDR+0x4c)
enum{
    FLD_MAX_CURRENT_CAP_1_0                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_1_b1       REG_ADDR8(SDMMC_BASE_ADDR+0x4d)
enum{
    FLD_MAX_CURRENT_CAP_1_1                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_1_b2       REG_ADDR8(SDMMC_BASE_ADDR+0x4e)
enum{
    FLD_MAX_CURRENT_CAP_1_2                 =BIT_RNG(0,7),
};
#define reg_sdmmc_max_current_cap_1_b3       REG_ADDR8(SDMMC_BASE_ADDR+0x4f)
enum{
    FLD_MAX_CURRENT_CAP_1_3                 =BIT_RNG(0,7),
};
#define reg_sdmmc_force_event_reg_b0         REG_ADDR8(SDMMC_BASE_ADDR+0x50)
enum{
    FLD_FE_FOR_ACMD_12_NOT_EXECUTED         =BIT(0),
    FLD_FE_FOR_ACMD_TIMEOUT_ERR             =BIT(1),
    FLD_FE_FOR_ACMD_CRC_ERR                 =BIT(2),
    FLD_FE_FOR_ACMD_END_BIT_ERR             =BIT(3),
    FLD_FE_FOR_ACMD_INDEX_ERR               =BIT(4),
    FLD_FE_FOR_ACMD_RESPONSE_ERR            =BIT(5),
    FLD_RESERVED_FE_7                       =BIT(6),
    FLD_FE_FOR_CMD_NOT_ISSUED_BY_ACMD12_ERR =BIT(7),
};
#define reg_sdmmc_force_event_reg_b1         REG_ADDR8(SDMMC_BASE_ADDR+0x51)
enum{
    FLD_RESERVED_FE_ATUOCMD_ERR_STS         =BIT_RNG(0,7),
};
#define reg_sdmmc_force_event_reg_b2         REG_ADDR8(SDMMC_BASE_ADDR+0x52)
enum{
    FLD_FE_FOR_CMD_TIMEOUT_ERR              =BIT(0),
    FLD_FE_FOR_CMD_CRC_ERR                  =BIT(1),
    FLD_FE_FOR_CMD_END_BIT_ERR              =BIT(2),
    FLD_FE_FOR_CMD_INDEX_ERR                =BIT(3),
    FLD_FE_FOR_DATA_TIMEOUT_ERR             =BIT(4),
    FLD_FE_FOR_DATA_CRC_ERR                 =BIT(5),
    FLD_FE_FOR_DATA_END_BIT_ERR             =BIT(6),
    FLD_FE_FOR_CURRENT_LINIT_ERR            =BIT(7),
};
#define reg_sdmmc_force_event_reg_b3         REG_ADDR8(SDMMC_BASE_ADDR+0x53)
enum{
    FLD_FE_FOR_ACMD_ERR                     =BIT(0),
    FLD_FE_FOR_ADMA_ERR_INTR_STS            =BIT(1),
    FLD_FE_FOR_TUNING_ERR                  =BIT(2),
    FLD_FE_FOR_RESPONSE_ERR                 =BIT(3),
    FLD_FE_FOR_VENDOR_SPC_ERR_STS           =BIT_RNG(4,7),
};
#define reg_sdmmc_adma_err_status            REG_ADDR8(SDMMC_BASE_ADDR+0x54)
enum{
    FLD_ADMA_ERR_STATES                     =BIT_RNG(0,1),
    FLD_ADMA_LENGTH_MISMATCH_ERR            =BIT(2),
    FLD_RESERVED_ADMA_ERR_STS_7_3           =BIT_RNG(3,7),
};
#define reg_sdmmc_adma_sys_addr0           REG_ADDR32(SDMMC_BASE_ADDR+0x58)

#define reg_sdmmc_adma_sys_addr1           REG_ADDR32(SDMMC_BASE_ADDR+0x5c)

#define reg_sdmmc_preset_0                 REG_ADDR32(SDMMC_BASE_ADDR+0x60)
enum{
    FLD_INT_SDCLK_FREQ_SELECT_VALUE_9_0     =BIT_RNG(0,9),
    FLD_INT_CLK_GENERATOR_SELECT            =BIT(10),
    FLD_INT_RESERVED_PRESET_13_11           =BIT_RNG(11,13),
    FLD_INT_DRIVER_SELECT_STRENGTH_VALUE    =BIT_RNG(14,15),
    FLD_DS_SDCLK_FREQ_SELECT_VALUE          =BIT_RNG(16,25),
    FLD_DS_CLKGENERATOR_SELECT              =BIT(26),
    FLD_DS_RESERVED_PRESET_13_11            =BIT_RNG(27,29),
    FLD_DS_DRIVER_SELECT_STRENGTH_VALUE     =BIT_RNG(30,31),
};
#define reg_sdmmc_preset_1                 REG_ADDR32(SDMMC_BASE_ADDR+0x64)
enum{
    FLD_HS_SDCLK_FREQ_SELECT_VALUE_9_0      =BIT_RNG(0,9),
    FLD_HS_CLK_GENERATOR_SELECT             =BIT(10),
    FLD_HS_RESERVED_PRESET_13_11            =BIT_RNG(11,13),
    FLD_HS_DRIVER_SELECT_STRENGTH_VALUE     =BIT_RNG(14,15),
    FLD_SDR12_SDCLK_FREQ_SELECT_VALUE       =BIT_RNG(16,25),
    FLD_SDR12_CLKGENERATOR_SELECT           =BIT(26),
    FLD_SDR12_RESERVED_PRESET_13_11         =BIT_RNG(27,29),
    FLD_SDR12_DRIVER_SELECT_STRENGTH_VALUE  =BIT_RNG(30,31),
};
#define reg_sdmmc_preset_2                 REG_ADDR32(SDMMC_BASE_ADDR+0x68)
enum{
    FLD_SDR25_SDCLK_FREQ_SELECT_VALUE_9_0   =BIT_RNG(0,9),
    FLD_SDR25_CLK_GENERATOR_SELECT          =BIT(10),
    FLD_SDR25_RESERVED_PRESET_13_11         =BIT_RNG(11,13),
    FLD_SDR25_DRIVER_SELECT_STRENGTH_VALUE  =BIT_RNG(14,15),
    FLD_SDR50_SDCLK_FREQ_SELECT_VALUE       =BIT_RNG(16,25),
    FLD_SDR50_CLKGENERATOR_SELECT           =BIT(26),
    FLD_SDR50_RESERVED_PRESET_13_11         =BIT_RNG(27,29),
    FLD_SDR50_DRIVER_SELECT_STRENGTH_VALUE  =BIT_RNG(30,31),
};
#define reg_sdmmc_preset_3                 REG_ADDR32(SDMMC_BASE_ADDR+0x6c)
enum{
    FLD_RFU_SDCLK_FREQ_SELECT_VALUE_9_0     =BIT_RNG(0,9),
    FLD_RFU_CLK_GENERATOR_SELECT            =BIT(10),
    FLD_RFU_RESERVED_PRESET_13_11           =BIT_RNG(11,13),
    FLD_RFU_DRIVER_SELECT_STRENGTH_VALUE    =BIT_RNG(14,15),
    FLD_DDR50_SDCLK_FREQ_SELECT_VALUE       =BIT_RNG(16,25),
    FLD_DDR50_CLKGENERATOR_SELECT           =BIT(26),
    FLD_DDR50_RESERVED_PRESET_13_11         =BIT_RNG(27,29),
    FLD_DDR50_DRIVER_SELECT_STRENGTH_VALUE  =BIT_RNG(30,31),
};
#define reg_sdmmc_preset_4                 REG_ADDR32(SDMMC_BASE_ADDR+0x70)
enum{
    FLD_HS400_SDCLK_FREQ_SELECT_VALUE_9_0   =BIT_RNG(0,9),
    FLD_HS400_CLK_GENERATOR_SELECT          =BIT(10),
    FLD_HS400_RESERVED_PRESET_13_11         =BIT_RNG(11,13),
    FLD_HS400_DRIVER_SELECT_STRENGTH_VALUE  =BIT_RNG(14,15),
    FLD_PRESET_4_RESERVED_31_16             =BIT_RNG(16,31)
};
#define reg_sdmmc_adma_desc_addr0          REG_ADDR32(SDMMC_BASE_ADDR+0x78)

#define reg_sdmmc_adma_desc_addr1          REG_ADDR32(SDMMC_BASE_ADDR+0x7c)

#define reg_sdmmc_boot_timeout_cnt         REG_ADDR32(SDMMC_BASE_ADDR+0xf4)

#define reg_sdmmc_vendor_specific_b          REG_ADDR16(SDMMC_BASE_ADDR+0xf8)
enum{
    FLD_VENDOR_ENBANCED_STROBE              =BIT(0),
    FLD_VENDOR_EMMC_HW_RESET                =BIT(1),
    FLD_VEMDOR_SPECIFIC_RFU1                =BIT_RNG(2,15),
};
#define reg_slot_intr_status_host_controller_ver REG_ADDR32(SDMMC_BASE_ADDR+0xfc)

#define reg_sdmmc_cq_version               REG_ADDR32(SDMMC_BASE_ADDR+0x200)

#define reg_sdmmc_cq_capabilities_b          REG_ADDR16(SDMMC_BASE_ADDR+0x204)
enum{
    FLD_CQ_FVAL                             =BIT_RNG(0,9),
    FLD_RFUO_CQ_CAPABILITIES                =BIT_RNG(10,11),
    FLD_CQ_FMUL                             =BIT_RNG(12,15),
};
#define reg_sdmmc_cq_config                REG_ADDR32(SDMMC_BASE_ADDR+0x208)
enum{
    FLD_CQ_ENABLE                           =BIT(0),
    FLD_RFU0_CQ_CONFIG                      =BIT_RNG(1,7),
    FLD_TASK_DESC_SIZE                      =BIT(8),
    FLD_RFU1_CQ_CONFIG                      =BIT_RNG(9,11),
    FLD_DCMD_ENABLE                         =BIT(12),
    FLD_RFU2_CQ_CONFIG                      =BIT_RNG(12,31),
};
#define reg_sdmmc_cq_control               REG_ADDR32(SDMMC_BASE_ADDR+0x20c)
enum{
    FLD_HALT_BIT                            =BIT(0),
    FLD_RFU0_CQ_CONTROL                     =BIT_RNG(1,7),
    FLD_CLEAR_ALL_TASKS                     =BIT(8),
    FLD_RFU1_CQ_CONTROL                     =BIT_RNG(9,31),
};
#define reg_sdmmc_irq_status_cq              REG_ADDR32(SDMMC_BASE_ADDR+0x210)
enum{
    FLD_IRQ_HALT_COMPLETE                   =BIT(0),
    FLD_IRQ_TASK_COMPLETE                   =BIT(1),
    FLD_IRQ_RESP_ERR_DET                    =BIT(2),
    FLD_IRQ_TASK_CLEARED                    =BIT(3),
    FLD_IRQ_TASK_ERR                        =BIT(4),
};
#define reg_sdmmc_intr_sts_en                REG_ADDR32(SDMMC_BASE_ADDR+0x214)
enum{
    FLD_INTR_HALT_COMPLETE                  =BIT(0),
    FLD_INTR_TASK_COMPLETE                  =BIT(1),
    FLD_INTR_RESP_ERR_DET                   =BIT(2),
    FLD_INTR_TASK_CLEARED                   =BIT(3),
    FLD_INTR_TASK_ERR                       =BIT(4),
};
#define reg_sdmmc_intr_sig_en                REG_ADDR8(SDMMC_BASE_ADDR+0x218)
enum{
    FLD_CQ_INTR_SIG_EN                      =BIT_RNG(0,7),
};
#define reg_sdmmc_cq_intr_coalescing       REG_ADDR32(SDMMC_BASE_ADDR+0x21c)
enum{
    FLD_TIMEOUT_VALUE                       =BIT_RNG(0,6),
    FLD_RFU0_CQ_INTR_COALESCING             =BIT(7),
    FLD_COALESCING_CTRTHRESOLD              =BIT_RNG(8,12),
    FLD_RFU1_CQ_INTR_COALESCING             =BIT_RNG(13,20),
    FLD_INTR_COALESCING_STSTUS              =BIT(21),
    FLD_RFU2_CQ_INTR_COALESCING             =BIT_RNG(22,30),
    FLD_COALESCING_ENABLE                   =BIT(31),
};
#define reg_sdmmc_cq_tdl_base_addresslo_b_low    REG_ADDR32(SDMMC_BASE_ADDR+0x220)

#define reg_sdmmc_cq_tdl_base_addresshi_b_high   REG_ADDR32(SDMMC_BASE_ADDR+0x224)

#define reg_sdmmc_cq_task_doorbell_b         REG_ADDR32(SDMMC_BASE_ADDR+0x228)

#define reg_sdmmc_cq_task_cpl_notf_b         REG_ADDR32(SDMMC_BASE_ADDR+0x22c)

#define reg_sdmmc_cq_dev_queue_stt_b         REG_ADDR32(SDMMC_BASE_ADDR+0x230)

#define reg_sdmmc_cq_dev_pending_tasks_b     REG_ADDR32(SDMMC_BASE_ADDR+0x234)

#define reg_sdmmc_cq_task_clear_b            REG_ADDR32(SDMMC_BASE_ADDR+0x238)

#define reg_sdmmc_cq_send_sts_config_1_b01   REG_ADDR16(SDMMC_BASE_ADDR+0x240)

#define reg_sdmmc_cq_send_sts_config_1_b2    REG_ADDR8(SDMMC_BASE_ADDR+0x242)

#define reg_sdmmc_cq_send_sts_config_2_b01   REG_ADDR16(SDMMC_BASE_ADDR+0x244)

#define reg_sdmmc_cq_dcmd_response_b        REG_ADDR32(SDMMC_BASE_ADDR+0x248)

#define reg_sdmmc_cq_resp_err_mask_b        REG_ADDR32(SDMMC_BASE_ADDR+0x250)

#define reg_sdmmc_cq_task_err_info        REG_ADDR32(SDMMC_BASE_ADDR+0x254)
enum{
    FLD_CMD_ERR_CMD_INDEX                  =BIT_RNG(0,5),
    FLD_RFU0_CQ_TASK_ERR_INFO              =BIT_RNG(6,7),
    FLD_CMD_ERR_TASK_ID                    =BIT_RNG(8,12),
    FLD_RFU1_CQ_TASK_ERR_INFO              =BIT_RNG(13,14),
    FLD_CMD_ERR_VALID                      =BIT(15),
    FLD_DAT_ERR_CMD_INDEX                  =BIT_RNG(16,21),
    FLD_RFU2_CQ_TASK_ERR_INFO              =BIT_RNG(22,23),
    FLD_DAT_ERR_TASK_ID                    =BIT_RNG(24,28),
    FLD_RFU3_CQ_TASK_ERR_INFO              =BIT_RNG(29,30),
    FLD_DAT_ERR_VALID                      =BIT(31),
};
#define reg_sdmmc_cq_last_cmd_index         REG_ADDR8(SDMMC_BASE_ADDR+0x258)
enum{
    FLD_LAST_CMDDIDX                       =BIT_RNG(0,5),
};
#define reg_sdmmc_cq_last_cmd_response    REG_ADDR32(SDMMC_BASE_ADDR+0x25c)

#define reg_sdmmc_err_taskid                REG_ADDR8(SDMMC_BASE_ADDR+0x260)
enum{
    FLD_TASKID                             =BIT_RNG(0,4),
};
#define reg_sdmmc_cfg_tuningcount           REG_ADDR8(SDMMC_BASE_ADDR+0x300)
enum{
    FLD_TUNING_COUNT                      =BIT_RNG(0,5),
    FLD_TUNING_1_5X_DIS                   =BIT(6),
};
#define reg_sdmmc_intr_sts_en_1             REG_ADDR8(SDMMC_BASE_ADDR+0x304)
enum{
    FLD_INTR_STS_EN_1                      =BIT(0),
};
#define reg_sdmmc_irq_status_1              REG_ADDR8(SDMMC_BASE_ADDR+0x308)
enum{
    FLD_CLOCK_READY_STS                    =BIT(0),
};
#define reg_sdmmc_intr_sig_en_1             REG_ADDR8(SDMMC_BASE_ADDR+0x30c)
enum{
    FLD_INTR_SIG_EN_1                      =BIT_RNG(0,1),
};
#define reg_sdmmc_driving_sampling_edge     REG_ADDR8(SDMMC_BASE_ADDR+0x310)
enum{
    FLD_WR_DATA_START_DRIVING_DEGE        =BIT(0),
    FLD_CMD_START_DRIVING_DEGE            =BIT(1),
    FLD_RD_DATA_START_DRIVING_DEGE        =BIT(2),
    FLD_RESP_START_SAMPLING_EDGE          =BIT(3),
    FLD_HS400_RD_DATA_START_SAMPLING_DEGE =BIT(4),
    FLD_HS400_RESP_START_SAMPLING_DEGE    =BIT(5),
};
#define reg_sdmmc_rd_blk_en                 REG_ADDR8(SDMMC_BASE_ADDR+0x314)
enum{
    FLD_RD_BLK_EN                          =BIT(0),
};
#define reg_sdmmc_ahb_beat_select           REG_ADDR8(SDMMC_BASE_ADDR+0x318)
enum{
    FLD_AHB_BEAT_SELECT                    =BIT_RNG(0,1),
};
#define reg_sdmmc_soc_timeout               REG_ADDR8(SDMMC_BASE_ADDR+0x1000)
enum{
    FLD_SOC_TIMEOUT                        =BIT_RNG(0,7),
};




#endif



