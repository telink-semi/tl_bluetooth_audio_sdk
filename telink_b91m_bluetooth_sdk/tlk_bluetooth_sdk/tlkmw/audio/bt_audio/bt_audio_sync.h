/********************************************************************************************************
 * @file    bt_audio_sync.h
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
#ifndef __BT_AUDIO_SYNC_H__
#define __BT_AUDIO_SYNC_H__

#include "tlkmw/audio/audio_mw_manager.h"

#if AUDIO_TWS_MODE

enum
{
    TSYNC_FLAG_SCO   = 0x01,
    TSYNC_FLAG_MUSIC = 0x02,
};

#define AUDIO_TYPE_MUSIC  0
#define AUDIO_TYPE_VOICE  1

#define TWS_SYNC_INFO_LEN 16

/**
 * @brief  update audio sync config
 * @param[in]  tick   - task tick
 * @param[in]  p_des  - config text
 * @param[in]  fno    - frame number
 * @returns BT_AUDIO_SUCCESS or BT_AUDIO_TYPE_INVALID
 */
uint8_t bt_audio_sync_update(uint32_t tick, uint8_t *p_des, uint16_t fno);

/**
 * @brief  check sync config difference between master and slave
 * @param[in]  p_master - master config
 * @param[in]  p_slave  - slave config
 * @param[in]  type     - AUDIO_TYPE_MUSIC AUDIO_TYPE_VOICE
 * @returns BT_AUDIO_SUCCESS or BT_AUDIO_TYPE_INVALID
 */
uint8_t bt_audio_check_sync_cfg(uint8_t *p_master, uint8_t *p_slave, uint8_t type);

#endif
#endif
