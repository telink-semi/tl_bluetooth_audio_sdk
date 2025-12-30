/********************************************************************************************************
 * @file    tlksys_devMgr.c
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

typedef struct
{
    uint8_t               count;
    uint8_t               busyNum;
    uint8_t               isReg;
    uint8_t               resv;
    tlksys_devmgr_item_t *list;
} tlksys_devMgr_t;

static tlksys_devMgr_t sTlksysDevMgr = {0};

/**
 * @brief  Update device manager state
 * @param  None.
 * @returns  None.
 */
static void tlksys_devmgr_updateState(void)
{
    if (sTlksysDevMgr.isReg == 0) {
        tlksys_pm_regChn(TLKSYS_PM_CHN_DEVICE);
        sTlksysDevMgr.isReg = 1;
    }
    if (sTlksysDevMgr.busyNum == 0) {
        tlksys_pm_setChn(TLKSYS_PM_CHN_DEVICE, 0, 0);
    } else {
        tlksys_pm_setChn(TLKSYS_PM_CHN_DEVICE, 0, 1);
    }
}

/**
 * @brief  Mount a device item (core implementation)
 * @param[in] item : Pointer to device item to mount
 * @param[in] info : Pointer to device information
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
static int tlksys_devmgr_mount_core(tlksys_devmgr_item_t *item, const tlksys_dev_info_t *info)
{
    if (item->isMount) {
        return TLK_ENONE;
    }
    if (sTlksysDevMgr.count == 255) {
        return -TLK_ENOMEM;
    }
    tlksys_devmgr_item_t *node = sTlksysDevMgr.list;
    while (node != NULL) {
        if (node == item) {
            return -TLK_EPARAM;
        }
        node = node->next;
    }
    if (sTlksysDevMgr.list == NULL) {
        sTlksysDevMgr.list = item;
    } else {
        node = sTlksysDevMgr.list;
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = item;
    }
    item->next    = NULL;
    item->isMount = 1;
    item->isBusy  = 0;
    item->info    = info;
    sTlksysDevMgr.count++;
    return TLK_ENONE;
}

/**
 * @brief  Unmount a device item (core implementation)
 * @param[in] item : Pointer to device item to unmount
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_unmount_core(tlksys_devmgr_item_t *item)
{
    if (item->isMount == 0) {
        return TLK_ENONE;
    }
    tlksys_devmgr_item_t *node = sTlksysDevMgr.list;
    tlksys_devmgr_item_t *prev = NULL;

    while (node != NULL) {
        if (node == item) {
            if (prev == NULL) {
                sTlksysDevMgr.list = node->next;
            } else {
                prev->next = node->next;
            }
            node->next = NULL;
            break;
        }
        prev = node;
        node = node->next;
    }
    sTlksysDevMgr.count--;
    item->isMount = 0;
    if (item->isBusy) {
        item->isBusy = 0;
        sTlksysDevMgr.busyNum--;
    }
    if (sTlksysDevMgr.busyNum == 0) {
        tlksys_devmgr_updateState();
    }
    return TLK_ENONE;
}

/**
 * @brief  Set busy state of a device item (core implementation)
 * @param[in] item : Pointer to device item
 * @param[in] isBusy : Busy state (non-zero for busy)
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_setBusy_core(tlksys_devmgr_item_t *item, uint8_t isBusy)
{
    if (item->isMount == 0) {
        return -TLK_EPARAM;
    }
    if (isBusy && item->isBusy == 0) {
        item->isBusy = 1;
        sTlksysDevMgr.busyNum++;
        if (sTlksysDevMgr.busyNum == 1) {
            tlksys_devmgr_updateState();
        }
    } else if (!isBusy && item->isBusy) {
        item->isBusy = 0;
        sTlksysDevMgr.busyNum--;
        if (sTlksysDevMgr.busyNum == 0) {
            tlksys_devmgr_updateState();
        }
    }
    return TLK_ENONE;
}

/**
 * @brief  Mount a device item
 * @param[in] item : Pointer to device item to mount
 * @param[in] info : Pointer to device information
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_mount(tlksys_devmgr_item_t *item, const tlksys_dev_info_t *info)
{
    tlksys_enter_critical();
    int ret = tlksys_devmgr_mount_core(item, info);
    tlksys_leave_critical();
    return ret;
}

/**
 * @brief  Unmount a device item
 * @param[in] item : Pointer to device item to unmount
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_unmount(tlksys_devmgr_item_t *item)
{
    tlksys_enter_critical();
    int ret = tlksys_devmgr_unmount_core(item);
    tlksys_leave_critical();
    return ret;
}

/**
 * @brief  Set busy state of a device item
 * @param[in] item : Pointer to device item
 * @param[in] isBusy : Busy state (non-zero for busy)
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_setBusy(tlksys_devmgr_item_t *item, uint8_t isBusy)
{
    tlksys_enter_critical();
    int ret = tlksys_devmgr_setBusy_core(item, isBusy);
    tlksys_leave_critical();
    return ret;
}

/**
 * @brief  Enter suspend mode for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_enterSuspend(void)
{
    tlksys_devmgr_item_t *node = sTlksysDevMgr.list;

    while (node != NULL) {
        if (node->info != NULL && node->info->enterSuspendCB != NULL) {
            node->info->enterSuspendCB();
        }
        node = node->next;
    }
}

/**
 * @brief  Leave suspend mode for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_leaveSuspend(void)
{
    tlksys_devmgr_item_t *node = sTlksysDevMgr.list;

    while (node != NULL) {
        if (node->info != NULL && node->info->leaveSuspendCB != NULL) {
            node->info->leaveSuspendCB();
        }
        node = node->next;
    }
}

/**
 * @brief  Switch clock for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_switchClock(void)
{
    tlksys_devmgr_item_t *node = sTlksysDevMgr.list;

    while (node != NULL) {
        if (node->info != NULL && node->info->switchClockCB != NULL) {
            node->info->switchClockCB();
        }
        node = node->next;
    }
}
