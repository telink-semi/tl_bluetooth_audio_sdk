/********************************************************************************************************
 * @file    tlkalg_md5.h
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
#ifndef TLKALG_MD5_H
#define TLKALG_MD5_H


#define TLKALG_MD5_HASH_SIZE (128 / 8)

typedef struct
{
    uint32_t lo;
    uint32_t hi;
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint8_t buff[64];
    uint32_t block[16];
} tlkalg_md5_context_t;

typedef struct
{
    uint8_t value[TLKALG_MD5_HASH_SIZE];
} tlkalg_md5_digest_t;

/**
 * @brief       Initialize an MD5 context for hashing.
 * @param[out]  pContext    - Pointer to the MD5 context to initialize.
 * @return      None.
 */
void tlkalg_md5_init(tlkalg_md5_context_t *pContext);

/**
 * @brief       Update the MD5 context with new data.
 * @param[in]   pContext    - Pointer to the MD5 context.
 * @param[in]   pData       - Pointer to the input data to hash.
 * @param[in]   dataLen     - Length of the input data in bytes.
 * @return      None.
 */
void tlkalg_md5_update(tlkalg_md5_context_t *pContext, uint8_t *pData, uint32_t dataLen);

/**
 * @brief       Finalize the MD5 hash computation and get the result.
 * @param[in]   pContext    - Pointer to the MD5 context.
 * @param[out]  pDigest     - Pointer to store the resulting MD5 hash.
 * @return      None.
 */
void tlkalg_md5_finish(tlkalg_md5_context_t *pContext, tlkalg_md5_digest_t *pDigest);

/**
 * @brief       Compute the MD5 hash of a single data block.
 * @param[in]   pData       - Pointer to the input data to hash.
 * @param[in]   dataLen     - Length of the input data in bytes.
 * @param[out]  pDigest     - Pointer to store the resulting MD5 hash.
 * @return      None.
 */
void tlkalg_md5_result(uint8_t *pData, uint16_t dataLen, tlkalg_md5_digest_t *pDigest);


#endif //TLKALG_MD5_H
