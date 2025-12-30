/********************************************************************************************************
 * @file    hash_kdf.h
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
#ifndef HASH_KDF_H
#define HASH_KDF_H

#include "hmac.h"

#ifdef __cplusplus
extern "C"
{
#endif


    //APIs

#ifdef SUPPORT_PBKDF2
    /**
 * @brief       dma md5 digest calculate
 * @param[out]  ctx_bak            - hmac ctx.
 * @param[in]   ctx                - hmac ctx to be backup.
 * @return      0:success     other:error
 */
    void pbkdf2_hmac_backup(HMAC_CTX *ctx_bak, HMAC_CTX *ctx);

    void pbkdf2_hmac_recover(HMAC_CTX *ctx, HMAC_CTX *ctx_bak);
    /**
 * @brief       pbkdf2 function(using hmac as PRF)
 * @param[in]   hash_alg         - specific hash algorithm.
 * @param[in]   pwd              - password, as the key of hmac.
 * @param[in]   sp_key_idx       - index of secure port key(password).
 * @param[in]   pwd_bytes        - byte length of password, it could be 0.
 * @param[in]   salt             - salt.
 * @param[in]   salt_bytes       - byte length of salt, it could be 0.
 * @param[in]   iter             - iteration times.
 * @param[out]  out              - derived key.
 * @param[in]   out_bytes        - byte length of derived key.
 * @return      0:success     other:error
 */
    unsigned int pbkdf2_hmac(HASH_ALG hash_alg, unsigned char *pwd, unsigned short sp_key_idx, unsigned int pwd_bytes, unsigned char *salt, unsigned int salt_bytes, unsigned int iter, unsigned char *out, unsigned int out_bytes);
#endif


#ifdef SUPPORT_ANSI_X9_63_KDF
    #ifdef SUPPORT_HASH_NODE
    /**
 * @brief       ansi_x9_63_kdf function
 *
 * This function performs the ANSI X9.63 key derivation function (KDF) on a set of messages and a counter.
 * If the input message `in` is NULL, it computes the KDF output. If `in` is not NULL, it computes the KDF
 * output and XORs it with the input message.
 *
 * @param[in]   hash_alg         - Specific hash algorithm to use.
 * @param[in]   hash_node        - Pointer to the HASH_NODE struct.
 * @param[in]   node_num         - Number of HASH_NODE elements, or number of message pieces.
 * @param[in]   counter_idx      - Index of the counter (4 bytes) in the hash_node array.
 * @param[in]   in               - Input message. If no input message, set this parameter as NULL.
 * @param[out]  out              - Output. If `in` is NULL, `out` is the KDF output; otherwise, `out` is the KDF output XORed with `in`.
 * @param[in]   out_bytes        - Byte length of the output. If `in` is not NULL, this is also the byte length of `in`.
 * @param[in]   check_whether_zero - 0 (do not check whether KDF output is zero or not), non-zero (check whether KDF output is zero or not).
 * @return      HASH_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. Ensure that the hash algorithm specified by hash_alg is valid.
 *             -# 2. hash_node[counter_idx] holds the 4-byte counter.
 *             -# 3. If `in` is NULL, `out` is set to the KDF output; otherwise, `out` is the KDF output XORed with `in`.
 *             -# 4. For ANSI X9.63, the initial counter is 1; for PKCS#1 RSA MGF, the initial counter is 0.
 * @endverbatim
 */
    unsigned int ansi_x9_63_kdf_node_with_xor_in(HASH_ALG hash_alg, HASH_NODE *node, unsigned int node_num, unsigned char *counter, unsigned char *in, unsigned char *out, unsigned int out_bytes, unsigned int check_whether_zero);

    /**
 * @brief       key = ansi_x9_63_kdf(msg1||msg2||...||counter||..., key_bytes).
 * @param[in]   hash_alg         - The specific hash algorithm to use.
 * @param[in]   hash_node        - Pointer to the HASH_NODE struct.
 * @param[in]   node_num         - The number of HASH_NODE elements, or the number of message pieces.
 * @param[in]   counter_idx      - The index of the 4-byte counter in the hash_node array.
 * @param[out]  key              - The output key.
 * @param[in]   key_bytes        - The byte length of the output key.
 * @return      HASH_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. Ensure that the hash algorithm specified by hash_alg is valid.
 *             -# 2. hash_node[counter_idx] holds the 4-byte counter.
 *             -# 3. key_bytes must be a multiple of the hash_alg digest byte length.
 * @endverbatim
 */
    unsigned int ansi_x9_63_kdf_internal(HASH_ALG hash_alg, HASH_NODE *hash_node, unsigned int node_num, unsigned char *counter, unsigned char *key, unsigned int key_bytes);

    /**
 * @brief       k1||k2 = ansi_x9_63_kdf(msg1||msg2||...||counter||... , k1_bytes + k2_bytes).
 * @param[in]   hash_alg         - The specific hash algorithm to use.
 * @param[in]   hash_node        - Pointer to the HASH_NODE struct.
 * @param[in]   node_num         - The number of HASH_NODE elements, or the number of message pieces.
 * @param[in]   counter_idx      - The index of the 4-byte counter in the hash_node array.
 * @param[out]  k1               - The output k1 part.
 * @param[in]   k1_bytes         - The byte length of k1.
 * @param[out]  k2               - The output k2 part.
 * @param[in]   k2_bytes         - The byte length of k2.
 * @return      HASH_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. Ensure that the hash algorithm specified by hash_alg is valid.
 *             -# 2. hash_node[counter_idx] holds the 4-byte counter.
 *             -# 3. k1 cannot be NULL, but k2 can be.
 * @endverbatim
 */
    unsigned int ansi_x9_63_kdf_node(HASH_ALG hash_alg, HASH_NODE *hash_node, unsigned int node_num, unsigned char *counter, unsigned char *k1, unsigned int k1_bytes, unsigned char *k2, unsigned int k2_bytes);

    /**
 * @brief       k1||k2 = ansi_x9_63_kdf(Z||counter||shared_info , k1_bytes + k2_bytes).
 * @param[in]   hash_alg         - The specific hash algorithm to use.
 * @param[in]   Z                - The byte string Z.
 * @param[in]   Z_bytes          - The byte length of Z.
 * @param[in]   shared_info      - The shared information.
 * @param[in]   shared_info_bytes - The byte length of shared information.
 * @param[out]  k1               - The output k1 part.
 * @param[in]   k1_bytes         - The byte length of k1.
 * @param[out]  k2               - The output k2 part.
 * @param[in]   k2_bytes         - The byte length of k2.
 * @return      HASH_SUCCESS (success), other (error)
 *
 * @verbatim
 *             -# 1. Ensure that the hash algorithm specified by hash_alg is valid.
 *             -# 2. k1 cannot be NULL, but k2 can be.
 * @endverbatim
 */
    unsigned int ansi_x9_63_kdf(HASH_ALG hash_alg, unsigned char *Z, unsigned int Z_bytes, unsigned char *shared_info, unsigned int shared_info_bytes, unsigned char *k1, unsigned int k1_bytes, unsigned char *k2, unsigned int k2_bytes);
    #endif
#endif


#ifdef __cplusplus
}
#endif


#endif
