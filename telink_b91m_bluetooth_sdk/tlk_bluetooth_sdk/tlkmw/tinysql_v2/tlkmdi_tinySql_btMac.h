/********************************************************************************************************
 * @file    tlkmdi_tinySql_btMac.h
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
 * @brief   Retrieves the MAC address of the Classic Bluetooth device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address of the Classic Bluetooth device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_getBtMacAddress(uint8_t *recBuffer);

/**
 * @brief   Sets the MAC address of the Classic Bluetooth device.
 * @param[in] inBuffer The buffer containing the new MAC address for the Classic Bluetooth device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_setBtMacAddress(uint8_t *inBuffer);

/**
 * @brief   Retrieves the MAC address of the Low Energy Bluetooth (BLE) device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address of the BLE device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_getLeMacAddress(uint8_t *recBuffer);

/**
 * @brief   Sets the MAC address of the Low Energy Bluetooth (BLE) device.
 * @param[in] inBuffer The buffer containing the new MAC address for the BLE device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_setLeMacAddress(uint8_t *inBuffer);

/**
 * @brief   Retrieves the MAC address of the tpsll slave device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note    Tps shares mac with bt.
 */
int tlkmdi_tinySql_getTpsAddr(uint8_t *recBuffer);

/**
 * @brief   Retrieves the MAC address of the tpsll dongle device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note    Tpd shares mac with bt.
 */
int tlkmdi_tinySql_getTpdAddr(uint8_t *recBuffer);
