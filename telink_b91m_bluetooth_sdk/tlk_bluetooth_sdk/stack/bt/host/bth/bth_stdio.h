/********************************************************************************************************
 * @file    bth_stdio.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef BTH_STDIO_H
#define BTH_STDIO_H

#include "stack/bt/host/tlkstk_mem.h"
/*****************host_bth*******************************/
#include "stack/bt/host/bth/bth_acl.h"
#include "stack/bt/host/bth/bth_adapt.h"
#include "stack/bt/host/bth/bth_config.h"
#include "stack/bt/host/bth/bth_define.h"
#include "stack/bt/host/bth/bth_event.h"
#include "stack/bt/host/bth/bth_handle.h"
#include "stack/bt/host/bth/bth_hcicmd.h"
#include "stack/bt/host/bth/bth_hcicod.h"
#include "stack/bt/host/bth/bth_hcievt.h"
#include "stack/bt/host/bth/bth_l2cap.h"
#include "stack/bt/host/bth/bth_sco.h"
#include "stack/bt/host/bth/bth_signal.h"
#include "stack/bt/host/bth/bth_struct.h"
#include "stack/bt/host/bth/bth.h"

typedef enum
{                                  // Major Class
    BTH_DEVICE_DTYPE_NONE     = 0,
    BTH_REMOTE_DTYPE_COMPUTER = 1, // desktop, notebook, PDA, organizer,
    BTH_REMOTE_DTYPE_PHONE    = 2, // cellular, cordless, pay phone, modem,
    BTH_REMOTE_DTYPE_HEADSET  = 4, // Audio/Video: headset, speaker, stereo, video display, VCR,
    BTH_REMOTE_DTYPE_OTHER    = 0xFF,
} BTH_DEVICE_DTYPE_ENUM;

extern void bth_acl_enterSniff(uint16_t aclHandle, bool isForce);
extern void bth_acl_leaveSniff(uint16_t aclHandle, uint16_t busyTime);


#endif // BTH_STDIO_H
