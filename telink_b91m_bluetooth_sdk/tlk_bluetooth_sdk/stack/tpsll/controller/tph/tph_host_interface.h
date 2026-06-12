/********************************************************************************************************
 * @file    tph_host_interface.h
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
#ifndef TLKSTK_TPH_HOST_INTERFACE_H_
#define TLKSTK_TPH_HOST_INTERFACE_H_

#include "tl_common.h"
#if (TLK_STK_TPH_ENABLE)

#define TPH_HOST_INTERFACE_DEBUG_EN              (1)
#define TPH_HOST_HEADSET_SETUP_COMMON_ACCESSCODE 0x9669c33c
#define TPH_HOST_HEADSET_SETUP_COMMON_CHN        0x13 //  2402+2*0x13=2440
#define TPH_HOST_DONGLE_SETUP_COMMON_ACCESSCODE  0x56291435
#define TPH_HOST_DONGLE_SETUP_COMMON_CHN         0x12 //  2402+2*0x10=2434
#define TPH_HOST_CONNECTED_AFH_COMMON_CHN        0x20
#define TPH_HOST_10MS_MIC_LC3_DATA_LEN           (30)

typedef enum
{
    TPH_HOST_LOCAL_POWER_CONTROL,  /* host to controller */
    TPH_HOST_REMOTE_POWER_CONTROL, /* master to dongle / slave */
} tph_host_power_control_obj_e;

typedef enum TPH_HOST_POWER_CONTROL_OPCODE
{
    TPH_HOST_INCR_POWER_REQ,
    TPH_HOST_DECR_POWER_REQ,
    TPH_HOST_SET_MAX_POWER,
    TPH_HOST_SET_MIN_POWER,
    TPH_HOST_SET_POWER_INDEX,
} tph_host_pwr_control_opcode_e;

typedef enum
{
    TPH_HOST_AUDIO_FORMAT_MSBC     = 0,
    TPH_HOST_AUDIO_FORMAT_LC3A     = 1,
    TPH_HOST_AUDIO_FORMAT_16K_MASK = 0x04,
} tph_audio_format_for_host_e;

typedef enum
{
    TPH_HOST_HEADSET_SETUP_MODE_IDLE,
    TPH_HOST_HEADSET_SETUP_MODE_NORMAL,
    TPH_HOST_HEADSET_SETUP_MODE_3S,
    TPH_HOST_HEADSET_SETUP_MODE_10S,
    TPH_HOST_DONGLE_SETUP_MODE_NORMAL,
    TPH_HOST_DONGLE_SETUP_MODE_PAIRING,
    TPH_HOST_DONGLE_SETUP_MODE_CC_HEADSET,
    TPH_HOST_DONGLE_SETUP_MODE_ULTRA_LOW_LATENCY,
    TPH_HOST_DONGLE_SETUP_MODE_EXIT_ULTRA_LOW_LATENCY,
} tph_headset_setup_mode_for_host_e;

typedef enum TPH_MODE_FOR_HOST
{
    TPH_HOST_MODE_BT_ACTIVE      = BIT(0),
    TPH_HOST_MODE_BT_MUSIC       = BIT(1),
    TPH_HOST_MODE_BT_VOICE       = BIT(2),
    TPH_HOST_MODE_BT_AUDIO       = BIT(2) | BIT(1),
    TPH_HOST_MODE_DONGLE_ACTIVE  = BIT(3),
    TPH_HOST_MODE_DONGLE_MUSIC   = BIT(4),
    TPH_HOST_MODE_DONGLE_PHONE   = BIT(5),
    TPH_HOST_MODE_DONGLE_AUDIO   = BIT(5) | BIT(4),
    TPH_HOST_MODE_HEADSET_LEFT   = BIT(6),
    TPH_HOST_MODE_HEADSET_ACTIVE = BIT(7),
} tph_mode_for_host_e;

typedef enum
{
    TPH_HOST_DISCONNECT_REASON_NONE,
    TPH_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPH_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPH_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPH_HOST_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPH_HOST_DISCONNECT_REASON_DONGLE_SETUP,
    TPH_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
    TPH_HOST_DISCONNECT_REASON_HEADSET_ENTER_ULTRA_LOW_LATENCY,
    TPH_HOST_DISCONNECT_REASON_HEADSET_EXIT_ULTRA_LOW_LATENCY,
    TPH_HOST_DISCONNECT_REASON_DONGLE_USB_ENTER_SUSPEND,
} tph_disconnect_reason_for_host_e;

/* CMD: ACL */
typedef enum TPH_PDU_CMD_ACL_FOR_HOST
{
    TPH_HOST_MSG_PDU_ACL_CMD_IDLE,
    TPH_HOST_MSG_PDU_ACL_CMD_NULL,
    TPH_HOST_MSG_PDU_ACL_CMD_SYNC,
    TPH_HOST_MSG_PDU_ACL_CMD_TONE_SYNC,
    TPH_HOST_MSG_PDU_ACL_CMD_LED_SYNC,
    TPH_HOST_MSG_PDU_ACL_CMD_TEST,
    TPH_HOST_MSG_PDU_ACL_CMD_UI,
    TPH_HOST_MSG_PDU_ACL_CMD_PROFILE,
    TPH_HOST_MSG_PDU_ACL_CMD_DFU,
    TPH_HOST_MSG_PDU_ACL_CMD_APP,

    TPH_HOST_MSG_PDU_ACL_CMD_MAX,
} tph_pdu_cmd_acl_for_host_e;

typedef enum
{
    TPH_HOST_MSG_ID_IDLE = 0,
    TPH_HOST_MSG_ID_SETUP,
    TPH_HOST_MSG_ID_LMP,
    TPH_HOST_MSG_ID_ACL,
    TPH_HOST_MSG_ID_SCO,
} tph_msg_id_for_host_t;

typedef enum
{
    TPH_HOST_MSG_LINK_ID_IDLE,
    TPH_HOST_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    TPH_HOST_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    TPH_HOST_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    TPH_HOST_MSG_LINK_ID_DONGLE,         /* dongle */
} tph_msg_dst_id_for_host_t;

/**
 * @brief Enumeration defining values for simulate task enable or disable
 */
typedef enum
{
    TPH_HOST_TASK_DISABLE = 0x00,
    TPH_HOST_TASK_ENABLE  = 0x01,
} tph_task_en_for_host_e;

typedef struct TPH_SCO_MSG_PDU_HEADER_FOR_HOST
{
    uint8_t data_format : 4;
    uint8_t data_wptr   : 4;
} __attribute__((packed)) __attribute__((aligned(4))) tph_sco_msg_pdu_header_for_host_t;

/* callback */
typedef int (*tph_mode_change_handler_cb_t)(uint8_t *mode, uint16_t len);
extern tph_mode_change_handler_cb_t gp_tlk_tpsll_tph_mode_change_handler_cb;

/**
 * @brief       This function registers a callback for handling mode change.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_mode_change_handler_cb_register(tph_mode_change_handler_cb_t callback)
{
    gp_tlk_tpsll_tph_mode_change_handler_cb = callback;
}

typedef int (*tph_dongle_mac_address_update_handler_cb_t)(uint8_t *mac_address, uint16_t len);
extern tph_dongle_mac_address_update_handler_cb_t gp_tlk_tpsll_tph_dongle_mac_address_update_handler_cb;

/**
 * @brief       This function registers a callback for handling dongle MAC address update.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_dongle_mac_address_update_handler_cb_register(tph_dongle_mac_address_update_handler_cb_t callback)
{
    gp_tlk_tpsll_tph_dongle_mac_address_update_handler_cb = callback;
}

typedef int (*tph_dongle_connected_handler_cb_t)(uint8_t *data, uint16_t len);
extern tph_dongle_connected_handler_cb_t gp_tlk_tpsll_tph_dongle_connected_handler_cb;

/**
 * @brief       This function registers a callback for handling dongle connection event.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_dongle_connected_handler_cb_register(tph_dongle_connected_handler_cb_t callback)
{
    gp_tlk_tpsll_tph_dongle_connected_handler_cb = callback;
}

typedef int (*tph_dongle_disconnected_handler_cb_t)(uint8_t *disconnect_reason, uint16_t len);
extern tph_dongle_disconnected_handler_cb_t gp_tlk_tpsll_tph_dongle_disconnected_handler_cb;

/**
 * @brief       This function registers a callback for handling dongle disconnection event.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_dongle_disconnected_handler_cb_register(tph_dongle_disconnected_handler_cb_t callback)
{
    gp_tlk_tpsll_tph_dongle_disconnected_handler_cb = callback;
}

typedef uint8_t (*tph_stimer_handler_callback_t)(uint32_t tick, uint8_t *p_des, uint16_t fno);
extern tph_stimer_handler_callback_t gp_tlk_tpsll_tph_stimer_handler_cb;

typedef uint8_t (*tph_stimer_start_callback_t)(void);
extern tph_stimer_start_callback_t gp_tlk_tpsll_tph_stimer_start_cb;

/**
 * @brief       This function registers a callback for handling stimer start event.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_stimer_start_cb_register(tph_stimer_start_callback_t callback)
{
    gp_tlk_tpsll_tph_stimer_start_cb = callback;
}

typedef void (*tph_rx_audio_data_callback_t)(uint32_t tick, tph_sco_msg_pdu_header_for_host_t *p_pdu_format);
extern tph_rx_audio_data_callback_t gp_tlk_tpsll_tph_rx_audio_data_cb;

/**
 * @brief       This function registers a callback for handling received audio data.
 * @param[in]   callback    - the callback function to be registered.
 * @return      none.
 * @note
 */
__INLINE void tlk_tpsll_tph_rx_audio_data_cb_register(tph_rx_audio_data_callback_t callback)
{
    gp_tlk_tpsll_tph_rx_audio_data_cb = callback;
}

/**
 * @brief       This function starts the connection setup for the dongle.
 * @param[in]   setup_mode  - the mode for connection setup.
 * @return      The result of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_start_connection_setup(uint8_t setup_mode);

/**
 * @brief       This function initiates disconnection of the headset.
 * @param[in]   disconnect_reason - the reason for disconnection.
 * @return      The result of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_start_disconnection(tph_disconnect_reason_for_host_e disconnect_reason);

/**
 * @brief       This function exits the connection setup process for the dongle.
 * @return      The result of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_exit_connection_setup(void);

/**
 * @brief       This function checks if the dongle connection setup is in progress.
 * @return      The status of the connection setup.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_connection_setup_is_going(void);

/**
 * @brief       This function checks if the BT ACL is active.
 * @return      The connection status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_bt_acl_is_connected(void);

/**
 * @brief       This function checks if BT audio is existed.
 * @return      The availability status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_bt_audio_is_existed(void);

/**
 * @brief       This function checks if the connected headset is slave.
 * @return      The mode status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_is_slave(void);

/**
 * @brief       This function checks if the connected headset is master.
 * @return      The mode status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_is_master(void);

/**
 * @brief       This function sets the BT active pending flag.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_bt_active_pending(void);

/**
 * @brief       This function sets the current headset mode.
 * @param[in]   mode    - the mode to be set.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_mode(uint8_t mode);

/**
 * @brief       This function unsets the current headset mode.
 * @param[in]   mode    - the mode to be unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_unset_mode(uint8_t mode);

/**
 * @brief       This function sets the headset mode to be saved.
 * @param[in]   mode    - the mode to be saved.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_mode_save(uint8_t mode);

/**
 * @brief       This function unsets the headset saved mode.
 * @param[in]   mode    - the mode to be unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_unset_mode_save(uint8_t mode);

/**
 * @brief       This function sets the headset mode.
 * @param[in]   mode    - the mode to be unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_mode(uint8_t mode);

/**
 * @brief       This function unsets the headset mode.
 * @param[in]   mode    - the mode to be unset.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_unset_mode(uint8_t mode);
/**
 * @brief       This function retrieves the current headset mode.
 * @return      The current mode.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_get_mode(void);

/**
 * @brief       This function retrieves the headset saved mode.
 * @return      The saved mode.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_get_mode_save(void);

/**
 * @brief       This function retrieves the frame number.
 * @return      The frame number.
 * @note
 */
extern uint16_t tlk_tpsll_tph_env_get_fno(void);

/**
 * @brief       This function checks if the headset role is single.
 * @return      The role status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_role_is_single(void);

/**
 * @brief       This function checks if the headset is the left one.
 * @return      The status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_headset_is_left(void);

/**
 * @brief       This function checks if the dongle is in idle state.
 * @return      The status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_is_idle(void);

/**
 * @brief       This function checks if the dongle is connected.
 * @return      The connection status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_is_connected(void);

/**
 * @brief       This function checks if dongle audio exists.
 * @return      The status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_audio_is_existed(void);

/**
 * @brief       This function checks if dongle music audio exists.
 * @return      The status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_audio_is_music(void);

/**
 * @brief       This function checks if dongle phone audio exists.
 * @return      The status.
 * @note
 */
extern uint8_t tlk_tpsll_tph_dongle_audio_is_phone(void);

/**
 * @brief       This function checks if BT audio exists.
 * @return      The status.
 * @note
 */
extern int tlk_tpsll_tph_bt_audio_is_existed(void);

/**
 * @brief       This function checks if BT phone audio exists.
 * @return      The status.
 * @note
 */
extern int tlk_tpsll_tph_bt_audio_is_phone(void);

/**
 * @brief       This function checks if BT music audio exists.
 * @return      The availability status.
 * @note
 */
extern int tlk_tpsll_tph_bt_audio_is_music(void);

/**
 * @brief       This function retrieves an SCO message from the receive FIFO.
 * @param[in]   rptr    - the read pointer position.
 * @return      A pointer to the SCO message.
 * @note
 */
extern uint8_t *tlk_tpsll_tph_get_sco_msg_from_rxfifo(uint8_t rptr);

/**
 * @brief       This function retrieves the number of available SCO messages in the receive FIFO.
 * @return      The number of available messages.
 * @note
 */
extern uint8_t tlk_tpsll_tph_get_number_of_sco_msg_rxfifo_available(void);


/**
 * @brief       This function pushes an ACL message to the transmit FIFO.
 * @param[in]   dst_id  - the destination ID.
 * @param[in]   msg_id  - the message ID.
 * @param[in]   cmd     - the command.
 * @param[in]   p       - pointer to the message data.
 * @param[in]   len     - the length of the message data.
 * @param[in]   callback - the callback function.
 * @return      The result of the operation.
 * @note
 */
extern uint32_t tlk_tpsll_tph_acl_msg_push_txfifo(uint8_t dst_id, uint8_t msg_id, uint8_t cmd, uint8_t *p, uint8_t len,
                                                  void (*callback)(uint8_t dst_id, uint8_t msg_id, uint16_t cmd));

/**
 * @brief       This function pushes an SCO message to the transmit FIFO.
 * @param[in]   format  - the format of the message.
 * @param[in]   idx     - the index.
 * @param[in]   p       - pointer to the message data.
 * @param[in]   len     - the length of the message data.
 * @param[in]   callback - the callback function.
 * @return      The result of the operation.
 * @note
 */
extern uint32_t tlk_tpsll_tph_sco_msg_push_txfifo(uint8_t format, uint8_t idx, uint8_t *p, uint8_t len, void (*callback)(uint8_t dst_id, uint8_t msg_id, uint8_t cmd));

/**
 * @brief       This function sets the setup AC and channel for the dongle.
 * @param[in]   setup_ac  - the setup AC.
 * @param[in]   setup_chn - the setup channel.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_dongle_set_setup_ac_chn(uint32_t setup_ac, uint8_t setup_chn);

/**
 * @brief       This function sets the local MAC address.
 * @param[in]   local_mac - pointer to the MAC address.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_local_mac(uint8_t *local_mac);

/**
 * @brief       This function sets the generated MAC address.
 * @param[in]   gen_mac   - pointer to the MAC address.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_gen_mac(uint8_t *gen_mac);

/**
 * @brief       This function sets the dongle MAC address.
 * @param[in]   dongle_mac - pointer to the MAC address.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_dongle_mac(uint8_t *dongle_mac);

/**
 * @brief       This function sets the flag indicating the existence of dongle MAC.
 * @param[in]   en        - enable flag.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_headset_set_dongle_mac_existed(uint8_t en);

/**
 * @brief       This function retrieves AC and channel from the MAC address.
 * @param[in]   mac     - pointer to the MAC address.
 * @param[out]  ac      - pointer to the AC.
 * @param[out]  chn     - pointer to the channel.
 * @return      The result of the operation.
 * @note
 */
extern uint8_t tlk_tpsll_tph_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

/**
 * @brief       This function handles the PWM interrupt.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_pwm_irq_handler(void);

/**
 * @brief       This function handles the bbtimer interrupt.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_bbtimer_irq_handler(void);

/**
 * @brief       This function sets a schedule task for the host.
 * @param[in]   en          - enable flag.
 * @param[in]   interval_us - interval in microseconds.
 * @param[in]   duration_unit - duration unit.
 * @param[in]   duration_cnt - duration count.
 * @param[in]   anchor_point_tick - anchor point tick.
 * @return      none.
 * @note
 */
void tlk_tpsll_tph_host_set_schedule_task(uint8_t en, uint32_t interval_us, uint16_t duration_unit, uint8_t duration_cnt, uint32_t anchor_point_tick);

/**
 * @brief       This function initializes the controller.
 * @return      none.
 * @note
 */
extern void tlk_tpsll_tph_controller_init(void);

/*
 * @brief       Gets ACL data Maximum-Transmission Unit of tph controller.
 * @param[in]   none.
 * @return      Maximum-Transmission Unit of the tph acl link.
 */
extern uint8_t tlk_tpsll_tph_host_get_acl_mtu(void);

extern void tlk_tpsll_tph_host_power_control(tph_host_power_control_obj_e obj, tph_host_pwr_control_opcode_e pwr_opcode, uint8_t power_index);

#endif

#endif //TLKSTK_TPH_HOST_INTERFACE_H_
