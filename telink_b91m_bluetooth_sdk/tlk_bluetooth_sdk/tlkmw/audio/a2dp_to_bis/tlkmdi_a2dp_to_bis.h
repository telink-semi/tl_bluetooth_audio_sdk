/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis.h
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
#ifndef _A2DP_TO_BIS_H_
#define _A2DP_TO_BIS_H_

#include "drivers.h"

typedef struct
{
    uint8_t  enable;
    uint8_t  codec_type;
    uint16_t acl_handle;
} tlkmdi_a2dp_to_bis_bt_env_t;

typedef struct
{
    uint16_t middle_buff_wptr;
    uint16_t middle_buff_rptr;
    uint32_t sample_rate;
    bool     bis_sync_flag;
    bool     timer_big_alig_flag;
} tlkmdi_a2dp_to_bis_ctl;

typedef enum
{
    TLKMDI_AUDIO_PATH_A2DP2BIS = 0,
    TLKMDI_AUDIO_PATH_BTMUSIC,
    TLKMDI_AUDIO_PATH_MAX,
} tlkmdi_audio_path_scene;

/**
 * @brief       Set the audio scene for A2DP to BIS
 * @param[in]   scene - Audio scene to set
 * @return      None
 */
void tlkmdi_a2dp_to_bis_set_audio_scene(tlkmdi_audio_path_scene scene);

/**
 * @brief       Get the current audio scene for A2DP to BIS
 * @param       None
 * @return      Current audio scene
 */
tlkmdi_audio_path_scene tlkmdi_a2dp_to_bis_get_audio_scene(void);

/**
 * @brief       Get ACL handle for A2DP to BIS
 * @param       None
 * @return      ACL handle
 */
uint16_t tlkmdi_a2dp_to_bis_bt_get_acl_handle(void);

/**
 * @brief       Check if A2DP to BIS is enabled
 * @param       None
 * @return      Non-zero if enabled, zero if disabled
 */
uint16_t tlkmdi_a2dp_to_bis_is_enabled(void);

/*
 * @brief  Initial the A2DP sink block, register the data callback.
 * @param[in] None
 * @return TLK_ENONE is success,other value is false.
 */
int tlkmdi_a2dp_to_bis_init(void);

/**
 * @brief       Start A2DP to BIS streaming
 * @param[in]   handle - Connection handle
 * @param[in]   param - Additional parameters (unused)
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if already enabled
 */
int tlkmdi_a2dp_to_bis_start(uint16_t handle, uint32_t param);

/**
 * @brief       Close A2DP to BIS streaming
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE
 */
int tlkmdi_a2dp_to_bis_close(uint16_t handle);

/**
 * @brief       Skip to next track
 * @param       None
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if not enabled
 */
bool tlkmdi_a2dp_to_bis_next(void);

/**
 * @brief       Skip to previous track
 * @param       None
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if not enabled
 */
bool tlkmdi_a2dp_to_bis_previous(void);

/**
 * @brief       Handle audio operations for A2DP to BIS
 * @param[in]   handle - Connection handle
 * @param[in]   opcode - Operation code
 * @param[in]   pdata - Pointer to data (unused)
 * @param[in]   dataLen - Length of data (unused)
 * @return      true if operation was handled, false otherwise
 */
bool tlkmdi_a2dp_to_bis_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Switch A2DP to BIS state
 * @param[in]   handle - Connection handle
 * @param[in]   status - New status
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Check if A2DP to BIS is busy
 * @param       None
 * @return      true if busy (enabled), false otherwise
 */
bool tlkmdi_a2dp_to_bis_is_busy(void);

/**
 * @brief       Start audio timer for A2DP to BIS
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_start_audio_timer(void);

/**
 * @brief       Get middle buffer write pointer
 * @param       None
 * @return      Write pointer position
 */
uint16_t tlkmdi_a2dp_to_bis_get_mid_buff_wptr(void);

/**
 * @brief       Get middle buffer read pointer
 * @param       None
 * @return      Read pointer position
 */
uint16_t tlkmdi_a2dp_to_bis_get_mid_buff_rptr(void);

/**
 * @brief       Get middle buffer available samples
 * @param       None
 * @return      Available samples count
 */
uint32_t tlkmdi_a2dp_to_bis_get_middle_available_sample(void);

/**
 * @brief       Get middle buffer idle size
 * @param       None
 * @return      Idle size in samples
 */
uint32_t tlkmdi_a2dp_to_bis_get_middle_idle_size(void);

/**
 * @brief       Fill middle buffer with data
 * @param[in]   pData - Pointer to data to fill
 * @param[in]   dataLen - Length of data in bytes
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_fill_middle_buff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Get data from middle buffer
 * @param[out]  pData - Pointer to buffer to store data
 * @param[in]   dataLen - Length of data to read in bytes
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_get_data_middle_buff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Timer interrupt handler for A2DP to BIS
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_timer_irq_handler(void);

/**
 * @brief       Synchronize middle buffer pointers
 * @param[in]   sample - Sample count to synchronize to
 * @return      None
 */
void tlkmdi_a2dp_to_bis_sync_middle_buff(uint16_t sample);

/**
 * @brief       Set BIS synchronization flag
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_set_bis_sync_flag(void);

/**
 * @brief       Get BIS synchronization flag
 * @param       None
 * @return      true if synchronized, false otherwise
 */
bool tlkmdi_a2dp_to_bis_get_sync_flag(void);

/**
 * @brief       Set total delay for A2DP to BIS
 * @param[in]   total_delay - Total delay in microseconds
 * @return      None
 */
void tlkmdi_a2dp_to_bis_set_total_delay(uint32_t total_delay);
#endif
