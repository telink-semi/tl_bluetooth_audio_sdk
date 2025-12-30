/********************************************************************************************************
 * @file    aes_sw.h
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
#ifndef _AES_SW_H_
#define _AES_SW_H_

/**
 * @brief       This function sets the AES encryption key.
 * @param[in]   key     - pointer to the AES key.
 * @return      none.
 */
void aes_sw_SetKey(unsigned char *key /*, int bothEncDec*/);

/**
 * @brief       This function encrypts a single block (16 bytes) of data using AES.
 * @param[in]   d       - pointer to the data block to be encrypted.
 * @return      none.
 */
void aes_sw_Encrypt(unsigned char *d);

/**
 * @brief       This function decrypts a single block (16 bytes) of data using AES.
 * @param[in]   d       - pointer to the data block to be encrypted.
 * @return      none.
 */
void aes_sw_Decrypt(unsigned char *d);

#endif
