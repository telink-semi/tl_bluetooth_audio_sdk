/********************************************************************************************************
 * @file    ext_aes.h
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
#ifndef DRIVERS_TL751X_EXT_DRIVER_DRIVER_LIB_EXT_AES_H_
#define DRIVERS_TL751X_EXT_DRIVER_DRIVER_LIB_EXT_AES_H_

#include "common/types.h"

//*********************Note: CRYPT*****************************/
enum
{
    FLD_TLK_CRYPT_ENABLE = BIT(1),
    FLD_TLK_MST_SLV      = BIT(2),
};

#define reg_rf_tlk_sk0  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa4)
#define reg_rf_tlk_sk1  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa8)
#define reg_rf_tlk_sk2  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xac)
#define reg_rf_tlk_sk3  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb0)

#define reg_tlk_sk(v)   REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa4 + (v * 4))


#define reg_rf_tlk_iv0  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb4)
#define reg_rf_tlk_iv1  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xb8)

#define reg_rf_tlk_aad  REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xbc)


#define reg_ccm_control REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x3f)

enum
{
    FLD_R_TXLEN_FLAG = BIT(0),
};

//39 bits
#define reg_rf_tx_ccm_pkt_cnt0_31  REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x40)
#define reg_rf_tx_ccm_pkt_cnt32_38 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x44)

#define reg_rf_rx_ccm_pkt_cnt0_31  REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x48)
#define reg_rf_rx_ccm_pkt_cnt32_38 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x4c)

#define CV_LLBT_BASE               (0xd4160000)

#define reg_rwbtcntl               REG_ADDR32(CV_LLBT_BASE)

enum
{

    FLD_NWINSIZE     = BIT_RNG(0, 5),
    FLD_RWBT_RSVD6_7 = BIT_RNG(6, 7),

    FLD_RWBTEN      = BIT(8),
    FLD_CX_DNABORT  = BIT(9),
    FLD_CX_RXBSYENA = BIT(10),
    FLD_CX_TXBSYENA = BIT(11),
    FLD_SEQNDSB     = BIT(12),
    FLD_ARQNDSB     = BIT(13),
    FLD_FLOWDSB     = BIT(14),
    FLD_HOPDSB      = BIT(15),

    FLD_WHITDSB       = BIT(16),
    FLD_CRCDSB        = BIT(17),
    FLD_CRYPTDSB      = BIT(18),
    FLD_LMPFLOWDSB    = BIT(19),
    FLD_SNIFF_ABORT   = BIT(20),
    FLD_PAGEINQ_ABORT = BIT(21),
    FLD_RFTEST_ABORT  = BIT(22),
    FLD_SCAN_ABORT    = BIT(23),


    FLD_RWBT_RSVD24_25     = BIT_RNG(24, 25),
    FLD_CRYPT_SOFT_RST     = BIT(26), /**HW AES_CMM module reset*/
    FLD_SWINT_REQ          = BIT(27),
    FLD_RADIOCNTL_SOFT_RST = BIT(28),
    FLD_REG_SOFT_RST       = BIT(29),
    FLD_MASTER_TGSOFT_RST  = BIT(30),
    FLD_MASTER_SOFT_RST    = BIT(31),

};

bool aes_resolve_irk_rpa(u8 *key, u8 *addr);

bool aes_encryption_hw(u8 *key, u8 *plaintext, u8 *encrypted_data);

void aes_encryption_le(u8 *key, u8 *plaintext, u8 *encrypted_data);
void aes_encryption_be(u8 *key, u8 *plaintext, u8 *encrypted_data);

void blt_ll_setAesCcmPara(u8 role, u8 *sk, u8 *iv, u8 aad, u64 enc_pno, u64 dec_pno, u8 lastTxLenFlag);

#endif /* DRIVERS_TL751X_EXT_DRIVER_DRIVER_LIB_EXT_AES_H_ */
