/********************************************************************************************************
 * @file    tlkmw_audio_dsp.h
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
#pragma once

#if (TLK_MW_DSP_COMM_ENABLE)

    #define D25F_FW_VERSION 0x80000001

    #ifndef audio_ram_code
        #define audio_ram_code _attribute_ram_code_
    #endif

    #define SL_DSP_EN 1

typedef enum
{
    DSP_TEST_MODE,
    DSP_BT_VOICE_CVSD_MODE,
    DSP_BT_VOICE_MSBC_MODE,
    DSP_BT_MUSIC_SBC_MODE,
    DSP_BT_MUSIC_AAC_MODE,
    DSP_DONGLE_MUSIC_MODE,
    DSP_DONGLE_VOICE_MODE,
} dsp_audio_mode_t;

typedef enum
{
    BT_VOICE_CVSD_ENC,
    BT_VOICE_CVSD_DEC,
    BT_VOICE_MSBC_ENC,
    BT_VOICE_MSBC_DEC,
    BT_MUSIC_SBC_DEC,
    BT_MUSIC_AAC_DEC,
    LL_AUDIO_ENC,
    LL_AUDIO_DEC,
    MIX_MODE_LL_AUDIO_DEC,
    HRA_ALG,
    HRA_MUSIC,
    HRA_VOICE,
    BT_VOICE,
    LL_VOICE,
	VAD_NN_NS,
    DSP_BYPASS
} dsp_process_type_t;

typedef struct
{
    uint8_t ll_audio_enc_idx;
    uint8_t alg_type;
    uint8_t current_mode;
    uint8_t dsp_is_ready;
    uint8_t cur_send_enc_wptr;
} app_dsp_context_t;

extern app_dsp_context_t g_app_dsp_ctx[];

/**
 * @brief       Get DSP application context
 * @param[in]   id - IPC data path ID
 * @return      Pointer to application DSP context
 */
app_dsp_context_t *d25f_get_dsp_app_ctx(uint8_t id);

/**
 * @brief       Initialize IPC buffer
 */
void               d25f_init_ipc_buffer(void);

/**
 * @brief       Set algorithm type
 * @param[in]   alg_type - Algorithm type
 * @param[in]   id - IPC data path ID
 */
void               d25f_set_alg_type(uint8_t alg_type, uint8_t id);

/**
 * @brief       Initialize DSP environment
 */
void               d25f_init_dsp_env(void);

/**
 * @brief       Initialize TLK middleware DSP
 */
void tlkmw_dsp_init(void);

/**
 * @brief       Pause TLK middleware DSP
 */
void tlkmw_dsp_pause(void);

/**
 * @brief       Resume TLK middleware DSP
 */
void tlkmw_dsp_resume(void);

/**
 * @brief       Check if TLK middleware DSP is working
 * @return      True if DSP is working, false otherwise
 */
bool tlkmw_dsp_isWorking(void);

/**
 * @brief       Get PCM data from DSP
 * @param[out]  data_len - Pointer to store data length
 * @param[in]   id - IPC data path ID
 * @return      Pointer to PCM data
 */
uint8_t           *d25f_get_pcm_data_from_dsp(uint16_t *data_len, uint8_t id);

/**
 * @brief       Send audio data to DSP
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   len - Data length
 * @param[in]   id - IPC data path ID
 * @return      1 if successful, 0 otherwise
 */
uint8_t            d25f_send_audio_data_to_dsp(uint8_t *p_data, uint16_t len, uint8_t id);

/**
 * @brief       Get BT voice available NN buffer length
 * @param[in]   ps - Source pointer
 * @param[out]  pd - Destination pointer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_get_bt_voice_available_nn_buff_len(int16_t *ps, int16_t *pd, uint16_t len);

/**
 * @brief       Get LL voice available NN buffer length
 * @param[in]   ps - Source pointer
 * @param[out]  pd - Destination pointer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_get_ll_voice_available_nn_buff_len(int *ps, int *pd, uint16_t len);

/**
 * @brief       Reset DSP voice NN communication status
 */
void dsp_voice_nn_comm_reset_status(void);

#endif
