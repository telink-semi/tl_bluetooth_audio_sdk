/********************************************************************************************************
 * @file    tlkalg_ecc_hw.h
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
#ifndef TLKALG_ECC_HW_H
#define TLKALG_ECC_HW_H

#include "drivers.h"
#include "tlkalg/crypt/ecc/tlkalg_ecc_hw.h"
#include "tlkalg/crypt/ecc/tlkalg_ecc_curve.h"

typedef enum
{
    ECC_IDLE,
    ECC_BUSY,
    ECC_POINT_VERIFY_START,
    ECC_POINT_MULTI_START,
    ECC_DONE,
} ecc_state_t;

/* hECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1 if
'dest' was filled with random data, or 0 if the random data could not be generated.
The filled-in values should be either truly random, or from a cryptographically-secure PRNG.
A correctly functioning RNG function must be set (using hECC_set_rng()) before calling
hECC_make_key(). */

typedef unsigned char (*hECC_rng_func)(unsigned char *dest, unsigned int size);

typedef void (*ecc_make_key_pair_callback_t)(int result, unsigned char *public_key, unsigned char *private_key);

typedef void (*ecc_get_secret_key_callback_t)(int result, unsigned char *secret_key);

struct ecc_env_t
{
    unsigned int                  k[8];
    unsigned int                  Px[8];
    unsigned int                  Py[8];
    eccp_curve_t                 *curve;
    ecc_make_key_pair_callback_t  make_key_cb;
    ecc_get_secret_key_callback_t get_secret_key_cb;
    ecc_state_t                   state;
    bool                          debug_key_used;
    ecc_curve_t                   curve_sel;
};

extern struct ecc_env_t ecc_ctrl;


/**
 * @brief		The function that will be used to generate random bytes.
 * @param[in]	resister predefined TRNG function
 * @return		none
 */
void hwECC_set_rng(hECC_rng_func rng_func);


/**
 * @brief		get ECCP key pair(the key pair could be used in ECDH).
 * @param[out]	public_key	- public key, big--endian.
 * @param[out]	private_key	- private key, big--endian.
 * @param[in]	curve_sel	- ecc_curve select, e.g.: p-256r1.
 * @return		1(success), 0(error).
 */
unsigned char hwECC_make_key(unsigned char *public_key, unsigned char *private_key, ecc_curve_t curve_sel);

int hwECC_make_key_async(ecc_make_key_pair_callback_t make_key_cb, ecc_curve_t curve_sel);
#if (!MCU_CORE_TL752X_TEMP)
/**
 * @brief		ECDH compute key.
 * @param[in]	local_prikey	- local private key, big--endian.
 * @param[in]	public_key		- peer public key, big--endian.
 * @param[out]	dhkey			- output dhkey, big--endian.
 * @param[in]	curve_sel		- ecc_curve select, e.g.: p-256r1.
 * @Return		1(success); 0(error).
 */
unsigned char hwECC_shared_secret(const unsigned char *public_key, const unsigned char *private_key, unsigned char *secret, ecc_curve_t curve_sel);
#endif
int hwECC_shared_secret_async(const unsigned char *public_key, const unsigned char *private_key, ecc_get_secret_key_callback_t get_secret_key_cb, ecc_curve_t curve_sel);

//TODO: add API notes
int  hwECC_is_busy_async(void);
int  hwECC_get_curve_sel(void);
bool hwECC_is_debug_key_used(void);
void hwECC_enable_debug_key(bool use_dbg_key);

#endif /* TLKALG_ECC_HW_H */
