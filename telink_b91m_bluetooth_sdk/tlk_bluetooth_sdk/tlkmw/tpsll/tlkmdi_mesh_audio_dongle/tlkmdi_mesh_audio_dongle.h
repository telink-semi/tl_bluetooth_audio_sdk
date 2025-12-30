/********************************************************************************************************
 * @file    tlkmdi_mesh_audio_dongle.h
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
#ifndef TLKMDI_MESH_AUDIO_DONGLE_H
#define TLKMDI_MESH_AUDIO_DONGLE_H

typedef enum
{
    TLKMDI_TPMD_STATE_CHANGE_CB_PAIR,
    TLKMDI_TPMD_STATE_CHANGE_CB_CONNECT,
    TLKMDI_TPMD_STATE_CHANGE_CB_DISCONNECT,
} tlkmdi_tpmd_state_change_cb_e;

typedef void (*tlkmdi_tpmd_state_change_cb)(uint8_t state);

typedef void (*tlkmdi_tpmd_hidCmdCB)(uint16_t handle,uint8_t cmd);

typedef enum
{
    TLKMDI_TPMD_HID_CMD_VOL_UP,
    TLKMDI_TPMD_HID_CMD_VOL_DOWN,
    TLKMDI_TPMD_HID_CMD_PLAY_PAUSE,
    TLKMDI_TPMD_HID_CMD_PREV,
    TLKMDI_TPMD_HID_CMD_NEXT,
} TLKMDI_TPMD_HID_CMD;

typedef struct
{
    uint32_t         tpsll_ac;
    uint8_t         tpsll_ch;
    uint8_t         cur_status;
    uint16_t         timeout;
    uint8_t         local_addr[6];
    uint8_t         null_addr0[6];
    uint8_t         null_addr1[6];
    uint8_t         addr_paired_headset[6];
    uint16_t         startPairing;
    uint16_t         headsetIsConn;
} tlkmdi_mesh_audio_dongle_item_t; // 52bytes

/**
 * @brief       Initialize the mesh audio dongle.
 * @return      none.
 * @note        none.
 */
void tlkmdi_mesh_audio_dongle_init(void);

/**
 * @brief      Power on reconnection for the paired headset.
 * @return     TLK_ENONE - success.
 * @note       none.
 */
int tlkmdi_mesh_audio_dongle_powerOnReconHeadset(void);

/**
 * @brief       Initialize the pairing process for the mesh audio dongle.
 * @param[in]   pData   - pointer to the data buffer.
 * @param[in]   dataLen - length of the data buffer.
 * @return      none.
 * @note        This function checks for a valid MAC address and sends a message if the MAC is invalid.
 */
void tlkmdi_mesh_audio_dongle_pairing_init(uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Register the state change callback function.
 * @param[in]   cb    - the callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpmd_regStateChgCB(tlkmdi_tpmd_state_change_cb cb);

/**
 * @brief       Register the hid command callback function.
 * @param[in]   cb    - the callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpmd_regGetHidCmdCB(tlkmdi_tpmd_hidCmdCB cb);

#endif
