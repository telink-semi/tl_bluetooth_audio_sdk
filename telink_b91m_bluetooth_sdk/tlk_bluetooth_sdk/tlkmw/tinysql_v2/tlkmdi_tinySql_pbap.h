/********************************************************************************************************
 * @file    tlkmdi_tinySql_pbap.h
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
 * @brief       Get the phone book state.
 * @return      true if phone book is in OK state, false otherwise.
 */
bool tlkmdi_tinySql_getPhoneBookState(void);

/**
 * @brief       Create a new phone book for the specified Bluetooth MAC address.
 * @param[in]   btMac - Bluetooth MAC address for the phone book.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_newPhoneBook(uint8_t *btMac);

/**
 * @brief       Add a PBAP item block to the phone book.
 * @param[in]   isLastOne - Indicates if this is the last block to be added.
 * @param[in]   itemsNum - Number of items in this block.
 * @param[in]   data - Pointer to the data to be added.
 * @param[in]   dataLen - Length of the data in bytes.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_addPbapItemBlock(bool isLastOne, uint16_t itemsNum, void *data, uint16_t dataLen);

/**
 * @brief       Get the MAC address associated with the phone book.
 * @param[out]  recbuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getPhoneBookMac(uint8_t *recbuffer);

/**
 * @brief       Get a pointer to the MAC address associated with the phone book.
 * @return      Pointer to the MAC address, or NULL if phone book is not in OK state.
 */
const uint8_t *tlkmdi_tinySql_getPhoneBookMacPointer(void);

/**
 * @brief       Get the total number of items in the phone book.
 * @return      Total number of items in the phone book.
 */
uint16_t tlkmdi_tinySql_getPhoneBookItemNum(void);

/**
 * @brief       Search the phone book using a custom search function.
 * @param[in]   searchFunc - Function pointer to the search criteria.
 * @param[in]   oneItemLen - Length of each item in the phone book.
 * @return      Pointer to the found item, or NULL if not found.
 */
const void *tlkmdi_tinySql_searchPhoneBook(tlkMdiTinySqlSearchFunc searchFunc, uint16_t oneItemLen);