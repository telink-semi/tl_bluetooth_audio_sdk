/********************************************************************************************************
 * @file    tlkdrv_key.c
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
#include "tlkapi/tlkapi.h"
#include "tlkdrv_key.h"

#if (TLK_DEV_KEY_ENABLE)

#define TLKDRV_KEY_LOW_SCAN_PERIOD_MS     50
#define TLKDRV_KEY_HIGH_SCAN_PERIOD_MS    10

#define TLKDRV_KEY_CLICK_TIMESTAMP_MS     400

#define TLKDRV_KEY_SHORT_THRESHOLD_MS     1000
#define TLKDRV_KEY_LONG_THRESHOLD_MS      2000
#define TLKDRV_KEY_LONG_LONG_THRESHOLD_MS 5000

typedef enum
{
    TLKDRV_KEY_STATE_IDLE = 0,
    TLKDRV_KEY_STATE_DEBOUNCING,
    TLKDRV_KEY_STATE_PRESSED,
    TLKDRV_KEY_STATE_RELEASE,
} TLKDRV_KEY_STATE_ENUM;

static tlkdrv_key_ctrl_t sTlkDrvKeyCtrl = {0};

/**
 * @brief       Find and return an idle key unit slot.
 * @param[in]   none.
 * @return      Pointer to an idle key unit, or NULL if none available.
 * @note        An idle unit is identified by having keyID == 0.
 */
static inline tlkdrv_key_unit_t *tlkdrv_key_getIdleUnit(void)
{
    for (size_t index = 0; index < TLKDRV_KEY_MAX_NUMB; index++) {
        if (sTlkDrvKeyCtrl.unit[index].keyID == 0) {
            return &sTlkDrvKeyCtrl.unit[index];
        }
    }
    return NULL;
}

/**
 * @brief       Find and return a key unit that matches the given keyID.
 * @param[in]   keyID    - The key identifier to search for.
 * @return      Pointer to the matching key unit, or NULL if not found or invalid keyID.
 * @note        Returns NULL if keyID is 0 or greater than TLKDRV_KEY_MAX_NUMB.
 */
static inline tlkdrv_key_unit_t *tlkdrv_key_getUsedUnit(uint8_t keyID)
{
    if (keyID == 0 || keyID > TLKDRV_KEY_MAX_NUMB) {
        return NULL;
    }
    for (size_t index = 0; index < TLKDRV_KEY_MAX_NUMB; index++) {
        if (sTlkDrvKeyCtrl.unit[index].keyID == keyID) {
            return &sTlkDrvKeyCtrl.unit[index];
        }
    }
    return NULL;
}

/**
 * @brief       GPIO interrupt handler for key events.
 * @param[in]   none.
 * @return      none.
 * @note        Restarts the key scanning timer and updates the last scan tick.
 */
static void tlkdrv_key_gpioIrqHandler(void)
{
    if (sTlkDrvKeyCtrl.scanTimer) {
        tlkos_timer_startFromISR(sTlkDrvKeyCtrl.scanTimer);
        sTlkDrvKeyCtrl.lastScanTick = clock_time() | 1;
    }
}

/**
 * @brief       Check if a key is released.
 * @param[in]   pUnit    - Pointer to the key unit to check.
 * @return      true if key is released, false otherwise.
 * @note        Handles both simple keys (with only input port) and matrix keys 
 *              (with both input and output ports).
 */
static inline uint8_t tlkdrv_key_release(tlkdrv_key_unit_t *pUnit)
{
    /*
        Principle:
        1 Common key (only one end of the button is connected to the SoC IO port)
            Read the level of the GPIO
        2 Matrix key (both input and output ports are connected to SoC IO port)
            a.When this key is scanned, set outPort fixed level(such as low level),
              pull up the inPort level then read inPort level. If read inPort level
              equal to the valid level set by the key, the key is press.
    */

    if (pUnit->outPort == 0) { // unuse outPort
        uint8_t level = gpio_get_level(pUnit->inPort) ? 1 : 0;
        return (level != pUnit->level);
    }
    // use outPort
    gpio_set_level(pUnit->inPort, !pUnit->level);
    /** set inPort input */
    gpio_function_en(pUnit->inPort);
    gpio_input_en(pUnit->inPort);
    gpio_output_dis(pUnit->inPort);

    /** set outPort output */
    gpio_function_en(pUnit->outPort);
    gpio_input_dis(pUnit->outPort);
    gpio_output_en(pUnit->outPort);

    /** set outPort out level */
    gpio_set_level(pUnit->outPort, pUnit->level); // keep outPort(low) and inPort(high) level

    uint8_t res = pUnit->level ? TLKHAL_GPIO_PULL_DOWN_DEFAULT : TLKHAL_GPIO_PULL_UP_DEFAULT;
    tlkhal_gpio_set_up_down_res(pUnit->inPort, res);

    uint8_t release = gpio_get_level(pUnit->inPort) ? 1 : 0;
    if (release != pUnit->level) {
        return true;
    } else { // other keys press down,and make the same pin to down_level,so reverse the pin to test
        gpio_set_level(pUnit->outPort, !pUnit->level);

        /** set inPort output */
        gpio_function_en(pUnit->inPort);
        gpio_input_dis(pUnit->inPort);
        gpio_output_en(pUnit->inPort);

        /** set outPort input */
        gpio_function_en(pUnit->outPort);
        gpio_input_en(pUnit->outPort);
        gpio_output_dis(pUnit->outPort);

        /** set inPort out level */
        gpio_set_level(pUnit->inPort, pUnit->level);

        uint8_t resCfg = pUnit->level ? TLKHAL_GPIO_PULL_DOWN_DEFAULT : TLKHAL_GPIO_PULL_UP_DEFAULT;
        tlkhal_gpio_set_up_down_res(pUnit->inPort, resCfg);

        release = gpio_get_level(pUnit->outPort) ? 1 : 0;
        if (release != pUnit->level) {
            release = true;
        } else {
            release = false;
        }
    }
    return release;
}

/**
 * @brief       Send a key event notification message.
 * @param[in]   pUnit    - Pointer to the key unit that generated the event.
 * @param[in]   evtID    - The event identifier to send.
 * @return      none.
 * @note        Only sends event if it's enabled in the unit's event mask.
 */
static inline void tlkdrv_key_evtHappenNotify(tlkdrv_key_unit_t *pUnit, uint8_t evtID)
{
    if (((pUnit->evtMsk) & (1 << evtID)) == 0) {
        return;
    }
    uint8_t buffer[2] = {
        [0] = pUnit->keyID,
        [1] = evtID,
    };
    tlksys_sendMsgFromTimer(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_KEY_ACTION, buffer, 2);
}

/**
 * @brief       Handle state transitions from IDLE state in key state machine.
 * @param[in]   pUnit       - Pointer to the key unit.
 * @param[in]   isPress     - Indicates if key is currently pressed.
 * @param[in]   pastTimeMs  - Time elapsed since last check (not used in this state).
 * @return      none.
 * @note        Transitions to PRESSED state if key is pressed, remains in IDLE otherwise.
 */
static inline void tlkdrv_key_state_machine_idle(tlkdrv_key_unit_t *pUnit, uint8_t isPress, uint16_t pastTimeMs)
{
    (void)pastTimeMs;
    pUnit->state  = isPress ? TLKDRV_KEY_STATE_DEBOUNCING : TLKDRV_KEY_STATE_IDLE;
    pUnit->timeMs = 0;
}

/**
 * @brief       Handle state transitions from DEBOUNCING state in key state machine.
 * @param[in]   pUnit       - Pointer to the key unit.
 * @param[in]   isPress     - Indicates if key is currently pressed.
 * @param[in]   pastTimeMs  - Time elapsed since last check.
 * @return      none.
 * @note        Waits for debounce period (8ms) before transitioning to PRESSED state.
 */
static inline void tlkdrv_key_state_machine_debouncing(tlkdrv_key_unit_t *pUnit, uint8_t isPress, uint16_t pastTimeMs)
{
    if (!isPress) {
        pUnit->state = TLKDRV_KEY_STATE_IDLE;
        return;
    }
    if (pUnit->timeMs + pastTimeMs >= 8) { //wait debouncing time 8ms
        pUnit->state  = TLKDRV_KEY_STATE_PRESSED;
        pUnit->timeMs = 0;
    } else {
        pUnit->timeMs += pastTimeMs;
    }
}

/**
 * @brief       Handle state transitions from PRESSED state in key state machine.
 * @param[in]   pUnit       - Pointer to the key unit.
 * @param[in]   isPress     - Indicates if key is currently pressed.
 * @param[in]   pastTimeMs  - Time elapsed since last check.
 * @return      none.
 * @note        Detects short clicks, long clicks, and long-long clicks. 
 *              Manages click counting for multiple consecutive clicks.
 */
static inline void tlkdrv_key_state_machine_pressed(tlkdrv_key_unit_t *pUnit, uint8_t isPress, uint16_t pastTimeMs)
{
    //still pressed:
    if (isPress) {
        pUnit->timeMs += pastTimeMs;
        pUnit->timeMs = min(pUnit->timeMs, 20 * 1000);
        return;
    }
    //this is a short click
    if (pUnit->timeMs <= TLKDRV_KEY_SHORT_THRESHOLD_MS) {
        pUnit->state  = TLKDRV_KEY_STATE_RELEASE;
        pUnit->timeMs = 0;
        pUnit->clickCnt++;
        pUnit->clickCnt = min(pUnit->clickCnt, 100);
        return;
    }

    //this is a long click
    if (pUnit->clickCnt == 0) {
        //only first long click ,send key event
        if (pUnit->timeMs <= TLKDRV_KEY_LONG_THRESHOLD_MS) {
            tlkdrv_key_evtHappenNotify(pUnit, TLKDRV_KEY_EVTID_LONG);
        } else {
            tlkdrv_key_evtHappenNotify(pUnit, TLKDRV_KEY_EVTID_LONG_LONG);
        }
    }
    pUnit->clickCnt = 0;
    pUnit->timeMs   = 0;
    pUnit->state    = TLKDRV_KEY_STATE_IDLE;
}

/**
 * @brief       Handle state transitions from RELEASE state in key state machine.
 * @param[in]   pUnit       - Pointer to the key unit.
 * @param[in]   isPress     - Indicates if key is currently pressed.
 * @param[in]   pastTimeMs  - Time elapsed since last check.
 * @return      none.
 * @note        Detects multiple consecutive clicks and sends appropriate events.
 */
static inline void tlkdrv_key_state_machine_release(tlkdrv_key_unit_t *pUnit, uint8_t isPress, uint16_t pastTimeMs)
{
    //still released:
    if (!isPress) {
        pUnit->timeMs += pastTimeMs;
        uint16_t extraTime = pUnit->clickCnt * 50;
        extraTime          = min(extraTime, 400);
        if (pUnit->timeMs <= TLKDRV_KEY_CLICK_TIMESTAMP_MS + extraTime) {
            return;
        }
        //over time stamp,send event
        uint32_t evt = pUnit->clickCnt + TLKDRV_KEY_EVTID_CLICK - 1;
        if (evt < TLKDRV_KEY_EVTID_MAX) {
            tlkdrv_key_evtHappenNotify(pUnit, evt);
        }
        pUnit->clickCnt = 0;
        pUnit->timeMs   = 0;
        pUnit->state    = TLKDRV_KEY_STATE_IDLE;
        return;
    }
    // press again
    pUnit->timeMs = 0;
    pUnit->state  = TLKDRV_KEY_STATE_PRESSED;
}

/**
 * @brief       Main state machine function for processing key events.
 * @param[in]   pUnit       - Pointer to the key unit to process.
 * @param[in]   pastTimeMs  - Time elapsed since last check.
 * @return      Non-zero if key is active (not in IDLE state), zero otherwise.
 * @note        Delegates to specific state handlers based on current state.
 */
static inline uint8_t tlkdrv_key_state_machine(tlkdrv_key_unit_t *pUnit, uint16_t pastTimeMs)
{
    uint8_t isPress = !tlkdrv_key_release(pUnit);
    switch (pUnit->state) {
    case TLKDRV_KEY_STATE_IDLE:
        tlkdrv_key_state_machine_idle(pUnit, isPress, pastTimeMs);
        break;
    case TLKDRV_KEY_STATE_DEBOUNCING:
        tlkdrv_key_state_machine_debouncing(pUnit, isPress, pastTimeMs);
        break;
    case TLKDRV_KEY_STATE_PRESSED:
        tlkdrv_key_state_machine_pressed(pUnit, isPress, pastTimeMs);
        break;
    case TLKDRV_KEY_STATE_RELEASE:
        tlkdrv_key_state_machine_release(pUnit, isPress, pastTimeMs);
        break;
    default:
        break;
    }
    return pUnit->state != TLKDRV_KEY_STATE_IDLE;
}

/**
 * @brief       Timer callback function for periodic key scanning.
 * @param[in]   timerHandle - Handle of the timer that triggered this callback.
 * @param[in]   userArg     - User-provided argument (unused).
 * @return      none.
 * @note        Processes all registered keys and adjusts scanning frequency based on activity.
 */
static void tlkdrv_key_timer(TlkOsTimerHandle_t timerHandle, void *userArg)
{
    (void)userArg;
    uint8_t  isBusy     = 0;
    uint32_t pastTimeMs = (clock_time() - sTlkDrvKeyCtrl.lastScanTick) / SYSTEM_TIMER_TICK_1MS;
    pastTimeMs          = min(pastTimeMs, 100);
    for (size_t index = 0; index < TLKDRV_KEY_MAX_NUMB; index++) {
        isBusy |= tlkdrv_key_state_machine(&sTlkDrvKeyCtrl.unit[index], pastTimeMs);
    }
    if (isBusy) {
        tlkos_timer_setPeriod(timerHandle, TLKDRV_KEY_HIGH_SCAN_PERIOD_MS);
    } else {
        if (sTlkDrvKeyCtrl.isNeedScanTimer) {
            tlkos_timer_setPeriod(timerHandle, TLKDRV_KEY_LOW_SCAN_PERIOD_MS);
        } else {
            tlkos_timer_stop(timerHandle);
            return;
        }
    }
    sTlkDrvKeyCtrl.lastScanTick = clock_time() | 1;
    tlkos_timer_start(timerHandle);
}

/******************************************************************************
 * Function: tlkdrv_key_insert
 * Descript: Insert a key,initial its GPIO and make timer working.
 * Params:
 *     @keyID[IN]--The ketID, for instance 0x01, 0x02.
 *     @evtMsk[IN]--A marker for key events, refer to 'TLKDRV_KEY_EVTMSK_ENUM'.
 *     @inPort[IN]--inPort
 *     @level[IN]--Key effective level.
 *     @upDown[IN]--refer to 'gpio_pull_type_e'.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
/**
 * @brief       Insert a key, initialize its GPIO and start the timer.
 * @param[in]   keyID    - The key identifier, for instance 0x01, 0x02.
 * @param[in]   evtMsk   - A marker for key events, refer to 'TLKDRV_KEY_EVTMSK_ENUM'.
 * @param[in]   inPort   - Input port configuration.
 * @param[in]   outPort  - Output port configuration.
 * @param[in]   level    - Key effective level.
 * @return      TLK_ENONE is success, other value is failure.
 * @retval      -TLK_EPARAM  Invalid parameter.
 * @retval      -TLK_EREPEAT Key already exists.
 * @retval      -TLK_EQUOTA  No more key slots available.
 * @note        Initializes GPIO settings for both simple and matrix keys.
 */
int tlkdrv_key_insert(uint8_t keyID, uint8_t evtMsk, uint16_t inPort, uint16_t outPort, uint8_t level)
{
    tlkdrv_key_unit_t *pUnit;
    if (keyID == 0 || keyID > TLKDRV_KEY_MAX_NUMB || evtMsk == 0 || inPort == 0) {
        return -TLK_EPARAM;
    }

    pUnit = tlkdrv_key_getUsedUnit(keyID);
    if (pUnit != NULL) {
        return -TLK_EREPEAT;
    }

    pUnit = tlkdrv_key_getIdleUnit();
    if (pUnit == NULL) {
        return -TLK_EQUOTA;
    }

    pUnit->keyID   = keyID;
    pUnit->evtMsk  = evtMsk;
    pUnit->inPort  = inPort;
    pUnit->outPort = outPort;
    pUnit->isrChn  = 0;
    if (level != 0) {
        pUnit->level = 1;
    } else {
        pUnit->level = 0;
    }

    uint8_t pmWakeUpEn = 1;
    if (pUnit->outPort != 0) {
        gpio_function_en(pUnit->outPort);
        gpio_input_dis(pUnit->outPort);
        gpio_output_en(pUnit->outPort);
        uint8_t resCfg = level ? TLKHAL_GPIO_PULL_DOWN_DEFAULT : TLKHAL_GPIO_PULL_UP_DEFAULT;
        tlkhal_gpio_set_up_down_res(pUnit->outPort, resCfg);
        gpio_set_level(pUnit->outPort, level);
        pUnit->isrChn = 0xFF;
        pmWakeUpEn    = 0;
    }

    TlkhalGpioInputCfg_t inputCfg = {
        .gpio          = pUnit->inPort,
        .pullUpDownCfg = level ? TLKHAL_GPIO_PULL_DOWN_DEFAULT : TLKHAL_GPIO_PULL_UP_DEFAULT,
        .pmWakeUpEn    = pmWakeUpEn,
        .pmWakeUpLevel = level ? 1 : 0,
    };
    tlkhal_gpio_setInput(&inputCfg);


    if (tlkos_timer_isHaveIsrApi() == false) {
        pUnit->isrChn = 0xFF;
    }
    if (sTlkDrvKeyCtrl.scanTimer == 0) {
        tlkos_timer_create("key_timer", TLKDRV_KEY_LOW_SCAN_PERIOD_MS, false, tlkdrv_key_timer, NULL, &sTlkDrvKeyCtrl.scanTimer);
    }
    if (pUnit->isrChn != 0xFF) {
        pUnit->isrChn                     = 0xFF;
        uint8_t               triggerMode = level ? TLKHAL_GPIO_IRQ_TRIGGER_RISING_EDGE : TLKHAL_GPIO_IRQ_TRIGGER_FALLING_EDGE;
        TlkhalGpioIrqChnCfg_t cfg         = {
                    .cb          = tlkdrv_key_gpioIrqHandler,
                    .gpio        = inPort,
                    .triggerMode = triggerMode,
        };
        bool res = tlkhal_gpio_mallocIrqChn(&pUnit->isrChn, &cfg);
        if (res == true) {
            tlkhal_gpio_startIrqChn(pUnit->isrChn);
            return TLK_ENONE;
        }
    }
    sTlkDrvKeyCtrl.isNeedScanTimer = 1;
    tlkos_timer_start(sTlkDrvKeyCtrl.scanTimer);
    sTlkDrvKeyCtrl.lastScanTick = clock_time() | 1;
    return TLK_ENONE;
}

static tlkdrv_vendor_config_cb_t s_tlkdrv_key_venodr_config_cb[KEY_EVT_VENDOR_CONFIG_END - KEY_EVT_VENDOR_CONFIG + 1] = {NULL};

/**
 * @brief       Register a vendor configuration callback function.
 * @param[in]   eventMode  - Event mode identifier (KEY_EVT_VENDOR_CONFIG_x).
 * @param[in]   cb         - Callback function to register.
 * @return      none.
 * @note        Only registers callback if eventMode is within valid range.
 */
void tlkdrv_key_registerVendorConfigCallback(uint8_t eventMode, tlkdrv_vendor_config_cb_t cb)
{
    if (eventMode >= KEY_EVT_VENDOR_CONFIG && eventMode <= KEY_EVT_VENDOR_CONFIG_END) {
        s_tlkdrv_key_venodr_config_cb[eventMode - KEY_EVT_VENDOR_CONFIG] = cb;
    }
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_1.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig1Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_1, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_2.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig2Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_2, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_3.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig3Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_3, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_4.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig4Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_4, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_5.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig5Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_5, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_6.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig6Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_6, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_7.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig7Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_7, cb);
}

/**
 * @brief       Register a vendor configuration callback for event mode KEY_EVT_VENDOR_CONFIG_8.
 * @param[in]   cb  - Callback function to register.
 * @return      none.
 * @note        Wrapper for tlkdrv_key_registerVendorConfigCallback with fixed event mode.
 */
void tlkdrv_key_registerVendorConfig8Callback(tlkdrv_vendor_config_cb_t cb)
{
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_8, cb);
}

/**
 * @brief       Execute a vendor configuration callback based on offset ID.
 * @param[in]   offsetId  - Index of the vendor configuration callback to execute.
 * @return      none.
 * @note        Only executes callback if offsetId is valid and callback is not NULL.
 */
void tlkdrv_key_vendorConfigCallback(uint8_t offsetId)
{
    if (offsetId <= ARRAY_SIZE(s_tlkdrv_key_venodr_config_cb)) {
        if (s_tlkdrv_key_venodr_config_cb[offsetId] != NULL) {
            s_tlkdrv_key_venodr_config_cb[offsetId]();
        }
    }
}

#endif // #if (TLK_DEV_KEY_ENABLE)
