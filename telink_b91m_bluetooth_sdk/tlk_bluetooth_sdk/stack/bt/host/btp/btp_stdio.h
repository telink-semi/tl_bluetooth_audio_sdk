/********************************************************************************************************
 * @file    btp_stdio.h
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
#ifndef BTP_STDIO_H
#define BTP_STDIO_H


#include "stack/bt/host/btp/btp_config.h"
#include "stack/bt/host/btp/btp_define.h"
#include "stack/bt/host/btp/btp_event.h"
#include "stack/bt/host/btp/btp_module.h"
#include "stack/bt/host/btp/btp.h"
#include "stack/bt/host/tlkstk_mem.h"

#include "stack/bt/host/btp/a2dp/btp_a2dp.h"

#include "stack/bt/host/btp/att/btp_att.h"
#include "stack/bt/host/btp/att/btp_attClt.h"
#include "stack/bt/host/btp/att/btp_attInner.h"
#include "stack/bt/host/btp/att/btp_attSrv.h"
#include "stack/bt/host/btp/att/btp_attStdio.h"

#include "stack/bt/host/btp/avctp/btp_avctp.h"

#include "stack/bt/host/btp/avdtp/btp_avdtp.h"

#include "stack/bt/host/btp/avrcp/btp_avrcp.h"

#include "stack/bt/host/btp/browsing/btp_browsing.h"
#include "stack/bt/host/btp/browsing/btp_browsingRecv.h"
#include "stack/bt/host/btp/browsing/btp_browsingSend.h"

#include "stack/bt/host/btp/hfp/btp_hfp.h"

#include "stack/bt/host/btp/hid/btp_hid.h"

#include "stack/bt/host/btp/iap/btp_iap.h"

#include "stack/bt/host/btp/map/btp_map.h"

#include "stack/bt/host/btp/obex/btp_obex.h"

#include "stack/bt/host/btp/pbap/btp_pbap.h"

#include "stack/bt/host/btp/rfcomm/btp_rfcomm.h"

#include "stack/bt/host/btp/sdp/btp_sdp.h"
#include "stack/bt/host/btp/sdp/btp_sdpConst.h"

#include "stack/bt/host/btp/spp/btp_spp.h"
#if (TLKBTP_CFG_CUSTP_ENABLE)
#include "stack/bt/host/btp/custp/btp_custp.h"
#endif

extern uint16_t gBtpConnTimeout;
extern uint16_t gBtpDiscTimeout;


#endif // BTP_STDIO_H
