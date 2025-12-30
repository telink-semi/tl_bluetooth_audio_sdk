/********************************************************************************************************
 * @file    tlkmdi_lea_bmr.h
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
#ifndef TLKMDI_LEA_BROADCAST_MEDIA_RECEIVER_H
#define TLKMDI_LEA_BROADCAST_MEDIA_RECEIVER_H

struct lea_bmr_config
{ // refer to struct lea_config.
    uint8_t  blocks;
    uint32_t location;
    uint16_t iso_handle;
};

struct lea_bmr_stream_param
{
    uint8_t               frequency;
    uint8_t               frame_duration;
    uint16_t              frame_octets;
    uint32_t              all_location;
    uint32_t              presentation_delay;
    uint8_t               num;
    struct lea_bmr_config bmr_config[2];
};

/**
 * @brief       Initialize BMR state and LC3 decoder configuration storage.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_bmr_init(void);

/**
 * @brief       Indicate whether BMR audio path is already active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_bmr_is_busy(void);

/**
 * @brief       Handle audio open/close requests from the audio manager.
 * @param[in]   handle  - audio handle from manager (unused).
 * @param[in]   status  - TLK_STATE_* command describing transition.
 * @return      true when the transition is accepted, false otherwise.
 */
bool tlkmdi_lea_bmr_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Save stream parameters and setup ISO controller data paths.
 * @param[in]   param   - pointer to stream parameters.
 * @return      none.
 */
void tlkmdi_lea_bmr_set_stream_param(struct lea_bmr_stream_param *param);

/**
 * @brief       Tear down ISO controller data paths if stream is active.
 * @return      none.
 */
void tlkmdi_lea_bmr_release_stream_param(void);

/**
 * @brief       Request audio subsystem to start BMR playback.
 * @return      none.
 */
void tlkmdi_lea_bmr_start_stream(void);

/**
 * @brief       Request audio subsystem to stop BMR playback.
 * @return      none.
 */
void tlkmdi_lea_bmr_stop_stream(void);

#endif
