/********************************************************************************************************
 * @file    tlkos_event.h
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
 * @brief     Gets the required static buffer length for creating an event tab.
 * @param[in] evtTabLen Length of the event tab.
 * @returns   Required buffer size in bytes.
 */
uint32_t tlkos_event_getStaticBufferLen(uint32_t evtTabLen);

/**
 * @brief     Creates an event tab.
 * @param[in] evtTabLen Length of the event tab.
 * @param[out] evtTabHandle Pointer to store the created event tab handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_createTab(uint32_t evtTabLen, TlkOsEventTabHandle_t *evtTabHandle);


/**
 * @brief     Creates an event tab with a static buffer.
 * @param[in] evtTabLen Length of the event tab.
 * @param[in] pStaticBuffer Pointer to the static buffer provided by the user.
 * @param[in] staticBufferSize Size of the static buffer in bytes.
 * @param[out] evtTabHandle Pointer to store the created event tab handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_createTabStatic(uint32_t evtTabLen, uint8_t *pStaticBuffer, uint32_t staticBufferSize, TlkOsEventTabHandle_t *evtTabHandle);

/**
 * @brief     Destroys an event tab.
 * @param[in] evtTabHandle Handle of the event tab to destroy.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_destroyTab(TlkOsEventTabHandle_t evtTabHandle);

/**
 * @brief     Registers an event deal callback.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @param[in] cb Callback function to register.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_regDealCB(TlkOsEventTabHandle_t evtTabHandle, uint32_t index, TlkOsEventDealCB cb);


/**
 * @brief     Sets an event bit.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_set(TlkOsEventTabHandle_t evtTabHandle, uint32_t index);


/**
 * @brief     Sets an event bit from interrupt context.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_setFromIsr(TlkOsEventTabHandle_t evtTabHandle, uint32_t index);

/**
 * @brief     Waits for events.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_wait(TlkOsEventTabHandle_t evtTabHandle, uint32_t blockTimeMs);

/**
 * @brief     Gets event bits.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[out] evt Pointer to store the event bits.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_get(TlkOsEventTabHandle_t evtTabHandle, uint32_t *evt);