/********************************************************************************************************
 * @file    tlkapp_sysCtrl.h
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
#ifndef TLKAPP_SYS_CTRL_H
#define TLKAPP_SYS_CTRL_H

#define TLKAPP_UI_HANDLE_MAX_NUM 5

typedef enum
{
    TLKAPP_UI_STATE_POWER_ON,
    TLKAPP_UI_STATE_POWER_OFF,
    TLKAPP_UI_STATE_IDLE,
    TLKAPP_UI_STATE_PARING,
    TLKAPP_UI_STATE_CONNECTED,
    TLKAPP_UI_STATE_LOWBATTARY,
    TLKAPP_UI_STATE_MAX,
} TLKAPP_UI_STATE_ENUM;

typedef enum
{
    TLKAPP_UI_HANDLE_GROUP_SYS,
    TLKAPP_UI_HANDLE_GROUP_BT,
    TLKAPP_UI_HANDLE_GROUP_BLE,
    TLKAPP_UI_HANDLE_GROUP_TPSLL,
    TLKAPP_UI_HANDLE_GROUP_MAX,
} TLKAPP_UI_HANDLE_GROUP_ENUM;

typedef struct
{
    uint8_t group;
    uint8_t state;
    uint16_t handle;
} tlkapp_sysUI_handleState_t;

/**
 * @brief       Initializes the system control module.
 * @param[in]   none.
 * @return      Returns TLK_ENONE on success.
 */
int tlkapp_sys_ctrlInit(void);

/**
 * @brief       Handles flash saving requests.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_flashHandler(void);

/**
 * @brief       Updates the handle state with mutex protection.
 * @param[in]   group  - The group ID.
 * @param[in]   handle - The handle ID.
 * @param[in]   state  - The new state.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sysUI_updateHandleState(uint8_t group, uint16_t handle, uint8_t state);

#endif // TLKAPP_SYS_CTRL_H
