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

#if (MCU_DUAL_CORE_ENABLE)
#define TLK_CFG_RTOS_ENABLE           1
#endif

#define TLK_CFG_FLASH_PROT_ENABLE		0

/********************  lynx adapt start *********************/
// 1. Lynx haven't LDO power mode. 
// 2. Lynx flash size is 4M.
// 3. "include_path_lynx" delete.
// 4. check all UART_0 1 used location!!!! ZEWEN
// ALL macro will delete when drv is ready.
#if (MCU_CORE_TL752X_TEMP)
#define TLK_CFG_TEMP_DRAM_OPTM			1
#else
#define TLK_CFG_TEMP_DRAM_OPTM			0
#endif
/********************  lynx adapt end  **********************/

#define TLK_CFG_SUSPEND_ENABLE   0 //1: enable suspend 

#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
#define TLK_USB_UDB_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
#define TLK_CFG_USB_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
#if MCU_CORE_TL752X_TEMP
#define TLK_DEV_MFI_ENABLE       0
#else
#define TLK_DEV_MFI_ENABLE       1
#endif
#define TLK_CFG_TONE_ENABLE      1
#define TLK_CFG_UART_TOOL_ENABLE 1

#define TLK_USB_UAC_ENABLE         	0
#define TLK_USB_UAC_PLAYBACK_ENABLE (0 & TLK_USB_UAC_ENABLE)

#define TLK_STK_BLE_ENABLE  1
#define TLK_STK_BLE_HID_ENABLE 1

#if MCU_CORE_TL752X_TEMP
#define TLK_BT_MULTIPNT_ENABLE     (0)
#else
#define TLK_BT_MULTIPNT_ENABLE     (1)
#endif

#define TLK_MW_TINYSQL_ENABLE    1
#if(TLK_BT_MULTIPNT_ENABLE)
#define TLK_STK_BTACL_NUMB 2
#endif

#if MCU_CORE_TL752X_TEMP
#define TLK_MW_BTINQ_ENABLE		1
#endif

#define TLK_MW_USER_CTRL_ENABLE       0


//BT Stack Configuration//
#define TLK_STK_BT_ENABLE         1
#define TLKBTP_CFG_RFC_ENABLE     (1 && TLK_STK_BT_ENABLE)

#define TLKBTP_CFG_SPP_ENABLE	  (0 && TLKBTP_CFG_RFC_ENABLE)

#define TLKBTP_CFG_HFP_ENABLE     (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE   (1 && TLKBTP_CFG_HFP_ENABLE)

#define TLKBTP_CFG_A2DP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE (1 && TLKBTP_CFG_A2DP_ENABLE)

#if MCU_CORE_TL752X_TEMP
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_SNK_BASIC)
#else
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_SNK_AAC)
#endif

#define TLKBTP_CFG_AVRCP_ENABLE   (1 && TLK_STK_BT_ENABLE)

#define TLKBTP_CFG_ATT_ENABLE     (0 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_ATTSRV_ENABLE  (0 && TLKBTP_CFG_ATT_ENABLE)

#if MCU_CORE_TL752X_TEMP
#if (!TLK_CFG_TEMP_DRAM_OPTM)
#define TLKBTP_CFG_IAP_ENABLE     (0 && TLK_STK_BT_ENABLE)
#else
#define TLKBTP_CFG_IAP_ENABLE     (0 && TLK_STK_BT_ENABLE)
#define LEA_CODEC_NO_ASRC			1
#endif
#else
#define TLKBTP_CFG_IAP_ENABLE     (0 && TLK_STK_BT_ENABLE)
#endif

//BLE Stack Configuration//
// LE Audio unicast server configuration, music and voice enable
#if (TLK_STK_BLE_ENABLE)
#if (!TLK_CFG_TEMP_DRAM_OPTM)
#define TLK_MW_LEA_US_MUSIC_ENABLE   1
#define TLK_MW_LEA_US_VOICE_ENABLE   1
#define CODEC_MIC_FIFO_SAMPLES          2048
#endif
#define LE_AUDIO_CODEC_INPUT_TYPE    LE_AUDIO_CODEC_TYPE_CODEC
#define LE_AUDIO_CODEC_OUTPUT_TYPE   LE_AUDIO_CODEC_TYPE_CODEC
#define APP_AUDIO_ASCSS_SINK_ASE_CNT 1
#define APP_AUDIO_ASCSS_SRC_ASE_CNT  1
#define SVC_DEFAULT_KEYBOARD_ENABLE  1
#endif

// Class of device
#define TLKCFG_BT_MAJOR_SERVICE_CLASS (BTH_COD_SERVICE_CLASS_AUDIO | BTH_COD_SERVICE_CLASS_RENDER | BTH_COD_SERVICE_CLASS_LE_AUDIO)
#define TLKCFG_BT_MGR_DEVICE_CLASS (TLKCFG_BT_MAJOR_SERVICE_CLASS | BTH_COD_MAJOR_CLASS_AUDIO | BTH_COD_AUDIO_WEARABLE_HEADSET) // 0x244404

//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE       1

#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)

#define AUDIO_CODEC_LOOPBACK       0 ///default 0; used for codec hardware test

#define TLK_SPI_DEBUG_ENABLE       0

#define APP_LOG_EN                 1


/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_SBC_DEC_ENABLE                   1
#define TLKALG_SBC_ENC_ENABLE                   1
#define TLKALG_MSBC_ENABLE                      1
#define TLKALG_CVSD_ENABLE                      1
#define TLKALG_AAC_DEC_ENABLE                   1

//asrc/ppm algorithm
#if (CHIP_TYPE == CHIP_TYPE_B92 || CHIP_TYPE == CHIP_TYPE_TL752X)
#define TLKALG_PPM_SPK_ENABLE				    1
#define TLKALG_PPM_MIC_ENABLE				    1
#endif

#if (!TLK_CFG_TEMP_DRAM_OPTM)
	#define TLKMDI_AUDMEM_TOTAL_SIZE ((71 * 1024) + 256)
#else
	#define TLKMDI_AUDMEM_TOTAL_SIZE (35 * 1024)
#endif

#define TLK_AUDIO_DSP_BYPASS_TEST_ENABLE         0//TLK_MW_DSP_COMM_ENABLE

#if(MCU_CORE_TYPE == MCU_CORE_TL752X && TLK_AUDIO_DSP_BYPASS_TEST_ENABLE)
#define TLK_MW_DSP_COMM_ENABLE      1
#endif

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)
#define TLKHW_TYPE TLKHW_TLSR9528A_EVK_C1T266A20
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE        TLKHW_TL751X_EVK_C1T368A20_V1_0

#else

#error "config hardware !!!"
#endif

#if (TLK_STK_BLE_HID_ENABLE)
#include "stack/ble/host_v1/services/svc_hid/hid_demo/keyboard_cfg.h"
#endif

/////////////////////// *********  Below : difference between  single-core chip and  dual-core chip //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)

#define BLE_HEADSET_CONTROLLER_EN	1


#if (TLK_STK_BLE_ENABLE)
#define ACL_CENTRAL_MAX_NUM          0 // BLE ACL central maximum number
#define ACL_PERIPHR_MAX_NUM          1 // BLE ACL peripheral maximum number
#define APP_SCAN_ENABLE              1
#endif

#define VCD_DEFINE_SELECT        VCD_DEFINE_BTBLE_HEADSET
//Remove when file merge to SDK end//


#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)

#define TLK_MW_DSP_COMM_ENABLE      0

#if !MCU_CORE_TL752X_TEMP
#define VCD_DEFINE_SELECT VCD_DEFINE_BTTPSLL_TL751X
#endif
#define TLK_PCM_DATA_WR_EN       1

#endif
#include "vendor/common/default_config.h"
