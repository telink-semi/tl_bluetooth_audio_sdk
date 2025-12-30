/********************************************************************************************************
 * @file    tlkmdi_bt_tph.h
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
#ifndef TLKMDI_BT_TPH_H
#define TLKMDI_BT_TPH_H

typedef void (*tlkmdi_tph_state_change_cb)(uint8_t state);
typedef void (*tlkmdi_tph_force_idle_finished_cb)(void);
typedef enum 
{    
    TLKMDI_TPSLL_STATE_CHANGE_CB_PAIR,
    TLKMDI_TPSLL_STATE_CHANGE_CB_CONNECT,
    TLKMDI_TPSLL_STATE_CHANGE_CB_DISCONNECT,
} tlkmdi_tph_state_change_cb_e;

enum  
{    
    TLKMDI_TPSLL_NO_INIT,
    TLKMDI_TPSLL_IDLE,
    TLKMDI_TPSLL_PAIRING_ASYNC_DISCON_WAITING,
    TLKMDI_TPSLL_PAIRING_ASYNC_DISCONNECTED,
    TLKMDI_TPSLL_PAIRING_BT_DSICON_WAITING,
    TLKMDI_TPSLL_PAIRING_BT_DISCONNECTED,
    TLKMDI_TPSLL_PAIRING_CONNECT_WAITING,
    TLKMDI_TPSLL_CONNECTED,
    TLKMDI_TPSLL_CRASH,
    TLKMDI_TPSLL_BT_CONN_ASYNC_DISCON,
};

enum 
{
    TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY  = 0x40,
    TLK_MDI_DONGLE_ACL_CMD_DONGLE_LED = 0x41,
};

enum
{
    TLK_MDI_HID_AUDIO_RELEASE,
    TLK_MDI_HID_AUDIO_VOL_UP,
    TLK_MDI_HID_AUDIO_VOL_DOWN,
    TLK_MDI_HID_AUDIO_PLAY_PAUSE,
    TLK_MDI_HID_AUDIO_FORWARD,
    TLK_MDI_HID_AUDIO_BACKWARD,
    TLK_MDI_HID_AUDIO_STOP,
    TLK_MDI_HID_AUDIO_PAUSE,
    TLK_MDI_HID_AUDIO_LAST,

    TLK_MDI_HID_AUDIO_MAX,
};

/**
 * @brief       Initializes the Bluetooth TPH module.
 * @return      none.
 * @note        This function sets up the necessary callbacks, initializes control structures,
 *              and get the local address and AC/CH settings from the Tiny SQL database.
 */
void tlkmdi_bt_tph_init(void);

/**
 * @brief       Starts the pairing process for the Bluetooth TPH module.
 * @param[in]   isRefactory - flag indicating if the pairing mode is refactory(10s) or not(3s).
 * @return      none.
 * @note        This function sets up the pairing process, cleans previous pairing data if refactory,
 *              and initiates a disconnection before starting the pairing timer.
 */
void tlkmdi_bt_tph_pair_start(bool isRefactory);

/**
 * @brief       This function handles BT TPH related tasks.
 * @return      none.
 * @note        Currently, it only calls the pairing handler function.
 */
int tlkmdi_bt_tph_pairing_handler(void);

/**
 * @brief       This function starts the reconnection process for a paired dongle.
 * @return      none.
 * @note        The function checks if the paired dongle address is null or invalid before attempting to reconnect.
 */
void tlkmdi_bt_tph_handler(void);

void tlkmdi_bt_tph_dongle_reconnStart(void);

/**
 * @brief       This function registers a callback for state changes.
 * @param[in]   cb      - the callback function to be registered.
 * @return      none.
 * @note        The callback function will be called on dongle connection and disconnection events.
 */
void tlkmdi_bt_tph_regStateChangeCB(tlkmdi_tph_state_change_cb cb);

/**
 * @brief       This function checks if the dongle is connected.
 * @return      true if connected, false otherwise.
 * @note        The function returns the current connection status of the dongle.
 */
bool tlkmdi_bt_tph_dongle_isConnect(void);

/**
 * @brief       This function forces the system to idle state.
 * @param[in]   cb      - the callback function to be called when the idling is finished.
 * @return      none.
 * @note        The function sets the system to force idle mode and starts the disconnection process.
 */
void tlkmdi_bt_tph_forceToIdle(tlkmdi_tph_force_idle_finished_cb cb);

/**
 * @brief       This function restarts the BT TPH module.
 * @return      none.
 * @note        The function initiates a restart of the BT TPH module,      
 *              setting up necessary parameters and starting reconnection.
 */
void tlkmdi_bt_tph_restart(void);
#endif
