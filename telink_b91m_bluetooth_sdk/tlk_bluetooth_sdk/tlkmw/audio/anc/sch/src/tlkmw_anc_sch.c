/********************************************************************************************************
 * @file    tlkmw_anc_sch.c
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
#if TLKALG_ANC_ENABLE
#include "tlkmw/tlkmw.h"
#include "../inc/tlkmw_anc_sch_inner.h"

typedef struct
{
    uint8_t                    enable    : 4;
    uint8_t                    isRunning : 4;
    uint8_t                    nowType;
    uint16_t                   nowHandle;
    const tlkmw_anc_sch_cfg_t *cfg;
} AncAudioSch_t;

typedef uint16_t (*AncAudioGetHandleCB)(void);

static AncAudioSch_t             sTlkmwAncSch                                     = {0};
static const AncAudioGetHandleCB sTlkmwAncSchGetHandle[TLKMW_ANC_SCH_CB_TYPE_NUM] = {
    [TLKMW_ANC_SCH_CB_TYPE_TPSLL]    = tlkmw_anc_tpsll_getRunningHandle,
    [TLKMW_ANC_SCH_CB_TYPE_BT_MUSIC] = tlkmw_anc_bt_music_getRunningHandle,
    [TLKMW_ANC_SCH_CB_TYPE_BT_VOICE] = tlkmw_anc_bt_voice_getRunningHandle,
};

#if TLKSTK_BTTPSLL_TWS_ENABLE
//only used for tws,need sync state each other.
static void tlkmw_anc_twsSyncAncStateCore(void)
{
    tlkmdi_bt_tpt_syncAncState(sTlkmwAncSch.enable);
}

static inline void tlkmw_anc_twsSyncAncState(void)
{
    tlksys_runFuncInTask(TLKSYS_TASKID_HOST, tlkmw_anc_twsSyncAncStateCore);
}
#endif

/**
 * @brief       Try to run callback function
 * @param[in]   type - Callback type
 * @param[in]   handle - Handle value
 * @param[in]   isStart - Start flag
 * @return      None
 */
static inline void tlkmw_anc_sch_try_run_cb(uint8_t type, uint16_t handle, uint8_t isStart)
{
    if (sTlkmwAncSch.isRunning == 0) {
        return;
    }
    sTlkmwAncSch.cfg->cbArray[type](handle, isStart);
}

/**
 * @brief       Core function for ANC scheduler
 * @param[in]   isStart - Start flag
 * @return      None
 */
static inline void tlkmw_anc_sch_core(uint8_t isStart)
{
    if (sTlkmwAncSch.nowHandle == 0) {
        return;
    }
    tlkmw_anc_sch_try_run_cb(sTlkmwAncSch.nowType, sTlkmwAncSch.nowHandle, isStart);
}

/**
 * @brief       Request update start for ANC scheduler
 * @param[in]   type - Type value
 * @param[in]   handle - Handle value
 * @return      None
 */
static inline void tlkmw_anc_sch_request_update_start(uint8_t type, uint16_t handle)
{
    if (sTlkmwAncSch.nowHandle != 0) {
        if (sTlkmwAncSch.nowType >= type) {
            return;
        }
        tlkmw_anc_sch_try_run_cb(sTlkmwAncSch.nowType, sTlkmwAncSch.nowHandle, 0);
    }
    sTlkmwAncSch.nowType   = type;
    sTlkmwAncSch.nowHandle = handle;
    tlkmw_anc_sch_try_run_cb(sTlkmwAncSch.nowType, sTlkmwAncSch.nowHandle, 1);
}

/**
 * @brief       Request update stop for ANC scheduler
 * @param[in]   type - Type value
 * @param[in]   handle - Handle value
 * @return      None
 */
static inline void tlkmw_anc_sch_request_update_stop(uint8_t type, uint16_t handle)
{
    if (sTlkmwAncSch.nowHandle != handle || sTlkmwAncSch.nowType != type) {
        return;
    }
    tlkmw_anc_sch_try_run_cb(sTlkmwAncSch.nowType, sTlkmwAncSch.nowHandle, 0);
    for (uint8_t item = type; item > 0; item--) {
        uint16_t runHandle = sTlkmwAncSchGetHandle[item - 1]();
        if (runHandle == 0) {
            continue;
        }
        sTlkmwAncSch.nowType   = item - 1;
        sTlkmwAncSch.nowHandle = runHandle;
        tlkmw_anc_sch_try_run_cb(sTlkmwAncSch.nowType, sTlkmwAncSch.nowHandle, 1);
        return;
    }
    sTlkmwAncSch.nowHandle = 0;
}

/**
 * @brief       Request ANC scheduler update
 * @param[in]   type - Type value
 * @param[in]   handle - Handle value
 * @param[in]   isStart - Start flag
 * @return      None
 */
void tlkmw_anc_sch_request_update(uint8_t type, uint16_t handle, uint8_t isStart)
{
    if (isStart) {
        tlkmw_anc_sch_request_update_start(type, handle);
    } else {
        tlkmw_anc_sch_request_update_stop(type, handle);
    }
}

/**
 * @brief       Initialize ANC scheduler
 * @param[in]   cfg - Configuration pointer
 * @return      None
 */
void tlkmw_anc_sch_init(const tlkmw_anc_sch_cfg_t *cfg)
{
    sTlkmwAncSch.cfg = cfg;
    tlkmw_anc_sch_bt_music_init();
    tlkmw_anc_sch_bt_voice_init();
    tlkmw_anc_sch_tpsll_init();
}

/**
 * @brief       Check if ANC scheduler is running
 * @param       None
 * @return      1 if running, 0 if not running
 */
_always_inline uint8_t tlkmw_anc_sch_isRunning(void)
{
    return sTlkmwAncSch.isRunning;
}

/**
 * @brief       Pause ANC scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_pause(void)
{
    if (sTlkmwAncSch.isRunning == 0) {
        return;
    }
    tlkmw_anc_sch_core(false);
    sTlkmwAncSch.isRunning = 0;
}

/**
 * @brief       Resume ANC scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_resume(void)
{
    if (sTlkmwAncSch.isRunning) {
        return;
    }
    sTlkmwAncSch.isRunning = 1;
    tlkmw_anc_sch_core(true);
}

/**
 * @brief       Enable or disable ANC function
 * @param[in]   enable - Enable flag (1 to enable, 0 to disable)
 * @return      None
 */
void tlkmw_anc_function_en(uint8_t enable)
{
    enable = enable ? 1 : 0;
    if (sTlkmwAncSch.enable == enable) {
        return;
    }
    sTlkmwAncSch.enable = enable;

    if (enable) {
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_ANC, 0XFFE0);
    } else {
        tlkmdi_audio_sendCloseEvtEx(TLKAUD_TYPE_ANC, 0XFFE0, 1);
    }
#if TLKSTK_BTTPSLL_TWS_ENABLE
    tlkmw_anc_twsSyncAncState();
#endif
}

/**
 * @brief       Check if ANC is enabled
 * @param       None
 * @return      1 if enabled, 0 if disabled
 */
uint8_t tlkmw_anc_is_enable(void)
{
    return sTlkmwAncSch.enable;
}


#endif