/********************************************************************************************************
 * @file    wt.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page WT
 *
 *  API Reference
 *  ===============
 *  Header File: wt.h
 */

#ifndef __WT_H_
#define __WT_H_

#include "lib/include/analog.h"
#include "lib/include/pm/pm.h"
#include "lib/include/stimer.h"
#include "gpio.h"

/**********************************************************************************************************************
 *                                                 WT enum/struct                                                     *
 *********************************************************************************************************************/
/*!
 * @name WT enum/struct
 * @{
 */

/**
 * @brief Voice Tracking parameter.
 * @note  This parameter allows to set the adaptation speed of the system depending on the voice input.
 *        When the setting of this parameter is low, the high-frequency sensitivity of the VAD increases,
 *        some phonemes can be detected easily but high-frequency ambient noise can be considered as voice.
 *        When the setting of this parameter is high, the high-frequency sensitivity of the VAD decreases,
 *        high-frequency ambient noise is filtered but some phonemes can be lost.
 */
typedef enum
{
    WT_VTRACK_0 = 0, /**< fast */
    WT_VTRACK_1 = 1, /**< default */
    WT_VTRACK_2 = 2,
    WT_VTRACK_3 = 3, /**< slow */
} wt_vtrack_e;

/**
 * @brief Background Noise Tracking parameter.
 * @note  This parame-ter allows to set the speed of the system adaptation to the ambient noise.
 *        This parameter gives the flexibility to adapt the VAD to the application environment.
 */
typedef enum
{
    WT_NTRACK_0 = 0, /**< fast */
    WT_NTRACK_1 = 1,
    WT_NTRACK_2 = 2,
    WT_NTRACK_3 = 3,
    WT_NTRACK_4 = 4, /**< default */
    WT_NTRACK_5 = 5,
    WT_NTRACK_6 = 6,
    WT_NTRACK_7 = 7, /**< slow */
} wt_ntrack_e;

/**
 * @brief Power Level Sensitivity.
 * @note  Ratio between ambient noise and voice level to be detected.
 *        When the setting of this parameter is low, the VAD sensitivity increases,
 *        leading to higher VDV(Voice Detected as Voice) and possibly higher NDV(Noise detected as Voice).
 *        When the setting of this parameter is high, the VAD sensitivity decreases,
 *        leading to lower NDV and possibly lower VDV.
 */

typedef enum
{
    WT_PWR_2dB  = 0,
    WT_PWR_3dB  = 1,
    WT_PWR_4dB  = 2,
    WT_PWR_5dB  = 3,
    WT_PWR_6dB  = 4, /**< default */
    WT_PWR_8dB  = 5,
    WT_PWR_10dB = 6,
    WT_PWR_16dB = 7,
} wt_power_level_sens_e;

/**
 * @brief Minimum Delay.
 * @note  This parameter allows to set the minimum time before a detection when switching to Always listening mode.
 *        This delay is defined as a number of clock cycle, divided from MCLK depending on MCLK_DIV setting.
 */

typedef enum
{
    WT_MINDELAY_768  = 0,
    WT_MINDELAY_1536 = 1, /**< default */
    WT_MINDELAY_3584 = 2,
    WT_MINDELAY_9632 = 3,
} wt_min_delay_e;

/**
 * @brief Minimum Event Duration.
 * @note  This parameter allows to set the Minimum vocal signal duration that can be detected by the system.
 *        When the setting of this parameter is low, the detection latency decreases
 *        but the high-frequency ambient noise can be considered as voice.
 *        When the setting of this parameter is high, the high-frequency ambient noise is filtered
 *        but the detection latency increase. This delay is defined as a number of clock cycle,
 *        divided from MCLK depending on MCLK_DIV setting.
 */

typedef enum
{
    WT_MINEVENT_1       = 0, /**< 1 Mclk cycle */
    WT_MINEVENT_16      = 1, /**< 16 Mclk cycle */
    WT_MINEVENT_32      = 2, /**< 32 Mclk cycle default */
    WT_MINEVENT_64      = 3, /**< 64 Mclk cycle */
    WT_MINEVENT_128     = 4, /**< 128 Mclk cycle */
    WT_MINEVENT_256     = 5, /**< 256 Mclk cycle */
    WT_MINEVENT_512     = 6, /**< 512 Mclk cycle */
    WT_MINEVENT_1024    = 7, /**< 1024 Mclk cycle */
} wt_min_event_duration_e;

/**
 * @brief Clock selection.
 * @note  The following table describes gives the MCLK(32K) frequency supported for each clock division ratio.
 *        Clock division ratio (MCLK_DIV)    Supported MCLK frequency
 *                1(default)                      24 kHz to 64 kHz   MCLK = 32K
 *                2                               48 kHz to 128 kHz
 *                4                               96 kHz to 256 kHz
 *                8                               192 kHz to 512 kHz
 *                16                              384 kHz to 1.024 MHz
 *                24                              576 kHz to 1.536 MHz
 *                48                              1.152 MHz to 3 MHz
 */

typedef enum
{
    WT_CLKDIV_1      = 0, /**< 1 MCLK_DIV default */
    WT_CLKDIV_2      = 1, /**< 2 MCLK_DIV */
    WT_CLKDIV_4      = 2, /**< 4 MCLK_DIV */
    WT_CLKDIV_8      = 3, /**< 8 MCLK_DIV */
    WT_CLKDIV_16     = 4, /**< 16 MCLK_DIV */
    WT_CLKDIV_24     = 5, /**< 24 MCLK_DIV */
    WT_CLKDIV_48     = 6, /**< 48 MCLK_DIV */
} wt_clkdiv_e;

/**
 * @brief vad pin enum.
 * @note  analog signals
 */
typedef enum
{
    WT_P_IN = GPIO_PI0,
    WT_N_IN = GPIO_PI1,
} wt_pin_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                   WT interface                                                     *
 *********************************************************************************************************************/
/*!
 * @name WT interface
 * @{
 */

/**
 * @brief      This function serves to initialize wt.
 * @return     none
  */
void wt_init(void);

/**
 * @brief      This function configures wt analog pin detecting signal.
 * @param[in]  ana_p - analog positive pin
 * @param[in]  ana_n - analog negative pin
 * @return     none
 */
void wt_set_ana_pin(wt_pin_e ana_p, wt_pin_e ana_n);

/**
 * @brief      This function serves to read wt analog register.
 * @param[in]  addr - wt analog addr
 * @return     none
  */
_attribute_ram_code_sec_noinline_ unsigned char wt_read_reg(unsigned int addr);

/**
 * @brief      This function serves to set wt voice tracking parameter.
 * @param      val - wt_vtrack_e
 * @return     none
  */
static inline void wt_set_vtr(wt_vtrack_e val)
{
    analog_write_reg8(areg_0x120, (wt_read_reg(areg_0x120) & (~FLD_WT_VTRACK)) | MASK_VAL(FLD_WT_VTRACK, val));
}

/**
 * @brief      This function serves to set wt background noise tracking parameter.
 * @param[in]  val - wt_ntrack_e
 * @return     none
  */
static inline void wt_set_ntr(wt_ntrack_e val)
{
    analog_write_reg8(areg_0x120, (wt_read_reg(areg_0x120) & (~FLD_WT_NTRACK)) | MASK_VAL(FLD_WT_NTRACK, val));
}

/**
 * @brief      This function serves to set wt power level sensitivity.
 * @param[in]  val - wt_power_level_sens_e
 * @return     none
  */
static inline void wt_set_pls(wt_power_level_sens_e val)
{
    analog_write_reg8(areg_0x120, (wt_read_reg(areg_0x120) & (~FLD_WT_PWR)) | MASK_VAL(FLD_WT_PWR, val));
}

/**
 * @brief      This function serves to set wt min delay.
 * @param[in]  val - wt_min_delay_e
 * @return     none
  */
static inline void wt_set_delay(wt_min_delay_e val)
{
    analog_write_reg8(areg_0x121, (wt_read_reg(areg_0x121) & (~FLD_WT_MINDELAY)) | MASK_VAL(FLD_WT_MINDELAY, val));
}

/**
 * @brief      This function serves to set wt minimum event duration.
 * @param[in]  val - wt_min_event_duration_e
 * @return     none
  */
static inline void wt_set_dur(wt_min_event_duration_e val)
{
    analog_write_reg8(areg_0x121, (wt_read_reg(areg_0x121) & (~FLD_WT_MINEVENT)) | MASK_VAL(FLD_WT_MINEVENT, val));
}

/**
 * @brief      This function serves to set wt nfi detection.
 * @param[in]  val
 * @return     This parameter defines the NFI threshold above witch a IRQ is sent.
  */
static inline void wt_set_nfi(unsigned char val)
{
    analog_write_reg8(areg_0x122, (wt_read_reg(areg_0x122) & (~FLD_WT_NFI_DET)) | MASK_VAL(FLD_WT_NFI_DET, val));
}

/**
 * @brief      This function serves to set wt mclk div.
 * @param[in]  val - wt_clkdiv_e
 * @return     none
  */
static inline void wt_set_mdiv(wt_clkdiv_e val)
{
    analog_write_reg8(areg_0x123, (wt_read_reg(areg_0x123) & (~FLD_WT_MCLK_DIV)) | MASK_VAL(FLD_WT_MCLK_DIV, val));
}

/**
 * @brief      This function serves to set wt stand-by mode.
 * @param[in]  val:  0 - The WhisperTrigger is activated (sleep or listening mode)
 *                   1 - The WhisperTrigger is deactivated (stand-by mode)
 * @return     none
  */
static inline void wt_set_sb_mode(unsigned char val)
{
    analog_write_reg8(areg_0x123, (wt_read_reg(areg_0x123) & (~FLD_WT_SB)) | MASK_VAL(FLD_WT_SB, val));
}

/**
 * @brief      This function serves to set wt sleep mode.
 * @param[in]  val:  0 - If SB = 0, The WhisperTrigger is in listening mode
 *                   1 - If SB = 0, The WhisperTrigger is in sleep mode
 * @return     none
  */
static inline void wt_set_sleep_mode(unsigned char val)
{
    analog_write_reg8(areg_0x123, (wt_read_reg(areg_0x123) & (~FLD_WT_SLEEP)) | MASK_VAL(FLD_WT_SLEEP, val));
}

/**
 * @brief      This function serves to set wt irq mode.
 * @param[in]  val:  0 - The generated IRQ is a high level
 *                   1 - The generated IRQ is a pulse, which duration is 8 internal clock cycles
 * @return     none
  */
static inline void wt_set_irq_mode(unsigned char val)
{
    analog_write_reg8(areg_0x124, (wt_read_reg(areg_0x124) & (~FLD_WT_IRQ_MODE)) | MASK_VAL(FLD_WT_IRQ_MODE, val));
}

/**
 * @brief      This function serves to read wt irq flag.
 * @return     return 1 when a voice detection event occurs
  */
static inline unsigned char wt_read_irq_flag(void)
{
    return (wt_read_reg(areg_0x124) & FLD_WT_IRQ_FLAG) >> 1;
}

/**
 * @brief      This function serves to clear wt irq flag.
 * @note       Writing 1 resets the flag and the IRQ to 0
  */
static inline void wt_clr_irq_flag(void)
{
    analog_write_reg8(areg_0x124, (wt_read_reg(areg_0x124) & (~FLD_WT_IRQ_FLAG)) | MASK_VAL(FLD_WT_IRQ_FLAG, 1));
}

#endif /* _WT_H_ */
