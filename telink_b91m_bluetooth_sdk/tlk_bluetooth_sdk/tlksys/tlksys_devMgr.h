/********************************************************************************************************
 * @file    tlksys_devMgr.h
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

typedef struct
{
    void (*switchClockCB)(void);
    void (*leaveSuspendCB)(void);
    void (*enterSuspendCB)(void);
} tlksys_dev_info_t;

struct tlksys_devmgr_item_s
{
    uint8_t                      isBusy;
    uint8_t                      isMount;
    uint16_t                     resv;
    const tlksys_dev_info_t     *info;
    struct tlksys_devmgr_item_s *next;
};

typedef struct tlksys_devmgr_item_s tlksys_devmgr_item_t;

/**
 * @brief  Mount a device item
 * @param[in] item : Pointer to device item to mount
 * @param[in] info : Pointer to device information
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_mount(tlksys_devmgr_item_t *item, const tlksys_dev_info_t *info);

/**
 * @brief  Unmount a device item
 * @param[in] item : Pointer to device item to unmount
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_unmount(tlksys_devmgr_item_t *item);

/**
 * @brief  Set busy state of a device item
 * @param[in] item : Pointer to device item
 * @param[in] isBusy : Busy state (non-zero for busy)
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_devmgr_setBusy(tlksys_devmgr_item_t *item, uint8_t isBusy);

/**
 * @brief  Enter suspend mode for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_enterSuspend(void);

/**
 * @brief  Leave suspend mode for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_leaveSuspend(void);

/**
 * @brief  Switch clock for all devices
 * @param  None.
 * @returns  None.
 */
void tlksys_devMgr_switchClock(void);