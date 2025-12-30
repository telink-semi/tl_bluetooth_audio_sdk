/********************************************************************************************************
 * @file    audio_mw_manager.h
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
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__
#include "common/types.h"
#include "common/compiler.h"
#include "tl_common.h"

#ifndef TWS_APP_ENABLE
    #define TWS_APP_ENABLE 0
#endif

#ifndef audio_ram_code

    #if TWS_APP_ENABLE
        #define audio_ram_code _attribute_ram_code_sec_
    #else
        #define audio_ram_code _attribute_ram_code_sec_
    #endif
#endif

/** BT VOICE VCD */
#ifndef SL_BT_VOICE_LOG_EN
	#define SL_BT_VOICE_LOG_EN 1
#endif

/** BT MUSIC VCD */
#ifndef SL_BT_MUSIC_LOG_EN
	#define SL_BT_MUSIC_LOG_EN 1
#endif

/** TWS VCD */
#ifndef SL_TWS_AUDIO_LOG_EN
	#define SL_TWS_AUDIO_LOG_EN 1
#endif

/** LL AUDIO VCD */
#ifndef SL_LL_AUDIO_LOG_EN
	#define SL_LL_AUDIO_LOG_EN 1
#endif

/** AUDIO PATH VCD */
#ifndef SL_AUDIO_PATH_LOG_EN
	#define SL_AUDIO_PATH_LOG_EN 1
#endif

typedef enum sepid_list
{
    SEPID_SBC = 0,
    SEPID_AAC = 2,
    SEPID_LHDC,
    SEPID_MAX,
} e_sepid_list_t;

#define COMMON_AUDIO_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)

#define COMMON_AUDIO_DBG_SIGN "[MDI]"

#define MIX_MODE_PPM_ENABLE 0

//#define AUDIO_PATH_GPIO_DEBUG
//#define DUMP_AUDIO_TASK_STACK_USAGE
#define TWS_AUDIO_PATH_GPIO_DEBUG 0

#if (TLKALG_AAC_DEC_ENABLE)
    #define AAC_CODEC_ENABLE 1
#else
    #define AAC_CODEC_ENABLE 0
#endif

// AUDIO TASK
#ifndef TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE
#if (AAC_CODEC_ENABLE)
    #define TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE (30 * 1024)
#else
    #define TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE (25 * 1024)
#endif
#endif

#ifndef TICK_PER_US
    #define TICK_PER_US 24
#endif

#ifndef TONE_ADPCM_EN
#define TONE_ADPCM_EN                     1
#endif

#define TONE_SBC_EN                       0

#define SCO_ENC_QUEUE_ENABLE              1

#define HFP_CODEC_ID_CVSD                 1
#define HFP_CODEC_ID_MSBC                 2


#define BT_TPSLL_AUDIO_MIX_VOL         200 /** bt_music_vol_table_android, 6 level */
#define SCHEDULER_NEXT_TASK_GUARD_TIME_US 100

#if (TWS_ENABLE)
    #define AUDIO_TWS_MODE 1
#else
    #define AUDIO_TWS_MODE 0
#endif

#define CODEC_RAM_CODE_EN  1

#define TWS_PLAY_FIFO_SIZE 2048
#define TWS_MIC_FIFO_SIZE  2048

// samples
#define CODEC_PLAY_FIFO_SIZE  2048
#define CODEC_MIC_FIFO_SIZE   2048

#define BT_AUDIO_BUFFER_CHECK 1

#define TMUSIC_AAC_BUFF_NUM_MAX 18
#define TMUSIC_AAC_BUFF_SIZE    400

#if LHDC_CODEC_ENABLE
    #define TLK_CODEC_LHDC_ENABLE 1
    #define LHDC_MAX_SAMPLE       (480)
    #define LHDC_FRAME_MAX_SIZE   (285)

    /*16k->96k asrc*/
    #define ASRC_LEN          (24)
    #define ASRC_OUT_BUFF_LEN (ASRC_LEN * 6)
    #define TONE_BUFF_LEN     (279)

#else
    #define TLK_CODEC_LHDC_ENABLE 0
#endif

#define EQ_ENABLE                    0
#define EQ_HARDWARE_MODE_ENABLE      0

#define HFP_PLC_EN                   0
#define PLC_TEST_EN                  0

#define ADPCM8_EN                    1

#define NO_DSP_AUDIO                 0

#define LE_AUDIO_BT_MUSIC_MIX_ENABLE 1
#define LE_AUDIO_BT_VOICE_MIX_ENABLE 1

#define DSP_GPIO_DBG                 0

#define BT_MUSIC_SILENT_DETECT       0

/** RESAMPLE */
#define RESAMPLE_16_TO_48_EN  0 /** 2: share buffer */
#define RESAMPLE_16_TO_441_EN 0 /** 2: share buffer */
#define RESAMPLE_48_TO_16_EN  0 /** 2: share buffer */

#if RESAMPLE_48_TO_16_EN
    #define SRC_48to16_NUM        (3)
    #define SRC_48to16_DEN        (1)
    #define SRC_48to16_FILTER_LEN ((24 * SRC_48to16_NUM / SRC_48to16_DEN) & (~0x3))
#endif

#define CODEC_ADC_MONO_MODE 0

#define CODEC_MUTE_MIC_EN   1

#define TVOICE_PLC_ENABLE   0

#define audio_codec_adc_enable(t)

#define AUDIO_PATH_DBG 0

static inline uint32_t audio_flag_get(u32 flag)
{
    (void)flag;
#if AUDIO_TWS_MODE
    return 1;
#else
    return 0;
#endif
}

#endif //AUDIO_MANAGER_H
