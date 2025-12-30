/********************************************************************************************************
 * @file    tlkapp_host_module.h
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
    uint8_t hostType;
    uint8_t resv[3];
    void (*init)(void);
    void (*start)(void);
    void (*handler)(void);
    int (*input)(uint16_t msgID, uint8_t *pData, uint16_t dataLen);
} TlkAppHostModuleCfg_t;

struct TlkAppHostModule_s
{
    const TlkAppHostModuleCfg_t *cfgs;
    struct TlkAppHostModule_s   *next;
};

typedef struct TlkAppHostModule_s TlkAppHostModule_t;

/**
 * @brief       Adds a module to the host module list.
 * @param[in]   module - Pointer to the module to add.
 * @return      none.
 */
void tlkapp_host_addModule(TlkAppHostModule_t *module);


/**
 * @brief       Starts all registered modules.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_host_startAllModules(void);


/**
 * @brief       Handles all registered modules.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_host_handleAllModules(void);

/**
 * @brief       Handles messages for all registered modules.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_host_msgHandleAllModules(uint16_t msgID, uint8_t *pData, uint16_t dataLen);