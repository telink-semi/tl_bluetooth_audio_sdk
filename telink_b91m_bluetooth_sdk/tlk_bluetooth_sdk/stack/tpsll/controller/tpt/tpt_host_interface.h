/********************************************************************************************************
 * @file    tpt_host_interface.h
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
#ifndef TLKSTK_TPT_HOST_INTERFACE_H_
#define TLKSTK_TPT_HOST_INTERFACE_H_
#include "tl_common.h"
#if (TLK_STK_TPT_ENABLE)


#define TPT_HOST_INTERFACE_DEBUG_EN              (1)
#define TPT_HOST_HEADSET_SETUP_COMMON_ACCESSCODE 0x9669c33c
#define TPT_HOST_HEADSET_SETUP_COMMON_CHN        0x13 //  2402+2*0x13=2440
#define TPT_HOST_DONGLE_SETUP_COMMON_ACCESSCODE  0x56291435
#define TPT_HOST_DONGLE_SETUP_COMMON_CHN         0x12 //  2402+2*0x10=2434
#define TPT_HOST_CONNECTED_AFH_COMMON_CHN        0x20
#define TPT_HOST_10MS_MIC_LC3_DATA_LEN           (30)

typedef enum
{
    TPT_HOST_AUDIO_FORMAT_MSBC     = 0,
    TPT_HOST_AUDIO_FORMAT_LC3A     = 1,
    TPT_HOST_AUDIO_FORMAT_16K_MASK = 0x04,
} tpt_audio_format_for_host_e;

typedef enum
{
    TPT_HOST_HEADSET_SETUP_MODE_IDLE,
    TPT_HOST_HEADSET_SETUP_MODE_NORMAL,
    TPT_HOST_HEADSET_SETUP_MODE_3S,
    TPT_HOST_HEADSET_SETUP_MODE_10S,
    TPT_HOST_DONGLE_SETUP_MODE_NORMAL,
    TPT_HOST_DONGLE_SETUP_MODE_PAIRING,
    TPT_HOST_DONGLE_SETUP_MODE_CC_HEADSET,
    TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY,
    TPT_HOST_HEADSET_SETUP_MODE_EXIT_ULTRA_LOW_LATENCY,
} tpt_headset_setup_mode_for_host_e;

typedef enum TPT_MODE_FOR_HOST
{
    TPT_HOST_MODE_BT_ACTIVE      = BIT(0),
    TPT_HOST_MODE_BT_MUSIC       = BIT(1),
    TPT_HOST_MODE_BT_VOICE       = BIT(2),
    TPT_HOST_MODE_BT_AUDIO       = BIT(2) | BIT(1),
    TPT_HOST_MODE_DONGLE_ACTIVE  = BIT(3),
    TPT_HOST_MODE_DONGLE_MUSIC   = BIT(4),
    TPT_HOST_MODE_DONGLE_PHONE   = BIT(5),
    TPT_HOST_MODE_DONGLE_AUDIO   = BIT(5) | BIT(4),
    TPT_HOST_MODE_HEADSET_LEFT   = BIT(6),
    TPT_HOST_MODE_HEADSET_ACTIVE = BIT(7),
} tpt_mode_for_host_e;

typedef enum
{
    TPT_HOST_DISCONNECT_REASON_NONE,
    TPT_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPT_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPT_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPT_HOST_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPT_HOST_DISCONNECT_REASON_DONGLE_SETUP,
    TPT_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
    TPT_HOST_DISCONNECT_REASON_ENTER_ULTRA_LOW_LATENCY_MODE,
    TPT_HOST_DISCONNECT_REASON_EXIT_ULTRA_LOW_LATENCY_MODE,
} tpt_disconnect_reason_for_host_e;

/* CMD: ACL */
typedef enum TPT_PDU_CMD_ACL_FOR_HOST
{
    TPT_HOST_MSG_PDU_ACL_CMD_IDLE,
    TPT_HOST_MSG_PDU_ACL_CMD_NULL,
    TPT_HOST_MSG_PDU_ACL_CMD_SYNC,
    TPT_HOST_MSG_PDU_ACL_CMD_TONE_SYNC,
    TPT_HOST_MSG_PDU_ACL_CMD_LED_SYNC,
    TPT_HOST_MSG_PDU_ACL_CMD_TEST,
    TPT_HOST_MSG_PDU_ACL_CMD_UI,
    TPT_HOST_MSG_PDU_ACL_CMD_PROFILE,
    TPT_HOST_MSG_PDU_ACL_CMD_DFU,
    TPT_HOST_MSG_PDU_ACL_CMD_APP,

    TPT_HOST_MSG_PDU_ACL_CMD_MAX,
} tpt_pdu_cmd_acl_for_host_e;

typedef enum
{
    TPT_HOST_MSG_ID_IDLE = 0,
    TPT_HOST_MSG_ID_SETUP,
    TPT_HOST_MSG_ID_LMP,
    TPT_HOST_MSG_ID_ACL,
    TPT_HOST_MSG_ID_SCO,
} tpt_msg_id_for_host_t;

typedef enum
{
    TPT_HOST_MSG_LINK_ID_IDLE,
    TPT_HOST_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    TPT_HOST_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    TPT_HOST_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    TPT_HOST_MSG_LINK_ID_DONGLE,         /* dongle */
} tpt_msg_dst_id_for_host_t;

typedef enum
{
    TPT_HOST_LOCAL_POWER_CONTROL,  /* host to controller */
    TPT_HOST_REMOTE_POWER_CONTROL, /* master to dongle / slave */
} tpt_power_control_obj_e;

typedef enum TPT_HOST_POWER_CONTROL_OPCODE
{
    TPT_HOST_INCR_POWER_REQ,
    TPT_HOST_DECR_POWER_REQ,
    TPT_HOST_SET_MAX_POWER,
    TPT_HOST_SET_MIN_POWER,
    TPT_HOST_SET_POWER_INDEX,
} tpt_host_pwr_control_opcode_e;

/**
 * @brief Enumeration defining values for simulate task enable or disable
 */
typedef enum
{
    TPT_HOST_TASK_DISABLE = 0x00,
    TPT_HOST_TASK_ENABLE  = 0x01,
} tpt_task_en_for_host_e;

typedef struct TPT_SCO_MSG_PDU_HEADER_FOR_HOST
{
    uint8_t data_format : 4;
    uint8_t data_wptr   : 4;
} __attribute__((packed)) __attribute__((aligned(4))) tpt_sco_msg_pdu_header_for_host_t;

/* callback */
typedef void (*tpt_dongle_mac_address_update_handler_cb_t)(uint8_t *mac_address);
extern tpt_dongle_mac_address_update_handler_cb_t gp_tlk_tpsll_tpt_dongle_mac_address_update_handler_cb;

/**
 * @brief       Registers a callback function for handling dongle MAC address updates.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_dongle_mac_address_update_handler_cb_register(tpt_dongle_mac_address_update_handler_cb_t callback)
{
    gp_tlk_tpsll_tpt_dongle_mac_address_update_handler_cb = callback;
}

typedef void (*tpt_dongle_connected_handler_cb_t)(void);
extern tpt_dongle_connected_handler_cb_t gp_tlk_tpsll_tpt_dongle_connected_handler_cb;

/**
 * @brief       Registers a callback function for handling dongle connection event.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_dongle_connected_handler_cb_register(tpt_dongle_connected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpt_dongle_connected_handler_cb = callback;
}

typedef void (*tpt_dongle_disconnected_handler_cb_t)(uint8_t disconnect_reason);
extern tpt_dongle_disconnected_handler_cb_t gp_tlk_tpsll_tpt_dongle_disconnected_handler_cb;

/**
 * @brief       Registers a callback function for handling dongle disconnection event.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_dongle_disconnected_handler_cb_register(tpt_dongle_disconnected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpt_dongle_disconnected_handler_cb = callback;
}

typedef void (*tpt_tws_headset_connected_handler_cb_t)(void);
extern tpt_tws_headset_connected_handler_cb_t gp_tlk_tpsll_tpt_tws_headset_connected_handler_cb;

/**
 * @brief       Registers a callback function for handling TWS headset connection event.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_tws_headset_connected_handler_cb_register(tpt_tws_headset_connected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpt_tws_headset_connected_handler_cb = callback;
}

typedef void (*tpt_tws_headset_disconnected_handler_cb_t)(uint8_t disconnect_reason);
extern tpt_tws_headset_disconnected_handler_cb_t gp_tlk_tpsll_tpt_tws_headset_disconnected_handler_cb;

/**
 * @brief       Registers a callback function for handling TWS headset disconnection event.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_tws_headset_disconnected_handler_cb_register(tpt_tws_headset_disconnected_handler_cb_t callback)
{
    gp_tlk_tpsll_tpt_tws_headset_disconnected_handler_cb = callback;
}

typedef uint8_t (*tpt_stimer_handler_callback_t)(uint32_t tick, uint8_t *p_des, uint16_t fno);
extern tpt_stimer_handler_callback_t gp_tlk_tpsll_tpt_stimer_handler_cb;

typedef uint8_t (*tpt_stimer_start_callback_t)(void);
extern tpt_stimer_start_callback_t gp_tlk_tpsll_tpt_stimer_start_cb;

/**
 * @brief       Registers a callback function for handling software timer start event.
 * @param[in]   callback - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_stimer_start_cb_register(tpt_stimer_start_callback_t callback)
{
    gp_tlk_tpsll_tpt_stimer_start_cb = callback;
}

typedef void (*tpt_rx_audio_data_callback_t)(uint32_t tick, tpt_sco_msg_pdu_header_for_host_t *p_pdu_format);
extern tpt_rx_audio_data_callback_t gp_tlk_tpsll_tpt_rx_audio_data_cb;

/**
 * @brief       Registers a callback function for handling audio data reception.
 * @param[in]   callback - the callback function to register.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tpt_rx_audio_data_cb_register(tpt_rx_audio_data_callback_t callback)
{
    gp_tlk_tpsll_tpt_rx_audio_data_cb = callback;
}

/**
 * @brief       Starts the connection setup process for the dongle.
 * @param[in]   setup_mode - the mode to set up the connection.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_start_connection_setup(uint8_t setup_mode);

/**
 * @brief       Initiates the disconnection process for the headset.
 * @param[in]   disconnect_reason - the reason for disconnecting the headset.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_start_disconnection(tpt_disconnect_reason_for_host_e disconnect_reason);

/**
 * @brief       Exits the connection setup process for the dongle.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_exit_connection_setup(void);

/**
 * @brief       Checks if the dongle connection setup is in progress.
 * @return      Boolean value indicating if the setup is ongoing.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_connection_setup_is_going(void);

/**
 * @brief       Exits the connection setup process for the headset.
 * @return      none.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_exit_connection_setup(void);

/**
 * @brief       Sets the pending status for the BT active mode.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_bt_active_pending(void);

/**
 * @brief       Checks if the BT ACL connection is established.
 * @return      Boolean value indicating if the connection is active.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_bt_acl_is_connected(void);

/**
 * @brief       Checks if there is BT audio data available from the headset.
 * @return      Boolean value indicating if audio data exists.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_bt_audio_is_existed(void);

/**
 * @brief       Checks if the headset is slave.
 * @return      Boolean value indicating if the headset is theslave.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_is_slave(void);

/**
 * @brief       Checks if the headset is the master.
 * @return      Boolean value indicating if the headset is the master.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_is_master(void);

/**
 * @brief       Sets the role of the device to single.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_role_set_single(void); // IDLE, or single

/**
 * @brief       Sets the specified saved mode for the headset.
 * @param[in]   mode - the mode to save.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_mode_save(uint8_t mode);

/**
 * @brief       Unsets the specified saved mode for the headset.
 * @param[in]   mode - the mode to unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_unset_mode_save(uint8_t mode);

/**
 * @brief       Sets the current mode for the headset.
 * @param[in]   mode - the mode to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_mode(uint8_t mode);

/**
 * @brief       Unsets the current mode for the headset.
 * @param[in]   mode - the mode to unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_unset_mode(uint8_t mode);

/**
 * @brief       Retrieves the current mode of the headset.
 * @return      The current mode of the headset.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_get_mode(void);

/**
 * @brief       Retrieves the saved mode of the headset.
 * @return      The saved mode of the headset.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_get_mode_save(void);

/**
 * @brief       Retrieves the frame number.
 * @return      The frame number.
 * @note
 */
extern uint16_t tlk_tpsll_tpt_env_get_fno(void);

/**
 * @brief       Checks if the headset is operating in single mode.
 * @return      Boolean value indicating if the headset role is single.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_role_is_single(void);

/**
 * @brief       Checks if the headset is the left.
 * @return      Boolean value indicating if the headset is the left.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_headset_is_left(void);

/**
 * @brief       Sets the left unit status for the headset.
 * @param[in]   left - the status to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_left(uint8_t left);

/**
 * @brief       Checks if the dongle is in idle state.
 * @return      Boolean value indicating if the dongle is idle.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_is_idle(void);

/**
 * @brief       Checks if the dongle is connected to a device.
 * @return      Boolean value indicating if the dongle is connected.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_is_connected(void);

/**
 * @brief       Checks if there is audio data available from the dongle.
 * @return      Boolean value indicating if audio data exists.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_audio_is_existed(void);

/**
 * @brief       Checks if the audio data from the dongle is music.
 * @return      Boolean value indicating if the audio data is music.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_audio_is_music(void);

/**
 * @brief       Checks if the audio data from the dongle is phone audio.
 * @return      Boolean value indicating if the audio data is phone audio.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_dongle_audio_is_phone(void);

/**
 * @brief       Checks if there is BT audio exusted.
 * @return      Boolean value indicating if BT audio exists.
 * @note
 */
extern int tlk_tpsll_tpt_bt_audio_is_existed(void);

/**
 * @brief       Checks if the BT audio is phone.
 * @return      Boolean value indicating if the BT audio is phone.
 * @note
 */
extern int tlk_tpsll_tpt_bt_audio_is_phone(void);

/**
 * @brief       Checks if the BT audio is music.
 * @return      Boolean value indicating if the BTudio is music.
 * @note
 */
extern int tlk_tpsll_tpt_bt_audio_is_music(void);

/**
 * @brief       Retrieves a SCO message from the receive FIFO.
 * @param[in]   rptr - the read pointer in the FIFO.
 * @return      Pointer to the retrieved SCO message.
 * @note
 */
extern uint8_t *tlk_tpsll_tpt_get_sco_msg_from_rxfifo(uint8_t rptr);

/**
 * @brief       Retrieves the number of available SCO messages in the receive FIFO.
 * @return      The number of available SCO messages.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_get_number_of_sco_msg_rxfifo_available(void);


/**
 * @brief       Pushes an ACL message to the transmit FIFO.
 * @param[in]   dst_id - the destination ID.
 * @param[in]   msg_id - the message ID.
 * @param[in]   cmd - the command.
 * @param[in]   p - pointer to the message data.
 * @param[in]   len - length of the message data.
 * @param[in]   callback - the callback function to invoke after pushing the message.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint32_t tlk_tpsll_tpt_acl_msg_push_txfifo(uint8_t dst_id, uint8_t msg_id, uint8_t cmd, uint8_t *p, uint8_t len,
                                                  void (*callback)(uint8_t dst_id, uint8_t msg_id, uint16_t cmd));

/**
 * @brief       Pushes an SCO message to the transmit FIFO.
 * @param[in]   format - the format of the SCO message.
 * @param[in]   idx - the index of the SCO message.
 * @param[in]   p - pointer to the message data.
 * @param[in]   len - length of the message data.
 * @param[in]   callback - the callback function to invoke after pushing the message.
 * @return      Status code indicating the success or failure of the operation.
 * @note
 */
extern uint32_t tlk_tpsll_tpt_sco_msg_push_txfifo(uint8_t format, uint8_t idx, uint8_t *p, uint8_t len, void (*callback)(uint8_t dst_id, uint8_t msg_id, uint16_t cmd));

/**
 * @brief       Sets the setup ACL channel for the dongle.
 * @param[in]   setup_ac - the ACL to set up.
 * @param[in]   setup_chn - the channel to set up.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_dongle_set_setup_ac_chn(uint32_t setup_ac, uint8_t setup_chn);

/**
 * @brief       Sets the local MAC address.
 * @param[in]   local_mac - the MAC address to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_local_mac(uint8_t *local_mac);

/**
 * @brief       Sets the generated MAC address.
 * @param[in]   gen_mac - the MAC address to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_gen_mac(uint8_t *gen_mac);

/**
 * @brief       Sets the dongle's MAC address.
 * @param[in]   dongle_mac - the MAC address to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_dongle_mac(uint8_t *dongle_mac);

/**
 * @brief       Sets the existence status of the dongle's MAC address.
 * @param[in]   en - the existence status to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_dongle_mac_existed(uint8_t en);

/**
 * @brief       Sets the disconnect status for OTA for the dongle.
 * @param[in]   en - the disconnect status to set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_headset_set_dg_disconnect_for_ota(uint8_t en);

/**
 * @brief       Retrieves the ACL channel from the MAC address.
 * @param[in]   mac - the MAC address.
 * @param[out]  ac - the ACL to retrieve.
 * @param[out]  chn - the channel to retrieve.
 * @return      Boolean value indicating the success or failure of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

/**
 * @brief       This function handles the PWM interrupt.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_pwm_irq_handler(void);

/**
 * @brief       This function handles the bbtimer interrupt.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_bbtimer_irq_handler(void);

/**
 * @brief       Sets a scheduled task for the host.
 * @param[in]   en - enables or disables the task.
 * @param[in]   interval_us - the interval in microseconds.
 * @param[in]   duration_unit - the duration unit.
 * @param[in]   duration_cnt - the duration count.
 * @param[in]   anchor_point_tick - the anchor point tick.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_host_set_schedule_task(uint8_t en, uint32_t interval_us, uint16_t duration_unit, uint8_t duration_cnt, uint32_t anchor_point_tick);

/**
 * @brief       Initializes the controller with the specified USB ID.
 * @param[in]   usb_id - the USB ID to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_controller_init(uint16_t usb_id);

/**
 * @brief       Controls the power.
 * @param[in]   obj - the power control object.
 * @param[in]   pwr_opcode - the power control opcode.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_host_power_control(tpt_power_control_obj_e obj, tpt_host_pwr_control_opcode_e pwr_opcode);

/**
 * @brief       Gets the TWS role.
 * @return      The TWS role.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_host_get_tws_role(void);

/**
 * @brief       Gets the TWS connection MAC addresses.
 * @param[out]  remote_mac - Pointer to store the remote MAC address.
 * @param[out]  target_mac - Pointer to store the target MAC address.
 * @param[out]  gen_mac - Pointer to store the generated MAC address.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tpt_host_get_tws_connect_mac(uint8_t *remote_mac, uint8_t *target_mac, uint8_t *gen_mac);

/**
 * @brief       Gets the handover host status info.
 * @param[out]  p_host_status_info - Pointer to store the handover host status info.
 * @return      The size of the handover host status info.
 * @note
 */
extern uint8_t tlk_tpsll_tpt_host_get_handover_host_info(uint8_t *p_host_status_info);

#endif
#endif //TLKSTK_TPT_HOST_INTERFACE_H_
