/********************************************************************************************************
 * @file    ipc_audio.h
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
#ifndef __IPC_AUDIO_H__
#define __IPC_AUDIO_H__

#if (TLK_MW_DSP_COMM_ENABLE)

#include <stdint.h>

#define DSP_DBG_FLAG  ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)

#define DSP_DBG_SIGN  "[DSP]"

#define DSP_GPIO_CHN4 GPIO_PJ3
#define DSP_GPIO_CHN5 GPIO_PJ2
#define DSP_GPIO_CHN6 GPIO_PJ1
#define DSP_GPIO_CHN7 GPIO_PJ0

typedef enum
{
    IPC_DATA_PATH_0,
    IPC_DATA_PATH_1,
    IPC_SET_PARAM_PATH_2,
    IPC_DATA_PATH_MAX,
} ipc_data_path_e;

#define BT_VOICE_ID              IPC_DATA_PATH_0
#define LL_VOICE_ID              IPC_DATA_PATH_0

#define AUDIO_ENC_ID             IPC_DATA_PATH_0
#define AUDIO_DEC_ID             IPC_DATA_PATH_0

#define BT_VOICE_ENC_ID          IPC_DATA_PATH_0
#define BT_VOICE_DEC_ID          IPC_DATA_PATH_1

#define BT_MUSIC_DEC_ID          IPC_DATA_PATH_1

#define LL_AUDIO_ENC_ID          IPC_DATA_PATH_0
#define LL_AUDIO_DEC_ID          IPC_DATA_PATH_1

#define NN_NS_16K_20MS_ID        IPC_DATA_PATH_0

#define ALG_ANC_ID               IPC_DATA_PATH_0

#define MIX_MODE_LL_AUDIO_DEC_ID IPC_DATA_PATH_0

// LC3 BUFFER + bt audio
#if 1
// DATA PATH 0 buffer, MSBC frame len:  120 samples 240 bytes
// DATA PATH 0 buffer, LC3 frame len:   120 samples 240 bytes
// lc3 16K frame length: 10ms, 160 samples, 16bits 320bytes, 24bits 640bytes
#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM     2
#define IPC_DATA_PATH0_PCM_BUFFER_NUM              2

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE 2048
#define IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE    2048

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN     IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN        IPC_DATA_PATH0_PCM_BUFFER_NUM *IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE

#define IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM     2
#define IPC_DATA_PATH1_PCM_BUFFER_NUM              2

#define IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE 1024
#define IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE    1024

#define IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN     IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH1_PCM_AUDIO_BUFFER_LEN        IPC_DATA_PATH1_PCM_BUFFER_NUM *IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE

// DATA PATH 2 buffer 9k, mix ll audio
#define IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_NUM     2
#define IPC_DATA_PATH2_PCM_BUFFER_NUM              2

#define IPC_DATA_PATH2_ENCODE_AUDIO_BUF_FRAME_SIZE 512
#define IPC_DATA_PATH2_PCM_AUDIO_BUF_FRAME_SIZE    1024 * 3

#define IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN     IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH2_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH2_PCM_AUDIO_BUFFER_LEN        IPC_DATA_PATH2_PCM_BUFFER_NUM *IPC_DATA_PATH2_PCM_AUDIO_BUF_FRAME_SIZE

//ANC
#if 0 //TLKALG_ANC_ENABLE
#define IPC_ANC_MIC_BUFFER_LEN (512 * 4)
#define IPC_ANC_SPK_BUFFER_LEN (512 * 4)
#else
#define IPC_ANC_MIC_BUFFER_LEN 0
#define IPC_ANC_SPK_BUFFER_LEN 0
#endif

//ALL PATH
#define IPC_DATA_PATH0_LEN      (IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN)
#define IPC_DATA_PATH1_LEN      (IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH1_PCM_AUDIO_BUFFER_LEN)
#define IPC_SET_PARAM_PATH2_LEN (IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH2_PCM_AUDIO_BUFFER_LEN)
#define IPC_ANC_BUFF_LEN        (IPC_ANC_MIC_BUFFER_LEN + IPC_ANC_SPK_BUFFER_LEN)
#define IPC_BUFF_LEN            (IPC_DATA_PATH0_LEN + IPC_DATA_PATH1_LEN + IPC_SET_PARAM_PATH2_LEN + IPC_ANC_BUFF_LEN)
#endif

// LC3 BUFFER
#if 0
// DATA PATH 0 buffer
#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM     4
#define IPC_DATA_PATH0_PCM_BUFFER_NUM              4

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE 1024
#define IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE    512

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN     IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN        IPC_DATA_PATH0_PCM_BUFFER_NUM *IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE

// DATA PATH 1 buffer
#define IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM     4
#define IPC_DATA_PATH1_PCM_BUFFER_NUM              4

#define IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE 256
#define IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE    4096

// DATA PATH 2 buffer 9k, mix ll audio
#define IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_NUM     4
#define IPC_DATA_PATH2_PCM_BUFFER_NUM              4

#define IPC_DATA_PATH2_ENCODE_AUDIO_BUF_FRAME_SIZE 256
#define IPC_DATA_PATH2_PCM_AUDIO_BUF_FRAME_SIZE    256

#endif

// BT sbc and msbc mode
#if 0
// DATA PATH 0 buffer
#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM     4
#define IPC_DATA_PATH0_PCM_BUFFER_NUM              4

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE 512
#define IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE    512

#define IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN     IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN        IPC_DATA_PATH0_PCM_BUFFER_NUM *IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE

// DATA PATH 1 buffer
#define IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM     4
#define IPC_DATA_PATH1_PCM_BUFFER_NUM              4

#define IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE 512
#define IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE    512
#endif

// PATH1
#define IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH1_PCM_AUDIO_BUFFER_LEN    IPC_DATA_PATH1_PCM_BUFFER_NUM *IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE

// PATH2
#define IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_NUM *IPC_DATA_PATH2_ENCODE_AUDIO_BUF_FRAME_SIZE
#define IPC_DATA_PATH2_PCM_AUDIO_BUFFER_LEN    IPC_DATA_PATH2_PCM_BUFFER_NUM *IPC_DATA_PATH2_PCM_AUDIO_BUF_FRAME_SIZE


#define IPC_DATA_PATH0_LEN                     (IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN)
#define IPC_DATA_PATH1_LEN                     (IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH1_PCM_AUDIO_BUFFER_LEN)

#define IPC_DATA_PATH2_LEN                     (IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN + IPC_DATA_PATH2_PCM_AUDIO_BUFFER_LEN)

typedef enum
{
    AUDIO_BIT_16_DATA,
    AUDIO_BIT_20_DATA,
    AUDIO_BIT_24_DATA,
} audio_data_mode_e;

typedef enum
{
    AUDIO_MONO,
    AUDIO_STEREO,
} audio_channel_type_e;

typedef struct
{
    uint8_t  enc_seq;
    uint8_t *p_enc_audio_buff;
    uint16_t enc_audio_buff_total_len;
    uint16_t enc_audio_buff_frame_size;
    uint16_t enc_audio_buff_num;
    uint16_t enc_audio_buff_wptr;
    uint16_t enc_audio_buff_rptr;
    uint16_t enc_audio_data_len;

    uint8_t  pcm_seq;
    uint8_t *p_pcm_buff;
    uint16_t pcm_buff_total_len;
    uint16_t pcm_buff_frame_size;
    uint16_t pcm_buff_num;
    uint16_t pcm_buff_wptr;
    uint16_t pcm_buff_rptr;
    uint16_t pcm_data_len[8];

    audio_data_mode_e    audio_data_mode;
    audio_channel_type_e audio_channel_type;

    uint8_t current_mode;
    uint8_t alg_type;
} audio_buffer_context_t;

/**
 * @brief       Initialize audio buffer
 * @param[in]   p_audio_buff - Pointer to audio buffer
 */
void audio_buff_init(uint8_t *p_audio_buff);

/**
 * @brief       Get audio buffer context pointer
 * @param[in]   id - IPC data path ID
 * @return      Pointer to audio buffer context
 */
audio_buffer_context_t *get_audio_buff_context_ptr(uint8_t id);

/**
 * @brief       Set audio data mode and channel type
 * @param[in]   mode - Audio data mode
 * @param[in]   type - Audio channel type
 * @param[in]   id - IPC data path ID
 */
void d25f_set_audio_data_mode_and_channel_type(uint8_t mode, uint8_t type, uint8_t id);

/**
 * @brief       Update encode buffer read pointer
 * @param[in]   id - IPC data path ID
 */
void d25f_update_enc_buff_rptr(uint8_t id);

/**
 * @brief       Update encode buffer write pointer
 * @param[in]   id - IPC data path ID
 */
void d25f_update_enc_buff_wptr(uint8_t id);

/**
 * @brief       Get number of free encode buffers
 * @param[in]   id - IPC data path ID
 * @return      Number of free encode buffers
 */
int16_t d25f_get_number_of_enc_buff_free(uint8_t id);

/**
 * @brief       Get number of available PCM buffers
 * @param[in]   id - IPC data path ID
 * @return      Number of available PCM buffers
 */
int16_t d25f_get_number_of_pcm_buff_available(uint8_t id);

/**
 * @brief       Get PCM buffer
 * @param[out]  pcm_data_len - Pointer to store PCM data length
 * @param[in]   id - IPC data path ID
 * @return      Pointer to PCM buffer
 */
uint8_t *d25f_get_pcm_buff(uint16_t *pcm_data_len, uint8_t id);

/**
 * @brief       Get encode buffer pointer
 * @param[in]   id - IPC data path ID
 * @return      Pointer to encode buffer
 */
uint8_t *d25f_get_enc_buff_ptr(uint8_t id);

/**
 * @brief       Post audio data to encode buffer
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   len - Length of audio data
 * @param[in]   id - IPC data path ID
 */
void d25f_post_audio_data_to_enc_buff(uint8_t *p_data, uint16_t len, uint8_t id);

/**
 * @brief       Test function for posting audio data
 * @param[in]   id - IPC data path ID
 */
void d25f_post_audio_data_test(uint8_t id);

/**
 * @brief       Test function for posting audio data in timer
 * @param[in]   id - IPC data path ID
 */
void d25f_post_audio_data_test_in_timer(uint8_t id);

/**
 * @brief       Post sine wave audio data
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   id - IPC data path ID
 */
void d25f_post_sin_audio_data(uint8_t *p_data, uint8_t id);
#endif
#endif
