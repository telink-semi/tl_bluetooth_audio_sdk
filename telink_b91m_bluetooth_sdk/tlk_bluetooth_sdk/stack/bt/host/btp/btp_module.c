/********************************************************************************************************
 * @file    btp_module.c
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
    #include "btp_stdio.h"
    #include "btp_adapt.h"
    #include "btp.h"
    #include "btp_module.h"
    #include "sdp/btp_sdp.h"
    #include "a2dp/btp_a2dp.h"
    #include "avrcp/btp_avrcp.h"
    #include "rfcomm/btp_rfcomm.h"
    #include "hfp/btp_hfp.h"
    #include "spp/btp_spp.h"
    #include "a2dp/btp_a2dp.h"
    #include "pbap/btp_pbap.h"
    #include "hid/btp_hid.h"
    #include "att/btp_att.h"
    #include "iap/btp_iap.h"

int btp_module_connect(uint16_t aclHandle, uint8_t ptype, uint8_t usrID, uint8_t channel)
{
    int ret;

    ret = -TLK_ENOSUPPORT;
    switch (ptype) {
    case BTP_PTYPE_SDP:
        ret = btp_sdp_connect(aclHandle, usrID);
        break;
    case BTP_PTYPE_RFC:
    #if (TLKBTP_CFG_PBAP_ENABLE)
        ret = btp_rfcomm_connect(aclHandle);
    #endif
        break;
    case BTP_PTYPE_HFP:
    #if (TLKBTP_CFG_HFP_ENABLE)
        ret = btp_hfp_connect(aclHandle, usrID, channel);
    #else
        (void)channel;
    #endif
        break;
    case BTP_PTYPE_SPP:
    #if (TLKBTP_CFG_SPP_ENABLE)
        ret = btp_spp_connect(aclHandle, channel);
    #endif
        break;
    case BTP_PTYPE_IAP:
    #if (TLKBTP_CFG_IAP_ENABLE)
        ret = btp_iap_connect(aclHandle, channel);
    #endif
        break;
    case BTP_PTYPE_A2DP:
    #if (TLKBTP_CFG_A2DP_ENABLE)
        ret = btp_a2dp_connect(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_AVRCP:
    #if (TLKBTP_CFG_AVRCP_ENABLE)
        ret = btp_avrcp_connect(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_PBAP:
    #if (TLKBTP_CFG_PBAP_ENABLE)
        ret = btp_pbap_connect(aclHandle, usrID, channel, false);
    #endif
        break;
    case BTP_PTYPE_HID:
    #if (TLKBTP_CFG_HID_ENABLE)
        ret = btp_hid_connect(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_ATT:
    #if (TLKBTP_CFG_ATT_ENABLE)
        ret = btp_att_connect(aclHandle, usrID);
    #endif
        break;
    }
    return ret;
}

int btp_module_disconn(uint16_t aclHandle, uint8_t ptype, uint8_t usrID)
{
    int ret;

    ret = -TLK_ENOSUPPORT;
    switch (ptype) {
    case BTP_PTYPE_SDP:
        ret = btp_sdp_disconn(aclHandle, usrID);
        break;
    case BTP_PTYPE_RFC:
    #if (TLKBTP_CFG_RFC_ENABLE)
        ret = btp_rfcomm_disconn(aclHandle);
    #endif
        break;
    case BTP_PTYPE_HFP:
    #if (TLKBTP_CFG_HFP_ENABLE)
        ret = btp_hfp_disconn(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_SPP:
    #if (TLKBTP_CFG_SPP_ENABLE)
        ret = btp_spp_disconn(aclHandle);
    #endif
        break;
    case BTP_PTYPE_IAP:
    #if (TLKBTP_CFG_IAP_ENABLE)
        ret = btp_iap_disconn(aclHandle);
    #endif
        break;
    case BTP_PTYPE_A2DP:
    #if (TLKBTP_CFG_A2DP_ENABLE)
        ret = btp_a2dp_disconn(aclHandle);
    #endif
        break;
    case BTP_PTYPE_AVRCP:
    #if (TLKBTP_CFG_AVRCP_ENABLE)
        ret = btp_avrcp_disconn(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_PBAP:
    #if (TLKBTP_CFG_PBAP_ENABLE)
        ret = btp_pbap_disconn(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_HID:
    #if (TLKBTP_CFG_HID_ENABLE)
        ret = btp_hid_disconn(aclHandle, usrID);
    #endif
        break;
    case BTP_PTYPE_ATT:
    #if (TLKBTP_CFG_ATT_ENABLE)
        ret = btp_att_disconn(aclHandle, usrID);
    #endif
        break;
    }
    return ret;
}


#endif // #if (TLK_STK_BT_ENABLE)
