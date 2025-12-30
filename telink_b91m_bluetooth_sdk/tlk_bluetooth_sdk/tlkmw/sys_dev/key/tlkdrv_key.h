/********************************************************************************************************
 * @file    tlkdrv_key.h
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
#ifndef TLKDRV_KEY_H
#define TLKDRV_KEY_H

typedef enum
{
    TLKDRV_KEY_DID_NONE = 0x0000,
    TLKDRV_KEY_DID_KEY1 = 0x0001,
    TLKDRV_KEY_DID_KEY2 = 0x0002,
    TLKDRV_KEY_DID_KEY3 = 0x0003,
    TLKDRV_KEY_DID_KEY4 = 0x0004,
} TLKDRV_KEY_DID_ENUM;

typedef enum
{
    TLKDRV_KEY_EVTID_LONG,
    TLKDRV_KEY_EVTID_LONG_LONG,
    TLKDRV_KEY_EVTID_CLICK,
    TLKDRV_KEY_EVTID_DCLICK,
    TLKDRV_KEY_EVTID_TCLICK,
    TLKDRV_KEY_EVTID_4CLICK,
    TLKDRV_KEY_EVTID_MAX
} TLKDRV_KEY_EVTID_ENUM;

typedef enum
{
    TLKDRV_KEY_EVTMSK_NONE      = 0x00,
    TLKDRV_KEY_EVTMSK_LONG      = (1 << TLKDRV_KEY_EVTID_LONG),
    TLKDRV_KEY_EVTMSK_LONG_LONG = (1 << TLKDRV_KEY_EVTID_LONG_LONG),
    TLKDRV_KEY_EVTMSK_CLICK     = (1 << TLKDRV_KEY_EVTID_CLICK),
    TLKDRV_KEY_EVTMSK_DCLICK    = (1 << TLKDRV_KEY_EVTID_DCLICK),
    TLKDRV_KEY_EVTMSK_TCLICK    = (1 << TLKDRV_KEY_EVTID_TCLICK),
    TLKDRV_KEY_EVTMSK_4CLICK    = (1 << TLKDRV_KEY_EVTID_4CLICK),

    TLKDRV_KEY_EVTMSK_DEFAULT =
        TLKDRV_KEY_EVTMSK_CLICK | TLKDRV_KEY_EVTMSK_DCLICK | TLKDRV_KEY_EVTMSK_TCLICK | TLKDRV_KEY_EVTMSK_4CLICK | TLKDRV_KEY_EVTMSK_LONG | TLKDRV_KEY_EVTMSK_LONG_LONG,


} TLKDRV_KEY_EVTMSK_ENUM;

typedef enum
{
    KEY_EVT_MODE_NONE = 0x00,

    KEY_EVT_MODE_ENABLE_PAIRING_MODE,
    KEY_EVT_MODE_CALL_ACCEPT,
    KEY_EVT_MODE_CALL_HUNG_UP,
    KEY_EVT_MODE_VOLUME_DOWN,
    KEY_EVT_MODE_VOLUME_UP,
    KEY_EVT_MODE_MUSIC_FORWARD,
    KEY_EVT_MODE_MUSIC_BACKWARD,
    KEY_EVT_MODE_MUSIC_START,
    KEY_EVT_MODE_MUSIC_CLOSE,
    KEY_EVT_MODE_MUSIC_PLAY_PAUSE,
    KEY_EVT_MODE_MUSIC_SWITCH,
    KEY_EVT_MODE_ANC_SWITCH,

    KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_3S,
    KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_10S,

    KEY_EVT_MODE_TRIGGER_SIRI,
    KEY_EVT_MODE_TPD_ENABLE_PAIRING_MODE,

    KEY_EVT_MODE_TPD_USB_MODE_SWITCH, // debug for tpsll dongle

    KEY_EVT_MODE_BT_TPT_PAIRING_MODE_3S,
    KEY_EVT_MODE_BT_TPT_PAIRING_MODE_10S,
    KEY_EVT_MODE_BT_TPT_ENTER_LOW_LATENCY,
    KEY_EVT_MODE_BT_TPT_START_HANDOVER,

    KEY_EVT_MODE_BTTPSLL_MIC_SWITCH, // debug for tpsll dongle
    KEY_EVT_MODE_SYSTEM_POWEROFF_REQ,
    KEY_EVT_VENDOR_CONFIG,
    KEY_EVT_VENDOR_CONFIG_1 = KEY_EVT_VENDOR_CONFIG,
    KEY_EVT_VENDOR_CONFIG_2,
    KEY_EVT_VENDOR_CONFIG_3,
    KEY_EVT_VENDOR_CONFIG_4,
    KEY_EVT_VENDOR_CONFIG_5,
    KEY_EVT_VENDOR_CONFIG_6,
    KEY_EVT_VENDOR_CONFIG_7,
    KEY_EVT_VENDOR_CONFIG_8,
    KEY_EVT_VENDOR_CONFIG_9,
    KEY_EVT_VENDOR_CONFIG_10,
    KEY_EVT_VENDOR_CONFIG_11,
    KEY_EVT_VENDOR_CONFIG_12,
    KEY_EVT_VENDOR_CONFIG_END = KEY_EVT_VENDOR_CONFIG_12,

    KEY_EVT_MODE_MAX
} tlkdrv_key_evt_mode_e;

typedef struct
{
    uint8_t  keyID;
    uint8_t  level;
    uint8_t  isrChn;
    uint8_t  state;
    uint8_t  clickCnt;
    uint8_t  evtMsk;
    uint16_t timeMs;
    uint16_t inPort;
    uint16_t outPort;
} tlkdrv_key_unit_t;

typedef struct
{
    uint8_t            isNeedScanTimer;
    uint8_t            resv3byte[3];
    uint32_t           lastScanTick;
    TlkOsTimerHandle_t scanTimer;
    tlkdrv_key_unit_t  unit[TLKDRV_KEY_MAX_NUMB];
} tlkdrv_key_ctrl_t;

typedef void (*tlkdrv_vendor_config_cb_t)(void);

/**
 * @brief       Insert a key, initialize its GPIO and start the timer.
 * @param[in]   keyID    - The key identifier, refer to 'TLKDRV_KEY_DID_ENUM'.
 * @param[in]   evtMsk   - A marker for key events, refer to 'TLKDRV_KEY_EVTMSK_ENUM'.
 * @param[in]   inPort   - Input port configuration.
 * @param[in]   outPort  - Output port configuration.
 * @param[in]   level    - Key effective level.
 * @return      TLK_ENONE is success, other value is failure.
                -TLK_EPARAM  Invalid parameter.
                -TLK_EREPEAT Key already exists.
                -TLK_EQUOTA  No more key slots available.
 * @note        Initializes GPIO settings for both simple and matrix keys.
 */
int tlkdrv_key_insert(uint8_t keyID, uint8_t evtMsk, uint16_t inPort, uint16_t outPort, uint8_t level);

/**
 * @brief       Register a vendor configuration callback function.
 * @param[in]   eventMode  - Event mode identifier (KEY_EVT_VENDOR_CONFIG_x).
 * @param[in]   cb         - Callback function to register.
 * @return      none.
 * @note        Only registers callback if eventMode is within valid range.
 */
void tlkdrv_key_registerVendorConfigCallback(uint8_t eventMode, tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_1.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig1Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_2.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig2Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_3.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig3Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_4.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig4Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_5.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig5Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_6.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig6Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_7.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig7Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_8.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig8Callback(tlkdrv_vendor_config_cb_t cb);

/**
 * @brief       Execute a vendor configuration callback based on offset ID.
 * @param[in]   offsetId  - Index of the vendor configuration callback to execute.
 * @return      none.
 * @note        Only executes callback if offsetId is valid and callback is not NULL.
 */
void tlkdrv_key_vendorConfigCallback(uint8_t offsetId);

#endif // #ifndef TLKDRV_KEY_H
