/********************************************************************************************************
 * @file    tlkmdi_bthfpag.h
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
#ifndef TLKMDI_BTAG_H
#define TLKMDI_BTAG_H


#define TLKMDI_BTAG_TIMEOUT         1000000                      // us
#define TLKMDI_BTAG_TIMEOUT_MS      (TLKMDI_BTAG_TIMEOUT / 5000) // 20ms
#define TLKMDI_BTAG_RING_TIMEOUT    (3000000 / (TLKMDI_BTAG_TIMEOUT / 2))

#define TLKMDI_AG_CALL_SETUP        (1 && TLKBTP_CFG_HFPAG_ENABLE)
#define TLKMDI_PHONE_NUMBER_MAX_LEN 64
#define TLKMDI_PHONE_SETUP_MAX_NUMB 2

typedef struct
{
    uint8_t       state;
    uint8_t       busys;
    uint8_t       pBtAddr[6];
    uint32_t      feature;
    TlkApiTimer_t timer;
} tlkmdi_btag_ctrl_t;

typedef enum
{
    TLKMDI_BTAG_ATTR_NONE = 0x00,
    TLKMDI_BTAG_ATTR_CONN = 0x01,
    TLKMDI_BTAG_ATTR_CIEV = 0x02,
    TLKMDI_BTAG_ATTR_CLIP = 0x04,
    TLKMDI_BTAG_ATTR_NREC = 0x08,
    TLKMDI_BTAG_ATTR_CMEE = 0x10,
} TLKMDI_HFPAG_ATTR_ENUM;

typedef enum
{
    TLKMDI_BTAG_BUSY_NONE       = 0x00,
    TLKMDI_BTAG_BUSY_OPEN       = 0x02,
    TLKMDI_BTAG_BUSY_RING       = 0x04,
    TLKMDI_BTAG_BUSY_CHG_CODEC  = 0x08,
    TLKMDI_BTAG_BUSY_CHG_MICVOL = 0x10,
    TLKMDI_BTAG_BUSY_CHG_SPKVOL = 0x20,
} TLKMDI_HFPAG_BUSY_ENUM;

typedef enum
{
    TLKMDI_BTAG_INDIC_NONE = 0x00,
    TLKMDI_BTAG_INDIC_SERVICE,
    TLKMDI_BTAG_INDIC_CALL,
    TLKMDI_BTAG_INDIC_CALLSETUP,
    TLKMDI_BTAG_INDIC_BATTCHG,
    TLKMDI_BTAG_INDIC_SIGNAL,
    TLKMDI_BTAG_INDIC_ROAM,
    TLKMDI_BTAG_INDIC_CALLHELD,
} TLKMDI_HFPAG_INDICATOR_ENUM;

typedef enum
{
    TLKMDI_BTAG_SERVICE_NON_EXIST = 0x00,
    TLKMDI_BTAG_SERVICE_EXIST,
} TLKMDI_HFPAG_SERVICE_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_CALL_NOT_PROGRESS = 0x00,
    TLKMDI_BTAG_CALL_IN_PROGRESS,
} TLKMDI_HFPAG_CALLS_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_CALLSETUP_NOT_SETUP = 0x00,
    TLKMDI_BTAG_CALLSETUP_INCOMING,
    TLKMDI_BTAG_CALLSETUP_OUTGOING,
    TLKMDI_BTAG_CALLSETUP_REMOTE_ALERTED,
} TLKMDI_HFPAG_CALLSETUP_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_CALLHELD_NOT_HELD = 0x00,
    TLKMDI_BTAG_CALLHELD_ACTIVE_HELD_SWAPPED, // The AG has both an active AND a held call
    TLKMDI_BTAG_CALLHELD_HOLD_NO_ACTIVE,      // Call on hold, no active call
} TLKMDI_HFPAG_CALLHELD_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_SSIGNAL_STRENGTH_0 = 0x00,
    TLKMDI_BTAG_SSIGNAL_STRENGTH_20P,
    TLKMDI_BTAG_SSIGNAL_STRENGTH_40P,
    TLKMDI_BTAG_SSIGNAL_STRENGTH_60P,
    TLKMDI_BTAG_SSIGNAL_STRENGTH_80P,
    TLKMDI_BTAG_SSIGNAL_STRENGTH_100P,
} TLKMDI_HFPAG_SIGNAL_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_ROAM_NOT_ACTIVE = 0x00,
    TLKMDI_BTAG_ROAM_ACTIVE,
} TLKMDI_HFPAG_ROAM_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_BATTERY_CHARGE_0 = 0x00,
    TLKMDI_BTAG_BATTERY_CHARGE_20P,
    TLKMDI_BTAG_BATTERY_CHARGE_40P,
    TLKMDI_BTAG_BATTERY_CHARGE_60P,
    TLKMDI_BTAG_BATTERY_CHARGE_80P,
    TLKMDI_BTAG_BATTERY_CHARGE_100P,
} TLKMDI_HFPAG_BATTERY_STATE_ENUM;

typedef enum
{
    TLKMDI_BTAG_STATE_IDLE = 0,
    TLKMDI_BTAG_STATE_CONNECTING,
    TLKMDI_BTAG_STATE_CONNECTED,
    TLKMDI_BTAG_STATE_DISCONNECT,
} TLKMDI_BTAG_STATE_ENUM;

typedef enum
{
    TLKMDI_HFPAG_CALL_STATE_NONE = 0,
    TLKMDI_HFPAG_CALL_STATE_PAUSED,
    TLKMDI_HFPAG_CALL_STATE_OPENED,
    TLKMDI_HFPAG_CALL_STATE_RINGING,
    TLKMDI_HFPAG_CALL_STATE_CONNECT,
} TLKMDI_HFPAG_CALL_STATE_ENUM;

typedef enum
{
    TLKMDI_HFP_CALL_HOLD_NO_HELD            = 0x30,
    TLKMDI_HFP_CALL_HOLD_HELD_AND_ACTIVE    = 0x31,
    TLKMDI_HFP_CALL_HOLD_HELD_AND_NO_ACTIVE = 0x32,
} TLKMDI_HFP_CALL_HOLD_ENUM;

typedef enum
{
    TLKMDI_HFP_CALL_STATE_HUNGUP = 0x30,
    TLKMDI_HFP_CALL_STATE_ACTIVE = 0x31,
} TLKMDI_HFP_CALL_STATE_ENUM;

typedef enum
{
    TLKMDI_HFP_CALL_SETUP_IDLE     = 0x30,
    TLKMDI_HFP_CALL_SETUP_INCOMING = 0x31,
    TLKMDI_HFP_CALL_SETUP_OUTGOING = 0x32,
    TLKMDI_HFP_CALL_SETUP_ALERTING = 0x33,
} TLKMDI_HFP_CALL_SETUP_ENUM;

typedef enum
{
    TLKMDI_HFP_CLCC_DIR_OUTGOING = 0x30,
    TLKMDI_HFP_CLCC_DIR_INCOMING = 0x31,
} TLKMDI_HFP_CLCC_DIR_ENUM;

typedef enum
{
    TLKMDI_HFP_CLCC_MODE_VOICE = 0x30,
    TLKMDI_HFP_CLCC_MODE_DATA  = 0x31,
    TLKMDI_HFP_CLCC_MODE_FAX   = 0x32,
} TLKMDI_HFP_CLCC_MODE_ENUM;

typedef enum
{
    TLKMDI_HFP_CLCC_MPTY_NOT_MULTI_PARTY = 0x30,
    TLKMDI_HFP_CLCC_MPTY_MULTI_PARTY     = 0x31,
} TLKMDI_HFP_CLCC_MPTY_ENUM;

typedef enum
{
    TLKMDI_HFP_CLCC_STATUS_NONE     = 0x00,
    TLKMDI_HFP_CLCC_STATUS_ACTIVE   = 0x30, // Active
    TLKMDI_HFP_CLCC_STATUS_HELD     = 0x31, // Held
    TLKMDI_HFP_CLCC_STATUS_DIALING  = 0x32, // Dialing (outgoing calls only)
    TLKMDI_HFP_CLCC_STATUS_ALERTING = 0x33, // Alerting (outgoing calls only)
    TLKMDI_HFP_CLCC_STATUS_INCOMING = 0x34, // Incoming (incoming calls only)
    TLKMDI_HFP_CLCC_STATUS_WAITING  = 0x35, // Waiting (incoming calls only)
    TLKMDI_HFP_CLCC_STATUS_CALLHELD = 0x36, // Call held by Response and Hold
} TLKMDI_HFP_CLCC_STATUS_ENUM;

typedef struct
{
    uint8_t status; // Refer TLKMDI_HFP_CLCC_STATUS_ENUM.
    uint8_t numbLen;
    uint8_t callDir;   // Refer TLKMDI_HFP_CLCC_DIR_ENUM.
    uint8_t callSetup; // Refer TLKMDI_HFP_CALL_SETUP_ENUM.
    uint8_t callIsHold;
    uint8_t reserve3;
    uint8_t number[TLKMDI_PHONE_NUMBER_MAX_LEN];
} tlkmdi_hfpag_unit_t;

typedef struct
{
    uint16_t            busys;
    uint8_t             callHold;    // Refer TLKMDI_HFP_CALL_HOLD_ENUM.
    uint8_t             callState;   // Refer TLKMDI_HFP_CALL_STATE_ENUM.
    bool                disc_active; //Whether to initiate a hang up call.
    uint8_t             resv[3];
    tlkmdi_hfpag_unit_t unit[TLKMDI_PHONE_SETUP_MAX_NUMB];
    TlkApiTimer_t       timer;
} tlkmdi_hfpag_ctrl_t;

/**
 * @brief       Initialize the HFP AG module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_init(void);

/**
 * @brief       Create a SCO connection for HFP AG
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_createSco(uint8_t *pBtAddr);

/**
 * @brief       Disconnect the SCO connection for HFP AG
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfag_disconnSco(uint8_t *pBtAddr);

/**
 * @brief       Get the disc active flag status
 * @param[in]   none.
 * @return      true if disconnect active flag is set, false otherwise.
 *              This flag indicates if a hang up call is being initiated.
 */
bool tlkmdi_bthfpag_get_discActive();

/**
 * @brief       Clear the disc active flag to indicate no hang up call is being initiated
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bthfpag_clear_discActive();

/**
 * @brief       Reset the HFP AG module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_reset(void);

/**
 * @brief       Send mute command to the peer device
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @param[in]   micSpk      - Device type, 1 is mic, 2 is speaker.
 * @param[in]   enable      - Enable or disable mute, 1 is enable, 0 is disable.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_sendMute(uint8_t *pBtAddr, uint8_t micSpk, uint8_t enable);

/**
 * @brief       Insert a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @param[in]   isIncoming  - Call direction, 1 is incoming, 0 is outgoing.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_insertCall(uint8_t *pNumber, uint8_t numbLen, uint8_t isIncoming);

/**
 * @brief       Remove a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_removeCall(uint8_t *pNumber, uint8_t numbLen);

/**
 * @brief       Active a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_activeCall(uint8_t *pNumber, uint8_t numbLen);

/**
 * @brief       Hang up a call
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_hungupCall(void);

/**
 * @brief       Reject waiting call and keep active call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_rejectWaitAndKeepActive(uint16_t aclHandle);

/**
 * @brief       Accept waiting call and hold active call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_acceptWaitAndHoldActive(uint16_t aclHandle);

/**
 * @brief       Hang up active call and resume held call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_hungUpActiveAndResumeHold(uint16_t aclHandle);

/**
 * @brief       Callback function for SCO connection status
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   scoHandle   - The SCO connection handle.
 * @param[in]   isConn      - Connection status, 1 is connected, 0 is disconnected.
 * @return      none.
 */
void tlkmdi_bthfag_scoActive(uint16_t aclHandle, uint16_t scoHandle, bool isConn);

/**
 * @brief       Send CIEV command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   indicators  - Indicator index.
 * @param[in]   indValue    - Indicator value.
 * @return      true is success, false is failure.
 */
bool tlkmdi_bthfpag_sendCievCmd(uint16_t aclHandle, uint8_t indicators, uint8_t indValue);

void tlkmdi_bthfpag_recvBCSCmdDeal(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen);

bool tlkmdi_hfpag_recvClccCmdDeal(uint16_t aclHandle);
bool tlkmdi_bthfpag_recvAtaCmdDeal(uint16_t aclHandle);
bool tlkmdi_bthfpag_recvChupCmdDeal(uint16_t aclHandle);
bool tlkmdi_hfpag_recvBVRACmdDeal(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen);
bool tlkmdi_hfpag_recvBLDNCmdDeal(uint16_t aclHandle);

tlkmdi_hfpag_ctrl_t *tlkmdi_hfpag_getItem(uint16_t handle);

#endif // TLKMDI_BTHFP_H
