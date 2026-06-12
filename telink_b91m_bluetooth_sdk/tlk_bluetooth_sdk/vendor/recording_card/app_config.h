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

#define TLK_CFG_CUSTOMER_SDK_VERSION "telink_recording_card_sdk_V1.1.1"

#define TLK_CFG_RTOS_ENABLE          1

#define TLK_CFG_AUDIO_CLOCK_LEVEL    1 //When audio open,clock set to level 1 (64M)

#define TLK_CFG_DYNAMIC_CLOCK_ENABLE 1
#define TLK_CFG_SUSPEND_ENABLE       1

#define TLK_CFG_FS_ENABLE            1
#define TLK_USB_MSC_ENABLE           (1 && TLK_CFG_FS_ENABLE)
#define TLK_USB_MSC_READ_ONLY        (1 && TLK_USB_MSC_ENABLE)

//diskio select config
#define TLK_CFG_FS_DS35_DISKIO_ENABLE   0
#define TLK_CFG_FS_SDCARD_DISKIO_ENABLE 0 //only support TL751X

#if TLK_CFG_FS_SDCARD_DISKIO_ENABLE
#define TLKMW_FS_DISK_IO_SELECT           TLKMW_FS_DISK_IO_SDCARD
#define APP_RC_FILE_STREAM_WAV_CACHE_SIZE (30 * 1024)
#endif

#if TLK_CFG_FS_DS35_DISKIO_ENABLE
#define TLKMW_FS_DISK_IO_SELECT TLKMW_FS_DISK_IO_DS35X
#define TLK_CFG_FS_SECTOR_SIZE  2048
#endif

#define TLK_CFG_WIFI_LOGIC_ENABLE      1
#define TLK_DEV_SERIAL_ENABLE          (1 && TLK_CFG_WIFI_LOGIC_ENABLE)
#define TLK_CFG_RF_SHARED              (1 && TLK_CFG_WIFI_LOGIC_ENABLE)


#define TLK_STK_BLE_ENABLE             1

#define TLK_DEV_KEY_ENABLE             1
#define TLK_MW_TINYSQL_ENABLE          1
#define TLK_DEV_LED_ENABLE             1
#define TLK_DEV_RTC_ENABLE             (1)

#define TLK_DEV_CODEC_ENABLE           1
#define TLKDRV_CODEC_ICODEC_ENABLE     (1 && TLK_DEV_CODEC_ENABLE)
#define TLK_DEV_CODEC_SPK_ENABLE       (0 && TLKDRV_CODEC_ICODEC_ENABLE) //default not use speaker
#define TLKDRV_CODEC_COLD_START_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#define CODEC_MIC_FIFO_SAMPLES         1024
#define TLK_OLED_UI_EN                 0
#define APP_LOG_EN                     1


#define TLK_DEBUG_ENABLE               (1)
#define TLK_CFG_USB_ENABLE             (1)
#define TLK_USB_UDB_ENABLE             (1 && !TLK_CFG_SUSPEND_ENABLE)
#define TLKDBG_CFG_GSU_LOG_ENABLE      (0 && TLK_CFG_SUSPEND_ENABLE)
#if TLKDBG_CFG_GSU_LOG_ENABLE
#define TLKDBG_CFG_GSU_LOG_PIN GPIO_PA4
#endif


#define TLK_MW_USER_CTRL_ENABLE         (1)
#define TLK_MW_OTA_ENABLE               (1 && TLK_MW_USER_CTRL_ENABLE)

#define TLK_RC_CFG_AUTO_POWER_OFF       (1)

#define TLK_RC_CFG_BT_CENTRAL_STREAM    (0 && MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLK_RC_CFG_BT_PERIPHERAL_STREAM (0 && MCU_CORE_TYPE == MCU_CORE_TL751X)
#if TLK_RC_CFG_BT_CENTRAL_STREAM
#include "app_bt_central_config.h"
#endif
#if TLK_RC_CFG_BT_PERIPHERAL_STREAM
#include "app_bt_peripheral_config.h"
#endif


// remove later, disable other code.
#define TLKMW_FIFO_IRQ_EN       1
#define TLK_SPI_DEBUG_ENABLE    0
#define TLKMW_RECORDING_CARD_EN 1

/////////////////////// Algorithm Select Configuration //////////////////////////////
//encode/decode algorithm
#define TLKALG_OPUS_ENC_ENABLE 1
///Opening this macro will occupy part of the WIFI communication interface.
#define TLKALG_GET_AUDIO_DATA_EN 0 ///debug: OUTPUT data from SPI interface,

///disable and CHN for BBF
#define TLKALG_BBF_DIS    0
#define TLKALG_BBF_2CH_EN 2
#define TLKALG_BBF_4CH_EN 4 //Not supported for the TL721x hardware
#define TLKALG_BBF_6CH_EN 6 //Not supported for the TL721x hardware

//asrc/ppm algorithm
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
///VAD_NN
#define TLKALG_NN_NS_VAD_ENABLE 1
///AGC
#define TLKALG_AGC_ENABLE 1
///BBF
#define TLKALG_BBF_ENABLE          TLKALG_BBF_DIS       ///config dis or CHN
#define TLKALG_BBF_PATH_MODE       0x10                 ///0x10:MCU ALG 0x00:DSP ALG
#define TLKALG_BBF_PATH_SEL        TLKALG_BBF_PATH_MODE ///select alg path

#define TLKALG_AUD_SCRATCH_BUF_POS TLKALG_AUD_SCRATCH_BUF_USE_STACK

#define TLKMDI_AUDMEM_TOTAL_SIZE   ((24 * 1024) + (11 * 1024) * TLKALG_NN_NS_VAD_ENABLE + (19 * 1024) * TLKALG_AGC_ENABLE)

#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
///BBF
#define TLKALG_BBF_ENABLE    TLKALG_BBF_DIS       ///config dis or CHN
#define TLKALG_BBF_PATH_MODE 0x00                 ///0x10:MCU ALG 0x00:DSP ALG
#define TLKALG_BBF_PATH_SEL  TLKALG_BBF_PATH_MODE ///select alg path

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)

#define DIGITAL_BONE_MIC_DIGITAL_GAIN AUDIO_IN_D_GAIN_10_DB ///digital bone gain setting
#define ANALOG_BONE_MIC_DIGITAL_GAIN  AUDIO_IN_D_GAIN_6_DB  ///analog bine gain setting

#define DIGITAL_BONE_MIC              1
#define ANALOG_BONE_MIC               2
#define BONE_TYPE_SEL                 DIGITAL_BONE_MIC         ///bone type select: DIGITAL_BONE_MIC / ANALOG_BONE_MIC
#define TLKALG_BONE_CODUCTION_EN      1                        ///Only six MICs are supported
#define TLKALG_EQ_ENABLE              TLKALG_BONE_CODUCTION_EN ///Correction of the bone frequency response characteristics
#endif

#if TLKALG_BBF_PATH_SEL
#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
#define TLKMDI_AUDMEM_TOTAL_SIZE ((43 + 64) * 1024)
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
#define TLKMDI_AUDMEM_TOTAL_SIZE ((33 + 64) * 1024)
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
#define TLKMDI_AUDMEM_TOTAL_SIZE ((23 + 64) * 1024)
#else
#define TLKMDI_AUDMEM_TOTAL_SIZE (64 * 1024)
#endif
#else
#define TLKMDI_AUDMEM_TOTAL_SIZE (64 * 1024)
#endif

#endif


/////////////////////// Board Select Configuration //////////////////////////////
#ifndef TLKHW_TYPE
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A87_V1_2
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define TLKHW_TYPE BOARD_721X_EVK_C1T315A87_V1_0
#endif
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)

#define BLE_ACL_SINGLE_RATE_OPTIMIZE     1
#define ACL_PERIPHR_TX_FIFO_NUM          9

#define TLKDRV_CODEC_TL721X_ABNORMAL_CFG 1
#define RECORDING_CARD_EN                1

#define VAD_FUNCTION_ENABLE              1
#define OPUS_ENC_BITRATE                 12000

#define TLKOS_CFG_HEART_TIMER_TICK_HZ    32768
#define TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE ((40 + 5) * 1024)

#define TLKMDI_DMIC_POWER_SUSPEND_DIS    1

#define TLK_CFG_RC_WAV_SAVE              0 //NO SAVE WAV FILE

#define TLKMW_AUDIO_MEMPOOL_INDEPENDENT  0

#if TLKMW_AUDIO_MEMPOOL_INDEPENDENT
#define TLKOS_CFG_OS_HEAP_SIZE (20 * 1024)
#else
#define TLKOS_CFG_OS_HEAP_SIZE (20 * 1024 + TLKMDI_AUDMEM_TOTAL_SIZE)
#endif

#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define VAD_FUNCTION_ENABLE              0
#define TLKALG_AGC_ENABLE                0
#define TLK_CFG_RC_WAV_SAVE              1 //SAVE WAV FILE
#define TLKOS_CFG_OS_HEAP_SIZE           (40 * 1024)
#define TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE (30 * 1024)
#define TLKMDI_CODEC_LOW_POWER_MODE_EN   1
#endif


//ram reduce cfg
#ifndef TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM
#define TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM 2
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
// #define CONN_MAX_NUM_CONFIG 1 //CONN_MAX_NUM_C0_P1
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER 0

#define TLKOS_CFG_DEBUG_ENABLE                                 0
#define BLE_HOST_MEMORY_POOL_RAM_SECTION                       _attribute_iram_data_
#define BLE_HOST_MEMORY_POOL_SIZE                              (10 * 1024)
#endif


/////////////////////// *********  Below : difference between  single-core chip and  dual-core chip //////////////////////////////
#if (MCU_DUAL_CORE_ENABLE)

#define TLK_MW_DSP_COMM_ENABLE (1) //system clk 48M, can not support run nn

#endif

#include "vendor/common/default_config.h"
