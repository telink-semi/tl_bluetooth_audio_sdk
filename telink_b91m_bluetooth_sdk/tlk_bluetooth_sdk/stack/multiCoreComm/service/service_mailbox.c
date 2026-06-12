/********************************************************************************************************
 * @file    service_mailbox.c
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
#include "service_mailbox.h"
#include "../drv/mailbox.h"
#include "drivers.h"
#include "stack/stack.h"

typedef void (*tlk_mailbox_receive_cb_f)(uint8_t *);

#if (MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22))
#define TLK_MAILBOX_RECEIVE_CB_NUM TLK_MESSAGE_FROM_N22_TO_D25F_MAX
#elif (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
#define TLK_MAILBOX_RECEIVE_CB_NUM TLK_MESSAGE_FROM_D25F_TO_N22_MAX
#else
#define TLK_MAILBOX_RECEIVE_CB_NUM 1
#endif


static tlk_mailbox_receive_cb_f sTlkMailboxReceiveCb[TLK_MAILBOX_RECEIVE_CB_NUM];

_attribute_ram_code_ void tlkipc_mailbox_receive_hook(uint8_t *cmd)
{
    if ((cmd[0] >= TLK_MAILBOX_RECEIVE_CB_NUM) || (sTlkMailboxReceiveCb[cmd[0]] == NULL)) {
        return;
    }
    sTlkMailboxReceiveCb[cmd[0]](&cmd[1]);
}

void tlk_mailbox_send_data(uint8_t cmd, uint8_t *data)
{
    tlk_mailbox_send_dataEx(cmd, data, MAILBOX_TX_MAX_BLOCK_TIME_US);
}

void tlk_mailbox_send_dataEx(uint8_t cmd, uint8_t *data, uint32_t maxBlockTime)
{
    uint8_t sendData[8] = {0};
    sendData[0]         = cmd;
    if (data != NULL) {
        tmemcpy(&sendData[1], data, 7);
    }
    tlkipc_mailbox_send_data(sendData, maxBlockTime);
}

void tlk_mailbox_register_message_cb(uint8_t cmd, tlk_mailbox_receive_cb_f cb)
{
    if (cmd >= TLK_MAILBOX_RECEIVE_CB_NUM) {
        return;
    }
    sTlkMailboxReceiveCb[cmd] = cb;
}

void tlk_mailbox_service_init(void)
{
#if MCU_DUAL_CORE_ENABLE
    tlkipc_mailbox_init();
#endif
}
