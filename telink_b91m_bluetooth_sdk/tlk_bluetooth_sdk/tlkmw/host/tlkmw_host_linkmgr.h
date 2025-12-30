/********************************************************************************************************
 * @file    tlkmw_host_linkmgr.h
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

typedef enum
{
    TLKMDI_HOST_LINK_TYPE_BT,
    TLKMDI_HOST_LINK_TYPE_BLE,
    TLKMDI_HOST_LINK_TYPE_TPSLL,
    TLKMDI_HOST_LINK_TYPE_NUM
} tlkmw_host_link_type_e;

/**
 * @brief       This function adds a link to the link manager.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      none.
 */
void tlkmw_host_linkmgr_addLink(uint8_t type, uint16_t handle);

/**
 * @brief       This function deletes a link from the link manager.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @return      none.
 */
void tlkmw_host_linkmgr_delLink(uint8_t type, uint16_t handle);

/**
 * @brief       This function sets the system busy state.
 * @param[in]   busy    - the busy state.
 * @return      none.
 */
void tlkmw_host_linkmgr_setSysBusy(uint8_t busy);

/**
 * @brief       This function is a hook called when link changes.
 * @param[in]   type    - the link type.
 * @param[in]   handle  - the link handle.
 * @param[in]   isDel   - true if the link is deleted, false otherwise.
 * @return      none.
 */
void tlkmw_host_linkmgr_linkChangeHook(uint8_t type, uint16_t handle, bool isDel);