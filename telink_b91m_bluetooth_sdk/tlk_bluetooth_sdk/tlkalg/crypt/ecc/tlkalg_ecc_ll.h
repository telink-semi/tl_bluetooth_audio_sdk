/********************************************************************************************************
 * @file    tlkalg_ecc_ll.h
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
#ifndef TLKALG_ECC_LL_H
#define TLKALG_ECC_LL_H

#include "tlkalg/crypt/ecc/tlkalg_ecc_curve.h"

extern const u8 tlkalg_ecc_dbg_priv_key192[24];
extern const u8 tlkalg_ecc_dbg_pub_key192[48];

extern const u8 tlkalg_ecc_dbg_priv_key256[32];
extern const u8 tlkalg_ecc_dbg_pub_key256[64];

typedef void (*ecc_make_key_pair_callback_t)(int result, unsigned char *public_key, unsigned char *private_key);
typedef void (*ecc_get_secret_key_callback_t)(int result, unsigned char *secret_key);

/**
* @brief		This function is used to register the random number function needed for ECC calculation
* @param		none
* @return		none
*/
void tlkalg_ecc_init(void);

/**
* @brief		This function is used to generate an ECDH public-private key pairs
* @param[out]  pub[64]:  output ecdh public key
* @param[out]  priv[64]: output ecdh private key
* @param[in]   use_dbg_key: 0: Non-debug key , others: debug key
* @return		1:  success
*              0: failure
*/
int tlkalg_ecc_gen_key_pair(unsigned char *pub, unsigned char *priv, ecc_curve_t curve, bool use_dbg_key);

int tlkalg_ecc_gen_key_pair_async(ecc_make_key_pair_callback_t make_key_cb, ecc_curve_t curve, bool use_dbg_key);

/**
* @brief		This function is used to calculate DHKEY based on the peer public key and own private key
* @param[in]   peer_pub_key[64]: peer public key
* @param[in]   own_priv_key[32]: own private key
* @param[out]  out_dhkey[32]: dhkey key
* @return		1:  success
*              0: failure
*/
int tlkalg_ecc_gen_dhkey(const unsigned char *peer_pub, const unsigned char *own_priv, unsigned char *out_dhkey, ecc_curve_t curve);

int tlkalg_ecc_gen_dhkey_async(const unsigned char *public_key, const unsigned char *private_key, ecc_get_secret_key_callback_t get_secret_key_cb, ecc_curve_t curve);

int tlkalg_ecc_is_busy_async(void);

#endif /* TLKALG_ECC_LL_H */
