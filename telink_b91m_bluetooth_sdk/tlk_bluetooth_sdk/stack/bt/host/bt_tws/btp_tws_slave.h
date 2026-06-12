/********************************************************************************************************
 * @file    btp_tws_slave.h
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
#ifndef BTP_TWS_SLAVE_H_
#define BTP_TWS_SLAVE_H_

#include "stack/tpsll/common/bttpsll_tws_profile.h"

/**
 * @brief  Build acl link in host called by tws slave.
 *
 * @param[in] f callback founction.
 *
 * @returns none.
 */
extern uint8_t bth_tws_build_acl_link_handle(uint16_t acl_handle);

/**
 * @brief  Build a2dp link in host called by tws slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns whether build is success.
 */
extern uint8_t btp_tws_build_a2dp_snk_link_handle(uint16_t acl_handle, bttpsll_tws_a2dpavrcp_info_t *a2dpInfo);

/**
 * @brief  Build avdtp media channel link in host called by tws slave for update media link(sbc aac switch during play
 * music).
 *
 * @param[in] acl_handle acl link handle.
 * @param[in] in_buf new link info.
 *
 * @returns none.
 */
uint8_t btp_tws_avdtp_media_channel_handle(uint16_t acl_handle, bttpsll_tws_avdtp_media_chn_t *in_buf);

/**
 * @brief  Build avrcp link in host called by tws slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns whether build is success.
 */
extern uint8_t btp_tws_build_avrcp_link_handle(uint16_t acl_handle, bttpsll_tws_a2dpavrcp_info_t *avrcpInfo);

/**
 * @brief  Build hfp link in host called by tws slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns whether build is success.
 */
extern uint8_t btp_tws_build_hfp_hf_link_handle(uint16_t acl_handle, bttpsll_tws_hfp_info_t *hfpInfo);

/**
 * @brief  Build spp link in host called by tws slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns whether build is success.
 */
extern uint8_t btp_tws_build_spp_link_handle(uint16_t acl_handle, bttpsll_tws_sppgatt_info_t *sppInfo);

/**
 * @brief  Build gatt link in host called by tws slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns whether build is success.
 */

extern uint8_t btp_tws_build_gatt_link_handle(uint16_t acl_handle, bttpsll_tws_sppgatt_info_t *gattInfo);

/**
 * @brief  Set the status and parameters in tws slave headset, then will change as master.
 *
 * @param[in] p tws_handover_cmd_t contents.
 * @param[in] len length of p.
 *
 * @returns none.
 */
_attribute_bt_ram_code_ void btp_tws_slave_handover_host_status_sync(uint8_t *pBtAddr, bttpsll_tws_handover_host_status_info_t *bt_host_info);

/**
 * @brief  Tws master headset extract the handover info for send to slave
 *         The contents refer to the tws_handover_cmd_t structure.
 * @param[in] none.
 *
 * @returns none.
 */
_attribute_bt_ram_code_ void btp_handover_info_extract(uint8_t *pBtAddr);

/**
 * @brief  Set the a2dp stream status, sync form tws master, called by salve.
 *
 * @param[in] none.
 *
 * @returns none.
 */
extern void btp_tws_set_a2dp_stream_status(void);

extern void btp_tws_slave_a2dp_event_handler(bttpsll_tws_sync_info_t *slave_profile, bttpsll_tws_a2dpavrcp_info_t *a2dpInfo);

extern void btp_tws_slave_avrcp_event_handler(bttpsll_tws_sync_info_t *slave_profile, bttpsll_tws_a2dpavrcp_info_t *avrcpInfo);

extern void btp_tws_slave_hfp_event_handler(bttpsll_tws_sync_info_t *slave_profile, bttpsll_tws_hfp_info_t *hfpInfo);

extern void btp_tws_slave_sppgatt_event_handler(bttpsll_tws_sync_info_t *slave_profile, bttpsll_tws_sppgatt_info_t *sppgattInfo);

/**
 * @brief  register the callback for set app data when handover succeed.
 *
 * @param[in] callback  the callback : set app data.
 *
 * @returns None.
 */
extern void btp_tws_handover_set_app_userdata_callback_register(void (*callback)(bttpsll_tws_handover_host_status_info_t));


#endif /* BTP_TWS_SLAVE_H_ */
