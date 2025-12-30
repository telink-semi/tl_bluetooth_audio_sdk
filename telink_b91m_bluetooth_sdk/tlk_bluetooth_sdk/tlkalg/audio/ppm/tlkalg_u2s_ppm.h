/********************************************************************************************************
 * @file    tlkalg_u2s_ppm.h
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
#ifndef TLKALG_U2S_PPM_H
#define TLKALG_U2S_PPM_H
#include "tlkalg_ppm_interface.h"

#ifndef TICK_PER_US
    #define TICK_PER_US 24
#endif
#define TICK_SAMPLERATE_COEFF      (48 / TICK_PER_US)
#define REF_SAMPLES                256
#define TICKS_PER_MS               (TICK_PER_US * 1000)
#define TICKS_PER_10S              (10 * 1000 * TICKS_PER_MS)

#define TICKSX2_PER_SAMPLE         (TICK_PER_US * 1000 * TICK_SAMPLERATE_COEFF / 48)
#define REF_SAMPLE_TICKS           (REF_SAMPLES * TICKSX2_PER_SAMPLE)
#define REF_TICK_DRIFT             (150 * TICK_PER_US)

#define TLKALG_USB_OUT_PPM_SAMPLES 48

typedef enum
{
    PPM_REF_MODE_NONE  = 0,
    PPM_REF_MODE_VOICE = 0x01,
    PPM_REF_MODE_MUSIC = 0x02,
} tlku2s_ppm_ref_mode_e;

typedef struct
{
    uint8_t st;
    uint8_t num;
    uint8_t ref_mode;                 //refer to tlku2s_ppm_ref_mode_e
    uint8_t valid;
    bool    down_ppm_set; /**< A flag to indicate that ppm processing needs to be initialized. */
    bool    up_ppm_set;
    uint16_t  resv2byte;
 
    uint32_t t_m_last;
    uint32_t t_m_ref;

    int32_t  t_m_samples_ma;
    int32_t  t_m_samples_last;
    uint32_t t_ref_avg;
    uint32_t t_ref;
    uint32_t t_m_samples_buff;

    int16_t t_m_ppm;
    int16_t t_m_rem_last;
    int16_t t_m_samples_cur;
    int16_t t_m_samples;
} tlku2s_ppm_ctrl_t;

typedef struct
{
    int8_t   sample_diff; /*mic: alg input sample number subtract output number 
                              spk: alg output sample number subtract input number  */
    bool     start_flag;
    uint16_t resv2byte;
    uint32_t tick_last;   /*last ticks*/
    uint32_t tick_rem;    /*remain ticks, not enough one millisecond*/
} tlku2h_ppm_para_t;

extern int32_t           g_usb_resample_diff;
extern tlku2s_ppm_ctrl_t g_usb_ppm;

/**
 * @brief       This function clears the status of the USB PPM control.
 * @return      none.
 */
void tlkalg_u2s_ppm_clear_status(void);

/**
 * @brief       This function sets the initialization flags for down and up PPM.
 * @return      none.
 */
void tlkalg_u2s_ppm_set_init_flag(void);

/**
 * @brief       This function clears the initialization flag for down PPM.
 * @return      none.
 */
void tlkalg_u2s_ppm_clear_init_flag_down(void);

/**
 * @brief       This function clears the initialization flag for up PPM.
 * @return      none.
 */
void tlkalg_u2s_ppm_clear_init_flag_up(void);

/**
 * @brief       This function retrieves the initialization flag for down PPM.
 * @return      The down PPM initialization flag.
 */
bool tlkalg_u2s_ppm_get_init_flag_down(void);

/**
 * @brief       This function retrieves the initialization flag for up PPM.
 * @return      The up PPM initialization flag.
 */
bool tlkalg_u2s_ppm_get_init_flag_up(void);

/**
 * @brief       This function retrieves the current PPM value.
 * @return      The current PPM value.
 */
int16_t tlkalg_u2s_ppm_get_val(void);

/**
 * @brief       This function sets the PPM value.
 * @param[in]   val     - The PPM value to set.
 * @return      none.
 */
void    tlkalg_u2s_ppm_set_val(int16_t val);

/**
 * @brief       This function sets the PPM mode.
 * @param[in]   mode    - The mode to set.
 * @return      none.
 */
void                  tlkalg_u2s_ppm_set_mode(tlku2s_ppm_ref_mode_e mode);

/**
 * @brief       This function retrieves the current PPM mode.
 * @return      The current PPM mode.
 */
tlku2s_ppm_ref_mode_e tlkalg_u2s_ppm_get_mode(void);

/**
 * @brief       This function performs the PPM calculation.
 * @param[in]   tick    - The current tick value.
 * @return      none.
 */
void                  tlkalg_u2s_ppm_calculation(uint32_t tick);

#endif
