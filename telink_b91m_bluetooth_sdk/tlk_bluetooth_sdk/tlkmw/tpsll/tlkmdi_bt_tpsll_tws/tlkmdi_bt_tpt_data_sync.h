/********************************************************************************************************
 * @file    tlkmdi_bt_tpt_data_sync.h
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
#pragma once

typedef void (*TlkmdiBtTptToneTmrSyncCB)(uint32_t synctick);

/**
 * @brief       Initialize the data synchronization module
 * @param[in]   none
 * @return      none
 * @note        This function creates a static timer for LED synchronization
 */
void tlkmdi_bt_tpt_data_sync_init(void);

/**
 * @brief       Send AVRCP key command to remote device
 * @param[in]   handle  - Connection handle
 * @param[in]   key     - Key code to send
 * @return      none
 * @note        This function prepares and executes AVRCP key command synchronization
 */
void tlkmdi_bt_tpt_sendAvrcpKey(uint16_t handle ,uint8_t key);

/**
 * @brief       Trigger Siri voice assistant
 * @param[in]   none
 * @return      none
 * @note        This function prepares and executes Siri trigger command synchronization
 */
void tlkmdi_bt_tpt_triggerSiri(void);

/**
 * @brief       Control call operations (accept/reject)
 * @param[in]   handle   - Connection handle
 * @param[in]   isAccept - Flag indicating whether to accept (true) or reject (false) the call
 * @return      none
 * @note        This function prepares and executes call control command synchronization
 */
void tlkmdi_bt_tpt_callCtrl(uint16_t handle, uint8_t isAccept);

/**
 * @brief       Perform initial volume synchronization
 * @param[in]   handle - Connection handle
 * @return      none
 * @note        This function retrieves stored volume values and synchronizes them, only works on master device
 */
void tlkmdi_bt_tpt_volFirstSync(uint16_t handle);

/**
 * @brief       Synchronize volume between TWS devices
 * @param[in]   handle  - Connection handle
 * @param[in]   isMusic - Flag indicating if it's music volume (true) or voice volume (false)
 * @param[in]   vol     - Volume level to synchronize
 * @return      none
 * @note        This function prepares and executes volume synchronization command
 */
void tlkmdi_bt_tpt_volSync(uint16_t handle, uint8_t isMusic, uint8_t vol);

/**
 * @brief       Send HID key command to remote device
 * @param[in]   handle - Connection handle
 * @param[in]   key    - HID key code to send
 * @return      none
 * @note        This function prepares and executes HID key command synchronization
 */
void tlkmdi_bt_tpt_sendHidkey(uint16_t handle, uint8_t key);

/**
 * @brief       Enable or disable automatic LED synchronization
 * @param[in]   en - Enable flag (true to enable, false to disable)
 * @return      TLK_ENONE - Operation completed successfully
 * @note        When enabled, this function requests initial LED sync and starts the sync timer
 */
int tlkmdi_bt_tpt_setAutoLedSync(bool en);

/**
 * @brief       Request LED synchronization with peer device
 * @param[in]   none
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function only works on master device, sends LED sync request to slave
 */
int tlkmdi_bt_tpt_requestLedSync(void);

/**
 * @brief       Request tone synchronization with peer device
 * @param[in]   tone_id - Tone identifier to synchronize
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function only works on master device, sends tone sync request to slave
 */
int tlkmdi_bt_tpt_requestToneSync(uint8_t tone_id);

/**
 * @brief       Register callback function for tone synchronization
 * @param[in]   cb - Callback function pointer
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function stores the provided callback for later use during tone synchronization
 */
int tlkmdi_bt_tpt_regToneSyncCB(TlkmdiBtTptToneTmrSyncCB cb);

/**
 * @brief       Synchronize A2DP media data from master to slave
 * @param[in]   handle  - Connection handle
 * @param[in]   pData   - Pointer to media data
 * @param[in]   dataLen - Length of media data
 * @return      none
 * @note        This function prepares and executes A2DP media data synchronization from master to slave
 */
void tlkmdi_bt_tpt_masterSyncA2dpMediaDataHandler(uint16_t handle, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Synchronize A2DP status data between TWS devices
 * @param[in]   handle  - Connection handle
 * @param[in]   pData   - Pointer to status data (unused)
 * @param[in]   dataLen - Length of status data (unused)
 * @return      none
 * @note        This function prepares and executes A2DP status synchronization between devices
 */
void tlkmdi_bt_tpt_syncA2dpStatusDataHandler(uint16_t handle, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Synchronize TWS pairing request between devices
 * @param[in]   isRefactory - Refactory mode flag
 * @return      none
 * @note        This function executes TWS pairing synchronization command
 */
void tlkmdi_bt_tpt_sync_tws_pairing(uint8_t isRefactory);

/**
 * @brief       Synchronize exit low latency mode between TWS devices
 * @param[in]   none
 * @return      none
 * @note        This function executes exit low latency mode synchronization command
 */
void tlkmdi_bt_tpt_sync_tws_exitLowLatency(void);

/**
 * @brief       Synchronize charging case state between TWS devices
 * @param[in]   state - Case state to synchronize
 * @return      none
 * @note        This function prepares and executes case state synchronization command
 */
void tlkmdi_bt_tpt_boxStateSync(TlkMdiTptCaseState_t state);

/**
 * @brief       Request handover operation between TWS devices
 * @param[in]   none
 * @return      none
 * @note        This function executes handover request command synchronization
 */
void tlkmdi_bt_tpt_reqHandOver(void);

/**
 * @brief       Synchronize ANC (Active Noise Cancellation) state between TWS devices
 * @param[in]   state - ANC state to synchronize
 * @return      none
 * @note        This function prepares and executes ANC state synchronization command
 */
void tlkmdi_bt_tpt_syncAncState(uint8_t state);

