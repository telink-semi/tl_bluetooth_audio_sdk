/********************************************************************************************************
 * @file    tlkmdi_audio_btif_core.h
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
#ifndef TLKMDI_AUDIO_BT_INTERFACE_CORE_H
#define TLKMDI_AUDIO_BT_INTERFACE_CORE_H

/**
 * @brief       Process host events related to Bluetooth audio interface
 * @param[in]   pData - Pointer to event data
 * @param[in]   dataLen - Length of event data
 * @return      None
 */
void tlkmdi_audio_btif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen);

#endif // TLKMDI_AUDIO_BT_INTERFACE_CORE_H
