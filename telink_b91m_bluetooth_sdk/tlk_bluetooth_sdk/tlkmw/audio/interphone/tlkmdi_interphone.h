/********************************************************************************************************
 * @file    tlkmdi_interphone.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef TLKMDI_INTERPHONE_H_
#define TLKMDI_INTERPHONE_H_
#if TLKMW_INTERPHONE_EN
typedef struct
{
    uint8_t codec_type;
    bool    busy;
    bool    bt_music_started;
    bool    bt_ag_sco_started;
    bool    mesh_started;
    bool    mesh_first_flag;
    bool    mic_first_flag;
    bool    spk_first_flag;
    // uint32_t run_timer;
} tlkmdi_interphone_env_t;

/**
 * @brief   Initializes the interphone module.
 * @param   None
 * @return  int - Returns 0 if initialization is successful; otherwise an error code.
 */
int tlkmdi_interphone_init(void);

/**
 * @brief     Switches the interphone status for a given handle.
 * @param[in] handle The connection handle.
 * @param[in] status The target status to switch to.
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_interphone_switch(uint16_t handle, uint8_t status);

/**
 * @brief   Checks if the interphone module is busy.
 * @param   None
 * @return  bool - Returns true if the interphone module is busy; otherwise false.
 */
bool tlkmdi_interphone_is_busy(void);

/**
 * @brief     Starts the interphone for a given handle with specified parameters.
 * @param[in] handle The connection handle.
 * @param[in] param  Additional parameters for starting the interphone.
 * @return    int - Returns 0 if start is successful; otherwise an error code.
 */
int tlkmdi_interphone_start(uint16_t handle, uint32_t param);

/**
 * @brief     Closes the interphone for a given handle.
 * @param[in] handle The connection handle.
 * @return    int - Returns 0 if close is successful; otherwise an error code.
 */
int tlkmdi_interphone_close(uint16_t handle);

/**
 * @brief   Skips to the next interphone track.
 * @param   None
 * @return  bool - Returns true if skip is successful; otherwise false.
 */
bool tlkmdi_interphone_next(void);

/**
 * @brief   Skips to the previous interphone track.
 * @param   None
 * @return  bool - Returns true if skip is successful; otherwise false.
 */
bool tlkmdi_interphone_previous(void);

/**
 * @brief     Operates on the interphone module with an opcode and data.
 * @param[in] handle  The connection handle.
 * @param[in] opcode  Operation code.
 * @param[in] pdata   Pointer to the data buffer.
 * @param[in] dataLen Length of the data.
 * @return    bool - Returns true if operation is successful; otherwise false.
 */
bool tlkmdi_interphone_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief   Retrieves the ACL handle associated with the interphone.
 * @param   None
 * @return  uint16_t - The ACL handle.
 */
uint16_t tlkmdi_interphone_get_acl_handle(void);
#endif

#endif
