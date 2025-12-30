/********************************************************************************************************
 * @file    tlkmw_anc_sch.h
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

typedef void (*tlkmw_anc_sch_cb)(uint16_t handle, uint8_t isStart);

enum
{
    TLKMW_ANC_SCH_CB_TYPE_TPSLL = 0,
    TLKMW_ANC_SCH_CB_TYPE_BT_MUSIC,
    TLKMW_ANC_SCH_CB_TYPE_BT_VOICE,
    TLKMW_ANC_SCH_CB_TYPE_NUM
};

typedef struct
{
    tlkmw_anc_sch_cb cbArray[TLKMW_ANC_SCH_CB_TYPE_NUM];
} tlkmw_anc_sch_cfg_t;

/**
 * @brief       Initialize ANC scheduler
 * @param[in]   cfg - Configuration pointer
 * @return      None
 */
void tlkmw_anc_sch_init(const tlkmw_anc_sch_cfg_t *cfg);

/**
 * @brief       Check if ANC scheduler is running
 * @param       None
 * @return      1 if running, 0 if not running
 */
uint8_t tlkmw_anc_sch_isRunning(void);

/**
 * @brief       Pause ANC scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_pause(void);

/**
 * @brief       Resume ANC scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_resume(void);

/**
 * @brief       UI function for next track
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_next(void);

/**
 * @brief       UI function for previous track
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_prev(void);

/**
 * @brief       UI function for volume up
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_volume_up(void);

/**
 * @brief       UI function for volume down
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_volume_down(void);

/**
 * @brief       UI function for play/pause control
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_playpause(void);

/**
 * @brief       UI function for accepting call
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_accept(void);

/**
 * @brief       UI function for rejecting call
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_reject(void);