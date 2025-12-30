/********************************************************************************************************
 * @file    ecc_p256.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/


#ifndef ECC_P256_H_
#define ECC_P256_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#if (SECURE_CONNECTIONS)

    #include <stdint.h>
    #include <stdbool.h>
    #include "stack/bt/ke/ke.h" // kernel task definitions
    #include "co_bt.h"
    /*
 * DEFINES
 ****************************************************************************************
 */

    #define ECC_PUBLICKEY_GENERATION 0x01
    #define ECC_DHKEY_GENERATION     0x02

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */


/// Multiplication result message structure
/*@TRACE*/
struct ecc_result_ind
{
    uint8_t key_res_x[32];
    uint8_t key_res_y[32];
}__attribute__((packed));

    /*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
    #if 0
/**
 ****************************************************************************************
 * Initialize Elliptic Curve algorithm
 ****************************************************************************************
 */
void ecc_init(bool reset);

/**
 ****************************************************************************************
 * @brief Generate a Secret Key compliant with ECC P256 algorithm
 *
 * If key is forced, just check its validity
 *
 * @param[out] secret_key Private key - MSB First
 * @param[in]  forced True if provided key is forced, else generate it.
 ****************************************************************************************
 */
void ecc_gen_new_secret_key(uint8_t* secret_key, bool forced);

/**
 ****************************************************************************************
 * @brief Generate a new Public key pair using ECC P256 algorithm
 *
 * @param[in] secret_key Private key - MSB First
 * @param[in] blocking   Force to run full algorithm without continue mode
 ****************************************************************************************
 */
void ecc_gen_new_public_key(uint8_t* secret_key256, ke_msg_id_t msg_id, ke_task_id_t task_id);

/**
 ****************************************************************************************
 * @brief Generate a new DHKey using ECC P256 algorithm
 *
 * @param[in] key_type   Type of key to generate (1: public key | 2: DH key)
 * @param[in] secret_key Private key                  - MSB First
 * @param[in] pub_key_x  Peer public key x coordinate - LSB First
 * @param[in] pub_key_y  Peer public key y coordinate - LSB First
 * @param[in] msg_id     Message task ID for the result indication
 * @param[in] task_id    Client task ID (Task type + instance)
 *
 * @return status   0 if key generation is started, > 0 otherwise
 ****************************************************************************************
 */
uint8_t ecc_generate_key256(uint8_t key_type, const uint8_t* secret_key, const uint8_t* public_key_x, const uint8_t* public_key_y, ke_msg_id_t msg_id, ke_task_id_t task_id);

/**
 ****************************************************************************************
 * @brief Retrieve debug private and public keys
 *
 * @param[out] secret_key Private key             - MSB First
 * @param[out] pub_key_x  Public key x coordinate - LSB First
 * @param[out] pub_key_y  Public key y coordinate - LSB First
 ****************************************************************************************
 */
void ecc_get_debug_Keys(uint8_t*secret_key, uint8_t* pub_key_x, uint8_t* pub_key_y);
    #endif
/**
 ****************************************************************************************
 * @brief Abort a current DHKey generation procedure
 *
 * @param[in] task_id    Client task ID (Task type + instance)
 ****************************************************************************************
 */
void ecc_abort_key256_generation(ke_task_id_t task_id);

#endif // (SECURE_CONNECTIONS)


#endif /* ECC_P256_H_ */
