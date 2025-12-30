/********************************************************************************************************
 * @file    tlkmdi_tpsll_audio_dongle.h
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
#ifndef TLKMDI_TPSLL_AUDIO_DONGLE_H
#define TLKMDI_TPSLL_AUDIO_DONGLE_H

typedef enum 
{    
    TLKMDI_TPD_STATE_CHANGE_CB_PAIR,
    TLKMDI_TPD_STATE_CHANGE_CB_CONNECT,
    TLKMDI_TPD_STATE_CHANGE_CB_DISCONNECT,
} tlkmdi_tpd_state_change_cb_e;

typedef void (*tlkmdi_tpd_state_change_cb)(uint8_t state);

typedef void (*tlkmdi_tpd_hidCmdCB)(uint16_t handle,uint8_t cmd);

typedef enum
{
    TLKMDI_TPD_HID_CMD_VOL_UP,
    TLKMDI_TPD_HID_CMD_VOL_DOWN,
    TLKMDI_TPD_HID_CMD_PLAY_PAUSE,
    TLKMDI_TPD_HID_CMD_PREV,
    TLKMDI_TPD_HID_CMD_NEXT,
} TLKMDI_TPD_HID_CMD;

typedef struct
{
    //word 0
    uint32_t         tpsll_ac;
    uint8_t         tpsll_ch;
    uint8_t         cur_status;
    uint16_t         timeout;
    //word 3
    uint8_t         local_addr[6];
    uint8_t         addr_paired_headset[6];
    // word 7
    uint16_t         startPairing;
    uint16_t         headsetIsConn;
} tlkmdi_tpsll_audio_dongle_item_t; // 24bytes

/**
 * @brief       Initialize the tpsll audio dongle control structure and registers necessary handlers.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpsll_audio_dongle_init(void);

/**
 * @brief       Power onreconnection of the headset by checking the MAC address and initiating a connection scan.
 * @return      TLK_ENONE - success.
 * @note        none.
 */
int tlkmdi_tpsll_audio_dongle_powerOnReconHeadset(void);

/**
 * @brief       Initialize the pairing process for the tpsll audio dongle.
 * @param[in]   pData   - Pointer to data (not used in this function).
 * @param[in]   dataLen - Length of data (not used in this function).
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpsll_audio_dongle_pairing_init(uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Register the state change callback function.
 * @param[in]   cb      - The callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpd_regStateChgCB(tlkmdi_tpd_state_change_cb cb);

/**
 * @brief       Register the hid command callback function.
 * @param[in]   cb      - The callback function to be registered.
 * @return      none.
 * @note        none.
 */
void tlkmdi_tpd_regGetHidCmdCB(tlkmdi_tpd_hidCmdCB cb);

#endif
