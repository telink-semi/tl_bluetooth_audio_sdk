/********************************************************************************************************
 * @file    tlkusb_uacSpk.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"

#if (TLKUSB_UAC_SPK_ENABLE)

#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"
#include "tlkalg/audio/audio_alg_interface.h"

#include "drivers.h"
#include "stack/stack.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"
/////////////////////////////////////
s16 last_speaker_vol   = 0x7fff;
u8  last_speaker_step  = 0xff;
s16 last_speaker1_vol  = 0x7fff;
u8  last_speaker1_step = 0xff;
////////////////////////////////////
int     usb_vol_music  = 0;
int     gradual_vol    = 0;
int     usb_vol1_music = 0;
int     gradual_vol1   = 0;
uint8_t gLeaPpmStatus  = 0;
// #if TLK_SIMPLE_PPM_EN
uint32_t ppm_tick_last      = 0;
uint32_t ppm_counter        = 0;
int32_t  tick_diff_val      = 0;
uint16_t spk_samples        = 0;
int      ppm_sample_counter = 0;
uint32_t over_tick[5]       = {0};
uint32_t ppm_tick           = 0;
uint8_t  g_ppm_flag         = 0;
uint32_t over_time_us       = PPM_WORK_OVERTIMER_US;
// #endif

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
const int16_t g_volume_relative_value_table[101] = {TLKUSB_AUDSPK_VOL_MIN,
                                                    0xc8a7,
                                                    0xcd7f,
                                                    0xd129,
                                                    0xd41d,
                                                    0xd696,
                                                    0xd8b6,
                                                    0xda93,
                                                    0xdc3c,
                                                    0xddbb,
                                                    0xdf18,
                                                    0xe058,
                                                    0xe17f,
                                                    0xe292,
                                                    0xe393,
                                                    0xe484,
                                                    0xe566,
                                                    0xe63d,
                                                    0xe708,
                                                    0xe7c9,
                                                    0xe881,
                                                    0xe930,
                                                    0xe9d8,
                                                    0xea79,
                                                    0xeb14,
                                                    0xeba9,
                                                    0xec38,
                                                    0xecc2,
                                                    0xed48,
                                                    0xedc9,
                                                    0xee45,
                                                    0xeebe,
                                                    0xef34,
                                                    0xefa6,
                                                    0xf014,
                                                    0xf080,
                                                    0xf0e9,
                                                    0xf14f,
                                                    0xf1b2,
                                                    0xf213,
                                                    0xf271,
                                                    0xf2ce,
                                                    0xf328,
                                                    0xf380,
                                                    0xf3d6,
                                                    0xf42a,
                                                    0xf47d,
                                                    0xf4ce,
                                                    0xf51d,
                                                    0xf56a,
                                                    0xf5b6 /*USB VOL:50*/,
                                                    0xf601,
                                                    0xf64a,
                                                    0xf692,
                                                    0xf6d9,
                                                    0xf71e,
                                                    0xf762,
                                                    0xf7a5,
                                                    0xf7e7,
                                                    0xf827,
                                                    0xf867,
                                                    0xf8a6,
                                                    0xf8e3,
                                                    0xf920,
                                                    0xf95c,
                                                    0xf996,
                                                    0xf9d0,
                                                    0xfa09,
                                                    0xfa42,
                                                    0xfa79,
                                                    0xfab0,
                                                    0xfae6,
                                                    0xfb1b,
                                                    0xfb4f,
                                                    0xfb83,
                                                    0xfbb6,
                                                    0xfbe9,
                                                    0xfc1a,
                                                    0xfc4c,
                                                    0xfc7c,
                                                    0xfcac,
                                                    0xfcdb,
                                                    0xfd0a,
                                                    0xfd38,
                                                    0xfd66,
                                                    0xfd93,
                                                    0xfdc0,
                                                    0xfdec,
                                                    0xfe18,
                                                    0xfe43,
                                                    0xfe6e,
                                                    0xfe98,
                                                    0xfec2,
                                                    0xfeeb,
                                                    0xff14,
                                                    0xff3c,
                                                    0xff64,
                                                    0xff8c,
                                                    0xffb3,
                                                    0xffda,
                                                    TLKUSB_AUDSPK_VOL_MAX};

const uint16_t g_volume_scale_table[101] = {0,    13,   14,   15,   16,   17,   19,   20,   22,   23,   25,    27,    29,    31,    33,    36,    39,   42,   45,   48,   52,
                                            56,   60,   64,   69,   74,   80,   86,   92,   99,   107,  115,   123,   133,   142,   153,   165,   177,  190,  204,  219,  236,
                                            253,  272,  293,  315,  338,  363,  390,  419,  451,  484,  520,   559,   601,   646,   694,   746,   801,  861,  925,  994,  1068,
                                            1148, 1233, 1325, 1424, 1530, 1644, 1766, 1898, 2039, 2191, 2355,  2530,  2719,  2921,  3139,  3373,  3624, 3894, 4184, 4495, 4830,
                                            5190, 5577, 5992, 6439, 6918, 7433, 7987, 8582, 9221, 9908, 10646, 11439, 12291, 13207, 14191, 15248, 16384};

const uint16_t g_cc_volume_scale_table[101] = {
    0,    18,   20,   23,   25,   29,   32,   38,   45,    52,    60,    70,    82,    89,    98,    109,   121,   134,   149,   165,   183,   194,   215,  239,  265,  294,
    326,  358,  384,  411,  440,  472,  506,  542,  581,   615,   667,   723,   784,   850,   921,   975,   1057,  1146,  1242,  1347,  1460,  1546,  1676, 1817, 1969, 2135,
    2314, 2451, 2596, 2750, 2913, 3086, 3269, 3462, 3667,  3885,  4115,  4359,  4617,  4891,  5181,  5332,  5488,  5648,  5813,  5983,  6157,  6337,  6522, 6790, 7053, 7326,
    7610, 7905, 8211, 8549, 8880, 9224, 9581, 9952, 10337, 10688, 10924, 11166, 11413, 11665, 11923, 12187, 12457, 12732, 13014, 13785, 14602, 15467, 16384};
#else
const s16 volume_relative_value_table[] = {0xa000, 0xbea8, 0xc825, 0xcdf1, 0xd221, 0xd568, 0xd8a1, 0xda64, 0xdc61, 0xde23, 0xdfb7, 0xe124, 0xe273, 0xe3a6, 0xe4c3, 0xe5cd, 0xe6c6,
                                           0xe7af, 0xe88c, 0xe95d, 0xea23, 0xeadf, 0xeb93, 0xec3f, 0xece3, 0xed81, 0xee19, 0xeeab, 0xef37, 0xefbf, 0xf043, 0xf0c1, 0xf13c, 0xf1b4,
                                           0xf227, 0xf298, 0xf305, 0xf36f, 0xf3d6, 0xf43b, 0xf49d, 0xf4fd, 0xf55a, 0xf5b5, 0xf60f, 0xf666, 0xf6bb, 0xf70e, 0xf760, 0xf7b0, 0xf7fe,
                                           0xf84b, 0xf897, 0xf8e0, 0xf929, 0xf970, 0xf9b6, 0xf9fb, 0xfa3e, 0xfa81, 0xfac2, 0xfb02, 0xfb41, 0xfb7f, 0xfbbc, 0xfbf9, 0xfc34, 0xfc6e,
                                           0xfca8, 0xfce0, 0xfd18, 0xfd4f, 0xfd86, 0xfdbb, 0xfdf0, 0xfe24, 0xfe58, 0xfe8a, 0xfebc, 0xfeee, 0xff1f, 0xff4f, 0xff7f, 0xffae, 0xffdc,
                                           0x0009, 0x0037, 0x0066, 0x0096, 0x00c5, 0x00f6, 0x0127, 0x0159, 0x018b, 0x01be, 0x01f2, 0x0226, 0x025c, 0x0291, 0x02c8, 0x0300};

const u16 volume_scale_table[] = {0,     18,    55,    92,    110,   130,   164,   201,   220,   251,   284,   320,   360,   400,   470,   520,   600,  680,  746,  902,  1050,
                                  1270,  1380,  1430,  1486,  1542,  1590,  1645,  1695,  1752,  1810,  1860,  1905,  1940,  1981,  2083,  2185,  2288, 2390, 2493, 2596, 2697,
                                  2799,  2902,  3009,  3112,  3215,  3385,  3556,  3726,  3896,  4066,  4237,  4407,  4578,  4748,  4918,  5081,  5268, 5439, 5609, 5780, 5952,
                                  6120,  6281,  6473,  6667,  6860,  7053,  7246,  7439,  7633,  7826,  8020,  8214,  8408,  8600,  8766,  8923,  9088, 9245, 9421, 9600, 9926,
                                  10253, 10579, 10907, 11213, 11550, 11826, 12153, 12906, 13449, 14109, 14973, 15920, 15966, 16105, 16196, 16278, 16384};

s16 volume_relative_table_iphone[] = {0xa000, 0xa180, 0xa300, 0xa480, 0xa600, 0xa780, 0xa900, 0xaa80, 0xac00, 0xad80, 0xaf00, 0xb080, 0xb200, 0xb380, 0xb500, 0xb680, 0xb800,
                                      0xb980, 0xbb00, 0xbc80, 0xbe00, 0xbf80, 0xc100, 0xc280, 0xc400, 0xc580, 0xc700, 0xc880, 0xca00, 0xcb80, 0xcd00, 0xce80, 0xd000, 0xd180,
                                      0xd300, 0xd480, 0xd600, 0xd780, 0xd900, 0xda80, 0xdc00, 0xdd80, 0xdf00, 0xe080, 0xe200, 0xe380, 0xe500, 0xe680, 0xe800, 0xe980, 0xeb00,
                                      0xec80, 0xee00, 0xef80, 0xf100, 0xf280, 0xf400, 0xf580, 0xf700, 0xf880, 0xfa00, 0xfb80, 0xfd00, 0xfe80, 0x0000, 0x0180, 0x3000};

u16 volume_scale_iphone_table[] = {0,    55,   58,   62,   69,   78,   87,   98,   110,  123,  138,  155,  174,  195,  218,   245,   260,   275,   291,   309,   346,  389,  436,
                                   489,  549,  581,  615,  652,  732,  691,  775,  821,  870,  921,  976,  1034, 1095, 1160,  1229,  1301,  1379,  1460,  1547,  1638, 1756, 2597,
                                   2751, 2913, 3086, 3463, 3841, 4115, 4559, 4918, 5681, 6523, 7318, 7752, 8403, 9213, 10338, 11599, 12428, 13785, 14062, 15467, 16384};
#endif
////////////////////////////////////////////////////////////////////////////////


static uint8_t sTlkUsbAudSpkEnable  = 0;
static uint8_t sTlkUsbAudSpk1Enable = 0;

static tlku2h_ppm_para_t tlkusb_uacspk_ppm_para = {.start_flag = false, .tick_last = 0, .tick_rem = 0, .sample_diff = 0};

/**
 * @brief     Initialize the USB audio speaker module
 * @param[in] none
 * @return    TLK_ENONE
 */
int tlkusb_uacspk_init(void)
{
    sTlkUsbAudSpkEnable           = false;
    g_tlk_usb_cfg.out_sample_rate = TLKUSB_AUD_SPK_SAMPLE_RATE_DEF;
    tlkusb_uacspk_setVolume(TLKUSB_AUDSPK_VOL_MAX);

    sTlkUsbAudSpk1Enable           = false;
    g_tlk_usb_cfg.out1_sample_rate = TLKUSB_AUD_SPK_SAMPLE_RATE_DEF;
    tlkusb_uacspk1_setVolume(TLKUSB_AUDSPK_VOL_MAX);
    return TLK_ENONE;
}

/**
 * @brief     Get the enable status of USB audio speaker
 * @param[in] none
 * @return    true if enabled, false otherwise
 */
bool tlkusb_uacspk_getEnable(void)
{
    return sTlkUsbAudSpkEnable;
}

bool tlkusb_uacspk1_getEnable(void)
{
    return sTlkUsbAudSpk1Enable;
}

/**
 * @brief     Enable or disable the USB audio speaker
 * @param[in] enable Enable(true) or disable(false) the speaker
 * @return    none
 */
void tlkusb_uacspk_setEnable(bool enable)
{
    sTlkUsbAudSpkEnable = enable;
    if (enable) {
#ifndef _B91_
        usbhw_reset_ep_ptr(TLKUSB_UAC_EDP_SPK);
#else
        reg_usb_ep_ptr(TLKUSB_UAC_EDP_SPK) = 0;
#endif
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK) = 0x01; // ACK first packet
    } else {
    }
}

void tlkusb_uacspk1_setEnable(bool enable)
{
    sTlkUsbAudSpk1Enable = enable;
    if (enable) {
#ifndef _B91_
        usbhw_reset_ep_ptr(TLKUSB_UAC_EDP_SPK1);
#else
        reg_usb_ep_ptr(TLKUSB_UAC_EDP_SPK1) = 0;
#endif
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK1) = 0x01; // ACK first packet
    } else {
    }
}

/**
 * @brief     Get the volume status of USB audio speaker
 * @param[in] none
 * @return    volume status
 */
uint tlkusb_uacspk_getVolumeStatus(void)
{
    return (g_tlk_usb_cfg.out_mute << 7) | (g_tlk_usb_cfg.out_vol_step & 0x7f);
}

uint tlkusb_uacspk1_getVolumeStatus(void)
{
    return (g_tlk_usb_cfg.out1_mute << 7) | (g_tlk_usb_cfg.out1_vol_step & 0x7f);
}

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)

/**
 * @brief     Convert volume from dB to linear scale
 * @param[in] db Volume in dB scale
 * @return    Volume in linear scale
 */
static int16_t tlkusb_uacspk_volume_db_to_linear(int16_t db)
{
    if (db >= TLKUSB_AUDSPK_VOL_MAX) {
        return APP_AUDIO_SPEAKER_VOL_LINEAR_MAX;
    } else if (db <= TLKUSB_AUDSPK_VOL_MIN) {
        return APP_AUDIO_SPEAKER_VOL_LINEAR_MIN;
    } else {
        uint32_t i;
        for (i = 0; i < APP_HALF(sizeof(g_volume_relative_value_table)); i++) {
            if (db <= g_volume_relative_value_table[i]) {
                break;
            }
        }
        if (APP_HALF(sizeof(g_volume_relative_value_table)) == i) {
            i--;
        }
        return (int16_t)g_cc_volume_scale_table[i];
    }
}

/**
 * @brief     Get the volume of USB audio speaker
 * @param[in] none
 * @return    volume value
 */
int16_t tlkusb_uacspk_getVolume(void)
{
    uint8_t        spk_status;
    int16_t        cur_vol_db;
    static uint8_t last_spk_step   = APP_AUDIO_DEFAULT_VOL_STEP;
    static int16_t last_spk_vol_db = APP_AUDIO_DEFAULT_VOL_DB;
    static int32_t linear_vol      = 0;

    spk_status = tlkusb_uacspk_getVolumeStatus();
    cur_vol_db = (int16_t)g_tlk_usb_cfg.out_volume;

    if ((last_spk_vol_db != cur_vol_db) || (last_spk_step != spk_status)) {
        if (spk_status & APP_AUDIO_SPK_MUTE_MASK) {
            linear_vol = 0;
        } else {
            linear_vol = tlkusb_uacspk_volume_db_to_linear(cur_vol_db);
        }
        last_spk_vol_db = cur_vol_db;
        last_spk_step   = spk_status;
    }

    return linear_vol;
}

int16_t tlkusb_uacspk1_getVolume(void)
{
    uint8_t        spk_status;
    int16_t        cur_vol_db;
    static uint8_t last_spk_step   = APP_AUDIO_DEFAULT_VOL_STEP;
    static int16_t last_spk_vol_db = APP_AUDIO_DEFAULT_VOL_DB;
    static int32_t linear_vol      = 0;

    spk_status = tlkusb_uacspk1_getVolumeStatus();
    cur_vol_db = (int16_t)g_tlk_usb_cfg.out1_volume;

    if ((last_spk_vol_db != cur_vol_db) || (last_spk_step != spk_status)) {
        if (spk_status & APP_AUDIO_SPK_MUTE_MASK) {
            linear_vol = 0;
        } else {
            linear_vol = tlkusb_uacspk_volume_db_to_linear(cur_vol_db);
        }
        last_spk_vol_db = cur_vol_db;
        last_spk_step   = spk_status;
    }

    return linear_vol;
}

#else
/**
 * @brief     Get iPhone volume scale index
 * @param[in] vol Volume value
 * @return    Volume scale index, 0xff if not found
 */
u8 blc_get_iphone_volume_scale(s16 vol)
{
    u8  temp = 0xff;
    int i;
    for (i = 0; i < 67; i++) {
        if (vol == volume_relative_table_iphone[i]) {
            temp = i;
            break;
        }
    }
    return temp;
}

/**
 * @brief     Get the volume of USB audio speaker
 * @param[in] none
 * @return    volume value
 */
uint tlkusb_uacspk_getVolume(void)
{
    u32 index;
    u8  i;

    if (gradual_vol != usb_vol_music) {
        if (gradual_vol > usb_vol_music) {
            usb_vol_music += 100;
            if (usb_vol_music > gradual_vol) {
                usb_vol_music = gradual_vol;
            }
        } else {
            usb_vol_music -= 100;
            if (usb_vol_music < gradual_vol) {
                usb_vol_music = gradual_vol;
            }
        }
    }

    s16 vol_temp  = g_tlk_usb_cfg.out_volume;
    u8  step_temp = (g_tlk_usb_cfg.out_mute << 7) | (g_tlk_usb_cfg.out_vol_step & 0x7f);

    if ((last_speaker_vol != vol_temp) || (last_speaker_step != step_temp)) {
        if (step_temp & 0x80) {
            usb_vol_music = 0;
            gradual_vol   = 0;
        } else {
            index = blc_get_iphone_volume_scale(vol_temp);
            if (index >= 67) {
                for (i = 0; i < sizeof(volume_relative_value_table) / 2; i++) {
                    if (vol_temp <= volume_relative_value_table[i]) {
                        break;
                    }
                }
                if (i > 100) {
                    i = 100;
                }
                gradual_vol = volume_scale_table[i];
            } else {
                gradual_vol = volume_scale_iphone_table[index];
            }
        }
        last_speaker_vol  = vol_temp;
        last_speaker_step = step_temp;
    }

    return usb_vol_music;
}

uint tlkusb_uacspk1_getVolume(void)
{
    u32 index;
    u8  i;

    if (gradual_vol1 != usb_vol1_music) {
        if (gradual_vol1 > usb_vol1_music) {
            usb_vol1_music += 100;
            if (usb_vol1_music > gradual_vol1) {
                usb_vol1_music = gradual_vol1;
            }
        } else {
            usb_vol1_music -= 100;
            if (usb_vol1_music < gradual_vol1) {
                usb_vol1_music = gradual_vol1;
            }
        }
    }

    s16 vol_temp  = g_tlk_usb_cfg.out1_volume;
    u8  step_temp = (g_tlk_usb_cfg.out1_mute << 7) | (g_tlk_usb_cfg.out1_vol_step & 0x7f);

    if ((last_speaker1_vol != vol_temp) || (last_speaker1_step != step_temp)) {
        if (step_temp & 0x80) {
            usb_vol1_music = 0;
            gradual_vol1   = 0;
        } else {
            index = blc_get_iphone_volume_scale(vol_temp);
            if (index >= 67) {
                for (i = 0; i < sizeof(volume_relative_value_table) / 2; i++) {
                    if (vol_temp <= volume_relative_value_table[i]) {
                        break;
                    }
                }
                if (i > 100) {
                    i = 100;
                }
                gradual_vol1 = volume_scale_table[i];
            } else {
                gradual_vol1 = volume_scale_iphone_table[index];
            }
        }
        last_speaker1_vol  = vol_temp;
        last_speaker1_step = step_temp;
    }

    return usb_vol1_music;
}
#endif

/**
 * @brief     Set the volume of USB audio speaker
 * @param[in] volume Volume to set
 * @return    none
 */
void tlkusb_uacspk_setVolume(int16_t volume)
{
    g_tlk_usb_cfg.out_volume = volume;
    if (volume < TLKUSB_AUDSPK_VOL_MIN) {
        g_tlk_usb_cfg.out_vol_step = 0;
    } else {
        g_tlk_usb_cfg.out_vol_step = (volume - TLKUSB_AUDSPK_VOL_MIN) / TLKUSB_AUDSPK_VOL_RES;
    }
}

void tlkusb_uacspk1_setVolume(int16_t volume)
{
    g_tlk_usb_cfg.out1_volume = volume;
    if (volume < TLKUSB_AUDSPK_VOL_MIN) {
        g_tlk_usb_cfg.out1_vol_step = 0;
    } else {
        g_tlk_usb_cfg.out1_vol_step = (volume - TLKUSB_AUDSPK_VOL_MIN) / TLKUSB_AUDSPK_VOL_RES;
    }
}

/**
 * @brief     Mute or unmute the USB audio speaker
 * @param[in] enable Mute(true) or unmute(false)
 * @return    none
 */
void tlkusb_uacspk_enterMute(bool enable)
{
    g_tlk_usb_cfg.out_mute = enable;
}

void tlkusb_uacspk1_enterMute(bool enable)
{
    g_tlk_usb_cfg.out1_mute = enable;
}

/**
 * @brief     Stop the PPM processing for USB audio speaker
 * @param[in] none
 * @return    none
 */
void tlkusb_uacspk_ppm_stop(void)
{
#if TLKLIB_UAC_PPM_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
    int                    ppm_value      = 0;
    p_audio_alg_if->audio_alg_param_set(0, &ppm_value);

    tlkalg_u2s_ppm_clear_status();

    if (tlkalg_u2s_ppm_get_init_flag_down()) {
        tlkalg_u2s_ppm_clear_init_flag_down();
    }
#endif
    tmemset(&tlkusb_uacspk_ppm_para, 0, sizeof(tlkusb_uacspk_ppm_para));
}

/**
 * @brief     Process PPM for USB audio speaker
 * @param[in] ticks Current system ticks
 * @return    none
 */
void tlkusb_ppm_process(uint32_t ticks)
{
    static uint32_t ticks_last = 0;
    uint32_t        ticks_diff = 0;

    ticks_diff = ticks - ticks_last; //first time input this function, ticks_diff must be more than 5ms

    if ((ticks_diff / (SYSTEM_TIMER_TICK_1US * 1000)) < 5) {
        return;
    }

    ticks_last = ticks;
    tlkusb_uac_copy_data(ticks);
}

/**
 * @brief     Copy USB audio data
 * @param[in] ticks Current system ticks
 * @return    none
 */
void tlkusb_uac_copy_data(uint32_t ticks)
{
    uint32_t tick_diff               = ticks - tlkusb_uacspk_ppm_para.tick_last + tlkusb_uacspk_ppm_para.tick_rem;
    tlkusb_uacspk_ppm_para.tick_last = ticks;

    if (!tlkusb_uacspk_ppm_para.start_flag) {
        if (tlkusb_uac_get_iso_out_used_samples() >= (TLKUSB_AUD_SPK_SAMPLES * 5)) {
            tlkusb_uacspk_ppm_para.start_flag = true;
            tlkusb_uac_sync_iso_out_samples(TLKUSB_AUD_SPK_SAMPLES * 5);
        }
        return;
    }


    uint16_t samples                = (tick_diff / (SYSTEM_TIMER_TICK_1US * 1000)) * TLKUSB_AUD_SPK_SAMPLES;
    tlkusb_uacspk_ppm_para.tick_rem = tick_diff % (SYSTEM_TIMER_TICK_1US * 1000);

    static uint32_t interval_ticks = 0;
    if (tlkusb_uac_get_iso_out_used_samples() <= (samples * 2)) {
        if (samples > (TLKUSB_AUD_SPK_SAMPLES * 8)) {
            samples = (TLKUSB_AUD_SPK_SAMPLES * 8);
        }
        /*8ms samples*/
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
        uint32_t usb_mute_samples[TLKUSB_AUD_SPK_SAMPLES * 2 * 8] = {0};
#else
        uint16_t usb_mute_samples[TLKUSB_AUD_SPK_SAMPLES * 2 * 8] = {0};
#endif
        tlkusb_uac_write_ppm_out_samples(usb_mute_samples, samples * 2);

        tlkalg_u2s_ppm_clear_status();
        interval_ticks += tick_diff;

        /*Close audio task if no data over 10s. TODO: For Yating:Place in uac loop later.*/
        if ((interval_ticks / SYSTEM_TIMER_TICK_1US / 1000) >= 10000) { //overtime 10s
            interval_ticks = 0;
            if (sTlkUsbReportUacStatusCB != NULL && (tlkusb_uac_get_state() != TLKUSB_UAC_IDLE)) {
                tlkusb_uac_reset_out_config();
                tlkusb_uacspk_ppm_para.start_flag = false;
                sTlkUsbReportUacStatusCB(false, false);
            }
        }
    } else {
        interval_ticks = 0;
#if TLKLIB_UAC_PPM_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        if (tlkalg_u2s_ppm_get_init_flag_down()) {
            tlkalg_u2s_ppm_clear_init_flag_down();
            int ppm_val = tlkalg_u2s_ppm_get_val();
            if (tlkusb_uac_get_state() == TLKUSB_UAC_VOICE) {
                if (16000 == tlkusb_uac_get_iso_in_SampleRate()) {
                    ppm_val = ppm_val * 4 / 5;
                }
            }
            p_audio_alg_if->audio_alg_param_set(0, &ppm_val);
        }

        if (tlkusb_uacspk_ppm_para.sample_diff != 0) {
            samples -= tlkusb_uacspk_ppm_para.sample_diff;
            tlkusb_uacspk_ppm_para.sample_diff = 0;
        }
#endif
        uint16_t samples_num = TLKUSB_AUD_SPK_SAMPLES * 12;

        if (samples > samples_num) {
            samples = samples_num;
        }
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
        int32_t usb_samples[samples_num * 2];
#else
        int16_t usb_samples[samples_num * 2];
#endif

        if (tlkusb_uac_get_iso_out_used_samples() < (samples * 2)) {
            return;
        }

        tlkusb_uac_read_iso_out_samples(usb_samples, samples * 2);

#if TLKLIB_UAC_PPM_ENABLE
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
        uint32_t ppm_pro_samples[samples_num * 2];
#else
        uint16_t ppm_pro_samples[samples_num * 2];
#endif

        int num_out = p_audio_alg_if->audio_alg_process((uint8_t *)usb_samples, (uint8_t *)ppm_pro_samples, samples, 0, 0);

        if (-1 == num_out) {
            tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm len oversize %d", samples);
        }

        tlkusb_uacspk_ppm_para.sample_diff = num_out - (samples);
        if (tlkusb_uac_get_state() == TLKUSB_UAC_MUSIC) {
            g_usb_resample_diff += tlkusb_uacspk_ppm_para.sample_diff;
        }

        // if (samples != (uint16_t)num_out) { //this if can disable, but don't delete
        //     tlkapi_trace(0xFFFFFFFF, "[TEST]", "usb_l u2s_l %d %d %d", tlkusb_uac_get_iso_out_used_samples(), tlkusb_uac_get_ppm_out_used_samples(), tlkusb_uacspk_ppm_para.sample_diff);
        // }
        tlkusb_uac_write_ppm_out_samples(ppm_pro_samples, num_out * 2);
#else
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
        tlkusb_uac_write_ppm_out_samples((uint32_t *)usb_samples, samples * 2);
#else
        tlkusb_uac_write_ppm_out_samples((uint16_t *)usb_samples, samples * 2);
#endif

#endif
    }
}

/**
 * @brief     Handle set interface command for USB audio speaker
 * @param[in] type Type of the feature to set
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacspk_setInfCmdDeal(int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        g_tlk_usb_cfg.out_mute = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        int16_t val = (int16_t)tlkusb_hal_read_ctrl_ep_u16(TLK_CFG_USB_UAC_INDEX);
        tlkusb_uacspk_setVolume(val);
    } else {
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

int tlkusb_uacspk1_setInfCmdDeal(int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        g_tlk_usb_cfg.out1_mute = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        int16_t val = (int16_t)tlkusb_hal_read_ctrl_ep_u16(TLK_CFG_USB_UAC_INDEX);
        tlkusb_uacspk1_setVolume(val);
    } else {
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

/**
 * @brief     Handle get interface command for USB audio speaker
 * @param[in] req  Request type
 * @param[in] type Type of the feature to get
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacspk_getInfCmdDeal(int req, int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, g_tlk_usb_cfg.out_mute);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        switch (req) {
        case AUDIO_REQ_GetCurrent:
            usbhw_write_ctrl_ep_u16(g_tlk_usb_cfg.out_volume);
            break;
        case AUDIO_REQ_GetMinimum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_MIN);
            break;
        case AUDIO_REQ_GetMaximum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_MAX);
            break;
        case AUDIO_REQ_GetResolution:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_RES);
            break;
        default:
            return -TLK_ENOSUPPORT;
        }
    } else {
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

int tlkusb_uacspk1_getInfCmdDeal(int req, int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, g_tlk_usb_cfg.out1_mute);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        switch (req) {
        case AUDIO_REQ_GetCurrent:
            usbhw_write_ctrl_ep_u16(g_tlk_usb_cfg.out1_volume);
            break;
        case AUDIO_REQ_GetMinimum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_MIN);
            break;
        case AUDIO_REQ_GetMaximum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_MAX);
            break;
        case AUDIO_REQ_GetResolution:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDSPK_VOL_RES);
            break;
        default:
            return -TLK_ENOSUPPORT;
        }
    } else {
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

/**
 * @brief     Handle set endpoint command for USB audio speaker
 * @param[in] type Type of the endpoint control
 * @return    TLK_ENONE
 */
int tlkusb_uacspk_setEdpCmdDeal(int type)
{
    if (type == AUDIO_EPCONTROL_SamplingFreq) {
        uint32_t value                = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out_sample_rate = value;
        value                         = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out_sample_rate |= value << 8;
        value = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out_sample_rate |= value << 16;
        // TODO: Sample Rate Changed
    }
    return TLK_ENONE;
}

int tlkusb_uacspk1_setEdpCmdDeal(int type)
{
    if (type == AUDIO_EPCONTROL_SamplingFreq) {
        uint32_t value                 = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out1_sample_rate = value;
        value                          = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out1_sample_rate |= value << 8;
        value = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.out1_sample_rate |= value << 16;
        // TODO: Sample Rate Changed
    }
    return TLK_ENONE;
}

//Fill cache data when usb irq is delayed;
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
static int32_t g_tlk_iso_out_cache_samples[TLKUSB_AUD_SPK_SAMPLES * 2] = {0};

/**
 * @brief     Read one channel sample data from the USB endpoint
 * @param[in] none
 * @return    sample data
 */
__INLINE uint32_t tlkusb_iso_out_read_ep_one_chn(uint8_t endpoint)
{
    uint32_t samples_data   = 0;
    uint8_t  chn_pos        = TLKUSB_AUD_SPK_RESOLUTION_BIT / 8;
    uint8_t *p_samples_data = (uint8_t *)&samples_data;

    for (int i = 0; i < chn_pos; i++) {
        p_samples_data[i] = reg_usb_ep_dat(endpoint);
    }

    if (p_samples_data[chn_pos - 1] & BIT(7)) {
        p_samples_data[chn_pos] = 0xff;
    }

    return samples_data;
}
#else
static int16_t g_tlk_iso_out_cache_samples[TLKUSB_AUD_SPK_SAMPLES * 2] = {0};

/**
 * @brief     Read one channel sample data from the USB endpoint
 * @param[in] none
 * @return    sample data
 */
__INLINE uint16_t tlkusb_iso_out_read_ep_one_chn(uint8_t endpoint)
{
    uint16_t data = 0;
    data          = reg_usb_ep_dat(endpoint);
    data |= (reg_usb_ep_dat(endpoint) << 8);

    return data;
}
#endif

/**
 * @brief     Receive data for USB audio speaker
 * @param[in] tick Current system tick
 * @return    none
 */
_attribute_ram_code_sec_ void tlkusb_uacspk_recvData(uint32_t tick)
{
    uint16_t index;
    uint16_t length        = TLKUSB_AUD_SPK_CHANNEL_LENGTH;
    uint32_t intvTick      = tick - g_tlk_usb_cfg.tick_out;
    g_tlk_usb_cfg.tick_out = tick;

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
    int32_t        data      = 0;
    static int32_t data_last = 0;
#else
    int16_t        data      = 0;
    static int16_t data_last = 0;
#endif
    usbhw_clr_eps_irq_inline(BIT(TLKUSB_UAC_EDP_SPK));
#if TLKLIB_UAC_PPM_ENABLE
    uint32_t ticks = clock_time() | 1;
    if (tlkusb_uac_get_state() == TLKUSB_UAC_MUSIC) {
        g_usb_ppm.t_m_samples_cur += TLKALG_USB_OUT_PPM_SAMPLES;
        tlkalg_u2s_ppm_calculation(ticks);
    }
#endif
    /*usb irq is delayed, fill cache data*/
    if (intvTick > (SYSTEM_TIMER_TICK_1US * 1100) && (intvTick / SYSTEM_TIMER_TICK_1US < 10000)) {
        tlkusb_uac_write_iso_out_samples(g_tlk_iso_out_cache_samples, TLKUSB_AUD_SPK_SAMPLES * 2);

        return;
    }

    /*dobule check data length*/
    length = usbhw_get_ep_ptr_inline(TLKUSB_UAC_EDP_SPK);
    if ((length != TLKUSB_AUD_SPK_CHANNEL_LENGTH) && (length != 0)) {
        return;
    }
    reg_usb_ep_ptr(TLKUSB_UAC_EDP_SPK) = 0;
#ifndef _B91_
    reg_usb_ep_ptrh(TLKUSB_UAC_EDP_SPK) = 0;
#endif
    if (tlkusb_uac_get_iso_out_idle_samples() < (TLKUSB_AUD_SPK_SAMPLES * 2)) {
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK) = FLD_EP_DAT_ACK;
        return;
    }

    for (index = 0; index < TLKUSB_AUD_SPK_SAMPLES * 2; index++) {
        data = tlkusb_iso_out_read_ep_one_chn(TLKUSB_UAC_EDP_SPK);

        g_tlk_iso_out_cache_samples[index]                                         = data;
        g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_w & APP_USB_ISO_OUT_BUFF_IDX_MASK] = data;
        g_tlk_usb_cfg.out_w++;
        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;

        if (gLeaPpmStatus) {
            if ((ppm_sample_counter != 0) && (index % 2 != 0)) {                                                                          //add sample
                if ((data > 0 && data_last < 0) || (data < 0 && data_last > 0) || clock_time_exceed(ppm_tick, (over_time_us * 8 / 10))) { //80% overtime
                    if (ppm_sample_counter < 0) {                                                                                         //lose sample
                        ppm_sample_counter++;
                        g_tlk_usb_cfg.out_w--;
                        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
                        g_tlk_usb_cfg.iso_out[(g_tlk_usb_cfg.out_w - 2) & APP_USB_ISO_OUT_BUFF_IDX_MASK] = (abs_ram(data) < abs_ram(data_last) ? data_last : data);

                        g_tlk_usb_cfg.out_w--;
                        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
                        g_tlk_usb_cfg.iso_out[(g_tlk_usb_cfg.out_w - 2) & APP_USB_ISO_OUT_BUFF_IDX_MASK] = (abs_ram(data) < abs_ram(data_last) ? data_last : data);

                    } else if (ppm_sample_counter > 0) { //insert sample
                        ppm_sample_counter--;
                        g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_w] = (abs_ram(data) > abs_ram(data_last) ? data_last : data);
                        g_tlk_usb_cfg.out_w++;
                        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;

                        g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_w] = (abs_ram(data) > abs_ram(data_last) ? data_last : data);
                        g_tlk_usb_cfg.out_w++;
                        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
                    }
                }
                data_last = data;
            }
        }
    }
#if (TLKLIB_UAC_PPM_SPK_ENABLE)
    if (gLeaPpmStatus) {
        if (ppm_tick_last) {
            tick_diff_val += tick - ppm_tick_last - TICK_PER_MS;

            if (abs_ram(tick_diff_val) >= (TICK_PER_SAMPLE / 2) * 4) {
                if (tick_diff_val < 0) { //lose sample
                    tick_diff_val += TICK_PER_SAMPLE;
                    ppm_sample_counter--;
                } else if (tick_diff_val > 0) { //insert sample
                    tick_diff_val -= TICK_PER_SAMPLE;
                    ppm_sample_counter++;
                }
                if (ppm_tick > 0) {
                    over_tick[g_ppm_flag % 5] = tick - ppm_tick; //SYSTEM_TIMER_TICK_1US
                    if (over_tick[4] == 0) {
                        over_time_us = (over_tick[0] + over_tick[1] + over_tick[2] + over_tick[3] + over_tick[4]) / (g_ppm_flag + 1) / SYSTEM_TIMER_TICK_1US;
                    } else {
                        over_time_us = (over_tick[0] + over_tick[1] + over_tick[2] + over_tick[3] + over_tick[4]) / 5 / SYSTEM_TIMER_TICK_1US;
                    }
                    g_ppm_flag++;
                    g_ppm_flag %= 5;
                }
                ppm_tick = tick;
            }
        }

        if (ppm_tick_last == 0) {
            tick_diff_val      = 0;
            data_last          = 0;
            ppm_sample_counter = 0;
            g_ppm_flag         = 0;
            memcpy(over_tick, 0, sizeof(over_tick));
            ppm_tick     = 0;
            over_time_us = 0;
        }
        ppm_tick_last = tick;
    }
#endif

    reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK) = FLD_EP_DAT_ACK;
#if TLKALG_PPM_CALC_BY_SAMPLE
    if (tlkalg_sample_ppm_mode_idle(&g_uac_ppm_ctrl)) {
        tlkalg_set_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_MUSIC_MODE);
        tlkalg_reset_sample_ppm_calc(&g_uac_ppm_ctrl);
        tlkalg_set_sample_ppm_mask(&g_uac_ppm_ctrl, APP_USB_ISO_OUT_BUFF_IDX_MASK);
        tlkalg_set_sample_ppm_channel(&g_uac_ppm_ctrl, tlkusb_uac_get_iso_out_Channels());
    }
#endif
}

#if (TLK_USB_UAC_DUAL_SOUNDCARD_MODE)
_attribute_ram_code_sec_ void tlkusb_uacspk1_recvData(uint32_t tick)
{
    g_tlk_usb_cfg.tick_out1 = tick;
    usbhw_clr_eps_irq_inline(BIT(TLKUSB_UAC_EDP_SPK1));
    unsigned int length = usbhw_get_ep_ptr_inline(TLKUSB_UAC_EDP_SPK1);

#ifndef _B91_
    usbhw_reset_ep_ptr(TLKUSB_UAC_EDP_SPK1);
#else
    reg_usb_ep_ptr(TLKUSB_UAC_EDP_SPK1) = 0;
#endif
    if ((length != TLKUSB_AUD_SPK_CHANNEL_LENGTH) && (length != 0)) {
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK1) = FLD_EP_DAT_ACK;
        return;
    }

    for (unsigned int index = 0; index < TLKUSB_AUD_SPK_SAMPLES * 2; index++) {
        unsigned int data                                                            = tlkusb_iso_out_read_ep_one_chn(TLKUSB_UAC_EDP_SPK1);
        g_tlk_usb_cfg.iso_out1[g_tlk_usb_cfg.out1_w & APP_USB_ISO_OUT_BUFF_IDX_MASK] = data;
        g_tlk_usb_cfg.out1_w++;
        g_tlk_usb_cfg.out1_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    }
    reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK1) = FLD_EP_DAT_ACK;
#if TLKALG_PPM_CALC_BY_SAMPLE
    if (tlkalg_sample_ppm_mode_idle(&g_uac_ppm_ctrl)) {
        tlkalg_set_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_MUSIC1_MODE);
        tlkalg_reset_sample_ppm_calc(&g_uac_ppm_ctrl);
        tlkalg_set_sample_ppm_mask(&g_uac_ppm_ctrl, APP_USB_ISO_OUT_BUFF_IDX_MASK);
        tlkalg_set_sample_ppm_channel(&g_uac_ppm_ctrl, tlkusb_uac_get_iso_out_Channels());
    }
#endif
}
#endif
/**
 * @brief     Close the USB audio speaker
 * @param[in] none
 * @return    none
 */
void tlkusb_uacspk_close(void)
{
    g_tlk_usb_cfg.tick_out   = 0;
    g_tlk_usb_cfg.iso_out_en = false;
    if (sTlkUsbReportUacStatusCB != NULL && (tlkusb_uac_get_state() != TLKUSB_UAC_IDLE)) {
        sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en || g_tlk_usb_cfg.iso_out1_en);
        tlkusb_uac_reset_out_config();
    }
}
#else
void tlkusb_uacspk_close(void) {}
#endif // #if (TLKUSB_UAC_SPK_ENABLE)
