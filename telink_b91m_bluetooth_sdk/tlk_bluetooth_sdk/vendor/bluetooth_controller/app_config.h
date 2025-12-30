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
#include "bluetooth_controller.h"

///////////////////////// System -app Configuration start/////////////////////////////////////////////////
#define DEBUG_COMMON_GPIO_ENABLE	1

#if (MCU_DUAL_CORE_ENABLE)
#define TLK_CFG_RTOS_ENABLE           1
#endif
/********************  lynx adapt start *********************/
// 1. Lynx haven't LDO power mode.
// 2. Lynx flash size is 4M.
// 3. "include_path_lynx" delete.
// ALL macro will delete when drv is ready.
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
#define TLK_CFG_TEMP_DRAM_OPTM			1
#define TLK_CFG_TEMP_OLD_N22_BOOT_READY 0
#define BT_MODE							1
#define BLE_MODE						0
#define TLK_STK_MODE					BLE_MODE
#define BT_BQB_D25F_TEST                1
#define BT_EMI_D25F_TEST                0
#else
#define TLK_CFG_TEMP_DRAM_OPTM			0
#define TLK_CFG_TEMP_OLD_N22_BOOT_READY 1    //wait drv code ok(use boot way same as tl751x)
#endif
#define TLK_CFG_TEMP_PM_DRV_READY		0
#define TLK_CFG_TEMP_WD_DRV_READY		0
#define TLK_CFG_TEMP_RAND_DRV_READY		0
#define TLK_CFG_TEMP_DRV_PWM_READY		0
#define TLK_CFG_TEMP_UART_DRV_READY		0
/********************  lynx adapt end  **********************/

#define TLK_CFG_SUSPEND_ENABLE   0 //1: enable suspend

#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       1
#define TLK_DEV_MFI_ENABLE       0
#define TLK_CFG_TONE_ENABLE      0
#define TLK_CFG_UART_TOOL_ENABLE 0
#define TLK_DEBUG_ENABLE         1

#define TLKSTK_HOST_ENABLE  1
//#define TLK_STK_BLE_ENABLE  1

#define TLK_MW_TINYSQL_ENABLE    1

#define TLK_MW_USER_CTRL_ENABLE       0

///////////////////////// System -app Configuration end/////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
////                          			BLE
////////////////////////////////////////////////////////////////////////////////////////
#define BLE_FEATURE_ALL_EN 1

#define ACL_CENTRAL_MAX_NUM                                 2 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM                                 2 // ACL peripheral maximum number

#define BUILD_TASK_EARLY_DONE_TIME_RE_USE_EN                1
#define MASTER_TIMING_USE_PLANNER_ENABLE                    1
#define MASTER_TIMING_PLANNER_IMPROVE_ENABLE                1

#define TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM           1
#define LL_DDI_ADV_BV61C                                    1
#define EXTENDED_ADV_RPT_MANUAL_EN                          1
#define LONG_CTRL_PDUS_AUTO_FEATURE_REQ_DIS                 1
#define LL_CON_PER_BV105C                                   1

#define LL_FEATURE_SUPPORT_MIN_USED_OF_USED_CHANNELS        1
#define LL_FEATURE_SUPPORT_EXTENDED_SCANNER_FILTER_POLICIES 1
#define LL_FEATURE_SUPPORT_PERIODIC_ADV_ADI_SUPPORT         1
#define LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN         1
#define LL_FEATURE_SUPPORT_SLEEP_CLK_ACCURACY_UPDATE        1
#define LL_FEATURE_SUPPORT_REMOTE_PUBLIC_KEY_VALIDATION     1
#define HCI_CONTROLLER_TO_HOST_FLOW_CTRL_EN                 1
#define HCI_SEND_NUM_OF_CMP_AFT_ACK                         1



///
/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)
	#define TLKHW_TYPE      TLKHW_TLSR9528A_EVK_C1T266A20
	#define HCI_TR_RX_PIN   GPIO_FC_PC7
	#define HCI_TR_TX_PIN   GPIO_FC_PC6
	#define HCI_TR_BAUDRATE (1000000)
	#define LE_HOST_SEND_HCI_MODE LE_HOST_SEND_HCI_MODE_NONE
#else
    #error "need config uart pin !!!"
#endif

#define HCI_TR_RX_BUF_SIZE (760)
#define HCI_TR_TX_BUF_SIZE (760)

/* print log */
#define APP_LOG_EN 1


#include "vendor/common/default_config.h"
