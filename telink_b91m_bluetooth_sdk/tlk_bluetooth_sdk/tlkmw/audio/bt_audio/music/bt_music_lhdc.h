/********************************************************************************************************
 * @file    bt_music_lhdc.h
 *
 * @brief   This is the header file for TLSR/TL
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
#include "tlkmw/audio/audio_mw_manager.h"
#if LHDC_CODEC_ENABLE
#ifndef __BT_MUSIC_LHDC_H__
#define __BT_MUSIC_LHDC_H__
#include "tl_common.h"
#include "drivers.h"

#include <stdbool.h>
#include <stdint.h>

#include "tlkalg/audio_alg/lhdc/tlk_lhdc_v5_dec_interface.h"

typedef struct
{
    uint8_t  bits;
    uint32_t srate;
    uint16_t samples_total;
    bool     low_latency;
} __attribute__((packed)) lhdc_sync_info_t;

#ifndef LHDC_MIN(a, b)
#define LHDC_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef LHDC_MAX(a, b)
#define LHDC_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define lhdc_dump_data           tlkapi_sendData
#define lhdc_dump_s32            tlkapi_sendU32s

#define TMUSIC_LHDC_BUFF_NUM_MAX (BT_MUSIC_ENC_FIFO_SIZE / LHDC_FRAME_MAX_SIZE)

#define LHDC_DEBUG_EN            1
#define LHDC_ERROR_CHECK(err)                                    \
    do {                                                         \
        if (!(err)) {                                            \
            lhdc_dump_data(LHDC_DEBUG_EN, "lhdcv5 error", 0, 0); \
            lhdc_dump_data(LHDC_DEBUG_EN, __func__, 0, 0);       \
        }                                                        \
    } while (0)

#define LHDC_VENDOR_ID1     0x3a
#define LHDC_VENDOR_ID2     0x05
#define LHDC_VENDOR_ID3     0x00
#define LHDC_VENDOR_ID4     0x00
#define LHDC_VENDRO_CODECID 0x4c35

///brief music fsm state
typedef enum
{
    FSM_NONE,
    FSM_START,
    FSM_SILENCE,
    FSM_FADE_IN_PLAY_TONE,
} music_fsm_e;

typedef enum
{
    LHDC_V50 = BIT(0),
    LHDC_V51 = BIT(1),
    LHDC_V52 = BIT(2),
    LHDC_V53 = BIT(3),
} lhdc_version_e;

typedef enum
{
    LHDC_INIT_NONE,
    LHDC_PARAM_INIT,
} lhdc_init_state_e;

typedef enum
{
    CHECK_DATA    = 0x00,
    CHECK_SUCCESS = 0x55,
    CHECK_FAIL    = 0xaa,
} license_ret_e;

typedef enum
{
    LHDC_FAIL    = -1,
    LHDC_SUCCESS = 0,
} lhdc_result_e;

typedef struct
{
    uint16_t length;
    uint16_t dcid;
} __attribute__((__packed__)) l2cap_frame_t;

typedef struct
{
    uint8_t version    : 2;
    uint8_t padding    : 1;
    uint8_t extension  : 1;
    uint8_t csrc_count : 4;

    uint8_t marker       : 1;
    uint8_t payload_type : 7;

    uint16_t sequence_num;
    uint32_t timestamp;
    uint32_t ssrc;
} __attribute__((__packed__)) media_pkt_header_t;

typedef struct
{
    uint8_t lantency    : 2;
    uint8_t frame_total : 4;
    uint8_t reserve     : 2;
    uint8_t sequence_num;
} __attribute__((__packed__)) lhdc_header_t;

typedef struct
{
    media_pkt_header_t media_header;
    lhdc_header_t      lhdc_header;
} __attribute__((__packed__)) lhdc_avdtp_pkt_t;

typedef struct
{
    lhdc_bits_e       bits;
    lhdc_samplerate_e srate;
    uint16_t          samples_total;
} __attribute__((packed)) lhdc_info_t;

typedef struct
{
    uint8_t *key;
    uint32_t flash_addr;
    uint16_t len;
} lhdc_license_t;

typedef struct
{
    lhdc_info_t       dec_param;
    lhdc_channel_t    channel;
    lhdc_frame_Info_t frame_info;
    lhdc_license_t    license;
    bool              lhdc_player_init;
    bool              eq_init;
    uint8_t          *dec_mem;
    uint8_t          *key;
    uint8_t          *asrc;
    uint16_t          sys_mute_dly;
    uint16_t          ready_time;
} lhdc_config_t;

typedef struct lhdc_codec_cap
{
    uint8_t  rfa0       : 4;
    uint8_t  media_type : 4;
    uint8_t  media_codec_type; // 0xff
    uint8_t  vendor_id1;       //0x3a
    uint8_t  vendor_id2;       // 0x05
    uint8_t  vendor_id3;       // 0x0000
    uint8_t  vendor_id4;       // 0x0000
    uint16_t vendor_codec_id;  //ID: 0x4c35

    uint8_t srate192000_en : 1;
    uint8_t reserve3       : 1;
    uint8_t srate96000_en  : 1;
    uint8_t reserve2       : 1;
    uint8_t srate48000_en  : 1;
    uint8_t srate44100_en  : 1;
    uint8_t reserve1       : 2;

    uint8_t bits32_en   : 1;
    uint8_t bits24_en   : 1;
    uint8_t bits16_en   : 1;
    uint8_t reserve4    : 1;
    uint8_t max_bitrate : 2;
    uint8_t min_bitrate : 2;

    uint8_t version_num : 4;
    uint8_t ms5         : 1;
    uint8_t reserve5    : 3;

    uint8_t ar       : 1;
    uint8_t jas      : 1;
    uint8_t meta     : 1;
    uint8_t reserve6 : 3;
    uint8_t ll       : 1;
    uint8_t lossless : 1;

    uint8_t reserve7;
} __attribute__((__packed__)) lhdc_codec_cap_t;

extern lhdc_config_t lhdc_config;

/**
 * @brief       Set LHDC slave parameters
 * @param[in]   src - Pointer to source LHDC sync information
 * @return      None
 */
void lhdc_slave_param_set(const lhdc_sync_info_t *src);

/**
 * @brief       Check if LHDC player is initialized
 * @param[in]   None
 * @return      true if LHDC player is initialized, false otherwise
 */
bool is_lhdc_player_init(void);

/**
 * @brief       Set LHDC player initialization flag
 * @param[in]   flag - Initialization flag value
 * @return      None
 */
void lhdc_player_init_set(uint8_t flag);

/**
 * @brief       Set LHDC EQ initialization flag
 * @param[in]   flag - Initialization flag value
 * @return      None
 */
void lhdc_eq_init_set(uint8_t flag);

/**
 * @brief       Check if LHDC EQ is initialized
 * @param[in]   None
 * @return      true if LHDC EQ is initialized, false otherwise
 */
bool is_lhdc_eq_init(void);

/**
 * @brief       Open LHDC audio
 * @param[in]   None
 * @return      None
 */
void lhdc_audio_open(void);

/**
 * @brief       Ready LHDC audio
 * @param[in]   None
 * @return      Operation result
 */
int lhdc_audio_ready(void);

/**
 * @brief       Update LHDC asynchronously
 * @param[in]   None
 * @return      None
 */
void async_lhdc_update(void);

/**
 * @brief       Delay LHDC mute for N milliseconds
 * @param[in]   n - Number of milliseconds to delay
 * @return      None
 */
void lhdc_mute_delay_5ms(uint16_t n);

/**
 * @brief       Reset LHDC ready time
 * @param[in]   None
 * @return      Reset value
 */
uint16_t lhdc_ready_time_reset(void);

/**
 * @brief       Adjust LHDC music frame
 * @param[in]   src - Source data pointer
 * @param[out]  dst - Destination data pointer
 * @param[in]   frame_len - Frame length
 * @return      None
 */
void lhdc_mus_frame_adjust(const uint8_t *src, uint8_t *dst, uint16_t frame_len);

/**
 * @brief       Initialize LHDC decoder player
 * @param[in]   None
 * @return      LHDC operation result
 */
lhdc_result_e lhdc_dec_player_init(void);

/**
 * @brief       Get LHDC configuration
 * @param[in]   None
 * @return      Pointer to LHDC configuration
 */
lhdc_config_t *lhdc_get_config(void);

/**
 * @brief       Verify LHDC license
 * @param[in]   None
 * @return      LHDC operation result
 */
lhdc_result_e lhdc_license_verify(void);

/**
 * @brief       Parse LHDC configuration
 * @param[in]   lhdc_cap - Pointer to LHDC codec capability
 * @return      0 if successful, non-zero otherwise
 */
uint8_t lhdc_config_parse(const lhdc_codec_cap_t *lhdc_cap);

/**
 * @brief       Verify LHDC v5 license
 * @param[in]   lhdc - Pointer to LHDC configuration
 * @return      LHDC operation result
 */
lhdc_result_e lhdcv5_license_verify(const lhdc_config_t *lhdc);

/**
 * @brief       Get license data
 * @param[in]   offset - Offset in data
 * @param[in]   data - Pointer to data
 * @param[in]   len - Length of data
 * @return      License verification result
 */
license_ret_e license_data_get(uint8_t offset, uint8_t *data, uint8_t len);

/**
 * @brief       Get LHDC v5 frame information
 * @param[in]   s - Pointer to data
 * @param[in]   len - Length of data
 * @return      Frame information result
 */
int32_t lhdcv5_get_frame_info(uint8_t *s, uint16_t len);

/**
 * @brief       Get LHDC encoder memory
 * @param[out]  addr - Pointer to memory address
 * @return      0 if successful, non-zero otherwise
 */
uint8_t lhdc_get_enc_mem(uint8_t **addr);
#endif
#endif /* __BT_MUSIC_LHDC_H__ */
