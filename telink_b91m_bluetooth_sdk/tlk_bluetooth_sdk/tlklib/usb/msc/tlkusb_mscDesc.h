/********************************************************************************************************
 * @file    tlkusb_mscDesc.h
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


//Serial num str
#define TLKUSB_MSC_STRING_VENDOR  L"Telink"
#define TLKUSB_MSC_STRING_PRODUCT L"Telink audio team storage demo"
#define TLKUSB_MSC_STRING_SERIAL  L"Telink audio team nand"

typedef struct
{
    tlkusb_stdConfigureDesc_t config;
    //	tlkusb_stdAssociateDesc_t audIAD;
    tlkusb_stdInterfaceDesc_t mscInf;
    tlkusb_stdEndpointDesc_t  mscEdpIn;
    tlkusb_stdEndpointDesc_t  mscEdpOut;
} tlkusb_mscConfigDesc_t;
