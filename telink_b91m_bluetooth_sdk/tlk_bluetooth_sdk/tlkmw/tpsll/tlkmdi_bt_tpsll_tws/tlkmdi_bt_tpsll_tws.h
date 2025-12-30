/********************************************************************************************************
 * @file    tlkmdi_bt_tpsll_tws.h
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
#ifndef TLKMDI_BT_TPSLL_TWS_H
#define TLKMDI_BT_TPSLL_TWS_H

#include "stack/tpsll/common/bttpsll_tws_profile.h"

#define BD_ADDR_LEN                                6
#define LOCAL_NAME_LEN_MAX                         32


#define TLK_MDI_BT_TPT_TIMER_PERIOD_US             1000
#define TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_MS 500
#define TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS       (TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_MS) /*+ 500)*/
#define TLK_MDI_BT_TPT_SETUP_CONTROLLER_TIMEOUT_US (TLK_MDI_BT_TPT_SETUP_HOST_TIMEOUT_MS * 1000)
#define TLK_MDI_BT_TPT_WAIT_N22_READY_TIMEOUT_MS   2000
#define TLKMDI_BT_TPT_TWS_WITHOUT_TWS_MASK         0xFE7B

enum TLKMDI_BT_TPT_CONNECT_STATUS
{
    TLKMDI_BT_TPT_CONNECT_STATUS_IDLE = 0,
    TLKMDI_BT_TPT_CONNECT_STATUS_CONNECTED,
    TLKMDI_BT_TPT_CONNECT_STATUS_DISCONNECTED,
};

enum TLKMDI_BT_TPT_PAIR_STATUS
{
    TLKMDI_BT_TPT_RECON_STATUS_IDLE = 0,
    TLKMDI_BT_TPT_RECON_STATUS_WAIT_N22_READY,
    TLKMDI_BT_TPT_RECON_STATUS_TWS_RECONNECTING,
    TLKMDI_BT_TPT_RECON_STATUS_TWS_CONNECTED,
    TLKMDI_BT_TPT_RECON_STATUS_MSW_WAITING, /* Only low latency mode to normal mode. MSW - mode switch */
    TLKMDI_BT_TPT_RECON_STATUS_MAX,

    TLKMDI_BT_TPT_PAIR_STATUS_IDLE,
    TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCON_WAITING, /*Only 3s pairing mode need discon dongle if exist */
    TLKMDI_BT_TPT_PAIR_STATUS_DONGLE_DISCONNECTED,
    TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCON_WAITING, /*Only for 10s pairing mode*/
    TLKMDI_BT_TPT_PAIR_STATUS_TWS_DONGLE_DISCONNECTED,   /*Only for 10s  pairing mode*/
    TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCON_WAITING,
    TLKMDI_BT_TPT_PAIR_STATUS_BT_DISCONNECTED,
    TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_WAITING,
    TLKMDI_BT_TPT_PAIR_STATUS_TWS_SYNC_COMPLETE,
    TLKMDI_BT_TPT_PAIR_STATUS_CONNECT_WAITING,
    TLKMDI_BT_TPT_PAIR_STATUS_MAX,
};

typedef enum
{
    TLKMDI_TPT_STATE_CHANGE_CB_PAIR,
    TLKMDI_TPT_STATE_CHANGE_CB_CONNECT,
    TLKMDI_TPT_STATE_CHANGE_CB_DISCONNECT,
} tlkmdi_tpt_state_change_cb_e;

typedef void (*tlkmdi_tpt_state_change_cb)(uint8_t state);

typedef struct
{
    uint8_t const_addr[BD_ADDR_LEN];     /*Local bt addr.*/
    uint8_t peer_addr[BD_ADDR_LEN];      /*Remote bt addr.*/
    uint8_t new_alloc_addr[BD_ADDR_LEN]; /*Use this addr to connect BT.(Left addr)*/
    uint8_t gen_ac_addr[BD_ADDR_LEN];    /*Use this addr to generate AC chn.*/
    uint8_t paired_dongle_addr[BD_ADDR_LEN];
    uint8_t ble_ch; /* not save flash will get by gen_ac_addr when poweron.*/
    uint8_t cur_role;

    // word 9
    uint8_t headset_status;
    uint8_t dongle_status;
    uint8_t ll_audio_mode; // 0 is normal(10/20ms), 1 is ultra mode(5ms), default is 0.
    uint8_t pair_mode    : 4;
    uint8_t pair_inBox   : 1;
    uint8_t is_shutdown  : 1;
    uint8_t hoPending    : 1; // HandOver Pending
    uint8_t isBusyWhenHo : 1; // when HandOver the BT is connecting

    // word 10
    uint8_t  cur_status;
    uint8_t  handover_status : 4;
    uint8_t  peerAclSetup    : 4; // slave headset acl setup
    uint16_t timeout;

    // word 11
    uint8_t *p_cur_addr;
    uint32_t ble_ac; /* not save flash will get by gen_ac_addr when poweron.*/

    // word 13
    btp_tws_get_bt_app_state_info_t handover_info;
    TlkApiTimer_t                   timer; // 6 word
    TlkApiTimer_t                   powerOffTimer;
    TlkApiTimer_t                   handoverTimer;

    // word 31
    uint8_t  btAddr[BD_ADDR_LEN];
    uint8_t  btHoAddr[BD_ADDR_LEN]; //  extract info when handover
    uint16_t busyHandleWhenHo;

} tlkmdi_bt_tpt_ctrl_t;

/**
 * @brief       Initialize the TWS (True Wireless Stereo) Bluetooth module and TPSLL protocol.
 * @return      none.
 * @note        none.
 */
void tlkmdi_bt_tpt_init(void);

/**
 * @brief       Start the TWS pairing process for dual headset reconnection
 * @param[in]   none
 * @return      none
 * @note        This function initializes the pairing mode to normal mode, sets up the headset 
 *              access code and channel ID, configures timeout and status for waiting N22 ready, 
 *              and starts the timer for the TWS pairing process.
 */
void tlkmdi_bt_tpt_start(void);

/**
 * @brief       Start the TWS pairing process
 * @param[in]   isRefactory - Pairing mode, refer to tpt_headset_setup_mode_for_host_e
 * @param[in]   peerMac - Peer device MAC address, if NULL triggers wireless pairing, 
 *              if not NULL indicates pairing with charging box
 * @return      none
 * @note        This function initializes the pairing process based on the specified mode.
 *              For 10-second pairing mode, it clears pairing information and handles 
 *              charging box pairing if peerMac is provided. It also handles termination
 *              of existing Bluetooth connections if needed and sets up appropriate 
 *              timeouts and statuses for the pairing process.
 */
void tlkmdi_bt_tpt_pair_start(uint8_t isRefactory, uint8_t *peerMac);

/**
 * @brief       Start the TWS pairing request process
 * @param[in]   isRefactory - Pairing mode, refer to tpt_headset_setup_mode_for_host_e
 *              - TPT_HOST_HEADSET_SETUP_MODE_NORMAL: Normal mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_3S: 3-second pairing mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_10S: 10-second pairing mode
 *              - TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY: Ultra low latency mode
 * @param[in]   peerMac - Peer headset MAC address, if NULL, will trigger wireless pairing
 * @return      None
 * @note        This function initiates the TWS pairing process. It checks connection status,
 *              handles special cases like ultra-low latency mode, synchronizes pairing between
 *              TWS headsets, and manages the pairing sequence based on the specified mode.
 */
void tlkmdi_bt_tpt_pair_start_req(uint8_t isRefactory, uint8_t *peerMac);

/**
 * @brief       Start pairing synchronization from remote device, triggered when remote single headset starts wireless pairing
 *              and notifies another TWS headset to enter pairing mode
 * @param[in]   data: Pointer to the data containing pairing mode information
 * @param[in]   dataLen: Length of the data (unused in current implementation)
 * @return      none
 * @note        
 */
void tlkmdi_bt_tpt_pair_start_sync_from_remote(void *data, uint16_t dataLen);

/**
 * @brief       Get the current TWS role of the device
 * @return      Current TWS role value
 *              Possible values:
 *              - BTH_TPT_TWS_ROLE_IDLE: No TWS connection
 *              - BTH_TPT_TWS_ROLE_SINGLE: Single device mode (no TWS partner)
 *              - BTH_TPT_TWS_ROLE_MASTER: Master role in TWS connection
 *              - BTH_TPT_TWS_ROLE_ARBITER: Arbiter role in TWS connection
 *              - BTH_TPT_TWS_ROLE_SLAVE: Slave role in TWS connection
 *              - BTH_TPT_TWS_ROLE_OBSERVER: Observer role in TWS connection
 * @note        This function returns the current TWS (True Wireless Stereo) role 
 *              stored in the global control structure. The role determines the 
 *              device's behavior in a TWS system.
 */
uint8_t tlkmdi_bt_tpt_getRole();

/**
 * @brief       Check if the TWS handover process is currently busy
 * @return      Returns true if handover is in progress and device is not busy, otherwise returns false
 * @note        This function checks if a handover operation is active (handover_status is non-zero)
 *              and the device is not marked as busy during handover (isBusyWhenHo is false).
 *              It is typically used to determine if handover operations can proceed.
 */
uint8_t tlkmdi_bt_tpt_handover_isBusy();

/**
 * @brief       This function initiates the handover process for TWS (True Wireless Stereo) devices.
 *              Depending on the device role (master or slave), it either starts the handover command
 *              or requests a handover from the master device.
 * @return      none.
 * @note        
 */
void tlkmdi_bt_tpt_handover_start();

/**
 * @brief       Register callback function for TWS state change notifications
 * @param[in]   cb - Callback function to be invoked when TWS state changes
 * @return      none.
 * @note        none.
 */
void tlkmdi_bt_tpt_regStateChangeCB(tlkmdi_tpt_state_change_cb cb);

/**
 * @brief       Shut down the TWS device
 * @param[in]   none.
 * @return      none.
 * @note        This function handles the shutdown process for TWS devices,
 *              sending disconnection command if device is slave and peer is out of case,
 *              otherwise setting power management channel accordingly.
 */
void tlkmdi_bt_tpt_shut_down(void);

/**
 * @brief       Exit the ultra low latency mode for TWS headset
 * @return      none
 * @note        This function handles the transition from ultra low latency mode back to normal mode.
 *              It performs necessary synchronization with the peer headset and initiates 
 *              reconnection procedures. The function only operates when currently in 
 *              ultra low latency mode and when dongle is connected without active BT ACL links.
 */
void tlkmdi_bt_tpt_headset_exitLowLatencyMode(void);

/**
 * @brief       This function handles the event when peer device exits low latency mode.
 *              It updates the connection status and triggers reconnection if needed.
 * @param[in]   data    - pointer to received data
 * @param[in]   dataLen - length of received data
 * @return      none.
 * @note        
 */
void tlkmdi_bt_tpt_headset_recvPeerExitLowLatencyHandler(void *data, uint16_t dataLen);

/**
 * @brief       This function gets the current audio channel of the TWS device.
 * @param[in]   none.
 * @param[out]  none.
 * @return      uint8_t - the current audio channel, ALG_CHANNEL_LEFT or ALG_CHANNEL_RIGHT.
 * @note        
 */
uint8_t tlkmdi_bt_tpt_audio_getCurChannel(void);

/**
 * @brief       This function extracts mono PCM data from stereo PCM data according to the specified channel.
 * @param[in]   pcm_stereo    - the start address of the stereo PCM data buffer.
 * @param[out]  pcm_mono      - the start address of the mono PCM data buffer.
 * @param[in]   length        - the length of PCM data to process.
 * @param[in]   channel       - the audio channel to extract (ALG_CHANNEL_LEFT, ALG_CHANNEL_RIGHT or ALG_CHANNEL_STEREO).
 * @return      1 if successfully extracted mono data from stereo, 0 if stereo channel is selected.
 * @note        
 */
uint8_t tlkmdi_bt_tpt_audio_getMonoPcmData(int32_t *pcm_stereo, int32_t *pcm_mono, uint16_t length, uint8_t channel);

/**
 * @brief       Check if there is a pending handover operation
 * @return      true if handover is pending, false otherwise
 * @note        This function checks the internal control structure's hoPending flag
 *              which indicates whether a handover operation is currently pending
 */
bool tlkmw_host_isC2hPending(void);
#endif
