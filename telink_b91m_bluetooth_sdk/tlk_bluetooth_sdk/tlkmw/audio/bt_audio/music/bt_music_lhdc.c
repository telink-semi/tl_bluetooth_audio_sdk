/********************************************************************************************************
 * @file    bt_music_lhdc.c
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
#include "vendor/common/user_config.h"
#if LHDC_CODEC_ENABLE
#include "tlkmw/aud/cc_headset/audio_common.h"
#include "tlkmw/aud/cc_headset/audio_mw_manager.h"
#include "tlkmw/aud/cc_headset/bt_audio/bt_audio_main.h"
#include "tlkmw/aud/cc_headset/bt_audio/bt_audio_sync.h"
#include "tlkmw/aud/cc_headset/bt_audio/music/bt_music.h"

#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "drivers.h"

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
#include "tlkmw/aud/cc_headset/bt_audio/music/bt_music_lhdc.h"
//#include "tlkalg/audio_alg/lhdc/tlk_lhdc_v5_dec_interface.h"

#define FLASH_SV_KEY_BASE   (1024 * 1024 - 4 * 1024)
#define LHDC_READY_TIME_5MS (100)

_attribute_iram_data_ lhdc_config_t lhdc_config = {
    .dec_param.bits          = BITS_16,
    .dec_param.srate         = SRATE_48K,
    .dec_param.samples_total = 240,
    .channel                 = LHDC_OUTPUT_LEFT_CHANNEL,
    .license.flash_addr      = FLASH_SV_KEY_BASE,
    .license.len             = LHDC_DECODE_LICENSE_SIZE,
};

static bool low_latency_enable = 0;

/**
 * @brief       Slave will receive this information during TWS master build A2DP protocol
 * @param[in]   src - Source LHDC sync information
 * @return      None
 */
void lhdc_slave_param_set(const lhdc_sync_info_t *src)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "lhdc_slave_param_set");

    if (!IS_LHDC_BITS(src->bits) || !IS_LHDC_SRATE(src->srate)) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "rx lhdcv5_util_dec param error!");
        return;
    }

    low_latency_enable           = src->low_latency;
    lhdc_config.lhdc_player_init = 0;
    lhdc_config.eq_init          = 0;
    lhdc_config.dec_param.bits   = src->bits;
    lhdc_config.dec_param.srate  = src->srate;
}

/**
 * @brief       Check if LHDC player is initialized
 * @param[in]   None
 * @return      true if LHDC player is initialized, false otherwise
 */
bool is_lhdc_player_init(void)
{
    return lhdc_config.lhdc_player_init;
}

/**
 * @brief       Set LHDC player initialization flag
 * @param[in]   flag - Initialization flag value
 * @return      None
 */
void lhdc_player_init_set(uint8_t flag)
{
    lhdc_config.lhdc_player_init = flag;
}

/**
 * @brief       Set LHDC EQ initialization flag
 * @param[in]   flag - Initialization flag value
 * @return      None
 */
void lhdc_eq_init_set(uint8_t flag)
{
    lhdc_config.eq_init = flag;
}

/**
 * @brief       Check if LHDC EQ is initialized
 * @param[in]   None
 * @return      true if LHDC EQ is initialized, false otherwise
 */
bool is_lhdc_eq_init(void)
{
    return lhdc_config.eq_init;
}

/**
 * @brief       Open LHDC audio
 * @param[in]   None
 * @return      None
 */
void lhdc_audio_open(void)
{
    lhdc_config.sys_mute_dly = 2;

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "Master lhdc_audio_ready");
}

/**
 * @brief       Ready LHDC audio
 * @param[in]   None
 * @return      Operation result
 */
_attribute_ram_code_ int lhdc_audio_ready(void)
{
    uint8_t param[1];
    int     ret;

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "Slave lhdc_audio_ready");
    //param[0] = LE_VENDOR_CMD_AUDIO_OPEN_SYNC;
    lhdc_config.sys_mute_dly = 2;
    //int ret = tws_sync_push_cmd (TWS_CMD_VENDOR, param, ARRAY_LEN(param), NULL);
    return ret;
}

/**
 * @brief       Update LHDC asynchronously
 * @param[in]   None
 * @return      None
 */
_attribute_ram_code_ void async_lhdc_update(void)
{
    if (lhdc_config.ready_time == 1) {
        if (tlkmdi_bt_tpt_isSlave()) {
            lhdc_audio_ready();
        }

        lhdc_config.ready_time--;
    }
}

/**
 * @brief       Select LHDC channel
 * @param[in]   lhdc - Pointer to LHDC configuration
 * @return      0
 */
static uint8_t lhdc_channel_sel(lhdc_config_t *lhdc)
{
    lhdc->channel = tph_headset_is_left() ? LHDC_OUTPUT_LEFT_CHANNEL : LHDC_OUTPUT_RIGHT_CHANNEL;
    return 0;
}

/**
 * @brief       Set LHDC encoder parameters
 * @param[in]   frame_len - Frame length
 * @param[in]   max_num - Maximum number
 * @param[in]   samples_per_frame - Samples per frame
 * @return      None
 */
static void lhdc_enc_param_set(uint16_t frame_len, uint16_t max_num, uint16_t samples_per_frame)
{
    bt_music_cfg.enc_buf_num       = max_num;
    bt_music_cfg.enc_buf_size      = frame_len;
    bt_music_cfg.samples_per_frame = samples_per_frame;
}

/**
 * @brief       Delay LHDC mute for N milliseconds
 * @param[in]   n - Number of milliseconds to delay
 * @return      None
 */
void lhdc_mute_delay_5ms(uint16_t n)
{
    tlkdrv_codec_muteSpkBuff();
    lhdc_config.sys_mute_dly = n;
}

/**
 * @brief       Reset LHDC ready time
 * @param[in]   None
 * @return      Reset value
 */
uint16_t lhdc_ready_time_reset(void)
{
    return lhdc_config.ready_time = LHDC_READY_TIME_5MS;
}

/**
 * @brief       Adjust LHDC music frame
 * @param[in]   src - Source data pointer
 * @param[out]  dst - Destination data pointer
 * @param[in]   frame_len - Frame length
 * @return      None
 */
void lhdc_mus_frame_adjust(const uint8_t *src, uint8_t *dst, uint16_t frame_len)
{
    if (!tph_headset_is_left()) {
        tmemcpy(&dst[2], &src[frame_len], frame_len - 2);
    }
}

/**
 * @brief       Initialize LHDC decoder player
 * @param[in]   None
 * @return      LHDC operation result
 */
lhdc_result_e lhdc_dec_player_init(void)
{
    lhdc_result_e ret      = LHDC_SUCCESS;
    uint32_t      ticks_ms = clock_time();
    uint32_t      req_mem_size;
    int32_t       result;

    if (lhdc_config.dec_mem == NULL || !IS_LHDC_BITS(lhdc_config.dec_param.bits) || !IS_LHDC_SRATE(lhdc_config.dec_param.srate)) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdcv5_util_dec param error: %d", lhdc_config.dec_param.bits);

        return LHDC_FAIL;
    }

    lhdc_channel_sel(&lhdc_config);

    result = lhdcv5_util_init_decoder((uint32_t *)lhdc_config.dec_mem,       // Buffer
                                      (uint32_t)lhdc_config.dec_param.bits,  // Bits per sample
                                      (uint32_t)lhdc_config.dec_param.srate, // sample rate
                                      LHDCV5_AUDIO_FMT_INOUT_ONE_CHANNEL,    // Format
                                      50,                                    // 5mS
                                      0,                                     // RSV
                                      VERSION_5                              // VERSION
    );
    if (result) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdcv5_util_init_decoder error, bits: %d, srate: %d", lhdc_config.dec_param.bits, lhdc_config.dec_param.srate);

        return ret = LHDC_FAIL;
    }

    result = lhdcv5_util_dec_channel_selsect((uint32_t *)lhdc_config.dec_mem, lhdc_config.channel);
    if (result) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdcv5_util_dec_channel_selsect error!");
        return ret = LHDC_FAIL;
    }

    lhdc_config.dec_param.samples_total = lhdcv5_util_dec_get_sample_size((uint32_t *)lhdc_config.dec_mem);
    lhdc_enc_param_set(LHDC_FRAME_MAX_SIZE, TMUSIC_LHDC_BUFF_NUM_MAX, lhdc_config.dec_param.samples_total);
    //lhdcv5_license_verify(&lhdc_config);

    bt_music_open_codec(lhdc_config.dec_param.srate);
    lhdc_config.lhdc_player_init = 1;
    ticks_ms                     = (clock_time() - ticks_ms) / 16 / 1000;

    if (tph_headset_is_master() || tlkmdi_bt_tpt_isSlave()) {
        lhdc_mute_delay_5ms(1000);
        lhdc_ready_time_reset();
    }

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[LHDC] lhdc_dec_player_init result %d %d %d %d", lhdc_config.dec_param.bits, lhdc_config.dec_param.srate,
                 lhdc_config.dec_param.samples_total, ticks_ms);

    return ret;
}

/**
 * @brief       Get LHDC configuration
 * @param[in]   None
 * @return      Pointer to LHDC configuration
 */
_attribute_ram_code_ lhdc_config_t *lhdc_get_config(void)
{
    return &lhdc_config;
}

/*
 *   slave will receive this information
 *   during tws master build a2dp protocol
 */
uint8_t lhdc_config_parse(const lhdc_codec_cap_t *lhdc_cap)
{
    LHDC_ERROR_CHECK(lhdc_cap);

    if (lhdc_cap->version_num != LHDC_V50) {
        LHDC_ERROR_CHECK(0);
    }

    lhdc_config.dec_param.bits = lhdc_cap->bits16_en ? BITS_16 : lhdc_cap->bits24_en ? BITS_24 : BITS_32;

    lhdc_config.dec_param.srate  = lhdc_cap->srate44100_en ? SRATE_441K : lhdc_cap->srate48000_en ? SRATE_48K : lhdc_cap->srate96000_en ? SRATE_96K : SRATE_192K;
    lhdc_config.lhdc_player_init = 0;

    tlkapi_info(0XFFFFFFFF, NULL, "[LHDC] lhdc_config_parse bits: %d, srate: %d", lhdc_config.dec_param.bits, lhdc_config.dec_param.srate);

    return 0;
}

/**
 * @brief       Request memory initialization for LHDC decoder
 * @param[in]   lhdc - Pointer to LHDC configuration
 * @return      Required memory size
 */
static uint32_t lhdc_dec_mem_request_init(const lhdc_config_t *lhdc)
{
    uint32_t req_mem_size;
    LHDC_ERROR_CHECK(!lhdcv5_util_dec_get_mem_req(VERSION_5, lhdc->dec_param.srate, LHDCV5_AUDIO_FMT_INOUT_ONE_CHANNEL, 0, &req_mem_size));

    return req_mem_size;
}

/**
 * @brief       Verify LHDC v5 license
 * @param[in]   lhdc - Pointer to LHDC configuration
 * @return      LHDC operation result
 */
lhdc_result_e lhdcv5_license_verify(const lhdc_config_t *lhdc)
{
    if (lhdc->dec_mem == NULL || !IS_LHDC_BITS(lhdc->dec_param.bits) || !IS_LHDC_SRATE(lhdc->dec_param.srate) || lhdc->license.key == NULL || !lhdc->license.len ||
        !lhdc->license.flash_addr) {
        tlkapi_info(0XFFFFFFFF, NULL, "lhdcv5_license_verify error, %d %d %d 0x%x", lhdc->dec_param.bits, lhdc->dec_param.srate, lhdc->license.len, lhdc->license.flash_addr);

        return LHDC_FAIL;
    }

    api_nvds_flash_protect(0);
    AUDIO_GLOBAL_INT_DISABLE();
    flash_read_page(lhdc->license.flash_addr, lhdc->license.len, lhdc->license.key);
    AUDIO_GLOBAL_INT_RESTORE();
    api_nvds_flash_protect(1024 * 1024);

    lhdc_dump_data(LHDC_DEBUG_EN, "lhdc license verify start...", &lhdc->license.key[0], 16);

    if (lhdcv5_util_set_license(lhdc->dec_mem, lhdc->license.key)) {
        tlkapi_info(0XFFFFFFFF, NULL, "lhdc license verify success");
        return LHDC_SUCCESS;
    } else {
        tlkapi_info(0XFFFFFFFF, NULL, "lhdc license verify fail");
        lhdcv5_util_set_license_check_period((uint32_t *)lhdc->dec_mem, 10);
        return LHDC_FAIL;
    }
}

/**
 * @brief       Get license data
 * @param[in]   offset - Offset in data
 * @param[in]   data - Pointer to data
 * @param[in]   len - Length of data
 * @return      License verification result
 */
license_ret_e license_data_get(uint8_t offset, uint8_t *data, uint8_t len)
{
    license_ret_e ret = CHECK_DATA;

    if (offset + len > 256) {
        len = 256 - offset;
    }

    tmemcpy(offset + lhdc_config.license.key, data, len);

    lhdc_dump_data(LHDC_DEBUG_EN, "license data", data, len);

    if (offset + len >= 256) {
        api_nvds_flash_protect(0);
        AUDIO_GLOBAL_INT_DISABLE();
        flash_write_page(lhdc_config.license.flash_addr, lhdc_config.license.len, lhdc_config.license.key);
        AUDIO_GLOBAL_INT_RESTORE();
        api_nvds_flash_protect(1024 * 1024);

        if (lhdc_license_verify() == LHDC_SUCCESS) {
            ret = CHECK_SUCCESS;
        } else {
            ret = CHECK_FAIL;
        }
    }

    return ret;
}

/**
 * @brief       Register LHDC license
 * @param[in]   lhdc - Pointer to LHDC configuration
 * @return      LHDC operation result
 */
static lhdc_result_e lhdc_license_register(const lhdc_config_t *lhdc)
{
    if (lhdc->dec_mem == NULL || !IS_LHDC_BITS(lhdc->dec_param.bits) || !IS_LHDC_SRATE(lhdc->dec_param.srate)) {
        tlkapi_info(0XFFFFFFFF, NULL, "lhdc reg parameter error, %d %d", lhdc->dec_param.bits, lhdc->dec_param.srate);

        return LHDC_FAIL;
    }

    int32_t ret = lhdcv5_util_init_decoder((uint32_t *)lhdc->dec_mem,          // Buffer
                                           (uint32_t)lhdc->dec_param.bits,     // Bits per sample
                                           (uint32_t)lhdc->dec_param.srate,    // sample rate
                                           LHDCV5_AUDIO_FMT_INOUT_ONE_CHANNEL, // Format
                                           50,                                 // 5mS
                                           0,                                  // RSV
                                           VERSION_5                           // VERSION
    );
    if (ret) {
        return (LHDC_FAIL);
    }

    return lhdcv5_license_verify(lhdc);
}

/**
 * @brief       Verify LHDC license
 * @param[in]   None
 * @return      LHDC operation result
 */
lhdc_result_e lhdc_license_verify(void)
{
    return lhdc_license_register(&lhdc_config);
}

/*
 *  \brief    lhdcv5_get_frame_info
 *  \note     Get the frame information from A Media packet
 *  \retval
 *
 */
int32_t lhdcv5_get_frame_info(uint8_t *s, uint16_t len)
{
    return lhdcv5_util_dec_fetch_frame_info((uint32_t *)lhdc_config.dec_mem, s, len, &lhdc_config.frame_info);
}

/*
 *   Get the memory of lhdc decode
 */
uint8_t lhdc_get_enc_mem(uint8_t **addr)
{
    LHDC_ERROR_CHECK(lhdc_config.dec_mem);
    *addr = lhdc_config.dec_mem;
    return 0;
}
#endif
#endif
