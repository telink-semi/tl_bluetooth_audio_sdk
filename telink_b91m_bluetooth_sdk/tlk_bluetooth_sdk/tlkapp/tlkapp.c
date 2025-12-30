/********************************************************************************************************
 * @file    tlkapp.c
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
#include "tlkapp/tlkapp.h"

/**
 * @brief       Creates all tasks for the application.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_create_allTasks(void)
{
#if (TLK_CFG_SYSTEM_ENABLE)
    tlksys_task_create(TLKSYS_TASKID_SYSTEM, tlkapp_system_getTaskCfg()); 
#endif
#if (TLKSTK_HOST_ENABLE)
    tlksys_task_create(TLKSYS_TASKID_HOST, tlkapp_host_getTaskCfg());
#endif

#if (TLK_MW_AUDIO_ENABLE)
    tlksys_task_create(TLKSYS_TASKID_AUDIO, tlkapp_audio_getTaskCfg());
#endif
#if (TLKAPP_LEMGR_ENABLE)
    tlksys_task_create(TLKSYS_TASKID_LEMGR, tlkapp_lemgr_getTaskCfg());//TODO: clean it
#endif

    tlksys_task_create(TLKSYS_TASKID_USER, tlkapp_user_getTaskCfg());
}


/**
 * @brief       Gets the task configuration for the user task.
 *              This is a weak function and can be overridden.
 * @param[in]   none.
 * @return      Returns a pointer to the user task configuration, or NULL if not implemented.
 */
__attribute__((weak))  const tlksys_task_cfg_t * tlkapp_user_getTaskCfg(void)
{
    return NULL;
}
