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

#define TLK_CFG_FLASH_PROT_ENABLE		0

#define TLK_CFG_SUSPEND_ENABLE   0 //1: enable suspend

#define TLK_CFG_RTOS_ENABLE      0

// APP System Configuration//
#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       1
#define TLK_MW_TINYSQL_ENABLE    1
#define TLK_CFG_TONE_ENABLE      1
#if (MCU_CORE_TYPE != MCU_CORE_TL751X)
#define AUDIO_HD_HAC_MODE        0//disable
#endif

#if (TLK_CFG_SUSPEND_ENABLE)
    #define TLK_DEBUG_ENABLE         1
    #define TLK_CFG_USB_ENABLE       0
    #define TLK_USB_UDB_ENABLE       0
	#define TLK_CFG_UART_TOOL_ENABLE 1
#else
    #define TLK_DEBUG_ENABLE         1
    #define TLK_CFG_USB_ENABLE       1
    #define TLK_USB_UDB_ENABLE       1
	#define TLK_CFG_UART_TOOL_ENABLE 1 //debug use uart, disable usb
#endif

#define TLK_STK_BLE_ENABLE  1

#define TLK_STK_BTACL_NUMB 1

#define TLK_CFG_A2DP_TO_BIS_ENABLE 1
#define APP_UI_ENABLE              1
#define APP_UI_LOG_ENABLE          1

//BT Stack Configuration//
#define TLK_STK_BT_ENABLE         1
#define TLKBTP_CFG_A2DP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG BTP_A2DP_FEATURE_OPTION_SNK_BASIC

#define TLKBTP_CFG_AVRCP_ENABLE   (1 && TLK_STK_BT_ENABLE)

#if MCU_CORE_TL752X_TEMP
#define TLKSTK_BT_HOST_MEM_POOL_SIZE (8 * 1024)
#endif


//BLE Stack Configuration//
// LE Audio unicast server configuration, music and voice enable

//BLE Stack Configuration//
#define TLK_STK_BLE_ENABLE			  1

#define APP_PERIODIC_ADV_SETS_NUMBER  1
#define APP_PERIODIC_ADV_DATA_LENGTH  200

#define BIG_BCST_NUMBER               1
#define BIS_NUM_IN_PER_BIG_BCST       2

#define TLK_MW_LEA_BMR_ENABLE         1
#define LE_AUDIO_CODEC_OUTPUT_TYPE    LE_AUDIO_CODEC_TYPE_CODEC
#define TLK_MW_LEA_A2DP_TO_BIS_ENABLE 1

#define BIS_STREAM_48kHz_96kbps       1
#define BIS_STREAM_48kHz_80kbps       2
#define BIS_STREAM_32KHz_64Kbps       3
#define BIS_STREAM_24KHz_48Kbps       4
#define BIS_STREAM_16KHz_32Kbps       5


// Class of device
#define TLKCFG_BT_MAJOR_SERVICE_CLASS (BTH_COD_SERVICE_CLASS_AUDIO | BTH_COD_SERVICE_CLASS_RENDER | BTH_COD_SERVICE_CLASS_LE_AUDIO)
#define TLKCFG_BT_MGR_DEVICE_CLASS (TLKCFG_BT_MAJOR_SERVICE_CLASS | BTH_COD_MAJOR_CLASS_AUDIO | BTH_COD_AUDIO_WEARABLE_HEADSET) // 0x244404

#define APP_PERIODIC_ADV_ENABLE       1
#define APP_BIG_ENABLE                1


//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE       1

#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#define CODEC_MIC_FIFO_SAMPLES     0

#define TLK_SPI_DEBUG_ENABLE        0
#define APP_LOG_EN                  1


/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE                   1
#define TLKALG_SBC_ENC_ENABLE                   0
#define TLKALG_MSBC_ENABLE                      0
#define TLKALG_CVSD_ENABLE                      0
#define TLKALG_AAC_DEC_ENABLE                   1


//asrc/ppm algorithm
#define TLKALG_PPM_SPK_ENABLE			        1
#define TLKALG_PPM_MIC_ENABLE                   0

#if (MCU_CORE_TYPE == MCU_CORE_B92)
#define TLKALG_ASRC_441TO16_16BIT_ENABLE        1
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKALG_LC3_24BIT_ENC_ENABLE             0
#define TLKALG_ASRC_441TO48_16BIT_ENABLE        0
#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)
#define TLKALG_ASRC_441TO48_16BIT_ENABLE        1
#endif

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE)
    #define TLKMDI_AUDMEM_TOTAL_SIZE (72 * 1024)
#else
    #define TLKMDI_AUDMEM_TOTAL_SIZE (64 * 1024)
#endif

/////////////////////// Board Select Configuration //////////////////////////////

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE          TLKHW_TL751X_EVK_C1T368A20_V1_0
#else
#error "config hardware !!!"
#endif

#define LL_FEATURE_SUPPORT_PHY_TEST_MODE        0

/////////////////////// *********  Below : difference between  single-core chip and  dual-core chip //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)

#define BIS_SOURCE_SIMPLE_MODE          1
#define BLE_A2DP_TO_BIS_CONTROLLER_EN   1

#define BIS_STREAM_CONFIG             BIS_STREAM_16KHz_32Kbps
#if (TLK_STK_BLE_ENABLE)
#define ACL_CENTRAL_MAX_NUM          0 // BLE ACL central maximum number
#define ACL_PERIPHR_MAX_NUM          1 // BLE ACL peripheral maximum number
#endif

//Remove when file merge to SDK end//


#elif (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL752X)

#define BIS_SOURCE_SIMPLE_MODE          0

#define BIS_STREAM_CONFIG             BIS_STREAM_48kHz_96kbps

//TODO: just for function test, need to remove later.


#endif
#include "vendor/common/default_config.h"
