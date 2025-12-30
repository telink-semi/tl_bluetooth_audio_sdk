/********************************************************************************************************
 * @file    tlkmdi_auduac_music.c
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
#if (TLK_USB_UAC_PLAYBACK_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"

#define TLKAUD_UAC_PLAY_SIN_TEST_ENABLE 0 // enable sin test

/**
 * @brief  Speaker handler for UAC music module.
 * @param  None.
 * @return None.
 */
static void tlkmdi_uac_music_spkHandler(void);

/**
 * @brief  Microphone handler for UAC music module.
 * @param  None.
 * @return None.
 */
static void tlkmdi_uac_music_micHandler(void);

typedef struct
{
    uint8_t       enable;
    uint8_t       spkEn;
    uint8_t       micEn;
    uint8_t       startflag;
    uint16_t      handle;
    uint16_t      readFrame;
    uint32_t      tick_rem;
    uint32_t      last_tick;
    TlkApiTimer_t uac_notify_timer;
} tlkmdi_auduac_music_ctrl_t;

static tlkmdi_auduac_music_ctrl_t sTlkMdiAudUacMusicCtrl = {0};

#if TLKLIB_UAC_PPM_SPK_ENABLE
static uint8_t *spTlkMdiUacMusicPpmSpkBuff = NULL;
#endif

#if (TLKLIB_UAC_PPM_MIC_ENABLE)
static uint8_t *spTlkMdiUacVoicePpmMicBuff = NULL;
#endif

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
typedef int64_t uac_codec_int;
#else
typedef int32_t uac_codec_int;
#endif

/**
 * @brief  Initializes the audio UAC music module.
 * @param  None.
 * @return int - Returns 0 if initialization is successful; otherwise an error code.
 */
int tlkmdi_auduac_music_init(void)
{
    memset(&sTlkMdiAudUacMusicCtrl, 0, sizeof(tlkmdi_auduac_music_ctrl_t));

    return TLK_ENONE;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
uint tlkmdi_auduac_music_intval(void)
{
    return 5000;
}

/**
 * @brief  This function is used to handle the audio UAC music module interrupt.
 * @param  None.
 * @return None.
 */
static void tlkmdi_auduac_music_irqProc(void)
{
    uint32_t intv = tlkmdi_auduac_music_intval();

    if (intv == 0) {
        intv = 3000;
    } else if (intv < 500) {
        intv = 500;
    } else if (intv > 1000000) {
        intv = 1000000;
    }

    tlkmdi_audio_task_set_next_irq(intv);

    uint32_t refTime = clock_time() | 1;
    uint32_t diff_tick;

    if (sTlkMdiAudUacMusicCtrl.last_tick == 0) {
        sTlkMdiAudUacMusicCtrl.readFrame = 5;
    } else {
        diff_tick                        = refTime - sTlkMdiAudUacMusicCtrl.last_tick + sTlkMdiAudUacMusicCtrl.tick_rem;
        sTlkMdiAudUacMusicCtrl.readFrame = (diff_tick / (SYSTEM_TIMER_TICK_1US * 1000));
        sTlkMdiAudUacMusicCtrl.tick_rem  = diff_tick % (SYSTEM_TIMER_TICK_1US * 1000);
    }

    sTlkMdiAudUacMusicCtrl.last_tick = refTime;

    if (sTlkMdiAudUacMusicCtrl.enable) {
        if (sTlkMdiAudUacMusicCtrl.spkEn) {
            tlkmdi_uac_music_spkHandler();
        }
        if (sTlkMdiAudUacMusicCtrl.micEn) {
            tlkmdi_uac_music_micHandler();
        }
    }
}

/**
 * @brief       This function is used to handle tle algorithm resources.
 * @param[in]   isMicEn - Indicates whether the mic is enabled.
 * @param[in]   isSpkEn - Indicates whether the speaker is enabled.
 * @return      bool - Returns true if the initialization is successful; otherwise false.
 */
static bool tlkmdi_auduac_local_init_alg(bool isMicEn, bool isSpkEn)
{
#if (TLKLIB_UAC_PPM_MIC_ENABLE || TLKLIB_UAC_PPM_SPK_ENABLE)
    audio_alg_interface_t *p_audio_alg_if;

    if (isSpkEn) {
#if TLKLIB_UAC_PPM_SPK_ENABLE
        if (spTlkMdiUacMusicPpmSpkBuff == NULL) {
            p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_PPM_SPK);
            uint16_t ppm_spk_size      = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            spTlkMdiUacMusicPpmSpkBuff = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
            if (spTlkMdiUacMusicPpmSpkBuff == NULL) {
                tlk_printf("tlkmdi_auduac_local_init_alg ppmSpkBuffer malloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(spTlkMdiUacMusicPpmSpkBuff, ALG_CHANNEL_STEREO);
        }
#endif
    } else {
#if TLKLIB_UAC_PPM_SPK_ENABLE
        if (spTlkMdiUacMusicPpmSpkBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
            tlkalg_free_func(spTlkMdiUacMusicPpmSpkBuff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiUacMusicPpmSpkBuff = NULL;
        }
#endif
    }

    if (isMicEn) {
#if TLKLIB_UAC_PPM_MIC_ENABLE
        if (spTlkMdiUacVoicePpmMicBuff == NULL) {
            p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_PPM_MIC);
            uint16_t ppm_mic_size      = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
            spTlkMdiUacVoicePpmMicBuff = (uint8_t *)tlkalg_malloc_func(ppm_mic_size);
            if (spTlkMdiUacVoicePpmMicBuff == NULL) {
                tlk_printf("tlkmdi_auduac_local_init_alg ppmMicBuffer malloc failed");
                return false;
            }
            p_audio_alg_if->audio_alg_init(spTlkMdiUacVoicePpmMicBuff, ALG_CHANNEL_LEFT);
        }
#endif
    } else {
#if TLKLIB_UAC_PPM_MIC_ENABLE
        if (spTlkMdiUacVoicePpmMicBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
            tlkalg_free_func(spTlkMdiUacVoicePpmMicBuff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiUacVoicePpmMicBuff = NULL;
        }
#endif
    }

    if (isSpkEn || isMicEn) {
        tlkalg_u2s_ppm_clear_status();
        tlkalg_u2s_ppm_set_val(0);
        if (isMicEn) {
            tlkusb_uac_set_state(TLKUSB_UAC_VOICE);
        } else {
            tlkusb_uac_set_state(TLKUSB_UAC_MUSIC);
        }
    } else {
        if (sTlkMdiAudUacMusicCtrl.micEn) {
            tlkusb_uacmic_ppm_stop();
        }
        if (sTlkMdiAudUacMusicCtrl.spkEn) {
            tlkusb_uacspk_ppm_stop();
        }
        tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
    }
#else
    (void)isMicEn;
    (void)isSpkEn;
#endif

    return true;
}

/**
 * @brief     Operates the UAC music module based on the provided opcode.
 * @param[in] handle - The connection handle.
 * @param[in] opcode - The operation code.
 * @param[in] pData  - Pointer to the data buffer.
 * @param[in] dataLen - Length of the data.
 * @return    bool - Returns true if the operation is successful; otherwise false.
 */
bool tlkmdi_auduac_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    (void)handle;
    tlk_printf("tlkmdi_auduac_music_operate: opcode[%d] dataLen[%d]", opcode, dataLen);
    if (opcode >= TLKAUD_OPCODE_MAX) {
        return false;
    }

    switch (opcode) {
    case TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE:
    {
        bool micEn = tlkusb_uac_get_iso_in_enable();
        bool spkEn = tlkusb_uac_get_iso_out_enable();

        tlk_printf("tlkmdi_auduac_music_operate switch: opcode[%d] cur: mic[%d], spk[%d], new: mic[%d], spk[%d]", opcode, sTlkMdiAudUacMusicCtrl.micEn,
                   sTlkMdiAudUacMusicCtrl.spkEn, micEn, spkEn);

        /*Double check mic and spk status.*/
        if (micEn == sTlkMdiAudUacMusicCtrl.micEn && spkEn == sTlkMdiAudUacMusicCtrl.spkEn) {
            return false;
        }
        if (tlkmdi_auduac_local_init_alg(micEn, spkEn) == false) {
            return false;
        }

        if (spkEn && !sTlkMdiAudUacMusicCtrl.spkEn) {
            uint32_t spkSampleRate;
            spkSampleRate       = tlkusb_uac_get_iso_out_SampleRate();
            uint8_t spkBitDepth = tlkusb_uac_get_iso_out_BitDepth();

            if (spkSampleRate == 0) {
                spkSampleRate = TLKDRV_CODEC_SAMPLERATE_48000;
            }

            tlkapi_trace(0xFFFFFFFF, "[TEST]", "operate: spkSamp[%d]", spkSampleRate);

            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, spkBitDepth, spkSampleRate, 0);
        }

        if (micEn && !sTlkMdiAudUacMusicCtrl.micEn) {
            uint32_t micSampleRate;
            micSampleRate       = tlkusb_uac_get_iso_in_SampleRate();
            uint8_t micBitDepth = tlkusb_uac_get_iso_in_BitDepth();


            if (micSampleRate == 0) {
                micSampleRate = TLKDRV_CODEC_SAMPLERATE_16000;
            }

            tlkapi_trace(0xFFFFFFFF, "[TEST]", "operate: micSamp[%d]", micSampleRate);

            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, micBitDepth, micSampleRate, 0);
        }

        sTlkMdiAudUacMusicCtrl.spkEn = spkEn;
        sTlkMdiAudUacMusicCtrl.micEn = micEn;
    } break;
    case TLKAUD_OPCODE_TYPE_GET_TYPE:
    {
    } break;
    default:
        return false;
    }
    return true;
}

/**
 * @brief     Notifies the UAC module about the state change.
 * @param[in] pTimer - The timer handle.
 * @param[in] userArg - The user argument.
 * @return    none
*/
static void tlkmdi_auduac_uac_notify_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;

    bool enable = userArg;
    tlk_printf("tlkmdi_auduac_uac_notify_timer enable[%d]", enable);
    if (enable) {
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            if (sTlkMdiAudUacMusicCtrl.micEn) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_START_RSP);
            } else {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_START_RSP);
            }
        }
    } else {
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            if (sTlkMdiAudUacMusicCtrl.micEn) {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_STOP_RSP);
            } else {
                sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_STOP_RSP);
            }
        }
    }

    tlksys_timer_stop(TLKSYS_TASKID_AUDIO, &sTlkMdiAudUacMusicCtrl.uac_notify_timer);
}

/**
 * @brief     Switches the state of the UAC music module.
 * @param[in] handle - The connection handle.
 * @param[in] status - The target status (e.g., TLK_STATE_OPENED or TLK_STATE_CLOSED).
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_auduac_music_switch(uint16_t handle, uint8_t status)
{
    bool enable;

    if (status == TLK_STATE_OPENED) {
        enable = true;
    } else {
        enable = false;
    }

    sTlkMdiAudUacMusicCtrl.handle    = handle;
    sTlkMdiAudUacMusicCtrl.spkEn     = tlkusb_uac_get_iso_out_enable();
    sTlkMdiAudUacMusicCtrl.micEn     = tlkusb_uac_get_iso_in_enable();
    sTlkMdiAudUacMusicCtrl.enable    = enable;
    sTlkMdiAudUacMusicCtrl.startflag = 0;
    sTlkMdiAudUacMusicCtrl.tick_rem  = 0;
    sTlkMdiAudUacMusicCtrl.last_tick = 0;

    if (tlkmdi_auduac_local_init_alg(sTlkMdiAudUacMusicCtrl.micEn, sTlkMdiAudUacMusicCtrl.spkEn) == false) {
        return false;
    }

    if (enable) {
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, TLKUSB_AUD_SPK_SAMPLE_RATE_DEF, 0);
#else
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, TLKUSB_AUD_SPK_SAMPLE_RATE_DEF, 0);
#endif
        if (sTlkMdiAudUacMusicCtrl.spkEn) {
            uint32_t spkSampleRate;
            spkSampleRate       = tlkusb_uac_get_iso_out_SampleRate();
            uint8_t spkBitDepth = tlkusb_uac_get_iso_out_BitDepth();

            if (spkSampleRate == 0) {
                spkSampleRate = TLKDRV_CODEC_SAMPLERATE_48000;
            }
            tlk_printf("tlkmdi_auduac_music_switch spkSamplerate:%d", spkSampleRate);
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, spkBitDepth, spkSampleRate, 0);
        }

        if (sTlkMdiAudUacMusicCtrl.micEn) {
            uint32_t micSampleRate;
            micSampleRate       = tlkusb_uac_get_iso_in_SampleRate();
            uint8_t micBitDepth = tlkusb_uac_get_iso_in_BitDepth();

            if (micSampleRate == 0) {
                micSampleRate = TLKDRV_CODEC_SAMPLERATE_16000;
            }
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, micBitDepth, micSampleRate, 0);
        }

        // uint send_data1[3];
        // send_data1[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
        // send_data1[1] = tlkdrv_codec_get_speaker_rptr();
        // send_data1[2] = tlkdrv_codec_get_spk_avail_samples();
        // spi_master_write_dma(0, (uint8_t *)send_data1, 12);
        // while(spi_is_busy(0));

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_auduac_music_irqProc);
        tlkmdi_audio_setup_and_start_timer();

        tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkMdiAudUacMusicCtrl.uac_notify_timer, 100000, false, tlkmdi_auduac_uac_notify_timer, (void *)(uint32_t)enable);
        tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &sTlkMdiAudUacMusicCtrl.uac_notify_timer);
    } else {
        tlkmdi_audio_stop_timer();

        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);

        tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkMdiAudUacMusicCtrl.uac_notify_timer, 100000, false, tlkmdi_auduac_uac_notify_timer, (void *)(uint32_t)enable);
        tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &sTlkMdiAudUacMusicCtrl.uac_notify_timer);
    }

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkmdi_auduac_music_switch: %d %d", handle, status);

    return true;
}

/**
 * @brief  Checks if the UAC music module is busy.
 * @param  None.
 * @return bool - Returns true if the module is busy; otherwise false.
 */
bool tlkmdi_auduac_music_isBusy(void)
{
    return sTlkMdiAudUacMusicCtrl.enable;
}

/**
 * @brief  Speaker handler for UAC music module.
 * @param  None.
 * @return None.
 */
static void tlkmdi_uac_music_spkHandler(void)
{
    uint16_t length;
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
    int32_t buffer[2048];
#else
    int16_t buffer[2048];
#endif
    // DBG_YATING_CHN1_HIGH;

    uint16_t samples_per_ms = tlkusb_uac_get_iso_out_SampleRate() / 1000;
    uint16_t bytes_per_ms   = samples_per_ms * sizeof(uac_codec_int);

    if (sTlkMdiAudUacMusicCtrl.readFrame == 6) {}

#if (TLK_DEV_CODEC_ENABLE)

    tlkusb_ppm_process(sTlkMdiAudUacMusicCtrl.last_tick);

    uint16_t used_samples = tlkusb_uac_get_ppm_out_used_samples();

    if (used_samples >= (TLKUSB_AUD_SPK_SAMPLES * 15) && !sTlkMdiAudUacMusicCtrl.startflag) {
        sTlkMdiAudUacMusicCtrl.startflag = 1;
        tlkdrv_codec_sync_speaker_samples(CODEC_SPK_FIFO_SAMPLES / 2);
    }

    if (!sTlkMdiAudUacMusicCtrl.startflag) {
        // DBG_YATING_CHN1_LOW;
        return;
    }

    length = 0;

    if (tlkdrv_codec_getSpkIdleLen() > (bytes_per_ms * sTlkMdiAudUacMusicCtrl.readFrame)) {
        length = tlkusb_uac_read_ppm_out_samples(buffer, samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudUacMusicCtrl.readFrame);
    } else {
        // DBG_YATING_CHN4_HIGH;
        // DBG_YATING_CHN4_LOW;

        // uint send_data1[3];
        // send_data1[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
        // send_data1[1] = tlkdrv_codec_get_speaker_rptr();
        // send_data1[2] = tlkdrv_codec_get_spk_avail_samples();
        // spi_master_write_dma(0, (uint8_t *)send_data1, 12);
        // while(spi_is_busy(0));
    }

    if (length == 0 || (length != samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudUacMusicCtrl.readFrame)) {
        // DBG_YATING_CHN1_LOW;
        return;
    }

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 16)
    int16_t *pData        = (int16_t *)buffer;
    uint     audio_volume = tlkusb_uacspk_getVolume();
    for (uint index = 0; index < length; index++) {
        pData[index] = (pData[index] * audio_volume + 0x2000) >> 14;
    }
#endif
    // DBG_YATING_CHN5_HIGH;
    // DBG_YATING_CHN5_LOW;

    tlkdrv_codec_fillSpkBuff((uint8_t *)buffer, bytes_per_ms * sTlkMdiAudUacMusicCtrl.readFrame);
// tlkdrv_codec_fillSpkBuff((uint8_t*)sin96k_24bit, bytes_per_ms * sTlkMdiAudUacMusicCtrl.readFrame);
// DBG_YATING_CHN1_LOW;
#endif
}

#if (TLKAUD_UAC_PLAY_SIN_TEST_ENABLE)
static signed short sin_16k[] __attribute__((aligned(4))) = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539,
};

static signed short sin_48k_mono[] __attribute__((aligned(4))) = {
    0, 1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,  7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,  4096,  3134,  2120,  1069,
    0, -1069, -2120, -3134, -4095, -4986, -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568, -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069,
};

static int32_t sin96k_24bit[] = {
    0,        0,        1094933,  1094933,  2171131,  2171131,  3210181,  3210181,  4194303,  4194303,  5106661,  5106661,  5931641,  5931641,  6655129,  6655129,
    7264747,  7264747,  7750062,  7750062,  8102772,  8102772,  8316841,  8316841,  8388607,  8388607,  8316841,  8316841,  8102772,  8102772,  7750062,  7750062,
    7264746,  7264746,  6655129,  6655129,  5931641,  5931641,  5106660,  5106660,  4194302,  4194302,  3210179,  3210179,  2171130,  2171130,  1094934,  1094934,
    0,        0,        -1094933, -1094933, -2171131, -2171131, -3210182, -3210182, -4194305, -4194305, -5106660, -5106660, -5931640, -5931640, -6655128, -6655128,
    -7264747, -7264747, -7750063, -7750063, -8102772, -8102772, -8316841, -8316841, -8388607, -8388607, -8316841, -8316841, -8102772, -8102772, -7750061, -7750061,
    -7264745, -7264745, -6655128, -6655128, -5931639, -5931639, -5106660, -5106660, -4194301, -4194301, -3210180, -3210180, -2171133, -2171133, -1094929, -1094929,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
    0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,
};

static uint32_t AAAA_length = 0;
#endif

/**
 * @brief  UAC music module mic interrupt handler.
 * @param  None.
 * @return None.
 */
static void tlkmdi_uac_music_micHandler(void)
{
    uint16_t bytes_per_ms   = 0;
    uint16_t samples_per_ms = 0;
    uint8_t  buffer[1024];
    uint32_t micSampleRate = tlkusb_uac_get_iso_in_SampleRate();
    uint8_t  micBitDepth   = tlkusb_uac_get_iso_in_BitDepth();
    uint8_t  micChannel    = tlkusb_uac_get_iso_in_Channels();

    samples_per_ms = micSampleRate / 1000;
    bytes_per_ms   = samples_per_ms * micChannel * micBitDepth / 8;

    uint16_t sample_num = samples_per_ms * sTlkMdiAudUacMusicCtrl.readFrame;
    uint16_t bytes_num  = bytes_per_ms * sTlkMdiAudUacMusicCtrl.readFrame;

    if (tlkdrv_codec_readMicData(buffer, bytes_num, 0) == false) {
    } else {
    }

/*sin test*/
#if (TLKAUD_UAC_PLAY_SIN_TEST_ENABLE)
    uint8_t *pBuffer = NULL;

    if (micSampleRate == TLKDRV_CODEC_SAMPLERATE_16000) {
        pBuffer = (uint8_t *)sin_16k;
    } else {
        pBuffer = (uint8_t *)sin_48k_mono;
    }

    pBuffer = (uint8_t *)sin_48k_mono;

    for (int i = 0; i < bytes_num; i++) {
        buffer[i] = pBuffer[AAAA_length++];
        if (AAAA_length == bytes_per_ms) {
            AAAA_length = 0;
        }
    }

    if (pBuffer == NULL) {
        return;
    }
#endif

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
    if (tlkusb_uac_write_iso_in_samples((int32_t *)buffer, sample_num) == 0) {
#else
    if (tlkusb_uac_write_iso_in_samples((int16_t *)buffer, sample_num) == 0) {
#endif

    } else {
    }

    tlkusb_uacmic_ppm_process(sTlkMdiAudUacMusicCtrl.last_tick);
}

#endif //(TLK_USB_UAC_PLAYBACK_ENABLE)
