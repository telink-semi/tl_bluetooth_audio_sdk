/********************************************************************************************************
 * @file    tlkusb_udbDefine.h
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
#ifndef TLKUSB_UDB_DEFINE_H
#define TLKUSB_UDB_DEFINE_H

#include "tlkapi/tlkapi_debug.h"
#define TLKUSB_UDB_VCD_ENABLE 1

typedef enum
{
    TLKUSB_UDB_INF_DBG,
#if (TLKUSB_UDB_VCD_ENABLE)
    TLKUSB_UDB_INF_VCD,
#endif
    TLKUSB_UDB_INF_MAX,
} TLKUSB_UDB_INF_ENUM;

#if (MCU_CORE_TYPE != MCU_CORE_TL751X&&MCU_CORE_TYPE != MCU_CORE_TL753X|| MCU_CORE_TYPE != MCU_CORE_TL752X)
    #if (USB_CNT == 0)
        #define TLKUSB_UDB_EDP_DBG_IN  USB_EDP3_IN
        #define TLKUSB_UDB_EDP_DBG_OUT USB_EDP5_OUT
        #define TLKUSB_UDB_EDP_VCD_IN  USB_EDP8_IN
        #define TLKUSB_UDB_EDP_VCD_OUT USB_EDP6_OUT
    #else
        #define TLKUSB_UDB_EDP_DBG_IN  USB1_EDP3_IN
        #define TLKUSB_UDB_EDP_DBG_OUT USB1_EDP5_OUT
        #define TLKUSB_UDB_EDP_VCD_IN  USB1_EDP8_IN
        #define TLKUSB_UDB_EDP_VCD_OUT USB1_EDP6_OUT
    #endif

#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL751X||MCU_CORE_TYPE == MCU_CORE_TL753X|| MCU_CORE_TYPE == MCU_CORE_TL752X)
    #if (USB_CNT == 1)
        #define TLKUSB_UDB_EDP_DBG_IN  USB1_EDP3_IN
        #define TLKUSB_UDB_EDP_DBG_OUT USB1_EDP5_OUT
        #define TLKUSB_UDB_EDP_VCD_IN  USB1_EDP8_IN
        #define TLKUSB_UDB_EDP_VCD_OUT USB1_EDP6_OUT
    #else
        #define TLKUSB0_UDB_EDP_DBG_IN  USB0_EDP3_IN
        #define TLKUSB0_UDB_EDP_DBG_OUT USB0_EDP5_OUT
        #define TLKUSB0_UDB_EDP_VCD_IN  USB0_EDP8_IN
        #define TLKUSB0_UDB_EDP_VCD_OUT USB0_EDP6_OUT
    #endif
#endif

typedef enum
{
    TLKUSB_PRNT_CSCP_PrinterClass          = 0x07,
    TLKUSB_PRNT_CSCP_PrinterSubclass       = 0x01,
    TLKUSB_PRNT_CSCP_BidirectionalProtocol = 0x02,
} TLKUSB_PRINT_CSCP_ENUM; // CSCP-ClassSubClassProtocol


#endif                    // TLKUSB_UDB_DEFINE_H
