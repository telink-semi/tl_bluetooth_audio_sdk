/********************************************************************************************************
 * @file    le_audio_common.h
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
#ifndef TLKMDI_LEA_UNICAST_SERVER_COMMON_H
#define TLKMDI_LEA_UNICAST_SERVER_COMMON_H

#define MARGIN_NUM               6
#define PPM_ALG_BUFF_LEN         (480 + MARGIN_NUM)
#define LEA_PPM_CALC_INTERVAL_MS 1000
#define LEA_PPM_MAX              500
#define LEA_PPM_SMOOTH_ALPHA     2 //1~10

struct lea_config
{ // refer to struct lea_bmr_config,
    uint8_t  blocks;
    uint32_t location;
    uint8_t  samplingFrequency;
    uint8_t  frameDuration;
    uint16_t frameOctets;

    uint16_t iso_handle;
    uint32_t presentationDelay;
};

typedef struct
{
    int16_t  sample_diff;
    uint16_t alg_wptr;
    uint16_t alg_rptr;
    uint16_t recv;
    int16_t  alg_data_buffer[480 + MARGIN_NUM];
} lea_ppm_ctl_t;

typedef struct
{
    int32_t samplerate;
    int32_t sample_nominal;
    int32_t current_ppm;
    int32_t last_ppm;
    int32_t asrc_diff;
    int32_t tick_remainder;
    int32_t ppm_remainder;
    int32_t tick_per_sample;
    bool    update_flag;
} lea_ppm_state_t;

void tlkalg_lea_ppm_init(int samplerate);
void tlkalg_lea_ppm_calc(int32_t diff_tick);

/**
 * @brief       Initialize LE Audio input configuration cache.
 * @return      none.
 */
void lea_input_config_initial(void);

/**
 * @brief       Initialize LE Audio output configuration cache.
 * @return      none.
 */
void lea_output_config_initial(void);

/**
 * @brief       Configure LC3 encoder workspace for all input locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_input_all_location(uint32_t location);

/**
 * @brief       Release LC3 encoder workspace allocated for input.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_input_location(void);

/**
 * @brief       Configure LC3 decoder workspace for all output locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_output_all_location(uint32_t location);

/**
 * @brief       Release LC3 decoder workspace allocated for output.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_output_location(void);

/**
 * @brief       Enable audio input path and reset acquisition timer.
 * @param[in]   get_time    - reference timestamp (optional).
 * @return      none.
 */
void lea_open_input(uint32_t get_time);

/**
 * @brief       Store ASE specific input configuration and start LC3 encoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_input_config(const struct lea_config *p_config);

/**
 * @brief       Disable audio input path and clear ASE state.
 * @return      none.
 */
void lea_close_input(void);

/**
 * @brief       Program input sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_input_sample_config_bap(uint8_t frequency, uint8_t duration);

/**
 * @brief       Remove stored input configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_input_config(uint16_t iso_handle);

/**
 * @brief       Periodic input task: read PCM, encode LC3, send ISO SDUs.
 * @return      none.
 */
void lea_input_main_process(void);

/**
 * @brief       Register ISO sink callback and prepare output path.
 * @return      none.
 */
void lea_open_output(void);

/**
 * @brief       Store ASE specific output configuration and start LC3 decoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_output_config(const struct lea_config *p_config);

/**
 * @brief       Stop ISO sink and release queued output audio packets.
 * @return      none.
 */
void lea_close_output(void);

/**
 * @brief       Program output sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_output_sample_config_bap(uint8_t frequency, uint8_t duration);

/**
 * @brief       Remove stored output configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_output_config(uint16_t iso_handle);

/**
 * @brief       Audio timer interrupt handler that renders queued PCM frames.
 * @return      none.
 */
void lea_output_timer_irq(void);

#endif //      TLKMDI_LEA_UNICAST_SERVER_COMMON_H
