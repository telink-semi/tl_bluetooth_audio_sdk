/********************************************************************************************************
 * @file    app_config.h
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
#pragma once

#include "core/mcu_type.h"

#define TLK_LOW_POWER_ENABLE       0
#define TLK_CFG_RTOS_ENABLE        1

#define DEBUG_SUSPEND_ENABLE       0
#define DEBUG_MINGQIAN_GPIO_ENABLE 0

#if DEBUG_SUSPEND_ENABLE
#define TLK_CFG_SUSPEND_ENABLE 1
#define DEBUG_BT_SNIFF_ENABLE  1
#else
#define DEBUG_BT_SNIFF_ENABLE 0
#endif

// APP System Configuration//
#if TLK_LOW_POWER_ENABLE
#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       0
#define TLK_DEBUG_ENABLE         0
#define TLK_CFG_USB_ENABLE       0
#define TLK_USB_UDB_ENABLE       0
#define TLK_CFG_UART_TOOL_ENABLE 0
#else
#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       1
#define TLK_CFG_UART_TOOL_ENABLE 1
#endif

#define TLK_DEV_SY5500_ENABLE  0
#define TLK_MW_TINYSQL_ENABLE  1
#define TLK_CFG_TONE_ENABLE    1

#define TLK_MW_BTREC_ENABLE    1

#define TLKBTP_PROF_VIR_ENABLE 1 // profile virtual link

//BT Stack Configuration//
#define TLK_STK_BT_ENABLE           1
#define TLKBTP_CFG_RFC_ENABLE       (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE       (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE     (1 && TLKBTP_CFG_HFP_ENABLE)
#define TLKBTP_CFG_SPP_ENABLE       (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_SPP_USER_ENABLE  (0 && TLKBTP_CFG_SPP_ENABLE)

#define TLKBTP_CFG_A2DP_ENABLE      (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE   (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG BTP_A2DP_FEATURE_OPTION_BIT_SNK_SBC

#define TLKBTP_CFG_AVRCP_ENABLE     (1 && TLK_STK_BT_ENABLE)


#define TLKBTP_CFG_ATT_ENABLE       (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_ATTCLT_ENABLE    (0 && TLKBTP_CFG_ATT_ENABLE)
#define TLKBTP_CFG_ATTSRV_ENABLE    (1 && TLKBTP_CFG_ATT_ENABLE)


#define AUDIO_PATH_24BITS_EN        1
#define VCD_DEFINE_SELECT           VCD_DEFINE_BTTPSLL_TWS_TL751X //VCD_DEFINE_MINGQIAN_TWS

#define TLK_CFG_TPSLL_HCI_ENABLE    1
#define TLK_CFG_TWS_TPT_ENABLE      1
#define TLKSTK_BTTPSLL_TWS_ENABLE   1
#define TLK_STK_TPT_ENABLE          1

//DONGLE VOICE MIC DATA EN
#define DONGLE_VOICE_MIC_EN 1

//ULTRA LOW LATENCY EN
#define ULTRA_LOW_LATENCY_EN 1

//SPK config
/* disable by mingqian 20250717, confirm by xiaogang, will make music mix function error.*/
#define CODEC_DAC_MONO_MODE 1

//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE       1
#define TLK_CFG_ALG_ENABLE         1
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#define AUDIO_CODEC_LOOPBACK       0 ///default 0; used for codec hardware test
#define CODEC_CUSTOMERS_SPEC_REQ   1 ///default 0;	1:enable  OUTPUT -3dB
#define TLK_MW_DSP_COMM_ENABLE     1
#define TLKALG_ANC_ENABLE          0
#define TLKMW_FIFO_IRQ_EN          1

#if TLKALG_ANC_ENABLE
#define TLKDRV_VOICE_MIC     TLKDRV_CODEC_MICA
#define TLKDRV_ANC_MIC       TLKDRV_CODEC_MICB
#define TX_FIFO_IRQ_ENABLE   0
#define DEC_PCM_DATA_TO_DSP  1
#define ANC_RECORD_BUFF_SIZE 512
#define ANC_PLAY_BUFF_SIZE   512
#define PCM_BUFF_WPTR_INDEX  (CODEC_SPK_FIFO_SAMPLES)
#define PCM_BUFF_RPTR_INDEX  (CODEC_SPK_FIFO_SAMPLES + 1)
#endif
#define GET_PCM_DATA_TICK_INDEX (CODEC_SPK_FIFO_SAMPLES + 2)

#define TWS_ENABLE              1

#define APP_LOG_EN              1

//#define BT_MUSIC_SUPPORT_DYNAMIC_LATENCY

#define BT_TPSLL_OPTIMIZE_LATENCY_TEST 1

#define DSP_NN_NS_BUFF_DISENABLE       1

/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE 1
#define TLKALG_SBC_ENC_ENABLE 1
#define TLKALG_MSBC_ENABLE    1
#define TLKALG_CVSD_ENABLE    1
#define TLKALG_AAC_DEC_ENABLE 0

#if ULTRA_LOW_LATENCY_EN
#define TLKALG_LC3_PLUS_ENC_ENABLE 1
#define TLKALG_LC3_PLUS_DEC_ENABLE 1
#else
#define TLKALG_LC3_24BIT_ENC_ENABLE 1
#define TLKALG_LC3_24BIT_DEC_ENABLE 1
#endif

//asrc/ppm algorithm
#define TLKALG_PPM_MIC_ENABLE           1
#define TLKALG_ASRC_16TO48_16BIT_ENABLE 1
#define TLKALG_PPM_SPK_24BIT_ENABLE     1
#define TLKALG_TWS_PPM_SPK_ENABLE       1

#define TLKMDI_AUDMEM_TOTAL_SIZE        ((95 * 1024) + 256)

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A20_V1_0
#else
#error "config hardware !!!"
#endif


#include "vendor/common/default_config.h"
