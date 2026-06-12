/********************************************************************************************************
 * @file    tlkos_storage.h
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

#include <stdint.h>

typedef void *tlkos_storage_kvdb_iter_t;

/**
 * @brief     Initializes the key-value database (KVDB) storage subsystem.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      This function must be called before any other KVDB operations.
 */
int tlkos_storage_kvdb_init(void);

/**
 * @brief     Stores a key-value pair into the KVDB.
 * @param[in] key       Null-terminated string identifying the key.
 * @param[in] value     Pointer to the data buffer to be stored.
 * @param[in] value_len Length of the data in bytes.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      If the key already exists, its value will be overwritten..
 */
int tlkos_storage_kvdb_set(const char *key, const void *value, uint32_t value_len);

/**
 * @brief     Retrieves the value associated with a given key from the KVDB.
 * @param[in]  key     Null-terminated string identifying the key to query.
 * @param[out] buf     Pointer to the buffer where the retrieved data will be copied.
 * @param[in]  buf_len Size of the output buffer in bytes.
 * @returns    The actual length of the value stored in the database.
 *             Returns 0 if the key does not exist.
 * @note      If buf_len is smaller than the actual value length, the data
 *            will be truncated and the function still returns the full length.
 */
uint32_t tlkos_storage_kvdb_get(const char *key, void *buf, uint32_t buf_len);

/**
 * @brief     Deletes a key-value pair from the KVDB.
 * @param[in] key Null-terminated string identifying the key to delete.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      If the key does not exist, the operation is considered successful
 *            and no error is returned.
 */
int tlkos_storage_kvdb_delete(const char *key);

/**
 * @brief     Initializes a KVDB iterator for traversing all key-value pairs.
 * @returns   A pointer to the allocated iterator handle on success, NULL on failure.
 * @note      The caller must call tlkos_storage_kvdb_iter_deinit() to free the
 *            iterator resource when it is no longer needed.
 */
tlkos_storage_kvdb_iter_t tlkos_storage_kvdb_iter_init(void);

/**
 * @brief     Deinitializes and frees a KVDB iterator previously created by
 *            tlkos_storage_kvdb_iter_init().
 * @param[in] iter  The iterator handle to free.
 * @note      Passing NULL is safe and will be ignored.
 */
void tlkos_storage_kvdb_iter_deinit(tlkos_storage_kvdb_iter_t iter);

/**
 * @brief     Advances the iterator to the next key-value pair in the KVDB.
 * @param[in] iter  The iterator handle.
 * @returns   0 indicates success (next entry available),
 *            -TLK_EPARAM if iter is NULL, -TLK_EFAIL if no more entries exist.
 * @note      This function must be called before accessing the first entry.
 */
int tlkos_storage_kvdb_iter_next(tlkos_storage_kvdb_iter_t iter);

/**
 * @brief     Returns the key name of the current key-value pair pointed to by
 *            the iterator.
 * @param[in] iter  The iterator handle.
 * @returns   A pointer to the null-terminated key string on success,
 *            NULL if iter is NULL or no valid current entry.
 */
const char *tlkos_storage_kvdb_iter_get_key(tlkos_storage_kvdb_iter_t iter);
