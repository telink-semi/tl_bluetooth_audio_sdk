/********************************************************************************************************
 * @file    tlkmdi_product_test.h
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

#define TLK_PT_CMD_MSG_MAX_LEN 32

typedef enum
{
    TLK_PT_CMD_ENTER_SIDETONE,
    TLK_PT_CMD_LEAVE_SIDETONE,
    TLK_PT_CMD_SET_BT_MAC,
    TLK_PT_CMD_SET_BT_NAME,
    TLK_PT_CMD_GET_BT_MAC,
    TLK_PT_CMD_GET_BT_NAME,
    TLK_PT_CMD_SET_LE_MAC,
    TLK_PT_CMD_GET_LE_MAC,
    TLK_PT_CMD_CLEAR_USER_INF,
    TLK_PT_CMD_ENTER_SCO_LOOP_BACK,
    TLK_PT_CMD_LEAVE_SCO_LOOP_BACK,
    TLK_PT_CMD_ENTER_EMI,
    TLK_PT_CMD_ENTER_BQB,
    TLK_PT_CMD_MAX,
} TLK_PT_CMD_ENUM;

typedef struct
{
    uint16_t cmd;
    uint16_t msgLen;
    uint8_t  msg[TLK_PT_CMD_MSG_MAX_LEN];
} tlk_pt_msg_t;

typedef void (*tlk_pt_ack_cb)(tlk_pt_msg_t *msg);

typedef void (*tlk_pt_cmdFunc_t)(tlk_pt_msg_t *msg);

/**
 * @brief     Sets the callback function for Bluetooth product test protocol acknowledgment.
 * @param[in] cb The callback function pointer to be set.
 */
void tlkmdi_pt_setAckCB(tlk_pt_ack_cb cb);

/**
 * @brief     Executes a Bluetooth product test protocol command.
 * @param[in] msg The message containing the command information.
 */
void tlkmdi_pt_cmd_exe(tlk_pt_msg_t *msg);
