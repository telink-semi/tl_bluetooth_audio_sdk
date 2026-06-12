/********************************************************************************************************
 * @file    tlkapp_host_module.c
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
#include "tlkapp_host_module.h"

static TlkAppHostModule_t *head = NULL;

/**
 * @brief       Adds a module to the host module list.
 * @param[in]   module - Pointer to the module to add.
 * @return      none.
 */
void tlkapp_host_addModule(TlkAppHostModule_t *module)
{
    if (module == NULL || module->next != NULL) {
        return;
    }
    if (head == NULL) {
        head = module;
    } else {
        TlkAppHostModule_t *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = module;
    }
    if (module->cfgs->init) {
        module->cfgs->init();
    }
}

/**
 * @brief       Starts all registered modules.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_host_startAllModules(void)
{
    TlkAppHostModule_t *current = head;
    while (current != NULL) {
        if (current->cfgs->start != NULL) {
            current->cfgs->start();
        }
        current = current->next;
    }
}

/**
 * @brief       Handles all registered modules.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_host_handleAllModules(void)
{
    TlkAppHostModule_t *current = head;
    while (current != NULL) {
        if (current->cfgs->handler != NULL) {
            current->cfgs->handler();
        }
        current = current->next;
    }
}

/**
 * @brief       Handles messages for all registered modules.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_host_msgHandleAllModules(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    TlkAppHostModule_t *current  = head;
    uint8_t             hostType = msgID >> 8;
    while (current != NULL) {
        if (current->cfgs->input != NULL && hostType == current->cfgs->hostType) {
            return current->cfgs->input(msgID, pData, dataLen);
        }
        current = current->next;
    }
    return -TLK_ENOSUPPORT;
}