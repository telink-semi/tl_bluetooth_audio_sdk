/********************************************************************************************************
 * @file    tlkos_msgq_freertos.c
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
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_FREERTOS_ENABLE
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/os/3rd-party/freertos-V5/include/FreeRTOS.h"
#include "tlklib/os/3rd-party/freertos-V5/include/queue.h"
#include "tlklib/os/3rd-party/freertos-V5/include/semphr.h"
#include "tlkos_freertos_inner.h"

//For higher efficiency, the message queue use fifo(malloced) instead of the native functions of FreeRTOS.
//User can use FreeRTOS native functions instead.
typedef struct
{
    void    *semHandle;
    uint32_t msgMaxSize;
    uint32_t qLength;
} tlkOsMsgCore_t;

typedef struct
{
    uint8_t *data;
    uint32_t datalen;
} tlkOsMsgItem_t;

/**
 * @brief     Creates a message queue.
 * @param[out] pMsgQHandle Pointer to store the created message queue handle.
 * @param[in] msgMaxSize Maximum size of each message in the queue.
 * @param[in] qLength Maximum number of messages in the queue.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_create(TlkOsMsgQHandle_t *pMsgQHandle, uint32_t msgMaxSize, uint32_t qLength)
{
    if (pMsgQHandle == NULL || msgMaxSize == 0 || qLength == 0) {
        return -TLK_EPARAM;
    }
    tlkOsMsgCore_t *core = tlkos_calloc(sizeof(tlkOsMsgCore_t));
    if (core == NULL) {
        return -TLK_ENOSPACE;
    }
    core->semHandle = xQueueCreate(qLength, sizeof(void *) + sizeof(msgMaxSize)); //point + data len
    if (core->semHandle == NULL) {
        tlkos_free(core);
        return -TLK_EFAIL;
    }
    core->qLength    = qLength;
    core->msgMaxSize = msgMaxSize;
    *pMsgQHandle     = core;
    return TLK_ENONE;
}

/**
 * @brief     Destroys a message queue.
 * @param[in] msgQHandle Handle of the message queue to be destroyed.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_destroy(TlkOsMsgQHandle_t msgQHandle)
{
    if (msgQHandle == NULL) {
        return -TLK_EPARAM;
    }
    tlkOsMsgCore_t *core = (tlkOsMsgCore_t *)msgQHandle;
    vQueueDelete(core->semHandle);
    tlkos_free(core);
    return TLK_ENONE;
}

/**
 * @brief     Sends a message to the message queue.
 * @param[in] msgQHandle Handle of the message queue.
 * @param[in] pData Pointer to the message data.
 * @param[in] dataLen Length of the message data.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_send(TlkOsMsgQHandle_t msgQHandle, uint8_t *pData, uint32_t dataLen, uint32_t blockTimeMs)
{
    tlkOsMsgCore_t *core = (tlkOsMsgCore_t *)msgQHandle;
    if (core == NULL || pData == NULL || dataLen > core->msgMaxSize) {
        return -TLK_EPARAM;
    }
    uint8_t *inbuffer = tlkos_malloc(dataLen);
    if (inbuffer == NULL) {
        return -TLK_ENOSPACE;
    }
    memcpy(inbuffer, pData, dataLen);
    tlkOsMsgItem_t msgItem = {
        .data    = inbuffer,
        .datalen = dataLen,
    };
    uint32_t   delayTick = tlkos_freertos_msToTick(blockTimeMs);
    BaseType_t ret       = xQueueSend(core->semHandle, (uint8_t *)&msgItem, delayTick);
    if (ret != pdTRUE) {
        tlkos_free(inbuffer);
        return -TLK_EFAIL;
    }
    return TLK_ENONE;
}

/**
 * @brief     Waits to receive a message from the message queue.
 * @param[in] msgQHandle Handle of the message queue.
 * @param[out] pBuff Buffer to store the received message.
 * @param[out] recLen Pointer to store the length of the received message.
 * @param[in] buffLen Size of the buffer.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_wait(TlkOsMsgQHandle_t msgQHandle, uint8_t *pBuff, uint32_t *recLen, uint32_t buffLen, uint32_t blockTimeMs)
{
    tlkOsMsgCore_t *core = (tlkOsMsgCore_t *)msgQHandle;
    if (core == NULL || pBuff == NULL || buffLen < core->msgMaxSize) {
        return -TLK_EPARAM;
    }
    uint32_t       delayTick = tlkos_freertos_msToTick(blockTimeMs);
    tlkOsMsgItem_t msgItem;
    BaseType_t     ret = xQueueReceive(core->semHandle, (uint8_t *)&msgItem, delayTick);
    if (ret != pdTRUE) {
        return -TLK_EFAIL;
    }
    memcpy(pBuff, msgItem.data, msgItem.datalen);
    if (recLen != NULL) {
        *recLen = msgItem.datalen;
    }

    tlkos_free(msgItem.data);
    return TLK_ENONE;
}


#endif //TLKOS_CFG_FREERTOS_ENABLE
