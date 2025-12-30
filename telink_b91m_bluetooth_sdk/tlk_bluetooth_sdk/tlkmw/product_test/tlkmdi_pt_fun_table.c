/********************************************************************************************************
 * @file    tlkmdi_pt_fun_table.c
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

typedef struct
{
    uint8_t isInSideTone  : 1;
    uint8_t isScoLoopBack : 1;
} tlkmdi_pt_ctrl_t;

static tlkmdi_pt_ctrl_t sTlkMdiPTCtrl;

void tlkmdi_pt_ack(tlk_pt_msg_t *msg);
/*note: only use in this .c file*/
#define CMD_FUNC(func_name) static void func_name##_FUNC(tlk_pt_msg_t *msg)

/*note: only use in this .c file*/
#define SET_CMD_FUNC(func_name) [func_name] = func_name##_FUNC

#define CMD_ACK(func_name)            \
    {                                 \
        tlk_pt_msg_t ack = {0};       \
        ack.cmd          = func_name; \
        tlkmdi_pt_ack(&ack);          \
    }

CMD_FUNC(TLK_PT_CMD_ENTER_SIDETONE)
{
    (void)msg;
    if (sTlkMdiPTCtrl.isInSideTone == 1) {
        return;
    }
    sTlkMdiPTCtrl.isInSideTone = 1;
    // tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_LEFT);
    // tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_RIGHT);
    // tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_STEREO);
    tlkmdi_audio_sendStartEvtEx(TLKAUD_TYPE_SIDETONE, 0xFFFF, 7);
    CMD_ACK(TLK_PT_CMD_ENTER_SIDETONE);
}

CMD_FUNC(TLK_PT_CMD_LEAVE_SIDETONE)
{
    (void)msg;
    if (sTlkMdiPTCtrl.isInSideTone == 0) {
        return;
    }
    sTlkMdiPTCtrl.isInSideTone = 0;
    tlkmdi_audio_sendCloseEvtEx(TLKAUD_TYPE_SIDETONE, 0xFFFF, true);
    CMD_ACK(TLK_PT_CMD_LEAVE_SIDETONE);
}

CMD_FUNC(TLK_PT_CMD_SET_BT_MAC)
{
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_ADDR, msg->msg, msg->msgLen);
    CMD_ACK(TLK_PT_CMD_SET_BT_MAC);
}

CMD_FUNC(TLK_PT_CMD_SET_BT_NAME)
{
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_NAME, msg->msg, msg->msgLen);
    CMD_ACK(TLK_PT_CMD_SET_BT_NAME);
}

CMD_FUNC(TLK_PT_CMD_GET_BT_MAC)
{
    (void)msg;
    tlk_pt_msg_t ack;
    ack.cmd    = TLK_PT_CMD_GET_BT_MAC;
    ack.msgLen = 6;
    tlkmdi_bt_mgr_getAddr(ack.msg);
    tlkmdi_pt_ack(&ack);
}

CMD_FUNC(TLK_PT_CMD_GET_BT_NAME)
{
    (void)msg;
    tlk_pt_msg_t ack;
    tlkmdi_bt_mgr_getName(ack.msg);

    uint32_t nameLen = strlen((char *)ack.msg);

    ack.cmd    = TLK_PT_CMD_GET_BT_NAME;
    ack.msgLen = nameLen + 1;
    tlkmdi_pt_ack(&ack);
}

CMD_FUNC(TLK_PT_CMD_SET_LE_MAC)
{
    if (msg->msgLen != 6) {
        return;
    }
    tlkmdi_tinySql_setLeMacAddress(msg->msg);
    void ble_host_set_bd_addr_telink(const uint8_t *mac);
    ble_host_set_bd_addr_telink(msg->msg);
    CMD_ACK(TLK_PT_CMD_SET_LE_MAC);
}

CMD_FUNC(TLK_PT_CMD_GET_LE_MAC)
{
    (void)msg;
    tlk_pt_msg_t ack;
    ack.cmd    = TLK_PT_CMD_GET_LE_MAC;
    ack.msgLen = 6;
    tlkmdi_tinySql_getLeMacAddress(ack.msg);
    tlkmdi_pt_ack(&ack);
}

CMD_FUNC(TLK_PT_CMD_CLEAR_USER_INF)
{
    (void)msg;
    tlksys_sendMsg(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_CLEAN_SQL, NULL, 0);
    CMD_ACK(TLK_PT_CMD_CLEAR_USER_INF);
}

CMD_FUNC(TLK_PT_CMD_ENTER_SCO_LOOP_BACK)
{
    (void)msg;
    if (sTlkMdiPTCtrl.isScoLoopBack == 1) {
        return;
    }
    sTlkMdiPTCtrl.isScoLoopBack = 1;
    tlkmdi_audio_hostif_setProductTestEn(1);
    CMD_ACK(TLK_PT_CMD_ENTER_SCO_LOOP_BACK);
}

CMD_FUNC(TLK_PT_CMD_LEAVE_SCO_LOOP_BACK)
{
    (void)msg;
#if TLKBTP_CFG_HFP_ENABLE

    if (sTlkMdiPTCtrl.isScoLoopBack == 0) {
        return;
    }
    sTlkMdiPTCtrl.isScoLoopBack = 0;
    tlkmdi_audio_hostif_setProductTestEn(0);
    extern uint16_t btp_hfp_getAgHandle(void);
    uint16_t        handle = btp_hfp_getAgHandle();
    if (handle) {
        tlkmdi_audio_sendCloseEvtEx(TLKAUD_TYPE_CC_BT_VOICE, handle, true);
    }
    CMD_ACK(TLK_PT_CMD_LEAVE_SCO_LOOP_BACK);
#endif
}

CMD_FUNC(TLK_PT_CMD_ENTER_EMI)
{
    (void)msg;
    tlkmdi_tinySql_setWorkMode(TLK_WORK_MODE_TEST_EMI);
    core_interrupt_disable();
    tlkmdi_tinySql_save();
    sys_reboot();
    while (1);
}

CMD_FUNC(TLK_PT_CMD_ENTER_BQB)
{
    (void)msg;
    tlkmdi_tinySql_setWorkMode(TLK_WORK_MODE_TEST_BQB);
    core_interrupt_disable();
    tlkmdi_tinySql_save();
    sys_reboot();
    while (1);
}

static const tlk_pt_cmdFunc_t tlkmdi_bt_tpt_pt_fun_table[TLK_PT_CMD_MAX] = {
    SET_CMD_FUNC(TLK_PT_CMD_ENTER_SIDETONE), SET_CMD_FUNC(TLK_PT_CMD_LEAVE_SIDETONE),      SET_CMD_FUNC(TLK_PT_CMD_SET_BT_MAC),          SET_CMD_FUNC(TLK_PT_CMD_SET_BT_NAME),
    SET_CMD_FUNC(TLK_PT_CMD_GET_BT_MAC),     SET_CMD_FUNC(TLK_PT_CMD_GET_BT_NAME),         SET_CMD_FUNC(TLK_PT_CMD_SET_LE_MAC),          SET_CMD_FUNC(TLK_PT_CMD_GET_LE_MAC),
    SET_CMD_FUNC(TLK_PT_CMD_CLEAR_USER_INF), SET_CMD_FUNC(TLK_PT_CMD_ENTER_SCO_LOOP_BACK), SET_CMD_FUNC(TLK_PT_CMD_LEAVE_SCO_LOOP_BACK), SET_CMD_FUNC(TLK_PT_CMD_ENTER_EMI),
    SET_CMD_FUNC(TLK_PT_CMD_ENTER_BQB),
};

tlk_pt_cmdFunc_t tlkmdi_bt_tpt_pt_getCmdHandlerFunc(uint16_t cmd)
{
    if (cmd >= TLK_PT_CMD_MAX) {
        return NULL;
    }
    return tlkmdi_bt_tpt_pt_fun_table[cmd];
}
#endif
