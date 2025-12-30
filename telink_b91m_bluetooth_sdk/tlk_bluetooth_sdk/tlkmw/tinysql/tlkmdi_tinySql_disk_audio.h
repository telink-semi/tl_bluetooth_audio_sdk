/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_audio.h
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

#define TLKMDI_TINYSQL_EQ_DATA_MAX_LEN 100

/**
 * @brief Retrieves the pointer to the equalizer parameters.
 * 
 * @param[out] len If not NULL, the actual length of the equalizer data will be returned through this parameter.
 * @returns   Returns the pointer to the equalizer parameters; if the length of the equalizer data is 0, NULL is returned.
 */
const void* tlkmdi_tinySql_getEqParamPointer(uint16_t *len);


/**
 * @brief Retrieves the equalizer parameter data.
 * 
 * @param[out] recBuffer Buffer to store the retrieved equalizer parameter data.
 * @param[in] bufferLen  Size of the buffer.
 * @returns   Returns the length of the retrieved data; if an error occurs, the corresponding error code is returned.
 */
int tlkmdi_tinySql_getEqParam(void *recBuffer, uint16_t bufferLen);


/**
 * @brief Saves the equalizer parameter data.
 * 
 * @param[in] pEqParam Pointer to the equalizer parameter data to be saved.
 * @param[in] dataLen  Length of the equalizer parameter data to be saved.
 * @returns   Returns the length of the saved data; if an error occurs, the corresponding error code is returned.
 */
int tlkmdi_tinySql_saveEqParam(void * pEqParam, uint16_t dataLen);
