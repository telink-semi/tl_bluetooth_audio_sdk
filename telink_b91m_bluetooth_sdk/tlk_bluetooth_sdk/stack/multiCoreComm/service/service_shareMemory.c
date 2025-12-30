/********************************************************************************************************
 * @file    service_shareMemory.c
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
#include "driver.h"
#include "../drv/shareMemory.h"
#include "../drv/platform/tlkipc_hal.h"
#include "service_shareMemory.h"
#include "service_mailbox.h"
#include "stack/multiCoreComm/drv/mailbox.h"


#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)

enum
{
    MAIN_CORE_HCI_TX = 0,
    MAIN_CORE_HCI_RX,
    MAIN_CORE_LOG_RX,
    MAIN_CORE_FIFO_NUM
};

TLKIPC_HAL_RAM_SECTION
static tlk_sm_fifo_t sTlkSmFifo[MAIN_CORE_FIFO_NUM];

TLKIPC_HAL_RAM_SECTION
static uint8_t mainCoreHciTxBuffer[TLK_SM_HCI_TX_BUFFER_SIZE];

TLKIPC_HAL_RAM_SECTION
static uint8_t mainCoreHciRxBuffer[TLK_SM_HCI_RX_BUFFER_SIZE];

static tlk_sm_rx_cb_f mainCoreHciRxCb[TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM] = {0};


#if (TLK_SM_LOG_ENABLE)
TLKIPC_HAL_RAM_SECTION
static uint8_t mainCoreLogRxBuffer[TLK_SM_LOG_BUFFER_SIZE];

static tlk_sm_rx_cb_f mainCoreLogRxCb[TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM] = {0};

//sTlkSmFifo[MAIN_CORE_LOG_RX]
void tlk_d25f_register_log_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb)
{
    if (type < TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM) {
        mainCoreLogRxCb[type] = cb;
    }
}
#else
void tlk_d25f_register_log_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb)
{
    (void)type;
    (void)cb;
}
#endif

//sTlkSmFifo[MAIN_CORE_HCI_TX]
tlk_sm_ret_e tlk_d25f_hci_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen)
{
    tlksys_mutex_lock(TLKSYS_MUTEX_H2C);
    tlk_sm_ret_e retSts = share_memory_data_push(&sTlkSmFifo[MAIN_CORE_HCI_TX], type, data, dataLen);
    tlksys_mutex_unlock(TLKSYS_MUTEX_H2C);
    if (retSts == TLK_SHARE_MEMORY_SUCCESS_NEED_NOTIFY) {
        tlk_mailbox_send_data(TLK_MESSAGE_FROM_D25F_TO_N22_WAKE_UP, NULL);
        retSts = TLK_SHARE_MEMORY_SUCCESS;
    }
    return retSts;
}

//sTlkSmFifo[MAIN_CORE_HCI_RX]
void tlk_d25f_register_hci_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb)
{
    if (type < TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM) {
        mainCoreHciRxCb[type] = cb;
    }
}


#elif MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22)

enum
{
    CONTROLLER_CORE_HCI_RX = 0,
    CONTROLLER_CORE_HCI_TX,
    CONTROLLER_CORE_LOG_TX,
    CONTROLLER_CORE_FIFO_NUM
};

static tlk_sm_fifo_t *volatile spTlkSmFifo[CONTROLLER_CORE_FIFO_NUM];
static tlk_sm_rx_cb_f controllerCoreHciRxCb[TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM] = {0};

void tlk_share_memory_n22_get_address_handler(uint32_t addr)
{
    for (size_t i = 0; i < CONTROLLER_CORE_FIFO_NUM; i++) {
        spTlkSmFifo[i] = (tlk_sm_fifo_t *)(addr + i * sizeof(tlk_sm_fifo_t));
    }
    share_memory_register_fifo_receive_cb(spTlkSmFifo[CONTROLLER_CORE_HCI_RX], controllerCoreHciRxCb);
    for (size_t i = 0; i < CONTROLLER_CORE_FIFO_NUM; i++) {
        if (spTlkSmFifo[i]->status != TLK_SHARE_MEMORY_STATUS_HAND_SHAKING) {
            continue;
        }
        spTlkSmFifo[i]->status = TLK_SHARE_MEMORY_STATUS_READY;
    }
}

//spTlkSmFifo[CONTROLLER_CORE_HCI_TX]
_attribute_ram_code_ tlk_sm_ret_e tlk_n22_hci_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen)
{
    tlk_sm_ret_e retSts = share_memory_data_push(spTlkSmFifo[CONTROLLER_CORE_HCI_TX], type, data, dataLen);
    if (retSts == TLK_SHARE_MEMORY_SUCCESS_NEED_NOTIFY) {
        uint8_t cmd[8] = {
            [0] = TLK_SM_DATA_READY_TYPE_HCI,
        };
        tlk_mailbox_send_data(TLK_MESSAGE_FROM_N22_TO_D25F_SM_DATA_READY, cmd);
        return TLK_SHARE_MEMORY_SUCCESS;
    }
    return retSts;
}

//spTlkSmFifo[CONTROLLER_CORE_LOG_TX]
_attribute_ram_code_ tlk_sm_ret_e tlk_n22_log_send_message(tlk_sm_message_type_e type, uint8_t *data, uint32_t dataLen)
{
    tlk_sm_ret_e retSts = share_memory_data_push(spTlkSmFifo[CONTROLLER_CORE_LOG_TX], type, data, dataLen);
    if (retSts == TLK_SHARE_MEMORY_SUCCESS_NEED_NOTIFY) {
        uint8_t cmd[8] = {
            [0] = TLK_SM_DATA_READY_TYPE_LOG,
        };
        tlk_mailbox_send_data(TLK_MESSAGE_FROM_N22_TO_D25F_SM_DATA_READY, cmd);
        return TLK_SHARE_MEMORY_SUCCESS;
    }
    return retSts;
}

//spTlkSmFifo[CONTROLLER_CORE_HCI_RX]
_attribute_ram_code_ void tlk_n22_register_hci_receive_cb(tlk_sm_message_type_e type, tlk_sm_rx_cb_f cb)
{
    if (type < TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM) {
        controllerCoreHciRxCb[type] = cb;
    }
}
#endif


void tlk_share_memory_service_init(void)
{
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    share_memory_fifo_init(&sTlkSmFifo[MAIN_CORE_HCI_TX], mainCoreHciTxBuffer, TLK_SM_HCI_TX_BUFFER_SIZE);
    share_memory_fifo_init(&sTlkSmFifo[MAIN_CORE_HCI_RX], mainCoreHciRxBuffer, TLK_SM_HCI_RX_BUFFER_SIZE);
    share_memory_register_fifo_receive_cb(&sTlkSmFifo[MAIN_CORE_HCI_RX], mainCoreHciRxCb);

#if (TLK_SM_LOG_ENABLE)
    share_memory_fifo_init(&sTlkSmFifo[MAIN_CORE_LOG_RX], mainCoreLogRxBuffer, TLK_SM_LOG_BUFFER_SIZE);
    share_memory_register_fifo_receive_cb(&sTlkSmFifo[MAIN_CORE_LOG_RX], mainCoreLogRxCb);
#endif
#endif
}

_attribute_ram_code_ void tlk_share_memory_service_loop(void)
{
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    share_memory_data_popAll(&sTlkSmFifo[MAIN_CORE_HCI_RX]);
    share_memory_data_popAll(&sTlkSmFifo[MAIN_CORE_LOG_RX]);
#elif (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
    share_memory_data_popAll(spTlkSmFifo[CONTROLLER_CORE_HCI_RX]);
#endif
}

_attribute_ram_code_sec_ uint8_t tlk_share_memory_service_hci_isOverTargetPercent(uint32_t percent)
{
    (void)percent;
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    return share_memory_fifo_isOverTargetPercent(&sTlkSmFifo[MAIN_CORE_HCI_RX], percent);
#elif (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
    return share_memory_fifo_isOverTargetPercent(spTlkSmFifo[CONTROLLER_CORE_HCI_RX], percent);
#else
    return 0;
#endif
}

void tlk_share_memory_service_hci_handler(void)
{
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    share_memory_data_popAll(&sTlkSmFifo[MAIN_CORE_HCI_RX]);
#endif
}

void tlk_share_memory_service_log_handler(void)
{
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    share_memory_data_popAll(&sTlkSmFifo[MAIN_CORE_LOG_RX]);
#endif
}

_always_inline void *tlkipc_share_memory_get_ctrlblock_addr(void)
{
#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
    return sTlkSmFifo;
#elif (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
    return spTlkSmFifo[0];
#else
    return NULL;
#endif
}