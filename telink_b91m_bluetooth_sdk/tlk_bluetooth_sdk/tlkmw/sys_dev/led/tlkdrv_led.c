/********************************************************************************************************
 * @file    tlkdrv_led.c
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
#include "tlkdrv_led.h"
#if (TLK_DEV_LED_ENABLE && TLKDRV_LED_MAX_NUMB != 0)

/******************************************************************************
                           private code begin
******************************************************************************/
#define TLKDRV_LED_SYNC_STATE_MAGIC_NUM 0XFFFFFFFE
#define TLKDRV_LED_PWM_PCLK_SPEED       1000000 // pwm clock 1M.

/**
 * @brief	LED PWM clock enumeration
 */
enum TLKDRV_LED_PWM_CLOCK_ENUM
{
    TLKDRV_LED_PWM_CLOCK_1S    = TLKDRV_LED_PWM_PCLK_SPEED,
    TLKDRV_LED_PWM_CLOCK_1MS   = (TLKDRV_LED_PWM_CLOCK_1S / 1000),
    TLKDRV_LED_PWM_CLOCK_1US   = (TLKDRV_LED_PWM_CLOCK_1S / 1000000),
    TLKDRV_LED_PWM_CLOCK_10MS  = (TLKDRV_LED_PWM_CLOCK_1S / 100),
    TLKDRV_LED_PWM_CLOCK_100MS = (TLKDRV_LED_PWM_CLOCK_1S / 10),
    TLKDRV_LED_PWM_CLOCK_10US  = (TLKDRV_LED_PWM_CLOCK_1S / 100000),
    TLKDRV_LED_PWM_CLOCK_100US = (TLKDRV_LED_PWM_CLOCK_1S / 10000),
};

/**
 * @brief	LED followed enumeration
 */
enum
{
    TLKDRV_LED_FOLLOWED_MAX_NUM = 16,
    TLKDRV_LED_FOLLOWED_NONE    = TLKDRV_LED_FOLLOWED_MAX_NUM - 1,
    TLKDRV_LED_FOLLOWED_CHILD   = TLKDRV_LED_FOLLOWED_NONE - 1, //child so that follower
};

/**
 * @brief	LED unit control structure
 */
typedef struct
{
    uint8_t  isInvt      : 1; //!< Invert control flag
    uint8_t  isCtrl      : 1; //!< Control flag
    uint8_t  isOpen      : 1; //!< Open flag
    uint8_t  level       : 1; //!< LED level
    uint8_t  followChild : 4; //!< Follow child index
    uint8_t  pwmID;           //!< PWM ID
    uint16_t ioPort;          //!< IO port
    uint16_t count;           //!< Count
    uint16_t dutyMax;         //!< Maximum duty cycle
    uint16_t curDuty;         //!< Current duty cycle
    uint16_t stepChg;         //!< Step change
    uint16_t lastTask;        //!< Last task
    uint16_t nowTask;         //!< Current task
    uint16_t waitingTask;     //!< Waiting task
    uint16_t flushTmoMs;      //!< Flush timeout in milliseconds
    uint16_t flushTmrMs;      //!< Flush timer in milliseconds
    uint16_t onTmoMs;         //!< On timeout in milliseconds
    uint16_t offTmoMs;        //!< Off timeout in milliseconds
    uint16_t resv;            //!< Reserved
    uint32_t runTmrUs;        //!< Run timer in microseconds
    uint32_t lastTime;        //!< Last time
} tlkdrv_led_unit_t;          //!< PWM Control LED

/**
 * @brief	LED control structure
 */
typedef struct
{
    uint32_t           syncTimer;                 //!< Synchronization timer
    TlkOsTimerHandle_t timerHandle;               //!< Timer handle
    tlkdrv_led_unit_t  unit[TLKDRV_LED_MAX_NUMB]; //!< LED units array
} tlkdrv_led_ctrl_t;

/**
 * @brief	LED pattern structure
 */
typedef struct
{
    uint16_t behavior;    //!< Behavior: 0=ledoff, 1=ledon, 2=flash, 3=breath
    uint16_t stepUs;      //!< Step in microseconds
    uint16_t dutyFlushMs; //!< Duty flush in milliseconds
    uint16_t onTimerMs;   //!< On timer in milliseconds
    uint16_t offTimerMs;  //!< Off timer in milliseconds
    uint16_t flashCounts; //!< Flash counts
} tlkdrv_led_pattern_t;

static tlkdrv_led_ctrl_t sTlkDrvLedCtrl = {0};

static const tlkdrv_led_pattern_t tlkdrv_led_patterns[TLKDRV_LED_PATTERN_MAX_STATE] = {
    [TLKDRV_LED_PATTERN_OFF]                = {.behavior = 0, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 0, .offTimerMs = 0, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_ON]                 = {.behavior = 1, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 0, .offTimerMs = 0, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_ON_LOW_DUTY]        = {.behavior = 1, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 0, .offTimerMs = 0, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_FLASH_SLOW]         = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 2000, .offTimerMs = 2000, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_FLASH_FAST]         = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_FLASH_PAIR]         = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 500, .offTimerMs = 1500, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_IDLE]               = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 200, .offTimerMs = 800, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_FAST_FLASH_1_TIME]  = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 1},
    [TLKDRV_LED_PATTERN_FAST_FLASH_2_TIMES] = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 2},
    [TLKDRV_LED_PATTERN_SLOW_FLASH_2_TIMES] = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 500, .offTimerMs = 500, .flashCounts = 2},
    [TLKDRV_LED_PATTERN_FAST_FLASH_3_TIMES] = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 3},
    [TLKDRV_LED_PATTERN_ON_500MS]           = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 500, .offTimerMs = 0, .flashCounts = 1},
    [TLKDRV_LED_PATTERN_ON_1S]              = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 1000, .offTimerMs = 0, .flashCounts = 1},
    [TLKDRV_LED_PATTERN_SLOW_FLASH_3_TIMES] = {.behavior = 2, .stepUs = 0, .dutyFlushMs = 0, .onTimerMs = 500, .offTimerMs = 500, .flashCounts = 3},

    [TLKDRV_LED_PATTERN_BREATH_SLOW]         = {.behavior = 3, .stepUs = 25, .dutyFlushMs = 10, .onTimerMs = 2000, .offTimerMs = 2000, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_BREATH_FAST]         = {.behavior = 3, .stepUs = 125, .dutyFlushMs = 5, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_BREATH_PAIR]         = {.behavior = 3, .stepUs = 50, .dutyFlushMs = 5, .onTimerMs = 500, .offTimerMs = 500, .flashCounts = 0xFFFFU},
    [TLKDRV_LED_PATTERN_SLOW_BREATH_3_TIMES] = {.behavior = 3, .stepUs = 25, .dutyFlushMs = 10, .onTimerMs = 2000, .offTimerMs = 2000, .flashCounts = 3},
    [TLKDRV_LED_PATTERN_FAST_BREATH_3_TIMES] = {.behavior = 3, .stepUs = 125, .dutyFlushMs = 5, .onTimerMs = 200, .offTimerMs = 200, .flashCounts = 3},
};

/**
 * @brief       This function handles LED timer events.
 * @param[in]   TimerHandle    - Timer handle.
 * @param[in]   userArg        - User argument.
 * @return      none.
 */
static void tlkdrv_led_timer(TlkOsTimerHandle_t TimerHandle, void *userArg);

/**
 * @brief       This function enters critical section for LED operations.
 * @return      none.
 */
_always_inline static void tlkdrv_led_enter_critical(void)
{
    tlksys_enter_critical();
}

/**
 * @brief       This function leaves critical section for LED operations.
 * @return      none.
 */
_always_inline static void tlkdrv_led_leave_critical(void)
{
    tlksys_leave_critical();
}

/**
 * @brief       This function re-synchronizes LED timer.
 * @param[in]   TimerHandle    - Timer handle.
 * @param[in]   syncTick       - Synchronization tick.
 * @return      none.
 */
static void tlkdrv_led_timer_reSync(TlkOsTimerHandle_t TimerHandle, uint32_t syncTick)
{
    uint32_t biasTimeUs = (clock_time() - syncTick) / SYSTEM_TIMER_TICK_1US;
    if (biasTimeUs >= 100 * 1000) {
        return;
    }
    uint32_t nextTime = 100 * 1000 - biasTimeUs;
    tlkos_timer_setPeriod(TimerHandle, nextTime / 1000);
}

/**
 * @brief       This function gets an idle LED unit.
 * @return      Pointer to idle LED unit, or NULL if none available.
 */
static tlkdrv_led_unit_t *tlkdrv_led_getIdleUnit(void)
{
    uint8_t index;
    for (index = 0; index < TLKDRV_LED_MAX_NUMB; index++) {
        if (sTlkDrvLedCtrl.unit[index].ioPort == 0) {
            break;
        }
    }
    if (index == TLKDRV_LED_MAX_NUMB) {
        return NULL;
    }
    return &sTlkDrvLedCtrl.unit[index];
}

/**
 * @brief       This function gets a used LED unit by IO port.
 * @param[in]   ioPort    - IO port to search for.
 * @return      Pointer to used LED unit, or NULL if not found.
 */
static tlkdrv_led_unit_t *tlkdrv_led_getUsedUnit(uint16_t ioPort)
{
    uint8_t index;
    if (ioPort == 0) {
        return NULL;
    }
    for (index = 0; index < TLKDRV_LED_MAX_NUMB; index++) {
        if (sTlkDrvLedCtrl.unit[index].ioPort == ioPort) {
            break;
        }
    }
    if (index == TLKDRV_LED_MAX_NUMB) {
        return NULL;
    }
    return &sTlkDrvLedCtrl.unit[index];
}

/**
 * @brief       This function turns on an LED unit.
 * @param[in]   pUnit         - Pointer to LED unit.
 * @param[in]   pwmDutyUs     - PWM duty cycle in microseconds.
 * @param[in]   pwmDutyMaxUs  - Maximum PWM duty cycle in microseconds.
 * @return      none.
 */
static void tlkdrv_led_innerOn(tlkdrv_led_unit_t *pUnit, uint16_t pwmDutyUs, uint16_t pwmDutyMaxUs)
{
    pUnit->isOpen = true;
    if (pUnit->pwmID <= PWM5_ID) {
        pwm_set_tmax(pUnit->pwmID, pwmDutyMaxUs * TLKDRV_LED_PWM_CLOCK_1US);
        pwm_set_tcmp(pUnit->pwmID, pwmDutyUs * TLKDRV_LED_PWM_CLOCK_1US);
        tlkhal_pwm_chnStart(pUnit->pwmID, NULL);
        if (pUnit->followChild < TLKDRV_LED_MAX_NUMB) {
            tlkdrv_led_unit_t *pChildUnit = &sTlkDrvLedCtrl.unit[pUnit->followChild];
            pwm_set_tmax(pChildUnit->pwmID, pwmDutyMaxUs * TLKDRV_LED_PWM_CLOCK_1US);
            pwm_set_tcmp(pChildUnit->pwmID, pwmDutyUs * TLKDRV_LED_PWM_CLOCK_1US);
            tlkhal_pwm_chnStart(pChildUnit->pwmID, NULL);
        }
    } else {
        gpio_set_level(pUnit->ioPort, pUnit->level);
        if (pUnit->followChild < TLKDRV_LED_MAX_NUMB) {
            tlkdrv_led_unit_t *pChildUnit = &sTlkDrvLedCtrl.unit[pUnit->followChild];
            gpio_set_level(pChildUnit->ioPort, pChildUnit->level);
        }
    }
}

/**
 * @brief       This function turns off an LED unit.
 * @param[in]   pUnit    - Pointer to LED unit.
 * @return      none.
 */
static void tlkdrv_led_innerOff(tlkdrv_led_unit_t *pUnit)
{
    pUnit->isOpen = false;
    if (pUnit->pwmID <= PWM5_ID) {
        pwm_set_tcmp(pUnit->pwmID, 0);
        tlkhal_pwm_chnStop(pUnit->pwmID, NULL);
        if (pUnit->followChild < TLKDRV_LED_MAX_NUMB) {
            tlkdrv_led_unit_t *pChildUnit = &sTlkDrvLedCtrl.unit[pUnit->followChild];
            pwm_set_tcmp(pChildUnit->pwmID, 0);
            tlkhal_pwm_chnStop(pChildUnit->pwmID, NULL);
        }
    } else {
        gpio_set_level(pUnit->ioPort, !pUnit->level);
        if (pUnit->followChild < TLKDRV_LED_MAX_NUMB) {
            tlkdrv_led_unit_t *pChildUnit = &sTlkDrvLedCtrl.unit[pUnit->followChild];
            gpio_set_level(pChildUnit->ioPort, !pChildUnit->level);
        }
    }
}

/**
 * @brief       This function turns on an LED with specified parameters.
 * @param[in]   pUnit         - Pointer to LED unit.
 * @param[in]   pwmPeriodUs   - PWM period in microseconds.
 * @param[in]   pwmDutyUs     - PWM duty cycle in microseconds.
 * @return      true.
 */
static bool tlkdrv_led_on(tlkdrv_led_unit_t *pUnit, uint16_t pwmPeriodUs, uint16_t pwmDutyUs)
{
    pUnit->isCtrl = false;
    tlkdrv_led_innerOn(pUnit, pwmDutyUs, pwmPeriodUs);
    return true;
}

/**
 * @brief       This function turns off an LED.
 * @param[in]   pUnit    - Pointer to LED unit.
 * @return      true.
 */
static bool tlkdrv_led_off(tlkdrv_led_unit_t *pUnit)
{
    pUnit->isCtrl = false;
    tlkdrv_led_innerOff(pUnit);
    return true;
}

/**
 * @brief       This function automatically controls LED with specified pattern.
 * @param[in]   isInTimer      - Whether called from timer context.
 * @param[in]   pUnit          - Pointer to LED unit.
 * @param[in]   count          - Number of repetitions.
 * @param[in]   onTimerMs      - On time in milliseconds.
 * @param[in]   offTimerMs     - Off time in milliseconds.
 * @param[in]   pwmPeriodUs    - PWM period in microseconds.
 * @param[in]   pwmDutyUs      - PWM duty cycle in microseconds.
 * @param[in]   dutyFlushMs    - Duty flush time in milliseconds.
 * @param[in]   stepUs         - Step in microseconds.
 * @return      true if successful, false otherwise.
 */
static bool tlkdrv_led_auto(bool isInTimer, tlkdrv_led_unit_t *pUnit, uint16_t count, uint16_t onTimerMs, uint16_t offTimerMs, uint16_t pwmPeriodUs, uint16_t pwmDutyUs,
                            uint16_t dutyFlushMs, uint16_t stepUs)
{
    if (onTimerMs == 0) {
        return false; // Param Error
    }
    if (dutyFlushMs != 0 && (stepUs == 0 || stepUs >= pwmPeriodUs)) {
        return false; // Param Error
    }

    pUnit->count      = count;
    pUnit->isCtrl     = true;
    pUnit->isInvt     = false;
    pUnit->stepChg    = stepUs * TLKDRV_LED_PWM_CLOCK_1US;
    pUnit->onTmoMs    = onTimerMs;
    pUnit->offTmoMs   = offTimerMs;
    pUnit->flushTmoMs = dutyFlushMs;
    pUnit->runTmrUs   = 0;
    pUnit->flushTmrMs = 0;
    pUnit->lastTime   = clock_time();
    pUnit->dutyMax    = pwmDutyUs * TLKDRV_LED_PWM_CLOCK_1US;
    if (pUnit->flushTmoMs != 0) {
        pUnit->curDuty = pUnit->stepChg;
    } else {
        pUnit->curDuty = pUnit->dutyMax;
    }

    tlkdrv_led_innerOn(pUnit, pUnit->curDuty, pUnit->dutyMax);

    if (isInTimer) {
        return true;
    }
    if (sTlkDrvLedCtrl.timerHandle == 0) {
        tlkos_timer_create("led_timer", 1, true, tlkdrv_led_timer, NULL, &sTlkDrvLedCtrl.timerHandle);
    }
    tlkos_timer_stop(sTlkDrvLedCtrl.timerHandle);
    tlkos_timer_setPeriod(sTlkDrvLedCtrl.timerHandle, 1);
    tlkos_timer_start(sTlkDrvLedCtrl.timerHandle);
    return true;
}

/**
 * @brief       This function selects LED pattern internally.
 * @param[in]   isInTimer       - Whether called from timer context.
 * @param[in]   pUnit           - Pointer to LED unit.
 * @param[in]   patternSelect   - Pattern to select.
 * @return      true if successful, false otherwise.
 */
static bool tlkdrv_led_patternSelect_inner(bool isInTimer, tlkdrv_led_unit_t *pUnit, tlkdrv_led_pattern_e patternSelect)
{
    const tlkdrv_led_pattern_t *pattern = &tlkdrv_led_patterns[patternSelect];
    pUnit->nowTask                      = patternSelect;
    if (pattern->behavior == 0) {
        return tlkdrv_led_off(pUnit);
    } else if (pattern->behavior == 1) {
        if (patternSelect == TLKDRV_LED_PATTERN_ON_LOW_DUTY) {
            return tlkdrv_led_on(pUnit, 5000, 1000);
        } else {
            return tlkdrv_led_on(pUnit, 5000, 5000);
        }

    } else if (pattern->behavior == 2) {
        return tlkdrv_led_auto(isInTimer, pUnit, pattern->flashCounts, pattern->onTimerMs, pattern->offTimerMs, 5000, 5000, 0, 0);
    } else if (pattern->behavior == 3) {
        return tlkdrv_led_auto(isInTimer, pUnit, pattern->flashCounts, pattern->onTimerMs, pattern->offTimerMs, 5000, 5000, pattern->dutyFlushMs, pattern->stepUs);
    }
    return false;
}

/**
 * @brief       This function handles LED breathing task.
 * @param[in]   pUnit        - Pointer to LED unit.
 * @param[in]   pastTimeUs   - Elapsed time in microseconds.
 * @return      Time to wait in milliseconds.
 */
static uint32_t tlkdrv_led_breathTask(tlkdrv_led_unit_t *pUnit, uint32_t pastTimeUs)
{
    if (pUnit->flushTmoMs == 0) { //no breath task
        return TLKOS_WAIT_FOREVER;
    }
    pUnit->flushTmrMs += pastTimeUs / 1000;
    if (pUnit->flushTmrMs < pUnit->flushTmoMs) { //not timeout,just wait
        return pUnit->flushTmoMs - pUnit->flushTmrMs;
    }
    //time out: change pwm duty:
    if (pUnit->isInvt) {
        if (pUnit->curDuty >= pUnit->stepChg) {
            pUnit->curDuty -= pUnit->stepChg;
        } else {
            pUnit->curDuty = 0;
        }
        if (pUnit->curDuty == 0) {
            pUnit->isInvt = false;
        }
    } else {
        if (pUnit->curDuty + pUnit->stepChg <= pUnit->dutyMax) {
            pUnit->curDuty += pUnit->stepChg;
        } else {
            pUnit->curDuty = pUnit->dutyMax;
        }
        if (pUnit->curDuty == pUnit->dutyMax) {
            pUnit->isInvt = true;
        }
    }
    if (pUnit->pwmID <= PWM5_ID) {
        pwm_set_tcmp(pUnit->pwmID, pUnit->curDuty);
        if (pUnit->followChild < TLKDRV_LED_MAX_NUMB) {
            tlkdrv_led_unit_t *pChildUnit = &sTlkDrvLedCtrl.unit[pUnit->followChild];
            pwm_set_tcmp(pChildUnit->pwmID, pUnit->curDuty);
        }
    }
    pUnit->flushTmrMs = 0;
    return pUnit->flushTmoMs;
}

/**
 * @brief       This function handles LED on task.
 * @param[in]   pUnit        - Pointer to LED unit.
 * @param[in]   pastTimeUs   - Elapsed time in microseconds.
 * @return      Time to wait in milliseconds.
 */
static uint32_t tlkdrv_led_onTask(tlkdrv_led_unit_t *pUnit, uint32_t pastTimeUs)
{
    pUnit->runTmrUs += pastTimeUs;
    if (pUnit->runTmrUs / 1000 < pUnit->onTmoMs) { //not timeout,just wait
        return pUnit->onTmoMs - pUnit->runTmrUs / 1000;
    }
    //time out: need turn off led
    pUnit->runTmrUs = 0;
    if (pUnit->offTmoMs != 0) {
        tlkdrv_led_innerOff(pUnit);
    }

    //check is last connt so that change pattern
    if (pUnit->count != 0 && pUnit->count != 0XFFFF) {
        pUnit->count--;
        if (pUnit->count == 0) {                                      //finished
            if (pUnit->waitingTask >= TLKDRV_LED_PATTERN_MAX_STATE) { //have no next task
                tlkdrv_led_patternSelect_inner(true, pUnit, pUnit->lastTask);
            } else { //have next task
                tlkdrv_led_patternSelect_inner(true, pUnit, pUnit->waitingTask);
                pUnit->waitingTask = TLKDRV_LED_PATTERN_MAX_STATE;
            }
        }
    }
    if (pUnit->isCtrl == 0) {
        return TLKOS_WAIT_FOREVER;
    }
    if (pUnit->isOpen) {
        return pUnit->onTmoMs;
    }
    return pUnit->offTmoMs;
}

/**
 * @brief       This function handles LED off task.
 * @param[in]   pUnit        - Pointer to LED unit.
 * @param[in]   pastTimeUs   - Elapsed time in microseconds.
 * @return      Time to wait in milliseconds.
 */
static uint32_t tlkdrv_led_offTask(tlkdrv_led_unit_t *pUnit, uint32_t pastTimeUs)
{
    pUnit->runTmrUs += pastTimeUs;
    if (pUnit->runTmrUs / 1000 < pUnit->offTmoMs) { //not timeout,just wait
        return pUnit->offTmoMs - pUnit->runTmrUs / 1000;
    }
    //time out: need turn on led
    uint32_t nextTaskMs = 0;
    pUnit->runTmrUs     = 0;
    if (pUnit->flushTmoMs != 0) {
        pUnit->curDuty = pUnit->stepChg;
        nextTaskMs     = min(pUnit->onTmoMs, pUnit->flushTmoMs);
    } else {
        pUnit->curDuty = pUnit->dutyMax;
        nextTaskMs     = pUnit->onTmoMs;
    }
    tlkdrv_led_innerOn(pUnit, pUnit->curDuty, pUnit->dutyMax);
    return nextTaskMs;
}

/**
 * @brief       This function handles LED tasks.
 * @param[in]   pUnit    - Pointer to LED unit.
 * @return      Time to wait in milliseconds.
 */
static uint32_t tlkdrv_led_handler(tlkdrv_led_unit_t *pUnit)
{
    uint32_t nextTaskMs = TLKOS_WAIT_FOREVER;
    if (sTlkDrvLedCtrl.syncTimer == TLKDRV_LED_SYNC_STATE_MAGIC_NUM) {
        tlkdrv_led_patternSelect_inner(true, pUnit, pUnit->nowTask);
    }
    if (pUnit->isCtrl == 0) {
        return nextTaskMs;
    }
    uint32_t pastTimeUs = (clock_time() - pUnit->lastTime) / SYSTEM_TIMER_TICK_1US;
    if (pUnit->isOpen) {
        nextTaskMs = min(nextTaskMs, tlkdrv_led_breathTask(pUnit, pastTimeUs));
        nextTaskMs = min(nextTaskMs, tlkdrv_led_onTask(pUnit, pastTimeUs));
        pastTimeUs = 0;
    }
    if (!pUnit->isOpen) {
        nextTaskMs = tlkdrv_led_offTask(pUnit, pastTimeUs);
    }
    pUnit->lastTime = clock_time();
    return nextTaskMs;
}

/**
 * @brief       This function handles LED timer events.
 * @param[in]   TimerHandle    - Timer handle.
 * @param[in]   userArg        - User argument.
 * @return      none.
 */
static void tlkdrv_led_timer(TlkOsTimerHandle_t TimerHandle, void *userArg)
{
    (void)userArg;
    tlkdrv_led_unit_t *pUnit;
    uint32_t           nextTaskMs = TLKOS_WAIT_FOREVER;
    for (uint8_t index = 0; index < TLKDRV_LED_MAX_NUMB; index++) {
        pUnit = &sTlkDrvLedCtrl.unit[index];
        if (pUnit->ioPort == 0 || pUnit->isCtrl == 0 || pUnit->followChild == TLKDRV_LED_FOLLOWED_CHILD) {
            continue;
        }
        uint32_t ms = tlkdrv_led_handler(pUnit);
        nextTaskMs  = min(nextTaskMs, ms);
    }
    if (sTlkDrvLedCtrl.syncTimer == TLKDRV_LED_SYNC_STATE_MAGIC_NUM) {
        sTlkDrvLedCtrl.syncTimer = 0;
    } else if (sTlkDrvLedCtrl.syncTimer != 0) {
        tlkdrv_led_timer_reSync(TimerHandle, sTlkDrvLedCtrl.syncTimer);
        sTlkDrvLedCtrl.syncTimer = TLKDRV_LED_SYNC_STATE_MAGIC_NUM;
        return;
    }
    if (nextTaskMs == TLKOS_WAIT_FOREVER) {
        tlkos_timer_destroy(TimerHandle);
        sTlkDrvLedCtrl.timerHandle = 0;
    } else {
        if (nextTaskMs == 0) {
            nextTaskMs = 1;
        }
        tlkos_timer_setPeriod(TimerHandle, nextTaskMs);
    }
}

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief       This function initializes and inserts an LED device.
 * @param[in]   ioPort    - GPIO port for the LED.
 * @param[in]   pwmID     - PWM ID for LED control.
 * @param[in]   onLevel   - LED on level (0 or 1).
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_insert(uint16_t ioPort, uint8_t pwmID, uint8_t onLevel)
{
    if (ioPort == 0) {
        return -TLK_EPARAM;
    }

    tlkdrv_led_unit_t *pUnit = tlkdrv_led_getUsedUnit(ioPort);
    if (pUnit != NULL) {
        return -TLK_EREPEAT;
    }

    pUnit = tlkdrv_led_getIdleUnit();
    if (pUnit == NULL) {
        return -TLK_EQUOTA;
    }
    pUnit->level       = onLevel ? 1 : 0;
    pUnit->pwmID       = pwmID;
    pUnit->ioPort      = ioPort;
    pUnit->nowTask     = TLKDRV_LED_PATTERN_OFF;
    pUnit->lastTask    = TLKDRV_LED_PATTERN_OFF;
    pUnit->waitingTask = TLKDRV_LED_PATTERN_MAX_STATE;
    pUnit->followChild = TLKDRV_LED_FOLLOWED_NONE;
    if (pUnit->pwmID <= PWM5_ID) {
        tlkhal_pwm_chn_cfg_t pwmCfg = {
            .chn = pUnit->pwmID,
            .pin = pUnit->ioPort,
        };
        tlkhal_pwm_chnInit(&pwmCfg);
        if (pUnit->level == 0) {
            pwm_n_invert_en(pUnit->pwmID);
        }
        pwm_set_clk((uint8_t)(sys_clk.pclk * 1000 * 1000 / TLKDRV_LED_PWM_PCLK_SPEED - 1));
        pwm_set_tcmp(pUnit->pwmID, TLKDRV_LED_PWM_CLOCK_10US);
        pwm_set_tmax(pUnit->pwmID, TLKDRV_LED_PWM_CLOCK_1MS);
        tlkhal_pwm_chnStop(pUnit->pwmID, NULL);
    } else {
        gpio_function_en(pUnit->ioPort);
        gpio_set_input_en(pUnit->ioPort, 0);
        gpio_set_output_en(pUnit->ioPort, 1);
        gpio_set_level(pUnit->ioPort, !pUnit->level);
    }
    return TLK_ENONE;
}

/**
 * @brief       This function sets up a follow relationship between two LEDs.
 * @param[in]   ioPort         - GPIO port for the child LED.
 * @param[in]   fatherIoPort   - GPIO port for the father LED.
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_follow(uint16_t ioPort, uint16_t fatherIoPort)
{
    if (ioPort == 0 || fatherIoPort == 0) {
        return -TLK_EPARAM;
    }
    tlkdrv_led_unit_t *pChildUnit = tlkdrv_led_getUsedUnit(ioPort);
    if (pChildUnit == NULL) {
        return -TLK_EFAIL;
    }
    tlkdrv_led_unit_t *pFatherUnit = tlkdrv_led_getUsedUnit(fatherIoPort);
    if (pFatherUnit == NULL) {
        return -TLK_EFAIL;
    }
    if (pChildUnit->followChild < TLKDRV_LED_FOLLOWED_CHILD || pFatherUnit->followChild < TLKDRV_LED_FOLLOWED_CHILD) {
        return -TLK_EFAIL;
    }
    if (pChildUnit->pwmID <= PWM5_ID && pFatherUnit->pwmID > PWM5_ID) {
        return -TLK_EFAIL;
    }
    if (pChildUnit->pwmID > PWM5_ID && pFatherUnit->pwmID <= PWM5_ID) {
        return -TLK_EFAIL;
    }
    pFatherUnit->followChild = ((uint32_t)pChildUnit - (uint32_t)&sTlkDrvLedCtrl.unit[0]) / sizeof(tlkdrv_led_unit_t);
    pChildUnit->followChild  = TLKDRV_LED_FOLLOWED_CHILD;
    return TLK_ENONE;
}

/**
 * @brief       This function removes an LED device.
 * @param[in]   ioPort    - GPIO port for the LED.
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_remove(uint16_t ioPort)
{
    if (ioPort == 0) {
        return -TLK_EPARAM;
    }
    tlkdrv_led_unit_t *pUnit = tlkdrv_led_getUsedUnit(ioPort);
    if (pUnit == NULL) {
        return -TLK_EFAIL;
    }
    tlkdrv_led_enter_critical();
    if (pUnit->pwmID <= PWM5_ID) {
        tlkhal_pwm_chnStop(pUnit->pwmID, NULL);
    } else {
        gpio_set_level(pUnit->ioPort, !pUnit->level);
        gpio_function_dis(pUnit->ioPort);
    }
    memset(pUnit, 0, sizeof(*pUnit));
    if (sTlkDrvLedCtrl.timerHandle) {
        tlkos_timer_stop(sTlkDrvLedCtrl.timerHandle);
        tlkos_timer_setPeriod(sTlkDrvLedCtrl.timerHandle, 1);
        tlkos_timer_start(sTlkDrvLedCtrl.timerHandle);
    }

    tlkdrv_led_leave_critical();
    return TLK_ENONE;
}

/**
 * @brief       This function removes all LED devices.
 * @return      none.
 */
void tlkdrv_led_removeAll(void)
{
    tlkdrv_led_enter_critical();
    if (sTlkDrvLedCtrl.timerHandle) {
        tlkos_timer_destroy(sTlkDrvLedCtrl.timerHandle);
    }
    for (size_t index = 0; index < TLKDRV_LED_MAX_NUMB; index++) {
        tlkdrv_led_unit_t *pUnit = &sTlkDrvLedCtrl.unit[index];
        if (pUnit->ioPort == 0) {
            continue;
        }
        if (pUnit->pwmID <= PWM5_ID) {
            tlkhal_pwm_chnStop(pUnit->pwmID, NULL);
        } else {
            gpio_set_level(pUnit->ioPort, !pUnit->level);
            gpio_function_dis(pUnit->ioPort);
        }
    }
    memset(&sTlkDrvLedCtrl, 0, sizeof(sTlkDrvLedCtrl));
    tlkdrv_led_leave_critical();
}

/**
 * @brief       This function selects and applies an LED pattern.
 * @param[in]   ioPort         - GPIO port for the LED.
 * @param[in]   patternSelect  - Pattern to apply.
 * @return      true: success, false: failure.
 */
bool tlkdrv_led_patternSelect(uint16_t ioPort, tlkdrv_led_pattern_e patternSelect)
{
    if (patternSelect >= TLKDRV_LED_PATTERN_MAX_STATE) {
        return false;
    }
    tlkdrv_led_unit_t *pUnit = tlkdrv_led_getUsedUnit(ioPort);
    if (pUnit == NULL) {
        return false;
    }
    if (pUnit->followChild == TLKDRV_LED_FOLLOWED_CHILD) {
        return false;
    }
    if (pUnit->pwmID > PWM5_ID && patternSelect >= TLKDRV_LED_PATTERN_BREATH_SLOW) {
        return false;
    }
    bool                        res        = false;
    const tlkdrv_led_pattern_t *nowPattern = &tlkdrv_led_patterns[pUnit->nowTask];
    tlkdrv_led_enter_critical();
    if (nowPattern->flashCounts == 0XFFFFU) { // if nowPattern's behavior is forever action
        pUnit->lastTask = pUnit->nowTask;     //Directly seize,and record lastTask
        res             = tlkdrv_led_patternSelect_inner(false, pUnit, patternSelect);
    } else { // not forever:wait it end
        pUnit->waitingTask = patternSelect;
        res                = true;
    }
    tlkdrv_led_leave_critical();
    return res;
}

/**
 * @brief       This function re-synchronizes all LEDs.
 * @param[in]   syncTick    - Synchronization tick value.
 * @return      none.
 */
void tlkdrv_led_reSyncAll(uint32_t syncTick)
{
    tlkdrv_led_enter_critical();
    if (sTlkDrvLedCtrl.timerHandle) {
        sTlkDrvLedCtrl.syncTimer = syncTick | 1;
        tlkos_timer_stop(sTlkDrvLedCtrl.timerHandle);
        tlkos_timer_setPeriod(sTlkDrvLedCtrl.timerHandle, 1);
        tlkos_timer_start(sTlkDrvLedCtrl.timerHandle);
    }
    tlkdrv_led_leave_critical();
}


#endif
