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

#define TLK_CFG_PRODUCT_TEST_ENABLE 1

#define TLK_DEV_KEY_ENABLE          1
#define TLK_DEV_LED_ENABLE          1
#define TLK_MW_TINYSQL_ENABLE       1
#define TLK_CFG_TONE_ENABLE         1
#define TLK_CFG_UART_TOOL_ENABLE    1

#define TLK_MW_BTINQ_ENABLE         1
#define TLK_STK_BLE_ENABLE          1

#define TLK_BT_MULTIPNT_ENABLE      (1)
#if (TLK_BT_MULTIPNT_ENABLE)
#define TLK_STK_BTACL_NUMB 2
#define TLK_STK_BTSCO_NUMB 2
#endif

#define TLK_CHECK_REMOTE_DEV     1
#define TLK_INTERPHONE_ENABLE    1
#define TLK_APP_REMOTE_NAME_DATA "X5 9TWMCC"
#define TLK_APP_REMOTE_NAME_LEN  2


#define TLK_MW_USER_CTRL_ENABLE  0
#define TLK_MW_OTA_ENABLE        (1 && TLK_MW_USER_CTRL_ENABLE)

//BT Stack Configuration//
#define TLK_STK_BT_ENABLE           1
#define TLKBTP_CFG_RFC_ENABLE       (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE       (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE     (1 && TLKBTP_CFG_HFP_ENABLE)
#define TLKBTP_CFG_HFPAG_ENABLE     (1 && TLKBTP_CFG_HFP_ENABLE)
#define TLKBTP_CFG_SPP_ENABLE       (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_SPP_USER_ENABLE  (0 && TLKBTP_CFG_SPP_ENABLE)

#define TLKBTP_CFG_A2DP_ENABLE      (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE   (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_A2DPSRC_ENABLE   (0 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_SNK_AAC | BTP_A2DP_FEATURE_OPTION_SRC_BASIC)

#define TLKBTP_CFG_AVRCP_ENABLE     (1 && TLK_STK_BT_ENABLE)


#define TLKBTP_CFG_ATT_ENABLE       (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_ATTSRV_ENABLE    (1 && TLKBTP_CFG_ATT_ENABLE)
//BLE Stack Configuration//
// LE Audio unicast server configuration, music and voice enable
#if (TLK_STK_BLE_ENABLE)
#define TLK_MW_LEA_US_MUSIC_ENABLE   1
#define TLK_MW_LEA_US_VOICE_ENABLE   1
#define LE_AUDIO_CODEC_INPUT_TYPE    LE_AUDIO_CODEC_TYPE_CODEC
#define LE_AUDIO_CODEC_OUTPUT_TYPE   LE_AUDIO_CODEC_TYPE_CODEC
#define APP_AUDIO_ASCSS_SINK_ASE_CNT 1
#define APP_AUDIO_ASCSS_SRC_ASE_CNT  1
#endif

//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE       1
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#define CODEC_MIC_FIFO_SAMPLES     1024
#define AUDIO_CODEC_LOOPBACK       0 ///default 0; used for codec hardware test

#define TLK_SPI_DEBUG_ENABLE       0

#define APP_LOG_EN                 1

/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE 1
#define TLKALG_SBC_ENC_ENABLE 1
#define TLKALG_MSBC_ENABLE    1
#define TLKALG_CVSD_ENABLE    1
#define TLKALG_AAC_DEC_ENABLE 1

//asrc/ppm algorithm
#define TLKALG_ASRC_16TO48_16BIT_ENABLE 1

//pcm data process algorithm
#define TLKALG_AGC_ENABLE        1
#define TLKALG_ANS_SPK_ENABLE    0

#define TLKMDI_AUDMEM_TOTAL_SIZE (126 * 1024)

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
//    #define TLKHW_TYPE        TLKHW_TL751X_EVK_C1T360A20
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A20_V1_0
// #define TLKHW_TYPE 			TLKHW_TL751X_EVK_C1T368A110_V1_0
#else
#error "config hardware !!!"
#endif

#define TLK_MW_DSP_COMM_ENABLE                 1
#define TLKMW_SIDETONE_EN                      1
#define TLKMW_INTERPHONE_EN                    1
#define TLKMW_FIFO_IRQ_EN                      1

#define TX_FIFO_IRQ_ENABLE                     1
#define INTERPHONE_I2S_FIFO                    FIFO3


#define TLK_PCM_DATA_WR_EN                     1

#define TLK_CFG_INTERPHONE_EXTRA_CONFIG_ENABLE 1

#if TLK_CFG_INTERPHONE_EXTRA_CONFIG_ENABLE
#include "vendor/bt_interphone/app_config_ex.h"
#endif

#include "vendor/common/default_config.h"
