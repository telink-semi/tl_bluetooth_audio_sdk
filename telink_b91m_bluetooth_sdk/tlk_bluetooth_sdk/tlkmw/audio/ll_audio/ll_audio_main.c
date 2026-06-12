/********************************************************************************************************
 * @file    ll_audio_main.c
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

int16_t  g_tone_buff[300] = {0};
int16_t *g_tone_buff_ptr;

#if (TLKSTK_BT_TPS_ENABLE)
#include "tlkmw/audio/audio_mw_manager.h"
#include "tlkmw/audio/bt_audio/bt_audio_main.h"
#include "tlkmw/audio/bt_audio/bt_audio_sync.h"
#include "tlkmw/audio/ll_audio/ll_audio_main.h"
#include "tlkmw/audio/common/dsp/tlkmw_audio_dsp.h"

#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"

#include "stack/tpsll/tpsll.h"
#include "drivers.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/multiCoreComm/service/service_mailbox.h"
#include "stack/tpsll/host/tpsll_hci.h"

#if AUDIO_TWS_MODE
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"
extern bool tlkmdi_bt_tpt_isLeft();
#endif

#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif

#ifndef TICK_OF_BLOCK_SAMPLE_48k
#define TICK_OF_BLOCK_SAMPLE_48k 24000
#endif

#ifndef NUM_OF_SAMPLES_PER_BLOCK_48K
#define NUM_OF_SAMPLES_PER_BLOCK_48K 48
#endif

#define LL_AUDIO_PPM_ENABLE 1

tpsll_ultra_ll_context_t g_tpsll_ultra_ll_ctx   = {0};
uint8_t                 *g_pcm_mix_dec_buff_ptr = NULL;


#define ABS_MACRO(v)     ((v) > 0 ? (v) : (-(v)))

#define TLKALG_HYBRID_EN 0

//#define TUS_MIC_ENC  (TLKALG_HYBRID_EN ? 1800 : 5000)
//#define TUS_SPK_DEC  (TLKALG_HYBRID_EN ? 5100 : 1900)


#if AUDIO_TWS_MODE
#define TUS_SPK_DEC (3200)
#define TUS_MIC_ENC (2500)
#else
#if ULTRA_LOW_LATENCY_EN
#define TUS_SPK_DEC (1600)
#define TUS_MIC_ENC (4400)
#else
#define TUS_SPK_DEC (3500)
#define TUS_MIC_ENC (2500)
#endif
#endif

#if AUDIO_TWS_MODE
#define ULTRA_LL_TUS_MIC_ENC (3860) //2200-->3860 for one more re-transmission
#else
#define ULTRA_LL_TUS_MIC_ENC (3200) //2200-->3200 for one more re-transmission
#endif
#define ULTRA_LL_TUS_SPK_DEC (1000)

#define SL_APP_AUDIO_EN      SL_STACK_SCO_EN

uint8_t ll_audio_enc_mute_buff[ASYNC_AUDIO_LC3A_FRMAE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static int16_t  s_ll_voice_mode            = 0;
static uint16_t s_ll_pkt_audio_last        = 0;
static uint8_t  s_ll_audio_playing_started = 0;

extern uint8_t g_tpsll_data_buff[TPSLL_ENC_BUFF_FRAME_SIZE * TPSLL_ENC_BUFF_NUM];
extern int     spk_ppm;

async_audio_context_t    async_audio_ctx;
tpsll_enc_buff_context_t g_tpsll_enc_buf_ctx;

int32_t g_ll_mic_mid_buf[MIC_BUF_CAP] = {0};

/******************************* PPM ************************/
#define KP_GAIN        2.5f
#define KI_GAIN        0.003f
#define KD_GAIN        0.05f

#define INTEGRAL_LIMIT 50000

static float s_integral_sum         = 0.0f;
static int   s_last_buffer_error    = 0;
static int   s_current_smoothed_ppm = 0;

static int s_clk_diff_remains   = 0;
static int s_ll_audio_sync_flag = 0;

extern uint32_t rcv_tick_diff;
static int      s_asrc_init = 0;

static uint8_t s_ll_audio_10ms_cnt         = 0;
static uint8_t s_ll_audio_debug_output_cnt = 0;
static int     s_ll_ppm_spk_value_set      = 0;

uint8_t  tpsll_lost_pkt_cnt = 0;
uint32_t g_stimer_start_cnt = 0;

static uint32_t s_stimer_irq_tick_last = 0;
uint32_t        tick_diff_sum_cnt      = 0;
int32_t         tick_diff_sum;

static uint16_t s_ll_spk_ppm         = 0;
static int      s_ll_spk_ppm_int     = 0;
static int      s_ll_spk_ppm_frac    = 0;
static int      s_ll_spk_ppm_int_ref = 17;

uint32_t g_ppm_cnt        = 0;
int32_t  g_ppm_sample_cnt = 0;

#if DONGLE_VOICE_MIC_EN
extern uint16_t g_ll_mic_mute_cnt;
#endif

void asrc_feedback_init(void)
{
    s_integral_sum         = 0.0f;
    s_last_buffer_error    = 0;
    s_current_smoothed_ppm = 0;
    s_asrc_init            = 1;
    s_clk_diff_remains     = 0;
}

/******************************* PPM ************************/

static uint8_t s_tph_mode = 0;

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE)
extern int      ll_dsp_ret_data_buff[TLKALG_DSP_RET_NN_DATA_LEN_LL];
extern uint16_t ll_dsp_wptr;
extern uint16_t ll_dsp_rptr;
#endif

#if 0
signed short ll_sin_48k_mono_16bit[] __attribute__((aligned(4))) = {
    0,     1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,
    7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,
    4096,  3134,  2120,  1069,  0,    -1069, -2120, -3134, -4095, -4986,
    -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568,
    -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069
};

int32_t ll_sin_48k_mono_24bit[] __attribute__((aligned(4))) = {
    0,       1094933,  2171131,  3210181,  4194303,  5106660,  5931641,  6655129,  7264747,  7750062,
    8102772, 8316841,  8388607,  8316841,  8102772,  7750062,  7264746,  6655129,  5931641,  5106660,
    4194303, 3210179,  2171130,  1094932,  0,       -1094933, -2171131, -3210181, -4194305, -5106661,
    -5931642, -6655129, -7264747, -7750063, -8102772, -8316841, -8388607, -8316841, -8102772, -7750062,
    -7264747, -6655129, -5931639, -5106660, -4194301, -3210180, -2171129, -1094933
};

int32_t ll_sin_48k_stereo_24bit[] __attribute__((aligned(4))) = {
    0, 0, 1094933, 1094933, 2171131, 2171131, 3210181, 3210181, 4194303, 4194303,
    5106660, 5106660, 5931641, 5931641, 6655129, 6655129, 7264747, 7264747, 7750062, 7750062,
    8102772, 8102772, 8316841, 8316841, 8388607, 8388607, 8316841, 8316841, 8102772, 8102772,
    7750062, 7750062, 7264746, 7264746, 6655129, 6655129, 5931641, 5931641, 5106660, 5106660,
    4194303, 4194303, 3210179, 3210179, 2171130, 2171130, 1094932, 1094932, 0, 0,
    -1094933, -1094933, -2171131, -2171131, -3210181, -3210181, -4194305, -4194305, -5106661, -5106661,
    -5931642, -5931642, -6655129, -6655129, -7264747, -7264747, -7750063, -7750063, -8102772, -8102772,
    -8316841, -8316841, -8388607, -8388607, -8316841, -8316841, -8102772, -8102772, -7750062, -7750062,
    -7264747, -7264747, -6655129, -6655129, -5931639, -5931639, -5106660, -5106660, -4194301, -4194301,
    -3210180, -3210180, -2171129, -2171129, -1094933, -1094933
};

int32_t sin_16k_mono_lc3_24bit[] __attribute__((aligned(4))) = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539,
    -23169, -30272, -32767, -30272, -23169, -12539
};

uint8_t sin_48k_mono_lc3_enc[] = {
    0x01, 0x00, 0xfb, 0x52, 0xb0, 0x21, 0x08, 0x40, 0x68, 0x39,
    0x53, 0x98, 0x53, 0x0c, 0x47, 0x30, 0xe1, 0x58, 0x82, 0x3a,
    0x09, 0x45, 0x35, 0x02, 0x7f, 0x94, 0x65, 0xad, 0x00, 0x0d,
    0x81, 0x3c, 0x7b, 0x90, 0x6d, 0xd4, 0x7a, 0xd8, 0x69, 0xc0,
    0xbc, 0x1d, 0xbb, 0xe8, 0x00, 0x39, 0x44, 0x75, 0xe4, 0x5e,
    0x45, 0xef, 0x33, 0xc1, 0xf0, 0x5f, 0xaf, 0x89, 0x02, 0xa6,
    0x11, 0x78, 0x12, 0x83, 0xbb, 0x5e, 0xa1, 0x67, 0xdf, 0x0c,
    0xd7, 0xdb, 0xb4, 0xe0, 0xa4, 0x66, 0x18, 0x82, 0x1c, 0x39,
    0x72, 0x9c, 0x11, 0xc0, 0xc4, 0x4f, 0xf4, 0x48, 0x97, 0x8a,
    0x05, 0xec, 0xfb, 0x52, 0xb0, 0x21, 0x08, 0x40, 0x68, 0x39,
    0x53, 0x98, 0x53, 0x0c, 0x47, 0x30, 0xe1, 0x58, 0x82, 0x3a,
    0x09, 0x45, 0x35, 0x02, 0x7f, 0x94, 0x65, 0xad, 0x00, 0x0d,
    0x81, 0x3c, 0x7b, 0x90, 0x6d, 0xd4, 0x7a, 0xd8, 0x69, 0xc0,
    0xbc, 0x1d, 0xbb, 0xe8, 0x00, 0x39, 0x44, 0x75, 0xe4, 0x5e,
    0x45, 0xef, 0x33, 0xc1, 0xf0, 0x5f, 0xaf, 0x89, 0x02, 0xa6,
    0x11, 0x78, 0x12, 0x83, 0xbb, 0x5e, 0xa1, 0x67, 0xdf, 0x0c,
    0xd7, 0xdb, 0xb4, 0xe0, 0xa4, 0x66, 0x18, 0x82, 0x1c, 0x39,
    0x72, 0x9c, 0x11, 0xc0, 0xc4, 0x4f, 0xf4, 0x48, 0x97, 0x8a,
    0x05, 0xec
};

uint8_t sin_48k_mono_lc3_24bit_enc[] = {
    0x01, 0x00, 0x38, 0x67, 0xa9, 0xf8, 0xc6, 0x85, 0xf4, 0x41,
    0xd1, 0x99, 0x3d, 0x27, 0x25, 0xef, 0xeb, 0x6d, 0xae, 0xed,
    0xa3, 0x1b, 0xe4, 0xdd, 0x3b, 0x22, 0x0a, 0x2b, 0x65, 0xc3,
    0x13, 0xbf, 0x99, 0x39, 0x19, 0x14, 0x01, 0x34, 0x12, 0x6e,
    0xc4, 0xa1, 0xf4, 0xbd, 0x74, 0xf7, 0xf8, 0x27, 0x83, 0x3c,
    0x10, 0x46, 0xc5, 0x2e, 0x2d, 0x78, 0x7d, 0xc2, 0x90, 0x09,
    0xe3, 0xd6, 0x37, 0x9d, 0x53, 0x71, 0xce, 0xa0, 0xfe, 0x3d,
    0xd0, 0xa2, 0xce, 0x72, 0xa7, 0x9a, 0xc4, 0xf9, 0x82, 0xc5,
    0x62, 0x13, 0x47, 0x1b, 0xc4, 0x4f, 0xf4, 0x48, 0x97, 0xb8,
    0xb0, 0xc4, 0x38, 0x67, 0xa9, 0xf8, 0xc6, 0x85, 0xf4, 0x41,
    0xd1, 0x99, 0x3d, 0x27, 0x25, 0xef, 0xeb, 0x6d, 0xae, 0xed,
    0xa3, 0x1b, 0xe4, 0xdd, 0x3b, 0x22, 0x0a, 0x2b, 0x65, 0xc3,
    0x13, 0xbf, 0x99, 0x39, 0x19, 0x14, 0x01, 0x34, 0x12, 0x6e,
    0xc4, 0xa1, 0xf4, 0xbd, 0x74, 0xf7, 0xf8, 0x27, 0x83, 0x3c,
    0x10, 0x46, 0xc5, 0x2e, 0x2d, 0x78, 0x7d, 0xc2, 0x90, 0x09,
    0xe3, 0xd6, 0x37, 0x9d, 0x53, 0x71, 0xce, 0xa0, 0xfe, 0x3d,
    0xd0, 0xa2, 0xce, 0x72, 0xa7, 0x9a, 0xc4, 0xf9, 0x82, 0xc5,
    0x62, 0x13, 0x47, 0x1b, 0xc4, 0x4f, 0xf4, 0x48, 0x97, 0xb8,
    0xb0, 0xc4
};

uint8_t sin_48k_mono_lc3_24bit_enc_new[] = {
    0x01, 0x00, 0x52, 0x1d, 0x3e, 0x21, 0xe3, 0x76, 0xcc, 0xef,
    0x53, 0x4b, 0x9e, 0xb2, 0x02, 0xea, 0xfd, 0xe2, 0x5e, 0x87,
    0x01, 0x07, 0xc1, 0xe3, 0x8e, 0x68, 0x6d, 0x15, 0x50, 0xf1,
    0xa0, 0x1a, 0x8f, 0x8c, 0x20, 0x22, 0x56, 0x06, 0x1f, 0xe4,
    0x4f, 0xf4, 0x48, 0x97, 0x8a, 0x10, 0xbc, 0x52, 0x1d, 0x3e,
    0x21, 0xe3, 0x76, 0xcc, 0xef, 0x53, 0x4b, 0x9e, 0xb2, 0x02,
    0xea, 0xfd, 0xe2, 0x5e, 0x87, 0x01, 0x07, 0xc1, 0xe3, 0x8e,
    0x68, 0x6d, 0x15, 0x50, 0xf1, 0xa0, 0x1a, 0x8f, 0x8c, 0x20,
    0x22, 0x56, 0x06, 0x1f, 0xe4, 0x4f, 0xf4, 0x48, 0x97, 0x8a,
    0x10, 0xbc
};

uint8_t g_enc_lc3_24bit_data[] = {
    0x18, 0x97, 0xaf, 0x81, 0x3d, 0x9f, 0x53, 0x7f, 0x45, 0xcb,
    0xeb, 0xb0, 0x5d, 0xc6, 0x27, 0x01, 0x05, 0xda, 0x08, 0x7a,
    0x60, 0x18, 0xbe, 0x1d, 0x62, 0x15, 0x49, 0xf0, 0xc6, 0x27
};

uint8_t g_lc3_plus_enc_out_data[] = {
    0xff, 0xdd, 0x1c, 0xea, 0x18, 0x2c, 0xee, 0xd7, 0x7e, 0x6b,
    0x88, 0x0b, 0x28, 0x18, 0x01, 0x90, 0x97, 0x52, 0x44, 0x3c,
    0xf9, 0x6b, 0xd9, 0x4e, 0xa3, 0x63, 0xc3, 0x49, 0x57, 0x2f,
    0x36, 0x25, 0xff, 0xcb, 0xd6, 0x8f, 0x50, 0x0a, 0xaa, 0x87,
    0x50, 0x70, 0x65, 0x7c, 0x46, 0x11, 0x43, 0x22, 0xd0, 0x72,
    0x3b, 0x31, 0x14, 0xd1, 0x91, 0x42, 0xa3, 0x60, 0xcf, 0x0a,
    0x3d, 0x67, 0x05, 0xcd, 0x4e, 0x2e, 0x05, 0x53, 0xcf, 0x62,
    0x4a, 0x37, 0x46, 0x78, 0xab, 0xeb, 0x0c, 0xca, 0x4a, 0x12,
    0xf4, 0x5a, 0xde, 0xf3, 0x6e, 0x76, 0x72, 0xf1, 0x4c, 0x32
};

static unsigned short lc3_24bit_rptr = 0;

int tlkalg_lc3_24bit_get_data(int *pdes, unsigned short len)
{
    for (int i = 0; i < len; i++) {
        *pdes++ = sin_16k_mono_lc3_24bit[lc3_24bit_rptr++];
        lc3_24bit_rptr %= 16;
    }

    return 1;
}
#endif

void ll_audio_headset_post_pcm_data(codec_mono_int *p_pcm_left, codec_mono_int *p_pcm_right, uint16_t samples_num);

void ll_audio_open_codec(void);

/**
 * @brief Set ultra-low latency flag.
 * @param[in] flag - Flag value.
 * @return none
 */
void ll_audio_set_ultra_low_latency_flag(uint8_t flag)
{
    if (flag) {
        g_tpsll_ultra_ll_ctx.ultra_ll_flag = 1;
    } else {
        g_tpsll_ultra_ll_ctx.ultra_ll_flag = 0;
    }

    if (g_tpsll_ultra_ll_ctx.ultra_ll_last_mode != flag) {
        g_tpsll_ultra_ll_ctx.ultra_ll_mode_change = 1;
        g_tpsll_ultra_ll_ctx.ultra_ll_last_mode   = flag;
    }
}

/**
 * @brief Get ultra-low latency flag.
 * @param[in] none
 * @return Ultra-low latency flag.
 */
uint8_t ll_audio_get_ultra_low_latency_flag(void)
{
    return g_tpsll_ultra_ll_ctx.ultra_ll_flag;
}

/**
 * @brief Get MIC encoding timestamp.
 * @param[in] none
 * @return MIC encoding timestamp.
 */
uint32_t ll_audio_get_tus_mic_enc(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? ULTRA_LL_TUS_MIC_ENC : TUS_MIC_ENC);
}

/**
 * @brief Get speaker decoding timestamp.
 * @param[in] none
 * @return Speaker decoding timestamp.
 */
uint32_t ll_audio_get_tus_spk_dec(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? ULTRA_LL_TUS_SPK_DEC : TUS_SPK_DEC);
}

/**
 * @brief Get frame length for low latency audio.
 * @param[in] none
 * @return Frame length.
 */
uint32_t ll_audio_get_frame_length(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? 5000 : 10000);
}

/**
 * @brief Get encoder frame length.
 * @param[in] none
 * @return Encoder frame length.
 */
uint16_t ll_audio_get_enc_frame_length(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? ASYNC_AUDIO_ULTRA_LL_DATA_LEN : ASYNC_AUDIO_DATA_LEN);
}

/**
 * @brief Get headset encoder frame length.
 * @param[in] none
 * @return Encoder frame length.
 */
uint16_t ll_audio_get_headset_enc_frame_length(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? ULTRA_LL_5MS_MIC_LC3_DATA_LEN : LL_10MS_MIC_LC3_DATA_LEN);
}

/**
 * @brief Get samples length for low latency audio.
 * @param[in] none
 * @return Samples length.
 */
uint16_t ll_audio_get_samples_length(void)
{
    return (ll_audio_get_ultra_low_latency_flag() ? ULTRA_LL_SAMPLES : SAMPLES);
}

uint8_t async_dongle_is_in_scan_mode(void)
{
    return 0;
}

uint8_t app_audio_ctl_get_audio_mode(void)
{
    return 0;
}

/**
 * @brief Get mode of TPH headset.
 * @param[in] none
 * @return Headset mode.
 */
uint8_t app_tph_headset_get_mode(void)
{
    return s_tph_mode;
}

/**
 * @brief Set low latency audio mode.
 * @param[in] mode - Audio mode to set.
 * @return none
 */
void ll_audio_set_mode(uint8_t mode)
{
    s_tph_mode = mode;
}

uint8_t app_tph_dongle_is_connected(void)
{
#if (TLK_STK_TPH_ENABLE)
    return (s_tph_mode & TPH_HOST_MODE_DONGLE_ACTIVE);
#elif (TLK_STK_TPT_ENABLE)
    return (s_tph_mode & TPT_HOST_MODE_DONGLE_ACTIVE);
#endif
}

/**
 * @brief Convert PCM16 to PCM32 format.
 * @param[in] channel - Audio channel.
 * @param[in] ps - PCM16 source data.
 * @param[in] samples - Number of samples.
 * @param[out] pcm32 - Converted PCM32 data.
 * @return none
 */
_attribute_ram_code_ void pcm16_2_pcm32(uint8_t channel, int16_t *ps, uint16_t samples, int32_t *pcm32)
{
    int16_t *pd = ((int16_t *)pcm32) + channel;

    while (samples--) {
        *pd = *ps++;
        pd += 2;
    }
}

_attribute_ram_code_ int pcm_s32_2_s16(int16_t *dst, const int32_t *src, uint16_t len)
{
    if (!len || dst == NULL || src == NULL) {
        return -1;
    }

    while (len--) {
        *dst++ = *src++ >> 8;
    };

    return 0;
}

uint8_t ll_audio_is_encoding_state(void)
{
    return (async_audio_ctx.pkt_rptr & 0x10);
}

uint8_t ll_audio_tone_is_playing(void)
{
    //return (async_audio_ctx.tone_st == 2);
    return 0;
}

/**
 * @brief Check if phone mode is active.
 * @param[in] none
 * @return Phone mode status.
 */
int8_t ll_audio_is_phone_mode(void)
{
    int8_t ret = 0;
#if (TLK_STK_TPH_ENABLE)
    if ((s_tph_mode & TPH_HOST_MODE_DONGLE_AUDIO) == TPH_HOST_MODE_DONGLE_PHONE) {
#elif (TLK_STK_TPT_ENABLE)
    if ((s_tph_mode & TPT_HOST_MODE_DONGLE_AUDIO) == TPT_HOST_MODE_DONGLE_PHONE) {
#endif
        ret = 1;
    }

    return ret;
}

/**
 * @brief Toggle GPIO for testing purposes.
 * @param[in] times - Number of toggles.
 * @return none
 */
_attribute_ram_code_sec_ void ll_aud_gpio_toggle_test(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++) {
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
        gpio_write(GPIO_PC1, 0);
        gpio_write(GPIO_PC1, 1);
        gpio_write(GPIO_PC1, 0);
#endif
    }
}

void ll_audio_enter_test_mode(void)
{
    uint32_t ref_tus_mic_enc = ll_audio_get_tus_mic_enc();

    ll_audio_open_codec();

    async_audio_ctx.tick0   = clock_time() | 1;
    async_audio_ctx.tus_mic = 0;

    // plic_interrupt_enable(IRQ_TIMER0);
    // plic_set_priority(IRQ_TIMER0, 1);

    // ll_audio_set_timer(ref_tus_mic_enc - async_audio_ctx.tus_mic);
    // ll_audio_start_timer();

    async_audio_ctx.tick_timer = async_audio_ctx.tick0 + (ref_tus_mic_enc - async_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;

    async_audio_ctx.pkt_rptr = 0x10;
}

/**
 * @brief Initialize encoder buffer for TPSLL.
 * @param[in] p_tpsll_audio_buff - Buffer for TPSLL audio.
 * @return none
 */
void tpsll_enc_buff_init(uint8_t *p_tpsll_audio_buff)
{
    tlkapi_printf(APP_AUDIO_LOG_EN, "tpsll enc buff init");

    uint16_t data_len = 0;

    if (ll_audio_get_ultra_low_latency_flag()) {
        data_len = TPSLL_ULTRA_LL_ENC_BUFF_FRAME_SIZE;
    } else {
        data_len = TPSLL_ENC_BUFF_FRAME_SIZE;
    }

    g_tpsll_enc_buf_ctx.p_tpsll_enc_buff          = p_tpsll_audio_buff;
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_frame_size = data_len;
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_num        = TPSLL_ENC_BUFF_NUM;
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr       = 0;
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr       = 0;
}

/**
 * @brief Get pointer to encoder buffer for TPSLL.
 * @param[in] none
 * @return Pointer to encoder buffer.
 */
_always_inline uint8_t *tpsll_get_enc_buff_ptr(void)
{
    uint16_t offset    = g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr * g_tpsll_enc_buf_ctx.tpsll_enc_buff_frame_size;
    uint8_t *p_enc_buf = g_tpsll_enc_buf_ctx.p_tpsll_enc_buff + offset;

    return p_enc_buf;
}

/**
 * @brief Get encoder buffer by write pointer for TPSLL.
 * @param[in] wptr - Write pointer.
 * @return Pointer to encoder buffer.
 */
_always_inline uint8_t *tpsll_get_enc_buff_by_wptr(uint16_t wptr)
{
    uint16_t offset    = wptr * g_tpsll_enc_buf_ctx.tpsll_enc_buff_frame_size;
    uint8_t *p_enc_buf = g_tpsll_enc_buf_ctx.p_tpsll_enc_buff + offset;

    return p_enc_buf;
}

/**
 * @brief Get encoder buffer by read pointer for TPSLL.
 * @param[in] rptr - Read pointer.
 * @return Pointer to encoder buffer.
 */
audio_ram_code uint8_t *tpsll_get_enc_buff_by_rptr(uint8_t rptr)
{
    uint8_t *p_buff = NULL;

    uint16_t offset = rptr * g_tpsll_enc_buf_ctx.tpsll_enc_buff_frame_size;
    p_buff          = g_tpsll_enc_buf_ctx.p_tpsll_enc_buff + offset;

    return p_buff;
}

/**
 * @brief Update read pointer of encoder buffer for TPSLL.
 * @param[in] none
 * @return none
 */
void tpsll_update_enc_buff_rptr(void)
{
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr = (g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr + 1) % g_tpsll_enc_buf_ctx.tpsll_enc_buff_num;
}

/**
 * @brief Update write pointer of encoder buffer for TPSLL.
 * @param[in] none
 * @return none
 */
_always_inline void tpsll_update_enc_buff_wptr(void)
{
    g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr = (g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr + 1) % g_tpsll_enc_buf_ctx.tpsll_enc_buff_num;
}

/**
 * @brief Check availability of encoder buffer for TPSLL.
 * @param[in] none
 * @return Availability status.
 */
uint8_t tpsll_get_enc_buff_available(void)
{
    int16_t available = g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr - g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr;
    if (available < 0) {
        available += g_tpsll_enc_buf_ctx.tpsll_enc_buff_num;
    }

    return available;
}

/**
 * @brief Get encoder buffer for TPSLL.
 * @param[in] none
 * @return Pointer to encoder buffer.
 */
audio_ram_code uint8_t *tpsll_get_enc_buff(void)
{
    uint8_t *p_buff             = NULL;
    uint8_t  tpsll_enc_buff_rcv = tpsll_get_enc_buff_available();

    if (tpsll_enc_buff_rcv) {
        uint16_t offset = g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr * g_tpsll_enc_buf_ctx.tpsll_enc_buff_frame_size;
        p_buff          = g_tpsll_enc_buf_ctx.p_tpsll_enc_buff + offset;
        tpsll_update_enc_buff_rptr();
    }

    return p_buff;
}

/**
 * @brief Enter audio mode for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_enter_audio_mode(void)
{
    uint32_t ref_tus_mic_enc = 0;

    tlktpsll_hci_registerC2HSpeakerData(ll_audio_audio_rx);
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_STIMER_START_EVT, tlk_mailbox_stimer_start_evt_callback);

    tlkapi_printf(APP_AUDIO_LOG_EN, "ll_audio_enter_audio_mode");

#if AUDIO_TWS_MODE
    ll_audio_spk_ppm_init(1);
#else
    ll_audio_spk_ppm_init(3);
#endif

    // ll_audio_open_codec();

#if (!MCU_CORE_TL752X_TEMP)
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);
#else
    plic_interrupt_enable(IRQ_TIMER0_0);
    plic_set_priority(IRQ_TIMER0_0, 1);
#endif

    async_audio_ctx.tick0   = clock_time() | 1;
    async_audio_ctx.tus_mic = 0;

    ref_tus_mic_enc            = ll_audio_get_tus_mic_enc();
    async_audio_ctx.tick_timer = async_audio_ctx.tick0 + (ref_tus_mic_enc - async_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;
    async_audio_ctx.pkt_rptr   = 0x10;
    s_ll_voice_mode            = 0;

    if (s_ll_audio_playing_started == 0) {
        s_ll_audio_playing_started = 1;
        tpsll_enc_buff_init(g_tpsll_data_buff);
    }

    g_tpsll_ultra_ll_ctx.ultra_ll_mode_change = 0;
}

/**
 * @brief Exit audio mode for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_exit_audio_mode(void)
{
    tlkapi_printf(APP_AUDIO_LOG_EN, "ll_audio_exit_audio_mode");
    ll_audio_spk_ppm_deinit();
    async_audio_ctx.timer_started = 0;
    async_audio_ctx.data_rcv_tick = 0;
    s_ll_voice_mode               = 0;
    // tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
    // tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);

    async_audio_ctx.mic_state  = 0;
    s_ll_audio_playing_started = 0;

    // #if TLK_MW_DSP_COMM_ENABLE
    // tlkmw_dsp_pause();
    // #endif
}

/**
 * @brief Initialize mailbox callback for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_mailbox_callback_init(void)
{
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_STIMER_START_EVT, tlk_mailbox_stimer_start_evt_callback);
}

int ll_audio_tpsll_event_cb(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen != 4) {
        return -TLK_EPARAM;
    }
#if (0) // clear warning, Xiaogang can open when debug Audio Path.
    uint32_t tick;

    tick = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
#endif
    ll_audio_start_timer_encode_task();
    //tlkapi_printf(APP_AUDIO_LOG_EN, "ll_audio_tpsll_event_cb: tick", tick);
    return 0;
}

/**
 * @brief Start timer encode task for low latency audio.
 * @param[in] none
 * @return Status of starting the timer.
 */
_attribute_ram_code_sec_noinline_ uint8_t ll_audio_start_timer_encode_task(void)
{
    uint8_t  ret             = 0;
    uint32_t ref_tus_mic_enc = 0;

    async_audio_ctx.tick0               = clock_time();
    async_audio_ctx.tick_stimer_trigger = async_audio_ctx.tick0;

    if (!(tlkmdi_tpsll_audio_is_busy()
#if TLKALG_ANC_ENABLE
          || tlkmdi_anc_ll_audio_is_busy()
#endif
              )) { //|| async_audio_ctx.data_rcv_tick == 0
        return 0;
    }

#if (TLK_STK_TPH_ENABLE)
    if ((app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO) == TPH_HOST_MODE_DONGLE_PHONE) {
#elif (TLK_STK_TPT_ENABLE)
    if ((app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_AUDIO) == TPT_HOST_MODE_DONGLE_PHONE) {
#endif
        async_audio_ctx.tus_mic = 0;
    }

    else {
        async_audio_ctx.tus_mic = 0;
    }

    ref_tus_mic_enc = ll_audio_get_tus_mic_enc();

    tlkmdi_audio_task_set_next_irq(ref_tus_mic_enc - async_audio_ctx.tus_mic);

    async_audio_ctx.tick_timer    = async_audio_ctx.tick0 + (ref_tus_mic_enc - async_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;
    async_audio_ctx.pkt_rptr      = 0x10;
    async_audio_ctx.timer_started = 1;

#if 0
    uint32_t enc_task_start_timer_us = ref_tus_mic_enc - async_audio_ctx.tus_mic - ll_tick_diff_avg/SYSTEM_TIMER_TICK_1US;

    tlkmdi_audio_task_set_next_irq(enc_task_start_timer_us);

    async_audio_ctx.tick_timer = async_audio_ctx.tick0 + enc_task_start_timer_us * SYSTEM_TIMER_TICK_1US;

    async_audio_ctx.pkt_rptr = 0x10;

    async_audio_ctx.timer_started = 1;
#endif

#ifdef SLET_tpsll_audio_start_timer
    log_tick_irq(1, SLET_tpsll_audio_start_timer);
#endif
#if DSP_GPIO_DBG
    gpio_write(GPIO_PJ1, 0);
    gpio_write(GPIO_PJ1, 1);
    gpio_write(GPIO_PJ1, 0);
#endif
    return ret;
}

_attribute_ram_code_sec_ void ll_aud_gpio_toggle_stimer_irq(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++) {
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
        gpio_write(GPIO_PF4, 0);
        gpio_write(GPIO_PF4, 1);
        gpio_write(GPIO_PF4, 0);
#endif
    }
}

extern void ll_audio_mix_get_mic_data(void);

/**
 * @brief Callback function for mailbox stimer start event.
 * @param[in] data - Event data.
 * @return none
 */
_attribute_ram_code_ void tlk_mailbox_stimer_start_evt_callback(u8 *data)
{
    uint8_t new_latency_mode  = data[4] & 0x0F;
    uint8_t bt_page_state     = (data[4] & 0x80) >> 7;
    uint8_t ultra_low_latency = (data[4] & 0x40) >> 6;

    ll_aud_gpio_toggle_stimer_irq(new_latency_mode);

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PB6, 0);
    gpio_write(GPIO_PB6, 1);
    gpio_write(GPIO_PB6, 0);
#endif
    ll_audio_set_ultra_low_latency_flag(ultra_low_latency);

    async_audio_ctx.sco_num_in_bt = data[6];

    //    if (tlkmdi_bt_voice_is_busy() && data[6] == 0) {
    //        return;
    //    }

    // gpio_set_high_level(GPIO_PC1);
    // gpio_set_low_level(GPIO_PC1);

    g_stimer_start_cnt = 0;
    ;
    tmemcpy((uint8_t *)&async_audio_ctx.stimer_irq_tick, data, 4);
    async_audio_ctx.mix_tick = async_audio_ctx.stimer_irq_tick;

#if (TLK_STK_TPH_ENABLE)
    if ((app_tph_headset_get_mode() & TPH_HOST_MODE_BT_VOICE) && tlkmdi_tpsll_audio_is_busy()) {
#elif (TLK_STK_TPT_ENABLE)
    if ((app_tph_headset_get_mode() & TPT_HOST_MODE_BT_VOICE) && tlkmdi_tpsll_audio_is_busy()) {
#endif
        return;
    }

#ifdef SLET_ll_aud_stimer_start
    log_tick_general(SL_AUDIO_PATH_LOG_EN, SLET_ll_aud_stimer_start);
#endif

    async_audio_ctx.bt_page_state = bt_page_state;

    if (async_audio_ctx.latency_mode == TPSLL_LATENCY_NORMAL && (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
                                                                 || tlkmdi_anc_btmusic_is_busy()
#endif
                                                                     )) {
        if (async_audio_ctx.mix_rptr == async_audio_ctx.stimer_ref_wptr) {
            //async_audio_ctx.mix_rptr = (async_audio_ctx.mix_rptr + 1) & 0x0f;
        }
    }

    async_audio_ctx.latency_mode             = new_latency_mode;
    async_audio_ctx.stimer_ref_wptr          = data[5] & 0x0f;
    async_audio_ctx.next_tpsll_rptr_and_flag = data[5];
    async_audio_ctx.sco_num_in_bt            = data[6];

    async_audio_ctx.tpsll_uplink_enc_pkt_sent_cnt = 2;
    async_audio_ctx.tpsll_mix_lc3_dec_cnt         = 0;

    ll_audio_start_timer_encode_task();

#ifdef SL16_mix_ll_ref_wptr
    log_b16(SL_AUDIO_PATH_LOG_EN, SL16_mix_ll_ref_wptr, async_audio_ctx.stimer_ref_wptr << 8 | new_latency_mode);
#endif

    ll_aud_gpio_toggle_test(async_audio_ctx.stimer_ref_wptr);

#if 0
    uint8_t wptr_flag = (data[5] & 0x80) >> 7;
    gpio_write(GPIO_PB5, wptr_flag);
    ll_aud_gpio_toggle_test(async_audio_ctx.stimer_ref_wptr);
#endif
}

audio_ram_code void ll_audio_headset_post_pcm_data(codec_mono_int *p_pcm_left, codec_mono_int *p_pcm_right, uint16_t samples_num)
{
    codec_mono_int pcm_stereo[48 * 2];

    uint16_t num;
#if !AUDIO_PATH_24BITS_EN
    (void)p_pcm_left;
    (void)p_pcm_right;
#endif
    while (samples_num) {
        num = samples_num > 48 ? 48 : samples_num;
        samples_num -= num;

#if 0
        static int sin_48k_count = 0;

        for (int i = 0; i < num; i++) {
            pcm16_tmp[i] = ll_sin_48k_mono_d1[sin_48k_count++];
            pcm32[i] = (uint16_t)pcm16_tmp[i] | (pcm16_tmp[i] << 16);
            sin_48k_count %= 48;
        }
#endif

#if 0
        static int sin_count = 0;

        for (int i = 0; i < num; i++) {
            pcm_mono[i] = ll_sin_48k_mono_24bit[sin_count++] / 60;
            sin_count %= 48;
        }
#endif

#if AUDIO_PATH_24BITS_EN
        int16_t pcm16_tone_left[48];
        int16_t pcm16_tone_right[48];
        if (tone_is_playing()) {
            tmemset(g_tone_buff, 0, sizeof(g_tone_buff));
            tone_get_sample(g_tone_buff, num * sizeof(tone_int), 48000);

            if (tlkmdi_audio_get_tone_mix_mode()) {
                tmemset(pcm16_tone_left, 0, sizeof(pcm16_tone_left));
                pcm_s32_2_s16(pcm16_tone_left, p_pcm_left, num);

                tmemset(pcm16_tone_right, 0, sizeof(pcm16_tone_right));
                pcm_s32_2_s16(pcm16_tone_right, p_pcm_right, num);

                g_tone_buff_ptr = app_mix_tone(pcm16_tone_left, pcm16_tone_right, g_tone_buff, num);

                for (int i = 0; i < num; i++) {
                    pcm_stereo[2 * i]     = (int32_t)pcm16_tone_left[i] << 8;
                    pcm_stereo[2 * i + 1] = (int32_t)pcm16_tone_right[i] << 8;
                }
            } else {
                for (int i = 0; i < num; i++) {
                    int32_t left_tone  = (int32_t)g_tone_buff[2 * i] << 8;
                    int32_t right_tone = (int32_t)g_tone_buff[2 * i + 1] << 8;

                    pcm_stereo[2 * i]     = left_tone;
                    pcm_stereo[2 * i + 1] = right_tone;
                }
            }
        } else {
#if AUDIO_TWS_MODE
            for (int i = 0; i < num; i++) {
                pcm_stereo[i] = p_pcm_left[i];
            }
#else
            for (int i = 0; i < num; i++) {
                pcm_stereo[2 * i]     = p_pcm_left[i];
                pcm_stereo[2 * i + 1] = p_pcm_right[i];
            }
#endif
        }
#endif

        codec_mono_int pcm32_stereo_ppm[48 * 2 + 20];

#if LL_AUDIO_PPM_ENABLE
#if AUDIO_TWS_MODE
        uint8_t channel = ALG_CHANNEL_LEFT;
#else
        uint8_t channel = ALG_CHANNEL_STEREO;
#endif

#if LL_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PC0, 1);
#endif
        uint16_t length = ll_audio_spk_ppm_process((uint8_t *)pcm_stereo, (uint8_t *)pcm32_stereo_ppm, 48, channel);
        if (length != 48) {
            //tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, ">>>>>>>>>> ll aud ppm len: %d", length);
            g_ppm_cnt++;
        }
#if LL_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PC0, 0);
#endif
        g_ppm_sample_cnt += length - 48;
#else
        uint16_t length = num;
        for (int i = 0; i < length; i++) {
            pcm32_stereo_ppm[2 * i]     = (int32_t)pcm_stereo[2 * i];
            pcm32_stereo_ppm[2 * i + 1] = (int32_t)pcm_stereo[2 * i + 1];
        }

#endif

#if TLKALG_EQ_ENABLE
        codec_mono_int         eq_out[48 * 2 + 20];
        audio_alg_interface_t *p_audio_alg = audio_alg_get_interface_by_type(ALG_EQ);
        p_audio_alg->audio_alg_process((uint8_t *)pcm32_stereo_ppm, (uint8_t *)eq_out, length, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
#endif

#if 0 //CODEC_DAC_MONO_MODE
		uint8_t channel = tlkmdi_bt_tpt_audio_getCurChannel();
		tlkmdi_bt_tpt_audio_getMonoPcmData(pcm32_stereo_ppm, pcm32_mono_ppm, length, channel);
		tlkdrv_codec_fillSpkBuff((uint8_t *)pcm32_mono_ppm, length * sizeof(codec_mono_int));
#else
#if TLKALG_EQ_ENABLE
        tlkdrv_codec_fillSpkBuff((uint8_t *)eq_out, length * sizeof(codec_int));
#else
        tlkdrv_codec_fillSpkBuff((uint8_t *)pcm32_stereo_ppm, length * sizeof(codec_int));
#endif
#endif
    }
}

/**
 * @brief Check mic samples in FIFO and reset MIC samples if they are not within the threshold.
 * @param[in] samples_num - Number of samples.
 * @param[in] tolerance - Tolerance value.
 * @return none
 */
audio_ram_code void ll_audio_sync_mic_samples(uint16_t samples_num, uint16_t tolerance)
{
    uint16_t num = tlkdrv_codec_get_mic_avail_samples();

    if (num < samples_num - tolerance || num > samples_num + tolerance) {
        tlkdrv_codec_sync_mic_samples(samples_num);

        tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "Reset MIC Samples %d %d %d", num, samples_num, tolerance);
    }
}

/**
 * @brief Check the samples in FIFO and reset playback samples if they are not within the threshold.
 * @param[in] none
 * @return none
 */
audio_ram_code void ll_audio_sync_samples(void)
{
    //uint16_t samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();

#if LE_AEC_ENABLE
    int16_t ref = 100 + 50;
#else
#if AUDIO_TWS_MODE
    int16_t ref = 180;
#else
    int16_t ref = 200; //250;
#endif

#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
    ref = 132;
#endif

    if (ll_audio_get_ultra_low_latency_flag()) {
#if AUDIO_TWS_MODE
        ref = 88;
#else
        ref = 120; //132;
#endif
    }
#endif

#if 0 //TWS_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PB0, 0);
    gpio_write(GPIO_PB0, 1);
    gpio_write(GPIO_PB0, 0);
#endif

    //ll_audio_spk_ppm_adjust(&spk_ppm);

    uint16_t samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();

    if (s_ll_audio_sync_flag) {
        s_clk_diff_remains   = 0;
        s_ll_audio_sync_flag = 0;
    }

    //uint16_t samples_in_fifo1 = samples_in_fifo;

    uint32_t clk_cur = 0;
#if TLKALG_ANC_ENABLE
    if (tlkmdi_anc_ll_audio_is_busy()) {
        clk_cur = g_codec_spk_buff[GET_PCM_DATA_TICK_INDEX] + 1000;
    } else
#endif
    {
        clk_cur = clock_time();
    }

    uint32_t ref_tus_mic_enc = ll_audio_get_tus_mic_enc();
    uint32_t ref_tus_mic_dec = ll_audio_get_tus_spk_dec();
    uint32_t dec_task_ts     = async_audio_ctx.tick_stimer_trigger + (ref_tus_mic_enc + ref_tus_mic_dec) * 24;

    int clk_diff = clk_cur - dec_task_ts;
    clk_diff += s_clk_diff_remains;

    int tick_of_48samples = TICK_OF_BLOCK_SAMPLE_48k;
    int nsb               = NUM_OF_SAMPLES_PER_BLOCK_48K;
    int tick_per_sample   = tick_of_48samples / nsb;
    int diff_samples      = abs_ram(clk_diff) / tick_per_sample;
    s_clk_diff_remains    = abs_ram(clk_diff) % tick_per_sample;

    // |stimer_____|dec_task_ts__ref__|clk_cur_<_ref_|,  clk_diff = clk_cur - dec_task_ts
    if (clk_diff >= 0) {
        samples_in_fifo = samples_in_fifo + diff_samples;
        //s_clk_diff_remains = -s_clk_diff_remains;
    } else { // |stimer_____|clk_cur_>_ref_|dec_task_ts__ref__|
        samples_in_fifo    = samples_in_fifo - diff_samples;
        s_clk_diff_remains = -s_clk_diff_remains;
    }

    if ((samples_in_fifo < ref - 10 || samples_in_fifo > ref + 10)) {
        if (clk_diff >= 0) {
            ref = ref - diff_samples;
        } else {
            ref = ref + diff_samples;
        }

        tlkdrv_codec_sync_speaker_samples(ref);
        tlkmdi_anc_inform_dsp_sync();
        uint16_t samples_in_fifo_tmp = ref;
        samples_in_fifo              = tlkdrv_codec_get_spk_avail_samples();

        asrc_feedback_init();
        s_ll_audio_sync_flag = 1;
        g_ppm_cnt            = 0;
        g_ppm_sample_cnt     = 0;

#if TWS_AUDIO_PATH_GPIO_DEBUG
//        gpio_write(GPIO_PA1, 0);
//        gpio_write(GPIO_PA1, 1);
//        gpio_write(GPIO_PA1, 0);
#endif

        tlkapi_printf(APP_AUDIO_LOG_EN, "Reset playback samples: %d, %d, ref:200/ %d", samples_in_fifo_tmp, samples_in_fifo, ref);
    } else {
        //ll_audio_spk_ppm_adjust(&spk_ppm, samples_in_fifo);
        s_ll_audio_10ms_cnt++;

        if (s_ll_audio_10ms_cnt == 50) {
            s_ll_audio_10ms_cnt = 0;

            int diff_s   = samples_in_fifo - ref;
            int ppm_step = 0;

            if (samples_in_fifo == ref) {
                s_ll_spk_ppm_int = s_ll_spk_ppm_int_ref;
            } else if (ref > samples_in_fifo) {
                // PPM is -
                diff_s = abs_ram(diff_s);
                if (diff_s > 1) {
                    if (diff_s == 2) {
                        ppm_step = -80;
                    } else {
                        ppm_step = -100;
                    }
                } else if (diff_s == 1) {
                    ppm_step = -30;
                } else {
                    ppm_step = -5;
                }

                s_ll_spk_ppm_int = -s_ll_spk_ppm_int_ref + ppm_step;
                if (s_ll_spk_ppm_int <= -400) {
                    s_ll_spk_ppm_int = -399;
                }
                s_ll_spk_ppm_frac = 0;
            } else {
                // PPM is +
                diff_s = abs_ram(diff_s);
                if (diff_s > 1) {
                    if (diff_s == 2) {
                        ppm_step = 60;
                    } else {
                        ppm_step = 80;
                    }
                } else if (diff_s == 1) {
                    ppm_step = 20;
                } else {
                    ppm_step = 0;
                }

                s_ll_spk_ppm_int = s_ll_spk_ppm_int_ref + ppm_step;
                if (s_ll_spk_ppm_int >= 400) {
                    s_ll_spk_ppm_int = 399;
                }
                s_ll_spk_ppm_frac = 0;
            }

            audio_alg_interface_t *p_ll_spk_ppm_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
            if (p_ll_spk_ppm_alg_if && p_ll_spk_ppm_alg_if->audio_alg_param_set) {
                if (s_ll_ppm_spk_value_set != s_ll_spk_ppm_int) {
                    p_ll_spk_ppm_alg_if->audio_alg_param_set(0, &s_ll_spk_ppm_int);
                    s_ll_ppm_spk_value_set = s_ll_spk_ppm_int;
                }
            }

            s_ll_audio_debug_output_cnt++;
            if (s_ll_audio_debug_output_cnt == 6) {
                s_ll_audio_debug_output_cnt = 0;
                tlkapi_printf(APP_AUDIO_LOG_EN, "samp: %d, new_ppm:%d, ref_ppm_int:%d, ppm_cnt:%d, ppm_samples: %d", samples_in_fifo, s_ll_spk_ppm_int, s_ll_spk_ppm_int_ref,
                              g_ppm_cnt, g_ppm_sample_cnt);
            }
        }
    }
}

/**
 * @brief Check the low latency audio status then update async audio parameters.
 * @param[in] p_src - Async audio data.
 * @return none
 */
audio_ram_code void ll_audio_packet_check(uint8_t *p_src)
{
    //    uint16_t threshold = 100;
    if (p_src == NULL) {
        tlkapi_printf(APP_AUDIO_LOG_EN, "tpsll aud check p_src is NULL");
        return;
    }

#if (TLK_STK_TPH_ENABLE)
    if (!(app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO)) {
#elif (TLK_STK_TPT_ENABLE)
    if (!(app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_AUDIO)) {
#endif
        async_audio_ctx.pkt_audio = 0;
    }

    else if ((p_src[ASYNC_AUDIO_DATA_FLAG] & ASYNC_AUDIO_DATA_READY0)) {
        async_audio_ctx.plc_num = 0;

        if (!async_audio_ctx.pkt_audio) {
            async_audio_ctx.pkt_audio  = 1;
            async_audio_ctx.audio_mute = 0;
        } else if (async_audio_ctx.pkt_audio && !(async_audio_ctx.pkt_audio & BIT(31))) {
            async_audio_ctx.pkt_audio++;
        }
    } else if (!app_audio_ctl_get_audio_mode()) {
        //log_event(SL_APP_AUDIO_EN, SLEV_le_rx_miss_audio_pkt);
        //tlkapi_printf(APP_AUDIO_LOG_EN, "[MISS] packet check: %x %x %x", p_src[0], p_src[1], p_src[2]);

#if TWS_AUDIO_PATH_GPIO_DEBUG
//        gpio_write(GPIO_PF4, 0);
//        gpio_write(GPIO_PF4, 1);
//        gpio_write(GPIO_PF4, 0);
#endif

#if TWS_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PB6, 0);
        gpio_write(GPIO_PB6, 1);
        gpio_write(GPIO_PB6, 0);
#endif

        if (!(p_src[ASYNC_AUDIO_DATA_FLAG] & ASYNC_AUDIO_DATA_READY0)) {
            p_src[ASYNC_AUDIO_DATA_FLAG] = ASYNC_AUDIO_DATA_NULL0;

#if DSP_GPIO_DBG
//			gpio_write(GPIO_PG1, 0);
//			gpio_write(GPIO_PG1, 1);
//			gpio_write(GPIO_PG1, 0);
#endif
        }

#if (TLK_STK_TPH_ENABLE)
        if (!(app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO)) {
#elif (TLK_STK_TPT_ENABLE)
        if (!(app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_AUDIO)) {
#endif
            //async_audio_ctx.audio_per = 0;
            async_audio_ctx.pkt_audio = 0;
        } else if (async_audio_ctx.pkt_audio) {
        }

        if (async_audio_ctx.dump_mask & ASYNC_DUMP_NULL_PACKET) {
            tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "audio packet null-2: %d", async_audio_ctx.audio_per);
        }
    }
}

/**
 * @brief      Open codec for audio processing.
 * @param      None.
 * @return     None.
 */
__attribute__((noinline)) void ll_audio_open_codec(void)
{
//g_codec_cfg.sample_rate = 48000;
#if CODEC_DAC_MONO_MODE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_24, 48000, 4096);
#if AUDIO_TWS_MODE
    if (ll_audio_get_ultra_low_latency_flag()) {
        tlkdrv_codec_sync_speaker_samples(240);
    } else
#endif
    {
        tlkdrv_codec_sync_speaker_samples(480);
    }

#else
#if TLKADU_MIDBUF_ENABLE
    tlkaud_set_audio_mode(AUDIO_TPSLL_SPK);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000, 4096);
#endif
    if (ll_audio_get_ultra_low_latency_flag()) {
        tlkdrv_codec_sync_speaker_samples(480);
    } else {
        tlkdrv_codec_sync_speaker_samples(480 * 2);
    }
#endif
    //audio_ctl_set_audio_mode(ASYNC_FLOW_LL_AUDIO);
}

/**
 * @brief      Get mic data from codec.
 * @param      None.
 * @return     None.
 */
audio_ram_code uint8_t ll_codec_get_mic_data(int *p_des, uint16_t samples, int mode)
{
    (void)mode;
#if TLKADU_MIDBUF_ENABLE
    mic_buf_typ mic_data[samples];
    adc_mono    mono_data[samples];
    adc_mono   *psrc     = (adc_mono *)mic_data;
    mic_chnl_e  mic_chnl = 0;

    tlkaud_sidetone_get_mic_data_16k((mic_buf_typ *)mic_data, samples, &mic_chnl);

    if (mic_chnl == MIC_CHNL_STEREO) {
        for (int i = 0; i < samples; i++) {
            mono_data[i] = psrc[2 * i];
        }
        psrc = mono_data;
    } else {
        (void)mono_data;
    }

    if (4 == sizeof(adc_mono)) {
        tmemcpy(p_des, psrc, samples * 4);
    } else if (2 == sizeof(adc_mono)) {
        for (int i = 0; i < samples; i++) { //16bit -> 24bit
            *p_des++ = (int32_t)(psrc[i] << 8);
        }
    }

#else
    adc_int mic_stereo_16k[160];
    //gpio_set_high_level(GPIO_CHN0);
    tlkdrv_codec_readMicData((uint8_t *)mic_stereo_16k, samples * sizeof(adc_int), 0); //stereo
    int *psrc = (int *)mic_stereo_16k;
    for (int i = 0; i < samples; i++) { //stereo -> mono
        p_des[i] = psrc[2 * i];
    }
    //gpio_set_low_level(GPIO_CHN0);
#endif
    return TRUE;
}

extern uint32_t g_dbg_mic_avail_samples;
extern uint8_t *ll_audio_mix_get_mic_buff(void);

/**
 * @brief Low latency audio mic data process task, called by timer0 irq handler.
 * @param[in] idx - Mic ID.
 * @return none
 */
audio_ram_code uint8_t ll_audio_mic_data_process_task_mcu(uint8_t idx)
{
    (void)idx;
    uint8_t         enc_data[64] = {0};
    int32_t         pcm_24bit[200];
    static uint32_t tick_mute = 0;

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && DSP_NN_NS_BUFF_DISENABLE)
    int     pcm_to_dsp[320 * 3];
    uint8_t pcm_ret = 0;
#endif

#if (TLK_STK_TPH_ENABLE)
    int16_t cur_ll_voice_mode = (app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_PHONE) >> 5;
#elif (TLK_STK_TPT_ENABLE)
    int16_t cur_ll_voice_mode = (app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_PHONE) >> 5;
#endif
    uint16_t data_len = ll_audio_get_frame_length() / 1000 * 16;

    //configuration after mode changing
    if (s_ll_voice_mode != cur_ll_voice_mode) {
        //tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "s_ll_voice_mode: %d, cur: %d %x", s_ll_voice_mode, cur_ll_voice_mode, app_tph_headset_get_mode());
        s_ll_voice_mode = cur_ll_voice_mode;
        if (s_ll_voice_mode) {
#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE)
            tlkmw_dsp_resume();
            ipc_msg_register_data_process_done_cb(ll_voice_dsp_msg_process_callback, LL_VOICE, LL_VOICE_ID);
            ll_voice_clear_dsp_ret_buff_status();
#if DSP_NN_NS_BUFF_DISENABLE
            dsp_voice_nn_comm_reset_status();
#endif
#endif

#if DONGLE_VOICE_MIC_EN
            if (!ll_mix_bt_music_get_ll_mic_start_flag())
#endif
            {
                tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "[TPSLL VOICE] MIC OPEN: %x %d", app_tph_headset_get_mode(), idx);

                async_audio_ctx.mic_state = 1;
#if TLKADU_MIDBUF_ENABLE
                tlkaud_set_audio_mode(AUDIO_TPSLL_MIC);
#else
                tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 16000, 0);

                tlkdrv_codec_sync_mic_samples(data_len * 2);
#endif
            }
            return 0;
        } else {
            tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "[TPSLL VOICE] MIC CLOSE");
#if TLKADU_MIDBUF_ENABLE
            tlkaud_clear_audio_mode(AUDIO_TPSLL_MIC);
#else
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
#endif
            async_audio_ctx.mic_state = 0;

#if (TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE)
            // pm_set_dig_module_power_switch(FLD_PD_DSP_EN, PM_POWER_DOWN);
            if (tlkmdi_tpsll_audio_is_busy()) {
                tlkmw_dsp_pause();
            }
#endif
        }
    }

    if (s_ll_voice_mode == 0) {
        return 0;
    }

#if ULTRA_LOW_LATENCY_EN
    if (g_tpsll_ultra_ll_ctx.ultra_ll_mode_change) {
        tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "[TPSLL ultra mode]");
        return 0;
    }
#endif

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PA2, 1);
#endif

    async_audio_ctx.tpsll_enc_is_ongoing = 1;

    uint8_t cur_state_is_mix_mode = 0;

    if (ll_audio_is_in_mix_mode() && (tlkmdi_bt_music_is_busy() || tlkmdi_bt_voice_is_busy())) {
        cur_state_is_mix_mode = 1;
    } else {
        cur_state_is_mix_mode = 0;
    }

    // codec_get_sin_data(pcm, 120);  ll_codec_get_mic_data((int16_t *)pcm, 80, 1)
    // 5000us / 62.5us = 80 samples, 16K
    // lc3 16K frame length: 10ms, 160 samples
    //if (tlkalg_lc3_24bit_get_data(pcm_24bit, 160)) {
#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
    if (ll_voice_dsp_dp_get_mic_data(pcm_24bit, 160)) {
#else
    if (!cur_state_is_mix_mode) {
        ll_codec_get_mic_data((int *)pcm_24bit, data_len, 0);
    } else {
        memcpy(pcm_24bit, g_ll_mic_mid_buf, data_len * sizeof(int));
    }

    {
#endif
        if (s_ll_voice_mode) {
            if (tick_mute) {
                //tmemset(pcm_24bit, 0, 160 * 2);
            }

#if TLKALG_LC3_24BIT_ENC_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
#elif TLKALG_LC3_PLUS_ENC_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
#else
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_DEFAULT);
        (void)p_audio_alg_if;
#endif

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE)
            // gpio_set_high_level(GPIO_CHN0);
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(LL_VOICE_ID);
            p_dsp_app_ctx->alg_type          = LL_VOICE;
#if DSP_NN_NS_BUFF_DISENABLE
            pcm_ret = d25f_get_ll_voice_available_nn_buff_len((int *)pcm_24bit, pcm_to_dsp, data_len);
            if (pcm_ret == 1) {
                //                gpio_set_high_level(GPIO_PB0);
                d25f_send_audio_data_to_dsp((uint8_t *)pcm_to_dsp, 16 * 20 * 4, LL_VOICE_ID);
                //                gpio_set_low_level(GPIO_PB0);
            }
#else
#if 0 //TLKALG_ANC_ENABLE
            if (!tlkmdi_anc_is_busy() || (tlkmdi_anc_dsp_is_sync_data() && tlkmdi_anc_is_busy()))
#endif
            {
                d25f_send_audio_data_to_dsp((uint8_t *)pcm_24bit, data_len * sizeof(int), LL_VOICE_ID);
            }
#endif
            // gpio_set_low_level(GPIO_CHN0);

            if (ll_voice_get_avia_len(ll_dsp_wptr, ll_dsp_rptr) >= data_len) {
// gpio_set_high_level(GPIO_CHN2);
#if 0 //TLKALG_ANC_ENABLE
                if (tlkmdi_anc_is_busy() && !tlkmdi_anc_dsp_is_sync_data()) {
                    for(int j = 0; j < data_len; j++) {
                        pcm_24bit[j] = 0;
                    }
                } else
#endif
                {
                    for (int j = 0; j < data_len; j++) {
                        pcm_24bit[j] = ll_dsp_ret_data_buff[ll_dsp_rptr++];
                        ll_dsp_rptr %= TLKALG_DSP_RET_NN_DATA_LEN_LL;
                    }
                }

#if TWS_AUDIO_PATH_GPIO_DEBUG
                gpio_write(GPIO_PB0, 1);
#endif

#if (!MCU_CORE_TL752X_TEMP && !MCU_CORE_TL753X_TEMP)
                if (p_audio_alg_if->audio_alg_process) {
                    p_audio_alg_if->audio_alg_process((uint8_t *)pcm_24bit, (uint8_t *)enc_data, data_len, ALG_WIDTH_24, ALG_CHANNEL_LEFT);
                }
#endif

#if TWS_AUDIO_PATH_GPIO_DEBUG
                gpio_write(GPIO_PB0, 0);
#endif
            } else {
                tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "[LL AUD] ll voice have not enough dsp data");
            }
#else
        if (p_audio_alg_if->audio_alg_process) {
            int32_t *p_mic_buf = g_ll_mic_mid_buf;
            if (cur_state_is_mix_mode) {
                p_mic_buf = g_ll_mic_mid_buf;

                if (g_ll_mic_mute_cnt < 300) {
                    g_ll_mic_mute_cnt++;
                    tmemset(p_mic_buf, 0, data_len * sizeof(int32_t));
                }
            } else {
                p_mic_buf = pcm_24bit;
            }

            int ret = p_audio_alg_if->audio_alg_process((uint8_t *)p_mic_buf, (uint8_t *)enc_data, data_len, ALG_WIDTH_24, ALG_CHANNEL_LEFT);
            if (ret) {
                //tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 24bit enc, ret: %x", ret);
            }


            // tlkapi_printf(APP_AUDIO_LOG_EN, "mic_avail_samples: %d, enc:%d",
            //               g_dbg_mic_avail_samples,
            //               ll_audio_mix_enc_buff_available());
        }
#endif
            if (cur_state_is_mix_mode) {
                uint8_t *p_enc_buff   = ll_audio_mix_get_enc_buff_ptr();
                uint8_t  enc_data_len = ll_audio_get_headset_enc_frame_length();
                tmemcpy(p_enc_buff, enc_data, enc_data_len);
                ll_audio_mix_update_enc_buff_wptr();
            } else {
                ll_audio_post_audio_data_to_async_fifo(0, enc_data);
            }
        }
    }

    async_audio_ctx.tpsll_enc_is_ongoing = 0;
    async_audio_ctx.tpsll_mix_bt_dec_cnt = 0;

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PA2, 0);
#endif

    return 1;
}

/**#if (!MCU_CORE_TL752X_TEMP && !MCU_CORE_TL753X_TEMP)
 * @brief  tpsll audio decoder task
 * @param  p_audio: pointer to audio data
 * @return none
 */
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
audio_ram_code void ll_audio_decoder_task_mcu(uint8_t *p_audio, int times)
#else
audio_ram_code void ll_audio_decoder_task_mcu(uint8_t *p_audio)
#endif
{
    uint16_t i;
    uint8_t  mute = 0;
    uint8_t  data_ready0;
#if AUDIO_TWS_MODE
    uint8_t *pcm_data = NULL;
    int32_t  pcm_24bit_left[SAMPLES];
    int32_t  pcm_24bit_right[SAMPLES];
#endif
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
    int32_t pcm_24bit_stereo[SAMPLES + 96];
#else
    int32_t pcm_24bit_stereo[SAMPLES * 2 + 96];
#endif
    uint8_t *p_lc3_data;
    uint8_t  plc_en = 0;

    if (p_audio == NULL) {
#if DSP_GPIO_DBG
        gpio_write(GPIO_PJ0, 0);
        gpio_write(GPIO_PJ0, 1);
        gpio_write(GPIO_PJ0, 0);
#endif
        tlkapi_printf(APP_AUDIO_LOG_EN, "tpsll aud decoder, p_src is NULL");
        return;
    }

    data_ready0 = p_audio[ASYNC_AUDIO_DATA_FLAG];

    /* check number of samples in playback buffer */
#if (TLK_STK_TPH_ENABLE)
    if (!(app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO) || !async_audio_ctx.pkt_audio) {
#elif (TLK_STK_TPT_ENABLE)
    if (!(app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_AUDIO) || !async_audio_ctx.pkt_audio) {
#endif
        if (s_ll_pkt_audio_last != async_audio_ctx.pkt_audio) {
            tlkdrv_codec_muteSpkBuff();
            async_audio_ctx.audio_mute = 1;
        }

        tlkapi_printf(APP_AUDIO_LOG_EN, "[LL AUD] async_audio_ctx.pkt_audio: %x", async_audio_ctx.pkt_audio);

        mute = 1;
    }

    if (async_audio_ctx.audio_mute || async_audio_ctx.pkt_audio < 1) {
        mute = 1;
    }

    /* channel 0 */
    p_audio = p_audio + 2;

    if (!app_tph_dongle_is_connected()
#if ULTRA_LOW_LATENCY_EN
        || g_tpsll_ultra_ll_ctx.ultra_ll_mode_change
#endif
    ) {
        tmemset(pcm_24bit_stereo, 0, sizeof(pcm_24bit_stereo));
    } else if (mute || (async_audio_ctx.plc_num & BIT(2))) {
        tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "channel 0 ll_audio_enc_mute %d", async_audio_ctx.plc_num);

        tmemset(pcm_24bit_stereo, 0, sizeof(pcm_24bit_stereo));
    } else {
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
        uint16_t data_len = ASYNC_AUDIO_DATA_LEN; //ll_audio_get_enc_frame_length()
#else
        uint16_t data_len = ll_audio_get_enc_frame_length();
#endif
        if ((data_ready0 & ASYNC_AUDIO_DATA_NULL0) || (data_ready0 == 0)) {
            if (!(async_audio_ctx.plc_num & BIT(2))) {
                async_audio_ctx.plc_num++;
            }
#if TLKALG_LC3_24BIT_DEC_ENABLE
            data_len = 0;
#endif
            plc_en = 1;
        }

#if TLKALG_LC3_24BIT_DEC_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
#elif TLKALG_LC3_PLUS_DEC_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
#else
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_DEFAULT);
#endif
        if (p_audio_alg_if->audio_alg_process) {
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
            gpio_write(GPIO_PC0, 1);
#endif

#if AUDIO_TWS_MODE
            uint8_t channel = ALG_CHANNEL_LEFT;

            if (tlkmdi_bt_tpt_isLeft()) {
                p_lc3_data = p_audio;
            } else {
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
                p_lc3_data = p_audio + data_len / 2;
#else
                p_lc3_data = p_audio + data_len;
#endif
            }
#else
            uint8_t channel = ALG_CHANNEL_STEREO;
            p_lc3_data      = p_audio;
#endif
            channel = (plc_en << 4) | (channel & 0x0F);

#ifdef SL01_ll_audio_dec
            log_task_begin_irq(SL_LL_AUDIO_LOG_EN, SL01_ll_audio_dec);
#endif
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
            int ret = p_audio_alg_if->audio_alg_process(p_lc3_data + times * data_len, (uint8_t *)pcm_24bit_stereo, data_len / 2, ALG_WIDTH_24, channel);
#else
            int ret = p_audio_alg_if->audio_alg_process(p_lc3_data, (uint8_t *)pcm_24bit_stereo, data_len, ALG_WIDTH_24, channel);
#endif
#ifdef SL01_ll_audio_dec
            log_task_end_irq(SL_LL_AUDIO_LOG_EN, SL01_ll_audio_dec);
#endif

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
            gpio_write(GPIO_PC0, 0);
#endif
            (void)ret;
        }
    }

    if ((data_ready0 & ASYNC_AUDIO_DATA_NULL0) || (data_ready0 == 0)) {
        tlkapi_printf(APP_AUDIO_LOG_EN, "audio packet null-1: ready: %d, pkt_audio: %d, mute: %d", data_ready0, async_audio_ctx.pkt_audio, async_audio_ctx.audio_mute);

#ifdef SLET_ll_audio_null
        log_tick_general(SL_LL_AUDIO_LOG_EN, SLET_ll_audio_null);
#endif

#ifdef TWS_AUDIO_PATH_GPIO_DEBUG
//        gpio_write(GPIO_PC0, 0);
//        gpio_write(GPIO_PC0, 1);
//        gpio_write(GPIO_PC0, 0);
#endif
    }

    s_ll_pkt_audio_last = async_audio_ctx.pkt_audio;

#if AUDIO_TWS_MODE
    pcm_data = (uint8_t *)pcm_24bit_stereo;
    if (ll_audio_get_ultra_low_latency_flag()) {
        tmemcpy(pcm_24bit_left, pcm_data, ULTRA_LL_SAMPLES * sizeof(codec_mono_int));
        for (i = 0; i < ULTRA_LL_SAMPLES; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)pcm_24bit_left + i, (codec_mono_int *)pcm_24bit_right + i, 48);
        }
    } else {
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
        tmemcpy(pcm_24bit_left, pcm_data, SAMPLES / 2 * sizeof(codec_mono_int));
        for (i = 0; i < SAMPLES / 2; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)pcm_24bit_left + i, (codec_mono_int *)pcm_24bit_right + i, 48);
        }
#else
        tmemcpy(pcm_24bit_left, pcm_data, SAMPLES * sizeof(codec_mono_int));
        ll_audio_headset_post_pcm_data((codec_mono_int *)pcm_24bit_left, (codec_mono_int *)pcm_24bit_right, 48);
        ll_audio_headset_post_pcm_data((codec_mono_int *)pcm_24bit_left + 48, (codec_mono_int *)pcm_24bit_right + 48, 48);

        for (i = 0; i < 384; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)pcm_24bit_left + 96 + i, (codec_mono_int *)pcm_24bit_right + 96 + i, 48);
        }
#endif
    }
#else
    codec_mono_int *p_pcm_24bit_left = pcm_24bit_stereo;
    codec_mono_int *p_pcm_24bit_right; // = pcm_24bit_stereo + SAMPLES;

    if (ll_audio_get_ultra_low_latency_flag()) {
        p_pcm_24bit_right = pcm_24bit_stereo + ULTRA_LL_SAMPLES;
        for (i = 0; i < ULTRA_LL_SAMPLES; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)p_pcm_24bit_left + i, (codec_mono_int *)p_pcm_24bit_right + i, 48);
        }
    } else {
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
        p_pcm_24bit_left  = pcm_24bit_stereo;
        p_pcm_24bit_right = pcm_24bit_stereo + SAMPLES / 2;
        for (i = 0; i < SAMPLES / 2; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)p_pcm_24bit_left + i, (codec_mono_int *)p_pcm_24bit_right + i, 48);
        }
#else
        p_pcm_24bit_right = pcm_24bit_stereo + SAMPLES;
        ll_audio_headset_post_pcm_data((codec_mono_int *)p_pcm_24bit_left, (codec_mono_int *)p_pcm_24bit_right, 48);
        ll_audio_headset_post_pcm_data((codec_mono_int *)p_pcm_24bit_left + 48, (codec_mono_int *)p_pcm_24bit_right + 48, 48);

        for (i = 0; i < 384; i += 48) {
            ll_audio_headset_post_pcm_data((codec_mono_int *)p_pcm_24bit_left + 96 + i, (codec_mono_int *)p_pcm_24bit_right + 96 + i, 48);
        }
#endif
    }
#endif
}

/**
 * @brief Low latency audio main handler, including encoding and decoding process.
 * @param[in] none
 * @return none
 */
audio_ram_code void ll_audio_main(void)
{
    uint32_t t_diff;
    uint8_t *p_src = NULL;
    uint32_t tick_current;

    uint32_t ref_tus_spk_dec = ll_audio_get_tus_spk_dec();
    uint16_t stimer_ref_us   = ll_audio_get_frame_length();
#ifdef LL_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PA0, 1);
#endif

#ifdef SL01_audio_task
    log_task_begin_irq(SL_AUDIO_PATH_LOG_EN, SL01_audio_task);
#endif

#ifdef SL16_codec_rptr
    // uint32_t rptr = tlkdrv_codec_get_speaker_rptr();
    // log_b16_irq(SL_APP_AUDIO_EN, SL16_codec_rptr, rptr);
#endif

#ifdef SL01_ll_timer0_irq
    log_task_begin_irq(SL_STACK_TPH_RF_EN, SL01_ll_timer0_irq);
#endif

    if (ll_audio_is_encoding_state()) {
        async_audio_ctx.pkt_rptr &= 6; // 0110

        tlkmdi_audio_task_set_next_irq(ref_tus_spk_dec + async_audio_ctx.tus_mic);
        async_audio_ctx.tick_timer += (ref_tus_spk_dec + async_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;

#if AUDIO_TWS_MODE
        if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
            ll_audio_mic_data_process_task_mcu(0);
        }

        if (tlkmdi_bt_tpt_isSlave() && async_audio_ctx.mic_state) {
            tlkapi_trace(LL_AUDIO_DBG_FLAG, LL_AUDIO_DBG_SIGN, "[TPSLL VOICE] SLAVE CLOSE MIC");
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
            async_audio_ctx.mic_state = 0;
            s_ll_voice_mode           = 0;

#if TLK_MW_DSP_COMM_ENABLE
            tlkmw_dsp_pause();
#endif
        }
#else
        ll_audio_mic_data_process_task_mcu(0);
#endif

    } else {
        ll_audio_sync_samples();

#if 1
        int8_t latency = 0;

        if (async_audio_ctx.latency_mode == TPSLL_LATENCY_NORMAL) {
            latency = 0;
        } else if (async_audio_ctx.latency_mode == TPSLL_LATENCY_PKTLOSS) {
            latency = 2;
        }

        if (async_audio_ctx.bt_page_state && async_audio_ctx.latency_mode == TPSLL_LATENCY_NORMAL) {
            latency = 1;
        }
        int cur_tpsll_rptr = (async_audio_ctx.stimer_ref_wptr - latency) & 0x0f;

        async_audio_ctx.stimer_ref_wptr = (async_audio_ctx.stimer_ref_wptr + 1) % g_tpsll_enc_buf_ctx.tpsll_enc_buff_num;

        ll_aud_gpio_toggle_test(cur_tpsll_rptr);
        p_src = tpsll_get_enc_buff_by_rptr(cur_tpsll_rptr);
        if (p_src == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "p_src is NULL");
            p_src = ll_audio_enc_mute_buff;
        }
#else
#if AUDIO_PATH_24BITS_EN
        p_src = g_lc3_plus_enc_out_data; //sin_48k_mono_lc3_24bit_enc_new;
#else
        p_src = sin_48k_mono_lc3_enc;
#endif
#endif

        /* decoding */
        async_audio_ctx.pkt_rptr = 0x10;
        tick_current             = clock_time();

        uint32_t ref_tus_mic_enc = ll_audio_get_tus_mic_enc();

        if ((tick_current - async_audio_ctx.tick0) > stimer_ref_us * SYSTEM_TIMER_TICK_1US) {
            async_audio_ctx.tick0 = tick_current;
            t_diff                = ref_tus_mic_enc - async_audio_ctx.tus_mic;
        } else {
            t_diff = (tick_current - async_audio_ctx.tick0) / SYSTEM_TIMER_TICK_1US;
            t_diff = stimer_ref_us - t_diff + ref_tus_mic_enc - async_audio_ctx.tus_mic;
            async_audio_ctx.tick0 += stimer_ref_us * SYSTEM_TIMER_TICK_1US;
            async_audio_ctx.tick_stimer_trigger = async_audio_ctx.tick0;
        }

        async_audio_ctx.tick_timer = async_audio_ctx.tick0 + (ref_tus_mic_enc - async_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;

        tlkmdi_audio_task_set_next_irq(t_diff);

        int16_t  diff_stimer_tick   = 0;
        uint32_t ref_tick           = stimer_ref_us * TICK_PER_US;
        int16_t  tick_diff_per_10ms = 0;
        uint32_t frame_length       = ll_audio_get_frame_length() / 100;

        if (s_asrc_init == 1) {
            if (s_stimer_irq_tick_last) {
                diff_stimer_tick = async_audio_ctx.stimer_irq_tick - s_stimer_irq_tick_last;

                tick_diff_per_10ms = diff_stimer_tick - ref_tick;

                tick_diff_sum_cnt++;
                tick_diff_sum += tick_diff_per_10ms;

                if (tick_diff_sum_cnt >= 50) {
                    uint32_t total_tick    = (frame_length * 50) * 1000 * TICK_PER_US + tick_diff_sum;
                    int32_t  tick_diff     = tick_diff_sum;
                    int32_t  tick_diff_abs = abs_ram(tick_diff);

                    double ppm_value       = (1000000.0 * tick_diff_abs / total_tick);
                    int    ppm_int_part    = (uint16_t)ppm_value;
                    int    fractional_part = (int)((ppm_value - ppm_int_part) * 1000);

                    s_ll_spk_ppm      = ppm_int_part * 1000 + fractional_part;
                    s_ll_spk_ppm_int  = (s_ll_spk_ppm + s_ll_spk_ppm_frac) / 1000;
                    s_ll_spk_ppm_frac = (s_ll_spk_ppm + s_ll_spk_ppm_frac) % 1000;

                    if (tick_diff >= 0) {
                        s_ll_spk_ppm_int_ref = s_ll_spk_ppm_int;
                    } else {
                        s_ll_spk_ppm_int_ref = s_ll_spk_ppm_int;
                    }

                    /*
                    tlkapi_printf(APP_AUDIO_LOG_EN, "ppm:%d, int:%d, f:%d, ref:%d, t_diff:%d",
                                  s_ll_spk_ppm, s_ll_spk_ppm_int, s_ll_spk_ppm_frac,
                                  s_ll_spk_ppm_int_ref, tick_diff);
                    */

                    tick_diff_sum_cnt      = 0;
                    tick_diff_sum          = 0;
                    s_stimer_irq_tick_last = 0;
                }
            }

            s_stimer_irq_tick_last = async_audio_ctx.stimer_irq_tick;
        }

        ll_audio_packet_check(p_src);
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
        ll_audio_decoder_task_mcu(p_src, 0);
        if (!ll_audio_get_ultra_low_latency_flag()) {
            ll_audio_decoder_task_mcu(p_src, 1);
        }
#else
        ll_audio_decoder_task_mcu(p_src);
#endif
        p_src[ASYNC_AUDIO_DATA_FLAG] = 0;
        uint16_t enc_frame_length    = ll_audio_get_enc_frame_length();
        tmemset(p_src, 0, enc_frame_length * 2 + 2);
    }

#ifdef LL_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PA0, 0);
#endif

#ifdef SL01_audio_task
    log_task_end_irq(SL_AUDIO_PATH_LOG_EN, SL01_audio_task);
#endif
}

#endif
