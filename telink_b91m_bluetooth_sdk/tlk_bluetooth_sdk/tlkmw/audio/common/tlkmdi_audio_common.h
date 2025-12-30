/********************************************************************************************************
 * @file    tlkmdi_audio_common.h
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
    TLKMDI_AUDIO_CB_MAIN = 0,
    TLKMDI_AUDIO_CB_TIMER,
    TLKMDI_AUDIO_CB_FIFO,
    TLKMDI_AUDIO_CB_HIGH_RIORITY_TASK,
    TLKMDI_AUDIO_CB_NUM,
} TLKMDI_AUDIO_CB_TYPE_ENUM;

/**
 * @brief Audio callback function pointer type
 */
typedef void (*TlkMdiAudioCB_t)(void);

/**
 * @brief       Get event task handler
 * @param[in]   evtID - Event ID
 * @return      Event task handler
 */
TlkOsEventDealCB tlkmdi_audio_getEvtTask(TLKSYS_TASK_EVT_ENUM evtID);

/**
 * @brief       Register audio callback
 * @param[in]   cbType - Callback type
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TYPE_ENUM cbType, TlkMdiAudioCB_t cb);

/**
 * @brief       Initialize RTOS for audio module
 * @return      None
 */
void tlkmdi_audio_use_rtos(void);

/**
 * @brief       Enter critical section for audio operations
 * @return      None
 */
void tlkmdi_audio_enterCritical(void);

/**
 * @brief       Leave critical section for audio operations
 * @return      None
 */
void tlkmdi_audio_leaveCritical(void);

/**
 * @brief       Run main task after specified time
 * @param[in]   ms - Delay time in milliseconds
 * @return      None
 */
void tlkmdi_audio_runMainAfterTimeMs(uint16_t ms);

/**
 * @brief       Wake up high priority task
 * @return      None
 */
void tlkmdi_audio_wakeUpHighPriorityTask(void);

/**
 * @brief       Handle audio timer interrupt
 * @return      None
 */
void tlkmdi_audio_timer_irq_handler(void);

/**
 * @brief       Handle audio FIFO interrupt
 * @return      None
 */
void tlkmdi_audio_fifo_irq_handler(void);