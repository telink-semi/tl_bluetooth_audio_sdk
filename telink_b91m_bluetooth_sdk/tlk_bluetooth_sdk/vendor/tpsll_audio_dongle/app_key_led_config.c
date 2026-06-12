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
#include "app_config.h"
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
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_TPD_ENABLE_PAIRING_MODE,
        },
#endif

#ifdef KEY2_ID
    [1] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_MUSIC_PLAY_PAUSE,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_MUSIC_BACKWARD,
        },
#endif

#ifdef KEY3_ID
    [2] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_VOLUME_UP,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_VOLUME_DOWN,
            [TLKDRV_KEY_EVTID_TCLICK] = KEY_EVT_MODE_MUSIC_FORWARD,
        },
#endif

#ifdef KEY4_ID
    [3] =
        {
            [TLKDRV_KEY_EVTID_4CLICK] = KEY_EVT_MODE_TPD_USB_MODE_SWITCH,
        },
#endif
};

/**
 * @brief      Initialize the default key configuration
 * @param[in]  none
 * @return     none
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
 * @brief      Handle key configuration updates in online mode
 * @param[in]  key_id     - the ID of the key being configured
 * @param[in]  key_evt_id - the ID of the key event being configured
 * @param[in]  key_mode   - the mode to assign to the key event
 * @return     none
 * @note       Uses tlkmdi_tinySql_getKeyCofnig and tlkmdi_tinySql_updateKeyCofnig to manage key configurations
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

#endif

/**
 * @brief       This function maps a system LED state to a corresponding LED pattern.
 * @param[in]   state   - the system LED state.
 * @return      the LED pattern corresponding to the given state.
 * @note        The function uses a static array to store default configurations for each LED state.
 */
tlkdrv_led_pattern_e tlkapp_sysLed_stateToPatternHook(TLKAPP_LED_STATE state)
{
    static const tlkdrv_led_pattern_e sTlkapp_sysLed_defaultCfg[TLKAPP_LED_STATE_NUM] = {
        [TLKAPP_LED_STATE_POWERON] = TLKDRV_LED_PATTERN_ON_1S,     [TLKAPP_LED_STATE_POWEROFF] = TLKDRV_LED_PATTERN_ON_1S,
        [TLKAPP_LED_STATE_POWERON_IDLE] = TLKDRV_LED_PATTERN_IDLE, [TLKAPP_LED_STATE_PARING] = TLKDRV_LED_PATTERN_FLASH_FAST,
        [TLKAPP_LED_STATE_CONNECTED] = TLKDRV_LED_PATTERN_ON,      [TLKAPP_LED_STATE_LOWBATTARY] = TLKDRV_LED_PATTERN_SLOW_FLASH_3_TIMES,
    };
    return sTlkapp_sysLed_defaultCfg[state];
}

/**
 * @brief       This function initializes the system key configuration after the initialization is completed.
 * @return      none.
 * @note        It calls the default configuration initialization function for the app key.
 */
void tlkapp_sysKey_initCompletedHook(void)
{
    app_key_defaultConfig_initial();
}
