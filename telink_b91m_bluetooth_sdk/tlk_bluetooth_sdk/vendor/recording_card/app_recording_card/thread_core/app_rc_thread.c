/********************************************************************************************************
 * @file    app_rc_thread.c
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
#include "tlkmw/tlkmw.h"
#include "app_rc_thread_msg.h"
#include "../data_path/app_rc_stream.h"
#include "../logic/app_rc_logic.h"
#include "../ui/app_rc_ui.h"
#include "../power/app_rc_power.h"
#include "vendor/recording_card/sdk_core/tlk_rc_sdk_version.h"

static void tlkapp_recording_card_init(void)
{
    app_rc_stream_initAll();
    app_rc_logic_init();
    app_rc_ui_init();
    app_rc_power_init();
    tlk_printf("sdk version:%s", tlk_rc_get_sdk_version());
}

static int tlkapp_recording_card_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    return app_recording_card_msgHandle(msgID, pData, dataLen);
}

const tlksys_task_cfg_t *tlkapp_user_getTaskCfg(void)
{
    _attribute_os_heap_sec_ __attribute__((aligned(4))) static uint8_t sAppTaskBuffer[6 * 1024 + TLKSYS_TASK_EXTRA_STATIC_BUFFER_SIZE];
    static const tlksys_task_cfg_t                                     sTlkAppUserTask = {
                                            .Init              = tlkapp_recording_card_init,
                                            .Input             = tlkapp_recording_card_input,
                                            .priority          = TLKSYS_TASK_SYSTEM_PRIORITY + 1,
                                            .stackSize         = sizeof(sAppTaskBuffer),
                                            .pTaskName         = "recording_card",
                                            .pTaskStaticBuffer = &sAppTaskBuffer,
    };
    return &sTlkAppUserTask;
}
