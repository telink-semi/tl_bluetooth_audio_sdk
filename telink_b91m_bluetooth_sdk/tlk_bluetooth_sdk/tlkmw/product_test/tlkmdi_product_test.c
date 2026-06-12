/********************************************************************************************************
 * @file    tlkmdi_product_test.c
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
#include "tlkmw/tlkmw.h"

static tlk_pt_ack_cb tlkmdi_pt_ack_cb = NULL;

/**
 * @brief     Acknowledge a Bluetooth product test message by calling the registered callback function
 * @param[in] msg The message to be acknowledged
 */
void tlkmdi_pt_ack(tlk_pt_msg_t *msg)
{
    if (tlkmdi_pt_ack_cb) {
        tlkmdi_pt_ack_cb(msg);
    }
}

/**
 * @brief     Sets the callback function for Bluetooth product test protocol acknowledgment.
 * @param[in] cb The callback function pointer to be set.
 */
void tlkmdi_pt_setAckCB(tlk_pt_ack_cb cb)
{
    tlkmdi_pt_ack_cb = cb;
}

/**
 * @brief     Executes a Bluetooth product test protocol command.
 * @param[in] msg The message containing the command information.
 */
void tlkmdi_pt_cmd_exe(tlk_pt_msg_t *msg)
{
    (void)msg;
#if TLK_CFG_PRODUCT_TEST_ENABLE
    extern tlk_pt_cmdFunc_t tlkmdi_bt_tpt_pt_getCmdHandlerFunc(uint16_t cmd);
    tlk_pt_cmdFunc_t        func = tlkmdi_bt_tpt_pt_getCmdHandlerFunc(msg->cmd);
    if (func == NULL) {
        return;
    }
    func(msg);
#endif
}
