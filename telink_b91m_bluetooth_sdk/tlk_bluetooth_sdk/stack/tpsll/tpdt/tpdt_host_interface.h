/********************************************************************************************************
 * @file    tpdt_host_interface.h
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
#ifndef TLKSTK_TPDT_HOST_INTERFACE_H_
#define TLKSTK_TPDT_HOST_INTERFACE_H_

#include "tl_common.h"
#if (CONTROLLER_MODE == BLETPSLL_TWS)

#define TPDT_HOST_INTERFACE_DEBUG_EN              (1)
#define TPDT_HOST_HEADSET_SETUP_COMMON_ACCESSCODE 0x9669c33c
#define TPDT_HOST_HEADSET_SETUP_COMMON_CHN        0x13 //  2402+2*0x13=2440
#define TPDT_HOST_DONGLE_SETUP_COMMON_ACCESSCODE  0x56291435
#define TPDT_HOST_DONGLE_SETUP_COMMON_CHN         0x12 //  2402+2*0x10=2434
#define TPDT_HOST_CONNECTED_AFH_COMMON_CHN        0x20
#define TPDT_HOST_10MS_MIC_LC3_DATA_LEN           (30)

typedef enum
{
    TPDT_HOST_AUDIO_FORMAT_MSBC     = 0,
    TPDT_HOST_AUDIO_FORMAT_LC3A     = 1,
    TPDT_HOST_AUDIO_FORMAT_16K_MASK = 0x04,
} tpdt_audio_format_for_host_e;

typedef enum
{
    TPDT_HOST_HEADSET_SETUP_MODE_IDLE,
    TPDT_HOST_HEADSET_SETUP_MODE_NORMAL,
    TPDT_HOST_HEADSET_SETUP_MODE_3S,
    TPDT_HOST_HEADSET_SETUP_MODE_10S,
    TPDT_HOST_DONGLE_SETUP_MODE_NORMAL,
    TPDT_HOST_DONGLE_SETUP_MODE_PAIRING,
    TPDT_HOST_DONGLE_SETUP_MODE_CC_HEADSET,
} tpdt_headset_setup_mode_for_host_e;

typedef enum TPDT_MODE_FOR_HOST
{
    TPDT_HOST_MODE_BT_ACTIVE      = BIT(0),
    TPDT_HOST_MODE_LE_ACTIVE      = BIT(0),
    TPDT_HOST_MODE_BT_MUSIC       = BIT(1),
    TPDT_HOST_MODE_LE_MUSIC       = BIT(1),
    TPDT_HOST_MODE_BT_VOICE       = BIT(2),
    TPDT_HOST_MODE_LE_VOICE       = BIT(2),
    TPDT_HOST_MODE_BT_AUDIO       = BIT(2) | BIT(1),
    TPDT_HOST_MODE_LE_AUDIO       = BIT(2) | BIT(1),
    TPDT_HOST_MODE_DONGLE_ACTIVE  = BIT(3),
    TPDT_HOST_MODE_DONGLE_MUSIC   = BIT(4),
    TPDT_HOST_MODE_DONGLE_PHONE   = BIT(5),
    TPDT_HOST_MODE_DONGLE_AUDIO   = BIT(5) | BIT(4),
    TPDT_HOST_MODE_HEADSET_LEFT   = BIT(6),
    TPDT_HOST_MODE_HEADSET_ACTIVE = BIT(7),
} tpdt_mode_for_host_e;

typedef enum
{
    TPDT_HOST_DISCONNECT_REASON_NONE,
    TPDT_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPDT_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPDT_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPDT_HOST_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPDT_HOST_DISCONNECT_REASON_DONGLE_SETUP,
    TPDT_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
} tpdt_disconnect_reason_for_host_e;

/* CMD: ACL */
typedef enum TPDT_PDU_CMD_ACL_FOR_HOST
{
    TPDT_HOST_MSG_PDU_ACL_CMD_IDLE,
    TPDT_HOST_MSG_PDU_ACL_CMD_NULL,
    TPDT_HOST_MSG_PDU_ACL_CMD_SYNC,
    TPDT_HOST_MSG_PDU_ACL_CMD_TONE_SYNC,
    TPDT_HOST_MSG_PDU_ACL_CMD_LED_SYNC,
    TPDT_HOST_MSG_PDU_ACL_CMD_TEST,
    TPDT_HOST_MSG_PDU_ACL_CMD_UI,
    TPDT_HOST_MSG_PDU_ACL_CMD_PROFILE,
    TPDT_HOST_MSG_PDU_ACL_CMD_DFU,
    TPDT_HOST_MSG_PDU_ACL_CMD_APP,

    TPDT_HOST_MSG_PDU_ACL_CMD_MAX,
} tpdt_pdu_cmd_acl_for_host_e;

typedef enum
{
    TPDT_HOST_MSG_ID_IDLE = 0,
    TPDT_HOST_MSG_ID_SETUP,
    TPDT_HOST_MSG_ID_LMP,
    TPDT_HOST_MSG_ID_ACL,
    TPDT_HOST_MSG_ID_SCO,
} tpdt_msg_id_for_host_t;

typedef enum
{
    TPDT_HOST_MSG_LINK_ID_IDLE,
    TPDT_HOST_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    TPDT_HOST_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    TPDT_HOST_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    TPDT_HOST_MSG_LINK_ID_DONGLE,         /* dongle */
} tpdt_msg_dst_id_for_host_t;

/**
 * @brief Enumeration defining values for simulate task enable or disable
 */
typedef enum
{
    TPDT_HOST_TASK_DISABLE = 0x00,
    TPDT_HOST_TASK_ENABLE  = 0x01,
} tpdt_task_en_for_host_e;

typedef struct TPDT_SCO_MSG_PDU_HEADER_FOR_HOST
{
    u8 data_format : 4;
    u8 data_wptr   : 4;
} __attribute__((packed)) __attribute__((aligned(4))) tpdt_sco_msg_pdu_header_for_host_t;

/* callback */
typedef void (*tpdt_dongle_mac_address_update_handler_cb_t)(u8 *mac_address);
extern tpdt_dongle_mac_address_update_handler_cb_t tpdt_dongle_mac_address_update_handler_cb;

/**
 * @brief       Registers a callback function to handle dongle MAC address update.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_dongle_mac_address_update_handler_cb_register(tpdt_dongle_mac_address_update_handler_cb_t callback)
{
    tpdt_dongle_mac_address_update_handler_cb = callback;
}

typedef void (*tpdt_dongle_connected_handler_cb_t)(void);
extern tpdt_dongle_connected_handler_cb_t tpdt_dongle_connected_handler_cb;

/**
 * @brief       Registers a callback function to handle dongle connection event.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_dongle_connected_handler_cb_register(tpdt_dongle_connected_handler_cb_t callback)
{
    tpdt_dongle_connected_handler_cb = callback;
}

typedef void (*tpdt_dongle_disconnected_handler_cb_t)(u8 disconnect_reason);
extern tpdt_dongle_disconnected_handler_cb_t tpdt_dongle_disconnected_handler_cb;

/**
 * @brief       Registers a callback function to handle dongle disconnection event.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_dongle_disconnected_handler_cb_register(tpdt_dongle_disconnected_handler_cb_t callback)
{
    tpdt_dongle_disconnected_handler_cb = callback;
}

typedef void (*tpdt_tws_headset_connected_handler_cb_t)(void);
extern tpdt_tws_headset_connected_handler_cb_t tpdt_tws_headset_connected_handler_cb;

/**
 * @brief       Registers a callback function to handle TWS headset connection event.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_tws_headset_connected_handler_cb_register(tpdt_tws_headset_connected_handler_cb_t callback)
{
    tpdt_tws_headset_connected_handler_cb = callback;
}

typedef void (*tpdt_tws_headset_disconnected_handler_cb_t)(u8 disconnect_reason);
extern tpdt_tws_headset_disconnected_handler_cb_t tpdt_tws_headset_disconnected_handler_cb;

/**
 * @brief       Registers a callback function to handle TWS headset disconnection event.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_tws_headset_disconnected_handler_cb_register(tpdt_tws_headset_disconnected_handler_cb_t callback)
{
    tpdt_tws_headset_disconnected_handler_cb = callback;
}

typedef uint8_t (*tpdt_stimer_handler_callback_t)(uint32_t tick, uint8_t *p_des, uint16_t fno);
extern tpdt_stimer_handler_callback_t tpdt_stimer_handler_cb;

typedef uint8_t (*tpdt_stimer_start_callback_t)(void);
extern tpdt_stimer_start_callback_t tpdt_stimer_start_cb;

/**
 * @brief       Registers a callback function for the start event of the stimer.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_stimer_start_cb_register(tpdt_stimer_start_callback_t callback)
{
    tpdt_stimer_start_cb = callback;
}

typedef void (*tpdt_rx_audio_data_callback_t)(uint32_t tick, tpdt_sco_msg_pdu_header_for_host_t *p_pdu_format);
extern tpdt_rx_audio_data_callback_t tpdt_rx_audio_data_cb;

/**
 * @brief       Registers a callback function to handle received audio data.
 * @param[in]   callback - The callback function to register.
 * @return      none.
 * @note        
 */
__INLINE void tpdt_rx_audio_data_cb_register(tpdt_rx_audio_data_callback_t callback)
{
    tpdt_rx_audio_data_cb = callback;
}

/**
 * @brief       Starts the connection setup process.
 * @param[in]   setup_mode - The mode for the connection setup.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_dongle_start_connection_setup(u8 setup_mode);

/**
 * @brief       Starts the disconnection process for the headset.
 * @param[in]   disconnect_reason - The reason for disconnection.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_headset_start_disconnection(tpdt_disconnect_reason_for_host_e disconnect_reason);

/**
 * @brief       Checks if the connection setup process is exited.
 * @return      The result of the check operation.
 * @note        
 */
extern u8 tpdt_dongle_exit_connection_setup(void);

/**
 * @brief       Checks if the dongle is currently in the connection setup process.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_dongle_connection_setup_is_going(void);

/**
 * @brief       Exits the connection setup process for the headset.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_headset_exit_connection_setup(void);

/**
 * @brief       Checks if the headset is connected via BT ACL.
 * @return      The result of the check operation.
 * @note        
 */
extern u8 tpdt_headset_bt_acl_is_connected(void);

/**
 * @brief       Checks if BT audio exists for the headset.
 * @return      The result of the check operation.
 * @note        
 */
extern u8 tpdt_headset_bt_audio_is_existed(void);

/**
 * @brief       Checks if the headset role is slave.
 * @return      The result of the check operation.
 * @note        
 */
extern u8 tpdt_headset_is_slave(void);

/**
 * @brief       Checks if the headset role is master.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_headset_is_master(void);

/**
 * @brief       Sets the role of the headset to single.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_role_set_single(void); // IDLE, or single

/**
 * @brief       Sets the saved mode for the headset.
 * @param[in]   mode - The mode to save.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_mode_save(u8 mode);

/**
 * @brief       Unsets the saved mode for the headset.
 * @param[in]   mode - The mode to unset.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_unset_mode_save(u8 mode);

/**
 * @brief       Sets the mode for the headset.
 * @param[in]   mode - The mode to set.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_mode(u8 mode);

/**
 * @brief       Sets the le mode for the headset.
 * @param[in]   mode - The mode to set.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_le_mode(u8 mode);

/**
 * @brief       Unsets the mode for the headset.
 * @param[in]   mode - The mode to unset.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_unset_mode(u8 mode);

/**
 * @brief       Unsets the le mode for the headset.
 * @param[in]   mode - The mode to unset.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_unset_le_mode(u8 mode);

/**
 * @brief       Retrieves the current mode of the headset.
 * @return      The current mode.
 * @note        
 */
extern u8 tpdt_headset_get_mode(void);

/**
 * @brief       Retrieves the saved mode of the headset.
 * @return      The saved mode.
 * @note        
 */
extern u8 tpdt_headset_get_mode_save(void);

/**
 * @brief       Retrieves the frame number.
 * @return      The frame number.
 * @note        
 */
extern u16 tpdt_env_get_fno(void);

/**
 * @brief       Checks if the headset role is single.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_headset_role_is_single(void);

/**
 * @brief       Checks if the headset is the left device.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_headset_is_left(void);

/**
 * @brief       Sets the left device status for the headset.
 * @param[in]   left - The status to set (0 or 1).
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_left(u8 left);

/**
 * @brief       Checks if the dongle state is idle.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_dongle_is_idle(void);

/**
 * @brief       Checks if the dongle is connected.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_dongle_is_connected(void);

/**
 * @brief       Checks if dongle audio exists.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_dongle_audio_is_existed(void);

/**
 * @brief       Checks if the dongle audio is music.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_dongle_audio_is_music(void);

/**
 * @brief       Checks if the dongle audio is phone.
 * @return      The result of the check.
 * @note        
 */
extern u8 tpdt_dongle_audio_is_phone(void);

/**
 * @brief       Checks if BT audio exists.
 * @return      The result of the check.
 * @note        
 */
extern int tpdt_bt_audio_is_existed(void);

/**
 * @brief       Checks if BT audio is phone.
 * @return      The result of the check.
 * @note        
 */
extern int tpdt_bt_audio_is_phone(void);

/**
 * @brief       Checks if BT audio is music.
 * @return      The result of the check.
 * @note        
 */
extern int tpdt_bt_audio_is_music(void);

/**
 * @brief       Clears the SCO message RX FIFO.
 * @return      none.
 * @note        
 */
extern void tpdt_clear_sco_msg_rxfifo(void);

/**
 * @brief       Retrieves a SCO message from the RX FIFO.
 * @param[in]   rptr - The read pointer for the FIFO.
 * @return      The pointer to the retrieved SCO message.
 * @note        
 */
extern u8 *tpdt_get_sco_msg_from_rxfifo(uint8_t rptr);

/**
 * @brief       Retrieves the number of available SCO messages in the RX FIFO.
 * @return      The number of available messages.
 * @note        
 */
extern u8 tpdt_get_number_of_sco_msg_rxfifo_available(void);


/**
 * @brief       Pushes an ACL message into the TX FIFO.
 * @param[in]   dst_id - The destination ID.
 * @param[in]   msg_id - The message ID.
 * @param[in]   cmd - The command.
 * @param[in]   p - Pointer to the message data.
 * @param[in]   len - Length of the message data.
 * @param[in]   callback - The callback function for the operation.
 * @return      The result of the operation.
 * @note        
 */
extern u32 tpdt_acl_msg_push_txfifo(u8 dst_id, u8 msg_id, u8 cmd, u8 *p, u8 len, void (*callback)(u8 dst_id, u8 msg_id, u8 cmd));

/**
 * @brief       Pushes an SCO message into the TX FIFO.
 * @param[in]   format - The format of the SCO message.
 * @param[in]   idx - The index of the SCO message.
 * @param[in]   p - Pointer to the message data.
 * @param[in]   len - Length of the message data.
 * @param[in]   callback - The callback function for the operation.
 * @return      The result of the operation.
 * @note        
 */
extern u32 tpdt_sco_msg_push_txfifo(u8 format, u8 idx, u8 *p, u8 len, void (*callback)(u8 dst_id, u8 msg_id, u8 cmd));

/**
 * @brief       Sets the setup AC and channel for the dongle.
 * @param[in]   setup_ac - The access code for setup.
 * @param[in]   setup_chn - The channel for setup.
 * @return      none.
 * @note        
 */
extern void tpdt_dongle_set_setup_ac_chn(u32 setup_ac, u8 setup_chn);

/**
 * @brief       Sets the local MAC address.
 * @param[in]   local_mac - Pointer to the MAC address to set.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_local_mac(u8 *local_mac);

/**
 * @brief       Sets the generated MAC address.
 * @param[in]   gen_mac - Pointer to the generated MAC address to set.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_gen_mac(u8 *gen_mac);

/**
 * @brief       Sets the dongle MAC address.
 * @param[in]   dongle_mac - Pointer to the dongle MAC address to set.
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_dongle_mac(u8 *dongle_mac);

/**
 * @brief       Sets the existence flag of the dongle MAC address.
 * @param[in]   en - The flag to set (0 or 1).
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_dongle_mac_existed(u8 en);

/**
 * @brief       Sets the dongle disconnect flag for OTA.
 * @param[in]   en - The flag to set (0 or 1).
 * @return      none.
 * @note        
 */
extern void tpdt_headset_set_dg_disconnect_for_ota(u8 en);

/**
 * @brief       Retrieves the AC and channel from a given MAC address.
 * @param[in]   mac - Pointer to the MAC address.
 * @param[out]  ac - Pointer to store the retrieved AC.
 * @param[out]  chn - Pointer to store the retrieved channel.
 * @return      The result of the operation.
 * @note        
 */
extern u8 tpdt_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

/**
 * @brief       This function handles the PWM interrupt.
 * @return      none.
 * @note        
 */
extern void tpdt_pwm_irq_handler(void);

/**
 * @brief       This function handles the bbtimer interrupt.
 * @return      none.
 * @note        
 */
extern void tpdt_bbtimer_irq_handler(void);

/**
 * @brief       Sets a schedule task
 * @param[in]   en - Enable or disable the task.
 * @param[in]   interval_us - The interval in microseconds.
 * @param[in]   duration_unit - The duration unit.
 * @param[in]   duration_cnt - The duration count.
 * @param[in]   anchor_point_tick - The anchor point tick.
 * @return      none.
 * @note        
 */
void tpdt_host_set_schedule_task(u8 en, u32 interval_us, u16 duration_unit, u8 duration_cnt, u32 anchor_point_tick);

/**
 * @brief       Initializes the controller with a given USB ID.
 * @param[in]   usb_id - The USB ID to initialize the controller.
 * @return      none.
 * @note        
 */
extern void tpdt_controller_init(u16 usb_id);
#endif
#endif //TLKSTK_TPDT_HOST_INTERFACE_H_
