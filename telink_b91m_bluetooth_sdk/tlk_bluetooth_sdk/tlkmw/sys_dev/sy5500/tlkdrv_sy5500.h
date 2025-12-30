/********************************************************************************************************
 * @file    tlkdrv_sy5500.h
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

/**
 * @brief SY5500 event enumeration definition
 */
typedef enum
{
    /** Box open event */
    TLKDRV_SY5500_EVT_BOX_OPEN = 0,

    /** Box close event */
    TLKDRV_SY5500_EVT_BOX_CLOSE,

    /** Box pairing event */
    TLKDRV_SY5500_EVT_BOX_PAIR,

    /** Box factory reset event */
    TLKDRV_SY5500_EVT_BOX_FACTORY_RESET,

    /** Get MAC address event */
    TLKDRV_SY5500_EVT_BOX_GET_MAC,

    /** Leave box event */
    TLKDRV_SY5500_EVT_LEAVE_BOX,

    /** Enter box event */
    TLKDRV_SY5500_EVT_ENTER_BOX,

    /** Need power off event */
    TLKDRV_SY5500_EVT_NEED_POWER_OFF,
} TLKDRV_SY5500_EVT_ENUM;

/**
 * @brief SY5500 event callback function pointer type
 * @param[in] evtID Event ID
 * @param[in] pData Event data pointer
 * @param[in] dataLen Event data length
 * @param[in] isNeedAck Whether acknowledgment is needed
 */
typedef void (*TlkDrvSy5500EvtCB)(uint16_t evtID, uint8_t *pData, uint16_t dataLen, bool isNeedAck);

/**
 * @brief Initialize SY5500 driver
 * @param[in] cb Event callback function
 */
void tlkdrv_sy5500_init(TlkDrvSy5500EvtCB cb);

/**
 * @brief Send state information to charging box
 * @param[in] state State value
 */
void tlkdrv_sy5500_sendStateToBox(uint8_t state);

/**
 * @brief Send MAC address to charging box
 * @param[in] mac MAC address pointer
 */
void tlkdrv_sy5500_sendMacToBox(uint8_t *mac);