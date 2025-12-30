/********************************************************************************************************
 * @file    app_key_led_config.c
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkapp/tlkapp.h"


#if (TLK_DEV_KEY_ENABLE)
static const uint16_t sApp_key_default_config[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_MUSIC_PLAY_PAUSE,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_MUSIC_FORWARD,
            [TLKDRV_KEY_EVTID_TCLICK] = KEY_EVT_MODE_ENABLE_PAIRING_MODE,
        },
#endif

#ifdef KEY2_ID
    [1] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_CALL_ACCEPT,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_MUSIC_BACKWARD,
        },
#endif

#ifdef KEY3_ID
    [2] =
        {
            [TLKDRV_KEY_EVTID_CLICK]     = KEY_EVT_MODE_VOLUME_UP,
            [TLKDRV_KEY_EVTID_LONG]      = KEY_EVT_MODE_TRIGGER_SIRI,
            [TLKDRV_KEY_EVTID_DCLICK]    = KEY_EVT_MODE_VOLUME_DOWN,
            [TLKDRV_KEY_EVTID_LONG_LONG] = KEY_EVT_MODE_TRIGGER_SIRI,
        },
#endif

#ifdef KEY4_ID
    [3] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_2,
            [TLKDRV_KEY_EVTID_TCLICK] = KEY_EVT_MODE_CALL_HUNG_UP,
        },
#endif
};

/**
 * @brief       Initialize default key configuration
 * @param       none
 * @return      none
 * @note        This function initializes the default key configuration in tinySQL database
 *              if no valid configuration is present. It sets up key mappings for various
 *              key events like click, double-click, triple-click, etc.
 */
void app_key_defaultConfig_initial(void)
{
    keyConfigs_t *sApp_key_cur_config = NULL;
    tlkmdi_tinySql_getKeyCofnig(&sApp_key_cur_config);
    if (
#ifdef KEY1_ID
        sApp_key_cur_config != NULL && sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_CLICK] == 0 &&
        sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_DCLICK] == 0 && sApp_key_cur_config->EvtModes[KEY1_ID][TLKDRV_KEY_EVTID_TCLICK] == 0
#else
        0
#endif
    ) {
        STATIC_ASSERT_THIS_FILE(sizeof(keyConfigs_t) == sizeof(sApp_key_default_config));
        //tmemcpy(sApp_key_cur_config, &sApp_key_default_config);
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)sApp_key_default_config);
    }
    /****TEMP add for default configuration be modified, use new default configuration*/
    if (sApp_key_cur_config != NULL && tmemcmp(sApp_key_default_config, sApp_key_cur_config, sizeof(keyConfigs_t))) {
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)sApp_key_default_config);
    }
}

/**
 * @brief       Handle key configuration updates
 * @param[in]   key_id - Key identifier
 * @param[in]   key_evt_id - Key event identifier
 * @param[in]   key_mode - Key mode to set
 * @return      none
 * @note        This function updates the key configuration in tinySQL database
 *              with the specified key mode for the given key and event
 */
void app_key_config_handle(uint8_t key_id, uint8_t key_evt_id, uint8_t key_mode) /*use for key configuration in online mode */
{
    keyConfigs_t *sApp_key_cur_config = NULL;
    if (sApp_key_cur_config == NULL) {
        tlkmdi_tinySql_getKeyCofnig(&sApp_key_cur_config);
    }
    if (sApp_key_cur_config != NULL) {
        sApp_key_cur_config->EvtModes[key_id][key_evt_id] = key_mode;
        sApp_key_cur_config->EvtModes[key_id][key_evt_id] = key_mode;
        tlkmdi_tinySql_updateKeyCofnig(sApp_key_cur_config);
    }
}

/**
 * @brief       Hook function called when system key initialization is completed
 * @param       none
 * @return      none
 * @note        This function initializes the default key configuration
 */
void tlkapp_sysKey_initCompletedHook(void)
{
    app_key_defaultConfig_initial();
}


#endif

/** led use default config**/
/** if don't want use ,overwrite weak function tlkapp_sysLed_stateToPatternHook**/