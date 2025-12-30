/********************************************************************************************************
 * @file    default_config.h
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
#include "user_config.h"
#include "vendor/common/boards/board_config.h"

#ifndef TLK_CFG_RTOS_ENABLE
#define TLK_CFG_RTOS_ENABLE 0
#endif

#ifndef TLK_CFG_FS_ENABLE
#define TLK_CFG_FS_ENABLE 0
#endif

#ifndef TLK_CFG_FLASH_PROT_ENABLE
#define TLK_CFG_FLASH_PROT_ENABLE 0
#endif


#ifndef TLK_CFG_RF_SHARED
#define TLK_CFG_RF_SHARED 0
#endif
#ifndef TLK_CFG_AUDIO_CLOCK_LEVEL
#define TLK_CFG_AUDIO_CLOCK_LEVEL 3
#endif

#ifndef TLK_CFG_DYNAMIC_CLOCK_ENABLE
#define TLK_CFG_DYNAMIC_CLOCK_ENABLE 0
#endif

#ifndef TLK_CFG_N22_VCD_ENABLE
#define TLK_CFG_N22_VCD_ENABLE 0
#endif

#ifndef TLK_LOW_POWER_ENABLE
#define TLK_LOW_POWER_ENABLE 0
#endif

#ifndef TLK_CFG_PRODUCT_TEST_ENABLE
#define TLK_CFG_PRODUCT_TEST_ENABLE 0
#endif

#ifndef DEBUG_SUSPEND_ENABLE
#define DEBUG_SUSPEND_ENABLE 0
#endif

#ifndef TLK_CFG_UART2USBVCD_ENABLE
#define TLK_CFG_UART2USBVCD_ENABLE 0
#endif

/**
 *  currently, condor and pardus are not support PM sleep mode,like suspend mode, so disable it.
 *  TODO: add support for condor and pardus PM sleep mode.
 */
#if (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL753X)
#undef TLK_CFG_SUSPEND_ENABLE
#undef BLMS_PM_ENABLE
#define BLMS_PM_ENABLE         0
#define TLK_CFG_SUSPEND_ENABLE 0
#endif

#ifndef TLK_CFG_SUSPEND_ENABLE
#define TLK_CFG_SUSPEND_ENABLE 0
#endif

/** product  Information */
#ifndef ID_VENDOR
/** for report */
#define ID_VENDOR 0x248a
#endif

#ifndef ID_PRODUCT_BASE
#define ID_PRODUCT_BASE 0x8800
#endif

#ifndef STRING_VENDOR
#define STRING_VENDOR L"Telink"
#endif
#ifndef STRING_PRODUCT
#define STRING_PRODUCT L"2.4G Wireless Audio"
#endif

#if 0 //(MCU_CORE_TYPE == MCU_CORE_9518)
#ifndef STRING_SERIAL
#define STRING_SERIAL L"TLSR9518"
#endif
#else
#ifndef STRING_SERIAL
#define STRING_SERIAL L"TLSR9518"
#endif
#endif


#ifndef MIC_RESOLUTION_BIT
#define MIC_RESOLUTION_BIT 16
#endif


#ifndef MIC_SAMPLE_RATE
#define MIC_SAMPLE_RATE 16000
#endif


#ifndef TLK_DRV_SPI_ENABLE
#define TLK_DRV_SPI_ENABLE 0
#endif
/////////////////////////////// Add for app_system layer and bt host profile configuration //////////////////////////
///////////////////////////////                     Begin                         ////////////////////////////
#ifndef TLK_APP_VERSION
#define TLK_APP_VERSION 0x00000000 // Application,
#endif
#ifndef TLK_LIB_VERSION
#define TLK_LIB_VERSION 0x00000000 // Library,
#endif
#ifndef TLK_DRV_VERSION
#define TLK_DRV_VERSION 0x00000000 // Driver,
#endif

#ifndef TLK_PRT_VERSION
#define TLK_PRT_VERSION 0x0000 // Protocol,
#endif

///////////////////////// APP Configuration//////////////////////////////////////////////////

#ifndef TLK_CFG_WDG_ENABLE
#define TLK_CFG_WDG_ENABLE 0 // WatchDog
#endif

#ifndef TLK_CFG_ALG_ENABLE
#define TLK_CFG_ALG_ENABLE 0
#endif


#ifndef TLK_CFG_UART_TOOL_ENABLE
#define TLK_CFG_UART_TOOL_ENABLE 0
#endif

#ifndef TLK_DEV_KEY_ENABLE
#define TLK_DEV_KEY_ENABLE 0
#endif

#ifndef TLK_DEV_LED_ENABLE
#define TLK_DEV_LED_ENABLE 0
#endif

#ifndef TLK_DEV_MFI_ENABLE
#define TLK_DEV_MFI_ENABLE 0
#endif

#ifndef TLK_DEV_SY5500_ENABLE
#define TLK_DEV_SY5500_ENABLE 0
#endif

#ifndef TLK_DEV_RTC_ENABLE
#define TLK_DEV_RTC_ENABLE 0
#endif

#ifndef TLK_CFG_TONE_ENABLE
#define TLK_CFG_TONE_ENABLE 0
#endif

#ifndef TLK_MW_TPSLL_ENABLE
#define TLK_MW_TPSLL_ENABLE 0
#endif


#ifndef TLK_CFG_AUDIO_TEST
#define TLK_CFG_AUDIO_TEST 0
#endif

#ifndef TLK_CFG_BT_LE_LINK_NOT_COEX
#define TLK_CFG_BT_LE_LINK_NOT_COEX 0
#endif


#ifndef TLK_MW_TINYSQL_ENABLE
#define TLK_MW_TINYSQL_ENABLE 0
#endif

#ifndef TLK_MW_USER_CTRL_ENABLE
#define TLK_MW_USER_CTRL_ENABLE 0
#endif

#ifndef TLK_CFG_TPSLL_HCI_ENABLE
#define TLK_CFG_TPSLL_HCI_ENABLE 0
#endif
///////////////////////// BLE Application Configuration/////////////////////////////////////////////

#ifndef ACL_CENTRAL_SMP_ENABLE
#define ACL_CENTRAL_SMP_ENABLE 1
#endif

#ifndef ACL_PERIPHR_SMP_ENABLE
#define ACL_PERIPHR_SMP_ENABLE 1
#endif

///////////////////////// USB device Configuration/////////////////////////////////////////////

#ifndef TLK_USB_UAC_ENABLE
#define TLK_USB_UAC_ENABLE 0
#endif

#ifndef TLK_USB_MSC_ENABLE
#define TLK_USB_MSC_ENABLE 0
#endif

#if defined(MCU_CORE_N22)
#ifndef TLK_USB_UDB_ENABLE
#define TLK_USB_UDB_ENABLE 0
#endif
#else
#ifndef TLK_USB_UDB_ENABLE
#define TLK_USB_UDB_ENABLE 1
#endif
#endif

#ifndef TLK_CFG_USB_ENABLE
#define TLK_CFG_USB_ENABLE (TLK_USB_UDB_ENABLE || TLK_USB_UAC_ENABLE || TLK_USB_MSC_ENABLE)
#endif

///////////////////////// Debug Configuration//////////////////////////////////////////////////
#ifndef TLK_DEBUG_ENABLE
#define TLK_DEBUG_ENABLE (1)
#endif

#ifndef JTAG_DEBUG_ENABLE
#define JTAG_DEBUG_ENABLE 0
#endif

// USB Debug
#ifndef TLKDBG_CFG_UDB_LOG_ENABLE
#define TLKDBG_CFG_UDB_LOG_ENABLE (TLK_USB_UDB_ENABLE && TLK_DEBUG_ENABLE)
#endif

// Hardware Protocol UART
#ifndef TLKDBG_CFG_HPU_LOG_ENABLE
#define TLKDBG_CFG_HPU_LOG_ENABLE (1 && TLK_DEBUG_ENABLE && TLK_CFG_UART_TOOL_ENABLE)
#endif

// Hardware UART
#ifndef TLKDBG_CFG_HWU_LOG_ENABLE
#define TLKDBG_CFG_HWU_LOG_ENABLE (0 && TLK_DEBUG_ENABLE)
#endif

#ifndef TLK_SM_LOG_ENABLE
#define TLK_SM_LOG_ENABLE 0
#endif

#ifndef TLK_DEV_SERIAL_ENABLE
#define TLK_DEV_SERIAL_ENABLE (TLK_CFG_UART_TOOL_ENABLE || TLKDBG_CFG_HWU_LOG_ENABLE || TLK_DEV_SY5500_ENABLE)
#endif


///////////////////////// BT Stack Configuration//////////////////////////////////////////////////
#ifndef TLKCFG_BT_MAJOR_SERVICE_CLASS
#define TLKCFG_BT_MAJOR_SERVICE_CLASS (BTH_COD_SERVICE_CLASS_AUDIO | BTH_COD_SERVICE_CLASS_RENDER)
#endif

#ifndef TLKCFG_BT_MAJOR_DEVICE_CLASS
#define TLKCFG_BT_MAJOR_DEVICE_CLASS (BTH_COD_MAJOR_CLASS_AUDIO)
#endif

#ifndef TLKCFG_BT_MINOR_DEVICE_CLASS
#define TLKCFG_BT_MINOR_DEVICE_CLASS (BTH_COD_AUDIO_WEARABLE_HEADSET)
#endif

#ifndef TLKCFG_BT_MGR_DEVICE_CLASS
#if TLK_CFG_GAME_PAD_ENABLE
#define TLKCFG_BT_MGR_DEVICE_CLASS 0x2508
#else
#define TLKCFG_BT_MGR_DEVICE_CLASS (TLKCFG_BT_MAJOR_SERVICE_CLASS | TLKCFG_BT_MAJOR_DEVICE_CLASS | TLKCFG_BT_MINOR_DEVICE_CLASS) // 0x240404//0x5a020c
#endif
#endif

#ifndef TLK_STK_BT_ENABLE
#define TLK_STK_BT_ENABLE 0
#endif

#ifndef TLK_STK_TPD_ENABLE
#define TLK_STK_TPD_ENABLE 0
#endif

#ifndef TLK_STK_BT_TPSLL_ENABLE
#define TLK_STK_BT_TPSLL_ENABLE 0
#endif

#ifndef TLKSTK_BTTPSLL_TWS_ENABLE
#define TLKSTK_BTTPSLL_TWS_ENABLE 0
#endif

#ifndef TLKSTK_BT_TPS_ENABLE
#define TLKSTK_BT_TPS_ENABLE (TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE)
#endif

#ifndef TLKSTK_HOST_ENABLE
#define TLKSTK_HOST_ENABLE (TLK_STK_BT_ENABLE || TLKSTK_BT_TPS_ENABLE || TLK_STK_TPD_ENABLE || TLK_STK_BLE_ENABLE || TLK_STK_TPMD_ENABLE)
#endif

#ifndef TLK_STK_BTACL_NUMB
#define TLK_STK_BTACL_NUMB 1
#endif

#ifndef TLK_STK_BTSCO_NUMB
#define TLK_STK_BTSCO_NUMB 1
#endif

#ifndef TLK_STK_BTPSM_NUMB
#define TLK_STK_BTPSM_NUMB 12
#endif

#ifndef TLK_STK_BTCHN_NUMB
#define TLK_STK_BTCHN_NUMB (TLK_STK_BTACL_NUMB * 8)
#endif

#ifndef TLKMDI_BTRECON_RETRY_NUM_POWERON
#define TLKMDI_BTRECON_RETRY_NUM_POWERON 3
#endif

#ifndef TLKMDI_BTRECON_RETRY_NUM_LINK_LOSS
#define TLKMDI_BTRECON_RETRY_NUM_LINK_LOSS 25
#endif
//start BT host profile configuration
#ifndef TLKBTP_CFG_RFC_ENABLE
#define TLKBTP_CFG_RFC_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif

#ifndef TLKBTP_CFG_SPP_ENABLE
#define TLKBTP_CFG_SPP_ENABLE (0 && TLKBTP_CFG_RFC_ENABLE)
#endif
#ifndef TLKBTP_CFG_SPP_USER_ENABLE
#define TLKBTP_CFG_SPP_USER_ENABLE (0 && TLKBTP_CFG_SPP_ENABLE)
#endif

#ifndef TLKBTP_CFG_HFP_ENABLE
#define TLKBTP_CFG_HFP_ENABLE (0 && TLKBTP_CFG_RFC_ENABLE)
#endif
#ifndef TLKBTP_CFG_HFPHF_ENABLE
#define TLKBTP_CFG_HFPHF_ENABLE (0 && TLKBTP_CFG_HFP_ENABLE)
#endif
#ifndef TLKBTP_CFG_HFPAG_ENABLE
#define TLKBTP_CFG_HFPAG_ENABLE (0 && TLKBTP_CFG_HFP_ENABLE)
#endif

#ifndef TLKBTP_CFG_AVRCP_ENABLE
#define TLKBTP_CFG_AVRCP_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif
#ifndef TLKBTP_CFG_AVRCP_BROWSING_ENABLE
#define TLKBTP_CFG_AVRCP_BROWSING_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif

#ifndef TLKBTP_CFG_A2DP_ENABLE
#define TLKBTP_CFG_A2DP_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif
#ifndef TLKBTP_CFG_A2DPSRC_ENABLE
#define TLKBTP_CFG_A2DPSRC_ENABLE (0 && TLKBTP_CFG_A2DP_ENABLE)
#endif
#ifndef TLKBTP_CFG_A2DPSNK_ENABLE
#define TLKBTP_CFG_A2DPSNK_ENABLE (0 && TLKBTP_CFG_A2DP_ENABLE)
#endif

#ifndef TLKBTP_CFG_A2DP_FEATURE_CFG
#if (TLKBTP_CFG_A2DPSRC_ENABLE && TLKBTP_CFG_A2DPSNK_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_BASIC)
#endif
#if (TLKBTP_CFG_A2DPSRC_ENABLE && !TLKBTP_CFG_A2DPSNK_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_SRC_BASIC)
#endif
#if (!TLKBTP_CFG_A2DPSRC_ENABLE && TLKBTP_CFG_A2DPSNK_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG (BTP_A2DP_FEATURE_OPTION_SNK_BASIC)
#endif
#if (!TLKBTP_CFG_A2DPSRC_ENABLE && !TLKBTP_CFG_A2DPSNK_ENABLE)
#define TLKBTP_CFG_A2DP_FEATURE_CFG 0
#endif
#endif
#ifndef TLKBTP_A2DP_DEFAULT_VOL
#define TLKBTP_A2DP_DEFAULT_VOL 68 /* AVRCP vol step 9 valume,bt_music_vol_table_ios*/
#endif
#ifndef TLKBTP_HFP_DEFAULT_VOL
#define TLKBTP_HFP_DEFAULT_VOL 15
#endif
#ifndef TLKBTP_CFG_OBEX_ENABLE
#define TLKBTP_CFG_OBEX_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif
#ifndef TLKBTP_CFG_PBAP_ENABLE
#define TLKBTP_CFG_PBAP_ENABLE (0 && TLKBTP_CFG_OBEX_ENABLE)
#endif
#ifndef TLKBTP_CFG_PBAPCLT_ENABLE
#define TLKBTP_CFG_PBAPCLT_ENABLE (0 && TLKBTP_CFG_PBAP_ENABLE)
#endif
#ifndef TLKBTP_CFG_PBAPSRV_ENABLE
#define TLKBTP_CFG_PBAPSRV_ENABLE (0 && TLKBTP_CFG_PBAP_ENABLE)
#endif
#ifndef TLKBTP_CFG_MAP_ENABLE
#define TLKBTP_CFG_MAP_ENABLE (0 && TLKBTP_CFG_OBEX_ENABLE)
#endif

#ifndef TLKBTP_CFG_HID_ENABLE
#define TLKBTP_CFG_HID_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif

#ifndef TLKBTP_CFG_CUSTP_ENABLE
#define TLKBTP_CFG_CUSTP_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif

#ifndef TLKBTBLE_CFG_MICS_ENABLE
#define TLKBTBLE_CFG_MICS_ENABLE (0 && TLK_STK_BT_ENABLE && TLK_STK_BLE_ENABLE)
#endif

#ifndef TLKBTBLE_CFG_MCS_ENABLE
#define TLKBTBLE_CFG_MCS_ENABLE (0 && TLK_STK_BT_ENABLE && TLK_STK_BLE_ENABLE)
#endif

#ifndef TLKBTP_CFG_COVERART_ENABLE
#define TLKBTP_CFG_COVERART_ENABLE (0 && TLKBTP_CFG_OBEX_ENABLE)
#endif
#ifndef TLKBTP_CFG_COVERARTCLT_ENABLE
#define TLKBTP_CFG_COVERARTCLT_ENABLE (0 && TLKBTP_CFG_COVERART_ENABLE)
#endif
#ifndef TLKBTP_CFG_COVERARTSRV_ENABLE
#define TLKBTP_CFG_COVERARTSRV_ENABLE (0 && TLKBTP_CFG_COVERART_ENABLE)
#endif
#ifndef TLKBTP_CFG_AVRC_COVER_ART_ENABLE
#define TLKBTP_CFG_AVRC_COVER_ART_ENABLE (0 && TLKBTP_CFG_AVRCP_ENABLE && TLKBTP_CFG_COVERARTCLT_ENABLE)
#endif

#ifndef TLKBTP_CFG_ATT_ENABLE
#define TLKBTP_CFG_ATT_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif
#ifndef TLKBTP_CFG_ATTSRV_ENABLE
#define TLKBTP_CFG_ATTSRV_ENABLE (0 && LKBTP_CFG_ATT_ENABLE)
#endif
#ifndef TLKBTP_CFG_ATTCLT_ENABLE
#define TLKBTP_CFG_ATTCLT_ENABLE (0 && TLKBTP_CFG_ATT_ENABLE)
#endif

#ifndef TLKBTP_CFG_IAP_ENABLE
#define TLKBTP_CFG_IAP_ENABLE (0 && TLK_STK_BT_ENABLE)
#endif
//end BT host profile configuration

#ifndef TLKAPP_LEMGR_DONGLE_EXT_SCAN_ENABLE
#define TLKAPP_LEMGR_DONGLE_EXT_SCAN_ENABLE 0
#endif

///////////////////////// MW Configuration////////////////////////////////////////////////////

#ifndef TLK_MW_BTINQ_ENABLE
#define TLK_MW_BTINQ_ENABLE (TLKBTP_CFG_HFPAG_ENABLE || TLKBTP_CFG_A2DPSRC_ENABLE)
#endif

#ifndef TLK_MW_BTREC_ENABLE
#define TLK_MW_BTREC_ENABLE (TLKBTP_CFG_HFPHF_ENABLE || TLKBTP_CFG_A2DPSNK_ENABLE)
#endif

#ifndef TLK_MW_BTPAIRING_ENABLE
#define TLK_MW_BTPAIRING_ENABLE (TLKBTP_CFG_HFPHF_ENABLE || TLKBTP_CFG_A2DPSNK_ENABLE)
#endif

// LE Audio Unicast client
#ifndef TLK_MW_LEA_UC_VOICE_ENABLE
#define TLK_MW_LEA_UC_VOICE_ENABLE (0)
#endif

#ifndef TLK_MW_LEA_UC_MIC_ENABLE
#define TLK_MW_LEA_UC_MIC_ENABLE (0)
#endif

#ifndef TLK_MW_LEA_UC_MUSIC_ENABLE
#define TLK_MW_LEA_UC_MUSIC_ENABLE (0)
#endif

#ifndef TLK_MW_LEA_UC_ENABLE
#define TLK_MW_LEA_UC_ENABLE (TLK_MW_LEA_UC_VOICE_ENABLE || TLK_MW_LEA_UC_MIC_ENABLE || TLK_MW_LEA_UC_MUSIC_ENABLE)
#endif

// LE Audio Unicast server
// music function.
#ifndef TLK_MW_LEA_US_MUSIC_ENABLE
#define TLK_MW_LEA_US_MUSIC_ENABLE (0)
#endif

// voice function.
#ifndef TLK_MW_LEA_US_VOICE_ENABLE
#define TLK_MW_LEA_US_VOICE_ENABLE (0)
#endif

#define TLK_MW_LEA_US_ENABLE (TLK_MW_LEA_US_MUSIC_ENABLE || TLK_MW_LEA_US_VOICE_ENABLE)


// LE Audio BAP Broadcast Media Sender.(BAP Broadcast source)
#ifndef TLK_MW_LEA_BMS_ENABLE
#define TLK_MW_LEA_BMS_ENABLE (0)
#endif

// LE Audio BAP Broadcast Media Receiver.(BAP Broadcast sink)
#ifndef TLK_MW_LEA_BMR_ENABLE
#define TLK_MW_LEA_BMR_ENABLE (0)
#endif

// LE Audio A2DP to BIS, for BIS source(only send one audio stream).
#ifndef TLK_MW_LEA_A2DP_TO_BIS_ENABLE
#define TLK_MW_LEA_A2DP_TO_BIS_ENABLE (0)
#endif

#define TLK_MW_LE_AUDIO_ENABLE (TLK_MW_LEA_UC_ENABLE || TLK_MW_LEA_US_ENABLE || TLK_MW_LEA_BMS_ENABLE || TLK_MW_LEA_A2DP_TO_BIS_ENABLE || TLK_MW_LEA_BMR_ENABLE)

#ifndef TLK_MW_AUDIO_ENABLE
#define TLK_MW_AUDIO_ENABLE (TLK_CFG_TONE_ENABLE || TLKBTP_CFG_HFPHF_ENABLE || TLKBTP_CFG_A2DP_ENABLE || TLK_USB_UAC_ENABLE || TLK_MW_LE_AUDIO_ENABLE || TLKMW_RECORDING_CARD_EN)
#endif


///////////////////////// MMI Configuration////////////////////////////////////////////////////
#ifndef TLK_CFG_SYSTEM_ENABLE
#define TLK_CFG_SYSTEM_ENABLE (TLK_DEV_KEY_ENABLE || TLK_DEV_LED_ENABLE || TLK_CFG_USB_ENABLE || TLK_MW_TINYSQL_ENABLE || TLK_CFG_UART_TOOL_ENABLE)
#endif

#ifndef TLKAPP_LEA_US_STATE_CONTROL_ENABLE
#define TLKAPP_LEA_US_STATE_CONTROL_ENABLE (0 && TLK_MW_AUDIO_ENABLE)
#endif

#ifndef TLKAPP_LEMGR_ENABLE
#define TLKAPP_LEMGR_ENABLE (0)
#endif

#ifndef TLKAPP_LEMGR_EXT_SCAN_ENABLE
#define TLKAPP_LEMGR_EXT_SCAN_ENABLE (1 & TLKAPP_LEMGR_ENABLE)
#endif

///////////////////////// Codec Configuration//////////////////////////////////////////////////

#ifndef TLK_DEV_CODEC_ENABLE
#define TLK_DEV_CODEC_ENABLE 0
#endif

#ifndef TLK_DEV_CODEC_MIC_ENABLE
#define TLK_DEV_CODEC_MIC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#endif

#ifndef TLK_DEV_CODEC_SPK_ENABLE
#define TLK_DEV_CODEC_SPK_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#endif

#ifndef TLKDRV_CODEC_ICODEC_ENABLE
#define TLKDRV_CODEC_ICODEC_ENABLE (0 && TLK_DEV_CODEC_ENABLE)
#endif

#ifndef TLKDRV_CODEC_I2S_SLAVE_ENABLE
#define TLKDRV_CODEC_I2S_SLAVE_ENABLE (0 && TLK_DEV_CODEC_ENABLE)
#endif

#ifndef TLKDRV_CODEC_I2S_MASTER_ENABLE
#define TLKDRV_CODEC_I2S_MASTER_ENABLE (0 && TLK_DEV_CODEC_ENABLE)
#endif


#ifndef AUDIO_PATH_24BITS_EN
#define AUDIO_PATH_24BITS_EN 0
#endif

#ifndef TLKALG_ENC_ENABLE
#define TLKALG_ENC_ENABLE 0
#endif

#ifndef CODEC_DAC_MONO_MODE
#define CODEC_DAC_MONO_MODE 0
#endif

#ifndef TLK_ALG_EC_ENABLE
#define TLK_ALG_EC_ENABLE (0)
#endif

#ifndef TLK_ALG_HYBRID_ENABLE
#define TLK_ALG_HYBRID_ENABLE (0)
#endif

#ifndef GET_PCM_DATA_TICK_INDEX
#define GET_PCM_DATA_TICK_INDEX 0
#endif

#ifndef TLK_ALG_AEC_ENABLE
#define TLK_ALG_AEC_ENABLE (0)
#endif

#ifndef TLK_ALG_PPM_ENABLE
#define TLK_ALG_PPM_ENABLE (1)
#endif

#ifndef TLK_MW_LL_DONGLE_PHONE_ENABLE
#define TLK_MW_LL_DONGLE_PHONE_ENABLE 0
#endif

#ifndef TLK_MW_LL_DONGLE_MUSIC_ENABLE
#define TLK_MW_LL_DONGLE_MUSIC_ENABLE 0
#endif
/////////////////////////////// Add for app layer and bt host profile configuration //////////////////////////
///////////////////////////////                     end                         ////////////////////////////
///////////////////  Board configuration   /////////////////////////////////
#ifndef TLKHW_TYPE
#if (MCU_CORE_TYPE == MCU_CORE_B92)
#define TLKHW_TYPE TLKHW_TLSR9528A_EVK_C1T266A20
#elif (MCU_CORE_TYPE == MCU_CORE_B91)
#define TLKHW_TYPE TLKHW_TLSR9518A_EVK_C1T213A20
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define TLKHW_TYPE BOARD_721X_EVK_C1T315A20
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T360A20
#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)
//#define TLKHW_TYPE TLKHW_TL752X_50PIN//TLKHW_TL752X_96PIN//TLKHW_TL752X_50PIN

#define TLKHW_TYPE TLKHW_TL752X_96PIN //TLKHW_TL752X_96PIN//TLKHW_TL752X_50PIN
#else
#error "SDK do not support this MCU!"
#endif
#endif


/******************do not modify this file start****************************/
/*B92*/
#if (TLKHW_TYPE == TLKHW_TLSR9528A_EVK_C1T266A20)
#include "vendor/common/boards/B92_C1T266A20.h"
/*721X*/
#elif (TLKHW_TYPE == BOARD_721X_EVK_C1T315A20)
#include "vendor/common/boards/TL721X_C1T315A20.h"
#elif (TLKHW_TYPE == BOARD_721X_EVK_C1TXA104_V1_1)
#include "vendor/common/boards/TL721X_C1TXA104_V1_1.h"
/*751x*/
#elif (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0)
#include "vendor/common/boards/TL751X_C1T368A20_V1_0.h"
#elif (TLKHW_TYPE == TLKHW_TL3228_EVK_C1T371A20_V1_1)
#include "vendor/common/boards/TL3228_C1T371A20_V1_1.h"
/******************do not modify this file end****************************/

#else
#error "need board config"
#endif


#ifndef DEBUG_GPIO_ENABLE
#define DEBUG_GPIO_ENABLE 0
#endif


#if (DEBUG_GPIO_ENABLE)
#ifdef GPIO_CHN0
#define DBG_CHN0_LOW    gpio_write(GPIO_CHN0, 0)
#define DBG_CHN0_HIGH   gpio_write(GPIO_CHN0, 1)
#define DBG_CHN0_TOGGLE gpio_toggle(GPIO_CHN0)
#else
#define DBG_CHN0_LOW
#define DBG_CHN0_HIGH
#define DBG_CHN0_TOGGLE
#endif

#ifdef GPIO_CHN1
#define DBG_CHN1_LOW    gpio_write(GPIO_CHN1, 0)
#define DBG_CHN1_HIGH   gpio_write(GPIO_CHN1, 1)
#define DBG_CHN1_TOGGLE gpio_toggle(GPIO_CHN1)
#else
#define DBG_CHN1_LOW
#define DBG_CHN1_HIGH
#define DBG_CHN1_TOGGLE
#endif

#ifdef GPIO_CHN2
#define DBG_CHN2_LOW    gpio_write(GPIO_CHN2, 0)
#define DBG_CHN2_HIGH   gpio_write(GPIO_CHN2, 1)
#define DBG_CHN2_TOGGLE gpio_toggle(GPIO_CHN2)
#else
#define DBG_CHN2_LOW
#define DBG_CHN2_HIGH
#define DBG_CHN2_TOGGLE
#endif

#ifdef GPIO_CHN3
#define DBG_CHN3_LOW    gpio_write(GPIO_CHN3, 0)
#define DBG_CHN3_HIGH   gpio_write(GPIO_CHN3, 1)
#define DBG_CHN3_TOGGLE gpio_toggle(GPIO_CHN3)
#else
#define DBG_CHN3_LOW
#define DBG_CHN3_HIGH
#define DBG_CHN3_TOGGLE
#endif

#ifdef GPIO_CHN4
#define DBG_CHN4_LOW    gpio_write(GPIO_CHN4, 0)
#define DBG_CHN4_HIGH   gpio_write(GPIO_CHN4, 1)
#define DBG_CHN4_TOGGLE gpio_toggle(GPIO_CHN4)
#else
#define DBG_CHN4_LOW
#define DBG_CHN4_HIGH
#define DBG_CHN4_TOGGLE
#endif

#ifdef GPIO_CHN5
#define DBG_CHN5_LOW    gpio_write(GPIO_CHN5, 0)
#define DBG_CHN5_HIGH   gpio_write(GPIO_CHN5, 1)
#define DBG_CHN5_TOGGLE gpio_toggle(GPIO_CHN5)
#else
#define DBG_CHN5_LOW
#define DBG_CHN5_HIGH
#define DBG_CHN5_TOGGLE
#endif

#ifdef GPIO_CHN6
#define DBG_CHN6_LOW    gpio_write(GPIO_CHN6, 0)
#define DBG_CHN6_HIGH   gpio_write(GPIO_CHN6, 1)
#define DBG_CHN6_TOGGLE gpio_toggle(GPIO_CHN6)
#else
#define DBG_CHN6_LOW
#define DBG_CHN6_HIGH
#define DBG_CHN6_TOGGLE
#endif

#ifdef GPIO_CHN7
#define DBG_CHN7_LOW    gpio_write(GPIO_CHN7, 0)
#define DBG_CHN7_HIGH   gpio_write(GPIO_CHN7, 1)
#define DBG_CHN7_TOGGLE gpio_toggle(GPIO_CHN7)
#else
#define DBG_CHN7_LOW
#define DBG_CHN7_HIGH
#define DBG_CHN7_TOGGLE
#endif

#ifdef GPIO_CHN8
#define DBG_CHN8_LOW    gpio_write(GPIO_CHN8, 0)
#define DBG_CHN8_HIGH   gpio_write(GPIO_CHN8, 1)
#define DBG_CHN8_TOGGLE gpio_toggle(GPIO_CHN8)
#else
#define DBG_CHN8_LOW
#define DBG_CHN8_HIGH
#define DBG_CHN8_TOGGLE
#endif

#ifdef GPIO_CHN9
#define DBG_CHN9_LOW    gpio_write(GPIO_CHN9, 0)
#define DBG_CHN9_HIGH   gpio_write(GPIO_CHN9, 1)
#define DBG_CHN9_TOGGLE gpio_toggle(GPIO_CHN9)
#else
#define DBG_CHN9_LOW
#define DBG_CHN9_HIGH
#define DBG_CHN9_TOGGLE
#endif

#ifdef GPIO_CHN10
#define DBG_CHN10_LOW    gpio_write(GPIO_CHN10, 0)
#define DBG_CHN10_HIGH   gpio_write(GPIO_CHN10, 1)
#define DBG_CHN10_TOGGLE gpio_toggle(GPIO_CHN10)
#else
#define DBG_CHN10_LOW
#define DBG_CHN10_HIGH
#define DBG_CHN10_TOGGLE
#endif

#ifdef GPIO_CHN11
#define DBG_CHN11_LOW    gpio_write(GPIO_CHN11, 0)
#define DBG_CHN11_HIGH   gpio_write(GPIO_CHN11, 1)
#define DBG_CHN11_TOGGLE gpio_toggle(GPIO_CHN11)
#else
#define DBG_CHN11_LOW
#define DBG_CHN11_HIGH
#define DBG_CHN11_TOGGLE
#endif

#ifdef GPIO_CHN12
#define DBG_CHN12_LOW    gpio_write(GPIO_CHN12, 0)
#define DBG_CHN12_HIGH   gpio_write(GPIO_CHN12, 1)
#define DBG_CHN12_TOGGLE gpio_toggle(GPIO_CHN12)
#else
#define DBG_CHN12_LOW
#define DBG_CHN12_HIGH
#define DBG_CHN12_TOGGLE
#endif

#ifdef GPIO_CHN13
#define DBG_CHN13_LOW    gpio_write(GPIO_CHN13, 0)
#define DBG_CHN13_HIGH   gpio_write(GPIO_CHN13, 1)
#define DBG_CHN13_TOGGLE gpio_toggle(GPIO_CHN13)
#else
#define DBG_CHN13_LOW
#define DBG_CHN13_HIGH
#define DBG_CHN13_TOGGLE
#endif

#ifdef GPIO_CHN14
#define DBG_CHN14_LOW    gpio_write(GPIO_CHN14, 0)
#define DBG_CHN14_HIGH   gpio_write(GPIO_CHN14, 1)
#define DBG_CHN14_TOGGLE gpio_toggle(GPIO_CHN14)
#else
#define DBG_CHN14_LOW
#define DBG_CHN14_HIGH
#define DBG_CHN14_TOGGLE
#endif

#ifdef GPIO_CHN15
#define DBG_CHN15_LOW    gpio_write(GPIO_CHN15, 0)
#define DBG_CHN15_HIGH   gpio_write(GPIO_CHN15, 1)
#define DBG_CHN15_TOGGLE gpio_toggle(GPIO_CHN15)
#else
#define DBG_CHN15_LOW
#define DBG_CHN15_HIGH
#define DBG_CHN15_TOGGLE
#endif
#endif


#ifndef DBG_CHN0_LOW
#define DBG_CHN0_LOW
#endif

#ifndef DBG_CHN0_HIGH
#define DBG_CHN0_HIGH
#endif

#ifndef DBG_CHN0_TOGGLE
#define DBG_CHN0_TOGGLE
#endif


#ifndef DBG_CHN1_LOW
#define DBG_CHN1_LOW
#endif

#ifndef DBG_CHN1_HIGH
#define DBG_CHN1_HIGH
#endif

#ifndef DBG_CHN1_TOGGLE
#define DBG_CHN1_TOGGLE
#endif


#ifndef DBG_CHN2_LOW
#define DBG_CHN2_LOW
#endif

#ifndef DBG_CHN2_HIGH
#define DBG_CHN2_HIGH
#endif

#ifndef DBG_CHN2_TOGGLE
#define DBG_CHN2_TOGGLE
#endif


#ifndef DBG_CHN3_LOW
#define DBG_CHN3_LOW
#endif

#ifndef DBG_CHN3_HIGH
#define DBG_CHN3_HIGH
#endif

#ifndef DBG_CHN3_TOGGLE
#define DBG_CHN3_TOGGLE
#endif


#ifndef DBG_CHN4_LOW
#define DBG_CHN4_LOW
#endif

#ifndef DBG_CHN4_HIGH
#define DBG_CHN4_HIGH
#endif

#ifndef DBG_CHN4_TOGGLE
#define DBG_CHN4_TOGGLE
#endif


#ifndef DBG_CHN5_LOW
#define DBG_CHN5_LOW
#endif

#ifndef DBG_CHN5_HIGH
#define DBG_CHN5_HIGH
#endif

#ifndef DBG_CHN5_TOGGLE
#define DBG_CHN5_TOGGLE
#endif


#ifndef DBG_CHN6_LOW
#define DBG_CHN6_LOW
#endif

#ifndef DBG_CHN6_HIGH
#define DBG_CHN6_HIGH
#endif

#ifndef DBG_CHN6_TOGGLE
#define DBG_CHN6_TOGGLE
#endif


#ifndef DBG_CHN7_LOW
#define DBG_CHN7_LOW
#endif

#ifndef DBG_CHN7_HIGH
#define DBG_CHN7_HIGH
#endif

#ifndef DBG_CHN7_TOGGLE
#define DBG_CHN7_TOGGLE
#endif


#ifndef DBG_CHN8_LOW
#define DBG_CHN8_LOW
#endif

#ifndef DBG_CHN8_HIGH
#define DBG_CHN8_HIGH
#endif

#ifndef DBG_CHN8_TOGGLE
#define DBG_CHN8_TOGGLE
#endif


#ifndef DBG_CHN9_LOW
#define DBG_CHN9_LOW
#endif

#ifndef DBG_CHN9_HIGH
#define DBG_CHN9_HIGH
#endif

#ifndef DBG_CHN9_TOGGLE
#define DBG_CHN9_TOGGLE
#endif


#ifndef DBG_CHN10_LOW
#define DBG_CHN10_LOW
#endif

#ifndef DBG_CHN10_HIGH
#define DBG_CHN10_HIGH
#endif

#ifndef DBG_CHN10_TOGGLE
#define DBG_CHN10_TOGGLE
#endif


#ifndef DBG_CHN11_LOW
#define DBG_CHN11_LOW
#endif

#ifndef DBG_CHN11_HIGH
#define DBG_CHN11_HIGH
#endif

#ifndef DBG_CHN11_TOGGLE
#define DBG_CHN11_TOGGLE
#endif


#ifndef DBG_CHN12_LOW
#define DBG_CHN12_LOW
#endif

#ifndef DBG_CHN12_HIGH
#define DBG_CHN12_HIGH
#endif

#ifndef DBG_CHN12_TOGGLE
#define DBG_CHN12_TOGGLE
#endif


#ifndef DBG_CHN13_LOW
#define DBG_CHN13_LOW
#endif

#ifndef DBG_CHN13_HIGH
#define DBG_CHN13_HIGH
#endif

#ifndef DBG_CHN13_TOGGLE
#define DBG_CHN13_TOGGLE
#endif


#ifndef DBG_CHN14_LOW
#define DBG_CHN14_LOW
#endif

#ifndef DBG_CHN14_HIGH
#define DBG_CHN14_HIGH
#endif

#ifndef DBG_CHN14_TOGGLE
#define DBG_CHN14_TOGGLE
#endif


#ifndef DBG_CHN15_LOW
#define DBG_CHN15_LOW
#endif

#ifndef DBG_CHN15_HIGH
#define DBG_CHN15_HIGH
#endif

#ifndef DBG_CHN15_TOGGLE
#define DBG_CHN15_TOGGLE
#endif
