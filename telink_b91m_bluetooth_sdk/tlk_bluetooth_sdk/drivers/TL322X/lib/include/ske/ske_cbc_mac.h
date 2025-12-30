/********************************************************************************************************
 * @file    ske_cbc_mac.h
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
#ifndef SKE_CBC_MAC_H
#define SKE_CBC_MAC_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        SKE_CTX       ske_cbc_mac_ctx[1];
        unsigned char mac_bytes;
        unsigned char is_updated;
        unsigned char left_bytes;
        unsigned char block_buf[16];
        SKE_PADDING   padding;
        SKE_MAC       mac_action;
    } SKE_CBC_MAC_CTX;

    typedef struct
    {
        SKE_CTX       ske_cbc_mac_ctx[1];
        unsigned char mac_bytes;
    } SKE_CBC_MAC_DMA_CTX;

    //APIs

    /**
 * @brief       ske_lp cbc mac init(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CBC_MAC_CTX context.
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   mac_action       - The MAC action to perform, must be SKE_GENERATE_MAC or SKE_VERIFY_MAC.
 * @param[in]   padding          - The SKE LP CBC MAC padding scheme, such as SKE_NO_PADDING or SKE_ZERO_PADDING.
 * @param[in]   key              - The key in bytes.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   mac_bytes        - MAC byte length. Must be greater than 1 and not exceed the block length.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1.If key is from user input, make sure key is not NULL (in which case sp_key_idx is ignored),
 *                otherwise, if key is from secure port, (sp_key_idx & 0x7FFF) must be within [1, SKE_MAX_KEY_IDX].
 * @endverbatim
 */
    unsigned int ske_lp_cbc_mac_init(SKE_CBC_MAC_CTX *ctx, SKE_ALG alg, SKE_MAC mac_action, SKE_PADDING padding, unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

    /**
 * @brief       ske_lp cbc_mac update message(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CBC_MAC_CTX context.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of the message.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# msg_bytes could be any value.
 * @endverbatim
 */
    unsigned int ske_lp_cbc_mac_update(SKE_CBC_MAC_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       ske_lp cbc_mac finish, and get the mac(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CBC_MAC_CTX context.
 * @param[in,out] mac            - Input (for verifying mac), output (for generating mac).
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If ctx->mac_action is SKE_GENERATE_MAC, mac is output. If ctx->mac_action is SKE_VERIFY_MAC,
 *                mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 *             -# 2. For the case that padding is SKE_NO_PADDING, if the total length of message is not a multiple of
 *                block length, it will return error.
 * @endverbatim
 */
    unsigned int ske_lp_cbc_mac_final(SKE_CBC_MAC_CTX *ctx, unsigned char *mac);

    /**
 * @brief       ske_lp cbc mac(CPU style, one-off style)
 *
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   mac_action       - The MAC action to perform, must be SKE_GENERATE_MAC or SKE_VERIFY_MAC.
 * @param[in]   padding          - The SKE LP CBC MAC padding scheme, such as SKE_NO_PADDING or SKE_ZERO_PADDING.
 * @param[in]   key              - The key in bytes.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of the message.
 * @param[in,out] mac            - Input (for verifying mac), output (for generating mac).
 * @param[in]   mac_bytes        - MAC byte length, must be greater than 1 and not exceed the block length.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *             -# 2. msg_bytes cannot be 0.
 *             -# 3. If mac_action is SKE_GENERATE_MAC, mac is output. If mac_action is SKE_VERIFY_MAC,
 *                mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 *             -# 4. For the case that padding is SKE_NO_PADDING, if the total length of message is not a multiple of
 *                block length, it will return error.
 * @endverbatim
 */
    unsigned int ske_lp_cbc_mac(SKE_ALG alg, SKE_MAC mac_action, SKE_PADDING padding, unsigned char *key, unsigned short sp_key_idx, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac, unsigned char mac_bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief       ske_lp cbc mac dma style init
 *
 * @param[in]   ctx              - Pointer to the SKE_CBC_MAC_DMA_CTX context.
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   key              - The key in bytes.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   mac_bytes        - MAC byte length, must be greater than 1 and not exceed the block length.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 * @endverbatim
 */
    unsigned int ske_lp_dma_cbc_mac_init(SKE_CBC_MAC_DMA_CTX *ctx, SKE_ALG alg, unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

    unsigned int ske_lp_dma_cbc_mac_update_blocks_excluding_last_block(SKE_CBC_MAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words, unsigned int *tmp_out, SKE_CALLBACK callback);

    /**
 * @brief       ske_lp cbc mac dma style update message including the last block (or message tail), and get the mac
 *
 * @param[in]   ctx              - Pointer to the SKE_CBC_MAC_DMA_CTX context.
 * @param[in]   msg              - Message including the last block (or message tail).
 * @param[in]   msg_bytes        - Byte length of msg, cannot be 0.
 * @param[out]  tmp_out          - Temporary output ciphertext, SKE LP needs to output, it occupies the same blocks as the input msg.
 * @param[out]  mac              - Output, CBC_MAC, occupies a block.
 * @param[in]   callback         - Callback function pointer, this could be NULL, meaning doing nothing.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If the actual message length msg_bytes is not a multiple of block length, please ensure the
 *                last block (or message tail) is padded with 0 already.
 * @endverbatim
 */
    unsigned int ske_lp_dma_cbc_mac_update_including_last_block(SKE_CBC_MAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);

    /**
 * @brief       ske_lp cbc_mac (DMA style, one-off style)
 *
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   key              - The key in byte buffer style.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of message, cannot be 0.
 * @param[out]  tmp_out          - Temporary output ciphertext, SKE LP needs to output, it occupies the same blocks as the input msg.
 * @param[out]  mac              - Output, CBC MAC.
 * @param[in]   mac_bytes        - MAC byte length, must be greater than 1 and not exceed the block length.
 * @param[in]   callback         - Callback function pointer, this could be NULL, meaning doing nothing.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *             -# 2. msg_bytes cannot be 0.
 *             -# 3. If the actual message length msg_bytes is not a multiple of block length, please ensure the last block
 *                is padded with 0 already.
 * @endverbatim
 */
    unsigned int ske_lp_dma_cbc_mac(SKE_ALG alg, unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, unsigned int mac_bytes, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
