/********************************************************************************************************
 * @file    tpd_host_interface.h
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
#ifndef TLKSTK_TPD_HOST_INTERFACE_H_
#define TLKSTK_TPD_HOST_INTERFACE_H_
#include "common/types.h"
#include "common/bit.h"
#include "drivers.h"
#if (TLK_STK_TPD_ENABLE)

#define TPD_HOST_INTERFACE_DEBUG_EN              (1)
#define TPD_HOST_HEADSET_SETUP_COMMON_ACCESSCODE 0x9669c33c
#define TPD_HOST_HEADSET_SETUP_COMMON_CHN        0x13 //  2402+2*0x13=2440
#define TPD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE  0x56291435
#define TPD_HOST_DONGLE_SETUP_COMMON_CHN         0x12 //  2402+2*0x10=2434
#define TPD_HOST_CONNECTED_AFH_COMMON_CHN        0x20
#define TPD_HOST_10MS_MIC_LC3_DATA_LEN           (30)

typedef enum TPD_HOST_DG_AUDIO_PATH_STATUS
{
    TPD_HOST_DG_AUDIO_PATH_STATUS_IDLE,
    TPD_HOST_DG_AUDIO_PATH_STATUS_RUNNING,
} tpd_dongle_host_audio_status_e;

typedef enum TPD_HOST_DG_SCO_AUDIO_MODE
{
    TPD_HOST_DG_SCO_AUDIO_MODE_IDLE,
    TPD_HOST_DG_SCO_AUDIO_MODE_SPEAKER,
    TPD_HOST_DG_SCO_AUDIO_MODE_MIC,
} tpd_dongle_host_sco_audio_mode_e;

typedef enum TPD_MODE_FOR_HOST
{
    TPD_HOST_MODE_BT_ACTIVE      = BIT(0),
    TPD_HOST_MODE_BT_MUSIC       = BIT(1),
    TPD_HOST_MODE_BT_VOICE       = BIT(2),
    TPD_HOST_MODE_BT_AUDIO       = BIT(2) | BIT(1),
    TPD_HOST_MODE_DONGLE_ACTIVE  = BIT(3),
    TPD_HOST_MODE_DONGLE_MUSIC   = BIT(4),
    TPD_HOST_MODE_DONGLE_PHONE   = BIT(5),
    TPD_HOST_MODE_DONGLE_AUDIO   = BIT(5) | BIT(4),
    TPD_HOST_MODE_HEADSET_LEFT   = BIT(6),
    TPD_HOST_MODE_HEADSET_ACTIVE = BIT(7),
} tpd_mode_for_host_e;

/* CMD: ACL */
typedef enum TPD_PDU_CMD_ACL_FOR_HOST
{
    TPD_HOST_MSG_PDU_ACL_CMD_IDLE,
    TPD_HOST_MSG_PDU_ACL_CMD_NULL,
    TPD_HOST_MSG_PDU_ACL_CMD_SYNC,
    TPD_HOST_MSG_PDU_ACL_CMD_TONE_SYNC,
    TPD_HOST_MSG_PDU_ACL_CMD_LED_SYNC,
    TPD_HOST_MSG_PDU_ACL_CMD_TEST,
    TPD_HOST_MSG_PDU_ACL_CMD_UI,
    TPD_HOST_MSG_PDU_ACL_CMD_PROFILE,
    TPD_HOST_MSG_PDU_ACL_CMD_DFU,
    TPD_HOST_MSG_PDU_ACL_CMD_APP,

    TPD_HOST_MSG_PDU_ACL_CMD_MAX,
} tpd_pdu_cmd_acl_for_host_e;

typedef enum
{
    TPD_HOST_MSG_ID_IDLE = 0,
    TPD_HOST_MSG_ID_SETUP,
    TPD_HOST_MSG_ID_LMP,
    TPD_HOST_MSG_ID_ACL,
    TPD_HOST_MSG_ID_SCO,
} tpd_msg_id_for_host_t;

typedef enum
{
    TPD_HOST_MSG_LINK_ID_IDLE,
    TPD_HOST_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    TPD_HOST_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    TPD_HOST_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    TPD_HOST_MSG_LINK_ID_DONGLE,         /* dongle */
} tpd_msg_dst_id_for_host_t;

typedef enum
{
    TPD_HOST_STATUS_UNSNIFF,
    TPD_HOST_STATUS_SNIFF_ENTER,
    TPD_HOST_STATUS_SNIFF,
    TPD_HOST_STATUS_SNIFF_EXIT,
} tpd_sniff_state_for_host_e;

typedef enum
{
    TPD_DISCONNECT_REASON_NONE,
    TPD_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPD_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPD_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPD_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPD_DISCONNECT_REASON_DONGLE_SETUP,
    TPD_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
    TPD_DISCONNECT_REASON_ENTER_ULTRA_LOW_LATENCY_MODE,
    TPD_DISCONNECT_REASON_EXIT_ULTRA_LOW_LATENCY_MODE,
    TPD_DISCONNECT_REASON_USB_ENTER_SUSPEND,
} tpd_disconnect_reason_e;

typedef enum
{
    AUDIO_CHANNEL_LEFT  = 0,
    AUDIO_CHANNEL_RIGHT = 1,
} audio_channel_e;

typedef enum
{
    TPD_SET_AUDIO_PATH_IDLE       = 0x00,
    TPD_STIMER_START_EVENT        = 0x01,
    TPD_DG_SCO_SETUP_SUCCESS      = 0x02,
    TPD_DG_EXIT_SCO_COMPLETE      = 0x03,
    TPD_SET_DG_AUDIO_PATH_PENDING = 0x04,
} tpd_audio_path_cmd_e;

typedef enum
{
    TPD_AUDIO_FORMAT_MSBC          = 0,
    TPD_AUDIO_FORMAT_LC3A          = 1,
    TPD_AUDIO_FORMAT_MASK          = 0x01,
    TPD_AUDIO_FORMAT_16K_MASK      = 0x04,
    TPD_AUDIO_FORMAT_USB_MUTE_MASK = 0x08,
} tpd_audio_format_e;

typedef struct TPD_SCO_MSG_PDU_HEADER_FOR_HOST
{
    uint8_t data_format : 4;
    uint8_t data_wptr   : 4;
} __attribute__((packed)) __attribute__((aligned(4))) tpd_sco_msg_pdu_header_for_host_t;

typedef uint8_t (*tpd_dongle_audio_path_callback_t)(uint16_t cmd);
extern tpd_dongle_audio_path_callback_t gp_tlk_tpsll_tpd_audio_path_cb;

/**
 * @brief       Registers a callback function for audio path.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_audio_path_cb_register(tpd_dongle_audio_path_callback_t callback)
{
    gp_tlk_tpsll_tpd_audio_path_cb = callback;
}

typedef void (*tpd_cmd_ui_handler_cb_t)(uint8_t *cmd);
extern tpd_cmd_ui_handler_cb_t gp_tlk_tpsll_tpd_cmd_ui_handler_cb;

typedef void (*tpd_cmd_dfu_handler_cb_t)(uint32_t task_id, uint8_t *pData, uint16_t dataLen);
extern tpd_cmd_dfu_handler_cb_t gp_tlk_tpsll_tpd_cmd_dfu_handler_cb;

typedef void (*tpd_cmd_app_handler_cb_t)(uint32_t task_id, uint8_t *pData, uint16_t dataLen);
extern tpd_cmd_app_handler_cb_t gp_tlk_tpsll_tpd_cmd_app_handler_cb;

typedef void (*tpd_gip_mode_ready_handler_cb_t)(void);
extern tpd_gip_mode_ready_handler_cb_t gp_tlk_tpsll_tpd_gip_mode_ready_handler_cb;

typedef void (*tpd_headset_controller_traffic_notify_cb_t)(uint8_t controller_traffic_state);
extern tpd_headset_controller_traffic_notify_cb_t gp_tlk_tpsll_tpd_controller_traffic_notify_cb;

__INLINE void tlk_tpsll_tpd_controller_traffic_notify_cb_register(tpd_headset_controller_traffic_notify_cb_t callback)
{
    gp_tlk_tpsll_tpd_controller_traffic_notify_cb = callback;
}

/**
 * @brief       Registers a callback function for handling UI commands.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_cmd_ui_handler_cb_register(tpd_cmd_ui_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_cmd_ui_handler_cb = callback;
}

/**
 * @brief       Registers a callback function for handling DFU data.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_cmd_dfu_handler_cb_register(tpd_cmd_dfu_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_cmd_dfu_handler_cb = callback;
}

/**
 * @brief       Registers a callback function for handling app data.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_cmd_app_handler_cb_register(tpd_cmd_app_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_cmd_app_handler_cb = callback;
}

/**
 * @brief       Registers a callback function for report enter gip mode.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_gip_mode_ready_handler_cb_register(tpd_gip_mode_ready_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_gip_mode_ready_handler_cb = callback;
}

/**
 * @brief       Retrieves the position of SCO in BT voice.
 * @return      The position of SCO in BT voice.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_get_sco_pos_in_bt_voice(void);

/**
 * @brief       Retrieves the current dongle mode.
 * @return      The dongle mode.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_get_dongle_mode(void);

/**
 * @brief       Starts the connection scan for the dongle.
 * @return      Status of the connection scan initiation.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_dongle_start_connection_scan(void);

/**
 * @brief       Starts SCO setup with the specified audio mode.
 * @param[in]   audio_mode - the audio mode to be set.
 * @return      Status of the SCO setup initiation.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_start_dongle_sco_setup(uint8_t audio_mode);

/**
 * @brief       Exits the current SCO connection.
 * @return      Status of the SCO exit operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_exit_dongle_sco(void);

/**
 * @brief       Switches the SCO audio mode.
 * @param[in]   audio_mode - the new audio mode to be set.
 * @return      Status of the audio mode switch.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_switch_dongle_sco_audio_mode(uint8_t audio_mode);

/**
 * @brief       Checks if the dongle is currently connected.
 * @return      Connection status of the dongle.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_dongle_is_connected(void);

/**
 * @brief       Handles PWM interrupt.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_pwm_irq_handler(void);

/**
 * @brief       Sets up a scheduled task.
 * @param[in]   en - enable or disable the task.
 * @param[in]   interval_us - interval in microseconds for the task.
 * @param[in]   duration_unit - duration unit for the task.
 * @param[in]   duration_cnt - duration count for the task.
 * @param[in]   anchor_point_tick - anchor point for the task.
 * @return      none.
 * @note
 */
void tlk_tpsll_tpd_host_set_schedule_task(uint8_t en, uint32_t interval_us, uint16_t duration_unit, uint8_t duration_cnt, uint32_t anchor_point_tick);

/**
 * @brief       Initializes the controller.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_controller_init(void);

/**
 * @brief       Sets up the AC and channel for the dongle.
 * @param[in]   setup_ac - the AC to be set.
 * @param[in]   setup_chn - the channel to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_dongle_set_setup_ac_chn(uint32_t setup_ac, uint8_t setup_chn);

/**
 * @brief       Retrieves the AC and channel from the MAC address.
 * @param[in]   mac - the MAC address.
 * @param[out]  ac - the retrieved AC.
 * @param[out]  chn - the retrieved channel.
 * @return      The AC and channel corresponding to the MAC address.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

/**
 * @brief       Sets the MAC address for the headset.
 * @param[in]   dongle_mac - the MAC address to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_dongle_set_headset_mac(uint8_t *dongle_mac);

/**
 * @brief       Retrieves the frame number.
 * @return      The frame number.
 * @note
 */
extern uint16_t tlk_tpsll_tpd_host_tpd_env_get_fno(void);

/**
 * @brief       Sets the local MAC address.
 * @param[in]   local_mac - the local MAC address to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_dongle_set_local_mac(uint8_t *local_mac);


typedef void (*tpd_headset_connected_handler_cb_t)(uint8_t *mac_address);
extern tpd_headset_connected_handler_cb_t gp_tlk_tpsll_tpd_headset_connected_handler_cb;

/**
 * @brief       Registers a callback function for handling headset connection.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_headset_connected_handler_cb_register(tpd_headset_connected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_headset_connected_handler_cb = callback;
}

typedef void (*tpd_headset_disconnected_handler_cb_t)(uint8_t disconnect_reason);
extern tpd_headset_disconnected_handler_cb_t gp_tlk_tpsll_tpd_headset_disconnected_handler_cb;

/**
 * @brief       Registers a callback function for handling headset disconnection.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpd_host_headset_disconnected_handler_cb_register(tpd_headset_disconnected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpd_headset_disconnected_handler_cb = callback;
}

/**
 * @brief       Checks if a CC headset is connected.
 * @return      Connection status of the CC headset.
 * @note
 */
uint8_t tlk_tpsll_tpd_host_headset_is_cc_headset_connected(void);

/**
 * @brief       Determines if the master headset is on the left side.
 * @return      Master headset position (left or not).
 * @note
 */
uint8_t tlk_tpsll_tpd_host_headset_master_is_left(void);

/**
 * @brief       Retrieves a pointer to the data in the mic voice data buffer for processing.
 * @return      Pointer to the data packet for processing.
 * @note
 */
extern uint8_t *tlk_tpsll_tpd_dongle_get_mic_data_ptr(void);

/**
 * @brief       Retrieves the setup AC and channel for the headset.
 * @param[out]  setup_ac - the retrieved AC.
 * @param[out]  setup_chn - the retrieved channel.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_headset_get_setup_ac_chn(uint32_t *setup_ac, uint8_t *setup_chn);

/**
 * @brief       Retrieves the generated MAC address for the headset.
 * @param[out]  gen_mac - the generated MAC address.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_headset_get_gen_mac(uint8_t *gen_mac);

/**
 * @brief       Checks if dongle audio exists.
 * @return      Existence status of dongle audio.
 * @note
 */
extern int tlk_tpsll_tpd_host_dongle_audio_is_existed(void);

/**
 * @brief       Pushes SCO music message to the transmit FIFO.
 * @param[in]   format - format of the message.
 * @param[in]   chnl - channel for the message.
 * @param[in]   len - length of the message.
 * @param[in]   callback - callback function for the message.
 * @return      Pointer to the data packet that was pushed.
 * @note
 */
extern uint8_t *tlk_tpsll_tpd_sco_music_msg_push_txfifo(uint8_t format, uint8_t chnl, uint8_t len, void (*callback)(uint8_t dst_id, uint8_t msg_id, uint16_t cmd));

/**
 * @brief       Handles the completion of message transmission.
 * @param[in]   dst_id - destination ID of the message.
 * @param[in]   msg_id - ID of the message.
 * @param[in]   cmd - command associated with the message.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_msg_tx_done_handler(uint8_t dst_id, uint8_t msg_id, uint16_t origin_cmd);

/**
 * @brief       Indicates completion of pushing SCO music message to the transmit FIFO.
 * @param[in]   chnl - channel for the message.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_sco_music_msg_push_txfifo_done(uint8_t chnl);

/**
 * @brief       Checks if BT audio exists.
 * @return      Existence status of BT audio.
 * @note
 */
extern int tlk_tpsll_tpd_host_bt_audio_is_existed(void);

/**
 * @brief       Checks if the headset is in single role mode.
 * @return      Single role mode status of the headset.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_headset_is_single_role(void);

/**
 * @brief       Starts the disconnection process for the dongle.
 * @param[in]   disconnect_reason - reason for disconnection.
 * @return      Status of the disconnection start operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_dongle_start_disconnection(tpd_disconnect_reason_e disconnect_reason);

/**
 * @brief       Sets the power index.
 * @param[in]   pwr_idx - the power index to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_set_power_index(rf_power_level_index_e pwr_idx);

/**
 * @brief       Sets the audio path status.
 * @param[in]   is_running - status indicating if the audio path is running.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_set_audio_path_status(uint8_t is_running);

/**
 * @brief       Checks if the host is in ultra latency mode.
 * @return      Ultra latency mode status of the host.
 * @note
 */
extern uint8_t tlk_tpsll_tpd_host_is_ultra_latency_mode(void);

/**
 * @brief       Checks if the host is in sniff status.
 * @return      Sniff status of the host.
 * @note
 */
extern bool tlk_tpsll_tpd_host_is_sniff_status(void);

/**
 * @brief       Requests the host to exit sniff mode.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_unsniff_request(void);

/**
 * @brief       Requests the host to enter sniff mode.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpd_host_sniff_request(void);

extern void tlk_tpsll_tpd_host_start_tpsll_task(void);

extern void tlk_tpsll_tpd_host_remove_tpsll_task(void);

/*
 * @brief       Sends ACL data to the tpd controller.
 * @param[in]   cmd - command for the ACL data, refer to tpd_pdu_cmd_acl_for_host_e.
 * @param[in]   data - pointer to the data to be sent.
 * @param[in]   dataLen - length of the data to be sent.
 * @param[in]   callback - callback function for the ACL data.
 */
extern void tlk_tpsll_tpd_host_send_acl_data(uint8_t cmd, uint8_t *data, uint16_t dataLen, void (*callback)(uint8_t dst_id, uint8_t msg_id, uint16_t cmd));

/*
 * @brief       Gets ACL data Maximum-Transmission Unit of tpd controller.
 * @param[in]   none.
 * @return      Maximum-Transmission Unit of the tpd acl link.
 */
extern uint16_t tlk_tpsll_tpd_host_get_acl_mtu(void);

extern void tlk_tpsll_tpd_host_set_usb_suspend_state(uint8_t en);

extern uint8_t tlk_tpsll_tpd_host_get_ota_state(void);

extern void tlk_tpsll_tpd_host_update_dg_ota_status(uint8_t is_ota_running);

extern bool tlk_tpsll_tpd_host_per_statistics_ctrl(uint8_t cmd);

extern bool tlk_tpsll_tpd_host_start_gip_mode(void);

extern bool tlk_tpsll_tpd_host_exit_gip_mode(void);

#endif //TLKSTK_TPD_HOST_INTERFACE_H_
#endif
