/********************************************************************************************************
 * @file    ske_cmac.h
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
#ifndef SKE_CMAC_H
#define SKE_CMAC_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char k1[16];
        unsigned char k2[16];
        SKE_CTX       ske_cmac_ctx[1];
        unsigned char mac_bytes;
        unsigned char left_bytes;
        unsigned char block_buf[16];
        SKE_MAC       mac_action;
    } SKE_CMAC_CTX;

    typedef struct
    {
        unsigned char k1[16];
        unsigned char k2[16];
        SKE_CTX       ske_cmac_ctx[1];
        unsigned char mac_bytes;
    } SKE_CMAC_DMA_CTX;

    //APIs
    /**
 * @brief       ske_lp cmac init(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CMAC_CTX context.
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   mac_action       - The MAC action to perform, must be SKE_GENERATE_MAC or SKE_VERIFY_MAC.
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
    unsigned int ske_lp_cmac_init(SKE_CMAC_CTX *ctx, SKE_ALG alg, SKE_MAC mac_action, unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

    /**
 * @brief       ske_lp cmac update message(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CMAC_CTX context.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of the message.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. msg_bytes can be any value.
 * @endverbatim
 */
    unsigned int ske_lp_cmac_update(SKE_CMAC_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       ske_lp cmac finish, and get the mac or verify the mac(CPU style)
 *
 * @param[in]   ctx              - Pointer to the SKE_CMAC_CTX context.
 * @param[in,out] mac           - Input (for generating mac), output (for verifying mac).
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If ctx->mac_action is SKE_GENERATE_MAC, mac is output. If ctx->mac_action is SKE_VERIFY_MAC,
 *                mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 * @endverbatim
 */
    unsigned int ske_lp_cmac_final(SKE_CMAC_CTX *ctx, unsigned char *mac);

 /**
 * @brief       ske_lp cmac(CPU style, one-off style)
 *
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   mac_action       - The MAC action to perform, must be SKE_GENERATE_MAC or SKE_VERIFY_MAC.
 * @param[in]   key              - The key in bytes.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of the message.
 * @param[in,out] mac           - Input (for generating mac), output (for verifying mac).
 * @param[in]   mac_bytes        - MAC byte length, must be greater than 1 and not exceed the block length.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *             -# 2. msg_bytes can be any value.
 *             -# 3. If mac_action is SKE_GENERATE_MAC, mac is output. If mac_action is SKE_VERIFY_MAC,
 *                mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 * @endverbatim
 */
    unsigned int ske_lp_cmac(SKE_ALG alg, SKE_MAC mac_action, unsigned char *key, unsigned short sp_key_idx, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac, unsigned char mac_bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
     * @brief           ske_lp cmac dma style init
     * @param[in]       ctx                  - ske_cmac_dma_ctx_t context pointer
     * @param[in]       alg                  - ske_lp algorithm
     * @param[in]       key                  - key in bytes
     * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
     * @param[in]       mac_bytes            - mac byte length, must be bigger than 1, and not bigger than block length
     * @return          SKE_SUCCESS(success), other(error)
     * @note
     *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
     *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX
     */
    unsigned int ske_lp_dma_cmac_init(SKE_CMAC_DMA_CTX *ctx, SKE_ALG alg, unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);
    /**
 * @brief       ske_lp cmac dma style init
 *
 * @param[in]   ctx              - Pointer to the SKE_CMAC_DMA_CTX context.
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
    unsigned int ske_lp_dma_cmac_update_blocks_excluding_last_block(SKE_CMAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words, unsigned int *tmp_out, SKE_CALLBACK callback);

    /**
 * @brief       ske_lp cmac dma style update message including the last block (or message tail), and get the mac
 *
 * @param[in]   ctx              - Pointer to the SKE_CMAC_DMA_CTX context.
 * @param[in]   msg              - Message including the last block (or message tail).
 * @param[in]   msg_bytes        - Byte length of msg, could be 0.
 * @param[out]  tmp_out          - Temporary output ciphertext, SKE LP needs to output, it occupies the same blocks as the input msg,
 *                                  and if the whole message length is 0, tmp_out must be one block.
 * @param[out]  mac              - Output, CMAC, occupies a block.
 * @param[in]   callback         - Callback function pointer, this could be NULL, meaning doing nothing.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If the whole message length is 0, this case is supported. In this case, msg occupies a block,
 *                and please set msg_bytes to 0.
 * @endverbatim
 */
    unsigned int ske_lp_dma_cmac_update_including_last_block(SKE_CMAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);

    /**
 * @brief       ske_lp cmac(DMA style, one-off style)
 *
 * @param[in]   alg              - The SKE LP algorithm to be used.
 * @param[in]   key              - The key in byte buffer style.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                  (sp_key_idx & 0x7FFF) must be within the range [1, SKE_MAX_KEY_IDX].
 *                                  If the MSB of sp_key_idx is 1, it indicates the use of the lower 128 bits of a 256-bit key.
 * @param[in]   msg              - The message.
 * @param[in]   msg_bytes        - Byte length of message.
 * @param[out]  tmp_out          - Temporary output ciphertext, SKE LP needs to output, it occupies the same blocks as the input msg.
 * @param[out]  mac              - Output, MAC.
 * @param[in]   mac_bytes        - Byte length of MAC.
 * @param[in]   callback         - Callback function pointer, this could be NULL, meaning doing nothing.
 * @return      SKE_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. If key is from user input, ensure key is not NULL (in which case sp_key_idx is ignored);
 *                otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1, SKE_MAX_KEY_IDX].
 *             -# 2. msg_bytes is the actual byte length of message, it can be any value (including 0).
 *                (1). If msg_bytes is not 0, msg must have (msg_bytes + 15) / 16 blocks; if the last block is not full,
 *                    please pad with zero.
 *                (2). If msg_bytes is 0, msg occupies a block.
 * @endverbatim
 */
    unsigned int ske_lp_dma_cmac(SKE_ALG alg, unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, unsigned char mac_bytes, SKE_CALLBACK callback);

#endif


#ifdef __cplusplus
}
#endif

#endif
