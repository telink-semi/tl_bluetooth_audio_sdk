/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_le.c
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
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"

#include "stack/ble/host_v1/l2cap/smp/inc/ble_smp_store.h"
#include "stack/ble/host_v1/inc/ble_host_sal.h"

#if (TLK_MW_TINYSQL_ENABLE && TLK_STK_BLE_ENABLE)

#define TLKMDI_TINYSQL_LE_SUPPORT_MAX_KEY_NUM (8)

/**
 * @brief Structure for LE disk format.
 */
typedef struct
{
    uint32_t                      used_flag;                                    //!< Used flag
    uint32_t                      version;                                      //!< Version
    uint32_t                      index;                                        //!< Index
    uint8_t                       queue[TLKMDI_TINYSQL_LE_SUPPORT_MAX_KEY_NUM]; //!< Queue
    struct ble_host_smp_store_key key[TLKMDI_TINYSQL_LE_SUPPORT_MAX_KEY_NUM];   //!< Keys
} sql_le_disk_format_t;

#define TEST_SIZE sizeof(sql_le_disk_format_t)

/******************************************************************************
                           private code begin
******************************************************************************/
static sql_le_disk_format_t sSqlLeDiskData = {0};
static tlkapi_save_ctrl_t   sSqlDiskLeCtrl = {0};

/**
 * @brief       Operate mutex for LE disk.
 * @param[in]   isLock - Lock or unlock flag.
 * @return      none.
 */
static void tlkmdi_tinySql_leDisk_mutexOperate(uint8_t isLock)
{
    tlkmdi_tinySql_mutex_operate(isLock);
}

/**
 * @brief       Reset LE disk data.
 * @return      none.
 */
static void tlkmdi_tinySql_leDiskReset(void)
{
    memset(&sSqlLeDiskData, 0, sizeof(sql_le_disk_format_t));
    sSqlLeDiskData.index = 1;
}

/**
 * @brief       Initialize LE disk.
 * @return      none.
 */
static void tlkmdi_tinySql_leDiskInit(void)
{
    unsigned int saveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK4_ADDR);
    tlkapi_save3_init(&sSqlDiskLeCtrl, TLKMDI_TINYSQL_SAVE_SIGN,
                      TLKMDI_TINYSQL_VER + 3, // always version + 1, different TLK_STK_BLE_VOLUME_STORE
                      sizeof(sSqlLeDiskData), saveAddress + 4096 * 0,
                      saveAddress + 4096 * 1); //2*4K
    int ret = tlkapi_save3_load(&sSqlDiskLeCtrl, (uint8_t *)&sSqlLeDiskData, sizeof(sSqlLeDiskData));
    if (ret < (int)sizeof(sSqlLeDiskData)) {
        tlkmdi_tinySql_leDiskReset();
    }
}

/**
 * @brief       Save LE disk data.
 * @return      none.
 */
static void tlkmdi_tinySql_leDiskSave(void)
{
    tlkapi_save3_smartSave(&sSqlDiskLeCtrl, (uint8_t *)&sSqlLeDiskData, sizeof(sSqlLeDiskData));
}

/**
 * @brief       Restore LE disk to factory settings.
 * @return      none.
 */
static void tlkmdi_tinySql_leDiskRestore(void)
{
    tlkapi_save3_clean(&sSqlDiskLeCtrl);
    tlkmdi_tinySql_leDiskReset();
    tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
}

const tinySqlDisk_t tinySql_le_disk = {
    .init           = tlkmdi_tinySql_leDiskInit,
    .restoreFactory = tlkmdi_tinySql_leDiskRestore,
    .save           = tlkmdi_tinySql_leDiskSave,
};

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
    (void)partition_name;
    if (sSqlLeDiskData.used_flag == 1) {
        if (sSqlLeDiskData.version != version) {
            return BLE_HOST_SAL_NVS_RESULT_VERSION_NOT_MATCH;
        }
    } else {
        tlkmdi_tinySql_leDisk_mutexOperate(true);
        sSqlLeDiskData.used_flag = 1;
        sSqlLeDiskData.version   = version;
        tlkmdi_tinySql_leDisk_mutexOperate(false);
        tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
    }

    return BLE_HOST_SAL_NVS_RESULT_OK;
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
 * @brief       Get key by index.
 * @param[in]   key - Key string.
 * @return      Pointer to the key structure.
 */
static struct ble_host_smp_store_key *tlkmdi_tinySql_get_key_by_index(const char *key)
{
    uint8_t index = 0;
    if (memcmp(key, "01_i", 4) == 0) {
        index = 0;
    } else if (memcmp(key, "02_i", 4) == 0) {
        index = 1;
    } else if (memcmp(key, "03_i", 4) == 0) {
        index = 2;
    } else if (memcmp(key, "04_i", 4) == 0) {
        index = 3;
    } else if (memcmp(key, "05_i", 4) == 0) {
        index = 4;
    } else if (memcmp(key, "06_i", 4) == 0) {
        index = 5;
    } else if (memcmp(key, "07_i", 4) == 0) {
        index = 6;
    } else if (memcmp(key, "08_i", 4) == 0) {
        index = 7;
    } else {
        return NULL;
    }
    return &sSqlLeDiskData.key[index];
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
    struct ble_host_smp_store_key *p_store_key = tlkmdi_tinySql_get_key_by_index(key);
    if (p_store_key != NULL) {
        tlkmdi_tinySql_leDisk_mutexOperate(true);
        memset(p_store_key, 0, sizeof(struct ble_host_smp_store_key));
        tlkmdi_tinySql_leDisk_mutexOperate(false);
        tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
    }
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
    if (memcmp(key, "index", 5) == 0) {
        tlkmdi_tinySql_leDisk_mutexOperate(true);
        sSqlLeDiskData.index = value;
        tlkmdi_tinySql_leDisk_mutexOperate(false);
        tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
    }
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
    if (memcmp(key, "index", 5) == 0) {
        if (value != NULL) {
            *value = sSqlLeDiskData.index;
        }
    } else {
        return BLE_HOST_SAL_NVS_RESULT_KEY_NOT_EXIST;
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

    if (memcmp(key, "queue", 5) == 0) {
        tlkmdi_tinySql_leDisk_mutexOperate(true);
        memcpy(sSqlLeDiskData.queue, value, length);
        tlkmdi_tinySql_leDisk_mutexOperate(false);
        tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
    } else {
        struct ble_host_smp_store_key *p_store_key = tlkmdi_tinySql_get_key_by_index(key);
        if (p_store_key != NULL) {
            tlkmdi_tinySql_leDisk_mutexOperate(true);
            memcpy(p_store_key, value, sizeof(struct ble_host_smp_store_key));
            tlkmdi_tinySql_leDisk_mutexOperate(false);
            tlkmdi_tinySql_requestSave(tinySql_leSaveIndex);
        }
    }
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
    if (memcmp(key, "queue", 5) == 0) {
        memcpy(value, sSqlLeDiskData.queue, *length);
    } else {
        struct ble_host_smp_store_key *p_store_key = tlkmdi_tinySql_get_key_by_index(key);
        if (p_store_key != NULL) {
            if (p_store_key->pairing_index == 0) {
                return BLE_HOST_SAL_NVS_RESULT_KEY_NOT_EXIST;
            }
            if (value != NULL) {
                memcpy(value, p_store_key, sizeof(struct ble_host_smp_store_key));
            }
            if (length != NULL) {
                *length = sizeof(struct ble_host_smp_store_key);
            }
        }
    }
    return BLE_HOST_SAL_NVS_RESULT_OK;
}

#endif // (TLK_MW_TINYSQL_ENABLE && TLK_STK_BLE_ENABLE)