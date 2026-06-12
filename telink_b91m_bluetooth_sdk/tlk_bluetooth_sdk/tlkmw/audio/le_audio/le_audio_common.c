/********************************************************************************************************
 * @file    le_audio_common.c
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
#include <string.h>
#include <sys/queue.h>

#include "common/types.h"

#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/lc3/tlkalg_lc3_interface.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#include "stack/ble/ble.h"

#include "drivers.h"

#include "codec/lea_codec.h"

#include "le_audio_common.h"

#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif

#include "tlklib/usb/uac/tlkusb_uac.h"

#if (TLK_MW_LE_AUDIO_ENABLE)

#define LEA_FIX_TIMESTAMP_ERROR      1

#define LE_AUDIO_ISO_HANDLE_INVALID  0xFFFF

#define LE_AUDIO_MAX_FRAME_SAMPLE    480 // now support 48kHz 10ms, 48 * 10 = 480Byte

#define LE_AUDIO_MAX_PER_CODEC_FRAME 155 // LC3_48_6

#define LE_AUDIO_CONFIG_MAX_COUNT    2

struct lea_config_info
{
    uint8_t           enable;
    uint32_t          all_location;
    uint16_t          sample_interval; // support 10ms and 7.5ms, unit is 1us.
    uint16_t          sample_count;    // not support 44.1kHz.
    uint8_t          *lc3_alg_buffer;  //must
    struct lea_config config[LE_AUDIO_CONFIG_MAX_COUNT];
};

static struct lea_config_info s_le_audio_input;

static struct lea_config_info s_le_audio_output;
#if TLKAUD_LEA_PLAY_FLOW_CTRL_EN
uint32_t lea_tmr_ideal_tick = 0;
bool     render_sync_flag   = false;
#endif
/**
 * @brief       Reset input configuration cache and mark ISO handles invalid.
 * @return      none.
 */
void lea_input_config_initial(void)
{
    memset(&s_le_audio_input, 0, sizeof(struct lea_config_info));
    s_le_audio_input.config[0].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    s_le_audio_input.config[1].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
}

/**
 * @brief       Reset output configuration cache and mark ISO handles invalid.
 * @return      none.
 */
void lea_output_config_initial(void)
{
    memset(&s_le_audio_output, 0, sizeof(struct lea_config_info));
    s_le_audio_output.config[0].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    s_le_audio_output.config[1].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
}

/**
 * @brief       Calculate channel count encoded by the provided location bitmap.
 * @param[in]   location    - bitmap of audio locations.
 * @return      2 when stereo bitmap is set, otherwise 1.
 */
static int lea_calc_location_valid_count(uint32_t location)
{
    return location == 3 ? 2 : 1;
}

/******************LE Audio common configure LC3 encode and decode API***********************/
/*
 * Unicast Client maybe can configure LC3 after configure codec operation.
 * 		if connected TWS(check CSIS Size == 2), must set all TWS LC3 configuration.
 *
 * 	Unicast Server should configure LC3 after all ASE(sink or source) enter enable mode.
 */
struct lea_lc3_config_param
{
    struct lea_config_info *config_info;
    audio_alg_interface_t  *alg_lc3;
};

/**
 * @brief       Free LC3 algorithm buffer and deinit interface.
 * @param[in]   p_lc3_config_param   - LC3 configuration context.
 * @return      0 always.
 */
static int lea_release_lc3_module(struct lea_lc3_config_param *p_lc3_config_param)
{
    uint8_t *p_lc3_alg_buffer = p_lc3_config_param->config_info->lc3_alg_buffer;

    if (p_lc3_alg_buffer != NULL) {
        p_lc3_config_param->alg_lc3->audio_alg_deinit();
        tlkalg_free_func(p_lc3_alg_buffer);
        tlk_printf("LC3 free buffer is %x", p_lc3_alg_buffer);
        p_lc3_config_param->config_info->lc3_alg_buffer = NULL;
    }
    return 0;
}

/**
 * @brief       Allocate and initialize LC3 algorithm instance if needed.
 * @param[in]   p_lc3_config_param   - LC3 configuration context.
 * @return      0 on success, negative value when already configured or alloc fails.
 */
static int lea_config_lc3_module(struct lea_lc3_config_param *p_lc3_config_param)
{
    uint8_t *p_lc3_alg_buffer = p_lc3_config_param->config_info->lc3_alg_buffer;

    if (p_lc3_alg_buffer != NULL) {
        return -1;
    }

    int lc3_channel_count = lea_calc_location_valid_count(p_lc3_config_param->config_info->all_location);

    p_lc3_alg_buffer = (uint8_t *)tlkalg_malloc_func(p_lc3_config_param->alg_lc3->audio_alg_get_size(lc3_channel_count));

    tlk_printf("lc3 malloc is %x %x", p_lc3_alg_buffer, p_lc3_config_param->alg_lc3->audio_alg_get_size(lc3_channel_count));
    if (p_lc3_alg_buffer == NULL) {
        return -2;
    }

    p_lc3_config_param->alg_lc3->audio_alg_init(p_lc3_alg_buffer, lc3_channel_count);
    p_lc3_config_param->config_info->lc3_alg_buffer = p_lc3_alg_buffer;
    return 0;
}

/**
 * @brief       Configure LC3 encoder workspace for all input locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_input_all_location(uint32_t location)
{
    s_le_audio_input.all_location                = location;
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_input,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_ENC),
    };
    return lea_config_lc3_module(&lc3_config_param);
}

/**
 * @brief       Release LC3 encoder workspace allocated for input.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_input_location(void)
{
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_input,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_ENC),
    };

    return lea_release_lc3_module(&lc3_config_param);
}

/**
 * @brief       Configure LC3 decoder workspace for all output locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_output_all_location(uint32_t location)
{
    s_le_audio_output.all_location               = location;
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_output,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_DEC),
    };
    return lea_config_lc3_module(&lc3_config_param);
}

/**
 * @brief       Release LC3 decoder workspace allocated for output.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_output_location(void)
{
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_output,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_DEC),
    };

    return lea_release_lc3_module(&lc3_config_param);
}

/**
 * @brief       Program LC3 encoder/decoder channel parameters for a specific ASE.
 * @param[in]   location    - aggregate location bitmap across ASEs.
 * @param[in]   p_config    - ASE configuration.
 * @param[in]   p_alg_lc3   - LC3 algorithm interface.
 * @return      none.
 */
static void lea_set_lc3_start(uint32_t location, const struct lea_config *p_config, audio_alg_interface_t *p_alg_lc3)
{
    struct lc3_initial_param_bap lc3Init = {
        .channelIndex  = 0,
        .samplingFreq  = p_config->samplingFrequency,
        .perCodecFrame = p_config->frameOctets,
        .frameDuration = p_config->frameDuration,
    };

    if (p_config->location == LEA_LOCATION_FRONT_LEFT) {
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    } else if (p_config->location == LEA_LOCATION_FRONT_RIGHT) {
        if (location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
            lc3Init.channelIndex = 1;
        }
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    } else {
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
        lc3Init.channelIndex = 1;
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    }
}

/**
 * @brief       Cache a new ASE configuration, ensuring frequency/duration consistency.
 * @param[in]   p_config_info   - configuration group (input/output).
 * @param[in]   p_config        - new ASE configuration.
 * @return      0 on success, negative value when conflicts detected.
 */
static int lea_set_audio_config(struct lea_config_info *p_config_info, const struct lea_config *p_config)
{
    int free_config_index = 0xFF;
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        if (p_config_info->config[i].iso_handle == LE_AUDIO_ISO_HANDLE_INVALID && free_config_index == 0xFF) {
            free_config_index = i;
        }

        if (p_config_info->config[i].iso_handle == p_config->iso_handle) {
            return -1; //this iso handle had configure
        }

        if (p_config_info->config[i].iso_handle != LE_AUDIO_ISO_HANDLE_INVALID) {
            if (p_config_info->config[i].samplingFrequency != p_config->samplingFrequency || p_config_info->config[i].frameDuration != p_config->frameDuration) {
                return -2; // must same frequency and duration.
            }
        }
    }

    if (free_config_index == 0xFF) {
        return -3;
    }

    memcpy(&p_config_info->config[free_config_index], p_config, sizeof(struct lea_config));
    return 0;
}

/**
 * @brief       Mark cached ASE configuration as invalid for an ISO handle.
 * @param[in]   p_config_info   - configuration group (input/output).
 * @param[in]   iso_handle      - ISO handle to release.
 * @return      none.
 */
static void lea_release_audio_config(struct lea_config_info *p_config_info, uint16_t iso_handle)
{
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        if (p_config_info->config[i].iso_handle == iso_handle) {
            p_config_info->config[i].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
            break;
        }
    }
}

/******************LE Audio common configuration LC3 encode and decode API***********************/

/******************LE Audio Input *********************************/
static uint32_t s_le_audio_get_input_time = 0;

/**
 * @brief       Enable audio input path and reset acquisition timer.
 * @param[in]   get_time    - reference timestamp (optional).
 * @return      none.
 */
void lea_open_input(uint32_t get_time)
{
    s_le_audio_input.enable = true;
    (void)get_time;
    s_le_audio_get_input_time = clock_time() | 1;

    lea_codec_input_clean_buffer();
}

/**
 * @brief       Disable audio input path and clear ASE state.
 * @return      none.
 */
void lea_close_input(void)
{
    s_le_audio_input.enable   = false;
    s_le_audio_get_input_time = 0;

    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        s_le_audio_input.config[i].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    }
}

/**
 * @brief       Translate LC3 selectors to PCM sample count per frame.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      Number of PCM samples per channel per frame.
 */
static uint32_t lea_calc_sample_count(uint8_t frequency, uint8_t duration)
{
    if (duration == LEA_SELECT_FRAME_DURATION_10) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 10;
        }
    } else if (duration == LEA_SELECT_FRAME_DURATION_7_5) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 7.5;
        }
    }
    return 0;
}

/**
 * @brief       Program input sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_input_sample_config_bap(uint8_t frequency, uint8_t duration)
{
#if (!TLK_CFG_TEMP_DRAM_OPTM)
    s_le_audio_input.sample_count = lea_calc_sample_count(frequency, duration);
#endif
    s_le_audio_input.sample_interval = duration == LEA_SELECT_FRAME_DURATION_7_5 ? 7500 : 10000;
}

/**
 * @brief       Store ASE specific input configuration and start LC3 encoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_input_config(const struct lea_config *p_config)
{
    lea_set_audio_config(&s_le_audio_input, p_config);
    lea_set_lc3_start(s_le_audio_input.all_location, p_config, audio_alg_get_interface_by_type(ALG_LC3_ENC));
}

/**
 * @brief       Remove stored input configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_input_config(uint16_t iso_handle)
{
    lea_release_audio_config(&s_le_audio_input, iso_handle);
}

/**
 * @brief       Periodic input task: read PCM, encode LC3, send ISO SDUs.
 * @return      none.
 */
void lea_input_main_process(void)
{
    if (!s_le_audio_input.enable || !clock_time_exceed(s_le_audio_get_input_time, s_le_audio_input.sample_interval)) {
        return;
    }

    int16_t left_data[LE_AUDIO_MAX_FRAME_SAMPLE], right_data[LE_AUDIO_MAX_FRAME_SAMPLE];

    if (clock_time_exceed(s_le_audio_get_input_time, s_le_audio_input.sample_interval * 2)) { // TODO:
        lea_codec_input_get_audio_data(left_data, right_data, s_le_audio_input.sample_count);
        lea_codec_input_get_audio_data(left_data, right_data, s_le_audio_input.sample_count);
        s_le_audio_get_input_time += 2 * s_le_audio_input.sample_interval * SYSTEM_TIMER_TICK_1US;
        return;
    }

    //gpio_toggle(GPIO_CHN8);
    s_le_audio_get_input_time += s_le_audio_input.sample_interval * SYSTEM_TIMER_TICK_1US;

    if (!lea_codec_input_get_audio_data(left_data, right_data, s_le_audio_input.sample_count)) {
        return;
    }

    audio_alg_interface_t *p_lc3_en_alg = audio_alg_get_interface_by_type(ALG_LC3_ENC);
    uint8_t                encode_data[LE_AUDIO_MAX_PER_CODEC_FRAME * 2]; // left + right;
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_input_config = &s_le_audio_input.config[i];
        if (p_input_config->iso_handle == LE_AUDIO_ISO_HANDLE_INVALID) {
            continue;
        }

        if (p_input_config->location == LEA_LOCATION_FRONT_LEFT) {
            p_lc3_en_alg->audio_alg_process((uint8_t *)left_data, encode_data, s_le_audio_input.sample_count * sizeof(int16_t), 16, 0); //LC3 default 0.
        } else if (p_input_config->location == LEA_LOCATION_FRONT_RIGHT) {
            int lc3_index = 0;
            if (s_le_audio_input.all_location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
                lc3_index = 1;
            }
            p_lc3_en_alg->audio_alg_process((uint8_t *)right_data, encode_data + p_input_config->frameOctets, s_le_audio_input.sample_count * sizeof(int16_t), 16,
                                            lc3_index); //LC3 default 0.
        } else {
            p_lc3_en_alg->audio_alg_process((uint8_t *)left_data, encode_data, s_le_audio_input.sample_count * sizeof(int16_t), 16, 0);
            p_lc3_en_alg->audio_alg_process((uint8_t *)right_data, encode_data + p_input_config->frameOctets, s_le_audio_input.sample_count * sizeof(int16_t), 16, 1);
        }
    }

    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_input_config = &s_le_audio_input.config[i];

        if (p_input_config->iso_handle == LE_AUDIO_ISO_HANDLE_INVALID) {
            continue;
        }
        if (p_input_config->location == LEA_LOCATION_FRONT_LEFT) {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data, p_input_config->frameOctets);
        } else if (p_input_config->location == LEA_LOCATION_FRONT_RIGHT) {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data + p_input_config->frameOctets, p_input_config->frameOctets);
        } else {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data, 2 * p_input_config->frameOctets);
        }
    }
}

/******************LE Audio Output *********************************/
static void lea_output_receive_sdu_packet(uint16_t conn_handle, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data);

/**
 * @brief       Register ISO sink callback and prepare output path.
 * @return      none.
 */
void lea_open_output(void)
{
    ble_host_iso_data_init(lea_output_receive_sdu_packet);
}

static void lea_release_all_output_audio_pkt(void);

/**
 * @brief       Stop ISO sink and release queued output audio packets.
 * @return      none.
 */
void lea_close_output(void)
{
    ble_host_iso_data_deinit();

    lea_release_all_output_audio_pkt();
    lea_release_output_location();
}

/**
 * @brief       Store ASE specific output configuration and start LC3 decoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_output_config(const struct lea_config *p_config)
{
    lea_set_audio_config(&s_le_audio_output, p_config);
    lea_set_lc3_start(s_le_audio_output.all_location, p_config, audio_alg_get_interface_by_type(ALG_LC3_DEC));
}

/**
 * @brief       Program output sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_output_sample_config_bap(uint8_t frequency, uint8_t duration)
{
    s_le_audio_output.sample_count    = lea_calc_sample_count(frequency, duration);
    s_le_audio_output.sample_interval = duration == LEA_SELECT_FRAME_DURATION_7_5 ? 7500 : 10000;
}

/**
 * @brief       Remove stored output configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_output_config(uint16_t iso_handle)
{
    lea_release_audio_config(&s_le_audio_output, iso_handle);
}

struct lea_output_audio_pkt_node
{
    SLIST_ENTRY(lea_output_audio_pkt_node) next;
    uint32_t render_point;
    uint32_t location;
    int16_t *left_data;
    int16_t *right_data;
};

SLIST_HEAD(lea_output_audio_pkt, lea_output_audio_pkt_node);

static struct lea_output_audio_pkt output_pkt_header = SLIST_HEAD_INITIALIZER(output_pkt_header);

/**
 * @brief       Remove the first queued output packet and free memory.
 * @return      none.
 */
static void lea_free_output_pkt_header(void)
{
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);
    SLIST_REMOVE_HEAD(&output_pkt_header, next); //TODO:

    // tlk_printf("free is %x clock:%x", pNode, clock_time());
    tlkalg_free_func((uint8_t *)pNode);
}

/**
 * @brief       Flush and free all queued output packets.
 * @return      none.
 */
static void lea_release_all_output_audio_pkt(void)
{
    while (!SLIST_EMPTY(&output_pkt_header)) {
        lea_free_output_pkt_header();
    }
}

/**
 * @brief       Allocate an output packet structure with PCM buffers.
 * @param[in]   render_point   - timestamp when packet should be rendered.
 * @return      pointer to allocated node (never NULL, blocks on failure).
 */
static struct lea_output_audio_pkt_node *lea_malloc_output_audio_pkt_node(uint32_t render_point)
{
    struct lea_output_audio_pkt_node *pNode =
        (struct lea_output_audio_pkt_node *)tlkalg_malloc_func(sizeof(struct lea_output_audio_pkt_node) + 2 * sizeof(int16_t) * s_le_audio_output.sample_count);

    // tlk_printf("malloc is %x %x", pNode, clock_time());
    if (pNode == NULL) {
        tlkapi_printf(1, "pNode is Null,malloc fail");
        tlkmdi_audmem_print();
        // malloc failed.
        while (1) {
            /* USB DEBUG */
#if TLK_USB_UDB_ENABLE
            tlk_udb_usb_handle_irq();
#endif
        }
    }

    pNode->render_point = render_point;
    pNode->location     = 0;
    pNode->left_data    = (int16_t *)(((uint8_t *)pNode) + sizeof(struct lea_output_audio_pkt_node));
    pNode->right_data   = (int16_t *)(((uint8_t *)pNode->left_data) + sizeof(int16_t) * s_le_audio_output.sample_count);
    return pNode;
}

/**
 * @brief       Insert packet node ordered by render point or reuse existing one.
 * @param[in]   render_point   - timestamp to insert.
 * @return      pointer to node representing that render point.
 */
static struct lea_output_audio_pkt_node *lea_insert_output_audio_pkt(uint32_t render_point)
{
    bool                              new_node_used = false;
    struct lea_output_audio_pkt_node *pNewNode      = lea_malloc_output_audio_pkt_node(render_point);
    struct lea_output_audio_pkt_node *pNode         = pNewNode;
    //    tlk_printf("malloc pNewNode is %p", pNewNode);
    tlksys_enter_critical();
    // tlkapi_printf(1, "header is %x, render_point is %x, current clock it %x", output_pkt_header.slh_first, render_point, clock_time());
    if (SLIST_EMPTY(&output_pkt_header)) {
        uint32_t next_irq_us = (render_point - clock_time()) / SYSTEM_TIMER_TICK_1US;
        if (next_irq_us > 200 * 1000) { // if next render point is to far.
            tlksys_leave_critical();
            tlkalg_free_func((uint8_t *)pNewNode);
            return NULL;
        }
        SLIST_INSERT_HEAD(&output_pkt_header, pNewNode, next);
        // tlkapi_printf(1, "insert first packet");
        // tlkapi_printf(1, "insert first packet next time is %x %x %x", render_point, clock_time(), render_point - clock_time());
#if !TLKAUD_LEA_PLAY_FLOW_CTRL_EN
        tlkmdi_audio_task_set_next_irq(next_irq_us);
#endif
        new_node_used = true;
    } else {
        pNode                                   = SLIST_FIRST(&output_pkt_header);
        struct lea_output_audio_pkt_node *pPrev = NULL;
        while (pNode != NULL) {
#if LEA_FIX_TIMESTAMP_ERROR
            bool check_timestamp = false;
            if (pNode->render_point > render_point) {
                check_timestamp = (pNode->render_point - render_point) < 200 * SYSTEM_TIMER_TICK_1US;
            } else {
                check_timestamp = (render_point - pNode->render_point) < 200 * SYSTEM_TIMER_TICK_1US;
            }
            if (check_timestamp) {
#else
            if (pNode->render_point == render_point) {
#endif
                tlksys_leave_critical();
                tlkalg_free_func((uint8_t *)pNewNode);
                // tlk_printf("free pNewNode is %p", pNewNode);
                //  tlkapi_printf(1, "found node.");
                return pNode;
            }

            // TODO: IAL need report event packet.
            if ((pNode->render_point - render_point) < (uint32_t)BIT(31)) {
                break;
            }

            pPrev = pNode;

            pNode = SLIST_NEXT(pNode, next);
        }
        //  tlkapi_printf(1, "create new node.");
        pNode         = pNewNode;
        new_node_used = true;
        if (pPrev == NULL) {
            SLIST_INSERT_HEAD(&output_pkt_header, pNode, next);
        } else {
            SLIST_INSERT_AFTER(pPrev, pNode, next);
        }
    }
    tlksys_leave_critical();
    if (new_node_used == false) {
        // tlk_printf("free pNewNode is %p", pNewNode);
        tlkalg_free_func((uint8_t *)pNewNode);
    }
    return pNode;
}

/**
 * @brief       Decode incoming ISO SDU into PCM buffers and queue for render.
 * @param[in]   p_lea_config   - ASE configuration describing ISO handle.
 * @param[in]   timestamp      - SDU timestamp from controller.
 * @param[in]   iso_data_len   - length of ISO payload.
 * @param[in]   p_iso_data     - pointer to ISO payload.
 * @return      none.
 */
static void lea_output_deal_sdu_packet(struct lea_config *p_lea_config, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data)
{
    int frame_octets = p_lea_config->frameOctets;
    int location     = p_lea_config->location;
    int location_cnt = lea_calc_location_valid_count(location);

    int frame_time = (p_lea_config->frameDuration == LEA_SELECT_FRAME_DURATION_10 ? 10000 : 7500) * SYSTEM_TIMER_TICK_1US;

    int            sdu_len  = iso_data_len;
    const uint8_t *sdu_data = p_iso_data;

    uint32_t render_point = timestamp + p_lea_config->presentationDelay * SYSTEM_TIMER_TICK_1US;

    for (int i = 0; i < p_lea_config->blocks; i++) {
        struct lc3_dec_set_bec_detect detect = {
            .channelIndex = 0,
            .detect       = 0,
        };

        if (sdu_len < frame_octets * location_cnt) {
            detect.detect = 1;
        }

        struct lea_output_audio_pkt_node *audio_pkt = lea_insert_output_audio_pkt(render_point);
        if (audio_pkt != NULL) {
            audio_alg_interface_t *p_lc3_dec_alg = audio_alg_get_interface_by_type(ALG_LC3_DEC);
            if (location == LEA_LOCATION_FRONT_LEFT) {
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->left_data, frame_octets, 16, 0);
                audio_pkt->location |= LEA_LOCATION_FRONT_LEFT;
            } else if (location == LEA_LOCATION_FRONT_RIGHT) {
                if (s_le_audio_output.all_location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
                    detect.channelIndex = 1;
                }
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->right_data, frame_octets, 16, detect.channelIndex);
                audio_pkt->location |= LEA_LOCATION_FRONT_RIGHT;
            } else {
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                detect.channelIndex = 1;
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->left_data, frame_octets, 16, 0);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data + frame_octets, (uint8_t *)audio_pkt->right_data, frame_octets, 16, 1);
                audio_pkt->location |= (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
            }
        }

        sdu_data += frame_octets * location_cnt;
        sdu_len -= frame_octets * location_cnt;
        render_point += frame_time;
    }
}

/**
 * @brief       ISO callback that dispatches SDUs to the matching ASE config.
 * @param[in]   conn_handle    - ISO connection handle.
 * @param[in]   timestamp      - SDU timestamp.
 * @param[in]   iso_data_len   - length of ISO payload.
 * @param[in]   p_iso_data     - pointer to ISO payload.
 * @return      none.
 */
static void lea_output_receive_sdu_packet(uint16_t conn_handle, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data)
{
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_lea_config = &s_le_audio_output.config[i];

        if (p_lea_config->iso_handle == conn_handle) {
            lea_output_deal_sdu_packet(p_lea_config, timestamp, iso_data_len, p_iso_data);
            break;
        }
    }
}

/**
 * @brief       Program audio timer IRQ based on head packet render time.
 * @return      none.
 */
#if !TLKAUD_LEA_PLAY_FLOW_CTRL_EN
static void lea_output_set_next_irq(void)
{
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);

    if (pNode != NULL) {
        uint32_t next_irq_us = (pNode->render_point - clock_time()) / SYSTEM_TIMER_TICK_1US;
        if (next_irq_us > 20 * 1000) {    // if next render point is to far.
            lea_free_output_pkt_header(); // free the first node.
            lea_output_set_next_irq();    // check next node.
        } else {
            tlkmdi_audio_task_set_next_irq(next_irq_us);
        }
    }
}
#endif
/**
 * @brief       Audio timer interrupt handler that renders queued PCM frames.
 * @return      none.
 */
void lea_output_timer_irq(void)
{
#if TLKAUD_LEA_PLAY_FLOW_CTRL_EN
    uint32_t tdiff_us, tdiff_tick;
    tlkmdi_audio_stop_timer();

    uint32_t tick_cur = clock_time();
    if (tick_cur > lea_tmr_ideal_tick) {
        tdiff_tick = tick_cur - lea_tmr_ideal_tick;
    } else {
        tdiff_tick = 0xffffffff - lea_tmr_ideal_tick + tick_cur;
    }
    tdiff_us = LEA_TRANS_PLAY_DATA_INTVAL - tdiff_tick / SYSTEM_TIMER_TICK_1US;
    lea_tmr_ideal_tick += LEA_TRANS_PLAY_DATA_INTVAL * SYSTEM_TIMER_TICK_1US;

    if (tdiff_us > LEA_TRANS_PLAY_DATA_INTVAL * 2) {
        tdiff_us           = LEA_TRANS_PLAY_DATA_INTVAL;
        lea_tmr_ideal_tick = tick_cur + LEA_TRANS_PLAY_DATA_INTVAL * SYSTEM_TIMER_TICK_1US;
    }

    tlkmdi_audio_task_set_next_irq(tdiff_us);

    if (!SLIST_EMPTY(&output_pkt_header) && tlkaud_midbuf_get_spk_idlelen() >= TLKADU_MIDBUF_SPK_LEN / 2) {
        struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);
        if (pNode == NULL) {
            return;
        }
        if (!render_sync_flag) {
            uint32_t tick_diff   = pNode->render_point - clock_time();
            uint16_t sample_diff = tick_diff / 500; //1sample = 500ticks in 48kHz
            if (sample_diff > (TLKADU_MIDBUF_SPK_LEN - s_le_audio_output.sample_count)) {
                tlkapi_printf(APP_LOG_EN, "render sync fail %d", sample_diff);
                return;
            }
            tlkmdi_midbuf_sync_spk(sample_diff);
            render_sync_flag = true;
            tlkapi_printf(APP_LOG_EN, "render sync succ %d", sample_diff);
        }
        int16_t *left_data  = lea_codec_get_mute_buffer();
        int16_t *right_data = lea_codec_get_mute_buffer();
        int      sample_num = s_le_audio_output.sample_count;

        if (pNode->location & LEA_LOCATION_FRONT_LEFT) {
            left_data = pNode->left_data;
        }

        if (pNode->location & LEA_LOCATION_FRONT_RIGHT) {
            right_data = pNode->right_data;
        }

        lea_codec_output_set_audio_data(left_data, right_data, sample_num);

        lea_free_output_pkt_header();
    }

    tlkaud_uac_spk_data_downlink(LEA_TRANS_PLAY_DATA_INTVAL / 1000);

    // if (tlkaud_get_audio_mode() & AUDIO_UAC_MIC) {
    //     uint32_t sr = tlkusb_uac_get_iso_in_SampleRate();
    //     uint16_t smp_len = (sr / 1000) * (LEA_TRANS_PLAY_DATA_INTVAL / 1000);
    //     tlkaud_uac_mic_data_uplink(smp_len, sr);
    // }


#else
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);

    if (pNode == NULL) {
        return;
    }

    //gpio_toggle(GPIO_CHN4);
    int16_t *left_data  = lea_codec_get_mute_buffer();
    int16_t *right_data = lea_codec_get_mute_buffer();
    int      sample_num = s_le_audio_output.sample_count;

    if (pNode->location & LEA_LOCATION_FRONT_LEFT) {
        left_data = pNode->left_data;
    }

    if (pNode->location & LEA_LOCATION_FRONT_RIGHT) {
        right_data = pNode->right_data;
    }

    lea_codec_output_set_audio_data(left_data, right_data, sample_num);

    lea_free_output_pkt_header();

    lea_output_set_next_irq();
#endif
}

#endif //#if (TLKAPI_LE_AUDIO_COMMON_ENABLE)
