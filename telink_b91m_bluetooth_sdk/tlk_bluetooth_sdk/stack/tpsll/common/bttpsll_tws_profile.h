/********************************************************************************************************
 * @file    bttpsll_tws_profile.h
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
#ifndef BTTPSLL_TWS_PROFILE_H_
#define BTTPSLL_TWS_PROFILE_H_

#include "tl_common.h"

typedef struct
{
    uint8_t seid_used;
    uint8_t a2dp_status;
    uint8_t a2dp_chid[2];
    uint8_t media_scid[2];
    uint8_t media_dcid[2];
} bttpsll_tws_avdtp_media_chn_t;

typedef struct BTTPSLL_TWS_A2DPAVRCP_INFO
{
    /* AVDTP */
    uint8_t  avdtp_seid_used;
    uint8_t  avdtp_resv01;
    uint16_t avdtp_sig_scid;
    uint16_t avdtp_sig_dcid;
    uint16_t avdtp_media_scid;
    uint16_t avdtp_media_dcid;
    uint16_t avdtp_resv02;
    /* AVCTP */
    uint16_t avctp_scid;
    uint16_t avctp_dcid;

    /* AVRCP */
    uint8_t avrcp_volume;
    uint8_t avrcp_is_supp_set_volume : 1;
    uint8_t avrcp_res                : 7;
    /* A2DP */
    uint8_t  a2dp_snk_state;
    uint8_t  a2dp_resv;
    uint32_t a2dp_frequency;
} bttpsll_tws_a2dpavrcp_info_t; // 5 words

typedef struct BTTPSLL_TWS_HFP_INFO
{
    // HFP
    uint16_t hf_rfcomm_scid;
    uint16_t hf_rfcomm_dcid;

    uint8_t  hf_rfcomm_dlci;
    uint8_t  hf_rfcomm_dlc_addr;
    uint16_t hf_idxCall      : 4;
    uint16_t hf_idxService   : 4;
    uint16_t hf_idxCallSetup : 4;
    uint16_t hf_idxCallHeld  : 4;

    uint8_t statusCall      : 4;
    uint8_t statusCallSetup : 4;
    uint8_t statusCallHeld  : 4;
    uint8_t hf_res01        : 4;
    uint8_t hfp_hf_state;
    uint8_t hfp_hf_vgs : 4; // 0~0x0f, hfp-hf spk volume.
    uint8_t hfp_hf_vgm : 4; // 0~0x0f, hfp-hf mic volume.

    uint8_t  hf_siri_support     : 1; // hfp-hf siri_support.
    uint8_t  hfp_hf_codec        : 2; // hfp-hf codec, cvsd or msbc.
    uint8_t  hfp_hf_initiator    : 1; // hfp-hf rfcomm_session_initiator.
    uint8_t  voice_recog_support : 1; /* Voice Recognition */
    uint8_t  hfp_hf_vol_support  : 1; /* Volume Control */
    uint8_t  hfp_channel;
    uint16_t hfp_res03;
} bttpsll_tws_hfp_info_t; // 4 words

typedef struct BTTPSLL_TWS_SPPGATT_INFO
{
    uint8_t spp_gatt_flag; // 1 spp, 2 gatt
    uint8_t spp_rfcomm_dlc_addr;
    uint8_t spp_rfcomm_dlci; // GATT
    uint8_t spp_rfc_initiator;

    uint16_t spp_rfcomm_scid; // for only con spp
    uint16_t spp_rfcomm_dcid;
    uint16_t spp_rfcomm_mtu;
    uint16_t gatt_scid;
    uint16_t gatt_dcid;
    uint16_t cccd_en_map; // Used for GATT CCCD enable synchronization
    uint16_t gatt_mtu_local;
    uint16_t gatt_mtu_remote;

    uint8_t  spp_channel;
    uint8_t  spp_res01;
    uint16_t spp_res02;
} bttpsll_tws_sppgatt_info_t; // 6 words

typedef struct BTTPSLL_TWS_SYNC_INFO
{
    /*  set 1 when existing */
    uint8_t btp_a2dp_info    : 1;
    uint8_t btp_avrcp_info   : 1;
    uint8_t btp_hfp_info     : 1;
    uint8_t btp_sppgatt_info : 1;

    /*  set 1 when update */
    uint8_t btp_a2dp_info_update    : 1;
    uint8_t btp_avrcp_info_update   : 1;
    uint8_t btp_hfp_info_update     : 1;
    uint8_t btp_sppgatt_info_update : 1;

    uint8_t btp_a2dp_info_pending    : 1;
    uint8_t btp_avrcp_info_pending   : 1;
    uint8_t btp_hfp_info_pending     : 1;
    uint8_t btp_sppgatt_info_pending : 1;
    uint8_t btp_rsvd_update          : 4;

    uint16_t btp_sync_res;
} bttpsll_tws_sync_info_t; // 1 word

typedef struct BTTPSLL_TWS_HANDOVER_HOST_STATUS_INFO
{
    uint32_t hf_state           : 4;
    uint32_t hf_statusCall      : 4;
    uint32_t hf_statusCallSetup : 4;
    uint32_t hf_statusCallHeld  : 4;

    uint32_t avrcp_state         : 4;
    uint32_t siri_support        : 1;
    uint32_t siri_enabled        : 1;
    uint32_t voice_recog_support : 1; /* Voice Recognition */
    uint32_t is_bt_pairing       : 1;

    uint32_t is_bt_auto_recon         : 1;
    uint32_t avrcp_is_supp_set_volume : 1;
    uint32_t link_before_pair_flag    : 1;
    uint32_t dongleVoice              : 1;
    uint32_t scanState                : 2;
    uint32_t isDongleParing           : 2;

    uint8_t avrcp_trans_label_tg;
    uint8_t avrcp_status_ct;
    uint8_t hf_at_cmd_pending;
    uint8_t reconRetryNum;

    uint8_t hf_rfc_tx_credits;
    uint8_t hf_rfc_rx_credits;
    uint8_t spp_rfc_tx_credits;
    uint8_t spp_rfc_rx_credits;

    uint16_t l2_local_scid_end;
    uint16_t scanTimeout;

    uint16_t donglePairTimeout;
    uint16_t resv;
} bttpsll_tws_handover_host_status_info_t; // 20byte

typedef struct BTTPSLL_TWS_PROFILE_INFO
{
    bttpsll_tws_a2dpavrcp_info_t a2dpavrcp_info;

    bttpsll_tws_hfp_info_t hfp_info;

    bttpsll_tws_sppgatt_info_t sppgatt_info;

} __attribute__((packed)) bttpsll_tws_profile_info_t; // 14 words

typedef struct
{
    uint16_t status         : 2;
    uint16_t status_setup   : 2;
    uint16_t status_held    : 2;
    uint16_t isDongleParing : 2;
    uint16_t is_bt_recon    : 1; // if recon, retry and start recon
    uint16_t scanState      : 2; // inq, page, both
    uint16_t dongleVoice    : 1;
    uint16_t reconRetryNum  : 4; // max 15 retry
    uint16_t scanTimeout;

    uint16_t donglePairTimeout;
    uint16_t resv;
} btp_tws_get_bt_app_state_info_t; // 8byte

typedef enum
{
    /*  set 1 when existing */
    BTTPSLL_TWS_A2DP_INFO_BIT    = (1 << 0),
    BTTPSLL_TWS_AVRCP_INFO_BIT   = (1 << 1),
    BTTPSLL_TWS_HFP_INFO_BIT     = (1 << 2),
    BTTPSLL_TWS_SPPGATT_INFO_BIT = (1 << 3),

    /*  set 1 when update */
    BTTPSLL_TWS_A2DP_INFO_UPDATE_BIT    = (1 << 4),
    BTTPSLL_TWS_AVRCP_INFO_UPDATE_BIT   = (1 << 5),
    BTTPSLL_TWS_HFP_INFO_UPDATE_BIT     = (1 << 6),
    BTTPSLL_TWS_SPPGATT_INFO_UPDATE_BIT = (1 << 7),

    BTTPSLL_TWS_A2DP_PENDING_BIT    = (1 << 8),
    BTTPSLL_TWS_AVRCP_PENDING_BIT   = (1 << 9),
    BTTPSLL_TWS_HFP_PENDING_BIT     = (1 << 10),
    BTTPSLL_TWS_SPPGATT_PENDING_BIT = (1 << 11),
} bttpsll_tws_sync_bit_e; /* refer to btp_tws_sync_info_t */

typedef enum
{
    /*  only for log */
    LOG_FOR_SYNC_A2DP_INFO_BIT  = (1 << 0),
    LOG_FOR_SYNC_AVRCP_INFO_BIT = (1 << 1),
    LOG_FOR_SYNC_HFP_INFO_BIT   = (1 << 2),
    LOG_FOR_SYNC_SPP_INFO_BIT   = (1 << 3),
    LOG_FOR_SYNC_GATT_INFO_BIT  = (1 << 4),
} bttpsll_tws_sync_log_bit_e;

typedef enum
{
    BTTPSLL_TWS_SLAVE_BTP_A2DP_INFO = 0x0A,
    BTTPSLL_TWS_SLAVE_BTP_AVRCP_INFO,
    BTTPSLL_TWS_SLAVE_BTP_HFP_INFO,
    BTTPSLL_TWS_SLAVE_BTP_SPPGATT_INFO,
} bttpsll_tws_profile_type_e;

typedef enum
{
    BTTPSLL_TWS_LMP_CMD_BT_A2DP_INFO = 0x11,
    BTTPSLL_TWS_LMP_CMD_BT_AVRCP_INFO,
    BTTPSLL_TWS_LMP_CMD_BT_HFP_INFO,
    BTTPSLL_TWS_LMP_CMD_BT_SPPGATT_INFO,
} bttpsll_tws_pdu_cmd_lmp_e;

typedef enum
{
    BTTPSLL_TWS_MSG_LINK_ID_IDLE,
    BTTPSLL_TWS_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    BTTPSLL_TWS_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    BTTPSLL_TWS_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    BTTPSLL_TWS_MSG_LINK_ID_DONGLE,         /* dongle */
} bttpsll_tws_msg_dst_id_t;

typedef enum
{
    BTTPSLL_TWS_MSG_ID_IDLE = 0,
    BTTPSLL_TWS_MSG_ID_SETUP,
    BTTPSLL_TWS_MSG_ID_LMP,
    BTTPSLL_TWS_MSG_ID_ACL,
    BTTPSLL_TWS_MSG_ID_SCO,
} bttpsll_tws_msg_id_t;

#endif /* BTTPSLL_TWS_PROFILE_H_ */
