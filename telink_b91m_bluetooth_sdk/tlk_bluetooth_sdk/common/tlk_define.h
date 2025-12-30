/********************************************************************************************************
 * @file    tlk_define.h
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
#ifndef TLK_DEFINE_H
#define TLK_DEFINE_H


#define TLK_INVALID_HANDLE 0xFFFF

typedef enum
{
    TLK_STATE_CLOSED = 0,
    TLK_STATE_OPENED,
    TLK_STATE_PAUSED,
    TLK_STATE_OPENING,
    TLK_STATE_CLOSING,
    TLK_STATE_CONNING,
    TLK_STATE_CONNECT,
    TLK_STATE_DISCING,
    TLK_STATE_IDLE,
    TLK_STATE_CRASH,
} TLK_STATE_ENUM;

typedef enum
{
    TLK_WORK_MODE_NORMAL   = 0,
    TLK_WORK_MODE_TEST_PTS = 1,
    TLK_WORK_MODE_TEST_FAT = 2,
    TLK_WORK_MODE_TEST_RDT = 3,
    TLK_WORK_MODE_TEST_EMI = 4,
    TLK_WORK_MODE_TEST_USR = 5,
    TLK_WORK_MODE_TEST_BQB = 6,
    TLK_WORK_MODE_TEST_ATS = 7,
} TLK_WORK_MODE_ENUM;


#endif // TLK_DEFINE_H
