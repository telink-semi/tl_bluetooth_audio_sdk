/********************************************************************************************************
 * @file    tlkipc_service_coreInfo.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/pm/pm_sys.h"
#include "stack/stack.h"
#include "service_shareMemory.h"
#include "../drv/platform/tlkipc_hal.h"
#include "service_dualcore_mutex.h"
#include "stack/multiCoreComm/drv/platform/tlkipc_port.h"

enum
{
    TLKIPC_STATE_NOINIT = 0,
    TLKIPC_STATE_INITING,
    TLKIPC_STATE_READY
};

typedef struct
{
    volatile uint32_t state;
    volatile uint8_t  mainCoreRecbox[8];
    volatile uint8_t  controllerCoreRecbox[8];
    volatile uint32_t shareMemAddr;
    volatile uint32_t pmInfoAddr;
    volatile uint32_t mutexAddr;
} tlkipc_service_coreInfo_t;

#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
TLKIPC_HAL_RAM_SECTION
static tlkipc_service_coreInfo_t s_tlkipc_service_coreInfo = {0};

int tlkipc_service_coreInfo_sync(void)
{
    memset(&s_tlkipc_service_coreInfo, 0, sizeof(s_tlkipc_service_coreInfo));
    s_tlkipc_service_coreInfo.shareMemAddr = (uint32_t)tlkipc_share_memory_get_ctrlblock_addr();
    s_tlkipc_service_coreInfo.mutexAddr    = tlkipc_mutex_getCtrlBlockAddr();

#if (TLK_CFG_SUSPEND_ENABLE)
    s_tlkipc_service_coreInfo.pmInfoAddr = tlksdk_pm_d25fGetPmShareAddr();
#endif
    uint32_t wait_time = clock_time();
    while (s_tlkipc_service_coreInfo.state == TLKIPC_STATE_NOINIT) {
        if (clock_time_exceed(wait_time, 1 * 1000 * 1000)) {
            return -1;
        }
    }
    uint32_t addr = (uint32_t)&s_tlkipc_service_coreInfo;
    tlkipc_hal_mailbox_send(&addr, 4);
    while (s_tlkipc_service_coreInfo.state != TLKIPC_STATE_READY);
    return 0;
}

_attribute_ram_code_sec_ uint8_t tlkipc_mailbox_first_receive_check_hook(uint32_t data)
{
    (void)data;
    if (s_tlkipc_service_coreInfo.state == TLKIPC_STATE_NOINIT) {
        s_tlkipc_service_coreInfo.state = TLKIPC_STATE_INITING;
        return 1;
    }
    return 0;
}

void tlkipc_hal_mailbox_send_by_ram(void *data, uint32_t dataLen)
{
    (void)dataLen;
    tlkipc_memcpy((void *)s_tlkipc_service_coreInfo.controllerCoreRecbox, data, 8);
}

void tlkipc_hal_mailbox_get_by_ram(void *buffer)
{
    tlkipc_memcpy((void *)buffer, (void *)s_tlkipc_service_coreInfo.mainCoreRecbox, 8);
}

#elif (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
static tlkipc_service_coreInfo_t *volatile sp_tlkipc_service_coreInfo = NULL;

int tlkipc_service_coreInfo_sync(void)
{
    tlkipc_hal_mailbox_send(NULL, 4);
    while (sp_tlkipc_service_coreInfo == NULL || sp_tlkipc_service_coreInfo->state != TLKIPC_STATE_READY);
    return 0;
}

_attribute_ram_code_sec_ uint8_t tlkipc_mailbox_first_receive_check_hook(uint32_t data)
{
    if (sp_tlkipc_service_coreInfo != NULL) {
        return 0;
    }
    sp_tlkipc_service_coreInfo = (tlkipc_service_coreInfo_t *)data;
    tlk_share_memory_n22_get_address_handler(sp_tlkipc_service_coreInfo->shareMemAddr);
#if (TLK_CFG_SUSPEND_ENABLE)
    tlksdk_pm_n22GetD25fPmShareAddr(sp_tlkipc_service_coreInfo->pmInfoAddr);
#endif
    tlkipc_mutex_syncCtrlBlockAddrHandler(sp_tlkipc_service_coreInfo->mutexAddr);
    sp_tlkipc_service_coreInfo->state = TLKIPC_STATE_READY;

    return 1;
}

void tlkipc_hal_mailbox_send_by_ram(void *data, uint32_t dataLen)
{
    (void)dataLen;
    tlkipc_memcpy((void *)sp_tlkipc_service_coreInfo->mainCoreRecbox, (void *)data, 8);
}

void tlkipc_hal_mailbox_get_by_ram(void *buffer)
{
    tlkipc_memcpy((void *)buffer, (void *)sp_tlkipc_service_coreInfo->controllerCoreRecbox, 8);
}

#else
int tlkipc_service_coreInfo_sync(void)
{
    return 0;
}

#endif
