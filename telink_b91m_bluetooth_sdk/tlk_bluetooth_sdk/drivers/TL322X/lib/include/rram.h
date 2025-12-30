/********************************************************************************************************
 * @file    rram.h
 *
 * @brief   This is the header file for tl322x
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

#include "compiler.h"
#include "gpio.h"
#include "reg_include/rram_reg.h"
#include "error_handler/error_handler.h"

typedef enum
{
    RRAM_READ_DATA_CMD       = 0x0B,
    RRAM_READ2_DATA_CMD      = 0x03,
    RRAM_WRITE_ENABLE_CMD    = 0x06,
    RRAM_WRITE_DISENB_CMD    = 0x04,
    RRAM_WRITE_DATA_CMD      = 0x02,
    RRAM_READ_SREG_CMD       = 0x05,
    RRAM_WRITE_SREG_CMD      = 0x01,
    RRAM_ENTER_OTP_CMD       = 0x07,
    RRAM_ENTER_MTP_CMD       = 0x17,
    RRAM_WRITE_CC_LOCK_CMD   = 0x0A,
    RRAM_READ_CC_LOCK_CMD    = 0x1A,
    RRAM_WRITE_CR_LOCK_CMD   = 0x0C,
    RRAM_READ_CR_LOCK_CMD    = 0x1C,
    RRAM_WRITE_MP_CMD        = 0x0D,
    RRAM_READ_MP_CMD         = 0x1D,
    RRAM_WRITE_CREG_CMD      = 0x10,
    RRAM_READ_CREG_CMD       = 0x11,
    RRAM_WRITE_TREG_CMD      = 0x18,
    RRAM_READ_TREG_CMD       = 0x19,
    RRAM_RUN_MBIST_CMD       = 0x1F,
    RRAM_WRITE_RAW_DWORD_CMD = 0x12,
} rram_command_e;

typedef enum
{
    RRAM_MP_LOCK_NONE     = 0x00, //000000h-000000h
    RRAM_MP_LOCK_LOW_4K   = 0x01, //000000h-000FFFh
    RRAM_MP_LOCK_LOW_8K   = 0x02, //000000h-001FFFh
    RRAM_MP_LOCK_LOW_16K  = 0x04, //000000h-003FFFh
    RRAM_MP_LOCK_LOW_32K  = 0x08, //000000h-007FFFh
    RRAM_MP_LOCK_LOW_64K  = 0x10, //000000h-00FFFFh
    RRAM_MP_LOCK_LOW_128K = 0x20, //000000h-01FFFFh
    RRAM_MP_LOCK_LOW_256K = 0x40, //000000h-03EFFFh
    RRAM_MP_LOCK_LOW_512K = 0x80, //000000h-07FFFFh
    RRAM_MP_LOCK_ALL_512K = 0x80, //000000h-07FFFFh
} rram_mp_lock_block_e;

typedef enum
{
    RRAM_CR_LOCK_NONE    = 0x00, //000000h-000000h
    RRAM_CR_LOCK_0K_4K   = 0x01, //000000h-000FFFh
    RRAM_CR_LOCK_4K_8K   = 0x02, //001000h-001FFFh
    RRAM_CR_LOCK_8K_12K  = 0x04, //002000h-002FFFh
    RRAM_CR_LOCK_12K_16K = 0x08, //003000h-003FFFh
    RRAM_CR_LOCK_16K_20K = 0x10, //004000h-004FFFh
    RRAM_CR_LOCK_20K_24K = 0x20, //005000h-005FFFh
    RRAM_CR_LOCK_ALL_24K = 0x3F, //000000h-005FFFh
} rram_cr_lock_block_e;

typedef enum
{
    RRAM_CC_LOCK_NONE     = 0x0000, //006000h-000000h
    RRAM_CC_LOCK_0_63     = 0x0001, //006000h-00603Fh
    RRAM_CC_LOCK_64_127   = 0x0002, //006040h-00607Fh
    RRAM_CC_LOCK_128_191  = 0x0004, //006080h-0060BFh
    RRAM_CC_LOCK_192_255  = 0x0008, //0060C0h-0060FFh
    RRAM_CC_LOCK_256_319  = 0x0010, //006100h-00613Fh
    RRAM_CC_LOCK_320_383  = 0x0020, //006140h-00617Fh
    RRAM_CC_LOCK_384_447  = 0x0040, //006180h-0061BFh
    RRAM_CC_LOCK_448_511  = 0x0080, //0061C0h-0061FFh
    RRAM_CC_LOCK_512_575  = 0x0100, //006200h-00623Fh
    RRAM_CC_LOCK_576_639  = 0x0200, //006240h-00627Fh
    RRAM_CC_LOCK_640_703  = 0x0400, //006280h-0062BFh
    RRAM_CC_LOCK_704_767  = 0x0800, //0062C0h-0062FFh
    RRAM_CC_LOCK_768_831  = 0x1000, //006300h-00633Fh
    RRAM_CC_LOCK_832_895  = 0x2000, //006340h-00637Fh
    RRAM_CC_LOCK_896_959  = 0x4000, //006380h-0063BFh
    RRAM_CC_LOCK_960_1023 = 0x8000, //0063C0h-0063FFh
    RRAM_CC_LOCK_ALL_1024 = 0xFFFF, //006000h-0063FFh
} rram_cc_lock_block_e;

typedef enum
{
    RRAM_ERROR_W = BIT(7), //Write Error
    RRAM_ERROR_R = BIT(6), //Read Error
    RRAM_ERROR_P = BIT(5), //Protection Write Error
    RRAM_ERROR_A = BIT(4), //Write Address Error
    RRAM_ERROR_M = BIT(3), //MBIST Error
    RRAM_OTP     = BIT(2), //OTP Mode
    RRAM_WEL     = BIT(1), //Write Enable Latch
    RRAM_BUSY    = BIT(0), //Internal Operation Status
} rram_status_reg_e;

/**
 * @brief       This function servers to set system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static inline void rram_set_irq_mask(rram_irq_mask_e mask)
{
    reg_rram_ctrl |= mask;
}

/**
 * @brief       This function servers to clear system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static inline void rram_clr_irq_mask(rram_irq_mask_e mask)
{
    reg_rram_ctrl &= (~mask);
}

/**
 * @brief       This function servers to clear system timer irq status.
 * @param[in]   status - the irq status.
 * @return      none.
 */
static inline void rram_clr_irq_status(rram_irq_status_e status)
{
    reg_rram_err_irq = (status);
}

/**
 * @brief       This function servers to get system timer irq status.
 * @param[in]   status - the irq status.
 * @retval      non-zero -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char rram_get_irq_status(rram_irq_status_e status)
{
    return (reg_rram_err_irq & status);
}

/**
 * @brief       This function servers to clear system timer irq status.
 * @param[in]   status - the irq status.
 * @return      none.
 */
static inline void rram_clr_err_status(rram_err_status_e status)
{
    reg_rram_err_state = (status);
}

/**
 * @brief       This function servers to get system timer irq status.
 * @param[in]   status - the irq status.
 * @retval      non-zero -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char rram_get_err_status(rram_err_status_e status)
{
    return (reg_rram_err_state & status);
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_set_addr0(unsigned char addr)
{
    reg_rram_par_ain0 = addr;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_set_addr(unsigned short addr)
{
    reg_rram_par_ain = addr;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_set_len(unsigned short len)
{
    reg_rram_data_len = len;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_set_cmd(unsigned char cmd)
{
    reg_rram_par_mode = cmd;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned int rram_ahb_read_data(unsigned int addr)
{
    return reg_rram_ahb0_rd_data_word(addr);
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_bypass_cache(void)
{
    reg_rram_ctrl |= FLD_RRAM_CIPHER_BYPASS;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned int rram_reg_read_data(void)
{
    return reg_rram_wr_rd_data_word;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_write_data(unsigned int data)
{
    reg_rram_wr_rd_data_word = data;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_write_reg0(unsigned char data)
{
    reg_rram_wr_rd_reg0 = data;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_write_reg(unsigned short data)
{
    reg_rram_wr_rd_reg = data;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned char rram_reg_read_reg0(void)
{
    return reg_rram_wr_rd_reg0;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned short rram_reg_read_reg(void)
{
    return reg_rram_wr_rd_reg;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_reg_txfifo_clr(void)
{
    reg_rram_clr |= FLD_RRAM_TXFIFO_CLR;
    while (reg_rram_state & BIT(2))
        ;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned short rram_get_error_addr(void)
{
    return reg_rram_error_addr_save;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned int rram_get_error_data(void)
{
    return reg_rram_error_ahbslv_rd_word;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_io_enable(void)
{
    reg_rram_ctrl0 = 0x00;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_io_disable(void)
{
    reg_rram_ctrl0 = 0x01;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_enable(void)
{
    reg_rram_spi_st = FLD_RRAM_SPI_ENABLE;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_disable(void)
{
    reg_rram_spi_st = 0x00;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_set_cfg(unsigned char cfg)
{
    reg_rram_spi_cfg = cfg;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_spi_write_data(unsigned char data)
{
    reg_rram_spi_wdata = data;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline unsigned char rram_spi_read_data(void)
{
    return reg_rram_spi_rdata;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_set_core0_space(unsigned char space)
{
    reg_rram_core0_space_start  = 0;
    reg_rram_core0_space_size   = space - 1;
    reg_rram_core0_space_offset = space;
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void rram_set_core1_space(unsigned char space)
{
    reg_rram_core1_space_start  = 0;
    reg_rram_core1_space_size   = space - 1;
    reg_rram_core1_space_offset = space;
}

void rram_enter_otp(void);

void rram_enter_mtp(void);

void rram_write_enable(void);

void rram_write_disable(void);

unsigned short rram_read_reg(unsigned char cmd);

void rram_write_reg(unsigned char cmd, unsigned short data);

_attribute_ram_code_sec_ 
void rram_ahb0_read_word(unsigned int addr, unsigned int len, unsigned int *data);

void rram_ahb0_read_byte(unsigned int addr, unsigned int len, unsigned char *data);

void rram_ahb1_read_word(unsigned int addr, unsigned int len, unsigned int *data);

void rram_ahb1_read_byte(unsigned int addr, unsigned int len, unsigned char *data);

void rram_reg_read(unsigned int addr, unsigned int len, unsigned int *data);

_attribute_ram_code_sec_ 
void rram_reg_write(unsigned int addr, unsigned int len, unsigned int *data);

void rram_reg_write_abort_test(unsigned int addr, unsigned int len, unsigned int *data);

_attribute_ram_code_sec_ 
unsigned char rram_read_creg(unsigned char addr);

void rram_write_creg(unsigned char addr, unsigned char data);

unsigned char rram_read_treg(unsigned char addr);

void rram_write_treg(unsigned char addr, unsigned char data);

void rram_run_mbist(unsigned int *data);


void rram_set_tx_dma_config(dma_chn_e chn);

void rram_set_rx_dma_config(dma_chn_e chn);

void rram_reg_write_dma(dma_chn_e chn, unsigned int addr, unsigned int len, unsigned int *src_data);

void rram_reg_read_dma(dma_chn_e chn, unsigned int addr, unsigned int len, unsigned int *dst_data);


void rram_spi_enter_otp(void);

void rram_spi_enter_mtp(void);

void rram_spi_write_enable(void);

void rram_spi_write_disable(void);

unsigned char rram_spi_read_state_reg(void);

void rram_spi_read(unsigned int addr, unsigned char len, unsigned char *data);

void rram_spi_write(unsigned int addr, unsigned char len, unsigned char *data);

void rram_spi_read_creg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_spi_write_creg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_spi_read_treg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_spi_write_treg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_spi_run_mbist(unsigned char *data);

void rram_spi_write_raw_dword(unsigned int addr, unsigned char *data);


void rram_ex_spi_io_enable(void);

void rram_ex_spi_enter_otp(void);

void rram_ex_spi_enter_mtp(void);

void rram_ex_spi_write_enable(void);

void rram_ex_spi_write_disable(void);

void rram_ex_spi_read_state_reg(unsigned char *data);

void rram_ex_spi_read(unsigned int addr, unsigned int len, unsigned char *data);

void rram_ex_spi_write(unsigned int addr, unsigned int len, unsigned char *data);

void rram_ex_spi_read_creg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_ex_spi_write_creg(unsigned char addr, unsigned char len, unsigned char *data);

void rram_ex_spi_read_treg(unsigned char addr, unsigned int len, unsigned char *data);

void rram_ex_spi_write_treg(unsigned char addr, unsigned int len, unsigned char *data);

void rram_ex_spi_run_mbist(unsigned char *data);

void rram_ex_spi_write_raw_dword(unsigned int addr, unsigned char *data);
