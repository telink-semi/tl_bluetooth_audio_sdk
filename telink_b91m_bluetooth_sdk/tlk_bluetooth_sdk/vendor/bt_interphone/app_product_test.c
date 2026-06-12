/********************************************************************************************************
 * @file    app_product_test.c
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

#if TLK_CFG_PRODUCT_TEST_ENABLE

static void app_usb_shell_pt_ack(tlk_pt_msg_t *msg)
{
    tlkapi_printf(1, "get pt cmd ack:%d", msg->cmd) if (msg->msgLen)
    {
        tlkapi_send_string_data(1, "extMsg:", msg->msg, msg->msgLen);
    }
}

/*pt  = product_test*/
void app_usb_shell_pt_demo(uint8_t *pdata, uint16_t len)
{
    if (len < 8 || pdata[1] != 0xFF || pdata[2] != 0x55 || pdata[3] != 0xAC) {
        return;
    }
    tlk_pt_msg_t *msg = (tlk_pt_msg_t *)&pdata[4];
    tlkmdi_pt_setAckCB(app_usb_shell_pt_ack);
    tlkmdi_pt_cmd_exe(msg);
}

#endif