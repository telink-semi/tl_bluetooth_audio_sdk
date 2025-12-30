/********************************************************************************************************
 * @file    tlkapp.h
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

#include "tlkapp/audio/tlkapp_audio.h"
#include "tlkapp/audio/tlkapp_audioCtrl.h"
#include "tlkapp/audio/tlkapp_audioModinf.h"
#include "tlkapp/audio/tlkapp_audioScheduler.h"
#include "tlkapp/audio/tlkapp_audioMsg.h"
#include "tlkapp/audio/tlkapp_audioUac.h"

#include "tlkapp/host/module/tlkapp_host_module.h"
#include "tlkapp/host/module/bt/tlkapp_host_bt.h"
#include "tlkapp/host/module/bt/tlkapp_host_bt_msg.h"
#include "tlkapp/host/module/bt/tlkapp_host_bt_hook.h"
#include "tlkapp/host/module/le/tlkapp_host_le.h"
#include "tlkapp/host/module/tpsll/tpd/tlkapp_host_tpd.h"
#include "tlkapp/host/module/tpsll/tph/tlkapp_host_tph.h"
#include "tlkapp/host/module/tpsll/tpt/tlkapp_host_tpt.h"
#include "tlkapp/host/module/tpsll/tpmd/tlkapp_host_tpmd.h"
#include "tlkapp/host/tlkapp_host.h"

#include "tlkapp/lemgr/tlkapp_lemgr.h"
#include "tlkapp/lemgr/tlkapp_lemgrAcl.h"
#include "tlkapp/lemgr/tlkapp_lemgrMsg.h"

#include "tlkapp/system/tlkapp_sys.h"
#include "tlkapp/system/tlkapp_sysCtrl.h"
#include "tlkapp/system/tlkapp_sysKey.h"
#include "tlkapp/system/tlkapp_sysLed.h"
#include "tlkapp/system/tlkapp_sysMsg.h"
#include "tlkapp/system/tlkapp_sysPm.h"


/**
 * @brief       Creates all tasks for the application.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_create_allTasks(void);


/**
 * @brief       Gets the task configuration for the user task.
 *              This is a weak function and can be overridden.
 * @param[in]   none.
 * @return      Returns a pointer to the user task configuration, or NULL if not implemented.
 */
const tlksys_task_cfg_t *tlkapp_user_getTaskCfg(void);

