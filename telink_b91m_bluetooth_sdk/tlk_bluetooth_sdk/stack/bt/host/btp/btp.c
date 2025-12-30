/********************************************************************************************************
 * @file    btp.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#if (TLK_STK_BT_ENABLE)
#include "drivers.h"
//#include "btp_stdio.h"
//#include "btp_adapt.h"
//#include "../bth/bth.h"
//#include "../bth/bth_handle.h"
//#include "../bth/bth_l2cap.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/btp_adapt.h"
#include "btp.h"
#include "btp_config.h"
#include "btp_define.h"
#include "sdp/btp_sdp.h"
#include "a2dp/btp_a2dp.h"
#include "avrcp/btp_avrcp.h"
#include "rfcomm/btp_rfcomm.h"
#include "hfp/btp_hfp.h"
#include "pbap/btp_pbap.h"
#include "spp/btp_spp.h"
#include "att/btp_att.h"
#include "hid/btp_hid.h"
#include "iap/btp_iap.h"
#include "browsing/btp_browsing.h"

int btp_init(void)
{
    int ret;

    ret = TLK_ENONE;

#if (TLKBTP_CFG_SDP_ENABLE)
    /* When 'BTP_SDP_SHARE_BUFFER_ENABLE' set to 1, HFP and HID fail to connect in
       a peer-to-peer connectivity scenario; set to 0 HFP and HID can be connected
       successfully, but will increase memory consumption.*/
    if (ret == TLK_ENONE) {
        ret = btp_sdp_init(TLK_BT_SDP_MAX_NUMB, BTP_SDPSRV_RSP_ATT_LEN, BTP_SDP_BUFFER_SIZE, BTP_SDP_SHARE_BUFFER_ENABLE);
    }
#endif
#if (TLKBTP_CFG_AVRCP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_avrcp_init(TLK_BT_AVRCP_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_A2DP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_a2dp_init(TLK_BT_A2DP_MAX_NUMB, TLKBTP_CFG_A2DP_FEATURE_CFG);
    }
#endif
#if (TLKBTP_CFG_RFC_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_rfcomm_init(TLK_BT_RFCOMM_SERVICE_MAX_NUMB, TLK_BT_RFCOMM_SESSION_MAX_NUMB, TLK_BT_RFCOMM_CHANNEL_MAX_NUMB, TLK_BT_RFCOMM_CHNDICT_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_hfp_init(TLK_BT_HFP_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_PBAP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_pbap_init(TLK_BT_PBAP_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_SPP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_spp_init(TLK_BT_SPP_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_IAP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_iap_init(TLK_BT_IAP_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_ATT_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_att_init(TLK_BT_ATT_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_HID_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_hid_init(TLK_BT_HID_MAX_NUMB);
    }
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_browsing_init(TLK_BT_BROWSING_MAX_NUMB);
    }
    if (ret == TLK_ENONE) {
        bth_l2cap_setExtFeatureBits(BTH_L2CAP_EXT_FEATURE_ENHANCED_RTN_MODE | BTH_L2CAP_EXT_FEATURE_FCS_OPTION);
    }
#endif
#if (TLKBTP_CFG_COVERART_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_coverArt_init(TLK_BT_COVERART_MAX_NUMB);
    }
    if (ret == TLK_ENONE) {
        bth_l2cap_setExtFeatureBits(BTH_L2CAP_EXT_FEATURE_ENHANCED_RTN_MODE | BTH_L2CAP_EXT_FEATURE_FCS_OPTION);
    }
#endif

#if (TLKBTP_CFG_CUSTP_ENABLE)
    if (ret == TLK_ENONE) {
        ret = btp_custp_init(TLK_BT_CUSTP_MAX_NUMB);
    }
#endif

    if (ret != TLK_ENONE) {
        btp_deinit();
    }
    return ret;
}

void btp_deinit(void)
{
#if (TLKBTP_CFG_SDP_ENABLE)
    /* When 'BTP_SDP_SHARE_BUFFER_ENABLE' set to 1, HFP and HID fail to connect in
       a peer-to-peer connectivity scenario; set to 0 HFP and HID can be connected
       successfully, but will increase memory consumption.*/
    btp_sdp_deinit();
#endif
#if (TLKBTP_CFG_AVRCP_ENABLE)
    btp_avrcp_deinit();
#endif
#if (TLKBTP_CFG_A2DP_ENABLE)
    btp_a2dp_deinit();
#endif
#if (TLKBTP_CFG_RFC_ENABLE)
    btp_rfcomm_deinit();
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    btp_hfp_deinit();
#endif
#if (TLKBTP_CFG_PBAP_ENABLE)
    btp_pbap_deinit();
#endif
#if (TLKBTP_CFG_SPP_ENABLE)
    btp_spp_deinit();
#endif
#if (TLKBTP_CFG_IAP_ENABLE)
    btp_iap_deinit();
#endif
#if (TLKBTP_CFG_ATT_ENABLE)
    btp_att_deinit();
#endif
#if (TLKBTP_CFG_HID_ENABLE)
    btp_hid_deinit();
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    btp_browsing_deinit();
#endif
#if (TLKBTP_CFG_CUSTP_ENABLE)
    btp_custp_deinit();
#endif
}

void btp_destroy(uint16_t aclHandle)
{
#if (TLKBTP_CFG_SDP_ENABLE)
    btp_sdp_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_A2DP_ENABLE)
    btp_a2dp_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_AVRCP_ENABLE)
    btp_avrcp_destroy(aclHandle);
#endif

#if (TLKBTP_CFG_PBAP_ENABLE)
    btp_pbap_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    btp_hfp_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_SPP_ENABLE)
    btp_spp_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_IAP_ENABLE)
    btp_iap_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_RFC_ENABLE)
    btp_rfcomm_destroy(aclHandle);
#endif

#if (TLKBTP_CFG_ATT_ENABLE)
    btp_att_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_HID_ENABLE)
    btp_hid_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    btp_browsing_destroy(aclHandle);
#endif
#if (TLKBTP_CFG_CUSTP_ENABLE)
    btp_custp_destroy(aclHandle);
#endif
}

int btp_needMemLen(void)
{
    int needLen = 0;

#if (TLKBTP_CFG_SDP_ENABLE)
    /* When 'BTP_SDP_SHARE_BUFFER_ENABLE' set to 1, HFP and HID fail to connect in
       a peer-to-peer connectivity scenario; set to 0 HFP and HID can be connected
       successfully, but will increase memory consumption.*/
    needLen += btp_sdp_getMemLen(TLK_BT_SDP_MAX_NUMB, BTP_SDPSRV_RSP_ATT_LEN, BTP_SDP_BUFFER_SIZE, BTP_SDP_SHARE_BUFFER_ENABLE);
#endif
#if (TLKBTP_CFG_AVRCP_ENABLE)
    needLen += btp_avrcp_getMemLen(TLK_BT_AVRCP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_A2DP_ENABLE)
    needLen += btp_a2dp_getMemLen(TLK_BT_A2DP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_RFC_ENABLE)
    needLen += btp_rfcomm_getMemLen(TLK_BT_RFCOMM_SERVICE_MAX_NUMB, TLK_BT_RFCOMM_SESSION_MAX_NUMB, TLK_BT_RFCOMM_CHANNEL_MAX_NUMB, TLK_BT_RFCOMM_CHNDICT_MAX_NUMB);
#endif
#if (TLKBTP_CFG_HFP_ENABLE)
    needLen += btp_hfp_getMemLen(TLK_BT_HFP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_PBAP_ENABLE)
    needLen += btp_pbap_getMemLen(TLK_BT_PBAP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_SPP_ENABLE)
    needLen += btp_spp_getMemLen(TLK_BT_SPP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_IAP_ENABLE)
    needLen += btp_iap_getMemLen(TLK_BT_IAP_MAX_NUMB);
#endif
#if (TLKBTP_CFG_ATT_ENABLE)
    needLen += btp_att_getMemLen(TLK_BT_ATT_MAX_NUMB);
#endif
#if (TLKBTP_CFG_HID_ENABLE)
    needLen += btp_hid_getMemLen(TLK_BT_HID_MAX_NUMB);
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    needLen += btp_browsing_getMemLen(TLK_BT_BROWSING_MAX_NUMB);
#endif
#if (TLKBTP_CFG_CUSTP_ENABLE)
    needLen += btp_custp_getMemLen(TLK_BT_CUSTP_MAX_NUMB);
#endif

    return needLen;
}

uint16_t gBtpConnTimeout = 5000 / BTP_TIMER_TIMEOUT_MS;
uint16_t gBtpDiscTimeout = 5000 / BTP_TIMER_TIMEOUT_MS;

void btp_setConnTimeout(uint16_t timeout)
{
    if (timeout < 3000) {
        timeout = 3000;
    } else if (timeout > 15000) {
        timeout = 15000;
    }
    gBtpConnTimeout = timeout / BTP_TIMER_TIMEOUT_MS;
}

void btp_setDiscTimeout(uint16_t timeout)
{
    if (timeout < 3000) {
        timeout = 3000;
    } else if (timeout > 15000) {
        timeout = 15000;
    }
    gBtpDiscTimeout = timeout / BTP_TIMER_TIMEOUT_MS;
}


#endif // #if (TLK_STK_BT_ENABLE)
