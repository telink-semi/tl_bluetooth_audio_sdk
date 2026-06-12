/********************************************************************************************************
 * @file    tlkmw_host_linkmgr.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmw_host_linkmgr.h"
#include "tlkmw/bt/tlkmdi_btacl.h"

typedef enum
{
    TLKMDI_HOST_LINK_STATE_NONE,
    TLKMDI_HOST_LINK_STATE_DESTROY = TLKMDI_HOST_LINK_STATE_NONE,
    TLKMDI_HOST_LINK_STATE_CONNECT_BUSY,
    TLKMDI_HOST_LINK_STATE_CONNECT_IDLE,
} tlkmw_host_link_state_e;

typedef struct
{
    uint8_t  type;
    uint8_t  state;
    uint16_t handle;
} host_link_item_t;

typedef struct
{
    uint8_t           sysBusy;
    uint8_t           resv;
    uint8_t           itemNum;
    uint8_t           capacity;
    host_link_item_t *itemArray;
} host_linkmgr_t;

static host_linkmgr_t sTlkmwHostLinkMgr = {0};

/**
 * @brief       This function gets a used link item.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      Returns the pointer to the used link item.
 */
static inline host_link_item_t *tlkmw_host_linkmgr_getUsedItem(uint8_t type, uint16_t handle)
{
    host_link_item_t *array = sTlkmwHostLinkMgr.itemArray;
    for (size_t i = 0; i < sTlkmwHostLinkMgr.itemNum; i++) {
        if (array[i].handle == handle && array[i].type == type) {
            return &array[i];
        }
    }
    return NULL;
}

/**
 * @brief       This function allocates a new link item.
 * @param[in]   none.
 * @return      Returns the pointer to the allocated link item.
 */
static inline host_link_item_t *tlkmw_host_linkmgr_mallocItem(void)
{
    if (sTlkmwHostLinkMgr.capacity > sTlkmwHostLinkMgr.itemNum) {
        sTlkmwHostLinkMgr.itemNum++;
        return &sTlkmwHostLinkMgr.itemArray[sTlkmwHostLinkMgr.itemNum - 1];
    }
    if (sTlkmwHostLinkMgr.capacity == 0) {
        sTlkmwHostLinkMgr.itemArray = tlkos_calloc(sizeof(host_link_item_t) * 2);
        sTlkmwHostLinkMgr.capacity  = 2;
    } else {
        host_link_item_t *newArray = tlkos_calloc(sTlkmwHostLinkMgr.capacity * 2 * sizeof(host_link_item_t));
        memcpy(newArray, sTlkmwHostLinkMgr.itemArray, sizeof(host_link_item_t) * sTlkmwHostLinkMgr.capacity);
        tlkos_free(sTlkmwHostLinkMgr.itemArray);
        sTlkmwHostLinkMgr.capacity *= 2;
        sTlkmwHostLinkMgr.itemArray = newArray;
    }
    sTlkmwHostLinkMgr.itemNum++;
    return &sTlkmwHostLinkMgr.itemArray[sTlkmwHostLinkMgr.itemNum - 1];
}

/**
 * @brief       This function creates a new link item.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      Returns the pointer to the new link item.
 */
static inline host_link_item_t *tlkmw_host_linkmgr_newItem(uint8_t type, uint16_t handle)
{
    host_link_item_t *item = tlkmw_host_linkmgr_mallocItem();
    item->handle           = handle;
    item->type             = type;
    return item;
}

/**
 * @brief       This function deletes a link item.
 * @param[in]   item    - the pointer to the link item to delete.
 * @return      none.
 */
static inline void tlkmw_host_linkmgr_delItem(host_link_item_t *item)
{
    host_link_item_t *array = sTlkmwHostLinkMgr.itemArray;
    uint8_t           pos   = item - array;
    memcpy(item, item + 1, (sTlkmwHostLinkMgr.itemNum - 1 - pos) * sizeof(host_link_item_t));
    sTlkmwHostLinkMgr.itemNum--;
    if (sTlkmwHostLinkMgr.itemNum == 0) {
        tlkos_free(sTlkmwHostLinkMgr.itemArray);
        sTlkmwHostLinkMgr.itemArray = NULL;
        sTlkmwHostLinkMgr.capacity  = 0;
    }
}

/**
 * @brief       This function sets tick less for a link.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @param[in]   en      - enable flag.
 * @return      none.
 */
static inline void tlkmw_host_linkSetTickLess(uint8_t type, uint16_t handle, bool en)
{
    (void)handle;
    (void)en;
    if (type == TLKMDI_HOST_LINK_TYPE_BT) {
#if (TLK_STK_BT_ENABLE && DEBUG_BT_SNIFF_ENABLE)
        if (!en) {
            tlkmdi_btacl_setSniffBusy(handle, true);
        } else {
            tlkmdi_btacl_clsSniffBusy(handle, false);
        }
#endif
    }
}

/**
 * @brief       This function sets tick less for all links.
 * @param[in]   en      - enable flag.
 * @return      none.
 */
static inline void tlkmw_host_allLinkSetTickLess(bool en)
{
    host_link_item_t *array = sTlkmwHostLinkMgr.itemArray;
    for (size_t i = 0; i < sTlkmwHostLinkMgr.itemNum; i++) {
        tlkmw_host_linkSetTickLess(array[i].type, array[i].handle, en);
    }
}

/**
 * @brief       This function sets the system busy state in the core.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_host_linkmgr_setSysBusyCore(void)
{
    tlksys_enter_critical();
    tlkmw_host_allLinkSetTickLess(!sTlkmwHostLinkMgr.sysBusy);
    tlksys_leave_critical();
}

/**
 * @brief       This function adds a link to the link manager.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      none.
 */
void tlkmw_host_linkmgr_addLink(uint8_t type, uint16_t handle)
{
    if (type >= TLKMDI_HOST_LINK_TYPE_NUM) {
        return;
    }
    tlksys_enter_critical();
    host_link_item_t *pItem = tlkmw_host_linkmgr_getUsedItem(type, handle);
    if (pItem != NULL) {
        tlksys_leave_critical();
        return;
    }
    pItem        = tlkmw_host_linkmgr_newItem(type, handle);
    pItem->state = TLKMDI_HOST_LINK_STATE_CONNECT_BUSY;
    tlksys_runFuncInTask(TLKSYS_TASKID_HOST, tlkmw_host_linkmgr_setSysBusyCore);
    tlksys_leave_critical();
    tlkmw_host_linkmgr_linkChangeHook(type, handle, false);
}

/**
 * @brief       This function deletes a link from the link manager.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      none.
 */
void tlkmw_host_linkmgr_delLink(uint8_t type, uint16_t handle)
{
    tlksys_enter_critical();
    host_link_item_t *pItem = tlkmw_host_linkmgr_getUsedItem(type, handle);
    if (pItem == NULL) {
        tlksys_leave_critical();
        return;
    }
    tlkmw_host_linkmgr_delItem(pItem);
    tlksys_leave_critical();
    tlkmw_host_linkmgr_linkChangeHook(type, handle, true);
}

/**
 * @brief       This function sets the system busy state.
 * @param[in]   busy    - the busy state.
 * @return      none.
 */
void tlkmw_host_linkmgr_setSysBusy(uint8_t busy)
{
    busy = busy ? 1 : 0;
    tlksys_enter_critical();
    if (sTlkmwHostLinkMgr.sysBusy == busy) {
        tlksys_leave_critical();
        return;
    }
    sTlkmwHostLinkMgr.sysBusy = busy;
    tlksys_runFuncInTask(TLKSYS_TASKID_HOST, tlkmw_host_linkmgr_setSysBusyCore); //.... ....
    tlksys_leave_critical();
}

/**
 * @brief       This function is a hook called when link changes.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @param[in]   isDel   - true if the link is deleted, false otherwise.
 * @return      none.
 */
__attribute__((weak)) void tlkmw_host_linkmgr_linkChangeHook(uint8_t type, uint16_t handle, bool isDel)
{
    (void)type;
    (void)handle;
    (void)isDel;
}