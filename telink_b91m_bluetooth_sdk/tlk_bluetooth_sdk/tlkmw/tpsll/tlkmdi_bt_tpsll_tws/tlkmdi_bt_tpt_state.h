/********************************************************************************************************
 * @file    tlkmdi_bt_tpt_state.h
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
    TLKMDI_BT_TPT_STATE_UNKNOWN = 0, //default
    TLKMDI_BT_TPT_STATE_TRUE    = 1,
    TLKMDI_BT_TPT_STATE_FALSE   = 2,
} TLKMDI_BT_TPT_STATE_ENUM;

typedef struct
{
    uint8_t isLeavebox : 2;
    uint8_t resv       : 6;
} TlkMdiTptCaseState_t;

bool tlkmdi_bt_tpt_isMaster();

bool tlkmdi_bt_tpt_isSlave();

bool tlkmdi_bt_tpt_isSingle();

bool tlkmdi_bt_tpt_isConnected();

bool tlkmdi_bt_tpt_isLeft();

bool tlkmdi_bt_tpt_isInBox();

bool tlkmdi_bt_tpt_isInEar();

TlkMdiTptCaseState_t tlkmdi_bt_tpt_getLocalCaseState();

TlkMdiTptCaseState_t tlkmdi_bt_tpt_getPeerCaseState();

void tlkmdi_bt_tpt_setPeerCaseState(TlkMdiTptCaseState_t state);

void tlkmdi_bt_tpt_setLocalCaseState(TlkMdiTptCaseState_t state);