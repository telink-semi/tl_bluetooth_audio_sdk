/********************************************************************************************************
 * @file    tlkmdi_tinySql_core.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmdi_tinySql_inner.h"
#include "tlklib/os/component/storage/tlkos_storage.h"

#if (TLK_MW_TINYSQL_V2_ENABLE)

/******************************************************************************
                           private code begin
******************************************************************************/

#undef tlk_this
#define tlk_this ((tlkmdi_tinySql_core_t *)(&sTlkmdiTinysqlCore))

#undef __tlk_node_t
#define __tlk_node_t tlkmdi_tinySql_save_item_t *

#undef __tlk_node_find
#define __tlk_node_find tlkmdi_tinySql_cache_node_find

#undef __tlk_node_insert
#define __tlk_node_insert tlkmdi_tinySql_cache_node_insert

#undef __tlk_node_remove
#define __tlk_node_remove tlkmdi_tinySql_cache_node_remove

#undef __tlk_node_update
#define __tlk_node_update tlkmdi_tinySql_cache_node_update

typedef enum
{
    TLKMDI_TINYSQL_STATE_NO_CHANGE = 0,
    TLKMDI_TINYSQL_STATE_NEED_SAVE,
    TLKMDI_TINYSQL_STATE_NEED_DEL,
} tlkmdi_tinySql_item_state_e;

typedef struct _tlkmdi_tinySql_save_item_t
{
    uint8_t                             state;
    uint8_t                             name_len; //with '0',example:"telink" = 7
    uint16_t                            val_len;
    struct _tlkmdi_tinySql_save_item_t *next;
    char                                payload[0];
} tlkmdi_tinySql_save_item_t;

typedef struct
{
    uint32_t                    saveEn;
    tlkmdi_tinySql_save_item_t *list;
} tlkmdi_tinySql_core_t;

static tlkmdi_tinySql_core_t sTlkmdiTinysqlCore = {0};

static __tlk_node_t __tlk_node_find(const char *key)
{
    //find from list
    __tlk_node_t node = tlk_this->list;
    while (node) {
        if (strcmp((char *)(node->payload), key) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

static __tlk_node_t __tlk_node_insert(const char *key, const void *value, uint16_t val_len, uint8_t state, uint32_t ttl_ms)
{
    (void)ttl_ms;
    size_t       key_len  = strlen(key) + 1;
    __tlk_node_t new_node = (__tlk_node_t)tlkos_malloc(sizeof(tlkmdi_tinySql_save_item_t) + key_len + val_len);
    if (new_node == NULL) {
        return NULL;
    }
    memcpy(new_node->payload, key, key_len);
    if (value != NULL && val_len > 0) {
        memcpy(new_node->payload + key_len, value, val_len);
    }
    new_node->name_len = key_len;
    new_node->val_len  = val_len;
    new_node->state    = state;
    new_node->next     = tlk_this->list;
    tlk_this->list     = new_node;
    return new_node;
}

static __tlk_node_t __tlk_node_remove(__tlk_node_t node)
{
    if (node == NULL) {
        return NULL;
    }
    if (tlk_this->list == node) {
        tlk_this->list = node->next;
    } else {
        __tlk_node_t prev = tlk_this->list;
        while (prev != NULL && prev->next != node) {
            prev = prev->next;
        }
        if (prev != NULL) {
            prev->next = node->next;
        }
    }
    tlkos_free(node);
    return NULL;
}

static __tlk_node_t __tlk_node_update(const char *key, const void *value, uint16_t val_len, uint8_t state, uint32_t ttl_ms)
{
    (void)ttl_ms;
    __tlk_node_t node = __tlk_node_find(key);
    if (node != NULL) {
        if (node->val_len != val_len) {
            __tlk_node_remove(node);
            return __tlk_node_insert(key, value, val_len, state, ttl_ms);
        }
        if (value != NULL && val_len > 0) {
            memcpy(node->payload + node->name_len, value, val_len);
        }
        node->state = state;
        return node;
    }
    return __tlk_node_insert(key, value, val_len, state, ttl_ms);
}

//api
/**
 * @brief   Operates the mutex for TinySQL module.
 * @param[in] isLocked - true to lock the mutex, false to unlock.
 * @note    This function is used to ensure thread safety when accessing TinySQL data.
 */
void tlkmdi_tinySql_mutex_operate(uint8_t isLocked)
{
    if (isLocked) {
        tlksys_mutex_lock(TLKSYS_MUTEX_TINYSQL);
    } else {
        tlksys_mutex_unlock(TLKSYS_MUTEX_TINYSQL);
    }
}

/**
 * @brief   Calculates the actual save address in flash.
 * @param[in] offset - The offset from the base address.
 * @return  The actual flash address for saving data.
 * @note    This function calculates the address based on the flash size.
 */
inline unsigned int tlkmdi_tinySql_getSaveAddr(unsigned int offset)
{
    return offset + tlkhal_flash_get_size() - 0x100000;
}

uint32_t tlkmdi_tinySql_key_get_unthreadsafe(const char *key, void *buf, uint32_t buf_len)
{
    __tlk_node_t node = __tlk_node_find(key);
    if (node == NULL) {
        return tlkos_storage_kvdb_get(key, buf, buf_len);
    }
    if (node->state == TLKMDI_TINYSQL_STATE_NEED_DEL || node->val_len == 0) {
        return 0;
    }
    memcpy(buf, node->payload + node->name_len, min(buf_len, node->val_len));
    return node->val_len;
}

void tlkmdi_tinySql_key_set_ex_unthreadsafe(const char *key, const void *value, uint32_t value_len, uint32_t cache_ttl_ms)
{
    (void)cache_ttl_ms;
    if (tlk_this->saveEn) {
        __tlk_node_t node = __tlk_node_find(key);
        if (node != NULL) {
            __tlk_node_remove(node);
        }
        tlkos_storage_kvdb_set(key, value, value_len);
    } else {
        __tlk_node_update(key, value, value_len, TLKMDI_TINYSQL_STATE_NEED_SAVE, cache_ttl_ms);
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
    }
}

//ttl time to live in cache.reserve for future.
void tlkmdi_tinySql_key_set_ex(const char *key, const void *value, uint32_t value_len, uint32_t cache_ttl_ms)
{
    tlkmdi_tinySql_mutex_operate(true);
    tlkmdi_tinySql_key_set_ex_unthreadsafe(key, value, value_len, cache_ttl_ms);
    tlkmdi_tinySql_mutex_operate(false);
}

void tlkmdi_tinySql_key_set(const char *key, const void *value, uint32_t value_len)
{
    tlkmdi_tinySql_key_set_ex(key, value, value_len, 0);
}

uint32_t tlkmdi_tinySql_key_get(const char *key, void *buf, uint32_t buf_len)
{
    tlkmdi_tinySql_mutex_operate(true);
    uint32_t ret = tlkmdi_tinySql_key_get_unthreadsafe(key, buf, buf_len);
    tlkmdi_tinySql_mutex_operate(false);
    return ret;
}

void tlkmdi_tinySql_key_delete(const char *key)
{
    tlkmdi_tinySql_mutex_operate(true);
    if (tlk_this->saveEn) {
        __tlk_node_t node = __tlk_node_find(key);
        if (node != NULL) {
            __tlk_node_remove(node);
        }
        tlkos_storage_kvdb_delete(key);
    } else {
        __tlk_node_update(key, NULL, 0, TLKMDI_TINYSQL_STATE_NEED_DEL, 0);
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
    }
    tlkmdi_tinySql_mutex_operate(false);
}

void tlkmdi_tinySql_cache_sync(void)
{
    //sync cache to flash
    if (tlk_this->saveEn == 0) {
        return;
    }
    tlkmdi_tinySql_mutex_operate(true);
    while (1) {
        __tlk_node_t node = tlk_this->list;
        if (node == NULL) {
            break;
        }
        if (node->state == TLKMDI_TINYSQL_STATE_NEED_SAVE) {
            tlkos_storage_kvdb_set(node->payload, node->payload + node->name_len, node->val_len);
        } else if (node->state == TLKMDI_TINYSQL_STATE_NEED_DEL) {
            tlkos_storage_kvdb_delete(node->payload);
        }
        __tlk_node_remove(node);
    }

    tlkmdi_tinySql_mutex_operate(false);
}

void tlkmdi_tinySql_core_save_en(uint8_t en)
{
    en = en ? 1 : 0;
    tlkmdi_tinySql_mutex_operate(true);
    if (en == tlk_this->saveEn) {
        tlkmdi_tinySql_mutex_operate(false);
        return;
    }
    tlk_this->saveEn = en;
    if (en) {
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE);
    }
    tlkmdi_tinySql_mutex_operate(false);
}

void tlkmdi_tinySql_core_init(void)
{
    tlk_this->saveEn = 1;
    tlkos_storage_kvdb_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE, tlkmdi_tinySql_cache_sync);
}
#endif