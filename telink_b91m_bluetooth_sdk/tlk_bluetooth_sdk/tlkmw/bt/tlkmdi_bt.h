/********************************************************************************************************
 * @file    tlkmdi_bt.h
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
#ifndef TLKMDI_BT_H
#define TLKMDI_BT_H

/**
 * @brief       Get the Bluetooth device name.
 * @param[out]  recBuffer - Pointer to buffer where the device name will be stored.
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_getName(uint8_t *recBuffer);

/**
 * @brief       Get the Bluetooth device address.
 * @param[out]  recBuffer - Pointer to buffer where the device address will be stored (6 bytes).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_getAddr(uint8_t *recBuffer);

/**
 * @brief       Set the Bluetooth device name.
 * @param[in]   pName - Pointer to the new device name string.
 * @param[in]   nameLen - Length of the name string (excluding null terminator).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_setName(uint8_t *pName, uint8_t nameLen);

/**
 * @brief       Set the Bluetooth device address.
 * @param[in]   pAddr - Pointer to the new device address (6 bytes).
 * @return      Returns TLK_ENONE (0) on success, negative error code on failure.
 */
int tlkmdi_bt_mgr_setAddr(uint8_t *pAddr);

/**
 * @brief       Initialize the Bluetooth module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_init(void);

/**
 * @brief       Initialize the Bluetooth manager control module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_mgr_ctrlInit(void);

#endif // TLKMDI_BT_H
