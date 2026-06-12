/********************************************************************************************************
 * @file    tlkalg_ec.h
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
#ifndef TLKALG_EC_H
#define TLKALG_EC_H

/**
 * @brief   Initializes the echo cancellation and noise suppression modules.
 * @param[in]   pNs     - Pointer to the noise suppression control structure.
 * @param[in]   pAec    - Pointer to the echo cancellation control structure.
 * @param[in]   pScratch- Pointer to the scratch buffer for processing.
 */
void tlkalg_ec_init(uint8_t *pNs, uint8_t *pAec, uint8_t *pScratch);

/**
 * @brief   Processes a frame for echo cancellation and noise suppression.
 * @param[in]   pMicData- Pointer to the microphone input data.
 * @param[in]   pSpkData- Pointer to the speaker input data.
 * @return      Pointer to the processed output data.
 */
short *tlkalg_ec_frame(uint8_t *pMicData, uint8_t *pSpkData);


#endif //TLKALG_EC_H
