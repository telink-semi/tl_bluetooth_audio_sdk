/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_pairingDevice.h
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
 * @brief       Get the count of pairing devices.
 * @return      The count of pairing devices.
 */
uint32_t tlkmdi_tinySql_getPairingDevicesCount(void);

/**
 * @brief       Update pairing device information.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   devClass    - Device class pointer, can be NULL.
 * @param[in]   pLinkKey    - Link key pointer, can be NULL.
 * @param[in]   pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_updatePairingDevice(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName);

/**
 * @brief       Delete a pairing device by device address.
 * @param[in]   pDevAddr    - Device address pointer.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_deletePairingDevice(uint8_t *pDevAddr);

/**
 * @brief       Clean all pairing devices.
 * @return      none.
 */
void tlkmdi_tinySql_cleanPairingDevices(void);

/**
 * @brief       Get pairing device information by device address.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceByAddr(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName);

/**
 * @brief       Get pairing device information by index.
 * @param[in]   index       - Index of the device.
 * @param[out]  pDevAddr    - Device address pointer, can be NULL.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceByIndex(uint32_t index, uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName);

/**
 * @brief       Get the last paired device information.
 * @param[out]  pDevAddr    - Device address pointer, can be NULL.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getLastPairingDevice(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName);

/**
 * @brief       Get RFC channel ID of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[out]  val         - Value pointer to store the result.
 * @param[in]   type        - Type of RFC channel ID.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceRfcChid(uint8_t *pDevAddr, void *val, uint8_t type);

/**
 * @brief       Set RFC channel ID of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   val         - Value to set.
 * @param[in]   type        - Type of RFC channel ID.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_setPairingDeviceRfcChid(uint8_t *pDevAddr, uint16_t val, uint8_t type);

/**
 * @brief       Get volume setting of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   isMusic     - Music volume flag (true for music, false for voice).
 * @param[out]  val         - Volume value pointer, can be NULL.
 * @param[out]  isIos       - iOS device flag pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceVolume(uint8_t *pDevAddr, uint8_t isMusic, uint8_t *val, uint8_t *isIos);

/**
 * @brief       Set volume setting of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   isMusic     - Music volume flag (true for music, false for voice).
 * @param[in]   val         - Volume value to set.
 * @param[in]   isIos       - iOS device flag.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_setPairingDeviceVolume(uint8_t *pDevAddr, uint8_t isMusic, uint8_t val, uint8_t isIos);

/**
 * @brief       Reset dongle pairing information.
 * @return      none.
 */
void tlkmdi_tinySql_donglePairingReset(void);

/**
 * @brief       Reset BTTPT pairing information.
 * @return      none.
 */
void tlkmdi_tinySql_bttptPairingReset(void);

/**
 * @brief       Reset TPD headset pairing information.
 * @return      none.
 */
void tlkmdi_tinySql_tpdHeadsetPairingReset(void);

/**
 * @brief       Print all pairing devices for debugging.
 * @return      none.
 */
void tlkmdi_tinySql_printPairingDevices(void);