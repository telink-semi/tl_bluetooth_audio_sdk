/********************************************************************************************************
 * @file    bt_audio_sync.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "stack/tpsll/tpsll.h"

#if (TLKBTP_CFG_HFP_ENABLE && TLKBTP_CFG_A2DPSNK_ENABLE)
//extern volatile uint8_t g_timer1_irq_process_is_ongoing;
#if AUDIO_TWS_MODE

/**
 * @brief  update audio sync config
 * @param[in]  tick   - task tick
 * @param[in]  p_des  - config text
 * @param[in]  fno    - frame number
 * @returns BT_AUDIO_SUCCESS or BT_AUDIO_TYPE_INVALID
 */
_attribute_ram_code_sec_ uint8_t bt_audio_sync_update(uint32_t tick, uint8_t *p_des, uint16_t fno)
{
    uint8_t ret = BT_AUDIO_SUCCESS;

#if TWS_AUDIO_PATH_GPIO_DEBUG
//    gpio_write(GPIO_PA0, 0);
//    gpio_write(GPIO_PA0, 1);
//    gpio_write(GPIO_PA0, 0);
#endif

#if TONE_SYNC_PLAYBACK_MODE
#error "open TONE_SYNC_PLAYBACK_MODE plz check all code is in _attribute_ram_code_sec_ "
    uint16_t rptr;

    if (!(fno & 0x3f)) {
        if (tlkmdi_bt_tpt_isSlave() || tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
            if (!g_bt_audio_task_env.irq_en) {
                bt_audio_task_register_run_cb(NULL, 1);
                g_tick_playback_active = clock_time() | 1;

                if (!tone_is_playing() && !async_mode_bt_audio()) {
                    rptr                  = codec_get_speaker_rptr(buff_playback);
                    g_codec_cfg.play_wptr = (rptr + 150) & TWS_PLAY_FIFO_MAX;
                    codec_mute_playback_buffer();
                }
            } else {
                if (audio_ctl_cc_check_is_bt_mode() && !tph_bt_audio_is_phone() && g_timer1_irq_process_is_ongoing == 0) {
                    g_bt_audio_task_env.tick_irq = clock_time() + 1500 * TICK_PER_US;
                    tlkmdi_audio_task_set_next_irq(1500);

                    if (!tone_is_playing() && !async_mode_bt_audio()) {
                        rptr                  = codec_get_speaker_rptr(buff_playback);
                        g_codec_cfg.play_wptr = (rptr + 150) & TWS_PLAY_FIFO_MAX;
                        codec_mute_playback_buffer();
                    }
                }
            }
        }
    }
#endif

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "tick: %d, fno: %d ---",
                 tick, fno); //!(fno & 0x3f) &&
*/
    if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btmusic_is_busy()
#endif
    ) {
        bt_music_update_sync_cfg(tick, p_des, fno);
    } else if (tlkmdi_bt_voice_is_busy()
#if TLKALG_ANC_ENABLE
               || tlkmdi_anc_btvoice_is_busy()
#endif
    ) {
        bt_voice_update_sync_cfg(tick, p_des);
    } else {
        ret = BT_AUDIO_TYPE_INVALID;
    }

    return ret;
}

/**
 * @brief  check sync config difference between master and slave
 * @param[in]  p_master - master config
 * @param[in]  p_slave  - slave config
 * @param[in]  type     - AUDIO_TYPE_MUSIC AUDIO_TYPE_VOICE
 * @returns BT_AUDIO_SUCCESS or BT_AUDIO_TYPE_INVALID
 */
_attribute_ram_code_sec_noinline_ uint8_t bt_audio_check_sync_cfg(uint8_t *p_master, uint8_t *p_slave, uint8_t type)
{
    uint8_t ret = BT_AUDIO_SUCCESS;

    if (type == AUDIO_TYPE_MUSIC) {
        bt_music_check_sync_cfg(p_master, p_slave);
    } else if (type == AUDIO_TYPE_VOICE) {
        bt_voice_check_sync_cfg(p_master, p_slave);
    } else {
        ret = BT_AUDIO_TYPE_INVALID;
    }

    return ret;
}

#endif
#endif
