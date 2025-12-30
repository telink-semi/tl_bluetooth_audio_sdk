/********************************************************************************************************
 * @file    tlkalg_crc32.h
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
#ifndef TLKALG_CRC32_H
#define TLKALG_CRC32_H

/**
 * @brief       Initializes the CRC32 value.
 * @param[out]  pCrc    - Pointer to the CRC32 value to be initialized.
 * @return      none.
 */
void tlkalg_crc32_init(uint32_t *pCrc);

/**
 * @brief       Updates the CRC32 value with new data.
 * @param[in]   pCrc    - Pointer to the current CRC32 value.
 * @param[in]   pData   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data buffer.
 * @return      none.
 */
void tlkalg_crc32_update(uint32_t *pCrc, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Finalizes the CRC32 value.
 * @param[in]   pCrc    - Pointer to the CRC32 value to be finalized.
 * @return      none.
 */
void tlkalg_crc32_finish(uint32_t *pCrc);

/**
 * @brief       Computes the CRC32 result for the given data.
 * @param[out]  pCrc    - Pointer to the CRC32 result.
 * @param[in]   pData   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data buffer.
 * @return      none.
 */
void tlkalg_crc32_result(uint32_t *pCrc, uint8_t *pData, uint16_t dataLen);


#endif //TLKALG_CRC32_H
