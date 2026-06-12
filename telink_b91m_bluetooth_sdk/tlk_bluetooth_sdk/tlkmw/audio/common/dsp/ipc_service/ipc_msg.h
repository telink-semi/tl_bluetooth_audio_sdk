/********************************************************************************************************
 * @file    ipc_msg.h
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
#ifndef __IPC_MSG_H__
#define __IPC_MSG_H__

#if (TLK_MW_DSP_COMM_ENABLE)
#include <stdint.h>
#include "ipc_audio.h"

#ifndef NULL
#define NULL (void *)0
#endif

/** message queue number */
#define MSG_QUEUE_NUMBER 32
/** message length, 2 words */
#define MSG_SIZE 2

/** dsp log queue number */
#define DSP_LOG_QUEUE_NUMBER 16
/** dsp log length, 2 words */
#define DSP_LOG_SIZE        2

#define IPC_MSG_MAGIC       0xff

#define IPC_MSG_PAYLOAD_LEN 6

typedef enum
{
    /* D25F to DSP Message: Request */
    IPC_D25F2DSP_HANDSHAKE = 0x0, /**< Init DSP module */
    IPC_D25F2DSP_DATA_PROCESS,    /**< Audio data is available */
    IPC_D25F2DSP_CLEANUP,         /**< Call DSP to clean up */
    IPC_D25F2DSP_SLEEP,           /**< Call DSP to sleep mode */
    IPC_D25F2DSP_SET_PARAM,       /**< Call DSP to set parameters */
    IPC_D25F2DSP_REINIT,          /**< Call DSP to reinit */
    IPC_D25F2DSP_MODE_CHANGE,     /**< Notify DSP that audio mode has changed */

    /* DSP to D25F Message: Response */
    IPC_DSP2D25F_HANDSHAKE_DONE = 0x0,
    IPC_DSP2D25F_DATA_PROCESS_DONE,
    IPC_DSP2D25F_DATA_PROCESS_ERROR,
    IPC_DSP2D25F_CLEANUP_DONE,
    IPC_D25F2DSP_SLEEP_DONE,
    IPC_DSP2D25F_SET_PARAM_DONE,
    IPC_DSP2D25F_REINIT_DONE,
    IPC_DSP2D25F_MODE_CHANGE_DONE,

    /* DSP to D25F Message: Notify */
    IPC_DSP2D25F_DEBUG_INFO, /**< DSP LOG output */
    IPC_DSP2D25F_SYS_INFO,   /**< Notify DSP system info to D25F */
} ipc_message_type_e;

typedef struct __attribute__((packed))
{
    ipc_message_type_e type         : 4; /**< message type */
    uint8_t            payload_size : 4; /**< message payload size */
    uint8_t            magic        : 8; /**< message magic number */
} ipc_message_header_t;

typedef union
{
    unsigned int ipc_msg[2];

    struct
    {
        ipc_message_header_t header; /**< 2 bytes message headher */
        uint8_t              payload[6];
    };
} ipc_message_t;

/** DSP process mode definition */
typedef enum
{
    DSP_PROCESS_TEST_MODE,
    DSP_PROCESS_VOICE_MODE,
    DSP_PROCESS_MUSIC_MODE,
    DSP_PROCESS_BYPASS_MODE,

    //    DSP_BT_VOICE_CVSD_MODE,
    //    DSP_BT_VOICE_MSBC_MODE,
    //    DSP_BT_MUSIC_SBC_MODE,
    //    DSP_BT_MUSIC_AAC_MODE,
    //    DSP_BT_LL_AUDIO_MODE,
    //    DSP_BT_LL_VOICE_MODE
} dsp_process_mode_t;

typedef enum
{
    COMMON_PARA,
    ANC_PARA,
    HRA_PARA,
    DRC_PARA,
    EQ_PARA,
} para_type_e;

/** Message payload definition */

/** IPC_D25F2DSP_HANDSHAKE */
typedef struct __attribute__((packed))
{
    uint32_t d25f_fw_version;
    uint8_t  reserved[2];
} handshake_t;

typedef struct __attribute__((packed))
{
    uint16_t d25f_alg_type;
    uint8_t  bbf_type;
    uint8_t  alg_dis;
    uint8_t  reserved[2];
} handshake_response_t;

/** IPC_DSP2D25F_HANDSHAKE_DONE */
typedef struct __attribute__((packed))
{
    uint32_t dsp_fw_version;
    uint8_t  reserved[2];
} handshake_done_t;

/** IPC_D25F2DSP_DATA_PROCESS */
typedef struct __attribute__((packed))
{
    uint8_t  mode;
    uint8_t  enc_buff_wptr;
    uint16_t enc_audio_data_len;
    uint8_t  alg_type;
#if 1
    uint8_t seq;
#else
    uint8_t audio_data_mode    : 4;
    uint8_t audio_channel_type : 4;
#endif
} data_process_t;

/** IPC_DSP2D25F_DATA_PROCESS_DONE */
typedef struct __attribute__((packed))
{
    uint8_t  mode;
    uint8_t  pcm_buff_wptr;
    uint16_t pcm_data_len;
#if 1
    uint8_t seq;
#else
    uint8_t audio_data_mode    : 4;
    uint8_t audio_channel_type : 4;
#endif
    uint8_t enc_buff_wptr : 4;
    uint8_t alg_type      : 4;
} data_process_done_t;

/** IPC_DSP2D25F_DATA_PROCESS_ERROR */
typedef struct __attribute__((packed))
{
    uint8_t reason;
    uint8_t reserved[5];
} data_process_error_t;

/** IPC_D25F2DSP_SET_PARAM */
typedef struct __attribute__((packed))
{
    uint8_t  buff_wptr;
    uint16_t data_len;
    uint8_t  para_type;
    uint8_t  reserved[2];
} set_param_t;

/** IPC_D25F2DSP_SET_PARAM */
typedef struct __attribute__((packed))
{
    uint8_t  status;
    uint8_t  buff_wptr;
    uint16_t data_len;
    uint8_t  para_type;
    uint8_t  reserved;
} set_param_done_t;

/** IPC_D25F2DSP_MODE_CHANGE */
typedef struct __attribute__((packed))
{
    uint8_t old_mode;
    uint8_t new_mode;
    uint8_t reserved[4];
} mode_change_t;

/** IPC_DSP2D25F_MODE_CHANGE_DONE */
typedef struct __attribute__((packed))
{
    uint8_t old_mode;
    uint8_t new_mode;
    uint8_t reserved[4];
} mode_change_done_t;

/** IPC_DSP2D25F_DEBUG_INFO */
typedef struct __attribute__((packed))
{
    uint8_t dsp_debug_info[6];
} debug_info_t;

/** IPC_DSP2D25F_SYS_INFO */
typedef struct __attribute__((packed))
{
    uint8_t current_mode;
    uint8_t reserved[5];
} sys_info_t;

/**
 * @brief       Send IPC message
 * @param[in]   msg_words - Message words to send
 */
void d25f_send_ipc_message(unsigned int *msg_words);

/**
 * @brief       Get IPC message
 * @param[out]  msg_words - Buffer to store received message words
 */
void d25f_get_ipc_message(unsigned int *msg_words);

/**
 * @brief       Send request to DSP
 * @param[in]   type - Message type
 * @param[in]   buffer - Data buffer
 * @param[in]   len - Data length
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_send_request_to_dsp(ipc_message_type_e type, uint8_t *buffer, uint8_t len);

/**
 * @brief       Send handshake message
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_send_handshake_msg(void);

uint8_t d25f_response_handshake_msg(void);

/**
 * @brief       Send mode change message
 * @param[in]   mode - New mode
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_send_mode_change_msg(uint8_t mode);

/**
 * @brief       Send data process message
 * @param[in]   p_audio_buf_ctx - Audio buffer context
 * @param[in]   id - IPC data path ID
 * @return      1 if successful, 0 otherwise
 */
uint8_t d25f_send_data_process_msg(audio_buffer_context_t *p_audio_buf_ctx, uint8_t id);

#endif
#endif
