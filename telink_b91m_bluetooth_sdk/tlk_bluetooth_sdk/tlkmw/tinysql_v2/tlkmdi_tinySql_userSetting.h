/********************************************************************************************************
 * @file    tlkmdi_tinySql_userSetting.h
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
 * @brief       Get the current working mode of the system.
 * @return      Current work mode as defined by TLK_WORK_MODE_ENUM.
 */
uint8_t tlkmdi_tinySql_getWorkMode(void);

/**
 * @brief       Set the working mode of the system.
 * @param[in]   mode - The new work mode to be set as per TLK_WORK_MODE_ENUM.
 * @return      none.
 */
void tlkmdi_tinySql_setWorkMode(uint8_t mode);

/**
 * @brief       Get the USB ID.
 * @return      Current USB ID.
 */
uint16_t tlkmdi_tinySql_getUsbID(void);

/**
 * @brief       Get the Bluetooth device name.
 * @param[out]  recBuffer - Buffer to store the retrieved device name.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getBtName(uint8_t *recBuffer);

/**
 * @brief       Set the Bluetooth device name.
 * @param[in]   inBuffer - Buffer containing the new device name to be set.
 * @param[in]   datalen - Length of the data to be copied.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_setBtName(uint8_t *inBuffer, uint32_t datalen);

/**
 * @brief       Get TPH MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTphMacAddr(uint8_t *pBuffer);

/**
 * @brief       Set TPH MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTphMacAddr(uint8_t *macAddr);

/**
 * @brief       Get TPD MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTpdMacAddr(uint8_t *pBuffer);

/**
 * @brief       Set TPD MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTpdMacAddr(uint8_t *macAddr);

/**
 * @brief       Get peer address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getPeerAddr(uint8_t *pBuffer);

/**
 * @brief       Set peer address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setPeerAddr(uint8_t *addr);

/**
 * @brief       Get newly allocated address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getNewAllocAddr(uint8_t *pBuffer);

/**
 * @brief       Set newly allocated address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setNewAllocAddr(uint8_t *addr);

/**
 * @brief       Get generated AC address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getGenAcAddr(uint8_t *pBuffer);

/**
 * @brief       Set generated AC address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setGenAcAddr(uint8_t *addr);

/**
 * @brief       Get key configuration information.
 * @param[out]  key_config_info - Pointer to store the key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_getKeyCofnig(keyConfigs_t **key_config_info);

/**
 * @brief       Update key configuration information.
 * @param[in]   key_config_info - Pointer to the new key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_updateKeyCofnig(keyConfigs_t *key_config_info);