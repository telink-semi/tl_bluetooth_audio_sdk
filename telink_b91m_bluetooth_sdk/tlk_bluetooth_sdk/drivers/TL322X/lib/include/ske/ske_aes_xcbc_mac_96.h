/********************************************************************************************************
 * @file    ske_aes_xcbc_mac_96.h
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
#ifndef AES_XCBC_MAC_96_H
#define AES_XCBC_MAC_96_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char k2[16];
        unsigned char k3[16];
        unsigned char block_buf[16];
        SKE_CTX       ske_xcbc_mac_ctx[1];
        unsigned char left_bytes;
        SKE_MAC       mac_action;
    } SKE_AES_XCBC_MAC_96_CTX;

    typedef struct
    {
        unsigned char k2[16];
        unsigned char k3[16];
        SKE_CTX       ske_xcbc_mac_ctx[1];
    } SKE_AES_XCBC_MAC_96_DMA_CTX;

    //APIs
    /**
 * @brief       Initialize AES-XCBC-MAC-96 for CPU style (ske_lp).
 * @param[in]   ctx              - Pointer to the SKE_AES_XCBC_MAC_96_CTX context.
 * @param[in]   mac_action       - The action to be performed:
 *                                 must be either SKE_GENERATE_MAC or SKE_VERIFY_MAC.
 * @param[in]   key              - AES key in bytes.
 * @param[in]   sp_key_idx       - Index of the secure port key.
 *                                 (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 *                                 If the MSB of sp_key_idx is 1, the low 128 bits of the 256-bit key are used.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. If key is from user input, please make sure key is not NULL (now sp_key_idx is useless),
 *         otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 * @endverbatim
 */
    unsigned int ske_lp_aes_xcbc_mac_96_init(SKE_AES_XCBC_MAC_96_CTX *ctx, SKE_MAC mac_action, unsigned char *key, unsigned short sp_key_idx);

    /**
 * @brief       Update the message for AES-XCBC-MAC-96 (CPU style) operation.
 * @param[in]   ctx              - Pointer to the SKE_AES_XCBC_MAC_96_CTX context.
 * @param[in]   msg              - Input message.
 * @param[in]   msg_bytes        - The byte length of the message.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. msg_bytes can be any value.
 * @endverbatim
 */
    unsigned int ske_lp_aes_xcbc_mac_96_update(SKE_AES_XCBC_MAC_96_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       Finish the AES-XCBC-MAC-96 operation and get or verify the MAC (CPU style).
 * @param[in]   ctx            - Pointer to the SKE_AES_XCBC_MAC_96_CTX context.
 * @param[in]   mac            - Input for generating MAC, output for verifying MAC.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. If ctx->mac_action is SKE_GENERATE_MAC, mac is the output. If ctx->mac_action is SKE_VERIFY_MAC, mac is the input. A return value of SKE_SUCCESS means the MAC is valid; otherwise, the MAC is invalid.
 * @endverbatim
 */
    unsigned int ske_lp_aes_xcbc_mac_96_final(SKE_AES_XCBC_MAC_96_CTX *ctx, unsigned char mac[12]);

    /**
 * @brief       Perform the AES-XCBC-MAC-96 operation (CPU style or one-off style).
 * @param[in]   mac_action       - The action to perform: must be either SKE_GENERATE_MAC or SKE_VERIFY_MAC.
 * @param[in]   key              - The key in bytes (user-provided or from secure port).
 * @param[in]   sp_key_idx       - Index of the secure port key. The value (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 *                                 If the MSB of sp_key_idx is set, the low 128 bits of a 256-bit key are used.
 * @param[in]   msg              - The message to process.
 * @param[in]   msg_bytes        - The byte length of the message.
 * @param[in]   mac              - Input for generating the MAC, output for verifying the MAC.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. If the key is from user input, ensure the key is not NULL. If the key is from the secure port,
 *         the value (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 *      -# 2. msg_bytes can be any value.
 *      -# 3. If mac_action is SKE_GENERATE_MAC, mac is the output. If mac_action is SKE_VERIFY_MAC,
 *         mac is the input. A return value of SKE_SUCCESS means the MAC is valid; otherwise, the MAC is invalid.
 * @endverbatim
 */
    unsigned int ske_lp_aes_xcbc_mac_96(SKE_MAC mac_action, unsigned char *key, unsigned short sp_key_idx, unsigned char *msg, unsigned int msg_bytes, unsigned char mac[12]);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief       Initialize the AES-XCBC-MAC-96 operation in DMA style.
 * @param[in]   ctx              - Pointer to the SKE_AES_XCBC_MAC_96_DMA_CTX context.
 * @param[in]   key              - The key in bytes (user-provided or from secure port).
 * @param[in]   sp_key_idx       - Index of the secure port key. The value (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 *                                 If the MSB of sp_key_idx is set, the low 128 bits of a 256-bit key are used.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. If the key is from user input, ensure the key is not NULL. If the key is from the secure port,
 *         the value (sp_key_idx & 0x7FFF) must be in the range [1, SKE_MAX_KEY_IDX].
 * @endverbatim
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_init(SKE_AES_XCBC_MAC_96_DMA_CTX *ctx, unsigned char *key, unsigned short sp_key_idx);

    /**
 * @brief       Update the AES-XCBC-MAC-96 operation in DMA style with message blocks (excluding the last block or message tail).
 * @param[in]   ctx              - Pointer to the SKE_AES_XCBC_MAC_96_DMA_CTX context.
 * @param[in]   msg              - The message blocks to update, excluding the last block (or message tail).
 * @param[in]   msg_words        - The word length of msg, must be a multiple of the block word length.
 * @param[out]  tmp_out          - Temporary output ciphertext, occupies the same block structure as the input message msg.
 * @param[in]   callback         - Pointer to the callback function. Can be NULL, in which case no action is performed.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. The input message msg must consist of full blocks and exclude the last block (or message tail).
 * @endverbatim
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_update_blocks_excluding_last_block(SKE_AES_XCBC_MAC_96_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words, unsigned int *tmp_out, SKE_CALLBACK callback);

    /**
 * @brief       Update the AES-XCBC-MAC-96 operation in DMA style, including the last block (or message tail), and generate the MAC.
 * @param[in]   ctx              - Pointer to the SKE_AES_XCBC_MAC_96_DMA_CTX context.
 * @param[in]   msg              - The message, including the last block (or message tail).
 * @param[in]   msg_bytes        - The byte length of msg, could be 0.
 * @param[out]  tmp_out          - Temporary output ciphertext, occupies the same block structure as the input msg.
 *                                 If the message length is 0, tmp_out must be one block.
 * @param[out]  mac              - The AES-XCBC-MAC-96 result, occupies 12 bytes.
 * @param[in]   callback         - Pointer to the callback function. Can be NULL, in which case no action is performed.
 * @return      SKE_SUCCESS(success), other(error)
 * @verbatim
 *      -# 1. If the whole message length is 0, the message must occupy one block, and msg_bytes should be set to 0.
 * @endverbatim
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_update_including_last_block(SKE_AES_XCBC_MAC_96_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);

    unsigned int ske_lp_dma_aes_xcbc_mac_96(unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_bytes, unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
