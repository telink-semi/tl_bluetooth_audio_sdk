/********************************************************************************************************
 * @file    btp_config.h
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
#ifndef BTP_CONFIG_H
#define BTP_CONFIG_H

#define BTP_TIMER_TIMEOUT    200000
#define BTP_TIMER_TIMEOUT_MS 200

#ifndef BTP_SDP_SHARE_BUFFER_ENABLE
#define BTP_SDP_SHARE_BUFFER_ENABLE (1 && TLK_STK_BT_ENABLE)
#endif
#define TLKBTP_CFG_SDP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_SDPSRV_ENABLE (1 && TLK_STK_BT_ENABLE && TLKBTP_CFG_SDP_ENABLE)
#define TLKBTP_CFG_SDPCLT_ENABLE (1 && TLK_STK_BT_ENABLE && TLKBTP_CFG_SDP_ENABLE)

#ifndef TLK_BT_SDP_MAX_NUMB
#define TLK_BT_SDP_MAX_NUMB (TLK_STK_BTACL_NUMB * 2)
#endif

#define TLK_BT_IAP_MAX_NUMB            (TLK_STK_BTACL_NUMB)
#define TLK_BT_HID_MAX_NUMB            (TLK_STK_BTACL_NUMB)
#define TLK_BT_SPP_MAX_NUMB            (TLK_STK_BTACL_NUMB)
#define TLK_BT_HFP_MAX_NUMB            (TLK_STK_BTACL_NUMB)
#define TLK_BT_ATT_MAX_NUMB            (1)
#define TLK_BT_A2DP_MAX_NUMB           (TLK_STK_BTACL_NUMB)
#define TLK_BT_AVRCP_MAX_NUMB          (TLK_STK_BTACL_NUMB)
#define TLK_BT_BROWSING_MAX_NUMB       (TLK_STK_BTACL_NUMB)
#define TLK_BT_RFCOMM_SERVICE_MAX_NUMB 6 // spp, pbap, hfp-ag, hfp-hf
#define TLK_BT_RFCOMM_SESSION_MAX_NUMB (TLK_STK_BTACL_NUMB)
#define TLK_BT_RFCOMM_CHANNEL_MAX_NUMB (TLK_STK_BTACL_NUMB * 6)
#define TLK_BT_RFCOMM_CHNDICT_MAX_NUMB (TLK_BT_RFCOMM_CHANNEL_MAX_NUMB + 4)
#define TLK_BT_MAP_MAX_NUMB            (TLK_STK_BTACL_NUMB)
#define TLK_BT_PBAP_MAX_NUMB           (TLK_STK_BTACL_NUMB)
#define TLK_BT_COVERART_MAX_NUMB       (TLK_STK_BTACL_NUMB)
#define TLK_BT_CUSTP_MAX_NUMB          (TLK_STK_BTACL_NUMB)


#define TLKBT_CFG_HS_RFC_CHANNEL       0x02
#define TLKBT_CFG_HFPHF_RFC_CHANNEL    0x05
#define TLKBT_CFG_HFPAG_RFC_CHANNEL    0x06
#define TLKBT_CFG_SPP_RFC_CHANNEL      0x04
#define TLKBT_CFG_IAP_RFC_CHANNEL      0x07
#define TLKBT_CFG_PBAP_RFC_CHANNEL     0x09
#define TLKBT_CFG_SPP_USER_RFC_CHANNEL 0x0a
#define TLKBT_CFG_BIP_RFC_CHANNEL      0x0b


#endif // BTP_CONFIG_H
