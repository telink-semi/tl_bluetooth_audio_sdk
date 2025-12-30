/********************************************************************************************************
 * @file    tlkmdi_btpairing.h
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
    TLKMDI_BTPAIRING_CB_STATE_ACTIVE = 0,
    TLKMDI_BTPAIRING_CB_STATE_TIMEOUT,
    TLKMDI_BTPAIRING_CB_STATE_CANCEL,
} TLKMDI_BTPAIRING_CB_STATE_ENUM;

typedef void (*TlkMdiBtPairingCB)(uint8_t state);//state refer to TLKMDI_BTPAIRING_CB_STATE_ENUM

/**
 * @brief  Register callback function for bt pairing.
 *
 * @param[in] cb :Callback function
 *
 * @returns  none.
 */
void tlkmdi_btPairing_regCB(TlkMdiBtPairingCB cb);

/**
 * @brief   Check if the bt pairing mode is in progress.
 *
 * @param[in]  none.
 *
 * @returns   The function returns a bool value.
 *            - true: Bluetooth pairing mode is in progress.
 *            - false: Bluetooth pairing mode is not in progress.
 */
bool tlkmdi_btPairing_isInProgress(void);

/**
 * @brief  start bt pairing.
 *
 * @param[in] none.
 *
 * @return The process result,TLK_ENONE means success, negative value mean fail.
 * 
 * @note Not thread safe,must call in bt host's thread.
 */
int tlkmdi_btParing_start(void);

/**
 * @brief  stop bt pairing.
 *
 * @param[in] none.
 *
 * @return The process result,TLK_ENONE means success, negative value mean fail.
 * 
 * @note Not thread safe,must call in bt host's thread.
 */
int tlkmdi_btParing_stop(void);