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
#include "core/mcu_type.h"

//this low power used for release , will close all unused device
#define TLK_LOW_POWER_ENABLE 0
#define TLK_CFG_RTOS_ENABLE  1

/********************  lynx adapt start *********************/
// 1. Lynx haven't LDO power mode.
// 2. Lynx flash size is 4M.
// 3. "include_path_lynx" delete.
// 4. "compiler_pass_lynx" delete.
// ALL macro will delete when drv is ready.
#if (MCU_CORE_TL752X_TEMP)
#define TLK_CFG_TEMP_DRAM_OPTM       1
#define TLK_CFG_TEMP_DRAM_OPTM_TPSLL 1
#else
#define TLK_CFG_TEMP_DRAM_OPTM 0
#endif

/********************  lynx adapt end  **********************/

// APP System Configuration//

#define TLK_DEV_KEY_ENABLE 1
#if TLK_LOW_POWER_ENABLE

#define TLK_DEBUG_ENABLE         0
#define TLK_CFG_USB_ENABLE       0
#define TLK_USB_UDB_ENABLE       0
#define TLK_CFG_UART_TOOL_ENABLE 0
#else
#define TLK_DEV_LED_ENABLE       1

#define TLK_CFG_UART_TOOL_ENABLE 1
#endif

#define TLK_MW_TINYSQL_ENABLE   1
#define TLK_CFG_TONE_ENABLE     1

#define TLK_STK_BT_TPSLL_ENABLE 1

//BT Stack Configuration//
#define TLK_STK_BT_ENABLE           1
#define TLKBTP_CFG_RFC_ENABLE       (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE       (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE     (1 && TLKBTP_CFG_HFP_ENABLE)
#define TLKBTP_CFG_SPP_ENABLE       (0 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_SPP_USER_ENABLE  (0 && TLKBTP_CFG_SPP_ENABLE)

#define TLKBTP_CFG_A2DP_ENABLE      (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE   (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_BIT_SNK_SBC)

#define TLKBTP_CFG_AVRCP_ENABLE     (1 && TLK_STK_BT_ENABLE)


//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE       1
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#define AUDIO_CODEC_LOOPBACK       0 ///default 0; used for codec hardware test
#define CODEC_CUSTOMERS_SPEC_REQ   1 ///default 0;	1:enable  OUTPUT -3dB

#define TLK_MW_DSP_COMM_ENABLE     1

#if (!MCU_CORE_TL752X_TEMP) // compiler_pass_lynx
#define TLK_USB_UAC_ENABLE 1
#else
#define TLK_USB_UAC_ENABLE 0
#endif
#define TLK_USB_UAC_PLAYBACK_ENABLE (0 & TLK_USB_UAC_ENABLE)
#define TLK_USB_UAC_AUDIO_LOCAL_ENABLE \
    (0 && TLKDRV_CODEC_ICODEC_ENABLE && TLK_USB_UAC_ENABLE && (TLKBTP_CFG_A2DPSNK_ENABLE || TLK_MW_LEA_US_MUSIC_ENABLE || TLKBTP_CFG_HFPHF_ENABLE || TLK_MW_LEA_US_VOICE_ENABLE))
#define TLKUSB_AUD_SPK_SAMPLE_RATE_DEF 48000

#define TLK_PM_BUSY_CHN_USB            0xFF

#define TLKMW_FIFO_IRQ_EN              1

#define TLKALG_UAC_PPM_MIC_ENABLE      0
#define TLKALG_UAC_PPM_SPK_ENABLE      0
#define TLKALG_UAC_PPM_ENABLE          0

#define APP_USB_ISO_IN_BUFF_SIZE       (1024 * 1)
#define APP_USB_ISO_OUT_BUFF_SIZE      (1024 * 1)

#define TLKUSB_AUD_MIC_RESOLUTION_BIT  16
#define TLKUSB_AUD_SPK_RESOLUTION_BIT  24

#define AUDIO_PATH_24BITS_EN           1
#define TLK_SPI_DEBUG_ENABLE           0

#define ULTRA_LOW_LATENCY_EN           1

//DONGLE VOICE MIC DATA EN
#define DONGLE_VOICE_MIC_EN 1

//DSP NN BUFF EN
#define DSP_NN_NS_BUFF_DISENABLE 1

//TONE configuration
#define TONE_SYNC_PLAYBACK_MODE 0

////LOW latency mode configuration
//#define BT_MUSIC_LOW_LATENCY_MODE_EN   1

#define APP_LOG_EN                     1
#define VCD_DEFINE_SELECT              VCD_DEFINE_MINGQIAN
#define DEBUG_ZHAOWEI_GPIO_ENABLE_LYNX 0


//#define BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
#define TLK_CFG_TPSLL_HCI_ENABLE 1
#define TLK_STK_TPH_ENABLE       1

//#define TLK_AUDIO_TEST_MODE		0		///0:default; 1:Audio Tests
#if (!MCU_CORE_TL752X_TEMP) // compiler_pass_lynx
#define TLK_STK_BLE_ENABLE 0
#else
#define TLK_STK_BLE_ENABLE 0
#endif

#if TLK_STK_BLE_ENABLE
#define TLK_MW_LEA_US_MUSIC_ENABLE 1
#define TLK_MW_LEA_US_VOICE_ENABLE 1
#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_CODEC
#define LE_AUDIO_CODEC_OUTPUT_TYPE LE_AUDIO_CODEC_TYPE_CODEC
#endif

#define BT_TPSLL_OPTIMIZE_LATENCY_TEST 1

/*TODO: Congxiu,Yating. Need adapt 24bit ppm alg later.*/
/////////////////////// Algorithm Select Configuration //////////////////////////////
#define TLKALG_ALG_LOOPBACK_TEST_ENABLE 0
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE 1
#define TLKALG_SBC_ENC_ENABLE 1
#define TLKALG_MSBC_ENABLE    1
#define TLKALG_CVSD_ENABLE    1
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKALG_AAC_DEC_ENABLE 0

#if ULTRA_LOW_LATENCY_EN
#define TLKALG_LC3_PLUS_ENC_ENABLE 1
#define TLKALG_LC3_PLUS_DEC_ENABLE 1
#else
#define TLKALG_LC3_24BIT_ENC_ENABLE 1
#define TLKALG_LC3_24BIT_DEC_ENABLE 1
#endif

//asrc/ppm algorithm
#define TLKALG_ASRC_16TO48_16BIT_ENABLE 1
#define TLKALG_TWS_PPM_SPK_ENABLE       1
#define TLKALG_PPM_MIC_ENABLE           1
#define TLKALG_PPM_SPK_24BIT_ENABLE     1
//#define TLKALG_PPM_SPK_ENABLE                   1

#define TLKALG_EQ_TOOL_ENABLE 1
#endif

#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
#if TLKALG_AAC_DEC_ENABLE
#define TLKMDI_AUDMEM_TOTAL_SIZE ((110 * 1024) + 256)
#else
#define TLKMDI_AUDMEM_TOTAL_SIZE ((75 * 1024) + 256)
#endif
#else
#define TLKMDI_AUDMEM_TOTAL_SIZE (256)
#endif

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A20_V1_0
#else
#error "config hardware !!!"
#endif

#include "vendor/common/default_config.h"
