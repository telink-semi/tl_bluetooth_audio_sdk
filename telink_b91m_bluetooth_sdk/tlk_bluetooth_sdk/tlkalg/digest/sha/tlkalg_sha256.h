/********************************************************************************************************
 * @file    tlkalg_sha256.h
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
#ifndef TLKALG_SHA256_H
#define TLKALG_SHA256_H


#define TLKALG_SHA256_BLOCK_SIZE  64
#define TLKALG_SHA256_DIGEST_SIZE (256 / 8)

typedef struct
{
    uint64_t length;
    uint32_t curlen;
    uint32_t state[8];
    uint8_t  buff[TLKALG_SHA256_BLOCK_SIZE];
} tlkalg_sha256_context_t;

typedef struct
{
    uint8_t value[TLKALG_SHA256_DIGEST_SIZE];
} tlkalg_sha256_digest_t;

/**
 * @brief       Initialize a SHA256 context for hashing.
 * @param[out]  pContext    - Pointer to the SHA256 context to initialize.
 * @return      None.
 */
void tlkalg_sha256_init(tlkalg_sha256_context_t *pContext);

/**
 * @brief       Update the SHA256 context with new data.
 * @param[in]   pContext    - Pointer to the SHA256 context.
 * @param[in]   pData       - Pointer to the input data to hash.
 * @param[in]   dataLen     - Length of the input data in bytes.
 * @return      None.
 */
void tlkalg_sha256_update(tlkalg_sha256_context_t *pContext, uint8_t *pData, uint32_t dataLen);

/**
 * @brief       Finalize the SHA256 hash computation and get the result.
 * @param[in]   pContext    - Pointer to the SHA256 context.
 * @param[out]  pDigest     - Pointer to store the resulting SHA256 hash.
 * @return      None.
 */
void tlkalg_sha256_finish(tlkalg_sha256_context_t *pContext, tlkalg_sha256_digest_t *pDigest);

/**
 * @brief       Compute the SHA256 hash of a single data block.
 * @param[in]   pData       - Pointer to the input data to hash.
 * @param[in]   dataLen     - Length of the input data in bytes.
 * @param[out]  pDigest     - Pointer to store the resulting SHA256 hash.
 * @return      None.
 */
void tlkalg_sha256_result(uint8_t *pData, uint32_t dataLen, tlkalg_sha256_digest_t *pDigest);


#endif //TLKALG_SHA256_H
