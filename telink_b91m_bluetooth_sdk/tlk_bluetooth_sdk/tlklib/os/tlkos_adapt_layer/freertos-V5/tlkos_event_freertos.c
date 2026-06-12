/********************************************************************************************************
 * @file    tlkos_event_freertos.c
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
#include "tlklib/os/3rd-party/freertos-V5/include/semphr.h"
#include "tlkos_freertos_inner.h"

typedef struct
{
    uint16_t          length;
    uint16_t          isMalloced;
    uint32_t          evtBits;
    StaticSemaphore_t semphrBuf;
    TlkOsEventDealCB  cbTab[0];
} TlkOsEventItem_t;

#define TLKOS_EVENT_SEMPHR(pTab) ((SemaphoreHandle_t) & (pTab)->semphrBuf)

/**
 * @brief       Get the count of trailing zeros in the binary representation of a 32-bit value.
 * @param[in]   val    - input value to count trailing zeros.
 * @return      Number of trailing zeros in val, or 32 if val is zero.
 */
static __attribute__((__always_inline__)) inline uint32_t tlkos_event_getCTZ(uint32_t val)
{
    if (val == 0) {
        return 32;
    }
    return __builtin_ctz(val);
}

/**
 * @brief     Creates an event tab.
 * @param[in] evtTabLen Length of the event tab.
 * @param[out] evtTabHandle Pointer to store the created event tab handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
uint32_t tlkos_event_getStaticBufferLen(uint32_t evtTabLen)
{
    if (evtTabLen == 0 || evtTabLen > 24) {
        return 0;
    }
    return sizeof(TlkOsEventItem_t) + sizeof(TlkOsEventDealCB) * evtTabLen;
}

int tlkos_event_createTabStatic(uint32_t evtTabLen, uint8_t *pStaticBuffer, uint32_t staticBufferSize, TlkOsEventTabHandle_t *evtTabHandle)
{
    if (evtTabLen == 0 || evtTabLen > 24 || pStaticBuffer == NULL) {
        return -TLK_EPARAM;
    }
    uint32_t needSize = tlkos_event_getStaticBufferLen(evtTabLen);
    if (staticBufferSize < needSize) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab = (TlkOsEventItem_t *)pStaticBuffer;
    memset(pTab, 0, needSize);
    pTab->length = evtTabLen;
    xSemaphoreCreateBinaryStatic(&pTab->semphrBuf);

    if (evtTabHandle != NULL) {
        *evtTabHandle = (TlkOsEventTabHandle_t *)pTab;
    }
    return TLK_ENONE;
}

int tlkos_event_createTab(uint32_t evtTabLen, TlkOsEventTabHandle_t *evtTabHandle)
{
    if (evtTabLen == 0 || evtTabLen > 24) {
        return -TLK_EPARAM;
    }
    uint32_t          totalSize = sizeof(TlkOsEventItem_t) + sizeof(TlkOsEventDealCB) * evtTabLen;
    TlkOsEventItem_t *pTab      = tlkos_calloc(totalSize);
    if (pTab == NULL) {
        return -TLK_ENOMEM;
    }
    pTab->length     = evtTabLen;
    pTab->isMalloced = 1;
    xSemaphoreCreateBinaryStatic(&pTab->semphrBuf);

    if (evtTabHandle != NULL) {
        *evtTabHandle = (TlkOsEventTabHandle_t *)pTab;
    }
    return TLK_ENONE;
}

/**
 * @brief     Destroys an event tab.
 * @param[in] evtTabHandle Handle of the event tab to destroy.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_destroyTab(TlkOsEventTabHandle_t evtTabHandle)
{
    if (evtTabHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab = (TlkOsEventItem_t *)evtTabHandle;
    vSemaphoreDelete(TLKOS_EVENT_SEMPHR(pTab));
    if (pTab->isMalloced) {
        tlkos_free(pTab);
    } else {
        memset(pTab, 0, sizeof(TlkOsEventItem_t) + sizeof(TlkOsEventDealCB) * pTab->length);
    }
    return TLK_ENONE;
}

/**
 * @brief     Registers an event deal callback.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @param[in] cb Callback function to register.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_regDealCB(TlkOsEventTabHandle_t evtTabHandle, uint32_t index, TlkOsEventDealCB cb)
{
    if (evtTabHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab = (TlkOsEventItem_t *)evtTabHandle;
    if (pTab->length <= index) {
        return -TLK_EPARAM;
    }
    uint32_t r         = core_interrupt_disable();
    pTab->cbTab[index] = cb;
    core_restore_interrupt(r);
    return TLK_ENONE;
}

/**
 * @brief     Sets an event bit.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_set(TlkOsEventTabHandle_t evtTabHandle, uint32_t index)
{
    if (evtTabHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab = (TlkOsEventItem_t *)evtTabHandle;
    if (pTab->length <= index) {
        return -TLK_EPARAM;
    }
    if (pTab->cbTab[index] == NULL) {
        return TLK_ENONE;
    }
    uint32_t r = core_interrupt_disable();
    pTab->evtBits |= (1 << index);
    core_restore_interrupt(r);
    xSemaphoreGive(TLKOS_EVENT_SEMPHR(pTab));
    return TLK_ENONE;
}

/**
 * @brief     Sets an event bit from interrupt context.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] index Index of the event.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
_attribute_ram_code_sec_ int tlkos_event_setFromIsr(TlkOsEventTabHandle_t evtTabHandle, uint32_t index)
{
    if (evtTabHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab = (TlkOsEventItem_t *)evtTabHandle;
    if (pTab->length <= index) {
        return -TLK_EPARAM;
    }
    if (pTab->cbTab[index] == NULL) {
        return TLK_ENONE;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t   r                        = core_interrupt_disable();
    pTab->evtBits |= (1 << index);
    core_restore_interrupt(r);
    xSemaphoreGiveFromISR(TLKOS_EVENT_SEMPHR(pTab), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return TLK_ENONE;
}

/**
 * @brief     Waits for events.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_wait(TlkOsEventTabHandle_t evtTabHandle, uint32_t blockTimeMs)
{
    if (evtTabHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkOsEventItem_t *pTab      = (TlkOsEventItem_t *)evtTabHandle;
    uint32_t          delayTick = tlkos_freertos_msToTick(blockTimeMs);
    xSemaphoreTake(TLKOS_EVENT_SEMPHR(pTab), delayTick);
    uint32_t r         = core_interrupt_disable();
    uint32_t eventBits = pTab->evtBits;
    pTab->evtBits      = 0;
    core_restore_interrupt(r);
    while (eventBits) {
        uint32_t index = tlkos_event_getCTZ(eventBits);
        eventBits &= ~(1 << index);

        if (index < pTab->length && pTab->cbTab[index] != NULL) {
            tlkos_debug_ioCtrlEx(TLKOS_DEBUG_IO_AUD_IRQ_THREAD, 1, TLKOS_DEBUG_IO_EX_FEATURE_AUD_IRQ, &evtTabHandle);
            pTab->cbTab[index]();
            tlkos_debug_ioCtrlEx(TLKOS_DEBUG_IO_AUD_IRQ_THREAD, 0, TLKOS_DEBUG_IO_EX_FEATURE_AUD_IRQ, &evtTabHandle);
        }
    }

    return TLK_ENONE;
}

/**
 * @brief     Gets event bits.
 * @param[in] evtTabHandle Handle of the event tab.
 * @param[out] evt Pointer to store the event bits.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_event_get(TlkOsEventTabHandle_t evtTabHandle, uint32_t *evt)
{
    if (evtTabHandle == NULL || evt == NULL) {
        return -TLK_EPARAM;
    }
    return -TLK_ENOSUPPORT;
}

#endif //TLKOS_CFG_FREERTOS_ENABLE