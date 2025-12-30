/********************************************************************************************************
 * @file    ske_xts.h
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
#ifndef SKE_XTS_H
#define SKE_XTS_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char t[16];
        SKE_CRYPTO    crypto;
        SKE_CTX       ske_xts_ctx[1];
        unsigned int  c_bytes;
        unsigned int  current_bytes;
    } SKE_XTS_CTX;

    //APIs
    /**
 * @brief       ske xts mode init config
 *
 * @param[in]   ctx              - Pointer to the SKE_XTS_CTX context.
 * @param[in]   alg              - The SKE algorithm to be used.
 * @param[in]   crypto           - Encrypting or decrypting.
 * @param[in]   key              - The key in bytes, key = key1||key2.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   i                - I value, it has the same length as block length.
 * @param[in]   c_bytes          - Byte length of plaintext/ciphertext, cannot be less than block byte length.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *                Note: sp_key_idx is reserved at present, please input key directly.
 *             -# 2. Key consists of key1 and key2.
 *             -# 3. c_bytes cannot be less than block byte length.
 * @endverbatim
 */
    unsigned int ske_lp_xts_init(SKE_XTS_CTX *ctx, SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *i, unsigned int c_bytes);

    /**
 * @brief       ske xts mode encryption or decryption
 *
 * @param[in]   ctx              - Pointer to the SKE_XTS_CTX context.
 * @param[in]   in               - Input, plaintext or ciphertext.
 * @param[out]  out              - Output, ciphertext or plaintext.
 * @param[in]   bytes            - Byte length of input or output.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. To save memory, in and out could be the same buffer; in this case, the output will
 *                cover the input.
 *             -# 2. bytes must be a multiple of block byte length.
 *             -# 3. If the whole plaintext/ciphertext is too long, you can divide it into sections
 *                by block (16 bytes), then call this function to process the sections respectively.
 *                However, if ctx->c_bytes is not a multiple of block byte length, the input here
 *                should not contain the last two blocks of the whole input. In this case, the last
 *                2 blocks (actually the last block is not full) are left to the function
 *                ske_hp_xts_update_including_last_2_blocks().
 * @endverbatim
 */
    unsigned int ske_lp_xts_update_blocks(SKE_XTS_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief       ske xts mode encryption or decryption (for the case where ctx->c_bytes % 16 is not 0)
 *
 * @param[in]   ctx              - Pointer to the SKE_XTS_CTX context.
 * @param[in]   in               - Input, plaintext or ciphertext.
 * @param[out]  out              - Output, ciphertext or plaintext.
 * @param[in]   bytes            - Byte length of input or output.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. To save memory, in and out could be the same buffer; in this case, the output will
 *                cover the input.
 *             -# 2. The input must contain the last 2 blocks. Actually, this function is for the case where
 *                ctx->c_bytes % 16 is not 0.
 * @endverbatim
 */
    unsigned int ske_lp_xts_update_including_last_2_blocks(SKE_XTS_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief       ske xts mode finish
 *
 * @param[in]   ctx              - Pointer to the SKE_XTS_CTX context.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. This is the last step of XTS calling, and it is optional.
 * @endverbatim
 */
    unsigned int ske_lp_xts_final(SKE_XTS_CTX *ctx);

    /**
 * @brief       ske xts mode encrypting/decrypting
 *
 * @param[in]   alg              - The SKE algorithm to be used.
 * @param[in]   crypto           - Encrypting or decrypting.
 * @param[in]   key              - The key in bytes, key = key1||key2.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   i                - I value, it has the same length as block length.
 * @param[in]   in               - Input, plaintext or ciphertext.
 * @param[out]  out              - Output, ciphertext or plaintext.
 * @param[in]   c_bytes          - Actual byte length of input or output.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *                Note: sp_key_idx is reserved at present, please input key directly.
 *             -# 2. Key consists of key1 and key2.
 *             -# 3. c_bytes cannot be less than block byte length.
 * @endverbatim
 */
    unsigned int ske_lp_xts_crypto(SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *i, unsigned char *in, unsigned char *out, unsigned int bytes);


#ifdef __cplusplus
}
#endif

#endif
