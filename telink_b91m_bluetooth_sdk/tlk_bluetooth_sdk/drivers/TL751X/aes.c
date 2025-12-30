/********************************************************************************************************
 * @file    aes.c
 *
 * @brief   This is the source file for tl751x
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "aes.h"
#include "compiler.h"

/**********************************************************************************************************************
 *                                            local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
_attribute_aes_data_sec_ unsigned int aes_data_buff[8];
unsigned int                          aes_embase_addr = 0xc0000000;
static unsigned int                   embase_offset   = 0; //the embase address offset with IRAM head address.
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to wait aes encrypt/decrypt done.
 * @return    none.
 */
static inline void aes_wait_done(void);

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to encrypt/decrypt to set key and data. AES module register must be used by word.
 *              All data need Little endian.
 * @param[in] key  - the key of encrypt/decrypt.
 * @param[in] data - the data which to do encrypt/decrypt. The address is 32 bits, but only the lower 16 bits are used.
 * @return    none.
 * @note      reg_embase_addr (32bit) +reg_aes_ptr (16bit) is the actual access address.
 *            reg_aes_ptr is only 16bit, so access space is only 64K. Adjusting reg_embase_addr changes the initial address of 64K.
 */
void aes_set_key_data(unsigned char *key, unsigned char *data)
{
    unsigned int temp;
    reg_embase_addr = aes_embase_addr; //set the embase addr
    for (unsigned char i = 0; i < 4; i++) {
        temp             = key[16 - (4 * i) - 4] << 24 | key[16 - (4 * i) - 3] << 16 | key[16 - (4 * i) - 2] << 8 | key[16 - (4 * i) - 1];
        reg_aes_key(i)   = temp;
        temp             = data[16 - (4 * i) - 4] << 24 | data[16 - (4 * i) - 3] << 16 | data[16 - (4 * i) - 2] << 8 | data[16 - (4 * i) - 1];
        aes_data_buff[i] = temp;
    }

    reg_aes_ptr = (unsigned int)aes_data_buff - embase_offset; //the aes data ptr is base on embase address.
}

/**
 * @brief     This function refer to encrypt/decrypt to get result. AES module register must be used by word.
 * @param[in] result - the result of encrypt/decrypt, Little endian.
 * @return    none.
 */
void aes_get_result(unsigned char *result)
{
    /* read out the result */
    unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
    for (unsigned char i = 0; i < 16; i++) {
        result[i] = ptr[15 - i];
    }
}

/**
 * @brief     This function refer to encrypt. AES module register must be used by word, all data need big endian.
 * @param[in] key       - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result    - the result of encrypt.
 * @return    none
 */
int aes_encrypt(unsigned char *key, unsigned char *plaintext, unsigned char *result)
{
    //set the key
    aes_set_key_data(key, plaintext);

    aes_set_mode(AES_ENCRYPT_MODE); //cipher mode

    aes_wait_done();

    aes_get_result(result);

    return 1;
}

/**
 * @brief     This function refer to decrypt. AES module register must be used by word.all data need big endian.
 * @param[in] key         - the key of decrypt.
 * @param[in] decrypttext - the text of decrypt.
 * @param[in] result      - the result of decrypt.
 * @return    none.
 */
int aes_decrypt(unsigned char *key, unsigned char *decrypttext, unsigned char *result)
{
    //set the key
    aes_set_key_data(key, decrypttext);

    aes_set_mode(AES_DECRYPT_MODE); //decipher mode

    aes_wait_done();

    aes_get_result(result);

    return 1;
}

/**
 * @brief     This function refer to match the rpa.
 * @param[in] irk         - the irk sequence, max 16 group(16byte a group). The address is 32 bits, but only the lower 16 bits are used.
 * @param[in] irk_len     - the irk group num.
 * @param[in] rpa         - the rpa which want to match.
 * @return    0xff-not matched, (0x0-0x0f)-the match group index.
 * @note      reg_embase_addr (32bit) +reg_aes_irk_ptr (16bit) is the actual access address.
 *            reg_aes_irk_ptr is only 16bit, so access space is only 64K. Adjusting reg_embase_addr changes the initial address of 64K.
 */
unsigned char aes_rpa_match(unsigned char *irk, unsigned char irk_len, unsigned char *rpa)
{
    unsigned int prand = rpa[3] | (rpa[4] << 8) | (rpa[5] << 16);
    unsigned int hash  = rpa[0] | (rpa[1] << 8) | (rpa[2] << 16);

    reg_embase_addr = aes_embase_addr; //set the embase addr

    reg_aes_hash_status |= FLD_AES_RPASE_STA_CLR;
    reg_aes_rpase_cnt = (prand & FLD_AES_PRAND) | ((irk_len << 24) & FLD_AES_IRK_NUM) | FLD_AES_RPASE_EN;

    reg_aes_irk_ptr = (unsigned int)irk;

    reg_aes_hash_status = (hash & FLD_AES_HASH_STA);

    reg_aes_rpase_cnt |= FLD_AES_RPASE_START;

    while (BIT(30) != (reg_aes_hash_status & FLD_AES_RPASE_STA))
        ;

    unsigned int match_status = reg_aes_hash_status;

    if (match_status & FLD_AES_HASH_MATCH) {
        return ((match_status >> 24) & 0x0f);
    }
    return (0xff);
}

/**********************************************************************************************************************
  *                                         local function implementation                                             *
  *********************************************************************************************************************/
/**
 * @brief     This function refer to set the embase addr.
 * @param[in] addr - the base addr of CEVA data.the [addr,addr+64k) need to cover the head address of the session of aes_data,
 *                      Maybe you should to modify the link file to change the aes_data session address.
 * @return    none.
 */
void aes_set_em_base_addr(unsigned int addr)
{
    aes_embase_addr = addr; //set the embase addr
    embase_offset   = addr;
}

/**
 * @brief     This function refer to wait aes crypt done.
 * @return    none.
 */
static inline void aes_wait_done(void)
{
    while (FLD_AES_START == (reg_aes_mode & FLD_AES_START))
        ;
}
