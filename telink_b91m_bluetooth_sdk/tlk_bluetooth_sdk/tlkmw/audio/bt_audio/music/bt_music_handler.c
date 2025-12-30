/********************************************************************************************************
 * @file    bt_music_handler.c
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
#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/audio/tlkmw_audio.h"

#include "common/utility.h"
#include "drivers.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"

#if AUDIO_TWS_MODE
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"
#endif

#if LHDC_CODEC_ENABLE
#include "tlkmw/aud/cc_tws/bt_audio/bt_music_lhdc.h"
#endif

#ifndef BT_BLE_CALL_MUSIC_MIX_VOL
/** bt_music_vol_table_android, 6 level */
#define BT_BLE_CALL_MUSIC_MIX_VOL 150
#endif

#if AUDIO_TWS_MODE
#define IN_MUSIC_MODE async_tws_get_status_flag(TSYNC_FLAG_MUSIC)
#else
#define IN_MUSIC_MODE audio_codec_flag_get(CODEC_FLAG_MUSIC)
#endif

#ifndef BT_MUSIC_ENC_BUFF_LOCAL
#define BT_MUSIC_ENC_BUFF_LOCAL 1
#endif

#ifndef AUDIO_LOW_LATENCY_PLC
#define AUDIO_LOW_LATENCY_PLC 0
#else

#define AUDIO_LOW_LATENCY_PLC BIT(15)
#define AUDIO_LOW_LATENCY_PLC (BIT(AUDIO_LOW_LATENCY_PLC) | 13)

#endif /* AUDIO_LOW_LATENCY_PLC */

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
uint16_t g_bt_music_buffer_size = BT_MUSIC_ENC_FIFO_SIZE;
uint8_t  g_low_latency_enable   = 0;

/** BT MUSIC LowLatency buffer size */
uint16_t          g_bt_music_ll_enc_buff_size          = 0;
volatile uint32_t ppm_rptr_offset                      = 0;
uint32_t          g_aac_frame_copy                     = 0;
volatile uint8_t  g_bt_music_enc_mode                  = 0;
volatile uint8_t  g_bt_music_enc_mode_is_changing      = 0;
uint8_t          *g_bt_music_enc_buf_ptr               = NULL;
volatile uint8_t  bt_music_dec_update_sync_enc_buf_num = 0;

bt_music_cfg_t bt_music_cfg = {0};

extern void bt_music_gpio_toggle(uint8_t times);

/**
 * @brief       Clear audio flag
 * @param[in]   None
 * @return      None
 */
void bt_music_clear_audio_flag(void)
{
    audio_codec_flag_set(CODEC_FLAG_MUSIC, 0);
}

/**
 * @brief       Set encode buffer
 * @param[in]   p_buff - Buffer pointer
 * @return      BT_MUSIC_ENC_FIFO_SIZE
 */
uint16_t bt_music_set_enc_buffer(uint8_t *p_buff)
{
    g_bt_music_enc_buf_ptr = p_buff;

    return BT_MUSIC_ENC_FIFO_SIZE;
}

/**
 * @brief       Get encode buffer
 * @param[in]   None
 * @return      encode buffer pointer
 */
uint8_t *bt_music_get_enc_buffer(void)
{
    return g_bt_music_enc_buf_ptr;
}

/**
 * @brief       Set ll encode buffer size
 * @param[in]   size_bytes - buffer size in bytes
 * @return      None
 */
void bt_music_ll_enc_buff_size_set(uint16_t size_bytes)
{
    g_bt_music_ll_enc_buff_size = size_bytes;
}

/**
 * @brief       Get ll encode buffer size
 * @param[in]   None
 * @return      ll encode buffer size
 */
uint16_t bt_music_ll_enc_buff_size_get(void)
{
    return g_bt_music_ll_enc_buff_size;
}

/**
 * @brief set enc buffer parameter.
 * @param[in]   num_of_buff - enc buff number SBC-86 AAC-18
 * @param[in]   size - buff size	SBC-sbc_framesize AAC-1200
 * @param[in]   samples - sample number in one frame SBC-128 AAC-1024
 * @returns none
 */
void bt_music_set_param(uint16_t num_of_buff, uint16_t size, uint16_t samples_per_frame)
{
    bt_music_cfg.enc_buf_num       = num_of_buff;
    bt_music_cfg.enc_buf_size      = size > SBC_MAX_FRAMESIZE ? size : SBC_MAX_FRAMESIZE;
    bt_music_cfg.sbc_framesize     = size;
    bt_music_cfg.samples_per_frame = samples_per_frame;
}

/**
 * @brief set dump mask
 * @param[in]   mask - mask value
 * @returns none
 */
void bt_music_set_dump_mask(uint16_t mask)
{
    bt_music_cfg.dump_mask = mask;
}

/**
 * @brief set ppm value when debugging ppm function
 * @param[in]   offset - ppm value
 * @returns none
 */
void bt_music_ppm_debug(int16_t offset)
{
    bt_music_cfg.ppm_debug = offset;
}

/**
 * @brief set low latency mode
 * @param[in]   enable - 0, off; 1, on
 * @returns none
 */
void bt_music_set_low_latency_mode(uint8_t enable)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_music_set_low_latency_mode %d", enable);

    if (enable) {
        //tws_tone_play(BT_AUDIO_GAME_MODE);
        //tws_tone_play(LOW_POWER_TONE);
        g_bt_music_buffer_size = BT_MUSIC_ENC_FIFO_SIZE;

        if (g_low_latency_enable == 0) {
            g_low_latency_enable = 1;
            bt_music_cfg.reset   = 1;
        }

    } else {
        //tws_tone_play(BT_AUDIO_MUSIC_MODE);
        //tws_tone_play(LOW_POWER_TONE);
        g_bt_music_buffer_size = BT_MUSIC_ENC_FIFO_SIZE;

        if (g_low_latency_enable == 1) {
            g_low_latency_enable = 0;
            bt_music_cfg.reset   = 1;
        }
    }
}

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
/**
 * @brief       Set latency mode
 * @param[in]   mode - Latency mode
 * @return      None
 */
void bt_music_set_latency_mode(uint8_t mode)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "set latency_mode %d", mode);

    if (mode == BT_LATENCY_NORMAL) {
        if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
            bt_music_cfg.reset = 1;
            //tone_play(TONE_LOW_POWER);
        }
    } else if (mode == BT_LATENCY_PKTLOSS) {
        if (bt_music_cfg.latency_mode == BT_LATENCY_NORMAL) {
            bt_music_cfg.reset = 1;
            //tone_play(TONE_POWER_OFF);
        }
    }

    bt_music_cfg.latency_mode = mode;
}
#endif

/**
 * @brief       Check if low latency mode is enabled
 * @param[in]   None
 * @return      1 if low latency mode is enabled, 0 otherwise
 */
uint8_t bt_music_is_low_latency_mode(void)
{
    return g_low_latency_enable;
}

/**
 * @brief register CRC function
 * @param[in]   p - CRC function
 * @returns none
 */
void bt_music_register_crc_callback(void *p)
{
    bt_music_cfg.crc_func = p;
    bt_music_cfg.crc_mute = 0;
}

/**
 * @brief register decode function
 * @param[in]   p - decode function
 * @returns none
 */
void bt_music_register_dec_callback(void *p)
{
    bt_music_cfg.dec_func = p;
}

/**
 * @brief       Check music status
 * @param[in]   None
 * @return      0
 */
uint8_t bt_music_check_status(void)
{
    if (bt_music_cfg.reset) {
        bt_music_cfg.reset = 0;

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
        if (bt_music_cfg.latency_mode == BT_LATENCY_NORMAL) {
            g_bt_music_buffer_size = 1024 * 12;
        } else if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
            g_bt_music_buffer_size = 1024 * 45;
        }
#endif

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "$$$ latency_mode: %d, size: %d", bt_music_cfg.latency_mode, g_bt_music_buffer_size);

#ifdef SL16_bt_music_st
        log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6500);
#endif
        bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);

        return 0;
    }

    if (!bt_music_cfg.sync_play && bt_music_cfg.enc_buf_wptr && audio_codec_flag_get(CODEC_FLAG_MUSIC)) {
        // gpio_set_high_level(GPIO_CHN2);
        bt_music_play_init();

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "state latency_mode %d", bt_music_cfg.latency_mode);

#if TLKMW_INTERPHONE_EN
        if (!tlkmdi_interphone_is_busy())
#endif
        {
            /* initialize tcodec.play_rptr and tcodec.play_wptr, tcodec.samples_played */
            tlkdrv_codec_sync_play_samples(bt_music_cfg.sync_samples);
        }
        // gpio_set_low_level(GPIO_CHN2);

#if AUDIO_TWS_MODE
        bt_music_cfg.sync_play = BT_MUSIC_SYNC_PLAY_INIT;
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_MUSIC_SYNC_PLAY_INIT %d %d %d %d", bt_music_cfg.enc_buf_id, bt_music_cfg.enc_buf_mute_num, bt_music_cfg.sync_play);
#if 0
        if (tlkmdi_bt_tpt_isSingle()) {
            bt_music_cfg.sync_play = BT_MUSIC_SYNC_PLAY_START + 1;
            tlkapi_trace(BT_AUDIO_DBG_FLAG,
                         BT_AUDIO_DBG_SIGN,
                         "BT_MUSIC_SYNC_PLAY_START, id: %d, mute: %d, sync_play: %d",
                         bt_music_cfg.enc_buf_id,
                         bt_music_cfg.enc_buf_mute_num,
                         bt_music_cfg.sync_play);
        } else {
            bt_music_cfg.sync_play = BT_MUSIC_SYNC_PLAY_INIT;
            tlkapi_trace(BT_AUDIO_DBG_FLAG,
                         BT_AUDIO_DBG_SIGN,
                         "BT_MUSIC_SYNC_PLAY_INIT %d %d %d %d",
                         bt_music_cfg.enc_buf_id,
                         bt_music_cfg.enc_buf_mute_num,
                         bt_music_cfg.sync_play);
        }
#endif
#else
        bt_music_cfg.sync_play = BT_MUSIC_SYNC_PLAY_START + 1;
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT_MUSIC_SYNC_PLAY_START, id: %d, mute: %d, sync_play: %d", bt_music_cfg.enc_buf_id, bt_music_cfg.enc_buf_mute_num,
                     bt_music_cfg.sync_play);
#endif

    } else if (!(bt_music_cfg.sync_play & 0x80) && bt_music_cfg.sync_play >= BT_MUSIC_SYNC_PLAY_START) {
        bt_music_cfg.sync_play++;
    }

    if (bt_music_cfg.sync_play < BT_MUSIC_SYNC_PLAY_START) {
        return 0;
    }
    return 0;
}

/**
 * @brief set music play status
 * @param[in]   status - IDLE INIT START
 * @returns none
 */
audio_ram_code void bt_music_set_play_status(uint8_t status)
{
    // uint32_t irq;

    if (BT_MUSIC_SYNC_PLAY_IDLE == status) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<bt_music_play_set_status> %d", bt_music_cfg.sync_play);

        AUDIO_GLOBAL_INT_DISABLE();

        bt_music_cfg.aac_offset_cur               = 0;
        bt_music_cfg.enc_buf_rptr                 = 0;
        bt_music_cfg.enc_buf_wptr                 = 0;
        bt_music_cfg.enc_para                     = 0;
        bt_music_cfg.buff_high                    = 0;
        bt_music_cfg.buff_low                     = 256;
        bt_music_cfg.seconds                      = 0;
        bt_music_cfg.sync_ppm_cur                 = 0;
        bt_music_cfg.sync_ppm                     = 0;
        bt_music_cfg.sync_countdown               = 0;
        bt_music_cfg.rate_change                  = 0;
        bt_music_cfg.enc_buf_mute_num             = 0;
        bt_music_cfg.sync_play                    = 0;
        bt_music_cfg.skip_samples                 = 0;
        bt_music_cfg.reset                        = 0;
        bt_music_cfg.sync_ppm_set_pending         = 0;
        bt_music_cfg.sync_enc_buf_num             = 0;
        bt_music_cfg.inc_peer_tx_power            = 0;
        bt_music_cfg.inc_peer_tx_power_available  = 0;
        bt_music_cfg.inc_peer_tx_power_is_pending = 0;

#if EQ_ENABLE
        tlkalg_eq_reset_all_state();
        if (audio_codec_flag_get(CODEC_FLAG_EQ_MUSIC_EN)) {
            audio_codec_flag_set(CODEC_FLAG_MUSIC_EQ_STATR, 0);
        }
#endif
        AUDIO_GLOBAL_INT_RESTORE();
    }

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "=== bt_music_set_play_status %d", bt_music_cfg.latency_mode);
}

/**
 * @brief   Get encoded frames, push data from enc buffer to play buffer
 *          Get ppm value and init ppm algorithm
 * @param[in]   num      - frame num
 * @param[in]   samples  - one frame include samples, A2DP: sbc->128, aac->1024
 * @return ppm_set
 */
audio_ram_code uint8_t bt_music_get_encoded_frame(uint8_t num, uint16_t samples)
{
    //AUDIO_GLOBAL_INT_DISABLE();
    tlkmdi_audio_enterCritical();
    uint8_t  ppm_set   = 0;
    uint16_t rptr_next = bt_music_cfg.enc_buf_rptr + num;

    if (rptr_next >= bt_music_cfg.enc_buf_num) {
        rptr_next -= bt_music_cfg.enc_buf_num;
    }

    if (bt_music_cfg.skip_samples == 0) {
        bt_music_cfg.tick_skip = 0;
    }

    bt_music_cfg.skip_frames  = 0;
    bt_music_cfg.enc_buf_rptr = rptr_next;
    bt_music_cfg.sync_enc_buf_num -= num;

    bt_music_dec_update_sync_enc_buf_num = 1;

    /* decoded samples num */
    bt_music_cfg.sync_samples += samples;

#ifdef SL16_bt_enc_buf_num
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_enc_buf_num, bt_music_cfg.sync_enc_buf_num);
#endif

    //    gpio_write(GPIO_PB0, 0);
    //    gpio_write(GPIO_PB0, 1);
    //    gpio_write(GPIO_PB0, 0);

    /*
    if (samples != 139 && samples != 140) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG,
                     BT_AUDIO_DBG_SIGN,
                     "+++ sync_samples: %d, samples: %d",
                     bt_music_cfg.sync_samples,
                     samples);
    }
*/

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "---enc_vail:%d, codec_samples:%d",
				 bt_music_get_num_of_enc_buff_avail(),
				 tlkdrv_codec_get_spk_avail_samples());
*/

    if (bt_music_cfg.sync_countdown) {
        bt_music_cfg.sync_countdown--;
        if (bt_music_cfg.sync_countdown == 0) {
            ppm_set = 1;
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "=============bt_music_cfg.sync_countdown ==========");
        }
    }

    /* For tws slave's ppm_set bug */
    if (bt_music_cfg.sync_ppm_set_pending) {
        bt_music_cfg.sync_ppm_set_pending = 0;
        ppm_set                           = 1;
    }

    if (bt_music_cfg.enc_buf_mute_num) {
        bt_music_cfg.enc_buf_mute_num--;
    }

    uint16_t avail_buf = bt_music_get_num_of_enc_buff_avail();

#ifdef SL16_avail_buf
    log_b16_irq(1, SL16_avail_buf, avail_buf);
#endif


    //add for avail buffer
    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        if (avail_buf < 3) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "aac, avail_buf: %d, buf_num: %d", avail_buf, bt_music_cfg.enc_buf_num);

#ifdef SLET_ld_avail_none
            log_tick_irq(1, SLET_ld_avail_none);
#endif
        }
    } else {
        if (avail_buf < 10) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "underflow, sbc avail_buf: %d, buf_num: %d", avail_buf, bt_music_cfg.enc_buf_num);
#ifdef SLET_ld_avail_none
            log_tick_irq(1, SLET_ld_avail_none);
#endif
        }

        if (avail_buf > bt_music_cfg.enc_buf_num - 10 && bt_music_cfg.sync_enc_buf_num < bt_music_cfg.enc_buf_num + 200) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "overflow, sbc avail_buf: %d, buf_num: %d", avail_buf, bt_music_cfg.enc_buf_num);
        }
    }
    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "a2dp play, mute: %d, enc r/w:%d, %d, avail: %d",
                 bt_music_cfg.enc_buf_mute_num, bt_music_cfg.enc_buf_rptr, bt_music_cfg.enc_buf_wptr,
                 bt_music_get_num_of_enc_buff_avail());
*/

#if AUDIO_TWS_MODE
//bt_music_cfg.sync_frac = tlk_ppm_get_frac();
#endif

    //AUDIO_GLOBAL_INT_RESTORE();
    tlkmdi_audio_leaveCritical();

    if (ppm_set) {
#if TLKALG_PPM_SPK_ENABLE && !AUDIO_HD_HAC_EN
        int                    ppm_val        = bt_music_cfg.sync_ppm;
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        p_audio_alg_if->audio_alg_param_set(0, &ppm_val);
#endif
#if AUDIO_HD_HAC_EN
        tlkdrv_hac_set_ppm(g_hd_hac_asrc_buffer, bt_music_cfg.sync_ppm);
#endif
        bt_music_cfg.sync_ppm_cur = bt_music_cfg.sync_ppm;

#if AUDIO_TWS_MODE
        //        if (tph_headset_is_slave()) {
        //            bt_music_cfg.sync_frac = bt_music_cfg.sync_frac_peer;
        //        }
        //        tlk_ppm_set_frac(bt_music_cfg.sync_frac);
#endif

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<ppm set> ppm:%d, id:%d, samples:%d, tick:%d", bt_music_cfg.sync_ppm, bt_music_cfg.enc_buf_id,
                     bt_music_cfg.sync_samples, bt_music_cfg.sync_tick);
    }

    return ppm_set;
}

/**
 * @brief   Convert sbc parameter from a2dp form to common form and given to struct
 * @param[in]   data - sbc data in a2dp form
 * @returns sample rate
 */
uint8_t bt_music_parse_sbc_param(uint8_t *data)
{
    uint8_t  acl_sbc_bitpool, acl_sbc_block, acl_sbc_subband;
    uint16_t acl_sbc_fre, acl_sbc_framesize;

    /* read bitpool from data */
    acl_sbc_bitpool = data[2];

    /* read frequency from data */
    if ((data[1] & 0xc0) == 0) {
        acl_sbc_fre = 16000;
    } else if ((data[1] & 0xc0) == 0x40) {
        acl_sbc_fre = 32000;
    } else if ((data[1] & 0xc0) == 0x80) {
        acl_sbc_fre = 44100;
    } else {
        acl_sbc_fre = 48000;
    }

    /* read block length from data */
    if ((data[1] & 0x30) == 0) {
        acl_sbc_block = 4;
    } else if ((data[1] & 0x30) == 0x10) {
        acl_sbc_block = 8;
    } else if ((data[1] & 0x30) == 0x20) {
        acl_sbc_block = 12;
    } else {
        acl_sbc_block = 16;
    }

    /* read subbands from data */
    if ((data[1] & 0x01) == 0) {
        acl_sbc_subband = 4;
    } else {
        acl_sbc_subband = 8;
    }

    bt_music_cfg.sbc_channel_mode = data[1] & 0x0c;

    /* read channel mode from data&calculate frame length */
    if ((data[1] & 0x0c) == 0) {
        /* mono */
        acl_sbc_framesize = 4 + acl_sbc_subband / 2 + (acl_sbc_block * acl_sbc_bitpool) / 8;
    } else if ((data[1] & 0x0c) == 4) {
        /* dual-mode */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool) / 4;
    } else if ((data[1] & 0x0c) == 8) {
        /* stereo */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool) / 8;
    } else {
        /* joint-stereo */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool + acl_sbc_subband) / 8;
    }

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "sbc_channel_mode: %d, acl_sbc_framesize: %d, bit_pool: %d", bt_music_cfg.sbc_channel_mode, acl_sbc_framesize,
                 acl_sbc_bitpool);

    bt_music_cfg.sample_rate   = acl_sbc_fre;
    bt_music_cfg.sbc_framesize = acl_sbc_framesize;

    if ((uint16_t)(data[0] | (data[1] << 8)) == (uint16_t)(bt_music_cfg.enc_para & 0xffff)) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "< sbc bitpool changed > %d %d %d", bt_music_cfg.enc_para, data[2], acl_sbc_framesize);
        return 0;
    }

    return 1;
}

/**
 * @brief  Init SBC decoder params and set sample rate
 * @param[in] param - AAC params
 * @param[in] p_data - a2dp data
 * @param[in] len - a2dp data length
 * @returns sample_rate
 */
_attribute_no_inline_ uint16_t bt_music_init_sbc_decoder_params(uint32_t param, uint8_t *p_data, uint16_t len)
{
    (void)len;
    uint16_t buff_num;

    uint8_t ret = bt_music_parse_sbc_param(p_data + 13);
    if (ret == 0) {
        bt_music_cfg.enc_para = param;
        return 0;
    }

    buff_num = g_bt_music_buffer_size / SBC_MAX_FRAMESIZE;
    buff_num = buff_num >= BT_MUSIC_SBC_BUFF_NUM_MAX ? BT_MUSIC_SBC_BUFF_NUM_MAX : buff_num;

    bt_music_set_param(buff_num, bt_music_cfg.sbc_framesize, 128);

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "buff_num:%d, buf_size:%d, enc_buf_size:%d, enc_buf_num:%d, framesize:%d", buff_num, g_bt_music_buffer_size,
                 bt_music_cfg.enc_buf_size, bt_music_cfg.enc_buf_num, bt_music_cfg.sbc_framesize);

    bt_music_cfg.src_low_latency = 0;

    if (p_data[8] == 0xa1 && p_data[9] == 0xb2 && p_data[10] == 0xc3 && p_data[11] == 0xd4) {
        /* num of frame */
        bt_music_cfg.src_low_latency = p_data[12];
        if (bt_music_cfg.src_low_latency == 6) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: telink low latency source (40ms)>", p_data + 6, 10);
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<enc: telink low latency source (20ms)>", p_data + 6, 10);
        }
    }

#ifdef SL16_bt_music_st
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6100);
#endif

    bt_music_cfg.enc_para = param;

#if EQ_ENABLE
    //tlkalg_eq_reset_all_state();

    if (audio_codec_flag_get(CODEC_FLAG_EQ_MUSIC_EN)) {
        audio_codec_flag_set(CODEC_FLAG_MUSIC_EQ_STATR, 1);
    }
#endif

    return 1;
}

#if AAC_CODEC_ENABLE
/**
 * @brief  Init AAC decoder params and set sample rate.
 * @param[in] param - AAC params.
 * @param[in] p - a2dp parameter address.
 * @param[in] len - parameter length.
 * @returns sample_rate
 */
_attribute_no_inline_ int bt_music_init_aac_decoder_params(uint32_t param, uint8_t *p, uint16_t len)
{
    (void)len;
    uint8_t buffer_num;

    //uint8_t channel = ALG_CHANNEL_STEREO;
    // uint8_t *p_buff;

    if (audio_flag_get(FLG_BTC_TWS_EN)) {
#if AUDIO_TWS_MODE
        /*
        if (tph_headset_is_left()) {
            channel = ALG_CHANNEL_LEFT;
        } else {
            channel = ALG_CHANNEL_RIGHT;
        }
        //audio_alg_if[ALG_AAC_DEC].audio_alg_init(p_buff, channel);
        */
#endif
    } else {
    }

    bt_music_cfg.sample_rate     = (0x8C == p[17]) ? AUDIO_SAMPLE_RATE_48K : AUDIO_SAMPLE_RATE_441K;
    bt_music_cfg.src_low_latency = 0;

    buffer_num = g_bt_music_buffer_size / TMUSIC_AAC_BUFF_SIZE;
    if (buffer_num > TMUSIC_AAC_BUFF_NUM_MAX) {
        buffer_num = TMUSIC_AAC_BUFF_NUM_MAX;
    }

    if (bt_music_is_low_latency_mode()) {
        //buffer_num = 5;
    }

    bt_music_set_param(buffer_num, 1200, 1024);

#ifdef SL16_bt_music_st
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6000);
#endif

    bt_music_cfg.enc_para = param;

#if EQ_ENABLE
    //tlkalg_eq_reset_all_state();

    if (audio_codec_flag_get(CODEC_FLAG_EQ_MUSIC_EN)) {
        audio_codec_flag_set(CODEC_FLAG_MUSIC_EQ_STATR, 1);
    }
#endif

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<aac enc: set sample rate> %d %d %d %d", bt_music_cfg.samples_per_frame, bt_music_cfg.sample_rate,
                 bt_music_cfg.enc_buf_size, bt_music_cfg.enc_buf_num);

    return 1;
}
#endif

/**
 * @brief   register decode function according to left-right ear and decode algorithm
 * @param[in]   none
 * @returns none
 */
void bt_music_choice_channel(void)
{
#if 0
    if (!IN_MUSIC_MODE || !audio_codec_flag_get(CODEC_FLAG_MUSIC) || !audio_flag_get(FLG_BTC_TWS_EN)) {
        return;
    }

    if (audio_flag_get(FLG_BTC_TWS_EN)) {
        if (tph_headset_is_left()) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<TSYNC_FLAG_LEFT:1>");
            if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                bt_music_register_dec_callback(music_sbc_dec_left);
            } else if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                bt_music_register_dec_callback(music_aac_dec_left);
            } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
            bt_music_register_dec_callback(music_lhdc_dec_left);
#endif
            }
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<TSYNC_FLAG_LEFT:0>");
            if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                bt_music_register_dec_callback(music_sbc_dec_right);
            } else if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                bt_music_register_dec_callback(music_aac_dec_right);
            } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
                bt_music_register_dec_callback(music_lhdc_dec_right);
#endif
            }
        }
    } else {
        if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            bt_music_register_dec_callback(music_sbc_dec_stereo);
        } else if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            bt_music_register_dec_callback(music_aac_dec_stereo);
        } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
            bt_music_register_dec_callback(music_lhdc_dec_stereo);
#endif
        }

#if CODEC_DAC_MONO_MODE
        tlkapi_trace(BT_AUDIO_DBG_FLAG,
                     BT_AUDIO_DBG_SIGN,
                     "<Music Mono> %d %d %d %d",
                     bt_music_cfg.enc_buf_num,
                     bt_music_cfg.enc_buf_size,
                     bt_music_cfg.samples_per_frame,
                     bt_music_cfg.sync_enc_buf_num);
#else
        tlkapi_trace(BT_AUDIO_DBG_FLAG,
                     BT_AUDIO_DBG_SIGN,
                     "<Music Stereo> %d %d %d %d",
                     bt_music_cfg.enc_buf_num,
                     bt_music_cfg.enc_buf_size,
                     bt_music_cfg.samples_per_frame,
                     bt_music_cfg.sync_enc_buf_num);
#endif
    }
#endif
}

/**
 * @brief       CRC check for music encode frames
 * @param[in]   aclHandle - ACL connection handle
 * @param[in]   p         - Data pointer
 * @param[in]   len       - Data length
 * @return      BT_AUDIO_SUCCESS
 */
uint8_t bt_music_enc_crc_frames(uint16_t aclHandle, uint8_t *p, uint16_t len)
{
    (void)aclHandle;
    uint8_t  aac_flag = 0;
    uint8_t *p_src;
    uint16_t crc_len;
    uint32_t crc;

    if (btif_get_bt_music_silent_flag() && len > 50) {
        uint8_t a[10] = {0}, b[10] = {0};
        tmemset(a, 0x00, 10);
        tmemset(b, 0x5a, 10);
        if (!(tmemcmp(a, p + 40, 10) == 0 || tmemcmp(b, p + 40, 10) == 0)) {
            tlkapi_sendData(BT_AUDIO_DBG_FLAG, "bt_music_silent_3s set as 0 change to bt mode ", 0, 0);
            btif_set_bt_music_silent_flag(0);
        }
    }

    if (bt_music_cfg.crc_mute && bt_music_cfg.crc_func && p && len > 17) {
        if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            aac_flag = 1;
        }
        p_src   = aac_flag ? p + 16 : p + 17;
        crc_len = aac_flag ? len - 16 : bt_music_cfg.enc_buf_size;
        crc     = bt_music_cfg.crc_func(0, p_src, crc_len);

        bt_music_enc_crc_check(crc, crc_len);
    }

    return BT_AUDIO_SUCCESS;
}

/**
 * @brief   get encode data start address
 *
 * @param[in]   none
 *
 * @returns encode data start address
 */
audio_ram_code uint8_t *bt_music_enc_get_frame(void)
{
    if (bt_music_cfg.enc_buf_rptr == bt_music_cfg.enc_buf_wptr) {
        return NULL;
    } else {
        return g_bt_music_enc_buf_ptr + bt_music_cfg.enc_buf_rptr * bt_music_cfg.enc_buf_size;
    }
}

/**
 * @brief       Initialize audio path for music
 * @param[in]   None
 * @return      None
 */
void bt_music_audio_path_init(void)
{
#if AUDIO_TWS_MODE
#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
    extern uint16_t master_latency_mode;
    if (master_latency_mode == 0) {
        bt_music_cfg.latency_mode = BT_LATENCY_NORMAL;
        g_bt_music_buffer_size    = 1024 * 12;
    } else {
        bt_music_cfg.latency_mode = BT_LATENCY_PKTLOSS;
        g_bt_music_buffer_size    = 1024 * 45;
    }

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "=== bt_music_audio_path_init, mode: %d, buf_size:%d", bt_music_cfg.latency_mode, g_bt_music_buffer_size);
#endif
#endif

    bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);

#if TLKMW_INTERPHONE_EN
    if (!tlkmdi_interphone_is_busy())
#endif
    {
        tlkdrv_codec_muteSpkBuff();
    }

    btif_register_a2dp_data_recv_callback();
}

/**
 * @brief       Deinitialize audio path for music
 * @param[in]   None
 * @return      None
 */
void bt_music_audio_path_deinit(void)
{
    bt_music_clear_audio_flag();
}

/**
 * @brief   set music play sample rate
 *
 * @param[in]   sample_rate
 *
 * @returns none
 */
void bt_music_open_codec(uint16_t sample_rate)
{
    g_codec_cfg.sample_rate = sample_rate;

#if TLKMW_INTERPHONE_EN
    if (tlkmdi_interphone_is_busy()) {
        tlkmdi_interphone_spk_enable(INTERPHONE_MODE_MUSIC);
        return;
    }
#endif

#if AUDIO_HD_HAC_EN
    g_codec_cfg.codec_sample_rate = 48000;

    st_audio_sys_clk      = 36864000;
    int one_frame_samples = 128;
    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 1024;
    } else if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 128;
    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 128;
    }
    if (hac_init && g_hd_hac_asrc_buffer) {
        tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
    }
    tlkdrv_hac_init(g_hd_hac_asrc_buffer, st_audio_sys_clk, g_codec_cfg.sample_rate, g_codec_cfg.codec_sample_rate, one_frame_samples, 16);
    tlkdrv_hac_set_ppm(g_hd_hac_asrc_buffer, 0);
#elif TLKALG_ASRC_441TO48_16BIT_ENABLE
    g_codec_cfg.codec_sample_rate = 48000;
#else
    g_codec_cfg.codec_sample_rate = sample_rate;
#endif

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_music_open_codec, stack_rate:%d, codec_rate:%d, mute_samples:%d, latency:%d, mode:%d", sample_rate,
                 g_codec_cfg.codec_sample_rate, g_codec_cfg.mute_samples, g_codec_cfg.latency, g_codec_cfg.mode);

#if AUDIO_PATH_24BITS_EN
//     #if (TLKALG_ASRC_441TO48_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
// tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000,4096);
// g_codec_cfg.codec_sample_rate = 48000;
//     #else
#if TLKALG_ANC_ENABLE
    if (tlkmdi_anc_btmusic_is_busy()) {
        // gpio_set_high_level(GPIO_CHN1);
        // tlkdrv_codec_init_env();
        // tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate,4096);
        // tlkdrv_codec_anc_enable(DISABLE_RESAMPLE);

        audio_tx_dma_en(TLKDRV_ANC0_SPK_DMA);
        audio_rx_dma_en(TLKDRV_ANC0_MIC_DMA);
        // gpio_set_low_level(GPIO_CHN1);
    } else
#endif
    {
#if CODEC_DAC_MONO_MODE
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate, 4096);
#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
        bt_music_alg_eq_init(g_codec_cfg.codec_sample_rate, ALG_CHANNEL_LEFT, EQ_TYPE_MUSIC);
#endif
#else
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate, 4096);
#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
        bt_music_alg_eq_init(g_codec_cfg.codec_sample_rate, ALG_CHANNEL_STEREO, EQ_TYPE_MUSIC);
#endif
#endif
    }
#if TLKMDI_HRA_MUSIC_MIC_EN
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, g_codec_cfg.codec_sample_rate, 4096);
#endif

// #endif
#else
#if TLK_CFG_HRA_ENABLE && TLKALG_ASRC_441TO48_16BIT_ENABLE
#if TLKALG_MIC_SPK_MONO_ENABLE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
#endif
#elif TLK_CFG_HRA_ENABLE && TLKALG_ASRC_441TO16_16BIT_ENABLE
#if TLKALG_MIC_SPK_MONO_ENABLE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
#endif
#elif (TLK_CFG_A2DP_TO_BIS_ENABLE && TLKALG_ASRC_441TO48_16BIT_ENABLE)
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
#elif (TLK_CFG_A2DP_TO_BIS_ENABLE && TLKALG_ASRC_441TO16_16BIT_ENABLE)
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 16000, 0);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, g_codec_cfg.codec_sample_rate, 4096);
#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
#if AUDIO_TWS_MODE
    bt_music_alg_eq_init(g_codec_cfg.codec_sample_rate, ALG_CHANNEL_LEFT, EQ_TYPE_MUSIC);
#else
    bt_music_alg_eq_init(g_codec_cfg.codec_sample_rate, ALG_CHANNEL_STEREO, EQ_TYPE_MUSIC);
#endif
#endif
#endif
#if TLK_CFG_A2DP_TO_BIS_ENABLE
    if (tlkmdi_a2dp_to_bis_is_busy()) {
        tlkmdi_a2dp_to_bis_start_audio_timer();
        tlkmdi_a2dp_to_bis_sync_middle_buff(CODEC_SPK_FIFO_SAMPLES / 2);
        tlkdrv_codec_sync_speaker_samples(CODEC_SPK_FIFO_SAMPLES / 2);
    }
#endif
#endif

#if LE_AUDIO_BT_MUSIC_MIX_ENABLE
#if (RESAMPLE_16_TO_48_EN == 2 || RESAMPLE_16_TO_441_EN == 2)
    if (g_codec_cfg.sample_rate == 48000) {
        tlk_resample16to48_init();
    } else {
        tlk_resample16to44_init();
    }
#endif
#endif
}

/**
 * @brief       Close codec for music
 * @param[in]   None
 * @return      None
 */
void bt_music_close_codec(void)
{
    tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
}

/**
 * @brief   music play init
 *
 * @param[in]   none
 *
 * @returns none
 */
audio_ram_code void bt_music_play_init(void)
{
    uint8_t buff_num;
    // uint32_t irq;
    int16_t rptr;

/* mute data in both play buffer and ENC buffer */
#if TLKMW_INTERPHONE_EN
    if (!tlkmdi_interphone_is_busy())
#endif
    {
        tlkdrv_codec_muteSpkBuff();
    }

#if EQ_ENABLE
    //tlkalg_eq_reset_all_state();
    audio_alg_if[ALG_EQ].audio_alg_param_reset();
#endif

    bt_music_cfg.sync_frac = 0;

#if ((TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE) && (LE_AUDIO_BT_MUSIC_MIX_ENABLE || LE_AUDIO_BT_VOICE_MIX_ENABLE))
    ll_audio_bt_music_mix_init();
#endif

    AUDIO_GLOBAL_INT_DISABLE();

    bt_music_cfg.sync_ppm = 0;

    if (bt_music_cfg.enc_buf_num > 110 && (g_codec_cfg.sample_rate == 48000) && bt_music_cfg.enc_buf_size < 85) {
        buff_num                  = 72;
        bt_music_cfg.sync_samples = CODEC_SPK_FIFO_SAMPLES * 3 / 4;
    } else {
        if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            buff_num = bt_music_cfg.enc_buf_num * 9 / 16;
        } else {
            buff_num                  = bt_music_cfg.enc_buf_num * 9 / 16;
            bt_music_cfg.sync_samples = CODEC_SPK_FIFO_SAMPLES * 3 / 4;
        }
    }

    if (bt_music_is_low_latency_mode()) {
        if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            buff_num = 20;
        } else if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
            buff_num = 3;
        }
    }

    rptr                          = bt_music_cfg.enc_buf_wptr - buff_num;
    bt_music_cfg.enc_buf_rptr     = rptr < 0 ? rptr + bt_music_cfg.enc_buf_num : rptr;
    bt_music_cfg.sync_enc_buf_num = buff_num;
    bt_music_cfg.enc_buf_mute_num = buff_num - 1;
    bt_music_cfg.sample_avg_ref   = bt_music_cfg.enc_buf_num * 5 / 8 * bt_music_cfg.samples_per_frame;

    AUDIO_GLOBAL_INT_RESTORE();

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<bt_music_play_init> sync_enc:%d %d %d %d, buff_num:%d r:%d w:%d enc_buf_num:%d", bt_music_cfg.sync_enc_buf_num,
                 bt_music_cfg.enc_buf_mute_num, bt_music_cfg.sample_avg_ref, bt_music_cfg.enc_buf_id, buff_num, bt_music_cfg.enc_buf_rptr, bt_music_cfg.enc_buf_wptr,
                 bt_music_cfg.enc_buf_num);

    if (btif_get_bt_music_silent_flag()) {
        btif_set_bt_music_silent_flag(FALSE);
    }
}

/**
 * @brief   check crc calculate
 *
 * @param[in]   crc: crc value
 * @param[in]   crc_len
 *
 * @returns BT_AUDIO_SUCCESS
 */
uint8_t bt_music_enc_crc_check(uint32_t crc, uint32_t crc_len)
{
    if (bt_music_cfg.crc_mute == 1) {
        bt_music_cfg.crc_mute = crc;
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<BT Music OFF> %d %d %d %d", bt_music_cfg.crc_mute, crc, bt_music_cfg.enc_buf_id, crc_len);
    }

    if (bt_music_cfg.crc_mute != crc) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<_____BT Music ON___> %d %d %d %d", bt_music_cfg.crc_mute, crc, bt_music_cfg.enc_buf_id, crc_len);

        bt_music_cfg.crc_mute    = 0;
        bt_music_cfg.tick_active = clock_time() | 1;
    }

    return BT_AUDIO_SUCCESS;
}

/**
 * @brief   bt_music_sync_task
 * @param[in]   none
 * @returns none
 */
audio_ram_code void bt_music_sync_task(void)
{
    if (!IN_MUSIC_MODE && bt_music_cfg.sync_play) {
#ifdef SL16_bt_music_st
        log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6300);
#endif
        bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);
    }
}

/**
 * @brief   detect mute data
 * @param[in]   p - decode data
 * @param[in]   len - decode data length
 * @returns none
 */
void bt_music_decoder_mute_detection(int16_t *p, uint16_t len)
{
    uint8_t  active = 0;
    uint16_t i;

    if (NULL == bt_music_cfg.crc_func) {
        return;
    }

    for (i = 0; i < len; i++) {
        if (abs_ram(p[i]) > 0) {
            active = 1;
            break;
        }
    }

    if (active) {
        bt_music_cfg.tick_active = clock_time() | 1;
    } else if (bt_music_cfg.tick_active && clock_time_exceed(bt_music_cfg.tick_active, 1500000)) {
        bt_music_cfg.tick_active = 0;
        bt_music_cfg.crc_mute    = 1;
    }
}

#if AUDIO_TWS_MODE
/**
 * @brief       Recovery for TWS slave mismatch
 * @param[in]   None
 * @return      1 if recovery was performed, 0 otherwise
 */
audio_ram_code uint8_t bt_music_slave_miss_match_recovery(void)
{
    int16_t buf_num, rptr;

    if (bt_music_cfg.sync_ppm_set) {
        bt_music_cfg.sync_ppm_set = 0;
        bt_music_cfg.sync_ppm_cur = bt_music_cfg.sync_ppm;

#ifdef SLET_aud_sync_mismatch
        log_tick_general(SL_TWS_AUDIO_LOG_EN, SLET_aud_sync_mismatch);
#endif

#if AUDIO_HD_HAC_EN
        tlkdrv_hac_set_ppm(g_hd_hac_asrc_buffer, bt_music_cfg.sync_ppm);
#endif

#if TLKALG_PPM_SPK_ENABLE
        int                    ppm_val        = bt_music_cfg.sync_ppm;
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        p_audio_alg_if->audio_alg_param_set(0, &ppm_val);
#endif

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
        if (bt_music_cfg.latency_mode == BT_LATENCY_NORMAL) {
            g_bt_music_buffer_size = 1024 * 12;
        } else if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
            g_bt_music_buffer_size = 1024 * 45;
        }

        uint16_t buff_num = g_bt_music_buffer_size / SBC_MAX_FRAMESIZE;
        buff_num          = buff_num >= BT_MUSIC_SBC_BUFF_NUM_MAX ? BT_MUSIC_SBC_BUFF_NUM_MAX : buff_num;

        bt_music_set_param(buff_num, bt_music_cfg.sbc_framesize, 128);
#endif

        /* enc_buf_wptr_last set in the bt_music_sync_update func, 40ms interval
         * bt_music_cfg.enc_buf_wptr_last = bt_music_cfg.enc_buf_wptr;
         */
        buf_num = bt_music_cfg.enc_buf_wptr - bt_music_cfg.enc_buf_wptr_last;
        buf_num += buf_num < 0 ? bt_music_cfg.enc_buf_num : 0;
        buf_num += bt_music_cfg.sync_buf_num_peer;

        bt_music_cfg.sync_enc_buf_num = buf_num;
        bt_music_cfg.enc_buf_mute_num = buf_num;

        rptr                      = bt_music_cfg.enc_buf_wptr - buf_num;
        uint16_t enc_buf_rptr_tmp = bt_music_cfg.enc_buf_rptr;
        bt_music_cfg.enc_buf_rptr = rptr < 0 ? rptr + bt_music_cfg.enc_buf_num : rptr;

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "slave recovery, mode:%d, size:%d, enc:%d, enc_r:%d, ppm_cur:%d, num_peer:%d, enc_buf_w/r:%d/%d, enc_buf_w_last:%d",
                     bt_music_cfg.latency_mode, g_bt_music_buffer_size, bt_music_cfg.sync_enc_buf_num, bt_music_cfg.enc_buf_rptr, bt_music_cfg.sync_ppm_cur,
                     bt_music_cfg.sync_buf_num_peer, bt_music_cfg.enc_buf_wptr, enc_buf_rptr_tmp, bt_music_cfg.enc_buf_wptr_last);

        bt_music_sync_samples();

        return 1;
    }

    return 0;
}
#endif

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
/**
 * @brief       Detect latency mode
 * @param[in]   enc_buf_num - Number of encoded buffers
 * @return      None
 */
audio_ram_code void bt_music_latency_mode_detect(uint16_t enc_buf_num)
{
    uint16_t enc_buf_num_min = bt_music_cfg.enc_buf_num * 2 / 16;

    if (enc_buf_num < enc_buf_num_min) {
        bt_music_set_latency_mode(BT_LATENCY_PKTLOSS);
    }
}

/**
 * @brief       Process dynamic latency
 * @param[in]   None
 * @return      None
 */
audio_ram_code void bt_music_dynamic_latency_process(void)
{
    uint16_t enc_buf_num = bt_music_get_num_of_enc_buff_avail();
#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster()) {
        bt_music_latency_mode_detect(enc_buf_num);
    }
#else
    bt_music_latency_mode_detect(enc_buf_num);
#endif
}

#endif

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
int32_t pcm_ppm_441to48[1024 + 256];
#elif (TLKALG_ASRC_441TO16_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE)
int32_t pcm_ppm_441to16[1024 + 256];
#endif

uint8_t bt_music_underflow_flag = 0;

/**
 * @brief   get music play data
 * @param[in]   p_des0 - address 0 to restore data
 * @param[in]   p_des1 - address 1 to restore data
 * @return  data length
 */
audio_ram_code uint16_t bt_music_get_playback_data(int16_t *p_des0, int16_t *p_des1)
{
    // uint16_t pcm_sin[256];
    // int16_t buf_num, rptr;
    uint16_t ret = 0, i;
    uint16_t samples_in_fifo;
    uint8_t *buf_ptr       = NULL;
    uint16_t frame_samples = 0;

#if 1
    int32_t pcm32[1024];
#elif LHDC_CODEC_ENABLE
    int32_t pcm32[LHDC_MAX_SAMPLE + 64];
#else
    int32_t pcm32[128 + 32];
#endif

    int16_t *pcm;
    int32_t *p_des   = (int32_t *)pcm32;
    uint16_t num_out = 0;

#if AUDIO_TWS_MODE
    if (bt_music_slave_miss_match_recovery()) {
        return 0;
    }

    /* TWS mode */
    if (bt_music_cfg.sync_play == BT_MUSIC_SYNC_PLAY_START) {
        bt_music_cfg.sync_play++;

        /* change todec.play_wptr */
        bt_music_sync_samples();

        return 0;
    }
#endif

#ifdef SL16_bt_music_st
    log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x4000);
#endif

    if (IN_MUSIC_MODE && (bt_music_cfg.sync_play > BT_MUSIC_SYNC_PLAY_INIT)) {
        if (bt_music_cfg.sync_enc_buf_num > bt_music_cfg.enc_buf_num || /* buff full */
            (bt_music_cfg.sync_enc_buf_num == 0 && (tlkdrv_codec_get_spk_avail_samples() < bt_music_cfg.samples_per_frame))) {
            for (i = 0; i < bt_music_cfg.samples_per_frame; i++) {
                pcm32[i] = 0;
            }

            frame_samples = bt_music_cfg.samples_per_frame;

        } else if (bt_music_cfg.enc_buf_rptr != bt_music_cfg.enc_buf_wptr) {
            buf_ptr = bt_music_enc_get_frame();

            if (bt_music_cfg.enc_buf_mute_num) {
                for (i = 0; i < bt_music_cfg.samples_per_frame; i++) {
                    pcm32[i] = 0;
                }

                frame_samples = bt_music_cfg.samples_per_frame;

#ifdef SL16_aud_mute_num
                log_b16(SL_AUDIO_PATH_LOG_EN, SL16_aud_mute_num, bt_music_cfg.enc_buf_mute_num - 1);
#endif

                //#ifdef SLEV_aud_get_mute_data
                //               log_tick_general(SL_AUDIO_PATH_LOG_EN, SLEV_aud_get_mute_data);
                //#endif
            } else {
                if (1) { //bt_music_cfg.dec_func
                    if (bt_music_cfg.dump_mask & FLG_DUMP_PLAYBACK_STATUS) {
                        samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();

                        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "samples in playback buffer %d %d %d", samples_in_fifo, bt_music_cfg.enc_buf_rptr,
                                     bt_music_cfg.enc_buf_wptr);
                    }

                    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if AAC_CODEC_ENABLE
                        uint16_t aac_enc_len;
                        if (buf_ptr) {
                            buf_ptr = g_bt_music_enc_buf_ptr + bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr];
                        }

                        if (NULL == buf_ptr || AAC_PACKET_FLAG != buf_ptr[0]) {
                            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# AAC enc pkt out err");
                            ret = 0xfe;
                        } else {
                            aac_enc_len = bt_music_cfg.aac_len[bt_music_cfg.enc_buf_rptr];

#ifdef SL01_bt_audio_dec
                            log_task_begin_irq(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec);
#endif
                            audio_alg_interface_t *audio_alg_if_handle = audio_alg_get_interface_by_type(ALG_AAC_DEC);
                            ret = audio_alg_if_handle->audio_alg_process(buf_ptr, (uint8_t *)p_des, aac_enc_len, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
#ifdef SL01_bt_audio_dec
                            log_task_end_irq(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec);
#endif

                            if (ret != 0) { // TLKA_AAC_DEC_OK
                                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# AAC dec err, ret:%d len:%d offset:%d", ret, aac_enc_len,
                                             bt_music_cfg.aac_offset[bt_music_cfg.enc_buf_rptr]);
                            }

                            bt_music_decoder_mute_detection((int16_t *)p_des, bt_music_cfg.samples_per_frame * (audio_flag_get(FLG_BTC_TWS_EN) ? 1 : 2));
                        }
#endif
                    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
#if LHDC_CODEC_ENABLE
                        if (tone_is_playing() && g_codec_cfg.sample_rate == 96000) {
                            ret = 0;
                            tmemset(pcm32, 0, sizeof(pcm32));
                        } else {
                            //log_task(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec, 1);
                            //ret = bt_music_cfg.dec_func(buf_ptr, lhdc_config.frame_info.frame_len, p_des);
                            //log_task(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec, 0);

                            bt_music_decoder_mute_detection(p_des, bt_music_cfg.samples_per_frame * (audio_flag_get(FLG_BTC_TWS_EN) ? 1 : 2));
                        }
                        if (ret) {
                            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "# lhdc dec err");
                        }
#endif // LHDC_CODEC_ENABLE
                    } else if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                        audio_alg_interface_t *audio_alg_if_handle = audio_alg_get_interface_by_type(ALG_SBC_DEC);

#ifdef SL01_bt_audio_dec
                        log_task_begin_irq(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec);
#endif
                        if (bt_music_cfg.sbc_channel_mode == 0) {
                            int16_t pcm16_mono[128 + 32];
                            ret = audio_alg_if_handle->audio_alg_process(buf_ptr, (uint8_t *)pcm16_mono, bt_music_cfg.sbc_framesize, ALG_WIDTH_16, ALG_CHANNEL_LEFT);

                            for (i = 0; i < ret; i++) {
                                p_des[i] = (uint16_t)pcm16_mono[i] | (pcm16_mono[i] << 16);
                            }
                        } else {
                            ret = audio_alg_if_handle->audio_alg_process(buf_ptr, (uint8_t *)p_des, bt_music_cfg.sbc_framesize, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
                        }
#ifdef SL01_bt_audio_dec
                        log_task_end_irq(SL_BT_MUSIC_LOG_EN, SL01_bt_audio_dec);
#endif

                        /* AAC ret TLKA_AAC_DEC_OK = 0, TLKA_AAC_DEC_FILL_ERR = 1, SBC 1 is OK */
                        //ret = (1 == ret) ? 0 : 1;

                        //bt_music_decoder_mute_detection(p_des, bt_music_cfg.samples_per_frame * 2);
                    }
                }

                frame_samples = bt_music_cfg.samples_per_frame;
            }
        }

        if (ret == 0) {
            /* bt music frame decode fail process. */
            if (0) { //!btif_power_off_is_ongoing()
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#BT_M dec error ret:%d frame_samples:%d buf_num:%d\n", ret, frame_samples, bt_music_cfg.sync_enc_buf_num);
            }

            frame_samples = bt_audio_get_mute_data(p_des0, p_des1);
        }

        if (frame_samples == 0) {
            for (i = 0; i < bt_music_cfg.samples_per_frame; i++) {
                pcm32[i] = 0;
            }
        }
#if AUDIO_HD_HAC_EN
        (void)pcm;
        int32_t pcm_ppm_buff[1024 + 128];
        int     sum_cnt = bt_music_cfg.samples_per_frame;
#if 1
        num_out     = tlkdrv_hac_process(g_hd_hac_asrc_buffer, (int *)p_des, (int *)pcm_ppm_buff, sum_cnt);
        int16_t *p0 = p_des0;
        int16_t *p1 = p_des1;

        pcm = (int16_t *)pcm_ppm_buff;
        for (int j = 0; j < num_out; j++) {
            *p0++ = pcm[2 * j];
            *p1++ = pcm[2 * j + 1];
        }

#else /// for 24bits test
        int tmp1 = 0;
        for (int j = 0; j < sum_cnt; j++) {
            tmp1                    = *p_des++;
            pcm_ppm_buff[2 * j]     = (tmp1 & 0xFFFF) << 8;
            pcm_ppm_buff[2 * j + 1] = (tmp1 & 0xFFFF0000) >> 8;
        }
        num_out          = tlkdrv_hac_process(g_hd_hac_asrc_buffer, (int *)pcm_ppm_buff, (int *)pcm_ppm_buff, sum_cnt);
        int16_t *p0      = p_des0;
        int16_t *p1      = p_des1;
        int32_t *pcm_int = pcm_ppm_buff;
        for (int j = 0; j < num_out; j++) {
            *p0++ = pcm_int[2 * j] >> 8;
            *p1++ = pcm_int[2 * j + 1] >> 8;
        }
#endif

        frame_samples = num_out;
#else
        if (bt_music_cfg.sync_ppm_cur) {
#if TLKALG_PPM_SPK_ENABLE
            audio_alg_interface_t *p_audio_alg_if;
            int32_t                pcm_ppm_buff[1024 + 64];
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
            //gpio_set_high_level(GPIO_PB10);
            num_out = p_audio_alg_if->audio_alg_process((uint8_t *)p_des, (uint8_t *)pcm_ppm_buff, bt_music_cfg.samples_per_frame, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
            //gpio_set_low_level(GPIO_PB10);
            if (num_out != bt_music_cfg.samples_per_frame) {
                //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "PPM num_out %d", num_out);
            }

            int32_t *pcm_ppm = pcm_ppm_buff;
#else
            int32_t *pcm_ppm = p_des;
            num_out          = bt_music_cfg.samples_per_frame;
#endif
// #if AUDIO_HD_HAC_EN
//     int32_t pcm_hac_buff[1024 + 240];
//     int sum_cnt = num_out;
//     gpio_set_high_level(GPIO_CHN0);
//     num_out = tlkdrv_hac_process(&(gp_hac_cfg_st[0]),(int * )pcm_ppm,(int * )pcm_hac_buff ,sum_cnt);
//     gpio_set_low_level(GPIO_CHN0);
//     pcm = (int16_t *)pcm_hac_buff;
// #else
#if (TLKALG_ASRC_441TO48_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
            if (g_codec_cfg.sample_rate == 44100) {
                uint16_t data_len = num_out;
                // p_audio_alg_if    = audio_alg_get_interface_by_type(ALG_ASRC_441TO48);
                p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
                num_out        = p_audio_alg_if->audio_alg_process((uint8_t *)pcm_ppm, (uint8_t *)pcm_ppm_441to48, data_len, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
                //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "===> ppm: %d, 441to48: %d", data_len, num_out);
            }
#endif
            // num_out = bt_music_cfg.samples_per_frame;
#if (TLKALG_ASRC_441TO16_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE)
            uint16_t data_len = num_out;
            // p_audio_alg_if    = audio_alg_get_interface_by_type(ALG_ASRC_441TO16);
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
            num_out        = p_audio_alg_if->audio_alg_process((uint8_t *)pcm_ppm, (uint8_t *)pcm_ppm_441to16, data_len, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
            //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "===> ppm: %d, 441to48: %d", data_len, num_out);
#endif

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
            if (g_codec_cfg.sample_rate == 44100) {
                pcm = (int16_t *)pcm_ppm_441to48;
            } else {
                pcm = (int16_t *)pcm_ppm;
            }
#elif (TLKALG_ASRC_441TO16_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE)
            pcm = (int16_t *)pcm_ppm_441to16;
#else
            pcm = (int16_t *)pcm_ppm;
#endif
            // #endif  //AUDIO_HD_HAC_EN
            int16_t *p0 = p_des0;
            int16_t *p1 = p_des1;
            for (int j = 0; j < num_out; j++) {
                *p0++ = pcm[2 * j];
                *p1++ = pcm[2 * j + 1];
            }

            if (frame_samples == 0 || num_out == 0) {
                tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "ppm_data overflow %d %d %d %d", frame_samples, num_out, bt_music_cfg.sync_ppm_cur, 0);
            }
            frame_samples = num_out;
        } else {
            /* bt_music_cfg.sync_ppm_cur is 0*/

            pcm         = (int16_t *)p_des;
            int16_t *p0 = p_des0;
            int16_t *p1 = p_des1;
            // int16_t pcm_tmp[132];

            num_out = bt_music_cfg.samples_per_frame;
// #if AUDIO_HD_HAC_EN
//     int32_t pcm_hac_buff[1024 + 240];
//     int sum_cnt = num_out;
//     gpio_set_high_level(GPIO_CHN1);
//     num_out = tlkdrv_hac_process(&(gp_hac_cfg_st[0]),(int * )p_des,(int * )pcm_hac_buff ,sum_cnt);
//     gpio_set_low_level(GPIO_CHN1);
//     pcm = (int16_t *)pcm_hac_buff;
// #else
#if (TLKALG_ASRC_441TO48_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
            if (g_codec_cfg.sample_rate == 44100) {
                audio_alg_interface_t *p_audio_alg_if; // = audio_alg_get_interface_by_type(ALG_ASRC_441TO48);
                p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
                num_out        = p_audio_alg_if->audio_alg_process((uint8_t *)p_des, (uint8_t *)pcm_ppm_441to48, bt_music_cfg.samples_per_frame, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
                //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "===>441to48: %d", num_out);
                pcm = (int16_t *)pcm_ppm_441to48;
            }
#endif

#if (TLKALG_ASRC_441TO16_16BIT_ENABLE) && (TLK_STK_BT_TPSLL_ENABLE)
            // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16);
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
            num_out        = p_audio_alg_if->audio_alg_process((uint8_t *)p_des, (uint8_t *)pcm_ppm_441to16, bt_music_cfg.samples_per_frame, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
            //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "===>441to48: %d", num_out);
            pcm = (int16_t *)pcm_ppm_441to16;
#endif
            // #endif //AUDIO_HD_HAC_EN
            // num_out = bt_music_cfg.samples_per_frame;

            /*
            tlkdrv_codec_get_sin_data(pcm_tmp, bt_music_cfg.samples_per_frame);

            for (i = 0; i < bt_music_cfg.samples_per_frame; i++) {
                *p0++ = pcm_tmp[i] / 4;
                *p1++ = pcm_tmp[i] / 4;
            }
            */

            for (i = 0; i < num_out; i++) {
                *p0++ = pcm[2 * i];
                *p1++ = pcm[2 * i + 1];
            }
            frame_samples = num_out;
        }
#endif

        bt_audio_control_music_volume(p_des0, frame_samples);
        bt_audio_control_music_volume_right(p_des1, frame_samples);

#if (MCU_CORE_TYPE == MCU_CORE_TL751X && AUDIO_TWS_MODE)
        int16_t  pcm_mono_ppm[1024];
        uint8_t *p_pcm_data;

        if (tlkmdi_bt_tpt_isLeft()) {
            p_pcm_data = (uint8_t *)p_des0;
        } else {
            p_pcm_data = (uint8_t *)p_des1;
        }

        int16_t *ptr0 = p_des0;

        if (tlkmdi_bt_tpt_isSlave()) {
            uint16_t length = app_tws_spk_ppm_process(p_pcm_data, (uint8_t *)pcm_mono_ppm, frame_samples, 1);
            for (i = 0; i < length; i++) {
                *ptr0++ = pcm_mono_ppm[i];
            }

            if (length != frame_samples) {
                //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "== tws ppm len: %d, f:%d", length, frame_samples);
            }

            frame_samples = length;
        }

        if (tlkmdi_bt_tpt_isMaster()) {
            extern int g_app_tws_spk_ppm_ready;
            extern int g_tws_spk_ppm_value;

            if (g_app_tws_spk_ppm_ready == 0 || g_tws_spk_ppm_value != 0) {
                app_tws_spk_set_ppm_value(0);
            }

            uint16_t length = app_tws_spk_ppm_process(p_pcm_data, (uint8_t *)pcm_mono_ppm, frame_samples, 1);
            for (i = 0; i < length; i++) {
                *ptr0++ = pcm_mono_ppm[i];
            }

            if (length != frame_samples) {
                //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "== tws ppm len: %d, f:%d", length, frame_samples);
            }

            frame_samples = length;
        }
#endif

        /*
        #if (MCU_CORE_TYPE == MCU_CORE_TL751X && (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE) && LE_AUDIO_BT_MUSIC_MIX_ENABLE)
        if (!tlkmdi_tpsll_audio_is_busy()) {
            mix_ll_audio_stereo(p_des0, p_des1, frame_samples);
        }
        #endif
        */

        //        dsp_codec_get_sin_data(pcm_sin, 256);
        //        pcm_data = pcm_sin;

        //        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "===> pcm_data_len: %d, type: %d, sam: %d",
        //                     pcm_data_len, tlkmw_audio_btif_music_get_avdtp_seid_type(), bt_music_cfg.enc_buf_samples);

        /* frame_samples, A2DP: sbc->128, aac->1024 */
        bt_music_get_encoded_frame(1, frame_samples);

        if (bt_music_cfg.sync_enc_buf_num > BT_MUSIC_SBC_BUFF_NUM_MAX || (bt_music_cfg.tick_skip && clock_time_exceed(bt_music_cfg.tick_skip, 1000000)) ||
            bt_music_underflow_flag) {
            //bt_audio_gpio_toggle_audio_tx();

            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "Encoded buffer empty, reset, id: %d, mute num:%d, play:%d ppm:%d, buf_num: %d", bt_music_cfg.enc_buf_id,
                         bt_music_cfg.enc_buf_mute_num, bt_music_cfg.sync_play, bt_music_cfg.sync_ppm, bt_music_cfg.sync_enc_buf_num);

            bt_music_cfg.tick_skip  = 0;
            bt_music_underflow_flag = 0;

#if TWS_ENABLE
            btif_audio_power_control(POWER_INC);
#endif

#ifdef SL16_bt_music_st
            log_b16(SL_BT_MUSIC_LOG_EN, SL16_bt_music_st, bt_music_cfg.sync_play | 0x6400);
#endif

            bt_music_set_play_status(BT_MUSIC_SYNC_PLAY_IDLE);

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
            bt_music_set_latency_mode(BT_LATENCY_PKTLOSS);

            if (bt_music_cfg.latency_mode == BT_LATENCY_NORMAL) {
                g_bt_music_buffer_size = 1024 * 12;
            } else if (bt_music_cfg.latency_mode == BT_LATENCY_PKTLOSS) {
                g_bt_music_buffer_size = 1024 * 45;
            }
#endif

            return frame_samples;
        }
    } else {
        frame_samples = bt_audio_get_mute_data(p_des0, p_des1);
    }

    bt_music_check_status();

    bt_music_calibrate_rate();

    return frame_samples;
}

/**
 * @brief   music printf info
 * @param[in]   none
 * @return  none
 */
void bt_music_dump(void)
{
    int16_t num             = bt_music_get_num_of_enc_buff_avail();
    int     samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "enc_buf_num:%d, enc_buf_size:%d, sync_enc_buf_num:%d, num:%d, samples_in_fifo:%d", bt_music_cfg.enc_buf_num,
                 bt_music_cfg.enc_buf_size, bt_music_cfg.enc_buf_num, num, samples_in_fifo);
}

/**
 * @brief       Music loop processing
 * @param[in]   None
 * @return      None
 */
void bt_music_loop(void)
{
    /*
    if (tlkmdi_bt_music_is_busy() && bt_music_cfg.inc_peer_tx_power_is_pending) {
        uint16_t handle = tlkmw_audio_btif_getPlayHandle();

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "=== set inc_peer_tx_power new/last: %d/%d",
                     bt_music_cfg.inc_peer_tx_power, bt_music_cfg.inc_peer_tx_power_available);

        bt_music_cfg.inc_peer_tx_power_is_pending = 0;
        if (bt_music_cfg.inc_peer_tx_power) {
            tlkmdi_audio_btif_setIncPeerPwrMax(handle, 1);
            bt_music_cfg.inc_peer_tx_power_available = 1;
        } else {
            tlkmdi_audio_btif_setIncPeerPwrMax(handle, 0);
            bt_music_cfg.inc_peer_tx_power_available = 0;
        }
    }
    */
}
#endif
