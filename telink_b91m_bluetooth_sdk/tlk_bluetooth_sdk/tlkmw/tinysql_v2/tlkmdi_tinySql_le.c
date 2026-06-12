/********************************************************************************************************
 * @file    tlkmdi_tinySql_le.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmdi_tinySql.h"
#include "tlkmdi_tinySql_inner.h"

#include "stack/ble/host_v1/l2cap/smp/inc/ble_smp_store.h"
#include "stack/ble/host_v1/inc/ble_host_sal.h"

#if (TLK_MW_TINYSQL_V2_ENABLE && TLK_STK_BLE_ENABLE)

/******************************************************************************
                           private code begin
******************************************************************************/

/**
 * @brief       Restore LE disk to factory settings.
 * @return      none.
 */
static void tlkmdi_tinySql_leDiskRestore(void)
{
    //must first delete this so that can mark le disk is erase
    tlkmdi_tinySql_mutex_operate(true);
    //TODO: ZIYU
    // tlkmdi_tinysql_delAllKeyStartWith("le_");
    tlkmdi_tinySql_key_delete("le_version");
    tlkmdi_tinySql_key_delete("index");
    tlkmdi_tinySql_key_delete("queue");
    tlkmdi_tinySql_key_delete("01_i");
    tlkmdi_tinySql_key_delete("02_i");
    tlkmdi_tinySql_key_delete("03_i");
    tlkmdi_tinySql_key_delete("04_i");
    tlkmdi_tinySql_key_delete("05_i");
    tlkmdi_tinySql_key_delete("06_i");
    tlkmdi_tinySql_key_delete("07_i");
    tlkmdi_tinySql_key_delete("08_i");
    tlkmdi_tinySql_mutex_operate(false);
}

const tinySqlDisk_t tinySql_le_disk = {
    .init           = NULL,
    .restoreFactory = NULL,
};

static int ble_host_sal_nvs_init_core(const char *partition_name, uint32_t version)
{
    (void)partition_name;
    uint32_t saved_version;
    uint32_t len = tlkmdi_tinySql_key_get("le_version", &saved_version, sizeof(saved_version));
    if (len == 0 || len != sizeof(saved_version)) {
        tlkmdi_tinySql_leDiskRestore();
        tlkmdi_tinySql_key_set("le_version", &version, sizeof(version));
        return BLE_HOST_SAL_NVS_RESULT_OK;
    }
    if (saved_version != version) {
        return BLE_HOST_SAL_NVS_RESULT_VERSION_NOT_MATCH;
    }
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief       Initialize BLE host NVS.
 * @param[in]   partition_name - Partition name.
 * @param[in]   version - Version number.
 * @return      Result of initialization.
 */
int ble_host_sal_nvs_init(const char *partition_name, uint32_t version)
{
    tlkmdi_tinySql_mutex_operate(true);
    int ret = ble_host_sal_nvs_init_core(partition_name, version);
    tlkmdi_tinySql_mutex_operate(false);
    return ret;
}

/**
 * @brief       Erase BLE host NVS.
 * @param[in]   partition_name - Partition name.
 * @return      Result of erase operation.
 */
int ble_host_sal_nvs_erase(const char *partition_name)
{
    (void)partition_name;
    tlkmdi_tinySql_leDiskRestore();
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Open BLE host NVS.
 * @param[in]   partition_name - Partition name.
 * @param[out]  handle - Handle to the opened NVS.
 * @return      Result of open operation.
 */
int ble_host_sal_nvs_open(const char *partition_name, ble_host_sal_nvs_handle_t *handle)
{
    (void)partition_name;
    *handle = NULL;
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Close BLE host NVS.
 * @param[in]   handle - Handle to close.
 * @return      Result of close operation.
 */
int ble_host_sal_nvs_close(ble_host_sal_nvs_handle_t handle)
{
    (void)handle;
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Commit BLE host NVS changes.
 * @param[in]   handle - Handle to commit.
 * @return      Result of commit operation.
 */
int ble_host_sal_nvs_commit(ble_host_sal_nvs_handle_t handle)
{
    (void)handle;
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Erase key from BLE host NVS.
 * @param[in]   handle - Handle to the NVS.
 * @param[in]   key - Key to erase.
 * @return      Result of erase operation.
 */
int ble_host_sal_nvs_erase_key(ble_host_sal_nvs_handle_t handle, const char *key)
{
    (void)handle;
    tlkmdi_tinySql_key_delete(key);
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Write uint32 value to BLE host NVS.
 * @param[in]   handle - Handle to the NVS.
 * @param[in]   key - Key to write.
 * @param[in]   value - Value to write.
 * @return      Result of write operation.
 */
int ble_host_sal_nvs_write_uint32(ble_host_sal_nvs_handle_t handle, const char *key, uint32_t value)
{
    (void)handle;
    tlkmdi_tinySql_key_set(key, &value, sizeof(value));
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Read uint32 value from BLE host NVS.
 * @param[in]   handle - Handle to the NVS.
 * @param[in]   key - Key to read.
 * @param[out]  value - Value read.
 * @return      Result of read operation.
 */
int ble_host_sal_nvs_read_uint32(ble_host_sal_nvs_handle_t handle, const char *key, uint32_t *value)
{
    (void)handle;
    uint32_t buffer = 0;
    uint32_t len    = tlkmdi_tinySql_key_get(key, &buffer, sizeof(buffer));
    if (len == 0) {
        return BLE_HOST_SAL_NVS_RESULT_KEY_NOT_EXIST;
    }
    if (len == sizeof(buffer)) {
        *value = buffer;
    } else {
        //???
    }
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Write blob data to BLE host NVS.
 * @param[in]   handle - Handle to the NVS.
 * @param[in]   key - Key to write.
 * @param[in]   value - Data to write.
 * @param[in]   length - Length of data.
 * @return      Result of write operation.
 */
int ble_host_sal_nvs_write_blob(ble_host_sal_nvs_handle_t handle, const char *key, const void *value, uint16_t length)
{
    (void)handle;
    (void)length;

    tlkmdi_tinySql_key_set(key, value, length);
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

/**
 * @brief       Read blob data from BLE host NVS.
 * @param[in]   handle - Handle to the NVS.
 * @param[in]   key - Key to read.
 * @param[out]  value - Buffer to store read data.
 * @param[in]   length - Length of data.
 * @return      Result of read operation.
 */
int ble_host_sal_nvs_read_blob(ble_host_sal_nvs_handle_t handle, const char *key, void *value, uint16_t *length)
{
    (void)handle;
    uint32_t read_len = tlkmdi_tinySql_key_get(key, value, *length);
    if (read_len == 0) {
        return BLE_HOST_SAL_NVS_RESULT_KEY_NOT_EXIST;
    }
    *length = read_len;
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

#endif // (TLK_MW_TINYSQL_ENABLE && TLK_STK_BLE_ENABLE)
