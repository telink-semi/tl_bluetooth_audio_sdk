/********************************************************************************************************
 * @file    tlkmdi_btacl.h
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
#ifndef TLKMDI_BTACL_H
#define TLKMDI_BTACL_H

#define TLKMDI_BTACL_TIMEOUT              200000 // us
#define TLKMDI_BTACL_TIMEOUT_MS           200
#define TLKMDI_BTACL_CONN_DEF_TIMEOUT     (20000000 / TLKMDI_BTACL_TIMEOUT)
#define TLKMDI_BTACL_CONN_MIN_TIMEOUT     (10000000 / TLKMDI_BTACL_TIMEOUT) // 6S
#define TLKMDI_BTACL_CONN_MAX_TIMEOUT     (40000000 / TLKMDI_BTACL_TIMEOUT) // 40S -- Spec specifies that the maximum value is 40.9s
#define TLKMDI_BTACL_DISC_DEF_TIMEOUT     (8000000 / TLKMDI_BTACL_TIMEOUT)
#define TLKMDI_BTACL_DISC_MIN_TIMEOUT     (5000000 / TLKMDI_BTACL_TIMEOUT)
#define TLKMDI_BTACL_DISC_MAX_TIMEOUT     (30000000 / TLKMDI_BTACL_TIMEOUT)

#define TLKMDI_BTACL_IDLE_DEF_TIMEOUT     (30000000 / TLKMDI_BTACL_TIMEOUT)
#define TLKMDI_BTACL_IDLE_DEF_TIMEOUT1    (3000000 / TLKMDI_BTACL_TIMEOUT) // 3s

#define TLKMDI_BTACL_PROFILE_CONN_TIMEOUT (15000000 / TLKMDI_BTACL_TIMEOUT)
#define TLKMDI_BTACL_PROFILE_DISC_TIMEOUT (10000000 / TLKMDI_BTACL_TIMEOUT)


#define TLKMDI_BTACL_ITEM_NUMB            TLK_STK_BTACL_NUMB
#define TLKMDI_BTACL_PROF_NUMB            8

#define TLKMDI_BTACL_PEER_ANDROID_DEV     0
#define TLKMDI_BTACL_PEER_IOS_DEV         1


#define TLKMW_BT_EVT_REGISTER(evtID, func)             \
    int evtID##_FUNC(uint8_t *pData, uint16_t dataLen) \
    {                                                  \
        return func(pData, dataLen);                   \
    }

typedef enum
{
    TLKMDI_BTACL_STAGE_NONE         = 0,
    TLKMDI_BTACL_CONNING_STAGE_NONE = 0,
    TLKMDI_BTACL_CONNING_STAGE_OPEN,
    TLKMDI_BTACL_CONNING_STAGE_WAIT,

    TLKMDI_BTACL_DISCING_STAGE_NONE = 0,
    TLKMDI_BTACL_DISCING_STAGE_PROFILE,
    TLKMDI_BTACL_DISCING_STAGE_MIDDLE,
    TLKMDI_BTACL_DISCING_STAGE_ACL,
    TLKMDI_BTACL_DISCING_STAGE_WAIT,
} TLKMDI_BTACL_STAGE_ENUM;

typedef enum
{
    TLKMDI_BTACL_ATTR_NONE    = 0x00,
    TLKMDI_BTACL_ATTR_REQUEST = 0x01,
    TLKMDI_BTACL_ATTR_CONNECT = 0x02,
    TLKMDI_BTACL_ATTR_ENCRYPT = 0x04,
} TLKMDI_BTACL_ATTRS_ENUM;

typedef enum
{
    TLKMDI_BTACL_BUSY_NONE      = 0x00,
    TLKMDI_BTACL_BUSY_CONN_PROF = 0x02,
    TLKMDI_BTACL_BUSY_DISC_PROF = 0x04,
    TLKMDI_BTACL_WAIT_DISC_PROF = 0x08,
    TLKMDI_BTACL_BUSY_DISC_RFC  = 0x01,
    TLKMDI_BTACL_WAIT_DISC_RFC  = 0x02,
    TLKMDI_BTACL_BUSY_DISC_ACL  = 0x80,
    TLKMDI_BTACL_WAIT_DISC_ACL  = 0x40,
    TLKMDI_BTACL_WAIT_CANCEL    = 0x20,
} TLKMDI_BTACL_BUSYS_ENUM;

typedef enum
{
    TLKMDI_BTACL_FLAG_NONE = 0x00,

} TLKMDI_BTACL_FLAGS_ENUM;

typedef enum
{
    TLKMDI_BT_RFC_CHID_HFP     = 0,
    TLKMDI_BT_RFC_CHID_SPP     = 1,
    TLKMDI_BT_RFC_CHID_IAP     = 2,
    TLKMDI_BT_RFC_CHID_PBAP    = 3,
    TLKMDI_BT_RFC_CHID_BIP     = 4,
    TLKMDI_BT_RFC_CHID_MAX     = 5,
    TLKMDI_BT_RFC_GATT_SUPPORT = TLKMDI_BT_RFC_CHID_MAX,
    TLKMDI_BT_AVRCP_ArtPsm     = 6,
} TLKMDI_BT_RFC_CH_PSM_ID_ENUM;

enum
{
    TLKMW_BT_CONNECT_REQUEST = 0,
    TLKMW_BT_CONNECT_COMPLETE,
    TLKMW_BT_DISCONNECT_COMPLETE,
    TLKMW_BT_ENCRYPTION_COMPLETE,
    TLKMW_BT_PROFILE_CONNECT,
    TLKMW_BT_PROFILE_DISCONNECT,
    TLKMW_BT_AFH_NOTIFY,
    TLKMW_BT_SET_LOCAL_ROLE,

    TLKMW_BT_EVENT_ID_MAX,
};

typedef int (*tlkmw_bt_event_func)(uint8_t *pData, uint16_t dataLen);

typedef struct
{
    uint8_t  state;
    uint8_t  ptype; // Profile Type, Refer to BTP_PTYPE_ENUM.
    uint8_t  usrID;
    uint8_t  cwait; // Connect Wait Time. ()
    uint16_t resv;
    uint16_t delay; //
} tlkmdi_btacl_prof_t;

typedef struct
{
    uint8_t profIsConn : 1;
    uint8_t active     : 1;
    uint8_t audioBusy  : 1;
    uint8_t resv       : 5;
    uint8_t flags;
    uint8_t state;
    uint8_t stage;

    uint8_t busys;
    uint8_t attrs;
    uint8_t btaddr[6];

    uint16_t handle;
    uint16_t timeout;
    uint16_t connFlag; // Connect Flag
    uint16_t idleTime;

    uint8_t  hfChannel;
    uint8_t  agChannel;
    uint8_t  sppChannel;
    uint8_t  iapChannel;
    uint8_t  pbapChannel;
    uint8_t  bipChannel;
    uint16_t avrcpCoverArtPsm;

    uint32_t devClass;

    TlkApiTimer_t       timer;
    tlkmdi_btacl_prof_t prof[TLKMDI_BTACL_PROF_NUMB];
} tlkmdi_btacl_item_t;

typedef struct
{
    tlkmdi_btacl_item_t item[TLKMDI_BTACL_ITEM_NUMB];
} tlkmdi_btacl_ctrl_t;

typedef struct
{
    uint32_t dev_class;
    uint8_t *pBtAddr;
} tlkmdi_bt_conn_request_evt_format;

typedef struct
{
    uint8_t  status;
    uint8_t  dtype;
    uint8_t  hfp_channel;
    uint8_t  resv[3];
    uint16_t aclHandle;
    uint8_t *pBtAddr;
} tlkmdi_bt_connect_evt_format;

typedef struct
{
    uint8_t  reason;
    uint8_t  dtype;
    uint16_t aclHandle;
    uint8_t *pBtAddr;
} tlkmdi_bt_disconnect_evt_format;

typedef struct
{
    uint8_t  status;
    uint8_t  dtype;
    uint8_t  hfp_channel;
    uint8_t  enable;
    uint8_t  resv[2];
    uint16_t aclHandle;
    uint8_t *pBtAddr;
} tlkmdi_bt_encryption_evt_format;

typedef struct
{
    uint8_t  status;
    uint8_t  ptype;
    uint8_t  usrID;
    uint8_t  is_first_prof;
    uint16_t aclHandle;
    uint16_t resv;
    uint8_t *pBtAddr;
} tlkmdi_bt_profile_connect_evt_format;

typedef struct
{
    uint8_t  reason;
    uint8_t  ptype;
    uint8_t  usrID;
    uint8_t  resv[3];
    uint16_t aclHandle;
    uint8_t *pBtAddr;
} tlkmdi_bt_profile_disconnect_evt_format;

typedef struct
{
    uint16_t aclHandle;
    uint16_t dataLen;
    uint8_t *pData;
} tlkmdi_bt_afh_notify_evt_format;

typedef struct
{
    uint32_t devClass;
    uint32_t timeout;
    uint8_t *pBtAddr;
    void    *param;
} tlkmdi_bt_set_local_role_format;

/**
 * @brief       This function initializes the ACL control block and register the callback
 * @param[in]   none.
 * @return      Return TLK_ENONE is success, other value is failure.
 */
int tlkmdi_btacl_init(void);

/**
 * @brief       This function checks the acl status is connected
 * @param[in]   handle    - The ACL handle
 * @return      Return true is active, false is de-active
 */
bool tlkmdi_btacl_isActive(uint16_t handle);

/**
 * @brief       This function checks rfcomm whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isHaveRfc(uint16_t handle);

/**
 * @brief       This function checks HFP whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindHfp(uint16_t handle);

/**
 * @brief       This function checks SPP whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindSpp(uint16_t handle);

/**
 * @brief       This function checks Pbap whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindPbap(uint16_t handle);

/**
 * @brief       This function checks iap whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindIap(uint16_t handle);

/**
 * @brief       This function checks bip whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindBip(uint16_t handle);

/**
 * @brief       This function cancels the acl link setup procedure
 * @param[in]   pBtAddr    - The bt address
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_cancel(uint8_t *pBtAddr);

/**
 * @brief       This function creates a connection setup procedure
 * @param[in]   pBtAddr     - The bt address
 * @param[in]   devClass    - The Device type
 * @param[in]   timeout     - The timeout value. Unit:ms, Range:3000~3600000
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_connect(uint8_t *pBtAddr, uint32_t devClass, uint32_t timeout); // timeout: unit-ms

/**
 * @brief       This function connects the acl link and set a timeout to handle acl connect timeout
 * @param[in]   btaddr     - The BT address
 * @param[in]   timeout    - The timeout value. Unit:ms
 * @return      TLK_ENONE is success, others value is failure
 */
int tlkmdi_btacl_connectEx(uint8_t btaddr[6], uint32_t timeout);

/**
 * @brief       This function disconnects the connection
 * @param[in]   handle     - The ACL handle
 * @param[in]   reason     - The disconnect reason
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_disconn(uint16_t handle, uint8_t reason);

/**
 * @brief       This function disconnects the acl link which specify by bt address
 * @param[in]   btaddr    - The Bt address
 * @return      TLK_ENONE is success, others value is failure
 */
int tlkmdi_btacl_disconnByAddr(uint8_t btaddr[6]);

/**
 * @brief       This function adds a new profile to a acl link
 * @param[in]   handle     - The ACL handle
 * @param[in]   ptype      - The profile type
 * @param[in]   usrID      - The user id
 * @param[in]   delayMs    - The delay time
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_appendProf(uint16_t handle, uint8_t ptype, uint8_t usrID, uint16_t delayMs);

/**
 * @brief       This function removes a profile from the acl link
 * @param[in]   handle    - The ACL handle
 * @param[in]   ptype     - The profile type
 * @param[in]   usrID     - The user id
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_removeProf(uint16_t handle, uint8_t ptype, uint8_t usrID);

/**
 * @brief       This function gets the role of acl link
 * @param[in]   devClass    - The device class
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_getRole(uint32_t devClass);

/**
 * @brief       This function gets the idle acl link count
 * @param[in]   none.
 * @return      The count of idle acl link
 */
uint8_t tlkmdi_btacl_getIdleCount(void);

/**
 * @brief       This function gets the used acl link count
 * @param[in]   none.
 * @return      The count of used acl link
 */
uint8_t tlkmdi_btacl_getUsedCount(void);

/**
 * @brief       This function gets the connected acl link count
 * @param[in]   none.
 * @return      The count of connected acl link
 */
uint8_t tlkmdi_btacl_getConnCount(void);

/**
 * @brief       This function resets the acl control block
 * @param[in]   pItem    - The acl control block
 * @return      none.
 */
void tlkmdi_btacl_resetItem(tlkmdi_btacl_item_t *pItem);

/**
 * @brief       This function gets a idle acl control block
 * @param[in]   none.
 * @return      The idle acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getIdleItem(void);

/**
 * @brief       This function gets a Connecting acl control block
 * @param[in]   none.
 * @return      The connecting acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getBusyItem(void);

/**
 * @brief       This function gets a Used acl control block
 * @param[in]   handle    - The ACL handle
 * @return      The used acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getUsedItem(uint16_t handle);

/**
 * @brief       This function gets a Connected acl control block
 * @param[in]   handle    - The ACL handle
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getConnItem(uint16_t handle);

/**
 * @brief       This function gets a Connected acl control block
 * @param[in]   index    - The index of connected acl control block
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getConnItemByIndex(uint8_t index);

/**
 * @brief       This function finds a Used acl control block
 * @param[in]   pBtAddr    - The Bt address of acl
 * @return      The Used acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_searchUsedItem(uint8_t *pBtAddr);

/**
 * @brief       This function finds a Connected acl control block
 * @param[in]   pBtAddr    - The Bt address of acl
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_searchConnItem(uint8_t *pBtAddr);

/**
 * @brief       This function checks if profile is connected
 * @param[in]   aclHandle    - The ACL handle
 * @param[in]   ptype        - The profile type
 * @return      true if profile is connected, false otherwise
 */
bool tlkmdi_btacl_searchProfIsConn(uint16_t aclHandle, uint8_t ptype);

/**
 * @brief       This function sets sniff busy flag
 * @param[in]   handle       - The ACL handle
 * @param[in]   audioBusy    - Audio busy flag
 * @return      none.
 */
void tlkmdi_btacl_setSniffBusy(uint16_t handle, uint8_t audioBusy);

/**
 * @brief       This function clears sniff busy flag
 * @param[in]   handle       - The ACL handle
 * @param[in]   audioBusy    - Audio busy flag
 * @return      none.
 */
void tlkmdi_btacl_clsSniffBusy(uint16_t handle, uint8_t audioBusy);

/**
 * @brief       This function checks if device is IOS device
 * @param[in]   handle    - The ACL handle
 * @return      true if device is IOS, false otherwise
 */
bool tlkmdi_btacl_isIOS_device(uint16_t handle);

/**
 * @brief     Provides a hook function to customer deal remote device info.
 * @param[in] None.
 * @returns   None
 */
void  tlkmdi_btacl_getRemoteNameChange(uint8_t *pData);
void  tlkmdi_btacl_set_remote_dev(uint8_t *pData, uint8_t isTrue);
void *tlkmdi_btacl_get_remote_dev(void);


#endif // TLKMDI_BTACL_H
