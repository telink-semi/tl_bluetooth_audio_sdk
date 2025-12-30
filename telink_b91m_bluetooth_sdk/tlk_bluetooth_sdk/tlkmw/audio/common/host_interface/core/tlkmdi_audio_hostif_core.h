/********************************************************************************************************
 * @file    tlkmdi_audio_hostif_core.h
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
#ifndef TLKMDI_AUDIO_HOST_INTERFACE_CORE_H
#define TLKMDI_AUDIO_HOST_INTERFACE_CORE_H

/**
 * @brief       Process host events for audio interface
 * @param[in]   pData - Pointer to event data
 * @param[in]   dataLen - Length of event data
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_hostif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Check if currently in product test mode
 * @param[in]   None
 * @return      True if in product test mode, false otherwise
 */
uint8_t tlkmdi_audio_hostif_isInProductTest(void);

/**
 * @brief       Set product test mode enable flag
 * @param[in]   en - Enable flag (0: disable, non-zero: enable)
 * @return      None
 */
void tlkmdi_audio_hostif_setProductTestEn(uint8_t en);

#endif // TLKMDI_AUDIO_HOST_INTERFACE_CORE_H
