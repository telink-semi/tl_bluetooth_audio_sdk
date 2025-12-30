/********************************************************************************************************
 * @file    tpmd_host_interface.h
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
#ifndef TLKSTK_TPMD_HOST_INTERFACE_H_
#define TLKSTK_TPMD_HOST_INTERFACE_H_

#include "common/types.h"
#include "common/bit.h"

#if (CONTROLLER_MODE == MESH_AUDIO_DONGLE)
#include "stack/tpsll/tpmd/tpmd_host_interface.h"

#define TPMD_HOST_INTERFACE_DEBUG_EN              (1)
#define TPMD_HOST_HEADSET_SETUP_COMMON_ACCESSCODE 0x9669c33c
#define TPMD_HOST_HEADSET_SETUP_COMMON_CHN        0x13 //  2402+2*0x13=2440
#define TPMD_HOST_DONGLE_SETUP_COMMON_ACCESSCODE  0x56291435
#define TPMD_HOST_DONGLE_SETUP_COMMON_CHN         0x12 //  2402+2*0x10=2434
#define TPMD_HOST_CONNECTED_AFH_COMMON_CHN        0x20
#define TPMD_HOST_10MS_MIC_LC3_DATA_LEN           (30)

typedef enum TPMD_HOST_DG_AUDIO_PATH_STATUS
{
    TPMD_HOST_DG_AUDIO_PATH_STATUS_IDLE,
    TPMD_HOST_DG_AUDIO_PATH_STATUS_RUNNING,
} tpmd_dongle_host_audio_status_e;

typedef enum TPMD_HOST_DG_SCO_AUDIO_MODE
{
    TPMD_HOST_DG_SCO_AUDIO_MODE_IDLE,
    TPMD_HOST_DG_SCO_AUDIO_MODE_SPEAKER,
    TPMD_HOST_DG_SCO_AUDIO_MODE_MIC,
} tpmd_dongle_host_sco_audio_mode_e;

typedef enum
{
    TPMD_HOST_AUDIO_FORMAT_MSBC     = 0,
    TPMD_HOST_AUDIO_FORMAT_LC3A     = 1,
    TPMD_HOST_AUDIO_FORMAT_16K_MASK = 0x04,
} tpmd_audio_format_for_host_e;

typedef enum
{
    TPMD_HOST_HEADSET_SETUP_MODE_IDLE,
    TPMD_HOST_HEADSET_SETUP_MODE_NORMAL,
    TPMD_HOST_HEADSET_SETUP_MODE_3S,
    TPMD_HOST_HEADSET_SETUP_MODE_10S,
    TPMD_HOST_DONGLE_SETUP_MODE_NORMAL,
    TPMD_HOST_DONGLE_SETUP_MODE_PAIRING,
    TPMD_HOST_DONGLE_SETUP_MODE_CC_HEADSET,
} tpmd_headset_setup_mode_for_host_e;

typedef enum TPMD_MODE_FOR_HOST
{
    TPMD_HOST_MODE_BT_ACTIVE      = BIT(0),
    TPMD_HOST_MODE_BT_MUSIC       = BIT(1),
    TPMD_HOST_MODE_BT_VOICE       = BIT(2),
    TPMD_HOST_MODE_BT_AUDIO       = BIT(2) | BIT(1),
    TPMD_HOST_MODE_DONGLE_ACTIVE  = BIT(3),
    TPMD_HOST_MODE_DONGLE_MUSIC   = BIT(4),
    TPMD_HOST_MODE_DONGLE_PHONE   = BIT(5),
    TPMD_HOST_MODE_DONGLE_AUDIO   = BIT(5) | BIT(4),
    TPMD_HOST_MODE_HEADSET_LEFT   = BIT(6),
    TPMD_HOST_MODE_HEADSET_ACTIVE = BIT(7),
} tpmd_mode_for_host_e;

typedef enum
{
    TPMD_HOST_DISCONNECT_REASON_NONE,
    TPMD_HOST_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPMD_HOST_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPMD_HOST_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPMD_HOST_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPMD_HOST_DISCONNECT_REASON_DONGLE_SETUP,
    TPMD_HOST_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
} tpmd_disconnect_reason_for_host_e;

/* CMD: ACL */
typedef enum TPMD_PDU_CMD_ACL_FOR_HOST
{
    TPMD_HOST_MSG_PDU_ACL_CMD_IDLE,
    TPMD_HOST_MSG_PDU_ACL_CMD_NULL,
    TPMD_HOST_MSG_PDU_ACL_CMD_SYNC,
    TPMD_HOST_MSG_PDU_ACL_CMD_TONE_SYNC,
    TPMD_HOST_MSG_PDU_ACL_CMD_LED_SYNC,
    TPMD_HOST_MSG_PDU_ACL_CMD_TEST,
    TPMD_HOST_MSG_PDU_ACL_CMD_UI,
    TPMD_HOST_MSG_PDU_ACL_CMD_PROFILE,
    TPMD_HOST_MSG_PDU_ACL_CMD_DFU,
    TPMD_HOST_MSG_PDU_ACL_CMD_APP,

    TPMD_HOST_MSG_PDU_ACL_CMD_MAX,
} tpmd_pdu_cmd_acl_for_host_e;

typedef enum
{
    TPMD_HOST_MSG_ID_IDLE = 0,
    TPMD_HOST_MSG_ID_SETUP,
    TPMD_HOST_MSG_ID_LMP,
    TPMD_HOST_MSG_ID_ACL,
    TPMD_HOST_MSG_ID_SCO,
} tpmd_msg_id_for_host_t;

typedef enum
{
    TPMD_HOST_MSG_LINK_ID_IDLE,
    TPMD_HOST_MSG_LINK_ID_HEADSET_MASTER, /* master headset */
    TPMD_HOST_MSG_LINK_ID_HEADSET_SLAVE,  /* slave headset */
    TPMD_HOST_MSG_LINK_ID_HEADSET_ALL,    /* all headset including master and slave */
    TPMD_HOST_MSG_LINK_ID_DONGLE,         /* dongle */
} tpmd_msg_dst_id_for_host_t;

typedef enum
{
    TPMD_DISCONNECT_REASON_NONE,
    TPMD_DISCONNECT_REASON_HEADSET_SHUTDOWN,
    TPMD_DISCONNECT_REASON_HEADSET_START_3S_SETUP,
    TPMD_DISCONNECT_REASON_HEADSET_START_10S_SETUP,
    TPMD_DISCONNECT_REASON_HEADSET_LINKSUPERVISION_TIMEOUT,

    TPMD_DISCONNECT_REASON_DONGLE_SETUP,
    TPMD_DISCONNECT_REASON_DONGLE_LINKSUPERVISION_TIMEOUT,
} tpmd_disconnect_reason_e;

/**
 * @brief Enumeration defining values for simulate task enable or disable
 */
typedef enum
{
    TPMD_HOST_TASK_DISABLE = 0x00,
    TPMD_HOST_TASK_ENABLE  = 0x01,
} tpmd_task_en_for_host_e;

typedef enum
{
    TPMD_HOST_SET_AUDIO_PATH_IDLE = 0x00,
    TPMD_HOST_STIMER_START_EVENT  = 0x01,
} tpmd_audio_path_cmd_for_host_e;

typedef enum
{
    TPMD_AUDIO_CHANNEL_LEFT  = 0,
    TPMD_AUDIO_CHANNEL_RIGHT = 1,
} tpmd_audio_channel_e;

typedef enum
{
    TPMD_SET_AUDIO_PATH_IDLE       = 0x00,
    TPMD_STIMER_START_EVENT        = 0x01,
    TPMD_DG_SCO_SETUP_SUCCESS      = 0x02,
    TPMD_DG_EXIT_SCO_COMPLETE      = 0x03,
    TPMD_SET_DG_AUDIO_PATH_PENDING = 0x04,
} tpmd_audio_path_cmd_e;

typedef enum
{
    TPMD_AUDIO_FORMAT_MSBC          = 0,
    TPMD_AUDIO_FORMAT_LC3A          = 1,
    TPMD_AUDIO_FORMAT_MASK          = 0x01,
    TPMD_AUDIO_FORMAT_16K_MASK      = 0x04,
    TPMD_AUDIO_FORMAT_USB_MUTE_MASK = 0x08,
} tpmd_audio_format_e;

typedef struct TPMD_SCO_MSG_PDU_HEADER_FOR_HOST
{
    u8 data_format : 4;
    u8 data_wptr   : 4;
} __attribute__((packed)) __attribute__((aligned(4))) tpmd_sco_msg_pdu_header_for_host_t;

typedef uint8_t (*tpmd_dongle_audio_path_callback_t)(u16 cmd);
extern tpmd_dongle_audio_path_callback_t tpmd_audio_path_cb;

/**
 * @brief       Registers a callback function for audio path.
 * @param[in]   callback - the audio path function to be called.
 * @return      none.
 * @note        
 */
__INLINE void tpmd_audio_path_cb_register(tpmd_dongle_audio_path_callback_t callback)
{
    tpmd_audio_path_cb = callback;
}

typedef void (*tpmd_cmd_ui_handler_cb_t)(u8 *cmd);
extern tpmd_cmd_ui_handler_cb_t tpmd_cmd_ui_handler_cb;

/**
 * @brief       Registers a callback function for UI command handling.
 * @param[in]   callback - the function to be called to handle UI commands.
 * @return      none.
 * @note        
 */
__INLINE void tpmd_cmd_ui_handler_cb_register(tpmd_cmd_ui_handler_cb_t callback)
{
    tpmd_cmd_ui_handler_cb = callback;
}

/**
 * @brief       Retrieves the position of SCO in BT voice.
 * @return      The position of SCO in BT voice.
 * @note        
 */
extern u8 tpmd_get_sco_pos_in_bt_voice(void);
/**
 * @brief       Retrieves the current dongle mode.
 * @return      The dongle mode.
 * @note        
 */
extern u8 tpmd_host_get_dongle_mode(void);

/**
 * @brief       Starts the setup process.
 * @param[in]   none.
 * @return      Result of the setup operation.
 * @note        
 */
extern u8 tpmd_host_dongle_start_connection_scan(void);

/**
 * @brief       Starts dongle SCO setup with specified audio mode.
 * @param[in]   audio_mode - the audio mode to be set.
 * @return      Result of the SCO setup operation.
 * @note        
 */
extern u8 tpmd_host_start_dongle_sco_setup(u8 audio_mode);
/**
 * @brief       Exits SCO setup for the dongle.
 * @param[in]   none.
 * @return      Result of the exit SCO setup operation.
 * @note        
 */
extern u8 tpmd_host_exit_dongle_sco(void);
/**
 * @brief       Switches the dongle SCO audio mode SCO.
 * @param[in]   audio_mode - the new audio mode to be set.
 * @return      Result of the switch dongle SCOaudio mode operation.
 * @note        
 */
extern u8 tpmd_host_switch_dongle_sco_audio_mode(u8 audio_mode);

/**
 * @brief       Checks if the dongle is connected.
 * @param[in]   none.
 * @return      Connection status of the dongle.
 * @note        
 */
extern u8 tpmd_dongle_is_connected(void);

/**
 * @brief       This function handles the PWM interrupt.
 * @return      none.
 * @note        
 */
extern void tpmd_pwm_irq_handler(void);
/**
 * @brief       This function handles the bbtimer interrupt.
 * @return      none.
 * @note        
 */
extern void tpmd_bbtimer_irq_handler(void);

/**
 * @brief       Sets a scheduled task for the host with specified parameters.
 * @param[in]   en - enables or disables the task.
 * @param[in]   interval_us - interval in microseconds.
 * @param[in]   duration_unit - duration unit.
 * @param[in]   duration_cnt - duration count.
 * @param[in]   anchor_point_tick - anchor point in ticks.
 * @return      none.
 * @note        
 */
void tpmd_host_set_schedule_task(u8 en, u32 interval_us, u16 duration_unit, u8 duration_cnt, u32 anchor_point_tick);

/**
 * @brief       Initializes the controller.
 * @return      none.
 * @note        
 */
extern void tpmd_controller_init(void);

/**
 * @brief       Sets up AC and channel for setup process.
 * @param[in]   setup_ac - AC value to be set.
 * @param[in]   setup_chn - channel value to be set.
 * @return      none.
 * @note        
 */
extern void tpmd_dongle_set_setup_ac_chn(u32 setup_ac, u8 setup_chn);

/**
 * @brief       Calculates the AC and channel from the MAC address.
 * @param[in]   mac - MAC address to Calculate AC and channel from.
 * @param[out]  ac - access code.
 * @param[out]  chn - channel.
 * @return      Result of the calculate operation.
 * @note        
 */
extern u8 tpmd_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

/**
 * @brief       Sets the headset MAC address.
 * @param[in]   gen_mac - the MAC address to be set.
 * @return      none.
 * @note        
 */
extern void tpmd_dongle_set_headset_mac(u8 *gen_mac);

/**
 * @brief       Gets the frame number.
 * @param[in]   none.
 * @return      The frame number.
 * @note        
 */
extern uint16_t tpmd_env_get_fno(void);

/**
 * @brief       Sets the local MAC address.
 * @param[in]   local_mac - the MAC address to be set.
 * @return      none.
 * @note        
 */
extern void tpmd_dongle_set_local_mac(u8 *local_mac);


typedef void (*tpmd_headset_connected_handler_cb_t)(uint8_t *mac_address);
extern tpmd_headset_connected_handler_cb_t tpmd_headset_connected_handler_cb;

/**
 * @brief       Registers a callback function for headset connection event.
 * @param[in]   callback - the function to be called when headset connect.
 * @return      none.
 * @note        
 */
inline void tpmd_headset_connected_handler_cb_register(tpmd_headset_connected_handler_cb_t callback)
{
    tpmd_headset_connected_handler_cb = callback;
}

typedef void (*tpmd_headset_disconnected_handler_cb_t)(u8 disconnect_reason);
extern tpmd_headset_disconnected_handler_cb_t tpmd_headset_disconnected_handler_cb;

/**
 * @brief       Registers a callback function for headset disconnection event.
 * @param[in]   callback - the function to be called when headset disconnected.
 * @return      none.
 * @note        
 */
inline void tpmd_headset_disconnected_handler_cb_register(tpmd_headset_disconnected_handler_cb_t callback)
{
    tpmd_headset_disconnected_handler_cb = callback;
}

/**
 * @brief       Checks if a headset is connected.
 * @param[in]   none.
 * @return      Connection status of the headset.
 * @note        
 */
uint8_t tpmd_headset_is_cc_headset_connected(void);

/**
 * @brief       DChecks if the master headset is on the left side.
 * @return      Master headset position (left or not).
 * @note        
 */
uint8_t tpmd_headset_master_is_left(void);


/**
 * @brief       Gets the pointer to the data in mic voice data buffer that is going to be processed.
 * @param[in]   none.
 * @return      Pointer to the data packet that is going to be processed.
 * @note        
 */
extern u8 *tpmd_dongle_get_mic_data_ptr(void);

/**
 * @brief       Retrieves the setup AC and channel.
 * @param[out]  setup_ac - setup access code value retrieved.
 * @param[out]  setup_chn - setup channel value retrieved.
 * @return      none.
 * @note        
 */
extern void tpmd_headset_get_setup_ac_chn(u32 *setup_ac, u8 *setup_chn);

/**
 * @brief       Retrieves the generated MAC address for the headset.
 * @param[out]  gen_mac - generated MAC address retrieved.
 * @return      none.
 * @note        
 */
extern void tpmd_headset_get_gen_mac(u8 *gen_mac);

/**
 * @brief       Checks if dongle audio exists.
 * @param[in]   none.
 * @return      Existence status of dongle audio.
 * @note        
 */
extern int dongle_audio_is_existed(void);

/**
 * @brief       Pushes SCO music message to the TX FIFO.
 * @param[in]   format - format of the message.
 * @param[in]   chnl - channel of the message.
 * @param[in]   len - length of the message.
 * @param[in]   callback - callback function to be called after message push.
 * @return      Pointer to the pushed message.
 * @note        
 */
extern u8 *tpmd_sco_music_msg_push_txfifo(u8 format, u8 chnl, u8 len, void (*callback)(u8 dst_id, u8 msg_id, u8 cmd));

/**
 * @brief       Pushes Opus music message to the TX FIFO.
 * @param[in]   p - pointer to the message data.
 * @param[in]   len - length of the message.
 * @param[in]   callback - callback function to be called after message push.
 * @return      Pointer to the pushed message.
 * @note        
 */
extern u8 *tpmd_opus_music_msg_push_txfifo(u8 *p, u8 len, void (*callback)(u8 dst_id, u8 msg_id, u8 cmd));

/**
 * @brief       Handles the completion of message transmission.
 * @param[in]   dst_id - destination ID of the message.
 * @param[in]   msg_id - message ID of the message.
 * @param[in]   cmd - command associated with the message.
 * @return      none.
 * @note        
 */
extern void tpmd_msg_tx_done_handler(u8 dst_id, u8 msg_id, u8 cmd);

/**
 * @brief       Handles the completion of SCO music message push to TX FIFO.
 * @param[in]   chnl - channel of the message.
 * @return      none.
 * @note        
 */
extern void tpmd_sco_music_msg_push_txfifo_done(u8 chnl);

/**
 * @brief       Checks if BT audio exists.
 * @param[in]   none.
 * @return      Existence status of BT audio.
 * @note        
 */
extern int bt_audio_is_existed(void);

/**
 * @brief       Checks if the headset role is single.
 * @param[in]   none.
 * @return      Single role mode status of the headset.
 * @note        
 */
extern u8 tpmd_headset_is_single_role(void);

/**
 * @brief       Initiates disconnection of the dongle with a specified reason.
 * @param[in]   disconnect_reason - reason for disconnection.
 * @return      Result of the disconnection operation.
 * @note        
 */
extern u8 tpmd_dongle_start_disconnection(tpmd_disconnect_reason_e disconnect_reason);

/**
 * @brief       Sets the power index.
 * @param[in]   pwr_idx - power index to be set.
 * @return      none.
 * @note        
 */
extern void tpmd_host_set_power_index(u8 pwr_idx);

/**
 * @brief       Sets the audio path status .
 * @param[in]   is_running - status indicating if audio path is running.
 * @return      none.
 * @note        
 */
extern void tpmd_host_set_audio_path_status(u8 is_running);

#endif
#endif //TLKSTK_TPMD_HOST_INTERFACE_H_
