/********************************************************************************************************
 * @file    app_bt_peripheral_config.h
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

#define TLK_REC_BT_PERIPHERAL_TEMP_DEBUG 1
#if TLK_REC_BT_PERIPHERAL_TEMP_DEBUG
#undef TLK_CFG_WIFI_LOGIC_ENABLE
#define TLK_CFG_WIFI_LOGIC_ENABLE 0
#define TLKHW_TYPE                TLKHW_TL751X_EVK_C1T368A20_V1_0
#endif

#ifdef TLK_CFG_SUSPEND_ENABLE
#undef TLK_CFG_SUSPEND_ENABLE
#endif
#define TLK_CFG_SUSPEND_ENABLE 0

#ifdef TLK_RC_CFG_AUTO_POWER_OFF
#undef TLK_RC_CFG_AUTO_POWER_OFF
#endif
#define TLK_RC_CFG_AUTO_POWER_OFF          0

#define TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM 8

#define TLK_STK_BT_ENABLE                  1
#define TLK_STK_BTACL_NUMB                 1


#define TLKBTP_CFG_RFC_ENABLE              (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE              (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE            (1 && TLKBTP_CFG_HFP_ENABLE)

#define TLKCFG_BT_MAJOR_SERVICE_CLASS      (BTH_COD_SERVICE_CLASS_AUDIO | BTH_COD_SERVICE_CLASS_RENDER | BTH_COD_SERVICE_CLASS_LE_AUDIO)
#define TLKCFG_BT_MGR_DEVICE_CLASS         (TLKCFG_BT_MAJOR_SERVICE_CLASS | BTH_COD_MAJOR_CLASS_AUDIO | BTH_COD_AUDIO_WEARABLE_HEADSET) // 0x244404

#define TLKALG_SBC_DEC_ENABLE              1
#define TLKALG_MSBC_ENABLE                 1
#define TLKALG_CVSD_ENABLE                 1

#ifdef TLK_DEV_CODEC_SPK_ENABLE
#undef TLK_DEV_CODEC_SPK_ENABLE
#define TLK_DEV_CODEC_SPK_ENABLE (1 && TLKDRV_CODEC_ICODEC_ENABLE)
#endif

#ifdef TLKDRV_CODEC_COLD_START_ENABLE
#undef TLKDRV_CODEC_COLD_START_ENABLE
#define TLKDRV_CODEC_COLD_START_ENABLE (0 && TLKDRV_CODEC_ICODEC_ENABLE)
#endif


#define CONFIG_TLK_AUDIO_TONE_DOWNLOAD_ADDR (FLASH_R_BASE_ADDR + 0X7A0000)