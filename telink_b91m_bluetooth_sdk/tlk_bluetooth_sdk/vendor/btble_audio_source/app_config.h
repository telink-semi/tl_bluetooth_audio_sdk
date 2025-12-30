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

#define TLK_CFG_FLASH_PROT_ENABLE		0

// LE Audio Music.
#define TLK_MW_LEA_UC_MUSIC_ENABLE (1)
// MIC function.
#define TLK_MW_LEA_UC_MIC_ENABLE (1)
// voice function.
#define TLK_MW_LEA_UC_VOICE_ENABLE (1)

#define TLKAPP_LEMGR_ENABLE        (1)

#define TLK_CFG_SUSPEND_ENABLE     0 //1: enable suspend


// Class of device
#define TLKCFG_BT_MAJOR_SERVICE_CLASS (BTH_COD_SERVICE_CLASS_AUDIO | BTH_COD_SERVICE_CLASS_RENDER | BTH_COD_SERVICE_CLASS_LE_AUDIO)
#define TLKCFG_BT_MGR_DEVICE_CLASS    (TLKCFG_BT_MAJOR_SERVICE_CLASS | BTH_COD_MAJOR_CLASS_AUDIO | BTH_COD_AUDIO_WEARABLE_HEADSET) // 0x244404

///////////////////////// System -app Configuration start/////////////////////////////////////////////////

#define TLK_CFG_RTOS_ENABLE 0
#define APP_LOG_EN          1
// APP System Configuration//
#define TLK_DEV_KEY_ENABLE                 1
#define TLK_DEV_LED_ENABLE                 1
#define TLK_MW_TINYSQL_ENABLE              1
#define TLK_CFG_TONE_ENABLE                0
#define TLK_CFG_UART_TOOL_ENABLE           1

#define TLK_MW_BTPAIRING_ENABLE            (1)
#define TLK_MW_BTREC_ENABLE                (1)

#define TLK_CFG_ROLE_PRECEDENCE_ROLE       (0) //BTH_ROLE_MASTER
#define TLK_CFG_CLASS_FILTER_FOR_UART_TOOL (1)
//BT Stack Configuration//
#define TLK_STK_BT_ENABLE         1
#define TLK_STK_BTACL_NUMB        2 //Number of supported bt acl links

#define TLKBTP_CFG_A2DP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSRC_ENABLE (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_AVRCP_ENABLE   (1 && TLK_STK_BT_ENABLE)

#if (TLK_STK_BTACL_NUMB > 1)
#define TLKBTP_CFG_HID_ENABLE (1 && TLK_STK_BT_ENABLE)
#endif


//BLE Stack Configuration//
#define TLK_STK_BLE_ENABLE        1

#define BLE_DONGLE_CONTROLLER_EN    1


/*Panther's codec do not support 32k.TODO: Junyuan.*/
#define BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM     1

/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE                   1
#define TLKALG_SBC_ENC_ENABLE                   1
#define TLKALG_MSBC_ENABLE                      1

//asrc/ppm algorithm
#if (MCU_CORE_TYPE != MCU_CORE_TL752X)
#define TLKALG_CVSD_ENABLE                      1
#define TLKALG_AAC_DEC_ENABLE                   1

#define TLKALG_PPM_SPK_ENABLE                   1
#define TLKALG_PPM_MIC_ENABLE                   1
#endif

#define TLKALG_ASRC_16BIT_ENABLE                1
#define TLKALG_ASRC_16TO48_16BIT_ENABLE         1
#define TLKALG_ASRC_48TO16_16BIT_ENABLE         1

#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
#define TLKALG_ASRC_48TO32_16BIT_ENABLE         1
#define TLKALG_ASRC_32TO48_16BIT_ENABLE         1
#define TLKALG_ASRC_32TO16_16BIT_ENABLE         1
#endif

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)
#define TLKHW_TYPE TLKHW_TLSR9528A_EVK_C1T266A20
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A20_V1_0
#else
#error "config hardware !!!"
#endif

/**
 *  @brief  DEBUG  Configuration
 */
/* print log */
#define APP_PAIR_LOG_EN                        1
#define APP_CIS_LOG_EN                         1
#define APP_SIMPLE_SDP_LOG_EN                  1
#define BLE_AUTOMATIC_HANDLE_ASYMMETRIC_CODECS 1
/////////////////////// *********  Below : difference between  single-core chip and  dual-core chip //////////////////////////////

#if (MCU_CORE_TYPE == MCU_CORE_B92)
#include "app_src_uac.h"

#define VCD_DEFINE_SELECT VCD_DEFINE_BTBLE_HEADSET
//Remove when file merge to SDK end//

// #define TLK_SIMPLE_PPM_EN        0
#define TLK_SPI_DEBUG_ENABLE 0

#elif (MCU_DUAL_CORE_ENABLE)

#include "app_src_uac.h"


#define VCD_DEFINE_SELECT           VCD_DEFINE_BTTPSLL_TL751X

#endif
#include "vendor/common/default_config.h"
