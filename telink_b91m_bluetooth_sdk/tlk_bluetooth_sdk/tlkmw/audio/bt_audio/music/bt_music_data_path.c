/********************************************************************************************************
 * @file    bt_music_data_path.c
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
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"
#endif

#if (TLKBTP_CFG_A2DPSNK_ENABLE)

#define RECV_A2DP_PACKET_COUNT_LOG 0

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
uint32_t g_a2dp_last_rev_tick  = 0;
uint32_t g_a2dp_rev_cnt        = 0;
uint32_t g_a2dp_rev_start_tick = 0;
int      g_a2dp_rcv_ok         = 0;
#endif

/**
 * @brief       Get the number of available encoded buffers
 * @param[in]   None
 * @return      Number of available encoded buffers
 */
int16_t bt_music_get_num_of_enc_buff_avail(void)
{
    int16_t available = bt_music_cfg.enc_buf_wptr - bt_music_cfg.enc_buf_rptr;
    if (available < 0) {
        available += bt_music_cfg.enc_buf_num;
    }

    return available;
}

/**
 * @brief       Receive A2DP data and post encoded data to queue
 * @param[in]   p_data - A2DP data pointer
 * @param[in]   len    - A2DP packet data length
 * @return      BT_AUDIO_SUCCESS if successful, others if failed
 */
audio_ram_code uint8_t bt_music_receive_a2dp_frames(uint8_t *p_data, uint16_t len)
{
#if LHDC_CODEC_ENABLE
    uint16_t sample_rate = 0;
#endif
    uint8_t  ret = BT_AUDIO_SUCCESS;
    uint32_t enc_para;
    uint8_t  init_ret = 0;

#if (AAC_CODEC_ENABLE)
    int16_t  wptr;
    uint8_t *p_last;
    uint16_t len_last;
#endif

    if (!(tlkmdi_bt_music_is_enabled()
#if TLKALG_ANC_ENABLE
          || tlkmdi_anc_btmusic_is_busy()
#endif
#if TLK_CFG_A2DP_TO_BIS_ENABLE
          || tlkmdi_a2dp_to_bis_is_enabled()
#endif
#if TLKMW_INTERPHONE_EN
          || tlkmdi_interphone_is_busy()
#endif
              )) {
        return 0;
    }

#ifdef SLET_bt_a2dp_rx
    log_tick_general(SL_BT_MUSIC_LOG_EN, SLET_bt_a2dp_rx);
#endif

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "A2DP DATA: %x %x %x %x %x %x % x%",
                 p_data[0], p_data[1], p_data[2], p_data[3],
                 p_data[4], p_data[5], p_data[6], p_data[7]);
    */

#if AUDIO_TWS_MODE
    async_tws_set_status_flag(TSYNC_FLAG_MUSIC, 1);
    async_tws_set_status_flag(TSYNC_FLAG_SCO, 0);
#endif

    audio_codec_flag_set(CODEC_FLAG_MUSIC, 1);
    audio_codec_flag_set(CODEC_FLAG_VOICE, 0);

    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if (AAC_CODEC_ENABLE)
        if (p_data[16] != 0xB0) {
            wptr     = bt_music_cfg.enc_buf_wptr ? bt_music_cfg.enc_buf_wptr - 1 : bt_music_cfg.enc_buf_num - 2;
            p_last   = g_bt_music_enc_buf_ptr + bt_music_cfg.aac_offset[wptr];
            len_last = bt_music_cfg.aac_len[wptr];

            if (len > 12 && p_data[11] == 2 && len_last + len - 12 <= bt_music_cfg.enc_buf_size) {
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: AAC continue packet> %d %d %d %d", len_last, len, p_data[3], bt_music_cfg.enc_buf_id);

                tmemcpy(p_last + len_last, p_data + 12, len - 12);

                len_last += len - 12;
                bt_music_cfg.aac_len[wptr] = len_last;
            } else {
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: not AAC packet> %d %d %d %d", len_last, len, p_data[3], bt_music_cfg.enc_buf_id);
            }

            return BT_AUDIO_PACKET_INVALID;
        } else {
            /* data is AAC packet */
            enc_para = p_data[16] | (p_data[17] << 8) | (p_data[18] << 16);

            if (bt_music_cfg.enc_para != enc_para) {
                if (g_bt_music_enc_mode) {
                    g_bt_music_enc_mode_is_changing = 1;
                }

                /* mark a2dp codec mode is aac */
                g_bt_music_enc_mode = 1;

                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_music_init_aac_decoder_params %d %d %d %d", bt_music_cfg.enc_para, enc_para, g_bt_music_enc_mode,
                             g_bt_music_enc_mode_is_changing);

                bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);
                init_ret = bt_music_init_aac_decoder_params(enc_para, p_data, len);
                if (init_ret) {
                    //bt_music_close_codec();
                    bt_music_open_codec(bt_music_cfg.sample_rate);
                }
            }
        }
#endif // AAC_CODEC_ENABLE
    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
        uint8_t           head_size    = sizeof(lhdc_avdtp_pkt_t);
        lhdc_avdtp_pkt_t *pkt          = p_data;
        uint16_t          payload_size = len - head_size;
        lhdc_config_t    *lhdc         = lhdc_get_config();

        if (!is_lhdc_player_init()) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdc init at RX FRAME");
            lhdc_dec_player_init();
            if (g_bt_music_enc_mode) {
                g_bt_music_enc_mode_is_changing = 1;
            }

            /* mark a2dp codec mode is lhdc */
            g_bt_music_enc_mode = 2;
            sample_rate         = lhdc->dec_param.srate;
        }

        lhdc_frame_Info_t frame_info;
        int32_t           ret = lhdcv5_util_dec_fetch_frame_info((uint32_t *)lhdc->dec_mem, &p_data[head_size], (uint32_t)payload_size, &frame_info);
        if (ret) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdc frame error %d %d", len, payload_size);
            return 0;
        } else if (lhdc->frame_info.frame_len != frame_info.frame_len) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] nc_rx_lhdc frame len change, len: %d, %d, srate: %d, sample: %d", frame_info.frame_len,
                         lhdc->frame_info.frame_len, lhdc->dec_param.srate, g_codec_cfg.sample_rate);

            lhdc->frame_info.frame_len = frame_info.frame_len;

        } else if (LHDC_FRAME_MAX_SIZE < frame_info.frame_len + 2) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdc frame len too long, pls adjust the size of macro :LHDC_FRAME_MAX_SIZE %d", frame_info.frame_len);
            return 0;
        }
#endif
    } else if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        /* 60 02 04 01 80 60 01 41  02 36 ee c5 00 00 00 00
         * 05 9c bd 35 b5 00 95 32  21 00 95
         * SBC packet: L2CAP + AVDTP
         */

#if RECV_A2DP_PACKET_COUNT_LOG
        int             count_diff = 0;
        static uint16_t seq_last   = 0;
        uint16_t        seq        = p_data[2] << 8 | p_data[3];
        count_diff                 = seq - seq_last;
        if (count_diff != 1) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "seq num current last %d %d", seq, seq_last);
        }
        seq_last = seq;
#endif

        if (p_data[13] != 0x9c) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: not SBC packet, copy last frame>");
            return BT_AUDIO_PACKET_INVALID;
        }

        enc_para = p_data[13] | (p_data[14] << 8) | (p_data[15] << 16);
        //        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "enc_para: %d, bt_music_cfg.enc_para: %d",
        //                     enc_para, bt_music_cfg.enc_para);


        if (bt_music_cfg.enc_para != enc_para) {
            if (g_bt_music_enc_mode) {
                g_bt_music_enc_mode_is_changing = 1;
            }

            /* mark a2dp codec mode is sbc */
            g_bt_music_enc_mode = 3;
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_music_init_sbc_decoder_params %d %d %d %d", bt_music_cfg.enc_para, enc_para, g_bt_music_enc_mode,
                         g_bt_music_enc_mode_is_changing);

            //bt_audio_task_register_run_cb(NULL, 1);

            bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);
            init_ret = bt_music_init_sbc_decoder_params(enc_para, p_data, len);
            if (init_ret) {
                //bt_music_close_codec();
                bt_music_open_codec(bt_music_cfg.sample_rate);
            }
        }
    }

    /* codec mode changing is done when code runs here */
    g_bt_music_enc_mode_is_changing = 0;

    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if (AAC_CODEC_ENABLE)
        uint32_t aac_pkt_len;

        aac_pkt_len = len - 12;

        if (g_aac_frame_copy == 0 && (0x47 != p_data[12] || aac_pkt_len > (uint32_t)(bt_music_cfg.enc_buf_size - 2))) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# aac enc rec err,", aac_pkt_len, bt_music_cfg.enc_buf_size, 0, 0);
            ret = BT_AUDIO_PACKET_LEN_INVALID;
        } else {
            if (g_aac_frame_copy) {
                g_aac_frame_copy--;
                wptr   = bt_music_cfg.enc_buf_wptr ? bt_music_cfg.enc_buf_wptr - 1 : bt_music_cfg.enc_buf_num - 2;
                p_last = g_bt_music_enc_buf_ptr + bt_music_cfg.aac_offset[wptr];

                len_last = bt_music_cfg.aac_len[wptr];
                if (len_last > bt_music_cfg.enc_buf_size) {
                    len_last = bt_music_cfg.enc_buf_size;
                }

                bt_music_push_frames_to_encoded_buffer(p_data[3], p_last, len_last);
            } else {
                bt_music_push_frames_to_encoded_buffer(p_data[3], p_data + 12, aac_pkt_len);
            }
        }
#endif
    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
        if (!is_lhdc_player_init()) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdc error");
            return 0;
        }

        lhdc_avdtp_pkt_t *avdtp_pkt      = (lhdc_avdtp_pkt_t *)p_data;
        uint8_t           avdtp_pkt_size = sizeof(lhdc_avdtp_pkt_t);

        bt_music_push_frames_to_encoded_buffer(avdtp_pkt->lhdc_header.sequence_num, p_data + avdtp_pkt_size, len - avdtp_pkt_size);
#endif
    } else if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        if (p_data[12] * bt_music_cfg.sbc_framesize == len - 13) {
            bt_music_push_frames_to_encoded_buffer(p_data[3], p_data + 13, len - 13);
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: receive packet length error> %d %d %d %d", bt_music_cfg.enc_buf_size, bt_music_cfg.sbc_framesize, p_data[12],
                         len - 13);

            ret = BT_AUDIO_PACKET_LEN_INVALID;
        }
    }

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster())
#endif
    {
        uint32_t cur_tick       = clock_time();
        uint32_t tdiff          = cur_tick - g_a2dp_last_rev_tick;
        uint32_t tdiff_us       = tdiff / TICK_PER_US;
        uint32_t tdiff1         = cur_tick - g_a2dp_rev_start_tick;
        uint32_t tdiff1_us      = tdiff1 / TICK_PER_US;
        uint32_t tspan_a2dp_cnt = tdiff1_us / ((1000000 / 44100) * 128 * 5);

        g_a2dp_rev_cnt++;

        if (tdiff1_us > 600000 && g_a2dp_rev_start_tick) {
            uint32_t audio_buff = (g_a2dp_rev_cnt * 5 * 128) * 1000 / 44100;

            static int dbg_cnt = 5;

            dbg_cnt--;
            if (dbg_cnt == 0) {
                dbg_cnt = 2;
                if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
                    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "t_buff/time: %d ms/%d ms, rcv_cnt/cnt: %d/%d, enc: %d, last_diff:%d ms, rcv_ok:%d", audio_buff,
                                 tdiff1_us / 1000, g_a2dp_rev_cnt, tspan_a2dp_cnt, bt_music_get_num_of_enc_buff_avail(), tdiff_us / 1000, g_a2dp_rcv_ok);
                }
            }

            if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
                if (g_a2dp_rev_cnt >= tspan_a2dp_cnt) {
                    g_a2dp_rcv_ok++;
                } else {
                    g_a2dp_rcv_ok = 0;
                }

                if (g_a2dp_rcv_ok >= 5) {
                    bt_music_set_latency_mode(BT_LATENCY_NORMAL);
                    g_a2dp_rcv_ok = 0;
                }
            }

            g_a2dp_rev_cnt        = 0;
            g_a2dp_last_rev_tick  = 0;
            g_a2dp_rev_start_tick = 0;
        } else {
            if (g_a2dp_rev_start_tick == 0) {
                g_a2dp_rev_start_tick = cur_tick;
            }
            g_a2dp_last_rev_tick = cur_tick;
        }
    }
#endif

#ifdef SL16_bt_music_st
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x5000);
#endif

    if (bt_music_cfg.sync_enc_buf_num > 0) {
        if (bt_music_cfg.sync_enc_buf_num * bt_music_cfg.samples_per_frame > (bt_music_cfg.enc_buf_num * bt_music_cfg.samples_per_frame + 200)) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "FLG_RATE_RESET on buffer full %d %d %d %d", bt_music_cfg.enc_buf_num, bt_music_cfg.enc_buf_mute_num,
                         bt_music_cfg.sync_play, bt_music_cfg.sync_enc_buf_num);

#if LHDC_CODEC_ENABLE
            if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                lhdc_ready_time_reset();
            }
#endif

#ifdef SL16_bt_music_st
            log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6200);
#endif
            bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);
        }
    }

    return ret;
}

/*
 * @brief   Update frames num and id
 * @param[in]   id  - A2DP packet serial number
 * @param[in]   frames  - Number of frames
 * @return none.
 */
audio_ram_code void bt_music_update_frames_with_id(uint8_t id, uint8_t frames)
{
    int16_t wptr;
    //AUDIO_GLOBAL_INT_DISABLE();
    tlkmdi_audio_enterCritical();

#if TWS_AUDIO_PATH_GPIO_DEBUG
//    gpio_write(GPIO_PB0, 0);
//    gpio_write(GPIO_PB0, 1);
//    gpio_write(GPIO_PB0, 0);
#endif
    bt_music_cfg.sync_enc_buf_num += frames;

#ifdef SL16_bt_enc_buf_num
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_enc_buf_num, bt_music_cfg.sync_enc_buf_num);
#endif

#if RECV_A2DP_PACKET_COUNT_LOG
#define CALCULATE_SECOND 1
    static bool     start_flag = true;
    static uint32_t time_tick;
    static uint16_t frame_number = 0;

    frame_number += frames;

    if (start_flag) {
        start_flag = false;
        time_tick  = clock_time();
    }

    if (clock_time_exceed(time_tick, CALCULATE_SECOND * 1000 * 1000)) {
        uint16_t theo_val = bt_music_cfg.sample_rate * CALCULATE_SECOND / 128;
        int16_t  diff     = frame_number - theo_val;
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "packer count actual %d theo %d diff %d", frame_number, theo_val, diff);
        time_tick    = clock_time();
        frame_number = 0;
    }
#endif

    wptr = bt_music_cfg.enc_buf_wptr + frames;
    if (wptr >= bt_music_cfg.enc_buf_num) {
        bt_music_cfg.enc_buf_wptr = wptr - bt_music_cfg.enc_buf_num;
    } else if (wptr < 0) {
        bt_music_cfg.enc_buf_wptr = wptr + bt_music_cfg.enc_buf_num;
    } else {
        bt_music_cfg.enc_buf_wptr = wptr;
    }

    bt_music_cfg.enc_buf_id = id;

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "+++enc_vail:%d, w_ptr:%d, r_ptr:%d, enc_num:%d",
				 bt_music_get_num_of_enc_buff_avail(),
				 bt_music_cfg.enc_buf_wptr,
				 bt_music_cfg.enc_buf_rptr,
				 bt_music_cfg.enc_buf_num);
    */

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
    static int dbg_cnt = 30;

    dbg_cnt--;
    if (dbg_cnt == 0) {
        dbg_cnt = 200;

        uint32_t enc_buff_cnt = bt_music_get_num_of_enc_buff_avail();
        uint32_t cur_samples  = enc_buff_cnt * bt_music_cfg.samples_per_frame;

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "+++ mode:%d, enc:%d, buf_total:%d, sample_ref/cur:%d/%d", bt_music_cfg.latency_mode, enc_buff_cnt,
                     bt_music_cfg.enc_buf_num, bt_music_cfg.sample_avg_ref, cur_samples);
    }
#endif

    tlkmdi_audio_leaveCritical();
    //AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief       Push frames to encoded buffer
 * @param[in]   id  - A2DP packet serial number
 * @param[in]   p   - Data pointer
 * @param[in]   len - Data length
 * @return      BT_AUDIO_SUCCESS is successful, others are failed.
 */
uint8_t bt_music_push_frames_to_encoded_buffer(uint8_t id, uint8_t *p, uint16_t len)
{
    uint8_t  crc_len = 0;
    uint8_t  ret     = BT_AUDIO_SUCCESS;
    uint16_t recv_sbc_frame_num;
    uint16_t available_enc_buf;
    uint16_t skip_sbc_frame_num;
    // uint32_t irq;
    uint16_t enc_buf_remain_num, sbc_frame_num;
    uint8_t *p_des;
    uint8_t  i;
    uint32_t crc;
    uint8_t *p_src = p;
    // uint8_t *p_data = p;

    available_enc_buf = bt_music_get_num_of_enc_buff_avail();

#if LHDC_CODEC_ENABLE
    uint8_t        recv_lhdc_frame_num = 0;
    lhdc_config_t *lhdc                = lhdc_get_config();

    if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        recv_lhdc_frame_num = len / ((lhdc->frame_info.frame_len - 2) * 2 + 2);
    }

    if (available_enc_buf >= bt_music_cfg.enc_buf_num - recv_lhdc_frame_num) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] music_buff_overflow, drop ERROR %d %d %d %d", recv_lhdc_frame_num, available_enc_buf, bt_music_cfg.enc_buf_num,
                     bt_music_cfg.sync_enc_buf_num);
        return 0;
    }
#endif

    if (available_enc_buf >= bt_music_cfg.enc_buf_num - 1) {
#if TWS_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PC1, 0);
        gpio_write(GPIO_PC1, 1);
        gpio_write(GPIO_PC1, 0);
#endif

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<bt_music_buff_overflow, drop, id:%d, %d %d %d %d, available_enc_buf: %d, enc_buf_num: %d", bt_music_cfg.enc_buf_id,
                     bt_music_cfg.buff_low | (bt_music_cfg.buff_high << 8), bt_music_cfg.sync_ppm, bt_music_cfg.sample_avg, bt_music_cfg.sample_avg_last, available_enc_buf,
                     bt_music_cfg.enc_buf_num);
#if TWS_ENABLE
        //btif_audio_power_control(POWER_INC);
#endif
        return BT_AUDIO_SUCCESS;
    }

    if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        recv_sbc_frame_num = len / bt_music_cfg.sbc_framesize;

        /*
        tlkapi_trace(BT_AUDIO_DBG_FLAG,
                     BT_AUDIO_DBG_SIGN,
                     "sbc rcv, len:%d, fsize:%d, num:%d", len, bt_music_cfg.sbc_framesize, recv_sbc_frame_num);
         */

        if (len % bt_music_cfg.sbc_framesize) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: encoder frame length error> %d", len);
            ret = BT_AUDIO_LENGTH_INVALID;
        } else {
            AUDIO_GLOBAL_INT_DISABLE();

            if (bt_music_cfg.skip_samples) {
                skip_sbc_frame_num = bt_music_cfg.skip_samples / bt_music_cfg.samples_per_frame;

                if (recv_sbc_frame_num > skip_sbc_frame_num) {
                    bt_music_cfg.skip_samples = 0;
                    recv_sbc_frame_num -= skip_sbc_frame_num;
                    p += skip_sbc_frame_num * bt_music_cfg.sbc_framesize;
                    len -= skip_sbc_frame_num * bt_music_cfg.sbc_framesize;
                } else {
                    bt_music_cfg.skip_samples -= recv_sbc_frame_num * bt_music_cfg.samples_per_frame;
                    AUDIO_GLOBAL_INT_RESTORE();
                    bt_music_update_frames_with_id(id, 0);

                    return BT_AUDIO_SUCCESS;
                }
            }

            AUDIO_GLOBAL_INT_RESTORE();

            crc_len            = bt_music_cfg.sbc_framesize;
            enc_buf_remain_num = bt_music_cfg.enc_buf_num - bt_music_cfg.enc_buf_wptr;
            sbc_frame_num      = enc_buf_remain_num >= recv_sbc_frame_num ? recv_sbc_frame_num : enc_buf_remain_num;

            if (enc_buf_remain_num > 0) {
                p_des = g_bt_music_enc_buf_ptr + bt_music_cfg.enc_buf_wptr * bt_music_cfg.enc_buf_size;

                for (i = 0; i < sbc_frame_num; i++) {
                    tmemcpy(p_des + i * bt_music_cfg.enc_buf_size, p, bt_music_cfg.sbc_framesize);
                    p += bt_music_cfg.sbc_framesize;
                }

                enc_buf_remain_num = recv_sbc_frame_num - sbc_frame_num;
            }

            if (enc_buf_remain_num) {
                for (i = 0; i < enc_buf_remain_num; i++) {
                    tmemcpy(g_bt_music_enc_buf_ptr + i * bt_music_cfg.enc_buf_size, p, bt_music_cfg.sbc_framesize);
                    p += bt_music_cfg.sbc_framesize;
                }
            }

            bt_music_update_frames_with_id(id, recv_sbc_frame_num);
        }
    } else if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if (AAC_CODEC_ENABLE)
        uint8_t aac_wptr_last = 0;
        uint8_t overflow      = 0;

        if (bt_music_cfg.skip_samples) {
            AUDIO_GLOBAL_INT_DISABLE();

            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: skip encoder frame> %d %d", bt_music_cfg.skip_samples, id);

            if (bt_music_cfg.samples_per_frame >= bt_music_cfg.skip_samples) {
                bt_music_cfg.skip_samples = 0;
            } else {
                bt_music_cfg.skip_samples -= bt_music_cfg.samples_per_frame;
            }

            AUDIO_GLOBAL_INT_RESTORE();
            bt_music_update_frames_with_id(id, 0);

            return BT_AUDIO_SUCCESS;
        }

        crc_len = len;

        if (bt_music_cfg.enc_buf_wptr == bt_music_cfg.enc_buf_rptr) {
            bt_music_cfg.aac_offset_cur = 0;
        }

        if (available_enc_buf > 8 && bt_music_cfg.enc_buf_mute_num == 0 && bt_music_cfg.aac_offset_cur < bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr] &&
            bt_music_cfg.aac_offset_cur + len >= bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr]) {
            overflow = 1;
        } else {
            overflow = 0;
        }

        if (overflow == 0 && bt_music_cfg.aac_offset_cur + len > g_bt_music_buffer_size) {
            bt_music_cfg.aac_offset_cur = 0;

            if (available_enc_buf > 8 && bt_music_cfg.enc_buf_mute_num == 0 && len >= bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr]) {
                overflow = 1;
            }
        }

        if ((len <= bt_music_cfg.enc_buf_size && 0x47 == p[0]) && overflow == 0) {
            bt_music_cfg.aac_len[bt_music_cfg.enc_buf_wptr] = len;

            bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_wptr] = bt_music_cfg.aac_offset_cur;

            tmemcpy(g_bt_music_enc_buf_ptr + bt_music_cfg.aac_offset_cur, p, len);

            bt_music_cfg.aac_offset_cur += len;
        } else {
            aac_wptr_last = bt_music_cfg.enc_buf_wptr ? bt_music_cfg.enc_buf_wptr - 1 : bt_music_cfg.enc_buf_num - 2;

            bt_music_cfg.aac_len[bt_music_cfg.enc_buf_wptr] = bt_music_cfg.aac_len[aac_wptr_last];

            bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_wptr] = bt_music_cfg.aac_offset[aac_wptr_last];

            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# aac_overflow, duplicate, %d %d %d %d %d %d %d", bt_music_cfg.enc_buf_id, aac_wptr_last, bt_music_cfg.enc_buf_rptr,
                         bt_music_cfg.aac_len[aac_wptr_last], bt_music_cfg.aac_offset[aac_wptr_last], bt_music_cfg.aac_len[bt_music_cfg.enc_buf_rptr],
                         bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr]);
        }

        bt_music_update_frames_with_id(id, 1);
#endif
    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
        if (len % (((lhdc->frame_info.frame_len - 2) * 2) + 2)) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] <enc: lhdc encoder frame length error> %d %d", len, lhdc->frame_info.frame_len);
            ret = 2;
        } else {
            enc_buf_remain_num = bt_music_cfg.enc_buf_num - bt_music_cfg.enc_buf_wptr;

            uint8_t  frame_num = enc_buf_remain_num >= recv_lhdc_frame_num ? recv_lhdc_frame_num : enc_buf_remain_num;
            uint16_t frame_len = 0;

            if (enc_buf_remain_num > 0) {
                p_des = g_bt_music_enc_buf_ptr + bt_music_cfg.enc_buf_wptr * bt_music_cfg.enc_buf_size;
                for (i = 0; i < frame_num; i++) {
                    frame_len = lhdc->frame_info.frame_len;
                    tmemcpy(&p_des[i * bt_music_cfg.enc_buf_size], &frame_len, 2);
                    lhdc_mus_frame_adjust(p, p, frame_len);
                    tmemcpy(p_des + i * bt_music_cfg.enc_buf_size + 2, p, lhdc->frame_info.frame_len);
                    p += (((lhdc->frame_info.frame_len - 2) * 2) + 2);
                }

                enc_buf_remain_num = recv_lhdc_frame_num - frame_num;
            }

            if (enc_buf_remain_num) {
                for (i = 0; i < enc_buf_remain_num; i++) {
                    frame_len = lhdc->frame_info.frame_len;
                    tmemcpy(&g_bt_music_enc_buf_ptr[i * bt_music_cfg.enc_buf_size], &frame_len, 2);

                    lhdc_mus_frame_adjust(p, p, frame_len);

                    tmemcpy(g_bt_music_enc_buf_ptr + i * bt_music_cfg.enc_buf_size + 2, p, lhdc->frame_info.frame_len);

                    p += (((lhdc->frame_info.frame_len - 2) * 2) + 2);
                }
            }
            bt_music_update_frames_with_id(id, recv_lhdc_frame_num);
        }
#endif
    }

    if (bt_music_cfg.crc_mute && bt_music_cfg.crc_func && crc_len) {
        crc = bt_music_cfg.crc_func(0, p_src, crc_len);
        bt_music_enc_crc_check(crc, crc_len);
    }

    return ret;
}
#endif
