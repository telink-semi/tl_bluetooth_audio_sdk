/********************************************************************************************************
 * @file    app_bt_central_config.h
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

#define TLK_REC_BT_CENTRAL_TEMP_DEBUG 0
#if TLK_REC_BT_CENTRAL_TEMP_DEBUG
#undef TLK_CFG_FS_ENABLE
#undef TLK_CFG_WIFI_LOGIC_ENABLE
#define TLK_CFG_FS_ENABLE         0
#define TLK_CFG_WIFI_LOGIC_ENABLE 0
#define TLKHW_TYPE                TLKHW_TL751X_EVK_C1T368A20_V1_0
#endif

//bt center device temp not support suspend
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


#if (TLK_CFG_RTOS_ENABLE)
#define TEMP_A2DP_OUT_NEW_VERSION 1
#endif

#define TLKBTP_CFG_A2DP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSRC_ENABLE (1 && TLKBTP_CFG_A2DP_ENABLE)
#define TLKBTP_CFG_AVRCP_ENABLE   (1 && TLK_STK_BT_ENABLE)

#define TLKBTP_CFG_RFC_ENABLE     (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE     (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPAG_ENABLE   (1 && TLKBTP_CFG_HFP_ENABLE)

#define BT_A2DP_OUT_INPUT_TYPE    TLKMDI_A2DP_OUT_TYPE_SINE_WAVE

#define TLKALG_SBC_DEC_ENABLE     1
#define TLKALG_SBC_ENC_ENABLE     1
#define TLKALG_MSBC_ENABLE        1