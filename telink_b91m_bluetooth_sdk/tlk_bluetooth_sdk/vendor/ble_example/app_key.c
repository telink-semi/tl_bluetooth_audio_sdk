/********************************************************************************************************
 * @file    app_key.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"

#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkapp/tlkapp.h"

#if (TLK_DEV_KEY_ENABLE)
/**
 * @brief  Default key configuration table.
 *          Defines the vendor configuration ID for each key under different event types.
 *          First dimension: key index (0-3 corresponds to KEY1-KEY4).
 *          Second dimension: key event type (click, double click, triple click, etc.).
 */
static const uint16_t sApp_key_default_config[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {
            /* KEY1 configuration */
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1, /* Click event maps to vendor config 1 */
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_5, /* Double click event maps to vendor config 5 */
        },
#endif

#ifdef KEY2_ID
    [1] =
        {
            /* KEY2 configuration */
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_2, /* Click event maps to vendor config 2 */
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_6, /* Double click event maps to vendor config 6 */
        },
#endif

#ifdef KEY3_ID
    [2] =
        {
            /* KEY3 configuration */
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_3, /* Click event maps to vendor config 3 */
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_7, /* Double click event maps to vendor config 7 */
        },
#endif


#ifdef KEY4_ID
    [3] =
        {
            /* KEY4 configuration */
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_4, /* Click event maps to vendor config 4 */
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_8, /* Double click event maps to vendor config 8 */
        },
#endif
};

/**
 * @brief  Initialize default key configuration.
 *          Read current key configuration from database. If configuration is empty or uninitialized,
 *          update database with default configuration. If default configuration has been modified,
 *          also update the configuration in database.
 */
static void app_key_defaultConfig_initial(void)
{
    keyConfigs_t *sApp_key_cur_config = NULL;
    /* Get current key configuration from database */
    tlkmdi_tinySql_getKeyCofnig(&sApp_key_cur_config);

    /* Check if KEY1 configuration is empty (uninitialized) */
    if (
#ifdef KEY1_ID
        sApp_key_cur_config != NULL && sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_CLICK] == 0 &&
        sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_DCLICK] == 0 && sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_TCLICK] == 0
#else
        0
#endif
    ) {
        /* Static assertion: ensure configuration structure size matches */
        STATIC_ASSERT_THIS_FILE(sizeof(keyConfigs_t) == sizeof(sApp_key_default_config));
        //tmemcpy(sApp_key_cur_config, &sApp_key_default_config);
        /* Update database with default configuration */
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)sApp_key_default_config);
    }

    /* Temporary: if default configuration has been modified, update database with new default configuration */
    if (sApp_key_cur_config != NULL && tmemcmp(sApp_key_default_config, sApp_key_cur_config, sizeof(keyConfigs_t))) {
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)sApp_key_default_config);
    }
}

/**
 * @brief  Initialize key module.
 *          Initialize default key configuration and clear all vendor configuration callback functions.
 */
static void app_key_init(void)
{
    /* Initialize default key configuration */
    app_key_defaultConfig_initial();

    /* Clear all vendor configuration callback functions (initialize to NULL) */
    tlkdrv_key_registerVendorConfig1Callback(NULL);
    tlkdrv_key_registerVendorConfig2Callback(NULL);
    tlkdrv_key_registerVendorConfig3Callback(NULL);
    tlkdrv_key_registerVendorConfig4Callback(NULL);
    tlkdrv_key_registerVendorConfig5Callback(NULL);
    tlkdrv_key_registerVendorConfig6Callback(NULL);
    tlkdrv_key_registerVendorConfig7Callback(NULL);
    tlkdrv_key_registerVendorConfig8Callback(NULL);
}

/**
 * @brief  Register callback function for a specific key.
 *          Register corresponding vendor configuration callback function according to key ID.
 *          Key ID to vendor configuration mapping:
 *          - key_id 0-3: Corresponds to KEY1-KEY4 click events (vendor config 1-4)
 *          - key_id 4-7: Corresponds to KEY1-KEY4 double click events (vendor config 5-8)
 *
 * @param[in] key_id     Key ID, range 0-7
 *                       - 0: KEY1 click event callback (vendor config 1)
 *                       - 1: KEY2 click event callback (vendor config 2)
 *                       - 2: KEY3 click event callback (vendor config 3)
 *                       - 3: KEY4 click event callback (vendor config 4)
 *                       - 4: KEY1 double click event callback (vendor config 5)
 *                       - 5: KEY2 double click event callback (vendor config 6)
 *                       - 6: KEY3 double click event callback (vendor config 7)
 *                       - 7: KEY4 double click event callback (vendor config 8)
 * @param[in] callback   Callback function pointer, called when corresponding key event is triggered.
 *                       Pass NULL to clear the callback function.
 */
void app_key_register_callback(uint8_t key_id, void (*callback)(void))
{
    if (key_id == 0) {
        tlkdrv_key_registerVendorConfig1Callback(callback);
    } else if (key_id == 1) {
        tlkdrv_key_registerVendorConfig2Callback(callback);
    } else if (key_id == 2) {
        tlkdrv_key_registerVendorConfig3Callback(callback);
    } else if (key_id == 3) {
        tlkdrv_key_registerVendorConfig4Callback(callback);
    } else if (key_id == 4) {
        tlkdrv_key_registerVendorConfig5Callback(callback);
    } else if (key_id == 5) {
        tlkdrv_key_registerVendorConfig6Callback(callback);
    } else if (key_id == 6) {
        tlkdrv_key_registerVendorConfig7Callback(callback);
    } else if (key_id == 7) {
        tlkdrv_key_registerVendorConfig8Callback(callback);
    }
}

/**
 * @brief  System key initialization completion hook function.
 *          Called by framework when system key initialization is completed to perform application layer key initialization.
 */
void tlkapp_sysKey_initCompletedHook(void)
{
    app_key_init();
}
#endif

/**
 * @brief  LED state to pattern mapping hook function.
 *          Use application layer configuration instead of default configuration to map LED state to corresponding LED display pattern.
 *
 * @param[in] state LED state
 *                  - TLKAPP_LED_STATE_POWERON: Power on state
 *                  - TLKAPP_LED_STATE_POWEROFF: Power off state
 *                  - TLKAPP_LED_STATE_POWERON_IDLE: Power on idle state
 *                  - TLKAPP_LED_STATE_PARING: Pairing state
 *                  - TLKAPP_LED_STATE_CONNECTED: Connected state
 *                  - TLKAPP_LED_STATE_LOWBATTARY: Low battery state
 *
 * @return  Corresponding LED display pattern.
 */
tlkdrv_led_pattern_e tlkapp_sysLed_stateToPatternHook(TLKAPP_LED_STATE state)
{
    /* LED state to display pattern mapping table */
    static const tlkdrv_led_pattern_e sTlkapp_sysLed_defaultCfg[TLKAPP_LED_STATE_NUM] = {
        [TLKAPP_LED_STATE_POWERON]      = TLKDRV_LED_PATTERN_ON_1S,              /* Power on: on for 1 second */
        [TLKAPP_LED_STATE_POWEROFF]     = TLKDRV_LED_PATTERN_ON_1S,              /* Power off: on for 1 second */
        [TLKAPP_LED_STATE_POWERON_IDLE] = TLKDRV_LED_PATTERN_IDLE,               /* Power on idle: idle mode */
        [TLKAPP_LED_STATE_PARING]       = TLKDRV_LED_PATTERN_FLASH_FAST,         /* Pairing: fast flash */
        [TLKAPP_LED_STATE_CONNECTED]    = TLKDRV_LED_PATTERN_ON,                 /* Connected: always on */
        [TLKAPP_LED_STATE_LOWBATTARY]   = TLKDRV_LED_PATTERN_SLOW_FLASH_3_TIMES, /* Low battery: slow flash 3 times */
    };
    return sTlkapp_sysLed_defaultCfg[state];
}
