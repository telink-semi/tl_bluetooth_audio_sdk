/********************************************************************************************************
 * @file    tlkmdi_audio_tpsif_core.h
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
#pragma once

typedef enum
{
    AUD_TPSIF_TYPE_TPH,
    AUD_TPSIF_TYPE_TPT,
    AUD_TPSIF_TYPE_UNKNOWN,
    AUD_TPSIF_TYPE_NUM,
} AUD_TPSIF_TYPE_ENUM;

/**
 * @brief Callback function for TPS interface state change.
 * @param[in] mode - The mode of the state change.
 */
typedef void (*TlkMdiAudTpsifStateChgCB)(uint16_t mode);

/**
 * @brief Get the TPS interface type.
 * @return The TPS interface type enumeration value.
 */
AUD_TPSIF_TYPE_ENUM tlkmdi_audio_tpsif_getType(void);

/**
 * @brief Add a callback function for state change notification.
 * @param[in] cb - Callback function to be added.
 * @return 0 - Success.
 * @return TLK_EPARAM - Invalid parameter.
 * @return TLK_EQUOTA - No space for new callback.
 */
int tlkmdi_audio_tpsif_addStateChgCB(TlkMdiAudTpsifStateChgCB cb);

/**
 * @brief Process host event data.
 * @param[in] pData - Pointer to the data buffer.
 * @param[in] dataLen - Length of the data.
 */
void tlkmdi_audio_tpsif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen);