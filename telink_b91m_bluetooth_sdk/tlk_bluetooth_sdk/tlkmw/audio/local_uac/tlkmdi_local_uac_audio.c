/********************************************************************************************************
 * @file    tlkmdi_local_uac_audio.c
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
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkmdi_local_uac_audio.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlklib/usb/tlkusb_hal.h"
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"
#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif


#define TLKMDI_LOC_UAC_SIGNAL           "[L_UAC]"
#define TLKMDI_LOC_UAC_FLAG             0xFFFFFFFF

#define TLKAUD_UAC_PLAY_SIN_TEST_ENABLE 0 // enable sin test
#define TLKAUD_UAC_IRQ_INTERVAL_US      1000
#define CODEC_SYNC_SAMPLE               48
#define CODEC_MAX_SAMPLES               (CODEC_SYNC_SAMPLE + 12)
#define CODEC_MIN_SAMPLES               (CODEC_SYNC_SAMPLE - 12)

// uint16_t mic_sample_num = 0;
// uint16_t iso_in_sample = 0;

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
} tlkmdi_local_uac_audio_ctrl_t;

static tlkmdi_local_uac_audio_ctrl_t sTlkMdiAudLocalUacAudCtrl = {0};

// #if (TLKLIB_UAC_PPM_MIC_ENABLE)
// static uint8_t *spTlkMdiLocalUacPpmMicBuff = NULL;
// #endif

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
typedef int64_t loacl_uac_codec_int;
#else
typedef int32_t loacl_uac_codec_int;
#endif

/**
 * @brief  Initializes the audio UAC music module.
 * @param  None.
 * @return int - Returns 0 if initialization is successful; otherwise an error code.
 */
int tlkmdi_local_uac_audio_init(void)
{
    memset(&sTlkMdiAudLocalUacAudCtrl, 0, sizeof(tlkmdi_local_uac_audio_ctrl_t));

    return TLK_ENONE;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
uint tlkmdi_local_uac_audio_intval(void)
{
    return 5000;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
int tlkmdi_local_uac_audio_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;

    return TLK_ENONE;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
int tlkmdi_local_uac_audio_close(uint16_t handle)
{
    (void)handle;

    return TLK_ENONE;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_next(void)
{
    return false;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_prev(void)
{
    return false;
}

void tlkmdi_local_uac_check_out_sample(unsigned int sync_sample)
{
    unsigned int t = 0;

    unsigned int r = core_interrupt_disable();

    if (tlkusb_uac_get_iso_out_en()) {
        t = stimer_get_tick() - g_tlk_usb_cfg.tick_out;
        t = t * 48 / (1000 * SYSTEM_TIMER_TICK_1US);
        t = t > 48 ? 48 : t;
        t *= tlkusb_uac_get_iso_out_Channels();
        int num = ((g_tlk_usb_cfg.out_w - g_tlk_usb_cfg.out_r) & APP_USB_ISO_IN_BUFF_IDX_MASK) + t; //

        int ndiff = num - sync_sample;

        if (abs_ram(ndiff) > 96) {
            tlkapi_trace(0xffffffff, "<test>", "out sample reset %d %d", num, sync_sample);
            g_tlk_usb_cfg.out_r = g_tlk_usb_cfg.out_w - sync_sample + t; //
            g_tlk_usb_cfg.out_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
            for (int i = 0; i < APP_USB_ISO_OUT_BUFF_SIZE; i++) {
                g_tlk_usb_cfg.iso_out[i] = 0;
            }
            tlkusb_hal_wakeup_usb_thread();
        }
    }

    if (tlkusb_uac_get_iso_out1_en()) {
        t = stimer_get_tick() - g_tlk_usb_cfg.tick_out1;
        t = t * 48 / (1000 * SYSTEM_TIMER_TICK_1US);
        t = t > 48 ? 48 : t;
        t *= tlkusb_uac_get_iso_out_Channels();

        int num = ((g_tlk_usb_cfg.out1_w - g_tlk_usb_cfg.out1_r) & APP_USB_ISO_IN_BUFF_IDX_MASK) + t; //

        int ndiff = num - sync_sample;

        if (abs_ram(ndiff) > 96) {
            tlkapi_sendU32s(APP_LOG_EN, "out1 sample reset", g_tlk_usb_cfg.out1_r | g_tlk_usb_cfg.out1_w << 16, ndiff, g_tlk_usb_cfg.tick_out1, clock_time());
            g_tlk_usb_cfg.out1_r = g_tlk_usb_cfg.out1_w - sync_sample + t;
            g_tlk_usb_cfg.out1_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
            for (int i = 0; i < APP_USB_ISO_OUT_BUFF_SIZE; i++) {
                g_tlk_usb_cfg.iso_out1[i] = 0;
            }
            tlkusb_hal_wakeup_usb_thread();
        }
    }
    core_restore_interrupt(r);
}

void tlkmdi_local_uac_check_in_sample(unsigned int sync_num)
{
    unsigned int t = 0;

    if (!g_tlk_usb_cfg.tick_in) {
        return;
    }

    unsigned int r = core_interrupt_disable();

    t = stimer_get_tick() - g_tlk_usb_cfg.tick_in;
    t = t * 16 / (1000 * SYSTEM_TIMER_TICK_1US);
    t = t > 16 ? 16 : t;
    t *= tlkusb_uac_get_iso_in_Channels();

    int num = ((g_tlk_usb_cfg.in_w - g_tlk_usb_cfg.in_r) & APP_USB_ISO_IN_BUFF_IDX_MASK) - t; //

    int ndiff = num - sync_num;

    if (abs_ram(ndiff) > 96) {
        tlkapi_trace(0xffffffff, "<test>", "usb in buffer reset %d %d %d %d %d", num, ndiff, g_tlk_usb_cfg.in_w, g_tlk_usb_cfg.in_r, t);
        tlkusb_hal_wakeup_usb_thread();
        g_tlk_usb_cfg.in_w = g_tlk_usb_cfg.in_r + sync_num + t; //
        g_tlk_usb_cfg.in_w &= APP_USB_ISO_IN_BUFF_IDX_MASK;
    }
    core_restore_interrupt(r);
}

void tlkmdi_local_uac_audio_check_sample(void)
{
    uint16_t samples_per_ms = tlkusb_uac_get_iso_out_SampleRate() / 1000;

    uint32_t avail_in_fifo = tlkdrv_codec_get_spk_avail_samples();
    if ((avail_in_fifo > CODEC_MAX_SAMPLES) || (avail_in_fifo < CODEC_MIN_SAMPLES)) {
        tlkdrv_codec_sync_speaker_samples(CODEC_SYNC_SAMPLE);
        tlkapi_sendData(APP_LOG_EN, "spk sample reset", &avail_in_fifo, 2);
    }

    tlkmdi_local_uac_check_out_sample(96 + 48 + samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudLocalUacAudCtrl.readFrame);

    if (g_tlk_usb_cfg.tick_in) {
#if !TLK_MW_DSP_COMM_ENABLE
        // mic_sample_num =
        avail_in_fifo          = tlkdrv_codec_get_mic_avail_samples();
        uint32_t micSampleRate = tlkusb_uac_get_iso_in_SampleRate();
        samples_per_ms         = micSampleRate / 1000;
        uint32_t micSampleRef  = samples_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame;

        if ((avail_in_fifo > micSampleRef + 48) || (avail_in_fifo < micSampleRef)) {
            tlkdrv_codec_sync_mic_samples(micSampleRef + 24);
            tlkapi_sendData(APP_LOG_EN, "mic sample reset", &avail_in_fifo, 2);
        }
#endif
        static bool iso_in_sync_flag = false;
        if (!iso_in_sync_flag) {
            g_tlk_usb_cfg.in_w = g_tlk_usb_cfg.in_r + 32; //
            g_tlk_usb_cfg.in_w &= APP_USB_ISO_IN_BUFF_IDX_MASK;
            iso_in_sync_flag = true;
        }
        // unsigned int r = core_interrupt_disable();
        // iso_in_sample = ((g_tlk_usb_cfg.in_w - g_tlk_usb_cfg.in_r) & APP_USB_ISO_IN_BUFF_IDX_MASK); //
        // core_restore_interrupt(r);

        // tlkmdi_local_uac_check_in_sample((96 + 96)/3);//16k
    }
}

/**
 * @brief  This function is used to handle the audio UAC music module interrupt.
 * @param  None.
 * @return None.
 */
static void tlkmdi_local_uac_audio_irqProc(void)
{
#if 0
    uint32_t intv = tlkmdi_local_uac_audio_intval();

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

    if (sTlkMdiAudLocalUacAudCtrl.last_tick == 0) {
        sTlkMdiAudLocalUacAudCtrl.readFrame = 5;
    } else {
        diff_tick                        = refTime - sTlkMdiAudLocalUacAudCtrl.last_tick + sTlkMdiAudLocalUacAudCtrl.tick_rem;
        sTlkMdiAudLocalUacAudCtrl.readFrame = (diff_tick / (SYSTEM_TIMER_TICK_1US * 1000));
        sTlkMdiAudLocalUacAudCtrl.tick_rem  = diff_tick % (SYSTEM_TIMER_TICK_1US * 1000);
    }

    sTlkMdiAudLocalUacAudCtrl.last_tick = refTime;
#else
    uint32_t intv_us                    = TLKAUD_UAC_IRQ_INTERVAL_US;
    uint32_t irq_interval               = clock_time() - sTlkMdiAudLocalUacAudCtrl.last_tick;
    sTlkMdiAudLocalUacAudCtrl.readFrame = TLKAUD_UAC_IRQ_INTERVAL_US / 1000;
    if (irq_interval < (TLKAUD_UAC_IRQ_INTERVAL_US * SYSTEM_TIMER_TICK_1US / 3)) {
        intv_us = TLKAUD_UAC_IRQ_INTERVAL_US - irq_interval / SYSTEM_TIMER_TICK_1US;
        sTlkMdiAudLocalUacAudCtrl.last_tick += TLKAUD_UAC_IRQ_INTERVAL_US * SYSTEM_TIMER_TICK_1US;
    } else {
        sTlkMdiAudLocalUacAudCtrl.last_tick = clock_time() + TLKAUD_UAC_IRQ_INTERVAL_US * SYSTEM_TIMER_TICK_1US;
        tlkapi_sendData(1, "local uac audio irq timeout", &irq_interval, 4);
    }
    tlkmdi_audio_task_set_next_irq(intv_us);
    tlkmdi_local_uac_audio_check_sample();
#endif
    if (sTlkMdiAudLocalUacAudCtrl.enable) {
        if (sTlkMdiAudLocalUacAudCtrl.spkEn) {
            tlkmdi_local_uac_audio_spkHandler();
        }
        if (sTlkMdiAudLocalUacAudCtrl.micEn) {
            tlkmdi_local_uac_audio_micHandler();
        }
    }
}

/**
 * @brief       This function is used to handle tle algorithm resources.
 * @param[in]   isMicEn - Indicates whether the mic is enabled.
 * @param[in]   isSpkEn - Indicates whether the speaker is enabled.
 * @return      bool - Returns true if the initialization is successful; otherwise false.
 */
static bool tlkmdi_local_uac_init_alg(bool isMicEn, bool isSpkEn)
{
    tlkapi_printf(APP_LOG_EN, "alg init spk:%d mic:%d", isSpkEn, isMicEn);
    if (isSpkEn) {
        tlkalg_init_common(TLKALG_PPM_CHN0 | TLKALG_PPM_USB_CALC);
    } else {
        tlkalg_deinit_common(TLKALG_PPM_CHN0);
    }
    if (isMicEn) {
        tlkalg_init_common(TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
    } else {
        tlkalg_deinit_common(TLKALG_PPM_CHN1);
    }

    if (!isSpkEn && !isMicEn) {
        tlkalg_deinit_common(TLKALG_PPM_USB_CALC);
    }

    return true;
}

audio_ram_code void tlkmdi_local_uac_mainloop(void)
{
    // tlkapi_printf(APP_LOG_EN, "iso_in %d, mic %d", iso_in_sample, mic_sample_num);
    tlkmdi_audio_runMainAfterTimeMs(500);
}

/**
 * @brief     Switches the state of the UAC music module.
 * @param[in] handle - The connection handle.
 * @param[in] status - The target status (e.g., TLK_STATE_OPENED or TLK_STATE_CLOSED).
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_local_uac_audio_switch(uint16_t handle, uint8_t status)
{
    bool enable;

    if (status == TLK_STATE_OPENED) {
        enable = true;
    } else {
        enable = false;
    }

    sTlkMdiAudLocalUacAudCtrl.handle    = handle;
    sTlkMdiAudLocalUacAudCtrl.spkEn     = tlkusb_uac_get_iso_out_enable();
    sTlkMdiAudLocalUacAudCtrl.micEn     = tlkusb_uac_get_iso_in_enable();
    sTlkMdiAudLocalUacAudCtrl.enable    = enable;
    sTlkMdiAudLocalUacAudCtrl.startflag = 0;
    sTlkMdiAudLocalUacAudCtrl.tick_rem  = 0;
    sTlkMdiAudLocalUacAudCtrl.last_tick = 0;

    if (tlkmdi_local_uac_init_alg(sTlkMdiAudLocalUacAudCtrl.micEn, sTlkMdiAudLocalUacAudCtrl.spkEn) == false) {
        return false;
    }

    if (enable) {
        if (sTlkMdiAudLocalUacAudCtrl.spkEn) {
            uint32_t spkSampleRate;
            spkSampleRate       = tlkusb_uac_get_iso_out_SampleRate();
            uint8_t spkBitDepth = tlkusb_uac_get_iso_out_BitDepth();

            if (spkSampleRate == 0) {
                spkSampleRate = TLKDRV_CODEC_SAMPLERATE_48000;
            }
            tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "SPK-Samp:[%d], bitdepth %d", spkSampleRate, spkBitDepth);
#if TLKADU_MIDBUF_ENABLE
            tlkaud_set_audio_mode(AUDIO_UAC_SPK);
#else
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, spkBitDepth, spkSampleRate, 0);
#endif
        }

        if (sTlkMdiAudLocalUacAudCtrl.micEn) {
            uint32_t micSampleRate;
            micSampleRate       = tlkusb_uac_get_iso_in_SampleRate();
            uint8_t micBitDepth = tlkusb_uac_get_iso_in_BitDepth();

            if (micSampleRate == 0) {
                micSampleRate = TLKDRV_CODEC_SAMPLERATE_16000;
            }

            tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "MIC-Samp:[%d] bitdepth %d", micSampleRate, micBitDepth);
#if TLKADU_MIDBUF_ENABLE
            tlkaud_set_audio_mode(AUDIO_UAC_MIC);
#else
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, micBitDepth, micSampleRate, 0);
#endif
        }

        // uint send_data1[3];
        // send_data1[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
        // send_data1[1] = tlkdrv_codec_get_speaker_rptr();
        // send_data1[2] = tlkdrv_codec_get_spk_avail_samples();
        // spi_master_write_dma(0, (uint8_t *)send_data1, 12);
        // while(spi_is_busy(0));

        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_local_uac_audio_irqProc);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, tlkmdi_local_uac_mainloop);
        tlkmdi_audio_setup_and_start_timer();
    } else {
        uint16_t sample_num = tlkdrv_codec_get_spk_avail_samples();
        tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "sample_num %d spk_en:%d", sample_num, sTlkMdiAudLocalUacAudCtrl.spkEn);

#if TLKADU_MIDBUF_ENABLE
        if (status == TLK_STATE_CLOSED) {
            tlkaud_clear_audio_mode(AUDIO_UAC_SPK);
            tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
        }
        tlkdrv_codec_muteSpkBuff();
#else
        if (1) { //sTlkMdiAudLocalUacAudCtrl.spkEn == false
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
        } else if (sTlkMdiAudLocalUacAudCtrl.micEn == false) {
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
        }
#endif
        if (sTlkMdiAudLocalUacAudCtrl.spkEn == false && sTlkMdiAudLocalUacAudCtrl.micEn == false) {
            tlkmdi_audio_stop_timer();
        }
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
    }

    tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "tlkmdi_local_uac_audio_switch: %d %d", handle, status);

    return true;
}

/**
 * @brief  Checks if the UAC music module is busy.
 * @param  None.
 * @return bool - Returns true if the module is busy; otherwise false.
 */
bool tlkmdi_local_uac_audio_isBusy(void)
{
    return sTlkMdiAudLocalUacAudCtrl.enable;
}

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)handle;
    (void)opcode;
    (void)pdata;
    (void)dataLen;

    tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "tlkmdi_local_uac_audio_operate: opcode[%d] dataLen[%d]", opcode, dataLen);
    if (opcode >= TLKAUD_OPCODE_MAX) {
        return false;
    }

    switch (opcode) {
    case TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE:
    {
        bool micEn = tlkusb_uac_get_iso_in_enable();
        bool spkEn = tlkusb_uac_get_iso_out_enable();

        tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "tlkmdi_local_uac_audio_operate: path_switch: mic_spk[%d][%d] -> [%d][%d]", sTlkMdiAudLocalUacAudCtrl.micEn,
                     sTlkMdiAudLocalUacAudCtrl.spkEn, micEn, spkEn);

        /*Double check mic and spk status.*/
        if (micEn == sTlkMdiAudLocalUacAudCtrl.micEn && spkEn == sTlkMdiAudLocalUacAudCtrl.spkEn) {
            return false;
        }
        if (tlkmdi_local_uac_init_alg(micEn, spkEn) == false) {
            return false;
        }

        if (spkEn && !sTlkMdiAudLocalUacAudCtrl.spkEn) {
            uint32_t spkSampleRate;
            spkSampleRate       = tlkusb_uac_get_iso_out_SampleRate();
            uint8_t spkBitDepth = tlkusb_uac_get_iso_out_BitDepth();

            if (spkSampleRate == 0) {
                spkSampleRate = TLKDRV_CODEC_SAMPLERATE_48000;
            }

            tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "tlkmdi_local_uac_audio_operate: spkSamp[%d] bitDepth[%d]", spkSampleRate, spkBitDepth);
// tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK,TLKDRV_CODEC_CHANNEL_STEREO,spkBitDepth,spkSampleRate,0);
#if TLKADU_MIDBUF_ENABLE
            tlkaud_set_audio_mode(AUDIO_UAC_SPK);
#else
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, spkBitDepth, spkSampleRate, 0);
#endif
        }

        if (micEn && !sTlkMdiAudLocalUacAudCtrl.micEn) { // music -> voice
            uint32_t micSampleRate;
            micSampleRate       = tlkusb_uac_get_iso_in_SampleRate();
            uint8_t micBitDepth = tlkusb_uac_get_iso_in_BitDepth();
            (void)micBitDepth;
            if (micSampleRate == 0) {
                micSampleRate = TLKDRV_CODEC_SAMPLERATE_16000;
            }

            tlkapi_trace(TLKMDI_LOC_UAC_FLAG, TLKMDI_LOC_UAC_SIGNAL, "tlkmdi_local_uac_audio_operate: micSamp[%d]", micSampleRate);
#if TLKADU_MIDBUF_ENABLE
            tlkaud_set_audio_mode(AUDIO_UAC_MIC);
#else
            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, micBitDepth, micSampleRate, 0);
#endif
        }
#if TLKADU_MIDBUF_ENABLE // voice -> music
        tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
        tlkaud_set_audio_mode(AUDIO_UAC_SPK);
#else
        if (!micEn && sTlkMdiAudLocalUacAudCtrl.micEn) {
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
        }
#endif

        sTlkMdiAudLocalUacAudCtrl.spkEn = spkEn;
        sTlkMdiAudLocalUacAudCtrl.micEn = micEn;
    } break;
    case TLKAUD_OPCODE_TYPE_GET_TYPE:
    {
    } break;
    default:
        return false;
    }
    return true;
    return false;
}

/**
 * @brief  Speaker handler for UAC music module.
 * @param  None.
 * @return None.
 */
void tlkmdi_local_uac_audio_spkHandler(void)
{
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
    int32_t buffer[2048];
#else
    int16_t buffer[2048];
#endif
    // DBG_YATING_CHN1_HIGH;

    uint16_t samples_per_ms = tlkusb_uac_get_iso_out_SampleRate() / 1000;
    uint16_t bytes_per_ms   = samples_per_ms * sizeof(loacl_uac_codec_int);

    if (sTlkMdiAudLocalUacAudCtrl.readFrame == 6) {}

#if (TLK_DEV_CODEC_ENABLE)

    // tlkusb_ppm_process(sTlkMdiAudLocalUacAudCtrl.last_tick);
#if 0
    uint16_t length;
    // uint16_t used_samples = tlkusb_uac_get_ppm_out_used_samples();
    uint16_t used_samples = tlkusb_uac_get_iso_out_used_samples();

    if (used_samples >= (TLKUSB_AUD_SPK_SAMPLES * 15) && !sTlkMdiAudLocalUacAudCtrl.startflag) {
        sTlkMdiAudLocalUacAudCtrl.startflag = 1;
        tlkdrv_codec_sync_speaker_samples(CODEC_SPK_FIFO_SAMPLES / 2);
    }

    if (!sTlkMdiAudLocalUacAudCtrl.startflag) {
        // DBG_YATING_CHN1_LOW;
        return;
    }

    length = 0;

    if (tlkdrv_codec_getSpkIdleLen() > (bytes_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame)) {
        // length = tlkusb_uac_read_ppm_out_samples(buffer, samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudLocalUacAudCtrl.readFrame);
        length = tlkusb_uac_read_iso_out_samples(buffer, samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudLocalUacAudCtrl.readFrame);
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

    if (length == 0 || (length != samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudLocalUacAudCtrl.readFrame)) {
        // DBG_YATING_CHN1_LOW;
        return;
    }

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 16) && 0
   int16_t *pData       = (int16_t *)buffer;
   uint    audio_volume = tlkusb_uacspk_getVolume();
   for (uint index = 0; index < length; index++) {
       pData[index] = (pData[index] * audio_volume + 0x2000) >> 14;
   }
#endif
    // DBG_YATING_CHN5_HIGH;
    // DBG_YATING_CHN5_LOW;
#else
    tlkusb_uac_read_iso_out_samples(buffer, samples_per_ms * tlkusb_uac_get_iso_out_Channels() * sTlkMdiAudLocalUacAudCtrl.readFrame);
#endif

#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_audio_alg = audio_alg_get_interface_by_type(ALG_EQ);
    p_audio_alg->audio_alg_process((uint8_t *)buffer, (uint8_t *)buffer, samples_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
#endif

    tlkdrv_codec_fillSpkBuff((uint8_t *)buffer, bytes_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame);
// tlkdrv_codec_fillSpkBuff((uint8_t*)sin96k_24bit, bytes_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame);
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

#if TLKADU_MIDBUF_ENABLE
bool tlkmdi_local_uac_audio_get_mic_data_mono(uint8_t *pdata, uint16_t samples, uint32_t sr, uint8_t bitdepth)
{
    bool        ret = false;
    mic_buf_typ data_buff[samples];
    adc_mono    mono_buff[samples];
    adc_mono   *psrc     = (adc_mono *)data_buff;
    mic_chnl_e  mic_chnl = 0;

    if (sr == 48000) {
        ret = tlkaud_sidetone_get_mic_data_48k((mic_buf_typ *)data_buff, samples, &mic_chnl);
    } else if (sr == 16000) {
        ret = tlkaud_sidetone_get_mic_data_16k((mic_buf_typ *)data_buff, samples, &mic_chnl);
    }

    if (ret) {
        if (mic_chnl == MIC_CHNL_STEREO) {
            for (int i = 0; i < samples; i++) {
                mono_buff[i] = psrc[2 * i];
            }
            psrc = mono_buff;
        } else {
            (void)mono_buff;
        }

        if (bitdepth == 16 && (sizeof(adc_mono) == 4)) { //usb-16bit & adc-24bit
            short *pdes = (short *)pdata;
            for (int i = 0; i < samples; i++) {
                *pdes++ = psrc[i] >> 8;
            }
        } else if (bitdepth == 24 && (sizeof(adc_mono) == 2)) { //usb-24bit & adc-16bit
            int *pdes = (int *)pdata;
            for (int i = 0; i < samples; i++) {
                *pdes++ = psrc[i] << 8;
            }
        } else { //usb-24bit & adc-24bit or usb-16bit & adc-16bit
            tmemcpy(pdata, psrc, samples * sizeof(adc_mono));
        }
    }

    return ret;
}
#endif
/**
 * @brief  UAC music module mic interrupt handler.
 * @param  None.
 * @return None.
 */
void tlkmdi_local_uac_audio_micHandler(void)
{
    uint16_t samples_per_ms = 0;
    uint8_t  buffer[1024];
    uint32_t micSampleRate = tlkusb_uac_get_iso_in_SampleRate();
    uint8_t  micBitDepth   = tlkusb_uac_get_iso_in_BitDepth();

    samples_per_ms = micSampleRate / 1000;

    uint16_t sample_num = samples_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame;

#if TLKADU_MIDBUF_ENABLE
    tlkmdi_local_uac_audio_get_mic_data_mono(buffer, sample_num, micSampleRate, micBitDepth);
#else
    (void)micBitDepth;
    // uint16_t  bytes_per_ms = 0;
    // uint8_t  micChannel    = tlkusb_uac_get_iso_in_Channels();
    // bytes_per_ms = samples_per_ms * micChannel * micBitDepth/8;
    // uint16_t bytes_num = bytes_per_ms * sTlkMdiAudLocalUacAudCtrl.readFrame;

    // if (tlkdrv_codec_readMicData(buffer, bytes_num, 0) == false) {

    // } else {

    // }
#endif

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

    // tlkusb_uacmic_ppm_process(sTlkMdiAudLocalUacAudCtrl.last_tick);
}

#endif //#if(LK_USB_UAC_AUDIO_LOCAL_ENABLE)
