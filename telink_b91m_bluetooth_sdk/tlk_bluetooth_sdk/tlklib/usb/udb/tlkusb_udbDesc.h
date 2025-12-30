/********************************************************************************************************
 * @file    tlkusb_udbDesc.h
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
#ifndef TLKUSB_UDB_DESC_H
#define TLKUSB_UDB_DESC_H

#if (TLK_USB_UDB_ENABLE)


typedef struct
{
    tlkusb_stdConfigureDesc_t config;
    //	tlkusb_stdAssociateDesc_t udbIAD;
    tlkusb_stdInterfaceDesc_t udbInf;
    tlkusb_stdEndpointDesc_t  udbEdpIn;
    tlkusb_stdEndpointDesc_t  udbEdpOut;
    #if (TLKUSB_UDB_VCD_ENABLE)
    //	tlkusb_stdAssociateDesc_t vcdIAD;
    tlkusb_stdInterfaceDesc_t vcdInf;
    tlkusb_stdEndpointDesc_t  vcdInE;
    tlkusb_stdEndpointDesc_t  vcdOutE;
    #endif
} tlkusb_udbConfigDesc_t;


#endif // #if (TLK_USB_UDB_ENABLE)

#endif // TLKUSB_UDB_DESC_H
