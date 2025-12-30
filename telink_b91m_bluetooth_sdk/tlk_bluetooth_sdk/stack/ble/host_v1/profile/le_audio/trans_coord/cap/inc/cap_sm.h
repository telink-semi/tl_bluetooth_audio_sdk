/********************************************************************************************************
 * @file    cap_sm.h
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

#ifndef TLK_BLUETOOTH_SRC_B92_CAP_SM_H
#define TLK_BLUETOOTH_SRC_B92_CAP_SM_H


/**
 * @brief Initialize the CAP (Common Audio Profile) state machine
 *
 * This function initializes the CAP state machine module.
 */
void cap_sm_init(void);

/**
 * @brief Start music streaming for a device
 *
 * This function starts music streaming on the specified connection handle
 * using the provided codec configuration.
 *
 * @param[in] conn_handle      Connection handle of the device
 * @param[in] p_music_config   Pointer to the music codec configuration
 */
void cap_sm_device_start_music_stream(uint16_t conn_handle, const struct ble_ase_codec_config *p_music_config);

/**
 * @brief Start both music and voice streaming for a device
 *
 * This function starts both music (speaker) and voice (microphone) streaming
 * on the specified connection handle using the provided codec configurations.
 *
 * @param[in] conn_handle      Connection handle of the device
 * @param[in] p_music_config   Pointer to the music codec configuration
 * @param[in] p_voice_config   Pointer to the voice codec configuration
 */
void cap_sm_device_start_music_voice_stream(uint16_t conn_handle, const struct ble_ase_codec_config *p_music_config, const struct ble_ase_codec_config *p_voice_config);

/**
 * @brief Stop streaming for a device
 *
 * This function stops both music and voice streaming on the specified
 * connection handle.
 *
 * @param[in] conn_handle      Connection handle of the device
 */
void cap_sm_device_stop_stream(uint16_t conn_handle);

/**
 * @brief Handle device connection event
 *
 * This function handles the connection event for a device and updates
 * the state machine accordingly.
 *
 * @param[in] conn_handle      Connection handle of the connected device
 */
void cap_sm_device_connect(uint16_t conn_handle);

/**
 * @brief Handle device disconnection event
 *
 * This function handles the disconnection event for a device and updates
 * the state machine accordingly.
 *
 * @param[in] conn_handle      Connection handle of the disconnected device
 */
void cap_sm_device_disconnect(uint16_t conn_handle);

/**
 * @brief Check if all devices are in streaming state
 *
 * This function checks if all active devices are currently in the
 * streaming state.
 *
 * @return true if all active devices are streaming, false otherwise
 */
bool cap_sm_all_in_streaming(void);

/**
 * @brief Check if all devices are in idle state
 *
 * This function checks if all active devices are currently in the
 * idle state.
 *
 * @return true if all active devices are idle, false otherwise
 */
bool cap_sm_all_in_idle(void);

enum
{
    CAP_INPUT_INIT_PARAM    = 0,
    CAP_OUTPUT_INIT_PARAM   = 1,
    CAP_INPUT_DEINIT_PARAM  = 2,
    CAP_OUTPUT_DEINIT_PARAM = 3,
    CAP_STREAMING_SUCCESS   = 4,
    CAP_DEVICE_CONNECTED    = 5,
    CAP_DEVICE_DISCONNECTED = 6,
    CAP_ENTER_IDLE          = 7,

    CAP_BOTH_INPUT_INIT_PARAM    = 8,
    CAP_BOTH_INPUT_DEINIT_PARAM  = 9,
    CAP_BOTH_OUTPUT_INIT_PARAM   = 10,
    CAP_BOTH_OUTPUT_DEINIT_PARAM = 11,

    CAP_CODEC_REJECT = 20,
};

typedef void (*cap_state_change_callback_func_t)(int state, const void *param);
/**
 * @brief       Registers a callback function for state changes.
 * @param[in]   callback - pointer to the callback function.
 * @return      none.
 */
void cap_sm_register_state_change_callback(cap_state_change_callback_func_t callback);
#endif //TLK_BLUETOOTH_SRC_B92_CAP_SM_H