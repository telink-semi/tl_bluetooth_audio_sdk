/********************************************************************************************************
 * @file    tlkdrv_sy5500_ctrl.h
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

/*
note:this headfile is private,only can be included in sy5500 .c file.
*/

/**
 * @brief SY5500 control status enumeration definition
 */
typedef enum
{
    /** Unknown status (default) */
    TLKDRV_SY5500_CTRL_STATE_UNKNOWN = 0,

    /** True status */
    TLKDRV_SY5500_CTRL_STATE_TRUE = 1,

    /** False status */
    TLKDRV_SY5500_CTRL_STATE_FALSE = 2,
} TLKDRV_SY5500_CTRL_STATE_ENUM;

/**
 * @brief SY5500 control structure
 */
typedef struct
{
    /** Box open status */
    uint8_t isBoxOpen : 2;

    /** In charging box status */
    uint8_t isInbox : 2;

    /** 5V power connected status */
    uint8_t is5vOn : 2;

    /** Reserved field */
    uint8_t resv : 2;

    /** Reserved bytes */
    uint8_t resv3B[3];

    /** Power timer */
    TlkApiTimer_t powerTimer;

    /** In-box timer */
    TlkApiTimer_t inboxTimer;
} TlkDrvSy5500Ctrl_t;