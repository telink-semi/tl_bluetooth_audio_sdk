/********************************************************************************************************
 * @file    tlkdrv_led.h
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
#pragma once

#define TLKDRV_LED_MAX_NUMB GPIO_LED_NUMS 

/**
 * @brief	LED patterns enumeration
 */
typedef enum
{
    TLKDRV_LED_PATTERN_OFF,                     //!< LED off pattern
    TLKDRV_LED_PATTERN_ON,                      //!< LED on pattern
    TLKDRV_LED_PATTERN_ON_LOW_DUTY,             //!< LED on with low duty pattern
    TLKDRV_LED_PATTERN_FLASH_SLOW,              //!< LED slow flash pattern
    TLKDRV_LED_PATTERN_FLASH_FAST,              //!< LED fast flash pattern
    TLKDRV_LED_PATTERN_FLASH_PAIR,              //!< LED pairing flash pattern
    TLKDRV_LED_PATTERN_IDLE,                    //!< LED idle flash pattern
    TLKDRV_LED_PATTERN_FAST_FLASH_1_TIME,       //!< LED fast flash 1 time pattern
    TLKDRV_LED_PATTERN_FAST_FLASH_2_TIMES,      //!< LED fast flash 2 times pattern
    TLKDRV_LED_PATTERN_SLOW_FLASH_2_TIMES,      //!< LED slow flash 2 times pattern
    TLKDRV_LED_PATTERN_FAST_FLASH_3_TIMES,      //!< LED fast flash 3 times pattern
    TLKDRV_LED_PATTERN_ON_500MS,                //!< LED on for 500ms pattern
    TLKDRV_LED_PATTERN_ON_1S,                   //!< LED on for 1 second pattern
    TLKDRV_LED_PATTERN_SLOW_FLASH_3_TIMES,      //!< LED slow flash 3 times pattern

    TLKDRV_LED_PATTERN_BREATH_SLOW,             //!< LED slow breath pattern
    TLKDRV_LED_PATTERN_BREATH_FAST,             //!< LED fast breath pattern
    TLKDRV_LED_PATTERN_BREATH_PAIR,             //!< LED pairing breath pattern
    TLKDRV_LED_PATTERN_SLOW_BREATH_3_TIMES,     //!< LED slow breath 3 times pattern
    TLKDRV_LED_PATTERN_FAST_BREATH_3_TIMES,     //!< LED fast breath 3 times pattern

    TLKDRV_LED_PATTERN_MAX_STATE,               //!< Max LED pattern number
} tlkdrv_led_pattern_e;

/**
 * @brief       This function initializes and inserts an LED device.
 * @param[in]   ioPort    - GPIO port for the LED.
 * @param[in]   pwmID     - PWM ID for LED control.
 * @param[in]   onLevel   - LED on level (0 or 1).
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_insert(uint16_t ioPort, uint8_t pwmID, uint8_t onLevel);

/**
 * @brief       This function removes an LED device.
 * @param[in]   ioPort    - GPIO port for the LED.
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_remove(uint16_t ioPort);

/**
 * @brief       This function removes all LED devices.
 * @return      none.
 */
void tlkdrv_led_removeAll(void);

/**
 * @brief       This function sets up a follow relationship between two LEDs.
 * @param[in]   ioPort         - GPIO port for the child LED.
 * @param[in]   fatherIoPort   - GPIO port for the father LED.
 * @return      0: success, <0: failure.
 */
int tlkdrv_led_follow(uint16_t ioPort, uint16_t fatherIoPort);

/**
 * @brief       This function selects and applies an LED pattern.
 * @param[in]   ioPort         - GPIO port for the LED.
 * @param[in]   patternSelect  - Pattern to apply.
 * @return      true: success, false: failure.
 */
bool tlkdrv_led_patternSelect(uint16_t ioPort, tlkdrv_led_pattern_e patternSelect);

/**
 * @brief       This function re-synchronizes all LEDs.
 * @param[in]   syncTick    - Synchronization tick value.
 * @return      none.
 */
void tlkdrv_led_reSyncAll(uint32_t syncTick);
