/********************************************************************************************************
 * @file    tlkmdi_lea_bms.h
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
#ifndef TLKMDI_LEA_BROADCAST_MEDIA_SENDER_H
#define TLKMDI_LEA_BROADCAST_MEDIA_SENDER_H

struct lea_bms_config
{ // refer to struct lea_config.
    uint8_t  blocks;
    uint32_t location;
    uint8_t  samplingFrequency;
    uint8_t  frameDuration;
    uint16_t frameOctets;

    uint16_t iso_handle;
    uint32_t presentationDelay;
};

struct lea_bms_stream_param
{
    uint8_t               frequency;
    uint32_t              location;
    uint8_t               num;
    struct lea_bms_config bms_config[2];
};

/**
 * @brief       Initialize BMS state, callbacks, and open 48 kHz codec.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_bms_init(void);

/**
 * @brief       Check whether BMS audio pipeline is currently active.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_bms_is_busy(void);

/**
 * @brief       Handle audio open/close requests for BMS pipeline.
 * @param[in]   handle  - audio manager handle (unused).
 * @param[in]   status  - TLK_STATE_* transition command.
 * @return      true if transition accepted, false otherwise.
 */
bool tlkmdi_lea_bms_switch(uint16_t handle, uint8_t status);

struct tlkmdi_lea_bms_config
{
    uint8_t  samplingFrequency;
    uint8_t  frameDuration;
    uint16_t frameOctets;
    uint16_t iso_handle[2];
};

/**
 * @brief       Configure codec/input path and add ASE configs for broadcast source.
 * @param[in]   p_config    - LC3 configuration from controller.
 * @return      none.
 */
void tlkmdi_lea_bms_start(struct tlkmdi_lea_bms_config *p_config);

/**
 * @brief       Cache stream parameters and set ISO data path host->controller.
 * @param[in]   param   - stream parameter descriptor.
 * @return      none.
 */
void tlkmdi_lea_bms_start_stream(struct lea_bms_stream_param *param);

/**
 * @brief       Notify audio manager to close BMS stream.
 * @return      none.
 */
void tlkmdi_lea_bms_stop_stream(void);

#endif
