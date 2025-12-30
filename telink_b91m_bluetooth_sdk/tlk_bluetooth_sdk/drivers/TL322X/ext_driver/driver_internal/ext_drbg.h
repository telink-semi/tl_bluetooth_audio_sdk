/********************************************************************************************************
 * @file    ext_drbg.h
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
#ifndef EXT_DRBG_H_
#define EXT_DRBG_H_

#include "reg_include/soc.h"
/*******************************      drbg registers: 0x170170      ******************************/
#define DRBG_BASE_ADDR              0x170170

#define reg_drbg_cs_iv(i)           REG_ADDR32(DRBG_BASE_ADDR + 0x00 + 4 * i)

#define reg_drbg_cs_pv(i)           REG_ADDR32(DRBG_BASE_ADDR + 0x10 + 4 * i)

#define reg_drbg_cs_in(i)           REG_ADDR32(DRBG_BASE_ADDR + 0x20 + 4 * i)

#define reg_drbg_cs_step_cnt        REG_ADDR16(DRBG_BASE_ADDR + 0x28)

#define reg_drbg_cs_transaction_id  REG_ADDR8(DRBG_BASE_ADDR + 0x2a)

#define reg_drbg_cs_transaction_cnt REG_ADDR8(DRBG_BASE_ADDR + 0x2b)

#define reg_drbg_channel_num        REG_ADDR8(DRBG_BASE_ADDR + 0x2c)

#define reg_drbg_hr1_in             REG_ADDR8(DRBG_BASE_ADDR + 0x2d)

#define reg_drbg_hr1_out            REG_ADDR8(DRBG_BASE_ADDR + 0x2e)

#define reg_drbg_byte_index         REG_ADDR8(DRBG_BASE_ADDR + 0x2f)

#define reg_drbg_ctrl               REG_ADDR8(DRBG_BASE_ADDR + 0x30)

enum
{
    FLD_DRBG_START_P      = BIT(0),
    FLD_DRBG_INIT_P       = BIT(1),
    FLD_DRBG_SOFT_RST_P   = BIT(2),
    FLD_FUNC_CR1_START_P  = BIT(3),
    FLD_FUNC_HR1_START_P  = BIT(4),
    FLD_FUNC_DRBG_START_P = BIT(5),
    FLD_FUNC_ADDR_START_P = BIT(6),
    FLD_DRBG_STATUS_CLR_P = BIT(7),
};

#define reg_drbg_status REG_ADDR8(DRBG_BASE_ADDR + 0x31)

enum
{
    FLD_DRBG_STATUS = BIT_RNG(0, 1),

    FLD_DRBG_IDLE        = 0,
    FLD_DRBG_RUNNING     = 1,
    FLD_DRBG_ERROR       = 2,
    FLD_DRBG_NORMAL_DONE = 3,
};

#define reg_drbg_mapped_channel_array_ptr REG_ADDR16(DRBG_BASE_ADDR + 0x32)

#define reg_k_drbg(i)                     REG_ADDR32(DRBG_BASE_ADDR + 0x34 + 4 * i)

#define reg_v_drbg(i)                     REG_ADDR32(DRBG_BASE_ADDR + 0x44 + 4 * i)

#define reg_cs_drbg(i)                    REG_ADDR32(DRBG_BASE_ADDR + 0x54 + 4 * i)

#define reg_cs_initiator_addr             REG_ADDR32(DRBG_BASE_ADDR + 0x64)

#define reg_cs_reflector_addr             REG_ADDR32(DRBG_BASE_ADDR + 0x68)

#define reg_drbg_channel_array_ptr        REG_ADDR16(DRBG_BASE_ADDR + 0x6c)

#define reg_drbg_randombits_ptr           REG_ADDR16(DRBG_BASE_ADDR + 0x6e)

#define reg_drbg_irq_mask                 REG_ADDR8(DRBG_BASE_ADDR + 0x70)

#define reg_drbg_irq_clr                  REG_ADDR8(DRBG_BASE_ADDR + 0x71)

enum
{
    FLD_ALL_IRQ_CLEAR = BIT_RNG(0, 4),

    FLD_DRBG_DONE_IRQ_CLEAR       = BIT(0),
    FLD_DRBG_ERROR_IRQ_CLEAR      = BIT(1),
    FLD_CSA_3C_DONE_IRQ_CLEAR     = BIT(2),
    FLD_CSA_3C_ERROR_IRQ_CLEAR    = BIT(3),
    FLD_CSA_3C_ABNORMAL_IRQ_CLEAR = BIT(4),
};

#define reg_drbg_irq_raw REG_ADDR8(DRBG_BASE_ADDR + 0x72)

enum
{
    FLD_DRBG_DONE_IRQ_RAW       = BIT(0),
    FLD_DRBG_ERROR_IRQ_RAW      = BIT(1),
    FLD_CSA_3C_DONE_IRQ_RAW     = BIT(2),
    FLD_CSA_3C_ERROR_IRQ_RAW    = BIT(3),
    FLD_CSA_3C_ABNORMAL_IRQ_RAW = BIT(4),
};

#define reg_drbg_irq      REG_ADDR8(DRBG_BASE_ADDR + 0x73)

#define reg_drbg_dbg_ctrl REG_ADDR8(DRBG_BASE_ADDR + 0x74)

#define reg_drbg_dbg      REG_ADDR8(DRBG_BASE_ADDR + 0x75)

#define reg_drbg_fsm      REG_ADDR8(DRBG_BASE_ADDR + 0x76)

#define reg_csa_3c_fsm    REG_ADDR8(DRBG_BASE_ADDR + 0x77)

#define reg_csa_3c_ctrl1  REG_ADDR8(DRBG_BASE_ADDR + 0x78)

enum
{
    FLD_CS_CSA_3C_SELECTION = BIT(0),
    FLD_CS_SHAPE_SELECTION  = BIT(1),
    FLD_CS_NUM_REPETITIONS  = BIT_RNG(2, 3),
    FLD_CS_CHANNEL_JUMP     = BIT_RNG(4, 7),
};

#define reg_csa_3c_ctrl2 REG_ADDR8(DRBG_BASE_ADDR + 0x79)

enum
{
    FLD_CS_NSHAPE_ITERATION = BIT_RNG(0, 1),
};

#define reg_csa_3c_chm(i) REG_ADDR8(DRBG_BASE_ADDR + 0x7a + i)

#define reg_csa_3c_param0 REG_ADDR8(DRBG_BASE_ADDR + 0x84)

#define reg_csa_3c_param1 REG_ADDR8(DRBG_BASE_ADDR + 0x85)

#define reg_csa_3c_param2 REG_ADDR8(DRBG_BASE_ADDR + 0x86)

#define reg_csa_3c_param3 REG_ADDR8(DRBG_BASE_ADDR + 0x87)

#define reg_csa_3c_param4 REG_ADDR8(DRBG_BASE_ADDR + 0x88)

#define reg_csa_3c_param5 REG_ADDR8(DRBG_BASE_ADDR + 0x89)

#define reg_csa_3c_param6 REG_ADDR8(DRBG_BASE_ADDR + 0x8a)

#define reg_csa_3c_param7 REG_ADDR8(DRBG_BASE_ADDR + 0x8b)

#define reg_csa_3c_param8 REG_ADDR8(DRBG_BASE_ADDR + 0x8c)

#define reg_csa_3c_param9 REG_ADDR8(DRBG_BASE_ADDR + 0x8d)

void cs_iv_setup(unsigned int *cs_iv);

void cs_pv_setup(unsigned int *cs_pv);

void cs_in_setup(unsigned int *cs_in);

void cs_drbg_core_trigger(void);

void cs_h9_instantiation_trigger(void);

void cs_cs_drbg_start(void);

void cs_working_status_clear(void);

void cs_kdrbg_load(unsigned int *kdrbg);

void cs_vdrbg_load(unsigned int *vdrbg);

void cs_kdrbg_setup(unsigned int *kdrbg);

void cs_vdrbg_setup(unsigned int *vdrbg);

void cs_step_cnt_setup(unsigned char stepCnt);

void cs_transaction_id_setup(unsigned char transactionId);

void cs_transaction_cnt_setup(unsigned char transactionCnt);

void cs_randombits_load(unsigned int *randomBits);

void cs_accesscode_load(unsigned int *reflector_accessaddr, unsigned int *initiator_accessaddr);

void cs_channel_num(unsigned char chnNum);

void cs_channel_array_pointer_setup(unsigned short chnBuffer);

void cs_mapped_channel_array_pointer_setup(unsigned short mappedChnBuffer);

void cs_randombits_pointer_setup(unsigned char *randombitsBuffer);

void cs_restore_drbg_randombyte_index(unsigned char randomByteIndex);

unsigned char cs_load_drbg_randombyte_index(void);

void cs_hr1_in(unsigned char hr1In);

unsigned char cs_hr1_out(void);

void chn_cas_3c_ctrl(unsigned char *chm, unsigned char CSShapeSelection, unsigned char CSChannelJump, unsigned char CSNumRepetitions);

void chn_cas_3c_disable(void);

void cs_drbg_3c_trigger(void);

void cs_drbg_irq_clear(void);

unsigned char cs_get_NonMode0ShuffledChannelArrayNum(void);

void cs_nShapeIteration_setup(unsigned char nShapeIteration);

#endif
