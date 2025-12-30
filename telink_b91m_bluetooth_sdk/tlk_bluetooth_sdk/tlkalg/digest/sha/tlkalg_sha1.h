/********************************************************************************************************
 * @file    tlkalg_sha1.h
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
#ifndef TLKALG_SHA1_H
#define TLKALG_SHA1_H


#define TLKALG_SHA1_HASH_SIZE (160 / 8)

typedef struct
{
    uint32_t state[5];
    uint32_t count[2];
    uint8_t buff[64];
} tlkalg_sha1_context_t;

typedef struct
{
    uint8_t value[TLKALG_SHA1_HASH_SIZE];
} tlkalg_sha1_digest_t;

/**
 * @brief       Initializes the SHA-1 context with default state values.
 * @param[out]  pContext    - the SHA-1 context to be initialized.
 * @return      None.
 */
void tlkalg_sha1_init(tlkalg_sha1_context_t *pContext);

/**
 * @brief       Updates the SHA-1 context with new data.
 * @param[in]   pContext    - the SHA-1 context to be updated.
 * @param[in]   pData       - the data to be added to the context.
 * @param[in]   dataLen     - the length of the data.
 * @return      None.
 */
void tlkalg_sha1_update(tlkalg_sha1_context_t *pContext, uint8_t *pData, uint32_t dataLen);

/**
 * @brief       Finalizes the SHA-1 computation and stores the result.
 * @param[in]   pContext    - the SHA-1 context to be finalized.
 * @param[out]  pDigest     - the buffer to store the resulting SHA-1 digest.
 * @return      None.
 */
void tlkalg_sha1_finish(tlkalg_sha1_context_t *pContext, tlkalg_sha1_digest_t *pHash);

/**
 * @brief       Computes the SHA-1 digest of the provided data.
 * @param[in]   pData       - the data to be hashed.
 * @param[in]   dataLen     - the length of the data.
 * @param[out]  pDigest     - the buffer to store the resulting SHA-1 digest.
 * @return      None.
 */
void tlkalg_sha1_result(uint8_t *pData, uint32_t dataLen, tlkalg_sha1_digest_t *pDigest);


#endif //TLKALG_SHA1_H
