/********************************************************************************************************
 * @file    tlkmdi_tone.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/audio/tlkmw_audio.h"

#if AUDIO_TWS_MODE
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"
#endif

#include "tone_sbc.h"
#if (TLK_CFG_TONE_ENABLE)

#define TLKMDI_TONE_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_CC_LL_AUDIO << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_TONE_DBG_SIGN "[MDI TONE]"

#define TLKMDI_TONE_WAIT_SYNC_TIMEOUT_MS    (150)
#define TLKMDI_TONE_SYNC_DELAY_PLAY_TIME_MS (200)
#define TLKMDI_TONE_SYNC_DELAY_PLAY_TIME_TICK (TLKMDI_TONE_SYNC_DELAY_PLAY_TIME_MS * 1000 * SYSTEM_TIMER_TICK_1US)
typedef struct
{
    uint8_t  enable;
    uint8_t  isActive;
    uint8_t  tone_id;
    uint8_t  waitSyncTimer;
    uint16_t handle;
    uint16_t sample_rate;
    uint32_t syncTick;
} tlkmdi_tone_context_t;

static tlkmdi_tone_context_t s_tlk_mdi_tone_ctx = {0};
void tlkmdi_tone_main(void);

/**
 * @brief Callback function for tone synchronization timer.
 * @param[in] syncTick The synchronization tick value received from the timer.
 *                     - Type: uint32_t
 *                     - Unit: Microseconds
 * 
 * @return None.
 */
static void tlkmdi_tone_tmrSyncCB(uint32_t syncTick)
{
   if(s_tlk_mdi_tone_ctx.syncTick == 0){
       s_tlk_mdi_tone_ctx.syncTick = syncTick | 1;
   }
}

/**
 * @brief       Waits for synchronized play start.
 * @param[in] tone_id The tone ID to wait for synchronization.
 * @return None.
*/
static void tlkmdi_tone_waitSyncPlayStart(uint8_t tone_id)
{
    //need sync play: wait sync tick then play
    s_tlk_mdi_tone_ctx.syncTick = 0;
    int ret = tlkmdi_audio_hostif_tone_requestSyncPlay(tone_id);
    if(ret == TLK_ENONE){
        s_tlk_mdi_tone_ctx.waitSyncTimer = TLKMDI_TONE_WAIT_SYNC_TIMEOUT_MS / 10;
    } else{
        s_tlk_mdi_tone_ctx.syncTick = 0;
    } 
}

/**
 * @brief Waits for synchronized playback.
 * @param None.
 * @return bool - True if still waiting or playback should be delayed, false if
 *                timeout occurred or no synchronization is needed.
 */
static bool tlkmdi_tone_waitSyncPlay(void)
{
    if(s_tlk_mdi_tone_ctx.waitSyncTimer == 0){
        s_tlk_mdi_tone_ctx.syncTick = 0;
        return false;
    }
    if(s_tlk_mdi_tone_ctx.syncTick){
        s_tlk_mdi_tone_ctx.waitSyncTimer = 0;
        uint32_t delayTick = s_tlk_mdi_tone_ctx.syncTick + TLKMDI_TONE_SYNC_DELAY_PLAY_TIME_TICK - clock_time();
        uint32_t delayUs = delayTick / SYSTEM_TIMER_TICK_1US;
        if(delayUs > TLKMDI_TONE_SYNC_DELAY_PLAY_TIME_MS * 1000){
            delayUs = 100;
        }
        tlkmdi_audio_task_set_next_irq(delayUs);
        s_tlk_mdi_tone_ctx.syncTick = 0;
        return true;
    }
    s_tlk_mdi_tone_ctx.waitSyncTimer--; 
    tlkmdi_audio_task_set_next_irq(10 * 1000);
    return true;
}

/**
 * @brief       Initializes the tone module.
 * @return      int - Status of initialization (0 for success, non-zero for failure).
 */
int tlkmdi_tone_init(void)
{
    tmemset(&s_tlk_mdi_tone_ctx, 0, sizeof(tlkmdi_tone_context_t));
    tone_sbc_init();
    if(tlkmdi_audio_hostif_tone_isNeedSyncPlay()){
        tlkmdi_audio_hostif_tone_regSyncCB(tlkmdi_tone_tmrSyncCB);
    }
    return TLK_ENONE;
}

/**
 * @brief       Starts the tone with the given parameters.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   param - Parameters for starting the tone.
 * @return      int - Status of the operation (0 for success, non-zero for failure).
 */
int tlkmdi_tone_start(uint16_t handle, uint32_t param)
{
    uint8_t tone_indx            = (param & 0xffff);
    uint8_t  isActive             = (param & 0xff00) >> 8;
    uint16_t  isNeedCreateToneTask = param >> 16;
    if(tlkmdi_audio_hostif_tone_isNeedRefuse(tone_indx)){
        tlkapi_trace(TLKMDI_TONE_DBG_FLAG, TLKMDI_TONE_DBG_SIGN, "Refuse play because not in ear");
        return -TLK_ENOSUPPORT;
    }
    if(isActive && tlkmdi_audio_hostif_tone_isSupportActivePlay(tone_indx) == false){
        return -TLK_ENOSUPPORT;
    }
    if (isNeedCreateToneTask) {
        s_tlk_mdi_tone_ctx.isActive = isActive;
        s_tlk_mdi_tone_ctx.tone_id  = tone_indx;
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_TONE, handle);
    }else{
        if(isActive && tlkmdi_audio_hostif_tone_isNeedSyncPlay()){  
            tlkmdi_audio_hostif_tone_requestSyncPlay(tone_indx);
        }
    }
    tlkapi_trace(TLKMDI_TONE_DBG_FLAG, TLKMDI_TONE_DBG_SIGN, "tlkmdi_tone_play:%d", tone_indx);
    tone_play(tone_indx);
    return TLK_ENONE;
}

/**
 * @brief   main loop for tone play
 * @param   none
 * @return  none
 */
void tlkmdi_tone_main_loop(void)
{
    if (tlkmdi_tone_is_busy()) {
        return;
    }
    tlkmdi_audio_sendCloseEvtEx(TLKAUD_TYPE_TONE, 0xffff, true);
}

/**
 * @brief       Switches the tone state.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   status - The desired status to switch to.
 * @return      bool - True if the switch was successful, false otherwise.
 */
bool tlkmdi_tone_switch(uint16_t handle, uint8_t status)
{
    tlkapi_trace(TLKMDI_TONE_DBG_FLAG, TLKMDI_TONE_DBG_SIGN, "tlkmdi_tone_switch: handle:%d, status: %d,tone:%d", handle, status,s_tlk_mdi_tone_ctx.handle);

    if (status == TLK_STATE_CLOSED && s_tlk_mdi_tone_ctx.handle != handle) {
        return false;
    }

    if (status == TLK_STATE_OPENED) {
        if (s_tlk_mdi_tone_ctx.enable == true) {
            return true;
        }
        s_tlk_mdi_tone_ctx.handle = handle;
        s_tlk_mdi_tone_ctx.enable = true;
    #if (MCU_CORE_TYPE == MCU_CORE_B92)
        #if (TLKBTP_CFG_HFP_ENABLE || TLKBTP_CFG_A2DPSNK_ENABLE)
        s_tlk_mdi_tone_ctx.sample_rate = g_codec_cfg.sample_rate;
        #else
        s_tlk_mdi_tone_ctx.sample_rate = 48000;
        #endif
    #else
        s_tlk_mdi_tone_ctx.sample_rate = 48000;
    #endif

        if(s_tlk_mdi_tone_ctx.isActive && tlkmdi_audio_hostif_tone_isNeedSyncPlay()){  
            tlkmdi_tone_waitSyncPlayStart(s_tlk_mdi_tone_ctx.tone_id);
        }
        if(!s_tlk_mdi_tone_ctx.isActive){
            s_tlk_mdi_tone_ctx.waitSyncTimer = TLKMDI_TONE_WAIT_SYNC_TIMEOUT_MS / 10;
        }

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN,tlkmdi_tone_main_loop);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER,tlkmdi_tone_main);
        tlkmdi_audio_setup_and_start_timer();

    #if AUDIO_PATH_24BITS_EN
        #if CODEC_DAC_MONO_MODE
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_24, s_tlk_mdi_tone_ctx.sample_rate,0);
        #else
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, s_tlk_mdi_tone_ctx.sample_rate,0);
        #endif
    #else
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, s_tlk_mdi_tone_ctx.sample_rate,0);
    #endif

        tlkdrv_codec_sync_play_samples(512);
    } else {
        tmemset(&s_tlk_mdi_tone_ctx, 0 ,sizeof(s_tlk_mdi_tone_ctx));
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
        //bt_audio_task_register_run_cb(tlkmdi_tone_is_busy, 0);
        tlkmdi_audio_stop_timer();
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER,NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN,NULL);
    }

    return true;
}

/**
 * @brief       Checks if the tone module is busy.
 * @return      bool - True if the tone module is busy, false otherwise.
 */
bool tlkmdi_tone_is_busy(void)
{
    return tone_is_playing() || s_tlk_mdi_tone_ctx.waitSyncTimer;
}

/**
 * @brief       Performs an operation on the tone.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   opcode - The operation code.
 * @param[in]   pdata - Pointer to the data for the operation.
 * @param[in]   dataLen - Length of the data.
 * @return      bool - True if the operation was successful, false otherwise.
 */
bool tlkmdi_tone_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
        case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
        {
            return true;
        }
        default:
        {
            return false;
        } break;
    }
    return true;
}

/**
 * @brief Plays tone data through the audio codec.
 * @param None.
 * @return None.
 */
static void tlkmdi_tone_player(void)
{
    uint16_t samples_num = 128;
    uint16_t codec_buffer_avail_size;
    uint16_t i;

    /* stereo mode */
    int16_t pcm16_tone[128 * 2];

    #if AUDIO_PATH_24BITS_EN
    /* tone pcm stereo is pcm32 */
    int32_t pcm32_stereo[128 * 2];
    #else
    int32_t pcm16_stereo[128];
    #endif

    codec_buffer_avail_size = tlkdrv_codec_get_spk_buf_idle_size();

    if (codec_buffer_avail_size > CODEC_PLAY_FIFO_SIZE / 2) {
        /* tone mix, stereo mode */
        tmemset(pcm16_tone, 0, sizeof(pcm16_tone));
    #if AUDIO_PATH_24BITS_EN
        tmemset(pcm32_stereo, 0, sizeof(pcm32_stereo));
    #else
        tmemset(pcm16_stereo, 0, sizeof(pcm16_stereo));
    #endif

        if (tone_is_playing()) {
            tone_get_sample(pcm16_tone, samples_num * sizeof(tone_int), s_tlk_mdi_tone_ctx.sample_rate);

    #if AUDIO_PATH_24BITS_EN
            if (audio_flag_get(FLG_BTC_TWS_EN)) {
                for (i = 0; i < samples_num; i++) {
                    pcm32_stereo[i] = (int32_t)pcm16_tone[i] << 8;
                }
            } else {
                for (i = 0; i < samples_num; i++) {
                    int32_t left_tone  = (int32_t)pcm16_tone[2 * i] << 8;
                    int32_t right_tone = (int32_t)pcm16_tone[2 * i + 1] << 8;

                    pcm32_stereo[2 * i]     = left_tone;
                    pcm32_stereo[2 * i + 1] = right_tone;
                }
            }

    #else
            for (i = 0; i < samples_num; i++) {
                pcm16_stereo[i] = (uint16_t)pcm16_tone[2 * i] | (pcm16_tone[2 * i + 1] << 16);
            }
    #endif
        }

    #if AUDIO_PATH_24BITS_EN
        if (audio_flag_get(FLG_BTC_TWS_EN)) {
            tlkdrv_codec_fillSpkBuff((uint8_t *)pcm32_stereo, samples_num * 4);
        } else {
            tlkdrv_codec_fillSpkBuff((uint8_t *)pcm32_stereo, samples_num * 4 * 2);
        }
    #else
        tlkdrv_codec_fillSpkBuff((uint8_t *)pcm16_stereo, samples_num * sizeof(codec_int));
    #endif
    }
}

/**
 * @brief       This function serves as the main entry point for tone handling.
 *              It checks if synchronization is required for tone playback and triggers the tone player accordingly.
 * @param[in]   none.
 * @return      none.
 * @note        Calls tlkmdi_tone_waitSyncPlay to determine whether to wait or proceed with tone playback.
 */
void tlkmdi_tone_main(void)
{
    bool isWait = tlkmdi_tone_waitSyncPlay();
    if(isWait){
        return;
    }
    tlkmdi_audio_task_set_next_irq(1500);
    tlkmdi_tone_player();
}

#endif
