/********************************************************************************************************
 * @file    bt_voice_data_path.c
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
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"
#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif

//#include "stack/inner/tlkstk_inner.h"
#include "drivers.h"

#if (TLKBTP_CFG_HFP_ENABLE)
#define REF_SAMPLES_TH                 60
#define REF_RATE_EQUAL                 0
#define REF_RATE_GT                    1
#define REF_RATE_LT                    2

#define HCI_SYNC_HDR_PSF_FLAG_MASK     (0x3000)

#define HCI_SYNC_HDR_PSF_GOOD          0
#define HCI_SYNC_HDR_PSF_INVALID       1
#define HCI_SYNC_HDR_PSF_NO_RCVD       2
#define HCI_SYNC_HDR_PSF_LOST          3


#define BT_SYNC_LOSS_CHECK_BY_PKT_NUMB (0)
#define BT_SYNC_LOSS_CHECK_BY_TIME_OUT (0)
#define BT_SYNC_LOSS_CHECK_ENABLE      (BT_SYNC_LOSS_CHECK_BY_PKT_NUMB || BT_SYNC_LOSS_CHECK_BY_TIME_OUT)

#if (TLKALG_ASRC_16TO48_16BIT_ENABLE && TLK_STK_BT_TPSLL_ENABLE)
#define REF_SAMPLES     288 * 3
#define REF_MIC_SAMPLES 288
#elif (TLKALG_ASRC_16TO48_16BIT_ENABLE && TLKSTK_BTTPSLL_TWS_ENABLE)
#define REF_SAMPLES     288 * 3
#define REF_MIC_SAMPLES 288
#else
#define REF_SAMPLES     240
#define REF_MIC_SAMPLES 240
#endif
#if (BT_SYNC_LOSS_CHECK_ENABLE)

#define BT_SYNC_ERR_CHECK_TIME_OUT_US (10000000) //10s
static uint32_t sTlkBtSyncErrCheckTick = 0;
uint16_t        sTlkBtSyncErrIndex     = 0;
uint16_t        sTlkBtSyncErrCount     = 0; // sync error caused by rf.
uint16_t        sTlkBtSyncLossCount    = 0; // sync error caused by other task.
#endif

typedef enum hci_type_e
{
    HCI_TYPE_CMD = 0x01,
    HCI_TYPE_ACL_DATA,
    HCI_TYPE_SCO_DATA,
    HCI_TYPE_EVENT,
    HCI_TYPE_ISO_DATA,
} hci_type_t;

uint8_t g_msbc_enc_frame[60];
uint8_t g_msbc_enc_2_frames[120];
uint8_t g_msbc_enc_id = 0;

volatile unsigned int bt_ll_sco_start_tick;
uint8_t               g_bt_sco_id[SCO_ENC_QUEUE_NUM];

/** allow use silent pkt instead of all zero pkt at esco beginning */
extern uint8_t voice_msbc_silence_pkt[60];

/**
 * @brief   Check if frame head is MSBC
 * @param[in]  p - SCO data
 * @returns    TRUE or FALSE
 */
audio_ram_code bool msbc_is_frame_head(uint8_t *p)
{
    uint8_t  frame_1st         = p[0];
    uint8_t  frame_sec         = p[1];
    uint8_t  frame_third       = p[2];
    uint16_t frame_forth_fifth = p[3] | (p[4] << 8);

    if (MSBC_SYNCWORD == frame_third && 0 == frame_forth_fifth && 1 == frame_1st && (0x08 == frame_sec || 0x38 == frame_sec || 0xC8 == frame_sec || 0xF8 == frame_sec)) {
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief   Check if MSBC data is all zero packet
 * @param[in]  p - SCO data
 * @returns    TRUE or FALSE
 */
audio_ram_code bool msbc_is_all_zero_pkt(uint8_t *p)
{
    uint32_t frame_1st = p[8] | (p[9] << 8) | (p[10] << 16) | (p[11] << 24);
    uint32_t frame_sec = p[12] | (p[13] << 8) | (p[14] << 16) | (p[15] << 24);

    if ((0 == frame_1st) && (0 == frame_sec)) {
        return TRUE;
    }

    return FALSE;
}

#if SCO_FORWARD_GPIO_DEBUG
audio_ram_code void bt_voice_gpio_toggle_sco_tx(uint8_t times, uint8_t queue_id)
{
    gpio_pin_e pin = GPIO_PA0;

    if (queue_id == SCO_ENC_QUEUE_ID_HF) {
        pin = GPIO_PB6; //GPIO_PA0;
    } else {
        pin = GPIO_PA1;
    }

    for (uint8_t i = 0; i < times; i++) {
        gpio_write(pin, 0);
        gpio_write(pin, 1);
        gpio_write(pin, 0);
    }
}

audio_ram_code void bt_voice_gpio_toggle_sco_rx(uint8_t times, uint8_t queue_id)
{
    gpio_pin_e pin = GPIO_PA2;

    if (queue_id == SCO_ENC_QUEUE_ID_HF) {
        pin = GPIO_PA2;
    } else {
        pin = GPIO_PB5;
    }

    for (uint8_t i = 0; i < times; i++) {
        gpio_write(pin, 0);
        gpio_write(pin, 1);
        gpio_write(pin, 0);
    }
}

audio_ram_code void bt_voice_gpio_toggle_sco_num(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++) {
        gpio_write(GPIO_PB3, 0);
        gpio_write(GPIO_PB3, 1);
        gpio_write(GPIO_PB3, 0);
    }
}
#endif

#if (SCO_FORWARD_WITH_ALG)
/**
 * @brief Process SCO audio data decode and re-encode
 * 
 * @param queue_id Queue ID (SCO_ENC_QUEUE_ID_HF or SCO_ENC_QUEUE_ID_AG)
 * @param p_des Input/output buffer pointer
 * @return NULL
 */
static void bt_voice_forward_alg_process(uint8_t queue_id, uint8_t *p_des)
{
    uint16_t ret = 0;
    int16_t  sco_pcm_buf[300];
    uint8_t *p_enc_src = p_des;
    int16_t *p_pcm_buf = sco_pcm_buf;

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA2, 1);
#endif

    if (queue_id == SCO_ENC_QUEUE_ID_HF) {
        ret = bt_voice_cfg.dec_func(p_enc_src, (uint8_t *)p_pcm_buf, 60, 0, 1);
    } else if (queue_id == SCO_ENC_QUEUE_ID_AG) {
        ret = bt_voice_cfg.dec_forward_func(p_enc_src, (uint8_t *)p_pcm_buf, 60, 0, 1);
    }

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA2, 0);
#endif

    if (!ret) {
        if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec(queue_id)) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_MSBC_DEC %d", ret);
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_CVSD_DEC %d", ret);
        }
    }

    uint8_t *p_enc_des = p_des;

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PB5, 1);
#endif

    if (queue_id == SCO_ENC_QUEUE_ID_HF && bt_voice_cfg.enc_forward_func != NULL) {
        bt_voice_cfg.enc_forward_func((uint8_t *)p_pcm_buf, p_enc_des, 240, 0, 3);
    } else if (queue_id == SCO_ENC_QUEUE_ID_AG && bt_voice_cfg.enc_func != NULL) {
        bt_voice_cfg.enc_func((uint8_t *)p_pcm_buf, p_enc_des, 240, 0, 3);
    }

    if (!ret) {
        if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec(queue_id)) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_MSBC_ENC %d", ret);
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_CVSD_ENC %d", ret);
        }
    }

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PB5, 0);
#endif
}
#endif

/**
 * @brief   Get SCO data from stack and push to encode buffer
 * @param[in]  id     - frame ID
 * @param[in]  p_data - SCO data start address
 * @param[in]  len    - SCO data length
 * @return     None
 */
audio_ram_code void bt_voice_receive_sco_frame(uint8_t id, uint8_t *p_data, uint16_t len, uint16_t sco_handle)
{
    (void)len;
    uint8_t        i   = 0;
    uint8_t        err = 0;
    uint8_t       *msbc_enc_start, *p_des;
    uint16_t       sync_hdl;
    uint8_t        msbc_frame_valid;
    uint16_t       frame_start_pos = 0;
    uint8_t        last_pos, sco_frame_len;
    uint8_t       *frame_start;
    static uint8_t frame_start_pos_last = 0;
    static uint8_t valid_msbc_frame_cnt = 0;

    uint16_t sco_conn_handle = sco_handle & 0x0FFF;
    uint8_t  queue_id        = btif_get_sco_queue_id(sco_conn_handle);

    sco_frame_len = p_data[3];

#if ((TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
    if (!tlkmdi_audu2h_voice_isBusy()) {
        return;
    }
#endif

    g_bt_sco_id[queue_id]++;
    if (g_bt_sco_id[queue_id] == 8) {
        g_bt_sco_id[queue_id] = 0;
    }
    id = g_bt_sco_id[queue_id];

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "=== voice rx, id: %d, fram_len:%d, len: %d, data: %x %x %x %x %x %x %x %x syncHead:%x",
                 id, sco_frame_len, len, p_data[0], p_data[1], p_data[2], 
                 p_data[3], p_data[4], p_data[5], p_data[6], p_data[7], syncHead);
    */

    /* start voice function on first valid packet */
    if (bt_voice_cfg.bt_voice_mode == BT_VOICE_PLAYBACK_MODE) {
        if ((bt_voice_cfg.status == BT_VOICE_ST_IDLE && (NULL == p_data)) || bt_voice_cfg.status == BT_VOICE_ST_INIT || NULL == p_data) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_SCO_RX skip, PLAYBACK");
            return;
        }
    } else {
        if (NULL == p_data) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_SCO_RX skip, FORWARD");
            return;
        }
    }

#if AUDIO_TWS_MODE
    async_tws_set_status_flag(TSYNC_FLAG_MUSIC, 0);
    async_tws_set_status_flag(TSYNC_FLAG_SCO, 1);
#endif

    audio_codec_flag_set(CODEC_FLAG_MUSIC, 0);
    audio_codec_flag_set(CODEC_FLAG_VOICE, 1);

#ifdef SL16_bt_voice_st
    log_b16(SL_BT_VOICE_LOG_EN, SL16_bt_voice_st, bt_voice_cfg.status | 0x100);
#endif

    if (bt_voice_cfg.bt_voice_mode == BT_VOICE_PLAYBACK_MODE) {
        if (bt_voice_cfg.status < BT_VOICE_ST_CODEC_READY) {
            bt_voice_cfg.status++;
            bt_voice_cfg.sync_init       = 0;
            bt_voice_cfg.codec_sync_init = 0;
        }

        if (bt_voice_cfg.status == BT_VOICE_ST_CODEC_READY && bt_voice_cfg.sync_init == 0) {
            bt_voice_cfg.sync_init = 1;

            bt_voice_cfg.sync_samples_peer = REF_SAMPLES;
            bt_voice_cfg.samples_cur       = REF_SAMPLES;
            bt_voice_cfg.samples_last      = REF_SAMPLES;
            bt_voice_cfg.samples_ref       = REF_SAMPLES;

            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_VOICE_ST_CODEC_READY sync_init");

#if (TLK_ALG_HYBRID_ENABLE)
            tlkalg_sync_mic_spk_index(bt_voice_cfg.samples_ref - 10);
#endif

#if TLKMW_INTERPHONE_EN
            if (tlkmdi_interphone_is_busy()) {
                // gpio_set_high_level(GPIO_CHN0);
                // gpio_set_low_level(GPIO_CHN0);
                uint16_t num_of_speaker_samples;

                tlkmdi_interphone_sync_BtDownBuff(bt_voice_cfg.samples_ref);
                tlkmdi_interphone_sync_BtUpBuff(bt_voice_cfg.samples_ref);
                num_of_speaker_samples = tlkmdi_interphone_get_BtDownBuff_DataLen() / (sizeof(codec_int));
                tlkmdi_interphone_voice_ag_enable();
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "num_of_speaker_samples %d", num_of_speaker_samples);
            } else
#endif
            {
                /* fill mute data to play back buffer, 240 samples */
#if TLKADU_MIDBUF_ENABLE
                if (tlkaud_codec_task_if_can_start()) {
                    tlkmdi_midbuf_sync_spk(bt_voice_cfg.samples_ref);
                } else {
                    tlkdrv_codec_sync_play_samples(bt_voice_cfg.samples_ref);
                }
#else
                tlkdrv_codec_sync_play_samples(bt_voice_cfg.samples_ref);
#endif

                /* fill mic mute data, 240 samples */
#if (TLKSTK_BT_TPS_ENABLE && !PROJ_HEARING_AID)
                //uint16_t mic_samples_ref = REF_MIC_SAMPLES;
                //tlkdrv_codec_sync_mic_samples(mic_samples_ref);
#else
                tlkdrv_codec_sync_mic_samples(bt_voice_cfg.samples_ref);
#endif
            }
        } else if (bt_voice_cfg.status > BT_VOICE_ST_CODEC_READY) {
        }
    }

    bt_voice_cfg.sync_id = id;
    msbc_enc_start       = p_data + 4;

#if (BT_SYNC_LOSS_CHECK_BY_TIME_OUT)
    if (sTlkBtSyncErrCheckTick == 0) {
        sTlkBtSyncErrCheckTick = clock_time() | 1;
    }
    if (sTlkBtSyncErrCheckTick != 0 && clock_time_exceed(sTlkBtSyncErrCheckTick, BT_SYNC_ERR_CHECK_TIME_OUT_US)) {
        sTlkBtSyncErrCheckTick = clock_time() | 1;
        if (sTlkBtSyncLossCount != 0 || sTlkBtSyncErrCount != 0) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_VOICE_SYNC_LOSS_CHECK [%d] loss by rf, [%d] loss by other per[%d]s", sTlkBtSyncErrCount, sTlkBtSyncLossCount,
                         BT_SYNC_ERR_CHECK_TIME_OUT_US / 1000 / 1000);
            sTlkBtSyncLossCount = 0;
            sTlkBtSyncErrCount  = 0;
        }
    }
#endif

    if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec(queue_id)) {
        sync_hdl         = p_data[1] + (p_data[2] << 8);
        msbc_frame_valid = ((sync_hdl & HCI_SYNC_HDR_PSF_FLAG_MASK) == HCI_SYNC_HDR_PSF_GOOD) ? 1 : 0;

#if (BT_SYNC_LOSS_CHECK_ENABLE)
        uint8_t frame_flag = ((sync_hdl & HCI_SYNC_HDR_PSF_FLAG_MASK) >> 12);
        if (frame_flag != HCI_SYNC_HDR_PSF_GOOD) {
            sTlkBtSyncErrIndex++;
            if (frame_flag == HCI_SYNC_HDR_PSF_LOST) {
                sTlkBtSyncLossCount++;
            } else {
                sTlkBtSyncErrCount++;
            }
#if (BT_SYNC_LOSS_CHECK_BY_PKT_NUMB)
            if (sTlkBtSyncErrIndex >= 100) {
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_VOICE_SYNC_LOSS_CHECK [%d] loss by rf, [%d] loss by other", sTlkBtSyncErrCount, sTlkBtSyncLossCount);

                sTlkBtSyncErrIndex  = 0;
                sTlkBtSyncLossCount = 0;
                sTlkBtSyncErrCount  = 0;
            }
#endif
        }
#endif

        if ((bt_ll_sco_start_tick && !clock_time_exceed(bt_ll_sco_start_tick, 400 * 1000)) || btif_power_off_is_ongoing()) {
            bt_voice_cfg.spk_enc_wptr[queue_id] = id & (SPK_ENC_BUFF_NUM - 1);
            p_des                               = g_spk_enc_buff_ptr[queue_id] + bt_voice_cfg.spk_enc_wptr[queue_id] * SPK_ENC_BUFF_SIZE;
            p_des[0]                            = 0;
            tmemcpy(p_des + 2, msbc_silence_pkt, 60);

            return;
        } else if (bt_ll_sco_start_tick && clock_time_exceed(bt_ll_sco_start_tick, 400 * 1000)) {
            bt_ll_sco_start_tick = 0;
        }

        if (msbc_frame_valid && (msbc_is_frame_head(msbc_enc_start) == FALSE)) {
            last_pos = 120 - 60 + 1;

            if (0 && msbc_is_all_zero_pkt(msbc_enc_start)) {
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "use silen pkt instead of al zero pkt at esco beginning");
                tmemcpy(msbc_enc_start, msbc_silence_pkt, 60);
                valid_msbc_frame_cnt = 0;
            } else if ((abs_ram(id - g_msbc_enc_id) == 1) || (0xff == g_msbc_enc_id && 0 == id)) {
                /* P1. save msbc frame. */
                valid_msbc_frame_cnt++;
                if (valid_msbc_frame_cnt > 2) {
                    valid_msbc_frame_cnt = 2;
                }

                tmemcpy(g_msbc_enc_2_frames, g_msbc_enc_2_frames + 60, 60);
                tmemcpy(g_msbc_enc_2_frames + 60, msbc_enc_start, 60);
            } else {
                valid_msbc_frame_cnt = 0;
                err                  = 3;
            }

            /* P3. already received continue 2 valid msbc frames. */
            if (2 == valid_msbc_frame_cnt) {
                for (i = 0; i < last_pos; i++) {
                    frame_start = &g_msbc_enc_2_frames[0] + i;
                    if (msbc_is_frame_head(frame_start) == TRUE) {
                        frame_start_pos = i;
                        tmemcpy(&g_msbc_enc_frame[0], frame_start, 60);
                        msbc_enc_start = &g_msbc_enc_frame[0];
                        err            = 0;
                        break;
                    }
                }

                if (i >= last_pos) {
                    err = 2;
                    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# not find in g_msbc_enc_2_frames error %d %d %d %d %d %d %d %d", err, i, last_pos, valid_msbc_frame_cnt,
                                 g_msbc_enc_id, id, frame_start_pos, frame_start_pos_last);
                } else {
                    frame_start_pos_last = frame_start_pos;
                }
            }

            if (err) {
                tlkapi_error(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# msbc add error %d %d %d %d %d %d %d %d", err, i, last_pos, valid_msbc_frame_cnt, g_msbc_enc_id, id,
                             frame_start_pos, frame_start_pos_last);

                tlkapi_array(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "msbc_enc_2_frames", g_msbc_enc_2_frames, 120);

                valid_msbc_frame_cnt = 0;
                tmemset(g_msbc_enc_2_frames, 0, 120);
            }

            g_msbc_enc_id = id;

        } else {
            valid_msbc_frame_cnt = 0;
        }
    } else {
    }

    bt_voice_cfg.tick_frame = clock_time();
    if (bt_voice_cfg.bt_voice_mode == BT_VOICE_PLAYBACK_MODE) {
        /* SCO playback mode */
        bt_voice_cfg.spk_enc_wptr[queue_id] = id & (SPK_ENC_BUFF_NUM - 1);
    } else if (bt_voice_cfg.bt_voice_mode == BT_VOICE_FORWARD_MODE || bt_voice_cfg.bt_voice_mode == BT_VOICE_FORWARD_WITH_ALG_MODE) {
        /* SCO forward mode */
        if (bt_voice_cfg.sco_forward_state[queue_id] == BT_VOICE_FORWARD_ST_IDLE) {
            bt_voice_cfg.sco_forward_state[queue_id] = BT_VOICE_FORWARD_ST_1ST_SCO_RX;
            bt_voice_cfg.spk_enc_wptr[queue_id]      = 2;
            bt_voice_cfg.spk_enc_rptr[queue_id]      = 0;
            bt_voice_cfg.mic_enc_wptr[queue_id]      = 2;
            bt_voice_cfg.mic_enc_rptr[queue_id]      = 0;
        }
        bt_voice_cfg.spk_enc_wptr[queue_id] = (bt_voice_cfg.spk_enc_wptr[queue_id] + 1) % SPK_ENC_BUFF_NUM_FORWARD;

#if SCO_FORWARD_GPIO_DEBUG
        bt_voice_gpio_toggle_sco_rx(bt_voice_cfg.spk_enc_wptr[queue_id] + 1, queue_id);
#endif
    }

    //log_b16_general(1, SL16_bt_sco_enc_wptr, bt_voice_cfg.spk_enc_wptr[queue_id]);

    p_des         = g_spk_enc_buff_ptr[queue_id] + bt_voice_cfg.spk_enc_wptr[queue_id] * SPK_ENC_BUFF_SIZE;
    sco_frame_len = p_data[3];

    if (p_data && p_data[0] == HCI_TYPE_SCO_DATA) {
        sync_hdl = p_data[1] + (p_data[2] << 8);

        if (sco_frame_len > 60) {
            tlkapi_error(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "sco_frame_len, err: %d", sco_frame_len);
            sco_frame_len = 60;
        }

        if ((sync_hdl & HCI_SYNC_HDR_PSF_FLAG_MASK) == HCI_SYNC_HDR_PSF_GOOD &&
            ((HFP_CODEC_ID_MSBC == btif_get_hfp_codec(queue_id) && msbc_is_frame_head(msbc_enc_start)) || (HFP_CODEC_ID_CVSD == btif_get_hfp_codec(queue_id))) && !err) {
            p_des[0] = 0;

            //tmemcpy(p_des + 2, msbc_test_pkt, sco_frame_len);
            tmemcpy(p_des + 2, msbc_enc_start, sco_frame_len);

            /*
            tlkapi_trace(BT_AUDIO_DBG_FLAG,
                         BT_AUDIO_DBG_SIGN,
                         "=== spk_enc_wptr %d, sco_frame_len: %d",
                         bt_voice_cfg.spk_enc_wptr,
                         sco_frame_len);
            */
        } else {
            // tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "==== PACKET_LOSS, wptr: %d, %x %x",
            // 			 bt_voice_cfg.spk_enc_wptr, msbc_enc_start[0], msbc_enc_start[1]);
            p_des[0] = BT_VOICE_FLAG_PACKET_LOSS;
            tmemset(p_des + 2, 0, sco_frame_len);

#if (SLET_sco_packet_loss)
            log_tick_irq(1, SLET_sco_packet_loss);
#endif
        }
    } else {
        tlkapi_error(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "voice_TYPE_SCO, err %d %x %d", *(uint32_t *)p_data, p_data, sco_frame_len);
        tmemset(p_des + 2, 0, 60);
    }

#if TLK_CFG_HRA_ENABLE
    tlkmdi_hra_stack_set_timer_flag(true);
#else
    if (bt_voice_cfg.bt_voice_mode != BT_VOICE_FORWARD_MODE) {
        g_bt_audio_task_env.tick_irq = clock_time() + 100 * TICK_PER_US;
        tlkmdi_audio_task_set_next_irq(100);
        bt_voice_cfg.sco_rx_tick = clock_time() | 1;
        bt_voice_cfg.sco_proc_id = queue_id;
    } else {
#if (SCO_FORWARD_WITH_ALG)
        bt_voice_forward_alg_process(queue_id, p_des);
#endif
    }
#endif
}

/**
 * @brief   Controller calls this function to post encoded PCM data to controller buffer
 * @param[in]  id     - frame ID
 * @param[in]  p_data - address to restore encoded PCM data
 * @param[in]  len    - data length
 * @param[in]  sco_handle - SCO connection handle
 * @return     None
 */
_attribute_bt_ram_code_ void bt_voice_mic_enc_get_frame(uint8_t id, uint8_t *p_data, uint16_t len, uint16_t sco_handle)
{
    (void)id;
    uint8_t *p_enc_buf = NULL;
    uint8_t  queue_id  = btif_get_sco_queue_id(sco_handle);

    if (bt_voice_cfg.bt_voice_mode == BT_VOICE_PLAYBACK_MODE) {
        if (bt_voice_cfg.status < 2) {
            /* mute mic in CEVA BT core before hfp is ready */
            //bt_e_scomutecntl_mute_source_setf(0, 1);
            return;
        }

        /* copy MIC encoded frame to ESCO buffer */
        p_enc_buf = g_mic_enc_buff_ptr[queue_id] + ((bt_voice_cfg.spk_enc_wptr[queue_id] - 1) & (MIC_ENC_BUFF_NUM - 1)) * MIC_ENC_BUFF_SIZE;
    } else if (bt_voice_cfg.bt_voice_mode == BT_VOICE_FORWARD_MODE || bt_voice_cfg.bt_voice_mode == BT_VOICE_FORWARD_WITH_ALG_MODE) {
        uint8_t src_queue_id = SCO_ENC_QUEUE_ID_HF;
        if (queue_id == SCO_ENC_QUEUE_ID_HF) {
            src_queue_id = SCO_ENC_QUEUE_ID_AG;
        } else if (queue_id == SCO_ENC_QUEUE_ID_AG) {
            src_queue_id = SCO_ENC_QUEUE_ID_HF;
        }

        if (bt_voice_cfg.sco_forward_state[src_queue_id] == BT_VOICE_FORWARD_ST_1ST_SCO_RX) {
            int16_t avail_enc_buf = 0;
            int16_t enc_rptr      = 0;

            if (bt_voice_cfg.bt_voice_mode == BT_VOICE_FORWARD_MODE) {
                uint8_t offset = bt_voice_cfg.spk_enc_rptr[src_queue_id] * SPK_ENC_BUFF_SIZE;
                p_enc_buf      = g_spk_enc_buff_ptr[src_queue_id] + offset;
                //p_enc_buf += 2; // skip 2 bytes head
                bt_voice_cfg.spk_enc_rptr[src_queue_id] = (bt_voice_cfg.spk_enc_rptr[src_queue_id] + 1) % SPK_ENC_BUFF_NUM_FORWARD;

                avail_enc_buf = bt_voice_cfg.spk_enc_wptr[src_queue_id] - bt_voice_cfg.spk_enc_rptr[src_queue_id];
                if (avail_enc_buf < 0) {
                    avail_enc_buf += SPK_ENC_BUFF_NUM_FORWARD;
                }
                enc_rptr = bt_voice_cfg.spk_enc_rptr[src_queue_id];
            } else {
                uint8_t offset                          = bt_voice_cfg.mic_enc_rptr[src_queue_id] * SPK_ENC_BUFF_SIZE;
                p_enc_buf                               = g_mic_enc_buff_ptr[src_queue_id] + offset;
                bt_voice_cfg.mic_enc_rptr[src_queue_id] = (bt_voice_cfg.mic_enc_rptr[src_queue_id] + 1) % SPK_ENC_BUFF_NUM_FORWARD;

                avail_enc_buf = bt_voice_cfg.mic_enc_wptr[src_queue_id] - bt_voice_cfg.mic_enc_rptr[src_queue_id];
                if (avail_enc_buf < 0) {
                    avail_enc_buf += SPK_ENC_BUFF_NUM_FORWARD;
                }
                enc_rptr = bt_voice_cfg.mic_enc_rptr[src_queue_id];
            }

#if SCO_FORWARD_GPIO_DEBUG
            bt_voice_gpio_toggle_sco_tx(enc_rptr + 1, queue_id);
            bt_voice_gpio_toggle_sco_num(avail_enc_buf);
#else
            (void)enc_rptr;
            (void)avail_enc_buf;
#endif
        } else {
            p_enc_buf = voice_msbc_silence_pkt;
        }
    } else {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "Invalid mode: %d", bt_voice_cfg.bt_voice_mode);
    }

    if (p_data && p_enc_buf) {
        tmemcpy(p_data, p_enc_buf, len);
    }
}

#endif
