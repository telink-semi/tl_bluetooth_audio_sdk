/********************************************************************************************************
 * @file    tlkalg_asrc_24bit_interface.h
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
#ifndef __TLKALG_ASRC_INTERFACE_H__
#define __TLKALG_ASRC_INTERFACE_H__

#include "tl_common.h"
#include "tlkalg/audio/audio_alg_interface.h"

/**
 * @brief       Get the size required for 16to48 24bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to48_24bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 16to48 24bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_24bit_init(uint8_t *p_buff, uint8_t channel);

uint8_t tlkalg_asrc_16to48_24bit_param_set(uint8_t type, void *param);
/**
 * @brief       Deinitialize the 16to48 24bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_24bit_deinit(void);

/**
 * @brief       Process the 16to48 24bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to48_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 16to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to48_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 16to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 16to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_16bit_deinit(void);

/**
 * @brief       Process the 16to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 48to16 24bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to16_24bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 48to16 24bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_24bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 48to16 24bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_24bit_deinit(void);

/**
 * @brief       Process the 48to16 24bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to16_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 48to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to16_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 48to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 48to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_16bit_deinit(void);

/**
 * @brief       Process the 48to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 441to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to48_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 441to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 441to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_deinit(void);

/**
 * @brief       Process the 441to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 441to48 16bit backup ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to48_16bit_backup_get_size(uint8_t channel);

/**
 * @brief       Initialize the 441to48 16bit backup ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_backup_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 441to48 16bit backup ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_backup_deinit(void);

/**
 * @brief       Process the 441to48 16bit backup ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to48_16bit_backup_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 441to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to16_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 441to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to16_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 441to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to16_16bit_deinit(void);

/**
 * @brief       Process the 441to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 16to441 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to441_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 16to441 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to441_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 16to441 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to441_16bit_deinit(void);

/**
 * @brief       Process the 16to441 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to441_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 48to32 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to32_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 48to32 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to32_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 48to32 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to32_16bit_deinit(void);

/**
 * @brief       Process the 48to32 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to32_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 32to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_32to48_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 32to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to48_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 32to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to48_16bit_deinit(void);

/**
 * @brief       Process the 32to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_32to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for 32to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_32to16_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the 32to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to16_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the 32to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to16_16bit_deinit(void);

/**
 * @brief       Process the 32to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_32to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for speaker PPM 16bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the speaker PPM 16bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Set the PPM parameter for the speaker 16bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_16bit_param_set(uint8_t type, void *param);

/**
 * @brief       Deinitialize the speaker PPM 16bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_16bit_deinit(void);

/**
 * @brief       Process the speaker PPM 16bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for speaker PPM 24bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_24bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the speaker PPM 24bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_24bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Set the PPM parameter for the speaker 24bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_24bit_param_set(uint8_t type, void *param);

/**
 * @brief       Deinitialize the speaker PPM 24bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_24bit_deinit(void);

/**
 * @brief       Process the speaker PPM 24bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for speaker PPM 24bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_mic_24bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the microphone PPM 24bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_24bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Set the PPM parameter for the microphone 24bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_mic_24bit_param_set(uint8_t type, void *param);

/**
 * @brief       Deinitialize the microphone PPM 24bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_24bit_deinit(void);

/**
 * @brief       Process the microphone PPM 24bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_mic_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for microphone PPM 16bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_mic_16bit_get_size(uint8_t channel);

/**
 * @brief       Initialize the microphone PPM 16bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_16bit_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Set the PPM parameter for the microphone 16bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_mic_16bit_param_set(uint8_t type, void *param);

/**
 * @brief       Deinitialize the microphone PPM 16bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_16bit_deinit(void);

/**
 * @brief       Process the microphone PPM 16bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_mic_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif
