/********************************************************************************************************
 * @file    soft_sha384_sha512.h
 *
 * @brief   This is the header file for tl751x
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
#ifndef SHA384_SHA512_H
#define SHA384_SHA512_H

#if 0
    #include "type.h"
#else
//typedef unsigned int   unsigned int;
//typedef unsigned short unsigned char;
//typedef unsigned char  unsigned char;
typedef signed char S8;
#endif


/* type to hold the SHA512 context */
typedef struct
{
    unsigned int count[4];
    unsigned int hash[16]; //*hash;//
    unsigned int wbuf[32];
} SHA512_Ctx;

/* type to hold the SHA384 context */
typedef SHA512_Ctx SHA384_Ctx;

/* type to hold the SHA512_224 context */
typedef SHA512_Ctx SHA512_224_Ctx;

/* type to hold the SHA512_256 context */
typedef SHA512_Ctx SHA512_256_Ctx;

/* type to hold the HMAC_SHA512 context */
typedef struct
{
    unsigned int K0[32];
    SHA512_Ctx   sha512_ctx[1];
} HMAC_SHA512_Ctx;

/* type to hold the HMAC_SHA384 context */
typedef HMAC_SHA512_Ctx HMAC_SHA384_Ctx;

/* type to hold the HMAC_SHA512_224 context */
typedef HMAC_SHA512_Ctx HMAC_SHA512_224_Ctx;

/* type to hold the HMAC_SHA512_256 context */
typedef HMAC_SHA512_Ctx HMAC_SHA512_256_Ctx;


//internal APIs
/**
 * @brief       Convert byte order of an array of 32-bit integers.
 * @param[in]   a          - Pointer to the array of unsigned integers.
 * @param[in]   bytelen    - Byte length of the array to process.
 * @note
 * @verbatim
 *      -# Ensure `a` is not NULL and `bytelen` is a multiple of 4.
 * @endverbatim
 */
void SHA512_convert(unsigned int *a, unsigned char bytelen);
/**
 * @brief       Copy 16 words (32-bit integers) from source to destination.
 * @param[in]   dst   - Pointer to the destination array.
 * @param[in]   src   - Pointer to the source array.
 * @note
 * @verbatim
 *      -# Ensure `dst` and `src` are not NULL and point to valid memory locations.
 *      -# Function assumes the source and destination arrays are at least 16 words in size.
 * @endverbatim
 */
void SHA512_Copy_16_Words(unsigned int *dst, unsigned int *src);
#if 0
void SHA512_XOR(unsigned int a[2], unsigned int b[2], unsigned int c[2]);
void SHA512_ROTR(unsigned int x[2], unsigned int y[2], unsigned char n);
void SHA512_SHR(unsigned int x[2], unsigned int y[2], unsigned char n);
void SHA512_mod_add(unsigned int a[2], unsigned int b[2], unsigned int c[2]);
#endif
/**
 * @brief       Add a byte length to a 128-bit integer represented as an array of four 32-bit integers.
 * @param[in,out]   a        - Array representing the 128-bit integer (four 32-bit words).
 * @param[in]       byteLen  - Byte length to add to the integer.
 * @note
 * @verbatim
 *      -# Ensure `a` is not NULL and points to an array of at least 4 unsigned integers.
 *      -# This function assumes the array `a` is ordered from most significant to least significant integer.
 *      -# Overflow handling is performed with carry propagation.
 * @endverbatim
 */
void SHA512_byteLen_add(unsigned int a[4], unsigned int byteLen);

/**
 * @brief       Get the current byte length of the block being processed in the SHA-512 context.
 * @param[in]   ctx  - Pointer to the SHA-512 context structure.
 * @return      The byte length of the current block, or 0 if the context is NULL.
 * @note
 * @verbatim
 *      -# Ensure `ctx` is not NULL before calling this function.
 *      -# This function extracts and returns the least significant 7 bits of `ctx->count[3]`.
 * @endverbatim
 */
unsigned char SHA512_block_byteLen(SHA512_Ctx *ctx);

/**
 * @brief       Process a block of data within the SHA-512 context.
 * @param[in,out]   ctx     - Pointer to the SHA-512 context structure.
 * @param[in]       byteLen - The byte length of the data block to be processed.
 * @note
 * @verbatim
 *      -# Ensure `ctx` is not NULL before calling this function.
 *      -# The function processes a single 1024-bit block of data.
 *      -# It updates the internal hash state of the context based on the input block.
 *      -# The function performs the main loop of 80 iterations, which involves message
 *         schedule computation and compression function application.
 * @endverbatim
 */
void SHA512_block(SHA512_Ctx *ctx, unsigned char byteLen);


//APIs for user.
/**
 * @brief       Initialize the SHA-512 context with optional initialization vector and length.
 * @param[out]  ctx            - Pointer to the SHA-512 context structure.
 * @param[in]   iv             - Pointer to the initialization vector (optional).
 * @param[in]   byte_length_h  - High 32 bits of the message length.
 * @param[in]   byte_length_l  - Low 32 bits of the message length.
 * @verbatim
 *      -# init with inputting iv, and updated message length(must be a
 *         multiple of SHA512 block length, i.e. 128bytes).
 * @endverbatim
 */
void SHA512_Init_with_iv_and_updated_length(SHA512_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief       Initialize the SHA-512 context.
 * @param[out]  ctx - Pointer to the SHA-512 context structure.
 */
void SHA512_Init(SHA512_Ctx *ctx);

/**
 * @brief       Process a message block within the SHA-512 context.
 * @param[in,out]   ctx     - Pointer to the SHA-512 context structure.
 * @param[in]       message - Pointer to the input message.
 * @param[in]       byteLen - Length of the message in bytes.
 */
void SHA512_Process(SHA512_Ctx *ctx, unsigned char *message, unsigned int byteLen);

/**
 * @brief       Finalize the SHA-512 digest and get the result.
 * @param[in,out]   ctx    - Pointer to the SHA-512 context structure.
 * @param[out]      digest - Pointer to the output digest (64 bytes).
 */

void SHA512_Done(SHA512_Ctx *ctx, unsigned char digest[64]);

/**
 * @brief       Compute the SHA-512 hash for a given message (one-off style).
 * @param[in]   message - Pointer to the input message.
 * @param[in]   byteLen - Length of the message in bytes.
 * @param[out]  digest  - Pointer to the output digest (64 bytes).
 */
void SHA512_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[64]);

/**
 * @brief       Initialize the SHA-384 context with optional initialization vector and length.
 * @param[out]  ctx            - Pointer to the SHA-384 context structure.
 * @param[in]   iv             - Pointer to the initialization vector (optional).
 * @param[in]   byte_length_h  - High 32 bits of the message length.
 * @param[in]   byte_length_l  - Low 32 bits of the message length.
 * @verbatim
 *      -# init with inputting iv, and updated message length(must be a
 *         multiple of SHA384 block length, i.e. 128bytes).
 * @endverbatim
 */
void SHA384_Init_with_iv_and_updated_length(SHA384_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief       Initialize the SHA-384 context.
 * @param[out]  ctx - Pointer to the SHA-384 context structure.
 */
void SHA384_Init(SHA384_Ctx *ctx);
/**
 * @brief       Process a message block within the SHA-384 context.
 * @param[in,out]   ctx     - Pointer to the SHA-384 context structure.
 * @param[in]       message - Pointer to the input message.
 * @param[in]       byteLen - Length of the message in bytes.
 */
void SHA384_Process(SHA384_Ctx *ctx, unsigned char *message, unsigned int byteLen);

/**
 * @brief       Finalize the SHA-384 digest and get the result.
 * @param[in,out]   ctx    - Pointer to the SHA-384 context structure.
 * @param[out]      digest - Pointer to the output digest (64 bytes).
 */
void SHA384_Done(SHA384_Ctx *ctx, unsigned char digest[48]);

/**
 * @brief       Compute the SHA-384 hash for a given message (one-off style).
 * @param[in]   message - Pointer to the input message.
 * @param[in]   byteLen - Length of the message in bytes.
 * @param[out]  digest  - Pointer to the output digest (64 bytes).
 */
void SHA384_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[48]);

/**
 * @brief       Initialize the SHA-512_224 context with optional initialization vector and length.
 * @param[out]  ctx            - Pointer to the SHA-512_224 context structure.
 * @param[in]   iv             - Pointer to the initialization vector (optional).
 * @param[in]   byte_length_h  - High 32 bits of the message length.
 * @param[in]   byte_length_l  - Low 32 bits of the message length.
 * @verbatim
 *      -# init with inputting iv, and updated message length(must be a
 *         multiple of SHA512_224 block length, i.e. 128bytes).
 * @endverbatim
 */
void SHA512_224_Init_with_iv_and_updated_length(SHA512_224_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);


/**
 * @brief       Initialize the SHA-512_224 context.
 * @param[out]  ctx - Pointer to the SHA-512_224 context structure.
 */
void SHA512_224_Init(SHA512_224_Ctx *ctx);

/**
 * @brief       Process a message block within the SHA-512_224 context.
 * @param[in,out]   ctx     - Pointer to the SHA-512_224 context structure.
 * @param[in]       message - Pointer to the input message.
 * @param[in]       byteLen - Length of the message in bytes.
 */
void SHA512_224_Process(SHA512_224_Ctx *ctx, unsigned char *message, unsigned int byteLen);

/**
 * @brief       Finalize the SHA-512_224 digest and get the result.
 * @param[in,out]   ctx    - Pointer to the SHA-512_224 context structure.
 * @param[out]      digest - Pointer to the output digest (64 bytes).
 */
void SHA512_224_Done(SHA512_224_Ctx *ctx, unsigned char digest[28]);

/**
 * @brief       Compute the SHA-512_224 hash for a given message (one-off style).
 * @param[in]   message - Pointer to the input message.
 * @param[in]   byteLen - Length of the message in bytes.
 * @param[out]  digest  - Pointer to the output digest (64 bytes).
 */
void SHA512_224_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[28]);

/**
 * @brief       Initialize the SHA-512_256 context with optional initialization vector and length.
 * @param[out]  ctx            - Pointer to the SHA-512_256 context structure.
 * @param[in]   iv             - Pointer to the initialization vector (optional).
 * @param[in]   byte_length_h  - High 32 bits of the message length.
 * @param[in]   byte_length_l  - Low 32 bits of the message length.
 * @verbatim
 *      -# init with inputting iv, and updated message length(must be a
 *         multiple of SHA512_256 block length, i.e. 128bytes).
 * @endverbatim
 */
void SHA512_256_Init_with_iv_and_updated_length(SHA512_256_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief       Initialize the SHA-512_256 context.
 * @param[out]  ctx - Pointer to the SHA-512_256 context structure.
 */
void SHA512_256_Init(SHA512_256_Ctx *ctx);

/**
 * @brief       Process a message block within the SHA-512_256 context.
 * @param[in,out]   ctx     - Pointer to the SHA-512_256 context structure.
 * @param[in]       message - Pointer to the input message.
 * @param[in]       byteLen - Length of the message in bytes.
 */
void SHA512_256_Process(SHA512_256_Ctx *ctx, unsigned char *message, unsigned int byteLen);

/**
 * @brief       Finalize the SHA-512_256 digest and get the result.
 * @param[in,out]   ctx    - Pointer to the SHA-512_256 context structure.
 * @param[out]      digest - Pointer to the output digest (64 bytes).
 */
void SHA512_256_Done(SHA512_256_Ctx *ctx, unsigned char digest[32]);

/**
 * @brief       Compute the SHA-512_256 hash for a given message (one-off style).
 * @param[in]   message - Pointer to the input message.
 * @param[in]   byteLen - Length of the message in bytes.
 * @param[out]  digest  - Pointer to the output digest (64 bytes).
 */
void SHA512_256_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[32]);

/**
 * @brief       Initialize HMAC with SHA-384, given a key.
 * @param[out]  ctx        - Pointer to the HMAC SHA-384 context structure.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 */
void HMAC_SHA384_Init(HMAC_SHA384_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);

/**
 * @brief       Process input data for HMAC with SHA-384.
 * @param[in,out]   ctx     - Pointer to the HMAC SHA-384 context structure.
 * @param[in]       input   - Pointer to the input data.
 * @param[in]       byteLen - Length of the input data in bytes.
 */
void HMAC_SHA384_Process(HMAC_SHA384_Ctx *ctx, unsigned char *input, unsigned int byteLen);

/**
 * @brief       Finalize HMAC with SHA-384 and produce the MAC.
 * @param[in,out]   ctx - Pointer to the HMAC SHA-384 context structure.
 * @param[out]      mac - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA384_Done(HMAC_SHA384_Ctx *ctx, unsigned char mac[48]);

/**
 * @brief       Compute the HMAC using SHA-384 for a given message and key.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 * @param[in]   msg        - Pointer to the input message.
 * @param[in]   msgByteLen - Length of the message in bytes.
 * @param[out]  mac        - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA384(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[48]);

/**
 * @brief       Initialize HMAC with SHA-512-224, given a key.
 * @param[out]  ctx        - Pointer to the HMAC SHA-512-224 context structure.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 */
void HMAC_SHA512_224_Init(HMAC_SHA512_224_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);

/**
 * @brief       Process input data for HMAC with SHA-512-224.
 * @param[in,out]   ctx     - Pointer to the HMAC SHA-512-224 context structure.
 * @param[in]       input   - Pointer to the input data.
 * @param[in]       byteLen - Length of the input data in bytes.
 */
void HMAC_SHA512_224_Process(HMAC_SHA512_224_Ctx *ctx, unsigned char *input, unsigned int byteLen);

/**
 * @brief       Finalize HMAC with SHA-512-224 and produce the MAC.
 * @param[in,out]   ctx - Pointer to the HMAC SHA-512-224 context structure.
 * @param[out]      mac - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA512_224_Done(HMAC_SHA512_224_Ctx *ctx, unsigned char mac[28]);

/**
 * @brief       Compute the HMAC using SHA-512-224 for a given message and key.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 * @param[in]   msg        - Pointer to the input message.
 * @param[in]   msgByteLen - Length of the message in bytes.
 * @param[out]  mac        - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA512_224(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[28]);

/**
 * @brief       Initialize HMAC with SHA-512-256, given a key.
 * @param[out]  ctx        - Pointer to the HMAC SHA-512-256 context structure.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 */
void HMAC_SHA512_256_Init(HMAC_SHA512_256_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);

/**
 * @brief       Process input data for HMAC with SHA-512-256.
 * @param[in,out]   ctx     - Pointer to the HMAC SHA-512-256 context structure.
 * @param[in]       input   - Pointer to the input data.
 * @param[in]       byteLen - Length of the input data in bytes.
 */
void HMAC_SHA512_256_Process(HMAC_SHA512_256_Ctx *ctx, unsigned char *input, unsigned int byteLen);

/**
 * @brief       Process input data for HMAC with SHA-512-256.
 * @param[in,out]   ctx     - Pointer to the HMAC SHA-512-256 context structure.
 * @param[in]       input   - Pointer to the input data.
 * @param[in]       byteLen - Length of the input data in bytes.
 */
void HMAC_SHA512_256_Done(HMAC_SHA512_256_Ctx *ctx, unsigned char mac[32]);

/**
 * @brief       Compute the HMAC using SHA-512-256 for a given message and key.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 * @param[in]   msg        - Pointer to the input message.
 * @param[in]   msgByteLen - Length of the message in bytes.
 * @param[out]  mac        - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA512_256(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[32]);

/**
 * @brief       Initialize HMAC with SHA-512, given a key.
 * @param[out]  ctx        - Pointer to the HMAC SHA-512 context structure.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 */
void HMAC_SHA512_Init(HMAC_SHA512_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);

/**
 * @brief       Process input data for HMAC with SHA-512.
 * @param[in,out]   ctx     - Pointer to the HMAC SHA-512 context structure.
 * @param[in]       input   - Pointer to the input data.
 * @param[in]       byteLen - Length of the input data in bytes.
 */
void HMAC_SHA512_Process(HMAC_SHA512_Ctx *ctx, unsigned char *input, unsigned int byteLen);

/**
 * @brief       Finalize HMAC with SHA-512 and produce the MAC.
 * @param[in,out]   ctx - Pointer to the HMAC SHA-512 context structure.
 * @param[out]      mac - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA512_Done(HMAC_SHA512_Ctx *ctx, unsigned char mac[64]);

/**
 * @brief       Compute the HMAC using SHA-512 for a given message and key.
 * @param[in]   key        - Pointer to the secret key.
 * @param[in]   keyByteLen - Length of the key in bytes.
 * @param[in]   msg        - Pointer to the input message.
 * @param[in]   msgByteLen - Length of the message in bytes.
 * @param[out]  mac        - Pointer to the output MAC (48 bytes).
 */
void HMAC_SHA512(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[64]);


#endif
