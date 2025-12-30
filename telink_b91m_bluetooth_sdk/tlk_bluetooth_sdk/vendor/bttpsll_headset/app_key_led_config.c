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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"


#if (TLK_DEV_KEY_ENABLE)
static const uint16_t sApp_key_default_config[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_MUSIC_PLAY_PAUSE,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_3S,
            [TLKDRV_KEY_EVTID_TCLICK] = KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_10S,
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
            [TLKDRV_KEY_EVTID_TCLICK]    = KEY_EVT_MODE_MUSIC_FORWARD,
            [TLKDRV_KEY_EVTID_LONG_LONG] = KEY_EVT_MODE_TRIGGER_SIRI,
        },
#endif


#ifdef KEY4_ID
    [3] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_BTTPSLL_MIC_SWITCH,
            [TLKDRV_KEY_EVTID_TCLICK] = KEY_EVT_MODE_CALL_HUNG_UP,
        },
#endif
};

/**
 * @brief      Initialize the default key configuration
 * @param[in]  none
 * @return     none
 */
static void app_key_defaultConfig_initial(void)
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
 * @brief       This function handles the configuration of a specific key in online mode.
 * @param[in]   key_id    - the identifier of the key. refer to TLKDRV_KEY_DID_ENUM.
 * @param[in]   key_evt_id - the event identifier of the key. Refer to TLKDRV_KEY_EVTID_ENUM.
 * @param[in]   key_mode  - the mode to be set for the key. Refer to tlkdrv_key_evt_mode_e.
 * @return      none.
 * @note        Uses tlkmdi_tinySql_getKeyCofnig and tlkmdi_tinySql_updateKeyCofnig to manage key configurations.
 */
void app_key_config_handle(uint8_t key_id, uint8_t key_evt_id, uint8_t key_mode) /*use for key configuration in online mode */
{
    keyConfigs_t *sApp_key_cur_config = NULL;
    if (sApp_key_cur_config == NULL) {
        tlkmdi_tinySql_getKeyCofnig(&sApp_key_cur_config);
    }
    if (sApp_key_cur_config != NULL) {
        sApp_key_cur_config->EvtModes[key_id][key_evt_id] = key_mode;
        tlkmdi_tinySql_updateKeyCofnig(sApp_key_cur_config);
    }
}

/**
 * @brief       This function is called after the system key initialization is completed.
 * @return      none.
 * @note        Initializes the default configuration for the application keys.
 */
void tlkapp_sysKey_initCompletedHook(void)
{
    app_key_defaultConfig_initial();
}
#endif


/** led use default config**/
/** if don't want use ,overwrite weak function tlkapp_sysLed_stateToPatternHook**/