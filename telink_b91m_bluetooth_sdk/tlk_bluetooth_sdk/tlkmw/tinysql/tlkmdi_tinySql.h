/********************************************************************************************************
 * @file    tlkmdi_tinySql.h
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

#include "tlkmw/tinysql/tlkmdi_tinySql_config.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_userSetting.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_pairingDevice.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_btMac.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_pbap.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_le.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_audio.h"

/**
 * @brief   Operates the mutex for TinySQL module.
 * @param[in] isLocked - true to lock the mutex, false to unlock.
 * @note    This function is used to ensure thread safety when accessing TinySQL data.
 */
void tlkmdi_tinySql_mutex_operate(uint8_t isLocked);

/**
 * @brief   Initializes the TinySQL module.
 * @note    This function initializes all registered disk modules.
 */
void tlkmdi_tinySql_init(void);

/**
 * @brief   Saves all pending data to flash.
 * @return  TLK_ENONE if successful, error code otherwise.
 * @note    This function saves data from all disk modules that have pending changes.
 */
int tlkmdi_tinySql_save(void);

/**
 * @brief   Restores all disk modules to factory settings.
 * @note    This function resets all data to default values.
 */
void tlkmdi_tinySql_restoreFactorySettings(void);

/**
 * @brief   Checks if there is any pending save request.
 * @return  true if there is a pending save request, false otherwise.
 */
bool tlkmdi_tinySql_isRequestSave(void);

/**
 * @brief   Enables or disables the saving functionality.
 * @param[in] en - 1 to enable saving, 0 to disable.
 * @note    When disabled, save requests will be ignored.
 */
void tlkmdi_tinySql_setSaveEnable(uint8_t en);

/**
 * @brief   Suspends saving functionality.
 * @note    This function increments the critical section counter to prevent saving.
 */
void tlkmdi_tinySql_suspendSave(void);

/**
 * @brief   Resumes saving functionality.
 * @note    This function decrements the critical section counter and may trigger a save if needed.
 */
void tlkmdi_tinySql_resumeSave(void);