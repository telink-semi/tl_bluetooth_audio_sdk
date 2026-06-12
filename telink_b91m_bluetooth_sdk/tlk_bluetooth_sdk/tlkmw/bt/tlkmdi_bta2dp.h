/********************************************************************************************************
 * @file    tlkmdi_bta2dp.h
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
#ifndef TLKMDI_BT_A2DP_H
#define TLKMDI_BT_A2DP_H

#define TLKMDI_COVER_ART_BUFFER_TOTAL_SIZE 2048
#define TLKMDI_COVER_ART_BUFFER_SEND_SIZE  128

#define TLKMDI_COVER_ART_SERIAL_TRAN_INTV  5000

/**
 * @brief       Initialize the A2DP module.
 *              This function initializes the A2DP control block and registers necessary callbacks.
 * @param[in]   none
 * @return      TLK_ENONE on success, other values indicate failure
 */
int tlkmdi_bta2dp_init(void);

/**
 * @brief       Handle A2DP connection event.
 *              This function processes connection events for A2DP connections.
 * @param[in]   aclHandle - The ACL connection handle
 * @param[in]   usrID     - User identifier for the connection
 * @return      none
 */
void tlkmdi_bta2dp_connectEvt(uint16_t aclHandle, uint8_t usrID);

/**
 * @brief       Send music state change event to host.
 *              This function sends notification about music playback state changes to the host.
 * @param[in]   handle - Connection handle associated with the music state change
 * @param[in]   state  - New music playback state (e.g., play, pause, stop)
 * @return      none
 */
void tlkmdi_bta2dp_sendHostMusicStateChgEvt(uint16_t handle, uint8_t state);

int tlkmdi_btavrcp_volumeChangeEvt(uint8_t *pData, uint16_t dataLen);

#endif // TLKMDI_BT_A2DP_H
