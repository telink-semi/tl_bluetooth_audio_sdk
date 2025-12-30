/********************************************************************************************************
 * @file    ext_audio.h
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
#ifndef DRIVERS_B92_EXT_DRIVER_EXT_AUDIO_H_
#define DRIVERS_B92_EXT_DRIVER_EXT_AUDIO_H_
#include "../driver.h"
#include "types.h"
#include "compiler.h"
#include <string.h>

typedef struct
{
    u8 dft_cap;
    u8 cap0;
    u8 rscv1;

    u8  calib_reset_cnt;
    u8  rscv2;
    u16 calib_last_pdu_num;

    u32 calib_last_timestamp;
    int calib_diff_t;
    u32 interval;


} blc_audio_ctr_t;

extern blc_audio_ctr_t audio_ctr;

/**
 * @brief     This function serves to init the usb module.
 * @return    none
 */
void blc_audio_usb_init(void);

/**
 * @brief     This function serves to calibrated the clock.
 * @param[in] timeStamp  - capture tick.
 * @param[in] baseTick   - calibration tick.
 * @return    none
 */
void blc_audio_clock_calib(u32 timeStamp, u32 baseTick);
void blc_audio_clock_calib_init(void);
void ble_audio_reset(void);
void blc_audio_clock_calib2(u32 timeStamp, u32 baseTick, u32 offset);

/**
 * @brief     This function serves to adjust volume.
 * @param[in] pcmData  - data need to adjust.
 * @param[in] len      - data length.
 * @return    none
 */
void blc_usb_adjust_volume(u16 *pcmData, u16 len);

/**
 * @brief     This function serves to init timer.
 * @param[in] timer      - TIMER0 or TIMER1.
 * @param[in] init_tick  - the start tick of the timer.
 * @param[in] cap_tick   - the capture tick of the timer.
 * @return    none
 */
void ble_audio_timer_init(timer_type_e timer);

/**
 * @brief     This function serves to set the capture of timer.
 * @param[in] timer      - TIMER0 or TIMER1.
 * @param[in] init_tick  - the start tick of the timer.
 * @param[in] cap_tick   - the capture tick of the timer.
 * @return    none
 */
void ble_audio_timer_set_capture(timer_type_e timer, unsigned int init_tick, unsigned int cap_tick);

#endif /* DRIVERS_B92_EXT_DRIVER_EXT_AUDIO_H_ */
