/********************************************************************************************************
 * @file    btp_tws_master.h
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
#ifndef BTP_TWS_MASTER_H_
#define BTP_TWS_MASTER_H_

#include "stack/tpsll/common/bttpsll_tws_profile.h"
/**********************     master sync. profile with slave    *************************/

typedef void (*btp_tws_get_bt_app_stat_info_cb_t)(uint16_t aclHandle, btp_tws_get_bt_app_state_info_t *pInfo);

/**
 * @brief  Tws master headset extract the a2dp link info for send to slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns Whether extracet is success.
 */
extern uint8_t btp_tws_a2dp_snk_link_info_extract(uint16_t acl_handle);

/**
 * @brief  Tws master headset extract the a2dp media link info send to slave.
 *
 * @param[in] acl_handle acl link handle.
 * @param[out] out_buf link info data.
 *
 * @returns Whether extracet is success.
 */
uint8_t btp_tws_avdtp_media_channel_extract(uint16_t acl_handle, bttpsll_tws_avdtp_media_chn_t *out_buf);

/**
 * @brief  Tws master headset extract the avrp link info for send to slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns Whether extracet is success.
 */
extern uint8_t btp_tws_avrcp_link_info_extract(uint16_t acl_handle);

/**
 * @brief  Tws master headset extract the hfp and rfcomm link info for send to slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns Whether extracet is success.
 */
extern uint8_t btp_tws_hfp_rfcomm_link_info_extract(uint16_t acl_handle);

/**
 * @brief  Tws master headset extract the spp and rfcomm link info for send to slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns Whether extracet is success.
 */
extern uint8_t btp_tws_spp_rfcomm_link_info_extract(uint16_t acl_handle);

/**
 * @brief  Tws master headset extract the gatt link info for send to slave.
 *
 * @param[in] acl_handle acl link handle.
 *
 * @returns Whether extracet is success.
 */
extern uint8_t btp_tws_gatt_link_info_extract(uint16_t acl_handle);


extern bool btp_tws_master_sync_profile_handler(uint8_t *pAddr, uint16_t *s_profile_info, uint16_t *m_profile_info);
/**
 * @brief  Tws master headset extract the handover info for send to slave
 *         The contents refer to the tws_handover_cmd_t structure.
 * @param[in] The btaddr.
 *
 * @returns none.
 */
_attribute_bt_ram_code_ void btp_handover_info_extract(uint8_t *pBtAddr);

/**
 * @brief  Tws set sync info flag.
 *
 * @param[in] info_bit  profile bit, refer to 'btp_tws_sync_bit_e'.
 * @param[in] true_or_false  TRUE or FALSE.
 *
 * @returns None.
 */
extern void btp_set_tws_sync_info_flag(bttpsll_tws_sync_bit_e info_bit, uint8_t true_or_false);

/**
 * @brief  Tws get sync info flag.
 *
 * @param[in] None.
 *
 * @returns profile bit, refer to 'btp_tws_sync_bit_e'.
 */
extern uint16_t btp_get_tws_sync_info_flag(void);


extern void btp_tws_get_bt_app_state_info_register(btp_tws_get_bt_app_stat_info_cb_t cb);

/**
 * @brief  when master extract the link info to slave, it will print log in mainloop.
 *
 * @param[in] None .
 *
 * @returns None.
 */
void log_tws_master_sync_info_in_mainloop(void);

void btp_tws_set_rfcommChnID(uint8_t chnID, uint8_t isHfp);

#endif /* BTP_TWS_MASTER_H_ */
