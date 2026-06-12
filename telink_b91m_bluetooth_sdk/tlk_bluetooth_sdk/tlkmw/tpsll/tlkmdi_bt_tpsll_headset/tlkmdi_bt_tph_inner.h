/********************************************************************************************************
 * @file    tlkmdi_bt_tph_inner.h
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
#ifndef TLKMDI_BT_TPH_INNER_H
#define TLKMDI_BT_TPH_INNER_H

typedef struct
{
    // word 0
    uint8_t  tpsll_ch;
    uint8_t  cur_status;
    uint16_t timeout;
    // word 1
    uint8_t local_addr[6];
    uint8_t addr_paired_dongle[6];
    // word 5
    uint8_t  dongleIsConn   : 4;
    uint8_t  isForceIdleing : 4;
    uint8_t  disReason;
    uint16_t startPairing;
    // word 7
    uint32_t tpsll_ac;
    // word 8
    uint32_t dongle_ver;
    // word 9
    uint16_t acl_mtu;
    uint8_t  ota_intv;
    uint8_t  resv;

    tlkmdi_tph_force_idle_finished_cb idleCB;
    TlkApiTimer_t                     timer;
} tlkmdi_bt_tph_ctrl_t; // 60bytes

extern tlkmdi_bt_tph_ctrl_t       gTlkMdiHeadsetCtrl;
extern tlkmdi_tph_state_change_cb gInnerTlkMdiBtTphStateChgCB;

#endif
